<?php
/**
 * AppCMS Copyright (c) 2012-2013  
 * AppCMS is a free open source mobile phone APP application download website content management system.
 * Custom development, production BUG report template, please contact loyjers@qq.com
 * Author: crane
 * Editor: crane
 */

/**
 * 加载核心文件类和公用方法函数
 */
require_once(dirname(__FILE__) . "/../core/init.php");

$time_start = helper :: getmicrotime();

$page['get'] = $_GET; // 取得GET和POST变量并进行验证，参数分支选择
$page['post'] = $_POST;

$dbm = new db_mysql(); //实例化数据库类

// 判断用户是否登录
check_admin();

/**
 * 页面动作 model 分支选择
 *    动作函数写在文件末尾，全部以前缀 m__ 开头
 */
$page['get']['m'] = isset($_GET['m'])?$_GET['m']:'list';
if (function_exists("m__" . $page['get']['m'])) {
    call_user_func("m__" . $page['get']['m']);
} 

$time_end = helper :: getmicrotime();

$page['data_fill_time'] = $time_end - $time_start;

/**
 * 模板载入选择
 *    模板页面为PHP+HTML混合编写，如果模板页面中也有区块函数，模板函数以 tpl__ 为前缀
 */
if (!isset($page['get']['ajax']) || $page['get']['ajax'] == null || $page['get']['ajax'] == '') {
    $tpl_filename=str_replace('\\', '', str_replace(dirname(__FILE__), '', __FILE__));
    $tpl_filename=str_replace('/','',$tpl_filename);
    require(dirname(__FILE__) . '/templates/tpl_' . $tpl_filename);
} else {
    if ($page['get']['ajax'] == 'json') {
        echo json_encode($page);
    } 
} 

/**
 * 获取友链列表
 */
function m__list() {
    global $page, $dbm;
    $p = isset($page['get']['p']) ? $page['get']['p'] : 1; 
    // 这个是jquery传过来编辑友链时获取数据的
    $params['where'] = " 1=1";
    if (isset($page['get']['search_type']) && isset($page['get']['search_txt']) && !empty($page['get']['search_txt']) && !empty($page['get']['search_type'])) {
        $where = " and " . $page['get']['search_type'] . " like '%" . $page['get']['search_txt'] . "%'" ;
        $params['where'] .= $where;
    } 

    if (isset($page['post']['flink_id']) && $page['post']['flink_id'] > 0) {
        $page['post']['flink_id'] = helper :: escape($page['post']['flink_id']);
        $params['where'] = ' 1=1 and flink_id="' . $page['post']['flink_id'] . '"';
    } 
    $params['table_name'] = TB_PREFIX . "flink";
    $params['count'] = 1 ;
    $params['suffix'] = " order by flink_order DESC,flink_id DESC ";
    $params['pagesize'] = PAGESIZE;
    $params['suffix'] .= $dbm -> get_limit_sql(PAGESIZE, $p);
    $flinks = $dbm -> single_query($params);
    if (!empty($flinks['list'])) {
        $page['flinks'] = $flinks;
    }
} 
/**
 * 编辑/添加友情链接
 */
function m__edit() {
    global $page, $dbm;
    $reg = "~^((https|http|ftp|rtsp|mms)?://)~";
    if (!is_numeric($page['post']['flink_id'])) die('{"code":"210","msg":"链接ID必须是数字"}');
    if (!is_numeric($page['post']['flink_order'])) die('{"code":"210","msg":"排序必须是数字"}');
    if (empty($page['post']['flink_name'])) die('{"code":"220","msg":"链接名称不能为空"}');
    if (empty($page['post']['flink_url'])) die('{"code":"230","msg":"链接地址不能为空"}');
    if (!preg_match($reg, $page['post']['flink_url'])) die('{"code":"230","msg":"链接地址不合法"}');
    $params['flink_name'] = helper :: escape($page['post']['flink_name']);
    $params['flink_url'] = helper :: escape($page['post']['flink_url']);
    $params['flink_img'] = helper :: escape($page['post']['flink_img']);
    $params['flink_is_site'] = helper :: escape($page['post']['flink_is_site']);
    $params['flink_order'] = helper :: escape($page['post']['flink_order']);
    if ($page['post']['flink_id'] > 0) {
        // 编辑
        $where = " flink_id='" . $page['post']['flink_id'] . "'";
        $res = $dbm -> single_update(TB_PREFIX . "flink", $params , $where);
        if (empty($res['error'])) die('{"code":"0","msg":"更新友情链接成功"}');
        die('{"code":"240","msg":"编辑友情链接失败"}');
    } else {
        // 添加友链
        $res = $dbm -> single_insert(TB_PREFIX . "flink", $params);
        if (empty($res['error']) && $res['autoid'] > 0) die('{"code":"0","msg":"添加友情链接成功"}');
        die('{"code":"250","msg":"添加友情链接失败，请核对重试"}');
    } 
} 
/**
 * 删除选中的友链
 */
function m__del() {
    global $page, $dbm; 
    // 直接传过来的删除动作
    if (isset($page['post']['flink_id'])) {
        $page['post']['params'] = $page['post']['flink_id'];
    } 
    if (empty($page['post']['params'])) die('{"code":"100","msg":"没有选中要删除的友链"}');
    $ids = stripslashes($page['post']['params']);
    $ids = explode(',', $ids);
    $count = 0;
    foreach($ids as $id) {
        $id = intval($id);
        $where = " flink_id = '" . $id . "'";
        $res = $dbm -> single_del(TB_PREFIX . "flink", $where);
        if (!empty($res['error'])) continue;
        $count++;
    } 
    // $msg = "共删除了".$count."个友情链接";
    die('{"code":"0","msg":"删除成功"}');
} 
/**
 * 修改排序
 */
function m__order() {
    global $page, $dbm;
    $orders = json_decode(stripslashes($page['post']['params']), true);
    if (empty($orders)) die('{"code":"200","msg":"没有要修改的排序"}');
    foreach ($orders as $order) {
        $params['flink_order'] = $order['val'];
        $where = "flink_id = '" . $order['id'] . "'";
        $res = $dbm -> single_update(TB_PREFIX . "flink", $params, $where);
    } 
    $msg = "修改排序成功";
    die('{"code":"0","msg":"' . $msg . '"}');
} 
