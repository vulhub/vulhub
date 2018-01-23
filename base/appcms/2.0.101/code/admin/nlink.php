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

$time_start = helper :: getmicrotime(); //开始时间

$dbm = new db_mysql(); //数据库类实例

$page['get'] = $_GET; //get参数的 m 和 ajax 参数是默认占用的，一个用来执行动作函数，一个用来判断是否启用模板还是直接输出JSON格式数据
$page['post'] = $_POST;

check_admin(); //判断用户是否登录

/**
 * 页面动作 model 分支选择  
 *    动作函数写在文件末尾，全部以前缀 m__ 开头
 */
$page['get']['m'] = isset($_GET['m'])?$_GET['m']:'list';

if (function_exists("m__" . $page['get']['m'])) {
    call_user_func("m__" . $page['get']['m']);
} 

$time_end = helper :: getmicrotime(); //主程序执行时间，如果要包含模板的输出时间，则可以调用该静态时间方法单独计算
$page['data_fill_time'] = $time_end - $time_start; //执行时间
$page['on'] = 8; //设置高亮的显示条

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
 * 页面动作函数和其他函数
 */

/**
 * 获取内链词列表
 */
function m__list() {
    global $page, $dbm; 
    // 判断是否需要查找
    $params['where'] = " 1=1";
    if (isset($page['get']['search_type']) && isset($page['get']['search_txt']) && !empty($page['get']['search_txt']) && !empty($page['get']['search_type'])) {
        $where = " and " . $page['get']['search_type'] . " like '%" . $page['get']['search_txt'] . "%'" ;
        $params['where'] .= $where;
    } 

    $p = isset($page['get']['p']) ? $page['get']['p'] : 1; 
    // 这个是jquery传过来编辑友链时获取数据的
    if (isset($page['post']['nlink_id']) && $page['post']['nlink_id'] > 0) {
        $page['post']['nlink_id'] = helper :: escape($page['post']['nlink_id']);
        $params['where'] .= ' and nlink_id="' . $page['post']['nlink_id'] . '"';
    } 
    $params['table_name'] = TB_PREFIX . "nlink";
    $params['count'] = 1 ;
    $params['suffix'] = " order by nlink_id DESC ";
    $params['pagesize'] = PAGESIZE;
    $params['suffix'] .= $dbm -> get_limit_sql(PAGESIZE, $p);
    $nlinks = $dbm -> single_query($params);
    $page['nlinks'] = $nlinks;
} 
/**
 * 编辑和添加
 */
function m__edit() {
    global $page, $dbm;
    $reg = "~^((https|http|ftp|rtsp|mms)?://)~";
    if (!is_numeric($page['post']['nlink_id'])) die('{"code":"210","msg":"内链词ID必须是数字"}');
    if (empty($page['post']['nlink_txt'])) die('{"code":"220","msg":"内链名称不能为空"}');
    if (empty($page['post']['nlink_url'])) die('{"code":"230","msg":"内链地址不能为空"}');
    if (!preg_match($reg, $page['post']['nlink_url'])) die('{"code":"230","msg":"链接地址不合法"}');
    $params['nlink_txt'] = helper :: escape($page['post']['nlink_txt']);
    $params['nlink_url'] = helper :: escape($page['post']['nlink_url']);
    if ($page['post']['nlink_id'] > 0) {
        // 编辑
        $where = " nlink_id='" . $page['post']['nlink_id'] . "'";
        $res = $dbm -> single_update(TB_PREFIX . "nlink", $params , $where);
        if (empty($res['error'])) die('{"code":"0","msg":"更新内链词成功"}');
        die('{"code":"240","msg":"更新内链词失败"}');
    } else {
        // 添加友链
        $res = $dbm -> single_insert(TB_PREFIX . "nlink", $params);
        if (empty($res['error']) && $res['autoid'] > 0) die('{"code":"0","msg":"添加内链词成功"}');
        die('{"code":"250","msg":"添加内链词失败，请核对重试"}');
    } 
} 
/**
 * 删除选中的数据
 */
function m__del() {
    global $page, $dbm; 
    // 直接传过来的删除动作
    if (isset($page['post']['nlink_id'])) {
        $page['post']['params'] = $page['post']['nlink_id'];
    } 
    if (empty($page['post']['params'])) die('{"code":"100","msg":"没有选中要删除的友链"}');
    $ids = stripslashes($page['post']['params']);
    $ids = explode(',', $ids);
    $count = 0;
    foreach($ids as $id) {
        $id = intval($id);
        $where = " nlink_id = '" . $id . "'";
        $res = $dbm -> single_del(TB_PREFIX . "nlink", $where);
        if (!empty($res['error'])) continue;
        $count++;
    } 
    // $msg = "共删除了".$count."个友情链接";
    die('{"code":"0","msg":"删除成功"}');
} 

?>