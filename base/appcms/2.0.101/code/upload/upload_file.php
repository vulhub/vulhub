<?php
/**
 * AppCMS: (C)2013  
 * AppCMS is a free open source mobile phone APP application download website content management system.
 * Custom development, production BUG report template, please contact loyjers@qq.com
 * Author: loyjers
 * Editor: loyjers
 * Create Date: 2013.05.13
 * File version: 2013.05.13.0001
 */

/**
 * 加载核心文件类和公用方法函数
 */
require_once(dirname(__FILE__) . "/../core/init.php");
require_once(dirname(__FILE__) . "/../core/img.class.php");
require_once(dirname(__FILE__) . "/../core/upload.class.php");
require_once(dirname(__FILE__) . "/../core/apk_parser.php");

$time_start = helper :: getmicrotime();

/**
 * get参数的 m 和 ajax 参数是默认占用的，一个用来执行动作函数，一个用来判断是否启用模板还是直接输出JSON格式数据
 */
$page['get'] = $_GET;
$page['post'] = $_POST;
$dbm = new db_mysql();

$params = $page['get']['params'];
/**
 * $params=json_encode(urldecode($params));
 * die('<script> alert('.$params.');</script>');
 */
$params = preg_replace('~(\\\")~', '"', $params);
$json_params = json_decode($params);
// 1.验证请求安全性
$verify = isset($page['get']['v'])?$page['get']['v']:'';
if ($verify == '') die('<script>alert("No Access 001");</script>');
$verify = helper :: decrypt($verify, UPLOAD_KEY);
$gsc = substr($verify, 0, strlen(UPLOAD_CODE));
if ($gsc != UPLOAD_CODE) die('<script>alert("No Access 002 ' . $gsc . '");</script>');
if (!preg_match('~(\d{10})~', substr($verify, strlen(UPLOAD_CODE)))) die('<script>alert("No Access 003' . $verify . '");</script>');
// 2.接收图片上传
$save_path = '.' . UPLOAD_PATH . date('Y/m/d') . '/';
$file_name=strtolower($_FILES['file']['name']);
if(strstr($file_name,'.apk')) {
    $save_path = '.' . (defined('UPLOAD_PATH_APK')?UPLOAD_PATH_APK:'/apk/') . date('Y/m/d') . '/';
}
$upload_config = array();
$upload_config['savePath'] = $save_path; //图片保存路径
$upload_config['allowExts'] = array('gif', 'jpg', 'jpeg', 'bmp', 'png','apk');
// 是否生成缩略图
if (isset($json_params -> thumb)) {
    $upload_config['thumb'] = true; //是否生成缩略图
    $upload_config['thumbMaxWidth'] = $json_params -> thumb -> width; // 缩略图最大宽度
    $upload_config['thumbMaxHeight'] = $json_params -> thumb -> height; // 缩略图最大高度
    $upload_config['thumbPrefix'] = 'thumb_'; // 缩略图前缀
    $upload_config['thumbPath'] = $save_path; // 缩略图保存路径
} 
/**
 * die('<script>alert(\''.json_encode($upload_config).'\');</script>');
 */
// 判断图片保存文件夹是否存在，不存在则创建
if (!is_dir($upload_config['savePath'])) helper :: mkdirs($upload_config['savePath']);
// 开始上传
$upload = new UploadFile($upload_config);
// 返回结果
$result = array();
if (!$upload -> upload()) {
    $result = $upload -> getErrorMsg();
} else {
    $result = $upload -> getUploadFileInfo();
} 
/**
 * die('<script>alert(\''.json_encode($result).'\');</script>');
 */
// 拼装回调参数
$files = array();
foreach($result as $k=>$f) {
    $img = Image :: getImageInfo($f['savepath'] . $f['savename']); //图片信息 
    // $resource_url=$file1['original']='http://'.$_SERVER['HTTP_HOST'].'/upload/'.substr($f['savepath'],2).$f['savename'];
    $resource_url = $file1['original'] = SITE_PATH . 'upload/' . substr($f['savepath'], 2) . $f['savename'];
    $r_size = $file1['size'] = $f['size'];
    $r_width = $file1['width'] = isset($img['width'])?$img['width']:0;
    $r_height = $file1['height'] = isset($img['height'])?$img['height']:0;
    
    switch ($f['extension']) {
        case 'zip':
        case 'rar':
            $resource_type = 3;
            break;
        case 'flv':
            $resource_type = 2;
            break;
        case 'apk':
            $resource_type = 4;
            //插入APK提取资源参数
            
            $p = new ApkParser();
            $res = $p->open(dirname(__FILE__).'/../'.$resource_url);
            $xml = $p->getXML();echo($xml);
            preg_match('~android:versionName="(.*?)"~',$xml,$matches);
            $version='';if(count($matches)>1){$version=$matches[1];}
            preg_match('~android:minSdkVersion="(.*?)"~',$xml,$matches);
            $minsys='';if(count($matches)>1){$minsys=get_android($matches[1]);}
            $version='';if(count($matches)>1){$version=$matches[1];}
            preg_match('~package="(.*?)"~',$xml,$matches);
            $packname='';if(count($matches)>1){$packname=$matches[1];}
            $file1['version']=$version;
            $file1['minsys']=$minsys;
            $file1['packname']=$packname;
            $file_size = $file1['size']/1024;
            if($file_size>1024){
                $file1['file_size']=round($file_size/1024,2).'M';
            }else{
                $file1['file_size']=round($file_size,2).'K';
            }
            $file1['server']=SITE_URL;
            break;
        default:
            $resource_type = 1;
    } 
    array_push($files, $file1);
    // 3.插入数据库
    $sql = "insert into " . TB_PREFIX . "resource_list(resource_url,width,height,size) values('$resource_url',$r_width,$r_height,$r_size)";
    /**
     * die('<script>alert(\''.$sql.'\');</script>');
     */
    $dbm -> query_insert($sql);
} 
//die('<script>alert(\''.json_encode($files).'\');</script>');
// 没得到数组，打印错误信息
if (count($files) <= 0) {
    die('<script>alert(\'' . json_encode($result) . '\');</script>');
} 
// 4.跳转到之前的域名进行参数回调
$params = urlencode($params);
die('<script>window.location.href="http://' . $json_params -> domain . SITE_PATH . 'upload/callback.php?params={\"params\":' . $params . ',\"files\":' . urlencode(json_encode($files)) . '}";</script>');

$time_end = helper :: getmicrotime();
$page['data_fill_time'] = $time_end - $time_start;

function get_android($i){
    $a=array(
        1=>'1.0',
        2=>'1.1',
        3=>'1.5',
        4=>'1.6',
        5=>'2.0',
        6=>'2.0.1',
        7=>'2.1.x',
        8=>'2.2.x',
        9=>'2.3.0-2.3.2',
        10=>'2.3.3,2.3.4',
        11=>'3.0.x',
        12=>'3.1.x',
        13=>'3.2',
        14=>'4.0,4.0.1,4.0.2',
        15=>'4.0.3,4.0.4',
        16=>'4.1,4.1.1',
        17=>'4.2',
    );
    foreach($a as $k=>$v){
        if($k==$i){return 'Android '.$v;}
    }
    return "";
}
?>