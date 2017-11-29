#!/usr/bin/env bash

if [ -f /.jboss_admin_pass_configured ]; then
    echo "JBoss admin user's password has been configured!"
    exit 0
fi


#generate password
PASS=${JBOSS_PASS:-"vulhub"}
_word=$( [ ${JBOSS_PASS} ] && echo "preset" || echo "default" )

echo "=> Configuring admin user with a ${_word} password in JBoss"
echo "admin:${PASS}" > /jboss-6.1.0.Final/server/default/conf/props/jmx-console-users.properties
echo "admin:${PASS}" > /jboss-6.1.0.Final/server/default/conf/props/jbossws-users.properties
echo "=> Done!"
echo "========================================================================"
echo "You can now configure to this JBoss server using:"
echo ""
echo "    admin:${PASS}"
echo ""
echo "========================================================================"

touch /.jboss_admin_pass_configured
