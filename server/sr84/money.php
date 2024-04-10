<?php

  function ChangeMoney($userid,$opponentid,$value,$comment) {
     $time = time();
     $money = queryone("SELECT money FROM users WHERE id=$userid");
     query("INSERT INTO money(userid,opponentid,value,comment,time,startmoney,endmoney) VALUES ($userid,$opponentid,$value,'$comment',$time,$money,if($money+$value<0,0,$money+$value))");
     query("UPDATE users SET money=if(money+$value<0,0,money+$value) WHERE id=$userid");      
  }
  
  function TeamBalance($teamid) {
    $money = queryone("SELECT money FROM teams WHERE id=$teamid"); 
    $freezed = queryone(
	  "SELECT sum(wager) FROM team_challenges 
	   WHERE (teamid=$teamid AND formoney=1 AND ended=0) OR
	         (o_teamid=$teamid AND accepted=1 AND ended=0)");
    $res = $money-$freezed;                     
    return $res;     
  }
  function ChangeTeamMoney($teamid,$o_teamid,$userid,$value,$comment) {
    query("INSERT INTO team_money(teamid,o_teamid,userid,value,comment) VALUES($teamid,$o_teamid,$userid,$value,'$comment')");
    query("UPDATE teams SET money=money+$value WHERE id=$teamid");
  }

  function UserFreeMoney($id) {
    $money = queryone("SELECT money FROM users WHERE id=$id");  
    $freezed = queryone("SELECT sum(wager) FROM challenges WHERE userid=$id AND answered=0");
    return  $money-$freezed;
  }

?>
