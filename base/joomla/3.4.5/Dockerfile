FROM php:5.6.12-apache

LABEL maintainer="phithon <root@leavesongs.com>"

# Enable Apache Rewrite Module
RUN a2enmod rewrite

RUN printf "deb http://deb.debian.org/debian/ jessie main\ndeb-src http://deb.debian.org/debian/ jessie main\ndeb http://security.debian.org jessie/updates main\ndeb-src http://security.debian.org jessie/updates main" > /etc/apt/sources.list

# Install PHP extensions
RUN apt-get update && apt-get install -y libpng-dev libjpeg-dev libmcrypt-dev zip unzip && rm -rf /var/lib/apt/lists/* \
	&& docker-php-ext-configure gd --with-png-dir=/usr --with-jpeg-dir=/usr \
	&& docker-php-ext-install gd mysqli mcrypt zip

# Download package and extract to web volume
RUN curl -o joomla.zip -SL https://github.com/joomla/joomla-cms/releases/download/3.4.5/Joomla_3.4.5-Stable-Full_Package.zip \
	&& mkdir /usr/src/joomla \
	&& unzip joomla.zip -d /usr/src/joomla \
	&& rm joomla.zip \
	&& chown -R www-data:www-data /usr/src/joomla

# Copy init scripts and custom .htaccess
COPY docker-entrypoint.sh /entrypoint.sh
COPY makedb.php /makedb.php

ENTRYPOINT ["bash", "/entrypoint.sh"]
CMD ["apache2-foreground"]