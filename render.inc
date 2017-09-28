<?php

/**
 * Print counters
 *
 * @param $errors
 * @param array $warnings
 * @param array $disabled
 * @param array $ok
 */
function dashCounters($errors, $warnings = array(), $disabled = array(), $ok = array()) {
  $ok_count = 0;
  foreach ($ok as $value) {
    $ok_count += count($value);
  }

  $warn_count = 0;
  foreach ($warnings as $value) {
    $warn_count += count($value);
  }

  $error_count = 0;
  foreach ($errors as $value) {
    $error_count += count($value);
  }

  $dis_count = 0;
  foreach ($disabled as $value) {
    $dis_count += count($value);
  }

  echo '<div class="center status-counters">';
  echo '<div class="status-counter down">', $error_count, ' - Down</div>';
  echo '<div class="status-counter warn">', $warn_count, ' - Warning</div>';
  echo '<div class="status-counter ok">', $ok_count, ' - UP/OK</div>';
  echo '<div class="status-counter disabled">', $dis_count, ' - Disabled</div>';
  echo '</div>';
}


/**
 * Print table content
 *
 * @param $output
 * @param array $errors
 * @param array $warnings
 * @param array $disabled
 * @param array $ok
 */

function tabledisplay($output, $display = 'default') {
  foreach ($output as $service) {
    switch($display) {
      case 'abandoned' :
        echo '<tr>';
        echo '<td>', $service['host_name'], '</td>';
        echo '<td>', $service['plugin_output'], '</td>';
        echo '</tr>';
        break;

      case 'default' :
          echo '<tr>';
          echo '<td>', date("d-m-Y H:i:s", $service['last_check']), '</td>';

          if ($service['problem_has_been_acknowledged'] == 0) {
            echo '<td>', $service['host_name'], '</td>';
          }
          else {
            echo '<td>', $service['host_name'], ' (ACK)</td>';
          }

          echo '<td>', $service['plugin_output'], '</td>';
          echo '<td>', $service['service_description'], '</td>';
          echo '</tr>';
        break;
    }
  }
}

?>