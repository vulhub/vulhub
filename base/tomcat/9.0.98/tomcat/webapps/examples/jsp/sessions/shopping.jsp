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
<%@ page import="sessions.Item" %>
<html>
<head>
<title>Shopping Cart Example</title>
</head>

<body bgcolor="white">
<font size = 5 color="#CC0000">

<form type=POST action=carts.jsp>
<br>
Please enter item to add or remove:
<br>
Select Item:

<select name="itemId">
<%
  for (Item item : Item.values()) {
%>
  <option value="<%= item.ordinal() %>"><%= item.getTitle() %></option>
<%
  }
%>
</select>

<br> <br>
<INPUT TYPE=submit name="submit" value="add">
<INPUT TYPE=submit name="submit" value="remove">

</form>

</font>
</body>
</html>
