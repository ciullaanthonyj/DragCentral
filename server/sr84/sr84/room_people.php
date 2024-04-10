<?php


include "whothis.php";


$room = $_GET["room"]; //id
$all = $_GET["all"];

$INVIS = "AND invisible=0";
if($all=="1") 
  $INVIS = "";

if(strlen($room)==0) die("Error: no room given");

$result = query(
         "SELECT nick, (admin*16+moder*8+member*4+male*2+female),id,invisible,
          noprivates
          FROM users WHERE inroom=$room $INVIS ORDER BY nick;"
);


echoquery($result);



?>