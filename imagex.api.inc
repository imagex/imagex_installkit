<?php
/**
 * @file
 */

/**
 * Allows specifying of new installation tasks to occur during Install Profile.
 * 
 * All tasks will for the "Install Profile" core step, enabling additional
 * events/actions/triggers to invoke for ordering (for example, before a
 * specific module.)
 *
 * @return array
 *   An array of installation task information.
 */
function hook_imagex_install_tasks() {
  // TODO: Create routine for allowing and modifying the requirements.
  return array();
}

/**
 * Hook that allows altering of tasks or batch operations prior to setting them.
 *
 * The passed array of operations or tasks to be invoked during the Install
 * Profile step of the installation is a two dimensional array of information.
 * Each element of the `$tasks` array is an array containing the
 * ImagexInstallTask derived class name (first element,) followed by a keyed
 * array of arguments to be specified at time of instantiation.
 *
 * @example
 *   array(
 *     array(
 *       'ImagexInstallTask',
 *       array(
 *         'module name' => 'MyModule',
 *       )
 *     )
 *   )
 *
 * @param array $tasks
 *   An array of operations to perform during the `Install Profile` step of
 *   installation.
 */
function hook_imagex_install_tasks_alter(array &$tasks) {
  // TODO: Provide example of how to alter.
}
