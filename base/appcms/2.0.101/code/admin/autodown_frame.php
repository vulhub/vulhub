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

/**
 * 页面动作 model 分支选择  
 *          动作函数写在文件末尾，全部以前缀 m__ 开头
 */
$page['get']['m'] = isset($_GET['m'])?$_GET['m']:'list';

if (function_exists("m__" . $page['get']['m'])) {
    call_user_func("m__" . $page['get']['m']);
} 
// 更新分类缓存
$c -> update_cate();

/**
 * 同步所有数据
 */
function m__downall() {
    global $page, $dbm, $c; 

    set_time_limit(0);

    // 判断是否申请了授权码
    $authcode = trim(AUTH_CODE);
    if (empty($authcode)) {
        die('<script>top.show_err(\'{"code":"400","msg":"还没有申请授权码，<a href=collect.php target=_parent>点击这里</a> 申请授权码"}\');</script>');
    } 

    $_GET['auth'] = isset($_GET['auth']) ? $_GET['auth'] : '';
    if ($_GET['auth'] != md5(AUTH_CODE)) {
        die('<script>top.show_err(\'{"code":"400","msg":"授权码错误"}\');</script>');
    }
	$p = isset($page['get']['p']) ? $page['get']['p'] : 1; 
	
	//手动输入页码
	//验证起始页必须是数字
	if (isset($_GET['start']) && !empty($_GET['start'])) {
		if (!is_numeric($_GET['start'])) die('<font color=red>开始页码必须是数字</font>');
		$start = (int)$_GET['start'];
		$p = $start;
	}
	//验证结束页必须是数字
	if (isset($_GET['end']) && !empty($_GET['end'])) {
		if(!is_numeric($_GET['end'])) die('<font color=red>结束页码必须是数字</font>');
		$end = (int)$_GET['end'];
		$url = "&end=$end";
	} else {
		$url = '';
	}
	//起始页码必须小于结束页码
	if (!empty($start) && !empty($end)) {
		if ($start > $end) die('<font color=red>开始页码必须小于结束页码</font>');
	}

    // 判断是否绑定分类
    is_bang_cate(); 

    // 开始处理数据
    // 循环添加数据开始
    $dataurl = DATA_CENTER_URL . "/appauto.php?auth=" . AUTH_CODE . "&p=" . $p.$url;
    $applist = helper :: get_contents($dataurl);
    echo('<style>body{margin:0;padding:0;font-size:12px;line-height:200%;}</style>');
    if (strlen($applist) < 2) {
        echo('没有请求到数据，1S后将重新请求：' . $dataurl);
        die("<script language='javascript' type='text/javascript'>setTimeout(function(){window.location.reload();},1000);</script>");
    } 
    // 获取并且解析内容
    if (preg_match('~\((.*)\)~', $applist, $applist)) {
        $applist = json_decode($applist[1], true);
        if (isset($applist['code']) && $applist['code'] != 0) {
            die('<script>top.show_err(\'{"code":"' . $applist['code'] . '","msg":"' . $applist['msg'] . '"}\');</script>');
        } else {
            $applist = $applist['data'];
        } 
    } else {
        die('<script>top.show_err(\'{"code":"400","msg":"未知错误，请重试"}\');</script>');
    } 
    echo('当前第' . $p . '页/共 ' . $applist['total_page'] . '页<br>开始处理数据：总共 ' . count($applist['list']) . ' 数据<br>');

    if ($p > $applist['total_page']) { // 结束判断条件
        die('所有数据同步完成');
    } 
    // 循环添加数据
    foreach($applist['list'] as $app) {
        addapp($app);
    } 
    // 处理数据结束，自动跳转下一页
    // $p++;
	//手动输入页码，处理结束页
	
	if (isset($end)) {
		if($p<$end) {
			$p++;
		} else if($p == $end){
			die('所有数据同步完成'); 
		}
	} else {
		// 处理数据结束，自动跳转下一页
		$p++;
	}
	//网站目录cahce缓存文件夹下autodown文件写入统计更新的页数
	file_put_contents('../cache/autodown',$p);
    $selfurl = "autodown_frame.php?m=downall&auth=" . md5(AUTH_CODE) . "&p=$p".$url;
    echo ('<script language="javascript" type="text/javascript">setTimeout(function(){window.location.href="' . $selfurl . '";},1000);</script>');
    global $time_start;
    echo('Processed in '.number_format(helper::getmicrotime()-$time_start, 2, '.', '').' MilliSeconds ');
} 
// 停止定时更新
function m__auto_stop() {
    if (file_exists("../cache/flag_auto_down")) {
        if (unlink('../cache/flag_auto_down')) {
            die('{"code":"0","msg":"定时更新停止成功"}');
        } else {
            die('{"code":"100","msg":"定时更新停止失败"}');
        } 
    } 
    die('{"code":"0","msg":"定时更新停止成功（已停止）"}');
} 
// 开始定时更新
function m__auto_start() {
    global $page, $c, $dbm;

    error_reporting(0); //报告所有错误，0为忽略所有错误
    ini_set('display_errors', '0'); //开启错误提示
     
    // 判断是否申请了授权码
    $authcode = trim(AUTH_CODE);
    if (empty($authcode)) {
        die('{"code":"100","msg":"还没有申请授权码，<a href=\'collect.php\' target=\'_parent\'>点击这里</a> 申请授权码"}');
    } 

    $_GET['auth'] = isset($_GET['auth']) ? $_GET['auth'] : '';
    if ($_GET['auth'] != md5(AUTH_CODE)) {
        die('{"code":"100","msg":"非法请求911"}');
    } 
    // 判断是否绑定分类
    is_bang_cate(); 
    // 标记请求次数
    $flag = 0;
    $flag_file = "../cache/flag_auto_down"; 
    // 创建标识文件
    if (!file_exists($flag_file)) {
        fopen($flag_file, 'a+');
    } 

    ignore_user_abort(); //关掉浏览器，PHP脚本也可以继续执行.
    set_time_limit(0); // 通过set_time_limit(0)可以让程序无限制的执行下去
    
    if (connection_status () != CONNECTION_NORMAL) {
    } else {
        while (file_exists($flag_file)) {
            // if(connection_status () != CONNECTION_NORMAL) break;//防止重复任务
            if (!file_exists($flag_file)) break; //标识文件不存在立即终止任务（任务延时未执行，已停止任务的情况）
            $next_time = 60; 
            // 请求数据接口
            $dataurlupdate = DATA_CENTER_URL . "/appauto.php?auth=" . AUTH_CODE . "&p=1";
            $applist = helper :: get_contents($dataurlupdate); 
            // 获取到解析内容
            if (strlen($applist) > 0) {
                helper :: logs('autodown', 'run' . $flag);
                $flag++; 
                // 解析内容插入数据
                if (preg_match('~\((.*)\)~', $applist, $applist)) {
                    $applist = json_decode($applist[1], true);
                    if (isset($applist['code']) && $applist['code'] != '0') {
                        if (file_exists($flag_file)) unlink($flag_file);
                        die('{"code":"100","msg":"' . $applist['msg'] . '"}');
                    } else {
                        $applist = $applist['data'];
                        $next_time = $applist['next_time'];
                    } 
                } 
                // 遍历插入数据
                foreach($applist['list'] as $app) {
                    addapp($app);
                } 
            } 
            // 标识文件存在则延时
            if (file_exists($flag_file)) {
                helper :: logs('autodown', 'sleep' . $next_time);
                sleep($next_time);
            } else {
                break;
            } 
        } 
    } 
    die('{"code":"100","msg":"定时更新任务结束"}');
} 
/**
 * 单个添加数据，添加版本必须在添加资源前面执行，因为资源的更新是根据添加版本的结果来的
 * 
 * @param  $app array 需要处理的远程app信息。
 */
