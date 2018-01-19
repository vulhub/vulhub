<?php
/**
 * AppCMS: (C)2013  
 * AppCMS is a free open source mobile phone APP application download website content management system.
 * Custom development, production BUG report template, please contact loyjers@qq.com
 * Author: huliangliang
 * Editor: huliangliang
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
$page['get']['p'] = isset($_GET['p'])?(int)$_GET['p']:1;

check_admin(); //判断用户是否登录
/**
 * 页面动作 model 分支选择  
 *        动作函数写在文件末尾，全部以前缀 m__ 开头
 */
$page['get']['m'] = isset($_GET['m'])?$_GET['m']:'list';

if (function_exists("m__" . $page['get']['m'])) {
    call_user_func("m__" . $page['get']['m']);
} 

$time_end = helper :: getmicrotime(); //主程序执行时间，如果要包含模板的输出时间，则可以调用该静态时间方法单独计算
$page['data_fill_time'] = $time_end;
/**
 * 模板载入选择
 *        模板页面为PHP+HTML混合编写，如果模板页面中也有区块函数，模板函数以 tpl__ 为前缀
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

function m__list() {
    global $dbm, $page, $c;
	$p = isset($page['get']['p']) ? intval($page['get']['p']) : 1;
    $params['where'] = ' 1=1'; 
	if (isset($page['get']['q']) && !empty($page['get']['q'])) {
	
		$where = " and q like '%" . helper :: escape($page['get']['q']) . "%' ";
		$params['where'] .= $where;
	}
	
	if(isset($page['post']['keyword_id']) && $page['post']['keyword_id'] > 0) {
		$page['post']['keyword_id'] = helper :: escape($page['post']['keyword_id']);
		$params['where'] = ' 1=1 and id="' . $page['post']['keyword_id'] . '"';
	}
	$params['table_name'] = TB_PREFIX . "search_keyword";
	$params['count'] = 1;
	$params['suffix'] = " order by qorder DESC,id DESC ";
	$params['pagesize'] = PAGESIZE;
	$params['suffix'] .= $dbm -> get_limit_sql(PAGESIZE, $p);

	$keyword = $dbm -> single_query($params);
	if(is_array($keyword['list']) && count($keyword['list']) > 0) {
		$page['keyword'] = $keyword;
	}
	$page['keyword']['pagecode'] = $keyword['pagebar']['pagecode'];
	
}

/**
 * 编辑/添加关键字
 */
function m__edit(){
	global $dbm, $page, $c;
	if (!is_numeric($page['post']['keyword_id'])) { die('{"code":"210","msg":"关键字ID必须是数字"}'); }
	if (!is_numeric($page['post']['q_order'])) { die('{"code":"210","msg":"排序必须是数字"}'); }
	if (empty($page['post']['q_name'])) { die('{"code":"220","msg":"关键字不能为空"}'); }
	if (empty($page['post']['q_num'])) {
		$page['post']['q_num'] = 0; 
	} else {
		if (!is_numeric($page['post']['q_num'])) {
			die('{"code":"210","msg":"搜索次数必须是数字"}');
		}
		$page['post']['q_num'] = intval($page['post']['q_num']); 
	}
	$params['qnum'] = helper :: escape($page['post']['q_num']);
	$params['qorder'] = helper :: escape($page['post']['q_order']);
	$params['q'] = htmlspecialchars(helper :: escape($page['post']['q_name']));
	
	if ($page['post']['keyword_id'] > 0) {
		//更新关键字
		$where = " id ='" . $page['post']['keyword_id'] . "'";
		$res = $dbm -> single_update(TB_PREFIX . "search_keyword", $params, $where);
		if (empty($res['error'])) { die('{"code":"0","msg":"更新关键字成功"}'); }
		die('{"code":"240","msg":"编辑关键字失败"}');
	} else {
		//添加关键字
		$res = $dbm -> single_insert(TB_PREFIX . "search_keyword", $params);
		if (empty($res['error']) && $res['autoid'] > 0) { die('{"code":"0","msg":"添加关键字成功"}'); }
		die('{"code":"250","msg":"添加关键字失败，请核对后重试"}');
	}
}

// 批量修改排序
function m__save_qorder() {
    global $page, $dbm, $c;
    $arr = json_decode(stripslashes($page['post']['params']), 1);
    if (count($arr) > 0) {
        foreach($arr as $v) {
            $sql = "update " . TB_PREFIX . "search_keyword set qorder=" . intval($v['val']) . " where id=" . intval($v['id']) . "";
            $res = $dbm -> query_update($sql); 
        } 
        if (empty($res['error'])) {
            die('{"code":"0","msg":"修改排序成功"}');
        } 
    } else {
        die('{"code":"1","msg":"没有数据可修改"}');
    } 
} 
//删除关键字
function m__del() {
    global $page, $dbm, $c;
	if (isset($page['post']['keyword_id'])) {
		$page['post']['params'] = intval($page['post']['keyword_id']);
	}
	if(empty($page['post']['params'])) { die('{"code":"100","msg":"没有选中删除的关键字"}'); }
	$ids = stripslashes($page['post']['params']);
	$ids = explode(',', $ids);
	$count = 0;
	foreach ($ids as $id) {
		$id = intval($id);
		$where = " id = '" . $id . "'";
		$res = $dbm -> single_del(TB_PREFIX . "search_keyword", $where);
		if (!empty($res['error'])) continue;
		$count++;
	}
	die('{"code":"0","msg":"删除成功"}');
} 


?>