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
  $layout = array(
    "#thumbnail" => "europeana:object",
    "#title" => "dc:title",
    "#description" => "dc:description",
  );
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

    $result_style = variable_get('delving_search_results_display_style', DELVING_SEARCH_RESULTS_DISPLAY_STYLE);

    //    if ($result_style == 'grid') {

    $output .= '<div class="results ' . $result_style . '">';

    $count = 1;

    foreach ($data->xpath("/results/items/item") as $item) {

      // THUMBNAIL SETTINGS ********************************************************************************************
      $thumbnail = implode('', $item->xpath($layout["#thumbnail"]));
      $thumb_width = variable_get('delving_search_thumb_width', DELVING_SEARCH_THUMB_WIDTH);
      $viewer_height = variable_get('delving_search_thumb_viewer_height', DELVING_SEARCH_THUMB_VIEWER_HEIGHT);

      // TITLE SETTINGS ************************************************************************************************
      $title = implode('', $item->xpath($layout["#title"]));
      if (trim($title) == '' || trim($title) == '-') {
        $title = t("Untitled");
      }
      $max_title = variable_get('delving_search_results_title_length', DELVING_SEARCH_RESULTS_TITLE_LENGTH);

      // DESCRIPTION SETTINGS ******************************************************************************************
      $description = implode('', $item->xpath($layout["#description"]));
      $max_description = variable_get('delving_search_results_description_length', DELVING_SEARCH_RESULTS_DESCRIPTION_LENGTH);

      // OBJECT LINK/VIEWPATH SETTTINGS ********************************************************************************
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

      // ROW SETTINGS **************************************************************************************************
      $row_class = '';
      $result_style == 'list' && $count % 2 == 0 ? $row_class = 'even' : $row_class = 'odd';

      // RESULTS  ******************************************************************************************************
      $output .= '<div class="result ' . $row_class . '">';
      $output .= '<div class="img-container">';
      $output .= '<div class="img-window" style="height:' . $viewer_height . 'px">';

      if ($thumbnail) {
        $output .= l('<img width="' . $thumb_width . '" class="overlay" src="' . $thumbnail . '"/>', $uri, array('attributes' => $link_attributes, 'html' => TRUE));
      }
      else {
        //        $output .= '<p>' . t('no thumbnail available') . '</p>';
        $thumbnail = base_path() . drupal_get_path('module', 'delvingsearch') . '/img/no-image.png';
        //        $output .= base_path();
        $output .= l('<img width="' . $thumb_width . '" src="' . $thumbnail . '"/>', $uri, array('attributes' => $link_attributes, 'html' => TRUE));
      }

      $output .= '</div>'; // end img-window
      $output .= '</div>'; // end img-container
      $output .= '<div class="meta-container">';
      $output .= '<div class="meta-window">';

      // TITLE DISPLAY *************************************************************************************************
      if (trim($max_title) != '') {
        $output .= '<h2>' . l(delving_teaser($title, $max_title, 'chars'), $uri, array('attributes' => $link_attributes)) . '</h2>';

      }
      else {
        //             $output .= '<div class="title">' . l($title, $uri, array('attributes' => $link_attributes)) . '</div>';
        $output .= '<h2>' . l($title, $uri, array('attributes' => $link_attributes)) . '</h2>';
        //             $output .= $title;
      }

      if ($result_style == 'list') {
      // DESCRIPTIONS DISPLAY ******************************************************************************************
      if (trim($max_description) != '') {
        $output .= '<p><strong>' . t('Description: ') . '</strong>' . delving_teaser($description, $max_description, 'chars') . '</p>';

      }
      else {
        $output .= '<p><strong>'  . t('Description: ') . '</strong>' . $description . '</p>';
      }
        foreach ($layout as $key => $value) {

          if (substr($key, 0, 1) != '#') {
            $value_array = $item->xpath($value);
            if ($value_array) {
              $value = implode(', ', $value_array);
              // TODO: either find a way to internationalize, or the in the config have admin define the field name explicitly in their chosen environment language
              // Trim field content length
              $output .= '<strong>' . $key . ': </strong>' . $value . '<br/>';
            }
          }
        }
      }
      else {
        foreach ($layout as $key => $value) {
          if (strtolower($key) == 'creator') {
            $output .= 'test';
          }
        }
      }

      $output .= '</div>'; // end meta-window
      $output .= '</div>'; // end meta-container
      $output .= '</div>'; // result
      if ($result_style == 'grid' && $count % 4 == 0) {
        $output .= '<div class="row-break"></div>';
      }
      $count++;
    }
    $output .= '</div>';
    $output .= theme('pager', $variables);
    $output .= '<div id="pb-delving">' . l('Search Powered by Delving', 'http://www.delving.eu', array('attributes' => array('target' => '_blank'))) . '</div>';
  }
  else {
    $output .= t('No results found. Please try another search');
  }

  $output .= '</div>';
  return $output;
}

/**
 * Trim a string to a given number of words
 *
 * @param $string
 *   the original string
 * @param $count
 *   the word count
 * @param $ellipsis
 *   TRUE to add "..."
 *   or use a string to define other character
 * @param $node
 *   provide the node and we'll set the $node->
 *
 * @return
 *   trimmed string with ellipsis added if it was truncated
 */
function delving_word_trim($string, $count, $ellipsis = FALSE)
{
  $words = explode(' ', $string);
  if (count($words) > $count) {
    array_splice($words, $count);
    $string = implode(' ', $words);
    if (is_string($ellipsis)) {
      $string .= $ellipsis;
    }
    elseif ($ellipsis) {
      $string .= '&hellip;';
    }
  }
  return $string;
}

/**
 * delving_teaser()
 *
 * Accepts types: words or chars
 *
 * Example Usage: echo delving_teaser($really_long_string_with_no_html, 150, 'chars');
 *
 * @param mixed $str
 * @param integer $num
 * @param string $type
 * @return
 */
function delving_teaser($str, $num = 50, $type = 'chars')
{
  $chunks = explode(' ', $str);

  if ($type == 'chars' && strlen($str) <= $num) {
    return $str;
  } elseif ($type == 'words' && count($chunks) <= $num) {
    return $str;
  }

  $ret = '';
  $i = 0;

  foreach ($chunks as $chunk) {
    if ($type == 'chars' && strlen($ret . ' ' . $chunk) > $num) {
      break;
    } elseif ($type == 'words' && $i == $num) {
      break;
    }
    $ret .= ' ' . $chunk;
    $i++;
  }
  //  $ret .= '&hellip;';
  $ret .= ' ...';
  return trim($ret);
}