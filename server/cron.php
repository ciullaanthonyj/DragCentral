<?php

  function Optimize($table) {
  	if(rand(0,60)==0) {
      query("OPTIMIZE TABLE $table");  	
  	}
  }
  
  
  // wget -O /dev/null http://www.drag-central.com/gm/cron.php?doitfast
  if($_SERVER['REMOTE_ADDR']!='127.0.0.1')  
  if($_SERVER['QUERY_STRING']!='doitfast') die();
  
  include "connect.php";
  include "functions.php";
  include "settings.php";

  //optimize tables randomly
  Optimize("messages");
  Optimize("iptrack");
  Optimize("bm");
  Optimize("challenges");

  
  
  echo "delete old iptracking data<br>";

  
  //save only last week
  $time = time();
  query("DELETE FROM iptrack WHERE time<($time-24*3600*7)");

  //leave last two hours
  query("DELETE FROM bm WHERE timestamp<($time-2*3600)");
  
  echo "delete old messages<br>";
  
 // DELETE OLD MESSAGES
  $time=time();
  $ago = $time - $storemessagesfor;
  query("DELETE FROM messages WHERE time<$ago");
  
  
  echo "gathering rooms statistics<br>";
  
// ROOM STATISTICS

  $res = query("SELECT id FROM rooms");
  
while ($line = mysql_fetch_array($res, MYSQL_ASSOC)) {
     $room = $line['id'];

    
  $count = queryone("SELECT count(id) FROM users WHERE inroom=$room");

   $time = time();
   query("UPDATE rooms SET maxpeople=$count, maxpeopletime=$time 
   WHERE id=$room AND $count>maxpeople");


if($count>2) {
  $roomname = queryone("SELECT name FROM rooms WHERE id=$room");
  $maxtime = queryone("SELECT max(timestamp) FROM roomstats WHERE roomname='$roomname'");
  $now = time();
  //every hour
  if($now-$maxtime>60*60) {
    query("INSERT INTO roomstats(timestamp,roomname) VALUES ($now, '$roomname')");
    $maxtime = $now;
  }

  query("UPDATE roomstats SET count=$count WHERE count<$count 
         AND roomname='$roomname' AND timestamp=$maxtime");
}

}

//ONLINE STATISTICS
$now_online = queryone("SELECT COUNT(id) FROM online");
$now = time();
$maxtime = queryone("SELECT max(timestamp) FROM onlinestats");
if($now-$maxtime>60*60) {
 query("INSERT INTO onlinestats(timestamp) VALUES ($now)");
 $maxtime = $now; 
}

query("UPDATE onlinestats SET count=$now_online WHERE count<$now_online AND timestamp=$maxtime");


//SYSTEM MESSAGES
  function SendToEveryRoom($message) {
     $res = query("SELECT id,name FROM rooms");
     $time = time();
     while ($line = mysql_fetch_array($res, MYSQL_ASSOC)) {          
       $room = $line['id'];
	   $roomname = $line['name'];      
//       echo "Message [$message] sent to room [$roomname]<br>";
       query("INSERT INTO messages(nick,message,room,time)
  	    VALUES('%SYSTEM%','$message',$room,$time)");
  	}
  }

  $res = query("SELECT id, message,periodsec,lasttimesend FROM sysmess");

  $time = time();  
  while ($line = mysql_fetch_array($res, MYSQL_ASSOC)) {
     $lasttime = $line['lasttimesend'];
     $period = $line['periodsec'];
//     echo "$lasttime - lasttime $period - period $time - time<br>";
     if($lasttime+$period<$time) {
        query("UPDATE sysmess SET lasttimesend=$time
          WHERE id=".$line['id']."");
        
        SendToEveryRoom($line['message']);       
     }


 }
?>