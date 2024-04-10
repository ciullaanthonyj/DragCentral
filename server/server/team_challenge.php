<?php
  if(!isset($team_challenge_included)) {
    include "whothis.php"; 
    parse_str($decryptquery);
  } else {
    unset($team_challenge_included);        
  }
  include_once "money.php";
  
  if($act=='startrace') {
    $time = time();
    if($init) { $prefix = ""; $other="o_"; }
    if($answer) { $prefix = "o_"; $other=""; }
    $started = queryone("SELECT {$prefix}started FROM team_challengers WHERE id=$cid");
    if($started) {
      echo "You didn't finish this race previously, writen as foul.";        
      $act='saverace';
      $reactiontime=-1;
      $racetime=-1;
      $brackettime=-1;
      $foul=1;      
      $l = queryline("SELECT {$other}ended as ended,{$other}foul as foul 
	                  FROM team_challengers WHERE id=$cid");
      if($l['ended']) {
        $win = 0;
        $draw = $l['foul'] ? 1 : 0;
      }
    } else {
      query("UPDATE team_challengers SET {$prefix}started=1, {$prefix}startedtime=$time WHERE id=$cid");  
  	  if(!mysql_errno()) die("Success");        
  	}
  }
  
 
  if($act=='saverace') {
    if($init) { $prefix = ""; $other="o_"; }
    if($answer) { $prefix = "o_"; $other=""; }
    
    query("UPDATE team_challengers SET 
	        {$prefix}reactiontime=$reactiontime,
	        {$prefix}racetime=$racetime,
	        {$prefix}brackettime=$brackettime,
	        {$prefix}foul=$foul,
	        {$prefix}ended=1	        
	       WHERE id=$cid");           

	// IF ANSWERING
	if(isset($win)) {
	  if($draw)
	    $otherwin = 0;
  	  else
	    $otherwin = 1-$win;
	  query("UPDATE team_challengers SET
	     {$prefix}win=$win,
	     {$other}win={$otherwin},
	     draw=$draw WHERE id=$cid");  

      // DETECT WHOLE RACE WINER	     
      $headerid = queryone("SELECT challengeid FROM team_challengers WHERE id=$cid");
	  $left = queryone("SELECT count(id) FROM team_challengers 
              WHERE challengeid=$headerid AND (ended=0 OR o_ended=0)");
      if($left==0) {
        $wins = queryone("SELECT count(id) FROM team_challengers 
              WHERE challengeid=$headerid AND {$prefix}win=1");
        $o_wins = queryone("SELECT count(id) FROM team_challengers
              WHERE challengeid=$headerid AND {$other}win=1");
        $win = $wins>$o_wins ? 1 : 0;
        $o_win = $wins<$o_wins ? 1 : 0;
        $draw = $wins==$o_wins ? 1 : 0;
        
        if(!$draw) {
  		  $l = queryline("SELECT formoney,wager,{$prefix}teamid as yourteam, {$other}teamid as otherteam
		                  FROM team_challenges WHERE id=$headerid");       
  		  $formoney = $l['formoney'];
		  $wager = $l['wager'];
		  $yourteam = $l['yourteam'];
		  $otherteam = $l['otherteam'];
		  
		  if($win) {
  		    query("UPDATE teams SET wins=wins+1 WHERE id=$yourteam");
  		    query("UPDATE teams SET loses=loses+1 WHERE id=$otherteam");
  		  }
  		  
  		  if($o_win) {
  		    query("UPDATE teams SET wins=wins+1 WHERE id=$otherteam");
  		    query("UPDATE teams SET loses=loses+1 WHERE id=$yourteam");  
  		  }
		  
		  
		  if($formoney) {
		     if($win) {
  		       ChangeTeamMoney($yourteam,$otherteam,0,$wager,"Challenge win");
			   ChangeTeamMoney($otherteam,$yourteam,0,-$wager,"Challenge lose");
			 }
			 
			 if($o_win) {
			   ChangeTeamMoney($yourteam,$otherteam,0,-$wager,"Challenge lose");
			   ChangeTeamMoney($otherteam,$yourteam,0,$wager,"Challenge win"); 
			 }
			 
		  }
		   
		}
		
        query("UPDATE team_challenges SET ended=1, draw=$draw, {$prefix}win=$win, {$other}win=$o_win WHERE id=$headerid");      
        
      }
	     
	}
    
  }
  
  if($act=='getresults') {
    if($init) $prefix = "";
    if($answer) $prefix = "o_";

    echoquery(    
  	  query("SELECT {$prefix}reactiontime,{$prefix}racetime,{$prefix}brackettime,{$prefix}foul 
	         FROM team_challengers WHERE id=$cid")
	);    
  }
  
  if($act=='answer') {
    if($accept) {
      $l = queryline("SELECT formoney,wager FROM team_challenges WHERE id=$cid AND o_teamid=$teamid");      
      $formoney = $l['formoney'];
      $wager = $l['wager'];
      
      $money = TeamBalance($teamid);
      if($wager>$money)
        die("Team doesn't have enough money. Team account: $money.");
      
      query("UPDATE team_challenges SET accepted=1 WHERE id=$cid AND o_teamid=$teamid");
    }
    
    if($decline) {
      query("UPDATE team_challenges SET declined=1 WHERE id=$cid AND o_teamid=$teamid");      
      
      query("DELETE FROM team_challengers WHERE challengeid=$cid");
      query("DELETE FROM team_challenges WHERE id=$cid AND o_teamid=$teamid");
    }
	if(!mysql_errno()) die("Success");    
  }

  // '',raced,win,draw
  if($act=='getchallengers') {
    $results = "";	          
    if($type=='completed') {
	   $results = ", CONCAT('4x',reactiontime,'x',racetime,'x',brackettime,'x',foul,'x'),
	                 CONCAT('4x',o_reactiontime,'x',o_racetime,'x',o_brackettime,'x',o_foul,'x') ";
	}	          
    echoquery(
	  query("SELECT team_challengers.id, 
	                userid, user.nick as usernick, 
					if(ended,  if(draw or win or o_win, if(draw,'draw',if(win,'win','lose')), 'raced'),'waiting'),
	                o_userid, user2.nick as o_usernick, 
					if(o_ended,  if(draw or win or o_win, if(draw,'draw',if(o_win,'win','lose')), 'raced'),'waiting')
					$results
	         FROM team_challengers
	         LEFT JOIN users as user ON user.id=userid
	         LEFT JOIN users as user2 ON user2.id=o_userid
	         WHERE team_challengers.challengeid=$cid") );     
  }
  if($act=='getlist') {
    
    $props = "if(bracket,'Bracket',if(headsup,'Heads Up','error')),if(forfun,'For Fun',
	          if(formoney,CONCAT('For Money ', wager),'error'))";
    if($incoming) {
     $res=query("SELECT team_challenges.id,teamid,teams.name as teamname, $props FROM
	          team_challenges 
			  LEFT JOIN teams ON teams.id = teamid
			  WHERE created=1 AND (o_teamid=$teamid) AND (accepted=0) AND (declined=0) AND (ended=0)
			  ORDER BY team_challenges.id DESC");	
	}
	
    if($outgoing) {
     $res=query("SELECT team_challenges.id,teamid,teams.name as teamname, $props FROM
	          team_challenges 
			  LEFT JOIN teams ON teams.id = o_teamid
			  WHERE created=1 AND (teamid=$teamid) AND (accepted=0) AND (declined=0) AND (ended=0)
			  ORDER BY team_challenges.id DESC");	
	}
			  
    if($active) {
     $res=query("SELECT team_challenges.id,if(teamid=$teamid,o_teamid,teamid),
	             if(teamid=$teamid,o_teams.name,teams.name), $props FROM
	          team_challenges 
			  LEFT JOIN teams ON teams.id = teamid
			  LEFT JOIN teams AS o_teams ON o_teams.id = o_teamid
			  WHERE created=1 AND (teamid=$teamid OR o_teamid=$teamid) AND (accepted=1) AND (ended=0)
			  ORDER BY team_challenges.id DESC");	
	}
	
    if($completed) {
     $res=query("SELECT team_challenges.id,if(teamid=$teamid,o_teamid,teamid),
	             if(teamid=$teamid,o_teams.name,teams.name), $props, 
				 if(draw,'Draw',if(win,if(teamid=$teamid,'You win','You lose'),if(teamid=$teamid,'You lose','You win'))) 
				 FROM
	          team_challenges 
			  LEFT JOIN teams ON teams.id = teamid
			  LEFT JOIN teams AS o_teams ON o_teams.id = o_teamid
			  WHERE created=1 AND (teamid=$teamid OR o_teamid=$teamid) AND (accepted=1) AND (ended=1)
			  ORDER BY team_challenges.id DESC");	
	}
	
	if(!mysql_errno()) echoquery($res);
  }
   
  if($act=='create') {

    $time = time();    
    query("INSERT INTO team_challenges(teamid,o_teamid,bracket,headsup,forfun,formoney,wager,creationtime)
       VALUES($teamid, $o_teamid,$bracket,$headsup,$forfun,$formoney,$wager,$time)");
       
    $id=queryone("SELECT max(id) FROM team_challenges WHERE created=0 AND teamid=$teamid");
    
    // u - user
    // c - car
    // challengers - "u1c1u2c2u3c3  
    $challengers = str_replace('u','&users[]=',$challengers);
    $challengers = str_replace('c','&cars[]=',$challengers);
 
    parse_str($challengers);
  
    for($i=0;$i<count($users);$i+=2) {     
      $user = $users[$i];
      $o_user = $users[$i+1];
      $car = $cars[$i];
      $o_car = $cars[$i+1];
      query("INSERT INTO team_challengers(challengeid,userid,usercarid,o_userid,o_usercarid)
             VALUES($id, $user,$car,$o_user,$o_car)");
    } 
    
    query("UPDATE team_challenges SET created=1 WHERE id=$id");
    if(!mysql_errno()) 
	  echo "Success";     
  }

?>