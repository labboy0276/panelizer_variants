<?php

/* 
* Custom function for rendering view modes in textarea
*/
function _panelizer_variant_view_modes_textarea() {
	$view_modes = db_query('SELECT view_mode_name FROM {panelizer_variant_view_modes}');
	$result = '';
	foreach($view_modes as $view_mode) {
  	$result .= $view_mode->view_mode_name . "\n";
	}
	//remove last carriage return
	$result = rtrim($result, "\n");
	return $result;
}

/* 
* Custom function for rendering view modes in select
*/
function _panelizer_variant_view_modes_select() {
	$view_modes = db_query('SELECT view_mode_name,view_mode_machine_name FROM {panelizer_variant_view_modes}');
	$result = array();
	foreach($view_modes as $view_mode) {
  	$result[$view_mode->view_mode_machine_name] = $view_mode->view_mode_name;
	}
	return $result;
}

/* 
* Custom function for panelizer_variant_entity_view_mode_alter
*/
function _panelizer_variant_entity_render($context) {
	$entity = array();
	$entity['type'] = $context['entity']->type;

	//grab the tid from the taxonomy term reference
	foreach($context['entity'] as $key => $value) {
		if (strpos($key,'field_') !== false) {
    	if (isset($value['und'][0]['tid'])) {
    		$current_tid = $value['und'][0]['tid'];
    	}
		}
	}
	
	//grab all context possibilities to match up
	foreach ($context['entity']->panelizer as $key => $value) {
		if (!empty($value->contexts)) {
			foreach ($value->contexts as $context_check) {
				if ($current_tid == $context_check['entity_id']) {
					$entity['view_mode'] = $value->view_mode;
				}
			}
		}
	}

	return $entity;
}