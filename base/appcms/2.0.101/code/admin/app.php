<?php
/**
 * AppCMS Copyright (c) 2012-2013  
 * AppCMS is a free open source mobile phone APP application download website content management system.
 * Custom development, production BUG report template, please contact loyjers@qq.com
 * Author: crane
 * Editor: crane,chenpeng
 */
// 加载核心文件类
require_once(dirname(__FILE__) . "/../core/init.php");

$time_start = helper :: getmicrotime();
// 取得GET和POST变量并进行验证，参数分支选择
$page['get'] = $_GET;
$page['post'] = $_POST; 
// 实例化数据库类
$dbm = new db_mysql();
$c = new common($dbm);
// 判断用户是否登录
check_admin();
// 设置页面数据
$page['get']['m'] = isset($_GET['m'])?$_GET['m']:'list';

if (function_exists("m__" . $page['get']['m'])) {
    call_user_func("m__" . $page['get']['m']);
} 

$time_end = helper :: getmicrotime();
$page['data_fill_time'] = $time_end - $time_start; //执行时间

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
 * 获取所有应用列表
 */
function m__list() {
    global $page, $dbm, $c;
    $p = isset($page['get']['p']) ? $page['get']['p'] : 1; 
    // 判断是否需要查找
    $params['where'] = " 1=1";
    if(isset($page['get']['search_type']) && $page['get']['search_type'] == 'app_id'){
        $where = " and ".$page['get']['search_type']  ."='".$page['get']['search_txt']."'";
        $params['where'] .= $where;
    }else if (isset($page['get']['search_type']) && isset($page['get']['search_txt']) && !empty($page['get']['search_txt']) && !empty($page['get']['search_type']) ) {
        $where = " and " . $page['get']['search_type'] . " like '%" . $page['get']['search_txt'] . "%'" ;
        $params['where'] .= $where;
    } 
    // 这个是jquery传过来编辑友链时获取数据的
    if (isset($page['post']['app_id']) && $page['post']['app_id'] > 0) {
        $page['post']['app_id'] = stripslashes(helper :: escape($page['post']['app_id']));
        $params['where'] .= ' and app_id="' . $page['post']['app_id'] . '"';
    } 
    if (isset($page['get']['cate_id']) && !empty($page['get']['cate_id'])) {
        $params['cate_id'] = $page['get']['cate_id'];
    } 
    $app = $page['post']; 
    // 判断id是否是数字
    $params['p'] = $p;
    $params['strip_tags_desc']=0;
    $params['type'] = 0; //0表示取应用列表
    $params['history'] = 1; //1表示去历史版本
    $params['resource'] = 1;
    $params['count'] = 1 ; //计算总数
    $params['order'] = 'order by app_id desc';
    $params['pagesize'] = PAGESIZE; 
    // exit(print_r($params));
    $apps = $c -> get_list($params,1);
    $page['apps'] = $apps;
    $page['tree'] = $c -> cate_tree('0'); 
	//echo $apps['sql'];
    // exit(print_r($page));
} 

/**
 * 编辑或者添加应用
 */
