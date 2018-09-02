# Tomcat7+ weak password && background getshell vulnerability

Tomcat version: 8.0

## Environment Description

Tomcat supports the deployment of war files in the background, you can directly deploy the webshell to the web directory. Among them, in order to access the background, the corresponding user needs to have the corresponding authority.

Tomcat7+ permissions are divided into:

 - manager (background management)
   - manager-gui has html page permissions
   - manager-status has permission to view status
   - manager-script has permission to text interface, and status permission
   - manager-jmx has jmx permissions, and status permissions
 - host-manager (virtual host management)
   - admin-gui has html page permissions
   - admin-script has text interface permissions

What is the role of these permissions, read http://tomcat.apache.org/tomcat-8.5-doc/manager-howto.html

Configure user permissions in the `conf/tomcat-users.xml` file:

```xml
<?xml version="1.0" encoding="UTF-8"?>
<tomcat-users xmlns="http://tomcat.apache.org/xml"
              Xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
              Xsi:schemaLocation="http://tomcat.apache.org/xml tomcat-users.xsd"
              Version="1.0">

    <role rolename="manager-gui"/>
    <role rolename="manager-script"/>
    <role rolename="manager-jmx"/>
    <role rolename="manager-status"/>
    <role rolename="admin-gui"/>
    <role rolename="admin-script"/>
    <user username="tomcat" password="tomcat" roles="manager-gui,manager-script,manager-jmx,manager-status,admin-gui,admin-script" />
    
</tomcat-users>
```

It can be seen that the user tomcat has all the above permissions, and the password is `tomcat`.

In the case of normal installation, there is no user by default in tomcat8, and the manager page only allows local IP access. Attacks can only be made if the administrator has manually modified these properties.

## Vulnerability Testing

Start the entire environment without compiling:

```
Docker-compose up -d
```

Open the tomcat management page `http://your-ip:8080/manager/html` and enter the weak password `tomcat:tomcat` to access the background:

![](1.png)

Upload the war package to get the shell directly.