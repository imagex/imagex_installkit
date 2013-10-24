<?php
/**
 * @file
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
      // Create the default role.
      $admin_role = new stdClass();
      $admin_role->name = 'administrator';
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

    $blocks = array();
    foreach (module_implements('imagex_installkit_install_block_info') as $module) {
      $return = module_invoke($module, 'imagex_installkit_install_block_info');
      if (is_array($return) && !empty($return)) {
        foreach ($return as $block) {
          $block = array_merge(imagex_installkit_block_info_defaults(), $block);
          // Prior to continuing, each block requires a delta.
          // If this block does not have one is empty, let's just simply move on.
          if (empty($block['delta'])) {
            continue;
          }
          // If no module name is specified, attempt to assign this module's name.
          if (empty($block['module'])) {
            $block['module'] = $module;
          }
          // Add the block to the blocks stack for installation.
          $blocks[$module][] = $block;
        }
      }
    }

    // If there are blocks defined that need to be configured and placed, let's do it!
    if (!empty($blocks)) {
      // Clear all previously set block data and information.
      // Warning! This is dangerous and could potentially have hazardous changes.
      db_delete('block')->execute();

      // Let's insert all of the newly defined and retrieved blocks.
      $block_query = db_insert('block')->fields(array('module', 'delta', 'theme', 'status', 'weight', 'region', 'pages', 'cache'));
      foreach ($blocks as $module_name => $blockinfo) {
        $block_query->values($block);
      }
      $block_query->execute();
    }

    // Set and share the completion message.
    $this->context['message'] = st('Default configuration and setup complete');
  }
}
