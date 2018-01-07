<?php
/**
 * @class
 * Purpose: Provide a table summary of data stored in a materialized view.
 *   This is a generic, configurable fields to make it easier to add summaries
 *   to Tripal Content pages.
 *
 * Display:
 *   Draws a table summarizing the data.
 * Configuration:
 *   None yet available.
 */
class local__single_series_chart_formatter extends TripalFieldFormatter {

  // The default lable for this field.
  public static $default_label = 'Single-Series Table';

  // The list of field types for which this formatter is appropriate.
  public static $field_types = array('local__single_series_chart');

  // The list of default settings for this formatter.
  public static $default_settings = array(
    'title' => 'Table',
    'legend' => 'Lorem ipsum dolor sit amet, consectetur adipiscing elit. Sed scelerisque facilisis fringilla. Donec id dui dapibus, tincidunt dui vitae, tincidunt lorem. Suspendisse lorem enim, bibendum id varius vel, fermentum non orci. ',

  );

  /**
   * Provides the field's setting form.
   *
   * This function corresponds to the hook_field_formatter_settings_form()
   * function of the Drupal Field API.
   *
   * The settings form appears on the 'Manage Display' page of the content
   * type administration page. This function provides the form that will
   * appear on that page.
   *
   * To add a validate function, please create a static function in the
   * implementing class, and indicate that this function should be used
   * in the form array that is returned by this function.
   *
   * This form will not be displayed if the formatter_settings_summary()
   * function does not return anything.
   *
   * param $field
   *   The field structure being configured.
   * param $instance
   *   The instance structure being configured.
   * param $view_mode
   *   The view mode being configured.
   * param $form
   *   The (entire) configuration form array, which will usually have no use
   *   here.  Typically for reference only.
   * param $form_state
   *   The form state of the (entire) configuration form.
   *
   * @return
   *   A Drupal Form array containing the settings form for this field.
   */
  public function settingsForm($view_mode, $form, &$form_state) {
    $element = array();

    $element['title'] = array(
      '#type' => 'textfield',
      '#title' => 'Figure Title',
      '#default_value' => self::$default_settings['title'],
      '#description' => 'A breif title for the figure.',
    );

    $element['legend'] = array(
      '#type' => 'textarea',
      '#title' => 'Figure Legend',
      '#default_value' => self::$default_settings['legend'],
      '#description' => 'Should describe the figure to the user.',
    );

    return $element;
  }

  /**
   *  Provides the display for a field
   *
   * This function corresponds to the hook_field_formatter_view()
   * function of the Drupal Field API.
   *
   *  This function provides the display for a field when it is viewed on
   *  the web page.  The content returned by the formatter should only include
   *  what is present in the $items[$delta]['values] array. This way, the
   *  contents that are displayed on the page, via webservices and downloaded
   *  into a CSV file will always be identical.  The view need not show all
   *  of the data in the 'values' array.
   *
   *  @param $element
   *  @param $entity_type
   *  @param $entity
   *  @param $langcode
   *  @param $items
   *  @param $display
   *
   *  @return
   *    An element array compatible with that returned by the
   *    hook_field_formatter_view() function.
   */
  public function view(&$element, $entity_type, $entity, $langcode, $items, $display) {

    // Grab the name of the field to create a unique ID for the chart.
    $field_name = $this->instance['field_name'];

    // Get the settings
    $settings = $display['settings'];
    // Fill in defaults for any settings not set.
    foreach (self::$default_settings as $key => $value) {
      if (!isset($settings[$key])) {
        $settings[$key] = $value;
      }
    }

    if (isset($items[0]) && !empty($items[0]['value'])) {

      // Load the Tripal D3 API
      tripald3_load_libraries();

      // Format the data.
      $rows = array();
      foreach ($items as $key => $value) {
        foreach ($value['value']['hydra:member'] as $part) {
          $rows[] = array(
            'label' => $part['rdfs:label'],
            'count' => number_format($part['local:count']),
          );
        }
      }

      // Finally, add the markup.
      $element[0] = array(
        '#type' => 'markup',
        '#theme' => 'table',
        '#header' => array('label' => 'Category', 'count' => 'Amount'),
        '#rows' => $rows,
        '#attributes' => array(
          'class' => array('tripald3-chart'),
        ),
        '#caption' => '<span class="tripald3-legend">'
          . '<span class="tripald3-title">Table: ' . $settings['title'] . '.</span> '
          . '<span class="tripald3-desc">' . $settings['legend'] . '</span>'
          . '</span>',
      );

    }
    else {
      $element[0] = array(
        '#type' => 'markup',
        '#markup' => 'No Data.'
      );
    }
  }

  /**
   * Provides a summary of the formatter settings.
   *
   * This function corresponds to the hook_field_formatter_settings_summary()
   * function of the Drupal Field API.
   *
   * On the 'Manage Display' page of the content type administration page,
   * fields are allowed to provide a settings form.  This settings form can
   * be used to allow the site admin to define how the field should be
   * formatted.  The settings are then available for the formatter()
   * function of this class.  This function provides a text-based description
   * of the settings for the site developer to see.  It appears on the manage
   * display page inline with the field.  A field must always return a
   * value in this function if the settings form gear button is to appear.
   *
   * See the hook_field_formatter_settings_summary() function for more
   * information.
   *
   * @param $field
   * @param $instance
   * @param $view_mode
   *
   * @return string
   *   A string that provides a very brief summary of the field settings
   *   to the user.
   *
   */
  public function settingsSummary($view_mode) {
    $settings = $this->instance['display'][$view_mode]['settings'];
    return '<strong>Title:</strong> ' . $settings['title'];
  }

}