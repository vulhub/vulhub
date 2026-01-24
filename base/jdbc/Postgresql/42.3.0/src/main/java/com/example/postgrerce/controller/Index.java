package com.example.postgrerce.controller;

import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestParam;
import org.springframework.web.bind.annotation.ResponseBody;

import java.sql.DriverManager;

@Controller
public class Index {
    @RequestMapping(value = "/")
    @ResponseBody
    public String index(@RequestParam(value = "jdbc", defaultValue = "") String jdbc) throws Exception {
        if (jdbc.equals("")) {
            return "POST jdbc";
        }
        DriverManager.getConnection(jdbc);
        return jdbc;
    }
}
