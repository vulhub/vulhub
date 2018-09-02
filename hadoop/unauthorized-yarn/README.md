# Hadoop YARN ResourceManager Unauthorized Access

## Principle

Reference http://archive.hack.lu/2016/Wavestone%20-%20Hack.lu%202016%20-%20Hadoop%20safari%20-%20Hunting%20for%20vulnerabilities%20-%20v1.0.pdf

## test environment

Running the test environment

```
Docker-compose up -d
```

After the environment is started, you can see the Hadoop YARN ResourceManager WebUI page by visiting `http://your-ip:8088`.

## Utilization

There are some differences in the methods and principles of use. Directly through the REST API without the hadoop client
  (https://hadoop.apache.org/docs/r2.7.3/hadoop-yarn/hadoop-yarn-site/ResourceManagerRest.html) You can also submit task execution.

The utilization process is as follows:

1. Listening locally to wait for a bounce shell connection
1. Call the New Application API to create an Application
1. Call the Submit Application API to submit

Reference [exp script] (exploit.py)