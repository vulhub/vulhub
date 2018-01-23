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

$page['get'] = $_GET; //get参数的 m 和 ajax 参数是默认占用的，一个用来执行动作函数，一个用来判断是否启用模板还是直接输出JSON格式数据
$page['post'] = $_POST;
// $page['cates']=caches('categorys','',3600,'/cache','categorys',1);
check_admin(); //判断用户是否登录

/**
 * 页面动作 model 分支选择  
 *       动作函数写在文件末尾，全部以前缀 m__ 开头
 */
$page['get']['m'] = isset($_GET['m'])?$_GET['m']:'list';

if (function_exists("m__" . $page['get']['m'])) {
    call_user_func("m__" . $page['get']['m']);
} 

$time_end = helper :: getmicrotime(); //主程序执行时间，如果要包含模板的输出时间，则可以调用该静态时间方法单独计算
$page['data_fill_time'] = $time_end - $time_start;
$page['on'] = 6;
/**
 * 模板载入选择
 *       模板页面为PHP+HTML混合编写，如果模板页面中也有区块函数，模板函数以 tpl__ 为前缀
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
    global $page, $dbm;

    $where = "  (1=1) ";
    if (isset($page['get']['search_txt']) && $page['get']['search_txt'] != '') {
        $page['get']['search_type'] = isset($page['get']['search_type']) && $page['get']['search_type'] != ''?$page['get']['search_type']:'keys';
        if ($page['get']['search_type'] == 'keys') {
            $where .= " and(content like '%" . helper :: escape($page['get']['search_txt']) . "%')";
        } 
    } 
    $suffix = '';
    $suffix .= " order by date_add desc ";
    $suffix .= $dbm -> get_limit_sql();
    $params = array('table_name' => TB_PREFIX . 'comment', 'where' => $where, 'count' => 1, 'suffix' => $suffix);
    $res = $dbm -> single_query($params);
	
	foreach ($res['list'] as $k => $v) {
		if ($v['type'] == 0) {
			$tablename = 'app_list';
			$where_field = 'app_id';
			$name = 'app_title';
		} else if($v['type'] == 1) {
			$tablename = 'info_list';
			$where_field = 'info_id';
			$name = 'info_title ';
		}
		$rs = $dbm -> query("SELECT * from " . TB_PREFIX . "$tablename WHERE $where_field = '{$v['id']}'");
		if (count($rs['list']) > 0) {
			if($rs['list'][0][$where_field] == $v['id']) {
				$res['list'][$k]['comment_name'] = $rs['list'][0][$name];	
			}
		}
		if (!empty($r['type'])) {
			$res['list'][$k]['url'] = SITE_URL . 'index.php?tpl=content_info&id=' . $v['id'];
		} else {
			$res['list'][$k]['url'] = SITE_URL . '/index.php?tpl=content_app&id=' . $v['id'];
		}
		
	}
	 
    $page['comments']['list'] = $res['list'];
    $page['comments']['pagecode'] = $res['pagebar']['pagecode'];
} 

function m__del() {
    global $page, $dbm;
    $ids = isset($page['post']['params']) && $page['post']['params'] != ''?$page['post']['params']:'';
    $ids = explode(',', $ids);
    foreach($ids as $id) {
        $id = intval($id);
        $sql = " delete " . TB_PREFIX . "comment from " . TB_PREFIX . "comment";
        $sql .= " where comment_id=" . $id . " ";
        $rsarrs = $dbm -> query_update($sql);
    } 
    if (empty($rsarrs['error'])) {
        die('{"code":"0","msg":"成功删除"}');
    } else {
        die('{"code":"1","msg":"操作失败"}');
    } 
} 
function m__edit() {
    global $page, $dbm;
    $ids = isset($page['post']['params']) && $page['post']['params'] != ''?$page['post']['params']:'';
    $ischeck = isset($page['get']['is_check']) && $page['get']['is_check'] != ''?intval($page['get']['is_check']):0;
    $ids = explode(',', $ids);
    foreach($ids as $id) {
        $id = intval($id);
        $sql = " update " . TB_PREFIX . "comment set is_check=$ischeck ";
        $sql .= " where " . TB_PREFIX . "comment.comment_id=" . $id . " "; 
        // echo $sql;
        $rsarrs = $dbm -> query_update($sql);
    } 
    if (empty($rsarrs['error'])) {
        die('{"code":"0","msg":"操作成功"}');
    } else {
        die('{"code":"1","msg":"操作失败"}');
    } 
} 

?>