function m__save_edit() {
    global $page, $dbm, $c;
    $resource = array();
    foreach($page['post'] as $key => $val) {
        $page['post'][$key] = helper :: escape(urldecode($val)); 
        // 获取资源的信息，放到行数组里面
        if (strstr($key, "resource_url")) {
            $resource[] = urldecode($val);
            unset($page['post'][$key]);
        } 
		if(preg_match("~app_version|app_size|appcms_history_id|down_url|app_system|packname~i",$key)){
			 unset($page['post'][$key]);
		}
    } 
    if (isset($page['get']['app_id'])) $page['post']['app_id'] = $page['get']['app_id']; 
    // exit(print_r($page));
    if (!is_numeric($page['post']['app_id'])) die('{"code":"210","msg":"应用ID必须是数字"}');
    if (!is_numeric($page['post']['last_cate_id'])) die('{"code":"210","msg":"终极分类ID必须是数字"}');
    if (empty($page['post']['app_title'])) die('{"code":"100","msg":"应用标题不能为空"}');
    if (!isset($page['post']['create_time']) || empty($page['post']['create_time'])) $page['post']['create_time'] = time();
   
	$page['post']['app_update_time'] = time();
	$page['post']['uid'] = isset($_SESSION['uid']) ? intval($_SESSION['uid']) : '0';
    $page['post']['uname'] = isset($_SESSION['uname']) ? htmlspecialchars($_SESSION['uname']) : '';
   /*修改分类关系的数据*/
    $relation['id'] = $page['post']['app_id'] ;
    $relation['field']['id_update_time'] = time() ;
    $relation['field']['id_visitors'] = $page['post']['app_visitors'];
    if (!isset($page['post']['app_id']) || $page['post']['app_id'] <= 0) {
        // 添加应用
		$where = " app_title = '" . $page['post']['app_title'] . "'";
		$resapp = $dbm -> single_query(array('table_name' => TB_PREFIX . 'app_list', 'where' => $where));
		if (!empty($resapp['list'])) {
			die('{"code":"100","msg":"已经存在该应用"}');
		}
		$param['reso'] = $page['post']['reso'];
		unset($page['post']['reso']);
		unset($page['post']['app_id']);
		//应用表添加数据
		$res = $dbm -> single_insert(TB_PREFIX . 'app_list', $page['post']);
		//die(print_r($res));
		if (empty($res['autoid'])) {
			die('{"code":"200","msg":"添加应用失败"}');
		}
		// 获得插入的app_id
		 $appid = $res['autoid'];
		// 添加分类关系,还需要添加当前分类的父分类关系
        $relacates = $c -> cate_father($page['post']['last_cate_id']);
        $relations['type'] = 0;
        $relations['id'] = $appid;
        $relations['id_down'] = isset($page['post']['app_down']) ? (int)$page['post']['app_down'] : 0;
        $relations['id_order'] = isset($page['post']['app_order']) ? (int)$page['post']['app_order'] : 0;
        $relations['id_update_time'] = time();
        $relations['id_create_time'] = time();
        $relations['id_visitors'] = 0;
        foreach($relacates as $relacate) {
            $relations['cate_id'] = $relacate['cate_id'];
            insert_relation($relations);
        }
		//添加资源表
        if (!empty($param['reso'])) {
            $resos = explode("#", $param['reso']);
            foreach($resos as $v) {
                $where = " resource_url='" . helper :: escape($v) . "' ";
                $fields['info_app_id'] = $res['autoid'];
                $fields['type'] = 0;
                $ras = $dbm -> single_update(TB_PREFIX . 'resource_list', $fields, $where);
            } 
        }
		//更新分类总数
		$c -> cate_restore_cdata($page['post']['last_cate_id']);
		die('{"code":"0","msg":"添加应用成功"}');
		
    } else {
        // 修改应用
		/*
		print_r($page['post']);
		die();*/
		unset($page['post']['reso']);
        $where = " app_id='" . $page['post']['app_id'] . "'";
        $res = $dbm -> single_update(TB_PREFIX . "app_list", $page['post'], $where);
        //更新应用关系
        update_relation_fields($relation);
        if (empty($res['error'])) {
            // 关联资源
            if (!empty($resource)) {
                $resparams['type'] = 0;
                $resparams['info_app_id'] = $page['post']['app_id'];
                foreach($resource as $resk => $resv) {
                    $reswhere = " resource_url='" . $resv . "'";
                    $resourceres = $dbm -> single_update(TB_PREFIX . "resource_list", $resparams, $reswhere);
                } 
            } 
            die('{"code":"0","msg":"更新应用成功"}');
        } 
        die('{"code":"240","msg":"编辑应用失败'.$res['error'].'"}');
    } 
} 

/**
 * 添加分类关系
 * 
 * @param  $relation array 需要添加的关系数组
 */
function insert_relation($relation) {
    global $dbm; 
    // 直接添加
    $newrel = $dbm -> single_insert(TB_PREFIX . 'cate_relation', $relation);
    if (!empty($newres['error'])) {
        helper::logs("系统错误,sql错误，调试");
    }
} 
/**
 * 删除选中的数据
 */
