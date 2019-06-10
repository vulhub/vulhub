FROM openjdk:8-jdk

LABEL maintainer="phithon <root@leavesongs.com>"

RUN apt-get update \
    && apt-get install --no-install-recommends -y git curl wget ca-certificates \
    && rm -rf /var/lib/apt/lists/*

ENV JENKINS_HOME /var/jenkins_home
ENV COPY_REFERENCE_FILE_LOG $JENKINS_HOME/copy_reference_file.log
ENV JENKINS_UC https://updates.jenkins.io
ENV JENKINS_UC_EXPERIMENTAL=https://updates.jenkins.io/experimental
ENV JENKINS_INCREMENTALS_REPO_MIRROR=https://repo.jenkins-ci.org/incrementals

ENV JENKINS_VERSION 2.46.1
RUN set -ex \
    && groupadd -g 1000 jenkins \
    && useradd -d "$JENKINS_HOME" -u 1000 -g 1000 -m -s /bin/bash jenkins \
    && [ -e /usr/share/jenkins ] || mkdir -p /usr/share/jenkins \
    && [ -e /usr/share/jenkins/ref ] || mkdir -p /usr/share/jenkins/ref \
    && mkdir -p /usr/share/jenkins/ref/init.groovy.d \
    && chown -R jenkins "$JENKINS_HOME" /usr/share/jenkins/ref \
    && curl -#SL http://mirrors.jenkins.io/war-stable/${JENKINS_VERSION}/jenkins.war -o /usr/share/jenkins/jenkins.war

EXPOSE 8080
EXPOSE 50000

COPY jenkins-support /usr/local/bin/jenkins-support
COPY jenkins.sh /usr/local/bin/jenkins.sh
COPY install-plugins.sh /usr/local/bin/install-plugins.sh
COPY init.groovy /usr/share/jenkins/ref/init.groovy.d

RUN chmod +x /usr/local/bin/jenkins.sh /usr/local/bin/install-plugins.sh

USER jenkins
ENV JAVA_OPTS="-Djenkins.install.runSetupWizard=false"

CMD ["/usr/local/bin/jenkins.sh"]