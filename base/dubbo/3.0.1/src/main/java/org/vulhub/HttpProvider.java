package org.vulhub;

public class HttpProvider {
    public static void main(String[] args) throws Exception {
        if (args.length > 0 && "consumer".equals(args[0])) {
            Consumer.main(args);
        } else {
            Provider.main(args);
        }
    }
}
