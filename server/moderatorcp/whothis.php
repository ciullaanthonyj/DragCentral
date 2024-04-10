<?php

//check if user is moderator, and wasn't demoded while in mod cp

  if($_SESSION['authorized']===true) { 
    $id = $_SESSION['id'];
    $moder = queryone("SELECT moder FROM users WHERE id=$id");
    if($moder!=1) {
	  $_SESSION['authorized']=false;
      die("Sorry, you were demoded");
    }
  }
  


?>