function m__del() {
    global $page, $dbm, $c; 
    $start = time();
    // 直接传过来的删除动作
    if (isset($page['post']['app_id'])) {
        $page['post']['params'] = $page['post']['app_id'];
    } 
    if (empty($page['post']['params'])) die('{"code":"100","msg":"没有选中要删除的应用"}');
    $ids = stripslashes($page['post']['params']);
    $ids = explode(',', $ids);
    $count = 0;
    foreach($ids as $id) {
        $id = intval($id);
        $where = " app_id = '" . $id . "'"; 
        // 获取分类ID，以备下面更改总数使用
        $catesql = " select last_cate_id,app_logo from " . TB_PREFIX . "app_list where " . $where;
        $cres = $dbm -> query($catesql); 
        // 删除资源
        $resous = $c -> get_resource($id, 0);
        if (!empty($resous)) {
            foreach($resous as $k) {
                if (!del_resource($k['resource_url']))die('{"code":"1","msg":"资源失败"}');
            } 
        } 
        // logo资源
        if (!del_resource($cres['list'][0]['app_logo']))die('{"code":"1","msg":"logo资源删除失败"}');
        $cateid = $cres['list'][0]['last_cate_id']; 
        
        // 先删除历史版本的所有信息
        $his = $dbm -> single_del(TB_PREFIX . "app_history", $where);
        if (!empty($his['error'])) die('{"code":"100","msg":"删除历史版本失败"}');
        $res = $dbm -> single_del(TB_PREFIX . "app_list", $where); 
        // 更新分类表中的总数
        $c -> cate_restore_cdata($cateid, 'del');

        /**
         * 更新分类关系
         */
        $c -> cate_insert_relation($cateid, $id, 0, 'del');
        /**
         * $catewhere = " cate_id='" . $cateid . "'";
         * $sql = "update " . TB_PREFIX . "category set cdata=`cdata`-1 where " . $catewhere;
         * $cateres = $dbm -> query_update($sql);
         */
        if (!empty($res['error'])) continue;
        $count++;
    } 
    // 更新分类缓存
    $c -> update_cate(); 
    // $msg = "共删除了".$count."个友情链接";
    die('{"code":"0","msg":"删除成功"}');
} 

/**
 * 编辑或者添加历史版本
 */
function m__history_edit() {
    global $page, $dbm;
    foreach($page['post'] as $key => $val) {
        $page['post'][$key] = helper :: escape(urldecode($val)); 
         //if(empty($val)) die('{"code":"210","msg":"信息不能为空"}');
    } 
    if (!is_numeric($page['post']['his_app_id'])) die('{"code":"210","msg":"应用ID必须是数字"}');
	if(!isset($page['post']['app_version_his']) || trim($page['post']['app_version_his']) == '') die('{"code":"210","msg":"请填写版本号"}');
    $params['app_id'] = $page['post']['his_app_id'];
    $params['app_version'] = $page['post']['app_version_his'];
    $params['app_size'] = $page['post']['app_size_his'];
    $params['app_system'] = $page['post']['app_system_his'];
    //$params['packname'] = $page['post']['packname_his'];
    //$params['appcms_history_id'] = $page['post']['appcms_history_id_his'];
	if(!isset($page['post']['down_url_his']) || trim($page['post']['down_url_his'])=='') die('{"code":"220","msg":"请填写下载地址"}');
	if(substr($page['post']['down_url_his'],0,7) != 'http://') die('{"code":"230","msg":"请正确填写下载地址"}');
    $params['down_url'] = $page['post']['down_url_his'];
    $params['app_update_time'] = time();
    $page['post']['history_id'] = $page['post']['history_id_his']; 
    // 需要更新的app_list字段
    $appparams['app_version'] = $params['app_version'];
    $appparams['app_update_time'] = $params['app_update_time'];
    $appparams['app_size'] = $params['app_size'];
    $appparams['app_system'] = $params['app_system'];
    unset($page['post']['history_id_his']);
    if (!isset($page['post']['history_id']) || $page['post']['history_id'] <= 0) {
        // 添加历史版本
        $res = $dbm -> single_insert(TB_PREFIX . "app_history", $params); 
        // 更新app_list
        $appwhere = " app_id='" . $params['app_id'] . "'";
        if (empty($res['error']) && $res['autoid'] > 0) {
            $appres = $dbm -> single_update(TB_PREFIX . "app_list", $appparams, $appwhere);
            die('{"code":"0","msg":"添加应用历史版本成功'.$res['error'].'"}');
        } 
        die('{"code":"200","msg":"添加历史版本失败，请核对后重新添加'.$res['error'].'"}');
    } else {
        // 修改历史版本
        $where = " history_id='" . $page['post']['history_id'] . "'";
        $res = $dbm -> single_update(TB_PREFIX . "app_history", $params, $where);
        if (empty($res['error'])) die('{"code":"0","msg":"更新应用成功"}');
        die('{"code":"240","msg":"编辑应用失败"}');
    } 
} 
// 获取某个应用的下面的某个历史历史版本
function m__history() {
    global $page, $dbm;
    $page['his'] = get_history($page['post']['history_id']);
} 
/**
 * 获取历史版本
 *
 * @param $history_id int 历史版本的历史id
 *
 * @return $history array 历史版本包含的所有信息，数组形式
 */
