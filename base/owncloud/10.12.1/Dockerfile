FROM owncloud/server:10.12.1

LABEL maintainer="phithon <root@leavesongs.com>"

RUN set -ex \
    && sed -i 's/public function isTrustedDomain($domainWithPort) {/public function isTrustedDomain($domainWithPort) { return true;/' \
       /var/www/owncloud/lib/private/Security/TrustedDomainHelper.php
