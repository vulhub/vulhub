package com.vulhub.authzvuln;

import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.GetMapping;


@Controller
public class DemoController {
    @GetMapping("/admin/*")
    public String Admin(){
        return "admin";
    }

    @GetMapping("/")
    public String User(){
        return "index";
    }
}