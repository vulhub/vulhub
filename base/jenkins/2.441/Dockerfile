FROM jenkins/jenkins:2.441-slim

LABEL maintainer="phithon <root@leavesongs.com>"

ENV JAVA_OPTS="-Djenkins.install.runSetupWizard=false"

COPY init.groovy /usr/share/jenkins/ref/init.groovy.d
