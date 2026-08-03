package org.vulhub.springviewmanipulation;

import javax.servlet.http.HttpServletResponse;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.stereotype.Controller;
import org.springframework.ui.Model;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.PathVariable;
import org.springframework.web.bind.annotation.RequestParam;
import org.springframework.web.bind.annotation.ResponseBody;

@Controller
public class HelloController {
    private static final Logger log = LoggerFactory.getLogger(HelloController.class);

    @GetMapping("/")
    public String index(Model model) {
        model.addAttribute("message", "happy birthday");
        return "welcome";
    }

    @GetMapping("/path")
    public String path(@RequestParam String lang) {
        return "user/" + lang + "/welcome";
    }

    @GetMapping("/fragment")
    public String fragment(@RequestParam String section) {
        return "welcome :: " + section;
    }

    @GetMapping("/doc/{document}")
    public void getDocument(@PathVariable String document) {
        log.info("Retrieving {}", document);
    }

    @GetMapping("/safe/fragment")
    @ResponseBody
    public String safeFragment(@RequestParam String section) {
        return "welcome :: " + section;
    }

    @GetMapping("/safe/redirect")
    public String redirect(@RequestParam String url) {
        return "redirect:" + url;
    }

    @GetMapping("/safe/doc/{document}")
    public void getDocument(@PathVariable String document, HttpServletResponse response) {
        log.info("Retrieving {}", document);
    }
}
