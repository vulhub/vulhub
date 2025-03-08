# Hadoop YARN ResourceManager Unauthorized Access

[中文版本(Chinese version)](README.zh-cn.md)

Hadoop YARN (Yet Another Resource Negotiator) is Apache Hadoop's cluster resource management system. If the YARN ResourceManager is exposed to the public internet without proper access controls, an attacker can submit and execute arbitrary applications on the cluster.

References:

- <http://archive.hack.lu/2016/Wavestone%20-%20Hack.lu%202016%20-%20Hadoop%20safari%20-%20Hunting%20for%20vulnerabilities%20-%20v1.0.pdf>
- <https://hadoop.apache.org/docs/r2.7.3/hadoop-yarn/hadoop-yarn-site/ResourceManagerRest.html>

## Environment Setup

Execute the following command to start vulnerable Hadoop YARN environment:

```
docker compose up -d
```

After the environment starts, visit `http://your-ip:8088` to access the Hadoop YARN ResourceManager WebUI.

## Vulnerability Reproduction

The exploitation method differs slightly from the original presentation. Even without a Hadoop client, you can submit tasks directly through the REST API (https://hadoop.apache.org/docs/r2.7.3/hadoop-yarn/hadoop-yarn-site/ResourceManagerRest.html).

The exploitation process is as follows:

1. Set up a listener on your local machine to receive the reverse shell connection
2. Call the New Application API to create an Application
3. Call the Submit Application API to submit the malicious application

For detailed implementation, refer to the [exploit script](exploit.py).
