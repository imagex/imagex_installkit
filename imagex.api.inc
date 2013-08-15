<?php
/**
 * @file
 */

/**
 *
 * Installation Steps:
 * * Choose a Profile
 * * Choose a Language (default locale = en)
 * * Verify Requirements [name = requirements]
 * * Setup a database (TODO: Rename, allow for additional info)
 * * Install Profile (this is simply the installation of all dependencies)
 * * Configure site (settings form)
 * * Finished
 *
 * All tasks will for the "Install Profile" core step, enabling additional
 * events/actions/triggers to invoke for ordering (for example, before a
 * specific module.)
 */
function hook_imagex_install_tasks() {
  // TODO: Create routine for allowing and modifying the requirements.
  return array();
}

/**
 */
function hook_imagex_install_tasks_alter(array &$tasks) {
  // TODO: Implement.
}
