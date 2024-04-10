<?php

  include "whothis.php";
  include_once "money.php";
  
  $id = $_GET["id"]; // userid
  $wager = $_GET["wager"];
  $win = $_GET["win"];
  $cid = $_GET["cid"]; //challengeid
  $act = $_GET["act"];

  $draw = 0;
  if($win==2) {
    $win=0;
    $draw=1;    
  }
  

if($act=='start') {
  $time = time();
  // PROCESS TOO OLD challenges AND substract them from USER account
  // actually this must be on some cron or login

  include "challenge.php";
  ProcessOldCompChallenges($id);

  // CREATE NEW
  query("INSERT INTO compchallenges(userid,wager,timestamp) VALUES($id,$wager,$time)");
  $cid = queryone("SELECT max(id) FROM compchallenges WHERE userid=$id");

  //return new created challenge id
  die("1|$cid|");  

}

if($act=='end') {

  $wager = queryone("SELECT wager FROM compchallenges WHERE id=$cid AND userid=$id");

  // if exists Challenge ID
  if(strlen($wager)!=0) {

    if(!$draw) {

      if(!$win)
         changemoney($id,0,-$wager,"lose to computer");
      if($win)
         changemoney($id,0,$wager,"win computer");           

   }
   query("DELETE FROM compchallenges WHERE id=$cid");

 }

}

?>