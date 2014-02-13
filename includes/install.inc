<?php
/**
 * This file is part of ImageX InstallKit.
 *
 * (c) ImageX Media Inc. (www.imagexmedia.com)
 *
 * This source file is subject to the GPL version 2 license that is
 * bundled with this source code in the file LICENSE.md.
 *
 * Drupal is a registered trademark of Dries Buytaert (www.buytaert.com).
 */

/**
 * Denotes a module install operation type.
 */
define('IMAGEX_INSTALLKIT_INSTALLOP_MODULE', 'module');

/**
 * Denotes the default task install operation type.
 */
define('IMAGEX_INSTALLKIT_INSTALLOP_DEFAULT_TASKS', 'default tasks');

/**
 * Bootstraps the installation tasks by altering core install tasks.
 * 
 * @param array tasks
 *   An array of installation tasks to be invoked. Allows altering.
 * @param array $install_state
 *   An array of the current installation state.
 */
function imagex_installkit_install_bootstrap(&$tasks, &$install_state) {
  $install_state['imagex'] = array();

  $tasks['install_select_locale']['display'] = FALSE;
  $tasks['install_select_locale']['run'] = INSTALL_TASK_SKIP;
  $install_state['parameters']['locale'] = 'en';

  $tasks['install_profile_modules']['function'] = 'imagex_installkit_install_profile';
  $tasks['install_finished']['function'] = 'imagex_installkit_install_finished';
}

/**
 * Handles the core installation of a single profile and its dependencies.
 * 
 * @param array $install_state
 *   An array of the current installation state.
 * 
 * @return array
 *   Returns a batch information to perform the installation.
 */
// TODO: Need to add the retrieval of all defined install tasks.
function imagex_installkit_install_profile(&$install_state) {
  $operations = array();

  // Retrieve all modules required to be installed during the installation.
  _imagex_installkit_install_profile_modules($install_state, $operations);

  // Attach the default configuration after the module installations.
  $operations['default_tasks'] = array('_imagex_installkit_install_profile_execute_task', array(
    'ImagexInstallKitInstallDefaultsTask',
    array(),
  ));

  // Allow for inherited profiles to alter the installation tasks.
  imagex_installkit_profile_alter('imagex_installkit_install_tasks', $operations);

  // Setup the final batch information and return.
  $batch = array(
    'operations' => $operations,
    'title' => st('Install @drupal', array(
      '@drupal' => drupal_install_profile_distribution_name(),
    )),
    'error_message' => st('The installation has encountered an error.'),
    'finished' => 'imagex_installkit_install_profile_finished',
  );
  return $batch;
}

/**
 * Invoked at the end of the profile installation (batch operations) step.
 */
function imagex_installkit_install_profile_finished() {
  // Flush all caches to ensure that any full bootstraps during the installer
  // do not leave stale cached data, and that any content types or other items
  // registered by the installation profile are registered correctly.
  drupal_flush_all_caches();

  // Remember the profiles that were used, in order that Drupal likes them.
  $profiles = drupal_get_profiles();
  variable_set('install_profile', $profiles[0]);
  variable_set('install_profiles', $profiles);

  // Installation profiles are always loaded last.
  $weight = 1000 + count($profiles);
  foreach ($profiles as $profile) {
    db_update('system')
      ->fields(array('weight' => $weight))
      ->condition('type', 'module')
      ->condition('name', $profile)
      ->execute();
    $weight = $weight - 1;
  }

  // Cache a fully-built schema.
  drupal_get_schema(NULL, TRUE);

  // Flush all caches to ensure that any full bootstraps during the installer
  // do not leave stale cached data, and that any content types or other items
  // registered by the installation profile are registered correctly.
  drupal_cron_run();
}

/**
 * Overrides the install_finished().
 *
 * Much of this function's implementation is the duplicated.
 *
 * @param array $install_state
 *   An array of the current installation state.
 *
 * @return string
 *   Returns an empty string if CLI, otherwise redirects to <front>.
 */
