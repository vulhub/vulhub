<%@ page contentType="text/html;charset=UTF-8" language="java" %>
<%@ taglib prefix="s" uri="/struts-tags"%>
<html>
<head>
    <title>S2-059 Demo</title>
</head>
<body>
<h2>S2-059 Demo</h2>
a： <s:url var="url" action="index"/><s:a id="%{skillName}" href="%{url}">List available Employees</s:a><br/>
label:  <s:label id="%{skillName}" value="label test"/><br/><br/>

<s:form action="index" method="POST">
    <s:textfield name="skillName" label="skillName" id="%{skillName}"/>
    <s:textfield name="url" label="url" id="%{url}"/>
    <s:submit value="Submit"/>
</s:form>

</body>
</html>