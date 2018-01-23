<?php
/**
 * AppCMS Copyright (c) 2012-2013  
 * AppCMS is a free open source mobile phone APP application download website content management system.
 * Custom development, production BUG report template, please contact loyjers@qq.com
 * Author: crane
 * Editor: crane,chenpeng,loyjers

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
$page['data_fill_time'] = $time_end - $time_start;
$page['on'] = 6; //设置高亮的显示条

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
 * 获取列表
 */
function m__list() {
    global $page, $dbm;
    $p = isset($page['get']['p']) ? $page['get']['p'] : 1; 
    // 判断是否需要查找
    $params['where'] = " 1=1"; 
    // 页面加载时传过来的默认选择类型
    if (isset($page['get']['type'])) {
        $where = " and area_type in (" . $page['get']['type'] . ")";
        $params['where'] .= $where;
    } 

    if (isset($page['get']['search_type']) && isset($page['get']['search_txt']) && !empty($page['get']['search_txt']) && !empty($page['get']['search_type'])) {
        if ($page['get']['search_txt'] == 'area_type') {
            $where = " and " . $page['get']['search_type'] . " = '" . $page['get']['search_txt'] . "'" ;
            $params['where'] .= $where;
        } else {
            $where = " and " . $page['get']['search_type'] . " like '%" . $page['get']['search_txt'] . "%'" ;
            $params['where'] .= $where;
        } 
    } 
    // 这个是jquery传过来编辑友链时获取数据的
    if (isset($page['post']['area_id']) && $page['post']['area_id'] > 0) {
        $page['post']['area_id'] = stripslashes(helper :: escape($page['post']['area_id']));
        $params['where'] .= ' and area_id="' . $page['post']['area_id'] . '"';
    } 
    $params['table_name'] = TB_PREFIX . "recommend_area";
    $params['count'] = 1 ;
    $params['suffix'] = " order by area_order DESC,area_id DESC ";
    $params['pagesize'] = PAGESIZE;
    $params['suffix'] .= $dbm -> get_limit_sql(PAGESIZE, $p);
    $recommends = $dbm -> single_query($params);
    $page['recommends'] = $recommends;
} 

/**
 * 编辑或者添加推荐位或者广告
 */
function m__edit() {
    global $page, $dbm;
    foreach($page['post'] as $key => $val) {
        $page['post'][$key] = helper :: escape(urldecode($val));
    } 
    if (!is_numeric($page['post']['area_id'])) die('{"code":"210","msg":"推荐位ID必须是数字"}');
    if (empty($page['post']['title'])) die('{"code":"100","msg":"标题不能为空"}');
    if ($page['post']['area_type'] > 2) {
        if (!isset($page['post']['area_type'])) die('{"code":"200","msg":"推荐类型不能为空"}');
        if (!is_numeric($page['post']['area_order'])) die('{"code":"200","msg":"排序必须是数字"}');
    } 
    if (!isset($page['post']['area_id']) || $page['post']['area_id'] <= 0) {
        // 添加推荐位或者广告
        unset($page['post']['area_id']);
        $res = $dbm -> single_insert(TB_PREFIX . "recommend_area", $page['post']);
        if (empty($res['error']) && $res['autoid'] > 0) die('{"code":"0","msg":"添加位置成功"}');
        die('{"code":"200","msg":"添加位置失败，请核对后重新添加"}');
    } else {
        // 修改推荐位或者广告
        $where = " area_id='" . $page['post']['area_id'] . "'";
        $res = $dbm -> single_update(TB_PREFIX . "recommend_area", $page['post'], $where);
        if (empty($res['error'])) die('{"code":"0","msg":"更新位置成功"}');
        die('{"code":"240","msg":"编辑推荐位失败"}');
    } 
} 

/**
 * 删除选中的友链
 */
function m__del() {
    global $page, $dbm; 
    // 直接传过来的删除动作
    if (isset($page['post']['area_id'])) {
        $page['post']['params'] = $page['post']['area_id'];
    } 
    if (empty($page['post']['params'])) die('{"code":"100","msg":"没有选中要删除的友链"}');
    $ids = stripslashes($page['post']['params']);
    $ids = explode(',', $ids);
    $count = 0;
    foreach($ids as $id) {
        $id = intval($id);
        $where = " area_id = '" . $id . "'";
        $res = $dbm -> single_del(TB_PREFIX . "recommend_area", $where);
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
        $params['area_order'] = $order['val'];
        $where = "area_id = '" . $order['id'] . "'";
        $res = $dbm -> single_update(TB_PREFIX . "recommend_area", $params, $where);
    } 
    $msg = "修改排序成功";
    die('{"code":"0","msg":"' . $msg . '"}');
} 

?>