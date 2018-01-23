# AppCMS comment.php文件SQL注入

## 漏洞简介

APPCMS是一套专业的APP内容管理系统，在APPCMS comment.php中存在SQL注入漏洞，攻击者可以利用该漏洞获取数据库敏感信息或执行未授权操作。
APPCMS允许游客进行评论，评论成功后会显示评论者的部分IP。该CMS会首先尝试使用PHP方法`getenv('HTTP_CLIENT_IP')`，获取HTTP请求中名为`CLIENT-IP`的值，这是一个非标准的HTTP请求头，但是被相当一部分服务器实现，包括不限于Nginx、Apache。该CMS没有对`CLIENT-IP`内容进行校验，直接将其中所有内容作为IP字段存入数据库，因此造成了一个SQL注入漏洞。

参考链接:

* https://www.seebug.org/vuldb/ssvid-96957
* https://www.bodkin.ren/index.php/archives/149/
* http://www.cnvd.org.cn/flaw/show/CNVD-2017-13891

## 环境搭建

启动漏洞环境

```
docker-compose up -d
```

8080是该镜像中CMS的默认端口，环境启动后，访问`http://your-ip:8080`，会进入安装页面，按照提示点击下一步即可。

最后一步需要填写数据库主机，填写mysql即可。

安装完成后需要删除install文件夹才能进入后台管理界面，需要通过`docker exec -it your-container-id sh`进入该容器后手动删除。

## 具体攻击过程

1. 任意打开一个APP详情页面，其中包含一个非标准化的表单，用于提交评论所需信息，其中包含三个隐藏的input组件，其对应表单信息依次为`type`、`id`、`parent_id`.

2. 构造一个提交评论的模拟请求，除上一步中提到的三个隐藏组件信息，还需要填写`code`(验证码)、`user`(游客昵称)、`comment`(评论内容)。手动构造HTTP请求中的`CLIENT-IP`信息，如

> 1.1.1.1'),({'101','0','0',(select user()),'DCjanus','65536000','0.0.0.0')#

需要注意的是，上面举例中的'101'最好与第1步中的`id`相同，以便于后续步骤中检测是否成功回显敏感信息。

## 总结

上述示例中仅利用SQL注入回显了数据库中user信息，实际使用中可以用于获取其他敏感信息，如管理用户密码等。也有文章表明可以以此漏洞为切入点，配合其他形式的攻击获取管理权限。

## 注意事项

PHP在5.4.0以后内置的WEB服务器，该服务器未实现对`CLIENT-IP`的支持，无法复现该漏洞。

该CMS版本发布时间较早，无法识别pdo_mysql环境，仅支持较老版本的php_mysql驱动，其中所用部分API在PHP 7下无法使用，故测试环境必须为PHP 5.x。
