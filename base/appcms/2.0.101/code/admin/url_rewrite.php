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

$time_start = helper :: getmicrotime(); //开始时间

$dbm = new db_mysql(); //数据库类实例
$c = new common($dbm); //实例化common类

$page['get'] = $_GET; //get参数的 m 和 ajax 参数是默认占用的，一个用来执行动作函数，一个用来判断是否启用模板还是直接输出JSON格式数据
$page['post'] = $_POST;

check_admin(); //判断用户是否登录

/**
 * 页面动作 model 分支选择  
 *      动作函数写在文件末尾，全部以前缀 m__ 开头
 */
$page['get']['m'] = isset($_GET['m'])?$_GET['m']:'list';

if (function_exists("m__" . $page['get']['m'])) {
    call_user_func("m__" . $page['get']['m']);
} 

$time_end = helper :: getmicrotime(); //主程序执行时间，如果要包含模板的输出时间，则可以调用该静态时间方法单独计算
$page['data_fill_time'] = $time_end - $time_start; //执行时间

/**
 * 模板载入选择
 *      模板页面为PHP+HTML混合编写，如果模板页面中也有区块函数，模板函数以 tpl__ 为前缀
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

/**
 * 页面动作函数和其他函数
 */

/**
 * 获取伪静态的地址
 */
function m__list() {
    global $page, $dbm; 
    // 判断是否需要查找
    $params['where'] = ' 1=1 ';
    $p = isset($page['get']['p']) ? $page['get']['p'] : 1; 
    // 这个是jquery传过来编辑友链时获取数据的
    if (isset($page['post']['url_id']) && $page['post']['url_id'] > 0) {
        $page['post']['url_id'] = helper :: escape($page['post']['url_id']);
        $params['where'] .= ' and url_id="' . $page['post']['url_id'] . '"';
    } 
    $params['table_name'] = TB_PREFIX . "url_rewrite";
    $params['count'] = 0 ;
    $params['suffix'] = " order by url_id asc ";
    $params['pagesize'] = PAGESIZE;
    $url_rewrites = $dbm -> single_query($params);
    if (!empty($url_rewrites['list'])) {
        $page['url_rewrites'] = $url_rewrites;
    } else {
        $page['url_rewrites'] = array();
    } 
} 
/**
 * 编辑和添加
 */
function m__edit() {
    global $page, $dbm, $c;
    foreach($page['post'] as $key => $val) {
        $page['post'][$key] = helper :: escape(urldecode($val));
    } 
    if (!is_numeric($page['post']['url_id'])) die('{"code":"210","msg":"规则节点ID必须是数字"}');
    if (empty($page['post']['url_title'])) die('{"code":"220","msg":"规则节点名称不能为空"}');
    if (!preg_match('~^[A-Za-z][A-Za-z]*[a-z0-9_]*$~', $page['post']['url_title'])) die('{"code":"230","msg":"规则节点名称必须以字母开头，只允许字母、下划线"}');
    if (empty($page['post']['url_true'])) die('{"code":"230","msg":"动态地址规则不能为空"}');
    if (empty($page['post']['url_rule'])) die('{"code":"230","msg":"伪静态地址规则不能为空"}');

    if ($page['post']['url_id'] > 0) {
        // 编辑
        $where = " url_id='" . $page['post']['url_id'] . "'";
        unset($page['post']['url_id']);
        $res = $dbm -> single_update(TB_PREFIX . "url_rewrite", $page['post'] , $where);
        /**
         * 更新缓存
         */
        $c -> update_url_config();
        $c -> update_cate();
        /**
         * 生成伪静态文件（.htaccess）
         */
        htaccess();
        iis_httpd_ini();
        web_config();
        nginx();
        if (empty($res['error'])) die('{"code":"0","msg":"更新规则成功"}');
        die('{"code":"240","msg":"更新规则失败"}');
    } else {
        // 添加
        unset($page['post']['url_id']);
        $res = $dbm -> single_insert(TB_PREFIX . "url_rewrite", $page['post']);
        /**
         * 更新缓存
         */
        $c -> update_url_config();
        $c -> update_cate();
        /**
         * 生成伪静态文件（.htaccess）
         */
        htaccess();
        iis_httpd_ini();
        web_config();
        nginx();
        if (empty($res['error']) && $res['autoid'] > 0) die('{"code":"0","msg":"添加规则成功"}');
        die('{"code":"250","msg":"添加规则失败，请核对重试"}');
    } 
} 
/**
 * 删除选中的数据
 */
