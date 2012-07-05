<?php

function delvingsearch_show_item($xml) {
  if (empty($xml)): {
    delvingsearch_set_page_title(t('Error'));
    $output = '<div id="delving" class="object">' . t('There was a problem displaying this item. Sorry!') . '</div>';
  }
  else: {
    $data = simplexml_load_string($xml);
  //  drupal_set_message($xml);

    $layout = array(
      "#thumbnail" => "europeana:object",
      "#title" => "dc:title",
      "#uri" => "europeana:uri",
      "#originalContext" => "europeana:isShownAt",
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
      "Created" => "dcterms:created",
      "Rights" => "dc:rights"
    );
    //    $layout = array();
    //    foreach ($data->xpath("/results/layout/drupal/field") as $field) {
    //        $layout[(string)$field->key] = (string)$field->value;
    //    }
    $link_attributes = array();
    if (variable_get('delving_search_item_hyperlink_button', DELVING_SEARCH_ITEM_HYPERLINK_BUTTON) == 1) {
      $link_attributes['class'] = 'button';
    }

    $ref = getenv("HTTP_REFERER");
    $back_text = variable_get('delving_search_item_page_return_to_results', DELVING_SEARCH_ITEM_PAGE_RETURN_TO_RESULTS);

    $output = '<div id="delving" class="object">';
    $output .= '<div class="delving-breadcrumbs">';
    $output .= l($back_text, $ref, array('attributes' => $link_attributes));
    $output .= '</div>';
    $output .= '<div class="result-object">';
    $output .= '<div class="result-object-img">';
    foreach ($data->xpath("/result/item/fields") as $item) {
      $thumbnail = implode('', $item->xpath($layout["#thumbnail"]));
      $title = implode('', $item->xpath($layout["#title"]));
      $prefix = str_replace('%/%', '', variable_get('delving_item_page_path', DELVING_SEARCH_ITEM_PAGE_PATH));
      $uri = $prefix . implode('', $item->xpath($layout["#uri"]));
      $originalContext = implode('', $item->xpath($layout["#originalContext"]));
      $img_width = variable_get('delving_search_object_img_width', DELVING_SEARCH_OBJECT_IMG_WIDTH);

      if ($thumbnail) {
        $shown_by = implode('', $item->xpath('europeana:isShownBy'));
        if(empty($shown_by)): {
          $output .= '<img width="' . $img_width . '" src="' . $thumbnail . '" alt="'. $title .'"/>';
        }
        else: {
          $output .= l('<img width="' . $img_width . '" src="' . $thumbnail . '" alt="'. $title .'"/>', trim($shown_by), array('html' => TRUE, 'attributes' => array('title' => t('Show original image'))));
        }
        endif;
      }
      else {
       $thumbnail = base_path() . drupal_get_path('module', 'delvingsearch') . '/img/no-image.png';
       $output .= '<img width="' . $img_width . '" src="' . $thumbnail . '"/>';
      }
      $output .= '</div>'; // result-object-img
      $output .= '<div class="result-object-meta">';
      foreach ($layout as $key => $value) {
        if (substr($key, 0, 1) != '#') {
          $value_array = $item->xpath($value);
          if ($value_array) {
            $field_value = str_replace(':', '_', $value);
            $key = implode('', $item->xpath('//field[name="' . $field_value . '"]/i18n'));
            $value = implode(', ', $value_array);
            $output .= '<dl class="' . $field_value . '">';
            $output .= '<dt>' . $key . ': </dt><dd>' . check_plain(trim($value)) . '</dd>';
            $output .= '</dl>';
          }
        }
      }

      $link_attributes['target'] = '_blank';
      if ($originalContext && variable_get('delving_search_item_show_in_original_context', DELVING_SEARCH_ITEM_SHOW_IN_ORIGINAL_CONTEXT) == 1) {
        $output .= l(t("Show in original context"), trim($originalContext), array('attributes' => $link_attributes));
      }


      $output .= '</div>'; // result-object-meta


      $output .= '</div>'; // result-object
      $social_enabled = variable_get('delving_search_social_widget_display', DELVING_SEARCH_SOCIAL_WIDGET_DISPLAY);
      if ($social_enabled = '1') {
        $pubid = variable_get('delving_search_addthis_pubid');
        if (strlen($pubid)<1) {
          $pubid = 'xa-4eef057268595af4';
        }
        $widget_type = variable_get('delving_search_social_widget_type', DELVING_SEARCH_SOCIAL_WIDGET_TYPE);
        if ($widget_type < 3): {
          $widget_class = 'addthis_toolbox addthis_default_style ';
          if ($widget_type == 2) {
            $widget_class = 'addthis_toolbox addthis_default_style addthis_32x32_style';
          }
          $output .= '<div class="' . $widget_class . '">
                      <a class="addthis_button_preferred_1"></a>
                      <a class="addthis_button_preferred_2"></a>
                      <a class="addthis_button_preferred_3"></a>
                      <a class="addthis_button_preferred_4"></a>
                      <a class="addthis_button_compact"></a>
                      <a class="addthis_counter addthis_bubble_style"></a>
                      </div>
                      <script type="text/javascript" src="http://s7.addthis.com/js/300/addthis_widget.js#pubid=' . $pubid . '"></script>';
        }
        else: $output .='<div class="addthis_toolbox addthis_default_style ">
                        <a class="addthis_button_facebook_like" fb:like:layout="button_count"></a>
                        <a class="addthis_button_tweet"></a>
                        <a class="addthis_button_google_plusone" g:plusone:size="medium"></a>
                        <a class="addthis_counter addthis_pill_style"></a>
                        </div>
                        <script type="text/javascript" src="http://s7.addthis.com/js/300/addthis_widget.js#pubid=' . $pubid . '"></script>';
        endif;
      }
    }

    $output .= '<div id="pb-delving">' . l('Search Powered by Delving', 'http://www.delving.eu', array('attributes' => array('target' => '_blank'))) . '</div>';
    $output .= '</div>';
    delvingsearch_set_page_title($title);
  }
  endif;
  return $output;
}

function delvingsearch_set_page_title($title) {
  drupal_set_title($title, $output = CHECK_PLAIN);
}

