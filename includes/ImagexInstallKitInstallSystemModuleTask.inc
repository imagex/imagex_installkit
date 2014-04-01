<?php
/**
 * @file
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
 * Installation Task; installs Drupal system module & base configuration.
 *
 * @author Aaron McGowan <aaron@imagexmedia.com>
 * @package ImageX.InstallKit.Install
 * @version  1.0.0
 */
class ImagexInstallKitInstallSystemModuleTask extends ImagexInstallKitInstallTask {
  /**
   * {@inheritdoc}
   */
  public function execute() {
    // Invoke Drupal's core system module installation task.
    install_system_module($this->context);

    // Create the default administrator role.
    $admin_role = new stdClass();
    $admin_role->name = IMAGEX_INSTALLKIT_USER_ADMINISTRATOR_ROLE;
    $admin_role->weight = 2;
    user_role_save($admin_role);
    user_role_grant_permissions($admin_role->rid, array_keys(
      module_invoke_all('permission')
    ));

    // Assign the admin (user id = 1) to administrator role.
    db_insert('users_roles')
      ->fields(array(
        'uid' => 1,
        'rid' => $admin_role->rid,
      ))->execute();

    // Disallow visitor accounts to be automatically created.
    variable_set('user_register', USER_REGISTER_VISITORS_ADMINISTRATIVE_APPROVAL);

    // Setup the absolute basics, starting with the admin theme.
    db_update('system')
      ->fields(array('status' => 1))
      ->condition('type', 'theme')
      ->condition('name', 'seven')
      ->execute();
    variable_set('admin_theme', 'seven');
    variable_set('node_admin_theme', '1');
  }
}
