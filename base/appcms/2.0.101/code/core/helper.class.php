<?php
/**
 * MCMS Copyright (c) 2012-2013 ZhangYiYeTai Inc.
 * The program developed by loyjers core architecture, individual all rights reserved, if you have any questions please contact loyjers@126.com
 */
class helper {
    /**
     * 可逆加密解密的公钥，不能出现重复字符，内有A-Z,a-z,0-9,/,=,+,_,-
     */
    public static $lockstream = 'st=lDEFABCkVWXYZabc89LMmGH012345uvdefIJK6NOPyzghijQRSTUwx7nopqr';

    /**
     * 中文字符截取
     *
     * @param  $str 要截取字符串
     * @param  $start 开始位置
     * @param  $length 长度
     */
    public static function utf8_substr($str, $start, $length) {
        if (function_exists('mb_substr')) {
            return mb_substr($str, $start, $length, 'UTF-8');
        }
        preg_match_all("/./u", $str, $arr);
        return implode("", array_slice($arr[0], $start, $length));
    }

    /**
     * 三次重试，获取指定url的内容
     *
     * @param  $url URL地址或者本地文件物理地址
     * @param  $charset 文件编码
     */
    public static function get_contents($url, $charset = 'UTF-8') {
        $retry = 3;
        $content = '';
        while (empty($content) && $retry > 0) {
            $content = @file_get_contents($url);
            $retry--;
        }
        if (strtoupper($charset) != 'UTF-8') $content = iconv($charset . "//IGNORE", "UTF-8", $content); //die($contents);
        return $content;
    }

    /**
     * 获取客户端IP地址
     */
    public static function getip() {
        $onlineip = '';
        if (getenv('HTTP_CLIENT_IP') && strcasecmp(getenv('HTTP_CLIENT_IP'), 'unknown')) {
            $onlineip = getenv('HTTP_CLIENT_IP');
        } elseif (getenv('REMOTE_ADDR') && strcasecmp(getenv('REMOTE_ADDR'), 'unknown')) {
            $onlineip = getenv('REMOTE_ADDR');
        } elseif (isset($_SERVER['REMOTE_ADDR']) && $_SERVER['REMOTE_ADDR'] && strcasecmp($_SERVER['REMOTE_ADDR'], 'unknown')) {
            $onlineip = $_SERVER['REMOTE_ADDR'];
        }
        return $onlineip;
    }

