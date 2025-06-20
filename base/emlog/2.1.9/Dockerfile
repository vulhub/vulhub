# 使用 Ubuntu 18.04 作为基础镜像（提供 PHP 7.2）
FROM ubuntu:18.04

# 设置非交互式环境和时区
ARG DEBIAN_FRONTEND=noninteractive
ENV TZ=Asia/Shanghai
RUN ln -snf /usr/share/zoneinfo/$TZ /etc/localtime && \
    echo $TZ > /etc/timezone

# 安装基础服务和PHP 7.2
RUN apt-get update && \
    apt-get install -y --no-install-recommends \
        apache2 \
        ca-certificates \
        curl \
        unzip \
        libapache2-mod-php7.2 \
        php7.2 \
        php7.2-cli \
        php7.2-common \
        php7.2-mbstring \
        php7.2-mysql \
        php7.2-xml \
        php7.2-zip \
        && \
    rm -rf /var/lib/apt/lists/*

# 配置Apache
RUN a2dismod mpm_event && \
    a2enmod mpm_prefork rewrite

# 设置工作目录和权限
WORKDIR /var/www/html
RUN chown -R www-data:www-data /var/www/html

# 安装Emlog 2.1.9（使用官方旧版地址）
ARG EMLOG_VERSION=2.1.9
ARG EMLOG_URL=https://gitee.com/snowsun/emlog/releases/download/pro-${EMLOG_VERSION}/emlog_pro_${EMLOG_VERSION}.zip

RUN curl -L ${EMLOG_URL} -o emlog.zip && \
    unzip -q emlog.zip -d /var/www/html/ && \
    rm emlog.zip && \
    chown -R www-data:www-data /var/www/html

# Apache虚拟主机配置
COPY apache-emlog.conf /etc/apache2/sites-available/000-default.conf

# 暴露端口和启动命令
EXPOSE 80
CMD ["apache2ctl", "-D", "FOREGROUND"]
