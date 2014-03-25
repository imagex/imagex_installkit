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
 * Defines the watchdog type.
 */
define('IMAGEX_INSTALLKIT_WATCHDOG_TYPE', 'imagex_installkit');

/**
 * Denotes the core default administrator role.
 */
define('IMAGEX_INSTALLKIT_ADMINISTRATOR_ROLE', 'administrator');

/**
 * Denotes the core admin (uid = 1) user identifier.
 */
define('IMAGEX_INSTALLKIT_ADMINISTRATOR_UID', 1);

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
 * Pass alterable variables to PROFILE_TYPE_alter().
 *
 * @param string $type
 *   The type to alter.
 * @param mixed $data
 *   The data to alter, passed by reference.
 * @param mixed $context1
 *   An additional variable that is passed by reference, optional.
 * @param mixed $context2
 *   An additional variable that is passed by reference, optional.
 */
function imagex_installkit_profile_alter($type, &$data, &$context1 = NULL, &$context2 = NULL) {
  foreach (imagex_installkit_get_install_profiles() as $profile) {
    $function = $profile . '_' . $type . '_alter';
    if (function_exists($function)) {
      $function($data, $context1, $context2);
    }
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

/**
 * Implements hook_imagex_installkit_install_profile_modules_alter().
 */
function imagex_installkit_imagex_installkit_install_profile_modules_alter(&$required, &$non_required) {
  // TODO: Improve the re-ordering of installkit's module install order of ops.
  // The UUID module does not require the dependencies for
  // the File module and therefore due to regular Drupal sorting and ordering
  // during the module list rebuild, UUID will be installed prior to File and as a
  // result the File entity will not have the UUID column added. This is problematic.
  if ($non_required['uuid'] > $non_required['file']) {
    // Swap the weights of the UUID and File should UUID's weight be
    // greater then the File contrib module.
    $weight = $non_required['uuid'];
    $non_required['uuid'] = $non_required['file'];
    $non_required['file'] = $weight;
    unset($weight);
  }
}