function imagex_installkit_install_finished(&$install_state) {
  // If not in CLI, perform a redirect to front page.
  if (!drupal_is_cli()) {
    drupal_goto();
  }

  // Clear all messages stored in the Drupal messaging system.
  drupal_get_messages(NULL, TRUE);

  return '';
}

/**
 * Handles the execution of a single installation task.
 * 
 * @param string $class_name
 *   An installation task class name to instantiate and invoke. Must be of type
 *   ImagexInstallTask.
 * @param array  $arguments
 *   An array of arguments to set for the install task.
 * @param array $context
 *   An array of contextual information for this install task.
 * 
 * @throws Exception
 *   Throws an Exception for various reasons.
 */
function _imagex_installkit_install_profile_execute_task($class_name, array $arguments, &$context) {
  imagex_installkit_load_include('inc', 'includes/ImagexInstallKitInstallTask');
  imagex_installkit_load_include('inc', 'includes/' . $class_name);

  try {
    $reflection = new ReflectionClass($class_name);
    if (!$reflection->isSubclassOf('ImagexInstallKitInstallTask')) {
      throw new Exception(st('The specified install task cannot be instantiated, it is not an instance of ImagexInstallKitInstallTask.'));
    }

    if (!$reflection->isInstantiable()) {
      throw new Exception(st('The install task class is not instantiatable.'));
    }

    // Create a new instance of the task class using reflection,
    // initialize and execute the install task.
    $instance = $reflection->newInstance();
    $instance->setContext($context);
    $instance->setArguments($arguments);
    $instance->execute();
  }
  catch (Exception $exception) {
    imagex_installkit_log_exception($exception);
    throw $exception;
  }
}

/**
 * Initializes the module install operations, retrieving modules and creating batch.
 *
 * @param array $install_state
 *   The current install state.
 * @param array $operations
 *   An array of batch operations to populate with module install, passed by reference.
 */
function _imagex_installkit_install_profile_modules(&$install_state, &$operations) {
  $modules = variable_get('install_profile_modules', array());
  $files = system_rebuild_module_data();
  variable_del('install_profile_modules');

  // Enforce that installation profiles are required and required by.
  $profile_count = count($install_state['profiles']);
  for ($i = 0, $size = $profile_count - 1; $i < $size; $i++) {
    $files[$install_state['profiles'][$i]]->info['required'] = FALSE;
    $files[$install_state['profiles'][$i + 1]]->required_by[] = $install_state['profiles'][$i];
    $files[$install_state['profiles'][$i]]->requires[$install_state['profiles'][$i + 1]] = array(
      'name' => $install_state['profiles'][$i + 1],
    );
  }
  // The absolute root profile (e.g. imagex_installkit) does not get its required
  // flag set to a boolean false in the for loop above; set it here.
  $files[$install_state['profiles'][$profile_count - 1]]->info['required'] = FALSE;

  // Add modules that other modules depend on.
  foreach ($modules as $module) {
    if ($files[$module]->requires) {
      $modules = array_merge($modules, array_keys($files[$module]->requires));
    }
  }
  unset($module);

  // Always install required modules first. Respect the dependencies between the modules.
  $required = array();
  $non_required = array();
  $modules = array_unique($modules);
  foreach ($modules as $module) {
    if (!empty($files[$module]->info['required'])) {
      $required[$module] = $files[$module]->sort;
    }
    else {
      $non_required[$module] = $files[$module]->sort;
    }
  }
  unset($module);

  // Allow other installation profiles to alter the profile modules.
  imagex_installkit_profile_alter('imagex_installkit_install_profile_modules', $required, $non_required);

  // Sort modules while maintaining key index.
  arsort($required);
  arsort($non_required);

  // Create the installation batch operations for installing each module,
  $modules = $required + $non_required;
  foreach ($modules as $module => $weight) {
    $key = IMAGEX_INSTALLKIT_INSTALLOP_MODULE . ':' . $module;
    $operations[$key] = array('_imagex_installkit_install_profile_execute_task', array(
      'ImagexInstallKitInstallModuleTask',
      array('module' => $module, 'module name' => $files[$module]->info['name']),
    ));
  }
  unset($weight, $modules);
}
