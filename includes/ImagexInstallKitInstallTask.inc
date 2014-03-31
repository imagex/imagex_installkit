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
 * Base installation task class that executes tasks during install.
 *
 * @author Aaron McGowan <aaron@imagexmedia.com>
 * @package ImageX.InstallKit.Install
 * @version 1.0.0
 */
abstract class ImagexInstallKitInstallTask implements ImagexInstallKitInstallTaskInterface {
  /**
   * @var array
   *   An array of arguments for processing.
   */
  protected $arguments = array();

  /**
   * @var array
   *   An array of contextual information.
   */
  protected $context = null;

  /**
   * Creates a new instance of ImagexInstallTask.
   * 
   * @access public
   */
  final public function __construct() {
    // Empty.
  }

  /**
   * {@inheritDoc}
   */
  final public function setContext(array &$context) {
    $this->context = &$context;
    return $this;
  }

  /**
   * {@inheritDoc}
   */
  final public function setArguments(array $arguments) {
    $this->arguments = $arguments;
    return $this;
  }
}
