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
  drupal_add_js(drupal_get_path('module', 'delvingsearch') . '/delvingsearch_admin.js');
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
  $form['results-display']['field-list'] = array(
    '#type' => 'fieldset',
    '#title' => t('Customize fields'),
    '#description' => t('Change these settings to manipulate the display order of the fields per object on the result page. This only applies for list view. Thumbnail, title and description is required and can not be changed or hidden through this interface. The number in parantheses show the number of records with data in this field.'),
  );
  $defaults = variable_get('delvingsearch_the_list',0);
  $lang = variable_get('delving_api_language', DELVING_API_LANGUAGE);
  $apiurl = variable_get('delving_api_remote_url', DELVING_API_REMOTE_URL);
  $xml = drupal_http_request($apiurl . '?explain&lang=' . $lang);
  $fields = array();
  $exclude = array(
      "europeana:object",
      "dc:title",
      "dc:description",
      "delving:pmhId",
      "europeana:isShownAt",
  );
  if ($xml->code == 200): {
    $data = simplexml_load_string($xml->data);
    foreach ($data->xpath("//fields/field") as $item) {
      $field = implode('', $item->xpath("@xml"));
      if (in_array($field, $exclude)): {
        //discard these from field list. They should be left alone anyway.
      }
      else: {
        $weight = '';
        if (!empty($defaults[$field]['weight'])) {
          $weight = $defaults[$field]['weight'];
        }
        $fields[$field] = $weight;
      }
      endif;
    }
  }
  endif;
  asort($fields);
  $form['results-display']['field-list']['delvingsearch_the_list'] = array(
    '#tree' => TRUE,
    '#theme' => 'delvingsearch_admin_settings',
  );
  foreach ($fields as $t=>$key) {
    foreach ($data->xpath("//field[@xml='" . $t . "']") as $f) {
      $search = implode('', $f->xpath("@search"));
      if (strpos($search, '_facet')<1) {
        $form['results-display']['field-list']['delvingsearch_the_list'][$t]['name'] = array(
          '#markup' => implode('',$f->xpath("@xml")) . ' (' . t('!count records', array('!count' => implode('',$f->xpath("@docs")))) . ')'
        );
        $form['results-display']['field-list']['delvingsearch_the_list'][$t]['hidden'] = array(
          '#type' => 'select',
          '#options' => array('hidden' => t('Hidden'), 'visible' => t('Visible')),
          '#default_value' => $defaults[$t]['hidden'],
        );
        $form['results-display']['field-list']['delvingsearch_the_list'][$t]['weight'] = array(
          '#type' => 'weight',
          '#default_value' => $defaults[$t]['weight'],
          '#delta' => 20,
        );
      }
    }
  }
  /**
   * RESULTS FACETS DISPLAY SETTINGS ***********************************************************************************
   */
  $form['results-display']['facets-display'] = array(
    '#type' => 'fieldset',
    '#title' => 'Facets Display',
  );

  $form['results-display']['facets-display']['#description'] = '<p>' . t('The display of the Facets blocks ("Refine Search") can be configured on the <strong class="drupal_link_admin_settings"><a href="' . url('admin/structure/block') . '">Blocks Configuration Page</a></strong>.') . '</p>';
  $form['results-display']['facets-display']['delvingsearch_sort_buttons'] = array(
    '#type' => 'checkbox',
    '#title' => t('Sort option for facet blocks'),
    '#options' => array(
      '1' => t('On'),
      '0' => t('Off'),
    ),
    '#default_value' => variable_get('delvingsearch_sort_buttons', 0),
  );
  $form['results-display']['facets-display']['delvingsearch_sortbox_number_limit'] = array(
    '#type' => 'textfield',
    '#title' => t('Number of hits to trigger sort buttons'),
    '#default_value' => variable_get('delvingsearch_sortbox_number_limit', 20),
    '#states' => array(
      'visible' => array(
        ':input[name="delvingsearch_sort_buttons"]' => array('checked' => TRUE),
      ),
    ),
  );
  $form['results-display']['simile'] = array(
    '#type' => 'fieldset',
    '#title' => 'Simile view',
  );
  $form['results-display']['simile']['delvingsearch_simile_view'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable Simile view on result lists'),
    '#options' => array(
      '1' => t('On'),
      '0' => t('Off'),
    ),
    '#default_value' => variable_get('delvingsearch_simile_view', 0),
    '#description' => t('<strong>NOTICE: </strong>This feature is customizable, and should not be enabled if you have not configured your Simile files accordingly.')
  );
  
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
    )
  );
  // If object viewed in remote delving portal, then show this form field
  $form['item-page']['location']['delving_search_item_external_url'] = array(
    '#type' => 'textfield',
    '#title' => t('External URL'),
    '#description' => t('If you chose "A Delving Platform compliant portal" above, then enter the URL here. For example "http://example.com/portal/object/"'),
    '#default_value' => variable_get('delving_search_item_external_url', DELVING_SEARCH_ITEM_EXTERNAL_URL),
    //#states show this immidiately with javascript instead of using ajax which is slower. Also, the ajax implemented earlier did not hide the field if you changed your mind.
    '#states' => array(
      'visible' => array(
        ':input[name="delving_search_item_view_path"]' => array('value' => 'delvingPlatform'),
      ),
    ),
  );

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
    '#title' => t('Show "Original Context" field'),
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
  $form['item-page']['social']['delving_search_addthis_pubid'] = array(
    '#type' => 'textfield',
    '#title' => t('addThis Profile ID (optional)'),
    '#description' => t('If you have an addThis profile, put your Profile ID here to track your visitors sharing. If you do not have a profile, you can either <a href="@addthis" target="_blank">register at addThis</a> or leave this blank. Your Profile ID is located under your addThis profile settings. Hover your username (top right) and click settings. Then click profiles, and your ID should show.', array('@addthis' => 'https://www.addthis.com/register')),
    '#default_value' => variable_get('delving_search_addthis_pubid', ''),
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
  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array('#type' => 'submit', '#value' => t('Save changes'));
  return system_settings_form($form);
}

/**
 * @param $variables
 *   An associative array containing:
 *   - form: A render element representing the form.
 *
 * @ingroup themeable
 */
function theme_delvingsearch_admin_settings($variables) {
  $form = $variables['form'];
  $test = array();
  $defaults = variable_get('delvingsearch_the_list',0);
  foreach (element_children($form) as $id) {
    $test[$id] = $defaults[$id]['weight'];
  }
  asort($test);
  drupal_add_tabledrag('delving-fields', 'order', 'sibling', 'delving-fields-weight');

  $rows = array();
  foreach (element_children($form) as $id) {
    $form[$id]['weight']['#attributes']['class'] = array('delving-fields-weight');
    $rows[] = array(
      'data' => array(
        drupal_render($form[$id]['name']),
        drupal_render($form[$id]['weight']),
        drupal_render($form[$id]['hidden']),
      ),
      'class' => array('draggable'),
    );
  }
  $header = array(t('Name'), t('Weight'), t('Visibilty'));
  $output = theme('table', array('header' => $header, 'rows' => $rows, 'attributes' => array('id' => 'delving-fields')));

  return $output;
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
