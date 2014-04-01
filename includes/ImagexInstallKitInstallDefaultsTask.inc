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
 * Handles the default configurations of the profile install.
 * 
 * @author Aaron McGowan <aaron@imagexmedia.com>
 * @package ImageX.InstallKit.Install
 * @version  1.0.1
 */
class ImagexInstallKitInstallDefaultsTask extends ImagexInstallKitInstallTask {
  /**
   * {@inheritdoc}
   */
  public function execute() {
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
      // Disable bartik, it is no longer needed.
      theme_disable(array('bartik'));
      // Enable the default theme and set it as a the default.
      theme_enable(array($default_theme));
      variable_set('theme_default', $default_theme);
    }

    // If the UUID Menu Links module exists, let's synchronize all links
    // and paths.
    if (module_exists('uuid_menu_links')) {
      uuid_menu_links_sync();
    }

    // Disable CTools page manager node view by default.
    variable_set('page_manager_node_view_disabled', FALSE);

    // Set and share the completion message.
    $this->context['message'] = st('Default configuration and setup complete');
  }
}
