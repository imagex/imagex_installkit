<?php
/**
 * @file
 */

/**
 * Bootstraps the installation tasks by altering core install tasks.
 * 
 * @param array tasks
 *   An array of installation tasks to be invoked. Allows altering.
 * @param array $install_state
 *   An array of the current installation state.
 */
function imagex_install_bootstrap(&$tasks, &$install_state) {
  $install_state['imagex'] = array();

  $tasks['install_select_locale']['display'] = FALSE;
  $tasks['install_select_locale']['run'] = INSTALL_TASK_SKIP;
  $install_state['parameters']['locale'] = 'en';

  $tasks['install_profile_modules']['function'] = 'imagex_install_profile';
  $tasks['install_finished']['function'] = 'imagex_install_finished';
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
function imagex_install_profile(&$install_state) {
  $operations = array();

  // Retrieve the `core` defined modules installation batch and alter them.
  $initial_batch = install_profile_modules($install_state);
  foreach ($initial_batch['operations'] as $module_batch) {
    if (!isset($module_batch[1])) {
      continue;
    }
    $module_batch_info = $module_batch[1];
    $operations[] = array('_imagex_install_profile_execute_task', array(
      'ImagexInstallModuleTask',
      array(
        'module' => $module_batch_info[0],
        'module name' => $module_batch_info[1],
      ),
    ));
  }

  // Attach the default configuration after the module installations.
  $operations[] = array('_imagex_install_profile_execute_task', array(
    'ImagexInstallDefaultsTask',
    array(),
  ));

  // Allow for inherited profiles to alter the installation tasks.
  drupal_alter('imagex_install_tasks_alter', $operations);

  // Setup the final batch information and return.
  $batch = array(
    'operations' => $operations,
    'title' => st('Install @drupal', array(
      '@drupal' => drupal_install_profile_distribution_name(),
    )),
    'error_message' => st('The installation has encountered an error.'),
    'finished' => 'imagex_install_profile_finished',
  );

  return $batch;
}

/**
 * Invoked at the end of the profile installation (batch operations) step.
 */
function imagex_install_profile_finished() {
  // Flush and clear all caches.
  drupal_flush_all_caches();
}

/**
 * Overrides the install_finished().
 *
 * Much of this function's implementation is the duplicated 
 *
 * @param array $install_state
 *   An array of the current installation state.
 */
function imagex_install_finished(&$install_state) {
  // Flush all caches to ensure that any full bootstraps during the installer
  // do not leave stale cached data, and that any content types or other items
  // registered by the installation profile are registered correctly.
  drupal_flush_all_caches();

  // Remember the profiles that were used.
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

  // Enable the administration theme, seven.
  db_update('system')
    ->fields(array('status' => 1))
    ->condition('type', 'theme')
    ->condition('name', 'seven')
    ->execute();
  variable_set('admin_theme', 'seven');
  variable_set('node_admin_theme', '1');

  // Cache a fully-built schema.
  drupal_get_schema(NULL, TRUE);

  // Flush all caches to ensure that any full bootstraps during the installer
  // do not leave stale cached data, and that any content types or other items
  // registered by the installation profile are registered correctly.
  drupal_cron_run();

  // If not in CLI, perform a redirect to front page.
  if (!drupal_is_cli()) {
    drupal_goto();
  }
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
function _imagex_install_profile_execute_task($class_name, array $arguments, &$context) {
  imagex_load_include('inc', 'includes/ImagexInstallTask');
  imagex_load_include('inc', 'includes/install/' . $class_name);

  try {
    $reflection = new ReflectionClass($class_name);
    if (!$reflection->isSubclassOf('ImagexInstallTask')) {
      throw new ImagexException(st('The specified install task cannot be instantiated, it is not an instance of ImagexInstallTask.'));
    }

    if (!$reflection->isInstantiable()) {
      throw new ImagexException(st('The install task class is not instantiatable.'));
    }

    // Create a new instance of the task class using the reflector.
    $instance = $reflection->newInstance();

    // Setup the installation task.
    $instance->setContext($context);
    $instance->setArguments($arguments);

    // And ... execute, let the magic happen.
    $instance->execute();
  }
  catch (Exception $exception) {
    // TODO: Improve handling of caught exceptions.
    throw $exception;
  }
}
