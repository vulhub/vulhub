<?php
/**
 * AppCMS Copyright (c) 2012-2013  
 * AppCMS is a free open source mobile phone APP application download website content management system.
 * Custom development, production BUG report template, please contact loyjers@qq.com
 * Author: chenpeng
 * Editor: chenpeng
 */

/**
 * * 加载核心文件类和公用方法函数*
 */
require_once(dirname(__FILE__) . "/../core/init.php");

$time_start = helper :: getmicrotime(); //开始时间

$dbm = new db_mysql(); //数据库类实例
$c = new common($dbm);

check_admin() ; //判断用户是否登录

set_time_limit(0);
/**
 * 页面动作 model 分支选择  
 *           动作函数写在文件末尾，全部以前缀 m__ 开头
 */
$_GET['m'] = isset($_GET['m'])?$_GET['m']:'list';

if (function_exists("m__" . $_GET['m'])) {
    call_user_func("m__" . $_GET['m']);
} 

$time_end = helper :: getmicrotime(); //主程序执行时间，如果要包含模板的输出时间，则可以调用该静态时间方法单独计算
$page['data_fill_time'] = $time_end - $time_start;

/**
 * 页面动作函数和其他函数
 */

function m__list() {
    global $c, $dbm, $page;
    //header("Content-type: multipart/x-mixed-replace;boundary=endofsection");
    $start = isset($_GET['s'])?$_GET['s']:1;
    $end = isset($_GET['end'])?$_GET['end']:10;
    
    echo('<html><head>');
    echo('<style>body{font-size:12px;line-height:200%;margin:0;padding:0;font-family:Arial;}</style>');
    echo('</head><body>');
    
    if(!is_numeric($start)) die('<font color=red>开始ID必须是数字</font>');
    if(!is_numeric($end)) die('<font color=red>结束ID必须是数字</font>');
    if(intval($start)>intval($end)) die('<font color=red>开始ID必须小于结束ID</font>');

    $params['type'] = 0; //0表示取应用列表
    $params['resource'] = 1;
    $params['count'] = 0 ; //计算总数
    $params['pagesize'] = 2;
    $now = isset($_GET['now'])?$_GET['now']:$start;
    if ($now > $end) die('<font color=green><b>全部下载完毕！</b></font>');
    $params['where'] = " app_id >=" . $now . " ";
    $params['order'] = " order by app_id asc";
    $apps = $c -> get_list($params,1);

    if (count($apps['list']) == 0) die('<font color=red>没有需要下载图片的应用</font>');
    if (count($apps['list']) > 0) {
        helper::print_state('正在下载应用《<a href="'.$apps['list'][0]['surl'].'" target="_blank">'.$apps['list'][0]['app_title'].'</a>》的图片，应用ID：'.$apps['list'][0]['app_id'].'');
        // 下载缩略图
        if (isset($apps['list'][0]['app_logo'])) {
            if (preg_match('~^http://~', $apps['list'][0]['app_logo'])) {
                $app_logo = down_url($apps['list'][0]['app_logo']);
                $fields['app_logo'] = $app_logo;
                 $where = " app_id=" . $apps['list'][0]['app_id'] . "" ;
                $res = $dbm -> single_update(TB_PREFIX . 'app_list', $fields, $where);
            } else {
                helper::print_state('<font color="orange">缩略图：本地图片无需下载</font> ' . $apps['list'][0]['app_logo'] . '');
            } 
        } 
        // 下载资源图
        if (isset($apps['list'][0]['resource'])) {
            foreach($apps['list'][0]['resource'] as $d) {
                if (preg_match('~^http://~', $d['resource_url'])) {
                    $newurl = down_url($d['resource_url']);
                    $where = " id=" . $d['id'] . " ";
                    $field['resource_url'] = $newurl;
                    $ras = $dbm -> single_update(TB_PREFIX . 'resource_list', $field, $where);
                } else {
                    helper::print_state('<font color="orange">应用截图：本地图片无需下载</font> ' . $d['resource_url'] . '');
                } 
            } 
        } 
        // 没有要继续下载图片的应用了
        if (count($apps['list']) == 1) {
            die('<font color=green><b>全部下载完毕！</b></font>');
        } else { // 跳转到下一个应用
            $location = "download_frame.php?m=list&s=" . $start . "&end=" . $end . "&now=" . $apps['list'][1]['app_id'] . " ";
            echo '跳转下一个应用：《'.$apps['list'][1]['app_title'].'》<script type="text/javascript" language="javascript"> setTimeout(function(){window.location.href="' . $location . '";},2000);</script>';
        } 
    } 
    echo('</body></html>');
} 
// 路径转换
function down_url($file_url) {
    $ary = parse_url($file_url);
    helper::print_state('开始下载远程图片：<a href="'.$file_url.'" target="_blank">' . $file_url . '</a>');
    $filename = basename($ary['path']);
    $file = helper :: get_contents($file_url);

    if (empty($file)) {
        helper::print_state('<font color=red>图片下载失败</font>');
        return $file_url;
    } 
    $save_path = SITE_PATH.'upload' . UPLOAD_PATH . substr($filename,0,3) . '/';
    $all_path = dirname(__FILE__) . '/../' . 'upload' . UPLOAD_PATH . '' . substr($filename,0,3) . '/';
    if (!is_dir($all_path)) {
        helper :: mkdirs($all_path);
    } 
    // 检测文件是否存在
    if (file_exists($save_path . $filename)) {
        unlink($save_path . $filename);
    } 

    $fopen = fopen($all_path . $filename, 'w');
    fwrite($fopen, $file);
    fclose($fopen);
    $url = $save_path . $filename;
    helper::print_state('图片下载完毕：<a href="'.$url.'" target="_blank">' . $url . '</a>');
    
    return $url;
} 

?>