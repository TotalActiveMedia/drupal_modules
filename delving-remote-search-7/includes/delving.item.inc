<?php

function delvingsearch_show_item($xml) {

  $data = simplexml_load_string($xml);

  $layout = array(
    "" => "europeana:object",
    "Title" => "dc:title",
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

  $output = "<table class='search-results-item'>";

  foreach ($data->xpath("/result/item") as $item) {
    $thumbnail = implode('', $item->xpath($layout[""]));
    $output .= '<tr><td>';
    if ($thumbnail) {
      $output .= '<img width="300px" class="overlay" src="' . $thumbnail . '"/>';
    }
    else {
      $output .= '<p>no thumbnail</p>';
    }
    $output .= '</td><td>';
    foreach ($layout as $key => $value) {
      if ($key) {
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
  return $output;
}