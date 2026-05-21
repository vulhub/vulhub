# Nacos JRaft Hessian反序列化远程代码执行漏洞（CNVD-2023-45001）

Nacos是阿里巴巴开源的一个云原生微服务基础设施平台，提供动态服务发现、配置管理和服务元数据管理等功能。

Nacos 1.4.0 至 1.4.5 版本（集群模式）及 2.0.0 至 2.2.3 版本（任意模式）中存在JRaft集群通信协议的反序列化漏洞。Nacos 的 JRaft 服务在 7848 端口监听集群内部通信，并使用 Hessian2 协议对消息进行序列化。由于 Hessian2 反序列化过程中未对类型进行限制，未经认证的远程攻击者可以向 7848 端口发送精心构造的恶意 payload，触发 gadget 链并在服务器上执行任意命令。2.x 系列中无论 Nacos 以 standalone 模式还是集群模式运行，该漏洞均可被利用，且无需任何身份认证。

参考链接：

- <https://xz.aliyun.com/news/13761>
- <https://github.com/c0olw/NacosRce/>
- <https://www.cnvd.org.cn/flaw/show/CNVD-2023-45001>

## 环境搭建

执行如下命令启动Nacos 2.2.2：

```
docker compose up -d
```

服务启动后，等待约 30 秒让 Nacos 完全初始化，访问 `http://your-ip:8848/nacos` 即可看到 Nacos 控制台。

## 漏洞复现

从 <https://github.com/c0olw/NacosRce/releases> 下载 `NacosRce.jar` 工具。该工具构造恶意的 Hessian2 序列化 payload，直接发送至 JRaft 端口（7848），利用反序列化漏洞在目标服务器上执行任意命令。

将 `your-ip` 替换为目标主机地址，执行以下命令发送 exploit payload：

```
java -jar NacosRce.jar http://your-ip:8848/nacos 7848 "touch /tmp/success"
```

![NacosRce工具通过反序列化漏洞成功执行命令](1.png)

命令执行后，进入容器验证文件是否已成功创建：

```
docker compose exec web ls /tmp/success
```

若 `/tmp/success` 文件存在，则证明远程代码执行成功。

![验证容器内/tmp/success文件存在](2.png)
