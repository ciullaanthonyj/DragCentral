<?php

include "whothis.php";

  include "../settings.php";

// DELETE ROOMS THAT WAS NOT VISITED TOO LONG
  $ago=time()-$killroomafter;
  query("DELETE FROM rooms WHERE lastvisittime<$ago AND permanent<>1");


  $userid = $_GET["userid"];


// check if this user is not chat baned

  // first remove expired bans
  $time = time();
  query("DELETE FROM bans WHERE (timestamp+length)<$time AND chat=1");

  // query expiration time in seconds
  $banid = queryone("SELECT id FROM bans WHERE userid=$userid AND chat=1"); 
  
  if(strlen($banid)!=0) {    
 
    $res = queryone("SELECT (timestamp+length) FROM bans WHERE id=$banid");
    $reason = queryone("SELECT reason FROM bans WHERE id=$banid");
    $nick = queryone("SELECT nick FROM users WHERE id=$userid");
    $res = $res - time();  
    $res = TimePeriod($res);
    die("Sorry, $nick. You have been chat banned.\nReason: $reason\n$res until unban.");
  }


// user

  $result = query(
           "SELECT rooms.id, rooms.name, Count(users.nick),not (rooms.pass is null or rooms.pass='') ".
           "FROM rooms LEFT JOIN users ON rooms.id = users.inroom and users.invisible=0 ".
           "GROUP BY rooms.name ".
           "ORDER BY rooms.id ;");


echoquery($result);

?>