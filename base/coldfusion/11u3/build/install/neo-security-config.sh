#!/bin/bash
if [ $# -lt 2 ]; then
  echo "This script can be used to toggle admin security on and off in a ColdFusion 10 server instance."
  echo ""
  echo "Usage: $0 <cf_home> <admin_security>"
  echo "where <cf_home> points to the root of a CF server instance, eg. /opt/coldfusion10/cfusion"
  echo "      <admin_security> specifies the desired admin security state. Specify 'false' to turn it off, and 'true' for on. "
  echo ""
  exit
fi

CF_HOME=$1
ADMIN_SECURITY=$2
XSL_FILE=/tmp/neo-security-config-$$.xsl

if [ ! -d "$CF_HOME" ]; then
        echo "$CF_HOME doesn't seem to exist. Terminating."
        exit 1
fi

if [ "$ADMIN_SECURITY" != "false" ] && [ "$ADMIN_SECURITY" != "true" ]; then
        echo "Invalid value for <admin_security>: $ADMIN_SECURITY. Please specify either 'true' or 'false'. "
        exit 1
fi

if [ ! -e $CF_HOME/lib/neo-security.xml.original ]; then
        echo "Backing up original neo-security.xml to $CF_HOME/lib/neo-security.xml.original .."
        cp $CF_HOME/lib/neo-security.xml $CF_HOME/lib/neo-security.xml.original
        if [ $? != 0 ]; then
          echo "cp failed with exit code $?. Terminating."
          exit 1
        fi
else
        echo "$CF_HOME/lib/neo-security.xml.original already exists."
fi

echo "Writing XSL to $XSL_FILE .."
cat > $XSL_FILE <<'EOXSL'
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
  <xsl:output method="xml" omit-xml-declaration="yes" indent="yes" />
  <xsl:param name="adminSecurityEnabled" />
  <xsl:template match="/wddxPacket/data/struct[@type='coldfusion.server.ConfigMap']/var[@name='admin.security.enabled']/boolean/@value">
    <xsl:attribute name="value">
       <xsl:value-of select="$adminSecurityEnabled" />
        </xsl:attribute>
  </xsl:template>
  <xsl:template match="@*|node()">
    <xsl:copy>
      <xsl:apply-templates select="@*|node()"/>
    </xsl:copy>
  </xsl:template>
</xsl:stylesheet>
EOXSL
if [ $? != 0 ]; then
  echo "Failed to write XSL template to $XSL_FILE. Terminating."
  exit 1
fi

echo "Modifying neo-security.xml .."
xsltproc --stringparam adminSecurityEnabled $ADMIN_SECURITY $XSL_FILE $CF_HOME/lib/neo-security.xml.original > $CF_HOME/lib/neo-security.xml
if [ $? != 0 ]; then
  echo "xsltproc failed with exit code $?. Terminating."
  exit 1
fi

echo "Removing $XSL_FILE .."
rm $XSL_FILE
if [ $? != 0 ]; then
  echo "rm failed with exit code $?. You may need to clean up manually."
  exit 1
fi
