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
abstract class ImagexInstallKitInstallTask {
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
   * Sets the contextual information for this task.
   *
   * @access public
   * @param array $context
   *   An array of contextual information.
   *
   * @return $this
   *   Returns this instance of the ImagexInstallTask.
   */
  final public function setContext(array &$context) {
    $this->context = $context;
    return $this;
  }

  /**
   * Sets the arguments to be used within the `execute()` of this task.
   *
   * @access public
   * @param array $arguments
   *   A keyed array of arguments to set.
   *
   * @return $this
   *   Returns this instance of the ImagexInstallTask.
   */
  final public function setArguments(array $arguments) {
    $this->arguments = $arguments;
    return $this;
  }

  /**
   * Performs the operations for this installation task.
   *
   * @access public
   */
  abstract public function execute();
}
