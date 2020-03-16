/* $RCSfile: xmlev.h $ $Date: 2016/02/29 17:11:43 $
 Copyright (c) 2004, 2016, Oracle and/or its affiliates. All rights reserved.

   FILE NAME
        xmlev.h - Oracle XML Events API

   FILE DESCRIPTION
     This header files implements the StAX based XML Parsing. We provide
     two features here
     1. An interface for the user to implement their own pull-based parsing.
     2. XDKs implementation of the pull-based parsing (Pull Parser).
     After creating the initial context, subsequent calls to XmlEvNext
     advance parsing to the next event.
     Additional information about events may be obtained via calls to 
     XmlEvGetXXXX functions. There are two flavors of the Get APIs. The API
     suffixed with '0' returns the string as null-terminated. The other returns
     the string plus the length of the string (may not be null-terminated)

     In the XDK's implementation of Pull-based paring, the Pull parser is
     initialized with a call to XmlEvCreatePPCtx. The context is destroyed
     with a call to XmlEvDestroyPPCtx. After initializing the context, the
     document must be loaded with a call to XmlEvLoadPPDoc. XmlEvCleanPPCtx
     can be used to clean up resources associated with the recent parse.

 
   EXPORT FUNCTION(S) 
     XmlEvIsVersionSpecified   - returns TRUE if version is specified
                                 START_DOCUMENT event
     XmlEvGetVersion           - Get version string from XML Decl
                                 START_DOCUMENT event
     XmlEvIsEncodingSpecified  - TRUE is encoding is specified in XML Decl
                                 START_DOCUMENT event
     XmlEvIsStandalone         - returns standalone value
                                 START_DOCUMENT event
     XmlEvIsPrologSpecified    - returns TRUE if prolog is present
                                 START_DOCUMENT event
     XmlEvNext                 - advances parsing to the next event, 
                                 returns event
     XmlEvNextTag              - advances parsing to the next tag event, 
                                 returns event
     XmlEvSkipAttrs            - advances parsing to the next non-attr 
                                 event
     XmlEvGetEncoding          - get the encoding value from XML decl
                                 START_DOCUMENT event
     XmlEvGetName              - returns name of the tag event for
     XmlEvGetName0               START_ELEMENT and END_ELEMENT events

     XmlEvGetLocalName         - returns local name of the tag event
     XmlEvGetLocalName0          START_ELEMENT and END_ELEMENT events

     XmlEvGetURI               - returns URI of the tag event
     XmlEvGetURI0                START_ELEMENT and END_ELEMENT events

     XmlEvGetPrefix            - returns prefix of the tag event
     XmlEvGetPrefix0             START_ELEMENT and END_ELEMENT events

     XmlEvGetStartPos          - returns start position of a tag
                                 START_ELEMENT event

     XmlEvGetEndPos            - returns end position of a tag
                                 START_ELEMENT event

     XmlEvGetAttrCount         - returns number of attributes for start tag
                                 START_ELEMENT

     XmlEvIsNamespaceAttr      - returne TRUE is attribute is namespace attr
                                 START_ELEMENT (while traversing the attribute
                                 list)

     XmlEvGetAttrName          - returns name of the attribute
     XmlEvGetAttrName0           START_ELEMENT (while traversing the attribute
                                 list)
     XmlEvGetAttrLocalName     - returns local name of the attribute
     XmlEvGetAttrLocaName0       START_ELEMENT (while traversing the attribute
                                 list)

     XmlEvGetAttrURI           - returns URI of the attribute
     XmlEvGetAttrURI0            START_ELEMENT (while traversing the attribute
                                 list)

     XmlEvGetAttrPrefix        - returns prefix of the attribute
     XmlEvGetAttrPrefix0         START_ELEMENT (while traversing the attribute
                                 list)

     XmlEvGetAttrValue         - returns value of the attribute
     XmlEvGetAttrValue           START_ELEMENT (while traversing the attribute
                                 list)

     XmlEvGetText              - returns text 
                                 CHARACTER, CDATA, COMMENT events

     XmlEvGetPIData            - returns data of PI events
     XmlEvGetPIData0           - 

     XmlEvGetPITarget          - returns target of PI events
     XmlEvGetPITarget0

     XmlEvGetAttrDeclCount     - returns number of attributes in 
                                 DTD attribute declaration

     XmlEvGetAttrDeclElName    - returns element name in DTD
     XmlEvGetAttrDeclElName0     attribute declaration

     XmlEvGetAttrDeclName      - returns attribute qname in DTD
     XmlEvGetAttrDeclName0       attribute declaration

     XmlEvGetAttrDeclLocalName - returns attribute localname in DTD
     XmlEvGetAttrDeclLocalName0  attribute declaration

     XmlEvGetAttrDeclURI       - returns uri in DTD attribute declaration
     XmlEvGetAttrDeclURI0

     XmlEvGetAttrDeclPrefix    - returns prefix in DTD attribute declaration
     XmlEvGetAttrDeclPrefix0 

     XmlEvGetAttrDeclBody      - returns body  of DTD attribute declaration
     XMlEvGetAttrDeclBody0

     XmlEvGetElDeclContent     - returns content of DTD element declaration
     XmlEvGetElDeclContent0

     XmlEvGetPEIsGen           - returns TRUE if declared Entity is General

     XmlEvGetPERepl            - returns replacement text for PE declaration
     XmlEvGetPERepl0

     XmlEvGetPubID             - returns public id of entity or notation 
     XmlEvGetPubID0              declaraion

     XmlEvGetSysID             - returns system id of enttity or notation
     XmlEvGetSysID0              declaration

     XmlEvGetUENdata           - returns ndata of UE declaraion
     XmlEvGetUENData0

     XmlEvGetTagID             - returns ID for the tag's QNAME
     XmlEvGetAttrID            - returns ID for the attribute's QNAME
     XmlEvGetTagUriID          - returns ID for the tag's URI 
     XmlEvGetAttrUriID         - returns ID for the attribute's URI   

     Pull Parser Specific APIs
     -------------------------
     XmlEvCreatePPCtx          - create a pull-parser event context
     XmlEvCreatePPCtxVA        - same as above  but with variable argument list
     XmlEvDestroyPPCtx         - destroy the pull-parser context. Context is
                                 unusable after this call.
   EXAMPLES
     xctx = XmlCreateNew(...);
     ... 
     evctx = XmlEvCreatePPCtx(xctx, ...);
     ...
     xerr = XmlEvLoadPPDoc(xctx, evctx, "file", filename[i],...);
     ...
     while(!parse_end && !ecode)
     {
        event = XmlEvNext(evctx);
        switch(event) {
          case XML_EVENT_START_DOCUMENT:
            printf("Encoding: %s\n", XmlEvGetEncoding(evctx));
            break;
          case XML_EVENT_START_ELEMENT:
            depth++;
            qname = XmlEvGetName0(evctx); 
            lname = XmlEvGetLocalName0(evctx); 
            attrCnt = XmlEvGetAttrCount(evctx);
            while (a = 0; a < attrCnt; a++)
              aname = XmlEvGetAttrName0(evctx, a);
            break;
          case XML_EVENT_END_ELEMENT:
            depth--;
            break;
          case XML_EVENT_END_DOCUMENT:
            parse_end = TRUE;
            break;
           
          ...

          case XML_EVENT_FATAL_ERROR:
            ecode = XmlEvGetError(evctx, &message);
            break;
        }
     }

     ...
 
     XmlEvDestroyPPCtx(xctx, evctx);
     XmlDestroy(xctx);
    

*/

#ifndef XMLEV_ORACLE
# define XMLEV_ORACLE

#ifndef ORATYPES
# include <oratypes.h>
#endif

#ifdef HPUX 
# if !defined(__LP64__) && !defined(__STDC_EXT__)
#  ifndef lint
typedef unsigned long long ub8;
typedef signed long long sb8;
#  else
#define ub8 unsigned long long
#define sb8 signed long long
#  endif
# endif
#endif
 

#ifndef XMLPROC_ORACLE
# include <xmlproc.h>
#endif
/*---------------------------------------------------------------------------
                     PUBLIC TYPES AND CONSTANTS
  ---------------------------------------------------------------------------*/
#define XMLEV_EXP_ENT_F     0x01  /* Expand entity mode ? */
#define XMLEV_ATTR_EVENTS_F 0x02  /* Attribute events mode */
#define XMLEV_PRESERV_WSP_F 0x04  /* does producer generate space events */
#define XMLEV_PULLPARSER_F  0x08  /* Is this XDK Pull Parser implementation */
/* DATATYPE xmlevtype - EVENT/PULL PARSER evs.
   The ev XML_EVENT_BEFORE_START is never generated but it is returned by 
   calls to XmlEvGetEvType before the first call to XmlEvNext.
   The evs XML_EVENT_ATTRIBUTE, XML_EVENT_NAMESPACE are never generated.
   The evs XML_EVENT_ENTITY_REFERENCE only generated in the case entities 
   are not expanded; the evs XML_EVENT_START_ENTITY and 
   XML_EVENT_END_ENTITY are generated otherwise.
*/
typedef enum  xmlevtype
{
  XML_EVENT_FATAL_ERROR      = -1,  /* Fatal Error */
  XML_EVENT_BEFORE_START     = 1,   /* Before Start Document */
  XML_EVENT_START_DOCUMENT,         /* Indicates Start Document */
  XML_EVENT_START_DTD,              /* Start DTD */
  XML_EVENT_END_DTD,                /* End DTD */
  XML_EVENT_NOTATION_DECLARATION,   /* Notation Decl */
  XML_EVENT_PE_DECLARATION,         /* PE Decl */
  XML_EVENT_UE_DECLARATION,         /* US Decl */
  XML_EVENT_ELEMENT_DECLARATION,    /* Element Decl */ 
  XML_EVENT_ATTLIST_DECLARATION,    /* Attribute Decl */
  XML_EVENT_START_ELEMENT,          /* Start Element */
  XML_EVENT_END_ELEMENT,            /* End Element */
  XML_EVENT_CHARACTERS,             /* Characters (text) */
  XML_EVENT_CHARACTERS_CONT,        /* Characters Continued */
  XML_EVENT_PI,                     /* Processing Instruction */
  XML_EVENT_PI_CONT,                /* Processing Instruction Continued */
  XML_EVENT_COMMENT,                /* Comment */
  XML_EVENT_COMMENT_CONT,           /* Comment Continued */
  XML_EVENT_SPACE,                  /* White Space */
  XML_EVENT_SPACE_CONT,             /* White Space Continued */
  XML_EVENT_ENTITY_REFERENCE,       /* Entity Reference */
  XML_EVENT_START_ATTR,             /* Start Attribute */
  XML_EVENT_END_ATTR,               /* End Attribute */
  XML_EVENT_CDATA,                  /* CDATA */
  XML_EVENT_CDATA_CONT,             /* CDATA continued */
  XML_EVENT_START_ENTITY,           /* Start Entity */
  XML_EVENT_END_ENTITY,             /* End Entity */
  XML_EVENT_END_DOCUMENT,           /* End Document */
  XML_EVENT_ERROR                   /* Error */

}xmlevtype;


