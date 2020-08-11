<?php

/**
 * @file
 * User details dashboard.
 */

/**
 * Callback function user_details_dashboard_form
 */
function user_details_dashboard_form($form, &$form_state) {
  $header = array(
    array('data' => t('Name'), 'field' => 'field_user_name_value', 'sort' => 'desc'),
    array('data' => t('Mobile')),
    array('data' => t('Email ID')),
    array('data' => t('Gender')),
    array('data' => t('User Type')),
  );

  $params = drupal_get_query_parameters();

  $rows = array();
  $data = user_details_get_user_details_dashboard_data($header, $params);
  list($rows, $header) = user_details_get_user_details_dashboard_table($header, $data);

  variable_set('user_details_dashboard_header', $header);
  variable_set('user_details_dashboard_row', $rows);

  // If rows are not empty theme and display the rows.
  $output = '';
  if (!empty($rows)) {
    $output = theme('table', array('header' => $header, 'rows' => $rows, 'attributes' => array('id' => 'sort-table')));
    $output .= l(t('Export CSV'), 'user-details/dashboard/download-csv', array('attributes' => array('class' => array('about-link', 'another-class'))));
    $output .= theme('pager');
  }

  $user_types = user_details_get_taxonomy_term_ids_from_vocab_name('user_types');
  $user_types[0] = 'None';
  ksort($user_types);
  $form['mobile_no'] = array(
    '#type' => 'textfield',
    '#title' => t('Mobile No'),
    '#default_value' => !empty($params['mobile_no']) ? $params['mobile_no'] : '',
    '#size' => 20,
    '#maxlength' => 128,
  );

  $form['name'] = array(
    '#type' => 'textfield',
    '#title' => t('Name'),
    '#default_value' => !empty($params['name']) ? $params['name'] : '',
    '#size' => 20,
    '#maxlength' => 128,
  );

  $form['email_id'] = array(
    '#type' => 'textfield',
    '#title' => t('Email ID'),
    '#default_value' => !empty($params['email_id']) ? $params['email_id'] : '',
    '#size' => 20,
    '#maxlength' => 128,
  );

  $form['user_type'] = array(
    '#type' => 'select',
    '#title' => t('User Type'),
    '#default_value' => !empty($params['user_type']) ? $params['user_type'] : '',
    '#options' => $user_types,
  );

  $form['filter'] = array(
    '#type' => 'submit',
    '#value' => 'Filter',
  );

  $form['cancel'] = array(
    '#type' => 'submit',
    '#value' => 'Cancel',
  );

  $form['table'] = array(
    '#markup' => $output,
  );

  return $form;
}

/**
 * Submit handler for user_details_dashboard_form
 */
function user_details_dashboard_form_submit($form, &$form_state) {

  $values  = $form_state['values'];
  $query = array();
  if ($values['op'] == "Filter") {
    $filter_list = array('mobile_no', 'name', 'email_id', 'user_type');
    foreach ($filter_list as $filter) {
      if (!empty($values[$filter])) {
        $query[$filter] = $values[$filter];
      }
    }
  }

  $form_state['redirect'] = array(
    'user-details/dashboard',
    array(
      'query' => array(
        $query,
      ),
    ),
  );
}
