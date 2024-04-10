<?php

$link = mysql_connect('localhost', 'dave1991_dc', 'tooker2008;)  or die('Could not connect: ' . mysql_error());
mysql_select_db('dave1991_dc') or print('Could not select database');

function query($query) {
  $result = mysql_query($query) or print('Query failed: ' . mysql_error());
  return $result;
}


?>