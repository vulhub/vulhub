<?php
/**
 * AppCMS Copyright (c) 2012-2013  
 * AppCMS is a free open source mobile phone APP application download website content management system.
 * Custom development, production BUG report template, please contact loyjers@qq.com
 * Author: crane
 * Editor: crane,loyjers

 */

/**
 * 加载核心文件类和公用方法函数
 */
require_once(dirname(__FILE__) . "/../core/init.php");

$time_start = helper :: getmicrotime();

$dbm = new db_mysql(); //数据库类实例

$page['get'] = $_GET; //get参数的 m 和 ajax 参数是默认占用的，一个用来执行动作函数，一个用来判断是否启用模板还是直接输出JSON格式数据
$page['post'] = $_POST;

check_admin(); //判断账号是否登录

/**
 * 页面动作 model 分支选择
 *     动作函数写在文件末尾，全部以前缀 m__ 开头
 */
$page['get']['m'] = isset($_GET['m'])?$_GET['m']:'list';

if (function_exists("m__" . $page['get']['m'])) {
    call_user_func("m__" . $page['get']['m']);
} 

$time_end = helper :: getmicrotime();
$page['data_fill_time'] = $time_end - $time_start; //执行时间
$page['on'] = 1; //设置高亮的显示条

/**
 * 模板载入选择
 *     模板页面为PHP+HTML混合编写，如果模板页面中也有区块函数，模板函数以 tpl__ 为前缀
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
 * 获取所有账号
 */
function m__list() {
    global $page, $dbm;
    $p = isset($page['get']['p']) ? $page['get']['p'] : 1;
    $user = $page['post']; 
    // 判断id是否是数字
    if (isset($user['uid']) && !is_numeric($user['uid'])) $page['admins'] = array();
    $params['table_name'] = TB_PREFIX . "admin_list";
    $params['count'] = 1 ;
    $params['suffix'] = " order by reg_date DESC ";
    $params['pagesize'] = PAGESIZE;
    $params['suffix'] .= $dbm -> get_limit_sql(PAGESIZE, $p);
    $user = $dbm -> single_query($params);
    $page['admins'] = $user;
} 

/**
 * 编辑或者添加账号资料
 */
function m__edit() {
    global $page, $dbm, $admin;
    $user = $page['post'];
    if (!is_numeric($user['uid'])) die('{"code":"210","msg":"账号ID必须是数字"}');
    if (intval($user['uid']) > 0) {
        $where = " uid='" . $user['uid'] . "'";
        if (isset($user['ustate'])) {
            $user['ustate'] = $user['ustate'] == 0 ? 1 : 0;
            if ($user['uid'] == $_SESSION['uid'])
                die('{"code":"220","msg":"不能停用当前账户"}');
            $params['ustate'] = $user['ustate'];
        } 
        if (isset($user['upass'])) { // 如果设置了密码，就是要改密码
            if (strlen($user['new_upass']) < 5) die('{"code":"230","msg":"密码过于简单"}');
            if (strlen($user['new_upass']) > 20) die('{"code":"240","msg":"密码超出限定长度"}'); 
            // 判断是否是当前账号的登陆状态
            if ($user['uid'] != $_SESSION['uid']) die('{"code":"250","msg":"不能修改其他账号的密码"}'); 
            // 判断原始密码是否正确
            $chu = $dbm -> single_query(array("where" => $where, 'table_name' => TB_PREFIX . "admin_list"));
            if (helper :: password_encrypt($user['upass']) != $chu['list'][0]['upass']) die('{"code":"260","msg":"旧密码错误"}');
            if ($user['new_upass'] != $user['re_upass']) die('{"code":"270","msg":"两次密码不一样"}');
            $params['upass'] = helper :: password_encrypt($user['new_upass']);
        } 
        $res = $dbm -> single_update(TB_PREFIX . "admin_list", $params, $where);
        if (empty($res['error'])) {
            die('{"code":"0","msg":"操作成功"}');
        } 
        die('{"code":"280","msg":"编辑账号失败，请核实后再添加"}');
    } else { // 添加账号
        // 验证注册信息是否合法
        if (empty($user['uname'])) die('{"code":"210","msg":"账号名不能为空"}');
        if (empty($user['upass'])) die('{"code":"220","msg":"密码不能为空"}');
        if (!preg_match('~^[A-Za-z][A-Za-z]*[a-z0-9_]*$~', $user['uname'])) die('{"code":"230","msg":"账号名必须以字母开头，只允许字母、数字、下划线"}');
        if (strlen($user['upass']) < 5) die('{"code":"240","msg":"密码过于简单"}');
        if (strlen($user['upass']) > 20) die('{"code":"250","msg":"密码超出限定长度"}');
        if ($user['upass'] != $user['re_pass']) die('{"code":"270","msg":"两次密码不一致"}');
        $where = " uname='" . $user['uname'] . "'";
        $a = $dbm -> single_query(array('where' => $where, 'table_name' => TB_PREFIX . "admin_list"));
        if (count($a['list']) > 0) die('{"code":"260","msg":"账号名不能重复"}');
        $params['uname'] = $user['uname'];
        $params['upass'] = helper :: password_encrypt($user['upass']);
        $params['reg_date'] = time();
        $params['ustate'] = 0;
        $res = $dbm -> single_insert(TB_PREFIX . "admin_list", $params);
        if ($res['autoid'] > 0) {
            die('{"code":"0","msg":"添加账号成功"}');
        } 
        die('{"code":"270","msg":"添加账号失败，请核实后再添加"}');
    } 
} 

/**
 * 更改账号状态
 */
function m__set_ustate() {
    global $dbm, $page;
    if (!is_numeric($page['get']['ustate'])) die('{"code":"210","msg":"状态必须是数字"}');
    $table_name = TB_PREFIX . "admin_list";
    $params['ustate'] = $page['get']['ustate'];
    $action = $page['get']['ustate'] == 1 ? "停用" : "启用" ;
    $uids = explode(',', stripslashes($page['post']['params']));
    $count = 0;
    $html = " ID 分别是：";
    foreach ($uids as $kuid => $vuid) {
        if (!is_numeric($vuid)) die('{"code":"220","msg":"账号ID必须是数字"}');
        if ($vuid == $_SESSION['uid']) {
            die('{"code":"230","msg":"不能停用当前账户,请核对后再操作"}');
            break;
        } 
        $where = " uid = '" . $vuid . "'";
        $res = $dbm -> single_update($table_name, $params, $where);
        if (!empty($res['error'])) continue;
        $count++;
        $html .= $vuid . ",";
    } 
    // $msg = "成功".$action."了 ".$count." 个账号,".$html;
    $msg = '设置成功';
    die('{"code":"0","msg":"' . $msg . '"}');
} 

?>