function m__del() {
    global $page, $dbm; 
    // 直接传过来的删除动作
    if (isset($page['post']['url_id'])) {
        $page['post']['params'] = $page['post']['url_id'];
    } 
    if (empty($page['post']['params'])) die('{"code":"100","msg":"没有选中要删除的友链"}');
    $ids = stripslashes($page['post']['params']);
    $ids = explode(',', $ids);
    $count = 0;
    foreach($ids as $id) {
        $id = intval($id);
        $where = " url_id = '" . $id . "'";
        $res = $dbm -> single_del(TB_PREFIX . "url_rewrite", $where);
        if (!empty($res['error'])) continue;
        $count++;
    } 
    // $msg = "共删除了".$count."个友情链接";
    die('{"code":"0","msg":"删除成功"}');
} 

/**
 * 生成伪静态的地址的配置文件
 */
function htaccess() {
    global $dbm, $page;
    m__list();
    $urls = $page['url_rewrites']; 
    // print_r($urls);
    /**
     * 循环匹配
     */
    $htastart = '<IfModule mod_rewrite.c> ' . chr(10) . chr(13) . 'RewriteEngine on' . chr(10) . chr(13);
    $htastart .= 'ErrorDocument 403 /404.html' . chr(10) . chr(13);
    $htastart .= 'ErrorDocument 404 /404.html' . chr(10) . chr(13);
    $htaccess = '';
    $flag = false;
    foreach($urls['list'] as $key => $url) {
        if ($url['url_title'] == 'content_app_history') {
            $url_rule = str_replace(array("{id}", "{hid}", "."), array("(\d+)", "(\d+)", "\."), $url['url_rule']);
            $url_true = str_replace(array("{id}", "{hid}"), array("$1", "$2"), $url['url_true']);
            $flag = true;
        } else if (in_array($url['url_title'], array('list_info', 'list_app'))) {
            $url_rule = str_replace(array("{cid}", "{p}", "."), array("(\d+)", "(\d+)", "\."), $url['url_rule']);
            $url_true = str_replace(array("{cid}", "{p}"), array("$1", "$2"), $url['url_true']);
            $flag = true;
        } else if (in_array($url['url_title'], array('content_app', 'content_info', 'special_content'))) {
            $url_rule = str_replace(array("{id}", "."), array("(\d+)", "\."), $url['url_rule']);
            $url_true = str_replace("{id}", "$1", $url['url_true']);
            $flag = true;
        } else if (in_array($url['url_title'], array('list_info_index', 'list_app_index', 'special_list_index', 'lastupdate_index'))) {
            $url_rule = str_replace(array("{cid}", "."), array("(\d+)", "\."), $url['url_rule']);
            $url_true = str_replace("{cid}", "$1", $url['url_true']);
            $flag = true;
        } else if (in_array($url['url_title'], array('cpy_list_app', 'cpy_list_info'))) {
            $url_rule = str_replace(array("{cpy}", "{p}", "."), array("(\w+?)", "(\d+)", "\."), $url['url_rule']);
            $url_true = str_replace(array("{cpy}", "{p}"), array("$1", "$2"), $url['url_true']);
            $flag = true;
        } else if (in_array($url['url_title'], array('cpy_list_app_index', 'cpy_list_info_index'))) {
            $url_rule = str_replace(array("{cpy}", "."), array("(\w+?)", "\."), $url['url_rule']);
            $url_true = str_replace("{cpy}", "$1", $url['url_true']);
            $flag = true;
        } else if (in_array($url['url_title'], array('special_list', 'lastupdate'))) {
            $url_rule = str_replace(array("{p}", "."), array("(\d+)", "\."), $url['url_rule']);
            $url_true = str_replace("{p}", "$1", $url['url_true']);
            $flag = true;
        } else if($url['url_title'] == 'img') {
			$url_rule = str_replace(array("{url}","{type}"),array("(.*)","(\w+)"), $url['url_rule']);
            $url_true = str_replace(array("{url}","{type}"), array("$1","$2"), $url['url_true']);
            $flag = true;
		} else if($url['url_title'] == 'content_info_list') {
			$url_rule = str_replace(array("{id}","{p}"), "(\d+)", $url['url_rule']);
            $url_true = str_replace(array("{id}","{p}"), array("$1","$2"), $url['url_true']);
            $flag = true;
		}
        if ($flag) {
            $htaccess .= 'RewriteRule ^' . $url_rule . "$ " . $url_true . chr(10) . chr(13);
            $flag = false;
        } 
    } 
    $htaend = "</IfModule>";
    /**
     * 生成文件
     */
    $data = $htastart . $htaccess . $htaend;
    $bastpath = (dirname(__FILE__) . "/../.htaccess");
    if (file_exists($bastpath)) {
        unlink($bastpath);
    } 

    $fopen = fopen($bastpath, 'w+');
    fwrite($fopen, $data);
    fclose($fopen);
} 

