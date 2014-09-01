<?php

/**
 * @file
 * Custom function file used throughout module.
 * @see .module & panelizer_variant_form.inc
 */

/**
 * Custom function for rendering view modes in textarea.
 */
function _panelizer_variant_view_modes_textarea() {
  $view_modes = db_query('SELECT view_mode_name FROM {panelizer_variant_view_modes}');
  $result = '';
  foreach ($view_modes as $view_mode) {
    $result .= $view_mode->view_mode_name . "\n";
  }
  // Remove last carriage return.
  $result = rtrim($result, "\n");
  return $result;
}

/**
 * Custom function for rendering view modes in select.
 */
function _panelizer_variant_view_modes_select() {
  $view_modes = db_query('SELECT view_mode_name,view_mode_machine_name FROM {panelizer_variant_view_modes}');
  $result = array();
  foreach ($view_modes as $view_mode) {
    $result[$view_mode->view_mode_machine_name] = $view_mode->view_mode_name;
  }
  return $result;
}

/**
 * Custom function for panelizer_variant_entity_view_mode_alter.
 */
function _panelizer_variant_entity_render($context) {
  $entity = array();

  // Checks to see if this entity is panelied.
  if (!isset($context['entity']->panelizer)) {
    $entity['panelized'] = FALSE;
    return $entity;
  }
  else {
    $entity['panelized'] = TRUE;
    $entity['type'] = $context['entity']->type;
    // Grab the tid from the taxonomy term reference.
    foreach ($context['entity'] as $key => $value) {
      if (strpos($key, 'field_') !== FALSE) {
        if (isset($value[LANGUAGE_NONE][0]['tid'])) {
          $current_tid = $value[LANGUAGE_NONE][0]['tid'];
        }
      }
    }
    // Grab the context, @todo make this work for all contexts.
    foreach ($context['entity']->panelizer as $key => $value) {
      if (isset($value->name) && !empty($value->contexts)) {
        $name = $value->name;
        $variant = db_query('SELECT variant FROM {panelizer_variant} WHERE name = :name', array(':name' => $name))->fetchField();
        if ($variant) {
          $datas = unserialize($variant);
          foreach ($datas as $data) {
            if ($data['name'] == 'taxonomy_term' && $data['entity_id'] == $current_tid) {
              $entity['view_mode'] = $value->view_mode;
            }
          }
        }
      }
    }
    return $entity;
  }
}
