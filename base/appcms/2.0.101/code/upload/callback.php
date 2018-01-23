<?php
    $params=$_GET['params'];
    $params=preg_replace('~(\\\")~','"',$params);
    echo('<script>window.parent.callback_upload(\''.$params.'\');</script>');
?>
