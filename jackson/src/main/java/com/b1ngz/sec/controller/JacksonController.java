package com.b1ngz.sec.controller;

import com.b1ngz.sec.model.Target;
import com.b1ngz.sec.util.ResourceUtils;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.PostMapping;
import org.springframework.web.bind.annotation.RequestBody;
import org.springframework.web.bind.annotation.RestController;

/**
 * Created by b1ngz on 2018/9/8.
 */
@RestController
public class JacksonController {

    @PostMapping(value = "exploit", consumes = "application/json")
    public String test(@RequestBody Target target) throws Exception {
        return target.toString();
    }

    @GetMapping(value = "spel.xml")
    public String getSpelXml() throws Exception {
        return ResourceUtils.readResourceFileAsStr("/spel.xml");
    }

}
