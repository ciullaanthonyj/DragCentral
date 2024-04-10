<?php
ini_set('zlib.output_compression_level', 9);
ob_start("ob_gzhandler");
?>

<?php
include_once "whothis.php";
// tryed to make it in one file it is kinda confusing
// INCOMING PARAMETERS show AND act

include "../connect.php";
include "../settings.php";
include "../functions.php";

$rnd  = microtime_float();
echo "<font face=tahoma size=1>";
 echo "<font color=red size=3>DRAG CENTRAL ADMIN CONTROL PANEL</font><br>";
 echo "<a href=?show=rooms&rnd=$rnd>ROOMS</a> :: ";
 echo "<a href=onlinegraph.php?rnd=$rnd>ONLINE GRAPHICS</a> :: ";
 echo "<a href=?show=users&rnd=$rnd>SHOW USERS</a> :: ";
 echo "<a href=?show=admins&rnd=$rnd>ADMINS AND MODERS</a> :: ";
 echo "<a href=?show=challenges&rnd=$rnd>CHALLENGES</a> ::";
 echo "<a href=?show=bans&rnd=$rnd>BANS</a> :: ";
 echo "<a href=?show=cheaters&rnd=$rnd>CHEATER IP BANS</a> :: ";
 echo "<a href=money_trans.php?rnd=$rnd>MONEY TRANSACTIONS LOG</a> :: ";
 echo "<a href=teams.php?rnd=$rnd>TEAMS</a> :: ";
 echo "<a href=wordsfilter.php?rnd=$rnd>WORDS FILTER</a> :: ";
 echo "<a href=bm.php?rnd=$rnd>SERVER LOAD</a>  :: ";
 echo "<a href=members.php?rnd=$rnd>MEMBERSHIPS</a>  :: ";
 echo "<a href=accountstats.php?rnd=$rnd>ACCOUNT STATS</a>  :: ";
 echo "<a href=sysmessage.php?rnd=$rnd>MESSAGE TO EVERY ROOM</a>  :: ";
 echo "<a href=sysmessages.php?rnd=$rnd>PERIODICAL SYSTEM MESSAGES</a>  :: ";
 echo "<a href=doubleaccounts.php?rnd=$rnd>DOUBLE ACCOUNTS</a>  :: ";
 
 echo "<br>";
 

$show=$_GET["show"]; 
$act=$_GET["act"];
$id   = $_GET["id"];
$val = $_GET["val"];

