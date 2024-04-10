<?php

  include_once "money.php";
  function ProcessOldCompChallenges($id) {

    while($oldid=queryone("SELECT id FROM compchallenges WHERE userid=$id")) {
      $wager = queryone("SELECT wager FROM compchallenges WHERE id=$oldid");
      $change = -$wager;
      query("UPDATE users SET money=if(money+$change<0,0,money+$change) WHERE id=$id");  
      query("DELETE FROM compchallenges WHERE id=$oldid");
    }
  }

  function ProcessNotAnswerEndedChallenges($userid) {

    while($oldid=queryone("SELECT id FROM challenges WHERE NOT answered=1 AND touserid=$userid AND answerstart=1 AND answerend=0 AND createend=1")) {

      $result = query("SELECT userid,touserid,formoney,wager,falsestart,bracket,brackettime,racetime,reactiontime FROM challenges WHERE id=$oldid");
      $line = mysql_fetch_array($result, MYSQL_ASSOC);

      $_formoney = $line['formoney'];
      $_wager = $line['wager'];
      $_userid = $line['userid'];
      $_touserid = $line['touserid'];
      $_falsestart = $line['falsestart'];
      $_bracket = $line['bracket'];
      $_racetime = $line['racetime'];
      $_reaction = $line['reactiontime'];
      $_brackettime = $line['brackettime'];

      $_foul = $_falsestart || ($_bracket && ( ($_racetime+$_reaction)<$_brackettime )   );

      // IF INITIATOR FOULED
      if($_foul) {

        query("UPDATE challenges SET answerfalsestart=1, draw=1,answered=1,answerend=1 WHERE id=$oldid");         

      }      

      if(!$_foul) {
        if($_formoney) {

          // CHANGE INITIATOR ACCOUNT
          changemoney($_userid,$_touserid,$_wager,"opponent answer failed: win challenge");
          // CHANGE ANSWERER ACCOUNT
          changemoney($_touserid,$_userid,-$_wager,"user answer failed: lose challenge");

        }

        query("UPDATE challenges SET answerfalsestart=1,answered=1,answerend=1 WHERE id=$oldid");
      }  

    }
  }

  function ProcessNotCreateEndedChallenges($userid) {

    while($oldid=queryone("SELECT id FROM challenges WHERE userid=$userid AND createstart=1 AND createend=0")) {
      query("UPDATE challenges SET falsestart=1, createend=1 WHERE id=$oldid");
    }

  }

?>