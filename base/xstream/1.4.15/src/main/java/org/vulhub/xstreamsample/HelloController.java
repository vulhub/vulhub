package org.vulhub.xstreamsample;

import com.thoughtworks.xstream.XStream;
import org.springframework.web.bind.annotation.*;

@RestController
public class HelloController {

    @GetMapping(value = "/")
    public String hello()
    {
        return "hello, input your information please.";
    }

    @PostMapping(value = "/")
    public String read(@RequestBody String data)
    {
        XStream xs = new XStream();
        xs.processAnnotations(User.class);
        User user = (User) xs.fromXML(data);

        return "My name is " + user.getName() + ", I am " + user.getAge().toString() + " years old.";
    }
}
