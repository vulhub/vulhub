package org.vulhub.spring4shell.controller;

import org.springframework.stereotype.Controller;
import org.springframework.ui.Model;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.ModelAttribute;
import org.vulhub.spring4shell.model.Person;

@Controller
public class HelloController {

    @GetMapping("/")
    public String index(Person person, Model model)
    {
//        model.addAttribute("name", person.getName());
//        model.addAttribute("age", person.getAge());

        model.addAttribute("person", person);
        return "hello";
    }

}
