<?php  
  
  include "../functions.php";
  include "../connect.php";
  $rnd = "&r=".microtime_float();
 
 
  parse_str($_SERVER['QUERY_STRING']);
  session_start(); 
  include "whothis.php";
  
  if(strlen($act)==0) $act = $_POST['act'];
  if(strlen($act2)==0) $act2 = $_POST['act2'];
  
  echo "
<style>
    .menuhead {
		color: red;
		font-weight: bold;
		font-family: Tahoma;        
		font-size: 24;
    }
	.main {
		color: #4d7097;
		font-weight: bold;
		font-family: Tahoma;
	}
	.simpletext {
		color: #4d7097;
		font-family: Tahoma;	  
		font-weight: ;
		font-size: 12;
	}
	.header {
		margin: 5px 0px 5px 5px;
		padding: 10px 10px 10px 30px;
		border: 1px dotted #ccc;
		background: #f6f9ff;
		color: #4d7097;
		font-weight: bold;
		font-family: Tahoma;
	}
</style>";

echo "<font class=main>";

  if($act=='logout') {
    $_SESSION['authorized'] = false;
	session_destroy();	
  }  
  
 
  if(!isset($_SESSION['authorized']) || !$_SESSION['authorized']) {
 
    $nick = $_POST['nick'];
    $pass = $_POST['pass'];
    $nick = noquotes($nick);
    if(strlen($nick)!=0) {
      $realpass = queryone("SELECT pass FROM users WHERE moder=1 AND nick='$nick'");    
      sleep(1);
    }
   
    if(strlen($pass)!=0 && $realpass==$pass) {
      $_SESSION['authorized'] = true;
      $_SESSION['id'] = queryone("SELECT id FROM users WHERE nick='$nick'");      
    } else {
    
      echo "<p class=header>Drag Central - Mod Tools<br></p>";
      echo "<FORM action=? METHOD=POST>
            <table align=center>
            <tr>
            <td>Nick: <td><input type=text name=nick value=$nick><br>
            <tr>
            <td>Password: <td><input type=password name=pass><br>
            <tr>
            <td><td><input type=submit value='      Login      '>
            </FROM>";  
      if(strlen($_POST['pass'])!=0)  {
      	Sleep(2);
	    echo "<br><font color=red>Wrong pass";
      }
      return;
    }  
  }


  $id = $_SESSION['id'];
  $nick = " - ".queryone("SELECT nick FROM users WHERE id=$id");
  echo "<p class=header>Drag Central - Mod Tools $nick<br></p>";


  if(strlen($act)==0) {  
    echo "<p align=center>";
    echo "<font class=menuhead>Main Menu</font>";
    echo "<table align=center>
    <tr>
    <td><a href=?act=logout$rnd>Log out</a><br>
    <tr>
    <td><a href=?act=personal$rnd>Personal settings</a>
    <tr>
    <td><a href=?act=users$rnd>Users</a><br>
    <tr>
    <td><a href=?act=rooms$rnd>Chat Rooms</a><br>    
    <tr>
    <td><a href=bug.php?$rnd>Bug report</a><br>    
    <tr>
    <td><a href=?act=modlist$rnd>Admins and Moderators</a><br>
    <tr>
    <td><a href=modterms.php?$rnd>Mod Terms</a><br>
     </table>
    ";
  } else {
    $mainmenu = "<br><a href=?$rnd><font size=0><< return to main menu</font></a><br>"; 
  }

  if($act=='modlist') {
     echo $mainmenu;
     $countadmins = queryone("SELECT count(id) FROM users WHERE admin=1");
     $countmoders = queryone("SELECT count(id) FROM users WHERE moder=1");
     echo "<font size=1>Admins:$countadmins</font><br>";
     htmlquery(
       query("SELECT nick,email,admin,moder,member,male,female FROM users WHERE admin=1 ORDER BY nick")
     );
     echo "<font size=1>Moderators:$countmoders</font><br>";
     htmlquery(
       query("SELECT nick,email,admin,moder,member,male,female FROM users WHERE moder=1 ORDER BY nick")
     );
  }

  if($act=='rooms') {
    if($act2=='delete') {
      query("DELETE FROM rooms WHERE id=$roomid AND permanent=0"); 
    }
    echo "<p align=center class=simpletext>";
    echo "<font class=menuhead>Chat Rooms</font>";
    echo $mainmenu;
    echo "<br>";
     htmlqueryID(
	   query("SELECT rooms.id, name, rooms.pass, 
	          count(users.id) as nowinroom, if(permanent,'yes','no') as permanent,
	          creator.nick as creator
	          FROM rooms 
	          LEFT JOIN users ON users.inroom=rooms.id
	          LEFT JOIN users as creator ON creator.id=creator
	          GROUP BY rooms.id	          
			  ORDER BY rooms.id"),
			  "<a href=?act=$act&act2=delete$rnd&roomid=ID$rnd>Delete room</a>"
	 );    
    return;    
  }





  if($act=='personal') {
    echo "<p align=center class=simpletext>";
    echo "<font class=menuhead>Personal Settings</font>";
    echo $mainmenu;
    if($act2=='setvisible') {
      query("UPDATE users SET invisible=0 WHERE id=$id");   
    }
    if($act2=='setinvisible') {
       query("UPDATE users SET invisible=1 WHERE id=$id");   
     }
    echo "<br><br>Your state in chat: ";
    $invis = queryone("SELECT invisible FROM users WHERE id=$id");
    echo $invis?"invisible":"visible";        
    $inviscaption = $invis?"Set Visible":"Set Invisible";
    $invisaction = $invis?"setvisible":"setinvisible";
    echo "<br><a href=?act=personal&act2=$invisaction$rnd>$inviscaption</a>";    
  }

  if($act=='users') {
    echo "<p align=center class=simpletext>";
    echo "<font class=menuhead>Users</font>";
    echo $mainmenu;
    if(strlen($name)=='') {
	  echo "<FORM action=?>
	        User lookup:
	        <input type=text name=name value=$name> 
	        <input type=hidden name=act value=users>
	        <input type=hidden name=r value=$rnd>
            <input type=submit value=search>";	  
	} else {
      echo "<p align=center class=simpletext>Found [$name]";
	  htmlqueryID(
	    query("SELECT id,nick,lastip FROM users 
		       WHERE nick like '%$name%';"),
	    "<a href=?act=userdetails&userid=ID$rnd>Details</a>");   
  
    }	
  }
  
  if($act=='userdetails') {
    echo "<p align=center class=simpletext>";
    echo "<font class=menuhead>User Details</font>";
    echo $mainmenu;
    $time = time();
    
    if($act2=='sameips') {
      $user = queryone("SELECT nick FROM users WHERE id=$userid");
      echo "<a href=?act=$act&userid=$userid$rnd><font size=0>
	        << return to user[$user]
	        </font>
		    </a><br>";
      $ip = queryone("SELECT lastip FROM users WHERE id=$userid");
      echo "Users with same ip as $user";
      htmlquery(
        query("SELECT nick, email, lastip, lastlogintime as last_login_time, 
               lastcheckmessagetime as last_chat_time,
               moder, member, male, female
		       FROM users WHERE lastip='$ip'
			   ORDER BY nick")
	  );
      return; 
    }
    
    if(strlen($userid)==0)
      $userid = $_POST['userid'];
    $banned = queryline("SELECT id FROM bans 
	                    WHERE (userid=$userid) AND (timestamp+length>$time)");
   
    $banned = (strlen($banned)!=0);
     if($act2=='ban') {
        if($banned) {
          $moder = queryone("SELECT users.nick FROM bans
             LEFT JOIN users ON moderid=users.id
             WHERE (userid=$userid) AND (timestamp+length>$time)");
          $user = queryone("SELECT users.nick FROM bans
             LEFT JOIN users ON userid=users.id
             WHERE (userid=$userid) AND (timestamp+length>$time)");
            
          echo "<font color=red>
		  User[$user] is banned now by moderator[$moder]<br> 
		  (unban user[$user] first).</font>";           
        } 
        
        if(!$banned) {
          $reason = $_POST['reason'];
          $reason = noquotes($reason);
		  $length = $_POST['length'];
		  if($length==0) {
		    echo "<br><font color=red>Enter ban length.</font><br>";  		  
		  } else
		  if(strlen($reason)==0) {
		    echo "<br><font color=red>Enter ban reason.</font><br>";  
		  } else {
		  
            query("INSERT INTO bans(userid,moderid,timestamp,length,reason,game,chat) 
  		     VALUES($userid,$id,$time,$length,'$reason',1,1);");            
		
	        query("INSERT INTO banhistory(userid,length,moderid,reason,timestamp,gameban,chatban)
             VALUES($userid,$length,$id,'Control Panel - ban [$reason]',$time,1,1)");
            $banned = true;
            $reason = "";          
          }
        }
     
     }
     
     if($act2=='unban') {
       if($banned) {
         $unbanreason = $_POST['reason'];
         if(strlen($unbanreason)==0) {
		   echo "<font color=red>Enter unban reason</font>";
		 } else {
           query("DELETE FROM bans WHERE userid=$userid");           
           query("INSERT INTO banhistory(userid,moderid,reason,timestamp)
                  VALUES($userid,$id,'Control Panel - unban [$unbanreason]',$time)");
           $banned = false;
        }
       }       
     }

     $usernick = queryone("SELECT nick FROM users WHERE id=$userid");
	 echo "<br><br>User: $usernick
	          <font size=0>
			  <a href=?act=$act&userid=$userid$rnd>refresh</a>
			  </font>
			  <font size=0>
			  <a href=?act=$act&act2=sameips&userid=$userid$rnd>
			  same ips</a>
			  <br>";       
	          
     $l = queryline("SELECT users.nick as moder,reason FROM bans                     
	                 LEFT JOIN users ON moderid=users.id	                 
					 WHERE (userid=$userid) AND (timestamp+length>$time)");	                 
     $banperiod = queryone("SELECT length FROM bans
               WHERE (userid=$userid) AND (timestamp+length>$time)");
     $banperiod = TimePeriod($banperiod);             
     if($l) {
       echo "Now banned by: {$l['moder']} for $banperiod. Reason: {$l['reason']}<br>";       
     } else {
	   echo "Now not banned."; 
	 }
	 if(!$banned)				       
     echo "<br><FORM action=? method=POST>
           <input type=hidden name=act value=userdetails>
           <input type=hidden name=userid value=$userid>
           <input type=hidden name=act2 value=ban>
           <input type=hidden name=rnd value=$rnd>
           <table align=center class=simpletext>
           <tr>
           <td>Length:
           <td>
           <SELECT size=4 name=length>
             <OPTION selected value=60>1 minute</OPTION>
             <OPTION value=300>5 minutes</OPTION>
             <OPTION value=600>10 minutes</OPTION>
             <OPTION value=900>15 minutes</OPTION>
             <OPTION value=1800>30 minutes</OPTION>
             <OPTION value=3600>1 hour</OPTION>
             <OPTION value=7200>2 hours</OPTION>
             <OPTION value=14400>4 hours</OPTION>
             <OPTION value=28800>8 hours</OPTION>
             <OPTION value=50400>14 hours</OPTION>
             <OPTION value=72000>20 hours</OPTION>
             <OPTION value=86400>1 day</OPTION>
             <OPTION value=172800>2 days</OPTION>
             <OPTION value=259200>3 days</OPTION>
             <OPTION value=604800>1 week</OPTION>
             <OPTION value=1209600>2 weeks</OPTION>
           </SELECT>           
           <tr>          
           <td>Reason:<td><input type=text cols=40 name=reason value='$reason'><br>
           <tr>
           <td><td><input type=submit value='  Game Ban '>
		   </FORM>";
		   
     if($banned)
     echo "
	       <br><br>
	       
	       <FORM action=? method=POST>
           <table align=center>
           <tr>
           <td>
           <font class=simpletext>
           Reason:
           </font>
           <td>
  		   <input type=text cols=20 name=reason value='$reason'><br>
           <input type=hidden name=act value=userdetails>
           <input type=hidden name=act2 value=unban>
           <input type=hidden name=userid value=$userid>
           <input type=hidden name=rnd value=$rnd>
           <tr>
           <td>
           <td>
           <input type=submit value='    Unban   '>
           </table>
		   </FORM>";
    
     
     echo "<br><br>Ban History";
     htmlquery(
	   query("SELECT users.nick as moderator,timestamp,
	          TRUNCATE(length/60,0) as minutes,reason,
	          chatban, gameban, chatkick FROM banhistory 
	          LEFT JOIN users ON users.id=moderid
	          WHERE userid=$userid 
			  ORDER BY banhistory.id"));
        
  }


?>