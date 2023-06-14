FROM httpd:2.4

LABEL maintainer="b1tkeeper <bohan.wang.growing@outlook.com>"

RUN sed -i 's/deb.debian.org/mirrors.ustc.edu.cn/g' /etc/apt/sources.list &&\
    apt-get update &&\
    apt-get install subversion -y &&\
    cd /root/ &&\
    svnadmin create svnrepo &&\
    sed -i 's/# anon-access = read/anon-access = write/g' svnrepo/conf/svnserve.conf &&\
    echo "[/]" >> svnrepo/conf/authz &&\
    echo "* = rw" >> svnrepo/conf/authz &&\
    svnserve -d -r svnrepo/ &&\
    cd /usr/local/apache2/ &&\
    rm -rf htdocs && svn checkout svn://localhost htdocs &&\
    echo "<html><body><h1>It works~</h1></body></html>" > htdocs/index.html &&\
    cd htdocs && svn add index.html && svn commit -m "first commit" &&\
    cd /usr/local/apache2/

EXPOSE 80
