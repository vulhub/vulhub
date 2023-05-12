FROM vulhub/rails:5.2.2

LABEL maintainer="phithon <root@leavesongs.com>"

RUN set -ex \
    && rails generate controller vulhub \
    && { \
        echo 'class VulhubController < ApplicationController'; \
        echo '  def index'; \
        echo '    render file: "#{Rails.root}/public/robots.txt"'; \
        echo '  end'; \
        echo 'end'; \
    } > app/controllers/vulhub_controller.rb \
    && { \
        echo 'Rails.application.routes.draw do'; \
        echo "  get 'robots', to: 'vulhub#index'"; \
        echo 'end'; \
    } > config/routes.rb