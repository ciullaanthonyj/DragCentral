<?php
  // permanent rooms editor
  include_once "whothis.php";

include_once "../connect.php";
include_once "../settings.php";
include_once "../functions.php";
  

  echo "<font face=Tahoma size=2>"; 
  parse_str($_SERVER['QUERY_STRING']);
  $rnd = "&r=".microtime_float();
  echo "<font size=1>";
//  echo "<a href=admin.php?$rnd><< back to main menu</a><br>";
  if($act=='edit') {
     echo
     "<a href=?show=rooms$rnd><< back to room list</a><br>";
     
     $roomname = queryone("SELECT name FROM rooms WHERE id=$roomid");
     
     echo "<font size=4>Room: <font color=red>$roomname</font></font><br>";
     
     echo
     "<a href=?show=rooms&act=delete&roomid=$roomid$rnd>Delete this room</a>";     
  
     echo
     "<FORM action=?>
     Rename room to:<input type=text name=newname>
     <input type=hidden name=act value=rename>
     <input type=hidden name=show value=rooms>
     <input type=hidden name=roomid value=$roomid>
     <input type=hidden name=r value=$rnd>
     <input type=submit value='  Rename   '>" ;
     
    return;    
    
  }
  $online = queryone("SELECT count(id) FROM online;");
  echo "Now users online: $online<br>";
  echo "<font color=red>Room list</font> ";
  echo "<font size=1><a href=?show=rooms$rnd>refresh</a></font>";

  if($act=='rename') {
     query("UPDATE rooms SET name='$newname' WHERE id=$roomid"); 
  }
  if($act=='delete') {
    query("DELETE FROM rooms WHERE id=$roomid");
  }
  if($act=='newroom') {
    query("INSERT INTO rooms(name,permanent) VALUES('$newname',1)");
  }  
  htmlqueryID(
    query("SELECT rooms.id, name, rooms.pass, 
	       if(permanent,'yes','no') as permanent,
	       count(users.id) as nowinroom,
	       lastvisittime as lastvisit_time,	       
	       maxpeople,
	       maxpeopletime as maxpeople_time
           FROM rooms 
		   LEFT JOIN users ON rooms.id = users.inroom 
           GROUP BY rooms.name 
           ORDER BY rooms.id            
		   "),
    "<a href=?show=rooms&act=edit&roomid=ID$rnd>Edit room</a>"    
  );
  $format = '%d/%m/%Y %H:%M:%S';
  echo "Server Time: ".strftime($format);
  echo "<br>";

   echo
   "<FORM action=?>
     Create new permanent room:<input type=text name=newname>
     <input type=hidden name=act value=newroom>
     <input type=hidden name=show value=rooms>
     <input type=hidden name=rooid value=$roomid>
     <input type=hidden name=r value=$rnd>
     <input type=submit value='Create'>
	</FORM>" ;

   echo "<br><font color=red>Room statistics</font> <a href=roomgraph.php>graphics</a><br>";  
   $day=$_GET["day"];
   if(strlen($day)==0) { 
    htmlqueryID(
	  query(
	     "SELECT (timestamp - timestamp%(3600*24)) as day, 
	      (timestamp - timestamp%(3600*24)) as id 
	      FROM roomstats GROUP BY day"
	   ),
       "<a href=?show=rooms&day=ID$rnd>Day stats</a>");       
   } else {   
     echo "<font size=1><a href=?show=rooms$rnd><< back to all days list</a></font><br>";
     htmlquery(query("SELECT roomname,count,timestamp as time 
	      FROM roomstats 
		  WHERE (timestamp>=$day) AND (timestamp<($day +3600*24))   
		  ORDER BY roomname,id"));
   }


?>