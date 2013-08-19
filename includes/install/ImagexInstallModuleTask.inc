<?php
/**
 * @file
 */

/**
 * Handles the installation of a module during the profile install.
 * 
 * @author Aaron McGowan <aaron@imagexmedia.com>
 * @package ImageX.Install
 * @version  1.0.0
 */
class ImagexInstallModuleTask extends ImagexInstallTask {
  /**
   * {@inheritDoc}
   */
  public function execute() {
    if (empty($this->arguments['module']) || empty($this->arguments['module name'])) {
      return;
    }

    if (module_enable(array($this->arguments['module']))) {
      $message = st('Installed module %module', array(
        '%module' => $this->arguments['module name'],
      ));
    }
    else {
      $message = st('Failed to install module %module', array(
        '%module' => $this->arguments['module name'],
      ));
    }

    $this->context['message'] = $message;
  }
}
