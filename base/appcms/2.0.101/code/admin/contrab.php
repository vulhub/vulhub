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


$page['cates'] = $c -> categories;
$page['tree'] = $c -> cate_tree(1);


auto_start();

// 更新分类缓存
$c -> update_cate();


// 开始定时更新
function auto_start() {
    global $page, $c, $dbm;

    error_reporting(0); //报告所有错误，0为忽略所有错误
    ini_set('display_errors', '0'); //开启错误提示
     
    // 判断是否申请了授权码
    $authcode = trim(AUTH_CODE);
    if (empty($authcode)) {
        die('no auth');
    } 

    
    // 判断是否绑定分类
    is_bang_cate(); 
    
    $next_time = 60; 
    // 请求数据接口
    $dataurlupdate = DATA_CENTER_URL . "/appauto.php?auth=" . AUTH_CODE . "&p=1";
    $applist = helper :: get_contents($dataurlupdate); 
    // 获取到解析内容
    if (strlen($applist) > 0) {
        helper :: logs('autodown', 'run' . $flag);
        // 解析内容插入数据
        if (preg_match('~\((.*)\)~', $applist, $applist)) {
            $applist = json_decode($applist[1], true);
            if (isset($applist['code']) && $applist['code'] != '0') {
                die( $applist['msg']);
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
        

    die('complete');
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