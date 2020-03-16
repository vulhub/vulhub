/* $RCSfile: xmlotn.h $ $Date: 2015/04/06 13:13:10 $
 Copyright (c) 2001, 2015, Oracle and/or its affiliates. All rights reserved.

  #####################################################################
  # !!!!WARNING!!!!   THIS HEADER GENERATES DOCUMENTATION DIRECTLY!   #
  # The syntax of Package/Interface/function block, etc is specific & #
  # must be maintained properly or documentation will not generate!!  #
  #####################################################################

   FILE NAME
        xmlotn.h - Oracle XML APIs for OTN

   FILE DESCRIPTION
        This header file implements the Unified XML interfaces.  This
        front end can be used to transparently invoke either the XDK
        or XDB (XMLType) back-ends, using the same APIs.  The XDK package
        is used for traditional XML documents read from files, URIs,
        stdio, etc.  The XDB package is for documents stored in an
        Oracle database.

                               +-----------+
                               |  Unified  |
                               |    XML    |
                               +-----------+
                              /             \
                             /               \
                         +-----+           +-----+
                         | XDK |           | XDB |
                         +-----+           +-----+
                            ^                 ^
                            |                 |
                         File, URI         XMLType
                       stdio, buffer     (Oracle DB)
                    user-defined stream

        All Unified XML Interfaces start with "Xml".
        All XMLType interfaces start with "OCIXml".

        *       *       *       *       *       *       *       *

   Before using the Unified XML APIs, an xmlctx must be obtained.  This
   context must be passed to all XML APIs.  The creation of this context
   is the only time implementation-dependent calls are used.

   For the XML Developers Toolkit (XDK), the creator is:

        xmlctx *XmlCreate(xmlerr *err, oratext *name, ...);

        *       *       *       *       *       *       *       *
*/

#ifndef XMLOTN_ORACLE
# define XMLOTN_ORACLE

#ifndef XMLERR_ORACLE
# include <xmlerr.h>
#endif

#ifndef XMLPROC_ORACLE
# include <xmlproc.h>
#endif

#ifndef XMLXSL_ORACLE
# include <xmlxsl.h>
#endif

#ifndef XMLXVM_ORACLE
# include <xmlxvm.h>
#endif

#ifndef XMLSCH_ORACLE
# include <xmlsch.h>
#endif

#ifndef XMLXPTR_ORACLE
# include <xmlxptr.h>
#endif

#ifndef XMLDAV_ORACLE
# include <xmldav.h>
#endif

#ifndef XMLDF_ORACLE
# include <xmldf.h>
#endif

#define XML_VERSION     BNRVERSION_STR 
#define XML_STATUS      BNRSTATUS_STR

#endif                                              /* XMLOTN_ORACLE */
