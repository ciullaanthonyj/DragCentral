<?php
  if(!isset($team_included)) {
    include "whothis.php";
    parse_str($decryptquery); 
  } else {
    unset($team_included);
  }
  include_once "money.php";

  // TEAM JOIN/QUIT
  // TEAM MONEY  
 
  
  if($act=='create') {
    
    $time = time();
    $name = noquotes($name);
    
    $exists = queryone("SELECT id FROM teams WHERE name='$name'");
    
    if(strlen($exists)!=0) {
      echo("Team [$name] allready exists.");
      return;
    }
      
    query("INSERT INTO teams(name,creationtime,comment) VALUES('$name',$time,'')");
    
    $id = queryone("SELECT id FROM teams WHERE name='$name'");  
    $scar = queryone("SELECT id FROM usercars WHERE userid=$userid AND selected=1");
    query("INSERT INTO team_members(teamid,userid,owner,jointime,usercarid) VALUES($id,$userid,1,$time,$scar)");
    query("DELETE FROM team_requests WHERE userid=$userid");
    
    echo "1|$id|";     
    
  }
  
  if($act=='getlist') {
    // this is when first quering team
    if(isset($team_inteam))
    if($team_inteam) return;    
    if(strlen($except)==0)
      $except = -1;
    if(strlen($part)==0) $part = "";
    $part = noquotes($part);
    $res = query("SELECT id,name FROM teams WHERE id<>$except AND (name LIKE '%$part%') ORDER BY name LIMIT 100");     
    if(strlen($part)!=0)
      $part = "";
    echoquery($res);       
  }


  if($act=='getuserteam') {
     $res = query("SELECT teams.id,name,owner,usercars.id as usercarid,usercars.carid
	               FROM teams 
				   LEFT JOIN team_members ON teams.id=teamid 
	               LEFT JOIN usercars ON team_members.usercarid=usercars.id
				   WHERE team_members.userid=$userid");
	 if(mysql_num_rows($res)==0)
	   $team_inteam = 0;
 	 else {
 	   $team_inteam = 1;
  	   echoquery($res);    
  	 }
  }

  if($act=='quitteam') {
     $owner = queryone("SELECT owner  FROM team_members WHERE userid=$userid");
     query("DELETE FROM team_members WHERE userid=$userid AND teamid=$teamid");
     // DETECT IF IT WAS LAST MEMBER - TEAM EMPTY
     $count = queryone("SELECT count(userid) FROM team_members WHERE teamid=$teamid");
     if($count==0) {
       query("DELETE FROM teams WHERE id=$teamid"); 
       query("UPDATE team_challengers 
	          LEFT JOIN team_challenges ON team_challenges.id=team_challengers.challengeid
	          SET team_challengers.challengeid=-1
	          WHERE team_challenges.teamid=$teamid OR team_challenges.o_teamid=$teamid");
	   query("DELETE FROM team_challengers WHERE challengeid=-1");
	   query("DELETE FROM team_challenges WHERE teamid=$teamid OR o_teamid=$teamid");
	   query("DELETE FROM team_money WHERE teamid=$teamid");
	   query("DELETE FROM team_requests WHERE teamid=$teamid");
     } else {
       //SET NEW OWNER
       if($owner) {
          $longesttime=queryone("SELECT min(jointime) FROM team_members WHERE teamid=$teamid");
          $id=queryone("SELECT userid FROM team_members WHERE teamid=$teamid AND jointime=$longesttime");
          query("UPDATE team_members SET owner=1 WHERE userid=$id");		 
       }
     }

     //DELETE CREATED NOT ACCEPTED
     while(true) {
       $cid = queryone(
  	      "SELECT challengeid FROM team_challengers 
	       LEFT JOIN team_challenges ON challengeid=team_challenges.id
	       WHERE accepted=0 AND declined=0 AND 
		   (userid=$userid OR o_userid=$userid) LIMIT 1");
	    if(strlen($cid)==0) break;
        query("UPDATE team_challenges SET declined=1 WHERE id=$cid");      
      
        query("DELETE FROM team_challengers WHERE challengeid=$cid");
        query("DELETE FROM team_challenges WHERE id=$cid");	   
	 }
	 
     //RECORD AS FOULED ACTIVE CHALLENGES (COMPLEX AS HELL, SHITTY CARS)
     while(true) {
       $foulc = queryline(
  	      "SELECT team_challengers.id,if(userid=$userid,0,1) as answer,
  	       if(userid=$userid,
			  if(team_challengers.o_ended=1,1,0),
			  if(team_challengers.ended=1,1,0)
			 ) as otherraced,
  	       if(userid=$userid,o_foul,foul) as otherfoul
		   FROM team_challengers
	       LEFT JOIN team_challenges ON challengeid=team_challenges.id
	       WHERE accepted=1 AND team_challenges.ended=0 
		   AND 
		   (
		     (team_challengers.ended=0 AND userid=$userid) 
		     OR (team_challengers.o_ended=0 AND o_userid=$userid)
		   ) LIMIT 1");
       $cid = $foulc['id'];
       if(strlen($cid)==0) break;
       $answer = $foulc['answer']; 
       $init = 1-$answer;
       $otherraced = $foulc['otherraced'];
       $otherfoul = $foulc['otherfoul'];
       unset($win);
       unset($draw);
       if($otherraced) {
         $win=0;
         $draw=0;
         if($otherfoul==1)
           $draw=1;
       }
	   $reactiontime = 0;
	   $racetime = 0;
	   $brackettime = 0;
	   $foul = 1;         
       $team_challenge_included = 1;
       $act='saverace';
	   include "team_challenge.php";               
       unset($win);
       unset($draw);
     }
	        
	
	 echo "Success";
  }  

  if($act=='requestjoin') {
    $id = queryone("SELECT id FROM team_requests WHERE teamid=$teamid AND userid=$userid");
    if(strlen($id)==0) {
      query("INSERT INTO team_requests(teamid,userid,comment) 
	    VALUES($teamid,$userid,'')"); 
    } else {
	  query("UPDATE team_requests SET declined=0 WHERE id=$id"); 
	}    
    if(!mysql_errno())
  	 echo "Success"; 
  }

  if($act=='acceptjoin') {
    query("UPDATE team_requests SET accepted=1 WHERE id=$id AND teamid=$teamid");    
    if(!mysql_errno())
  	  echo "Success"; 
  }
  
  if($act=='declinejoin') {
    query("UPDATE team_requests SET declined=1 WHERE id=$id AND teamid=$teamid");
    if(!mysql_errno())
	  echo "Success";  
  }
  
  if($act=='joinaccepted') {
    $time = time();
    $teamid = queryone("SELECT teamid FROM team_requests WHERE id=$id AND userid=$userid");
    if(strlen($teamid)==0) {
      echo ("Error: no such request.");
      return;
    }
    query("DELETE FROM team_requests WHERE userid=$userid");    
    $scar = queryone("SELECT id FROM usercars WHERE userid=$userid AND selected=1");
    query("INSERT INTO team_members(teamid,userid,owner,jointime,usercarid) 
	       VALUES($teamid,$userid,0,$time,$scar)");
 	echo "Success"; 
}

  if($act=='userrequests') {   
    if(isset($team_inteam)) 
    if($team_inteam) return;
    $res = query("SELECT if(teams.id is null, '', team_requests.id), if(teams.name is null,'-deleted team-',teams.name),accepted,declined FROM team_requests
                  LEFT JOIN teams ON team_requests.teamid=teams.id 
				  WHERE userid=$userid");
    echoquery($res);    
  }
  
  if($act=='teamrequests') {
    $res = query("SELECT team_requests.id, users.nick FROM team_requests
                  LEFT JOIN users ON team_requests.userid=users.id 
				  WHERE teamid=$teamid AND accepted=0 AND declined=0");
    echoquery($res);        
  }
  
  if($act=='teammembers') {
    $res = query("SELECT team_members.userid,
	                     if(users.nick is null,'-deleted user-',users.nick),
						 owner, cars.id,cars.name
	              FROM team_members 
				  LEFT JOIN users ON team_members.userid=users.id				  
				  LEFT JOIN usercars ON team_members.usercarid=usercars.id
				  LEFT JOIN cars ON usercars.carid=cars.id
	              WHERE teamid=$teamid
				  ORDER BY owner DESC, users.nick"); 
    echoquery($res);    
    return;
  }
  
  if($act=='removemember') {
    query("DELETE FROM team_members WHERE userid=$userid AND teamid=$teamid");
    if(!mysql_errno()) 
	  echo "Success";     
  }
  
  if($act=='getbalance') {
    $res = TeamBalance($teamid);    
    echo("1|$res|");
    return;
  }

  if($act=='deposit') {
    $freemoney = UserFreeMoney($userid);
    if($value>$freemoney) {
      echo "You don't have enough money.";
      return;
    }
    
    ChangeMoney($userid,0,-$value,"team: deposit");
    ChangeTeamMoney($teamid,0,$userid,$value,"Deposit"); 
    if(!mysql_errno()) echo "Success";         
    return;
  }
  
  if($act=='gettransactions') {
    $res = 
      query("SELECT users.nick,value,teams.name,o_teamid,team_money.comment FROM team_money
	         LEFT JOIN users ON team_money.userid=users.id
	         LEFT JOIN teams ON o_teamid=teams.id
		     WHERE teamid=$teamid
			 ORDER BY team_money.id DESC
			 LIMIT 20"); 
    echoquery($res);    
    return;
  }
  
  if($act=='distribute') {
	ChangeMoney($userid,0,$value,"team: distribute");
	ChangeTeamMoney($teamid,0,$userid,-$value,"Distributed"); 
    if(!mysql_errno()) echo "Success";             
    return;
  }
  
  if($act=='getstats') {
    $res = query("SELECT wins,loses FROM teams WHERE id=$teamid");
    if(!mysql_errno()) echoquery($res);    
	return;      
  }

?>
