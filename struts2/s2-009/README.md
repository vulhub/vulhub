# S2-009 远程代码执行漏洞

影响版本: 2.1.0 - 2.3.1.1

漏洞详情: http://struts.apache.org/docs/s2-009.html

## 测试环境搭建

```
docker-compose build
docker-compose up -d
```

## 原理

> 前置阅读： 这个漏洞再次来源于s2-003、s2-005。了解该漏洞原理，需要先阅读s2-005的说明：https://github.com/phith0n/vulhub/blob/master/struts2/s2-005/README.md

参考![Struts2漏洞分析之Ognl表达式特性引发的新思路](https://www.t00ls.net/viewthread.php?tid=21197)，文中说到，该引入ognl的方法不光可能出现在这个漏洞中，也可能出现在其他java应用中。

Struts2对s2-003的修复方法是禁止`#`号，于是s2-005通过使用编码`\u0023`或`\43`来绕过；于是Struts2对s2-005的修复方法是禁止`\`等特殊符号，使用户不能提交反斜线。

但是，如果当前action中接受了某个参数`example`，这个参数将进入OGNL的上下文。所以，我们可以将OGNL表达式放在`example`参数中，然后使用`/helloword.acton?example=<OGNL statement>&(example)('xxx')=1`的方法来执行它，从而绕过官方对`#`、`\`等特殊字符的防御。