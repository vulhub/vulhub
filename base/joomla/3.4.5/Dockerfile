FROM php:5.6.12-apache

LABEL maintainer="phithon <root@leavesongs.com>"

RUN printf "deb http://archive.debian.org/debian jessie main" > /etc/apt/sources.list

# Install PHP extensions
RUN set -ex \
	&& apt-get update --allow-unauthenticated \
	&& apt-get install --allow-unauthenticated -y libpng12-dev libjpeg-dev libmcrypt-dev libzip-dev unzip mysql-client \
	&& rm -rf /var/lib/apt/lists/* \
	&& docker-php-ext-configure gd --with-png-dir=/usr --with-jpeg-dir=/usr \
	&& docker-php-ext-install gd mysqli mcrypt zip

# Download package and extract to web volume
RUN curl -o /tmp/joomla.zip -SL https://github.com/joomla/joomla-cms/releases/download/3.4.5/Joomla_3.4.5-Stable-Full_Package.zip \
	&& unzip /tmp/joomla.zip -d /var/www/html \
	&& rm /tmp/joomla.zip \
	&& chown -R www-data:www-data /var/www/html

RUN set -ex \
	&& curl -sSL -k -o /usr/local/bin/composer https://getcomposer.org/download/2.2.21/composer.phar \
	&& chmod +x /usr/local/bin/composer \
	&& composer config --global disable-tls true \
	&& composer config --global secure-http false

RUN set -ex \
	&& mkdir /usr/local/joomla \
	&& curl -k -sSL -o- https://github.com/joomlatools/joomlatools-console/archive/refs/tags/v1.4.11.tar.gz | tar xz --strip-components=1 -C /usr/local/joomla \
	&& cd /usr/local/joomla \
	&& composer install \
	&& /usr/local/joomla/bin/joomla -V

# Enable Apache Rewrite Module
RUN a2enmod rewrite

ENV PATH /usr/local/joomla/bin:${PATH}
COPY docker-entrypoint.sh /docker-entrypoint.sh
COPY .htaccess /var/www/html/.htaccess
ENTRYPOINT [ "bash", "/docker-entrypoint.sh" ]
CMD ["apache2-foreground"]
