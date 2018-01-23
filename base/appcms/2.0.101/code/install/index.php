<?php
/**
 * AppCMS: (C)2013  
 * AppCMS is a free open source mobile phone APP application download website content management system.
 * Custom development, production BUG report template, please contact loyjers@qq.com
 * Author: loyjers
 * Editor: loyjers
 * Create Date: 2013.05.12
 * File version: 2013.05.13.0001
 */
// 加载核心类
@header('Content-type: text/html; charset=UTF-8');
require_once(dirname(__FILE__) . '/../core/init.php');
// 初始化
$page['get'] = $_GET;
$page['post'] = $_POST;
$step = isset($page['get']['step']) && $page['get']['step'] != ''?(int)$page['get']['step']:1;

/**
 * *****************开始自定义代码************************
 */
// 判断php版本
if (phpversion() < '5.0.0') exit('您的php版本太低了 我们不支持');
// 检查是否已经安装过
if (file_exists(dirname(__FILE__) . '/install.lock.php'))exit('您已经安装过appcms了');
// 安装开始
switch ($step) {
    case 1 : // 加载安装协议书
        
        include_once(dirname(__FILE__) . '/templates/step' . $step . '.php');
        break;

    case 2 :// 环境变量 
        // 判断gd库是否加载
        $PHP_GD = '';
        if (extension_loaded('gd')) {
            if (function_exists('imagepng')) $PHP_GD .= 'png';
            if (function_exists('imagejpeg')) $PHP_GD .= ' jpg';
            if (function_exists('imagegif')) $PHP_GD .= ' gif';
        } 
        // 判断是否支持json
        $PHP_JSON = '0';
        if (extension_loaded('json')) {
            if (function_exists('json_decode') && function_exists('json_encode')) $PHP_JSON = '1';
        } 

        $PHP_DNS = preg_match("/^[0-9.]{7,15}$/", @gethostbyname('www.baidu.com')) ? 1 : 0; 
        // 是否满足appcms安装需求
        $is_right = (phpversion() >= '5.0.0' && extension_loaded('mysql') && $PHP_JSON && $PHP_GD) ? 1 : 0;

        include_once(dirname(__FILE__) . '/templates/step' . $step . '.php');
        break;

    case 3:// 判断文件的权限
        /**
         * 1.cache/ 是否可写
         * 2.当前文件是否可写
         * 3.配置文件是否可写
         */
		 /*
		 1，根目录有创建目录的权限（cache）
		 2，cache 目录有没有写入文件的权限
		 3，upload 目录要有写入文件的权限
		 4，core 目录的2个文件(config.conn.php,config.php)要有写入权限
		 5，install 目录有没有写权限 install.lock.php
		 6，根目录的 .htaccess 和 httpd.ini 要有写权限
		 */
		 /*
        $dirs = array('网站根目录' => array('path' => $_SERVER['DOCUMENT_ROOT'], 'iswrite' => 2),
            'cache/' => array('path' => dirname(__FILE__) . '/../cache', 'iswrite' => 0),
            'upload/' => array('path' => dirname(__FILE__) . '/../upload', 'iswrite' => 0),
            '数据库配置文件' => array('path' => dirname(__FILE__) . '/../core/config.conn.php', 'iswrite' => 0),
			'网站配置文件' => array('path' => dirname(__FILE__) . '/../core/config.php', 'iswrite' => 0)
            );
		*/
        if (!is_dir(dirname(__FILE__) . '/../cache')) {
            mkdir(dirname(__FILE__) . '/../cache');
        } 
        $is_right = 1; //需要确认那些文件是必须有读写权限的 再做判断
		/*
        foreach($dirs as $k => $v) {
            if (is_dir($v['path'])) {
                $iswrite = @fopen($v['path'] . '/dd.txt', 'w');
                if ($iswrite) {
                    $dirs[$k]['iswrite'] = 1;
                    fclose($iswrite);
                    unlink($v['path'] . '/dd.txt');
                }
            } else {
                if (is_writeable($v['path'])) {
                    $v['iswrite'] = 1;
                    $dirs[$k]['iswrite'] = 1;
                } else {
                    if ($k == '数据库配置文件' || $k == '网站配置文件') $is_right = 0;
                } 
            } 
        } */
		$dir = get_file_names('../');
		array_unshift($dir,'./');
		$dirs = array();
		foreach ($dir as $k => $v) {
			$dirs[$v] = array('path' => dirname(__FILE__) . '/../'.$v,'iswrite'  => 0);
		}
		$dirs['数据库配置文件'] = array('path' => dirname(__FILE__) . '/../core/config.conn.php', 'iswrite' => 0);
		$dirs['网站配置文件']   = array('path' => dirname(__FILE__) . '/../core/config.php', 'iswrite' => 0);
		foreach ($dirs as $key => $val) {
			if (is_dir($val['path'])) {
				$iswrite = @fopen($val['path'] . '/dd.txt', 'w');
				if ($iswrite) {
					$dirs[$key]['iswrite'] = 1;
					fclose($iswrite);
					unlink($val['path'] . '/dd.txt');
				} else {
					$is_right = 0;
				}
			} else {
			
				if (is_writeable($val['path'])) {
					$val['iswrite'] = 1;
					$dirs[$key]['iswrite'] = 1;
				} else {
					 if ($key == '数据库配置文件' || $key == '网站配置文件') $is_right = 0;
				}
			}
		
		}
		
        include_once(dirname(__FILE__) . '/templates/step' . $step . '.php');
        break;

    case 4 :// 数据库信息设置
        include_once(dirname(__FILE__) . '/templates/step' . $step . '.php');
        break;

    case 5 :// 创建表 初始化基本数据
        $isok = ''; //安装完成判断变量
        $host = isset($page['get']['host']) && $page['get']['host'] != ''?trim($page['get']['host']):'127.0.0.1';
        $dbuname = isset($page['get']['dbuname']) && $page['get']['dbuname'] != ''?trim($page['get']['dbuname']):'root';
        $dbpass = isset($page['get']['dbpass']) && $page['get']['dbpass'] != ''?trim($page['get']['dbpass']):'';
        $dbname = isset($page['get']['dbname']) && $page['get']['dbname'] != ''?trim($page['get']['dbname']):'appcms';
        $dbprefix = isset($page['get']['dbprefix']) && $page['get']['dbprefix'] != ''?trim($page['get']['dbprefix']):'appcms_';
        $dbcharset = isset($page['get']['dbcharset']) && $page['get']['dbcharset'] != ''?trim($page['get']['dbcharset']):'utf8';
        $aduname = isset($page['get']['aduname']) && $page['get']['aduname'] != ''?trim($page['get']['aduname']):'admin';
        $adpass = isset($page['get']['adpass']) && $page['get']['adpass'] != ''?helper :: password_encrypt(trim($page['get']['adpass'])):'77e2edcc9b40441200e31dc57dbb8829'; //后台加密方式
        $safecode = isset($page['get']['safecode']) && $page['get']['safecode'] != ''?trim($page['get']['safecode']):''; //后台登录安全码

        try {
            // 把需改过的配置 写入到 conf.conn.php
            $confile = dirname(__FILE__) . '/../core/config.conn.php';
           //跟换表前缀
		    if (!file_exists(dirname(__FILE__) . '/../data/data.sql')) die('数据库文件不存在！');
		    $sqlfile=file_get_contents(dirname(__FILE__) . '/../data/data.sql');
			$newdata=preg_replace("~(create table.*?)(appcms_)(.*?\()~","$1$dbprefix$3",$sqlfile);
			 if (!file_exists(dirname(__FILE__) . '/../data/data_init.sql')) die('数据库文件不存在！');
			$sqlfile=file_get_contents(dirname(__FILE__) . '/../data/data_init.sql');
			$newinitdata=str_replace('appcms_',$dbprefix,$sqlfile);
			$c = file_get_contents($confile); 
            // preg_match_all('/define\(\'[\w]+\',\s{0,}\'([\w\d\.-]+)\'\)/',$c,$match);
            // 有待优化
            $c = preg_replace('/define\(\'DB_HOST\',\s{0,}\'([\w\d\.-]+)\'\)/i', "define('DB_HOST', '" . $host . "')", $c, 1);
            $c = preg_replace('/define\(\'DB_USERNAME\',\s{0,}\'([\w\d\.-]+)\'\)/i', "define('DB_USERNAME', '" . $dbuname . "')", $c, 1);
            $c = preg_replace('/define\(\'DB_PASS\',\s{0,}\'([\w\d\.-]+)\'\)/i', "define('DB_PASS', '" . $dbpass . "')", $c, 1);
            $c = preg_replace('/define\(\'DB_DBNAME\',\s{0,}\'([\w\d\.-]+)\'\)/i', "define('DB_DBNAME', '" . $dbname . "')", $c, 1);
            $c = preg_replace('/define\(\'DB_CHARSET\',\s{0,}\'([\w\d\.-]+)\'\)/i', "define('DB_CHARSET', '" . $dbcharset . "')", $c, 1);
            $c = preg_replace('/define\(\'TB_PREFIX\',\s{0,}\'([\w\d\.-]+)\'\)/i', "define('TB_PREFIX', '" . $dbprefix . "')", $c, 1);
            file_put_contents($confile, $c); 
            // 后台登录验证码
            $r = file_get_contents(dirname(__FILE__) . '/../core/config.php'); 
            // 上传安全码
            $str = '@-_A0B1C3DEFG4HIJK5L6MNO8PQRS7TUVWX9YZabc1de3fg2hi5jkl4mnop7qrst8uvwxyz';
            $len = strlen($str);
            $code = '';
            for($i = 0;$i < 6;$i++) {
                $start = rand(0, $len-1);
                $code .= substr($str, $start, 1);
            } 
            $uplode_code = $code;
            $key = '';
            for($i = 0;$i < 6;$i++) {
                $start = rand(0, $len-1);
                $key .= substr($str, $start, 1);
            } 
            $uplode_key = $key; 
            // 安装目录
            $flie = dirname(__FILE__);
            $pos = strrpos($flie, "\\");
            $s = str_replace('\\', '/', substr($flie, 0, $pos + 1));
            $len = strlen($_SERVER['DOCUMENT_ROOT']);
            $sitepath = substr($s, $len);
            if($sitepath=='') $sitepath='/';
            if(substr($sitepath,0,1)!='/') $sitepath='/'.$sitepath;
            //die($sitepath);
            $r = preg_replace('/define\(\"SITE_SAFECODE\"\s{0,},\s{0,}\"(.*?)\"\)/i', 'define("SITE_SAFECODE", "' . $safecode . '")', $r, 1);
            $r = preg_replace('/define\(\"SITE_PATH\"\s{0,},\s{0,}\"(.*?)\"\)/i', 'define("SITE_PATH", "' . $sitepath . '")', $r, 1);
            $r = preg_replace('/define\(\"UPLOAD_CODE\"\s{0,},\s{0,}\"(.*?)\"\)/i', 'define("UPLOAD_CODE", "' . $uplode_code . '")', $r, 1);
            $r = preg_replace('/define\(\"UPLOAD_KEY\"\s{0,},\s{0,}\"(.*?)\"\)/i', 'define("UPLOAD_KEY", "' . $uplode_key . '")', $r, 1);
            file_put_contents(dirname(__FILE__) . '/../core/config.php', $r); 
            // 整理sql语句 初始化数据
            $sqls = get_sqls($newdata);
            $inits = get_sqls($newinitdata, 2);

            if (!@mysql_connect($host, $dbuname, $dbpass)) die("数据库连接失败");
            if (@mysql_select_db($dbname)) {
                //if (!@mysql_query("DROP DATABASE " . $dbname . "")) die("删除数据库失败"); //删除原有的数据库
            } else{
                if (!@mysql_query(" CREATE DATABASE " . $dbname . " ")) die("创建数据库失败");
            }
            mysql_query("SET NAMES " . $dbcharset . " ");
            mysql_select_db($dbname); 
            // 循环执行sql
            foreach($sqls as $v) {
                if (!mysql_query($v)) {
                    echo mysql_error();
                } 
            } 
            // 执行初始化数据
            foreach($inits as $v) {
                if (!mysql_query($v)) {
                    echo mysql_error();
                } 
            } 
            // 初始化管理员信息
            $sql="INSERT ignore INTO ".$dbprefix."admin_list (uid,uname,upass,ustate) VALUES(1,'" . $aduname . "','" . $adpass . "',0)";//echo($sql);
            mysql_query($sql);
            $r = fopen(dirname(__FILE__) . '/install.lock.php', 'w+'); //安装后创建
            fclose($r);
            $isok = 1;
        } 
        catch(Exception $e) {
            mysql_query("DROP DATABASE " . $dbname . "");
            unlink(dirname(__FILE__) . '/install.lock.php');
            $isok = 0;
            echo $e -> getMessage();
            die('数据加载失败');
        } 
        // 重复安装时应该删除缓存文件
        if (file_exists(dirname(__FILE__) . '/../cache/url_config')) unlink(dirname(__FILE__) . '/../cache/url_config');
        if (file_exists(dirname(__FILE__) . '/../cache/categories')) unlink(dirname(__FILE__) . '/../cache/categories');
        if (file_exists(dirname(__FILE__) . '/../cache/cate_bind')) unlink(dirname(__FILE__) . '/../cache/cate_bind');
        if (file_exists(dirname(__FILE__) . '/../cache/flag_auto_down')) unlink(dirname(__FILE__) . '/../cache/flag_auto_down');
        if (file_exists(dirname(__FILE__) . '/../cache/flag_all_down')) unlink(dirname(__FILE__) . '/../cache/flag_all_down');
        if (file_exists(dirname(__FILE__) . '/../cache/logs_update')) unlink(dirname(__FILE__) . '/../cache/logs_update');

        include_once(dirname(__FILE__) . '/templates/step' . $step . '.php');
        break;

    case 10:// 测试数据库是否能连接{"host":host,"dbuname":dbuname,"dbpass":dbpass,"dbprefix":dbprefix,"dbcharset":dbcharset,}
        $host = isset($page['get']['host']) && $page['get']['host'] != ''?trim($page['get']['host']):'127.0.0.1';
        $dbuname = isset($page['get']['dbuname']) && $page['get']['dbuname'] != ''?trim($page['get']['dbuname']):'root';
        $dbpass = isset($page['get']['dbpass']) && $page['get']['dbpass'] != ''?trim($page['get']['dbpass']):'';
        $dbname = isset($page['get']['dbname']) && $page['get']['dbname'] != ''?trim($page['get']['dbname']):'appcms';
        $dbprefix = isset($page['get']['dbprefix']) && $page['get']['dbprefix'] != ''?trim($page['get']['dbprefix']):'appcms_'; 
        $aduname = isset($page['get']['aduname']) && $page['get']['aduname'] != ''?trim($page['get']['aduname']):'admin';
        $adpass = isset($page['get']['adpass']) && $page['get']['adpass'] != '' ? helper :: password_encrypt(trim($page['get']['adpass'])) : helper :: password_encrypt(''); //后台加密方式
		// 此处不必做空值判断
        if (!@mysql_connect($host, $dbuname, $dbpass)) die('{"code":"1","msg":"不能连接数据库"}'); //不能连接数据库
        if (!@mysql_select_db($dbname)) {
            if (!@mysql_query("CREATE DATABASE " . $dbname . " ")) die('{"code":"2","msg":"不能创建数据库"}'); //不能创建数据库
        } else {
            //die('{"code":"3","msg":"数据库已经存在如果安装就请换一个名字"}'); //数据库已经存在如果安装就请换一个名字
        } ;
		//验证用户名和密码
		if (empty($aduname)) die('{"code":"210","msg":"账号名不能为空"}');
        if (empty($page['get']['adpass'])) die('{"code":"220","msg":"密码不能为空"}');
		if (strlen($page['get']['adpass']) < 5) die('{"code":"240","msg":"密码过于简单"}');
        if (strlen($page['get']['adpass']) > 20) die('{"code":"250","msg":"密码超出限定长度"}');
		if(!preg_match('~^[A-Za-z][A-Za-z]*[a-z0-9_]*$~', $aduname)){
			die('{"code":"230","msg":"账号名必须以字母开头，只允许字母、数字、下划线"}'); //用户名规则不符合	       
		}
        die('{"code":"0","msg":"测试成功"}'); //测试成功
        break;
		
    default:
        break;
} 

function get_sqls($sqlf, $type = 1) {
   /* if (!file_exists($sqlfile)) die('数据库文件不存在！');
    $sqlf = file_get_contents($sqlfile); */
    $sqlf = preg_replace('/\/[\*]+.*?[\*]+\//', '', $sqlf); //删除注释
    if ($type == 1) {
        $sqlf = preg_replace('~(\n)|(\r)~', '', $sqlf);
        $sqll = explode(';', $sqlf);
    } else {
        $sqlf = preg_replace('~(\n)|(\r)~', '@#@', $sqlf);
        $sqll = explode('@#@', $sqlf);
    } 
    $sqls = array();
    foreach($sqll as $k => $v) {
        if ($v == '') continue;
        $sqls[] = $v;
    } 
    return $sqls;
} 

/**
 * *****************结束自定义代码************************
 */
?>