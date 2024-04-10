<?

include "backtoadmin.php";


$id=$_GET['id'];
$act=$_GET['act'];
$newvalue=$_GET['newvalue'];

$time = time();

$fieldname = 'nick';

if($act=='edit') {  
  if(strlen($newvalue)!=0) {
    query("UPDATE users SET $fieldname='$newvalue' WHERE id=$id");
    echo "<font color=red>$fielname changed</font><br>";
  }
}

$curvalue = queryone("SELECT $fieldname FROM users WHERE id=$id");

echo "Nick:".$curvalue."<br>";
//echo "Email:".queryone("SELECT email FROM users WHERE id=$id")."<br>";



echo "<FORM ACTION=?>
<input type=hidden name=rnd value=$rnd>
<input type=hidden name=act value=edit>
<input type=hidden name=id value=$id>
Change $fieldname to:<br>
<input type=text name=newvalue value='$curvalue'>
<input type=submit   value='Process'>
</FORM>";

?>
