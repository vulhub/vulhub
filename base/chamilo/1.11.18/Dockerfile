FROM php:7.4-apache as php7_apache_base

# install packages
RUN apt-get update && apt-get install -yq --no-install-recommends \
    msmtp \
    curl \
    git \
    wget \
    parallel \
    libzip-dev \
    zlib1g-dev \
    libpng-dev \
    libicu-dev \
    libxslt-dev \
    libjpeg-dev \
    libgd3 \
    libgd-dev \
    libfreetype6-dev \
    libmemcached-dev \
    libwebp-dev \
    libxpm-dev \
    libjpeg62-turbo-dev \
    g++ \
    && apt-get autoclean \
    && apt-get autoremove -y \
    && rm -rf /var/lib/apt/lists/*

# Install APCu
RUN pecl install apcu && \
    pecl install memcached

# install and configure extensions
RUN docker-php-ext-install -j$(nproc) opcache pdo_mysql zip xsl
RUN docker-php-ext-configure intl && \
    docker-php-ext-install -j$(nproc) intl
RUN docker-php-ext-configure gd --with-jpeg --with-freetype && \
    docker-php-ext-install -j$(nproc) gd
RUN docker-php-ext-enable memcached apcu

WORKDIR /var/www

FROM php7_apache_base as php7_apache_chamilo_lms

ARG CHAMILO_VERSION="1.11.18"
ARG CHAMILO_TAR="chamilo-${CHAMILO_VERSION}-php74.tar.gz"

# download and install chamilo
ADD "https://github.com/chamilo/chamilo-lms/releases/download/v${CHAMILO_VERSION}/${CHAMILO_TAR}" "/var/www/chamilo.tar.gz"
# unpack, delete archive, rename folders
RUN tar -zxf chamilo.tar.gz \
    && rm chamilo.tar.gz \
    && mv chamilo* chamilo
# delete unneeded files
RUN find . -iname ".git*" -exec rm -rf {} + \
    && rm -rf chamilo/tests chamilo/.htac*

WORKDIR /var/www/chamilo

RUN bash -c 'mkdir main/inc/lib/javascript/bigupload/files/'
RUN bash -c 'chown www-data:www-data main/inc/lib/javascript/bigupload/files/'

RUN bash -c 'set -o pipefail; ls -d app main/default_course_document/images main/lang vendor web | parallel chown -R www-data:www-data "{}"'

# Configure and start Apache
RUN a2dissite 000-default; rm -rf /etc/apache2/sites-enabled/000-default.conf
COPY files/security.conf /etc/apache2/conf-available/
COPY files/chamilo.conf /etc/apache2/sites-available/chamilo.conf
RUN a2ensite chamilo; a2enmod remoteip rewrite headers

# configure /etc/hosts
# RUN echo "127.0.0.1 docker.chamilo.net" >> /etc/hosts

EXPOSE 80
