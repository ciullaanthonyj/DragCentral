<?php

include "whothis.php";


$room = $_GET["room"];
$pass = $_GET["pass"];
$userid = $_GET["userid"];

if(strlen($room)==0) die("Error: no room given");


$result = query("SELECT * FROM rooms WHERE name='$room'");
if(mysql_num_rows($result)!=0)
  die("Error: room [$room] exists.");

$time=time();
query("INSERT INTO rooms(name,pass,lastvisittime,creator) values('$room','$pass',$time,$userid);");
echo "Success: room created.";

?>