/* $RCSfile: xmlxptr.h $ $Date: 2016/03/08 08:04:25 $
 Copyright (c) 2001, 2016, Oracle and/or its affiliates. All rights reserved.

   FILE NAME
        xmlxptr.h - Oracle XML XPointer APIs

   FILE DESCRIPTION
        This files defines the XPointer interfaces.
*/


#ifndef XMLXPTR_ORACLE
# define XMLXPTR_ORACLE

#ifndef STDIO_ORACLE
# define STDIO_ORACLE
# include <stdio.h>
#endif

#ifndef STDARG_ORACLE
# define STDARG_ORACLE
# include <stdarg.h>
#endif

#ifndef ORATYPES_ORACLE
# define ORATYPES_ORACLE
# include <oratypes.h>
#endif

#ifndef XMLERR_ORACLE
# include <xmlerr.h>
#endif

#ifndef XMLPROC_ORACLE
# include <xmlproc.h>
#endif

/*---------------------------------------------------------------------------
                       Package XPointer - XPointer APIs
  ---------------------------------------------------------------------------*/

/* DATATYPE xmlxptrloc - XPointer location data type
   MISSING FULL DESCRIPTION OF THIS TYPE
*/
typedef struct xmlxptrloc xmlxptrloc;

/* DATATYPE xmlxptrloctype - XPointer location type
   MISSING FULL DESCRIPTION OF THIS TYPE
*/
typedef enum xmlxptrloctype {
  XMLXPTRLOCTYPE_UNDEF     = 0,
  XMLXPTRLOCTYPE_NODE      = 1,
  XMLXPTRLOCTYPE_POINT     = 2,
  XMLXPTRLOCTYPE_RANGE     = 3,
  XMLXPTRLOCTYPE_BOOL      = 4,
  XMLXPTRLOCTYPE_NUM       = 5,
  XMLXPTRLOCTYPE_STR       = 6
} xmlxptrloctype;

/* DATATYPE xmlpoint - XPointer point location
   MISSING FULL DESCRIPTION OF THIS TYPE
*/
typedef struct xmlpoint xmlpoint;

/****************************************************************************
                               Interface XPtrLoc
 ****************************************************************************/

/*------------------------------------------------------------------------
    NAME
        XmlXPtrLocGetType

    PURPOSE
        Returns type of XPtrLoc

    DESCRIPTION
        Returns type of location

    RETURNS
        (xmlxptrloctype) Type of location

    ARGUMENTS
        loc (IN) location
*/
xmlxptrloctype XmlXPtrLocGetType(xmlxptrloc* loc);

/*------------------------------------------------------------------------
    NAME
        XmlXPtrLocGetNode

    PURPOSE
        Returns xmlnode from XPtrLoc

    DESCRIPTION
        Returns node from location

    RETURNS
        (xmlnode *) Node from location

    ARGUMENTS
        loc (IN) location
*/
xmlnode* XmlXPtrLocGetNode(xmlxptrloc* loc);

/*------------------------------------------------------------------------
    NAME
        XmlXPtrLocGetPoint

    PURPOSE
        Returns xmlpoint from XPtrLoc

    DESCRIPTION
        Returns point from location

    RETURNS
        (xmlpoint *) Point from location

    ARGUMENTS
        loc (IN) location
*/
xmlpoint* XmlXPtrLocGetPoint(xmlxptrloc* loc);

/*------------------------------------------------------------------------
    NAME
        XmlXPtrLocGetRange

    PURPOSE
        Returns xmlrange from XPtrLoc

    DESCRIPTION
        Returns range from location

    RETURNS
        (xmlrange *) Range from location

    ARGUMENTS
        loc (IN) location
*/
xmlrange* XmlXPtrLocGetRange(xmlxptrloc* loc);

/*------------------------------------------------------------------------
    NAME
        XmlXPtrLocToString

    PURPOSE
        Returns string for a location

    DESCRIPTION
        Returns string for a location:
          - node name
          - name of the container node
          - names of container nodes
          - "not a location" otherwise

    RETURNS
        (oratext *) string

    ARGUMENTS
        loc (IN) location
*/
oratext* XmlXPtrLocToString(xmlxptrloc* loc);

/****************************************************************************
                             Interface XPtrLocSet
 ****************************************************************************/

/* DATATYPE xmlxptrlocset - XPointer location set data type
   MISSING FULL DESCRIPTION OF THIS TYPE
*/
typedef struct xmlxptrlocset xmlxptrlocset;

/*------------------------------------------------------------------------
    NAME
        XmlXPtrLocSetGetLength

    PURPOSE
        Returns length of XPtrLocSet

    DESCRIPTION
        Returns the number of locations in the location set

    RETURNS
        (ub4) number of nodes in locset

    ARGUMENTS
        locset (IN) location set
*/
ub4 XmlXPtrLocSetGetLength(xmlxptrlocset* locset);

/*------------------------------------------------------------------------
    NAME
        XmlXPtrLocSetGetItem

    PURPOSE
        Returns location with idx position in XPtrLocSet

    DESCRIPTION
        Returns location with idx position in the location set.
        First posistion is 1.

    RETURNS
        (xmlxptrloc *) location with the position idx

    ARGUMENTS
        locset (IN) location set
        idx    (IN) location index
*/
xmlxptrloc* XmlXPtrLocSetGetItem(xmlxptrlocset* locset, ub4 idx);

/*------------------------------------------------------------------------
    NAME
        XmlXPtrLocSetFree

    PURPOSE
        Free a location set

    DESCRIPTION
        It is user reponsibility to call this function on every
        location set returned by XPointer or XPtrLocSet interfaces

    RETURNS
        (void)

    ARGUMENTS
        locset (IN) location set
*/
void XmlXPtrLocSetFree(xmlxptrlocset* locset);

/****************************************************************************
                               Interface XPointer
 ****************************************************************************/

/*------------------------------------------------------------------------
    NAME
        XmlXPointerEval

    PURPOSE
        Evaluates xpointer string

    DESCRIPTION
        Parses and evaluates xpointer string and calculates locations in
        the document

    RETURNS
        (xmlxptrlocset *) calculated location set

    ARGUMENTS
        doc     (IN) document node of the corresponding DOM tree
        xptrstr (IN) xpointer string
*/
xmlxptrlocset* XmlXPointerEval(xmldocnode* doc, oratext* xptrstr);

#endif                                              /* XMLXPTR_ORACLE */
