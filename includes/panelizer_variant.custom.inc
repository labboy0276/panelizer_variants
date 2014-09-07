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
  $entity = array();
  $entity_display = array();

  // Grab the tid from the taxonomy term reference.
  $key = (key($contexts));
  if (isset($contexts[$key]->panelizer)) {
    foreach ($contexts[$key] as $context => $value) {
      if (strpos($context, 'field_') !== FALSE) {
        // If a term reference taxonomy terms.
        if (isset($value[LANGUAGE_NONE][0]['tid'])) {
          $current_tid = $value[LANGUAGE_NONE][0]['tid'];
        // If a entity reference taxonomy terms.
        } elseif (isset($value[LANGUAGE_NONE][0]['target_id'])) {
          $current_tid = $value[LANGUAGE_NONE][0]['target_id'];
        }
        elseif (!isset($current_tid)) {
          $current_tid = NULL;
        }
      }
    }

    // Grab all the set varinats in the DB to check, then apply the entity.
    $variants = db_query('SELECT * FROM {panelizer_variant}')->fetchAll();
    foreach ($variants as $variant) {
      $datas = unserialize($variant->context);
      foreach ($datas as $data) {
        if ($data['name'] == 'taxonomy_term' && $data['entity_id'] == $current_tid) {
          $args = array(':name' => $variant->name, ':pnid' => $variant->pnid);
          $entities = db_query('SELECT * FROM {panelizer_defaults} WHERE name = :name AND pnid = :pnid', $args)->fetch();
          foreach ($entities as $key => $value) {
            $entity[$key] = $value;
          }
          $displays = panels_load_displays(array($variant->did));
          foreach ($displays as $display) {
            $entity_display['display'] = $display;
          }
          $entity = array_merge($entity, $entity_display);
        }
      }
    }
  }

  return $entity;
}
