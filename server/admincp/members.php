<?php
ini_set('zlib.output_compression_level', 9);
ob_start("ob_gzhandler");
  include "whothis.php";
?>
<?php
  echo "<font face=verdana>";
  include "../functions.php";
  include "../connect.php";
  $rnd  = microtime_float();
  
  parse_str($_SERVER['QUERY_STRING']);
  if($act=='modify') {
     $nick = queryone("SELECT nick FROM users WHERE id=$userid");    
     
     echo "nick: $nick<br>";
     
     echo "<a href=?rnd=$rnd&userid=$userid&act=quitmembers>delete membership</a>";
    
     echo "
          <FORM ACTION=?>
          <input type=hidden name=rnd value=$rnd>
          <input type=hidden name=act value=modifymember>
          <input type=hidden name=userid value=$userid>
          Day:<br>
         <input type=text name=day value='1'><br>
          Month:<Br>
         <input type=text name=month value='1'><br>
          Year:<br>
         <input type=text name=year value='2006'><br>
         <input type=submit   value='Set Membership Expiration Date'>
         </FORM>";
    return;
  }
  
  if($act=='modifymember') {
    $exp = mktime(0,0,0,$month,$day,$year);
    query("UPDATE users SET expires=$exp, member=1 WHERE id=$userid");
  }
  
  echo "
        <FORM ACTION=?>
        <input type=hidden name=rnd value=$rnd>
        <input type=hidden name=show value=users>
       Search for nick (to search by part enter %part%, example %jov%, %% - all members):<br>
       <input type=text name=nick value='$nick'>
      <input type=submit   value='Process'>
      </FORM>";

 if($show=='users') {
    $count = queryone("SELECT count(id) FROM users WHERE nick like '$nick' AND member=1");
    echo "Count: $count";
    htmlqueryID(
     query("SELECT id,nick,if(member,'member','-') as membership,expires as day FROM users WHERE nick like '$nick' AND member=1"),
     "<a href=?rnd=$rnd&userid=ID&act=modify>Modify</a>");
 }
?>