/****************************************************************************** 
 * DATATYPE xmlevctx - XML EVENT top-level context
 * SEE ALSO XmlEvCreateCtx, XmlEvDestroyCtx
 * Contains document information and also EVENT specific options. 
 * Required to parse XML document in pull-parse mode.
 * Support for chaining xmlevctx's is provided thru the field
 * input_xmlevctx. The dispatch functions will call the corresponding
 * non-nul function in the chain. 
 ******************************************************************************/
typedef struct xmlevctx
{
  void             *ctx_xmlevctx;      /* Implementation dependent Ctx. 
                                        * In XDK implementation this will
                                        * be lpxctx. */

  const struct xmlevdisp *disp_xmlevctx;
  ub4              checkword_xmlevctx; /* checkword for integrity check */
  ub4              flags_xmlevctx;     /* flags to indicate the mode
                                        * attr mode, expand-entity mode */ 
  struct xmlevctx  *input_xmlevctx;    /* input xmlevctx  - chaining */
} xmlevctx;


/* type and constants used in the function */
typedef enum 
{
  XML_EVENT_ID_URI,
  XML_EVENT_ID_QNAME
}xmlevidtype;
#define XML_ERR_EVENT_NOIDCBK  0

/* The dispatch table structure */
typedef struct xmlevdisp
{
  /* Get Version */
  oratext *(*XmlEvGetVersion_xmlevdisp)(void *ctx);

  /* Is Encoding  specified in the document*/
  boolean (*XmlEvIsEncodingSpecified_xmlevdisp)(void *ctx);

  /* is standalone specified irrespective of the value*/
  sword (*XmlEvIsStandalone_xmlevdisp)(void *ctx);

  /* Fetch the next Event */
  xmlevtype (*XmlEvNext_xmlevdisp)(void *ctx);

  /* Fetch the next Tag Event - Start element, End element etc */
  xmlevtype (*XmlEvNextTag_xmlevdisp)(void *ctx);

  /* In attr mode, skip attributes to the next non-attr event  */
  xmlevtype (*XmlEvSkipAttrs_xmlevdisp)(void *ctx);

  /* Get the current data encoding */
  oratext *(*XmlEvGetEncoding_xmlevdisp)(void *ctx);

  /* Get the QName (null-terminated) */
  oratext *(*XmlEvGetName0_xmlevdisp)(void *ctx);

  /* Get the QName */
  oratext *(*XmlEvGetName_xmlevdisp)(void *ctx, ub4 *len);

  /* Get the Local Name (null-terminated) */
  oratext *(*XmlEvGetLocalName0_xmlevdisp)(void *ctx);

  /* Get the Local Name */
  oratext *(*XmlEvGetLocalName_xmlevdisp)(void *ctx, ub4 *len);

  /* Get the URI (null-terminated) */
  oratext *(*XmlEvGetURI0_xmlevdisp)(void *ctx);

  /* Get the URI */
  oratext *(*XmlEvGetURI_xmlevdisp)(void *ctx, ub4 *len);

  /* Get the Prefix (null-terminated) */
  oratext *(*XmlEvGetPrefix0_xmlevdisp)(void *ctx);

  /* Get the Prefix */
  oratext *(*XmlEvGetPrefix_xmlevdisp)(void *ctx, ub4 *len);

  /* Get the Start Position of element in the input stream */
  ub4     (*XmlEvGetStartPos_xmlevdisp)(void *ctx);

  /* Get the End Position of element in the input stream */
  ub4     (*XmlEvGetEndPos_xmlevdisp)(void *ctx);

  /* Get the Attrbute count of current element */
  ub4     (*XmlEvGetAttrCount_xmlevdisp)(void *ctx);

  /* Is this Namespace Attribute */
  boolean (*XmlEvIsNamespaceAttr_xmlevdisp)(void *ctx, ub4 index);

  /* Get Attribute Name (null terminated) */
  oratext *(*XmlEvGetAttrName0_xmlevdisp)(void *ctx, ub4 index);

  /* Get Attribute Name */
  oratext *(*XmlEvGetAttrName_xmlevdisp)(void *ctx, ub4 index, ub4 *len);

  /* Get Attribute Local Name (null terminated) */
  oratext *(*XmlEvGetAttrLocalName0_xmlevdisp)(void *ctx, ub4 index);

  /* Get Attribute Local Name */
  oratext *(*XmlEvGetAttrLocalName_xmlevdisp)(void *ctx, ub4 index, ub4 *len);

  /* Get Attribute URI (null terminated) */
  oratext *(*XmlEvGetAttrURI0_xmlevdisp)(void *ctx, ub4 index);

  /* Get Attribute URI */
  oratext *(*XmlEvGetAttrURI_xmlevdisp)(void *ctx, ub4 index, ub4 *len);

  /* Get Attribute Prefix (null terminated) */
  oratext *(*XmlEvGetAttrPrefix0_xmlevdisp)(void *ctx, ub4 index);

  /* Get Attribute Prefix */
  oratext *(*XmlEvGetAttrPrefix_xmlevdisp)(void *ctx, ub4 index, ub4 *len);

  /* Get Attribute Value (null terminated) */
  oratext *(*XmlEvGetAttrValue0_xmlevdisp)(void *ctx, uword index);

  /* Get Attribute Value */
  oratext *(*XmlEvGetAttrValue_xmlevdisp)(void *ctx, uword index, ub4 *len);

  /* Get Text (null terminated) */
  oratext *(*XmlEvGetText0_xmlevdisp)(void *ctx);

  /* Get Text */
  oratext *(*XmlEvGetText_xmlevdisp)(void *ctx, ub4 *len);

  /* Get Processing Instruction Data (null terminated) */
  oratext *(*XmlEvGetPIData0_xmlevdisp)(void *ctx);

  /* Get Processing Instruction Data */
  oratext *(*XmlEvGetPIData_xmlevdisp)(void *ctx, ub4 *len);

  /* Get Processing Instruction Target (null terminated) */
  oratext *(*XmlEvGetPITarget0_xmlevdisp)(void *ctx);

  /* Get Processing Instruction Target */
  oratext *(*XmlEvGetPITarget_xmlevdisp)(void *ctx, ub4 *len);

  /* Get Attribute Declaration count */
  ub4     (*XmlEvGetAttrDeclCount_xmlevdisp)(void *ctx);

  /* Get Attribute Declaration Element Name (null terminated)*/
  oratext *(*XmlEvGetAttrDeclElName0_xmlevdisp)(void *ctx);

  /* Get Attribute Declaration Element Name */
  oratext *(*XmlEvGetAttrDeclElName_xmlevdisp)(void *ctx, ub4 *len);

  /* Get Attribute Declaration Name (null terminated)*/
  oratext *(*XmlEvGetAttrDeclName0_xmlevdisp)(void *ctx, ub4 index);

  /* Get Attribute Declaration Name */
  oratext *(*XmlEvGetAttrDeclName_xmlevdisp)(void *ctx, ub4 index, ub4 *len);

  /* Get Attribute Declaration Local Name (null terminated)*/
  oratext *(*XmlEvGetAttrDeclLocalName0_xmlevdisp)(void *ctx, ub4 index);

  /* Get Attribute Declaration Local Name */
  oratext *(*XmlEvGetAttrDeclLocalName_xmlevdisp)(void *ctx, ub4 index,
                                                                   ub4 *len);

  /* Get Attribute Declaration Prefix (null terminated)*/
  oratext *(*XmlEvGetAttrDeclPrefix0_xmlevdisp)(void *ctx, ub4 index);

  /* Get Attribute Declaration Prefix */
  oratext *(*XmlEvGetAttrDeclPrefix_xmlevdisp)(void *ctx, ub4 index, ub4 *len);

  /* Get Attribute Declaration Body (null terminated)*/
  oratext *(*XmlEvGetAttrDeclBody0_xmlevdisp)(void *ctx, ub4 index);

  /* Get Attribute Declaration Body */
  oratext *(*XmlEvGetAttrDeclBody_xmlevdisp)(void *ctx, ub4 index, ub4 *len);

  /* Get Element Declaraion Content (null terminated) */
  oratext *(*XmlEvGetElDeclContent0_xmlevdisp)(void *ctx);

  /* Get Element Declaraion Content */
  oratext *(*XmlEvGetElDeclContent_xmlevdisp)(void *ctx, ub4 *len);

  /* Is this a general entitity */
  boolean (*XmlEvGetPEIsGen_xmlevdisp)(void *ctx);

  /* Get entity replacement text (null terminated) */
  oratext *(*XmlEvGetPERepl0_xmlevdisp)(void *ctx);

  /* Get entity replacement text */
  oratext *(*XmlEvGetPERepl_xmlevdisp)(void *ctx, ub4 *len);

  /* Get Public Id (null terminated) */
  oratext *(*XmlEvGetPubId0_xmlevdisp)(void *ctx);

  /* Get Public Id */
  oratext *(*XmlEvGetPubId_xmlevdisp)(void *ctx, ub4 *len);

  /* Get System Id (null terminated) */
  oratext *(*XmlEvGetSysId0_xmlevdisp)(void *ctx);

  /* Get System Id */
  oratext *(*XmlEvGetSysId_xmlevdisp)(void *ctx, ub4 *len);

  /* Get User Entity Declaration Data  (null terminated)*/
  oratext *(*XmlEvGetUENData0_xmlevdisp)(void *ctx);

  /* Get User Entity Declaration Data  )*/
  oratext *(*XmlEvGetUENData_xmlevdisp)(void *ctx, ub4 *len);

  /* Get Tag ID */
  sb8     (*XmlEvGetTagID_xmlevdisp)(void *ctx);

  /* Get Attribute Tag ID */
  sb8     (*XmlEvGetAttrID_xmlevdisp)(void *ctx, ub4 index);

  /* Get Tag URI ID */
  sb8     (*XmlEvGetTagUriID_xmlevdisp)(void *ctx);

  /* Get Attribute URI ID */
  sb8     (*XmlEvGetAttrUriID_xmlevdisp)(void *ctx, ub4 index);

  /* Get Attribute Property */
  void*   (*XmlEvGetAttrProperty_xmlevdisp)(void *ctx, ub4 index);

  /* Get Element Property */
  void*   (*XmlEvGetElProperty_xmlevdisp)(void *ctx);

  /* Get Location information */
  void    (*XmlEvGetLocation_xmlevdisp)(void *ctx, ub4 *line, oratext **path);

  /* Get Error/Error message */
  xmlerr  (*XmlEvGetError_xmlevdisp)(void *ctx, oratext **msg);

  /* Locator inside binary stream */
  void (*XmlEvGetLocator_xmlevdisp)(void *ctx, ub1 **loc, ub4 *loclen);

  /* Is Prolog Specified */
  boolean (*XmlEvIsPrologSpecified_xmlevdisp)(void *ctx);

  /* is version specified in XML declaration */
  boolean (*XmlEvIsVersionSpecified_xmlevdisp)(void *ctx);

  /* XDB specific: Get Path components from path Id */
  boolean (*XmlEvGetPathCompsFromId_xmlevdisp)(void *ctx,
                                               /* The path id */
                                               sb8  pid,
                                               /* array of comp ids */
                                               sb8  **pathcomps,
                                               /* # of entries */
                                               ub4  *pathncomps,
                                               boolean *isattr);

  /* XDB specific: Get local name from component Id */
  ub4  (*XmlEvGetLocalFromId_xmlevdisp)(void    *ctx,
                                        /* the component id */
                                        sb8     compid,
                                        /* returned local name */
                                        oratext **local,
                                        /* returned namespace id */
                                        sb8     *nmspcid,
                                        boolean *isattr);

  /* XDB specific: Get URI from component Id */
  ub4  (*XmlEvGetUriFromId_xmlevdisp)(void    *ctx,
                                      /* the component id */
                                      sb8     compid,
                                      /* returned uri name */
                                      oratext **uri);

  /* Get the localname offset in qname */
  ub1     (*XmlEvGetLocalNameOffset_xmlevdisp)(void *ctx);

  /* Get the Attribute localname offset in qname */
  ub1     (*XmlEvGetAttrLocalNameOffset_xmlevdisp)(void *ctx, ub4 index);
 
  oratext *(*XmlEvGetWsp_xmlevdisp)(void *ctx, ub4 *len, ub4 index, ub4 flag);

  boolean (*XmlEvIsEmpty_xmlevdisp)(void *ctx); 

  oratext *(*XmlEvGetBlock_xmlevdisp)(void *ctx, ub4 *len);

} xmlevdisp;

