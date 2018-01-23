<?php
/**
 * AppCMS Copyright (c) 2012-2013  
 * AppCMS is a free open source mobile phone APP application download website content management system.
 * Custom development, production BUG report template, please contact loyjers@qq.com
 * Author: Josephu
 * Editor: Josephu
 * Create Date: 2013.05.25
 * Last Editor Date: 2013.05.25
 * File version: 2013.05.13.0001
 * 评论处理页
 */


 require_once(dirname(__FILE__) . "/core/init.php");
 $time_start = helper :: getmicrotime(); //开始时间
 
 $dbm = new db_mysql(); //数据库类实例
 $c = new common($dbm);
 $page['get'] = $_GET; //get参数的 m 和 ajax 参数是默认占用的，一个用来执行动作函数，一个用来判断是否启用模板还是直接输出JSON格式数据
 $page['post'] = $_POST;
 
 $page['get']['m'] = isset($page['get']['m'])?$page['get']['m']:'';

require_once(dirname(__FILE__) . "/core/init.php");
$time_start = helper :: getmicrotime(); //开始时间
$dbm = new db_mysql(); //数据库类实例
$c = new common($dbm);
$page['get'] = $_GET; //get参数的 m 和 ajax 参数是默认占用的，一个用来执行动作函数，一个用来判断是否启用模板还是直接输出JSON格式数据
$page['post'] = $_POST;


$page['get']['m'] = isset($page['get']['m'])?$page['get']['m']:'';

if (function_exists("m__" . $page['get']['m'])) {
    call_user_func("m__" . $page['get']['m']);
}
$time_end = helper :: getmicrotime(); //主程序执行时间，如果要包含模板的输出时间，则可以调用该静态时间方法单独计算
$page['data_fill_time'] = $time_end - $time_start;

 


// 评论列表
function m__list() {
    global $page, $dbm, $c;
    foreach($page['get'] as $key => $val) {
        $page['get'][$key] = helper :: escape($val);
    } 
    $id = $page['get']['id'];if(!is_numeric($id)) die();
    $type = $page['get']['type'];if(!is_numeric($type)) die();
    $p = isset($page['get']['p']) ? $page['get']['p'] : 1;if(!is_numeric($p)) die();
    echo $c -> get_comments($id, $type, $p) . $c -> getcomment_form($type, $id);
    die();
} 
// 添加评论
function m__add() {
    global $page, $dbm, $c;

    $fields = array();
    foreach($page['post'] as $key => $val) {
        $page['post'][$key] = htmlspecialchars(helper :: escape($val));
    } 
    if (empty($page['post']['comment'])) {
        die('{"code":"1","msg":"发表内容不能为空"}');
    } 
    $code = md5(strtoupper($page['post']['code']));
    if ($code != $_SESSION['feedback']) {
        die('{"code":"140","msg":"验证码错误"}');
    }
    $fields['id'] = $page['post']['id'];if(!is_numeric($fields['id'])) die();
    $fields['type'] = $page['post']['type'];if(!is_numeric($fields['type'])) die();
    $fields['parent_id'] = $page['post']['parent_id'];if(!is_numeric($fields['parent_id'])) die();
	$content = $c -> filter_words($page['post']['comment']);
    $fields['content'] = helper :: utf8_substr($content, 0, 300);
	$user = $c -> filter_words($page['post']['user'], 'user');
    $fields['uname'] = helper :: utf8_substr($user, 0, 10);
    $fields['date_add'] = time();
    $fields['ip'] = helper :: getip();
    if ($fields['parent_id'] != 0) {
        $ress = $dbm -> query_update("UPDATE " . TB_PREFIX . "comment SET son = son + 1 WHERE comment_id = '{$fields['parent_id']}'");
    } 
    $res = $dbm -> single_insert(TB_PREFIX . 'comment', $fields);
    if (empty($res['error']) && empty($ress['error'])) die('{"code":"0","msg":"恭喜发表成功"}');
    die('{"code":"1","msg":"发表失败：' . $ress['error'] . '"}');
} 
// ajax请求评论顶一下
function m__cgood() {
    global $page, $dbm, $c;
    $fields = array();
    foreach($page['post'] as $key => $val) {
        $page['post'][$key] = helper :: escape($val);
    } 
    $comment_id = $page['post']['comment_id'];
    if(is_numeric($comment_id)){
        $dbm -> query_update("UPDATE " . TB_PREFIX . "comment SET good = good + 1 WHERE comment_id = '$comment_id'");
        $rs = $dbm -> query("SELECT * from " . TB_PREFIX . "comment WHERE comment_id = '$comment_id'");
        if(count($rs['list'])>0){
            echo $rs['list'][0]['good'];
        }else{
            echo('0');
        }
    }
    die();
} 
// ajax请求评论采一下
function m__cbad() {
    global $page, $dbm, $c;
    $fields = array();
    foreach($page['post'] as $key => $val) {
        $page['post'][$key] = helper :: escape($val);
    } 
    $comment_id = $page['post']['comment_id'];
    if(is_numeric($comment_id)){
        $dbm -> query_update("UPDATE " . TB_PREFIX . "comment SET bad = bad + 1 WHERE comment_id = '$comment_id'");
        $rs = $dbm -> query("SELECT * from " . TB_PREFIX . "comment WHERE comment_id = '$comment_id'");
        if(count($rs['list'])>0){
            echo $rs['list'][0]['bad'];
        }else{
            echo('0');
        }
    }
    die();
} 

?>