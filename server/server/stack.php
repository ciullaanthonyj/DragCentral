<?php

  $qs = $_SERVER["QUERY_STRING"];

  // &q[]=user_login.php&p[]=ASDFASFSDFASF
  // UNSET IS FOR SERVERS WHERE QUERYSTRING IS PARSED BEFORE
  unset($q);
  unset($p);
  parse_str($qs);
  $stack_q = $q;
  $stack_p = $p;
  
  for($stack_i=0;$stack_i<count($stack_q);$stack_i++) {    
    $_SERVER["QUERY_STRING"] = $stack_p[$stack_i];
    {
      $stack_script = $stack_q[$stack_i];
      if(strpos($stack_script,"..")!==false ||
	     strpos($stack_script,"\\")!==false ||
		 strpos($stack_script,"/")!==false ||
		 strpos($stack_script,":")!==false) {
        continue;
      }
      include $stack_script;  
    }
	echo "\r\n\r\n";   
  }
  
?>