<?php
/**
 * AppCMS Copyright (c) 2012-2013
 * AppCMS is a free open source mobile phone APP application download website content management system.
 * Custom development, production BUG report template, please contact loyjers@qq.com
 * Author: loyjers
 * Editor: loyjers
 * Create Date: 2013.05.25
 *
 * 页面全局变量：
 *
 * @param  $ => $dbm       数据库对象
 * @param  $ => $c         核心方法类对象
 * @param  $ => $cid       分类id，                    默认值=''
 * @param  $ => $cpy       分类拼音                    默认值=''
 * @param  $ => $id        应用id，资讯id，专题id      默认值=''
 * @param  $ => $p         分页页码                    默认值=1
 * @param  $ => $tpl       模板                        默认值='index'
 * @param  $ => $ncate     当前分类数组【仅分类页】
 * @param  $ => $con       内容【仅应用、资讯详情页】
 * @param  $ => $hid       当前历史版本【仅应用历史版本页】
 * @param  $ => $history   当前历史版本数组【仅应用历史版本页】
 * @param  $ => $topid     顶级分类ID【仅应用、应用历史版本、资讯页】
 */

if (is_dir("install") && !file_exists("install/install.lock.php")) {
    header('Location: install/');
    die();
}
require_once(dirname(__FILE__) . "/core/init.php");
// 预防XSS漏洞
foreach ($_GET as $k => $v) {
    $_GET[$k] = htmlspecialchars($v);
}
$dbm = new db_mysql();
// 预处理搜索时的值，主要是防止sql的注入
if (isset($_GET['q'])) {
    //搜索框为空鼠标点击显示15个热搜词
    if (isset($_GET['act']) && $_GET['act'] == 'hot') {
        if (trim($_GET['q']) == '') {
            $sql = "SELECT id,q,qnum FROM " . TB_PREFIX . "search_keyword LIMIT 15";
            $res = $dbm->query($sql);
            if (empty($res['error']) && is_array($res['list'])) {
                foreach ($res['list'] as $k => $v) {
                    $res['list'][$k]['q'] = helper :: utf8_substr($v['q'], 0, 20);
                }
                echo json_encode($res['list']);
                exit;
            } else {
                die();
            }
        }
    }
    // 超出长度截取
    if (strlen($_GET['q']) > 20) {
        $_GET['q'] = helper :: utf8_substr($_GET['q'], 0, 20);
    }

    if (trim($_GET['q']) == '0' || trim($_GET['q']) == '') die('搜索词不能为0或空，请重新输入。点此 <a href ="' . SITE_PATH . '">回到首页</a>');
    if (!preg_match("/^[\x{4e00}-\x{9fa5}\w {0}]+$/u", $_GET['q'])) {
        die('搜索词只允许下划线，数字，字母，汉字和空格，请重新输入。点此<a href ="' . SITE_PATH . '">回到首页</a>');
    }
    //ajax请求联想关键字
    if (trim($_GET['q']) != '' && !isset($_GET['tpl'])) {
        $str = '';
        $sql = "SELECT app_id,app_title,app_down FROM " . TB_PREFIX . "app_list WHERE app_title LIKE '%" . trim($_GET['q']) . "%' LIMIT 15";
        $app_list = $dbm->query($sql);
        if (count($app_list['list']) > 0) {
            foreach ($app_list['list'] as $k => $v) {
                $app_list['list'][$k]['app_title'] = helper :: utf8_substr($v['app_title'], 0, 20);
            }
            echo json_encode($app_list['list']);
            exit;
        } else {
            exit;
        }
    }


    $sql = "SELECT * FROM " . TB_PREFIX . "search_keyword WHERE q = '{$_GET['q']}'";
    $res = $dbm->query($sql);
    if (is_array($res['list']) && count($res['list']) > 0) {
        $sql = "UPDATE " . TB_PREFIX . "search_keyword SET qnum = qnum+1 WHERE q = '{$_GET['q']}'";
        $dbm->query_update($sql);
    } else {
        $fields = array();
        $fields['q'] = helper :: escape($_GET['q']);
        $fields['qnum'] = 1;
        $fields['qorder'] = 0;
        $dbm->single_insert(TB_PREFIX . "search_keyword", $fields);
    }
}


/**
 * 初始化对象
 */
$time_start = helper :: getmicrotime();

$c = new common($dbm);
$cid = isset($_GET['cid']) ? $_GET['cid'] : ''; //分类ID
if (isset($_GET['cid']) && !is_numeric($cid)) die('分类不存在1');

$cpy = isset($_GET['cpy']) ? $_GET['cpy'] : ''; //分类拼音
if (isset($_GET['cpy'])) {
    $cid = $c->cate_cpy($cpy);
    if ($cid == '0') die('分类不存在2');
}

