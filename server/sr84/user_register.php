<?php

include "whothis.php";

  parse_str($decryptquery); 

  if(strlen($nick)==0) die("No nick given");
  if(strlen($pass)==0) die("No pass given");
  if(strlen($email)==0) die("No e-mail given");


  $email = strtolower($email);


  
$result = query("SELECT * FROM users WHERE nick='$nick'");

if(mysql_num_rows($result)!=0)
  die("User [$nick] exists.");


$result = query("SELECT * FROM users WHERE email='$email'");

if(mysql_num_rows($result)!=0)
  die("User with such email [$email] exists.");



query("INSERT INTO users(nick,pass,email,male,female) values('$nick','$pass','$email',$male,$female);");

$carid = $_GET['carid'];

$id=queryone("SELECT id FROM users WHERE nick='$nick'");

query("INSERT INTO usercars(userid,carid,selected) VALUES($id,$carid,1)");
$emailrandom = rand(100000000,999999999);
$time = time();
query("INSERT INTO activation(userid,emailrandom,time) VALUES($id,'$emailrandom',$time)");

$activateid = queryone("SELECT id FROM activation WHERE userid=$id");
$headers = "From: registration@drag-central.co.cc\r\n" .
           "Reply-To: $email\r\n";
    
mail($email,"Drag Central Account Activation", 
"Drag Central Account - Nick: [$nick]\r\n".
"Go to this link to activate your drag-central account:\r\n".
"http://drag-central.co.cc/gm/activation.php?id=$activateid&rn=$emailrandom",
$headers);

if(!mysql_errno()) echo "Success.Successfully registered. Activation instructions were sent to your e-mail [$email].";

?>