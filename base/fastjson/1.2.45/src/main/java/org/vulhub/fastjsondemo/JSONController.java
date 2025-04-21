package org.vulhub.fastjsondemo;

import com.alibaba.fastjson.support.spring.annotation.ResponseJSONP;
import org.springframework.http.MediaType;
import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.RequestBody;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestMethod;
import org.springframework.web.bind.annotation.ResponseBody;

@Controller
public class JSONController {
    @RequestMapping(value = "/", method = RequestMethod.GET, produces = MediaType.APPLICATION_JSON_UTF8_VALUE)
    @ResponseBody
    public Object getUser() {
        User user = new User();
        user.setName("Bob");
        user.setAge(25);

        return user;
    }

    @RequestMapping(value = "/", method = RequestMethod.POST, produces = MediaType.APPLICATION_JSON_UTF8_VALUE)
    @ResponseBody
    public Object setUser(@RequestBody User user) {
        user.setAge(20);
        return user;
    }
}
