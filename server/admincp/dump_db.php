<?php
ini_set('zlib.output_compression_level', 9);
ob_start("ob_gzhandler");
?>
<?php

include '../connect.php';
include '../functions.php';


function DUMPTable($tablename) {
  echo "query('DELETE FROM $tablename');\r\n";
  $res=query("SELECT * FROM $tablename ORDER BY id");
  echo "echo '<br>$tablename<br>';\r\n";

  while ($line = mysql_fetch_array($res, MYSQL_ASSOC)) {

    $fields = "";
    $vals = "";

    foreach($line as $colname => $col) {

         if($fields!="")
            $fields .= ",";
         $fields .= $colname;

         if($vals!="")
           $vals .= ",";
         $fieldval = $col;
         
         
         if(    $colname=='nick' || $colname=='pass' || $colname=='email' 
		     || $colname=='name' || $colname=='comment' || $colname=='lastip' ||
			 $colname=='reason' || $colname=='ip' || $colname=='roomname' ||
                $colname=='word' || $colname=='repl') {
			   
           $fieldval = str_replace("\\","\\\\",$fieldval);
           $fieldval = str_replace('"','\"',$fieldval);
           $fieldval = str_replace("'","\'",$fieldval);
			   
           $fieldval = "'$fieldval'";
        }

         $vals .= $fieldval;        
                    
     }

     $out = "INSERT INTO $tablename($fields) VALUES($vals);";
     echo 'query("'.$out.'"'.");\r\n";
     
  }  
}

echo "<?php\r\n";
echo "include '../connect.php';\r\n";
echo "include '../functions.php';\r\n";

//   DUMPTable("users");
//   DUMPTable("challenges");
//   DUMPTable("bans");
//   DUMPTable("bruteips");
//   DUMPTable("carparts");
//   DUMPTable("usercars");
//   DUMPTable("teams");
//   DUMPTable("team_members");
//   DUMPTable("team_challengers");
//   DUMPTable("team_challenges");   
//   DUMPTable("team_requests");      
//   DUMPTable("team_money");
//   DUMPTable("money");
   DUMPTable("wordsfilter");


echo "?>\r\n";



?>