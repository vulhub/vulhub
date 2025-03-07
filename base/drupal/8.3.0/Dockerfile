FROM drupal:8.3.0

LABEL maintainer="phithon <root@leavesongs.com>"

RUN set -ex \
    && echo 'deb [trusted=yes check-valid-until=no]     http://snapshot.debian.org/archive/debian/20190624/ jessie main' > /etc/apt/sources.list \
    && echo 'deb-src [trusted=yes check-valid-until=no] http://snapshot.debian.org/archive/debian/20190624/ jessie main' >> /etc/apt/sources.list \
    && echo 'deb [trusted=yes check-valid-until=no]     http://snapshot.debian.org/archive/debian-security/20190624/ jessie/updates main' >> /etc/apt/sources.list \
    && echo 'deb-src [trusted=yes check-valid-until=no] http://snapshot.debian.org/archive/debian-security/20190624/ jessie/updates main' >> /etc/apt/sources.list

RUN set -ex \
    && apt-get update \
    && apt-get install --no-install-recommends -y libyaml-dev \
    && pecl install yaml-2.0.3 \
    && docker-php-ext-enable yaml \
    && echo 'yaml.decode_php = 1' >> /usr/local/etc/php/conf.d/docker-php-ext-yaml.ini
