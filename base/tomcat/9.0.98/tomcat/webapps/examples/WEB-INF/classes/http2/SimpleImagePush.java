/*
 *  Licensed to the Apache Software Foundation (ASF) under one or more
 *  contributor license agreements.  See the NOTICE file distributed with
 *  this work for additional information regarding copyright ownership.
 *  The ASF licenses this file to You under the Apache License, Version 2.0
 *  (the "License"); you may not use this file except in compliance with
 *  the License.  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */
package http2;

import java.io.IOException;
import java.io.PrintWriter;

import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import javax.servlet.http.PushBuilder;

public class SimpleImagePush extends HttpServlet {

    private static final long serialVersionUID = 1L;

    @Override
    protected void doGet(HttpServletRequest req, HttpServletResponse resp)
            throws ServletException, IOException {

        resp.setCharacterEncoding("UTF-8");
        resp.setContentType("text/html");
        PrintWriter pw = resp.getWriter();

        PushBuilder pb = req.newPushBuilder();
        if (pb != null) {
            pb.path("servlets/images/code.gif");
            pb.push();
            pw.println("<html>");
            pw.println("<body>");
            pw.println("<p>The following image was provided via a push request.</p>");
            pw.println("<img src=\"" + getServletContext().getContextPath() + "/servlets/images/code.gif\"/>");
            pw.println("</body>");
            pw.println("</html>");
            pw.flush();
        } else {
            pw.println("<html>");
            pw.println("<body>");
            pw.println("<p>Server push requests are not supported by this protocol.</p>");
            pw.println("</body>");
            pw.println("</html>");
        }
    }
}
