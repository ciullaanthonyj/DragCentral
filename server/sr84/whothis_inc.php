<?php
  function FromHex($in) {

    $out = $in; 

    $l = strlen($in);
    $p = 0;

    while(true) {
      if($p>=$l) break;
   
      $c = "0x".$in[$p].$in[$p+1];
      $out[$p/2] = chr(0+$c);

      $p += 2;
    }
    $out = substr($out,0,$l/2);
    return $out;

  }

  function Decrypt($in, $Key)
  {
    $C1 = 42815;
    $C2 = 32732;
    $out = $in;
    $l = strlen($in);
    for($i=0;$i<$l;$i++) {
      $out[$i] = chr(ord($in[$i]) ^ ($Key >> 8));
      $Key = ((    ( $Key+ord($in[$i]) ) & 0xffff  ) * $C1 + $C2) & 0xffff;
    }
    return $out;
  }
?>