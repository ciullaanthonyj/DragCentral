<?php
ini_set('zlib.output_compression_level', 9);
ob_start("ob_gzhandler");
?>
<?php
include "whothis.php";

include "../functions.php";
include "../connect.php";
$rnd = microtime_float();
$day = $_GET['day'];

   echo "<font face=verdana>";
   echo "MONEY TRANSACTIONS<br>";
   echo "<font size=0> <a href=admin.php?rnd=$rnd><< back to admin</a><br>";
   
   if(strlen($day)!=0) {
     echo "<font size=0> <a href=?rnd=$rnd>other day</a><br>";
     htmlqueryID(query("
       SELECT money.id as moneyid, userid as id, user1.nick as user, value,comment,startmoney,endmoney,time,user2.nick as opponent 
       FROM money LEFT JOIN users as user1 ON userid=user1.id
                  LEFT JOIN users as user2 ON opponentid=user2.id
        WHERE (time>=$day) AND (time<($day +3600*24))                  
                  ORDER BY moneyid "), 
  	  "<a href=admin.php?act=modifyuser&id=ID&rnd=$rnd>show user</a>");  
     
   }
   
   if(strlen($day)==0) { 
    htmlqueryID(query("SELECT (time - time%(3600*24)) as day, (time - time%(3600*24)) as id FROM money GROUP BY day"),
	  "<a href=?day=ID&rnd=$rnd>show for this day</a>");
   }
   
     


?>