function showuser($id) {

    $rnd  = time();
    echo "Nick: <font size=4>".queryone("SELECT nick FROM users WHERE id=$id")."</font>"." <a href=editnick.php?rnd=$rnd&id=$id><-edit</a>";    
    echo " Money: ".queryone("SELECT money FROM users WHERE id=$id");
    echo " Admin: ".queryone("SELECT admin FROM users WHERE id=$id");
    echo " Member: ".queryone("SELECT member FROM users WHERE id=$id");
    echo " Moder: ".queryone("SELECT moder FROM users WHERE id=$id");
    echo " Invisible: ".queryone("SELECT invisible FROM users WHERE id=$id");
    echo " Male: ".queryone("SELECT if(male,'male','-') FROM users WHERE id=$id");
    echo " Female: ".queryone("SELECT if(female,'female','-') FROM users WHERE id=$id");
    echo " E-mail: ".queryone("SELECT email FROM users WHERE id=$id")." <a href=editemail.php?rnd=$rnd&id=$id><-edit</a>";
    echo "<br>Choose action<br>";
    echo "<a href=?rnd=$rnd&act=deluser&id=$id>Delete user</a> :: ";
    echo "<a href=?rnd=$rnd&act=setadmin&val=1&id=$id>Set admin</a> :: ";
    echo "<a href=?rnd=$rnd&act=setadmin&val=0&id=$id>Unset admin</a> :: ";
    echo "<a href=?rnd=$rnd&act=setmember&val=1&id=$id>Set member</a> :: ";
    echo "<a href=?rnd=$rnd&act=setmember&val=0&id=$id>Unset member</a> :: ";
    echo "<a href=?rnd=$rnd&act=setmoder&val=1&id=$id>Set moderator</a> :: ";
    echo "<a href=?rnd=$rnd&act=setmoder&val=0&id=$id>Unset moderator</a> :: ";
    echo "<a href=?rnd=$rnd&act=setinvisible&val=1&id=$id>Set invisible</a> :: ";
    echo "<a href=?rnd=$rnd&act=setinvisible&val=0&id=$id>Unset invisible</a> :: ";
    echo "<a href=?rnd=$rnd&act=setmale&id=$id>Set male</a> :: ";
    echo "<a href=?rnd=$rnd&act=setfemale&id=$id>Set female</a> :: ";
    
    echo "<br>";
    echo "<a href=managemoney.php?rnd=$rnd&id=$id>Manage money</a> :: ";
    echo "<br>";    
    echo "<a href=?rnd=$rnd&show=userips&id=$id>User ips log</a> :: ";
    echo "<a href=?rnd=$rnd&show=sameips&id=$id>Users with same ip</a>";
    echo "<br>";
    echo "<a href=?rnd=$rnd&show=challenges_outcoming&id=$id>Show Outgoing Challenges</a>";
    echo "::";
    echo "<a href=?rnd=$rnd&show=challenges_incoming&id=$id>Show Incoming Challenges</a>";
    echo "::";
    echo "<a href=?rnd=$rnd&act=deleteanswered&id=$id>Delete Answered Challenges</a>";
    echo "::";
    echo "<br>";
    $carscount = queryone("select count(id) from usercars where userid=$id");
    echo "<a href=?rnd=$rnd&show=usercars&id=$id>Show User Cars [$carscount]</a>";  
	
	echo "<br>";
    
}

  if($act=="delroom") {
    if(strlen($id)==0) die("No id given");   
    query("DELETE FROM rooms WHERE id=$id and permanent<>1");
    die ("ROOM DELETED.");

  }

  if($act=="userinfo") {
    echo queryone("SELECT nick FROM users WHERE id=$id");
    die();
  }

  if($act=="modifyuser") {
    showuser($id);
    die ();
  }


  if($act=="setmoder") {
    query("UPDATE users SET moder=$val WHERE id=$id");
    showuser($id);
    if($val)
      die("SET MODER"); 
    else
      die("UNSET MODER");
  }

  if($act=="setadmin") {
    query("UPDATE users SET admin=$val WHERE id=$id");
    showuser($id);
    if($val)
      die("SET ADMIN"); 
    else
      die("UNSET ADMIN");
  }

  if($act=="setfemale") {
    query("UPDATE users SET female=1,male=0 WHERE id=$id");
    showuser($id);
    die("SET FEMALE"); 
  }

  if($act=="setmale") {
    query("UPDATE users SET male=1,female=0 WHERE id=$id");
    showuser($id);
    die("SET MALE"); 
  }

  if($act=="setinvisible") {
    query("UPDATE users SET invisible=$val WHERE id=$id");
    showuser($id);
    if($val)
      die("SET INVISIBLE"); 
    else
      die("UNSET INVISIBLE");
  }
  if($act=="setmember") {
    query("UPDATE users SET member=$val WHERE id=$id");
    showuser($id);
    if($val)
      die("SET MEMBER"); 
    else
      die("UNSET MEMBER");
  }

  if($act=="deluser") {
    query("DELETE FROM users WHERE id=$id");
    showuser($id);
    die("USER DELETED.");
  }

  if($act=="deleteanswered") {
    showuser($id);
    query("delete from challenges where userid=$id and answered=1");    

  }

  if($show=='userips') {
     showuser($id);
	 echo "<br>USER IPS LOG<br>";	 
	 htmlquery(query("SELECT * FROM iptrack WHERE userid=$id ORDER BY id DESC")); 
  }
   
  if($show=='sameips') {    
    showuser($id);
    echo "<br>SAME IPS<br>";
    htmlqueryID(query("SELECT iptrack.ip,
               users.id, users.nick, users.pass, users.email, 
               if(users.male,'male','') as male, 
			   if(users.female,'female','') as female
       from iptrack left join iptrack as iptrack2 on iptrack.ip = iptrack2.ip
       left join users on iptrack2.userid=users.id
       where iptrack2.userid<>iptrack.userid and iptrack.userid=$id;"),
	   "<a href=?act=modifyuser&id=$id&rnd=$rnd>Details</a>");
    
//    htmlqueryID( query("SELECT users2.id, users2.nick, users2.pass, users2.email, 
//	                   if(users2.male,'male','') as male, 
//					   if(users2.female,'female','') as female,
//					    users2.lastip FROM users 
//	                  LEFT JOIN users as users2 ON users2.lastip=users.lastip
//	                  WHERE users.id=$id AND (users2.lastip<>'' OR users2.id=$id)
//					  ORDER BY users2.nick"), 
//				  "<a href=?show=sameips&id=ID&rnd=$rnd>Details</a>"); 
    
    
  }

  if($act=="challenge_kill") {
    $challengeid = $_GET["challengeid"];
    query("DELETE FROM challenges WHERE id=$challengeid");
    echo("CHALLENGE $challengeid DELETED");  
  }

  $challengeselect =  "
                       challenges.id, 
                       if(forfun,'fun','-') as fun, if(formoney,'money','-') as money, if(wager,wager,'-') wager, if(bracket,'bracket','-') as bracket, 
                       if(headsup,'headsup','-') as headsup,
                       if(answered,'answered','waiting') as answered, if(answerwin,'answerwin','answerlose') as answerwin,
                       if(falsestart,'falsestart','-') as falsestart,
                       reactiontime,racetime,brackettime, if(answerfalsestart,'answerfalsestart','-') answerfalsestart,
                       answerreactiontime,answerracetime,answerbrackettime,
                       creationtime, if(draw,'draw','-') as draw";
   
  if($show=="challenges_outcoming") {
    showuser($id);
    htmlqueryID(query("select users.nick,  $challengeselect
                       from challenges left join users on challenges.touserid=users.id where userid=$id order by challenges.id"),
           "<a href=?act=challenge_kill&show=challenges_outcoming&challengeid=ID&rnd=$rnd&id=$id>Kill Challenge</a>");

  }

  if($show=="challenges_incoming") {
    showuser($id);
    htmlqueryID(query("select users.nick,
                       challenges.id, $challengeselect
                        from challenges left join users on challenges.userid=users.id where touserid=$id order by challenges.id"),
           "<a href=?act=challenge_kill&show=challenges_incoming&challengeid=ID&rnd=$rnd&id=$id>Kill Challenge</a>");

  }

   
  function deletecar($carid) {
    $selected = queryone("SELECT selected FROM usercars WHERE id=$carid");
    $inteam = queryone("SELECT count(id) FROM team_members WHERE usercarid=$carid");
    if($selected) {
      echo "<font color=red>Can not remove selected car</font><br>";
    } else     
    if($inteam) {
      echo "<font color=red>Can not remove car that is in team</font><br>";      
    } else {
      query("DELETE FROM carparts WHERE usercarid=$carid");
      query("DELETE FROM challenges WHERE usercarid=$carid or answerusercarid=$carid");
      query("DELETE FROM usercars WHERE id=$carid");
    } 
    
  }
  if($act=='delete_all_cars') {
    $res = query("SELECT id FROM usercars WHERE userid=$id");
    while ($line = mysql_fetch_array($res, MYSQL_ASSOC)) {
       deletecar($line['id']);       
    }
  }

  if($act=='remove_car') {
    deletecar($carid);	
  }
  
  
  if($show=="usercars") {
    showuser($id);
    echo "<a href=?act=delete_all_cars&show=usercars&id=$id&rnd=$rnd> DELETE ALL USER CARS</a>";
    htmlqueryID(query("select usercars.id, cars.name, selected from usercars 
                       left join cars on carid=cars.id
	                   where userid=$id"),
           "<a href=?act=remove_car&show=usercars&carid=ID&rnd=$rnd&id=$id>Delete car</a>");
  }



  if($show=="rooms") {
     include "rooms.php";
     return;
   
  }

  if($show=="admins") {
     $countadmins = queryone("SELECT count(id) FROM users WHERE admin=1");
     $countmoders = queryone("SELECT count(id) FROM users WHERE moder=1");
     $countmembers = queryone("SELECT count(id) FROM users WHERE member=1");
     echo "Admins:$countadmins<br>";
     htmlquery(
       query("SELECT nick,email,admin,moder,member,male,female FROM users WHERE admin=1 ORDER BY nick")
     );
     echo "Moderators:$countmoders<br>";
     htmlquery(
       query("SELECT nick,email,admin,moder,member,male,female FROM users WHERE moder=1 ORDER BY nick")
     );
     echo "Payed members:$countmembers<br>";
  }
  // USERS
  if($show=="users") {
    
    
    $nick = $_GET['nick'];
      echo "
        <FORM ACTION=?>
        <input type=hidden name=rnd value=$rnd>
        <input type=hidden name=show value=users>
       Search for nick to search for part enter '%part%':<br>
       <input type=text name=nick value='$nick'>
      <input type=submit   value='Process'>
      </FORM>";
    
    
    if(!is_null($nick)) {
     $count = queryone("SELECT count(id) FROM users WHERE nick like '$nick'");
     echo "$count user(s).<br>";

     $result = query("SELECT * FROM users WHERE nick like '$nick' ORDER BY nick ;");

     // Printing results in HTML
     echo "<table border=0  cellpadding=1 cellspacing=0 font=verdana size=1>\n";
     echo "<tr><td>Details<td>nick<td>pass<td>email<td>money<td>login ago<td>member<td>admin<td>moder<td>male<td>female<td>invisible<td>lastip";
     while ($line = mysql_fetch_array($result, MYSQL_ASSOC)) {
        echo "\t<tr>\n";    
        $nick = $line['nick'];
        $pass = $line['pass'];
        $email = $line['email'];
        $moder = $line['moder']  ? "moder" : "-";
        $admin = $line['admin']  ? "admin" : "-";
        $member = $line['member'] ? "member" : "-";
        $female = $line['female'] ? "female": "-";
        $male = $line['male'] ? "male" : "-";
        $money = $line['money'];
        $time = $line['lastlogintime'];
        $invisible = $line['invisible'] ? "invisible": "-";
        $lastip = $line['lastip'];
        if($time!="" && $time!=0 && strlen($time)!=0) {
           $time=TimePeriodShort(time()-$time);
        } else
           $time="-";
         
        $id = $line['id'];

        echo "<td><a href=?act=modifyuser&id=$id&rnd=$rnd>Details</a><td>$nick<td>$pass<td>$email<td>$money<td>$time<td>$member<td>$admin<td>$moder<td>$male<td>$female<td>$invisible<td>$lastip";
    }
     echo "</table>\n";
   }
   die();
 }


  // CHALLENGES
  if($show=="challenges") {
     $count = queryone('SELECT count(id) FROM challenges');
     echo "last 30 challenge(s) from $count challenge(s).<br>";
     $result = query('SELECT users.nick as fromuser,users2.nick as touser,bracket,headsup,brackettime,
	           racetime,falsestart,answerbrackettime,answerracetime,answerfalsestart,forfun,formoney,wager, 
               answered,answerwin,draw,creationtime as time FROM challenges 
               LEFT JOIN users ON challenges.userid=users.id 
               LEFT JOIN users as users2 ON challenges.touserid = users2.id 
               ORDER BY challenges.id DESC LIMIT 30;');

     htmlquery($result);
     die();
  }

  // BANS
  if($act=="killban") {
    query("DELETE FROM bans WHERE id=$id");
    echo("BAN [$id] WAS REMOVED");
  }

  if($show=="bans") {

     $time = time();
     $result = query("SELECT bans.id,users.nick as usernick, users2.nick as modernick, game, chat, timestamp as time, length as period, (timestamp+length)-$time as leftsec, reason FROM bans".
                     ' LEFT JOIN users ON bans.userid=users.id '.
                     ' LEFT JOIN users as users2 ON bans.moderid=users2.id '.
                     ' ORDER BY bans.id;');

     htmlqueryID($result,"<a href=?act=killban&show=bans&id=ID&rnd=$rnd>Kill Ban</a>");

  }
  if($act=='killcheatban') {
    query("DELETE FROM bruteiphistory WHERE bruteipid=$id");
    query("DELETE FROM bruteips WHERE id=$id");
    echo "Ban killed [$id].<br>";
  }

  if($show=="cheaterban") {
    htmlquery(query("SELECT * FROM bruteiphistory WHERE bruteipid=$id ORDER BY id"));	

  }
  if($act=="banip") {
    $ip = $_GET['ip'];
    $time = time();
    query("INSERT INTO bruteips(ip,lasttime,count) VALUES ('$ip',$time,100)");
  }
  if($show=="cheaters") {
    echo "
      <FORM ACTION=?>
      <input type=hidden name=rnd value=<?php echo $rnd ?>>
      <input type=hidden name=show value=cheaters>
      <input type=hidden name=act  value=banip>
       IP Addres to ban:<br>
       <input type=text name=ip>
      <input type=submit   value='Process'>
      </FORM>";

     htmlqueryID( query("SELECT id,ip,lasttime as time,count FROM bruteips"),
      "<a href=?act=killcheatban&show=cheaters&id=ID&rnd=$rnd>Kill Ban</a> :: <a href=?show=cheaterban&id=ID&rnd=$rnd>Queries</a>");

  }

?>