/*****************************************************************************
 * The following structures and macros are used to efficiently dispatch the  *
 * function using the chained xmlevctx's                                     *
 * The goal is to avoid a function call during disptach. So we have          *
 * laborious macros (XMLEV_DISPATCH*) that optimize for a chain length of    * 
 * depth = 5, after which a function will be called to dispatch properly     *
 *****************************************************************************/
/* Dispatch function index */
enum {
  XMLEV_FUNC_GETVERSION = 1,
  XMLEV_FUNC_ISENCODINGSPECIFIED,
  XMLEV_FUNC_ISSTANDALONE,
  XMLEV_FUNC_NEXT,
  XMLEV_FUNC_NEXTTAG,
  XMLEV_FUNC_SKIPATTRS,
  XMLEV_FUNC_GETENCODING,
  XMLEV_FUNC_GETNAME0,
  XMLEV_FUNC_GETNAME,
  XMLEV_FUNC_GETLOCALNAME0,
  XMLEV_FUNC_GETLOCALNAME,
  XMLEV_FUNC_GETURI0,
  XMLEV_FUNC_GETURI,
  XMLEV_FUNC_GETPREFIX0,
  XMLEV_FUNC_GETPREFIX,
  XMLEV_FUNC_GETSTARTPOS,
  XMLEV_FUNC_GETENDPOS,
  XMLEV_FUNC_GETATTRCOUNT,
  XMLEV_FUNC_ISNSPATTR,
  XMLEV_FUNC_GETATTRNAME0,
  XMLEV_FUNC_GETATTRNAME,
  XMLEV_FUNC_GETATTRLOCALNAME0,
  XMLEV_FUNC_GETATTRLOCALNAME,
  XMLEV_FUNC_GETATTRURI0,
  XMLEV_FUNC_GETATTRURI,
  XMLEV_FUNC_GETATTRPREFIX0,
  XMLEV_FUNC_GETATTRPREFIX,
  XMLEV_FUNC_GETATTRVALUE0,
  XMLEV_FUNC_GETATTRVALUE,
  XMLEV_FUNC_GETTEXT0,
  XMLEV_FUNC_GETTEXT,
  XMLEV_FUNC_GETPIDATA0,
  XMLEV_FUNC_GETPIDATA,
  XMLEV_FUNC_GETPITARGET0,
  XMLEV_FUNC_GETPITARGET,
  XMLEV_FUNC_GETATTRDECLCOUNT,
  XMLEV_FUNC_GETATTRDECLELNAME0,
  XMLEV_FUNC_GETATTRDECLELNAME,
  XMLEV_FUNC_GETATTRDECLNAME0,
  XMLEV_FUNC_GETATTRDECLNAME,
  XMLEV_FUNC_GETATTRDECLLOCALNAME0,
  XMLEV_FUNC_GETATTRDECLLOCALNAME,
  XMLEV_FUNC_GETATTRDECLPREFIX0,
  XMLEV_FUNC_GETATTRDECLPREFIX,
  XMLEV_FUNC_GETATTRDECLBODY0,
  XMLEV_FUNC_GETATTRDECLBODY,
  XMLEV_FUNC_GETELDECLCONTENT0,
  XMLEV_FUNC_GETELDECLCONTENT,
  XMLEV_FUNC_GETPEISGEN,
  XMLEV_FUNC_GETPEREPL0,
  XMLEV_FUNC_GETPEREPL,
  XMLEV_FUNC_GETPUBID0,
  XMLEV_FUNC_GETPUBID,
  XMLEV_FUNC_GETSYSID0,
  XMLEV_FUNC_GETSYSID,
  XMLEV_FUNC_GETUENDATA0,
  XMLEV_FUNC_GETUENDATA,
  XMLEV_FUNC_GETTAGID,
  XMLEV_FUNC_GETATTRID,
  XMLEV_FUNC_GETTAGURIID,
  XMLEV_FUNC_GETATTRURIID,
  XMLEV_FUNC_GETATTRPROPERTY,
  XMLEV_FUNC_GETELPROPERTY,
  XMLEV_FUNC_GETLOCATION,
  XMLEV_FUNC_GETERROR,
  XMLEV_FUNC_GETLOCATOR,
  XMLEV_FUNC_ISPROLOGSPECIFIED,
  XMLEV_FUNC_ISVERSIONSPECIFIED,
  XMLEV_FUNC_GETPATHCOMPSFROMID,
  XMLEV_FUNC_GETLOCALFROMID,
  XMLEV_FUNC_GETURIFROMID,
  XMLEV_FUNC_GETLOCALNAMEOFFSET,
  XMLEV_FUNC_GETATTRLOCALNAMEOFFSET,
  XMLEV_FUNC_GETWSP,
  XMLEV_FUNC_ISEMPTY,
  XMLEV_FUNC_GETBLOCK
};

/***************************************************************************
 * As part of dispatching the functions we want to minimize functions calls
 * For this we use the macros below upto a certain depth after which we
 * call these functions appropriately
 ***************************************************************************/
boolean   XmlEvDispatch1(xmlevctx *evctx, ub4 name);
xmlevtype XmlEvDispatch2(xmlevctx *evctx, ub4 name);
oratext*  XmlEvDispatch3_0(xmlevctx *evctx, ub4 name);
oratext*  XmlEvDispatch3(xmlevctx *evctx, ub4 name, ub4 *len);
oratext*  XmlEvDispatch4_0(xmlevctx *evctx, ub4 name, ub4 index);
oratext*  XmlEvDispatch4(xmlevctx *evctx, ub4 name, ub4 index, ub4 *len);
ub4       XmlEvDispatch5(xmlevctx *evctx, ub4 name);
sb8       XmlEvDispatch6(xmlevctx *evctx, ub4 name);
sb8       XmlEvDispatch7(xmlevctx *evctx, ub4 name, ub4 index);
void*     XmlEvDispatch8(xmlevctx *evctx, ub4 name, ub4 index);
void*     XmlEvDispatch9(xmlevctx *evctx, ub4 name);
void      XmlEvDispatch10(xmlevctx *evctx, ub4 name, ub4 *line, oratext **path);
void      XmlEvDispatch11(xmlevctx *evctx, ub4 name, ub1 **loc, ub4 *loclen);
xmlerr    XmlEvDispatch12(xmlevctx *evctx, ub4 name, oratext **msg);
boolean   XmlEvDispatch13(xmlevctx *evctx, ub4 name, ub4 index);
boolean   XmlEvDispatch14(xmlevctx *evctx, ub4 name, sb8 pid, sb8 **pathcomps,
                          ub4 *pathncomps, boolean *isattr);
ub4       XmlEvDispatch15(xmlevctx *evctx, ub4 name, sb8 pathid,
                          oratext **local, sb8 *nmspcid, boolean *isattr);
ub4       XmlEvDispatch16(xmlevctx *evctx, ub4 name, sb8 nmspcid,
                          oratext **uri);
ub1       XmlEvDispatch17(xmlevctx *evctx, ub4 name);
ub1       XmlEvDispatch18(xmlevctx *evctx, ub4 name, ub4 index);
oratext*  XmlEvDispatch19(xmlevctx *evctx, ub4 name, ub4 *len, ub4 index, ub4 flag);
boolean   XmlEvDispatch20(xmlevctx *evctx, ub4 name);
oratext*  XmlEvDispatch21(xmlevctx *evctx, ub4 name, ub4 *len);

/**************************************************************************** 
 * User API's are implemented as macros which call the
 * corresponding dispatch function 
 * We maintain three version of this macro 
 * 0 - Macro for func's that take evctx as the argument
 * 1 - Macro for func's that take evctx plus 1 arguments
 * 2 - Macro for func's that take evctx plus 2 arguments
 * 3 - Macro for func's that take evctx plus 3 arguments
 * 4 - Macro for func's that take evctx plus 4 arguments
 ****************************************************************************/
#define XMLEV_DISPATCH0(evctx, func) \
  evctx->disp_xmlevctx->XmlEv##func##_xmlevdisp ? \
  evctx->disp_xmlevctx->XmlEv##func##_xmlevdisp(evctx->ctx_xmlevctx) : \
   evctx->input_xmlevctx->disp_xmlevctx->XmlEv##func##_xmlevdisp ? \
    evctx->input_xmlevctx->disp_xmlevctx->XmlEv##func##_xmlevdisp(evctx->input_xmlevctx->ctx_xmlevctx) : \
     evctx->input_xmlevctx->input_xmlevctx->disp_xmlevctx->XmlEv##func##_xmlevdisp ? \
      evctx->input_xmlevctx->input_xmlevctx->disp_xmlevctx->XmlEv##func##_xmlevdisp(evctx->input_xmlevctx->input_xmlevctx->ctx_xmlevctx) : \
       evctx->input_xmlevctx->input_xmlevctx->input_xmlevctx->disp_xmlevctx->XmlEv##func##_xmlevdisp ? \
        evctx->input_xmlevctx->input_xmlevctx->input_xmlevctx->disp_xmlevctx->XmlEv##func##_xmlevdisp(evctx->input_xmlevctx->input_xmlevctx->input_xmlevctx->ctx_xmlevctx) : \
         evctx->input_xmlevctx->input_xmlevctx->input_xmlevctx->input_xmlevctx->disp_xmlevctx->XmlEv##func##_xmlevdisp ? \
          evctx->input_xmlevctx->input_xmlevctx->input_xmlevctx->input_xmlevctx->disp_xmlevctx->XmlEv##func##_xmlevdisp(evctx->input_xmlevctx->input_xmlevctx->input_xmlevctx->input_xmlevctx->ctx_xmlevctx) 

