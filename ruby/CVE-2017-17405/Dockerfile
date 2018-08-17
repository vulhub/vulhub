FROM vulhub/ruby:2.4.1

MAINTAINER phithon <root@leavesongs.com>

RUN gem install sinatra

ADD web.rb /usr/src/web.rb

WORKDIR /usr/src

CMD ["ruby", "web.rb", "-p", "8080", "-o", "0.0.0.0"]