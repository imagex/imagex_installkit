<?php
/**
 * @file
 */

/**
 * Handles the default configurations of the profile install.
 * 
 * @author Aaron McGowan <aaron@imagexmedia.com>
 * @package ImageX.InstallKit.Install
 * @version  1.0.0
 */
class ImagexInstallKitInstallDefaultsTask extends ImagexInstallKitInstallTask {
  /**
   * {@inheritDoc}
   */
  public function execute() {
    if (module_exists('taxonomy')) {
      // Create a default vocabulary named "Tags".
      $vocabulary = (object) array(
        'name' => st('Tags'),
        'description' => st('Use tags to group content on similar topics into categories.'),
        'machine_name' => 'tags',
        'help' => st('Enter a comma-separated list of words to describe your content.'),
      );
      taxonomy_vocabulary_save($vocabulary);
    }

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

    // Set and share the completion message.
    $this->context['message'] = st('Default configuration and setup complete');
  }
}
