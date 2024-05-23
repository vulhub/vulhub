FROM sonatype/nexus3:3.68.0

LABEL maintainer="phithon <root@leavesongs.com>"

USER root

RUN set -ex \
    && sed -i 's/storage\.diskCache\.diskFreeSpaceLimit=4096/storage.diskCache.diskFreeSpaceLimit=512/' /opt/sonatype/nexus/etc/karaf/system.properties \
    && echo "-agentlib:jdwp=transport=dt_socket,server=y,suspend=n,address=5005" >> /opt/sonatype/nexus/bin/nexus.vmoptions
