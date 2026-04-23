FROM registry.cn-qingdao.aliyuncs.com/dataease/dataease:v2.10.7

LABEL maintainer="phithon <root@leavesongs.com>"

COPY application.yml /opt/apps/config/application.yml

ENV JAVA_OPTIONS="-Dfile.encoding=utf-8 -Dloader.path=/opt/apps -Dspring.config.additional-location=/opt/apps/config/ -agentlib:jdwp=transport=dt_socket,server=y,suspend=n,address=*:5005"

EXPOSE 8100 5005
