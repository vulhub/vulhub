<?php
/**
 * AppCMS Copyright (c) 2012-2013  
 * AppCMS is a free open source mobile phone APP application download website content management system.
 * Custom development, production BUG report template, please contact loyjers@qq.com
 * Author: huliangliang
 * Editor: huliangliang

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
 * 清理缓存
 */
function m__list() {
    global $page, $dbm;
	if (isset($_GET['act']) && $_GET['act'] != '') {
		$act = trim($_GET['act']);
		if ($act == 'category') {
			if (file_exists("../cache/categories")) {
				unlink("../cache/categories");
			}
		} else if($act == 'url_rewrite') {
			if (file_exists("../cache/url_config")) {
				unlink("../cache/url_config");
			}
		} else if($act == 'page') {
			$page = array("../cache/content", "../cache/list", "../cache/count");
			foreach($page as $val) {
				del_dir($val);
			}

		}
		die('{"code":"100","msg":"缓存清除成功!"}');
	}
} 



?>