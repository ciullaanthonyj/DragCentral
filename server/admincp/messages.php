<?php
include "whothis.php";

// GETS ALL MESSAGES

include "../connect.php";


$query = 'SELECT * FROM messages ORDER BY ID DESC';
$result = mysql_query($query) or die('Query failed: ' . mysql_error());

// Printing results in HTML
echo "<table>\n";
while ($line = mysql_fetch_array($result, MYSQL_ASSOC)) {
    echo "\t<tr>\n";
    foreach ($line as $col_value) {
        echo "\t\t<td>[$col_value]</td>\n";
    }
    echo "\t</tr>\n";
}
echo "</table>\n";

?>