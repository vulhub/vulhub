<?php
/**
 * AppCMS Copyright (c) 2012-2013  
 * AppCMS is a free open source mobile phone APP application download website content management system.
 * Custom development, production BUG report template, please contact loyjers@qq.com
 * Author: crane
 * Editor: crane,loyjers
 */

require_once(dirname(__FILE__) . "/../core/init.php");

/**
 * 加载核心文件类和公用方法函数
 */

$time_start = helper :: getmicrotime();
/**
 * 开始时间
 */

$dbm = new db_mysql();
/**
 * 数据库类实例
 */

$page['get'] = $_GET;
/**
 * get参数的 m 和 ajax 参数是默认占用的，一个用来执行动作函数，一个用来判断是否启用模板还是直接输出JSON格式数据
 */
$page['post'] = $_POST;

check_admin();
/**
 * 判断用户是否登录
 */

$page['get']['m'] = isset($_GET['m'])?$_GET['m']:'list';
/**
 * 页面动作 model 分支选择，动作函数写在文件末尾，全部以前缀 m__ 开头
 */

if (function_exists("m__" . $page['get']['m'])) {
    call_user_func("m__" . $page['get']['m']);
} 

$time_end = helper :: getmicrotime();
/**
 * 主程序执行时间，如果要包含模板的输出时间，则可以调用该静态时间方法单独计算
 */

/**
 * 模板载入选择，模板页面为PHP+HTML混合编写，如果模板页面中也有区块函数，模板函数以 tpl__ 为前缀
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

function m__list() {
    global $page;
    $dir = dirname(__FILE__) . '/../templates/' . TEMPLATE;
    $page['files'] = get_file_list($dir);
} 
/**
 * 获取要编辑的内容
 */
function m__show() {
    global $page;
    $dir = dirname(__FILE__) . '/../templates/' . TEMPLATE;
    if (is_file($dir . "/" . $page['get']['filename'])) {
        $filecont = helper :: get_contents($dir . "/" . $page['get']['filename']);
    } else {
        $filecont = '';
    } 
    $page['content'] = $filecont;
} 
/**
 * 保存修改
 */
function m__save_edit() {
    global $page;
    $dir = dirname(__FILE__) . '/../templates/' . TEMPLATE;
    if (!empty($page['post']['content'])) {
        $file = file_put_contents($dir . "/" . $page['post']['filename'], helper :: escape_stripslashes($_POST['content']));
        if ($file > 0) {
            echo '<script>window.location.href="template.php";</script>';
            exit;
        } 
        echo '<script>window.location.href="template.php?m=show&filename=' . $page['post']['filename'] . '";</script>';
        exit;
    } 
} 
/**
 * 添加模板是创建文件
 */
function m__create_file() {
    global $page;
    $filename = helper :: escape($page['post']['filename']);
    if (empty($filename)) {
        die('{"code":"100","msg":"文件名不能为空"}');
    } 
    // if (!preg_match('~^$~', $filename)) die('{"code":"100","msg":"文件名只允许是字母"}');
    $dir = dirname(__FILE__) . '/../templates/' . TEMPLATE;
    $files = str_replace('+', '%20', get_file_list($dir)); 
    // 判断文件名是否存在
    if (in_array($filename, $files)) {
        die('{"code":"100","msg":"文件名已经存在"}');
    } 
    // 创建文件
    $handle = fopen($dir . "/" . $filename, "a");
    if (fclose($handle)) {
        die('{"code":"0","msg":"创建文件成功","filename":"' . $filename . '"}');
    } 
} 

?>