/* XMLEV_DISPATCH1 */
#define XMLEV_DISPATCH1(evctx, func, arg1) \
  evctx->disp_xmlevctx->XmlEv##func##_xmlevdisp ? \
  evctx->disp_xmlevctx->XmlEv##func##_xmlevdisp(evctx->ctx_xmlevctx, arg1) : \
   evctx->input_xmlevctx->disp_xmlevctx->XmlEv##func##_xmlevdisp ? \
    evctx->input_xmlevctx->disp_xmlevctx->XmlEv##func##_xmlevdisp(evctx->input_xmlevctx->ctx_xmlevctx, arg1) : \
     evctx->input_xmlevctx->input_xmlevctx->disp_xmlevctx->XmlEv##func##_xmlevdisp ? \
      evctx->input_xmlevctx->input_xmlevctx->disp_xmlevctx->XmlEv##func##_xmlevdisp(evctx->input_xmlevctx->input_xmlevctx->ctx_xmlevctx, arg1) : \
       evctx->input_xmlevctx->input_xmlevctx->input_xmlevctx->disp_xmlevctx->XmlEv##func##_xmlevdisp ? \
        evctx->input_xmlevctx->input_xmlevctx->input_xmlevctx->disp_xmlevctx->XmlEv##func##_xmlevdisp(evctx->input_xmlevctx->input_xmlevctx->input_xmlevctx->ctx_xmlevctx, arg1) : \
         evctx->input_xmlevctx->input_xmlevctx->input_xmlevctx->input_xmlevctx->disp_xmlevctx->XmlEv##func##_xmlevdisp ? \
          evctx->input_xmlevctx->input_xmlevctx->input_xmlevctx->input_xmlevctx->disp_xmlevctx->XmlEv##func##_xmlevdisp(evctx->input_xmlevctx->input_xmlevctx->input_xmlevctx->input_xmlevctx->ctx_xmlevctx, arg1) 

/* XMLEV_DISPATCH2 */
#define XMLEV_DISPATCH2(evctx, func, arg1, arg2) \
  evctx->disp_xmlevctx->XmlEv##func##_xmlevdisp ? \
  evctx->disp_xmlevctx->XmlEv##func##_xmlevdisp(evctx->ctx_xmlevctx, arg1, arg2) : \
   evctx->input_xmlevctx->disp_xmlevctx->XmlEv##func##_xmlevdisp ? \
    evctx->input_xmlevctx->disp_xmlevctx->XmlEv##func##_xmlevdisp(evctx->input_xmlevctx->ctx_xmlevctx, arg1, arg2) : \
     evctx->input_xmlevctx->input_xmlevctx->disp_xmlevctx->XmlEv##func##_xmlevdisp ? \
      evctx->input_xmlevctx->input_xmlevctx->disp_xmlevctx->XmlEv##func##_xmlevdisp(evctx->input_xmlevctx->input_xmlevctx->ctx_xmlevctx, arg1, arg2) : \
       evctx->input_xmlevctx->input_xmlevctx->input_xmlevctx->disp_xmlevctx->XmlEv##func##_xmlevdisp ? \
        evctx->input_xmlevctx->input_xmlevctx->input_xmlevctx->disp_xmlevctx->XmlEv##func##_xmlevdisp(evctx->input_xmlevctx->input_xmlevctx->input_xmlevctx->ctx_xmlevctx, arg1, arg2) : \
         evctx->input_xmlevctx->input_xmlevctx->input_xmlevctx->input_xmlevctx->disp_xmlevctx->XmlEv##func##_xmlevdisp ? \
          evctx->input_xmlevctx->input_xmlevctx->input_xmlevctx->input_xmlevctx->disp_xmlevctx->XmlEv##func##_xmlevdisp(evctx->input_xmlevctx->input_xmlevctx->input_xmlevctx->input_xmlevctx->ctx_xmlevctx, arg1, arg2) 

/* XMLEV_DISPATCH3 */
#define XMLEV_DISPATCH3(evctx, func, arg1, arg2, arg3) \
  evctx->disp_xmlevctx->XmlEv##func##_xmlevdisp ? \
  evctx->disp_xmlevctx->XmlEv##func##_xmlevdisp(evctx->ctx_xmlevctx, arg1, arg2, arg3) : \
   evctx->input_xmlevctx->disp_xmlevctx->XmlEv##func##_xmlevdisp ? \
    evctx->input_xmlevctx->disp_xmlevctx->XmlEv##func##_xmlevdisp(evctx->input_xmlevctx->ctx_xmlevctx, arg1, arg2, arg3) : \
     evctx->input_xmlevctx->input_xmlevctx->disp_xmlevctx->XmlEv##func##_xmlevdisp ? \
      evctx->input_xmlevctx->input_xmlevctx->disp_xmlevctx->XmlEv##func##_xmlevdisp(evctx->input_xmlevctx->input_xmlevctx->ctx_xmlevctx, arg1, arg2, arg3) : \
       evctx->input_xmlevctx->input_xmlevctx->input_xmlevctx->disp_xmlevctx->XmlEv##func##_xmlevdisp ? \
        evctx->input_xmlevctx->input_xmlevctx->input_xmlevctx->disp_xmlevctx->XmlEv##func##_xmlevdisp(evctx->input_xmlevctx->input_xmlevctx->input_xmlevctx->ctx_xmlevctx, arg1, arg2, arg3) : \
         evctx->input_xmlevctx->input_xmlevctx->input_xmlevctx->input_xmlevctx->disp_xmlevctx->XmlEv##func##_xmlevdisp ? \
          evctx->input_xmlevctx->input_xmlevctx->input_xmlevctx->input_xmlevctx->disp_xmlevctx->XmlEv##func##_xmlevdisp(evctx->input_xmlevctx->input_xmlevctx->input_xmlevctx->input_xmlevctx->ctx_xmlevctx, arg1, arg2, arg3) 

/* XMLEV_DISPATCH4 */
#define XMLEV_DISPATCH4(evctx, func, arg1, arg2, arg3, arg4) \
  evctx->disp_xmlevctx->XmlEv##func##_xmlevdisp ? \
  evctx->disp_xmlevctx->XmlEv##func##_xmlevdisp(evctx->ctx_xmlevctx, arg1, arg2, arg3, arg4) : \
   evctx->input_xmlevctx->disp_xmlevctx->XmlEv##func##_xmlevdisp ? \
    evctx->input_xmlevctx->disp_xmlevctx->XmlEv##func##_xmlevdisp(evctx->input_xmlevctx->ctx_xmlevctx, arg1, arg2, arg3, arg4) : \
     evctx->input_xmlevctx->input_xmlevctx->disp_xmlevctx->XmlEv##func##_xmlevdisp ? \
      evctx->input_xmlevctx->input_xmlevctx->disp_xmlevctx->XmlEv##func##_xmlevdisp(evctx->input_xmlevctx->input_xmlevctx->ctx_xmlevctx, arg1, arg2, arg3, arg4) : \
       evctx->input_xmlevctx->input_xmlevctx->input_xmlevctx->disp_xmlevctx->XmlEv##func##_xmlevdisp ? \
        evctx->input_xmlevctx->input_xmlevctx->input_xmlevctx->disp_xmlevctx->XmlEv##func##_xmlevdisp(evctx->input_xmlevctx->input_xmlevctx->input_xmlevctx->ctx_xmlevctx, arg1, arg2, arg3, arg4) : \
         evctx->input_xmlevctx->input_xmlevctx->input_xmlevctx->input_xmlevctx->disp_xmlevctx->XmlEv##func##_xmlevdisp ? \
          evctx->input_xmlevctx->input_xmlevctx->input_xmlevctx->input_xmlevctx->disp_xmlevctx->XmlEv##func##_xmlevdisp(evctx->input_xmlevctx->input_xmlevctx->input_xmlevctx->input_xmlevctx->ctx_xmlevctx, arg1, arg2, arg3, arg4) 


/******************************************************************************
 *                  PUBLIC INTERFACES                                         *
 *****************************************************************************/
/******************************************************************************
    NAME
        XmlEvGetVersion

    DESCRIPTION
       Provides information about version specification in XML declaration
       for the following event:
       XML_EVENT_START_DOCUMENT.

    PARAMETERS:
      ctx      (IN)  - XML event implementation ctx

    RETURNS
        Version String 
******************************************************************************/
#ifndef XmlEvGetVersion
#define XmlEvGetVersion(evctx) \
  XMLEV_DISPATCH0(evctx, GetVersion) : \
  XmlEvDispatch3_0((evctx), (ub4)XMLEV_FUNC_GETVERSION)
#endif


/******************************************************************************
    NAME
        XmlEvIsEncodingSpecified

    DESCRIPTION
       Provides information about encoding specification in XML declaration
       for the following event:
        XML_EVENT_START_DOCUMENT.

    PARAMETERS:
      ctx      (IN)  - XML event implementation ctx

    RETURNS
        TRUE if encoding was specified in XML Decl. FALSE otherwise
*******************************************************************************/
#ifndef XmlEvIsEncodingSpecified
#define XmlEvIsEncodingSpecified(evctx) \
  (XMLEV_DISPATCH0(evctx, IsEncodingSpecified) : \
    XmlEvDispatch1((evctx), (ub4)XMLEV_FUNC_ISENCODINGSPECIFIED))
#endif


/******************************************************************************
    NAME
        XmlEvIsStandalone

    DESCRIPTION
       Provides information about standalone specification in XML declaration
       for the following event:
        XML_EVENT_START_DOCUMENT.

    PARAMETERS:
      ctx      (IN)  - XML event implementation ctx

    RETURNS
    -1   if standalone was not specified in XML declaration, 
     0 if FALSE was specified for standalone,
     1 if TRUE was specified for standalone.
******************************************************************************/
#ifndef XmlEvIsStandalone
#define XmlEvIsStandalone(evctx) \
  (XMLEV_DISPATCH0(evctx, IsStandalone) : \
  ((sword)XmlEvDispatch1((evctx), (ub4)XMLEV_FUNC_ISSTANDALONE)))
#endif


/*****************************************************************************
    NAME
        XmlEvNext

    DESCRIPTION
        Advance parse to the next event

    PARAMETERS:
      ctx      (IN)  - XML event implementation ctx

    RETURNS
        The event 
******************************************************************************/
#ifndef XmlEvNext
#define XmlEvNext(evctx) \
  XMLEV_DISPATCH0(evctx, Next) : XmlEvDispatch2((evctx), (ub4)XMLEV_FUNC_NEXT)
#endif


/*****************************************************************************
    NAME
        XmlEvNextTag

    DESCRIPTION
        Advance parse to the next tag event

    PARAMETERS:
      ctx      (IN)  - XML event implementation ctx

    RETURNS
        The event 
******************************************************************************/
#ifndef XmlEvNextTag
#define XmlEvNextTag(evctx) \
  XMLEV_DISPATCH0(evctx, NextTag) : \
  XmlEvDispatch2((evctx), (ub4)XMLEV_FUNC_NEXTTAG) 
#endif


