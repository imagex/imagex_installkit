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
 * Handles the installation of a module during the profile install.
 * 
 * @author Aaron McGowan <aaron@imagexmedia.com>
 * @package ImageX.InstallKit.Install
 * @version  1.0.0
 */
class ImagexInstallKitInstallModuleTask extends ImagexInstallKitInstallTask {
  /**
   * {@inheritDoc}
   */
  public function execute() {
    // If no module is specified, this task cannot run, exit early.
    if (empty($this->arguments['module'])) {
      imagex_installkit_log(WATCHDOG_DEBUG, t('Unspecified module attempting to be enabled.'));
      return;
    }

    // Attempt to enable the specified module and log the results.
    $enabled = module_enable(array($this->arguments['module']), TRUE);
    $module_name = empty($this->arguments['module name']) ? $this->arguments['module'] : $this->arguments['module name'];
    if ($enabled) {
      $message = st('Installed module @module', array('@module' => $module_name));
      imagex_installkit_log(WATCHDOG_NOTICE, $message);
    }
    else {
      $message = st('Failed to install module @module', array('@module' => $module_name));
      imagex_installkit_log(WATCHDOG_ERROR, $message);
    }

    // Set the context message so that user's know what is happening.
    $this->context['message'] = $message;
  }
}
