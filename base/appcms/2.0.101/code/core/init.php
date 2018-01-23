<?php
/**
 * AppCMS Copyright (c) 2012-2013  
 * AppCMS is a free open source mobile phone APP application download website content management system.
 * Custom development, production BUG report template, please contact loyjers@qq.com
 * Author: loyjers
 * Editor: loyjers
 */

if (!session_id()) session_start(); //开启SESSION
date_default_timezone_set('Asia/Shanghai'); //默认时区
ini_set('default_charset', 'utf-8'); //默认编码
ini_set('magic_quotes_runtime', '0'); //魔法反斜杠转义关闭

/**
 * 开发测试环境使用
 */
error_reporting(-1); //报告所有错误，0为忽略所有错误
ini_set('display_errors', '1'); //开启错误提示

// 开启GZIP压缩，如果报错则注释此处
ob_start('ob_gzip');

// 兼容某些IIS+PHP环境无法读取 DOCUMENT_ROOT 变量
if (empty($_SERVER['DOCUMENT_ROOT']) && !empty($_SERVER['SCRIPT_FILENAME'])) {
    $_SERVER['DOCUMENT_ROOT'] = str_replace('\\', '/', substr($_SERVER['SCRIPT_FILENAME'], 0, 0 - strlen($_SERVER['PHP_SELF'])));
}
if (empty($_SERVER['DOCUMENT_ROOT']) && !empty($_SERVER['PATH_TRANSLATED'])) {
    $_SERVER['DOCUMENT_ROOT'] = str_replace('\\', '/', substr(str_replace('\\\\', '\\', $_SERVER['PATH_TRANSLATED']), 0, 0 - strlen($_SERVER['PHP_SELF'])));
}
if ( !isset($_SERVER['QUERY_STRING']) || empty($_SERVER['QUERY_STRING'])) $_SERVER['QUERY_STRING']='';
if (!isset($_SERVER['REQUEST_URI']) || empty($_SERVER['REQUEST_URI'])) $_SERVER['REQUEST_URI']='';

require_once(dirname(__FILE__) . "/config.conn.php"); //数据库连接
require_once(dirname(__FILE__) . "/config.php"); //基本配置
require_once(dirname(__FILE__) . "/config.struct.php"); //结构
require_once(dirname(__FILE__) . "/database.class.php"); //数据库操作类
require_once(dirname(__FILE__) . "/helper.class.php"); //通用方法类
require_once(dirname(__FILE__) . "/urlrewrite.class.php"); //URL重写
require_once(dirname(__FILE__) . "/vars.class.php"); //词组变量
require_once(dirname(__FILE__) . "/function.php"); //前后台公用方法
require_once(dirname(__FILE__) . "/common.class.php"); //前后台公用类


function ob_gzip($content) {
    if (!headers_sent() && // 如果页面头部信息还没有输出
            extension_loaded("zlib") && // 而且zlib扩展已经加载到PHP中
            strstr($_SERVER["HTTP_ACCEPT_ENCODING"], "gzip") // 而且浏览器说它可以接受GZIP的页面
            ) {
        $content = gzencode($content, 9); //用zlib提供的gzencode()函数执行级别为9的压缩，这个参数值范围是0-9，0表示无压缩，9表示最大压缩，当然压缩程度越高越费CPU。 
        // 然后用header()函数给浏览器发送一些头部信息，告诉浏览器这个页面已经用GZIP压缩过了！
        header("Content-Encoding: gzip");
        header("Vary: Accept-Encoding");
        header("Content-Length: " . strlen($content));
    } 
    return $content; //返回压缩的内容，或者说把压缩好的饼干送回工作台。
} 

?>