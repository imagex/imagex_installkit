<?php
/**
 * @file
 */

/**
 * Implements hook_system_info_alter().
 */
function imagex_installkit_system_info_alter(&$info, $file, $type) {
  // If the type is not a module, exit early.
  if ('module' !== $type) {
    return;
  }

  if ('done' !== variable_get('install_task', NULL)) {
    $module_list = &_imagex_installkit_system_list_modules();
    if (!empty($info['imagex_installkit']['dependencies']['conditional'])) {
      foreach ($info['imagex_installkit']['dependencies']['conditional'] as $dependency) {
        if (isset($module_list[$dependency])) {
          $info['dependencies'][] = $dependency;
        }
      }
    }
  }
  else {
    // TODO: Implement what is needed here.
  }
}

/**
 *
 * @return array
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
