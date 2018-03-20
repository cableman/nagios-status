<?php
  require_once('utils.inc');

  // Xmas in December.
  $xmas = (date('m') == 12) ? TRUE : FALSE;

  // Easter
  $easter_eggs_timestamp = 0;

  // Easter start 26.03.2018 - Ascension 10.5.2018
  $easter = (1521504000 < time() && time() < 1525910400 ) ? TRUE : FALSE;
  // Eggs during the holidays. start 26.03.2018 - End 01.04.2018
  $easter_eggs = (1521504000 < time() && time() < 1522540800) ? TRUE : FALSE;
  if ($easter_eggs) {
    $easter_eggs_timestamp = time();
  }

// Easterpack stuff /end.

  // Data
  $file = 'data/status.dat';

  // Image on no errors.
  $okImage = 'images/thumbs.jpg';

  // Check for xmas
  if ($xmas) {
    // Xmas image
    $xmas_image = 'xmas-pack/santa_thumbs.png';
    $okImage = $xmas_image;
  }

  // Check for easter
  if ($easter) {
    // Easter image
    $easter_image = 'easter-pack/ok-jesus.png';
    $okImage = $easter_image;
  }

  // Refresh values
	$refreshvalue = 60;
  $refreshrate = 60;

  // Check for notifications.
  showNotifications($file, 'critical', FALSE);
  showNotifications($file, 'notification', FALSE);
?>

<html>
<title>Dashboard</title>
<link rel="stylesheet" href="https://maxcdn.bootstrapcdn.com/bootstrap/4.0.0-beta/css/bootstrap.min.css" integrity="sha384-/Y6pD6FV/Vv2HJnA6t+vslU6fwYXjCFtcEpHbNJ0lyAFsXTsjBbfaDjzALeQsN6M" crossorigin="anonymous">
<link href="https://fonts.googleapis.com/css?family=Open+Sans:400,700" rel="stylesheet">
<link type="text/css" rel="stylesheet" href="style.css" media="all">
<?php if ($xmas) : ?><link type="text/css" rel="stylesheet" href="xmas-pack/style-xmas.css" media="all"><?php endif; ?>
<?php if ($easter || $easter_eggs) : ?><link type="text/css" rel="stylesheet" href="easter-pack/style-easter.css" media="all"><?php endif; ?>
<body>
  <?php if($xmas){include_once "xmas-pack/snowflakes.php";} ?>
  <?php if($easter_eggs){include_once "easter-pack/eggs.php";} ?>
  <div class="row no-gutters">
    <div class="col">
      <div class="timer--wrapper">
        <div class="timer" id="timer">
        </div>
      </div>
    </div>
  </div>
  <div class="row no-gutters header--inner align-items-center">
    <div class="col-3">
      <img src="logo.svg" class="logo">
    </div>
    <div class="col-6">
      <h1 class="header">Nagios status</h1>
    </div>
    <div class="col-3">
      <div class="calendar--wrapper">
        <div class="calendar">
          <div class="month"><?php echo date('F'); ?></div>
          <div class="date"><?php echo date('j'); ?></div>
          <div class="year"><?php echo date('Y'); ?></div>
        </div>
      </div>
    </div>
  </div>
  <?php if ($hasCritical) : ?>
    <div class="row no-gutters critical">
      <table class="table text-danger">
        <thead>
        <tr>
          <th>Last Checked</th>
          <th>Host</th>
          <th>Status Info</th>
          <th>Service</th>
        </tr>
        </thead>
        <tbody>
          <?php showNotifications($file, 'critical') ?>
        </tbody>
      </table>
    </div>
  <?php endif; ?>
  <?php if ($hasNotification) : ?>
    <div class="row no-gutters notifications">
      <table class="table text-info">
        <thead>
        <tr>
          <th>Last Checked</th>
          <th>Host</th>
          <th>Status Info</th>
          <th>Service</th>
        </tr>
        </thead>
        <tbody>
          <?php showNotifications($file, 'notification') ?>
        </tbody>
      </table>
    </div>
  <?php endif; ?>

  <?php if (!$hasCritical && !$hasNotification) : ?>
    <img src="<?php print $okImage; ?>">
  <?php endif; ?>
  <div class="footer">
    <div class="row no-gutters footer--inner">
      <div class="col-8 col-offset-2 mx-auto summed-updates">
        <table class="table text-info">
          <thead>
          <tr>
            <th># of servers</th>
            <th>Noncritical updates</th>
            <th>Critical updates</th>
          </tr>
          </thead>
          <tbody>
          <?php echo '<td>' . serverCount($file). '</td>'; ?>
          <?php echo '<td>' . displayAptCount($file, 'non-security') . '</td>'; ?>
          <?php echo '<td>' . displayAptCount($file, 'security') . '</td>'; ?>
          </tbody>
        </table>
      </div>
    </div>
  </div>
  <script>
    var milisec = 0;
    var seconds = <?php echo($refreshvalue); ?>;
    var refreshRate = <?php echo($refreshrate); ?>;

    // Easterpack stuff.
    var eggs_timer = <?php echo($easter_eggs_timestamp); ?>;
    var digits_2 = parseInt(eggs_timer.toString().substr(-2));
    var digits_1 = parseInt(eggs_timer.toString().substr(-1));
    // Easterpack stuff /end.

    function display() {
      if (milisec <= 0) {
        milisec = 9;
        seconds -= 1;
      }
      if (seconds <= -1) {
        milisec = 0;
        seconds += 1;
      }
      else {
        milisec -= 1;
      }

      // Easterpack stuff.
      // Use last two timer digits to decide whether to display an egg.
      if(digits_2 === seconds ) {
        // Use last timer digit to decide whether to display an egg and which.
        if (digits_1 > 0 && digits_1 < 4) {
          var expire = digits_1;
          // USe timer digits to decide location of egg.
          document.getElementById("easter-egg-" + digits_1).style.left = digits_2 + "%";
          document.getElementById("easter-egg-" + digits_1).style.top = digits_2 + "%";
          document.getElementById("easter-egg-" + digits_1).style.display = "block";
          document.getElementById("easter-egg-" + digits_1).style.zIndex = "99";
        }
      } else {
        var eggs_to_hide = document.getElementsByClassName("easter-egg")
        for(var i = 0; i < eggs_to_hide.length; i++){
          eggs_to_hide[i].style.display = "none";
        }
      }
      // Easterpack stuff /end.

      var percentWidth = (seconds + milisec/10)/refreshRate * 100;
      document.getElementById("timer").style.width = percentWidth + "%";
      if (seconds === 0 && milisec === 0) {
        location.reload();
      }
      else {
        setTimeout("display()", 100);
      }
    }

    display();
  </script>
</body>
</html>
