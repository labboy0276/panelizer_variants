<?php

/**
 * @file
 * Custom function file used throughout module.
 * @see .module & panelizer_variant_form.inc
 */

/**
 * Custom function for panelizer_variant_entity_view_mode_alter.
 */
function _panelizer_variant_entity_render($contexts) {

  // Grab the tid from the taxonomy term reference.
  $key = (key($contexts));
  if (isset($contexts[$key]->panelizer)) {
    foreach ($contexts[$key] as $context => $value) {
      if (strpos($context, 'field_') !== FALSE) {
        if (isset($value[LANGUAGE_NONE][0]['tid'])) {
          $current_tid = $value[LANGUAGE_NONE][0]['tid'];
        }
      }
    }

    // Grab all the set varinats in the DB to check, then apply the entity.
    $variants = db_query('SELECT * FROM {panelizer_variant}')->fetchAll();
    foreach ($variants as $variant) {
      $datas = unserialize($variant->context);
      foreach ($datas as $data) {
        if ($data['name'] == 'taxonomy_term' && $data['entity_id'] == $current_tid) {
          $args = array(':name' => $variant->name, ':did' => $variant->did);
          $entity = db_query('SELECT * FROM {panelizer_entity} WHERE name = :name AND did = :did', $args)->fetch();
        }
      }
    }
  }

  return $entity;
}
