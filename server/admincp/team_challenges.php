<?php
  include "whothis.php";
  include "../functions.php";
  include "../connect.php";
  
  
  parse_str($_SERVER['QUERY_STRING']);
  $rn = microtime_float();
  echo "<font face=tahoma size=1>";
  echo "<a href=admin.php?rn=$rn>back to admin</a><br>";
  
  if($deletechallenge) {
    query("DELETE FROM team_challengers WHERE challengeid=$id");
	query("DELETE FROM team_challenges WHERE id=$id");
  }
  
  htmlqueryID(
    query("SELECT teams.name as from_team, teams2.name as to_team, team_challenges.* 
	       FROM team_challenges 
	       LEFT JOIN teams ON teamid=teams.id
	       LEFT JOIN teams as teams2 ON o_teamid=teams2.id
		   ORDER BY team_challenges.id"),
    "<a href=?showchallenge=1&id=ID&rn=$rn>details</a>");
    
  
  if($showchallenge) {
    echo"<a href=?rn=$rn&deletechallenge=1&id=$id>DELETE THIS CHALLENGE</a>";
    htmlquery(
	  query("SELECT users.nick as from_nick, users2.nick as to_nick, team_challengers.* FROM team_challengers 
	         LEFT JOIN users ON users.id=userid
	         LEFT JOIN users AS users2 ON users2.id=o_userid
	         WHERE challengeid=$id")
	);       
  }
  

?>