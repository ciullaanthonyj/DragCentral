<?php

  $profile_names[] = "first row";
  $profile_start[] = 0;
  $profile_whole[] = 0;
  
  function profile_time()
  {
      list($usec, $sec) = explode(" ", microtime());
      return round( 1000* ((float)$usec + (float)$sec));
  }
  
  function PSTART($name) {
    global $profile_names;
    global $profile_start;
    
    $i = array_search($name,$profile_names);
    if($i===false) {
      $profile_names[] = $name;
      $i = array_search($name,$profile_names);
      $profile_whole[$i] = 0;
    }
    
    $profile_start[$i] = profile_time();  
    
  
  }
  
  function PEND($name) {
    global $profile_names;
    global $profile_start;
    global $profile_whole;
    
    $i = array_search($name,$profile_names);
    
    $profile_whole[$i] += profile_time()-$profile_start[$i];      
  }

  function PSHOW() {
    global $profile_names;
    global $profile_start;
    global $profile_whole;

    for($i=1;$i<count($profile_names);$i++)  {
      $name = $profile_names[$i];
      echo "$name:".$profile_whole[$i]."<br>";    
    }    
    
  }

?>