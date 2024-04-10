<?php

  $scriptstarttime = round(microtime_float()*1000);
  
  function UpdateUserOnline($id) {           
    $time = time();
    $online = queryone("SELECT id FROM online WHERE userid=$id");    
    if(strlen($online)==0) {
       query("INSERT INTO online(userid,lastactivity) VALUES($id,$time)");
    } else {
       query("UPDATE online SET lastactivity=$time WHERE id=$online"); 
    }
  }
  
  function EndBenchmark() {
    $endtime = round(microtime_float()*1000);
    global $scriptstarttime;
    global $stack_q;
    $endtime = $endtime - $scriptstarttime;
    $time = time();
    global $decryptquery;
    if(!isset($stack_q)) {
      $script = $_SERVER['SCRIPT_NAME']."?".$decryptquery;
    } else {
      $stackscripts = implode("|",$stack_q);
      $script =  $_SERVER['SCRIPT_NAME']."?".$stackscripts;      
    }
    $script = substr($script,1,64);
    $time = $time - $endtime/1000; // show started time not ended
    query("INSERT INTO bm(scriptname,timestamp,msecs) VALUES('$script',$time,$endtime)");
  }
  
  register_shutdown_function("EndBenchmark");

 function noquotes($s) {
    $s = str_replace("\\","\\\\",$s);
    $s = str_replace('"','\"',$s);
    $s = str_replace("'","\'",$s);      
    return $s;
  }
  
   // QUERY ONLY ONE VALUE
  function queryone($query) {
    $result = mysql_query($query) or 
	  print("Query[$query] Query failed: " . mysql_error());
    $res="";
    while ($line = mysql_fetch_array($result, MYSQL_ASSOC)) {
      foreach ($line as $col_value) 
        $res = $col_value;       
    }
    return $res;
  }

  function queryline($query) {
    $result = mysql_query($query) or print("Query[$query] Query failed: ". mysql_error());
    $line = mysql_fetch_array($result, MYSQL_ASSOC);
    return $line;
  }
  
  function echoquery($result) {
    $first = true;
    while ($line = mysql_fetch_array($result, MYSQL_ASSOC)) {
      if($first) {
         echo count($line)."|";
         $first = false;
      }

      foreach ($line as $col_value) {
          $col_value = urlencode($col_value);
          echo "$col_value|";
      }
    }
  }

     // Printing results in HTML
  function htmlquery($result) {
    htmlqueryID($result,"");
  }

  function htmlqueryID($result,$actionstring) {
     echo "<table border=0  cellpadding=1 cellspacing=1>\n";
     $firstline = true;
     while ($line = mysql_fetch_array($result, MYSQL_ASSOC)) {

        if($firstline) {
          echo "\t<tr style='background:#000000;color:#ffffff;font-weight:bold;'>\n";    
          if(strlen($actionstring)!=0)
            echo "<td><font face=verdana size=1>Action";
          foreach($line as $colname => $col) {
            if(strlen($actionstring)!=0 && $colname=='id') 
              continue;
            echo "<td><font face=verdana size=1>$colname";
          }
          $firstline = false;         
        }         

        echo "\t<tr>\n";    
        if(strlen($actionstring)!=0) {
          $id = $line['id'];
          $idactionstring = str_replace("ID", $id, $actionstring);
          echo "<td><font face=verdana size=1>$idactionstring";
        }
        foreach($line as $colname => $col) {
            if($colname=="day") {
               $col = strftime("%d/%m/%Y %a %H:%M:%S",$col);
            }
            if(strpos($colname,'_time') || $colname=="time" || $colname=="creationtime" || $colname=="timestamp") {
               if($col==0)
                 $col = '-';
               else
                 $col = strftime("%d/%m/%Y %H:%M:%S",$col);
            }
            if($colname=='period')
               $col = TimePeriodShort($col);
            if(strlen($actionstring)!=0 && $colname=='id') 
              continue;
            if(strlen($col)==0) $col = "-";
            echo "<td><font face=verdana size=1>$col";
            
        }

      }
      echo "</table>\n";
  }

  function rightchars($in) {
    $goodchars = 'abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890~!@#$%^&*()_+{}|<>?:\'";,./\-=[] ';
    $p = 0;
    $l = strlen($in);
    $out = '';
    while(true) {
      if($p>=$l) break;
      $c = $in[$p];
      if(strpos($goodchars,$c)!==false) 
        $out .= $c;
      $p++;
    }
    return $out;

  }

  function TimePeriodShort($seconds) {

    $sec = $seconds % 60;
    $min = ($seconds - $sec) / 60;

    $hour = ($min - $min % 60) / 60;
    $min = $min % 60;

    $day = ($hour - $hour % 24) / 24;
    $hour = $hour % 24;

      return "$day:$hour:$min:$sec";

  }

  function TimePeriod($seconds) {

    $sec = $seconds % 60;
    $min = ($seconds - $sec) / 60;

    $hour = ($min - $min % 60) / 60;
    $min = $min % 60;

    $day = ($hour - $hour % 24) / 24;
    $hour = $hour % 24;

    return "$day day(s) $hour hour(s) $min minute(s) $sec second(s)";
  }

  function microtime_float()
  {
      list($usec, $sec) = explode(" ", microtime());
      return ((float)$usec + (float)$sec);
  }
  
 
?>