<?php

include "whothis.php";
include_once "../settings.php";

// returns last max message for this room

  $time=time();
  $nick   = $_GET["nick"];
  $roomid = $_GET["room"];
  $pass   = $_GET["pass"];

  $nick = noquotes($nick);
  $time = time();

  if(strlen($nick)==0) die("No nick given");
  if(strlen($roomid)==0) die("No room given");
  
// REMOVE USERS THAT ARE LOGED OUT AND NOT ACTIVE IN CHAT FOR SOME TIME
$ago = time()-$showinroomfor;
query("INSERT INTO messages(nick,message,room,time) 
       SELECT users.nick,'%QUIT%',rooms.id,$time 
       FROM users 
       INNER JOIN rooms ON users.inroom=rooms.id
	   WHERE lastcheckmessagetime<$ago AND inroom=$roomid");  
query("UPDATE users SET users.inroom = -1 WHERE lastcheckmessagetime<$ago AND inroom=$roomid;");
  
  // CHECK PASSWORD
  $roompass = queryone("SELECT pass FROM rooms WHERE id=$roomid");

  if(strlen($roompass)!=0) {
    if($roompass!=$pass)
      die("Error: wrong password.");
  }
  

  // FIND OUT IN WHICH ROOM NOW
  $fromroom = queryone("SELECT inroom FROM users WHERE nick='$nick'");

  // QUIT ROOM MESSAGE
  if ($fromroom!=-1 && strlen($fromroom)!=0) {
    query("INSERT INTO messages(nick,message,room,time) values('$nick','%QUIT%','$fromroom',$time);");  
  }

  query("UPDATE users SET lastcheckmessagetime=$time WHERE nick='$nick'");
  query("UPDATE users SET users.inroom = $roomid WHERE nick='$nick';");
  

// return LastMaxMess
  $res = query("SELECT id FROM messages where room=$roomid  
          and message<>'%ENTER%' and message<>'%QUIT%'
          ORDER BY id DESC LIMIT 12");
  // show last 10 non private messages in room
  while ($line = mysql_fetch_array($res, MYSQL_ASSOC)) {
      $result = $line['id'];
  }          
    
  echo $result;      

  // ENTER ROOM MESSAGE
  query("INSERT INTO messages(nick,message,room,time) values('$nick','%ENTER%','$roomid',$time);");  
 
  // UPDATE LAST VISIT TIME
  query("UPDATE rooms SET lastvisittime=$time WHERE id=$roomid");




?>