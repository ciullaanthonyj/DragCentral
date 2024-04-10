<?php
  include "whothis.php";
  include "../functions.php";
  include "../connect.php";
 
  parse_str($_SERVER['QUERY_STRING']); 
  echo "<font face=verdana size=1>";
  $rnd = microtime_float();
  echo "<a href=admin.php?rnd=$rnd><< back to admin</a> ::";
  echo "<a href=teamstats.php?rnd=$rnd>Biggest team money accounts</a><br>";

  if($act=='delete') {
      echo "<br>DELETING<br>";
      $teamid=$id;
       query("DELETE FROM teams WHERE id=$teamid"); 
       query("UPDATE team_challengers 
	          LEFT JOIN team_challenges ON team_challenges.id=team_challengers.challengeid
	          SET team_challengers.challengeid=-1
	          WHERE team_challenges.teamid=$teamid OR team_challenges.o_teamid=$teamid");
	   query("DELETE FROM team_challengers WHERE challengeid=-1");
	   query("DELETE FROM team_challenges WHERE teamid=$teamid OR o_teamid=$teamid");
	   query("DELETE FROM team_money WHERE teamid=$teamid");
	   query("DELETE FROM team_requests WHERE teamid=$teamid");    
	   query("DELETE FROM team_members WHERE teamid=$teamid");
  }

  if($act=='info') {
    $name = queryone("SELECT name FROM teams WHERE id=$id");
    echo "<br>Team name:$name<br>";    
    echo "<br>Team members";
    htmlqueryID(
	   query("SELECT users.id, users.nick,if(owner,'owner','-') as state,cars.name 
	          FROM team_members 	          
	          LEFT JOIN users ON team_members.userid=users.id
	          LEFT JOIN usercars ON usercars.id=usercarid
	          LEFT JOIN cars ON usercars.carid=cars.id
			  WHERE teamid=$id"),
			  			   "<a href=admin.php?act=modifyuser&id=ID&rnd=$rnd>Details</a>" 
	);
    echo "<br><a href=?act=delete&id=$id&rnd=$rnd>DELETE THIS TEAM (ARE YOU SURE?)</a><br>";    
    echo "<br><a href=?act=teammoney&id=$id&rnd=$rnd>TEAM MONEY TRANSACTIONS</a><br>";    
    
	
    return;    
  }
  if($act=='teammoney') {
    
    if($act2=='add') {
	  
      if(strlen($value)!=0) {

       $money = queryone("SELECT money FROM teams WHERE id=$id");
       $moneynew = $money  + $value;
       $time = time();
       query("INSERT INTO team_money(teamid,value,comment)
              VALUES ($id,$value,'Change from admin panel')");
       query("UPDATE teams SET money=$moneynew WHERE id=$id");
       echo "Added [$value] was [$money]<br>";
     } else {
       echo "ERROR: Enter value to add.<br>";
     }	  
	}
	
    if($act2=='set') {
	  
      if(strlen($value)!=0) {
       $money = queryone("SELECT money FROM teams WHERE id=$id");
       $moneynew = $value;
       $time = time();
       query("INSERT INTO team_money(teamid,value,comment)
              VALUES ($id,$value,'Set from admin panel')");
       query("UPDATE teams SET money=$moneynew WHERE id=$id");
       echo "Set to [$value] was [$money]<br>";
     } else {
       echo "ERROR: Enter value to add.<br>";
     }	  
	}
	
	
	$money = queryone("SELECT money FROM teams WHERE id=$id");
	echo "<br><font size=4 color=red>Team money account: $money</font>";
	htmlquery(
	  query("SELECT users.nick,value,comment FROM
	           team_money 
			   LEFT JOIN users ON userid = users.id
			   WHERE teamid=$id
			   ORDER BY team_money.id DESC"));         
			   
			   // FORMS
    		echo "<br><br>
   			<FORM ACTION=?>
			<input type=hidden name=rnd value=$rnd>
			<input type=hidden name=act value=teammoney>
			<input type=hidden name=act2 value=add>
			<input type=hidden name=id value=$id>
				Value to add (negative value to substract):<br>
			<input type=text name=value>
			<input type=submit   value='Process'>
			</FORM>

			<FORM ACTION=?>
			<input type=hidden name=rnd value=$rnd>
			<input type=hidden name=act value=teammoney>
			<input type=hidden name=act2 value=set>
			<input type=hidden name=id value=$id>
				Set money to:<br>
			<input type=text name=value>
			<input type=submit   value='Process'>
			</FORM>			   ";
			   
			   
			   
			   
			   
	return;
  }
  echo "
        <FORM ACTION=?>
        <input type=hidden name=rnd value=$rnd>
       Search for team (name or part):<br>
       <input type=text name=teamlike value='$teamlike'>
      <input type=submit   value='Search'>
      </FORM>";
    
    
  if(!is_null($teamlike)) {
     $count = queryone("SELECT count(id) FROM teams WHERE name like '%$teamlike%'");
     echo "$count team(s).<br>";
     $res = query("SELECT * FROM teams WHERE name like '%$teamlike%'");
     
     htmlqueryID( $res,
				  "<a href=?act=info&id=ID&rnd=$rnd&teamlike=$teamlike>Info</a>");
  }
?>