<?php
  include "whothis.php";
  include_once "money.php";          
  parse_str($decryptquery);
  
  if($act=='getrank') {
    echoquery(query("SELECT rank FROM users WHERE id=$userid"));
    return;
  }

  if($act=='getsettings') {
     echoquery(query("SELECT noprivates FROM users WHERE id=$userid"));
	 return;    
  }
  
  if($act=='setnoprivates') {
    query("UPDATE users SET noprivates=$val WHERE id=$userid");    
    return; 
  }
  
  if($act=='logout') {
    query("DELETE FROM online WHERE userid=$userid");        
    return;
  }

  if($act=='getstats') {    
	$res = query("SELECT wins,loses FROM users WHERE id=$userid");    
    if(!mysql_errno()) { echoquery($res); }		
    return;
  }
  
  if($act=='getmembershipstate') {
     $member = queryone("SELECT member FROM users WHERE id=$userid");
     if(!$member) {
       $expires = "you don't have membership";
     } else {
       $expires = queryone("SELECT expires FROM users WHERE id=$userid"); 
       $expires = "expires - ".strftime("%d %b %Y",$expires);
     }
     echo "1|$expires|";
     return;
  }
  
  if($act=='getbalance') {  
    echo "1|".UserFreeMoney($userid)."|";
    return;
  } 
  
  if($act=='sendpass') {
    $email = strtolower($email);
    if(strlen($email)==0) die("No email given.");
    $result = query("SELECT pass,nick FROM users WHERE email='$email';");
    if(mysql_num_rows($result)==0) {
      echo("No such email registered."); return;
    }
    $line = mysql_fetch_array($result, MYSQL_ASSOC);
    $nick = $line['nick'];
    $pass = $line['pass'];
    $headers = "From: Drag Central Game <support@drag-central.com>\r\n" .
               "Reply-To: $email\r\n";
    mail($email,"Drag Central Login", "Login: $nick\r\nPass: $pass",
     	$headers);
    echo "Success: Pass sent.";
    return;
  }

  if($act=='changepass') {  
    $old=$_GET['old'];
    $new=$_GET['new'];
    $id=$_GET['id'];
    $realold = queryone("SELECT pass FROM users WHERE id=$id");
    if($realold!=$old) {
      die ("Error: wrong current password.");
    }
    query("UPDATE users SET pass='$new' WHERE id=$id");
    echo "Successfully changed.";
    return;   
  }
  

?>