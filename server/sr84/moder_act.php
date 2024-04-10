<?php

include "whothis.php";

  $time=time();
  $act = $_GET["act"];


  if($act=="chatkick" || $act=="chatban" || $act=="gameban") {
    

   // GOT IN ROOM -1 AND SEND MESSAGE %KICKED%
      $moderid   = $_GET["moderid"];
      $userid    = $_GET["userid"];
      $reason    = $_GET["reason"];


      $moder = queryone("SELECT moder FROM users WHERE id=$moderid");
      $admin = queryone("SELECT admin FROM users WHERE id=$moderid");

   // CHECK PERMISSION      
      if(!($moder || $admin))
        die("Error: you have no moderator permissions.");
  
   // GET USER NICK
       $usernick = queryone("SELECT nick FROM users WHERE id=$userid");
       $usernick = str_replace("'","\'",$usernick);

   // GET MODER NICK
       $modernick = queryone("SELECT nick FROM users WHERE id=$moderid");
       $modernick = str_replace("'","\'",$modernick);

    // FIND OUT IN WHICH ROOM NOW
       $roomid   = queryone("SELECT inroom FROM users WHERE id=$userid");

  
      if($act=="chatkick") {
        // IF USER IN CHAT
        if($roomid!=-1 && strlen($roomid)!=0) 
          query("INSERT INTO messages(nick,message,room,time) 
		    values('$usernick','%KICKED%$reason - by $modernick','$roomid',$time);");  
		    
        query("INSERT INTO banhistory(userid,moderid,reason,timestamp,chatkick)
          VALUES($userid,$moderid,CONCAT('Game - ','$reason'),$time,1)");
        
      }

      if($act=="chatban") {
        $length = $_GET["length"];
        $period = TimePeriod($length);
        // IF USER IN CHAT
        if($roomid!=-1 && strlen($roomid)!=0) 
          query("INSERT INTO messages(nick,message,room,time) 
		    values('$usernick','%KICKEDBANED%$period. $reason - by $modernick',
		    '$roomid',$time);");  

        
        query("INSERT INTO bans(userid,moderid,timestamp,length,reason,chat) 
		  VALUES($userid,$moderid,$time,$length,'$reason',1);");
        
        query("INSERT INTO banhistory(userid,length,moderid,reason,timestamp,chatban)
          VALUES($userid,$length,$moderid,CONCAT('Game - ','$reason'),$time,1)");
      }

      if($act=="gameban") {
        $length = $_GET["length"];
        $period = TimePeriod($length);
        // IF USER IN CHAT
        if($roomid!=-1 && strlen($roomid)!=0) 
          query("INSERT INTO messages(nick,message,room,time) 
  		    values('$usernick','%KICKEDGAMEBANED%$period. $reason - by $modernick',
		    '$roomid',$time);");  

      
        query("INSERT INTO bans(userid,moderid,timestamp,length,reason,game) VALUES($userid,$moderid,$time,$length,'$reason',1);");
        query("INSERT INTO banhistory(userid,length,moderid,reason,timestamp,gameban)
          VALUES($userid,$length,$moderid,CONCAT('Game - ','$reason'),$time,1)");
      }

        
    // SEND HIM IN OUT ROOM
    query("UPDATE users SET users.inroom=-1 WHERE id=$userid;");


    echo "Succes"; // user kicked
  }


?>
