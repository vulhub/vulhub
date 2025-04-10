<?xml version="1.0" encoding="UTF-8"?>
<!--
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
-->
<html xmlns:jsp="http://java.sun.com/JSP/Page"
      xmlns:fmt="http://java.sun.com/jsp/jstl/fmt"
      xmlns="http://www.w3.org/1999/xhtml">
  <jsp:output doctype-root-element="html"
              doctype-public="-//W3C//DTD XHTML Basic 1.0//EN"
              doctype-system="http://www.w3.org/TR/xhtml-basic/xhtml-basic10.dtd"/>
  <jsp:directive.page contentType="application/xhtml+xml" />
  <head>
    <title>JSPX - XHTML Basic Example</title>
  </head>
  <body>
    <h1>JSPX - XHTML Basic Example</h1>
    This example illustrates how to use JSPX to produce an XHTML basic
    document suitable for use with mobile phones, televisions,
    PDAs, vending machines, pagers, car navigation systems,
    mobile game machines, digital book readers, smart watches, etc.
    <p/>
    JSPX lets you create dynamic documents in a pure XML syntax compatible
    with existing XML tools.  The XML syntax in JSP 1.2 was awkward and
    required &amp;lt;jsp:root&amp;gt; to be the root element of the document.
    This is no longer the case in JSP 2.0.
    <p/>
    This particular example uses
    namespace declarations to make the output of this page a valid XHTML
    document.
    <p/>
    Just to prove this is live, here's some dynamic content:
    <jsp:useBean id="now" class="java.util.Date" />
    <fmt:formatDate value="${now}" pattern="MMMM d, yyyy, H:mm:ss"/>
  </body>
</html>
