<?php


  include_once "whothis.php";

include_once "../connect.php";
include_once "../functions.php";

   $rnd = "&r=".microtime_float();
   echo "<font face=verdana>";
   echo "PERIODICAL MESSAGES<br>";
   echo "<font size=0> <a href=admin.php?rnd=$rnd><< back to admin</a><br>";
  parse_str($_SERVER['QUERY_STRING']);
   
  if($act=='addmessage') {
    $time = time();
    if($message=='') {
      echo "<br>enter message";
    } else
    if($period=='') {
      echo "<br>specify period";    
    } else {
      query("INSERT INTO sysmess(message,periodsec,lasttimesend)
        VALUES('$message',$period*60,$time)");
      // reset send timer
      query("UPDATE sysmess SET lasttimesend=$time");
      echo "<br>inserted";
    }
  }
  
  if($act=='deletemessage') {
    query("DELETE FROM sysmess WHERE id=$id");
	echo "<br>deleted"; 
  }
   htmlqueryID(query("SELECT id,TRUNCATE(periodsec/60,2) as period_min, message FROM sysmess"),
   "<a href=?act=deletemessage&id=ID$rnd>Delete</a>");   
   
   
   
//  echo "<font face=tahoma size=2>";

echo
     "<FORM action=?>
     Message text:<input type=text name=message value='$message'><br>
     Period (minutes):<input type=text name=period value='$period'><br>
     <input type=hidden name=act value=addmessage>
     <input type=hidden name=r value=$rnd>
     <input type=submit value='      Add message     '>
	 </FORM>
	 " ;


  
?>