
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
import java.net.URLEncoder;
import java.util.Date;
import java.util.Enumeration;
import java.util.ResourceBundle;

import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import javax.servlet.http.HttpSession;

import util.HTMLFilter;

/**
 * Example servlet showing request headers
 *
 * @author James Duncan Davidson &lt;duncan@eng.sun.com>
 */

public class SessionExample extends HttpServlet {

    private static final long serialVersionUID = 1L;

    private static final int SESSION_ATTRIBUTE_COUNT_LIMIT = 10;


    @Override
    public void doGet(HttpServletRequest request, HttpServletResponse response) throws IOException, ServletException {
        ResourceBundle rb = ResourceBundle.getBundle("LocalStrings", request.getLocale());

        response.setContentType("text/html");
        response.setCharacterEncoding("UTF-8");

        PrintWriter out = response.getWriter();
        out.println("<!DOCTYPE html><html>");
        out.println("<head>");
        out.println("<meta charset=\"UTF-8\" />");


        String title = rb.getString("sessions.title");
        out.println("<title>" + title + "</title>");
        out.println("</head>");
        out.println("<body bgcolor=\"white\">");

        out.println("<a href=\"../sessions.html\">");
        out.println(
                "<img src=\"../images/code.gif\" height=24 " + "width=24 align=right border=0 alt=\"view code\"></a>");
        out.println("<a href=\"../index.html\">");
        out.println(
                "<img src=\"../images/return.gif\" height=24 " + "width=24 align=right border=0 alt=\"return\"></a>");

        out.println("<h3>" + title + "</h3>");

        HttpSession session = request.getSession(true);
        out.println(rb.getString("sessions.id") + " " + session.getId());
        out.println("<br>");
        out.println(rb.getString("sessions.created") + " ");
        out.println(new Date(session.getCreationTime()) + "<br>");
        out.println(rb.getString("sessions.lastaccessed") + " ");
        out.println(new Date(session.getLastAccessedTime()));

        // Count the existing attributes
        int sessionAttributeCount = 0;
        Enumeration<String> names = session.getAttributeNames();
        while (names.hasMoreElements()) {
            names.nextElement();
            sessionAttributeCount++;
        }

        String dataName = request.getParameter("dataname");
        String dataValue = request.getParameter("datavalue");
        if (dataName != null) {
            if (dataValue == null) {
                session.removeAttribute(dataName);
                sessionAttributeCount--;
            } else if (sessionAttributeCount < SESSION_ATTRIBUTE_COUNT_LIMIT) {
                session.setAttribute(dataName, dataValue);
                sessionAttributeCount++;
            } else {
                out.print("<p> Session attribute [");
                out.print(HTMLFilter.filter(dataName));
                out.print("] not added as there are already "+ SESSION_ATTRIBUTE_COUNT_LIMIT + " attributes in the ");
                out.println("session. Delete an attribute before adding another.");
            }
        }

        out.println("<p>");
        out.println(rb.getString("sessions.data") + "<br>");
        names = session.getAttributeNames();
        while (names.hasMoreElements()) {
            String name = names.nextElement();
            String value = session.getAttribute(name).toString();
            out.println(HTMLFilter.filter(name) + " = " + HTMLFilter.filter(value));
            out.print("<a href=\"");
            out.print(HTMLFilter.filter(
                    response.encodeURL("SessionExample?dataname=" + URLEncoder.encode(name, "UTF-8"))));
            out.println("\" >delete</a>");
            out.println("<br>");
        }

        if (sessionAttributeCount < SESSION_ATTRIBUTE_COUNT_LIMIT) {
            out.println("<p>");
            out.print("<form action=\"");
            out.print(response.encodeURL("SessionExample"));
            out.print("\" ");
            out.println("method=POST>");
            out.println(rb.getString("sessions.dataname"));
            out.println("<input type=text size=20 name=dataname>");
            out.println("<br>");
            out.println(rb.getString("sessions.datavalue"));
            out.println("<input type=text size=20 name=datavalue>");
            out.println("<br>");
            out.println("<input type=submit>");
            out.println("</form>");

            out.println("<p>GET based form:<br>");
            out.print("<form action=\"");
            out.print(response.encodeURL("SessionExample"));
            out.print("\" ");
            out.println("method=GET>");
            out.println(rb.getString("sessions.dataname"));
            out.println("<input type=text size=20 name=dataname>");
            out.println("<br>");
            out.println(rb.getString("sessions.datavalue"));
            out.println("<input type=text size=20 name=datavalue>");
            out.println("<br>");
            out.println("<input type=submit>");
            out.println("</form>");

            out.print("<p><a href=\"");
            out.print(HTMLFilter.filter(response.encodeURL("SessionExample?dataname=exampleName&datavalue=exampleValue")));
            out.println("\" >URL encoded </a>");
        } else {
            out.print("<p>You may not add more than " + SESSION_ATTRIBUTE_COUNT_LIMIT + " attributes to this session.");
        }

        out.println("</body>");
        out.println("</html>");
    }

    @Override
    public void doPost(HttpServletRequest request, HttpServletResponse response) throws IOException, ServletException {
        doGet(request, response);
    }

}
