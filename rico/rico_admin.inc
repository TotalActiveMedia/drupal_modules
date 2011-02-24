<?php
// $Id$
/**
 * @file
 * Form builder for Delving Remote-Search API settings.
 *
 * @ingroup forms
 * @see system_settings_form()
 */
function rico_admin_settings() {

    $options_do = array('1' => t('true'), '0' => t('false'));
    $options_df = array('1' => t('true'), '0' => t('false'));

    $form['api-settings'] = array (
        '#type' => 'fieldset',
        '#title' => 'Delving Remote-Search API settings',
    );
    $form['api-settings']['rico_search_remote_url'] = array(
        '#type' => 'textfield',
        '#title' => t('Delving Remote-search API URL'),
        '#description' => t('URL to send search request to. For example: <a href="@api_url">@api_url</a>', array(
            '@api_url' => url(RICO_SEARCH_REMOTE_URL),
        )),
        '#default_value' => variable_get('drs_search_url', RICO_SEARCH_REMOTE_URL),
        '#required' => TRUE,
    );
    $form['api-settings']['rico_search_digital_object'] = array(
        '#type' => 'radios',
        '#title' => t('Return only results with digital objects'),
        '#default_value' =>  variable_get('rico_search_digital_object', RICO_SEARCH_DIGITAL_OBJECT),
        '#options' => $options_do,
    );
    $form['api-settings']['rico_search_results_rows'] = array(
        '#type' => 'textfield',
        '#title' => t('Number of rows to return in search results'),
        '#size' => 2,
        '#default_value' => variable_get('rico_search_results_rows', RICO_SEARCH_RESULTS_ROWS),
    );
    $form['search-display-settings'] = array(
        '#type' => 'fieldset',
        '#title' => 'Display Settings',
    );
    $form['search-display-settings']['#description'] = '<p>' . t('The display of the facets block can be configured on the <strong class="drupal_link_admin_settings"><a href="'. url('admin/structure/block') . '">Blocks Configuration Page</a></strong>.') . '</p>';


    return system_settings_form($form);
}