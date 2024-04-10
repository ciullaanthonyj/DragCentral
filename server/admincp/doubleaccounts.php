<?php


include_once "backtoadmin.php";

   echo "DOUBLE ACCOUNTS<br>";
   parse_str($_SERVER['QUERY_STRING']);
   $rnd = "&r=".microtime_float();
   
   if($show=='theip') {
     htmlqueryID(query("SELECT users.id, ip, users.nick, MAX(iptrack.time) as last_time FROM iptrack 
	   LEFT JOIN users ON users.id=userid WHERE ip='$ip' GROUP BY users.id, users.nick "),
	   "<a href=admin.php?act=modifyuser&id=ID$rnd>User Details</a>");
	 return;     
   }


      htmlqueryID(query("SELECT ip as id, ip, count(DISTINCT userid) as Different_Accounts from iptrack
	              group by ip 
	              order by Different_Accounts desc limit 100"),
	   "<a href=doubleaccounts.php?show=theip&ip=ID&rnd=$rnd>Show loged in from this ip</a>");
   


  
?>