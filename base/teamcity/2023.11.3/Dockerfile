FROM jetbrains/teamcity-server:2023.11.3

LABEL maintainer="phithon <root@leavesongs.com>"

ENV TEAMCITY_SERVER_OPTS="-Dteamcity.startup.maintenance=false -Dteamcity.firstStart.setupAdmin.enabled=false"

COPY --chmod=0755 entrypoint.sh /entrypoint.sh

ENTRYPOINT ["/entrypoint.sh"]
CMD ["/run-services.sh"]
