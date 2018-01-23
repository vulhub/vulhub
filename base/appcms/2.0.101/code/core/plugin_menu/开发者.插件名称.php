<?php
/**
 * AppCMS Copyright (c) 2012-2013  
 * AppCMS is a free open source mobile phone APP application download website content management system.
 * Custom development, production BUG report template, please contact loyjers@qq.com
 * Author: loyjers
 * Editor: crane,loyjers
 */
/* 
 * 插件菜单，请参照/core/config.struct.php 里的$menu菜单数组的格式，进行push插入
 * 如 下所示*/
 $tmp_menu = array('title' => '清理缓存1', 'url' => 'cache.php',);
 array_push($menu[0]['sonmenu'],$tmp_menu);

?>