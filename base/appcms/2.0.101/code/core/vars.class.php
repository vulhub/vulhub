<?php
/**
 * AppCMS Copyright (c) 2012-2013  
 * AppCMS is a free open source mobile phone APP application download website content management system.
 * Custom development, production BUG report template, please contact loyjers@qq.com
 * 预定义词组
Author: crane
 * Editor: crane,loyjers,chenpeng

 */
class vars {
    /**
     * 变量列表
     */
    public static $fields = array(
        // 用户状态
        'ustate' => array(
            array('value' => '0', 'txt' => '正常', 'txt_color' => ''),
            array('value' => '1', 'txt' => '停用', 'txt_color' => ''),
            ), 
        // 栏目类型
        'catetype' => array(
            array('value' => '0', 'txt' => '应用', 'txt_color' => 'green'),
            array('value' => '1', 'txt' => '资讯', 'txt_color' => 'red'),
            ), 
        // 网站设置伪静态
        'site_rewriteurl' => array(
            array('value' => '0', 'txt' => '关闭', 'txt_color' => ''),
            array('value' => '1', 'txt' => '开启', 'txt_color' => ''),
            ), 
		// 网站设置图片伪静态
        'img_rewrite' => array(
            array('value' => '0', 'txt' => '关闭', 'txt_color' => ''),
            array('value' => '1', 'txt' => '开启', 'txt_color' => ''),
            ), 
		//网站设置手机版
		'content_mobile'=>array(
            array('value' => '0', 'txt' => '关闭', 'txt_color' => ''),
            array('value' => '1', 'txt' => '开启', 'txt_color' => ''),
            ), 
        // 应用图片本地化
        'app_img_local' => array(
            array('value' => '0', 'txt' => '关闭', 'txt_color' => ''),
            array('value' => '1', 'txt' => '开启', 'txt_color' => ''),
            ), 
        // 正文内链
        'content_nlink' => array(
            array('value' => '0', 'txt' => '关闭', 'txt_color' => ''),
            array('value' => '1', 'txt' => '开启', 'txt_color' => ''),
            ), 
        // 链接位置
        'flink_is_site' => array(
            array('value' => '0', 'txt' => '首页', 'txt_color' => ''),
            array('value' => '1', 'txt' => '全站', 'txt_color' => ''),
            ), 
		//分类显示
        'cat_show' => array(
            array('value' => '1', 'txt' => '显示', 'txt_color' => ''),
            array('value' => '0', 'txt' => '隐藏', 'txt_color' => ''),
            ), 
        // 审核状态
        'is_check' => array(
            array('value' => '0', 'txt' => '未审核', 'txt_color' => 'red'),
            array('value' => '1', 'txt' => '已审核', 'txt_color' => 'green'),
            ), 
        // 默认模板页面
        'tpl_system' => array(
            array('value' => 'content_info.php', 'txt' => '资讯内容模板', 'txt_color' => 'green'),
            array('value' => 'content_app.php', 'txt' => '应用内容模板', 'txt_color' => 'green'),
            array('value' => 'content_app_history.php', 'txt' => '应用版本模板', 'txt_color' => 'green'),
            array('value' => 'list_info.php', 'txt' => '资讯列表模板', 'txt_color' => 'orange'),
            array('value' => 'list_app.php', 'txt' => '应用列表模板', 'txt_color' => 'orange'),
            array('value' => 'cover_info.php', 'txt' => '资讯封面模板', 'txt_color' => 'red'),
            array('value' => 'cover_app.php', 'txt' => '应用封面模板', 'txt_color' => 'red'),
            array('value' => 'index.php', 'txt' => '首页', 'txt_color' => 'red'),
            array('value' => 'search.php', 'txt' => '搜索列表模板', 'txt_color' => 'red'),
            array('value' => 'special_content.php', 'txt' => '专题内容模板', 'txt_color' => 'red'),
            array('value' => 'special_list.php', 'txt' => '专题列表模板', 'txt_color' => 'red'),
            array('value' => 'inc_head.php', 'txt' => '头部包含模板', 'txt_color' => 'blue'),
            array('value' => 'inc_foot.php', 'txt' => '底部包含模板', 'txt_color' => 'blue'),
            array('value' => 'inc_right.php', 'txt' => '所有页面右侧模板', 'txt_color' => 'blue'),
            ), 
        // 推荐类型
        'area_type' => array(
            array('value' => '0', 'txt' => '应用推荐', 'txt_color' => ''),
            array('value' => '1', 'txt' => '资讯推荐', 'txt_color' => ''),
            array('value' => '2', 'txt' => '广告', 'txt_color' => ''),
            array('value' => '3', 'txt' => '应用专题', 'txt_color' => ''),
            array('value' => '4', 'txt' => '资讯专题', 'txt_color' => ''),
            ),
        );

    /**
     * 返回某个节点的某个值
$node   节点
     *    $value  值
     */
    public static function get_field($node, $value) {
        // 遍历某个节点
        foreach(vars :: $fields[$node] as $f) {
            if ($f['value'] == $value) {
                return $f; //print_r($field);
            } 
        } 
        return array('value' => '', 'txt' => '-', 'txt_color' => '');
    } 

    /**
     * 根据值，返回文本或者HTML
     *    $node   节点
     *    $value  值
$type   返回类型 txt 文本 html 带颜色属性的HTML文本
     */
    public static function get_field_str($node, $value, $type = 'txt') {
        $field = vars :: get_field($node, $value); //print_r($field);
        if ($type == 'txt') {
            return $field['txt'];
        } else {
            return '<font color="' . $field['txt_color'] . '">' . $field['txt'] . '</font>';
        } 
    } 
    /**
     * 输出HTML表单
     *    $params 参数数组 array('node'=>'','type'=>'','default'=>'')
     *    -> $type 表单类型 select,checkbox,radio
     *    -> $node    节点
     *    -> $default 默认选中
     *    -> $name    表单名称后缀，用于一个页面多次出现时候区分
     */
    public static function input_str($params) {
        // 初始化
        $node = isset($params['node'])?$params['node']:'';
        $type = isset($params['type'])?$params['type']:'select';
        $default = isset($params['default'])?$params['default']:'';
        $name = isset($params['name'])?$params['name']:''; 
        // 下拉框
        if ($type == 'select') {
            $html = '<select name="' . $node . $name . '" id="' . $node . $name . '">';
            foreach(vars :: $fields[$node] as $f) {
                $select = '';
                if (strlen($default) > 0 && $f['value'] == $default) $select = ' selected';
                $html .= '<option value="' . $f['value'] . '"' . $select . '>' . $f['txt'] . '</option>';
            } 
            $html .= '</select>';
            return $html;
        } 
        // 单选框
        if ($type == 'radio') {
            $html = '';
            foreach(vars :: $fields[$node] as $f) {
                $select = '';
                if (strlen($default) > 0 && $f['value'] == $default) $select = ' checked';
                $html .= '<input type="radio" name="' . $node . $name . '" value="' . $f['value'] . '"' . $select . '>&nbsp;' . $f['txt'] . '&nbsp;&nbsp;';
            } 
            return $html;
        } 
        // 复选框
        if ($type == 'checkbox') {
            $html = '';
            foreach(vars :: $fields[$node] as $f) {
                $select = '';
                if (strlen($default) > 0 && $f['value'] == $default) $select = ' checked';
                $html .= '<input type="checkbox" name="' . $node . $name . '" value="' . $f['value'] . '"' . $select . '>&nbsp;' . $f['txt'] . '&nbsp;&nbsp;';
            } 
            return $html;
        } 
        return '-';
    } 
} 
