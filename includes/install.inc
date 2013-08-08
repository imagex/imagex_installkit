<?php
/**
 * @file
 */

/**
 * Returns an array of child profiles for ImageX.
 *
 * @return array
 *   Returns an of profile names for all child profiles of ImageX.
 */
/* function imagex_install_get_profiles() {
  $profiles = &drupal_static(__FUNCTION__, array());
  if (empty($profiles)) {
    $profiles = array();
    $available_install_profiles = file_scan_directory('./profiles', '/\.info$/', array('key' => 'name'));
    foreach ($available_install_profiles as $profile) {
      $profile_info = drupal_parse_info_file($profile->uri);
      if (isset($profile_info['base']) && 'imagex' == $profile_info['base']) {
        $profiles[] = $profile->name;
      }
    }
  }

  return $profiles;
} */

/**
 * @param array tasks
 * @param array $install_state
 * @throws ImagexException
 */
function imagex_install_bootstrap(&$tasks, &$install_state) {
  $install_state['imagex'] = array(
    'profile' => NULL,
  );

  $tasks['install_select_locale']['display'] = FALSE;
  $tasks['install_select_locale']['run'] = INSTALL_TASK_SKIP;
  $install_state['parameters']['locale'] = 'en';

  $tasks['install_finished']['function'] = 'imagex_install_finished';
}

/**
 * Overrides the install_finished().
 *
 * @param array $install_state
 */
function imagex_install_finished(&$install_state) {
  // Flush all caches.
  drupal_flush_all_caches();

  // Cache a fully built schema.
  drupal_get_schema(NULL, TRUE);

  // Run cron to perform initial jobs.
  drupal_cron_run();
}
