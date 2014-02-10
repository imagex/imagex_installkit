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
 * Handles the execution of a callback function as a installation task.
 *
 * @author Aaron McGowan <aaron@imagexmedia.com>
 * @package ImageX.InstallKit.Install
 * @version  1.0.0
 */
class ImagexInstallKitInstallCallbackTask extends ImagexInstallKitInstallTask {
  /**
   * {@inheritDoc}
   */
  public function execute() {
    if (empty($this->arguments['callback'])) {
      imagex_installkit_log(WATCHDOG_DEBUG, t('Callback for ImagexInstallKitInstallCallbackTask missing.'));
      return;
    }

    if (!function_exists($this->arguments['callback'])) {
      imagex_installkit_log(WATCHDOG_DEBUG, t('The specified callback function does not exist.'));
      return;
    }

    $function = $this->arguments['callback'];
    $function($this->context);
  }
}
