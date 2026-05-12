FROM inlong/manager:1.3.0

LABEL maintainer="phithon <root@leavesongs.com>"

# The image ships mysql-connector-java 8.0.27, which patched the
# ServerStatusDiffInterceptor.populateMapWithSessionStatusValues() path
# from rs.getObject() to rs.getString() in 8.0.20. autoDeserialize is no
# longer reachable through SHOW SESSION STATUS on >= 8.0.20, so the RCE
# half of CVE-2022-40955 is unreachable unless we downgrade. Replace the
# driver with 8.0.19, which is the last version that still routes
# rs.getObject() through ResultSetImpl.getObject() -> ObjectInputStream.
RUN set -ex \
    && apt-get update \
    && apt-get install -y --no-install-recommends wget ca-certificates \
    && rm /opt/inlong-manager/lib/mysql-connector-java-8.0.27.jar \
    && wget -q -O /opt/inlong-manager/lib/mysql-connector-java-8.0.19.jar \
        https://repo1.maven.org/maven2/mysql/mysql-connector-java/8.0.19/mysql-connector-java-8.0.19.jar \
    && apt-get purge -y wget ca-certificates \
    && apt-get autoremove -y \
    && rm -rf /var/lib/apt/lists/* \
    && sed -i 's/^#JAVA_OPT="\${JAVA_OPT} -agentlib:jdwp=.*$/JAVA_OPT="${JAVA_OPT} -agentlib:jdwp=transport=dt_socket,server=y,suspend=n,address=5005"/' /opt/inlong-manager/bin/startup.sh
