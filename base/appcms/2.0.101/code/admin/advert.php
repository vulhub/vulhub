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

function m__list() {
    global $dbm, $page, $c;
    $params['where'] = ' area_type=2 ';
    if (isset($page['get']['search_txt']) && $page['get']['search_txt'] != '' && isset($page['get']['search_type']) && $page['get']['search_type'] != '') {
        $params['where'] .= " and(" . $page['get']['search_type'] . " like '%" . helper :: escape($page['get']['search_txt']) . "%')";
    } 
    $parms['count'] = 1;
    $res = $c -> get_recommend($params);
    $ress = array();
    if (!empty($res['list'])) {
        foreach($res['list'] as $v) {
            $v['area_html']=preg_replace('!s:(\d+):"(.*?)";!se', '"s:".strlen("$2").":\"$2\";"', $v['area_html'] );
            $tem = unserialize($v['area_html']);
            $tem = $tem['type'];
            $v['type'] = $tem;
            /**
             * $v['title']=urldecode($v['title']);
             * $v['area_remarks']=urldecode($v['area_remarks']);
             */
            $ress[] = $v;
        } 
    } 
    $page['adverts'] = $ress;
    $page['pagecode'] = $res['pagebar']['pagecode'];
} 

function m__save_adpace() {
    global $dbm, $c, $page;
    $fields['title'] = isset($_POST['title'])?urldecode(helper :: escape($_POST['title'])):'';
    $fields['area_remarks'] = isset($_POST['area_remarks'])?urldecode(helper :: escape($_POST['area_remarks'])):'';
    $area_id = isset($_POST['area_id'])?$_POST['area_id']:0;
    $fields['area_type'] = 2;
    $type = isset($_POST['type'])?$_POST['type']:'1'; 
    // 添加广告位
    if ($area_id == 0) {
        if ($type == 1) {
            $arr = array('type' => $type,
                'area_html' => '' // 文字广告
                );
        } else {
            $arr = array('type' => $type,
                'area_html' => array() // 轮播广告
                );
        } 
        $html = serialize($arr);
        $fields['area_html'] = $html;
		//print_r($fields);
        $res = $dbm -> single_insert(TB_PREFIX.'recommend_area', $fields);
        if (empty($res['error'])) die('{"code":0,"msg":"添加成功"}');
    } else {
        // 编辑广告位
        $res = $dbm -> single_update(TB_PREFIX.'recommend_area', $fields, " area_id='" . $area_id . "'");
        if (empty($res['error'])) die('{"code":0,"msg":"编辑成功"}');
    } 
} 

function m__edit_adpace() {
    global $dbm, $c, $page;
    $area_id = isset($_POST['area_id'])?$_POST['area_id']:'';
    $params['area_id'] = $area_id;
    $res = $c -> get_recommend($params);
    $res['title'] = $res['title'];
    $res['area_remarks'] = $res['area_remarks'];
    $area_html = $res['area_html'];
    $area_html = unserialize($area_html);
    $res['type'] = $area_html['type'];
    die(json_encode($res));
} 
// 保存广告
function m__save_ads() {
    global $dbm;
    $ads = isset($_POST['list'])?$_POST['list']:'';
    $area_id = isset($_POST['area_id'])?$_POST['area_id']:'';

    if ($_POST['type'] == 1) {
        // 文字广告
        $ads = helper :: escape($ads);
        $ads = preg_replace('~"~', '&#34;', $ads);
        $ads = helper::escape_stripslashes($ads);
        $arr = array('type' => $_POST['type'],
            'area_html' => $ads
            );
        $fields['area_html'] = serialize($arr);
    } elseif ($_POST['type'] == 2) {
        // 轮播广告
        $ads = helper :: escape_stripslashes($ads);
        $list = json_decode(urldecode($ads), 1);
        $arr = array('type' => $_POST['type'],
            'area_html' => $list,
            );

        $fields['area_html'] = serialize($arr);
    } 
    $fields['area_html'] = preg_replace("~'~", "\'", $fields['area_html']);
    $res = $dbm -> single_update(TB_PREFIX.'recommend_area', $fields, "  area_id='" . $area_id . "'");
    if (empty($res['error'])) die('{"code":"0","msg":"操作成功"}');
    die('{"code":"1","msg":"失败"}');
} 

function m__edit_ads() {
    global $dbm, $c;
    $area_id = isset($_POST['area_id'])?$_POST['area_id']:'';
    $params['area_id'] = $area_id;
    $res = $c -> get_recommend($params);
    $area_html = $res['area_html'];
    $area_html = unserialize($area_html);
    $area_html = json_encode($area_html);
    die($area_html);
} 
// 删除广告位
function m__del_adpace() {
    global $page, $dbm; 
    // 直接传过来的删除动作
    if (isset($page['post']['area_id'])) {
        $page['post']['params'] = $page['post']['area_id'];
    } 
    if (empty($page['post']['params'])) die('{"code":"100","msg":"没有选中数据"}');
    $ids = stripslashes($page['post']['params']);
    $ids = explode(',', $ids);
    $count = 0;
    foreach($ids as $id) {
        $id = intval($id);
        $where = " area_id = '" . $id . "'";
        $res = $dbm -> single_del(TB_PREFIX . "recommend_area", $where);
        if (!empty($res['error'])) continue;
        $count++;
    } 
    // $msg = "共删除了".$count."个友情链接";
    die('{"code":"0","msg":"删除成功"}');
} 

?>