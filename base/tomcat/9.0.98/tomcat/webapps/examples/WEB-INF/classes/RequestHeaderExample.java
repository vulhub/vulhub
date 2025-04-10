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
import java.util.Enumeration;
import java.util.Locale;
import java.util.ResourceBundle;

import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import javax.servlet.http.HttpSession;

import org.apache.tomcat.util.json.JSONFilter;

import util.CookieFilter;
import util.HTMLFilter;

/**
 * Example servlet showing request headers
 *
 * @author James Duncan Davidson &lt;duncan@eng.sun.com>
 */
public class RequestHeaderExample extends HttpServlet {

    private static final long serialVersionUID = 1L;

    @Override
    public void doGet(HttpServletRequest request, HttpServletResponse response) throws IOException, ServletException {
        if (prefersJSON(request.getHeader("Accept"))) {
            renderJSON(request, response);
        } else {
            renderHTML(request, response);
        }
    }

    /**
     * Returns true if the client appears to prefer a JSON response, false otherwise. Note that this method is not very
     * pedantic and uses only a very lazy algorithm for checking whether JSON is "preferred".
     *
     * @param acceptHeader The value of the HTTP "Accept" header from the client.
     *
     * @return true if the client appears to prefer a JSON response, false otherwise.
     */
    protected boolean prefersJSON(String acceptHeader) {
        if (null == acceptHeader) {
            return false;
        }
        // mime/type, mime/type;q=n, ...

        // Don't bother with the q-factor.
        // This is not expected to be 100% accurate or spec-compliant
        String[] accepts = acceptHeader.split(",");
        for (String accept : accepts) {
            if (accept.contains("application/json")) {
                return true;
            }

            // text/html, application/html, etc.
            if (accept.contains("html")) {
                return true;
            }
        }
        return false;
    }

    protected void renderHTML(HttpServletRequest request, HttpServletResponse response) throws IOException {
        ResourceBundle rb = ResourceBundle.getBundle("LocalStrings", request.getLocale());

        response.setContentType("text/html");
        response.setCharacterEncoding("UTF-8");

        PrintWriter out = response.getWriter();
        out.println("<!DOCTYPE html><html>");
        out.println("<head>");
        out.println("<meta charset=\"UTF-8\" />");

        String title = rb.getString("requestheader.title");
        out.println("<title>" + title + "</title>");
        out.println("</head>");
        out.println("<body bgcolor=\"white\">");

        out.println("<a href=\"../reqheaders.html\">");
        out.println(
                "<img src=\"../images/code.gif\" height=24 " + "width=24 align=right border=0 alt=\"view code\"></a>");
        out.println("<a href=\"../index.html\">");
        out.println(
                "<img src=\"../images/return.gif\" height=24 " + "width=24 align=right border=0 alt=\"return\"></a>");

        out.println("<h3>" + title + "</h3>");
        out.println("<table border=0>");
        Enumeration<String> e = request.getHeaderNames();
        while (e.hasMoreElements()) {
            String headerName = e.nextElement();
            String headerValue = request.getHeader(headerName);
            out.println("<tr><td bgcolor=\"#CCCCCC\">");
            out.println(HTMLFilter.filter(headerName));
            out.println("</td><td>");
            if (headerName.toLowerCase(Locale.ENGLISH).contains("cookie")) {
                HttpSession session = request.getSession(false);
                String sessionId = null;
                if (session != null) {
                    sessionId = session.getId();
                }
                out.println(HTMLFilter.filter(CookieFilter.filter(headerValue, sessionId)));
            } else {
                out.println(HTMLFilter.filter(headerValue));
            }
            out.println("</td></tr>");
        }
        out.println("</table>");
    }

    protected void renderJSON(HttpServletRequest request, HttpServletResponse response) throws IOException {
        response.setContentType("application/json");
        response.setCharacterEncoding("UTF-8");

        PrintWriter out = response.getWriter();

        out.append('[');
        Enumeration<String> e = request.getHeaderNames();
        while (e.hasMoreElements()) {
            String headerName = e.nextElement();
            String headerValue = request.getHeader(headerName);

            out.append("{\"").append(JSONFilter.escape(headerName)).append("\":\"");


            if (headerName.toLowerCase(Locale.ENGLISH).contains("cookie")) {
                HttpSession session = request.getSession(false);
                String sessionId = null;
                if (session != null) {
                    sessionId = session.getId();
                }
                out.append(JSONFilter.escape(CookieFilter.filter(headerValue, sessionId)));
            } else {
                out.append(JSONFilter.escape(headerValue));
            }
            out.append("\"}");

            if (e.hasMoreElements()) {
                out.append(',');
            }
        }

        out.print("]");
    }

    @Override
    public void doPost(HttpServletRequest request, HttpServletResponse response) throws IOException, ServletException {
        doGet(request, response);
    }

}

