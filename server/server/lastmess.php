<?php


include "whothis.php";
include_once "../settings.php";

$delayed = '  ';
// GETS LAST MESSAGES BIGGER THAN ID IN ROOM OR FOR NICK
// THIS SCRIPT IS CALLED CONSTANTLY

  unset($nk); //nick
  unset($rm); //room
  unset($ms); //message
  unset($tonk); //to nick
  parse_str($decryptquery); 

// ADDING MESSAGES 
  if(isset($nk) && count($nk)!=0)  {
    $time=time();

    for($i=0;$i<count($nk);$i++) {
      $a_nick = $nk[$i];
      $a_tonick = $tonk[$i];
      $a_room = $rm[$i];
      if(strlen($a_room)==0) $a_room = -1;
      $a_mess = $ms[$i];

      // CHECK NOW IN ROOM AND MODIFY (FOR REMOVING INVISIBILITY BUG)
      $nowin = queryone("SELECT inroom FROM users WHERE nick='$a_nick'");
      if($nowin==-1 && $a_room!=-1) {

        query("UPDATE users SET users.inroom = $a_room WHERE nick='$a_nick';"); 
        // ENTER ROOM MESSAGE
        query("INSERT $delayed INTO messages(nick,message,room,time) values('$a_nick','%ENTER%','$a_room',$time);");  
      }
      
      //WORDS FILTER
      
      $low_mess = strtolower($a_mess);
      // NEW SCHEME
      $noquotes = noquotes($a_mess);
      $res=query("SELECT word,repl FROM wordsfilter 
	     WHERE '$noquotes' like CONCAT('%',word,'%')");
//      $res=query("SELECT word,repl FROM wordsfilter");
      while ($line = mysql_fetch_array($res, MYSQL_ASSOC)) {          
          $look = $line['word'];
          $repl = $line['repl'];
          while(true) {
            $p = strpos($low_mess,$look);
            if($p===false) break;
            $a_mess = substr($a_mess,0,$p).$repl.substr($a_mess,$p+strlen($look),1024);
            $low_mess = substr($low_mess,0,$p).$repl.substr($low_mess,$p+strlen($look),1024);
          }
	  } 
      
      
      
      
      $a_mess = noquotes($a_mess);	   
      $a_nick = noquotes($a_nick);
      $a_tonick = noquotes($a_tonick);

      $ignored = 0;
      // PRIVATE MESSAGE
	  if(strlen($a_tonick)!=0) {
	     
	     $ignored=queryone("SELECT ignores.id FROM ignores 
	           LEFT JOIN users ON users.id=userid
	           LEFT JOIN users as ignored ON ignored.id=ignoreid
	           WHERE users.nick='$a_tonick' AND ignored.nick='$a_nick'");
	    if(strlen($ignored)!==0)
	      $ignored = 1;
	    
	  }

      if($ignored) 
	    query("INSERT $delayed INTO messages(nick,tonick,message,room,time)
		       values('$a_tonick','$a_nick','You are blocked.','$a_room',$time)");
	  else			  
        query("INSERT $delayed INTO messages(nick,tonick,message,room,time) 
  	          values('$a_nick','$a_tonick','$a_mess','$a_room',$time);");
    }
    
  }


if(strlen($nick)==0) $nick = "<nobody>";

if(strlen($id)==0) die("no id parameter.");
  if(strlen($room)==0) $room = "-1";


// UPDATE ROOM LAST VISIT TIME
  if($room!=-1) {
    $timestamp=time();
    query("UPDATE rooms SET lastvisittime=$timestamp WHERE id=$room");
  }

//MAKE IT EVERY THIRD TIME
//if(rand(0,2)==0) {
  $time = time();
  query("UPDATE users SET lastcheckmessagetime=$time WHERE nick='$nick'");
//}


// QUERY MESSAGES TO ROOM WHERE USER OR TO THIS USER (PRIVATE)
// OR FROM THIS USER TO ANOTHER USER (PRIVATE)
// LATER THAN ID

$userid = queryone("SELECT id FROM users WHERE nick='$nick'");
$query = "SELECT messages.id,messages.nick,message,tonick,
          (tonick='$nick' or (messages.nick='$nick' and tonick<>'')) as private,
		  (users.admin*16+users.moder*8+users.member*4+users.male*2+users.female) as attribs
          FROM messages 
         LEFT JOIN users ON messages.nick = users.nick 
         LEFT JOIN ignores ON (ignores.userid=$userid AND users.id=ignores.ignoreid)
         WHERE (messages.id>$id) AND 
         (((room=$room) or (tonick='$nick')) or (messages.nick='$nick' and tonick<>'')) AND
         ignores.id is null
		 ORDER BY messages.id";
$result = mysql_query($query) or die('Query failed: ' . mysql_error());


echoquery($result);

?>