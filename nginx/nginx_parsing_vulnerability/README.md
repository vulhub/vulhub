# Nginx 解析漏洞复现

Nginx解析漏洞复现。

版本信息：

- Nginx 1.x 最新版
- PHP 7.x最新版

由此可知，该漏洞与Nginx、php版本无关，属于用户配置不当造成的解析漏洞。

直接执行`docker-compose up -d`启动容器，无需编译。

访问`http://your-ip/uploadfiles/nginx.png`和`http://your-ip/uploadfiles/nginx.png/.php`即可查看效果。

正常显示：

![image](1.jpg)

增加`/.php`后缀，被解析成PHP文件：

![image](2.jpg)

访问`http://your-ip/index.php`可以测试上传功能，上传代码不存在漏洞，但利用解析漏洞即可getshell：

![image](3.jpg)

![image](4.jpg)
