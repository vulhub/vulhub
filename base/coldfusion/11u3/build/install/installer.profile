INSTALLER_UI=SILENT
SILENT_LICENSE_MODE=trial

SILENT_INSTALLER_TYPE=standalone

SILENT_INSTALL_ODBC=false
SILENT_INSTALL_SAMPLES=false
SILENT_INSTALL_JNBRIDGE=false
SILENT_INSTALL_ADMIN=true
SILENT_INSTALL_SOLR=false
SILENT_INSTALL_PDFG=false
SILENT_INSTALL_FOLDER=/opt/coldfusion11

#Enable one of Prod(secure/prod)/dev profile. Thse 3 are mutually exclusive and only one of them can be ture and others are false.
SILENT_ENABLE_PROD_SECURE_PROFILE=false
SILENT_ENABLE_PROD_DEFAULT_PROFILE=true
SILENT_ENABLE_DEV_PROFILE=false

SILENT_ADMIN_IP=
SILENT_ADMIN_USERNAME=admin
SILENT_ADMIN_PASSWORD=vulhub
SILENT_ENABLE_RDS=false

# Runtime user, typically nobody.
SILENT_RUNTIME_USER=coldfusion

SILENT_JETTY_USERNAME=admin
SILENT_JETTY_PASSWORD=vulhub

SILENT_CONTEXT_ROOT=
SILENT_AUTO_ENABLE_UPDATES=true

#Enable/Disable the servlets depending on if they are not used by your application or not.
#Applicable only if production OR Production+Secure profile is enabled. For Dev profile all these are enabled by default.

ENABLE_RDS=false
ENABLE_WSRP=false
ENABLE_JSDEBUG=false
ENABLE_CFR=false
ENABLE_CFSWF=true
ENABLE_CFFORMS=true
