<?

include "backtoadmin.php";


$id=$_GET['id'];
$act=$_GET['act'];
$value=$_GET['value'];

$time = time();


if($act=='edit') {
  if(strlen($value)!=0) {
    query("UPDATE users SET email='$value' WHERE id=$id");
    echo "<font color=red>Email changed</font><br>";
  }
}

echo "Nick:".queryone("SELECT nick FROM users WHERE id=$id")."<br>";
echo "Email:".queryone("SELECT email FROM users WHERE id=$id")."<br>";



echo "<FORM ACTION=?>
<input type=hidden name=rnd value=$rnd>
<input type=hidden name=act value=edit>
<input type=hidden name=id value=$id>
Change e-mail to:<br>
<input type=text name=value>
<input type=submit   value='Process'>
</FORM>";

?>
