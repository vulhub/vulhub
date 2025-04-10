<?xml version="1.0" encoding="UTF-8"?>
<%--
 Licensed to the Apache Software Foundation (ASF) under one or more
  contributor license agreements.  See the NOTICE file distributed with
  this work for additional information regarding copyright ownership.
  The ASF licenses this file to You under the Apache License, Version 2.0
  (the "License"); you may not use this file except in compliance with
  the License.  You may obtain a copy of the License at

      http://www.apache.org/licenses/LICENSE-2.0

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.
--%>
<%@page session="false" contentType="text/html; charset=UTF-8" %>
<%@page import="java.util.Map" %>
<%@page import="java.util.Map.Entry" %>
<%@page import="java.util.List" %>
<!DOCTYPE html
     PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN"
     "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">

<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="en">
<% Map<String,List<String>> certList = (Map<String,List<String>>) request.getAttribute("certList");
%>
<head>
    <meta http-equiv="content-type" content="text/html; charset=UTF-8"/>
    <meta http-equiv="pragma" content="no-cache"/><!-- HTTP 1.0 -->
    <meta http-equiv="cache-control" content="no-cache,must-revalidate"/><!-- HTTP 1.1 -->
    <meta http-equiv="expires" content="0"/><!-- 0 is an invalid value and should be treated as 'now' -->
    <meta http-equiv="content-language" content="en"/>
    <meta name="copyright" content="copyright 2005-2024 the Apache Software Foundation"/>
    <meta name="robots" content="noindex,nofollow,noarchive"/>
    <title>Configured certificate chains per Connector</title>
</head>
<body>
<h1>Configured certificate chains per Connector</h1>

<table border="1" cellpadding="2" cellspacing="2" width="100%">
    <thead>
        <tr>
            <th>Connector / TLS Virtual Host / Certificate type</th>
            <th>Certificate chain</th>
        </tr>
    </thead>
    <tbody>
        <%
        for (Map.Entry<String, List<String>> entry : certList.entrySet()) {
        %>
        <tr>
            <td><%=entry.getKey()%></td>
            <td>
            <%
            for (String cert : entry.getValue()) {
            %>
                <pre><%=cert%></pre>
            <%
            }
            %>
            </td>
        </tr>
        <%
        }
        %>
    </tbody>
</table>

<form method="get" action="<%=request.getContextPath()%>/html">
  <p style="text-align: center;">
    <input type="submit" value="Return to main page" />
  </p>
</form>

<%--div style="display: none;">
<p>
    <a href="http://validator.w3.org/check?uri=referer"><img
        src="http://www.w3.org/Icons/valid-html401"
        alt="Valid HTML 4.01!" height="31" width="88"></a>
    <a href="http://validator.w3.org/check?uri=referer"><img
        src="http://www.w3.org/Icons/valid-xhtml10"
        alt="Valid XHTML 1.0!" height="31" width="88" /></a>
    <a href="http://validator.w3.org/check?uri=referer"><img
        src="http://www.w3.org/Icons/valid-xhtml11"
        alt="Valid XHTML 1.1!" height="31" width="88" /></a>
</p>
</div--%>

</body>
</html>
