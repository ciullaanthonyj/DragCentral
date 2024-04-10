<? 
  include_once "whothis.php";
  include_once "../connect.php";
  include_once "../functions.php";
  echo "<font face=verdana size=1>";

  $rnd = microtime_float();
  echo "<a href=admin.php?rnd=$rnd><< back to admin</a><br>";

?>