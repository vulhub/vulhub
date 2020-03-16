/* $RCSfile: xml.h $ $Date: 03-nov-2003.16:31:15 $
   Copyright (c) 2001, 2003, Oracle.  All Rights Reserved.

  #####################################################################
  # !!!!WARNING!!!!   THIS HEADER GENERATES DOCUMENTATION DIRECTLY!   #
  # The syntax of Package/Interface/function block, etc is specific & #
  # must be maintained properly or documentation will not generate!!  #
  #####################################################################

   FILE NAME
        xml.h - Oracle XML APIs

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


#ifndef XML_ORACLE
# define XML_ORACLE

#ifndef XMLOTN_ORACLE
# include <xmlotn.h>
#endif

#endif /* XML_ORACLE */
