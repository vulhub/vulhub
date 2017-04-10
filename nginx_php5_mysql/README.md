# Nginx+PHP+Mysql 环境

该环境是Nginx+PHP+Mysql环境，三个服务的版本为：

 - Nginx 1.x 最新版
 - PHP 5.x 最新版
 - Mysql 5.x 最新版

Web目录为`./www`，对应到PHP容器里是`/var/www/html`，对应到nginx容器中是`/usr/share/nginx/html`。

使用`docker-compose up -d`启动，启动后监听80和443端口，访问`http://your-ip/index.php`即可查看到phpinfo。

Mysql相关参数：

 - 地址：`mysql`
 - 端口：`3306`
 - ROOT密码：`root`

你可以在PHP中使用环境变量，来直接获取Mysql地址和ROOT密码：

```php
new mysqli($_ENV['DB_HOST'], "root", $_ENV['DB_PASS'], "mysql");
```

## Nginx 配置文件

Nginx配置文件为`./nginx/nginx.conf`。Nginx Server段配置文件在`./nginx/sites-enabled`目录下，你可以直接修改default.conf或在该目录下添加新的虚拟主机。

修改完成配置，执行`docker-compose restart nginx`重启Nginx容器即可。

## PHP 配置

PHP配置需要进入容器修改，执行`docker-compose exec php bash`进入容器，将你的配置文件放在`/usr/local/etc/php`下即可。

安装新的扩展：

```
docker-php-ext-install extension_name
```

更多配置方法可以参考 https://hub.docker.com/_/php/

## Mysql 配置

将你的Mysql配置文件放置在`mysql/conf.d`目录下即可。

Mysql的数据文件放在`mysql/data/`目录下（在第一次启动Mysql容器时生成），只要不删除这个目录，即使Mysql容器被删除，下次重新创建时数据也不会丢失。

## 更换配置

如果你想更换PHP/Mysql/Nginx的版本：

1. Mysql版本：修改docker-compose.yml中mysql段的image的值，如`mysql:5.5`，阅读 https://hub.docker.com/_/mysql/ 查看Docker支持的Mysql版本
2. Nginx版本：修改docker-compose.yml中nginx段的image的值，如`nginx:1.10`，阅读 https://hub.docker.com/_/nginx/ 查看Docker支持的Nginx版本
3. PHP版本：PHP版本变动比较大，当前Docker支持的PHP版本最低5.6，修改Dockerfile中`FROM php:5-fpm`为其他版本即可。阅读 https://hub.docker.com/_/php/ 查看Docker支持的PHP版本

如果你对Dockerfile进行了一定修改，请执行`docker-compose build`重新编译环境。

执行`docker-compose up -d`更新整体。