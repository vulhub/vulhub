FROM alpine:3.7 

LABEL DCjanus <DCjanus@dcjanus.com>

RUN apk add --no-cache nginx php5-fpm php5-mysql php5-json php5-gd php5-zlib php5-ftp \
	&& mkdir /run/nginx 

COPY ./conf.d/default.conf /etc/nginx/conf.d/default.conf
COPY ./code /code
COPY ./conf.d/php-fpm.conf /etc/php5
COPY ./conf.d/php.ini /etc/php5

RUN chmod -R 777 /code

ENTRYPOINT php-fpm5 && nginx && tail -f /var/log/nginx/access.log

EXPOSE 80