/******************************************************************************
    NAME
        XmlEvSkipAttrs

    DESCRIPTION
        Advance parse to the next non-attribute event
        To be used when 'attr_events' property is TRUE.

    PARAMETERS:
      ctx      (IN)  - XML event implementation ctx

    RETURNS
        The event 
*******************************************************************************/
#ifndef XmlEvSkipAttrs
#define XmlEvSkipAttrs(evctx) \
  XMLEV_DISPATCH0(evctx, SkipAttrs) : \
  XmlEvDispatch2((evctx), (ub4)XMLEV_FUNC_SKIPATTRS) 
#endif


/******************************************************************************
    NAME
        XmlEvGetEncoding

    DESCRIPTION
       Provides information about encoding specification in XML declaration
       for the following event:
        XML_EVENT_START_DOCUMENT.

    PARAMETERS:
      ctx      (IN)  - XML event implementation ctx

    RETURNS
        The encoding string 
******************************************************************************/
#ifndef XmlEvGetEncoding
#define XmlEvGetEncoding(evctx) \
  XMLEV_DISPATCH0(evctx, GetEncoding) : \
  XmlEvDispatch3_0((evctx), (ub4)XMLEV_FUNC_GETENCODING)
#endif


/*****************************************************************************
    NAME
        XmlEvGetName0, XmlEvGetName

    DESCRIPTION
        Get name for the following events:
        XML_EVENT_START_ELEMENT,
        XML_EVENT_END_ELEMENT,
        XML_EVENT_START_ENTITY,
        XML_EVENT_ENTITY_REFERENCE,
        XML_EVENT_ELEMENT_DECLARATION,
        XML_EVENT_PE_DECLARATION,
        XML_EVENT_UE_DECLARATION,
        XML_EVENT_NOTATION_DECLARATION.

    PARAMETERS:
      ctx      (IN)  - XML event implementation ctx

    RETURNS
        Name
******************************************************************************/
#ifndef XmlEvGetName0
#define XmlEvGetName0(evctx) \
  XMLEV_DISPATCH0(evctx, GetName0) : \
              XmlEvDispatch3_0((evctx), (ub4)XMLEV_FUNC_GETNAME0)
#endif
#ifndef XmlEvGetName
#define XmlEvGetName(evctx, len) \
  XMLEV_DISPATCH1(evctx, GetName, len) : \
  XmlEvDispatch3((evctx), (ub4)XMLEV_FUNC_GETNAME, len) 
#endif


/******************************************************************************
    NAME
        XmlEvGetLocalName0, XmlEvGetLocalName

    DESCRIPTION
        Get local name of a tag for the following events:
        XML_EVENT_START_ELEMENT,
        XML_EVENT_END_ELEMENT.

    PARAMETERS:
      ctx      (IN)  - XML event implementation ctx

    RETURNS
        Local name of a tag
******************************************************************************/
#ifndef XmlEvGetLocalName0
#define XmlEvGetLocalName0(evctx) \
  XMLEV_DISPATCH0(evctx, GetLocalName0) : \
  XmlEvDispatch3_0((evctx), (ub4)XMLEV_FUNC_GETLOCALNAME0)
#endif
#ifndef XmlEvGetLocalName
#define XmlEvGetLocalName(evctx, len) \
  XMLEV_DISPATCH1(evctx, GetLocalName, len) : \
  XmlEvDispatch3((evctx), (ub4)XMLEV_FUNC_GETLOCALNAME, len) 
#endif


/******************************************************************************
    NAME
        XmlEvGetURI0, XmlEvGetURI

    DESCRIPTION
        Get URI of a tag for the following events:
        XML_EVENT_START_ELEMENT,
        XML_EVENT_END_ELEMENT.

    PARAMETERS:
      ctx      (IN)  - XML event implementation ctx

    RETURNS
      URI of a tag
*******************************************************************************/
#ifndef XmlEvGetURI0
#define XmlEvGetURI0(evctx) \
  XMLEV_DISPATCH0(evctx, GetURI0) : \
  XmlEvDispatch3_0((evctx), (ub4)XMLEV_FUNC_GETURI0) 
#endif
#ifndef XmlEvGetURI
#define XmlEvGetURI(evctx, len) \
  XMLEV_DISPATCH1(evctx, GetURI, len) : \
  XmlEvDispatch3((evctx), (ub4)XMLEV_FUNC_GETURI, len) 
#endif


/******************************************************************************
    NAME
        XmlEvGetPrefix0, XmlEvGetPrefix

    DESCRIPTION
        Get prefix of a tag for the following events:
        XML_EVENT_START_ELEMENT,
        XML_EVENT_END_ELEMENT.

    PARAMETERS:
      ctx      (IN)  - XML event implementation ctx

    RETURNS
      Prefix of a tag
*******************************************************************************/
#ifndef XmlEvGetPrefix0
#define XmlEvGetPrefix0(evctx) \
  XMLEV_DISPATCH0(evctx, GetPrefix0) : \
  XmlEvDispatch3_0((evctx), (ub4)XMLEV_FUNC_GETPREFIX0) 
#endif
#ifndef XmlEvGetPrefix
#define XmlEvGetPrefix(evctx, len) \
  XMLEV_DISPATCH1(evctx, GetPrefix, len) : \
  XmlEvDispatch3((evctx), (ub4)XMLEV_FUNC_GETPREFIX, len) 
#endif


/******************************************************************************
    NAME
        XmlEvtGetStartPos

    DESCRIPTION
        Get start position of tag for the following events:
        XML_EVENT_START_ELEMENT, XML_EVENT_END_ELEMENT.
        The start position is the position of the < that starts the tag.
        Note that for multi-byte character sets, the position is given
        as 2 times the number of characters (e.g. bytes of UCS2 code units).

    PARAMETERS:
      ctx      (IN)  - XML event implementation ctx

    RETURNS
      Start position of a tag
*******************************************************************************/
#ifndef XmlEvGetStartPos
#define XmlEvGetStartPos(evctx) \
  (XMLEV_DISPATCH0(evctx, GetStartPos) : \
  XmlEvDispatch5((evctx), (ub4)XMLEV_FUNC_GETSTARTPOS)) 
#endif


/******************************************************************************
    NAME
        XmlEvGetEndPos

    DESCRIPTION
        Get start position of tag for the following events:
        XML_EVENT_START_ELEMENT, XML_EVENT_END_ELEMENT.
        The end position is the position of the > that ends the tag.
        Note that for multi-byte character sets, the position is given
        as 2 times the number of characters (e.g. bytes of UCS2 code units).

    PARAMETERS:
      ctx      (IN)  - XML event implementation ctx

    RETURNS
      End position of a tag
*******************************************************************************/
#ifndef XmlEvGetEndPos
#define XmlEvGetEndPos(evctx) \
  (XMLEV_DISPATCH0(evctx, GetEndPos) : \
  XmlEvDispatch5((evctx), (ub4)XMLEV_FUNC_GETENDPOS)) 
#endif


/******************************************************************************
    NAME
        XmlEvGetAttrCount

    DESCRIPTION
        Get number of attributes for the following event:
        XML_EVENT_START_ELEMENT.

    PARAMETERS:
        ctx      (IN)  - XML event implementation ctx

    RETURNS
        Number of attributes
*******************************************************************************/
#ifndef XmlEvGetAttrCount
#define XmlEvGetAttrCount(evctx) \
  XMLEV_DISPATCH0(evctx, GetAttrCount) : \
  XmlEvDispatch5((evctx), (ub4)XMLEV_FUNC_GETATTRCOUNT) 
#endif


/******************************************************************************
    NAME
        XmlEvIsNamespaceAttr

    DESCRIPTION
        Get number of attributes for the following event:
        XML_EVENT_START_ELEMENT, XML_EVENT_START_ATTR.

    PARAMETERS:
        ctx      (IN)  - XML event implementation ctx
        index    (IN) - index of the attribute

    NOTES
      index ignored when 'attr_events' property is TRUE.
    RETURNS
        Number of attributes
*******************************************************************************/
#ifndef XmlEvIsNamespaceAttr
#define XmlEvIsNamespaceAttr(evctx, index) \
  (XMLEV_DISPATCH1(evctx, IsNamespaceAttr, index) : \
  XmlEvDispatch13((evctx), (ub4)XMLEV_FUNC_ISNSPATTR, index)) 
#endif


/******************************************************************************
    NAME
        XmlEvGetAttrName0, XmlEvGetAttrName

    DESCRIPTION
        Get attribute name for the following event:
        XML_EVENT_START_ELEMENT, XML_EVENT_START_ATTR.

    PARAMETERS:
      ctx      (IN)  - XML event implementation ctx
      index    (IN) - index of the attribute

    NOTES
      index ignored when 'attr_events' property is TRUE.
    RETURNS
      Attribute name
*******************************************************************************/
#ifndef XmlEvGetAttrName0
#define XmlEvGetAttrName0(evctx, index) \
  XMLEV_DISPATCH1(evctx, GetAttrName0, index) : \
  XmlEvDispatch4_0((evctx), (ub4)XMLEV_FUNC_GETATTRNAME0, index)
#endif
#ifndef XmlEvGetAttrName
#define XmlEvGetAttrName(evctx, index, len) \
  XMLEV_DISPATCH2(evctx, GetAttrName, index, len) : \
  XmlEvDispatch4((evctx), (ub4)XMLEV_FUNC_GETATTRNAME, index, len)
#endif



/******************************************************************************
    NAME
        XmlEvGetAttrLocalName0, XmlEvGetAttrLocalName

    DESCRIPTION
        Get attribute local name  for the following events:
        XML_EVENT_START_ELEMENT, XML_EVENT_START_ATTR.

    PARAMETERS:
      ctx      (IN)  - XML event implementation ctx
      index    (IN)  - index of attribute

    NOTES
      In the 'attr_events' case, the index is ignored.
    RETURNS
      Attribute local name
******************************************************************************/
#ifndef XmlEvGetAttrLocalName0
#define XmlEvGetAttrLocalName0(evctx, index) \
  XMLEV_DISPATCH1(evctx, GetAttrLocalName0, index) : \
  XmlEvDispatch4_0((evctx), (ub4)XMLEV_FUNC_GETATTRLOCALNAME0, index)
#endif
#ifndef XmlEvGetAttrLocalName
#define XmlEvGetAttrLocalName(evctx, index, len) \
  XMLEV_DISPATCH2(evctx, GetAttrLocalName, index, len) : \
  XmlEvDispatch4((evctx), (ub4)XMLEV_FUNC_GETATTRLOCALNAME, index, len)
#endif


/******************************************************************************
    NAME
        XmlEvGetAttrURI0, XmlEvGetAttrURI

    DESCRIPTION
        Get attribute URI for the following event:
        XML_EVENT_START_ELEMENT, XML_EVENT_START_ATTR.

    PARAMETERS:
      ctx      (IN)  - XML event implementation ctx

    RETURNS
      Attribute URI
*******************************************************************************/
#ifndef XmlEvGetAttrURI0
#define XmlEvGetAttrURI0(evctx, index) \
  XMLEV_DISPATCH1(evctx, GetAttrURI0, index) : \
  XmlEvDispatch4_0((evctx), (ub4)XMLEV_FUNC_GETATTRURI0, index)
