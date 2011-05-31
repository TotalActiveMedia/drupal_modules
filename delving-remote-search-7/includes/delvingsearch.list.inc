<?php
/**
 * Outputs html to display the returned results
 *
 * @param $xml
 * @param $query
 * @return void
 */
function delvingsearch_show_list($xml, $query)
{

  $data = simplexml_load_string($xml);
  $total_records = $data->query['numFound'];
  $num_per_page = variable_get('delving_api_results_rows', DELVING_API_RESULTS_NR_ITEMS);

  $layout = array();
  foreach ($data->xpath("/results/layout/drupal/field") as $field) {
    $layout[(string)$field->key] = (string)$field->value;
  }

  pager_default_initialize($total_records, $num_per_page);

  $output = '<div id="delving">';
  $output .= '<div class="delving-count"><h4>' . t('Number of records found: ') . $total_records . '</h4></div>';

  if ($total_records > 0 && $layout) {
    // set up the variables to pass along to the pager
    $variables = array(
      'parameters' => array(
        'query' => $query,
        'rows' => $num_per_page,
      ),
    );

    $output .= delvingsearch_breadcrumbs();

    $output .= theme('pager', $variables);

    $display_style = variable_get('delving_api_results_display_style', DELVING_API_RESULTS_DISPLAY_STYLE);

    if ($display_style == 'grid') {

      $output .= '<div class="results">';

      $count = 1;

      foreach ($data->xpath("/results/items/item") as $item) {

        $thumbnail = implode('', $item->xpath($layout["#thumbnail"]));
        $title = implode('', $item->xpath($layout["#title"]));

        $link_attributes = array();

        switch ((string)variable_get('delving_search_item_view_path', DELVING_SEARCH_ITEM_VIEW_PATH)) {
          case 'isShownAt':
            $uri = trim(implode('', $item->xpath($layout['#isShownAt'])));
            $link_attributes['target'] = "_blank";
            break;
          case 'delvingPlatform':
            $prefix = variable_get('delving_search_item_external_url', DELVING_SEARCH_ITEM_EXTERNAL_URL);
            $uri = $prefix . implode('', $item->xpath($layout['#uri'])) . '.html';
            $link_attributes['target'] = "_blank";
            break;
          default:
            $prefix = str_replace('%/%', '', variable_get('delving_search_item_page_path', DELVING_SEARCH_ITEM_PAGE_PATH));
            $uri = $prefix . implode('', $item->xpath($layout["#uri"]));
            break;
        }


        $output .= '<div class="result">';
        if ($thumbnail) {
          $output .= l('<img width="100px" class="overlay" src="' . $thumbnail . '"/>', $uri, array('attributes' => $link_attributes, 'html' => TRUE));
        }
        else {
          $output .= '<p>' . t('no thumbnail available') .  $thumbnail . '</p>';
        }

        $output .= '<h2>' . l($title, $uri, array('attributes' => $link_attributes)) . '</h2>';
//        foreach ($layout as $key => $value) {
//          if (substr($key, 0, 1) != '#') {
//            $value_array = $item->xpath($value);
//            if ($value_array) {
//              $value = implode(', ', $value_array);
//
//              // TODO: either find a way to internationalize, or the in the config have admin define the field name explicitly in thier chosen environment language
//              // Trim field content length
//              /*
//              if (strtolower($key) == 'description') {
//                $dLength = variable_get('delving_api_results_description_length', DELVING_API_RESULTS_DESCRIPTION_LENGTH);
//                if ($dLength > 0) {
//                   $value = substr($value, 0, $dLength) . '...';
//                }
//              }
//              */
//
//              $output .= '<strong>' . $key . ': </strong>' . $value . '<br/>';
//            }
//          }
//        }
        $output .= '</div>';
        if ($count%4==0){
           $output .= '<div class="clearfix"></div>';
        }
        $count++;
      }
      $output .= '</div>';


    }
    else {

      $output .= "<table class='search-results'>";

      foreach ($data->xpath("/results/items/item") as $item) {

        $thumbnail = implode('', $item->xpath($layout["#thumbnail"]));
        $title = implode('', $item->xpath($layout["#title"]));

        $link_attributes = array();

        switch ((string)variable_get('delving_search_item_view_path', DELVING_SEARCH_ITEM_VIEW_PATH)) {
          case 'isShownAt':
            $uri = trim(implode('', $item->xpath($layout['#isShownAt'])));
            $link_attributes['target'] = "_blank";
            break;
          case 'delvingPlatform':
            $prefix = variable_get('delving_search_item_external_url', DELVING_SEARCH_ITEM_EXTERNAL_URL);
            $uri = $prefix . implode('', $item->xpath($layout['#uri'])) . '.html';
            $link_attributes['target'] = "_blank";
            break;
          default:
            $prefix = str_replace('%/%', '', variable_get('delving_search_item_page_path', DELVING_SEARCH_ITEM_PAGE_PATH));
            $uri = $prefix . implode('', $item->xpath($layout["#uri"]));
            break;
        }


        $output .= '<tr><td>';
        if ($thumbnail) {
          $output .= l('<img width="100px" class="overlay" src="' . $thumbnail . '"/>', $uri, array('attributes' => $link_attributes, 'html' => TRUE));
        }
        else {
          $output .= t('no thumbnail available');
        }
        $output .= '</td><td>';
        $output .= '<h3>' . l($title, $uri, array('attributes' => $link_attributes)) . '</h3>';
        foreach ($layout as $key => $value) {
          if (substr($key, 0, 1) != '#') {
            $value_array = $item->xpath($value);
            if ($value_array) {
              $value = implode(', ', $value_array);

              // TODO: either find a way to internationalize, or the in the config have admin define the field name explicitly in thier chosen environment language
              // Trim field content length
              /*
              if (strtolower($key) == 'description') {
                $dLength = variable_get('delving_api_results_description_length', DELVING_API_RESULTS_DESCRIPTION_LENGTH);
                if ($dLength > 0) {
                   $value = substr($value, 0, $dLength) . '...';
                }
              }
              */

              $output .= '<strong>' . $key . ': </strong>' . $value . '<br/>';
            }
          }
        }
        $output .= '</td></tr>';
      }
      $output .= '</table>';

    }


    $output .= theme('pager', $variables);
    $output .= '<div id="pb-delving">' . l('Search Powered by Delving', 'http://www.delving.eu', array('attributes' => array('target' => '_blank'))) . '</div>';
  }
  else {
    $output .= t('No results found. Please try another search');
  }

  $output .= '</div>';
  return $output;
}