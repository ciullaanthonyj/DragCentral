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
  echo "30 biggest user accounts";
  htmlqueryID(query("SELECT id, nick, money, 
    if(member,'member','') as membership,
    if(moder,'moder','') as moderator
    FROM users ORDER BY money DESC LIMIT 30"),
	"<a href=admin.php?act=modifyuser&id=ID&rnd=$rnd>Details</a>");
  
?>