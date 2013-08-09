<?php
/**
 * @file
 */

/**
 * @param array tasks
 * @param array $install_state
 * @throws ImagexException
 */
function imagex_install_bootstrap(&$tasks, &$install_state) {
  $install_state['imagex'] = array();

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
