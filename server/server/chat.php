<?php

include "whothis.php";

  parse_str($decryptquery); 
  
  
  if($act=='ignoreuser') {
    $ignorenick=$nick;
    $ignorenick=noquotes($ignorenick);
	$id=queryone("SELECT id FROM users WHERE nick='$ignorenick'"); 
    $exists=queryone("SELECT id FROM ignores WHERE userid=$userid AND ignoreid=$id");
    if(strlen($exists)==0) {
      query("INSERT INTO ignores(userid,ignoreid) VALUES ($userid,$id)");
    }
    if(!mysql_errno()) echo "Success";
  }
  
  if($act=='unignoreuser') {
    $ignorenick=$nick;
    $ignorenick=noquotes($ignorenick);
	$id=queryone("SELECT id FROM users WHERE nick='$ignorenick'"); 
    $exists=queryone("SELECT id FROM ignores WHERE userid=$userid AND ignoreid=$id");
    if(strlen($exists)!=0) {
      query("DELETE FROM ignores WHERE id=$exists");
    }
    if(!mysql_errno()) echo "Success";
  }

?>