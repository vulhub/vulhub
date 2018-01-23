<?php
/**
 * AppCMS Copyright (c) 2012-2013  
 * AppCMS is a free open source mobile phone APP application download website content management system.
 * Custom development, production BUG report template, please contact loyjers@qq.com
 * Author: loyjers
 * Editor: crane,loyjers
 */
// 左侧菜单导航
$menu = array(
    array('title' => '系统管理',
        'url' => '',
        'bgimg' => 'templates/css/img/menu1/system.png',
        'sonmenu' => array(
            array('title' => '管理首页', 'url' => 'frame.php',),
            array('title' => '网站配置', 'url' => 'set.php',),
            array('title' => '后台账号', 'url' => 'admin_user.php',),
            array('title' => '伪静态规则', 'url' => 'url_rewrite.php',),
			array('title' => '清理缓存', 'url' => 'cache.php',),
            ),
        ),
    array('title' => '赚钱联盟',
        'url' => '',
        'bgimg' => 'templates/css/img/menu1/union.png',
        'sonmenu' => array(
            array('title' => '', 'url' => 'product.php',),
            ),
        ),
    array('title' => '分类管理',
        'url' => '',
        'bgimg' => 'templates/css/img/menu1/category.png',
        'sonmenu' => array(
            array('title' => '分类列表', 'url' => 'category.php',),
            ),
        ),
    array('title' => '应用管理',
        'url' => '',
        'bgimg' => 'templates/css/img/menu1/app.png',
        'sonmenu' => array(
            array('title' => '应用列表', 'url' => 'app.php',),
            array('title' => '添加应用', 'url' => 'collect.php',),
			array('title' => '百度提交', 'url' => 'baidu_app.php',),
			array('title' => '定时更新', 'url' => 'autodown.php',),
            array('title' => '图片本地化', 'url' => 'download.php',),
			array('title' => '推荐位', 'url' => 'recommend.php?type=0',),
            array('title' => '应用专题', 'url' => 'special.php?type=3',),
            ),
        ),
    array('title' => '资讯管理',
        'url' => '',
        'bgimg' => 'templates/css/img/menu1/info.png',
        'sonmenu' => array(
            array('title' => '资讯列表', 'url' => 'info.php',),
			array('title' => '百度提交', 'url' => 'baidu_info.php',),
            array('title' => '推荐位', 'url' => 'recommend.php?type=1',),
            array('title' => '资讯专题', 'url' => 'special.php?type=4',),
            ),
        ),
    array('title' => '扩展模块',
        'url' => '',
        'bgimg' => 'templates/css/img/menu1/extern.png',
        'sonmenu' => array(
            array('title' => '友情链接', 'url' => 'flink.php',),
            array('title' => '正文内链', 'url' => 'nlink.php',),
            array('title' => '评论管理', 'url' => 'comment.php',),
            array('title' => '广告管理', 'url' => 'advert.php',),
            array('title' => '备份还原', 'url' => 'dbbak.php',),
			array('title' => '关键字管理', 'url' => 'keyword.php',),
			array('title' => '应用百度提交', 'url' => 'baidu1.php',),
			array('title' => '资讯百度提交', 'url' => 'baidu2.php',),
               ),
        ),
    array('title' => '模板管理',
        'url' => '',
        'bgimg' => 'templates/css/img/menu1/template.png',
        'sonmenu' => array(
            array('title' => 'PC站模板', 'url' => 'template.php',),
			array('title' => '手机版模板', 'url' => 'template_wap.php',),
            ),
        ),
    );
   
/*
 * 遍历包含插件管理菜单
 * 插件开发者的菜单在 /core/plugin_menu 目录中，文件名以 插件开发者.插件名称.php 的格式命名比较好，防止跟其他开发者冲突
 */
 /*
 $tpl_filename=str_replace('\/', '', '\../core/plugin_menu/*.*');
 //$tpl_filename=str_replace('/','',$tpl_filename);
 //echo $tpl_filename;
foreach(glob(dirname(__FILE__).$tpl_filename) as $v){
    $file=preg_split('~\/core\/~',$v);
    if(count($file)>1){
        require_once(dirname(__FILE__) . '/'.$file[1]);
    }
}
*/
?>