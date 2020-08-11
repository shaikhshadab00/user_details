<?php

/**
 * @file
 * user_details_db_creation.features.taxonomy.inc
 */

/**
 * Implements hook_taxonomy_default_vocabularies().
 */
function user_details_db_creation_taxonomy_default_vocabularies() {
  return array(
    'user_types' => array(
      'name' => 'User Types',
      'machine_name' => 'user_types',
      'description' => 'Different types of user',
      'hierarchy' => 0,
      'module' => 'taxonomy',
      'weight' => 0,
      'rdf_mapping' => array(
        'rdftype' => array(
          0 => 'skos:ConceptScheme',
        ),
        'name' => array(
          'predicates' => array(
            0 => 'dc:title',
          ),
        ),
        'description' => array(
          'predicates' => array(
            0 => 'rdfs:comment',
          ),
        ),
      ),
    ),
  );
}
