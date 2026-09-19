package org.vulhub;

import org.apache.dubbo.config.ApplicationConfig;
import org.apache.dubbo.config.ConfigCenterConfig;
import org.apache.dubbo.config.ReferenceConfig;
import org.apache.dubbo.config.RegistryConfig;
import org.vulhub.api.CalcService;

public class Consumer {
    public static void main(String[] args) throws Exception {
        String zookeeper = System.getenv().getOrDefault("ZOOKEEPER_ADDRESS", "zookeeper:2181");

        ReferenceConfig<CalcService> reference = new ReferenceConfig<>();
        reference.setApplication(new ApplicationConfig("dubbo-consumer"));
        reference.setRegistry(new RegistryConfig("zookeeper://" + zookeeper));
        ConfigCenterConfig configCenter = new ConfigCenterConfig();
        configCenter.setAddress("zookeeper://" + zookeeper);
        reference.setConfigCenter(configCenter);
        reference.setInterface(CalcService.class);
        reference.setRouter("mesh-rule");
        reference.setCheck(false);

        CalcService service = reference.get();
        while (true) {
            try {
                System.out.println("consumer invoke result: " + service.add(1, 2));
            } catch (Throwable e) {
                System.err.println("consumer invoke failed: " + e.getMessage());
            }
            Thread.sleep(3000);
        }
    }
}
