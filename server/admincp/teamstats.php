<?php
ini_set('zlib.output_compression_level', 9);
ob_start("ob_gzhandler");
  include "whothis.php";
?>
<?php
  echo "<font face=verdana>";
  include "../functions.php";
  include "../connect.php";
  $rnd  = microtime_float();

  // biggest accounts  
  echo "30 first teams with biggest accounts";
  htmlqueryID(query("SELECT id, name, money 
    FROM teams ORDER BY money DESC LIMIT 30"),
	"<a href=teams.php?act=info&id=ID&rnd=$rnd>Details</a>");
  
?>