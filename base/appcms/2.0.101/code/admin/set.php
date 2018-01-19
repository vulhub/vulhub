<?php
/**
 * AppCMS Copyright (c) 2012-2013  
 * AppCMS is a free open source mobile phone APP application download website content management system.
 * Custom development, production BUG report template, please contact loyjers@qq.com
 * Author: chenpeng
 * Editor: chenpeng,loyjers
 */

/**
 * * 加载核心文件类和公用方法函数*
 */
require_once(dirname(__FILE__) . "/../core/init.php");

$time_start = helper :: getmicrotime(); //开始时间

$dbm = new db_mysql(); //数据库类实例

$page['get'] = $_GET; //get参数的 m 和 ajax 参数是默认占用的，一个用来执行动作函数，一个用来判断是否启用模板还是直接输出JSON格式数据
$page['post'] = $_POST;
// $page['cates']=caches('categorys','',3600,'/cache','categorys',1);
check_admin(); //判断用户是否登录

/**
 * 页面动作 model 分支选择  
 *          动作函数写在文件末尾，全部以前缀 m__ 开头
 */
$page['get']['m'] = isset($_GET['m'])?$_GET['m']:'show';

if (function_exists("m__" . $page['get']['m'])) {
    call_user_func("m__" . $page['get']['m']);
} 

$time_end = helper :: getmicrotime(); //主程序执行时间，如果要包含模板的输出时间，则可以调用该静态时间方法单独计算
$page['data_fill_time'] = $time_end - $time_start;
/**
 * 模板载入选择
 *          模板页面为PHP+HTML混合编写，如果模板页面中也有区块函数，模板函数以 tpl__ 为前缀
 */
if (!isset($page['get']['ajax']) || $page['get']['ajax'] == null || $page['get']['ajax'] == '') {
    $tpl_filename = str_replace('\\', '', str_replace(dirname(__FILE__), '', __FILE__));
    $tpl_filename = str_replace('/', '', $tpl_filename);
    require(dirname(__FILE__) . '/templates/tpl_' . $tpl_filename);
} else {
    if ($page['get']['ajax'] == 'json') {
        echo json_encode($page);
    } 
} 
// 保存
function m__save() {
    global $dbm, $page; 
    // 其他站点配置
    $config = dirname(__FILE__) . '/../core/config.php';
    $cf = file_get_contents($config);
    if (!empty($_POST)) {
        foreach($_POST as $k => $v) {
            $v = urldecode($v);
            if (get_magic_quotes_gpc()) {
                $v = stripslashes($v);
            } 
            if ($k == 'comment_code' || $k == 'count_code') {

				$v = preg_replace("~\t|\r|\n~",'', $v);

                //$v = preg_replace('~\t|\r|\n~', '', $v);

            } else {
                $v = preg_replace('~"~', '&#34;', $v);
            } 
            $v = preg_replace('~"~', '\\\"', $v); 
            // echo($v);
            set_config(strtoupper($k), $v, $cf);
			
        } 
    } 
    file_put_contents($config, $cf);
    if (file_exists(dirname(__FILE__) . '/..' . SITE_PATH . 'cache/url_config')) unlink(dirname(__FILE__) . '/..' . SITE_PATH . 'cache/url_config');
    if (file_exists(dirname(__FILE__) . '/..' . SITE_PATH . 'cache/categories')) unlink(dirname(__FILE__) . '/..' . SITE_PATH . 'cache/categories');
    die('{"code":"0","msg":"配置成功"}');
} 
// 选择模版
function show() {
    global $dbm, $page;
    $basedir = '../templates/';
    $f = array();
    $handle = opendir($basedir);
    while ($file = readdir($handle)) {
        if ($file == "." or $file == ".." or $file == 'admin' or $file == 'lib') {
        } else {
            if (is_dir($basedir . '/' . $file)) {
                $f[] = $file;
            } 
        } 
    } 
    $selec = '<select id="template">';
    foreach($f as $v) {
        $ed = '';
        if ($v == TEMPLATE) $ed = " selected";
        $selec .= '<option value="' . $v . '" ' . $ed . '>' . $v . '</option>';
    } 
    $selec .= "</select>";
    echo $selec;
} 


// 选择wap模版
function show_wap() {
    global $dbm, $page;
    $basedir = '../templates/';
    $f = array();
    $handle = opendir($basedir);
    while ($file = readdir($handle)) {
        if ($file == "." or $file == ".." or $file == 'admin' or $file == 'lib') {
        } else {
            if (is_dir($basedir . '/' . $file)) {
                $f[] = $file;
            } 
        } 
    } 
    $selec = '<select id="wap_tpl">';
    foreach($f as $v) {
        $ed = '';
		$wap_tpl = defined('WAP_TPL') ?  WAP_TPL : 'm';
        if ($v == $wap_tpl) $ed = " selected";
        $selec .= '<option value="' . $v . '" ' . $ed . '>' . $v . '</option>';
    } 
    $selec .= "</select>";
    echo $selec;
} 
?>