#endif
#ifndef XmlEvGetAttrURI
#define XmlEvGetAttrURI(evctx, index, len) \
  XMLEV_DISPATCH2(evctx, GetAttrURI, index, len) : \
  XmlEvDispatch4((evctx), (ub4)XMLEV_FUNC_GETATTRURI, index, len)
#endif


/******************************************************************************
    NAME
        XmlEvGetAttrPrefix0, XmlEvGetAttrPrefix 

    DESCRIPTION
        Get attribute prefix for the following event:
        XML_EVENT_START_ELEMENT, XML_EVENT_START_ATTR.

    PARAMETERS:
      ctx      (IN)  - XML event implementation ctx

    RETURNS
      Attribute prefix
*******************************************************************************/
#ifndef XmlEvGetAttrPrefix0
#define XmlEvGetAttrPrefix0(evctx, index) \
  XMLEV_DISPATCH1(evctx, GetAttrPrefix0, index) : \
  XmlEvDispatch4_0((evctx), (ub4)XMLEV_FUNC_GETATTRPREFIX0, index)
#endif
#ifndef XmlEvGetAttrPrefix
#define XmlEvGetAttrPrefix(evctx, index, len) \
  XMLEV_DISPATCH2(evctx, GetAttrPrefix, index, len) : \
  XmlEvDispatch4((evctx), (ub4)XMLEV_FUNC_GETATTRPREFIX, index, len)
#endif


/******************************************************************************
    NAME
        XmlEvGetAttrValue0, XmlEvGetAttrValue

    DESCRIPTION
        Get attribute value  for the following event:
        XML_EVENT_START_ELEMENT.

    PARAMETERS:
      ctx      (IN)  - XML event implementation ctx

    RETURNS
      Attribute value
*******************************************************************************/
#ifndef XmlEvGetAttrValue0
#define XmlEvGetAttrValue0(evctx, index) \
  XMLEV_DISPATCH1(evctx, GetAttrValue0, index) : \
  XmlEvDispatch4_0((evctx), (ub4)XMLEV_FUNC_GETATTRVALUE0, index)
#endif
#ifndef XmlEvGetAttrValue
#define XmlEvGetAttrValue(evctx, index, len) \
  XMLEV_DISPATCH2(evctx, GetAttrValue, index, len) : \
  XmlEvDispatch4((evctx), (ub4)XMLEV_FUNC_GETATTRVALUE, index, len)
#endif


/******************************************************************************
    NAME
        XmlEvGetText0, XmlEvGetText

    DESCRIPTION
        Get text for the following events:
        XML_EVENT_CHARACTERS,
        XML_EVENT_CHARACTERS_CONT,
        XML_EVENT_SPACE,
        XML_EVENT_SPACE_CONT,
        XML_EVENT_COMMENT,
        XML_EVENT_COMMENT_CONT,
        XML_EVENT_CDATA,
        XML_EVENT_CDATA_CONT.

    PARAMETERS:
      ctx      (IN)  - XML event implementation ctx

    RETURNS
      text
*******************************************************************************/
#ifndef XmlEvGetText0
#define XmlEvGetText0(evctx) \
  XMLEV_DISPATCH0(evctx, GetText0) : \
  XmlEvDispatch3_0((evctx), (ub4)XMLEV_FUNC_GETTEXT0)
#endif
#ifndef XmlEvGetText
#define XmlEvGetText(evctx, len) \
  XMLEV_DISPATCH1(evctx, GetText, len) : \
  XmlEvDispatch3((evctx), (ub4)XMLEV_FUNC_GETTEXT, len)
#endif


/******************************************************************************
    NAME
        XmlEvGetPIData0, XmlEvGetPIData

    DESCRIPTION
        Get data for the following events:
        XML_EVENT_PI,
        XML_EVENT_PI_CONT.

    PARAMETERS:
        ctx      (IN)  - XML event implementation ctx

    RETURNS
        Data
*******************************************************************************/
#ifndef XmlEvGetPIData0
#define XmlEvGetPIData0(evctx) \
  XMLEV_DISPATCH0(evctx, GetPIData0) : \
  XmlEvDispatch3_0((evctx), (ub4)XMLEV_FUNC_GETPIDATA0)
#endif
#ifndef XmlEvGetPIData
#define XmlEvGetPIData(evctx, len) \
  XMLEV_DISPATCH1(evctx, GetPIData, len) : \
  XmlEvDispatch3((evctx), (ub4)XMLEV_FUNC_GETPIDATA, len)
#endif


/******************************************************************************
    NAME
        XmlEvGetPITarget0, XmlEvGetPITarget

    DESCRIPTION
        Get target for  the following events:
        XML_EVENT_PI,
        XML_EVENT_PI_CONT.

    PARAMETERS:
        ctx      (IN)  - XML event implementation ctx

    RETURNS
        Target
*******************************************************************************/
#ifndef XmlEvGetPITarget0
#define XmlEvGetPITarget0(evctx) \
  XMLEV_DISPATCH0(evctx, GetPITarget0) : \
  XmlEvDispatch3_0((evctx), (ub4)XMLEV_FUNC_GETPITARGET0)
#endif
#ifndef XmlEvGetPITarget
#define XmlEvGetPITarget(evctx, len) \
  XMLEV_DISPATCH1(evctx, GetPITarget, len) : \
  XmlEvDispatch3((evctx), (ub4)XMLEV_FUNC_GETPITARGET, len)
#endif


/******************************************************************************
    NAME
        XmlEvGetAttrDeclCnt

    DESCRIPTION
        Get number of attributes in DTD attribute declaration 
        (XML_EVENT_ATTLIST_DECLARATION).

    PARAMETERS:
        ctx      (IN)  - XML event implementation ctx

    RETURNS
        Number of attributes
*******************************************************************************/
#ifndef XmlEvGetAttrDeclCount
#define XmlEvGetAttrDeclCount(evctx) \
  XMLEV_DISPATCH0(evctx, GetAttrDeclCount) : \
  XmlEvDispatch5((evctx), (ub4)XMLEV_FUNC_GETATTRDECLCOUNT) 
#endif


/******************************************************************************
    NAME
        XmlEvGetAttrDeclElName0, XmlEvGetAttrDeclElName

    DESCRIPTION
        Get element name in DTD attribute declaration  
        (XML_EVENT_ATTLIST_DECLARATION).

    PARAMETERS:
      ctx      (IN)  - XML event implementation ctx

    RETURNS
      Element name
*******************************************************************************/
#ifndef XmlEvGetAttrDeclElName0
#define XmlEvGetAttrDeclElName0(evctx) \
  XMLEV_DISPATCH0(evctx, GetAttrDeclElName0) : \
  XmlEvDispatch3_0((evctx), (ub4)XMLEV_FUNC_GETATTRDECLELNAME0)
#endif
#ifndef XmlEvGetAttrDeclElName
#define XmlEvGetAttrDeclElName(evctx, len) \
  XMLEV_DISPATCH1(evctx, GetAttrDeclElName, len) : \
  XmlEvDispatch3((evctx), (ub4)XMLEV_FUNC_GETATTRDECLELNAME, len)
#endif


/******************************************************************************
    NAME
        XmlEvGetAttrDeclName0, XmlEvGetAttrDeclName

    DESCRIPTION
        Get attribute name in DTD attribute declaration
        (XML_EVENT_ATTLIST_DECLARATION).

    PARAMETERS:
      ctx      (IN)  - XML event implementation ctx
      index    (IN)  - index of attribute

    RETURNS
      Attribute name
******************************************************************************/
#ifndef XmlEvGetAttrDeclName0
#define XmlEvGetAttrDeclName0(evctx, index) \
  XMLEV_DISPATCH1(evctx, GetAttrDeclName0, index) : \
   XmlEvDispatch4_0((evctx), (ub4)XMLEV_FUNC_GETATTRDECLNAME0, index)
#endif
#ifndef XmlEvGetAttrDeclName
#define XmlEvGetAttrDeclName(evctx, index, len) \
  XMLEV_DISPATCH2(evctx, GetAttrDeclName, index, len) : \
  XmlEvDispatch4((evctx), (ub4)XMLEV_FUNC_GETATTRDECLNAME, index, len)
#endif


/******************************************************************************
    NAME
        XmlEvGetAttrDeclLocalName0, XmlEvGetAttrDeclLocalName

    DESCRIPTION
        Get attribute local name in DTD attribute declaration
        (XML_EVENT_ATTLIST_DECLARATION).

    PARAMETERS:
      ctx      (IN)  - XML event implementation ctx
      index    (IN)  - index of attribute

    RETURNS
      Attribute local name
*******************************************************************************/
#ifndef XmlEvGetAttrDeclLocalName0
#define XmlEvGetAttrDeclLocalName0(evctx, index) \
  XMLEV_DISPATCH1(evctx, GetAttrDeclLocalName0, index) : \
  XmlEvDispatch4_0((evctx), (ub4)XMLEV_FUNC_GETATTRDECLLOCALNAME0, index)
#endif
#ifndef XmlEvGetAttrDeclLocalName
#define XmlEvGetAttrDeclLocalName(evctx, index, len) \
  XMLEV_DISPATCH2(evctx, GetAttrDeclLocalName, index, len) : \
  XmlEvDispatch4((evctx), (ub4)XMLEV_FUNC_GETATTRDECLLOCALNAME, index, len)
#endif


/******************************************************************************
    NAME
        XmlEvGetAttrDeclPrefix0, XmlEvGetAttrDeclPrefix

    DESCRIPTION
        Get attribute name prefix in DTD attribute declaration 
        (XML_EVENT_ATTLIST_DECLARATION).

    PARAMETERS:
      ctx      (IN)  - XML event implementation ctx
      index    (IN)  - index of attribute

    RETURNS
      Attribute declaration prefix
*******************************************************************************/
#ifndef XmlEvGetAttrDeclPrefix0
#define XmlEvGetAttrDeclPrefix0(evctx, index) \
  XMLEV_DISPATCH1(evctx, GetAttrDeclPrefix0, index) : \
  XmlEvDispatch4_0((evctx), (ub4)XMLEV_FUNC_GETATTRDECLPREFIX0, index)
#endif
#ifndef XmlEvGetAttrDeclPrefix
#define XmlEvGetAttrDeclPrefix(evctx, index, len) \
  XMLEV_DISPATCH2(evctx, GetAttrDeclPrefix, index, len) : \
  XmlEvDispatch4((evctx), (ub4)XMLEV_FUNC_GETATTRDECLPREFIX, index, len)
#endif


/*****************************************************************************
    NAME
        XmlEvGetAttrDeclBody0, XmlEvGetAttrDeclBody

    DESCRIPTION
        Get the  body in DTD attribute declaration 
        (XML_EVENT_ATTLIST_DECLARATION).

    PARAMETERS:
      ctx      (IN)  - XML event implementation ctx
      index    (IN)  - index of attribute

    RETURNS
      Attribute declaration body
******************************************************************************/
#ifndef XmlEvGetAttrDeclBody0
#define XmlEvGetAttrDeclBody0(evctx, index) \
  XMLEV_DISPATCH1(evctx, GetAttrDeclBody0, index) : \
  XmlEvDispatch4_0((evctx), (ub4)XMLEV_FUNC_GETATTRDECLBODY0, index)
