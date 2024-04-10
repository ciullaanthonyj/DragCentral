<?php

error_reporting(E_ERROR);
//echo "starting socket server<br>";

/* Allow the script to hang around waiting for connections. */
set_time_limit(0);

/* Turn on implicit output flushing so we see what we're getting
 * as it comes in. */
ob_implicit_flush();

$address = $_SERVER["SERVER_ADDR"];
$port = 10000;

//echo "socket create";
$sock = socket_create(AF_INET, SOCK_STREAM, SOL_TCP);
if($sock===false) {
    die("socket_create() failed: reason: " . socket_strerror($sock) . "\n");
}

//echo "start binding";
// BIND UNTIL FIND RIGHT SOCKET
while(true) {
//  echo "trying port: $port<br>";
  flush();
  if (false === socket_bind($sock, $address, $port)) {
//    echo "abort port: $port<br>";
    $port++;
    if($port==20000) die ("Game Full");
  } else {
    echo "PR$port";
    flush();  
    ob_end_flush();
    break;
  }
}


if (($ret = socket_listen($sock, 5)) < 0) {
    echo "socket_listen() failed: reason: " . socket_strerror($ret) . "\n";
}

$includeout = "";
function callback($buffer) {
  global $includeout;
  $includeout .= $buffer;
  return "";
}

do {
    if (($msgsock = socket_accept($sock)) < 0) {
        echo "socket_accept() failed: reason: " . socket_strerror($msgsock) . "\n";
        break;
    }
    $count = 0;
    do {
        if (false === ($buf = socket_read($msgsock, 2048, PHP_NORMAL_READ))) {
            echo "socket_read() failed: reason: " . socket_strerror($ret) . "\n";
            break 2;
        }
//        $count++;
//        echo "$count:";
//        if($count==10000) {
//          socket_close($msgsock);
//          break 2;
//        }
        
        if (!$buf = trim($buf)) {
            continue;
        }
        if ($buf == 'quit') {
            break;
        }
        if ($buf == 'shutdown') {
            socket_close($msgsock);
            break 2;
        }  
        
        $p = strpos($buf,"?");
        if($p!==FALSE) {
          $script = substr($buf,0,$p);  
          $params = substr($buf,$p+1,strlen($buf));
        } else {
          $script = $buf;
		  $params = ""; 
        }
        
        if(strpos($script,"..")!==false ||
	       strpos($script,"\\")!==false ||
  		   strpos($script,"/")!==false ||
		   strpos($script,":")!==false) {
          continue;
        }        
        
        $_SERVER["QUERY_STRING"] = $params;
        $includeout = "";
        ob_start("callback");       
        include $script;
        ob_end_flush();
        $includeout = $includeout."\r\nEND\r\n";
        socket_write($msgsock, $includeout, strlen($includeout));
    } while (true);
    socket_close($msgsock);
    
} while (true);

socket_close($sock);
?>