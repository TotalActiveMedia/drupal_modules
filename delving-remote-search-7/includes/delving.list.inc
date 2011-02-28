<?php
/**
 * Outputs html to display the returned results
 *
 * @param $xml
 * @param $query
 * @return void
 */
function delvingsearch_show_list($xml, $query) {

  $data = simplexml_load_string($xml);
  $total_records = $data->query['numFound'];
  $num_per_page = variable_get('delving_api_results_rows', DELVING_API_RESULTS_ROWS);

  $layout = array();
  foreach ($data->xpath("/results/layout/drupal/field") as $field) {
    $layout[(string) $field->key] = (string) $field->value;
  }

  pager_default_initialize($total_records, $num_per_page);

  $output = '<div id="delving">';

  if ($total_records > 0 && $layout) {
    // set up the variables to pass along to the pager
    $variables = array(
      'parameters' => array(
        'query' => $query,
        'rows' => $num_per_page,
      ),
    );
    $output .= delvingsearch_breadcrumbs();
    $output .= '<h4>Number of records found: ' . $total_records . '</h4>';
        $output .= theme('pager', $variables);
    $output .= "<table class='search-results'>";
    foreach ($data->xpath("/results/items/item") as $item) {

      $thumbnail = implode('', $item->xpath($layout["#thumbnail"]));
      $title = implode('', $item->xpath($layout["#title"]));

            switch (variable_get('delving_item_page_path', DELVING_SEARCH_ITEM_PAGE_PATH)) {
                case 'isShownAt':
                    $uri = implode('',$item->xpath($layout['#isShownAt']));
                    break;
                case 'norvegiana':
                    $prefix = variable_get('delving_item_norvegiana_path', DELVING_SEARCH_ITEM_NORVEGIANA_PATH);
                    $uri = $prefix . implode('',$item->xpath($layout['#uri'])) . '.html';
                    break;
                default:
                    $prefix = str_replace('%/%','',variable_get('delving_item_page_path', DELVING_SEARCH_ITEM_PAGE_PATH));
                    $uri = $prefix . implode('',$item->xpath($layout["#uri"]));
                    break;
            }

      $output .= '<tr><td>';
      if ($thumbnail) {
        $output .= '<img width="100px" class="overlay" src="' . $thumbnail . '"/>';
      }
      else {
        $output .= '<p>no thumbnail</p>';
      }
      $output .= '</td><td>';
      $output .= '<h3>' . l($title, $uri) . '</h3>';
      foreach ($layout as $key => $value) {
        if (substr($key, 0, 1) != '#') {
          $value_array = $item->xpath($value);
          if ($value_array) {
            $value = implode(', ', $value_array);
            $output .= '<strong>' . $key . ': </strong>' . $value . '<br/>';
          }
        }
      }
      $output .= '</td></tr>';
    }
    $output .= '</table>';
    $output .= theme('pager', $variables);
  }
  else {
    $output .= "No results found. Please try another search";
  }

  $output .= '</div>';
  return $output;
}