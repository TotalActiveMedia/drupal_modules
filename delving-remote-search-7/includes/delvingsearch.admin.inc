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

  /**
   * RESULTS DISPLAY SETTINGS
   */
  $form['results-display'] = array(
    '#type' => 'fieldset',
    '#title' => 'Results display settings',
  );
  $form['results-display']['delving_search_results_display_style'] = array(
    '#type' => 'radios',
    '#title' => t('Show the initials results as a list or gird'),
    '#default_value' => variable_get('delving_search_results_display_style', DELVING_SEARCH_RESULTS_DISPLAY_STYLE),
    '#options' => $options_dl,
  );
  $form['results-display']['delving_search_results_title_length'] = array(
    '#type' => 'textfield',
    '#title' => t('Maximum length of the title field.'),
    '#description' => t('The number of characters visible for the title field in the returned results list. Leave blank for full description.'),
    '#size' => 4,
    '#default_value' => variable_get('delving_search_results_title_length', DELVING_SEARCH_RESULTS_TITLE_LENGTH),
  );
  $form['results-display']['delving_search_results_description_length'] = array(
    '#type' => 'textfield',
    '#title' => t('Maximum length of the description field.'),
    '#description' => t('The number of characters visible for the description field in the returned results list. Leave blank for full description.'),
    '#size' => 4,
    '#default_value' => variable_get('delving_search_results_description_length', DELVING_SEARCH_RESULTS_DESCRIPTION_LENGTH),
  );
  $form['results-display']['delving_search_thumb_width'] = array(
    '#type' => 'textfield',
    '#title' => t('Thumbnail width'),
    '#description' => t('The WIDTH (in pixels) of the thumbnail image displayed in the results.'),
    '#size' => 4,
    '#default_value' => variable_get('delving_search_thumb_width', DELVING_SEARCH_THUMB_WIDTH),
  );
  $form['results-display']['delving_search_thumb_viewer_height'] = array(
    '#type' => 'textfield',
    '#title' => t('Thumbnail viewer container height'),
    '#description' => t('The HEIGHT (in pixels) of the thumbnail viewer container. This is a wrapper around the thumbnail acting as a window with a set height'),
    '#size' => 4,
    '#default_value' => variable_get('delving_search_thumb_viewer_height', DELVING_SEARCH_THUMB_VIEWER_HEIGHT),
  );
  /**
   * FACETS DISPLAY SETTINGS
   */
  $form['facets-display'] = array(
    '#type' => 'fieldset',
    '#title' => 'Facet display settings',
  );

 $form['facets-display']['#description'] = '<p>' . t('The display of the Facets blocks ("Refine Search") can be configured on the <strong class="drupal_link_admin_settings"><a href="' . url('admin/structure/block') . '">Blocks Configuration Page</a></strong>.') . '</p>';
  /**
   * PAGE DISPLAY SETTINGS
   */
  $form['search-display-settings'] = array(
    '#type' => 'fieldset',
    '#title' => 'Page Display Settings',
  );
  $form['search-display-settings']['delving_search_page_name'] = array(
    '#type' => 'textfield',
    '#title' => 'Search-page name value',
    '#default_value' => variable_get('delving_search_page_name', DELVING_SEARCH_PAGE_NAME),
    '#required' => TRUE,
    '#description' => t('The name for the Delving-Remote-Search page. This is the name that will show in the menu and the top of the search page')
  );
  $form['search-display-settings']['delving_search_page_return'] = array(
    '#type' => 'textfield',
    '#title' => 'Item page link text',
    '#default_value' => variable_get('delving_search_page_return', DELVING_SEARCH_PAGE_RETURN),
    '#required' => TRUE,
    '#description' => t('The text which appears in the link back to results page')
  );
  /**
   *  OBJECT / ITEM PAGE SETTINGS
   */
  $form['search-item-view-settings'] = array(
    '#type' => 'fieldset',
    '#title' => 'Object Display Settings',
  );
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
  );
  $form['search-item-view-settings']['delving_search_item_external_url'] = array(
    '#type' => 'textfield',
    '#title' => t('External URL'),
    '#description' => t('If you chose "A Delving Platform compliant portal" above, then enter the URL here. For example "http://norvegiana2.delving.org/portal/object/"'),
    '#default_value' => variable_get('delving_search_item_external_url', DELVING_SEARCH_ITEM_EXTERNAL_URL)
  );



  return system_settings_form($form);
}