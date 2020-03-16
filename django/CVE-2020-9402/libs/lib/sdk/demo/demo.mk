#/bin/make
###############################################################################
#                       Make file for OCI and OCCI demos
###############################################################################
#  Usage :
# For compiling & linking the cdemo81.c file
#    make -f demo.mk buildoci EXE=cdemo81 OBJS=cdemo81.o 
#
# For compiling & linking the occidml.cpp
#    make -f demo.mk buildocci EXE=occidml OBJS=occidml.o
#
# For compiling & linking the occiobj.cpp
#    make -f demo.mk occiobj 
#
# In general, for any occi program
#    make -f demo.mk buildocci EXE=<exename> OBJS="<list of dependent objs>"
#
# For compiling all demos
#    make -f demo.mk
#
# NOTE: Please change cc and CC to point to the appropiate location on your
#       machine.
#
###############################################################################

.SUFFIXES: .o .c .cpp

CC=/opt/SunProd/SUNWspro6.1/bin/CC
cc=/opt/SunProd/SUNWspro6.1/bin/cc

ICINCHOME=../
ICLIBHOME=../../
ICLIBPATH=-L$(ICLIBHOME)
THREADLIBS=-lthread
CCLIB=$(ICLIBPATH) -locci -lclntsh $(THREADLIBS)

CCINCLUDES = -I$(ICINCHOME)include

CCFLAGS=$(CCINCLUDES) -D_REENTRANT -g -xs 
LDFLAGS=
SO_EXT=.so

REMOVE=rm -rf
MAKE=make
MAKEFILE=demo.mk
CLNCACHE=cleancache
CACHEDIR=SunWS_cache

CDEMOEXE=cdemo81
CDEMOOBJS=cdemo81.o
OCCIDEMO=occidml
OCCIOBJDEMO=occiobj
OTT=../ott
OCCIOTTUSR=scott
OCCIOTTPWD=tiger

.cpp.o:
	$(CC) -c -I$(ICINCHOME)include $(CCFLAGS) $<

.c.o:
	$(cc) -c -I$(ICINCHOME)include $(CCFLAGS) $<

all: clean buildoci $(OCCIDEMO) $(OCCIOBJDEMO)

buildoci: $(CLNCACHE) $(LIBCLNT) $(CDEMOOBJS)
	$(CC) -o $(CDEMOEXE) $(LDFLAGS) $(CDEMOOBJS) $(CCLIB)

buildocci: $(CLNCACHE) $(LIBCLNT) $(OBJS)
	$(CC) -o $(EXE) $(LDFLAGS) $(OBJS) $(CCLIB)

$(OCCIDEMO):
	$(MAKE) -f $(MAKEFILE) buildocci OBJS=$@.o EXE=$@

$(OCCIOBJDEMO):
	$(OTT) userid=$(OCCIOTTUSR)/$(OCCIOTTPWD) \
                intype=$@.typ \
                outtype=$@out.type \
                code=cpp \
                hfile=$@.h \
                cppfile=$@o.cpp \
                attraccess=private \
                unicode=none
	$(MAKE) -f $(MAKEFILE) buildocci OBJS="$@.o $@m.o $@o.o" EXE=$@

cleancache:
	$(REMOVE) $(CACHEDIR)

clean: $(CLNCACHE)
	$(REMOVE) cdemo81 cdemo81.o occidml occidml.o occiobj occiobj.o occiobjo* occiobjm* occiobj.h occiobjout.type



#
# This port-specific file is currently empty on Solaris. Product
# lines may use this file to override compiler definitions and
# flags used in occi.mk.
#

# Linux compiler definitions
CC=/usr/bin/g++
cc=/usr/bin/gcc

ifeq ($(BUILD32),T)
CCFLAGS=$(CCINCLUDES) -DLINUX -D_GNU_SOURCE -D_REENTRANT -g -m32
LDFLAGS=-g -m32
else
CCFLAGS=$(CCINCLUDES) -DLINUX -D_GNU_SOURCE -D_REENTRANT -g
LDFLAGS=-g
endif
CLNCACHE=

# This macro CCINCLUDES has to be redefined on Linux because of
# the existence of the 'new' directory in t_work. The name new
# clashes with a system header file.
CCINCLUDES = -I$(SRCHOME)/rdbms/public/ \
-I$(SRCHOME)/oracore/include -I$(SRCHOME)/oracore/public \
-I$(SRCHOME)/oracore/port/include \
-I$(SRCHOME)/nlsrtl/include -I$(SRCHOME)/plsql/public \
-I$(SRCHOME)/plsql/include -I$(SRCHOME)/network/public \
-I$(SRCHOME)/network/include -I$(SRCHOME)/otrace/public \
-I$(SRCHOME)/otrace/include/ -I$(SRCHOME)/precomp/public \
-I$(SRCHOME)/precomp/include/ -I$(SRCHOME)/slax/include \
-I$(SRCHOME)/ordts/public -I$(SRCHOME)/ordts/include \
-I$(SRCHOME)/javavm/include \
-I$(SRCHOME)/javavm/include/osds/unix/solaris \
-I$(SRCHOME)/ctx/public -I$(SRCHOME)/ordvir/public \
-I$(SRCHOME)/ordvir/include -I$(SRCHOME)/rdbms/src/hdir \
-idirafter .

THREADLIBS=-lpthread

ifdef BUILD_ICC
COMPDIR=/usr/local/packages/icc_remote/10.1.022
CC=$(COMPDIR)/bin/icpc
CCFLAGS += -DOCCI_NO_WSTRING=1
endif

