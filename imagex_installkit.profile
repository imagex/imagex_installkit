<?php
/**
 * @file
 */

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
  return 'bartik';
}
