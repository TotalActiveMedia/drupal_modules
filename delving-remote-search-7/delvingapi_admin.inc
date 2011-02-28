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

function delving_admin_settings($form, &$form_state) {

  $options_do = array('1' => t('true'), '0' => t('false'));
  $options_df = array('1' => t('true'), '0' => t('false'));
  $options_counties = array(
    '&#248;stfold' => '&#248;stfold',
    'akershus' => 'akershus',
    'Oslo' => 'Oslo',
    'hedmark' => 'hedmark',
    'oppland' => 'oppland',
    'buskerud' => 'buskerud',
    'vestfold' => 'vestfold',
    'telemark' => 'telemark',
    'aust-agder'=> 'aust-agder',
    'vest-agder' => 'vest-agder',
    'rogaland' => 'rogaland',
    'hordaland' => 'hordaland',
    'sogn og fjordane' => 'sogn og fjordane',
    'm&#248;re og romsdal' => 'm&#248;re og romsdal',
    's&#248;r-tr&#248;ndelag' => 's&#248;r-tr&#248;ndelag',
    'nord-tr&#248;ndelag' => 'nord-tr&#248;ndelag',
    'nordland' => 'nordland',
    'troms' => 'troms',
    'finnmark' => 'finnmark',
  );
 // '¿stfold','akershus','oslo','hedmark','oppland','buskerud','vestfold','telemark','aust-agder','vest-agder','rogaland','hordaland','sogn og fjordane','m¿re og romsdal','s¿r-tr¿ndelag','nord-tr¿ndelag','nordland','troms','finnmark'
  $form['search-display-settings'] = array(
    '#type' => 'fieldset',
    '#title' => 'Display Settings',
  );

// TODO: find a way to override/reset the 'delving_search_page_path' variable so that it is 'dynamic'
// This setting is used by the blocks 'page' setting to identify the page on which the blocks should be visible
// If this setting is changed here, in the admin section, then the blocks 'page' setting is not updated in the db
// The variable is set when the module is activated and if changes are made here, then they must also be made 'by hand'
// on each individual block's configuration page. That's just not very administrator friendly

//  $form['search-display-settings']['delving_search_page_path'] = array(
//    '#type' => 'textfield',
//    '#title' => 'Search-page path value',
//    '#default_value' => variable_get('delving_search_page_path', DELVING_SEARCH_PAGE_PATH),
//    '#required' => TRUE,
//    '#description' => t('The path for the Delving-Remote-Search page')
//  );

  $form['search-display-settings']['delving_search_page_name'] = array(
    '#type' => 'textfield',
    '#title' => 'Search-page name value',
    '#default_value' => variable_get('delving_search_page_name', DELVING_SEARCH_PAGE_NAME),
    '#required' => TRUE,
    '#description' => t('The name for the Delving-Remote-Search page. This is the name that will show in the menu and the top of the search page')
  );
  $form['search-display-settings']['#description'] = '<p>' . t('The display of the Facets blocks ("Refine Search") can be configured on the <strong class="drupal_link_admin_settings"><a href="' . url('admin/structure/block') . '">Blocks Configuration Page</a></strong>.') . '</p>';

  $form['api-settings'] = array(
    '#type' => 'fieldset',
    '#title' => 'Delving Remote-Search API settings',
  );
  $form['api-settings']['delving_api_remote_url'] = array(
    '#type' => 'textfield',
    '#title' => t('Delving Remote-search API URL'),
    '#description' => t('URL to send search request to. For example: <a href="@api_url">@api_url</a>', array(
      '@api_url' => url(DELVING_API_REMOTE_URL),
    )),
    '#default_value' => variable_get('delving_api_remote_url', DELVING_API_REMOTE_URL),
    '#required' => TRUE,
  );
  $form['api-settings']['delving_api_results_rows'] = array(
    '#type' => 'textfield',
    '#title' => t('Number of rows to return in search results'),
    '#size' => 2,
    '#default_value' => variable_get('delving_api_results_rows', DELVING_API_RESULTS_ROWS),
  );
  $form['search-constraints'] = array(
    '#type' => 'fieldset',
    '#title' => 'Search Constraints',

  );
  $form['search-constraints']['delving_api_digital_object'] = array(
    '#type' => 'radios',
    '#title' => t('Return only results with digital objects'),
    '#default_value' => variable_get('delving_api_digital_object', DELVING_API_DIGITAL_OBJECT),
    '#options' => $options_do,
  );
  $form['search-constraints']['delving_api_counties'] = array(
    '#title' => t('Limit results to these counties only'),
    '#description' => t('Enter a comma separted list of county names to which you wish to limit the search'),
    '#type' => 'textfield',
    '#default_value' => variable_get('delving_api_counties', DELVING_API_COUNTIES),
    '#required' => FALSE,
  );
//  $form['search-constraints']['delving_api_counties'] = array(
//    '#title' => t('Limit results to these counties only'),
//    '#description' => t('Leave ALL unchecked to return results from all counties'),
//    '#type' => 'checkboxes',
//    '#tree' => TRUE,
//    '#options' => $options_counties,
//    '#default_value' => variable_get('delving_api_counties', DELVING_API_COUNTIES),
//
//  );


  return system_settings_form($form);
}