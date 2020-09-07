<%@ page
        language="java"
        contentType="text/html; charset=UTF-8"
        pageEncoding="UTF-8" %>
<%@ taglib prefix="s" uri="/struts-tags" %>

<html>
<head>
    <title>S2-059 demo</title>

</head>
<body>
<s:a id="%{id}">your input id: ${id}
    <br>has ben evaluated again in id attribute
</s:a>
</body>
</html>