function addapp($app) {
    global $dbm, $c; 
    // 应用数据存放变量
    $params = array(); 
    // 判断是否绑定了分类
    /**
     * if (strnatcasecmp(PHP_VERSION, '5.4.0') >= 0) { //作字符处理
} else { 
     * $page['apps'] = stripslashes($page['apps']);
     * }
     */
    // 如果存在就不需要添加
    //根据服务端的版本号取应用
    $where = " data_app_id = '" . $app['app_id'] . "'";
    $resapp = $dbm -> single_query(array('table_name' => TB_PREFIX . 'app_list', 'where' => $where));
    if (!empty($resapp['error'])) {
        die($resapp['sql']);
    } //可注释
    if (empty($resapp['list'])) {
        // 检查是否绑定分类
        $cates = get_cates();
        $params['last_cate_id'] = is_bind($cates, $app['last_cate_id']);

        if (empty($params['last_cate_id'])) {
            echo('<span style="color:red">' . $app['app_title'] . '当前的系统分类没有绑定到本地分类,请到添加应用处理</span><br>'); 
            // 不处理直接下一个
            return ;
        } 
        // 没有数据，需要添加
        // 保存数据
        $params['app_title'] = $app['app_title'];
        $params['app_version'] = $app['app_version'];
        $params['app_update_time'] = time();
        $params['app_size'] = $app['app_size'];
        $params['app_system'] = $app['app_system'];
        $params['app_type'] = $app['app_type'];
        $params['app_logo'] = $app['app_logo'];
        $params['app_desc'] = $app['app_desc'];
        $params['app_company'] = $app['app_company'];
        $params['app_company_url'] = $app['app_company_url'];
        $params['app_tags'] = $app['app_tags'];
        $params['create_time'] = time();
        $params['app_recomment'] = $app['app_recomment'];
        $params['uid'] = '0'; //$_SESSION['uid'];
        $params['uname'] = ''; //$_SESSION['uname'];
        $params['app_down'] = $app['app_down'];
        $params['app_order'] = $app['app_order'];
        $params['data_app_id'] = $app['app_id']; 
        // 入库
        $insertres = $dbm -> single_insert(TB_PREFIX . 'app_list', $params);
        if (empty($insertres['autoid'])) {
            // 说明没有添加成功，找原因，一般不会的
            // die($insertres['error']);
            echo '添加应用' . $params['app_title'] . '失败：'.$insertres['error'].'<br>';
            return ;
        } 
        echo "成功添加了<span style='color:green;'>" . $params['app_title'] . "</span><br>"; 
        // 插入历史版本
        /**
         * 获取新加入的应用ID
         */
        $newid = $insertres['autoid']; 
        // 反序数组，因为是按更新时间从大到小的，所以要反序才会从低到高添加
        $app['history'] = array_reverse($app['history']);
        foreach($app['history'] as $his) {
            $his['app_id'] = $newid;
            insert_history($his, $app['resource'], $his['app_id']);
        } 
        // 添加分类关系,还需要添加当前分类的父分类关系
        $relacates = $c -> cate_father($params['last_cate_id']);
        $relation['type'] = 0;
        $relation['id'] = $newid;
        $relation['id_down'] = $params['app_down'];
        $relation['id_order'] = $params['app_order'];
        $relation['id_update_time'] = time();
        $relation['id_create_time'] = time();
        $relation['id_visitors'] = 0;

        foreach($relacates as $relacate) {
            // 更新关系
            $relation['cate_id'] = $relacate['cate_id'];
            insert_relation($relation);
        } 
        // 更新分类总数
        $c -> cate_restore_cdata($params['last_cate_id']);
        return;
    } else {
        // 已经有了，判断版本是否有需要添加的
        /**
         * 获取新加入的应用ID
         */
        // 当前的应用ID
        echo "应用<span style='color:green;'>" . $app['app_title'] . "</span>存在，开始检查版本<br>";
        $appid = $resapp['list'][0]['app_id'];
        foreach($app['history'] as $his) {
            $his['app_id'] = $appid;
            insert_history($his, $app['resource'], $appid);
        } 
    } 
} 
/**
 * 添加历史版本，如果存在就忽略
 * 图片资源也放在这里一起添加
 * 
 * @param  $history array 历史版本信息。
 * @apram $resource array 资源列表，二维数组
 */