    /**
     * 获取某个文件夹下面的所有文件
     *
     * @param  $dir 某个文件夹所在的路径
     * @return array
     */
    public static function get_dir_files($dir) {
        $files = array();
        if (!file_exists($dir)) return $files;
        $key = 0;
        if ($handle = opendir($dir)) {
            while (($file = readdir($handle)) !== false) {
                $key++;
                if ($file != ".." && $file != ".") {
                    if (is_dir($dir . "/" . $file)) {

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
     * 如果不存在，则根据传入目录自动创建多级目录
     *
     * @param  $dir 目录
     */
    public static function mkdirs($dir) {
        if (!is_dir($dir)) {
            if (!helper :: mkdirs(dirname($dir))) {
                return false;
            }
            if (!mkdir($dir, 0777)) {
                return false;
            }
        }
        return true;
    }

    /**
     * 一般文件缓存
     *
     * @param  $cache_name 缓存名称
     * @param  $content 缓存内容
     * @param  $cache_time 缓存时间，默认3600秒=1小时
     * @param  $cache_path 缓存目录，默认为/cache
     */
    public static function file_cache($cache_name, $content = '', $cache_time = 3600, $cache_path = 'cache') {
        $cache_path = $cache_path;
        $file_content = '';
        // 创建缓存目录，以网站根目录为起始位置
        $cache_path = $_SERVER["DOCUMENT_ROOT"] . $cache_path;
        $file_path = $cache_path . '/' . $cache_name;
        helper :: mkdirs($cache_path);
        // 如果没传入内容，则读取缓存
        if ($content == '') {
            // echo('读缓存');
            if (file_exists($file_path) && time() - filemtime($file_path) < $cache_time) {
                $file_contents = file_get_contents($file_path);
                return unserialize($file_contents);
            } else {
                return 'timeout';
            }
            // 否则是强制写缓存
        } else {
            // echo('写缓存');
            $fp = fopen($file_path, 'w');
            $content = serialize($content);
            fwrite($fp, $content);
            fclose($fp);
            return unserialize($content);
        }
    }
    /**
     * 二维数组排序
     *
     * @param  $arr 数组
     * @param  $keys 排序字段
     * @param  $type 升序降序
     */
    public static function array_sort($arr, $keys, $type = 'asc') {
        $keysvalue = $new_array = array();
        foreach ($arr as $k => $v) {
            $keysvalue[$k] = $v[$keys];
        }
        if ($type == 'asc') {
            asort($keysvalue);
        } else {
            arsort($keysvalue);
        }
        reset($keysvalue);
        foreach ($keysvalue as $k => $v) {
            $new_array[$k] = $arr[$k];
        }
        return $new_array;
    }
    /**
     * 将二维数组的某个字符串维度转换为数字
     *
     * @param  $arr 要转换的数组
     * @param  $col_name 要转换的列名
     */
    public static function ver_sort($arr, $col_name) {
        for($i = 0;$i < count($arr);$i++) {
            $arr[$i]['new_order_set'] = helper :: get_str_num($arr[$i][$col_name]);
        }
        // 排序
        $arr_tmp = helper :: array_sort($arr, 'new_order_set', 'desc');

        $ret = array();
        foreach($arr_tmp as $k => $v) {
            unset($v['new_order_set']);
            array_push($ret, $v);
        }
        return $ret;
    }
    /**
     * 根据时间戳返回距现在的秒，分钟，小时
     *
     * @param  $stamp 时间戳
     */
    public static function datef($stamp) {
        $time_add = time() - $stamp;
        if ($time_add < 60) return $time_add . ' 秒前';
        if ($time_add >= 60 and $time_add < 60 * 60) return intval($time_add / 60) . ' 分钟前';
        if ($time_add >= 60 * 60 and $time_add < 60 * 60 * 12) return intval($time_add / (60 * 60)) . ' 小时前';
        return date('Y-m-d', $stamp);
    }
    /**
     * 根据.号分段提取字符中数字，并转换为数字，支持4段格式 2.1.1.104
     *
     * @param  $str 字符串
     */
    public static function get_str_num($str) {
        $a = explode('.', $str);
        $i = 10000;
        $r = 0;
        foreach($a as $b) {
            if (is_numeric($b)) {
                $r += $i * $b;
            } else {
                if (strpos($b, '-') > 0) {
                    $b = substr($b, 0, strpos($b, '-'));
                }
                preg_match_all('~(\d+)~', $b, $c);
                $e = '';
                foreach($c[1] as $d) {
                    $e .= $d;
                }
                if ($e == '') $e = 0;
                $r += $i * $e;
            }
            $i = $i / 10;
            if ($i == 1) break;
        }
        return $r;
    }
    /**
     * 转换IP为真实地址
     *
     * @param  $ip IP地址
     */
    public static function convertip($ip) {
        global $default_charset;
        $return = '';
        if (preg_match("/^\d{1,3}\.\d{1,3}\.\d{1,3}\.\d{1,3}$/", $ip)) {
            $iparray = explode('.', $ip);
            if ($iparray[0] == 10 || $iparray[0] == 127 || ($iparray[0] == 192 && $iparray[1] == 168) || ($iparray[0] == 172 && ($iparray[1] >= 16 && $iparray[1] <= 31))) {
                $return = '- LAN';
            } elseif ($iparray[0] > 255 || $iparray[1] > 255 || $iparray[2] > 255 || $iparray[3] > 255) {
                $return = '- Invalid IP Address';
            } else {
                $tinyipfile = './../data/ipdata/tinyipdata.dat';
                $fullipfile = './../data/ipdata/wry.dat';
                if (@file_exists($tinyipfile)) { // die($tinyipfile);
                    $return = $this -> convertip_tiny($ip, $tinyipfile);
                } elseif (@file_exists($fullipfile)) { // die($fullipfile);
                    $return = $this -> convertip_full($ip, $fullipfile);
                } else {
                    $return = '';
                }
            }
        }
        return iconv($default_charset . "//IGNORE", "UTF-8", $return);
    }

    /**
     * 隐藏IP地址最后一位
     *
     * @param  $ip IP地址
     */
    public static function ip_hide($ip) {
        $t = strrpos($ip, ".");
        $sr = substr($ip, 0, $t);
        return $sr . ".*";
    }

    /**
     * 记录文本日志，如果根目录有 logs 目录才会记录
     *
     * @param  $logs_type 日志类型，日志文件名称
     * @param  $logs_txt 日志内容
     */
    public static function logs($logs_type, $logs_txt) {
        // 创建缓存目录
        if(!is_dir(dirname(__FILE__) . '/../logs/')) return;
        try {
            $fp = fopen(dirname(__FILE__) . '/../logs/' . $logs_type . '_' . date('Y-m-d') . '.log', 'a');
            fwrite($fp, date('Y-m-d H:i:s') . ' ' . helper :: getip() . ' ' . $logs_txt . ' ' . chr(10));
            fclose($fp);
        }
        catch(Exception $e) {
            echo($e -> getMessage());
        }
    }

    /**
     * URL定时重定向
     */
    public static function redirect($url, $time = 0, $msg = '') {
        if (empty($msg)) $msg = "系统将在 {$time} 秒之后自动跳转到 {$url}";
        $str = '<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />';
        $str .= '<script type="text/javascript">setTimeout(function(){window.location.href="' . $url . '";},' . ($time * 1000) . ');</script>';
        if ($time != 0) $str .= $msg;
        exit($str);
    }

    /**
     * 分页条输出，兼容确定量参数和不确定量
     *
     * @param  $params =array('total'=>0,'pagesize'=>20,'rewrite'=>0,'rule'=>array('node'=>'','obj'=>'','params'=>''));
     * @param total $ 总记录数 必填
     * @param pagesize $ 分页大小，必填
     * @param rewrite $ 0=动态地址，适用于参数数量确定的情况； 1=伪静态地址，首页去除 p 参数； 2=常规动态地址，适用于参数数量不确定的情况，如果rewrite 为 2，则只需要写 total 和 pagesize 和 rewrite 3个参数
     * @param  $rule -> node 伪静态规则节点
     * @param  $rule -> obj 伪静态实例对象
     * @param  $rule -> params 确定的参数值对数组
     */
    public static function pagebar($params) { // print_r($params);
        if (!is_array($params)) return '分页码HTML函数参数调用错误，必须是数组';
        if (!isset($params['total'])) $params['total'] = 0;
        if (!isset($params['pagesize'])) $params['pagesize'] = 10;
        if (!isset($params['rewrite'])) $params['rewrite'] = 0; //print_r($params);
        // if($params['total']==0) return '';
        if (isset($params['rule'])) {
            $rewrite_obj = $params['rule']['obj']; //URL重写对象
            $node = $rewrite_obj -> url_config[$params['rule']['node']]; //当前节点
            $node_params = $params['rule']['params'];
            // print_r($node_params);
        }
        // 如果rewrite模式为2，则直接返回常规动态分页，否则继续过滤分页参数后的分页
        // print_r($params);die();
        if ($params['rewrite'] == 2) return helper :: pagehtml($params['total'], $params['pagesize']);

        $params['rule']['obj'] -> rewrite = $params['rewrite'];

        $page_name = 'p'; //分页参数名称
        $_page = !isset($_GET[$page_name]) || intval($_GET[$page_name]) == 0?1:intval($_GET[$page_name]);
        $_pages = ceil($params['total'] / $params['pagesize']); //计算总分页
        $_html = '';
        // 判断循环起止数字
        if ($_pages <= 6) {
            $_pstart = 1;
            $_pend = $_pages;
        } else {
            if ($_page <= 4) {
                $_pstart = 1;
                $_pend = 6;
            } else {
                $_pstart = $_page-3;
                if ($_pstart <= 0) $_pstart = $_page-1;
                if ($_pstart <= 0) $_pstart = $_page;
                $_pend = $_pstart + 5;
                if ($_pend > $_pages) $_pend = $_pages;
            }
        }
        // 第一页导航
        if ($_pstart > 1) {
            $node_params[$page_name] = 1;
            $_nurl = $params['rule']['obj'] -> encode($params['rule']['node'] . '_index', $node_params);
            $_html .= '<a href="' . $_nurl . '">1...</a>';
        }
        // 中间循环
        for($_i = $_pstart;$_i <= $_pend;$_i++) {
            if ($_i == $_page) {
                $_html .= '<span class="now_class">' . $_i . '</span>';
            } else {
                $node_params[$page_name] = $_i;
                if ($_i == 1) {
                    $_nurl = $params['rule']['obj'] -> encode($params['rule']['node'] . '_index', $node_params);
                } else {
                    $_nurl = $params['rule']['obj'] -> encode($params['rule']['node'], $node_params);
                }
                $_html .= '<a href="' . $_nurl . '">' . $_i . '</a>';
            }
        }
        // 最末页导航
        if ($_page < $_pages && $_pend < $_pages) {
            $node_params[$page_name] = $_pages;
            $_nurl = $params['rule']['obj'] -> encode($params['rule']['node'], $node_params);
            $_html .= '<a href="' . $_nurl . '">' . $_pages . '</a></span>';
        }
        if ($_pages == 0) $_page = 0;
        $_html .= '<span class="ptpage"> ' . $_page . '/' . $_pages . ' 页</span>';
        $pagevars = array('totalpage' => $_pages, 'pagecode' => $_html, 'pagesize' => $params['pagesize'], 'total' => $params['total'], 'offset' => ($_page-1) * $params['pagesize']);
        // print_r($pagevars);
        return $pagevars;
    }

    /**
     * 常规分页条，私有方法
     *
     * @param  $_total 记录总数
     * @param  $_pagesize 分页大小
     */
    private static function pagehtml($_total = 0, $_pagesize = 20) {
        // die($_total.$_pagesize);
        // if($_total==0) return '';
        $page_name = 'p'; //分页参数名称
        $_page = !isset($_GET[$page_name]) || intval($_GET[$page_name]) == 0?1:intval($_GET[$page_name]);

        $_pagesize = intval($_pagesize);
        $_pages = ceil($_total / $_pagesize); //计算总分页
        $_html = '';
        // 判断循环起止数字
        if ($_pages <= 6) {
            $_pstart = 1;
            $_pend = $_pages;
        } else {
            if ($_page <= 4) {
                $_pstart = 1;
                $_pend = 6;
            } else {
                $_pstart = $_page-3;
                if ($_pstart <= 0) $_pstart = $_page-1;
                if ($_pstart <= 0) $_pstart = $_page;
                $_pend = $_pstart + 5;
                if ($_pend > $_pages) $_pend = $_pages;
            }
        }
        // 第一页导航
        if ($_pstart > 1) {
            $_nurl = helper :: url_replace(1, $page_name);
            $_html .= '<a href="' . $_nurl . '">1...</a>';
        }
        // 中间循环
        for($_i = $_pstart;$_i <= $_pend;$_i++) {
            if ($_i == $_page) {
                $_html .= '<span class="now_class">' . $_i . '</span>';
            } else {
                $_nurl = helper :: url_replace($_i, $page_name);
                $_html .= '<a href="' . $_nurl . '">' . $_i . '</a>';
            }
        }
        // 最末页导航
        if ($_page < $_pages && $_pend < $_pages) {
            $_nurl = helper :: url_replace($_pages, $page_name);
            $_html .= '<a href="' . $_nurl . '">' . $_pages . '</a></span>';
        }
        if ($_pages == 0) $_page = 0;
        $_html .= '<span class="ptpage"> ' . $_page . '/' . $_pages . ' 页</span>';
        $pagevars = array('totalpage' => $_pages, 'pagecode' => $_html, 'pagesize' => $_pagesize, 'total' => $_total, 'offset' => ($_page-1) * $_pagesize);
        return $pagevars;
    }

    /**
     * URL页码替换，仅用于常规分页条使用,私有方法
     *
     * @param  $p 页码
     * @param  $get 替换分页参数名
     */
    private static function url_replace($p, $get) {
        $url = $_SERVER['REQUEST_URI'];
        if (preg_match('~(' . $get . '=\d+)~', $url)) {
            return preg_replace('~(' . $get . '=\d+)~', '' . $get . '=' . $p, $url);
        } else if (!stristr($url, '?')) {
            return $url . '?' . $get . '=' . $p;
        } else {
            return $url . '&' . $get . '=' . $p;
        }
    }

    /**
     * 可逆加密
     *
     * @param  $txtStream 要加密的字符串
     * @param  $password 加密私钥=解密私钥
     */
    public static function encrypt($txtStream, $password) {
        // 随机找一个数字，并从密锁串中找到一个密锁值
        $lockstream=defined('LOCK_STREAM')?LOCK_STREAM:(self :: $lockstream);
        $lockLen = strlen($lockstream);
        $lockCount = rand(0, $lockLen-1);
        $randomLock = $lockstream[$lockCount];
        // 结合随机密锁值生成MD5后的密码
        $password = md5($password . $randomLock);
        // 开始对字符串加密
        $txtStream = base64_encode($txtStream);
        $tmpStream = '';
        $i = 0;
        $j = 0;
        $k = 0;
        for ($i = 0; $i < strlen($txtStream); $i++) {
            $k = $k == strlen($password) ? 0 : $k;
            $j = (strpos($lockstream, $txtStream[$i]) + $lockCount + ord($password[$k])) % ($lockLen);
            $tmpStream .= $lockstream[$j];
            $k++;
        }
        return $tmpStream . $randomLock;
    }

    /**
     * 可逆加密
     *
     * @param  $txtStream 要解密的字符串
     * @param  $password 解密私钥=加密私钥
     */
    public static function decrypt($txtStream, $password) {
        $lockstream=defined('LOCK_STREAM')?LOCK_STREAM:(self :: $lockstream);
        $lockLen = strlen($lockstream);
        // 获得字符串长度
        $txtLen = strlen($txtStream);
        // 截取随机密锁值
        $randomLock = $txtStream[$txtLen - 1];
        // 获得随机密码值的位置
        $lockCount = strpos($lockstream, $randomLock);
        // 结合随机密锁值生成MD5后的密码
        $password = md5($password . $randomLock);
        // 开始对字符串解密
        $txtStream = substr($txtStream, 0, $txtLen-1);
        $tmpStream = '';
        $i = 0;
        $j = 0;
        $k = 0;
        for ($i = 0; $i < strlen($txtStream); $i++) {
            $k = $k == strlen($password) ? 0 : $k;
            $j = strpos($lockstream, $txtStream[$i]) - $lockCount - ord($password[$k]);
            while ($j < 0) {
                $j = $j + ($lockLen);
            }
            $tmpStream .= $lockstream[$j];
            $k++;
        }
        return base64_decode($tmpStream);
    }

    /**
     * 返回时间，单位是毫秒 ms
     */
    public static function getmicrotime() {
        list($usec, $sec) = explode(" ", microtime());
        $tim = ((float)$usec + (float)$sec) * 1000;
        return $tim;
    }

    /**
     * 密码加密方式1
     *
     * @param  $string 要加密字符串
     */
    public static function password_encrypt($string) {
        $string = md5(md5(md5($string)));
        return $string;
    }

    /**
     * 密码加密方式2
     *
     * @param  $str 要加密字符串
     */
    public static function password_encrypt_net($str) {
        return helper :: md5_net(helper :: md5_net(helper :: md5_net($str)));
    }

    /**
     * .NET方式MD5加密用户密码
     *
     * @param  $str 要加密字符串
     */
    private static function md5_net($str) {
        $md5hex = md5($str);
        $len = strlen($md5hex) / 2;
        $md5raw = "";
        for($i = 0;$i < $len;$i++) {
            $md5raw = $md5raw . chr(hexdec(substr($md5hex, $i * 2, 2)));
        }
        $keyMd5 = base64_encode($md5raw);
        return $keyMd5;
    }
    /**
     * 输出带时间的状态
     *
     * @param  $str 状态字符串
     * @param  $type 0=直接输出，1=返回值
     */
    public static function print_state($str, $type = 0) {
        $state = date('Y-m-d H:i:s', time()) . ' - ' . $str . '<br>';
        if ($type == 0) {
            echo($state);
        }
        return $state;
    }

    /**
     * 字符串防SQL注入编码，对GET,POST,COOKIE的数据进行预处理
     *
     * @param  $str 要处理字符串
     * @param  $urlencode 是否要URL编码
     */
    public static function escape($str, $urldecode = 0) {
        if ($urldecode == 1) $str = urldecode($str);
        // PHP版本大于5.4.0，直接转义字符
        if (strnatcasecmp(PHP_VERSION, '5.4.0') >= 0) {
            $str = addslashes($str);
        } else {
            // 魔法转义没开启，自动加反斜杠
            if (!get_magic_quotes_gpc()) {
                $str = addslashes($str);
            }
        }
        return $str;
    }
    /**
     * 字符串去反斜杠处理，模板编辑源码时候需要使用
     *
     * @param  $str 反斜杠
     */
    public static function escape_stripslashes($str) {
        // PHP版本大于5.4.0，直接转义字符
        if (strnatcasecmp(PHP_VERSION, '5.4.0') < 0) {
            // 魔法转义没开启，自动加反斜杠
            if (get_magic_quotes_gpc()) {
                $str = stripslashes($str);
            }
        }
        return $str;
    }
    /**
     * 根据百度搜索结果自动提取关键词
     * @param $title 要提取关键词的标题
     * @param $filter_words1 精准过滤词数组 array('过滤词1','过滤词2')
     * @param $filetr_words2 模糊过滤词数组 array('过滤词1','过滤词2')
     */
    public static function get_tags_baidu($title,$filter_words1=array(),$filter_words2=array()) {
        if (strlen($title) <= 4) die('{"code":1,"msg":"","data":["' . $title . '"]}');
        $ret = helper :: get_contents('http://www.baidu.com/s?wd=' . urlencode($title));
        preg_match_all('~<em>(.*?)</em>~', $ret, $keys);
        //过滤字符
        for($i=0;$i<count($keys[1]);$i++){
            $keys[1][$i] = preg_replace('~"|\'|“|”|【|】|\(|\)|（|）|:|：|\-|—~','',$keys[1][$i]);
        }
        //去重并过滤
        $nkeys = array();
        foreach($keys[1] as $a) {
            //去重
            $is_key = 0;
            for($b = 0;$b < count($nkeys);$b++) {
                if ($a == $nkeys[$b]['k']) {
                    $is_key = 1;
                    $nkeys[$b]['t'] = 1 + $nkeys[$b]['t'];
                    break;
                }
            }
            //过滤
            $is_k1=0;
            foreach($filter_words1 as $b){
                if($b==$a) {$is_k1=1;break;}
            }
            $is_k2=0;
            foreach($filter_words2 as $b){
                if(strstr($a,$b)) {$is_k2=1;break;}
            }
            if ($is_key == 0 && $is_k1==0 && $is_k2==0) array_push($nkeys, array('k' => $a, 't' => 1,'l'=>strlen($a)));
        }
        //过滤字符长度
        $tags = array();
        for($i = 0;$i < count($nkeys);$i++) {
            if (strlen($nkeys[$i]['k']) >= 9 && strlen($nkeys[$i]['k']) <= 18) array_push($tags, $nkeys[$i]);
        }
        //排序
        $tags = helper :: array_sort($tags, 'l');//print_r($info_tags);
        $ntags=array();
        //重做数组
        foreach($tags as $a){
            array_push($ntags,$a);
        }

        return $ntags;
    }
    /**
     * 转换IP库1,私有方法
     */
    private static function convertip_tiny($ip, $ipdatafile) {
        static $fp = null, $offset = array(), $index = null;
        $ipdot = explode('.', $ip);
        $ip = pack('N', ip2long($ip));

        $ipdot[0] = (int)$ipdot[0];
        $ipdot[1] = (int)$ipdot[1];

        if ($fp === null && $fp = @fopen($ipdatafile, 'rb')) {
            $offset = @unpack('Nlen', @fread($fp, 4));
            $index = @fread($fp, $offset['len'] - 4);
        } elseif ($fp == false) {
            return '- Invalid IP data file';
        }

        $length = $offset['len'] - 1028;
        $start = @unpack('Vlen', $index[$ipdot[0] * 4] . $index[$ipdot[0] * 4 + 1] . $index[$ipdot[0] * 4 + 2] . $index[$ipdot[0] * 4 + 3]);

        for ($start = $start['len'] * 8 + 1024; $start < $length; $start += 8) {
            if ($index{$start} . $index{$start + 1} . $index{$start + 2} . $index{$start + 3} >= $ip) {
                $index_offset = @unpack('Vlen', $index{$start + 4} . $index{$start + 5} . $index{$start + 6} . "\x0");
                $index_length = @unpack('Clen', $index{$start + 7});
                break;
            }
        }

        @fseek($fp, $offset['len'] + $index_offset['len'] - 1024);
        if ($index_length['len']) {
            return '- ' . @fread($fp, $index_length['len']);
        } else {
            return '- Unknown';
        }
    }

    /**
     * 转换IP库2，私有方法
     */
    private static function convertip_full($ip, $ipdatafile) {
        if (!$fd = @fopen($ipdatafile, 'rb')) {
            return '- Invalid IP data file';
        }
        $ip = explode('.', $ip);
        $ipNum = $ip[0] * 16777216 + $ip[1] * 65536 + $ip[2] * 256 + $ip[3];

        if (!($DataBegin = fread($fd, 4)) || !($DataEnd = fread($fd, 4))) return;
        @$ipbegin = implode('', unpack('L', $DataBegin));
        if ($ipbegin < 0) $ipbegin += pow(2, 32);
        @$ipend = implode('', unpack('L', $DataEnd));
        if ($ipend < 0) $ipend += pow(2, 32);
        $ipAllNum = ($ipend - $ipbegin) / 7 + 1;

        $BeginNum = $ip2num = $ip1num = 0;
        $ipAddr1 = $ipAddr2 = '';
        $EndNum = $ipAllNum;

        while ($ip1num > $ipNum || $ip2num < $ipNum) {
            $Middle = intval(($EndNum + $BeginNum) / 2);

            fseek($fd, $ipbegin + 7 * $Middle);
            $ipData1 = fread($fd, 4);
            if (strlen($ipData1) < 4) {
                fclose($fd);
                return '- System Error';
            }
            $ip1num = implode('', unpack('L', $ipData1));
            if ($ip1num < 0) $ip1num += pow(2, 32);

            if ($ip1num > $ipNum) {
                $EndNum = $Middle;
                continue;
            }

            $DataSeek = fread($fd, 3);
            if (strlen($DataSeek) < 3) {
                fclose($fd);
                return '- System Error';
            }
            $DataSeek = implode('', unpack('L', $DataSeek . chr(0)));
            fseek($fd, $DataSeek);
            $ipData2 = fread($fd, 4);
            if (strlen($ipData2) < 4) {
                fclose($fd);
                return '- System Error';
            }
            $ip2num = implode('', unpack('L', $ipData2));
            if ($ip2num < 0) $ip2num += pow(2, 32);

            if ($ip2num < $ipNum) {
                if ($Middle == $BeginNum) {
                    fclose($fd);
                    return '- Unknown';
                }
                $BeginNum = $Middle;
            }
        }

        $ipFlag = fread($fd, 1);
        if ($ipFlag == chr(1)) {
            $ipSeek = fread($fd, 3);
            if (strlen($ipSeek) < 3) {
                fclose($fd);
                return '- System Error';
            }
            $ipSeek = implode('', unpack('L', $ipSeek . chr(0)));
            fseek($fd, $ipSeek);
            $ipFlag = fread($fd, 1);
        }

        if ($ipFlag == chr(2)) {
            $AddrSeek = fread($fd, 3);
            if (strlen($AddrSeek) < 3) {
                fclose($fd);
                return '- System Error';
            }
            $ipFlag = fread($fd, 1);
            if ($ipFlag == chr(2)) {
                $AddrSeek2 = fread($fd, 3);
                if (strlen($AddrSeek2) < 3) {
                    fclose($fd);
                    return '- System Error';
                }
                $AddrSeek2 = implode('', unpack('L', $AddrSeek2 . chr(0)));
                fseek($fd, $AddrSeek2);
            } else {
                fseek($fd, -1, SEEK_CUR);
            } while (($char = fread($fd, 1)) != chr(0))
            $ipAddr2 .= $char;

            $AddrSeek = implode('', unpack('L', $AddrSeek . chr(0)));
            fseek($fd, $AddrSeek);

            while (($char = fread($fd, 1)) != chr(0))
            $ipAddr1 .= $char;
        } else {
            fseek($fd, -1, SEEK_CUR);
            while (($char = fread($fd, 1)) != chr(0))
            $ipAddr1 .= $char;

            $ipFlag = fread($fd, 1);
            if ($ipFlag == chr(2)) {
                $AddrSeek2 = fread($fd, 3);
                if (strlen($AddrSeek2) < 3) {
                    fclose($fd);
                    return '- System Error';
                }
                $AddrSeek2 = implode('', unpack('L', $AddrSeek2 . chr(0)));
                fseek($fd, $AddrSeek2);
            } else {
                fseek($fd, -1, SEEK_CUR);
            } while (($char = fread($fd, 1)) != chr(0))
            $ipAddr2 .= $char;
        }
        fclose($fd);

        if (preg_match('/http/i', $ipAddr2)) {
            $ipAddr2 = '';
        }
        $ipaddr = "$ipAddr1 $ipAddr2";
        $ipaddr = preg_replace('/CZ88\.NET/is', '', $ipaddr);
        $ipaddr = preg_replace('/^\s*/is', '', $ipaddr);
        $ipaddr = preg_replace('/\s*$/is', '', $ipaddr);
        if (preg_match('/http/i', $ipaddr) || $ipaddr == '') {
            $ipaddr = '- Unknown';
        }
        return '- ' . $ipaddr;
    }
    /**
     * 检测来源是手机用户
     *
     * @return boolean true 是手机端  false 是其他终端
     */
    public static function from_mobile() {
        $regex_match = "/(nokia|iphone|android|motorola|^mot\-|softbank|foma|docomo|kddi|up\.browser|up\.link|";
        $regex_match .= "htc|dopod|blazer|netfront|helio|hosin|huawei|novarra|CoolPad|webos|techfaith|palmsource|meizu|miui|ucweb";
        $regex_match .= "blackberry|alcatel|amoi|ktouch|nexian|samsung|^sam\-|s[cg]h|^lge|ericsson|philips|sagem|wellcom|bunjalloo|maui|";
        $regex_match .= "symbian|smartphone|midp|wap|phone|windows ce|iemobile|^spice|^bird|^zte\-|longcos|pantech|gionee|^sie\-|portalmmm|";
        $regex_match .= "jig\s browser|hiptop|^ucweb|^benq|haier|^lct|opera\s*mobi|opera\*mini|320x320|240x320|176x220";
        $regex_match .= ")/i";

        if (isset($_SERVER['HTTP_X_WAP_PROFILE']) || isset($_SERVER['HTTP_PROFILE']) || (isset($_SERVER['HTTP_USER_AGENT']) && preg_match($regex_match, strtolower($_SERVER['HTTP_USER_AGENT'])))) {
            return true;
        }
        return false;
    }
	/**
	* 获取字体文件的名称
	* @param string array $FilePath  字体文件名（含路径）
	* @return array $names 读取到字体文件名称 失败则返回 false
	* 此函数在读取字体文件的名称时需要用到
	*/
	public static function get_font_name($FilePath) {
		$fp=fopen($FilePath,'r');
		if($fp){
			//从二进制字符串对数据进行解包
			$meta=unpack('n6',fread($fp,12));
			//检查是否是一个true type字体文件以及版本号是否为1.0
			if($meta[1] != 1 || $meta[2] != 0){
				return false;
			}
			$found = false;
			for($i=0; $i<$meta[3]; $i++){
				//TT_TABLE_DIRECTORY
				$tablemeta=unpack('N4',$data=fread($fp, 16));
				if(substr($data,0,4)=='name'){
					$found = TRUE;
					break;
				}
			}
			if($found){
				fseek($fp, $tablemeta[3]);
				//TT_NAME_TABLE_HEADER
				$tablecount=unpack('n3',fread($fp,6));
				$found = FALSE;
				for($i=0; $i<$tablecount[2]; $i++){
					//TT_NAME_RECORD
					$table=unpack('n6',fread($fp,12));
					if($table[4] == 1){
						$npos=ftell($fp);
						fseek($fp,$n=$tablemeta[3] + $tablecount[3] + $table[6], SEEK_SET);
						@$fontname=trim($x=fread($fp,$table[5]));
						//print_r($fontname);
							if(strlen($fontname) > 0){
							if($table[3]==1033){
								$code = 'utf-16le';
							}elseif($table[3]==2052){
								$code = 'utf-16be';
							}elseif($table[3]==1042){
								$code = 'utf-16';
							}else{
								$code = 'utf-8';
							}
							if($code != 'utf-8'){
								@$fontname = iconv($code,"UTF-8//TRANSLIT",$fontname);
							}
							$names[]=array(
								'platform'=>$table[1], //平台（操作系统）
								'language'=>$table[3], //字体名称的语言
								'encoding'=>$table[2], //字体名称的编码
								'name'=>$fontname //字体名称
							);
						}
						fseek($fp,$npos,SEEK_SET);
					}
				}
			}
			fclose($fp);
		}
		if(isset($names)){
			return $names;
		}else{
			return false;
		}
	}
}
?>