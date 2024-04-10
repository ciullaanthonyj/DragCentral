<?php  
  
  include "../functions.php";
  include "../connect.php";
  session_start();
  include "whothis.php":
  if(!isset($_SESSION['authorized']) || !$_SESSION['authorized']) {
    echo "Not authorized.";
    return;
  }

  $id = $_SESSION['id'];
  $nick = queryone("SELECT nick FROM users WHERE id=$id");

  parse_str($_SERVER['QUERY_STRING']);
  $rnd = "&r=".microtime_float();

  if(strlen($act)==0) $act = $_POST['act'];
  if($act=='add') {
      $time = time();
      $bug = $_POST['bug'];
      query("INSERT INTO bugs(bug,author, timestamp)
             VALUES('$bug','$nick', $time)");
  }  

  echo "<font face=verdana size=1>";
  echo "<font color=red size=4>DC Bug Reports</font>";    
  echo "<br><a href=index.php?$rnd><< back to main menu</a>";
  if($act=='delete') {
     query("DELETE FROM bugs WHERE id=$bugid");    
  }
  htmlqueryID(
    query("SELECT id,timestamp, bug, author, fixed FROM bugs ORDER BY id DESC"),
    "<a href=?act=delete&bugid=ID$rnd>delete</a>"
  );

  echo "<FORM action=? METHOD=post>
    <input type=hidden name=act value=add>
    <input type=hidden name=rnd value=$rnd>
    Bug description<br>
    <TEXTAREA ROWS=7 COLS=80 NAME=bug>$bug</TEXTAREA><br>
	<input type=submit value=' Add bug report '>
    </FORM>
  "; 

?>