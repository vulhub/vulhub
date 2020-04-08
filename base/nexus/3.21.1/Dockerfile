FROM sonatype/nexus3:3.21.1

LABEL maintainer="phithon <root@leavesongs.com>"

USER root

RUN sed -i 's/storage\.diskCache\.diskFreeSpaceLimit=4096/storage.diskCache.diskFreeSpaceLimit=512/' /opt/sonatype/nexus/etc/karaf/system.properties