<?php

include "whothis.php";
unset($except);
parse_str($decryptquery);


  if($act=='forban') {
     echoquery(
	   query("SELECT id,nick, if(moder,'moder',''), if(member,'member','')  FROM users 
  	     WHERE nick like '$nicklike'   	     
	     ORDER BY nick")
	 );
	 return; 
  }


if(strlen($except)==0) die("No exceptid");


$time=time()-24*60*60;
$result = query(
         "SELECT id,nick ".
         "FROM users WHERE (id<>$except) AND lastlogintime>$time ORDER BY nick;"
);

echoquery($result);


?>