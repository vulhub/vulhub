# Struts2 S2-059 Remote Code Execution Vulnerablity(CVE-2019-0230)

[中文版本(Chinese version)](README.zh-cn.md)

Affected Version: Struts 2.0.0 - Struts 2.5.20

Details:

- https://cwiki.apache.org/confluence/display/WW/S2-059
- https://www.cnblogs.com/ph4nt0mer/p/13512599.html
- https://mp.weixin.qq.com/s/VyLiLrUV0yakh_lzTBYGyQ
- https://github.com/xinhaiyucheng/Struts2-Vuln-Demo/tree/master/s2-059

## Setup

Start the Struts 2.3.24 environment:

```
docker-compose up -d
```

After the environment is started, visit `http://your-ip:8080/` and you will see the Struts2 test page.

## Exploit

After the ognl expression `%{11*11}` is submitted，the values of `id` attributes were evaluated.

![1](1.jpg)