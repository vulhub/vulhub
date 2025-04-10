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
<%@ page import="java.net.URLEncoder" %>
<%@ page import="java.nio.charset.StandardCharsets" %>
<%@ page import="java.security.Principal" %>
<%@ page import="java.util.Enumeration" %>
<%@ page import="org.apache.catalina.TomcatPrincipal" %>
<%
  if (request.getParameter("logoff") != null) {
    session.invalidate();
    response.sendRedirect("index.jsp");
    return;
  }
%>
<html>
<head>
<title>Protected Page for Examples</title>
</head>
<body bgcolor="white">

You are logged in as remote user
<b><%= util.HTMLFilter.filter(request.getRemoteUser()) %></b>
in session <b><%= session.getId() %></b><br><br>

<%
  if (request.getUserPrincipal() != null) {
%>
    Your user principal name is
    <b><%= util.HTMLFilter.filter(request.getUserPrincipal().getName()) %></b>
    <br><br>
<%
  } else {
%>
    No user principal could be identified.<br><br>
<%
  }
%>

<%
  String role = request.getParameter("role");
  if (role == null)
    role = "";
  if (role.length() > 0) {
    if (request.isUserInRole(role)) {
%>
      You have been granted role
      <b><%= util.HTMLFilter.filter(role) %></b><br><br>
<%
    } else {
%>
      You have <i>not</i> been granted role
      <b><%= util.HTMLFilter.filter(role) %></b><br><br>
<%
    }
  }
%>

To check whether your user name has been granted a particular role,
enter it here:
<form method="GET" action='<%= response.encodeURL("index.jsp") %>'>
<input type="text" name="role" value="<%= util.HTMLFilter.filter(role) %>">
<input type="submit" >
</form>
<br><br>

<%
  Principal p = request.getUserPrincipal();
  if (!(p instanceof TomcatPrincipal)) {
%>
<p>The principal does not support attributes.</p>
<%
  } else {
    TomcatPrincipal principal = (TomcatPrincipal) p;
%>
<p>The principal contains the following attributes:</p>
<table>
<tr><th>Name</th><th>Value</th><th>Type</th></tr>
<%
    Enumeration<String> names = principal.getAttributeNames();
    while (names.hasMoreElements()) {
      String name = names.nextElement();
      Object value = principal.getAttribute(name);
      String type = value != null ? value.getClass().getName() : "unknown";
      if (value instanceof Object[]) {
        Object[] values = (Object[]) value;
        value = "";
        for (int i = 0; i < values.length; i++) {
          value += values[i] + "<br/>";
        }
        if (values.length > 0) {
          type = values[0].getClass().getName() + "[]";
        } else {
          type = "unknown";
        }
      }
      type = type.replaceFirst("^java\\.lang\\.", "");
%>
<tr>
  <td><%= util.HTMLFilter.filter(name) %></td>
  <td><%= util.HTMLFilter.filter(String.valueOf(value)) %></td>
  <td><%= util.HTMLFilter.filter(type) %></td>
</tr>
<%
    }
%>
</table>
<%
  }
%>
<br><br>

<%
  // Count the existing attributes
  int sessionAttributeCount = 0;
  Enumeration<String> names = session.getAttributeNames();
  while (names.hasMoreElements()) {
    names.nextElement();
    sessionAttributeCount++;
  }

  String dataName = request.getParameter("dataName");
  String dataValue = request.getParameter("dataValue");
  if (dataName != null) {
    if (dataValue == null) {
      session.removeAttribute(dataName);
      sessionAttributeCount--;
    } else if (sessionAttributeCount < 10) {
      session.setAttribute(dataName, dataValue);
      sessionAttributeCount++;
    } else {
%>
<p>Session attribute [<%= util.HTMLFilter.filter(dataName) %>] not added as there are already 10 attributes in the
session. Delete an attribute before adding another.</p>
<%
    }
  }

  if (sessionAttributeCount < 10) {
%>
To add some data to the authenticated session, enter it here:
<form method="GET" action='<%= response.encodeURL("index.jsp") %>'>
<input type="text" name="dataName">
<input type="text" name="dataValue">
<input type="submit" >
</form>
<%
  } else {
%>
<p>You may not add more than 10 attributes to this session.</p>
<%
  }
%>
<br><br>

<p>The authenticated session contains the following attributes:</p>
<table>
<tr><th>Name</th><th>Value</th></tr>
<%
  names = session.getAttributeNames();
  while (names.hasMoreElements()) {
    String name = names.nextElement();
    String value = session.getAttribute(name).toString();
%>
<tr>
  <td><%= util.HTMLFilter.filter(name) %></td>
  <td><%= util.HTMLFilter.filter(value) %></td>
  <td><a href='<%= response.encodeURL("index.jsp?dataName=" + URLEncoder.encode(name, "UTF-8")) %>'>delete</a></td>
</tr>
<%
  }
%>
</table>
<br><br>

If you have configured this application for form-based authentication, you can
log off by clicking
<a href='<%= response.encodeURL("index.jsp?logoff=true") %>'>here</a>.
This should cause you to be returned to the login page after the redirect
that is performed.

</body>
</html>