#endif
#ifndef XmlEvGetAttrDeclBody
#define XmlEvGetAttrDeclBody(evctx, index, len) \
  XMLEV_DISPATCH2(evctx, GetAttrDeclBody, index, len) : \
  XmlEvDispatch4((evctx), (ub4)XMLEV_FUNC_GETATTRDECLBODY, index, len)
#endif


/******************************************************************************
    NAME
        XmlEventGetElDeclContent0

    DESCRIPTION
        Get DTD element declaration content
        (XML_EVENT_ELEMENT_DECLARATION).

    PARAMETERS:
        ctx      (IN)  - XML event implementation ctx

    RETURNS
        Element declaration content
*******************************************************************************/
#ifndef XmlEvGetElDeclContent0
#define XmlEvGetElDeclContent0(evctx) \
  XMLEV_DISPATCH0(evctx, GetElDeclContent0) : \
  XmlEvDispatch3_0((evctx), (ub4)XMLEV_FUNC_GETELDECLCONTENT0)
#endif
#ifndef XmlEvGetElDeclContent
#define XmlEvGetElDeclContent(evctx, len) \
  XMLEV_DISPATCH1(evctx, GetElDeclContent, len) : \
  XmlEvDispatch3((evctx), (ub4)XMLEV_FUNC_GETELDECLCONTENT, len)
#endif


/******************************************************************************
    NAME
        XmlEvGetPEIsGen

    DESCRIPTION
        Check if general entity was declared
        (XML_EVENT_PE_DECLARATION).

    PARAMETERS:
        ctx      (IN)  - XML event implementation ctx

    RETURNS
        TRUE if general entity, FALSE if parameter
*******************************************************************************/
#ifndef XmlEvGetPEIsGen
#define XmlEvGetPEIsGen(evctx) \
  (XMLEV_DISPATCH0(evctx, GetPEIsGen) : \
  XmlEvDispatch1((evctx), (ub4)XMLEV_FUNC_GETPEISGEN))
#endif


/******************************************************************************
    NAME
        XmlEvGetPERepl0, XmlEvGetPERepl

    DESCRIPTION
        Get replacement text of PE declaration
        (XML_EVENT_PE_DECLARATION).

    PARAMETERS:
      ctx      (IN)  - XML event implementation ctx

    RETURNS
      PE name
*******************************************************************************/
#ifndef XmlEvGetPERepl0
#define XmlEvGetPERepl0(evctx) \
  XMLEV_DISPATCH0(evctx, GetPERepl0) : \
  XmlEvDispatch3_0((evctx), (ub4)XMLEV_FUNC_GETPEREPL0)
#endif
#ifndef XmlEvGetPERepl
#define XmlEvGetPERepl(evctx, len) \
  XMLEV_DISPATCH1(evctx, GetPERepl, len) : \
  XmlEvDispatch3((evctx), (ub4)XMLEV_FUNC_GETPEREPL, len)
#endif


/******************************************************************************
    NAME
        XmlEvGetPubId0, XmlEvGetPubId

    DESCRIPTION
        Get pubid of a declaration for the following events:
        XML_EVENT_PE_DECLARATION,
        XML_EVENT_UE_DECLARATION,
        XML_EVENT_NOTATION_DECLARATION.

    PARAMETERS:
      ctx      (IN)  - XML event implementation ctx

    RETURNS
      pubid
*******************************************************************************/
#ifndef XmlEvGetPubId0
#define XmlEvGetPubId0(evctx) \
  XMLEV_DISPATCH0(evctx, GetPubId0) : \
  XmlEvDispatch3_0((evctx), (ub4)XMLEV_FUNC_GETPUBID0)
#endif
#ifndef XmlEvGetPubId
#define XmlEvGetPubId(evctx, len) \
  XMLEV_DISPATCH1(evctx, GetPubId, len) : \
  XmlEvDispatch3((evctx), (ub4)XMLEV_FUNC_GETPUBID, len)
#endif


/******************************************************************************
    NAME
        XmlEvGetSysId0, XmlEvGetSysId

    DESCRIPTION
        Get sysid of a declaration for the following events:
        XML_EVENT_PE_DECLARATION,
        XML_EVENT_UE_DECLARATION,
        XML_EVENT_NOTATION_DECLARATION.

    PARAMETERS:
      ctx      (IN)  - XML event implementation ctx

    RETURNS
      sysid
*******************************************************************************/
#ifndef XmlEvGetSysId0
#define XmlEvGetSysId0(evctx) \
  XMLEV_DISPATCH0(evctx, GetSysId0) : \
  XmlEvDispatch3_0((evctx), (ub4)XMLEV_FUNC_GETSYSID0)
#endif
#ifndef XmlEvGetSysId
#define XmlEvGetSysId(evctx, len) \
  XMLEV_DISPATCH1(evctx, GetSysId, len) : \
  XmlEvDispatch3((evctx), (ub4)XMLEV_FUNC_GETSYSID, len)
#endif


/******************************************************************************
    NAME
        XmlEvGetUENdata0, XmlEvGetUENdata

    DESCRIPTION
        Get ndata of XML_EVENT_UE_DECLARATION event.

    PARAMETERS:
      ctx      (IN)  - XML event implementation ctx

    RETURNS
      ndata
*******************************************************************************/
#ifndef XmlEvGetUENdata0
#define XmlEvGetUENdata0(evctx) \
  XMLEV_DISPATCH0(evctx, GetUENData0) : \
  XmlEvDispatch3_0((evctx), (ub4)XMLEV_FUNC_GETUENDATA0)
#endif
#ifndef XmlEvGetUENdata
#define XmlEvGetUENdata(evctx, len) \
  XMLEV_DISPATCH1(evctx, GetUENData, len) : \
  XmlEvDispatch3((evctx), (ub4)XMLEV_FUNC_GETUENDATA, len)
#endif


/******************************************************************************
    NAME
        XmlEvGetTagID

    DESCRIPTION
        Get ID for the tag's QNAME
        for XML_EVENT_START_ELEMENT.

    PARAMETERS:
      ctx      (IN)  - XML event implementation ctx

    RETURNS
      ID
*******************************************************************************/
#ifndef XmlEvGetTagID
#define XmlEvGetTagID(evctx) \
  XMLEV_DISPATCH0(evctx, GetTagID) : \
  XmlEvDispatch6((evctx), (ub4)XMLEV_FUNC_GETTAGID)
#endif


/******************************************************************************
    NAME
        XmlEvGetAttrID

    DESCRIPTION
        Get ID for the attribute's QNAME
        (XML_EVENT_START_ELEMENT XML_EVENT_START_ATTR)

    PARAMETERS:
      ctx      (IN)  - XML event implementation ctx
      index    (IN)  - index of attribute

    NOTES
      index ignored for START_ATTR and END_ATTR
    RETURNS
      ID
*******************************************************************************/
#ifndef XmlEvGetAttrID
#define XmlEvGetAttrID(evctx, index) \
  XMLEV_DISPATCH1(evctx, GetAttrID, index) : \
  XmlEvDispatch7((evctx), (ub4)XMLEV_FUNC_GETATTRID, index)
#endif


/******************************************************************************
    NAME
        XmlEvGetTagUriID

    DESCRIPTION
        Get ID for the tag's URI
        for XML_EVENT_START_ELEMENT

    PARAMETERS:
      ctx      (IN)  - XML event implementation ctx

    RETURNS
      ID
*******************************************************************************/
#ifndef XmlEvGetTagUriID
#define XmlEvGetTagUriID(evctx) \
  XMLEV_DISPATCH0(evctx, GetTagUriID) : \
  XmlEvDispatch6((evctx), (ub4)XMLEV_FUNC_GETTAGURIID)
#endif


/******************************************************************************
    NAME
        XmlEvGetAttrUriID

    DESCRIPTION
        Get ID for the attribute's URI
        (XML_EVENT_START_ELEMENT, XML_EVENT_START_ATTR)

    PARAMETERS:
      ctx      (IN)  - XML event implementation ctx
      index    (IN)  - index of attribute

    NOTES
      index ignored for START_ATTR and END_ATTR
    RETURNS
      ID
*******************************************************************************/
#ifndef XmlEvGetAttrUriID
#define XmlEvGetAttrUriID(evctx, index) \
  XMLEV_DISPATCH1(evctx, GetAttrUriID, index) : \
  XmlEvDispatch7((evctx), (ub4 )XMLEV_FUNC_GETATTRURIID, index)
#endif


/******************************************************************************
    NAME                                            
        XmlEvGetLocation

    DESCRIPTION
      Get the location info during parsing. The application can use this
      API anytime during the parse to find out where the parser is currently.
      The information returned by this API is 
      1.Path: This is the URL/filename of the input stream being parsed.
      2.Linenum: The line number in that input stream.  


    PARAMETERS:
      ctx      (IN)  - XML event implementation ctx

    NOTES

    RETURNS
      void 
*******************************************************************************/
#ifndef XmlEvGetLocation
#define XmlEvGetLocation(evctx, line, path) \
  XMLEV_DISPATCH2(evctx, GetLocation, line, path) : \
  XmlEvDispatch10((evctx), (ub4)XMLEV_FUNC_GETLOCATION, line, path)
#endif


/******************************************************************************
    NAME                                            
        XmlEvGetError

    DESCRIPTION
        Get error number when XML_EVENT_FATAL_ERROR is returned 
        from a call to XmEvNext.

    PARAMETERS:
      ctx      (IN)  - XML event implementation ctx

    NOTES

    RETURNS
      xmlerr 
*******************************************************************************/
#ifndef XmlEvGetError
#define XmlEvGetError(evctx, msg) \
  (XMLEV_DISPATCH1(evctx, GetError, msg) : \
  XmlEvDispatch12((evctx), (ub4)XMLEV_FUNC_GETERROR, msg))
#endif


/******************************************************************************
    NAME                                            
        XmlEvGetAttrProperty

    DESCRIPTION
        Get property associated with this attribute (type information) 
        XML_EVENT_START_ELEMENT XML_EVENT_START_ATTR,

    PARAMETERS:
      ctx      (IN)  - XML event implementation ctx
      index    (IN)  - index of the attribute

    NOTES
      index is ignored for attr events. 
      Currently Pull Parser does not implement this.

    RETURNS
      void *
*******************************************************************************/
#ifndef XmlEvGetAttrProperty
#define XmlEvGetAttrProperty(evctx, index) \
  XMLEV_DISPATCH1(evctx, GetAttrProperty, index) : \
  XmlEvDispatch8((evctx), (ub4)XMLEV_FUNC_GETATTRPROPERTY, index)
#endif


/******************************************************************************
    NAME                                            
        XmlEvGetElProperty

    DESCRIPTION
        Get property associated with this element (type information) 
        XML_EVENT_START_ELEMENT

    PARAMETERS:
      ctx      (IN)  - XML event implementation ctx

    NOTES
      Currently, Pull Parser does not implement this.

    RETURNS
      void *
*******************************************************************************/
#ifndef XmlEvGetELProperty
#define XmlEvGetELProperty(evctx) \
  XMLEV_DISPATCH0(evctx, GetElProperty) : \
  XmlEvDispatch9((evctx), (ub4)XMLEV_FUNC_GETELPROPERTY)
