<?php
// $Id$
/**
 * @file
 * Form builder for Delving Remote Search API settings.
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
    '#type' => 'vertical_tabs',
    '#title' => t('Results page display settings'),
    '#prefix' => t('<h2>Results Page Settings</h2>Configure page name, results layout style, field lengths and thumbnail display properties.')
  );

  // Page title
  $form['results-display']['display-page-title'] = array(
    '#type' => 'fieldset',
    '#title' => t('Page Name'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['results-display']['display-page-title']['delving_search_page_name'] = array(
    '#type' => 'textfield',
    '#title' => t('Page Name'),
    '#default_value' => variable_get('delving_search_page_name', DELVING_SEARCH_PAGE_NAME),
    '#required' => TRUE,
    '#description' => t('The name for the Delving Remote Search page. This is the name that will appear in the menu and as the page title.<br/> You will need to clear the Drupal cache for this change to be visible.')
  );

  // Page layout style grid / list
  $form['results-display']['display-layout-page'] = array(
    '#type' => 'fieldset',
    '#title' => t('Results Layout'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['results-display']['display-layout-page']['delving_search_results_display_style'] = array(
    '#type' => 'radios',
    '#title' => t('Show the initials results as a list or grid'),
    '#default_value' => variable_get('delving_search_results_display_style', DELVING_SEARCH_RESULTS_DISPLAY_STYLE),
    '#options' => $options_dl,
  );

  // Field lengths
  $form['results-display']['display-layout-fields'] = array(
    '#type' => 'fieldset',
    '#title' => t('Field Lengths'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,

  );
  // Title field
  $form['results-display']['display-layout-fields']['delving_search_results_title_length'] = array(
    '#type' => 'textfield',
    '#title' => t('Maximum length of the title field.'),
    '#description' => t('The number of characters or words visible for the title field in the returned results list. Leave blank for full title.'),
    '#size' => 4,
    '#maxlength' => 3,
    '#default_value' => variable_get('delving_search_results_title_length', DELVING_SEARCH_RESULTS_TITLE_LENGTH),
    ''
  );
  // Description field
  $form['results-display']['display-layout-fields']['delving_search_results_description_length'] = array(
    '#type' => 'textfield',
    '#title' => t('Maximum length of the description field.'),
    '#description' => t('The number of characters visible for the description field in the returned results list. Leave blank for full description.'),
    '#size' => 4,
    '#maxlength' => 3,
    '#default_value' => variable_get('delving_search_results_description_length', DELVING_SEARCH_RESULTS_DESCRIPTION_LENGTH),
  );

  // Thumbnails settings
  $form['results-display']['display-layout-thumbnails'] = array(
    '#type' => 'fieldset',
    '#title' => t('Thumbnail Display'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  // Thumbnail width
  $form['results-display']['display-layout-thumbnails']['delving_search_thumb_width'] = array(
    '#type' => 'textfield',
    '#title' => t('Thumbnail Width'),
    '#description' => t('The WIDTH (in pixels) of the thumbnail image displayed in the results.'),
    '#size' => 4,
    '#maxlength' => 3,
    '#default_value' => variable_get('delving_search_thumb_width', DELVING_SEARCH_THUMB_WIDTH),
  );
  // Thumbnail container height
  $form['results-display']['display-layout-thumbnails']['delving_search_thumb_viewer_height'] = array(
    '#type' => 'textfield',
    '#title' => t('Thumbnail Container Height'),
    '#description' => t('The HEIGHT (in pixels) of the thumbnail viewer container. This is a wrapper around the thumbnail acting as a window with a set height'),
    '#size' => 4,
    '#maxlength' => 3,
    '#default_value' => variable_get('delving_search_thumb_viewer_height', DELVING_SEARCH_THUMB_VIEWER_HEIGHT),
  );

  /**
   * RESULTS FACETS DISPLAY SETTINGS ***********************************************************************************
   */
  $form['results-display']['facets-display'] = array(
    '#type' => 'fieldset',
    '#title' => 'Facets Display',
  );

  $form['results-display']['facets-display']['#description'] = '<p>' . t('The display of the Facets blocks ("Refine Search") can be configured on the <strong class="drupal_link_admin_settings"><a href="' . url('admin/structure/block') . '">Blocks Configuration Page</a></strong>.') . '</p>';

  /**
   *  OBJECT / ITEM PAGE SETTINGS **************************************************************************************
   */
  $form['item-page'] = array(
    '#type' => 'vertical_tabs',
    '#prefix' => '<h2>' . t('Item Page Settings') . '</h2>',
  );

  // Where the object is viewed

  $form['item-page']['location'] = array(
    '#type' => 'fieldset',
    '#title' => 'Item Page Location',
  );
  $form['item-page']['location']['delving_search_item_view_path'] = array(
    '#type' => 'select',
    '#title' => t('Choose where you would like to view the item page'),
    '#description' => t('All links on the titles of the returned results will point to this setting.'),
    '#default_value' => variable_get('delving_search_item_view_path', DELVING_SEARCH_ITEM_VIEW_PATH),
    '#options' => array(
      'drupal' => t('Within drupal'),
      'isShownAt' => t('In original context from the Data Provider'),
      'delvingPlatform' => t('A Delving Platform portal'),
    ),
    '#ajax' => array(
      'callback' => 'delving_show_object_page_input',
      'wrapper' => 'delving_object_page'
    )
  );
  // Empty placeholder for ajax fill
  $form['item-page']['location']['delving_search_item_object_page'] = array(
    '#type' => 'markup',
    '#prefix' => '<div id="delving_object_page">',
    '#suffix' => '</div>'
  );
  // If object viewed in remote delving portal, then show this form field
  if (isset($form_state['values']['delving_search_item_view_path']) && $form_state['values']['delving_search_item_view_path'] == 'delvingPlatform') {
    $form['item-page']['delving_search_item_object_page']['delving_search_item_external_url'] = array(
      '#type' => 'textfield',
      '#title' => t('External URL'),
      '#description' => t('If you chose "A Delving Platform compliant portal" above, then enter the URL here. For example "http://example.com/portal/object/"'),
      '#default_value' => variable_get('delving_search_item_external_url', DELVING_SEARCH_ITEM_EXTERNAL_URL)
    );
  }

  // Text string for 'return to results' hyperlink
  $form['item-page']['return-to-results'] = array(
    '#type' => 'fieldset',
    '#title' => t('Return to Results'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['item-page']['return-to-results']['delving_search_page_return'] = array(
    '#type' => 'textfield',
    '#title' => 'Hyperlink text',
    '#default_value' => variable_get('delving_search_page_return', DELVING_SEARCH_ITEM_PAGE_RETURN_TO_RESULTS),
    '#required' => TRUE,
    '#description' => t('The text which appears in the link back to results page')
  );

  // Show original context field
  $form['item-page']['original-context-options'] = array(
    '#type' => 'fieldset',
    '#title' => 'Original Context Field',
    '#description' => t('If a record contains a field with a URI to the object in it\'s original context, choose here what you what like to do with this URI'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['item-page']['original-context-options']['delving_search_item_show_in_original_context'] = array(
    '#type' => 'radios',
    '#title' => t('Show "Original Context" field afdadfasfsdf'),
    '#default_value' => variable_get('delving_search_item_show_in_original_context', DELVING_SEARCH_ITEM_SHOW_IN_ORIGINAL_CONTEXT),
    '#options' => $options_oc,
    '#description' => t('By choosing "show" a link will be created (if the field is available) which will open the object in a new window in it\'s original context.')
  );

    // Hyperlink styles
  $form['item-page']['display-layout-hyperlinks'] = array(
    '#type' => 'fieldset',
    '#title' => t('Hyperlink Styles'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['item-page']['display-layout-hyperlinks']['delving_search_item_hyperlink_style'] = array(
    '#type' => 'radios',
    '#title' => t('Render a hyperlink as a button or as a plain hyperlink'),
    '#default_value' => variable_get('delving_search_item_hyperlink_button', DELVING_SEARCH_ITEM_HYPERLINK_BUTTON),
    '#options' => $options_hyperlink_button,
  );

  // Thumbnails settings
  $form['item-page']['object-image-width'] = array(
    '#type' => 'fieldset',
    '#title' => t('Object Image'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  // Thumbnail width
  $form['item-page']['object-image-width']['delving_search_object_img_width'] = array(
    '#type' => 'textfield',
    '#title' => t('Image width'),
    '#description' => t('The WIDTH (in pixels) of the image displayed on the object page.'),
    '#size' => 4,
    '#default_value' => variable_get('delving_search_object_img_width', DELVING_SEARCH_OBJECT_IMG_WIDTH),
  );
  //Social media widgets
  $form['item-page']['social'] = array(
    '#type' => 'fieldset',
    '#title' => 'Share on social media networks',
  );
  $form['item-page']['social']['delving_search_social_widget_display'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable social media widget'),
    '#description' => t('Enable social media widget to share items on Facebook, twitter etc.'),
    '#options' => array(
      '1' => t('Enabled'),
      '0' => t('Disabled'),
    ),
    '#default_value' => variable_get('delving_search_social_widget_display', DELVING_SEARCH_SOCIAL_WIDGET_DISPLAY),
  );
  $form['item-page']['social']['delving_search_social_widget_type'] = array(
    '#type' => 'radios',
    '#title' => t('Select which widget type to use'),
    '#options' => array(
      '1' => '<img src="' . base_path() . drupal_get_path('module', 'delvingsearch') . '/img/widget1.png">',
      '2' => '<img src="' . base_path() . drupal_get_path('module', 'delvingsearch') . '/img/widget2.png">',
      '3' => '<img src="' . base_path() . drupal_get_path('module', 'delvingsearch') . '/img/widget3.png">',
    ),
    '#default_value' => variable_get('delving_search_social_widget_type', DELVING_SEARCH_SOCIAL_WIDGET_TYPE),
  );

  $form['wiki'] = array(
    '#type' => 'vertical_tabs',
    '#title' => 'Extra Search Result Blocks from Wikipedia or similar targets',
    '#prefix' => t('<h2>Extra Search Result Blocks from Wikipedia or similar targets</h2><p>Add API and TARGET URL\'s for up to 3 Wikipedia (or similar) targets<br/>
      The display of the Wikipedia results blocks can be configured on the <strong class="drupal_link_admin_settings"><a href="' . url('admin/structure/block') . '">Blocks Configuration Page</a></strong>.</p>'),
  );
  for ($i = 1; $i < 4; $i++) {
    $wiki_nr = $i;
    $form['wiki']['wiki-api'][$i] = array(
      '#type' => 'fieldset',
      '#title' => t('Wiki ' . $i ),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
    );
    $form['wiki']['wiki-api'][$i]['delving_search_wiki_api_url_'.$i] = array(
      '#type' => 'textfield',
      '#title' => t("Wiki @wiki_nr API URL", array('@wiki_nr' => $i)),
      '#description' => t('<p>The base URL of the wikipedia api URL (<strong>no trailing slash</strong>). Eg: http://en.wikipedia.org/w/api.php, http://no.wikipedia.org/w/api.php, http://lokalhistoriewiki.no/api.php</p>'),
      '#default_value' => variable_get('delving_search_wiki_api_url_'.$i),
    );
    $form['wiki']['wiki-api'][$i]['delving_search_wiki_target_url_'.$i] = array(
      '#type' => 'textfield',
      '#title' => t('Wiki @wiki_nr TARGET URL', array('@wiki_nr' => $i)),
      '#description' => t('<p>The base URL of the wikipedia target URL (<strong>with trailing slash</strong>). Eg: http://en.wikipedia.org/wiki/, http://no.wikipedia.org/wiki/, http://lokalhistoriewiki.no/index.php/</p>'),
      '#default_value' => variable_get('delving_search_wiki_target_url_'.$i),
    );
  }

  $form['#validate'][] = 'delving_search_validate_numeric';
  return system_settings_form($form);
}

/**
 * Callback element needs only select the portion of the form to be updated.
 * Since #ajax['callback'] return can be HTML or a renderable array (or an
 * array of commands), we can just return a piece of the form.
 */
function delving_show_object_page_input($form, $form_state)
{
  return $form['item-page']['delving_search_item_object_page']['delving_search_item_external_url'];
}

function delving_search_validate_numeric($form, &$form_state){
  $length_title = &$form_state['values']['delving_search_results_title_length'];
  $length_description = &$form_state['values']['delving_search_results_description_length'];
  $thumb_width = &$form_state['values']['delving_search_thumb_width'];
  $thumb_viewer_width = &$form_state['values']['delving_search_thumb_viewer_height'];
  $object_img_width = &$form_state['values']['delving_search_object_img_width'];

  if(trim($length_title)!= '' && !is_numeric($length_title)){
    form_set_error('delving_search_results_title_length', t('Field Lengths: Title field length must be numeric'));
  }
  if(trim($length_description)!= '' && !is_numeric($length_description)){
    form_set_error('delving_search_results_description_length', t('Field Lengths: Description field length must be numeric'));
  }
  if(trim($thumb_width)!= '' && !is_numeric($thumb_width)){
    form_set_error('delving_search_thumb_width', t('Thumbnail Display: Thumbnail width must be numeric'));
  }
  if(trim($thumb_viewer_width)!= '' && !is_numeric($thumb_viewer_width)){
    form_set_error('delving_search_thumb_viewer_height', t('Thumbnail Display: Thumbnail viewer container height must be numeric'));
  }
  if(trim($object_img_width)!= '' && !is_numeric($object_img_width)){
    form_set_error('delving_search_object_img_width', t('Object Image: Object image width must be numeric'));
  }
}