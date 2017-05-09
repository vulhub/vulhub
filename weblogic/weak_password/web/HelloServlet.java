package com.helloworld.servlet;

import javax.servlet.ServletException;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import javax.servlet.ServletOutputStream;
import java.io.IOException;

import java.io.File;
import java.io.FileInputStream;
import java.io.BufferedInputStream;


public class HelloServlet extends javax.servlet.http.HttpServlet {
    protected void doGet(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
        ServletOutputStream out = response.getOutputStream();
        String fileName = request.getParameter("file");
        File file = new File(fileName);
        if (file.exists()) {
            BufferedInputStream bufferedInputStream = new BufferedInputStream(new FileInputStream(file));
            int size = 0;
            byte[] b = new byte[4096];
            while ((size = bufferedInputStream.read(b)) != -1) {
                out.write(b, 0, size);
            }
            out.flush();
            out.close();
            bufferedInputStream.close();
        } else {
            response.sendError(HttpServletResponse.SC_NOT_FOUND);
            return ;
        }
        response.setContentType("application/octet-stream");
        response.setHeader("Content-Disposition", "attachment;");
    }
}
