<?php
/**
 * AppCMS: (C)2013  
 * AppCMS is a free open source mobile phone APP application download website content management system.
 * Custom development, production BUG report template, please contact loyjers@qq.com
 * Author: chenpeng
 * Editor: chenpeng,loyjers
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
$page['cates'] = $c -> categories;

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
$page['on'] = 5;
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
    $where = ' 1=1 '; 
    // 根据分类筛
    $limit = '';
    $params = array('cate_id' => '', 'where' => '', 'order' => ' order by info_order desc, create_time desc ', 'p' => $page['get']['p'], 'count' => 1, 'resource' => 1, 'type' => 1);
    if (isset($page['get']['cate_id']) && !empty($page['get']['cate_id'])) {
        $params['cate_id'] = $page['get']['cate_id'];
    } 
    if (isset($page['get']['search_type']) && $page['get']['search_type'] == 'keys') {
        if (isset($page['get']['search_txt']) && !empty($page['get']['search_txt']))
            $where .= " and b.info_title like '%" . helper :: escape($page['get']['search_txt']) . "%' ";
    } elseif (isset($page['get']['search_type']) && $page['get']['search_type'] == 'info_id') {
        if (isset($page['get']['search_txt']) && !empty($page['get']['search_txt']))
            $where .= " and b.info_id=" . (int)$page['get']['search_txt'] . " ";
    } 

    $params['where'] = $where;
    $rres = $c -> get_list($params, 1);
    $page['infos']['list'] = count($rres['list']) > 0?$rres['list']:array();
    $page['infos']['pagecode'] = $rres['pagebar']['pagecode'];
    $page['tree'] = $c -> cate_tree(1);
} 
// 添加资讯
function m__edit() {
    global $dbm, $page, $c;
    $info_id = isset($page['post']['info_id']) && !empty($page['post']['info_id'])?(int)$page['post']['info_id']:0;
    $res = $c -> get_content($info_id, 1, 0, 1,0);
    $res = json_encode($res);
    die($res);
}

// 保存资讯
function m__save() {
    global $dbm, $page, $c;
    $field = array();
    if (!isset($page['post']['last_cate_id']) || empty($page['post']['last_cate_id'])) die('{"code":"1","msg":"分类不能为空"}');
    $field['last_cate_id'] = (int)$page['post']['last_cate_id'];
    $field['info_title'] = isset($page['post']['info_title']) && $page['post']['info_title'] != ''?helper :: escape($page['post']['info_title'], 1):'';
    if ($field['info_title'] == '') die('{"code":"1","msg":"标题不能为空"}');

    $field['info_img'] = isset($page['post']['info_img']) && $page['post']['info_img'] != ''?helper :: escape($page['post']['info_img'], 1):'';
    $field['info_body'] = isset($page['post']['info_body']) && $page['post']['info_body'] != ''?helper :: escape($page['post']['info_body'], 1):'';
    $field['info_stitle'] = isset($page['post']['info_stitle']) && $page['post']['info_stitle'] != ''?helper :: escape($page['post']['info_stitle'], 1):'';
    $field['info_tags'] = isset($page['post']['info_tags']) && $page['post']['info_tags'] != ''?helper :: escape($page['post']['info_tags'], 1):'';
    $field['info_from'] = isset($page['post']['info_from']) && $page['post']['info_from'] != ''?helper :: escape($page['post']['info_from'], 1):'';

    $field['uname'] = isset($_SESSION['uname']) && $_SESSION['uname'] != ''?helper :: escape($_SESSION['uname'], 1):'';
    $field['uid'] = $_SESSION['uid'];

    $field['info_url'] = isset($page['post']['info_url']) && $page['post']['info_url'] != ''?helper :: escape($page['post']['info_url'], 1):'';
    $field['info_desc'] = isset($page['post']['info_desc']) && $page['post']['info_desc'] != ''?helper :: escape($page['post']['info_desc'], 1):'';
	if (!empty($field['info_body'])){
		$field['info_body'] = stripslashes($field['info_body']);
		$basehost = "http://" . $_SERVER["HTTP_HOST"];
		preg_match_all("~(src|SRC)=[\"|'| ]{0,}(http:\/\/([^>]*)\.(gif|jpg|jpeg|png))~isU", $field['info_body'], $img_array);
		$img_array = array_unique($img_array[2]);
		$img_path ='/upload' . UPLOAD_PATH  . date("Y") . '/' . date("m") . '/' . date("d") . '/';
		$all_path = dirname(__FILE__)  . '/..' . $img_path;
		helper :: mkdirs(".." . $img_path);
		foreach ($img_array as $key => $value) {
			if (preg_match("~" . $basehost . "~i", $value)) {
				continue;
			}
			if(SITE_URL != $basehost && preg_match("~" . $basehost . "~")) {
				continue;
			}
			if (!preg_match("~^http:\/\/~i", $value)) {
				continue;
			}
			$file = helper :: get_contents($value);
			if(empty($file)) { die('{"code":"0","msg":"<font color=red>图片下载失败</font>"}'); }
			$filename = basename($value);
			$fileurl = $basehost . $img_path . $filename;
			//$fileurl = $basehost . "/appcms" . $img_path . $filename;
			$fopen = fopen($all_path . $filename, 'w');
			fwrite($fopen, $file);
			fclose($fopen);
			if ($file) {
				$field['info_body'] = str_replace($value, $fileurl, $field['info_body']);
				
			}
		}
		$field['info_body'] = helper :: escape($field['info_body']);
	}
    if (isset($page['post']['info_id']) && !empty($page['post']['info_id'])) {
        // 更新
        $field['info_update_time'] = time();
        $info_id = (int)$page['post']['info_id'];
        $where = " info_id=" . $info_id . " ";
        $res = $dbm -> single_update(TB_PREFIX . 'info_list', $field, $where); 
        // 修改关系表
        $params = array('id' => $info_id, 'type' => 1, 'field' => array('id_update_time' => $field['info_update_time']));
        update_relation_fields($params); 
        // 操作资源表
        if (!empty($page['post']['reso'])) {
            $resos = explode("#", $page['post']['reso']);
            foreach($resos as $v) {
                $where = " resource_url='" . helper :: escape($v) . "' ";
                $fields['info_app_id'] = $page['post']['info_id'];
                $fields['type'] = 1;
                $ras = $dbm -> single_update(TB_PREFIX . 'resource_list', $fields, $where);
            } 
        } 
        if (!empty($ras['error']))die('{"code":"1","msg":"资源操作失败"}');
        if (empty($res['error'])) {
            die('{"code":"0","msg":"修改成功"}');
        } else {
            die('{"code":"1","msg":"' . $res['sql'] . '"}');
        } 
    } else {
        // 添加资讯，改变分类表cdata 信息
        $field['create_time'] = time(); //时间只有在添加的时候可以改
        $field['info_update_time'] = time();
        $ress = $dbm -> single_insert(TB_PREFIX . 'info_list', $field);
        $cates = $page['cates'];
        $parents = get_cate_parent($cates, $field['last_cate_id']);
        array_pop($parents);
        array_push($parents, $field['last_cate_id']);
        foreach($parents as $v) {
            $sql = "update " . TB_PREFIX . "category set cdata=cdata+1 where cate_id=" . $v . " and type=1 ";
            $res = $dbm -> query_update($sql);
            $res = $dbm -> single_insert(TB_PREFIX . 'cate_relation', array('cate_id' => $v, 'id' => $ress['autoid'], 'type' => 1));
            if (!empty($res['error'])) die('{"code":1,"msg":"' . $res['error'] . '"}');
        } 
        // 修改关系表
        $params = array('id' => $ress['autoid'], 'type' => 1, 'field' => array('id_create_time' => $field['create_time'], 'id_update_time' => $field['info_update_time']));
        update_relation_fields($params); 
        // 操作资源表
        if (!empty($page['post']['reso'])) {
            $resos = explode("#", $page['post']['reso']);
            foreach($resos as $v) {
                $where = " resource_url='" . helper :: escape($v) . "' ";
                $fields['info_app_id'] = $ress['autoid'];
                $fields['type'] = 1;
                $ras = $dbm -> single_update(TB_PREFIX . 'resource_list', $fields, $where);
            } 
        } 
        if (!empty($ras['error']))die('{"code":"1","msg":"资源操作失败"}');
        /**
         * 更新缓存
         */

        $c -> update_cate();
        if (empty($res['error']) && empty($ress['error'])) die('{"code":"0","msg":"添加成功"}');
        die('{"code":"1","msg":"添加失败：' . $ress['error'] . '"}');
    } 
} 
// 批量修改排序
function m__save_corder() {
    global $page, $dbm, $c;
    $arr = json_decode(stripslashes($page['post']['params']), 1);
    if (count($arr) > 0) {
        foreach($arr as $v) {
            $sql = "update " . TB_PREFIX . "info_list set info_order=" . intval($v['val']) . " where info_id=" . intval($v['id']) . "";
            $res = $dbm -> query_update($sql); 
            // 修改关系表
            $params = array('id' => intval($v['id']), 'type' => 1, 'field' => array('id_order' => $v['val']));
            update_relation_fields($params);
        } 
        if (empty($res['error'])) {
            $c -> update_cate();
            die('{"code":"0","msg":"修改排序成功"}');
        } 
    } else {
        die('{"code":"1","msg":"没有数据可修改"}');
    } 
} 
//提取标签
function m__tags(){
	$info_title = isset($_GET['info_title']) ? trim($_GET['info_title']) : '';
	if($info_title) {
		$tags = helper :: get_tags_baidu($info_title);
	}
	$keyword = array();
	for ($i=0; $i<count($tags);$i++){
		$keyword[] = $tags[$i]['k'];
	}
	die(implode(',', $keyword));

	
}
function m__del() {
    global $page, $dbm, $c;

    $ids = (isset($page['post']['params']) && $page['post']['params'] != '')?$page['post']['params']:'';
    $ids = explode(',', $ids);

    foreach($ids as $id) {
        $id = intval($id);
        $sql = "select last_cate_id,info_img from " . TB_PREFIX . "info_list where info_id=" . $id . " limit 1 ";
        $res = $dbm -> query($sql); 
        // 删除资源
        $resous = $c -> get_resource($id, 1);
        if (!empty($resous)) {
            foreach($resous as $k) {
                if (!del_resource($k['resource_url']))die('{"code":"1","msg":"资源失败"}');
            } 
        } 
        // 删除缩略图
        if (!del_resource($res['list'][0]['info_img']))die('{"code":"1","msg":"缩略图删除失败"}');
        $sql = "delete from " . TB_PREFIX . "info_list where info_id='$id'";
        $dbm -> query_update($sql); //echo($sql);
        $sql = "delete from " . TB_PREFIX . "cate_relation where id='$id' and type='1'";
        $dbm -> query_update($sql); //echo($sql);  
        // 修改分类cata_id
        $cates = $page['cates'];
        $parents = get_cate_parent($cates, $res['list'][0]['last_cate_id']);
        array_pop($parents);
        array_push($parents, $res['list'][0]['last_cate_id']);
        foreach($parents as $v) {
            $sql = "update " . TB_PREFIX . "category set cdata=cdata-1 where cate_id=" . $v . " and type=1 ";
            $res = $dbm -> query_update($sql);
        } 
        // 更改此条资讯的所属类及其父类cdata -1
    } 
    $c -> update_cate();
    if (!empty($res['error'])) die('{"code":"1","msg":"操作失败"}');
    die('{"code":"0","msg":"成功删除"}');
} 
// 删除资源
function m__delse() {
    global $dbm, $page, $c;
    $id = isset($page['post']['id']) && !empty($page['post']['id'])?$page['post']['id']:0;
    $path = isset($page['post']['path']) && !empty($page['post']['path'])?helper :: escape($page['post']['path']):'';
    if (del_resource($path)) {
        die('{"code":"0","msg":"成功删除"}');
    } else {
        die('{"code":"1","msg":"删除失败"}');
    } 
} 

?>