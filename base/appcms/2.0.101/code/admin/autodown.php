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

$page['get'] = $_GET; //get参数的 m 和 ajax 参数是默认占用的，一个用来执行动作函数，一个用来判断是否启用模板还是直接输出JSON格式数据
$page['post'] = $_POST;
$page['cates'] = $c -> categories;
$page['tree'] = $c -> cate_tree(1);
check_admin(); //判断用户是否登录

    //页面载入删除标记
if(file_exists("../cache/flag_all_down")){
    unlink('../cache/flag_all_down');
}
/**
 * 页面动作 model 分支选择  
 *         动作函数写在文件末尾，全部以前缀 m__ 开头
 */
$page['get']['m'] = isset($_GET['m'])?$_GET['m']:'list';

if (function_exists("m__" . $page['get']['m'])) {
    call_user_func("m__" . $page['get']['m']);
} 

$time_end = helper :: getmicrotime(); //主程序执行时间，如果要包含模板的输出时间，则可以调用该静态时间方法单独计算
$page['data_fill_time'] = $time_end - $time_start;

/**
 * 模板载入选择
 *         模板页面为PHP+HTML混合编写，如果模板页面中也有区块函数，模板函数以 tpl__ 为前缀
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
 * 数据入库
 */

/**
 * 页面动作函数和其他函数
 */
 /*
  *初始化页面
  *
  **/
function m__list(){
    global $page;
    $page['message'] = "使用本功能的前提是必须<span style='color:green;'>智能绑定分类</span>。如果你之前在<span style='color:green;'>添加应用</span>有过绑定分类，那么这里就可以直接使用了。如果没有，请去绑定分类。具体见<span style='color:green;'>添加应用</span>的<span style='color:green;'>智能添加应用</span>";
}

function m__auto_start(){
    global $page;
    set_time_limit(0);
    session_write_close();
    $host = $_SERVER['HTTP_HOST'];
    $errno = '';
    $errstr = '';
    $fp = @fsockopen("$host", 80, $errno, $errstr, 30);
    if (!$fp) {
        die('{"code":"100","msg":"'.$errno.':'.$errstr.'"}');
    } else {
        $php_self=$_SERVER['PHP_SELF'];
        $path=substr($php_self,0,strrpos($php_self,'/')+1);
        
        $out = "GET ".$path."autodown_frame.php?m=auto_start&auth=".md5(AUTH_CODE)." HTTP/1.1\r\n";
        $out .= "Host: $host\r\n";
        $out .= "Connection: Close\r\n\r\n";
        
        fwrite($fp, $out);
        while (!feof($fp)) {
            $res = @fgets($fp,1024);
        }
        fclose($fp);
        die($res);
    }
    die('{"code":"100","msg":"定时更新停止"}');
}
 
?>