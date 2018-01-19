<?php
/**
 * AppCMS Copyright (c) 2012-2013  
 * AppCMS is a free open source mobile phone APP application download website content management system.
 * Custom development, production BUG report template, please contact loyjers@qq.com
 * Author: loyjers
 * Editor: loyjers
 * Create Date: 2013.05.25
 * Last Editor Date: 2013.05.25
 * File version: 2013.05.13.0001
 * 跳转到真实下载地址
 */

require_once(dirname(__FILE__) . "/core/init.php");
$dbm = new db_mysql(); 
// 计费包ID,数据中心应用ID
$cid = $aid = 0;
if (isset($_GET['hid'])) $_GET['id'] = $_GET['hid']; //兼容hid传参方式

if (isset($_GET['id']) && is_numeric($_GET['id'])) {
    // 本地应用下载跳转地址
    $hid_sql = "select app_id,down_url from " . TB_PREFIX . "app_history where history_id ='{$_GET['id']}'";
    $rs = $dbm -> query($hid_sql);
    if (count($rs['list']) > 0) {
        $down_url = $rs['list'][0]['down_url'];
        $appcms_history_id = $rs['list'][0]['appcms_history_id'];
        $app_id = $rs['list'][0]['app_id'];
        if(substr($down_url,0,7)=='http://' && $appcms_history_id==0){
            $sql = "update " . TB_PREFIX . "app_list set app_down=`app_down`+1 where app_id='" . $app_id . "'";
            $dbm -> query_update($sql); 
            // 更新关联表下载量
            $sql = "update " . TB_PREFIX . "cate_relation set id_down=`id_down`+1 where id='" . $app_id . "' and type ='0'";
            $dbm -> query_update($sql);
            header("Location:" . $down_url);
            die();
        }
    } 
    update_down($_GET['id']);
} else {
    die('参数错误');
} 
// 判断联盟ID是否有效
if (defined('UNION_ID') && UNION_ID != '' && UNION_ID != 0 && is_numeric(UNION_ID)) {
    $url = '&uid=' . UNION_ID;
} else {
    $url = '';
} 
// 判断计费包ID是否有效
if (isset($cid) && $cid != 0) {
    $url .= '&cid=' . $cid;
} 
if (isset($aid) && $aid > 0) {
    $url .= '&aid=' . $aid;
} 

header('HTTP/1.1 301 Moved Permanently');
header('Location:' . DATA_CENTER_URL . '/apk.php?hid=' . (isset($_GET['id'])?$_GET['id']:'') . $url);
// 更新下载量
function update_down($id) {
    global $cid, $aid, $dbm; 
    // $dbm = new db_mysql();
    // 根据app_history_id取应用app_id
    $app_id_sql = "select app_id from " . TB_PREFIX . "app_history where appcms_history_id='" . $id . "'";
    $app_id_res = $dbm -> query($app_id_sql);
    if (count($app_id_res['list']) <= 0) return;
    $app_id = $app_id_res['list'][0]['app_id']; 
    // 查询出计费包ID，数据中心应用ID
    $sqlbuf = "select data_app_id,charge_app_id from " . TB_PREFIX . "app_list where app_id ='" . $app_id . "'";
    $row = $dbm -> query($sqlbuf);
    if (count($row['list']) > 0) {
        $cid = $row['list'][0]['charge_app_id'];
        $aid = $row['list'][0]['data_app_id'];
    } 
    $sql = "update " . TB_PREFIX . "app_list set app_down=`app_down`+1 where app_id='" . $app_id . "'";
    $res = $dbm -> query_update($sql);
    /**
     * 更新关联表下载量
     */
    $sql = "update " . TB_PREFIX . "cate_relation set id_down=`id_down`+1 where id='" . $app_id . "' and type ='0'";
    $res = $dbm -> query_update($sql);
} 

?>