<?php
/**
 * AppCMS Copyright (c) 2012-2013  
 * AppCMS is a free open source mobile phone APP application download website content management system.
 * Custom development, production BUG report template, please contact loyjers@qq.com
 * Author: crane
 * Editor: chenpeng,crane
 */

/**
 * 删除资源文件
 * 
 * @param int $resource_id 资源ID;
 *        string $resource_url 资源路径
 * @return boolean 成功返回true,失败返回false;
 */
function del_resource($resource_url) {
    global $dbm;
    if ($resource_url == '') return true;
    $url = $_SERVER['DOCUMENT_ROOT'] . $resource_url;
    $where = " resource_url='" . $resource_url . "'"; 
    // 如果图片路径为服务器上的则只删记录
    if (substr($resource_url,0,7)=='http://') {
        $dbm -> single_del(TB_PREFIX . "resource_list", $where);
        return true;
    } 

    if (file_exists($url)) {
        $thumb = explode("/", $url);
        $thumb[(count($thumb)-1)] = "thumb_" . $thumb[(count($thumb)-1)];
        $thumb = implode("/", $thumb);

        if (@unlink($url) && @unlink($thumb)) {
            $dbm -> single_del(TB_PREFIX . "resource_list", $where);
            return true;
        } else {
            return false;
        } 
    } else {
        $dbm -> single_del(TB_PREFIX . "resource_list", $where);
        return true;
    } 
} 

//删除目录及目录下的所以文件
function del_dir($dir_adds='',$del_def=0) {
	$result = false;
	if (! is_dir($dir_adds)) {
		return false;
	}
	$handle = opendir($dir_adds);
	while (($file = readdir($handle)) !== false) {
	if($file != '.' && $file != '..') {
		$dir = $dir_adds . DIRECTORY_SEPARATOR . $file;
		is_dir($dir) ? del_dir($dir) : @unlink($dir);
	}
	}
	closedir($handle);
	if ($del_def == 0) {
		$result = @rmdir($dir_adds) ? true : false;
	} else {
		$result = true;
	}
	return $result;
}


/**
 * 检查用户是否登陆
 */
function check_admin() {
    // print_r($_SESSION);
    if (!isset($_SESSION['uname']) || $_SESSION["ustate"] == '1') {
        //die('你还没有登录，请登陆后操作');
        die('<script language="javascript">top.location.href="index.php";</script>');
    } 
} 

/**
 * 获取功能导航菜单数组,在后台管理页面的左边要显示的功能菜单列表
 */
function get_menu() {
    global $system_struct;
    $menu = array();
    $system = $system_struct[0]['files'];
    foreach ($system as $ks => $vs) {
        if ($vs['menu'] == 1) {
            $menu[] = $vs;
        } 
    } 
    // print_r($menu);
    return $menu;
} 
/**
 * 根据类型后去推荐位的列表
 */
function getrecommend($where) {
    global $dbm, $page;
    $recommend = $dbm -> single_query(array("table_name" => TB_PREFIX . "recommend_area", "where" => $where));
    return $recommend['list'];
} 

/**
 * 给$arr2(一个数组)添加$arr1里面有，但是自己没有的数据
 * 
 * @param  $arr1 需要比较的数组，如果他和$arr2没有相同的值，就添加的$arr2里面
 * @param  $arr2 被比较的值
 * @return array ();
 */
function checkarray($arr1, $arr2) {
    if (empty($arr2)) return $arr1;
    foreach($arr2 as $key => $val) {
        foreach($arr1 as $k => $v) {
            if (empty($val) || empty($v)) continue;
            if (!in_array($v, $arr2)) {
                $arr2[] = $v;
            } 
        } 
    } 
    return $arr2;
} 
/**
 * 退出登录，销毁session
 * 
 * @return bool 退出成功：true;退出失败：false
 */
function login_out() {
    session_destroy();
    if (isset($_SESSION['uname']) || $_SESSION["ustate"] == '0') {
        return false;
    } 
    return true;
} 

/**
 * 左侧菜单使用
 *    比较两个地址的，如果是文件名相同就返回true，不同返回false
 * 
 * @param  $url1 ,要比较的地址1，$url2,需要比较的地址2
 * @return boolean 返回值，成功true,失败false.
 */
