<?php

function delvingsearch_show_item($xml) {

  $data = simplexml_load_string($xml);

  $layout = array(
    "#thumbnail" => "europeana:object",
    "#title" => "dc:title",
    "#uri" => "europeana:uri",
    'Title' => "dc:title",
    "Creator" => "dc:creator",
    "Description" => "dc:description",
    "Subject(s)" => "dc:subject",
    "County" => "abm:county",
    "Municipality" => "abm:municipality",
    "Place" => "abm:namedPlace",
    "Person(s)" => "abm:aboutPerson",
    "Contributor(s)" => "dc:contributor",
    "Language" => "dc:language",
    "Dataprovider" => "europeana:dataProvider",
    "Provider" => "europeana:provider",
    "#originalContext" => "europeana:isShownAt"
  );
  //    $layout = array();
  //    foreach ($data->xpath("/results/layout/drupal/field") as $field) {
  //        $layout[(string)$field->key] = (string)$field->value;
  //    }
  $link_attributes = array();
  if(variable_get('delving_search_item_hyperlink_button', DELVING_SEARCH_ITEM_HYPERLINK_BUTTON) == 1) {
    $link_attributes['class'] = 'button';
  }

  $ref = getenv("HTTP_REFERER");
  $back_text = variable_get('delving_search_item_page_return_to_results', DELVING_SEARCH_ITEM_PAGE_RETURN_TO_RESULTS);

  $output = '<div id="delving">';
  $output .= l($back_text, $ref, array('attributes' => $link_attributes));
  $output .= "<table class='search-results-item'>";

  foreach ($data->xpath("/result/item") as $item) {
    $thumbnail = implode('', $item->xpath($layout["#thumbnail"]));
    $title = implode('', $item->xpath($layout["#title"]));
    $prefix = str_replace('%/%', '', variable_get('delving_item_page_path', DELVING_SEARCH_ITEM_PAGE_PATH));
    $uri = $prefix . implode('', $item->xpath($layout["#uri"]));
    $originalContext =  implode('', $item->xpath($layout["#originalContext"]));
    $output .= '<tr><td>';
    if ($thumbnail) {
      $output .= '<img width="300px" class="overlay" src="' . $thumbnail . '"/>';
    }
    else {
      $output .= t('no thumbnail available');
    }
    $output .= '</td><td>';
    //      $output .= '<h3>' . l($title, $uri) . '</h3>';
    foreach ($layout as $key => $value) {
      if (substr($key, 0, 1) != '#') {
        $value_array = $item->xpath($value);
        if ($value_array) {
          $value = implode(', ', $value_array);
          $output .= '<strong>' . $key . ': </strong>' . $value . '<br/>';
        }
      }
    }

    if ($originalContext && variable_get('delving_search_item_show_in_original_context', DELVING_SEARCH_ITEM_SHOW_IN_ORIGINAL_CONTEXT) == 1) {
//      $output .= '<a class="button" href="' . $originalContext .'" target="_blank">' . t("Show in original context") . '</a>';
      $output .= l(t("Show in original context"), $originalContext, array('attributes' => $link_attributes));
    }

    $output .= '</td></tr>';

  }
  $output .= '</table>';
  $output .= '<div id="pb-delving">' . l('Search Powered by Delving', 'http://www.delving.eu',  array('attributes' => array('target' => '_blank')))  . '</div>';
  $output .= '</div>';

  delvingsearch_set_page_title($title);

  return $output;
}

function delvingsearch_set_page_title($title) {
  drupal_set_title($title, $output = CHECK_PLAIN);
}

