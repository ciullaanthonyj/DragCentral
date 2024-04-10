<?php

include "whothis.php";
include_once "money.php";

  $id = $_GET["id"];
  $userid = $_GET["userid"];
  $brackettime = $_GET["brackettime"];
  $racetime = $_GET["racetime"];
  $falsestart = $_GET["falsestart"];
  $reactiontime = $_GET["reactiontime"];
  $win = $_GET["win"];
  $act = $_GET["act"];
  $cid = $id;

  $draw = 0;
  // DRAW
  if($win==2) {
   $win=0;
   $draw=1;
  }

  if($act=='decline') {
    $forcar = queryone("SELECT forcar FROM challenges 
	   WHERE touserid=$userid AND id=$id");
	if($forcar) {
	  $lockedid = queryone("SELECT usercarid FROM challenges
	     WHERE touserid=$userid AND id=$id");
	  query("UPDATE usercars SET locked=0 WHERE id=$lockedid");
	}
    query("DELETE FROM challenges WHERE touserid=$userid AND id=$id"); 
    if(!mysql_errno()) echo "Success";    
    return;
  }

  if($act=='start') {
    
    $result = query("SELECT formoney,wager FROM challenges WHERE id=$id");
    $formoney = $line['formoney'];
    $wager = $line['wager'];
    if($formoney) {
      $money=queryone("SELECT money FROM users WHERE id=$userid");    
      if($wager>$money)
        die("(_._)");
    }

    // CREATE ANSWER START 
    $time = time();
    query("UPDATE challenges SET answerstart=1,answerstarttime=$time WHERE id=$cid AND answerstart=0 AND createend=1");

  }


  if($act=='end') {
    $result = query("
	   SELECT userid,touserid,formoney,wager,forcar,
	   usercarid,answerusercarid
	   FROM challenges WHERE id=$id");

    if(mysql_num_rows($result)==0)
      die("No such challenge.");

    $line = mysql_fetch_array($result, MYSQL_ASSOC);

    $formoney = $line['formoney'];
    $wager = $line['wager'];
    $userid = $line['userid'];
    $touserid = $line['touserid'];
    $forcar = $line['forcar'];
    $usercarid = $line['usercarid'];
    $answerusercarid =$line['answerusercarid'];
   
    $answerwin = $win;
    
    if(!$draw) {
      $userip = queryone("SELECT lastip FROM users WHERE id=$userid");
      $touserip = queryone("SELECT lastip FROM users WHERE id=$touserid");
      $sameip = $userip==$touserip;
      if(!$sameip && $answerwin) {
        $rank = queryone("SELECT rank FROM users WHERE id=$userid");
        $rankdiff = $rank/10;
        query("UPDATE users SET rank=rank+$rankdiff WHERE id=$touserid");
        query("UPDATE users SET rank=rank-$rankdiff WHERE id=$userid");
        
        query("UPDATE users SET wins=wins+1 WHERE id=$touserid");
        query("UPDATE users SET loses=loses+1 WHERE id=$userid");
      }
      
      if(!$sameip && !$answerwin) {
        $rank = queryone("SELECT rank FROM users WHERE id=$touserid");
        $rankdiff = $rank/10;
        query("UPDATE users SET rank=rank-$rankdiff WHERE id=$touserid");
        query("UPDATE users SET rank=rank+$rankdiff WHERE id=$userid");
        
        query("UPDATE users SET wins=wins+1 WHERE id=$userid");
        query("UPDATE users SET loses=loses+1 WHERE id=$touserid");        
      }   
      
    }

    if($formoney==1 && !$draw) {


       if($answerwin) {
         changemoney($userid,$touserid,-$wager,"initiator: lose");
         changemoney($touserid,$userid,+$wager,"answer: win");
       }
       
       if(!$answerwin) {
         changemoney($userid,$touserid,+$wager,"initiator: win");
         changemoney($touserid,$userid,-$wager,"answer: lose");
       }

    }

    // unlock initiator car      
    if($forcar==1)
      query("UPDATE usercars SET locked=0 WHERE id=$usercarid");
    
	if($forcar==1 && !$draw) {
      // move initiator car to winner - answerer
      if($answerwin) {
         $teamid = queryone("SELECT teamid 
		    FROM team_members WHERE usercarid=$usercarid");
		 // make initiator quit team
		 if(strlen($teamid)!=0) {
		     $userid=$userid;
             $team_included = 1;
      	     $act='quitteam';       
             include "team.php";         
		 } 
		 
	     query("UPDATE usercars SET userid=$touserid
		        WHERE id=$usercarid");
	  } 
	  // answerer lose              
      if(!$answerwin) {
	    
         $selected = queryone("SELECT selected FROM usercars
            WHERE id=$answerusercarid");
         if($selected) {
            // find other not locked
            $othercar = queryone("SELECT id FROM usercars 
			  WHERE userid=$touserid AND locked=0 AND id<>$answerusercarid");
   			// select it
			query("UPDATE usercars SET selected=1 
			  WHERE id=$othercar AND userid=$touserid");              
   			// unselect car that you lose
            query("UPDATE usercars SET selected=0 WHERE id=$answerusercarid");
         } 
         
         // move answerer car to challenge initiator        
	     query("UPDATE usercars SET userid=$userid
		        WHERE id=$answerusercarid");
		  
         // if you lose team
         $teamid = queryone("SELECT teamid FROM team_members
		   WHERE usercarid=$answerusercarid");
		 // quit team
		 if(strlen($teamid)!=0) {
		     $userid=$touserid;
             $team_included = 1;
      	     $act='quitteam';       
             include "team.php";         
         }		   

	  }               
	  
    }


    query("UPDATE challenges SET answerbrackettime=$brackettime, answerracetime=$racetime, 
              answerwin=$win, answered=1,answerfalsestart=$falsestart,answerreactiontime=$reactiontime,draw=$draw,
              answerend=1 WHERE id=$id AND answerstart=1;");
              
    return;
  }

?>