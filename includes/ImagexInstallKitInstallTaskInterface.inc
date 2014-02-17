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
 * Interface for describing an installation task with InstallKit.
 *
 * @author Aaron McGowan <aaron@imagexmedia.com>
 * @package ImageX.InstallKit.Install
 * @version 1.0.0
 */
interface ImagexInstallKitInstallTaskInterface {
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
  public function setContext(array &$context);

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
  public function setArguments(array $arguments);

  /**
   * Performs the operations for this installation task.
   *
   * @access public
   */
  public function execute();
}
