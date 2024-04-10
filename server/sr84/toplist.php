<?php


  include "whothis.php";

  $show = $_GET["show"];

//	  CONCAT(TRUNCATE(if(caseone,reactiontime,answerreactiontime),2),'/',
//	  TRUNCATE(if(caseone,racetime,answerracetime),2)), 


  if($show=="topteams") {
    $res = query("SELECT name,wins,loses FROM teams WHERE wins<>0 ORDER BY wins DESC LIMIT 20");
	echoquery($res);
	return;    
  }

  if($show=="topresults") {
    $res = query("SELECT users.nick, CONCAT('Rank: ',users.rank) FROM users ORDER BY rank DESC LIMIT 30");
    echoquery($res);
    return;      
  }

  if($show=="topcars") {

      $res = query("SELECT users.nick, 
	  TRUNCATE( min(if(caseone,reactiontime+racetime,answerreactiontime+answerracetime))  ,2) as minracetime 
	                FROM challenges 
                    LEFT JOIN variants ON 1
                    LEFT JOIN users ON if(caseone,userid,touserid)=users.id
                    WHERE 
                      if(caseone,racetime,answerracetime)>0 AND 
                      if(caseone,falsestart,answerfalsestart)=0 AND
                      headsup=1 AND
                      answered=1                      
                          
                    GROUP BY users.nick
                    ORDER BY minracetime
                    LIMIT 30
                    "
                   );


    // mysql 5.0
//    if($_GET["mysql"]=="5")
//    $res = query("
//
//        SELECT nick, min(racetime) as minracetime
//        FROM (
//           SELECT userid, users.nick, racetime
//           FROM challenges
//           LEFT JOIN users ON userid=users.id
//           WHERE racetime>0 AND falsestart=0 AND headsup=1 AND answered=1
//           UNION
//           SELECT touserid, users.nick, answerracetime        
//           FROM challenges
//           LEFT JOIN users ON touserid=users.id
//           WHERE answerracetime>0 AND answerfalsestart=0 AND headsup=1 AND answered=1
//        ) AS allres
//        GROUP BY userid
//        ORDER BY minracetime
//        LIMIT 20

//        ");
    
    
    echoquery($res);


  }



?>