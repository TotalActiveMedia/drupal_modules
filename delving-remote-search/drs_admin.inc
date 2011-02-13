<?php
// $Id$
/**
 * @file
 * Form builder for Delving Remote-Search API settings.
 *
 * @ingroup forms
 * @see system_settings_form()
 */
function drs_admin_settings() {

    $options_do = array('1' => t('true'), '0' => t('false'));
    $options_df = array('1' => t('true'), '0' => t('false'));

    $form['api-settings'] = array (
        '#type' => 'fieldset',
        '#title' => 'Delving Remote-Search API settings',
    );
    $form['api-settings']['drs_search_url'] = array(
        '#type' => 'textfield',
        '#title' => t('Delving Remote-search API URL'),
        '#description' => t('URL to send search request to. For example: <a href="@api_url">@api_url</a>', array(
            '@api_url' => url(DRS_SEARCH_URL),
        )),
        '#default_value' => variable_get('drs_search_url', DRS_SEARCH_URL),
        '#required' => TRUE,
    );
    $form['api-settings']['drs_search_digital_objects'] = array(
        '#type' => 'radios',
        '#title' => t('Return only results with digital objects'),
        '#default_value' =>  variable_get('drs_search_digital_objects', DRS_SEARCH_DIGITAL_OBJECTS),
        '#options' => $options_do,
    );
    $form['api-settings']['drs_search_rows'] = array(
        '#type' => 'textfield',
        '#title' => t('Number of rows to return in search results'),
        '#size' => 4,
        '#default_value' => variable_get('drs_search_rows', DRS_SEARCH_ROWS),
    );
    $form['search-display-settings'] = array(
        '#type' => 'fieldset',
        '#title' => 'Display Settings',
    );
    $form['search-display-settings']['drs_search_name'] = array(
        '#type' => 'textfield',
        '#title' => t('Delving Remote-Search Tab Name'),
        '#description' => t('Name placed on the Search Tab on the Search Page: Default set as "@drs_name"', array(
            '@drs_name' => DRS_SEARCH_NAME,
        )),
        '#default_value' => variable_get('drs_search_name', DRS_SEARCH_NAME),
        '#required' => TRUE,
    );
    $form['search-display-settings']['#description'] = '<p>' . t('The display of the facets block can be configured on the <strong class="drupal_link_admin_settings"><a href="'. url('admin/build/block') . '">Blocks Configuration Page</a></strong>.') . '</p>';


    return system_settings_form($form);
}