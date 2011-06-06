<?php
// $Id$
/**
 * @file
 * Form builder for Delving Remote-Search API settings.
 *
 * @ingroup forms
 * @see system_settings_form()
 */

header('Content-Type: text/html; charset=utf-8');

function delvingsearch_admin_settings($form, &$form_state)
{

  $options_dl = array('grid' => t('grid'), 'list' => t('list'));
  $options_hyperlink_button = array(1 => t('Button'), 0 => t('Regular hyperlink'));
  $options_oc = array(1 => t('show'), 0 => t('hide'));
  /**
   * RESULTS PAGE DISPLAY SETTINGS *************************************************************************************
   */
  $form['results-display'] = array(
    '#type' => 'fieldset',
    '#title' => 'Results page display settings',
    '#description' => t('Configure page name, results layout style, field lengths and thumbnail display properties.')
  );

  // Page title
  $form['results-display']['display-page-title'] = array(
    '#type' => 'fieldset',
    '#title' => t('Page title'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['results-display']['display-page-title']['delving_search_page_name'] = array(
    '#type' => 'textfield',
    '#title' => 'Search-page name value',
    '#default_value' => variable_get('delving_search_page_name', DELVING_SEARCH_PAGE_NAME),
    '#required' => TRUE,
    '#description' => t('The name for the Delving-Remote-Search page. This is the name that will appear in the menu and as the page title.<br/> You will need to clear the Drupal cache for this change to be visible.')
  );

  // Page layout style grid / list
  $form['results-display']['display-layout-page'] = array(
    '#type' => 'fieldset',
    '#title' => t('Results layout'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['results-display']['display-layout-page']['delving_search_results_display_style'] = array(
    '#type' => 'radios',
    '#title' => t('Show the initials results as a list or gird'),
    '#default_value' => variable_get('delving_search_results_display_style', DELVING_SEARCH_RESULTS_DISPLAY_STYLE),
    '#options' => $options_dl,
  );

  // Field lengths
  $form['results-display']['display-layout-fields'] = array(
    '#type' => 'fieldset',
    '#title' => t('Field lengths'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  // Title field
  $form['results-display']['display-layout-fields']['delving_search_results_title_length'] = array(
    '#type' => 'textfield',
    '#title' => t('Maximum length of the title field.'),
    '#description' => t('The number of characters or words visible for the title field in the returned results list. Leave blank for full title.'),
    '#size' => 4,
    '#default_value' => variable_get('delving_search_results_title_length', DELVING_SEARCH_RESULTS_TITLE_LENGTH),
  );
  // Description field
  $form['results-display']['display-layout-fields']['delving_search_results_description_length'] = array(
    '#type' => 'textfield',
    '#title' => t('Maximum length of the description field.'),
    '#description' => t('The number of characters visible for the description field in the returned results list. Leave blank for full description.'),
    '#size' => 4,
    '#default_value' => variable_get('delving_search_results_description_length', DELVING_SEARCH_RESULTS_DESCRIPTION_LENGTH),
  );

  // Thumbnails settings
  $form['results-display']['display-layout-thumbnails'] = array(
    '#type' => 'fieldset',
    '#title' => t('Thumbnail display'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  // Thumbnail width
  $form['results-display']['display-layout-thumbnails']['delving_search_thumb_width'] = array(
    '#type' => 'textfield',
    '#title' => t('Thumbnail width'),
    '#description' => t('The WIDTH (in pixels) of the thumbnail image displayed in the results.'),
    '#size' => 4,
    '#default_value' => variable_get('delving_search_thumb_width', DELVING_SEARCH_THUMB_WIDTH),
  );
  // Thumbnail container height
  $form['results-display']['display-layout-thumbnails']['delving_search_thumb_viewer_height'] = array(
    '#type' => 'textfield',
    '#title' => t('Thumbnail viewer container height'),
    '#description' => t('The HEIGHT (in pixels) of the thumbnail viewer container. This is a wrapper around the thumbnail acting as a window with a set height'),
    '#size' => 4,
    '#default_value' => variable_get('delving_search_thumb_viewer_height', DELVING_SEARCH_THUMB_VIEWER_HEIGHT),
  );

  /**
   * RESULTS FACETS DISPLAY SETTINGS ***********************************************************************************
   */
  $form['facets-display'] = array(
    '#type' => 'fieldset',
    '#title' => 'Facet display settings',
  );

  $form['facets-display']['#description'] = '<p>' . t('The display of the Facets blocks ("Refine Search") can be configured on the <strong class="drupal_link_admin_settings"><a href="' . url('admin/structure/block') . '">Blocks Configuration Page</a></strong>.') . '</p>';

  /**
   *  OBJECT / ITEM PAGE SETTINGS **************************************************************************************
   */
  $form['search-item-view-settings'] = array(
    '#type' => 'fieldset',
    '#title' => 'Item Page Display Settings',
  );

  // Where the object is viewed
  $form['search-item-view-settings']['delving_search_item_view_path'] = array(
    '#type' => 'select',
    '#title' => t('Choose where you would like to view the object'),
    '#description' => t('All links on the titles of the returned results will point to this setting.'),
    '#default_value' => variable_get('delving_search_item_view_path', DELVING_SEARCH_ITEM_VIEW_PATH),
    '#options' => array(
      'drupal' => t('Within drupal'),
      'isShownAt' => t('In original context from the Dataprovider'),
      'delvingPlatform' => t('A Delving Platform compliant portal'),
    ),
    '#ajax' => array(
      'callback' => 'delving_show_object_page_input',
      'wrapper' => 'delving_object_page'
    )
  );
  // Empty placeholder for ajax fill
  $form['search-item-view-settings']['delving_search_item_object_page'] = array(
    '#type' => 'markup',
    '#prefix' => '<div id="delving_object_page">',
    '#suffix' => '</div>'
  );
  // If object viewed in remote delving portal, then show this form field
  if (isset($form_state['values']['delving_search_item_view_path']) && $form_state['values']['delving_search_item_view_path'] == 'delvingPlatform') {
    $form['search-item-view-settings']['delving_search_item_object_page']['delving_search_item_external_url'] = array(
      '#type' => 'textfield',
      '#title' => t('External URL'),
      '#description' => t('If you chose "A Delving Platform compliant portal" above, then enter the URL here. For example "http://norvegiana2.delving.org/portal/object/"'),
      '#default_value' => variable_get('delving_search_item_external_url', DELVING_SEARCH_ITEM_EXTERNAL_URL)
    );
  }

  // Text string for 'return to results' hyperlink
  $form['search-item-view-settings']['return-to-results'] = array(
    '#type' => 'fieldset',
    '#title' => t('"Return to results" hyperlink'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['search-item-view-settings']['return-to-results']['delving_search_page_return'] = array(
    '#type' => 'textfield',
    '#title' => 'Hyperlink text',
    '#default_value' => variable_get('delving_search_page_return', DELVING_SEARCH_ITEM_PAGE_RETURN_TO_RESULTS),
    '#required' => TRUE,
    '#description' => t('The text which appears in the link back to results page')
  );

  // Hyperlink styles
  $form['search-item-view-settings']['display-layout-hyperlinks'] = array(
    '#type' => 'fieldset',
    '#title' => t('Hyperlink styles'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['search-item-view-settings']['display-layout-hyperlinks']['delving_search_item_hyperlink_style'] = array(
    '#type' => 'radios',
    '#title' => t('Render a hyperlink as a button or as a plain hyperlink'),
    '#default_value' => variable_get('delving_search_item_hyperlink_button', DELVING_SEARCH_ITEM_HYPERLINK_BUTTON),
    '#options' => $options_hyperlink_button,
  );

  // Show original context field
  $form['search-item-view-settings']['original-context-options'] = array(
    '#type' => 'fieldset',
    '#title' => 'Original Context Field',
    '#description' => t('If a record contains a field with a URI to the object in it\'s original context, choose here what you what like to do with this URI'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['search-item-view-settings']['original-context-options']['delving_search_show_in_original_context'] = array(
    '#type' => 'radios',
    '#title' => t('Show "Original Context" field'),
    '#default_value' => variable_get('delving_search_item_show_in_original_context', DELVING_SEARCH_ITEM_SHOW_IN_ORIGINAL_CONTEXT),
    '#options' => $options_oc,
    '#description' => t('By choosing "show" a link will be created (if the field is available) which will open the object in a new window in it\'s original context.')
  );
  // Thumbnails settings
  $form['search-item-view-settings']['object-image-width'] = array(
    '#type' => 'fieldset',
    '#title' => t('Object Image'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  // Thumbnail width
  $form['search-item-view-settings']['object-image-width']['delving_search_object_img_width'] = array(
    '#type' => 'textfield',
    '#title' => t('Image width'),
    '#description' => t('The WIDTH (in pixels) of the image displayed on the object page.'),
    '#size' => 4,
    '#default_value' => variable_get('delving_search_object_img_width', DELVING_SEARCH_OBJECT_IMG_WIDTH),
  );

  $form['wikipedia'] = array(
    '#type' => 'fieldset',
    '#title' => 'Extra Search Results',
  );
  $form['wikipedia']['wikipedia-api'] = array(
    '#type' => 'fieldset',
    '#title' => t('Wikipedia Results'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['wikipedia']['wikipedia-api']['delving_search_wikipedia_api_url'] = array(
    '#type' => 'textfield',
    '#title' => t('Wikipedia URL'),
    '#description' => t('<p>The base url of the wikipedia url (no trailing slash). Eg: http://en.wikipedia.org, http://no.wikipedia.org, http://nl.wikipedia.org</p>
    <p>The display of the Wikipedia results block can be configured on the <strong class="drupal_link_admin_settings"><a href="' . url('admin/structure/block') . '">Blocks Configuration Page</a></strong>.</p>'),
    '#default_value' => variable_get('delving_search_wikipedia_api_url', DELVING_SEARCH_WIKIPEDIA_API_URL),
  );
  return system_settings_form($form);
}

/**
 * Callback element needs only select the portion of the form to be updated.
 * Since #ajax['callback'] return can be HTML or a renderable array (or an
 * array of commands), we can just return a piece of the form.
 */
function delving_show_object_page_input($form, $form_state)
{
  return $form['search-item-view-settings']['delving_search_item_object_page']['delving_search_item_external_url'];
}