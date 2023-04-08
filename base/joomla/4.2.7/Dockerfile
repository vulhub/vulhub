FROM php:7.4.33-apache

LABEL maintainer="phithon <root@leavesongs.com>"

# Install PHP extensions
RUN apt-get update && apt-get install -y libpng-dev libjpeg-dev libmcrypt-dev libzip-dev default-mysql-client unzip wait-for-it && rm -rf /var/lib/apt/lists/* \
	&& docker-php-ext-configure gd --with-jpeg \
	&& docker-php-ext-install gd
RUN docker-php-ext-install mysqli
RUN docker-php-ext-install zip

# Download package and extract to web volume
RUN curl -o /tmp/joomla.zip -SL https://github.com/joomla/joomla-cms/releases/download/4.2.7/Joomla_4.2.7-Stable-Full_Package.zip \
	&& unzip /tmp/joomla.zip -d /var/www/html \
	&& rm /tmp/joomla.zip \
	&& chown -R www-data:www-data /var/www/html

RUN curl -sSL -o /usr/local/bin/composer https://getcomposer.org/download/2.5.5/composer.phar \
	&& chmod +x /usr/local/bin/composer \
	&& composer global require joomlatools/console:2.0.2 \
	&& /root/.composer/vendor/bin/joomla -V

# Enable Apache Rewrite Module
RUN a2enmod rewrite

ENV PATH /root/.composer/vendor/bin:${PATH}
COPY docker-entrypoint.sh /docker-entrypoint.sh
COPY .htaccess /var/www/html/.htaccess
ENTRYPOINT [ "bash", "/docker-entrypoint.sh" ]
CMD ["apache2-foreground"]
