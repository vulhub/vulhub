<?php
/**
 * AppCMS Copyright (c) 2012-2013
 * AppCMS is a free open source mobile phone APP application download website content management system.
 * Custom development, production BUG report template, please contact loyjers@qq.com
 * Author: loyjers
 * Editor: loyjers

 */
// 伪静态URL类
class url_rewrite
{
    public $url_config = array();
    public $root_path = "/";
    public $rewrite = 0;

    // 初始化类
    public function __construct($url_config)
    {
        $this->url_config = ($url_config);
    }

    // URL转换
    public function encode($url_tag, $params = array())
    {
        // 判断规则定义中是否有这个URL的定义
        if (!isset($this->url_config[$url_tag])) return $url_tag . 'URL规则定义不存在';
        $url = $this->url_config[$url_tag];
        $rule_node = 'URL转换失败';
        if ($this->rewrite == 0) $rule_node = $url['url'];
        if ($this->rewrite == 1) $rule_node = $url['rule'];
        // 处理动态首页
        if ($this->rewrite == 0 && isset($params['p']) && $params['p'] == 1) {
            $rule_node = preg_replace('~&p={p}~', '', $rule_node);
            $rule_node = preg_replace('~\?p={p}~', '', $rule_node);
        }
        // 处理静态首页
        if ($this->rewrite == 1 && isset($params['p']) && $params['p'] == 1) $rule_node = preg_replace('~.p{p}~', '', $rule_node);
        // if(isset($params['p'])) print_r($params);
        // 遍历传递进来的参数进行替换
        foreach ($params as $key => $val) {
            $rule_node = preg_replace('~({' . $key . '})~', $val, $rule_node);
        }
        if (!empty($params['rewrite_tag'])) {
            $rule_node = preg_replace('~(\w+)/(\d+).html~', '$1/' . $params['rewrite_tag'] . '.html', $rule_node);
        }
        // 返回替换好的URL
        return $this->root_path . $rule_node;
    }
}

?>