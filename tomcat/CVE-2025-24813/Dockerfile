FROM vulhub/tomcat:9.0.97

LABEL maintainer="phithon <root@leavesongs.com>"

RUN set -ex \
    && sed -i '/<load-on-startup>1<\/load-on-startup>/i \        <init-param>\n            <param-name>readonly</param-name>\n            <param-value>false</param-value>\n        </init-param>' /usr/local/tomcat/conf/web.xml \
    && sed -i '/<\/Context>/i \    <Manager className="org.apache.catalina.session.PersistentManager">\n        <Store className="org.apache.catalina.session.FileStore"/>\n    </Manager>' /usr/local/tomcat/conf/context.xml
