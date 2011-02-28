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
  );

  $ref = getenv("HTTP_REFERER");
  $back_text = variable_get('delving_search_page_return', DELVING_SEARCH_PAGE_RETURN);

  $output = l($back_text, $ref);
  $output .= "<table class='search-results-item'>";

  foreach ($data->xpath("/result/item") as $item) {
      $thumbnail = implode('', $item->xpath($layout["#thumbnail"]));
      $title = implode('', $item->xpath($layout["#title"]));
      $prefix = str_replace('%/%', '', variable_get('delving_item_page_path', DELVING_ITEM_PAGE_PATH));
      $uri = $prefix . implode('', $item->xpath($layout["#uri"]));

      $output .= '<tr><td>';
      if ($thumbnail) {
        $output .= '<img width="300px" class="overlay" src="' . $thumbnail . '"/>';
      }
      else {
        $output .= '<p>no thumbnail</p>';
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
      $output .= '</td></tr>';
  }
  $output .= '</table>';
  delvingsearch_set_page_title($title);
  return $output;
}

function delvingsearch_set_page_title($title){
  drupal_set_title($title, $output = CHECK_PLAIN);
}

