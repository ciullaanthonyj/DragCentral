<?php


include "whothis.php";
include_once "../settings.php";
include_once "challenge.php";

  $userid = $_GET["userid"];
  $type = $_GET["type"];


  $ago = time() - $deletechallengesafter;
  query("DELETE FROM challenges WHERE creationtime<=$ago");
  

  if($type=="in") {

      ProcessNotAnswerEndedChallenges($userid);      
      $result = query(
              "SELECT challenges.id,challenges.userid,
			          users.nick,forfun,bracket,answered,answerwin,
			          formoney,wager,headsup,draw,creationtime-$ago,
			          usercarid,answerusercarid,forcar,cars.membercar,message
               FROM challenges 
			   LEFT JOIN users ON challenges.userid = users.id 
			   LEFT JOIN usercars ON usercarid=usercars.id
			   LEFT JOIN cars ON usercars.carid=cars.id
			   WHERE touserid=$userid AND createend=1 AND hideincoming=0
			   ORDER BY challenges.id DESC;"
        ); 

  }

  if($type=="out") {

      ProcessNotCreateEndedChallenges($userid);      
      $result = query(
           "SELECT challenges.id,touserid,users.nick,forfun,bracket,answered,answerwin,
		    formoney,wager,headsup,draw,creationtime-$ago,forcar,message
            FROM challenges LEFT JOIN users ON touserid = users.id 
			WHERE userid=$userid AND createend=1 AND hideoutgoing=0
			ORDER BY challenges.id DESC;"
      );

  }


echoquery($result);

?>