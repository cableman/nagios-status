<?php

$hasCritical = FALSE;
$hasNotification = FALSE;

require_once('parse.inc');
require_once('render.inc');

/**
 * Display notifications (Errors that might be relevant for time planning).
 *
 * @param $file
 *  The data.
 * @param bool $context
 *  Which part of the display to use
 * @param bool $print
 *  If function is used for printing.
 */

function showNotifications($file, $context = FALSE, $print = TRUE) {
  global $hasCritical, $hasNotification;
	$data = parseData($file);

	$output = array();
  $display = 'default';
	foreach ($data['services'] as $serverName => $services) {
    switch ($context) {
      // current_state: 0 for ok, 1 for noncritical, 2 for critical
      case 'critical' :
        $services = dropServer($serverName, $services);
        if(addServiceByAttributeValue($services, 'Root Partition', 'current_state', '2')) {
          $output[] = addServiceByAttributeValue($services, 'Root Partition', 'current_state', '2');
          $hasCritical = TRUE;
        }
        if(addServiceByAttributeValue($services, 'PING', 'current_state', '2')) {
          $output[] = addServiceByAttributeValue($services, 'PING', 'current_state', '2');
          $hasCritical = TRUE;
        }

        if (addServiceByAttributeValue($services, 'MEM', 'current_state', '2')) {
          $output[] = addServiceByAttributeValue($services, 'MEM', 'current_state', '2');
          $hasCritical = TRUE;
        }

        if (addServiceByAttributeValue($services, 'Swap Usage', 'current_state', '2')) {
          $output[] = addServiceByAttributeValue($services, 'Swap Usage', 'current_state', '2');
          $hasCritical = TRUE;
        }
        break;

      case 'notification' :
        $services = dropServer($serverName, $services);
        if(addServiceByAttributeValue($services, 'Root Partition', 'current_state', '1')) {
          $output[] = addServiceByAttributeValue($services, 'Root Partition', 'current_state', '1');
          $hasNotification = TRUE;
        }

        if(addServiceByAttributeValue($services, 'PING', 'current_state', '1')) {
          $output[] = addServiceByAttributeValue($services, 'PING', 'current_state', '1');
          $hasNotification = TRUE;
        }

        if (addServiceByAttributeValue($services, 'MEM', 'current_state', '1')) {
          $output[] = addServiceByAttributeValue($services, 'MEM', 'current_state', '1');
          $hasNotification = TRUE;
        }

        if (addServiceByAttributeValue($services, 'Swap Usage', 'current_state', '1')) {
          $output[] = addServiceByAttributeValue($services, 'Swap Usage', 'current_state', '1');
          $hasNotification = TRUE;
        }
        break;

      case 'abandoned' :
        if(in_array($serverName, getDroppedServersNames())) {
          $output[] = addServiceByAttributeValue($services, 'APT', 'current_state', '2');
          $display = 'abandoned';
        }
        break;
    }
	}
  if ($print) {
    tabledisplay($output, $display);
  }
}

/**
 * Add service if an attribute of the service has a specific value.
 *
 * @param $services
 *  All available services.
 * @param $service_name
 *  A specific service name
 * @param $attribute
 *  An attribute label of the service
 * @param $value
 *  The value of the attribute
 * @return mixed
 *  Array or false.
 */
function addServiceByAttributeValue($services, $service_name, $attribute, $value) {
	if (array_key_exists($service_name, $services)) {
		if ($services[$service_name][$attribute] == $value) {
      $output = $services[$service_name];
      return $output;
		}
	}
  return FALSE;
}

/**
 * Remove servers that are not being updated from the list.
 *
 * @param $serverName
 *  Name of the server.
 * @param $services
 *  All available services.
 * @return array
 *  Array of services, maybe an empty array.
 */

function dropServer($serverName, $services) {
	$droplist = getDroppedServersNames();
  if(in_array($serverName, $droplist)) {
    $services = array();
  }

	return $services;
}


/**
 * Define a list of servers that we don't upgrade.
 *
 * @return array
 *  An array of server names.
 */
function getDroppedServersNames() {
	return array(
		'xorn.odaa.dk',
    'zemo.odaa.dk'
	);
}

/**
 * Count APT errors
 *
 * @param $file
 *  The data file.
 * @param $urgency
 *  THe criticality.
 * @return int
 *  Number of errors.
 */
function displayAptCount($file, $urgency) {
  $aptErrorCount = 0;
  $data = parseData($file);
  foreach ($data['services'] as $serverName => $services) {
    $services = dropServer($serverName, $services);
    if (isset($services['APT'])) {
      $pluginOutputArray = explode(' ', $services['APT']['plugin_output']);
      if ($urgency == $pluginOutputArray[1]) {
        $aptErrorCount += $pluginOutputArray[0];
      }
    }
  }
  return $aptErrorCount;
}

/**
 * Count the number of servers.
 *
 * @param $file
 *  File of data
 * @return int
 *  Number of services.
 */
function serverCount($file) {
  $data = parseData($file);
  return count($data['services'])-count(getDroppedServersNames());
}