function check_url($url) {
    $a = explode('?', $url);
    $b = explode('/', $_SERVER['PHP_SELF']); 
    // 文件名
    $url_file = $a[0];
    $now_file = $b[count($b)-1]; 
    // 参数
    $url_suffix = count($a) > 1?$a[1]:'';
    $now_suffix = $_SERVER['QUERY_STRING']; 
    // echo substr($now_suffix,0,strlen($url_suffix)).'<br>'.$url_suffix.'<br><br><br>';
    if (stripos($_SERVER['PHP_SELF'], $url_file) > 0 && $url_suffix == '') return true;
    if (
        stripos($_SERVER['PHP_SELF'], $url_file) > 0 && $url_suffix != '' && $now_suffix != '' && $url_file != 'template.php' && $url_file != 'app.php' && $url_file != 'info.php' && $url_file != 'category.php' &&
            substr($now_suffix, 0, strlen($url_suffix)) == $url_suffix
            ) return true;
    return false;
} 

/**
 * 获取某个文件夹下面的所有文件
 * 
 * @param  $dir 某个文件夹所在的路径
 * @return array 
 */
function get_file_list($dir) {
    $files = array();
    if(!file_exists($dir)) return $files;
	$key = 0;
    if(!file_exists($dir)) return $files;
    if ($handle = opendir($dir)) {
        while (($file = readdir($handle)) !== false) {
            $key++;
            if ($file != ".." && $file != ".") {
                if (is_dir($dir . "/" . $file)) {
                    // if($file=="css" ) continue;
                    // $files[$file] = get_file_list($dir . "/" . $file);
                } else {
                    $files[$key]['name'] = $file;
                    $files[$key]['size'] = filesize($dir . "/" . $file);
                    $files[$key]['update_time'] = filemtime($dir . "/" . $file); ;
                } 
            } 
        } 
        closedir($handle);
        return $files;
    } 
} 
/**
 * 获取某个路径的文件夹和文件的名称
 * 
 * @param  $dir 某个文件夹所在的路径
 * @return array 
 */
function get_file_names($dir){
    $files=array();
    if (is_dir($dir)){
        if ($dh = opendir($dir)){
            while (($file = readdir($dh)) !== false){
                if($file != ".." && $file != ".")
                $files[]=$file;
            }
            closedir($dh);
        }
    }
    return $files;
}

function set_config($name, $value, &$file) {
    global $page;
    if(!isset($value)) $value='';
    if (preg_match('~"' . $name . '"~', $file)) {
        $file = preg_replace('~define\("' . $name . '"\s*,\s*"(.*)"\)\s*;~i', 'define("' . $name . '", "' . $value . '");', $file, 1);
    } else {
        $file .= chr(13) . chr(10) . 'define("' . $name . '","'.$value.'");';
    } 
} 

// 获取某分类下的所有父分类 不是树状
function get_cate_parent($cates, $cateid, $i = 0) {
    static $res = array();
    if (empty($i)) $res = array();
    foreach($cates as $v) {
        if ($cateid == $v['cate_id'] && $v['cate_id'] != 0) {
            array_push($res, $v['parent_id']);
            $i++;
            get_cate_parent($cates, $v['parent_id'], $i);
        } 
    } 
    return $res;
} 
/**
 * 更新关系表的排序字段
 * 
 * @params 'type'=>'' 要修改的资讯或者应用0=应用1=资讯
 * @params 'id'=>'' 要修改的资讯或者应用ID
 * @params 'field'=>'' 要修改的字段值
 */
function update_relation_fields($params) {
    global $dbm;
    $type = isset($params['type'])?$params['type']:0;
    $id = isset($params['id'])?$params['id']:'';
    if ($id == '') return array('sql' => '', 'error' => '更新失败，插入id为空', 'sql_time' => 0);
    $where = " id=" . $id . " and type=" . $type . " ";
    $fields = isset($params['field'])?$params['field']:'';
    if ($id == '') return array('sql' => '', 'error' => '更新失败，插入字段为空', 'sql_time' => 0);
    $table_name = "" . TB_PREFIX . "cate_relation ";
    $res = $dbm -> single_update($table_name, $fields, $where); 
    // exit(print_r($res));
    return $res;
} 

?>