if (isset($_GET['id']) && is_string($_GET['id'])) {
    $appInfo = $c->dbm->query('select app_id from ' . TB_PREFIX . 'app_list where rewrite_tag="' . $_GET['id'] . '"');
    isset($appInfo['list'][0]['app_id']) ? $_GET['id'] = $appInfo['list'][0]['app_id'] : '';
}

$id = isset($_GET['id']) ? $_GET['id'] : ''; //应用ID，资讯ID，专题ID
if (isset($_GET['id']) && !is_numeric($id)) die('信息不存在');

$p = isset($_GET['p']) ? $_GET['p'] : 1; //分页页码
if (!is_numeric($p)) $p = 1;

$hid = isset($_GET['hid']) ? $_GET['hid'] : 0; //历史版本ID

if (!isset($_GET['hid']) && !is_numeric($hid)) {
    die('历史版本不存在 ^V^');
}
/**
 * 页面动作 model 分支选择，动作函数写在文件末尾，全部以前缀 m__ 开头
 */
$_GET['m'] = isset($_GET['m']) ? $_GET['m'] : '';
if (function_exists("m__" . $_GET['m'])) call_user_func("m__" . $_GET['m']);

/**
 * 参数tpl判断模板，模板载入
 */
// 1)模板选择参数
$tpl = isset($_GET['tpl']) ? $_GET['tpl'] : 'index';
// 2.1)判断分类绑定模板
if (intval($cid) > 0) {
    $ncate = isset($c->categories[$cid]) ? $c->categories[$cid] : '';
    if ($ncate == '') die('分类不存在');
    if ($ncate['tpl_listvar'] != '') { // 列表页不为空，当前页面大于第一页，替换为列表页模板
        $tpl = substr($ncate['tpl_listvar'], 0, stripos($ncate['tpl_listvar'], "."));
    }
    if ($ncate['tpl_index'] != '') { // 封面页不为空，当前页面为第一页，替换为封面模板
        if ($p == 1) $tpl = substr($ncate['tpl_index'], 0, stripos($ncate['tpl_index'], "."));
    }
}
// 2.2)判断分类下内容页面模板，资讯详情页，应用详情页，应用历史版本页

if ($tpl == 'content_app') {
    $c->update_vistor($id, 0);
    $con = $c->get_content($id, 0);
    if (!isset($con['app_id'])) {
        die('信息不存在');
    }
} elseif ($tpl == 'content_app_history') {
    $c->update_vistor($id, 0);
    $con = $c->get_content($id, 0);
    // exit(print_r($con['history']));
    foreach ($con['history'] as $a) { // 判断当前应用版本信息
        if ($a['history_id'] == $hid) {
            $history = $a;
            // print_r($history);
            continue;
        }
    }
    if (!isset($history)) die('历史版本数据不存在');
} elseif ($tpl == 'content_info') {
    $c->update_vistor($id, 1);
    $con = $c->get_content($id, 1);
    if (!isset($con['info_id'])) {
        die('信息不存在');
    }
}

if (isset($con) && $c->categories[$con['last_cate_id']]['tpl_content'] != '') $tpl = $c->categories[$con['last_cate_id']]['tpl_content'];
// 内容页顶级分类ID判断，导航条使用
if (isset($con)) {
    $top = $c->cate_father($con['last_cate_id']);
    $topid = $top[0]['cate_id'];
}
// 3)组合模板页面路径
$from_mobile = TEMPLATE;

if (defined('CONTENT_MOBILE') && CONTENT_MOBILE == 1) { // 自适应WAP浏览
    if ($c->from_mobile()) {

        $from_mobile = defined('WAP_TPL') && WAP_TPL ? WAP_TPL : m;
    }
}

if (defined('WAP_URL') && WAP_URL != '' && 'http://' . $_SERVER['HTTP_HOST'] == WAP_URL) { // WAP独立域名浏览
    $from_mobile = WAP_TPL;
}

if (substr($_SERVER['HTTP_HOST'], 0, 4) == 'mkt.') { // 手机客户端
    $from_mobile = "mkt";
}

if (substr($tpl, strlen($tpl) - 4, 4) == '.php') {
    $tmp_file = '/templates/' . $from_mobile . '/' . $tpl;
} else {
    $tmp_file = '/templates/' . $from_mobile . '/' . $tpl . '.php';
}
if (!file_exists(dirname(__FILE__) . $tmp_file)) die('模板页面不存在' . $tmp_file);
require(dirname(__FILE__) . $tmp_file);
/**
 * 其他动作函数开始
 */
echo('<!-- Powerd by AppCMS 1.3.139 -->');
?>