function iis_httpd_ini() {
    global $dbm, $page;
    m__list();
    $urls = $page['url_rewrites'];

    $htastart = "[ISAPI_Rewrite] \r\n";
    $htastart .= "CacheClockRate 3600 \r\n";
    $htastart .= "RepeatLimit 32  \r\n";
    $htastart .= "RewriteRule .*(?:global.asa|default\.ida|root\.exe|\.\.).* . [F,I,O] \r\n";
    $htaccess = '';
    $flag = false;

    foreach($urls['list'] as $key => $url) {
        if ($url['url_title'] == 'content_app_history') {
            $url_rule = str_replace(array("{id}", "{hid}", "."), array("(\d+)", "(\d+)", "\."), $url['url_rule']);
            $url_true = str_replace(array("{id}", "{hid}", "?"), array("$2", "$3", "\?"), $url['url_true']);
            $flag = true;
        } else if (in_array($url['url_title'], array('list_info', 'list_app'))) {
            $url_rule = str_replace(array("{cid}", "{p}", "."), array("(\d+)", "(\d+)", "\."), $url['url_rule']);
            $url_true = str_replace(array("{cid}", "{p}", "?"), array("$2", "$3", "\?"), $url['url_true']);
            $flag = true;
        } else if (in_array($url['url_title'], array('content_app', 'content_info', 'special_content'))) {
            $url_rule = str_replace(array("{id}", "."), array("(\d+)", "\."), $url['url_rule']);
            $url_true = str_replace(array("{id}", "?"), array("$2", "\?"), $url['url_true']);
            $flag = true;
        } else if (in_array($url['url_title'], array('list_info_index', 'list_app_index', 'special_list_index', 'lastupdate_index'))) {
            $url_rule = str_replace(array("{cid}", "."), array("(\d+)", "\."), $url['url_rule']);
            $url_true = str_replace(array("{cid}", "?"), array("$2", "\?"), $url['url_true']);
            $flag = true;
        } else if (in_array($url['url_title'], array('cpy_list_app', 'cpy_list_info'))) {
            $url_rule = str_replace(array("{cpy}", "{p}", "."), array("(\w+?)", "(\d+)", "\."), $url['url_rule']);
            $url_true = str_replace(array("{cpy}", "{p}", "?"), array("$2", "$3", "\?"), $url['url_true']);
            $flag = true;
        } else if (in_array($url['url_title'], array('cpy_list_app_index', 'cpy_list_info_index'))) {
            $url_rule = str_replace(array("{cpy}", "."), array("(\w+?)", "\."), $url['url_rule']);
            $url_true = str_replace(array("{cpy}", "?"), array("$2", "\?"), $url['url_true']);
            $flag = true;
        } else if (in_array($url['url_title'], array('special_list', 'lastupdate'))) {
            $url_rule = str_replace(array("{p}", "."), array("(\d+)", "\."), $url['url_rule']);
            $url_true = str_replace(array("{p}", "?"), array("$2", "\?"), $url['url_true']);
            $flag = true;
        }  else if($url['url_title'] == 'img') {
			$url_rule = str_replace(array("{url}","{type}"),array("(.*)","(\w+)"), $url['url_rule']);
            $url_true = str_replace(array("{url}","{type}","?"), array("$2","$3","\?"), $url['url_true']);
            $flag = true;
		} else if($url['url_title'] == 'content_info_list') {
			$url_rule = str_replace(array("{id}","{p}"), "(\d+)", $url['url_rule']);
            $url_true = str_replace(array("{id}","{p}","?"), array("$2","$3","\?"), $url['url_true']);
            $flag = true;
		}
        if ($flag) {
            $htaccess .= 'RewriteRule ^(.*)' . substr(SITE_PATH,1) . $url_rule . "$ $1" . $url_true . "\r\n";
            $flag = false;
        } 
    } 

    $data = $htastart . $htaccess;
    $bastpath = (dirname(__FILE__) . "/../httpd.ini");
    if (file_exists($bastpath)) {
        unlink($bastpath);
    } 
    $fopen = fopen($bastpath, 'w+');
    fwrite($fopen, $data);
    fclose($fopen);
} 


