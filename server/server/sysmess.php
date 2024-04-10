<?php

include "whothis.php";

  parse_str($decryptquery); 
  UpdateUserOnline($userid);

  $incoming_challenges = queryone("SELECT count(id) 
        FROM challenges 
        WHERE (touserid=$userid) and (answered=0) and (createend=1)");

  $incoming_team_join_requests = 0;
  $active_team_challenges = 0;
  
  
  //GAME BAN CHECK

  $line = queryline("SELECT (timestamp+length) as until,reason FROM bans 
			WHERE userid=$userid AND game=1");
  $res = $line['until'];
  $reason = $line['reason'];

  if(strlen($res)!=0) {
    $res = $res - time();  
    $res = TimePeriod($res);
    $nick = queryone("SELECT nick FROM users WHERE id=$userid");
    $kick_to_login_message =
	"Sorry, $nick. You have been game banned. Reason: $reason. \n$res until unban.";
  } else {
    $kick_to_login_message = "";
  }
  
          
  echo "4|".
  "$incoming_challenges|".
  "$incoming_team_join_requests|".
  "$kick_to_login_message|".
  "$active_team_challenges|".
  "";  

  
?>