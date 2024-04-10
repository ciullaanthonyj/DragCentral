<?php
ini_set('zlib.output_compression_level', 9);
ob_start("ob_gzhandler");
  include "whothis.php";
?>

<FORM action=?>
<TEXTAREA ROWS=12 COLS=110 NAME="query"><?php echo str_replace("\'","'",$_GET['query']);?></TEXTAREA><BR>
<input type=submit   value='Process'>
</FORM>

<?php

  include "../functions.php";
  include "../connect.php";

  $query = $_GET['query'];  
  if(strlen($query)==0) die();
  $query = str_replace("\'","'",$query);
  $res = query($query);

  $endtime = round(microtime_float()*1000);
  $endtime = $endtime - $scriptstarttime;
  echo "<br>Server Execute Time(msecs):$endtime<br>";
  htmlquery($res);
  
  $scriptstarttime = round(microtime_float()*1000);
  
?>