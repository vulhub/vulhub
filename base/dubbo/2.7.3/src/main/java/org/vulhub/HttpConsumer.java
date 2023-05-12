package org.vulhub;

import org.springframework.context.support.ClassPathXmlApplicationContext;
import org.vulhub.api.CalcService;

public class HttpConsumer {
    public static void main(String []args) throws Exception {
        ClassPathXmlApplicationContext context = new ClassPathXmlApplicationContext("spring/http-consumer.xml");
        context.start();

        CalcService demoService = (CalcService) context.getBean("calcService");
        Integer result = demoService.add(123, 456);
        System.out.printf("Your result is %d\n", result);
    }
}
