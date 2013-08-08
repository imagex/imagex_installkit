<?php
/**
 * @file
 */

/**
 *
 * @author Aaron McGowan <aaron@imagexmedia.com>
 * @package ImageX
 * @version 1.0.0
 */
abstract class ImagexInstallTask {
  /**
   * Creates a new instance of ImageXInstallTask.
   *
   * @access public
   */
  final public function __construct() {
    // Empty.
  }

  /**
   * @return array
   */
  abstract public function getInfo();

  /**
   * @access public
   */
  abstract public function execute();
}