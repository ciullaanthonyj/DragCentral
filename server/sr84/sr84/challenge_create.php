<?php

include "whothis.php";

  parse_str($decryptquery); 
   
  
  $time = time();

  if($act=='getuserid') {
    $nick = noquotes($nick);
    $id = queryone("SELECT id FROM users WHERE nick='$nick'");	
    echo "1|$id|";
	return;     	
  }  
  
  if($act=='start') {

    $money=queryone("SELECT money FROM users WHERE id=$userid");
    if($wager>$money) {
      echo("You don't have enough money.");
      return;
  }
    
	$message = mysql_real_escape_string($message);
    query("INSERT INTO challenges(userid,touserid,bracket,forfun,formoney,forcar,
	         wager,headsup,creationtime,createstart,usercarid,answerusercarid,message)
             values($userid,$touserid,$bracket,$forfun,$formoney,$forcar,$wager,
			 $headsup,$time,1,$usercarid,$answerusercarid,'$message')");
    $cid = queryone("SELECT max(id) FROM challenges WHERE userid=$userid AND touserid=$touserid AND createend=0");
    
    echo "1|$cid|";

  }

  if($act=='end') {
    $cid = $_GET["cid"];
    query("UPDATE challenges SET 
	        brackettime=$brackettime,
			racetime=$racetime,
			reactiontime=$reactiontime,
			falsestart=$falsestart,
			createend=1
        WHERE id=$cid AND createend=0");
    
    $pinkslip = queryone("SELECT forcar FROM challenges WHERE id=$cid");
    
    //SELECT OTHER CAR
    if($pinkslip) {
      $userid = queryone("SELECT userid FROM challenges WHERE id=$cid");
      $usercarid = queryone("SELECT usercarid FROM challenges WHERE id=$cid");
	  query("UPDATE usercars SET selected=0, locked=1 WHERE id=$usercarid");
	  $newsid = queryone("SELECT id FROM usercars 
	      WHERE  selected=0 AND locked=0 AND userid=$userid AND id<>$usercarid
		  LIMIT 1");
	  query("UPDATE usercars SET selected=1 WHERE id=$newsid");	
	}	    
    
  }

?>