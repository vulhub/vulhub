package org.vulhub;

import org.apache.dubbo.config.ApplicationConfig;
import org.apache.dubbo.config.ConfigCenterConfig;
import org.apache.dubbo.config.ProtocolConfig;
import org.apache.dubbo.config.RegistryConfig;
import org.apache.dubbo.config.ServiceConfig;
import org.vulhub.api.DemoService;
import org.vulhub.impl.DemoServiceImpl;

import java.util.concurrent.CountDownLatch;

public class Provider {
    public static void main(String[] args) throws Exception {
        String zookeeper = System.getenv().getOrDefault("ZOOKEEPER_ADDRESS", "zookeeper:2181");

        ServiceConfig<DemoService> service = new ServiceConfig<>();
        service.setApplication(new ApplicationConfig("demo-provider"));
        service.setRegistry(new RegistryConfig("zookeeper://" + zookeeper));

        ConfigCenterConfig configCenter = new ConfigCenterConfig();
        configCenter.setAddress("zookeeper://" + zookeeper);
        service.setConfigCenter(configCenter);

        service.setProtocol(new ProtocolConfig("dubbo", 20880));
        service.setInterface(DemoService.class);
        service.setRef(new DemoServiceImpl());
        service.setVersion("1.0.0");
        service.export();

        System.out.println("dubbo provider started");
        new CountDownLatch(1).await();
    }
}