function get_history($history_id) {
    global $dbm;
    $sql = "select * from " . TB_PREFIX . "app_history";
    if ($history_id > 0) {
        $where = " where 1=1 and history_id='" . $history_id . "'";
    } else {
        $where = "where 1=1";
    } 
    $sql = $sql . $where;
    $history = $dbm -> query($sql);
    return $history;
} 

/**
 * 修改排序
 */
function m__order() {
    global $page, $dbm ,$c;
    $orders = json_decode(stripslashes($page['post']['params']), true);
    if (empty($orders)) die('{"code":"200","msg":"没有要修改的排序"}');
    foreach ($orders as $order) {
        $params['app_order'] = $order['val'];
        $where = "app_id = '" . $order['id'] . "'";
        $res = $dbm -> single_update(TB_PREFIX . "app_list", $params, $where);

        /*修改分类排序*/
        $rel['id'] = $order['id'];
        $rel['field']['id_order'] = $params['app_order'];
        update_relation_fields($rel);
    } 
    $msg = "修改排序成功";
    die('{"code":"0","msg":"' . $msg . '"}');
} 

/**
 * 删除历史版本
 */
function m__his_del() {
    global $page, $dbm; 
    // 直接传过来的删除动作
    if (isset($page['post']['history_id'])) {
        $page['post']['params'] = $page['post']['history_id'];
    } 
    if (empty($page['post']['params'])) die('{"code":"100","msg":"没有选中要删除的应用"}');
    $ids = stripslashes($page['post']['params']);
    $ids = explode(',', $ids);
    $count = 0;
    foreach($ids as $id) {
        $id = intval($id);
        $where = " history_id = '" . $id . "'"; 
        // 先删除历史版本的所有信息
        $his = $dbm -> single_del(TB_PREFIX . "app_history", $where);
        if (!empty($his['error'])) die('{"code":"100","msg":"删除' . $id . '失败"}');;
        $count++;
    } 
    // $msg = "共删除了".$count."个友情链接";
    die('{"code":"0","msg":"删除成功"}');
} 
/**
 * 显示推荐位列表
 */
function m__recommend() {
    global $page, $dbm;
    $area_type = $page['post']['area_type'];
    $where = " area_type='" . $page['post']['area_type'] . "'";
    $recommend = getrecommend($where);
    $html = "<select name='recommend_list' class='search_type' id='recommend_list'>";
    if (!empty($recommend)) {
        foreach($recommend as $key => $value) {
            $html .= "<option value='" . $value['area_id'] . "'>" . $value['title'] . "</option>";
        } 
    } 
    $html .= "</select>";
    die('{"code":"0","msg":"' . $html . '"}');
} 
/**
 * 保存添加到推荐位的内容
 */
function m__save_recommend() {
    global $page, $dbm;
    $arrids = explode(",", $page['post']['ids']);
    $where = " area_id='" . $page['post']['area_id'] . "'";
    $oldidstr = getrecommend($where);;
    if (!empty($oldidstr[0]['id_list'])) {
        $arroldids = explode(",", $oldidstr[0]['id_list']);
    } else {
        $arroldids = array();
    } 
    $ids = implode(",", checkarray($arrids, $arroldids)); 
    // 保存数据
    $param["id_list"] = $ids;
    $res = $dbm -> single_update(TB_PREFIX . "recommend_area", $param, $where);
    if (empty($res['error'])) {
        die('{"code":"0","msg":"推荐成功"}');
    } 
    die('{"code":"100","msg":"推荐失败"}');
} 
/**
 * 删除关联的资源并且上传文件，将是不可恢复的删除
 */
function m__del_resource() {
    global $page, $dbm; 
    // exit(print_r($page));
    if (del_resource($page['post']['url'])) {
        die('{"code":"0","msg":"删除资源成功","div":"' . $page['post']['id'] . '"}');
    } else {
        die('{"code":"100","msg":"删除资源失败，请重试"}');
    } 
} 

//提取标签
function m__get_tags(){
	$ret = helper :: get_tags_baidu($_POST['title']);
	die(json_encode($ret));
}

?>