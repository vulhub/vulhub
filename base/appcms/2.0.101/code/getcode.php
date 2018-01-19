<?php
if (!session_id()) session_start();
require_once(dirname(__FILE__)."/core/imgcode.class.php");

$name=(!isset($_GET['c']) || $_GET['c']=='')?'login':$_GET['c'];
if($name=='login' || $name=='reg' || $name='feedback'){
    Image::buildImageVerify(5, 2, 'png', 60, 32, $name);
}
?>