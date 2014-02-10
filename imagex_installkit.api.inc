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
 * Hook that allows for altering of required and non-required module stacks.
 *
 * @param array $required
 *   An array of `required` modules, passed by reference.
 * @param array $non_required
 *   An array of `non-required` modules, passed by reference.
 */
function hook_imagex_installkit_install_profile_modules(array &$required, array &$non_required) {
  // TODO: Provide example of how to alter.
}

/**
 * Returns a multi-dimensional array of block settings information.
 *
 * @return array
 */
function hook_imagex_installkit_install_block_info() {
  // TODO: Write better documentation.
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
