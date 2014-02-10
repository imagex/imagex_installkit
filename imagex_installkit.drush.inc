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
 * Implements hook_drush_command().
 */
function imagex_installkit_drush_command() {
  $commands = array();
  $commands['imagex-installkit-block-rebuild'] = array(
    'description' => 'Rebuilds the blocks defined by hook_imagex_installkit_install_block_info() at runtime.',
    'options' => array(
      'all' => 'Signals that all blocks should be removed.',
    ),
    'aliases' => array('block-rebuild'),
  );
  return $commands;
}

/**
 * Drush callback for rebuilding blocks defined.
 */
function drush_imagex_installkit_block_rebuild() {
  imagex_installkit_load_include('inc', 'includes/block');

  // Determine if we are all removing all blocks or just those defined.
  $remove_all_blocks = (bool) drush_get_option('all', FALSE);

  // Rebuild all blocks.
  imagex_installkit_block_rebuild($remove_all_blocks);

  // Display a success message.
  drush_log(dt('ImageX InstallKit has rebuilt blocks defined.'), 'ok');
}
