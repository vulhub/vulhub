package org.vulhub;

import org.springframework.context.support.ClassPathXmlApplicationContext;

import java.util.concurrent.CountDownLatch;

public class HttpProvider {
    public static void main(String []args) throws Exception {
        ClassPathXmlApplicationContext context = new ClassPathXmlApplicationContext("spring/http-provider.xml");
        context.start();

        System.out.println("dubbo service started");
        new CountDownLatch(1).await();
    }
}
