<?php
$get = $_GET['get'];
if($get=='image')
  $whothis_noheaders=1;
include_once "whothis.php";
include_once "../connect.php";
include_once "../functions.php";
// tryed to make it in one file it is kinda confusing
// INCOMING PARAMETERS show AND act

  $h = 240;
  $w = 640;
  $im = imagecreate($w,$h);
  $color = imagecolorallocate($im, 255, 255, 255);
  $color = imagecolorallocate($im, 0, 0, 0);
  
  $maxstamp = queryone("SELECT max(timestamp) FROM onlinestats");
  $minstamp = queryone("SELECT min(timestamp) FROM onlinestats");  
  $mincount = queryone("SELECT min(count) FROM onlinestats");
  $maxcount = queryone("SELECT max(count) FROM onlinestats");
  
  $result = mysql_query("SELECT timestamp,count 
     FROM onlinestats ORDER BY ID");
  while ($line = mysql_fetch_array($result, MYSQL_ASSOC)) {
    
    $time = $line['timestamp'];
    $count = $line['count'];
    
    $x1 = ($w-1)*($time-$minstamp)/($maxstamp-$minstamp);
    $y1 = $h-$h*($count)/($maxcount);

    $x1 = round($x1);
	$y1 = round($y1);    
    $x2 = $x1;
    $y2 = $h;

    
    if($get=='image') {
      imageline($im,$x1,$y1,$x2,$y2,$color);    
    } else {
//      echo "($x1,$y2,$y1) ";
    }
  }  

 
  if($get=='image') {
    header("Content-type: image/png");
    imagepng($im);
  } else {
    echo "<font face=verdana size=1>";
    echo "<br>Online Stats<br>";
    echo "Time:".strftime("%d/%m/%Y %H:%M:%S",$minstamp)."-".
      strftime("%d/%m/%Y %H:%M:%S",$maxstamp)."<br>";
    echo "People Online: $mincount-$maxcount<br>";
    echo "<img src=?get=image>";
  }


?>