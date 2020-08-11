<?php

/**
 * @file
 * user_details_db_creation.features.inc
 */

/**
 * Implements hook_views_api().
 */
function user_details_db_creation_views_api($module = NULL, $api = NULL) {
  return array("api" => "3.0");
}

/**
 * Implements hook_node_info().
 */
function user_details_db_creation_node_info() {
  $items = array(
    'user_details' => array(
      'name' => t('User details'),
      'base' => 'node_content',
      'description' => t('Store user details'),
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
  );
  drupal_alter('node_info', $items);
  return $items;
}
