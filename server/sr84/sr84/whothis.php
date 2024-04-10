<?php

include_once "../functions.php";
include_once "../connect.php";
include_once "whothis_inc.php";

  /// DECRYPTING QUERYSTRING
  $s = $_SERVER['QUERY_STRING'];
  $s = FromHex($s);
  $decryptquery = Decrypt($s,0xced1);
  // PARSE TO $_GET array  
  parse_str($decryptquery, $_GET);


  /// NOW CHECK TAG VALUE 
   $qs = $decryptquery;

   $tagpos = strpos($qs,"&tag");
   $qs = substr($qs,0,$tagpos);

   $k2="Unknown error in server chat thread, please contact nearest police station.";
   $k="This code was originally written using my poor brains.";

   $tagval = 0;
   $l=strlen($k);
   $l2=strlen($k2);
   for($i=0;$i<strlen($qs);$i++) {
     $c = ord($qs[$i]);
     $c = ord($k[$c%$l]);
     $c = ord($k2[$c%$l2]); 
     $tagval += $c+($i+1)%2+($i+1+4)%7+($i+1)%5+$c%2;
   }
   


   $ip = $_SERVER["REMOTE_ADDR"];
   
   $count = queryone("SELECT count FROM bruteips WHERE ip='$ip'");

   $bruter = false;
   if($count>3) 
     $bruter = true;


   $tag = $_GET["tag"];
   
   
   if(strlen($tag)==0 || $tag!=$tagval || $bruter) {


     $time=time();
     if(strlen($count)==0) {
       query("INSERT INTO bruteips(ip,lasttime,count) VALUES('$ip',$time,1)");      
     } else {
       query("UPDATE bruteips SET count=count+1,lasttime=$time WHERE ip='$ip'");
     }

     $bid = queryone("SELECT id FROM bruteips WHERE ip='$ip'");
     $query = str_replace("'","",$decryptquery);
     $query = $_SERVER['SCRIPT_NAME']."?".$query;
     
     query("INSERT INTO bruteiphistory(bruteipid,time,query)
            VALUES($bid,$time,'$query')");
        

     die ("(_._)");    


   }


   $agent=$_SERVER["HTTP_USER_AGENT"];
   if($agent!="rp") die ("(_._)");

?>