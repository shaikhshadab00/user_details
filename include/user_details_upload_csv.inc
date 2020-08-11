<?php

/**
 * @file
 * User details upload csv.
 */

/**
 * Callback function for upload user details csv.
 */
function user_details_upload_csv_form($form, &$form_state) {

  $import_format_link = l(t('Click here to download import format'), 'user-details/upload-csv/format', array('attributes' => array('class' => array('link')), 'html' => TRUE));
  $form['markup'] = array(
    '#markup' => t("Import csv file to upload user details "),
  );

  $form['markup_1'] = array(
    '#markup' => $import_format_link,
  );

  $form['csv_upload'] = array(
    '#type' => 'file',
    '#title' => t('Choose a file'),
    '#title_display' => 'invisible',
    '#size' => 22,
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Import'),
  );

  $form['#attributes']['enctype'] = "multipart/form-data";

  return $form;
}

/**
 * Validate handler for user_details_upload_csv_form
 */
function user_details_upload_csv_form_validate($form, &$form_state) {
  $validators = array('file_validate_extensions' => array('csv'));

  // Check for a new uploaded file.
  $file = file_save_upload('csv_upload', $validators);
  //$file = $form_state['values']['csv_upload'];

  if (isset($file)) {
    // File upload was attempted.
    if ($file) {
      // Put the temporary file in form_values so we can save it on submit.
      $form_state['values']['csv_upload_file'] = $file;
    } else {
      // File upload failed.
      form_set_error('csv_upload', t('The file could not be uploaded.'));
    }
  }
}

/**
 * Submit handler for user_details_upload_csv_form
 */
function user_details_upload_csv_form_submit($form, &$form_state) {

  $file = $form_state['values']['csv_upload_file'];
  $file->status = FILE_STATUS_PERMANENT;
  $file->filename = str_replace(' ', '_', $file->filename);
  file_save($file);

  $csv_file = file_load($file->fid);
  $file = fopen($csv_file->uri, "r");

  $count = 0;
  $processed = FALSE;
  $new = $update = $deactivate = array();
  $operations = array();
  while (!feof($file)) {
    $csv_row = fgetcsv($file);
    if ($count == 0) {
      if (trim($csv_row[0]) == 'Name' && trim($csv_row[1]) == 'Mobile' && trim($csv_row[2]) == 'Email ID' && trim($csv_row[3]) == 'Gender' && trim($csv_row[4]) == 'User Type' && trim($csv_row[5]) == 'Status') {
        $processed = TRUE;
      } else {
        drupal_set_message('CSV header Formate not match', 'error');
      }
    }
    if ($processed == TRUE && $count > 0) {
      $status = strtolower(trim($csv_row[5]));
      switch ($status) {
        case 'update':
          $nid = user_details_get_user_details_nid_from_mobile_no(trim($csv_row[1]));
          if (is_numeric($nid)) {
            $node = node_load($nid);

            if (!empty(trim($csv_row[0]))) {
              $node->field_user_name['und'][0]['value'] = trim($csv_row[0]);
            }
            if (!empty(trim($csv_row[2]))) {
              $node->field_user_email_id['und'][0]['value'] = trim($csv_row[2]);
              $node->title = trim($csv_row[2]);
            }
            if (!empty(trim($csv_row[3]))) {
              $node->field_user_gender['und'][0]['value'] = lcfirst(trim($csv_row[3]));
            }
            $taxonomy = taxonomy_get_term_by_name(trim($csv_row[4]), 'user_types');
            if (!empty($taxonomy)) {
              $node->field_user_type['und'][0]['tid'] = array_pop($taxonomy)->tid;
            }
            node_save($node);
            $update[] = 'Updated';

          }
          else {
            drupal_set_message(t('could not update user from row @count because same mobile is not used', array('@count' => $count)), 'error');
          }
          break;
        case 'deactivate':
          $nid = user_details_get_user_details_nid_from_mobile_no(trim($csv_row[1]));
          if (is_numeric($nid)) {
            $node = node_load($nid);
            $node->status = 0;
            node_save($node);
            $deactivate[] = 'deactivate';
          }
          else {
            drupal_set_message(t('could not deactivate user from row @count because same mobile is not used', array('@count' => $count)), 'error');
          }
          break;
        default:
        case 'new':
          global $user;
          $nid = user_details_get_user_details_nid_from_mobile_no(trim($csv_row[1]));
          if (empty($nid)) {
            $node = new stdClass();  // Create a new node object
            $node->type = 'user_details';  // Content type
            $node->language = LANGUAGE_NONE;  // Or e.g. 'en' if locale is enabled
            node_object_prepare($node);  //Set some default values

            $node->title = 'Your node title';
            $node->status = 1;   // (1 or 0): published or unpublished
            // Add author of the node
            $node->uid = $user->uid;

            if (!empty(trim($csv_row[0]))) {
              $node->field_user_name['und'][0]['value'] = trim($csv_row[0]);
            }
            if (!empty(trim($csv_row[1]))) {
              $node->field_user_mobile_no['und'][0]['value'] = trim($csv_row[1]);
            }
            if (!empty(trim($csv_row[2]))) {
              $node->field_user_email_id['und'][0]['value'] = trim($csv_row[2]);
              $node->title = trim($csv_row[2]);
            }
            if (!empty(trim($csv_row[3]))) {
              $node->field_user_gender['und'][0]['value'] = lcfirst(trim($csv_row[3]));
            }
            $taxonomy = taxonomy_get_term_by_name(trim($csv_row[4]), 'user_types');
            if (!empty($taxonomy)) {
              $node->field_user_type['und'][0]['tid'] = array_pop($taxonomy)->tid;
            }
            $node->field_user_date_of_birth['und']['0']['value'] = time();

            // Save the node
            node_save($node);
            $new[] = 'new';
          }
          else {
            drupal_set_message(t('could not create new user form row @count because same mobile is used', array('@count' => $count)), 'error');
          }
          break;
      }
    }
    $count++;
  }
  fclose($file);
  drupal_set_message(t('successfully created @new_count new users, updated @update_count users and deactivated @deactivate_count users', array('@new_count' => count($new), '@update_count' => count($update), '@deactivate_count' => count($deactivate)))); 
}