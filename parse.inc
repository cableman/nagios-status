<?php

function parseData($statusFile)
{
    $data = getData($statusFile);
    $hosts = $data['hosts'];
    $services = $data['services'];
    $program = "";
    if (array_key_exists("program", $data)) {
        $program = $data['program'];
    }

    $output = array();

    // program status
    if ($program != "") {
        $output['programStatus'] = array();
        foreach ($program as $key => $val) {
            $output['programStatus'][$key]= $val;
        }
    }

    // hosts
    $output['hosts'] = array();
    foreach ($hosts as $hostName => $hostArray) {
        isset($output['hosts'][$hostName]) ?: $output['hosts'][$hostName] = array();
        foreach ($hostArray as $key => $val) {
            $output['hosts'][$hostName][$key] = $val;
        }
    }
    unset($hostName, $hostArray);

    // loop through the services
    $output['services'] = array();
    foreach ($services as $hostName => $service) {
        $output['services'][$hostName] = array();
        foreach ($service as $serviceDesc => $serviceArray) {
            $output['services'][$hostName][$serviceDesc] = array();
            foreach ($serviceArray as $key => $val) {
                $output['services'][$hostName][$serviceDesc][$key] = $val;
            }
        }
    }
    unset($hostName, $service);

    return $output;
}

// figure out what version the file is
function getFileVersion($statusFile)
{
    global $created_ts;
    $version = 2;

    $fh = fopen($statusFile, 'r');
    $inInfo = false;
    while ($line = fgets($fh)) {
        if (trim($line) == "info {") {
            $inInfo = true;
        } elseif (trim($line) == "}") {
            $inInfo = false;
            break;
        } elseif ($inInfo) {
            $vals = explode("=", $line);
            if (trim($vals[0]) == "created") {
                $created = $vals[1];
            } elseif (trim($vals[0]) == "version") {
                $version = substr($vals[1], 0, 1);
            }
        }
    }

    return $version;
}

function getData($statusFile)
{
    global $debug;

    # open the file
    $fh = fopen($statusFile, 'r');

    # variables to keep state
    $inSection = false;
    $sectionType = "";
    $lineNum = 0;
    $sectionData = array();

    $hostStatus = array();
    $serviceStatus = array();
    $programStatus = array();

    #variables for total hosts and services
    $typeTotals = array();

    # loop through the file
    while ($line = fgets($fh)) {
        $lineNum++; // increment counter of line number, mainly for debugging
        $line = trim($line); // strip whitespace
        if ($line == "") {
            continue;
        } // ignore blank line
        if (substr($line, 0, 1) == "#") {
            continue;
        } // ignore comment

        // ok, now we need to deal with the sections
        if (!$inSection) {
            // we're not currently in a section, but are looking to start one
            if (substr($line, strlen($line) - 1, 1) == "{") // space and ending with {, so it's a section header
            {
                $sectionType = substr($line, 0, strpos($line, " ")); // first word on line is type
                $inSection = true;
                // we're now in a section
                $sectionData = array();

                // increment the counter for this sectionType
                if (isset($typeTotals[$sectionType])) {
                    $typeTotals[$sectionType] = $typeTotals[$sectionType] + 1;
                } else {
                    $typeTotals[$sectionType] = 1;
                }

            }
        } elseif ($inSection && trim($line) == "}") // closing a section
        {
            if ($sectionType == "servicestatus") {
                $serviceStatus[$sectionData['host_name']][$sectionData['service_description']] = $sectionData;
            } elseif ($sectionType == "hoststatus") {
                $hostStatus[$sectionData["host_name"]] = $sectionData;
            } elseif ($sectionType == "programstatus") {
                $programStatus = $sectionData;
            }
            $inSection = false;
            $sectionType = "";
            continue;
        } else {
            // we're currently in a section, and this line is part of it
            $lineKey = substr($line, 0, strpos($line, "="));
            $lineVal = substr($line, strpos($line, "=") + 1);

            // add to the array as appropriate
            if ($sectionType == "servicestatus" || $sectionType == "hoststatus" || $sectionType == "programstatus") {
                if ($debug) {
                    echo "LINE " . $lineNum . ": lineKey=" . $lineKey . "= lineVal=" . $lineVal . "=\n";
                }
                $sectionData[$lineKey] = $lineVal;
            }
            // else continue on, ignore this section, don't save anything
        }

    }

    fclose($fh);

    $retArray = array("hosts" => $hostStatus, "services" => $serviceStatus, "program" => $programStatus);
    return $retArray;
}

// this formats the age of a check in seconds into a nice textual description
function ageString($seconds)
{
    $age = "";
    if ($seconds > 86400) {
        $days = (int)($seconds / 86400);
        $seconds = $seconds - ($days * 86400);
        $age .= $days . " days ";
    }
    if ($seconds > 3600) {
        $hours = (int)($seconds / 3600);
        $seconds = $seconds - ($hours * 3600);
        $age .= $hours . " hours ";
    }
    if ($seconds > 60) {
        $minutes = (int)($seconds / 60);
        $seconds = $seconds - ($minutes * 60);
        $age .= $minutes . " minutes ";
    }
    $age .= $seconds . " seconds ";
    return $age;
}
?>
