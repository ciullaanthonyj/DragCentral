<?php

include "whothis.php";

  parse_str($decryptquery); 
 
  if($act=="getinfo") {
  
    $result = query(
             "SELECT forfun,bracket,brackettime,racetime,answered,formoney,headsup,falsestart,
			 reactiontime,wager,answerbrackettime,answerracetime,answerreactiontime,
			 answerfalsestart, forcar
             FROM challenges WHERE id=$id"
      );
    echoquery($result);     
    return;
  };

  if($act=="hideincoming") {
    query("UPDATE challenges SET hideincoming=1 WHERE touserid=$userid AND id=$id");
    if(!mysql_errno()) { echo("Success"); return; }  
  }

  if($act=="hideoutgoing") {
    query("UPDATE challenges SET hideoutgoing=1 WHERE userid=$userid AND id=$id");
    if(!mysql_errno()) { echo("Success"); return; }  
  }
  
?>