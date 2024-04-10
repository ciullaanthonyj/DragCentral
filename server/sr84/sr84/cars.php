<?php
  include "whothis.php";
  include_once "money.php";
            
  parse_str($decryptquery); 
  // DON'T REMOVE IT IS $ACT INITIALISATION IN CASE DOUBLE CALLING OF THIS SCRIPT
  $act = $_GET['act'];
   
  if($act=='getfreeleft') {
    //IF PREVIOUSLY user_login WAS INCLUDED IN stack.php
    if(isset($login_userid))
      $userid=$login_userid;
    $res = queryone("SELECT count(id) FROM usercars
	   WHERE userid=$userid AND locked=0");
	echo "1|$res|";
	return;       
  }

  if($act=='getcarinfo') {
    $res = query("SELECT carid, cars.name FROM usercars 
                  LEFT JOIN cars ON cars.id=usercars.carid
        	      WHERE usercars.id=$usercarid");       
	echoquery($res);
    return;  
  }

  if(isset($getlist)) {
     unset($getlist);
     $res = query("SELECT id,name,price,if(membercar,'member car',''),TRUNCATE(weight/0.4536,0),horses 
	               FROM cars ORDER BY id");	 
	 echoquery($res);         
  }
  
  if($act=='getselected') {
    //IF PREVIOUSLY user_login WAS INCLUDED IN stack.php
    if(isset($login_userid))
      $userid=$login_userid;
    
    $id = queryone("SELECT carid FROM usercars WHERE userid=$userid AND selected=1");

    // if user have no selected car	give him shitty audi
	if(strlen($id)==0) {
      query("INSERT INTO usercars(userid,carid,selected) VALUES($userid,1,1)");
      $id = 1;	  
	}
	$usercarid = queryone("SELECT id FROM usercars WHERE userid=$userid AND selected=1"); 
    // not optimal two queries, gotta be one (but less complex instead)	
	$res = query("SELECT id,TRUNCATE(weight/0.4536,0),horses,$usercarid,name 
	              FROM cars WHERE id=$id");
	 
    if(!mysql_errno()) echoquery($res);
    return;
    
  }
  if(isset($buycar)) { unset($buycar);
    $money = UserFreeMoney($userid);
	$price = queryone("SELECT price FROM cars WHERE id=$carid");
	
	if($price>$money)  {
	  echo("You don't have enough money.");
	  return;
	}
	$member = queryone("SELECT member FROM users WHERE id=$userid");
	$membercar = queryone("SELECT membercar FROM cars WHERE id=$carid");
	if($membercar && !$member) {
	  echo "Car only for members.";
	  return;	  
	}
	
	query("INSERT INTO usercars(userid,carid) VALUES($userid,$carid)");
	ChangeMoney($userid,0,-$price,"Buy car");
    if(!mysql_errno()) { echo("Success"); return; }
  }
  
  if($act=='sellcar') {    
    $teamid = queryone("SELECT teamid FROM team_members 
	                WHERE usercarid=$usercarid AND userid=$userid");	                
    $price = queryone("SELECT TRUNCATE(price/2, 0)
	           FROM usercars LEFT JOIN cars ON carid=cars.id
	           WHERE userid=$userid AND usercars.id=$usercarid");
	           
	ChangeMoney($userid,0,$price,"Sell car");
	query("DELETE FROM carparts WHERE usercarid=$usercarid");
	query("DELETE FROM usercars WHERE userid=$userid AND id=$usercarid");
	// DELETE NOT ANSWERED CHALLENGES FOR THIS CAR
	query("DELETE FROM challenges WHERE touserid=$userid AND answerusercarid=$usercarid
	       AND answered=0");
	// DELETE NOT ACCEPTED CHALLENGES FOR THIS CAR
	query("DELETE FROM challenges WHERE userid=$userid AND usercarid=$usercarid
	       AND answered=0");
	

    if(strlen($teamid)!=0) {
       $team_included = 1;
	   $act='quitteam';       
       include "team.php";         
    }
	
    if(!mysql_errno()) { echo("Success"); return; }
   
  }


  if($act=='getusercars') {
    if($unlocked=="1") 
      $lockcondition = " AND usercars.locked=0 ";
    else
      $lockcondition = "";
    $res = query("SELECT carid,cars.name,TRUNCATE(price / 2,0) ,usercars.id, if(selected,'selected','garaged'),
                  TRUNCATE(weight/0.4536,0),horses,
                  if(team_members.id is null,'','team car'),
                  locked
	              FROM usercars 
				  LEFT JOIN cars ON carid=cars.id
				  LEFT JOIN team_members ON 
				  (usercars.userid=team_members.userid AND team_members.usercarid=usercars.id)
	              
	              WHERE usercars.userid=$userid $lockcondition 
				  ORDER BY usercars.id");
	echoquery($res);    
	return;
  }
  
  if(isset($registerlist)) {unset($registerlist);
    $res = query("SELECT id,name,TRUNCATE(weight/0.4536,0),horses FROM cars 
                  WHERE membercar=0
                  ORDER BY id");
	echoquery($res);     
  }
  
  if(isset($selectcar)) {unset($selectcar);
    query("UPDATE usercars SET selected=if(id=$usercarid,1,0) WHERE userid=$userid");
    if(!mysql_errno()) { echo("Success"); return; }
  }

  $FULLNITROCOST = 50;  
  // not only upgrade values, also maxspeed
  if($act=='getupgradevalues') { 
    $exists = query("SELECT id FROM usercars WHERE userid=$userid AND id=$usercarid");
    if(strlen($exists)==0) {
      echo ("Car doesn't belong to user");
      return;
    }
    $res = queryline("SELECT sum(if(parts.groupid=3,0,hp)) as addhp,
	             sum(slip) as slip,
				 sum(if(parts.groupid=3,hp,0)) as nitro
				 FROM carparts 
                 LEFT JOIN parts ON carparts.partid=parts.id
                 WHERE carparts.usercarid = $usercarid AND carparts.installed=1");
    if(!mysql_errno()) {
	   $addhp = $res['addhp'];
	   $slip = $res['slip'];
	   $nitro = $res['nitro'];
	
	   $addhp = is_null($addhp) ? 0 : $addhp;
	   $slip = is_null($slip) ? 0 : $slip;
	   $nitro = is_null($nitro) ? 0 : $nitro;
	  
	   $nitrosleft = queryone("SELECT nitrousleft FROM usercars WHERE id=$usercarid");   
	   $nitrosleft = Round($nitrosleft);
	   $nitroscost = (100-$nitrosleft) * $FULLNITROCOST / 100;
	   $nitroscost = Round($nitroscost);
	   
	   $turbocharger = queryone("SELECT installed 
	     FROM carparts WHERE usercarid=$usercarid AND partid=9");
	   $supercharger = queryone("SELECT installed 
	     FROM carparts WHERE usercarid=$usercarid AND partid=10");
	

       $magicgear = queryone("SELECT installed 
	     FROM carparts WHERE usercarid=$usercarid AND partid=11");
		 
	   $gears = queryone("SELECT gears
	     FROM carparts WHERE usercarid=$usercarid AND partid=11");	   

       $maxspeed = queryone("SELECT speednum
	     FROM cars LEFT JOIN usercars ON cars.id=usercars.carid 
		 WHERE usercars.id=$usercarid");	     
		 
	   $enginedamage = queryone("SELECT enginedamage FROM usercars 
	     WHERE  usercars.id=$usercarid");
	   $carprice = queryone("SELECT price FROM cars LEFT JOIN usercars ON carid=cars.id WHERE usercars.id=$usercarid");
	   $repaircost = $carprice*$enginedamage/7000; //also modify it in act=repairengine
	   $repaircost = (int)$repaircost;
	   echo "12|$addhp|$slip|$nitro|$nitrosleft|$nitroscost|$turbocharger|$supercharger|".
	        "$magicgear|$gears|$maxspeed|$enginedamage|$repaircost|";  
	}
  }
  
  if($act=='setmagicgear') {
    query("UPDATE carparts SET gears='$gears' WHERE
	  usercarid=$usercarid AND partid=11"); 
    
  }
  if(isset($refillnitrous)) {unset($refillnitrous);
    $exists = query("SELECT id FROM usercars WHERE userid=$userid AND id=$usercarid");
    if(strlen($exists)==0) {
      echo ("Car doesn't belong to user");
      return;
    }
    $nitrosleft = queryone("SELECT nitrousleft FROM usercars WHERE id=$usercarid");   
	$nitroscost = (100-$nitrosleft) * $FULLNITROCOST / 100;
	$nitroscost = Round($nitroscost);
    
	$money = UserFreeMoney($userid);
    if($nitroscost>$money) {    
	  echo ("You don't have enough money.");
	  return;
	}	
    ChangeMoney($userid,0,-$nitroscost,"Refill nitrous");
	
	query("UPDATE usercars SET nitrousleft = 100 WHERE id=$usercarid");
    if(!mysql_errno()) { echo("Success"); return; }
    
  }
  

  if($act=="resetcar") {
    $exists = query("SELECT id FROM usercars WHERE userid=$userid AND id=$usercarid");
    if(strlen($exists)==0) {
      echo ("Car doesn't belong to user");
      return;
    }

    query("DELETE FROM carparts WHERE usercarid=$usercarid");
	query("UPDATE usercars SET enginedamage=0, nitrousleft=100 WHERE id=$usercarid");

    if(!mysql_errno()) { echo("Success"); }
	return;
    
  }









  if($act=="repairengine") {
    $exists = query("SELECT id FROM usercars WHERE userid=$userid AND id=$usercarid");
    if(strlen($exists)==0) {
      echo ("Car doesn't belong to user");
      return;
    }

	   $enginedamage = queryone("SELECT enginedamage FROM usercars 
	     WHERE  usercars.id=$usercarid");
	   $carprice = queryone("SELECT price FROM cars LEFT JOIN usercars ON carid=cars.id WHERE usercars.id=$usercarid");
	   $repaircost = $carprice*$enginedamage/7000;
	   $repaircost = (int)$repaircost;
    
	$money = UserFreeMoney($userid);
    if($repaircost>$money) {    
	  echo ("You don't have enough money.");
	  return;
	}	
    ChangeMoney($userid,0,-$repaircost,"Repair Engine");
	
	query("UPDATE usercars SET enginedamage = 0 WHERE id=$usercarid");
    if(!mysql_errno()) { echo("Success"); return; }

    
    
  }
  
  if($act=="setnitrousanddamage") {
    $exists = query("SELECT id FROM usercars WHERE userid=$userid AND id=$usercarid");
    if(strlen($exists)==0) {
      echo ("Car doesn't belong to user");
      return;
    }
    query("UPDATE usercars SET nitrousleft=$nitrousleft, enginedamage=$damage WHERE id=$usercarid");         
  }
  
?>