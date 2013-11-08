<?php
/**
 * @file
 */

/**
 * Returns default information regarding block settings.
 *
 * @return array
 *   Returns an array of default values for saving block info in the `{block}` table.
 */
function imagex_installkit_block_info_defaults() {
  return array(
    'module' => '',
    'delta' => '',
    'theme' => '',
    'visibility' => 0,
    'region' => '',
    'status' => 1,
    'pages' => '',
    'title' => '<none>',
    'weight' => 0,
    'cache' => -1,
  );
}

/**
 * Performs the block rebuild for blocks defined by block info hook.
 *
 * @param bool $remove_all
 *   A boolean to determine whether or not all blocks should be removed prior.
 */
function imagex_installkit_block_rebuild($remove_all = FALSE) {
  $blocks = array();
  foreach (module_implements('imagex_installkit_install_block_info') as $module) {
    $return = module_invoke($module, 'imagex_installkit_install_block_info');
    if (is_array($return) && !empty($return)) {
      foreach ($return as $block) {
        $block = array_merge(imagex_installkit_block_info_defaults(), $block);
        // Prior to continuing, each block requires a delta.
        // If this block does not have one and is empty, let's just move on.
        if (empty($block['delta'])) {
          continue;
        }
        // If no module name is specified, attempt to assign this module's name.
        if (empty($block['module'])) {
          $block['module'] = $module;
        }
        // Add the block to the blocks stack for installation.
        $blocks[$module][] = $block;
      }
      unset($block);
    }
    unset($return);
  }

  // If there are blocks defined that need to be configured and placed,
  // let's do it now!
  if (!empty($blocks)) {
    if (!$remove_all) {
      foreach ($blocks as $module_name => $blockinfo) {
        foreach ($blockinfo as $block) {
          $query = db_delete('block')
            ->condition('module', $block['module'])
            ->condition('delta', $block['delta'])
            ->condition('theme', $block['theme']);
          $query->execute();
        }
        unset($block);
      }
      unset($blockinfo);
    }
    else {
      // Clear all previously set block data and information.
      // Warning! This is dangerous and could be potentially have
      // hazardous changes that are unknown.
      db_delete('block')->execute();
    }
    unset($block_delete_query);

    // Let's clear the block cache so a full rebuild can occur.
    cache_clear_all(NULL, 'cache_block');

    // Now let's either update or insert those that need to be.
    $block_query = db_insert('block')->fields(array('module', 'delta', 'theme', 'status', 'weight', 'region', 'pages', 'cache'));
    foreach ($blocks as $module_name => $blockinfo) {
      foreach ($blockinfo as $block) {
        $block_query->values($block);
      }
    }
    $block_query->execute();
  }
}
