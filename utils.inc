<?php

require_once('parse.inc');

function showNotifications($file) {
	$data = parseData($file);

	$warnings = array();
	$errors = array();
	$disabled = array();
	$ok = array();

	foreach ($data['services'] as $serverName => $services) {
		foreach ($services as $key => $status) {
			switch ($status["current_state"]) {
				case 1:
					$warnings[$status['host_name']][] = $status;
					break;

				case 2:
					$errors[$status['host_name']][] = $status;
					break;

				case 3:
					$disabled[$status['host_name']][] = $status;
					break;

				default:
					$ok[$status['host_name']][] = $status;
					break;
			}
		}
	}

	dashCounters($errors, $warnings, $disabled, $ok);
	dashTabledisplay($errors, $warnings, $disabled);
}

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

function dashTabledisplay($errors, $warnings = array(), $disabled = array(), $ok = array()) {
	foreach ($errors as $host => $services) {
		foreach ($services as $service) {
			echo '<tr bgcolor="red">';
      echo '<td>', date("d-m-Y H:i:s", $service['last_check']), '</td>';

      if ($service['problem_has_been_acknowledged'] == 0) {
        echo '<td>', $host, '</td>';
      }
      else {
      	echo '<td>', $host, ' (ACK)</td>';
      }

			echo '<td>', $service['plugin_output'], '</td>';
      echo '<td>', $service['service_description'], '</td>';
    	echo '</tr>';
		}
	}

	foreach ($warnings as $host => $services) {
		foreach ($services as $service) {
			echo '<tr bgcolor="yellow">';
      echo '<td>', date("d-m-Y H:i:s", $service['last_check']), '</td>';

      if ($service['problem_has_been_acknowledged'] == 0) {
        echo '<td>', $host, '</td>';
      }
      else {
      	echo '<td>', $host, ' (ACK)</td>';
      }

			echo '<td>', $service['plugin_output'], '</td>';
      echo '<td>', $service['service_description'], '</td>';
    	echo '</tr>';
		}
	}
}
