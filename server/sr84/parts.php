<?php
  include "whothis.php";
  include_once "money.php";
  parse_str($decryptquery);

  if($getlist) {
    $getlist=0;
    $exists = queryone("SELECT id FROM usercars WHERE userid=$userid AND usercars.id=$usercarid");
    if(strlen($exists)==0) { echo("Car doesn't belong to the user."); return; }
    
    $res = query("SELECT parts.id,partgroups.name as grp,parts.name as part,price,hp,slip,
	              if(carparts.installed is null,'','bought')
	             FROM parts 
	             LEFT JOIN partgroups ON parts.groupid=partgroups.id
	             LEFT JOIN carparts ON (carparts.usercarid=$usercarid AND carparts.partid=parts.id)
	             LEFT JOIN usercars ON carparts.usercarid=usercars.id
				 ORDER BY partgroups.name, parts.id");
    
    if(!mysql_errno()) echoquery($res);
    return;    
  }
  
  if($getcarparts) {
    $getcarparts=0;
    $res = query("SELECT carparts.id, partgroups.name as grp, parts.name as prt,
	              if(installed,'installed','not installed'),
				  parts.hp,parts.slip FROM
	              carparts 
				  LEFT JOIN parts ON carparts.partid=parts.id
				  LEFT JOIN partgroups ON parts.groupid=partgroups.id
				  LEFT JOIN usercars ON carparts.usercarid=usercars.id
				  WHERE carparts.usercarid=$usercarid AND usercars.userid=$userid
				  ORDER BY partgroups.name, parts.id");
    if(!mysql_errno()) echoquery($res);
    return;        
  }
  
  if($act=='buypart') {
    $money = UserFreeMoney($userid);
    $partprice = queryone("SELECT price FROM parts WHERE id=$partid");
	
	$installed = queryone("SELECT id FROM carparts 
	  WHERE usercarid=$usercarid AND partid=$partid");
	if(strlen($installed)!=0) {
	   echo "You have bought this part for this car allready."; return;
	}
	if($partprice>$money) { echo("You don't have enough money"); return; }
    
    $exist = queryone("SELECT id FROM usercars WHERE userid=$userid AND id=$usercarid");
    if(strlen($exist)==0)
      { echo("Car doesn't belong to the user."); return; }
    
    ChangeMoney($userid,0,-$partprice,"Bought car part");
	query("INSERT INTO carparts(usercarid,partid,installed) VALUES($usercarid,$partid,0)");

    $money = UserFreeMoney($userid);
    
    $group = queryone("SELECT groupid FROM parts WHERE id=$partid");
    //IF NITRO AUTO REFILL BOTTLE
    if($group==3) {
      query("UPDATE usercars SET nitrousleft=100 WHERE id=$usercarid");      
    }
    
    if(!mysql_errno()) echo "Success";
	
  }
  
  if($installpart) {
    $installpart=0;
    $exist = queryone("SELECT carparts.id FROM carparts 
                       LEFT JOIN usercars ON usercars.id=carparts.usercarid
	                   WHERE usercars.userid=$userid AND carparts.id=$usercarpartid");
	if(strlen($exist)==0)
	  { echo("No such part in user cars."); return; }

    if($install) {	
  	  $grp = queryone("SELECT parts.grp FROM carparts
  	                   LEFT JOIN parts ON carparts.partid=parts.id
  	                   WHERE carparts.id=$usercarpartid");
      if($grp!="0") {
    	  $usercarid = queryone("SELECT usercarid FROM carparts WHERE id=$usercarpartid");
    	  query("UPDATE carparts 
    	         LEFT JOIN parts ON partid=parts.id
		         SET installed=0 WHERE usercarid=$usercarid AND grp=$grp");
      }
  	  
  	}
	query("UPDATE carparts SET installed=$install WHERE id=$usercarpartid");
    if(!mysql_errno()) echo "Success";    
  }
?>