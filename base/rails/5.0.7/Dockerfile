FROM ruby:2.5

LABEL maintainer="phithon <root@leavesongs.com>"

RUN set -ex \
    && cd /usr/src \
    && gem install rails -v 5.0.7 --no-document \
    && rails new blog --skip-bundle

COPY Gemfile.lock Gemfile /usr/src/blog/

WORKDIR /usr/src/blog

RUN set -ex \
    && bundle install

CMD ["rails", "server", "-b", "0.0.0.0", "-p", "3000"]