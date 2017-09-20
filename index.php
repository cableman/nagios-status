<?php
  require_once('utils.inc');
  $file = 'data/status.dat';
	$refreshvalue = 30;
?>

<html>
<body>
<title>Dashboard</title>
<link type="text/css" rel="stylesheet" href="style.css" media="all">
</style>

<div class="center">
<h1>Refresh in <span class="secs"></span></h1>
<span style="font-size:14pt;"><?php echo date('Y-m-d H:i:s'); ?></span>
</div>

  <script>
    var milisec = 0;
    var seconds = <?php echo($refreshvalue); ?>;
    var el = document.querySelector(".secs");
    el.innerHTML = '<?php echo($refreshvalue); ?>';

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
      el.innerHTML = seconds + "." + milisec + "s";

      if (seconds === 0 && milisec === 0) {
         location.reload();
      }
      else {
        setTimeout("display()", 100);
      }
    }

    display()
  </script>

  <table width="100%" border="0" align="center">
    <tr>
      <th width="160">Last Checked</th>
      <th width="140">Host</th>
      <th width="290">Status Info</th>
      <th width="150">Service</th>
    </tr>

    <?php showNotifications($file) ?>

  </table>
</body>
</html>
