FROM jenkins/jenkins:2.138-slim

LABEL maintainer="phithon <root@leavesongs.com>"

ENV JAVA_OPTS="-Djenkins.install.runSetupWizard=false"

COPY init.groovy /usr/share/jenkins/ref/init.groovy.d

COPY plugins.txt /usr/share/jenkins/ref/plugins.txt
RUN /usr/local/bin/install-plugins.sh < /usr/share/jenkins/ref/plugins.txt