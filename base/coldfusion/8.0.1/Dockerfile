FROM ubuntu:14.04

LABEL maintainer="phithon <root@leavesongs.com>"

ENV COLD_FUSION_URL = https://vulhub.oss-cn-shanghai.aliyuncs.com/download/coldfusion/coldfusion-801-lin64.bin
ADD installer.properties /opt/installer.properties

RUN apt-get update && apt-get install wget libstdc++5 -y \
    && wget -O /opt/coldfusion-801-lin64.bin -q $COLD_FUSION_URL && chmod +x /opt/coldfusion-801-lin64.bin && /opt/./coldfusion-801-lin64.bin -i silent -f /opt/installer.properties

EXPOSE 8500

CMD ["bash", "-c", "/opt/coldfusion11/cfusion/bin/coldfusion start && tail -f /opt/coldfusion11/cfusion/logs/coldfusion-out.log"]