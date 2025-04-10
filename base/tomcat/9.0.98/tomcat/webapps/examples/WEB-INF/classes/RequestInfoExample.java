/*
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to You under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

import java.io.IOException;
import java.io.PrintWriter;
import java.util.ResourceBundle;

import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import util.HTMLFilter;

/**
 * Example servlet showing request information.
 *
 * @author James Duncan Davidson &lt;duncan@eng.sun.com>
 */

public class RequestInfoExample extends HttpServlet {

    private static final long serialVersionUID = 1L;

    @Override
    public void doGet(HttpServletRequest request, HttpServletResponse response) throws IOException, ServletException {
        ResourceBundle rb = ResourceBundle.getBundle("LocalStrings", request.getLocale());

        response.setContentType("text/html");
        response.setCharacterEncoding("UTF-8");

        PrintWriter out = response.getWriter();
        out.println("<!DOCTYPE html><html>");
        out.println("<head>");
        out.println("<meta charset=\"UTF-8\" />");

        String title = rb.getString("requestinfo.title");
        out.println("<title>" + title + "</title>");
        out.println("</head>");
        out.println("<body bgcolor=\"white\">");

        /*
         * Can't use relative paths since this servlet might have pathInfo. Allows for deployment to a different context
         * path but otherwise assumes that the web application structure is not changed.
         */
        String baseURI = request.getServletContext().getContextPath() + "/servlets";

        out.println("<a href=\"" + baseURI + "/reqinfo.html\">");
        out.println("<img src=\"" + baseURI + "/images/code.gif\" height=24 " +
                "width=24 align=right border=0 alt=\"view code\"></a>");
        out.println("<a href=\"" + baseURI + "/index.html\">");
        out.println("<img src=\"" + baseURI + "/images/return.gif\" height=24 " +
                "width=24 align=right border=0 alt=\"return\"></a>");

        out.println("<h3>" + title + "</h3>");
        out.println("<table border=0><tr><td>");
        out.println(rb.getString("requestinfo.label.method"));
        out.println("</td><td>");
        out.println(HTMLFilter.filter(request.getMethod()));
        out.println("</td></tr><tr><td>");
        out.println(rb.getString("requestinfo.label.requesturi"));
        out.println("</td><td>");
        out.println(HTMLFilter.filter(request.getRequestURI()));
        out.println("</td></tr><tr><td>");
        out.println(rb.getString("requestinfo.label.protocol"));
        out.println("</td><td>");
        out.println(HTMLFilter.filter(request.getProtocol()));
        out.println("</td></tr><tr><td>");
        out.println(rb.getString("requestinfo.label.pathinfo"));
        out.println("</td><td>");
        out.println(HTMLFilter.filter(request.getPathInfo()));
        out.println("</td></tr><tr><td>");
        out.println(rb.getString("requestinfo.label.remoteaddr"));
        out.println("</td><td>");
        out.println(HTMLFilter.filter(request.getRemoteAddr()));
        out.println("</td></tr>");

        String cipherSuite = (String) request.getAttribute("javax.servlet.request.cipher_suite");
        if (cipherSuite != null) {
            out.println("<tr><td>");
            out.println("SSLCipherSuite:");
            out.println("</td><td>");
            out.println(HTMLFilter.filter(cipherSuite));
            out.println("</td></tr>");
        }

        out.println("</table>");
    }

    @Override
    public void doPost(HttpServletRequest request, HttpServletResponse response) throws IOException, ServletException {
        doGet(request, response);
    }

}

