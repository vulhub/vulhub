#!/bin/sh
#
# $Header: rdbms/install/instantclient/sdk/demo/setuporamysql.sh /main/3 2013/03/24 19:05:29 sdhamoth Exp $
#
# setuporamysql.sh
#
# Copyright (c) 2012, 2013, Oracle and/or its affiliates. All rights reserved. 
#
#    NAME
#      setuporamysql.sh - Setup a directory emmulating MySQL Connector/C
#
#    DESCRIPTION
#      Creates a directory structure that programs can use to build
#      with the liboramysql driver. This script may need to be
#      modified for your purposes.
#
#    NOTES
#      Pre-requisistes:
#        - MySQL client header files must be installed
#        - An ORACLE_HOME or Oracle Instant Client Basic or Basic Lite package must be installed
#     
#      Usage:
#        setuporamysql.sh [mysql_include_dir] [oracle_library_dir]
#     
#        If the MySQL include header file directory is not given, then
#        setuporamysql.sh looks for header files under /usr/include.
#     
#        If the Oracle library directory argument is not given, then
#        setuporamysql.sh looks for an Oracle Instant Client RPM
#        installation.
#     
#      Examples:
#        $ setuporamysql.sh
#        $ setuporamysql.sh /usr/include/mysql
#        $ setuporamysql.sh /usr/include /opt/oracle/instantclient_12_1
#        $ setuporamysql.sh /opt/mysql-connector-c-6.0.2-linux-glibc2.3-x86-64bit /opt/oracle/instantclient_12_1
#        $ setuporamysql.sh /usr/include/mysql /u01/app/oracle/product/12.1.0/dbhome_1/lib
#     
#      Output:
#        A directory called 'oramysql' is created in the current
#        working directory. This can be used to build programs and
#        tools with liboramysql. For example if the new directory is
#        in /opt/oramysl, then PHP MySQL extensions can be built with:
#          $ ./configure --with-mysql=/opt/oramysql --with-mysqli=/opt/oramysql/bin/mysql_config --with-pdo-mysql=/opt/oramysql
#        Python's MySQL-python can be built with:
#          $ export PATH=/opt/oramysql/bin:$PATH
#          $ python setup.py build && python setup.py install
#        Ruby's MySQL gem can be installed with:
#          $ export PATH=/opt/oramysql/bin:$PATH
#          $ export LD_LIBRARY_PATH=/opt/oracle/instantclient_12_1
#          $ gem install mysql
#
#    MODIFIED   (MM/DD/YY)
#    cjones      03/22/13 - use any headers
#    slari       12/21/12 - XbranchMerge slari_bug-14558465 from
#                           st_rdbms_12.1.0.1
#    cjones      11/28/12 - add basic lite & ORACLE_HOME support
#    slari       11/07/12 - b14614117: add setuporamysql.sh in sdk.zip
#    cjones      11/07/12 - Creation
#


# Constants
SO=.so                                    # Liboramysql file extension
ORACLEVER=12                              # Library suffix version number 
LIBORAMYSQL=liboramysql${ORACLEVER}${SO}  # Liboramysql library name

# Output settings
ORAMYSQLDIR=oramysql # Output directory to create
VERSION=6.0.2        # The version number that mysql_config will return
LIBMYSQLVER=16       # Library version number that libmysql will use

# Tools
SED=sed
TAIL1=`echo a | tail -n1 2> /dev/null`
if [ "$TAIL1" = "a" ]; then
    TAIL1="tail -n1"
else
    # e.g. Solaris
    TAIL1="tail -1" 
fi

