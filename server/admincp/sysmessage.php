<?php


  include_once "whothis.php";

include_once "../connect.php";
include_once "../functions.php";

   echo "<font face=verdana>";
   echo "SEND MESSAGE TO EVERY ROOM (ONE TIME)<br>";
   echo "<font size=0> <a href=admin.php?rnd=$rnd><< back to admin</a><br>";
   
//  echo "<font face=tahoma size=2>";
  parse_str($_SERVER['QUERY_STRING']);
  $rnd = "&r=".microtime_float();

echo
     "<FORM action=?>
     Message text:<input type=text name=message value='$message'>
     <input type=hidden name=act value=sendmessage>
     <input type=hidden name=r value=$rnd>
     <input type=submit value='  Process  '>
	 </FORM>
	 " ;

  function SendToEveryRoom($message) {
     $res = query("SELECT id,name FROM rooms");
     $time = time();
     while ($line = mysql_fetch_array($res, MYSQL_ASSOC)) {          
       $room = $line['id'];
	   $roomname = $line['name'];      
       echo "Message [$message] sent to room [$roomname]<br>";
       query("INSERT INTO messages(nick,message,room,time)
  	    VALUES('%SYSTEM%','$message',$room,$time)");
  	}
  }
  
  if($act=='sendmessage') {
     $message = str_replace("\'","'",$message);
     SendToEveryRoom($message);
   
  }
  
?>