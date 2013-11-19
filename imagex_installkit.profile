<?php
/**
 * @file
 */

/**
 * Defines the watchdog type.
 */
define('IMAGEX_INSTALLKIT_WATCHDOG_TYPE', 'imagex_installkit');

/**
 * Implements hook_flush_caches().
 */
function imagex_installkit_flush_caches() {
  if (imagex_installkit_block_rebuild_on_flush_caches()) {
    imagex_installkit_load_include('inc', 'includes/block');
    imagex_installkit_block_rebuild();
  }
  return array();
}

/**
 * Loads an ImageX include specifically.
 *
 * This function makes use of the `require_once` language construct. Therefore,
 * ensure this is a noted behavior when implementing usages of this function.
 *
 * @param string $type
 *   The type of file to include. Example: inc.
 * @param string $name
 *   The name of the file to locate and include.
 *
 * @return mixed
 *   Returns the absolute file path as a string if found and included, otherwise
 *   returns FALSE if the file was not found.
 */
function imagex_installkit_load_include($type, $name) {
  if (function_exists('drupal_get_path')) {
    $file = DRUPAL_ROOT . '/' . drupal_get_path('profile', 'imagex_installkit') . '/' . $name . '.' . $type;
    if (is_file($file)) {
      require_once $file;
      return $file;
    }
  }

  return FALSE;
}

/**
 * Implements hook_install_tasks_alter().
 */
function imagex_installkit_install_tasks_alter(&$tasks, $install_state) {
  global $install_state;
  imagex_installkit_load_include('inc', 'includes/install');
  imagex_installkit_install_bootstrap($tasks, $install_state);
}

/**
 * Implements hook_form_FORM_ID_alter() for install_configure_form().
 *
 * Allows the profile to alter the site configuration form.
 */
function imagex_installkit_form_install_configure_form_alter(&$form, $form_state) {
  $form['site_information']['site_name']['#default_value'] = $_SERVER['SERVER_NAME'];
}

/**
 * Implements hook_imagex_installkit_default_theme().
 */
function imagex_installkit_imagex_installkit_default_theme() {
  return 'parrot';
}

/**
 * Returns an array of installation profiles, in reverse order.
 *
 * The returned array of installation profiles allow for proper execution
 * hierarchy, for example this base installation profile will be the last in
 * the returned array vs. the "concrete" instances will be first.
 *
 * @return array $profiles
 *   Returns an array of installation profile names.
 */
function imagex_installkit_get_install_profiles() {
  static $profiles = NULL;
  if (NULL === $profiles) {
    $profiles = drupal_get_profiles();
    $profiles = array_reverse($profiles);
  }

  return $profiles;
}

/**
 * Returns a boolean indicating whether or not blocks should be rebuilt.
 *
 * @return boolean
 *   Returns TRUE if blocks should be rebuilt on cache flush, otherwise FALSE.
 */
function imagex_installkit_block_rebuild_on_flush_caches() {
  return variable_get('imagex_installkit_block_rebuild_on_cache_flushes', TRUE);
}

/**
 * Log handler for logging watchdog like messages.
 * 
 * @param int $type
 *   The watchdog severity level.
 * @param string $message
 *   The log message.
 * @param array $variables
 *   An array of variables.
 */
function imagex_installkit_log($type, $message, array $variables = array()) {
  watchdog(IMAGEX_INSTALLKIT_WATCHDOG_TYPE, $message, $variables, $type);
}

/**
 * Logs an exception.
 * 
 * @param Exception $exception
 *   The Exception object to log.
 */
function imagex_installkit_log_exception(Exception $exception) {
  imagex_installkit_log(WATCHDOG_ERROR, $exception->getMessage(), array(
    'exception' => $exception,
  ));
}

/**
 * Implements hook_watchdog().
 */
function imagex_installkit_watchdog(array $log_entry) {
  // Attempt to assume we are using `drush`, therefore add this watchdog
  // log to the drush log output.
  if (drupal_is_cli() && function_exists('drush_log')) {
    if (!is_array($log_entry['variables'])) {
      $log_entry['variables'] = NULL;
    }

    $message = isset($log_entry['variables']) && !empty($log_entry['variables']) ? dt($log_entry['message'], $log_entry['variables']) : $log_entry['message'];
    drush_log($message, _imagex_installkit_watchdog_severity_string($log_entry['severity']));
  }
}

/**
 * Returns a string representation for Drush log for watchdog severity.
 * 
 *  @param $severity
 *   A Drupal core's WATCHDOG severity level.
 *
 * @return string
 *   Returns a string representation for drush log.
 */
function _imagex_installkit_watchdog_severity_string($severity) {
  switch ($severity) {
    case WATCHDOG_EMERGENCY:
    case WATCHDOG_ALERT:
    case WATCHDOG_CRITICAL:
    case WATCHDOG_ERROR:
      return 'error';

    case WATCHDOG_WARNING:
      return 'warning';

    case WATCHDOG_NOTICE:
    case WATCHDOG_INFO:
    case WATCHDOG_DEBUG:
    default:
      return 'notice';
  }
}
