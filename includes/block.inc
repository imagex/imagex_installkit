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
