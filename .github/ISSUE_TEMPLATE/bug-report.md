---
name: Bug Report
about: 请按照模板填写错误报告，以帮助我们改进vulhub

---

提交issue前，请检查你本地的vulhub是否是最新版，否则可能存在一些由于时间问题导致而今已经修复的bug。

填写如下信息

 - Which environment: 哪个环境出现BUG [e.g. python/ssti]
 - Host OS: 操作系统 [e.g. Ubuntu]
 - OS Version: 操作系统版本 [e.g. 18.04]
 - Docker version: Docker 版本 [e.g. Docker version 18.04.0-ce, build 3d479c0]
 - Compose version: Docker-Compose 版本 [e.g. docker-compose version 1.22.0, build f46880f]
 - Describe your bug: 描述你的Bug，什么情况下出现这个bug
 - [ ] 主机是否在中国大陆
 - [ ] 是否重试过仍然出现这个错误

注意，issue仅接受vulhub自身的bug，如：

- 编译时出现bug导致编译失败
- 运行后，环境无法访问
- 环境运行后，按照README中的操作，无法复现漏洞
- README中出现的错误，如错别字、参考链接失效等

注意：关于环境搭建成功，但复现漏洞不成功的情况，我可能不会测试并回复issue，因为我在搭建环境的时候均已测试成功。建议此类issue作者自行测试并寻找错误原因，如果找到原因的确是vulhub的问题（比如某种情况没考虑到），则再创建issue。

不接受：

- 安装docker或docker-compose时出现的bug
- 运行docker、docker-compose时出现的bug
- 拉取/下载vulhub时出现的bug
- 拉取docker镜像因为网络原因导致拉取失败

## 附加信息

请贴出完整错误信息，可以是命令行输出、软件报错信息、截图等。

**注意，请贴出完整错误信息，不要只粘贴错误的最后一行！**
