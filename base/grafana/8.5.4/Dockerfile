FROM grafana/grafana:8.5.4

LABEL maintainer="phithon <root@leavesongs.com>"

USER root
RUN set -ex \
    && sed -i 's|\[auth.anonymous\]|[auth.anonymous]\nenabled = true\norg_role = Admin\n|ig' /etc/grafana/grafana.ini

USER grafana
