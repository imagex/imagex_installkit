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
 * Performs feature revert for all features within this installation.
 *
 * @author Aaron McGowan <aaron@imagexmedia.com>
 * @package ImageX.InstallKit.Install
 * @version  1.0.0
 */
class ImagexInstallKitInstallFeaturesRevertTask extends ImagexInstallKitInstallTask {
  /**
   * {@inheritDoc}
   */
  public function execute() {
    if (!module_exists('features')) {
      imagex_installkit_log(WATCHDOG_DEBUG, st('Features module does not exist and or is not enabled and therefore, features revert cannot be ran.'));
      return;
    }
    // Set timeout to 0, unlimited, so that the execution does not time out.
    drupal_set_time_limit(0);
    // Revert all features that are part of this installation.
    features_revert();
    // Set the context message so that user's know what is happening.
    $this->context['message'] = st('Features reverted.');
  }
}