function insert_history($history, $resources, $appid) {
    global $dbm; 
    // 判断历史版本是否存在
    $history['appcms_history_id'] = $history['history_id'];
    $where = " appcms_history_id = '" . $history['appcms_history_id'] . "'";
    $hisres = $dbm -> single_query(array('table_name' => TB_PREFIX . 'app_history', 'where' => $where));
    if (!empty($hisres['list'])) {
        // 说明有了。不需要操作，直接返回
        echo $history['app_version'] . " 版本存在，略过<br>";
        return ;
    } 

    unset($history['history_id']); 
    // unset($history['new_order_set']);
    $newhis = $dbm -> single_insert(TB_PREFIX . 'app_history', $history);
    if (!empty($newhis['error'])) {
        helper::logs($newhis['error'] . "系统错误,sql错误");
    } 
    echo "成功添加版本<span sytle='color:green;'>" . $history['app_version'] . "</span><br>"; 
    // 如果添加了新的历史版本。就要删除资源图片。重新添加资源图片
    if (isset($newhis['autoid']) && $newhis['autoid'] > 1) { // 说明添加版本成功了
        // 更新app_list的相关版本信息
        $where_update = " app_id = '" . $appid . "'";
        $app_update['app_version'] = $history['app_version'];
        $app_update['app_size'] = $history['app_size'];
        $app_update['app_system'] = $history['app_system'];
        $app_update['app_update_time'] = time();
        $updateres = $dbm -> single_update(TB_PREFIX . 'app_list', $app_update, $where_update);
        if (!empty($updateres['error'])) {
            helper::logs($updateres['sql']);
        } 
        // 删除原有的版本信息
        $deletewhere = " info_app_id='" . $appid . "' and type = 0";
        $deleres = $dbm -> single_del(TB_PREFIX . 'resource_list', $deletewhere);
        if (!empty($delers['error'])) {
            helper::logs($deleres['sql']);
        } 
        // 重新添加资源
        foreach($resources as $resource) {
            $resource['info_app_id'] = $appid;
            insert_resource($resource);
        } 
        echo "成功更新了<span style='color:green;'>" . count($resources) . "</span>张资源图片<br>";
    } 
} 
/**
 * 添加资源表,如有存在就不添加
 * 
 * @params $resource  array 需要加入的相关资源信息
 */
function insert_resource($resource) {
    global $dbm;

    unset($resource['id']);
    unset($resource['app_id']);
    $newres = $dbm -> single_insert(TB_PREFIX . 'resource_list', $resource);
    if (!empty($newres['error'])) {
        helper::logs($newres['sql']);
    } 
    // echo "添加资源图片 <a href='".$resource['resource_url']."' target='_blank'>".$resource['resource_url']."</a> <br>";
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
// 判断分类是否绑定
function is_bind($cates, $last_cate_id) {
    $cateid = '';
    foreach($cates as $cate) {
        if ($cate[1] == $last_cate_id) {
            $cateid = $cate[0];
            break;
        } else {
            $cateid = '';
        } 
    } 
    return $cateid;
} 
/**
 * 获取分类
 */
function get_cates() {
    $file = "../cache/cate_bind";
    is_bang_cate();
    $cateids = helper :: get_contents($file);
    $cateids = unserialize($cateids);
    return $cateids;
} 

function is_bang_cate() {
    $file_cate = "../cache/cate_bind";
    if (!file_exists($file_cate)) {
        die('{"code":"300","msg":"<span>请先 <a href=\'collect.php\' target=\'_parent\'>绑定分类</a>"}');
    } 
} 

?>