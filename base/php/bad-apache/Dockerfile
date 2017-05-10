FROM vulhub/httpd:2.4.23

MAINTAINER phithon <root@leavesongs.com>

RUN { \
        echo 'LoadModule php5_module        /usr/lib/apache2/modules/libphp5.so'; \
		echo '<FilesMatch \.php$>'; \
		echo '\tSetHandler application/x-httpd-php'; \
		echo '</FilesMatch>'; \
		echo; \
		echo 'DirectoryIndex disabled'; \
		echo 'DirectoryIndex index.php index.html'; \
		echo; \
		echo '<Directory /usr/local/apache2/htdocs>'; \
		echo '\tOptions -Indexes'; \
		echo '\tAllowOverride All'; \
		echo '</Directory>'; \
	} | tee "/usr/local/apache2/conf/php5.conf"