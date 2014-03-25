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
 * Handles the default configurations of the profile install.
 * 
 * @author Aaron McGowan <aaron@imagexmedia.com>
 * @package ImageX.InstallKit.Install
 * @version  1.0.1
 */
class ImagexInstallKitInstallDefaultsTask extends ImagexInstallKitInstallTask {
  /**
   * {@inheritDoc}
   */
  public function execute() {
    if (module_exists('user')) {
      // Include ImageX User Roles so that constants may be used.
      module_load_include('module', 'imagex_user_roles');

      // Create the default role.
      $admin_role = new stdClass();
      $admin_role->name = IMAGEX_USER_ROLES_ADMINISTRATOR_ROLE;
      $admin_role->weight = 2;
      user_role_save($admin_role);
      user_role_grant_permissions($admin_role->rid, array_keys(
        module_invoke_all('permission')
      ));

      // Assign the admin (user id = 1) to admin role.
      db_insert('users_roles')
        ->fields(array(
          'uid' => 1,
          'rid' => $admin_role->rid,
        ))->execute();

      // Disallow visitor accounts to be automatically created.
      variable_set('user_register', USER_REGISTER_VISITORS_ADMINISTRATIVE_APPROVAL);
    }

    // Setup the absolute basics, starting with the admin theme.
    db_update('system')
      ->fields(array('status' => 1))
      ->condition('type', 'theme')
      ->condition('name', 'seven')
      ->execute();
    variable_set('admin_theme', 'seven');
    variable_set('node_admin_theme', '1');

    // Disable bartik, it is no longer needed.
    theme_disable(array('bartik'));

    // For each profile that is a child of this, attempt to find the
    // default theme by invoking hook_imagex_installkit_default_theme().
    $default_theme = NULL;
    foreach (imagex_installkit_get_install_profiles() as $profile) {
      $function = $profile . '_imagex_installkit_default_theme';
      if (function_exists($function)) {
        $default_theme = $function();
      }
    }
    
    if ($default_theme) {
      // Enable the default theme and set it as a the default.
      theme_enable(array($default_theme));
      variable_set('theme_default', $default_theme);
    }

    // Time to retrieve and configure any defined blocks.
    imagex_installkit_load_include('inc', 'includes/block');
    imagex_installkit_block_rebuild(TRUE);

    // If the UUID Menu Links module exists, let's synchronize all links and paths.
    if (module_exists('uuid_menu_links')) {
      uuid_menu_links_sync();
    }

    variable_set('page_manager_node_view_disabled', FALSE);

    // Set and share the completion message.
    $this->context['message'] = st('Default configuration and setup complete');
  }
}
