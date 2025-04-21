package org.vulhub.shirodemo;

import org.apache.shiro.SecurityUtils;
import org.apache.shiro.authc.AuthenticationException;
import org.apache.shiro.authc.UsernamePasswordToken;
import org.apache.shiro.subject.Subject;
import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.*;

@Controller
public class UserController {
    @PostMapping("/doLogin")
    public String doLoginPage(@RequestParam("username") String username, @RequestParam("password") String password, @RequestParam(name="rememberme", defaultValue = "") String rememberMe) {
        Subject subject = SecurityUtils.getSubject();
        try {
            subject.login(new UsernamePasswordToken(username, password, rememberMe.equals("remember-me")));
        } catch (AuthenticationException e) {
            return "forward:/login";
        }

        return "forward:/";
    }

    @RequestMapping("/admin")
    public String helloPage() {
        return "admin";
    }

    @RequestMapping("/unauth")
    public String errorPage() {
        return "error";
    }

    @RequestMapping("/login")
    public String loginPage() {
        return "login";
    }

    @GetMapping("/")
    public String index() {
        return "login";
    }
}
