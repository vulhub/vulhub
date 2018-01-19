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
$page['self_cate'] = selft_cate();
$page['tree'] = $c -> cate_tree(1);
check_admin(); //判断用户是否登录
/**
 * 页面动作 model 分支选择  
 *            动作函数写在文件末尾，全部以前缀 m__ 开头
 */
$page['get']['m'] = isset($_GET['m'])?$_GET['m']:'list';

if (function_exists("m__" . $page['get']['m'])) {
    call_user_func("m__" . $page['get']['m']);
} 

$time_end = helper :: getmicrotime(); //主程序执行时间，如果要包含模板的输出时间，则可以调用该静态时间方法单独计算
$page['data_fill_time'] = $time_end - $time_start;

/**
 * 模板载入选择
 *            模板页面为PHP+HTML混合编写，如果模板页面中也有区块函数，模板函数以 tpl__ 为前缀
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
// 数据入库
function m__into_app() {
    global $dbm, $page, $c;
    $message = array();
    $page['apps'] = isset($page['post']['apps']) && !empty($page['post']['apps'])?$page['post']['apps']:'';
    $page['cateid'] = isset($page['post']['cateid']) && !empty($page['post']['cateid'])?$page['post']['cateid']:'';
    if ($page['apps'] == '') {
        array_unshift($message, '数据为空');
        die();
    } 
    //$page['apps']=preg_replace('~\r|\n|\t~','',$page['apps']);
    $page['apps'] = str_replace(array("\r","\n","\t"),array("{r}","{n}","{t}"),$page['apps']);
    $page['apps'] = helper::escape_stripslashes($page['apps']);
	$page['apps'] = str_replace(array("{r}","{n}","{t}"),array("<br>","<br>",""),$page['apps']);
   
    if ($page['cateid'] == '') {
        $file = "../cache/cate_bind";
        if (!file_exists($file)) {
            array_unshift($message, '请先绑定分类');
            die();
        } 
        $cateids = file_get_contents($file);
        $cateids = unserialize($cateids);
    } 
    $apps = json_decode($page['apps'], 1);//print_r($page['apps']);
    if (!isset($apps)) die('{"code":"1","msg":"数据转化失败，可能有不正常字符！"}');
    $total = 0;
    $sucsse = 0;
    $chongfu = 0; 
    $upnum=0;
    // 插入数据<u></u>
    foreach($apps as $v) {
        $total += 1; 
        // 检查是否绑定了分类
        if ($page['cateid'] == '') {
            $cateid = is_bind($cateids, $v['last_cate_id']);
        } else {
            $cateid = $page['cateid'];
        } 
        if ($cateid == '' || $cateid == 0) {
            array_unshift($message, '《' . $v['app_title'] . '》智能添加失败：<font color="red">无法入库，请先 <a href="javascript:void(0);" onclick="C.alert.opacty_close(\'#message\');blindcate();">绑定分类</a></font>');
            continue;
        } 
        $time = time();
        $filed['last_cate_id'] = $cateid;
        $filed['app_title'] = $v['app_title'];
        $filed['app_version'] = $v['app_version'];
        $filed['app_update_time'] = $time;
        $filed['app_size'] = $v['app_size'];
        $filed['app_system'] = $v['app_system'];
        $filed['app_type'] = $v['app_type'];
        $filed['app_logo'] = $v['app_logo'];
        $filed['app_desc'] = $v['app_desc'];
        $filed['app_tags'] = $v['app_tags'];
        $filed['app_company'] = $v['app_company'];
        $filed['app_company_url'] = $v['app_company_url'];
        $filed['data_app_id'] = $v['app_id'];
        $filed['app_recomment'] = $v['app_recomment'];
        $filed['app_down'] = $v['app_down'];
        $order = $v['app_order'];
        $filed['create_time'] = $time; 
        // 判断是否数据库中已经存在该条数据
        try {
            $ress = $c -> get_list(array('type' => 0, 'fields' => 'app_id,last_cate_id', 'where' => ' b.data_app_id="' . $v['app_id'] . '" ', 'history' => 1),1);
        } 
        catch(Exception $e) {
            print_r($e -> getMessage());
        } 
        // 如果该条数据存在且没有版本变化则 continue 否则插入版本
        if (!empty($ress['list'])) {
            if (!empty($ress['list'][0]['history']) && !empty($v['history'])) {
                foreach($v['history'] as $d) {
                    $i = 0;
                    foreach($ress['list'][0]['history'] as $b) {
                        $arr[$i] = $b['appcms_history_id'];
                        $i++;
                    } 
                    if (!in_array($d['history_id'], $arr)) {
                        // 插入历史版本号表
                        $hist['app_id'] = $ress['list'][0]['app_id'];
                        $hist['app_version'] = $d['app_version'];
                        $hist['app_update_time'] = $d['app_update_time'];
                        $hist['app_size'] = $d['app_size'];
                        $hist['app_system'] = $d['app_system'];
                        $hist['appcms_history_id'] = $d['history_id'];
                        $res = $dbm -> single_insert(TB_PREFIX . 'app_history', $hist);
                        if (!empty($res['error'])) {
                            array_unshift($message, '《' . $v['app_title'] . '》<font color=red>插入版本失败</font>');
                        } else {
                            array_unshift($message, '<font color=green>《' . $v['app_title'] . '》插入版本成功</font>');
                        } 
                        // 更新主表
                        $sql = "update " . TB_PREFIX . "app_list set app_version='" . $d['app_version'] . "',app_update_time='" . $d['app_update_time'] . "',app_size='" . $d['app_size'] . "',app_system='" . $d['app_system'] . "' where data_app_id=" . $v['app_id'] . " ";
                        $res = $dbm -> query_update($sql);
                        if (!empty($res['error'])) array_unshift($message, '《' . $v['app_title'] . '》<font color=red>主表版本信息更新失败</font>');
                        $upnum++;
                    } 
                } 
            }
            array_unshift($message, '《' . $v['app_title'] . '》没有新版本，无需更新');
            $chongfu++;
            continue;
        } 
        $res = $dbm -> single_insert(TB_PREFIX . 'app_list', $filed);
        if (!empty($res['error'])) {
            array_unshift($message, '《' . $v['app_title'] . '》<font color=red>新增失败</font>');
            continue;
        } else {
            array_unshift($message, '<font color=green>《' . $v['app_title'] . '》新增成功</font>');
        } 
        // 获得插入的app_id
        $appid = $res['autoid'];
        $cates = $page['cates'];
        $parents = get_cate_parent($cates, $filed['last_cate_id']);
        array_pop($parents);
        array_push($parents, $filed['last_cate_id']);
        foreach($parents as $j) {
            $sql = "update " . TB_PREFIX . "category set cdata=cdata+1 where cate_id=" . $j . " and type=0 ";
            $res = $dbm -> query_update($sql);
            $dbm -> single_insert(TB_PREFIX . 'cate_relation', array('cate_id' => $j, 'id' => $appid));
        } 
        // 更新关系表的字段
        $params = array('id' => $appid, 'type' => 0, 'field' => array('id_down' => $filed['app_down'], 'id_order' => $order, 'id_update_time' => $time, 'id_create_time' => $time));
        update_relation_fields($params); 
        // 更新分类缓存
        $c -> update_cate(); 
        // 插入历史版本信息
        if (!empty($v['history'])) {
            foreach($v['history'] as $k) {
                $hist['app_id'] = $appid;
                $hist['app_version'] = $k['app_version'];
                $hist['app_update_time'] = $k['app_update_time'];
                $hist['app_size'] = $k['app_size'];
                $hist['app_system'] = $k['app_system'];
                $hist['appcms_history_id'] = $k['history_id'];
                $res = $dbm -> single_insert(TB_PREFIX . 'app_history', $hist);
                if (!empty($res['error'])) {
                    array_push($message, '《' . $v['app_title'] . '》<font color=red>版本添加失败</font>');
                } else {
                    array_push($message, '<font color=green>《' . $v['app_title'] . '》版本添加成功</font>');
                } 
            } 
        } 
        // 资源图片入库
        if (!empty($v['resource'])) {
            // 如果开启了应用图片本地化 则下载服务器上的照片 保存到本地
            foreach($v['resource'] as $u) {
                $resou['info_app_id'] = $appid;
                $resou['type'] = 0;
                $resou['resource_url'] = $u['resource_url'];
                $resou['width'] = $u['width'];
                $resou['height'] = $u['height'];
                $resou['size'] = $u['size'];
                $res = $dbm -> single_insert(TB_PREFIX . 'resource_list', $resou);
            } 
            array_unshift($message, '《' . $v['app_title'] . '》应用截图，共 <b>' . count($v['resource']) . '</b> 张');
        } 

        $sucsse += 1;
        array_unshift($message, '《' . $v['app_title'] . '》新增完毕');
    } 
    array_unshift($message, '<b>新增 <font color=green>' . $sucsse . '</font> 个，更新 <font color=orange>' . $upnum . '</font> 个，重复 <font color=red>' . $chongfu . '</font> 个</b>');
    $message = implode('<br />', $message);
    die($message);
} 
// 绑定分类
function m__blindcate() {
    global $dbm, $page;
    $my_serv = $page['post']['my_serv'];
    if (strnatcasecmp(PHP_VERSION, '5.4.0') >= 0) {
        // $str = addslashes($str);
    } else {
        if (get_magic_quotes_gpc()) {
            $my_serv = stripslashes($my_serv);
        } 
    } 
    $my_serv = json_decode($my_serv, 1);
    $res = serialize($my_serv);
    $file = "../cache/cate_bind";
    fopen($file, 'w+');
    file_put_contents($file, $res);
    die('{"code":"0","msg":"绑定成功"}');
} 
// 判断绑定文件是否存在
function m__exsits_bind() {
    if (file_exists("../cache/cate_bind")) {
        die('{"code":"0","msg":"绑定文件存在"}');
    } else {
        die('{"code":"1","msg":"请绑定分类"}');
    } 
} 
// 设置授权码
function m__set_auth() {
    $config ='../core/config.php';
    $cf = file_get_contents($config);
    $code = isset($_POST['auth']) && !empty($_POST['auth'])?$_POST['auth']:'';
    if (AUTH_CODE != $_POST['auth']) {
        set_config('AUTH_CODE', $code, $cf);
        file_put_contents($config, $cf);
    } 
    // 删除授权验证文件
    unlink('../appcms.txt');
    die('{"code":"0","msg":"成功设置授权码"}');
} 
// 创建验证授权码文件
function m__creatauthfile() {
    // 创建服务器验证文件 get_contents.txt
    if (file_put_contents('../appcms.txt', 'ok')) {
        die('{"code":"0","msg":"验证文件创建成功"}');
    } else {
        die('{"code":"1","msg":"验证文件创建失败"}');
    } 
} 
// 编辑绑定
function m__editbind() {
    global $dbm, $page, $c;
    $file = "../cache/cate_bind";
    if (!file_exists($file))die('{"code":"1","msg":"没有绑定分类"}');
    $bang = file_get_contents($file);
    $res = unserialize($bang);
    //$res['self'] = $c -> categories();
    $res = json_encode($res);
    die($res);
} 
function selft_cate(){
    global $dbm ,$c;
    $self_cate = $c -> categories();
    $option = "";
    foreach($self_cate as $self){
        if($self['son']==0&&$self['type']==0){
            $option .= '<option value="'.$self['cate_id'].'">'.$self['cname']."</option>";
        }
    }
    return $option;
}
// 判断分类是否绑定
function is_bind($cates, $last_cate_id) {
    $cateid = '';
    foreach($cates as $f) {
        if ($f[1] == $last_cate_id) {
            $cateid = $f[0];
            break;
        } else {
            $cateid = '';
        } 
    } 
    return $cateid;
} 

?>