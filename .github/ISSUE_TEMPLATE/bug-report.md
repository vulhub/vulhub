---
name: Bug Report
about: Please fill out this bug report template to help us improve vulhub / 请按照模板填写错误报告，以帮助我们改进vulhub
---

Before submitting an issue, please make sure following things:

1. your local vulhub is up to date
2. Your host OS is on the AMD64 architecture. If you're using a Macbook with an M-series chip, make sure you've tried the methods listed at <https://vulhub.org/documentation/faq>.
3. If you're having trouble pulling the image, make sure you're not in mainland China and being affected by the GFW (Great Firewall).

Please fill in the following information:

- Which environment has the bug (e.g. langflow/CVE-2025-3248):
- Host OS (e.g. Ubuntu 24.04):
- Host CPU Architecture (e.g. x86_64):
- Docker version (e.g. Docker version 24.0.1, build 1160cc8):
- Is your Vulhub up to date: Yes / No
- Is your host in mainland China: Yes / No
- Have you retried and the error still occurs: Yes / No

After answering all the questions above, please describe the issue you encountered here in natural language, and provide logs and screenshots.

Only bugs related to vulhub itself are accepted, such as:

- Build image fails due to errors
- Environment is inaccessible after running
- Vulnerability cannot be reproduced following the README
- Errors in the README, such as typos or invalid reference links

Not accepted:

- Bugs during docker installation
- Bugs that occur within Docker when running it
- Failed to pull/download vulhub due to network issues
- Failed to pull docker images due to network issues

Note: If the environment is set up successfully but the vulnerability cannot be reproduced, I may not test or reply to such issues, as all environments have been tested during setup. Please troubleshoot on your own first. If you find the cause is indeed a vulhub issue (e.g., an unconsidered scenario), then create an issue.

Please paste the complete error message, which can be command line output, software error messages, screenshots, etc.

**Note: Please paste the complete error message, not just the last line!**

------------------

提交issue前，请先检查下面的问题：

1. 你本地的vulhub是最新版，否则可能存在一些由于时间问题导致而今已经修复的bug
2. 你的主机是AMD64架构，如果是M系列芯片的Mac，请先尝试这个文档中列出的方法：<https://vulhub.org/documentation/faq>
3. 如果你在拉取镜像时遇到网络问题，请确保你不在中国大陆，没有受到GFW（Great Firewall）的影响

检查完毕上述问题后，再填写下面的列表，确保我们能够帮助你进行Debug：

- 哪个环境出现了BUG（例如langflow/CVE-2025-3248）：
- 主机使用的操作系统（例如Ubuntu 24.04）：
- 主机CPU架构（例如x86_64）：
- Docker版本（例如Docker version 24.0.1, build 1160cc8）：
- 你的Vulhub是否是最新版：是 / 否
- 主机是否在中国大陆：是 / 否
- 是否重试过仍然出现这个错误：是 / 否

在填写完上面的所有问题后，再在此处使用自然语言描述你遇到的问题，并提供日志和截图。

我们仅接受Vulhub自身的bug，如：

- 编译时出现bug导致编译失败
- 漏洞环境运行后，环境无法访问
- 漏洞环境运行后，按照README中的操作，无法复现漏洞
- README 中出现的错误，如错别字、参考链接失效等

不接受如下问题：

- 安装docker时出现的bug
- 运行docker时，docker本身出现的bug
- 拉取/下载vulhub时，因为网络原因导致拉取失败
- 拉取docker镜像时，因为网络原因导致拉取失败

注意：关于环境搭建成功，但复现漏洞不成功的情况，我可能不会测试并回复issue，因为所有环境在搭建的时候均已测试成功。建议此类issue作者自行测试并寻找错误原因，如果找到原因的确是Vulhub的问题（比如某种情况没考虑到），则再创建issue。

**附加信息**

请贴出完整错误信息，可以是命令行输出、软件报错信息、截图等。

**注意，请贴出完整错误信息，不要只粘贴错误的最后一行！**
