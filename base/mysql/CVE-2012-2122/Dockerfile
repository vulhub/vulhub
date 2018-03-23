FROM ubuntu:latest
MAINTAINER sayantsing@gmail.com

WORKDIR /root/

RUN apt-get update && \
apt-get -y install libncurses5-dev \
 gcc \
 g++ \
 cmake \
 wget && \
 wget https://ftp.gnu.org/gnu/m4/m4-1.4.9.tar.gz && \
 wget https://ftp.gnu.org/gnu/bison/bison-2.5.1.tar.gz && \
 wget https://github.com/mysql/mysql-server/archive/mysql-5.6.5.tar.gz && \
 tar zxvf m4-1.4.9.tar.gz && \
 tar zxvf bison-2.5.1.tar.gz && \
 tar zxvf mysql-5.6.5.tar.gz && \
 rm -rf ./*.tar.gz

WORKDIR /root/m4-1.4.9
RUN ./configure && make && make install && rm -rf /root/m4-1.4.9

WORKDIR /root/bison-2.5.1/
RUN ./configure && make && make install && rm -rf /root/bison-2.5.1

WORKDIR /root/mysql-server-mysql-5.6.5
RUN cmake . && make && make install && \
rm -rf /root/mysql-server-mysql-5.6.5 && \
groupadd mysql && useradd -r -g mysql -s /bin/false mysql && \
mkdir /usr/local/mysql/mysql-files && \
chmod 750 /usr/local/mysql/mysql-files && \
chown -R mysql /usr/local/mysql && \
chgrp -R mysql /usr/local/mysql && \
ln -s /usr/local/mysql/bin/mysqld_safe /usr/local/bin && \
ln -s /usr/local/mysql/bin/mysqladmin /usr/local/bin && \
ln -s /usr/local/mysql/bin/mysql /usr/local/bin && \
cp /usr/local/mysql/support-files/mysql.server /etc/init.d/mysql && \
/usr/local/mysql/scripts/mysql_install_db --user=mysql --basedir=/usr/local/mysql --datadir=/usr/local/mysql/data && \
service mysql start && \
mysql -uroot -e "UPDATE mysql.user SET password=PASSWORD('123456') WHERE User='root';" && \ 
mysql -uroot -e "GRANT ALL PRIVILEGES ON *.* TO 'root'@'%' IDENTIFIED BY '123456' WITH GRANT OPTION;" && \
mysql -uroot -e "FLUSH PRIVILEGES;"

EXPOSE 3306

CMD ["/bin/bash", "-c", "mysqld_safe"]
