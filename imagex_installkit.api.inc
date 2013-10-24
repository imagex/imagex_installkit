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
function hook_imagex_installkit_install_tasks() {
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
function hook_imagex_installkit_install_tasks_alter(array &$tasks) {
  // TODO: Provide example of how to alter.
}

/**
 * Returns a multi-dimensional array of block settings information.
 *
 * @return array
 */
// TODO: Write better documentation.
function hook_imagex_installkit_install_block_info() {
  return array();
}

/**
 * Hook to specify the default theme that should be enabled and set.
 *
 * The most derived installation profile's implementation of this hook will be
 * used for determining the default theme at installation. Therefore, as a
 * result, the imagex_installkit has implemented a top level version specifying
 * a theme to enable during installation if no child profile specifies one.
 * 
 * @return string
 *   The name of the theme to enable for default usage.
 */
function hook_imagex_installkit_default_theme() {
  return 'parrot';
}