#endif

#define XMLEV_WSP_AFTERQNAME    0x1  /* after qname */
#define XMLEV_WSP_AFTEREQ       0x2  /* after equation mark */
#define XMLEV_WSP_AFTERVAL      0x4  /* after attribute value */
/******************************************************************************
    NAME                                            
        XmlEvGetWsp

    DESCRIPTION
        Get the whitespace in oratext.
        If current StAX event is START_ELEMENT/END_ELEMENT, returns wsp# after
          element qname;
        Otherwise, check <flag> and return the requested wsp block within
          attribute <index>. The possible values for <flag> are:
          XMLEV_WSP_AFTERQNAME
          XMLEV_WSP_AFTEREQ
          XMLEV_WSP_AFTERVAL

    PARAMETERS:
        evctx      (IN)  - XML event implementation ctx

    RETURNS
        oratext *
*******************************************************************************/
#ifndef XmlEvGetWsp
#define XmlEvGetWsp(evctx, len, index, flag) \
  (XMLEV_DISPATCH3(evctx, GetWsp, len, index, flag) : \
   XmlEvDispatch19((evctx), (ub4)XMLEV_FUNC_GETWSP, len, index, flag))
#endif
/******************************************************************************
    NAME                                            
        XmlEvIsEmpty

    DESCRIPTION
        Check if current element is empty in form of <a/>
        Valid only when current StAX event is START_ELEMENT

    PARAMETERS:
        evctx      (IN)  - XML event implementation ctx

    RETURNS
        boolean 
*******************************************************************************/
#ifndef XmlEvIsEmpty
#define XmlEvIsEmpty(evctx) \
  (XMLEV_DISPATCH0(evctx, IsEmpty) : \
   XmlEvDispatch20((evctx), (ub4)XMLEV_FUNC_ISEMPTY))
#endif

/******************************************************************************
    NAME
        XmlEvGetBlock

    DESCRIPTION
        Get block data for xmldecl/dtd

    PARAMETERS:
        evctx      (IN)  - XML event implementation ctx

    RETURNS
        oratext *
*******************************************************************************/
#ifndef XmlEvGetBlock
#define XmlEvGetBlock(evctx, len) \
  (XMLEV_DISPATCH1(evctx, GetBlock, len): \
   XmlEvDispatch21((evctx), (ub4)XMLEV_FUNC_GETBLOCK, len))
#endif   
/******************************************************************************
 *                  PUBLIC FUNCTION PROTOTYPES                                *
 *****************************************************************************/
/* support for IDs, sb8 IDs are used instead of QNAMES */
/* user must provide function to convert name strings into IDs */
/* the function type, pointer */
typedef sb8 (* xmlev_id_cb_funcp)(void *ctx, ub1 type, ub1 *token, ub4 tok_len,
                                     sb8 nmspid, boolean isAttribute);


/* Error Callback function type for Illegal API's */
typedef  void (* xmlev_err_cb_funcp)(xmlctx *xctx,
                                     xmlevctx *evctx, xmlevtype cur_event);

/******************************************************************************
 *  NAME
 *       XmlEvCreatePPCtx
 *
 *  PURPOSE
 *       Create a Events context and start parsing an XML document in 
 *       pull-parse mode
 *
 *  DESCRIPTION
 *        Create a XML Events (Pull Parser) context. The document is loaded
 *        using XmlEvLoadPPDoc.  The actual parsing is driven by multiple
 *        calls to XmlEvNext.  After each call relevant information may be
 *        retrieved by calls to the various XmlEvGetXXXXX  functions.  Basic
 *        set of properties are the same as for XmlLoadDom.  Input source
 *        should be specified with XmlEvLoadPPDoc call.
 *        There are seven additional properties:
 *        ("expand_entities", boolean)
 *            when FALSE, causes (parsed non-parameter) entitity references 
 *            to be left unexpanded. By default such references are expanded.  
 *        ("use_buffer", address of buffer) 
 *            when specified, buffer will be used for data collected in case
 *            of the following events:
 *            XML_EVENT_CDATA, XML_EVENT_CDATA_CONT,  
 *            XML_EVENT_CHARACTERS, XML_EVENT_CHARACTERS_CONT,XML_EVENT_COMMENT,
 *            XML_EVENT_COMMENT_CONT, XML_EVENT_PI, XML_EVENT_PI_CONT,
 *            XML_EVENT_SPACE, XML_EVENT_SPACE_CONT.
 *            The get functions (see below) will return this buffer as data
 *            pointer.
 *        ("use_buffer_len", # bytes in buffer)
 *            the actual lenght of the buffer, not more than specified length is
 *            collected.  In the case only part of the data is collected, the 
 *            "CONT" flavor of the ev is generated.  Subsequent calls to 
 *            XmlEvsNext provide additional data.  Sequence of "CONT" flavored 
 *            events is always terminated by non "CONT" ev.  
 *            Buffer might be only partially filled.
 *        ("get_id_callback", address)
 *            provides an address of callback function to convert text base 
 *            names to 8 bytes IDs. Only when such function is supplied,
 *            the user
 *            is allowed to use XmlEvGetTagID and XmlEvGetAttrID)
 *        ("attr_events", boolean)
 *            If the user wants XML_EVENT_ATTRIBUTE events. By default this is 
 *            false. 
 *        ("raw_buffer_len", # bytes in buffer)
 *            If the user wants to set the raw buffer size. By default it is
 *            256K. Raw buffer is used to read the input data and perform
 *            character conversion plus converting CRLF, CR's to LF's
 *        ("error_callback", address)
 *            provides an address of callback function to be invoked to signal 
 *            illegal use of an API for that event. Useful for debugging.
 *
 *
 *  RETURNS
 *        (xmlevctx) EVENT contex to be passed on subsequent call 
 *        to XmlEvNext. NULL if error. 
 *
 *  ARGUMENTS
 *        xctx   (IN) XML context
 *        xmlerr (OUT)  numeric error code, XMLERR_OK [0] on success
 *        ...    (IN) variable arguments, with final NULL
 *
 ******************************************************************************/
xmlevctx *XmlEvCreatePPCtx(xmlctx *xctx, xmlerr  *xerr, ...);
xmlevctx *XmlEvCreatePPCtxVA(xmlctx *xctx, xmlerr  *xerr, va_list va);

/******************************************************************************
 *    NAME
 *       XmlEvLoadPPDoc
 *
 *   DESCRIPTION
 *
 *   PARAMETERS:
 *     xctx         (IN)  - XML context
 *     evctx        (IN)  - XML event context
 *     inputType    (IN)  - Input Type (file, stream, buffer etc)
 *     input        (IN)  - the input itself
 *     inputLen     (IN)  - input lenght to be used in case of buffer inputType
 *     inputEncoding  (IN)  - input encoding (optional)
 *     in_lid       (IN)  - input lid (optional)
 *     lxglo        (IN)  - global NLS context
 *     flags        (IN)  - input flags
 *
 *   RETURNS
 *    Load a document to be parsed based on the pull model.
*******************************************************************************/
xmlerr   XmlEvLoadPPDoc(xmlctx *xctx, xmlevctx *evctx, oratext *inputType,
                        void *input, ub4 inputLen, oratext *inputEncoding);

/******************************************************************************
 *   NAME
 *       XmlEvCleanPPCtx
 *
 *    DESCRIPTION
 *
 *    PARAMETERS:
 *      xctx      (IN)  - XML context
 *      evctx     (IN)  - XML event context
 *
 *    RETURNS
 *      Clean the memory associated with the latest parse.
 *****************************************************************************/

xmlerr   XmlEvCleanPPCtx(xmlctx *xctx, xmlevctx *evctx);

/******************************************************************************
 *   NAME
 *       XmlEvDestroyPPCtx
 *
 *   DESCRIPTION
 *
 *   PARAMETERS:
 *    xctx      (IN)  - XML context
 *    evctx     (IN)  - XML event context
 *
 *   RETURNS
 *    Destroy a Pull Parser context
*******************************************************************************/
void     XmlEvDestroyPPCtx(xmlctx *xctx, xmlevctx *evctx);


/* ------------------ INTERNAL USE ONLY ------------------------------------- */
#ifndef XmlEvGetLocator
#define XmlEvGetLocator(evctx, loc, loclen) \
  XMLEV_DISPATCH2(evctx, GetLocator, loc, loclen) : \
  XmlEvDispatch11((evctx), (ub4)XMLEV_FUNC_GETLOCATOR, loc, loclen)
#endif

#ifndef XmlEvIsPrologSpecified
#define XmlEvIsPrologSpecified(evctx) \
  (XMLEV_DISPATCH0(evctx, IsPrologSpecified) : \
  XmlEvDispatch1((evctx), (ub4)XMLEV_FUNC_ISPROLOGSPECIFIED))
#endif

#ifndef XmlEvIsVersionSpecified
#define XmlEvIsVersionSpecified(evctx) \
  (XMLEV_DISPATCH0(evctx, IsVersionSpecified) : \
  XmlEvDispatch1((evctx), (ub4)XMLEV_FUNC_ISVERSIONSPECIFIED))
#endif

#ifndef XmlEvGetPathCompsFromId
#define XmlEvGetPathCompsFromId(evctx, pid, pcomps, pncomps, isattr) \
  (XMLEV_DISPATCH4(evctx, GetPathCompsFromId, pid, pcomps, pncomps, isattr) : \
  XmlEvDispatch14((evctx), (ub4)XMLEV_FUNC_GETPATHCOMPSFROMID, pid, pcomps, \
                  pncomps, isattr))
#endif

#ifndef XmlEvGetLocalFromId
#define XmlEvGetLocalFromId(evctx, pathid, local, nmspcid, isattr) \
  (XMLEV_DISPATCH4(evctx, GetLocalFromId, pathid, local, nmspcid, \
                   isattr) : \
  XmlEvDispatch15((evctx), (ub4)XMLEV_FUNC_GETLOCALFROMID, pathid, local, \
                   nmspcid, isattr))
#endif

#ifndef XmlEvGetUriFromId
#define XmlEvGetUriFromId(evctx, nmspcid, uri) \
  (XMLEV_DISPATCH2(evctx, GetUriFromId, nmspcid, uri) : \
  XmlEvDispatch16((evctx), (ub4)XMLEV_FUNC_GETURIFROMID, nmspcid, uri))
#endif

#ifndef XmlEvGetLocalNameOffset
#define XmlEvGetLocalNameOffset(evctx) \
  (XMLEV_DISPATCH0(evctx, GetLocalNameOffset) : \
   XmlEvDispatch17((evctx), (ub4)XMLEV_FUNC_GETLOCALNAMEOFFSET))
#endif

#ifndef XmlEvGetAttrLocalNameOffset
#define XmlEvGetAttrLocalNameOffset(evctx, index) \
  (XMLEV_DISPATCH1(evctx, GetAttrLocalNameOffset, index) : \
   XmlEvDispatch18((evctx), (ub4)XMLEV_FUNC_GETATTRLOCALNAMEOFFSET, index))
#endif

#endif  /* XMLEV_ORACLE */
