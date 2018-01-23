<?php
/**
 * AppCMS Copyright (c) 2012-2013  
 * AppCMS is a free open source mobile phone APP application download website content management system.
 * Custom development, production BUG report template, please contact loyjers@qq.com
 * Author: loyjers
 * Editor: loyjers

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

check_admin(); //判断用户是否登录

/**
 * 页面动作 model 分支选择  
 *       动作函数写在文件末尾，全部以前缀 m__ 开头
 */
$page['get']['m'] = isset($_GET['m'])?$_GET['m']:'';

if (function_exists("m__" . $page['get']['m'])) {
    call_user_func("m__" . $page['get']['m']);
} 

$time_end = helper :: getmicrotime(); //主程序执行时间，如果要包含模板的输出时间，则可以调用该静态时间方法单独计算
$page['data_fill_time'] = $time_end - $time_start;

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
// 保存分类数据
function m__cate_save() {
    global $dbm, $c; 
    // 预处理数据
    foreach($_POST as $a => $b) {
        $_POST[$a] = helper :: escape($b, 1);
    } 
    $_POST['type'] = $_POST['catetype'];
    unset($_POST['catetype']); 
    // 验证数据
    if ($_POST['parent_id'] > 0 && $_POST['type'] != $c -> categories[$_POST['parent_id']]['type']) die('{"code":"100","msg":"子分类必须保持模型一致"}');
    if ($_POST['cname'] == '') die('{"code":"100","msg":"分类名称必须填写"}');
    if ($_POST['cname_py'] == '') die('{"code":"100","msg":"字母别名必须填写"}'); 
    // print_r($_POST);die();
    // 添加分类
    if ($_POST['cate_id'] == '0') {
        $has_py = $dbm -> scalar("select count(0) as t from " . TB_PREFIX . "category where cname_py='" . $_POST['cname_py'] . "'");
        if ($has_py['t'] > 0) die('{"code":"100","msg":"字母别名不允许重复"}'); 
        // 判断是否可以添加子类
        if ($c -> cate_last($_POST['parent_id']) == 0 && @$c -> categories[$_POST['parent_id']]['cdata'] > 0) die('{"code":"100","msg":"分类下有数据，不允许添加子类"}'); 
        // 添加分类
        $a = $dbm -> single_insert(TB_PREFIX . 'category', $_POST); //print_r($a);
        $c -> update_cate();
        die('{"code":"0","msg":"分类添加成功"}');
    } else { // 修改分类
        $_CATE = $dbm -> scalar("select cname_py from " . TB_PREFIX . "category where cate_id='" . $_POST['cate_id'] . "'"); 
        // 如果字母别名发生了变动，则需要判断字母别名是否重复
        if ($_CATE['cname_py'] != $_POST['cname_py']) {
            $has_py = $dbm -> scalar("select count(0) as t from " . TB_PREFIX . "category where cname_py='" . $_POST['cname_py'] . "'");
            if ($has_py['t'] > 0) die('{"code":"100","msg":"字母别名不允许重复"}');
        } 
        // 修改分类
        $where = "cate_id='" . $_POST['cate_id'] . "'";
        $a = $dbm -> single_update(TB_PREFIX . 'category', $_POST, $where);
        $c -> update_cate();
        die('{"code":"0","msg":"分类修改成功"}');
    } 
} 
// 获取某个分类的全部信息
function m__get_cate() {
    global $c;
    $cate_id = $_POST['cate_id'];
    echo(json_encode($c -> categories[$cate_id]));
    die();
} 
// 删除分类
function m__cate_del() {
    global $dbm, $c;
    $cate_id = $_POST['cate_id'];
    if ($c -> categories[$cate_id]['son'] > 0) die('{"code":"0","msg":"有子分类，不可删除"}');
    if ($c -> categories[$cate_id]['cdata'] > 0) die('{"code":"0","msg":"分类下有数据，不可删除"}');
    $sql = "delete from " . TB_PREFIX . "category where cate_id='$cate_id'";
    $dbm -> query_update($sql);
    $c -> update_cate();
    die('{"code":"0","msg":"分类删除成功"}');
    die();
} 
// 移动分类
function m__cate_move() {
    global $dbm, $c;
    $mycateid = $_POST['mycateid']; //当前分类ID
    $moveid = $_POST['moveid']; //目标分类ID
    
    if ($mycateid == $moveid) die('{"code":"100","msg":"当前分类不能移动为自己的子类，请移动到其他分类"}');
    if ($c -> categories[$mycateid]['parent_id'] == $moveid) die('{"code":"100","msg":"分类关系未变，无需移动"}');

    $my_parent = $c -> cate_father($mycateid); //老关系
    $data_count = 0;
    $data_only = 0;
    if (intval($moveid) > 0) {
        // 模型判断
        if ($c -> categories[$mycateid]['type'] != $c -> categories[$moveid]['type']) die('{"code":"100","msg":"不同模型分类不可移动"}'); 
        // 只有终极分类可以移动
        if ($c -> cate_last($mycateid) == 1 && $c -> categories[$mycateid]['cdata'] > 0) die('{"code":"100","msg":"该分类不可移动"}'); 
        // 如果当前分类没有数据
        if ($c -> categories[$mycateid]['cdata'] == 0) {
            // 目标分类有数据并且是终极分类，不可移动
            if ($c -> cate_last($moveid) == 0 && $c -> categories[$moveid]['cdata'] > 0) die('{"code":"100","msg":"目标分类下有数据，无法移动分类"}');
        } else { // 当前分类有数据
            // 目标分类有数据并且是终极分类，只移动数据
            if ($c -> cate_last($moveid) == 0 && $c -> categories[$moveid]['cdata'] > 0) $data_only = 1; //die('{"code":"100","msg":"目标分类下有数据，无法移动分类"}'); 
            // 如果目标分类不是终极分类，不可移动 if ($c -> cate_last($moveid) == 1 && $c -> categories[$moveid]['cdata'] > 0) die('{"code":"100","msg":"目标分类不是终极分类，无法移动数据"}');
            // 查询要移动的数据ID
            $tbname = 'app';
            if ($c -> categories[$mycateid]['type'] == 1) $tbname = 'info'; //表类型
            $rs = $dbm -> query("select " . $tbname . "_id from " . TB_PREFIX . $tbname . "_list where last_cate_id='$mycateid'");
            $move_parent = $c -> cate_father($moveid);
            $data_count = count($rs['list']);
            foreach($rs['list'] as $r) { // 遍历待移动的数据
                // 设置 信息的 last_cate_id
                if ($data_only == 1) $dbm -> single_update(TB_PREFIX . $tbname . "_list", array('last_cate_id' => $moveid), $tbname . "_id='" . $r[$tbname . '_id'] . "'"); 
                // 删除老关系
                $dbm -> query_update("delete from " . TB_PREFIX . "cate_relation where id='" . $r[$tbname . '_id'] . "'"); 
                // 重建新关系
                foreach($move_parent as $p) {
                    $dbm -> single_insert(TB_PREFIX . "cate_relation", array('cate_id' => $p['cate_id'], 'id' => $r[$tbname . '_id']));
                    $dbm -> query_update("update " . TB_PREFIX . "category set cdata=cdata+1 where cate_id='" . $p['cate_id'] . "'"); //计算新关系cdata
                } 
                if ($data_only == 0) $dbm -> single_insert(TB_PREFIX . "cate_relation", array('cate_id' => $mycateid, 'id' => $r[$tbname . '_id']));
            } 
        } 
    } else { // 移动为顶级栏目
        // 查询要移动的数据ID
        $tbname = 'app';
        if ($c -> categories[$mycateid]['type'] == 1) $tbname = 'info'; //表类型
        $rs = $dbm -> query("select " . $tbname . "_id from " . TB_PREFIX . $tbname . "_list where last_cate_id='$mycateid'");
        $data_count = count($rs['list']);
        foreach($rs['list'] as $r) {
            // 删除老关系
            $dbm -> query_update("delete from " . TB_PREFIX . "cate_relation where id='" . $r[$tbname . '_id'] . "'"); 
            // 重建新关系
            $dbm -> single_insert(TB_PREFIX . "cate_relation", array('cate_id' => $mycateid, 'id' => $r[$tbname . '_id']));
        } 
    } 
    // 重算老关系cdata
    foreach($my_parent as $p) {
        if ($p['cate_id'] != $mycateid || $data_only == 1)// 不包括自己
            $dbm -> query_update("update " . TB_PREFIX . "category set cdata=cdata-$data_count where cate_id='" . $p['cate_id'] . "'");
    } 
    // 更改父类ID
    if ($data_only == 0) $dbm -> single_update(TB_PREFIX . "category", array('parent_id' => $moveid), "cate_id='$mycateid'");
    $c -> update_cate();
    die('{"code":"0","msg":"分类移动成功"}');
} 
// 修改分类排序
function m__cate_order() {
    global $dbm, $c;
    $_POST['params'] = helper :: escape_stripslashes($_POST['params']);
    $ids = json_decode($_POST['params'], 1); //print_r($_POST);die();
    foreach($ids as $a) {
        $dbm -> single_update(TB_PREFIX . "category", array('corder' => $a['val']), "cate_id='" . $a['id'] . "'");
    } 
    $c -> update_cate();
    die('{"code":"0","msg":"分类排序修改成功"}');
}
//修复应用和资讯数cdata字段统计数值
function m__cate_cdata(){
    global $dbm,$c;
    set_time_limit(0);
    foreach($c->categories as $a){
		if($a['type'] == 0) {
			$sql = "select count(1) as total from ".TB_PREFIX."cate_relation a left join ".TB_PREFIX."app_list b on a.id=b.app_id where a.cate_id='".$a['cate_id']."'";
		} else {
			$sql = "select count(1) as total from ".TB_PREFIX."cate_relation a left join ".TB_PREFIX."info_list b on a.id=b.info_id where a.cate_id='".$a['cate_id']."'";
		}
	   $rs=$dbm->query($sql);//print_r($rs);
       $sql="update ".TB_PREFIX."category set cdata='".$rs['list'][0]['total']."' where cate_id='".$a['cate_id']."'";
       $rs=$dbm->query_update($sql);
        //print_r($rs);
    }
    $c -> update_cate();
    die('{"code":0,"msg":"修复成功"}');
	
}

