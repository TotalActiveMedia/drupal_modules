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

    $display_style = variable_get('delving_search_results_display_style', DELVING_SEARCH_RESULTS_DISPLAY_STYLE);

    if ($display_style == 'grid') {

      $output .= '<div class="results">';

      $count = 1;

      foreach ($data->xpath("/results/items/item") as $item) {

        $thumbnail = implode('', $item->xpath($layout["#thumbnail"]));
        $title = implode('', $item->xpath($layout["#title"]));
//        $description =  implode('', $item->xpath($layout["#description"]));

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
          $thumb_width = variable_get('delving_search_thumb_width', DELVING_SEARCH_THUMB_WIDTH);
          $viewer_height = variable_get('delving_search_thumb_viewer_height', DELVING_SEARCH_THUMB_VIEWER_HEIGHT);
          $output .= '<div class="img-container" style="height:' . $viewer_height . 'px">';
          $output .= l('<img width="' . $thumb_width  .  '" class="overlay" src="' . $thumbnail . '"/>', $uri, array('attributes' => $link_attributes, 'html' => TRUE));
          $output .= '</div>';
        }
        else {
          $output .= '<p>' . t('no thumbnail available') .  $thumbnail . '</p>';
        }

        $max_title = variable_get('delving_search_results_title_length', DELVING_SEARCH_RESULTS_TITLE_LENGTH);
//        $output .= strlen($title) . ' ';
//        $output .= strlen(substr($title, 0, $max_title));
        if (trim($max_title) != '') {
          $output .= '<h2>' . l(delving_teaser($title, $max_title, 'chars'), $uri, array('attributes' => $link_attributes)) . '</h2>';

        }
        else {
             $output .= '<h2>' . l($title, $uri, array('attributes' => $link_attributes)) . '</h2>';
        }

//        foreach ($layout as $key => $value) {
//          if (substr($key, 0, 1) != '#') {
//            $value_array = $item->xpath($value);
//            if ($value_array) {
//              $value = implode(', ', $value_array);
//
//              // TODO: either find a way to internationalize, or the in the config have admin define the field name explicitly in their chosen environment language
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


        $output .= '<tr><td class="result">';
        if ($thumbnail) {
          $output .= '<div class="img-container">';
          $output .= l('<img width="100px" class="overlay" src="' . $thumbnail . '"/>', $uri, array('attributes' => $link_attributes, 'html' => TRUE));

          $output .= '</div>';
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
function delving_word_trim($string, $count, $ellipsis = FALSE){
  $words = explode(' ', $string);
  if (count($words) > $count){
    array_splice($words, $count);
    $string = implode(' ', $words);
    if (is_string($ellipsis)){
      $string .= $ellipsis;
    }
    elseif ($ellipsis){
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
function delving_teaser($str, $num=50, $type='chars'){
	$chunks = explode(' ', $str);

	if($type == 'chars' && strlen($str) <= $num){
		return $str;
	} elseif($type == 'words' && count($chunks) <= $num){
		return $str;
	}

	$ret = '';
	$i=0;

	foreach($chunks as $chunk){
		if($type == 'chars' && strlen($ret.' '.$chunk) > $num){
			break;
		} elseif($type == 'words' && $i == $num){
			break;
		}
		$ret .= ' '.$chunk;
		$i++;
	}
  $ret .= '...';
	return trim($ret);
}