/**
 * 生成nginx伪静态的地址的配置文件
 */
function nginx() {
    global $dbm, $page;
    m__list();
    $urls = $page['url_rewrites']; 
    // print_r($urls);
    /**
     * 循环匹配
     */
    $htastart = 'location / {'. chr(10) . chr(13);;
    $htaccess = '';
    $flag = false;
    foreach($urls['list'] as $key => $url) {
        if ($url['url_title'] == 'content_app_history') {
            $url_rule = str_replace(array("{id}", "{hid}"), array("(\d+)", "(\d+)"), $url['url_rule']);
            $url_true = str_replace(array("{id}", "{hid}"), array("$1", "$2"), $url['url_true']);
            $flag = true;
        } else if (in_array($url['url_title'], array('list_info', 'list_app'))) {
            $url_rule = str_replace(array("{cid}", "{p}"), array("(\d+)", "(\d+)"), $url['url_rule']);
            $url_true = str_replace(array("{cid}", "{p}"), array("$1", "$2"), $url['url_true']);
            $flag = true;
        } else if (in_array($url['url_title'], array('content_app', 'content_info', 'special_content'))) {
            $url_rule = str_replace(array("{id}"), array("(\d+)"), $url['url_rule']);
            $url_true = str_replace("{id}", "$1", $url['url_true']);
            $flag = true;
        } else if (in_array($url['url_title'], array('list_info_index', 'list_app_index', 'special_list_index', 'lastupdate_index'))) {
            $url_rule = str_replace(array("{cid}"), array("(\d+)"), $url['url_rule']);
            $url_true = str_replace("{cid}", "$1", $url['url_true']);
            $flag = true;
        } else if (in_array($url['url_title'], array('cpy_list_app', 'cpy_list_info'))) {
            $url_rule = str_replace(array("{cpy}", "{p}"), array("(\w+?)", "(\d+)"), $url['url_rule']);
            $url_true = str_replace(array("{cpy}", "{p}"), array("$1", "$2"), $url['url_true']);
            $flag = true;
        } else if (in_array($url['url_title'], array('cpy_list_app_index', 'cpy_list_info_index'))) {
            $url_rule = str_replace(array("{cpy}"), array("(\w+?)"), $url['url_rule']);
            $url_true = str_replace("{cpy}", "$1", $url['url_true']);
            $flag = true;
        } else if (in_array($url['url_title'], array('special_list', 'lastupdate'))) {
            $url_rule = str_replace(array("{p}"), array("(\d+)"), $url['url_rule']);
            $url_true = str_replace("{p}", "$1", $url['url_true']);
            $flag = true;
        } else if($url['url_title'] == 'img') {
			$url_rule = str_replace(array("{url}","{type}"),array("(.*)","(\w+)"), $url['url_rule']);
            $url_true = str_replace(array("{url}","{type}"), array("$1","$2"), $url['url_true']);
            $flag = true;
		}
        if ($flag) {
            $htaccess .= 'rewrite ^' . $url_rule . "$ " . '/' . $url_true . " break;" . chr(10) . chr(13);
            $flag = false;
        } 
    } 
    $htaend = "}";
    /**
     * 生成文件
     */
    $data = $htastart . $htaccess . $htaend;
    $bastpath = (dirname(__FILE__) . "/../nginx.conf");
    if (file_exists($bastpath)) {
        unlink($bastpath);
    } 

    $fopen = fopen($bastpath, 'w+');
    fwrite($fopen, $data);
    fclose($fopen);
} 