// 获取拼音
function m__get_pinyin() {
    include_once(dirname(__FILE__) . "/../core/pinyin.class.php");
    $py = new cn2pinyin();
    $cname = isset($_GET['cname'])?trim($_GET['cname']):'';
    $cname_py = $py -> get($cname);
    die($cname_py);
} 
// 列表模版
function show_tpl() {
    global $page;
    $dir = dirname(__FILE__) . '/../templates/' . TEMPLATE;
    $flils = get_file_list($dir);
    $tpl_list = array();
    foreach($flils as $t) {
        if (preg_match('/^list_/', $t['name'])) {
            $tpl_list['list'][] = $t['name'];
        } elseif (preg_match('/^cover_/', $t['name'])) {
            $tpl_list['cover'][] = $t['name'];
        } elseif (preg_match('/^content_/', $t['name'])) {
            $tpl_list['content'][] = $t['name'];
        } 
    } 
    return $tpl_list;
} 
// 显示全部分类的管理列表
function html_cate_list($tree = array(), $i = 0) {
    global $c;
    $i++;
    foreach($tree as $t) {
        $flag = '';
        for($j = 1;$j < $i;$j++) {
            $flag .= '　　';
        } 
        if (count($t['son']) > 0) {
            $flag .= '┗ ';
        } else {
            $flag .= '┗ ';
        } 
        echo('<tr>');
        echo('<td><input class="corder" pid="' . $t['cate_id'] . '" type="text" size="3" value="' . $t['corder'] . '"></td>');
        echo('<td>' . $t['cate_id'] . '</td>');
        $ctype = '<font color=red>目录分类</font>';
        if ($c -> cate_last($t['cate_id']) == 0) $ctype = '<font color=red>终极分类</font>';
        if ($c -> cate_last($t['cate_id']) == 0 && $t['cdata'] > 0) $ctype = '<font color=green>内容分类</font>';
        echo('<td class="alignleft">' . $flag . '<b style="color:#666;">' . $t['cname'] . '</b> ' . $ctype . ' <font color=#ccc>' . $t['cname_py'] . '</font></td>');
        echo('<td>' . $t['tpl_listvar'] . '</td>');
        echo('<td>' . $t['tpl_content'] . '</td>');
        echo('<td>' . vars :: get_field_str('catetype', $t['type'], '') . '</td>');
        echo('<td>' . $t['cdata'] . '</td>');
        echo('<td>');
        if ($c -> cate_last($t['cate_id']) == 1 && $c -> categories[$t['cate_id']]['cdata'] > 0) {
            echo('<a class="but2 but2s" href="javascript:void(0);" style="background:#ccc;border-color:#ccc;color:#888;cursor:default;">移动</a>');
        } else {
            echo('<a class="but2 but2s" href="javascript:void(0);" onclick="cate_move_show(\'' . $t['cate_id'] . '\',\'' . $t['cname'] . '\');">移动</a>');
        } 
        if ($t['cdata'] == 0) {
            echo('<a class="but2 but2s" href="javascript:void(0);" onclick="cate_del(' . $t['cate_id'] . ');">删除</a>');
        } else {
            echo('<a class="but2 but2s" href="javascript:void(0);" style="background:#ccc;border-color:#ccc;color:#888;cursor:default;">删除</a>');
        } 
        echo('<a class="but2 but2s" href="javascript:void(0);" onclick="cate_show(' . $t['cate_id'] . ',' . $t['parent_id'] . ',' . $t['type'] . ');">修改</a>');
        if ($t['cdata'] == 0 || $c -> cate_last($t['cate_id']) == 1) {
            echo('<a class="but2 but2s" href="javascript:void(0);" onclick="cate_show(0,' . $t['cate_id'] . ',' . $t['type'] . ');">添加子类</a>');
        } else {
            echo('<a class="but2 but2s" href="javascript:void(0);" style="background:#ccc;border-color:#ccc;color:#888;cursor:default;">添加子类</a>');
        } 
        echo('</td>');
        echo('</tr>');
        if (is_array($t['son'])) html_cate_list($t['son'], $i);
    } 
} 

?>