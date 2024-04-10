<?php
ini_set('zlib.output_compression_level', 9);
ob_start("ob_gzhandler");
?>
<?php
include "whothis.php";

include "../connect.php";
include "../settings.php";
include "../functions.php";

  htmlquery( query("SELECT * FROM bm ORDER BY timestamp DESC LIMIT 50"));





?>