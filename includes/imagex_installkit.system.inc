<?php
/**
 * @file
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
 * Implements hook_system_info_alter().
 */
function imagex_installkit_system_info_alter(&$info, $file, $type) {
  if ('module' == $type) {
    $module_list = &_imagex_installkit_system_list_modules();
    if (!empty($info['imagex_installkit']['dependencies']['conditional'])) {
      foreach ($info['imagex_installkit']['dependencies']['conditional'] as $dependency) {
        if (isset($module_list[$dependency])) {
          $info['dependencies'][] = $dependency;
        }
      }
    }
    _imagex_installkit_system_module_info_alter($info, $file);
  }
}

/**
 * Perform last-minute alterations to enforce InstallKit requirements.
 *
 * @param array $info
 *   The module's parsed .info file data.
 * @param object $file
 *   The primary file information for the .module from drupal_system_listing.
 */
function _imagex_installkit_system_module_info_alter(&$info, $file) {
  switch ($file->name) {
    case 'file':
      $module_list = &_imagex_installkit_system_list_modules();
      if (isset($module_list['uuid'])) {
        $info['dependencies'][] = 'uuid';
        $info['dependencies'][] = 'uuid_helper';
      }
      break;
  }
}

/**
 * Returns an array of module data from {system}.
 *
 * @return array
 *   Returns an array of module data.
 */
function &_imagex_installkit_system_list_modules() {
  static $module_list;
  if (!isset($module_list)) {
    $module_list = array();
    $results = db_query("SELECT * FROM {system} WHERE type = 'module' ORDER BY weight ASC, name ASC");
    foreach ($results as $result) {
      $result->info = unserialize($result->info);
      $module_list[$result->name] = $result;
    }
  }
  return $module_list;
}
