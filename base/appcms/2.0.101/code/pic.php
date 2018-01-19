<?php
if(isset($_GET['url']) && trim($_GET['url']) != '' && isset($_GET['type'])) {
    $img_url=trim($_GET['url']);
	$img_url = base64_decode($img_url);
    $img_url=strtolower(trim($img_url));
    $_GET['type']=strtolower(trim($_GET['type']));
    
    $urls=explode('.',$img_url);
    if(count($urls)<=1) die('image type forbidden 0');
    $file_type=$urls[count($urls)-1]; 
    
    if(in_array($file_type,array('jpg','gif','png','jpeg'))){}else{ die('image type foridden 1');}

    if(strstr($img_url,'php')) die('image type forbidden 2');

    if(strstr($img_url,chr(0)))die('image type forbidden 3');
    if(strlen($img_url)>256)die('url too length forbidden 4');

    header("Content-Type: image/{$_GET['type']}");
    readfile($img_url);
    
} else {
	die('image not find！');
}

?>