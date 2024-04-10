<?php
  // set_time_limit(0);
//  error_reporting(E_ERROR);

  parse_str($_SERVER['QUERY_STRING']);

  include "connect.php";
  include "functions.php";
  // id and rn

  
  if(strlen($id)==0)  
    die("no id");
  if(strlen($rn)==0)
    die("no rn");

  if(!is_numeric($id) || !is_numeric($rn))
    die("not numeric");

  $exist = queryone("SELECT id FROM activation WHERE id=$id AND emailrandom=$rn");
  if(strlen($exist)==0) {
//    Sleep(10);
    echo "Wrong activation link.";
	return; 
  }
  

if($act=="image") {

  header("Content-type: image/png");
  $im = imagecreate(120, 30)
      or die("Cannot Initialize new GD image stream");
  $background_color = imagecolorallocate($im, 245, 245, 245);

  function Letter($im,$x,$y,$c) {
    $text_color = imagecolorallocate($im, 0, 0, 0);
    imagestring($im, 5, $x, $y,  $c, $text_color);  
    imagestring($im, 5, $x+1, $y+1,  $c, $text_color);  
  }

  $text_color = imagecolorallocate($im, 0, 0, 0);
  for($i=0;$i<4;$i++)
    imageline($im,rand(1,120),rand(1,30),rand(1,120),rand(1,30),$text_color);


  $text = queryone("SELECT activatenumber FROM activation WHERE id=$id");
//  $text = "".rand(100000,999999);

  $x = rand(1,10);
  for($i=0;$i<strlen($text);$i++) {
    Letter($im,$x,rand(1,10),$text[$i]);
    $x += rand(14,20);
 
  }


  imagejpeg($im,'',60);
  imagedestroy($im);
  return;
}

  
  
  $userid = queryone("SELECT userid FROM activation WHERE id=$id");
  $nick = queryone("SELECT nick FROM users WHERE id=$userid");

  echo "<font face=verdana size=2>";	 
  echo "Drag-Central Account Activation<br>";
  echo "Nick: [$nick]<br>";
  
  
  if(strlen($number)!=0) {
     $realnumber = queryone("SELECT activatenumber FROM
       activation WHERE id=$id");
     $try = queryone("SELECT trycount FROM activation WHERE id=$id");     
     sleep($try);
     if($realnumber==$number) {
       query("UPDATE activation SET activated=1 WHERE id=$id");       
     } else {
       echo "You entered wrong number. Try again.<br>";
     }
  }
  
  $activated = queryone("SELECT activated FROM activation WHERE id=$id");
  if($activated) {
    echo "Account activated. You may login in game.<br>";    
    return;	
  }  
  
  
  $random = rand(100000,999999);
 
  query("UPDATE activation SET activatenumber='$random',trycount=trycount+1 WHERE id=$id");
//  sleep(1);


  $rnpic=microtime_float().rand(1000000000,9999999999);
  

   echo
   "<FORM ACTION=?>
    <input type=hidden name=id value=$id>
    <input type=hidden name=rn value=$rn>
    <table>
    <tr>
    <td>
    Turing number:
    <td>
    <input type=text name=number value=$number>
    <td>
    <img src=?act=image&id=$id&rn=$rn&rnpic=$rnpic><br>
    <tr>
    <td>
    <td>
    <td>
    <font size=0>Enter sequence of numbers displayed above.</font><br>
    <tr>
    <td>
    <td>
    <input type=submit   value='Activate account'>
    <td>
    </table>
    </FORM>";



?>
