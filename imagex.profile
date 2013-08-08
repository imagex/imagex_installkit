<?php
/**
 * @file
 */

define('IMAGEX_MODE_DEVELOPMENT', 1);
define('IMAGEX_MODE_STAGING', 2);
define('IMAGEX_MODE_PRODUCTION', 3);

/**
 * Returns the current operating mode of ImageX install profile.
 *
 * @return int
 *   Returns the current operating mode.
 */
function imagex_mode() {
  return variable_get('ixm_mode', ixm_default_mode());
}

/**
 * Returns the default operating mode.
 *
 * @return int
 *   Returns the IMAGEX_MODE_PRODUCTION value.
 */
function imagex_default_mode() {
  return IMAGEX_MODE_PRODUCTION;
}

/**
 * Loads an ImageX include specifically.
 *
 * @param $type
 * @param $name
 * @return bool|string
 */
function imagex_load_include($type, $name) {
  if (function_exists('drupal_get_path')) {
    $file = DRUPAL_ROOT . '/' . drupal_get_path('profile', 'imagex') . '/' . $name . '.' . $type;
    if (is_file($file)) {
      include_once $file;
      return $file;
    }
  }

  return FALSE;
}

/**
 * Implements hook_install_tasks_alter().
 */
function imagex_install_tasks_alter(&$tasks, $install_state) {
  global $install_state;
  if (empty($install_state['active_task']) && !$install_state['installation_finished']) {
    imagex_load_include('inc', 'includes/install');
    imagex_install_bootstrap($tasks, $install_state);
  }
}

/**
 * Implements hook_form_FORM_ID_alter() for install_configure_form().
 *
 * Allows the profile to alter the site configuration form.
 */
function imagex_form_install_configure_form_alter(&$form, $form_state) {
  $form['site_information']['site_name']['#default_value'] = $_SERVER['SERVER_NAME'];
}


/**
 * Returns the an array of default variables for the ImageX profile.
 *
 * @return array
 *   Returns variable and default value map array.
 */
function imagex_default_variables() {
  return array(
    'ixm_mode' => imagex_default_mode(),
  );
}
