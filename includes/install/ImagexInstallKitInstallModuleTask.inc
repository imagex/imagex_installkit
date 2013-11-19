<?php
/**
 * @file
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
    if (empty($this->arguments['module'])) {
      imagex_installkit_log(WATCHDOG_DEBUG, t('Unspecified module attempting to be enabled.'));
      return;
    }

    $enabled = module_enable(array($this->arguments['module']), TRUE);
    if ($enabled) {
      $message = st('Installed module %module', array(
        '%module' => empty($this->arguments['module name']) ? $this->arguments['module'] : $this->arguments['module name'],
      ));
      imagex_installkit_log(WATCHDOG_NOTICE, $message);
    }
    else {
      $message = st('Failed to install module %module', array(
        '%module' => empty($this->arguments['module name']) ? $this->arguments['module'] : $this->arguments['module name'],
      ));
      imagex_installkit_log(WATCHDOG_ERROR, $message);
    }

    $this->context['message'] = $message;
  }
}