# Check parameters
[ $# -gt 2 ] && { echo "Usage: $0 [mysql_include_dir] [oracle_library_dir]" >&2; exit 1; }

if [ $# -eq 0 ]; then
    MYINCDIR=/usr/include
else
    MYINCDIR=$1
fi

CWD=`pwd`

# Find MySQL include files
cd $MYINCDIR 2> /dev/null && MYINCDIR=`pwd` && cd $CWD
if [ -f $MYINCDIR/mysql/mysql.h ]; then
    MYINCDIR=$MYINCDIR/mysql
elif [ -f $MYINCDIR/include/mysql.h ]; then
    MYINCDIR=$MYINCDIR/include
elif [ ! -f $MYINCDIR/mysql.h ]; then
    echo "Can't locate $MYINCDIR/mysql.h" >&2
    exit 1;
fi

# Find Oracle client shared library directory
if [ $# -eq 2 ]; then
    ORCLLIBDIR=`ls -d $2/libclntsh.* 2> /dev/null | $TAIL1 | $SED -e 's#/libclntsh[^/]*##'`
else
    # Look for the highest version Instant Client RPM library directory
    ORCLLIBDIR=`ls -d /usr/lib/oracle/*/client*/lib/libclntsh.* 2> /dev/null | $TAIL1 | $SED -e 's#/libclntsh[^/]*##'`
fi
if [ -z "$ORCLLIBDIR" ]; then
    echo "Can't locate Oracle Client libraries" >&2
    echo "Usage: $0 [mysql_include_dir] [oracle_library_dir]" >&2
    exit 1
fi
cd $ORCLLIBDIR 2> /dev/null && ORCLLIBDIR=`pwd` && cd $CWD

# Make new directory structure
TMP=`ls $ORAMYSQLDIR 2> /dev/null`
[ -n "$TMP" ] && { echo "Directory $ORAMYSQLDIR already exists" >&2; exit 1; }
mkdir -p $ORAMYSQLDIR/bin $ORAMYSQLDIR/lib 2> /dev/null || { echo "Can't create $ORAMYSQLDIR directory structure" >&2; exit 1; }

echo "Creating libmysql $ORAMYSQLDIR/include link"
ln -s $MYINCDIR $ORAMYSQLDIR/include

echo "Creating libmysql $ORAMYSQLDIR/lib links"
ln -s $ORCLLIBDIR/$LIBORAMYSQL $ORAMYSQLDIR/lib/libmysql${SO}.${LIBMYSQLVER}.0.0
ln -s $ORCLLIBDIR/$LIBORAMYSQL $ORAMYSQLDIR/lib/libmysql${SO}.${LIBMYSQLVER}
ln -s $ORCLLIBDIR/$LIBORAMYSQL $ORAMYSQLDIR/lib/libmysql${SO}
ln -s $ORCLLIBDIR/$LIBORAMYSQL $ORAMYSQLDIR/lib/libmysqlclient${SO}
ln -s $ORCLLIBDIR/$LIBORAMYSQL $ORAMYSQLDIR/lib/libmysqlclient_r${SO}
# The following are needed for PHP "mysql" extension configuration
ln -s $ORCLLIBDIR/libclntsh${SO}.${ORACLEVER}.1     $ORAMYSQLDIR/lib
ln -s $ORCLLIBDIR/libclntshcore${SO}.${ORACLEVER}.1 $ORAMYSQLDIR/lib
ln -s $ORCLLIBDIR/libnnz${ORACLEVER}${SO}           $ORAMYSQLDIR/lib
ln -s $ORCLLIBDIR/libons${SO}                       $ORAMYSQLDIR/lib
[ -f $ORCLLIBDIR/libociei${SO} ] && { ln -s $ORCLLIBDIR/libociei${SO} $ORAMYSQLDIR/lib; }
[ -f $ORCLLIBDIR/libociicus${SO} ] && { ln -s $ORCLLIBDIR/libociicus${SO} $ORAMYSQLDIR/lib; }

echo "Creating $ORAMYSQLDIR/bin/mysql_config"
cat > $ORAMYSQLDIR/bin/mysql_config <<EOF
#!/bin/sh

# Find absolute path of script directory
case \$0 in
    /*) BDIR=\`echo \$0 | sed s/\\\`basename \$0\\\`\$//\`;;
     *) BDIR=\`pwd\`/\`echo \$0 | sed s/\\\`basename \$0\\\`\$//\`;;
esac
BDIR=\`echo \$BDIR | $SED -e 's#/\$##' -e 's#\/.\$##'\`

MYINCDIR=\$BDIR/../include
cd \$MYINCDIR && MYINCDIR=\`pwd\`
MYLIBDIR=\$BDIR/../lib
cd \$MYLIBDIR && MYLIBDIR=\`pwd\`

function usage
{
    echo "Usage: \$0 [OPTIONS]
    --cflags            [-I\$MYINCDIR]
    -?, --help          Display this help and exit.
    --include           [-I\$MYINCDIR]
    --libs              [-L\$MYLIBDIR -lmysql]
    --libs_r            [-L\$MYLIBDIR -lmysql]
    --version           [$VERSION]" >&2
    exit 1
}

if [ \$# -ne 1 -o "\$1" = "-?" -o "\$1" = "--help" ]; then
    usage
fi

case "\$1" in
"--cflags")  echo "-I\$MYINCDIR";;
"--include") echo "-I\$MYINCDIR";;
"--libs")    echo "-L\$MYLIBDIR -lmysql";;
"--libs_r")  echo "-L\$MYLIBDIR -lmysql";;

"--version") echo "$VERSION";;
*)           echo "mysql_config: unknown option '\$1'" >&2
             exit 2;;
esac

exit 0
EOF

chmod 755 $ORAMYSQLDIR/bin/mysql_config

exit 0
