FROM jenkins/jenkins:2.441-slim

LABEL maintainer="phithon <root@leavesongs.com>"

ENV JAVA_OPTS="-Djenkins.install.runSetupWizard=false"

USER root
RUN set -ex \
    && sed -i 's/suspend=y/suspend=n/g' /usr/local/bin/jenkins.sh

COPY init.groovy /usr/share/jenkins/ref/init.groovy.d
