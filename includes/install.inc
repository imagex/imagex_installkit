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
  imagex_installkit_load_include('inc', 'includes/ImagexInstallKitException');
  imagex_installkit_load_include('inc', 'includes/ImagexInstallKitInstallTaskInterface');
  imagex_installkit_load_include('inc', 'includes/ImagexInstallKitInstallTask');

  $install_state['imagex'] = array();
  $original_tasks = $tasks;
  // Re-create the installation tasks originally created by Drupal core.
  $tasks = array(
    'install_select_profile' => $original_tasks['install_select_profile'],
    'install_load_profile' => $original_tasks['install_load_profile'],
    'install_select_locale' => $original_tasks['install_select_locale'],
    'install_verify_requirements' => $original_tasks['install_verify_requirements'],
    'install_settings_form' => $original_tasks['install_settings_form'],
    'install_system_module' => $original_tasks['install_system_module'],
    'install_bootstrap_full' => $original_tasks['install_bootstrap_full'],
    'install_profile_modules' => $original_tasks['install_profile_modules'],
    'install_profile_tasks' => array(
      'type' => 'batch',
      'display_name' => st('Install tasks'),
      'function' => 'imagex_installkit_install_profile_tasks',
    ),
    'install_import_locales' => $original_tasks['install_import_locales'],
    'install_configure_form' => $original_tasks['install_configure_form'],
    'install_import_locales_remaining' => $original_tasks['install_import_locales_remaining'],
    'install_finished' => $original_tasks['install_finished'],
  );
  unset($original_tasks);
  // Do not display the locale selection and set the default to english.
  $tasks['install_select_locale']['display'] = FALSE;
  $tasks['install_select_locale']['run'] = INSTALL_TASK_SKIP;
  $install_state['parameters']['locale'] = 'en';
  // Alter the callback function for module installation and finished tasks.
  $tasks['install_profile_modules']['function'] = 'imagex_installkit_install_profile_modules';
  $tasks['install_finished']['function'] = 'imagex_installkit_install_finished';
}

/**
 * Handles the core installation of a single profile and its dependencies.
 * 
 * @param array $install_state
 *   An array of the current installation state.
 * 
 * @return array
 *   Returns an array of batch information for performing the task execution.
 */
function imagex_installkit_install_profile_modules(&$install_state) {
  $operations = array();

  // Retrieve all modules required to be installed during the installation.
  _imagex_installkit_install_profile_modules($install_state, $operations);

  // Allow for inherited profiles to alter the installation of dependencies.
  imagex_installkit_profile_alter('imagex_installkit_install_profile_modules', $operations);

  // Setup the final batch information and return.
  $batch = array(
    'operations' => $operations,
    'title' => st('Install @drupal modules', array(
      '@drupal' => drupal_install_profile_distribution_name(),
    )),
    'error_message' => st('The installation has encountered an error.'),
    'finished' => 'imagex_installkit_install_profile_modules_finished',
  );
  return $batch;
}

/**
 * Invoked at the end of the profile installation (batch operations) step.
 */
function imagex_installkit_install_profile_modules_finished() {
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
 * Handles the core installation of tasks defined by an installation profile.
 *
 * @param $install_state
 *   An array of the current installation state.
 *
 * @return array
 *   Returns an array of batch information for performing the task execution.
 */
// TODO: Need to add the retrieval of all defined install tasks.
function imagex_installkit_install_profile_tasks(&$install_state) {
  $operations = array();

  // Attach the default configuration after the module installations.
  $operations['default_tasks'] = array('imagex_installkit_install_task_execution_handler', array(
    'ImagexInstallKitInstallDefaultsTask',
    array(),
  ));

  // Allow inheritable profiles to alter the installation tasks.
  imagex_installkit_profile_alter('imagex_installkit_install_profile_tasks', $operations);

  // Setup the final batch information and return.
  $batch = array(
    'operations' => $operations,
    'title' => st('Install @drupal tasks', array(
      '@drupal' => drupal_install_profile_distribution_name(),
    )),
    'error_message' => st('The installation has encountered an error.'),
    'finished' => 'imagex_installkit_install_profile_tasks_finished',
  );
  return $batch;
}

/**
 * Invoked at the end of the task installation step.
 */
function imagex_installkit_install_profile_tasks_finished() {
  // Empty, this is a placeholder for future implementation.
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
 * @param string $classname
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
function imagex_installkit_install_task_execution_handler($classname, array $arguments, &$context) {
  try {
    // Attempt to include the class file, imagex_installkit_load_include will return FALSE
    // if the file was unable to be included for whatever reason.
    if (!imagex_installkit_load_include('inc', 'includes/' . $classname)) {
      throw new ImagexInstallKitException(st('Unable to include ImagexInstallKitInstallTask class @classname.', array(
        '@classname' => $classname,
      )));
    }

    // Create a new reflection class instance.
    $reflection = new ReflectionClass($classname);
    // Enforce that the task in question implements the install task interface.
    if (!$reflection->implementsInterface('ImagexInstallKitInstallTaskInterface')) {
      throw new ImagexInstallKitException(st('Install task @classname does not implement interface ImagexInstallKitInstallTaskInterface.', array(
        '@classname' => $classname,
      )));
    }
    // Ensure the task is instantiatable prior to continuing, if not throw an exception.
    if (!$reflection->isInstantiable()) {
      throw new ImagexInstallKitException(st('Install task @classname is not instantiatable.', array('@classname' => $classname)));
    }

    $instance = $reflection->newInstance();
    $reflection->getMethod('setContext')
      ->invoke($instance, $context);
    $reflection->getMethod('setArguments')
      ->invoke($instance, $arguments);
    $reflection->getMethod('execute')
      ->invoke($instance);
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
    $operations[$key] = array('imagex_installkit_install_task_execution_handler', array(
      'ImagexInstallKitInstallModuleTask',
      array('module' => $module, 'module name' => $files[$module]->info['name']),
    ));
  }
  unset($weight, $modules);
}
