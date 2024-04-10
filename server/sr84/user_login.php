<?php

include "whothis.php";

  $nick = $_GET["nick"];
  $pass = $_GET["pass"];
  $ver = $_GET["ver"];
  $getinfo = $_GET["getinfo"];
  // this is since to 0.73 for some additional protection
  // so they couldn't login with previous versions
  // also all previous servers should be removed
  // or this script should be replaced in them
  // at least in previous 0.72 server, all other should be killed
  $ver2 = $_GET["track"]; 


  if(strlen($nick)==0) { echo("No nick given"); return; }
  if(strlen($pass)==0) { echo("No pass given"); return; }


include_once "../settings.php";



// REMOVE FROM CHAT USERS THAT ARE THERE TOO LONG WITHOUT ACTIVITY (checking new messsages)
// (no quit message)


$query = "SELECT pass FROM users WHERE nick='$nick'";
$result = mysql_query($query);
if(mysql_errno()) {
    echo "Error in query.";
    return;
}

if(mysql_num_rows($result)==0)
  die("Wrong nick [$nick].");

while ($line = mysql_fetch_array($result, MYSQL_ASSOC)) {
    foreach ($line as $col_value) 
      if($pass!=$col_value) die("Wrong pass.");
       
}  

// CHECK GAME BAN

// delete expired bans
  $time = time();
  query("DELETE FROM bans WHERE (timestamp+length)<$time AND game=1");

  $line = queryline("SELECT (timestamp+length) as until,reason FROM bans 
            LEFT JOIN users ON bans.userid = users.id 
			WHERE users.nick='$nick' AND game=1");
  $res = $line['until'];
  $reason = $line['reason'];

  if(strlen($res)!=0) {
    $res = $res - time();  
    $res = TimePeriod($res);
    echo("Sorry, $nick. You have been game banned. Reason: $reason. \n$res until unban.");
    return;
  }


  $userid = queryone("SELECT id FROM users WHERE nick='$nick'");
  
  $time = time();
  $ip = $_SERVER['REMOTE_ADDR'];
  
  TrackIP($userid,$ip);
  
  query("UPDATE users SET users.lastlogintime=$time, users.lastip='$ip' 
     WHERE nick='$nick'");


  if(strlen($ver)==0 || $ver<$lastversion || $ver!=$ver2) {
  
    echo ("Thank you for trying to login, but this version of client will not work anymore :-(. Please download latest. ".
          "\n$downloadlink\nLatest version: $lastversion\n$versionmessage");
 
    return;
  }


  include_once "challenge.php";

  $activated=queryone("SELECT activated FROM activation WHERE userid=$userid");
  if($activated=="0") {
     $email = queryone("SELECT email FROM users WHERE id=$userid");
     echo "Your account is not activated. In order to activate account, follow instructions that were sent to your e-mail [$email].";
	 return;    
  }

  // don't check double login when on localhost
  $ago = time()-$removefromonline;
  query("DELETE FROM online WHERE lastactivity<$ago");
 
  if($_SERVER['SERVER_NAME']!='localhost') {
    $online = queryone("SELECT lastactivity FROM online 
                        WHERE userid=$userid");
    if(strlen($online)!=0) {
       echo "Double login. Please wait $removefromonline seconds.";
       return;
    }
  }

  UpdateUserOnline($userid);
  ProcessOldCompChallenges($userid);
  ProcessNotAnswerEndedChallenges($userid);
  ProcessNotCreateEndedChallenges($userid);  

  $login_userid = $userid;
  echo "Authorized";

  if($getinfo) {

    echo "|";

    echoquery(
      query("SELECT id,(admin*16+moder*8+member*4+male*2+female),nick
	  FROM users WHERE nick='$nick'")
    );

  }

function TrackIP($userid,$ip) {
  $lastip = queryone("SELECT ip FROM iptrack WHERE userid=$userid ORDER BY id DESC");
  if($lastip!=$ip) {
    $time = time();
    query("INSERT INTO iptrack(userid,time,ip)
      VALUES($userid,$time,'$ip');");
  } 
}

?>