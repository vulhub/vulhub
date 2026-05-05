FROM inlong/manager:1.3.0

LABEL maintainer="phithon <root@leavesongs.com>"

RUN sed -i 's/^#JAVA_OPT="\${JAVA_OPT} -agentlib:jdwp=.*$/JAVA_OPT="${JAVA_OPT} -agentlib:jdwp=transport=dt_socket,server=y,suspend=n,address=5005"/' /opt/inlong-manager/bin/startup.sh
