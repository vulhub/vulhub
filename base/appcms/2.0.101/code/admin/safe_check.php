<?php

/**
 * AppCMS Copyright (c) 2012-2013  
 * AppCMS is a free open source mobile phone APP application download website content management system.
 * Custom development, production BUG report template, please contact loyjers@qq.com
 * Author: crane
 * Editor: crane,loyjers
 */

require_once(dirname(__FILE__) . "/../core/init.php");

/**
 * 加载核心文件类和公用方法函数
 */

$time_start = helper :: getmicrotime();
/**
 * 开始时间
 */

$dbm = new db_mysql();
/**
 * 数据库类实例
 */

$page['get'] = $_GET;
/**
 * get参数的 m 和 ajax 参数是默认占用的，一个用来执行动作函数，一个用来判断是否启用模板还是直接输出JSON格式数据
 */
$page['post'] = $_POST;

/**
 * 判断用户是否登录
 */

/*需要过滤的方法名称或者是系统参数*/

$words = array('AUTH_CODE', 'SITE_SAFECODE', 'UPLOAD_CODE', 'UPLOAD_KEY', 'DB_HOST', 'DB_USERNAME', 'DB_PASS', 'DB_DBNAME', 'DB_CHARSET', 'TB_PREFIX', // 系统配置常量
    'SESSION.*?uname', 'SESSION.*?ustate', // 后台登录SESSION
    '[^&]copy', 'rename', 'delete', 'opendir', 'readdir', 'mkdir', 'rmdir', 'unlink', 'fscanf','fseek','fopen', 'fread', 'readfile', 'fpassthru', 'parse_ini_file', 'fget', 'get_contents', 'put_contents','fwrite', 'curl', // 文件操作读写
    'eval', 'get_defined_vars', 'get_defined_constants', 'phpinfo','del_','_del','edit', // 其他
    'mysql_connect', 'mysql_query', // 数据库操作函数
    '\$dbm', // 默认数据库操作对象
    );

$page['get']['m'] = isset($_GET['m'])?$_GET['m']:'list';
/**
 * 页面动作 model 分支选择，动作函数写在文件末尾，全部以前缀 m__ 开头
 */

if (function_exists("m__" . $page['get']['m'])) {
    call_user_func("m__" . $page['get']['m']);
} 

$time_end = helper :: getmicrotime();
/**
 * 主程序执行时间，如果要包含模板的输出时间，则可以调用该静态时间方法单独计算
 */

/**
 * 模板载入选择，模板页面为PHP+HTML混合编写，如果模板页面中也有区块函数，模板函数以 tpl__ 为前缀
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
 * 模板隐藏安全检测
只是匹配一些敏感函数
 */
function m__safe_check() {
    global $page, $words;
    $template = $page['get']['file'];
    $dir = dirname(__FILE__) . '/../templates/' . $template;
    $files = get_file_list($dir); 
    // exit(print_r($files));
    $saferes = ''; 
    // 遍历所有模板页
    foreach($files as $f) {
        $filecont = strtolower(helper :: get_contents($dir . "/" . $f['name']));
        /**
         * 判断模板文件中是否有敏感变量或方法名
         */
        foreach($words as $sa) {
            if (preg_match('~(<\?|<%)[\w\W]*?' . strtolower($sa) . '[\w\W]*?(%|\?>)~', $filecont)) {
                $saferes .= '模板文件 ' . $f['name'] . " 代码中有敏感字符 " . $sa . "，可能存在安全隐患\\r\\n";
            } 
        } 
        if (!empty($saferes)) {
            die('{"code":"100","msg":"' . $saferes . '请仔细检查模板中存在该字符的代码，本提示不保证绝对准确！"}');
        } 
        if (is_file($f['name'])) {
            /**
             * 判断是否包含其他文件
             */
            if (preg_match_all('~(require_once|include_once|require|include)(.*?)(;|\?>)~', $filecont, $arr)) {
                $arrs = $arr[2];
                foreach($arrs as $fileurl) {
                    $count1 = substr_count($fileurl, '"');
                    $count2 = substr_count($fileurl, "'");
                    if ($count1 > 2 || $count2 > 2) {
                        die('{"code":"100","msg":"模板 ' . $f['name'] . ' 的包含了其他目录的文件 ' . $fileurl . '，可能存在安全隐患"}');
                    } 
                    if (count(explode('/', $fileurl)) > 1) {
                        die('{"code":"100","msg":"模板 ' . $f['name'] . ' 的包含了其他目录的文件 ' . $fileurl . '，可能存在安全隐患"}');
                    } 
                } 
            } 
            /**
             * 判断是否又实例化对象（模板是不需要实例化任何东西的）
             */
            if (preg_match('~<\?[\w\W]*?new [\w\W]*?\?>~', $filecont)) {
                die('{"code":"100","msg":"模板文件 ' . $f['name'] . ' 实例化了一个对象，可能存在安全隐患"}');
            } 
        } 
        // echo $f['name'].'<br>';
    } 
    die('{"code":"0","msg":"我们不保证文件绝对安全"}');
} 


?>
