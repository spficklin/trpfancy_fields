<?php
/**
 * @file
 * Contains all field specific code outside the classes.
 */

/**
 * Implements hook_bundle_create_user_field().
 *
 * A priviledged user has the ability to add new fields to the bundle.
 * All Tripal Fancy Fields are allowed to be added dynamically by the user.
 * But, Drupal doesn't know how to deal with it, so this function is
 * called for any field attached to a TripalEntity bundle type.
 *
 * Specific to this implementation:
 * Any fields whose TripalField::$module argument is set to 'trpfancy_fields'
 * and that can be added dynamically will result in a call to this function.
 */
function trpfancy_fields_bundle_create_user_field($new_field, $bundle) {

  $field_name = $new_field['field_name'];
  $field_type = $new_field['type'];
  // @todo: make the description depend on the field type.
  // Currently there is only one field so this is fine.
  $description = 'A type of graph in which all the data is part of a single series. Specifically, the dataset consists of labels and corresponding values.';

  // Create the field.
  $field = array(
    'field_name' => $field_name,
    'type' => $field_type,
    'cardinality' => 1,
    'locked' => FALSE,
    'storage' => array(
      'type' => 'tripal_no_storage',
    ),
  );
  field_create_field($field);

  // Then create the instance.
  $instance =  array(
    'field_name' => $field_name,
    'entity_type' => $bundle->type,
    'bundle' => $bundle->name,
    'label' => $new_field['label'],
    'description' => $description,
    'required' => FALSE,
    'settings' => array(
      'auto_attach' => FALSE,
    ),
    'widget' => array(
      // @ASSUMPTION: Assumes there is a widget that matches the field type.
      'type' => $field_type . '_widget',
      'settings' => array(),
    ),
    'display' => array(
      'default' => array(
        'label' => 'hidden',
        // @ASSUMPTION: Assumes there is a formatter that matches the field type.
        'type' => $field_name . '_formatter',
        'settings' => array(),
      ),
    ),
  );
  field_create_instance($instance);

}