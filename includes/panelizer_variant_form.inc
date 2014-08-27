<?php

/**
 * Page callback: panelizer variant settings
 *
 * @see panelizer_variant_menu()
 */
function panelizer_variant_form($form, &$form_state) {
  $form['view_modes'] = array(
    '#type' => 'textarea',
    '#title' => t('View Mode(s)'),
    '#default_value' => _panelizer_variant_view_modes_textarea(),
    '#rows' => 10,
    '#description' => t('The View Modes. Enter one value per line'),
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Save Configuration',
  );

  return $form;
}

function panelizer_variant_form_submit($form, &$form_state) {
		if ($form_state['input']['view_modes'] != '') {
			// clear out existing view modes
			// @todo could be dangerous if things are already selected, find a more gooder way
			db_delete('panelizer_variant_view_modes')->execute();
			//get data to parse
			$view_modes = preg_split("/\r\n|\n|\r/", $form_state['values']['view_modes']);
			foreach ($view_modes as $view_mode) {
				$machine_name = str_replace(' ', '_', $view_mode);
				db_insert('panelizer_variant_view_modes')
				->fields(array(
						'view_mode_name' => $view_mode,
						'view_mode_machine_name' => $machine_name,
					))
				->execute();
			}
			drupal_set_message(t('The settings have been saved'));
		} else {
			db_delete('panelizer_variant_view_modes')->execute();
			drupal_set_message(t('Everything is gone, All hope is lost'));
		}
    
}