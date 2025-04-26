package org.vulhub;

import java.io.IOException;
import java.io.ObjectInputStream;

import org.apache.logging.log4j.core.net.server.ObjectInputStreamLogEventBridge;
import org.apache.logging.log4j.core.net.server.TcpSocketServer;

public class App {

    public static void main(String[] args) {
        TcpSocketServer<ObjectInputStream> myServer = null;
        try {
            myServer = new TcpSocketServer<ObjectInputStream>(4712, new ObjectInputStreamLogEventBridge());
        } catch (IOException e) {
            e.printStackTrace();
        }
        myServer.run();
    }
}