//IIS无需组件支持伪静态
function web_config() {
    global $dbm, $page;
    m__list();
    $urls = $page['url_rewrites'];

    $htastart = "<?xml version=\"1.0\"	encoding=\"UTF-8\"?> \r\n";
    $htastart .= "<configuration> \r\n";
	$htastart .= "<system.webServer> \r\n";
    $htastart .= "<rewrite>  \r\n";
    $htastart .= "<rules>";
    $htaccess = '';
    $flag = false;

    foreach($urls['list'] as $key => $url) {
        if ($url['url_title'] == 'content_app_history') {
            $url_rule = str_replace(array("{id}", "{hid}"), array("(\d+)", "(\d+)"), $url['url_rule']);
            $url_true = str_replace(array("{id}", "{hid}"), array("{R:1}", "{R:2}"), $url['url_true']);
            $flag = true;
        } else if (in_array($url['url_title'], array('list_info', 'list_app'))) {
            $url_rule = str_replace(array("{cid}", "{p}"), array("(\d+)", "(\d+)"), $url['url_rule']);
            $url_true = str_replace(array("{cid}", "{p}"), array("{R:1}", "{R:2}"), $url['url_true']);
            $flag = true;
        } else if (in_array($url['url_title'], array('content_app', 'content_info', 'special_content'))) {
            $url_rule = str_replace(array("{id}"), array("(\d+)"), $url['url_rule']);
            $url_true = str_replace(array("{id}"), array("{R:1}"), $url['url_true']);
            $flag = true;
        } else if (in_array($url['url_title'], array('list_info_index', 'list_app_index', 'special_list_index', 'lastupdate_index'))) {
            $url_rule = str_replace(array("{cid}"), array("(\d+)"), $url['url_rule']);
            $url_true = str_replace(array("{cid}"), array("{R:1}"), $url['url_true']);
            $flag = true;
        } else if (in_array($url['url_title'], array('cpy_list_app', 'cpy_list_info'))) {
            $url_rule = str_replace(array("{cpy}", "{p}"), array("(\w+?)", "(\d+)"), $url['url_rule']);
            $url_true = str_replace(array("{cpy}", "{p}"), array("{R:1}", "{R:2}"), $url['url_true']);
            $flag = true;
        } else if (in_array($url['url_title'], array('cpy_list_app_index', 'cpy_list_info_index'))) {
            $url_rule = str_replace(array("{cpy}"), array("(\w+?)"), $url['url_rule']);
            $url_true = str_replace(array("{cpy}"), array("{R:1}"), $url['url_true']);
            $flag = true;
        } else if (in_array($url['url_title'], array('special_list', 'lastupdate'))) {
            $url_rule = str_replace(array("{p}"), array("(\d+)"), $url['url_rule']);
            $url_true = str_replace(array("{p}"), array("{R:1}"), $url['url_true']);
            $flag = true;
        }  else if($url['url_title'] == 'img') {
			$url_rule = str_replace(array("{url}","{type}"),array("(.*)","(\w+)"), $url['url_rule']);
            $url_true = str_replace(array("{url}","{type}"), array("{R:1}","{R:2}"), $url['url_true']);
            $flag = true;
		}
        if ($flag) {
			$htaccess .="<rule name=\"".$url['url_title']."\">". "\r\n";
            $htaccess .="	<match url=\"^".substr(SITE_PATH,1). $url_rule . "$\" />". "\r\n";
			$htaccess .="	<action type=\"Rewrite\" url=\"".$url_true."\" />". "\r\n";
			$htaccess .="</rule>". "\r\n";
            $flag = false;

        } 
    } 
	$htaend = "</rules>\r\n";
	$htaend .="</rewrite> \r\n";
	$htaend .="</system.webServer> \r\n";
	$htaend .="</configuration> \r\n";
    $data = $htastart . $htaccess . $htaend;
    $bastpath = (dirname(__FILE__) . "/../web.config");
    if (file_exists($bastpath)) {
        unlink($bastpath);
    } 
    $fopen = fopen($bastpath, 'w+');
    fwrite($fopen, $data);
    fclose($fopen);
} 
?>