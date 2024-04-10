<font face=verdana size=1>
<?php

include "whothis.php";

$rnd  = time();

include "../connect.php";
include "../functions.php";

$id=$_GET['id'];
$act=$_GET['act'];
$value=$_GET['value'];

$time = time();


if($act=='add') {
  if(strlen($value)!=0) {

    $money = queryone("SELECT money FROM users WHERE id=$id");

    $moneynew = $money  + $value;

    $time = time();
    query("INSERT INTO money(userid,opponentid,value,comment,time,startmoney,endmoney) 
           VALUES ($id,0,$value,'change from admin panel',$time,$money,$moneynew)");

    query("UPDATE users SET money=$moneynew WHERE id=$id");


    echo "Added [$value] was [$money]<br>";
  } else {
    echo "ERROR: Enter value to add.<br>";
  }

}

if($act=='set') {

  if(strlen($value)!=0) {
    $money = queryone("SELECT money FROM users WHERE id=$id");

    query("UPDATE users SET money=$value WHERE id=$id");

    echo "Money set to [$value] from [$money]<br>";

    query("INSERT INTO money(userid,opponentid,value,comment,time,startmoney,endmoney) 
           VALUES ($id,0,$value,'set from admin panel',$time,$money,$value)");


  } else {
    echo "ERROR: Enter value to set.<br>";
  }


}

echo "Nick:".queryone("SELECT nick FROM users WHERE id=$id")."<br>";
echo "Money:".queryone("SELECT money FROM users WHERE id=$id")."<br>";



?>

<FORM ACTION=?>
<input type=hidden name=rnd value=<?php echo $rnd ?>>
<input type=hidden name=act value=add>
<input type=hidden name=id value=<?php echo $id ?>>
Value to add (negative value to substract):<br>
<input type=text name=value>
<input type=submit   value='Process'>
</FORM>

<FORM ACTION=?>
<input type=hidden name=rnd value=<?php echo $rnd ?>>
<input type=hidden name=act value=set>
<input type=hidden name=id value=<?php echo $id ?>>
Set money to:<br>
<input type=text name=value>
<input type=submit   value='Process'>
</FORM>

<?php
  echo "<a href=admin.php?rnd=$rnd>BACK TO ADMIN</a>";
  htmlquery( query("SELECT * FROM money WHERE userid=$id ORDER BY id DESC LIMIT 100"));
?>
