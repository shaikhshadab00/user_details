<?php

/**
 * @file
 * User details upload csv format.
 */

/**
 * Callback function for user_details_upload_csv_format
 */
function user_details_upload_csv_format() {
  //add necessary headers for browsers
  drupal_add_http_header('Content-Type', 'text/csv; utf-8');
  drupal_add_http_header('Content-Disposition', 'attachment; filename = import_user.csv');

  //instead of writing down to a file we write to the output stream
  $fh = fopen('php://output', 'w');
  //form header
  fputcsv($fh, array('Name', 'Mobile', 'Email ID', 'Gender', 'User Type', 'Status'));

  //close the stream
  fclose($fh);

}
