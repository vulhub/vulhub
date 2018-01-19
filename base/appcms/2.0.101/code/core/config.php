<?php

/**
 * 系统参数
 */
define("VERSION", "2.0.101"); // 版本号
define("AUTH_CODE", ""); // 授权码
define("DATA_CENTER_URL", "http://data.appcms.cc"); //数据中心
define("UNION_URL", "http://union.appcms.cc"); //计费联盟URL
define("SITE_PATH", "/appcms_2.0.0/upload/"); //站点安装路径，默认为根目录
/**
 * 以下参数为可后台编辑
 */
define("SITE_NAME", "安卓市场"); //站点名称
define("SITE_URL", "http://www.appcms.cc"); //站点域名
define("SITE_LOGO", "/upload/logo.png"); //站点LOGO
define("UPLOAD_PATH", "/img/"); //资源上传路径,相对于upload
define("SITE_REWRITEURL", "0"); //是否开启伪静态重写 0==不开启，1==开启
define("SITE_SAFECODE", ""); //后台登录安全码
define("TEMPLATE", "default"); //模板风格
define("PAGESIZE", "10"); //默认每页显示行数
define("UPLOAD_CODE", "V4F_z0"); //上传安全码，该值安装时候自动生成，6-12位随机字母（大小写）数字，特殊符号
define("UPLOAD_KEY", "-0iMaK"); //上传密钥，该值安装时候自动生成，6-12位随机字母（大小写）数字，特殊符号
define("COMMENT_CODE", ""); //评论代码
define("APP_IMG_LOCAL", "0"); //添加应用是否图片本地化
define("CONTENT_NLINK", "0"); //是否开启正文内链
define("COUNT_CODE", ""); //统计代码
define("SEO_TITLE", ""); //首页SEO标题
define("SEO_KEYWORDS", ""); //首页关键词
define("SEO_DESCRIPTION", ""); //首页描述
define("CONTENT_MOBILE", "1"); //是否开启手机版
define("WAP_URL", ""); //手机版独立域名
define("WAP_LOGO", "/upload/logos.png"); //手机版图片
define("WAP_TPL", "m"); //手机版模板
define("CACHE_TIME", "");
define("UNION_ID", "");
define("BAIDU_SUBMIT", "");

define("IMG_REWRITE", "0");
define("UPLOAD_PATH_APK", "/apk/");