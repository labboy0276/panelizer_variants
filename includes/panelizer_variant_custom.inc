<?php

/**
 * Custom function file used throughout module
 * @see .module & panelizer_variant_form.inc
 **/

/**
* Custom function for rendering view modes in textarea
**/
function _panelizer_variant_view_modes_textarea() 
{
    $view_modes = db_query('SELECT view_mode_name FROM {panelizer_variant_view_modes}');
    $result = '';
    foreach($view_modes as $view_mode) {
        $result .= $view_mode->view_mode_name . "\n";
    }
    //remove last carriage return
    $result = rtrim($result, "\n");
    return $result;
}

/** 
* Custom function for rendering view modes in select
**/
function _panelizer_variant_view_modes_select() 
{
    $view_modes = db_query('SELECT view_mode_name,view_mode_machine_name FROM {panelizer_variant_view_modes}');
    $result = array();
    foreach($view_modes as $view_mode) {
        $result[$view_mode->view_mode_machine_name] = $view_mode->view_mode_name;
    }
    return $result;
}

/**
* Custom function for getting path for variable setting
**/
function _panelizer_variant_get_path($path) 
{
    $path = explode('/', $path);
    return $path[5];
}

/**
* Custom function for getting panelizer id # from the name
**/
function _panelizer_variant_get_panelizer_name($path) 
{
    $result = array();
    $path = explode('/', $path);
    $result['path'] = $path[5];
    //Set the name like the panelizer_default name column
    $name = 'node:' . str_replace('.', ':default:', $path[5]);
    $result['name'] = $name;
    //Grab PNID
    $pnid = db_query('SELECT pnid FROM {panelizer_defaults} WHERE name = :name', array(':name' => $name))->fetch();
    $result['pnid'] = $pnid->pnid;

    return $result;
}

/**
* Custom function for updating db table, better than having it in the verify
**/
function _panelizer_variant_update_table($result, $value) 
{
    $data = array(
        'pnid' => $result['pnid'],
        'name' => $result['name'],
        'variant' => $value,
    );
    $exist = db_query('SELECT pnid FROM {panelizer_variant} WHERE pnid = :pnid', array(':pnid' => $result['pnid']))->rowCount();
    if ($exist > 0) {
        drupal_write_record('panelizer_variant', $data, 'pnid');
    } else {
        drupal_write_record('panelizer_variant', $data);
    }
}


/**
* Custom function for panelizer_variant_entity_view_mode_alter
**/
function _panelizer_variant_entity_render($context) 
{
    $entity = array();

    //checks to see if this entity is panelied
    if (!isset($context['entity']->panelizer)) {
        $entity['panelized'] = false;
        return $entity;
    } else {
        $entity['panelized'] = true;
        $entity['type'] = $context['entity']->type;
           //grab the tid from the taxonomy term reference
        foreach($context['entity'] as $key => $value) {
            if (strpos($key, 'field_') !== false) {
                if (isset($value[LANGUAGE_NONE][0]['tid'])) {
                    $current_tid = $value[LANGUAGE_NONE][0]['tid'];
                }
            }
        }
        //grab all context possibilities to match up
        foreach ($context['entity']->panelizer as $key => $value) {
            if(isset($value->pnid) && !empty($value->contexts)) {
                $variant=db_query('SELECT variant FROM {panelizer_variant} WHERE pnid = :pnid', array(':pnid' => $value->pnid))->fetch();
                if($variant->variant) { 
                    foreach ($value->contexts as $context_check) {
                        if ($current_tid == $context_check['entity_id']) {
                            $entity['view_mode'] = $value->view_mode;
                        }
                    } 
                }
            }
        }
        return $entity;
    }
}