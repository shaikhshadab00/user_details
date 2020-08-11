<?php

/**
 * @file
 * User details dashboard download.
 */

/**
 * Callback function to download csv
 */
function user_details_dashboard_download_csv() {
  $header = variable_get('user_details_dashboard_header', array());
  $rows = variable_get('user_details_dashboard_row', array());

  //add necessary headers for browsers
  drupal_add_http_header('Content-Type', 'text/csv; utf-8');
  drupal_add_http_header('Content-Disposition', 'attachment; filename = user_details.csv');

  //instead of writing down to a file we write to the output stream
  $fh = fopen('php://output', 'w');

  if (!empty($header) && !empty($rows)) {
    $csv_header = array();
    foreach ($header as $header_data) {
      $csv_header[] = $header_data['data'];
    }

    //form header
    fputcsv($fh, $csv_header);

    //write data in the CSV format
    foreach ($rows as $row) {
      fputcsv($fh, $row);
    }
  } else {
    fputcsv($fh, array(t('Data is empty')));
  }

  //close the stream
  fclose($fh);
}