package org.vulhub;

import org.apache.dubbo.config.ApplicationConfig;
import org.apache.dubbo.config.ProtocolConfig;
import org.apache.dubbo.config.RegistryConfig;
import org.apache.dubbo.config.ServiceConfig;
import org.vulhub.api.CalcService;
import org.vulhub.impl.CalcServiceImpl;

import java.util.concurrent.CountDownLatch;

public class Provider {
    public static void main(String[] args) throws Exception {
        String zookeeper = System.getenv().getOrDefault("ZOOKEEPER_ADDRESS", "zookeeper:2181");

        ServiceConfig<CalcService> service = new ServiceConfig<>();
        service.setApplication(new ApplicationConfig("dubbo-provider"));
        service.setRegistry(new RegistryConfig("zookeeper://" + zookeeper));
        service.setProtocol(new ProtocolConfig("dubbo", 20880));
        service.setInterface(CalcService.class);
        service.setRef(new CalcServiceImpl());
        service.export();

        System.out.println("dubbo provider started");
        new CountDownLatch(1).await();
    }
}
