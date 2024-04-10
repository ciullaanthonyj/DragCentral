<?php
ini_set('zlib.output_compression_level', 9);
ob_start("ob_gzhandler");
?>
<?php
  include "whothis.php";

include "../connect.php";
include "../settings.php"; 
include "../functions.php";
$rnd  = microtime_float();
echo "<font face=verdana size=1>";
parse_str($_SERVER['QUERY_STRING']);

echo "Chat Words Filter";

if($act=='delete') {
  query("DELETE FROM wordsfilter WHERE id=$id");   
}

if($act=='add') {
  query("INSERT INTO wordsfilter(word,repl) VALUES ('$word','$replace')");  
}

htmlqueryID(query("SELECT * FROM wordsfilter ORDER BY word"),
  "<a href=?act=delete&id=ID&rnd=$rnd>Delete</a>");
  
echo "  
<FORM ACTION=?>
<input type=hidden name=rnd value=$rnd>
<input type=hidden name=act value=add>
<table>
<tr>
<td>
Word:
<td>
<input type=text name=word value=''><br>
<tr>
<td>
Replace:
<td>
<input type=text name=replace value=''><br>
<tr>
<td>
<input type=submit   value='Add word'>
<td>
</table>
</FORM>"
  
  


?>