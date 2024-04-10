<?php

include "whothis.php";

  $time=time();
  $nick   = $_GET["nick"];


  if(strlen($nick)==0) die("No nick given");
  


  // FIND OUT IN WHICH ROOM NOW
  $result = query("SELECT inroom FROM users WHERE nick='$nick'");
  while ($line = mysql_fetch_array($result, MYSQL_ASSOC)) {
    foreach ($line as $col_value) 
      $fromroom = $col_value;       
  }

  // QUIT ROOM MESSAGE
  if ($fromroom!=-1) {
    query("INSERT INTO messages(nick,message,room,time) values('$nick','%QUIT%','$fromroom',$time);");  
  }

  query("UPDATE users SET users.inroom = -1 WHERE nick='$nick';");

?>