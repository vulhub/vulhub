package org.vulhub;

import org.apache.dubbo.config.ApplicationConfig;
import org.apache.dubbo.config.ConfigCenterConfig;
import org.apache.dubbo.config.ReferenceConfig;
import org.apache.dubbo.config.RegistryConfig;
import org.vulhub.api.DemoService;

public class Consumer {
    public static void main(String[] args) throws Exception {
        String zookeeper = System.getenv().getOrDefault("ZOOKEEPER_ADDRESS", "zookeeper:2181");

        ReferenceConfig<DemoService> reference = new ReferenceConfig<>();
        reference.setApplication(new ApplicationConfig("demo-consumer"));
        reference.setRegistry(new RegistryConfig("zookeeper://" + zookeeper));

        ConfigCenterConfig configCenter = new ConfigCenterConfig();
        configCenter.setAddress("zookeeper://" + zookeeper);
        reference.setConfigCenter(configCenter);

        reference.setInterface(DemoService.class);
        reference.setVersion("1.0.0");
        reference.setTimeout(3000);
        reference.setCheck(false);

        DemoService service = reference.get();
        System.out.println("dubbo consumer started and subscribed to config center");

        while (true) {
            try {
                System.out.println("consumer invoke result: " + service.sayHello("vulhub"));
            } catch (Throwable e) {
                System.err.println("consumer invoke failed: " + e.getMessage());
            }
            Thread.sleep(3000);
        }
    }
}
