<?php
  include "whothis.php";
  parse_str($decryptquery); 
  include_once "../settings.php";
  
  if($act=='post') {
     $message = noquotes($message);
     $nick = queryone("SELECT nick FROM users WHERE id=$userid");
     $nick = noquotes($nick);
     $time = time();
     query("INSERT INTO team_messages(userid,teamid,nick,time,message)
       VALUES($userid,$teamid,'$nick',$time,'$message')");
     return;   
  }
  
  if($act=='edit') {
     // some extra parameters for stupid extra protection from extra clever asshole hackers
     $message = noquotes($message);
     query("UPDATE team_messages SET message='$message' WHERE
	    id=$id AND userid=$userid AND teamid=$teamid");
     return;    
  }
  
  if($act=='del') {
     // some extra parameters for stupid extra protection from extra clever asshole hackers
     $owner=queryone("SELECT owner FROM team_members 
	   WHERE userid=$userid AND teamid=$teamid");
     query("DELETE FROM team_messages 
	        WHERE id=$id AND teamid=$teamid AND (userid=$userid OR $owner)");
     return;    
  }
  
  if($act=='get') {
    $time = time();
    $ago = $time - $store_team_messages;
    query("DELETE FROM team_messages WHERE teamid=$teamid AND time<$ago");
    echoquery(query("SELECT id,userid,nick,$time-time,message
      FROM team_messages WHERE teamid=$teamid ORDER BY time DESC"));    
  }

?>