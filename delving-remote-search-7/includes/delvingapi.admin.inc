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

function delvingapi_admin_settings($form, &$form_state)
{

  $options_do = array('1' => t('true'), '0' => t('false'));
  $options_language = array('en' => 'English', 'no' => 'Norsk', 'nb' => 'Bokmal');
  /**
   * API SETTINGS
   */
  $form['api-settings'] = array(
    '#type' => 'fieldset',
    '#title' => 'Delving Remote Search API settings',
  );
  $form['api-settings']['delving_api_remote_url'] = array(
    '#type' => 'textfield',
    '#title' => t('API URL'),
    '#description' => t('URL to send search request to. For example: <a href="@api_url">@api_url</a>', array('@api_url' => url(DELVING_API_REMOTE_URL),)),
    '#default_value' => variable_get('delving_api_remote_url', DELVING_API_REMOTE_URL),
    '#required' => TRUE,
  );
  $form['api-settings']['delving_api_language'] = array(
    '#type' => 'select',
    '#title' => t('Environment Language'),
    '#description' => t('All metadata field names will be returned in this language.'),
    '#options' => $options_language,
    '#default_value' => variable_get('delving_api_language', DELVING_API_LANGUAGE),
  );
  $form['api-settings']['delving_api_results_nr_items'] = array(
    '#type' => 'textfield',
    '#title' => t('Nr. Results'),
    '#size' => 2,
    '#default_value' => variable_get('delving_api_results_nr_items', DELVING_API_RESULTS_NR_ITEMS),
    '#description' => 'Number of items to return per query.',
    '#required' => TRUE,
  );
  /**
   * SEARCH CONSTRAINTS
   */
  $form['search-constraints'] = array(
    '#type' => 'fieldset',
    '#title' => 'Search Constraints',

  );
  $form['search-constraints']['delving_api_digital_object'] = array(
    '#type' => 'radios',
    '#title' => t('Digital Objects Only'),
    '#default_value' => variable_get('delving_api_digital_object', DELVING_API_DIGITAL_OBJECT),
    '#options' => $options_do,
    '#description' => 'Setting this to true will drastically increase the probability that returned results contain a thumbnail image, though the total number of returned results will decrease since this is a query constraint.',
  );

  $form['search-constraints']['delving_api_hqfs'] = array(
    '#title' => t('Custom Constraints'),
    '#description' => t(
      'Enter a comma separated list of facets by which you wish to limit all searches.<br/>For example "COUNTY:Oslo,MUNICIPALITY:Majorstuen".
      When using values with multiple words please surround with double quotes, e.g. PROVIDER:"Sunnfjord Museum".<br/>
      If you are not sure which facets can be used for this operation then check with your Delving Platform administrator.'),
    '#type' => 'textfield',
    '#default_value' => variable_get('delving_api_hqfs', DELVING_API_HQFS),
    '#required' => FALSE,
  );

  $form['#validate'][] = 'delving_api_validate_numeric';

  return system_settings_form($form);
}

function delving_api_validate_numeric($form, &$form_state){
  $length_title = &$form_state['values']['delving_api_results_nr_items'];

  if(trim($length_title)!= '' && !is_numeric($length_title)){
    form_set_error('delving_api_results_nr_items', t('Nr. Results: field length must be numeric'));
  }
}