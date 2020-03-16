/* $RCSfile: oraxml.h $ $Date: 2016/09/26 16:26:39 $
 Copyright (c) 1999, 2016, Oracle and/or its affiliates. All rights reserved.

  ########################################################################
  #  WARNING!!  This header is used to generate documentation directly!  #
  # exact structure and syntax of this file must be maintained properly  #
  #       in order for documentation to be produced automatically!       #
  ########################################################################

  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
                                             _       _           _       
    ___    _ __    __ _  __  __  _ __ ___   | |     | |__       (_)  ___ 
   / _ \  | '__|  / _` | \ \/ / | '_ ` _ \  | |     | '_ \      | | / __|
  | (_) | | |    | (_| |  >  <  | | | | | | | |  _  | | | |     | | \__ \
   \___/  |_|     \__,_| /_/\_\ |_| |_| |_| |_| (_) |_| |_|     |_| |___/
                                                                       
       _                                               _                _ 
    __| |   ___   _ __    _ __    ___    ___    __ _  | |_    ___    __| |
   / _` |  / _ \ | '_ \  | '__|  / _ \  / __|  / _` | | __|  / _ \  / _` |
  | (_| | |  __/ | |_) | | |    |  __/ | (__  | (_| | | |_  |  __/ | (_| |
   \__,_|  \___| | .__/  |_|     \___|  \___|  \__,_|  \__|  \___|  \__,_|
                 |_|                                                      

  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

                         oraxml.h HAS BEEN DEPRECATED

                 DO NOT WRITE ANY NEW CODE USING THIS INTERFACE

                         SEE xml.h FOR REPLACEMENT API

  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

   FILE 
     oraxml.h

   DESCRIPTION 
     DEPRECATED Public header for the Oracle XML parser

   PUBLIC FUNCTION(S) 
     Top-level parser APIs, plus DOM (1.0 and partial 2.0) and SAX APIs
*/
 
#ifndef ORAXML_ORACLE
# define ORAXML_ORACLE

#ifndef STDIO_ORACLE
# define STDIO_ORACLE
# include <stdio.h>
#endif

#ifndef ORASTRUC
# include <orastruc.h>
#endif

/*---------------------------------------------------------------------------
                     PUBLIC TYPES AND CONSTANTS
  ---------------------------------------------------------------------------*/
#ifndef XML_TYPES
#define XML_TYPES
/* DATATYPE xmlnode - DOM node
    A basic DOM node, could be any type.
*/
typedef struct xmlnode xmlnode;
typedef xmlnode xmlentnode;
typedef xmlnode xmlfragnode;
typedef xmlnode xmlentrefnode;
typedef xmlnode xmldocnode;
typedef xmlnode xmltextnode;
typedef xmlnode xmlcdatanode;
typedef xmlnode xmlcommentnode;
typedef xmlnode xmlattrnode;
typedef xmlnode xmlpinode;
typedef xmlnode xmlelemnode;
typedef xmlnode xmlnotenode;
typedef xmlnode xmldtdnode;
#endif
/* DATATYPE boolean - Basic TRUE/FALSE type
    The basic flag type, which has one of the values TRUE or FALSE.
*/
/* typedef int boolean; */

/* DATATYPE oratext - Character pointer (all encodings)
    The basic character-pointer type, used for all data encodings.  It
    should be cast as required, especially for Unicode strings, whose
    type will be lxuchar (2 byte).
*/
/* typedef unsigned char oratext; */

/* DATATYPE uword - Native unsigned word
    The basic unsigned scalar value, size depends on architecture.  Used
    primary for returning error codes.
*/
/* typedef unsigned int uword; */

#ifndef XML_ORACLE
# ifndef XMLCTX_DEFINED
#  define XMLCTX_DEFINED
/* DATATYPE xmlctx - XML parser context
    The XML parser's context.  Created by xmlinit and passed to most
    functions.
*/
struct xmlctx; typedef struct xmlctx xmlctx;
# endif

/* DATATYPE xslctx - XSL processor context
    The XSL processor's context.  Created by xslinit and passed to
    many XSLT functions.
*/
typedef struct xslctx xslctx;

/* DATATYPE xpctx - XPath context
    The XPath context.  Created by xpmakexpathctx and passed to most
    of the XPath functions.
*/
typedef struct xpctx xpctx;

/* DATATYPE xpexpr - XPath expression
    A parsed XPath expression, as returned by xpparsexpathexpr.
*/
typedef struct xpexpr xpexpr;

/* DATATYPE xpobj - XPath object
    An XPath object.  See xpobjtyp for the possible types.
*/
typedef struct xpobj xpobj;

#endif

/* DATATYPE xpnset - XPath node-set
    An XPath node-set, as returned by xpgetnsetval.
*/
typedef struct xpnset xpnset;

/* DATATYPE xpnsetele - XPath node-set element
    An XPath node-set element.  See xpgetfirstnsetelem, xpgetnextnsetelem,
    and xpgetnsetelemnode.
*/
typedef struct xpnsetele xpnsetele;

/* DATATYPE xmlihdl - I/O input handle
    Input handle union, for user-specified input callbacks
*/
typedef union {
    void *ptr_xmlihdl;   /* generic pointer (cast as needed) */
    struct {
        int fd;          /* integral file descriptor */
        int fd2;         /* second file descriptor (FTP data proto) */
        int fd3;         /* third file descriptor (FTP actual data) */
    } fds_xmlihdl;
} xmlihdl;

/* DATATYPE xmlacctype - XML URI access types
    URI access methods.
*/
typedef enum {
    XMLACCESS_NONE   = 0, /* not specified */
    XMLACCESS_UNK    = 1, /* specified but unknown */
    XMLACCESS_FILE   = 2, /* filesystem access */
    XMLACCESS_HTTP   = 3, /* HTTP */
    XMLACCESS_FTP    = 4, /* FTP */
    XMLACCESS_GOPHER = 5, /* Gopher */
    XMLACCESS_ORADB  = 6, /* Oracle DB */
    XMLACCESS_STREAM = 7  /* user-defined stream */
} xmlacctype;
#define XMLACCESS_MAX 7 /* range is 0 to XMLACCESS_MAX */

/* DATATYPE xmldomimp - DOM implementation
    The DOMImplementation structure.  Returned by getImplementation and
    required by functions in the DOMImplementation interface.
*/
typedef struct xmldomimp xmldomimp;

/* DATATYPE xmlnodes - DOM NamedNodeMap/NodeList
    A list of nodes; the underlying implementation of NodeList and
    NamedNodeMap.
*/
typedef struct xmlnodes xmlnodes;

/* DATATYPE xsloutputmethod - XSL output method
    Choice of output method for XSL processing
*/
typedef enum {
    XSLMETHOD_UNKNOWN = 0,
    XSLMETHOD_XML,
    XSLMETHOD_TEXT,
    XSLMETHOD_HTML
} xsloutputmethod;

/* DATATYPE xpobjtyp - XPath object type
    XSL object types
*/
typedef enum xpobjtyp {
    XPOBJTYP_BOOL,   /* boolean */ 
    XPOBJTYP_NUM,    /* number */
    XPOBJTYP_STR,    /* string */
    XPOBJTYP_NSET,   /* node set */
    XPOBJTYP_RTFRAG  /* result tree fragment */
} xpobjtyp;

/* DATATYPE xmlflags - XML parser flags
   Flag bits for xmlparse functions.
   Note: The flag options here are matching with
   the flags in LpxParseXXX(), refer to lpx.h.
*/
#define XML_FLAG_VALIDATE               0x01
#define XML_FLAG_DISCARD_WHITESPACE     0x02
#define XML_FLAG_DTD_ONLY               0x04
#define XML_FLAG_STOP_ON_WARNING        0x08
#define XML_FLAG_WARN_DUPLICATE_ENTITY  0x10
#define XML_FLAG_FORCE_INCODING         0x20
#define XML_FLAG_DONT_EXPAND_CHAR_REF   0x40
#define XML_FLAG_OLDPARSER              0x400
#define XML_FLAG_URL_DONT_OPEN          0x4000
#define XML_FLAG_FILE_DONT_OPEN         0x20000
typedef ub4 xmlflags;

/* DATATYPE xmlpflags - XML printer flags
    Flag bits for print functions.
*/
#define XML_FLAG_PRINT_XMLDECL          0x01
typedef ub4 xmlpflags;

/* DATATYPE xmlntype - DOM node types
    Parse tree node types.  Names & values through #12 match DOM specification.
*/
#define ELEMENT_NODE                1  /* element */
#define ATTRIBUTE_NODE              2  /* attribute */
#define TEXT_NODE                   3  /* char data (not CDATA) */
#define CDATA_SECTION_NODE          4  /* char data (CDATA) */
#define ENTITY_REFERENCE_NODE       5  /* entity reference */
#define ENTITY_NODE                 6  /* entity */
#define PROCESSING_INSTRUCTION_NODE 7  /* <?pi?> */
#define COMMENT_NODE                8  /* <!-- Comments --> */
#define DOCUMENT_NODE               9  /* Document */
#define DOCUMENT_TYPE_NODE         10  /* DTD */
#define DOCUMENT_FRAGMENT_NODE     11  /* Document fragment */
#define NOTATION_NODE              12  /* notation */
#define ELEMENT_DECL_NODE          13  /* <!ELEMENT> declaration */
#define ATTR_DECL_NODE             14  /* <!ATTLIST> attr decl */
#define CP_ELEMENT_NODE            15  /* element */
#define CP_CHOICE_NODE             16  /* choice (a|b) */
#define CP_SEQUENCE_NODE           17  /* sequence (a,b) */
#define CP_PCDATA_NODE             18  /* #PCDATA */
#define CP_STAR_NODE               19  /* '*' (zero or more) */
#define CP_PLUS_NODE               20  /* '+' (one or more) */
#define CP_OPT_NODE                21  /* '?' (optional) */
typedef ub1 xmlntype;

/* forward declare */
struct xmlstream;
typedef struct xmlstream xmlstream;

/* obsolete, do not use */
#define XML_PRINTF(func) \
    boolean func(void *ctx, oratext *s, uword len)

/*---------------------------------------------------------------------------
                      ERROR CODES, XMLERR_xxx

                        0000 0099 Generic
                        0100 0199 VC and other Warnings
                        0200 0299 Parser
                        0300 0399 XSL
                        0400 0499 XPath

  ---------------------------------------------------------------------------*/

/* Errors in the following range are considered "warnings" and may continue */
#define XMLERR_WARN_MIN         100     /* minimum warning code */
#define XMLERR_WARN_MAX         199     /* maximum warning code */

#define XMLERR_OK               0       /* success return */
#define XMLERR_NULL_PTR         1       /* NULL pointer */
#define XMLERR_NO_MEMORY        2       /* out of memory */
#define XMLERR_HASH_DUP         3       /* duplicate entry in hash table */
#define XMLERR_INTERNAL         4       /* internal error */
#define XMLERR_BUFFER_OVERFLOW  5       /* name/quoted string too long */
#define XMLERR_INVALID_CHILD    6       /* invalid child for parent */
#define XMLERR_EOI              7       /* unexpected End Of Information */
#define XMLERR_INVALID_MEMCB    8       /* invalid memory callbacks */
#define XMLERR_UNICODE_ALIGN    12      /* Unicode data misalignment */
#define XMLERR_NODE_TYPE        13      /* wrong node type */
#define XMLERR_UNCLEAN          14      /* context is not clean */

/* Errors 50 99 are reserved for lpxinit errors and have no associated
   error messages since they are assumed to be due to errors made by the 
   caller. */
#define XMLERR_NLS_MISMATCH     50      /* specify both lxglo/lxd or neither*/
#define XMLERR_NLS_INIT         51      /* error during NLS initialization */
#define XMLERR_LEH_INIT         52      /* error during LEH initialization */

#define XMLERR_VC_ROOT_ELEM     100     /* root element mismatch */
#define XMLERR_VC_DECL_PE_NEST  101     /* improper decl/pe nesting */
#define XMLERR_VC_STDALONE_DOC  102     /* bogus standalone doc decl */
#define XMLERR_VC_INVALID_ELEM  103     /* invalid element (mismatch DTD) */
#define XMLERR_VC_UNDEF_ELEM    104     /* element is not defined */
#define XMLERR_VC_NOT_EMPTY     105     /* element should be empty */
#define XMLERR_VC_UNDEF_ATTR    106     /* undefined attribute */
#define XMLERR_VC_UNIQUE_ELEM   107     /* element multiply defined */
#define XMLERR_VC_GROUP_PE_NEST 108     /* improper group/pe nesting */
#define XMLERR_VC_DUP_TYPE      109     /* duplicate name in mixed decl */
#define XMLERR_VC_BAD_NAME      110     /* bad Name */
#define XMLERR_VC_BAD_NMTOKEN   111     /* bad Nmtoken */
#define XMLERR_VC_ONE_ID        112     /* multiple ID attributes */
#define XMLERR_VC_ID_ATTR_DEF   113     /* ID not IMPLIED or REQUIRED */
#define XMLERR_VC_ENUM          114     /* attr val not in enumeration */
#define XMLERR_VC_REQ_ATTR      115     /* missing required attribute */
#define XMLERR_VC_FIXED_ATTR    116     /* wrong val for fixed attribute */
#define XMLERR_VC_UNDEF_ENTITY  118     /* undefined entity */
#define XMLERR_VC_ENTITY_PARSE  119     /* entity attribute not unparsed */
#define XMLERR_VC_ENTITY_NDATA  120     /* undefined entity NDATA */
#define XMLERR_VC_UNDEF_NOTE    121     /* undefined notation */
#define XMLERR_VC_UNDEF_ID      122     /* undefined ID (in IDREF) */
#define XMLERR_VC_DUP_ID        123     /* duplicate ID */
#define XMLERR_VC_ATTR_REFS     124     /* attr value missing refs */
#define XMLERR_DUP_ENTITY       125     /* duplicate entity */

#define XMLERR_CONVERT          200     /* encoding conversion problem */
#define XMLERR_BAD_ENCODING     201     /* invalid encoding */
#define XMLERR_OPEN_FILE        202     /* could not open file */
#define XMLERR_READ_FILE        203     /* could not read from file */
#define XMLERR_SYNTAX           204     /* generic syntax error */
#define XMLERR_COMMENT_SYNTAX   205     /* comment syntax error */
#define XMLERR_CDATA_SYNTAX     206     /* CDATA syntax error */
#define XMLERR_COND_SYNTAX      207     /* conditional section syntax error */
#define XMLERR_DTD_KEYWORD      208     /* unknown keyword found in DTD */
#define XMLERR_XML_RESERVED     209     /* XML cannot be used in a PI */
#define XMLERR_NOT_EXPECTED     210     /* syntax error */
#define XMLERR_DEF_DECL         211     /* error in default declaration */
#define XMLERR_COMMENT_END      212     /* comment syntax error */
#define XMLERR_COMMENT_NOEND    213     /* comment syntax error */
#define XMLERR_CDATA_NOEND      214     /* CDATA syntax error */
#define XMLERR_PIDATA_NOEND     215     /* PIDATA syntax error */
#define XMLERR_INVALID_WIDE_CHAR    216 /* Invalid lxwchar */
#define XMLERR_INVALID_UNICODE_CHAR 217 /* Invalid lxuchar */
#define XMLERR_INVALID_NATIVE_CHAR  218 /* Invalid ASCII/EBCDIC character */
#define XMLERR_BAD_CHARREF      219     /* Invalid character */
#define XMLERR_CHARDATA         220     /* Error while processing text */
#define XMLERR_ATTR_VALUE       221     /* Invalid char in attribute value */
#define XMLERR_SAX              222     /* Error from SAX callback */
#define XMLERR_WFC_EXT_ENTITY   223     /* No external entity in attrib val */
#define XMLERR_WFC_UNIQUE_ATTR  224     /* Attributes must be unique */
#define XMLERR_WFC_ELEM_MATCH   225     /* Start tag and end tag of elem */
#define XMLERR_WFC_ENTITY_DECL  226     /* Entity not declared */
#define XMLERR_WFC_PARSED_ENT   227     /* Must be a parsed entity */
#define XMLERR_WFC_RECUR        228     /* No recursion in entity refs */
#define XMLERR_EMPTY_FILE       229     /* Empty file */
#define XMLERR_INVALID_NM_UNICODE_CHAR \
                                230     /* Invalid character in NAME/NMTOKEN */
#define XMLERR_INVALID_NM_NATIVE_CHAR \
                                231     /* Invalid character in NAME/NMTOKEN */
#define XMLERR_WFC_INVALID_PE   232     /* PE improperly used in internal DTD*/
#define XMLERR_NSC_LEADING_XML  233     /* Leading XML for namespace prefix */
#define XMLERR_NSC_PREFIX_DECL  234     /* Namespace prefix not declared */
#define XMLERR_BAD_VERSION      235     /* XML Version not supported */
#define XMLERR_INVALID_PUB_CHAR 236     /* Invalid pubid character */
#define XMLERR_COND_KEYWORD     237     /* condition section keyword invalid */
#define XMLERR_COND_UNFINISHED  238     /* condition is unfinished (open) */
#define XMLERR_ATTR_TYPE        239     /* invalid attribute type */
#define XMLERR_NWF_ELEM_START   240     /* element-start tag is NWF */
#define XMLERR_NWF_ENTREF       241     /* entity reference is NWF */
#define XMLERR_AMPERSAND        242     /* invalid use of ampersand char */
#define XMLERR_ATTR_QUOTE       243     /* elem attr value not in quotes */
#define XMLERR_LESSTHAN         244     /* invalid use of '<' character */
#define XMLERR_EXTRA_DATA       245     /* extra data at end of document */
#define XMLERR_NO_SYSID         246     /* missing SYSID after PUBID */
#define XMLERR_BAD_DTD          247     /* bad DTD declaration */
#define XMLERR_BAD_ENTITY_DECL  248     /* bad entity declaration */
#define XMLERR_BAD_EXTID        249     /* bad external ID declaration */
#define XMLERR_BAD_ATTR_DECL    250     /* bad attribute declaration */
#define XMLERR_INT_COND         251     /* no conditionals in internal DTD */
#define XMLERR_ENTITY_NESTING   252     /* improper entity nesting */
#define XMLERR_NO_VERSION       253     /* missing required version# */
#define XMLERR_BAD_XML_DECL     254     /* bad XML declaration */
#define XMLERR_STANDALONE_YN    255     /* invalid standalone yes/no */
#define XMLERR_ELEMENT_DECL     256     /* invalid element declaration */
#define XMLERR_CHILDREN_DECL    257     /* invalid children declaration */
#define XMLERR_MIXED_DECL       258     /* invalid mixed declaration */
#define XMLERR_NOTATION_DECL    259     /* invalid notation declaration */
#define XMLERR_XMLSPACE_DECL    260     /* invalid xml:space declaration */
#define XMLERR_INVALID_URL      261     /* invalid URL */
#define XMLERR_UNSUP_PROT       262     /* unsupported protocol */
#define XMLERR_OPEN_TCP         263     /* failed to open TCP connection */
#define XMLERR_SEND_TCP         264     /* failed to send to TCP connection */
#define XMLERR_RECV_TCP         265     /* failed to read from TCP conn. */
#define XMLERR_INVALID_LANG     266     /* invalid language specification */
#define XMLERR_RESOLVE_URL      267     /* couldn't resolve relative URL */
#define XMLERR_INVALID_ACCESS   268     /* invalid access method */
#define XMLERR_ACCESS_FUNCS     269     /* all access funcs must be provided */
#define XMLERR_FTP_CMD          270     /* FTP command failed */
#define XMLERR_FTP_LOGIN        271     /* FTP login failed */
#define XMLERR_FTP_UNAVAIL      272     /* FTP server unavailable */
#define XMLERR_INIT_TCP         273     /* failed to initialize TCP/IP */
#define XMLERR_CANT_IMPORT      274     /* can't import that node type */
#define XMLERR_CANT_SET_ENC     275     /* can't set output encoding */
#define XMLERR_BAD_HTTP_MIME    276     /* bad Mime header in HTTP reply */
#define XMLERR_END_QUOTE        277     /* No ending quote was seen */
#define XMLERR_INVALID_PROXY    278     /* Invalid protocol proxy */
#define XMLERR_INVALID_NO_PROXY 279     /* Invalid no-proxy specification */
#define XMLERR_HTTP_ERROR       280     /* HTTP protocol error */
#define XMLERR_UNSUP_ENCODING   281     /* unsupported encoding */
#define XMLERR_SHARED_DTD_MIX   282     /* doc can't have DTD if set shared */
#define XMLERR_WRONG_ENCODING   283     /* input document in wrong encoding */
#define XMLERR_NULL_URI         284     /* NULL URI in namespace pfx decl */

/* XSL error codes */

#define XMLERR_NONAMEINATTRSET           300   /* no name in attribute set */
#define XMLERR_ERROR_IN_XPATH            301   /* The XPATH function returns an
                                                  error */
#define XMLERR_CANNOT_TRANSFORM          302   /* The child node is of invalid
                                                  type or has invalid name */
#define XMLERR_ATTRIBUTE_NOT_EXPECTED    303   /* Attribute is found but its
                                                  value is not the expected
                                                  value */
#define XMLERR_NULL_INPUT_STRING         304   /* input string is null */
#define XMLERR_MISSING_TOKEN             305   /* expected token is missing */
#define XMLERR_INCOM_ATTR_VAL_TMPL       306   /* inputed string ended with no 
                                                  corresponding closing } */
#define XMLERR_NS_PREFIX_NOT_DECLARED    307   /* Namespace prefix used but not
                                                  not declared */
#define XMLERR_ATTRIBUTE_NOT_FOUND       308   /* The expected attribute for
                                                  this node is not found. */
#define XMLERR_CANNOT_INIT_XPATH         309   /* XPATH context could not be
                                                  initialized */
#define XMLERR_ELEMENT_NOT_FOUND         310   /* The expected element is not
                                                  found. */
#define XMLERR_FEATURE_NOT_SUPPORTED     311   /* feature not supported */
#define XMLERR_CANNOT_CONS_PI            312   /* The content of XML PI node
                                                  might be invalid. */
#define XMLERR_CANNOT_CONS_COMMENT       313   /* The content of XML comment
                                                  node might be invalid. */
#define XMLERR_FAIL                      314   /* An internal error occurred in
                                                  the code. */
#define XMLERR_EXT_FUNC_NOT_SUPPORTED    315   /* extension function not
                                                  supported */
#define XMLERR_INVALID_ATTR_VALUE        316   /* invalid value for attr */
#define XMLERR_UNDEF_DECIMAL_FORMAT      317   /* undefined decimal-format */
#define XMLERR_DUP_DEC_FORMAT            318   /* duplicate decimal-format */
#define XMLERR_INVALID_DATA              319   /* The data is not allowed */

#define XMLERR_OUTOFORDERATTRIBUTE       320   /* Cant add more attributes */
#define XMLERR_NULL_OUTPUT               321   /* No mechanism for output */
#define XMLERR_DOCUMENTNOTFOUND          322   /* Could not open doc */
#define XMLERR_APPLY_IMPORTS             323   /* Cant apply imports */
#define XMLERR_INV_LANG                  324   /* Invalid LANG for xsl:sort */

/* XPATH error codes */

#define XMLERR_XPATH_INTERNAL       400   /* Internal error */
#define XMLERR_XPATH_INVNAME        401   /* Invalid QName */
#define XMLERR_XPATH_INVAXIS        402   /* Invalid axis name */
#define XMLERR_XPATH_QTNOTMATCH     403   /* Unmatched quote */
#define XMLERR_XPATH_NSERR          404   /* Unable to resolve namespace */
#define XMLERR_XPATH_MEMERR         405   /* Unable to allocate memory */
#define XMLERR_XPATH_INVOBJTYP      406   /* Incorrect object type */
#define XMLERR_XPATH_NOTRSQB        407   /* Right square bracket missing */
#define XMLERR_XPATH_NOTRPAREN      408   /* Right parenthesis missing */
#define XMLERR_XPATH_INVTOK         409   /* Invalid token */
#define XMLERR_XPATH_VARREF         410   /* Unable to resolve the variable */
#define XMLERR_XPATH_UNKFUNC        411   /* Unknown function */
#define XMLERR_ENCODING_MISMATCH    412   /* encoding mismatch in Inc/Import*/

/* The range 470 499 are internal error codes. There would not be any error 
   mesgs corresponding to this in the mesg file. The error mesg given to the 
   user should be internal error. 
*/

#define XMLERR_XPATH_INVEXPRTREE    497   /* Invalid expression tree */
#define XMLERR_XPATH_INVOP          498   /* Invalid operator */
#define XMLERR_XPATH_INVCTX         499   /* Invalid context list/node */

/*---------------------------------------------------------------------------
                        Package Callback - User callback functions

    This package defines macros which declare functions (or function
    pointers) for XML callbacks.  Callbacks are used for error-message
    handling, memory allocation & freeing, and stream operations.

  ---------------------------------------------------------------------------*/

/****************************************************************************
                               Interface Callbacks
 ****************************************************************************/

/*---------------------------------------------------------------------------
    NAME
        XML_ALLOC [xmlmemcb]

    FUNCTION
        Memory allocation callback

    DESCRIPTION
        This macro defines a prototype for the low-level memory allocation
        function used in a xmlmemcb memory-callback structure.  If no memory
        callbacks are provided, malloc and free are used; if provided, the
        callbacks will be used instead.  Note allocated memory does not need
        to be initialized.

    RETURNS
        (void *) pointer to allocated memory [unitialized]

    ARGUMENTS
        ctx  (IN)  user-defined memory context
        size (OUT) number of bytes to allocate

    SEE ALSO
        xmlmemcb, xmlinit, xmlinitenc, XML_FREE
*/
#define XML_ALLOC(func) void *func(void *ctx, ub4 size)

/*---------------------------------------------------------------------------
    NAME
        XML_FREE [xmlmemcb]

    FUNCTION
        Memory freeing callback

    DESCRIPTION
        This macro defines a prototype for the low-level memory freeing
        function used in a xmlmemcb memory-callback structure.  If no
        memory callbacks are provided, malloc and free are used; if provided,
        the user's callbacks will be used instead.

    RETURNS
        (void)

    ARGUMENTS
        ctx (IN)  user-defined memory context
        ptr (OUT) pointer to memory to be freed

    SEE ALSO
        xmlmemcb, xmlinit, xmlinitenc, XML_ALLOC
*/
#define XML_FREE(func) void func(void *ctx, void *ptr)

/* DATATYPE xmlmemcb - Memory callbacks
    The parser allocates memory in large chunks, which are then doled
    out internally in small pieces.  The default system memory allocator
    (malloc, free) will be used to allocate and free the chunks unless a
    memory callback structure is provided.  To override the defaults, a
    xmlmemcb containing function pointers to alloc & free functions can
    be passed to xmlinit; the user's function will be used instead.  The
    memory callback context memcbctx is passed to the callback functions.
*/
struct xmlmemcb
{
    XML_ALLOC((*alloc));
    XML_FREE((*free));
};
typedef struct xmlmemcb xmlmemcb;

/*---------------------------------------------------------------------------
    NAME
        XML_OPENF [xmlaccess]

    FUNCTION
        I/O open callback

    DESCRIPTION
        This macro defines a prototype for the open function callback, which
        is called once to open the input source.  It should set its persistent
        handle in the xmlihdl union, which has two choices, a generic pointer
        (void *), and an integer (as Unix file or socket handle).  This function
        should return XMLERR_OK on success.  Args:

    RETURNS
        (uword) numeric error code, XMLERR_OK [0] on success

    ARGUMENTS
        ctx    (IN)  XML context
        path   (IN)  full path of the URI to be opened
        parts  (IN)  URI broken down into components (opaque pointer)
        length (OUT) total length of input data if known, 0 if not
        ih     (OUT) the opened handle is placed here

    SEE ALSO
        xmlaccess, XML_CLOSEF, XML_READF
*/
#define XML_OPENF(func) \
    uword func(xmlctx *ctx, oratext *path, void *parts, \
               ubig_ora *length, xmlihdl *ih)

typedef XML_OPENF((*xml_openf));

/*---------------------------------------------------------------------------
    NAME
        XML_CLOSEF [xmlaccess]

    FUNCTION
        I/O close callback

    DESCRIPTION
        This macro defines a prototype for the close function callback, called
        to close an open source and free its resources.  Args:

    RETURNS
        (void)

    ARGUMENTS
        ctx (IN) XML context
        ih  (IN) input handle union

    SEE ALSO
        xmlaccess, XML_OPENF, XML_READF
*/
#define XML_CLOSEF(func) \
    void func(xmlctx *ctx, xmlihdl *ih)

typedef XML_CLOSEF((*xml_closef));

/*---------------------------------------------------------------------------
    NAME
        XML_READF [xmlaccess]

    FUNCTION
        I/O read callback

    DESCRIPTION
        This macro defines a prototype for the read function callback, called
        to read data from an open source into a buffer, returning the number
        of bytes read: if 0, an EOI condition is indicated; if > 0, then the
        EOI flag determines if this is the final data.  On EOI, the matching
        close function will be called automatically.

    RETURNS
        (uword) number of bytes read [0 on EOI]

    ARGUMENTS
        ctx      (IN)  XML context
        path     (IN)  full URI of the open source (for error messages)
        ih       (IN)  input handle union
        dest     (OUT) destination buffer to read data into
        destsize (IN)  size of dest buffer
        nraw     (OUT) number of bytes read
        eoi      (OUT) End of Information? (last chunk of data)

    SEE ALSO
        xmlaccess, XML_OPENF, XML_CLOSEF
*/
#define XML_READF(func) \
    uword func(xmlctx *ctx, oratext *path, xmlihdl *ih, oratext *dest, \
               size_t destsize, sbig_ora *nraw, ub1 *eoi)

typedef XML_READF((*xml_readf));


/*---------------------------------------------------------------------------
    NAME
        XMLSTREAM_OPENF [xmlstream]

    FUNCTION
        Stream open callback

    DESCRIPTION
        Macro which defines the prototype for a stream open function.

    RETURNS
        (uword) error code, XMLERR_OK [0] for success

    ARGUMENTS
        stream (IN) stream descriptor
        rsvd   (IN) reserved for future use [must be 0]

    SEE ALSO
        xmlstream, XMLSTREAM_WRITEF, XMLSTREAM_CLOSEF, xslsetoutputstream,
        xslgetoutputstream
*/
#define XMLSTREAM_OPENF(func) \
    uword func(xmlstream *stream, ub4 rsvd)

/*---------------------------------------------------------------------------
    NAME
        XMLSTREAM_WRITEF [xmlstream]

    FUNCTION
        Stream write callback

    DESCRIPTION
        Macro which defines the prototype for a stream write function.

    RETURNS
        (uword) error code, XMLERR_OK [0] for success

    ARGUMENTS
        stream     (IN)  stream descriptor
        len        (IN)  number of bytes to write
        buf        (IN)  buffer to write
        lenwritten (OUT) bytes actually written
        rsvd       (IN)  reserved for future use [must be 0]

    SEE ALSO
        xmlstream, XMLSTREAM_OPENF, XMLSTREAM_CLOSEF, xslsetoutputstream,
        xslgetoutputstream
*/
#define XMLSTREAM_WRITEF(func) \
    uword func(xmlstream *stream, size_t len, oratext *buf, \
               size_t *lenwritten, ub4 rsvd)

/*---------------------------------------------------------------------------
    NAME
        XMLSTREAM_CLOSEF [xmlstream]

    FUNCTION
        Stream close callback

    DESCRIPTION
        Macro which defines the prototype for a stream close function.

    RETURNS
        (uword) error code, XMLERR_OK [0] for success

    ARGUMENTS
        stream     (IN)  stream descriptor
        rsvd       (IN)  reserved for future use [must be 0]

    SEE ALSO
        xmlstream, XMLSTREAM_OPENF, XMLSTREAM_WRITEF, xslsetoutputstream,
        xslgetoutputstream
*/
#define XMLSTREAM_CLOSEF(func) \
    uword func(xmlstream *stream, ub4 rsvd)

/*---------------------------------------------------------------------------*/

/* DATATYPE xmlstream - User-defined stream
    Generic user-defined stream object
*/
struct xmlstream {
    XMLSTREAM_OPENF((*open));
    XMLSTREAM_WRITEF((*write));
    XMLSTREAM_CLOSEF((*close));
    /* User can associate any data to xmlstream structure using this member
       so that he can access that inside the callbacks which will always
       have xmlstream as the first argument.  */
    void *userdata;
};

/*---------------------------------------------------------------------------
    NAME
        XML_MSGHDLRF [xmlinit]

    FUNCTION
        Error message handler

    DESCRIPTION
        This macro is used to define an error-message-handling function to
        be passed to xmlinit or xmlinitenc; if not defined, errors will be
        printed to stderr.

    RETURNS
        (void)

    ARGUMENTS
        msgctx  (IN) user-define message handler context
        msg     (IN) text of error message
        errcode (IN) numeric error code

    SEE ALSO
       xmlinit, xmlinitenc
*/
#define XML_MSGHDLRF(func) \
    void func(void *msgctx, const oratext *msg, uword errcode)

typedef XML_MSGHDLRF((*xml_msghdlr));

/*---------------------------------------------------------------------------
                           Package SAX - SAX APIs

    SAX is a standard interface for event-based XML parsing, developed
    collaboratively by the members of the XML-DEV mailing list.

    To use SAX, an xmlsaxcb structure is initialized with function
    pointers and passed to the xmlinit call. A pointer to a user-defined
    context structure may also be included; that context pointer will be
    passed to each SAX function.

  ---------------------------------------------------------------------------*/

#ifndef XMLSAXCB_DEFINED
# define XMLSAXCB_DEFINED

/****************************************************************************
                               Interface SAX
 ****************************************************************************/

/* SAX callback structure.  Set function pointers for those events you wish
   to receive, and make the rest NULL.  The entire structure should be
   initialized to zeros so that unused pointers at the end of the struct
   are NULL (to avoid future problems when they are defined to new events)

   All SAX functions return a (sword) error code.  A zero values means
   success and processing will continue; any non-zero value indicates a
   failure and parsing stops.

   Pointers passed to SAX callbacks (names, character data, etc), are only
   valid for the duration of the callback!  Once it has returned, the memory
   pointed to will be recycled and written over.  If you need to persistently
   keep the datums, you must copy them.
*/

struct xmlsaxcb
{
/*---------------------------------------------------------------------------
    NAME
        startDocument [SAX 1]

    FUNCTION
        Receives SAX start-of document notification

    DESCRIPTION
        The first SAX event, called once per document, indicating the
        start of the document.  Matching event is endDocument.

    EXAMPLE
        <?xml version="1.0"?>
        <top/>
          -SAX->
        xmlDecl(version="1.0", encoding=NULL)
        startDocument()
        ...

    RETURNS
        (sword) error code, XMLERR_OK [0] for success

    ARGUMENTS
        ctx (IN) user's SAX context [see xmlinit]

    SEE ALSO
        xmlinit, endDocument
*/
    sword (*startDocument)(void *ctx);

/*---------------------------------------------------------------------------
    NAME
        endDocument [SAX 1]

    FUNCTION
        Receives SAX end-of-document notification

    DESCRIPTION
        The last SAX event, called once per document, indicating the
        end of the document.  Matching event is startDocument.

    EXAMPLE
        <?xml version="1.0"?>
        <top/>
          -SAX->
        xmlDecl(version="1.0", encoding=NULL)
        ...
        endDocument()

    RETURNS
        (sword) error code, XMLERR_OK [0] for success

    ARGUMENTS
        ctx (IN) user's SAX context [see xmlinit]

    SEE ALSO
        xmlinit, startDocument
*/
    sword (*endDocument)(void *ctx);

/*---------------------------------------------------------------------------
    NAME
        startElement [SAX 1]

    FUNCTION
        Receives SAX start-of-element notification

    DESCRIPTION
        This event marks the start of an element.  Note this is the original
        SAX 1 non-namespace-aware version; startElementNS is the SAX 2
        namespace-aware version.  If both are registered, only the NS version
        will be called.  The element's name will be in the data encoding,
        as are all the attribute parts.  See the functions in the NamedNodeMap
        interface for operating on the attributes map.  The matching function
        is endElement (there is no endElementNS).

    EXAMPLE
        <?xml version="1.0"?>
        <top/>
          -SAX->
        xmlDecl(version="1.0", encoding=NULL)
        ...
        startElement(name="top", attrs=NULL)
        endElement(name="top")
        ...

    RETURNS
        (sword) error code, XMLERR_OK [0] for success

    ARGUMENTS
        ctx   (IN) user's SAX context [see xmlinit]
        name  (IN) name of element [in data encoding]
        attrs (IN) NamedNodeMap of element's attributes, or NULL

    SEE ALSO
        xmlinit, endElement, getNodeMapLength, getNamedItem, getChildNode
*/
    sword (*startElement)(void *ctx, const oratext *name, 
                          const struct xmlnodes *attrs);

/*---------------------------------------------------------------------------
    NAME
        endElement [SAX 1]

    FUNCTION
        Receives SAX end-of-element notification

    DESCRIPTION
        This event marks the close of an element; it matches the startElement
        or startElementNS events.  The name is the tagName of the element
        (which may be a QName for namespace-aware elements) and is in the data
        encoding.

    EXAMPLE
        <?xml version="1.0"?>
        <top/>
          -SAX->
        xmlDecl(version="1.0", encoding=NULL)
        ...
        startElement(name="top", attrs=NULL)
        endElement(name="top")
        ...

    RETURNS
        (sword) error code, XMLERR_OK [0] for success

    ARGUMENTS
        ctx  (IN) user's SAX context [see xmlinit]
        name (IN) name of ending element [in data encoding]

    SEE ALSO
        xmlinit, endElement
*/
    sword (*endElement)(void *ctx, const oratext *name);

/*---------------------------------------------------------------------------
    NAME
        characters [SAX 1]

    FUNCTION
        Receives SAX notification of character data

    DESCRIPTION
        This event marks character data, either Text or CDATA.  If a cdata
        callback is provided, then CDATA will be send to that instead; with
        no cdata callback, both Text and CDATA go to the characters callback.
        The data will be in the data encoding, and the returned length is in
        characters, not bytes.  See also ignorableWhitespace, which receiving
        notification about ignorable (whitespace formatting) character data.

    EXAMPLE
        <?xml version="1.0"?>
        <top>junk</top>
          -SAX->
        xmlDecl(version="1.0", encoding=NULL)
        ...
        characters(ch="junk", len=4)
        ...

    RETURNS
        (sword) error code, XMLERR_OK [0] for success

    ARGUMENTS
        ctx (IN) user's SAX context [see xmlinit]
        ch  (IN) pointer to data [in data encoding]
        len (IN) length of data [in characters]

    SEE ALSO
        xmlinit, ignorableWhitespace
*/
    sword (*characters)(void *ctx, const oratext *ch, size_t len);

/*---------------------------------------------------------------------------
    NAME
        ignorableWhitespace [SAX 1]

    FUNCTION
        Receives SAX notification of ignorable (whitespace) data

    DESCRIPTION
        This event marks ignorable whitespace data such as newlines, and
        indentation between lines.  The matching function is characters,
        which receives notification of normal character data.  The data
        is in the data encoding, and the returned length is in characters,
        not bytes.

    EXAMPLE
        <?xml version="1.0"?>
        <top>
            <sub>junk</sub>
        </top>
          -SAX->
        xmlDecl(version="1.0", encoding=NULL)
        ...
        startElement(name="top", attrs=NULL)
        ignorableWhitespace(ch="\n    ", len=5)
        startElement(name="sub", attrs=NULL)
        ...

    RETURNS
        (sword) error code, XMLERR_OK [0] for success

    ARGUMENTS
        ctx (IN) user's SAX context [see xmlinit]
        ch  (IN) pointer to data [in data encoding]
        len (IN) length of data [in characters]

    SEE ALSO
        xmlinit, characters
*/
    sword (*ignorableWhitespace)(void *ctx, const oratext *ch, size_t len);

/*---------------------------------------------------------------------------
    NAME
        processingInstruction [SAX 1]

    FUNCTION
        Receives SAX notification of a processing instruction

    DESCRIPTION
        This event marks a processing instruction.  The PI's target and
        data will be in the data encoding.  There is always a target, but
        the data may be NULL.

    EXAMPLE
        <?xml version="1.0"?>
        <?keywords sax example?>
        <top/>
          -SAX->
        xmlDecl(version="1.0", encoding=NULL)
        processingInstruction(target="keywords", data="sax example")
        ...

    RETURNS
        (sword) error code, XMLERR_OK [0] for success

    ARGUMENTS
        ctx    (IN) user's SAX context [see xmlinit]
        target (IN) PI's target [in data encoding]
        data   (IN) PI's data [in data encoding] or NULL

    SEE ALSO
        xmlinit
*/
    sword (*processingInstruction)(void *ctx, const oratext *target, 
                                   const oratext *data);

/*---------------------------------------------------------------------------
    NAME
        notationDecl [SAX 1]

    FUNCTION
        Receives SAX notification of a notation declaration

    DESCRIPTION
        The even marks the declaration of a notation in the DTD.  The
        notation's name, public ID, and system ID will all be in the
        data encoding.  Both IDs are optional and may be NULL.

    EXAMPLE
        <?xml version="1.0"?>
        <!DOCTYPE foo [
            <!NOTATION foo PUBLIC "[" "null.ent">
        ]>
        ...
          -SAX->
        xmlDecl(version="1.0", encoding=NULL)
        notationDecl(name="foo", publicID="[", systemID="null.ent")

    RETURNS
        (sword) error code, XMLERR_OK [0] for success

    ARGUMENTS
        ctx      (IN) user's SAX context [see xmlinit]
        name     (IN) notation's name [in data encoding]
        publicID (IN) notation's public ID [in data encoding] or NULL
        systemID (IN) notation's system ID [in data encoding] or NULL

    SEE ALSO
        xmlinit
*/
    sword (*notationDecl)(void *ctx, const oratext *name, 
                          const oratext *publicId, const oratext *systemId);

/*---------------------------------------------------------------------------
    NAME
        unparsedEntityDecl [SAX 1]

    FUNCTION
        Receives SAX notification of a unparsed entity declaration

    DESCRIPTION
        Marks an unparsed entity declaration in the DTD; see parsedEntityDecl
        for parsed entity notification.  The unparsed entity's name,
        public ID, system ID, and notation name will all be in the data
        encoding.

    EXAMPLE
        <?xml version="1.0"?>
        <!DOCTYPE foo [
            <!ENTITY e PUBLIC "p-p-pub-id" 'entity.dat' NDATA endayta>
        ]>
        ...
          -SAX->
        xmlDecl(version="1.0", encoding=NULL)
        unparsedEntityDecl(name="e", publicID="p-p-pub-id",
                           systemID="entity.dat", notationName="endayta")

    RETURNS
        (sword) error code, XMLERR_OK [0] for success

    ARGUMENTS
        ctx          (IN) user's SAX context [see xmlinit]
        name         (IN) entity's name [in data encoding]
        publicID     (IN) entity's public ID [in data encoding] or NULL
        systemID     (IN) entity's system ID [in data encoding]
        notationName (IN) entity's notation name [in data encoding]

    SEE ALSO
        xmlinit, parsedEntityDecl
*/
    sword (*unparsedEntityDecl)(void *ctx, const oratext *name, 
                                const oratext *publicId,
                                const oratext *systemId, 
                                const oratext *notationName);

/*---------------------------------------------------------------------------
    NAME
        nsStartElement [SAX 2]

    FUNCTION
        Receives SAX namespace-aware start-of-element notification

    DESCRIPTION
        This event marks the start of an element.  Note this is the new
        SAX 2 namespace-aware version; startElement is the SAX 1
        non-namespace-aware version.  If both are registered, only the NS
        version will be called.  The element's QName, local name, and
        namespace URI will be in the data encoding, as are all the attribute
        parts.  See the functions in the NamedNodeMap interface for
        operating on the attributes map.  The matching function is
        endElement (there is no endElementNS).

    EXAMPLE
        <?xml version="1.0"?>
        <foo:top xmlns:foo="/foo/bar"/>
          -SAX->
        xmlDecl(version="1.0", encoding=NULL)
        ...
        startElement(qname="foo:top", local="top", nsp="/foo/bar", attrs=NULL)
        endElement(name="foo:top")
        ...

    RETURNS
        (sword) error code, XMLERR_OK [0] for success

    ARGUMENTS
        ctx   (IN) user's SAX context [see xmlinit]
        qname (IN) element's qualified name [in data encoding]
        local (IN) element's namespace local name [in data encoding]
        nsp   (IN) element's namespace URI [in data encoding]
        attrs (IN) NamedNodeMap of element's attributes, or NULL

    SEE ALSO
        xmlinit, startElement
*/
    sword (*nsStartElement)(void *ctx, const oratext *qname, 
                            const oratext *local, const oratext *nsp,
                            const struct xmlnodes *attrs);

/*---------------------------------------------------------------------------
    NAME
        comment [SAX 2]

    FUNCTION
        Receives SAX notification of a comment

    DESCRIPTION
        This event marks a comment in the XML document.  The comment's data
        will be in the data encoding.

    EXAMPLE
        <?xml version="1.0"?>
        <!--Just a comment-->
        <foo/>
          -SAX->
        xmlDecl(version="1.0", encoding=NULL)
        comment(data="Just a comment")
        ...

    RETURNS
        (sword) error code, XMLERR_OK [0] for success

    ARGUMENTS
        ctx  (IN) user's SAX context [see xmlinit]
        data (IN) comment's data [in data encoding]

    SEE ALSO
        xmlinit
*/
    sword (*comment)(void *ctx, const oratext *data);

/*---------------------------------------------------------------------------
    NAME
        elementDecl [SAX X]

    FUNCTION
        Receives SAX notification of an element's declaration

    DESCRIPTION
        This event marks an element declaration in the DTD.  The element's
        name and content will be in the data encoding.

    EXAMPLE
        <?xml version="1.0"?>
        <!DOCTYPE foo [
            <!ELEMENT e (#PCDATA)>
        ]>
        ...
          -SAX->
        xmlDecl(version="1.0", encoding=NULL)
        elementDecl(name="e", content="(#PCDATA)")
        ...

    RETURNS
        (sword) error code, XMLERR_OK [0] for success

    ARGUMENTS
        ctx     (IN) user's SAX context [see xmlinit]
        name    (IN) element's name
        content (IN) element's context model

    SEE ALSO
        xmlinit, attributeDecl
*/
    sword (*elementDecl)(void *ctx, const oratext *name,const oratext *content);

/*---------------------------------------------------------------------------
    NAME
        attributeDecl [SAX X]

    FUNCTION
        Receives SAX notification of an attribute's declaration

    DESCRIPTION
        This event marks an element declaration in the DTD.  The element's
        name and content will be in the data encoding.  Note that an
        attribute may be declared before the element it belongs to!

    EXAMPLE
        <?xml version="1.0"?>
        <!DOCTYPE foo [
            <!ATTLIST a id ID #IMPLIED>
        ]>
        ...
          -SAX->
        xmlDecl(version="1.0", encoding=NULL)
        attributeDecl(elem="a", name="id", body="ID #IMPLIED")
        ...

    RETURNS
        (sword) error code, XMLERR_OK [0] for success

    ARGUMENTS
        ctx  (IN) user's SAX context [see xmlinit]
        elem (IN) element that attribute is declared for [in data encoding]
        attr (IN) attribute's name [in data encoding]
        body (IN) body of attribute declaration [in data encoding]

    SEE ALSO
        xmlinit, attributeDecl
*/
    sword (*attributeDecl)(void *ctx, const oratext *elem,
                           const oratext *attr, const oratext *body);

/*---------------------------------------------------------------------------
    NAME
        xmlDecl [SAX X]

    FUNCTION
        Receives SAX notification of an XML declaration

    DESCRIPTION
        This event marks an XML declaration (XMLDecl).  The startDocument
        event is always first; if this callback is registered and an XMLDecl
        exists, it will be the second event.  The encoding flag says whether
        an encoding was specified.  Since the document's own encoding
        specification may be overridden, and the input will be converted to
        the data encoding anyway, the actual encoding specified is not
        provided.  getEncoding returns the name of the data encoding.  For
        the standalone flag, -1 will be returned if it was not specified,
        otherwise 0 for FALSE, 1 for TRUE.

    EXAMPLE
        <?xml version="1.0"?>
        <top/>
          -SAX->
        xmlDecl(version="1.0", encoding=FALSE)
        ...

    RETURNS
        (sword) error code, XMLERR_OK [0] for success

    ARGUMENTS
        ctx        (IN) user's SAX context [see xmlinit]
        version    (IN) version string from XMLDecl [in data encoding]
        encoding   (IN) encoding was specified?
        standalone (IN) value of standalone-document flag [< 0 not specified]

    SEE ALSO
        xmlinit, getEncoding
*/
    sword (*xmlDecl)(void *ctx, const oratext *version,
                     boolean encoding, sword standalone);

/*---------------------------------------------------------------------------
    NAME
        cdata [SAX 1]

    FUNCTION
        Receives SAX notification of CDATA

    DESCRIPTION
        This event marks CDATA, as distinct from Text.  If no CDATA
        callback is provided, the Text callback will be invoked.  The
        data will be in the data encoding, and the returned length is
        in characters, not bytes.  See also ignorableWhitespace, which
        receiving notification about ignorable (whitespace formatting)
        character data.

    EXAMPLE
        <?xml version="1.0"?>
        <doc><![CDATA [ stuff]]></doc>
          -SAX->
        xmlDecl(version="1.0", encoding=NULL)
        ...
        cdata(ch=" stuff", len=6)
        ...

    RETURNS
        (sword) error code, XMLERR_OK [0] for success

    ARGUMENTS
        ctx (IN) user's SAX context [see xmlinit]
        ch  (IN) pointer to CDATA [in data encoding]
        len (IN) length of CDATA [in characters]

    SEE ALSO
        xmlinit, ignorableWhitespace
*/
    sword (*cdata)(void *ctx, const oratext *ch, size_t len);

/*---------------------------------------------------------------------------
    NAME
        parsedEntityDecl

    FUNCTION
        Receives SAX notification of a parsed entity declaration

    STATUS
        SAX 1

    DESCRIPTION
        Marks an parsed entity declaration in the DTD.  The parsed entity's
        name, public ID, system ID, and notation name will all be in the
        data encoding.

    EXAMPLE
        <?xml version="1.0"?>
        <!DOCTYPE foo [
            <!ENTITY e1 "foobar">
            <!ENTITY e2 SYSTEM "nop.ent">
            <!ENTITY % e3 "parameter_entity_value">
        ]>
        ...
          -SAX->
        xmlDecl(version="1.0", encoding=NULL)
        parsedEntityDecl(name="e1", value="foobar", pubID=NULL, sysID=NULL,
                        general=TRUE)
        parsedEntityDecl(name="e2", value=NULL, pubID=NULL, sysID="nop.ent",
                        general=TRUE)
        parsedEntityDecl(name="e3", value="parameter_entity_value",
                        pubID=NULL, sysID=NULL, general=FALSE)

    RETURNS
        (sword) error code, XMLERR_OK [0] for success

    ARGUMENTS
        ctx     (IN) user's SAX context [see xmlinit]
        name    (IN) entity's name [data encoding]
        value   (IN) entity's value if internal [data encoding]
        pubId   (IN) entity's public ID [data encoding] or NULL
        sysId   (IN) entity's system ID [data encoding]
        general (IN) general entity?  parameter entity if FALSE.

    SEE ALSO
        unparsedEntityDecl
*/
    sword (*parsedEntityDecl)(void *ctx, const oratext *name,
                              const oratext *value, const oratext *pubId,
                              const oratext *sysId, boolean general);

/* The following 2 fields are reserved for future use. */
   void (*empty1)();
   void (*empty2)();
};

typedef struct xmlsaxcb xmlsaxcb;

#endif /* ifndef XMLSAXCB_DEFINED */

/*---------------------------------------------------------------------------
                      Package Parser - Top-Level APIs

    This C implementation of the XML processor (or parser) follows the W3C
    XML specification (rev REC-xml-19980210) and implements the required
    behavior of an XML processor in terms of how it must read XML data and
    the information it must provide to the application.

  ---------------------------------------------------------------------------*/

/****************************************************************************
                           Interface XMLParser
 ****************************************************************************/

/*---------------------------------------------------------------------------
    NAME
        xmlinit [DOM X]

    FUNCTION
        Initializes XML parser

    DESCRIPTION
        Initializes the C XML parser and returns a new parser context.  Must
        be called before any parsing can take place.  If this call is not
        successful, do not call any other XML parser functions!

        xmlinitenc is the same as xmlinit except that the DOM/SAX data
        encoding may be specified.  For xmlinit, the data encoding will match
        that of the first document parsed.

        This function should only be called once before parsing any XML files.
        xmlterm should be called after all parsing and DOM use has completed.
        Multiple parses should call xmlclean between runs if only the current
        document needs to be available.  Until clean is called, data pointers
        from all previous parses will continue to be valid.

        All arguments may be NULL except for err, which is required.  On
        success, an XML context (xmlctx *) is returned.  If this is NULL, a
        failure occurred: the error code is stored in *err, and depending
        on how far along in the initialization it got, an error message may
        be produced.

        DATA ENCODING: The encoding of input documents is detected
        automatically (by BOM, XMLDecl, etc).  If the encoding cannot be
        determined, 'incoding' is assumed.  If 'incoding' is not specified
        (NULL), UTF-8 is assumed.  'incoding' should be an IANA/Mine
        encoding name, e.g. "UTF-16", "ASCII", etc.

        The presentation encoding (via DOM/SAX) is automatically set to the
        encoding of the first document parsed.  See also the xmlinitenc
        function, which allows the data encoding to explicitly set.

        DOM vs SAX: By default, a DOM parse tree is built.  To use SAX
        instead, specify a SAX callback structure (saxcb); the presence of
        the callback struct acts as a flag.  The callbacks will be invoked
        with the given SAX context pointer.  If any of the SAX functions
        returns a non-zero error code, parsing stops immediately.

        ERROR MESSAGES: By default, error messages are printed to stderr.  To
        handle messages yourself, specify a handler function pointer.  The
        formatted error string and numeric error code will be passed to your
        function, along with the user-defined message context msgctx.  The
        error strings will be UTF-8; any data included as part of the error
        message will be converted to UTF-8.  The location where an error
        occurred may be fetched with the xmlwhere function.  The error
        handler function should be declared with the XML_MSGHDLRF function
        prototype macro.

        ERROR LANGUAGE: The error language is specified as lang, e.g.
        "AMERICAN", JAPANESE", "FRENCH", etc, and defaults to American.

        PERFORMANCE: A separate, fast code path exists for single-byte
        encodings (ASCII, ISO-8859, EBCDIC, etc).  This path is considerably
        faster than the multibyte and Unicode paths, so if you are sure your
        input documents are single-byte, you are strongly encouraged to tell
        the parser by setting the 'incoding' hint properly.

        Since all multibyte/Unicode inputs are converted to UTF-16 internally
        for homogenization, choosing UTF-16 output is next fastest since no
        extra conversion is needed.  The slowest code path is multibyte input
        with multibyte output, since there is a round trip to UTF-16 between.

    RETURNS
        (xmlctx *) XML context or NULL

    ARGUMENTS
        err       (OUT) Numeric error code, XMLERR_OK [0] on success
        incoding  (IN)  Default input encoding [in compiler encoding]
        msghdlr   (IN)  Error message handler function
        msgctx    (IN)  Context for the error message handler
        saxcb     (IN)  SAX callback structure filled with function pointers
        saxcbctx  (IN)  Context for SAX callbacks
        memcb     (IN)  Memory function callbacks
        memcbctx  (IN)  Context for the memory function callbacks
        lang      (IN)  Language for error messages [in compiler encoding]

    SEE ALSO
        xmlinitenc, xmlclean, xmlterm, xmlmemcb
*/
xmlctx *xmlinit(uword *err, const oratext *incoding,
                XML_MSGHDLRF((*msghdlr)), void *msgctx,
                const xmlsaxcb *saxcb, void *saxcbctx, 
                const xmlmemcb *memcb, void *memcbctx,
                const oratext *lang);

/*---------------------------------------------------------------------------
    NAME
        xmlinitenc [DOM X]

    FUNCTION
        Initalizes XML parser (specifying data encoding)

    DESCRIPTION
        Initializes the C XML parser and returns a new parser context.  Details
        are the same as for xmlinit except that an additional argument is
        supplied: the data encoding.  All DOM and SAX data will be presented
        in the given data encoding.  See the documentation for xmlinit.

    RETURNS
        (xmlctx *) XML context or NULL

    ARGUMENTS
        err       (OUT) Numeric error code, XMLERR_OK [0] on success
        incoding  (IN)  Default input encoding [in compiler encoding]
        outcoding (IN)  Output [DOM/SAX data] encoding [in compiler encoding]
        msghdlr   (IN)  Error message handler function
        msgctx    (IN)  Context for the error message handler
        saxcb     (IN)  SAX callback structure filled with function pointers
        saxcbctx  (IN)  Context for SAX callbacks
        memcb     (IN)  Memory function callbacks
        memcbctx  (IN)  Context for the memory function callbacks
        lang      (IN)  Language for error messages [in compiler encoding]

    SEE ALSO
        xmlinit, xmlclean, xmlterm, xmlmemcb
*/
xmlctx *xmlinitenc(uword *err, const oratext *incoding,
                               const oratext *outcoding,
                   XML_MSGHDLRF((*msghdlr)), void *msgctx,
                   const xmlsaxcb *saxcb, void *saxcbctx, 
                   const xmlmemcb *memcb, void *memcbctx,
                   const oratext *lang);

/*---------------------------------------------------------------------------
    NAME
        xmlterm [DOM X]

    FUNCTION
        Terminates XML parser

    DESCRIPTION
        Terminates (shuts down) the XML parser.  It should be called after
        xmlinit, and before exiting the main program.

        All allocated memory is freed, giving it back to the system (through
        free or the user's memory callback).  Contrast to xmlclean, which
        recycles memory internally without giving it back to the system.

        No additional XML parser calls can be made until xmlinit is called
        again to get a new context.

    RETURNS
        (uword) numeric error code, XMLERR_OK [0] on success

    ARGUMENTS
        ctx (IN) XML context

    SEE ALSO
        xmlinit, xmlinitenc, xmlclean
*/
uword xmlterm(xmlctx *ctx);

/*---------------------------------------------------------------------------
    NAME
        xmlclean [DOM X]

    FUNCTION
        Cleans up an XML context between parses

    DESCRIPTION
        Recycles memory within the XML parser, but does not free it to the
        system-- only xmlterm finally releases all memory back to the
        system.  If xmlclean is not called between parses, then the data
        used by the previous documents remains allocated, and pointers to
        it are valid.  Thus, the data for multiple documents can be accessible
        simultaneously, although only the current document can be manipulated
        with DOM.

        If you just want to access one document's data at a time (within a
        single context), then call xmlclean before each new parse.

    RETURNS
        (void)

    ARGUMENTS
        ctx (IN) XML context

    SEE ALSO
        xmlterm
*/
void xmlclean(xmlctx *ctx);

/*---------------------------------------------------------------------------
    NAME
        xmlparse [DOM X]

    FUNCTION
        Parses XML document from a URI

    DESCRIPTION
        Parses an XML given specified by a URI.  The parser must have been
        initialized successfully with a call to xmlinit or xmlinitenc first.

        Data pointers returned by DOM APIs remain valid until xmlclean
        or xmlterm is called.  For SAX, the data pointers only remain
        valid for the duration of the user's callback function.  That is,
        once the callback function has returned, the data pointers become
        invalid.  If longer access is needed, the data must be saved,
        either in the user's own store, or in the XML memory's pool using
        stringSave (or stringSave2 for UCS2 data).

        Parser options are specified as flag bits OR'd together into the
        "flags" mask.  Flags are:

        XML_FLAG_VALIDATE turns validation on.  By default, the parser does
        not validate the input.  To validate against a DTD, set this flag.
        Validation problems are considered warnings, not errors, and by
        default validation will continue after warnings have occurred.  To
        treat warnings as errors set the flag XML_FLAG_STOP_ON_WARNING.

        XML_FLAG_DISCARD_WHITESPACE causes extraneous whitespace to be
        discarded.  The default behavior for whitespace processing is to be
        fully conformant to the XML 1.0 spec, i.e. all whitespace is reported
        back to the application, but it is indicated which whitespace is
        "ignorable".  However, some applications may prefer to set the
        XML_FLAG_DISCARD_WHITESPACE which will discard all whitespace
        between an end-element tag and the following start-element tag.

        XML_FLAG_STOP_ON_WARNING stops validation on warnings.  Validation
        problems are considered warnings (non-fatal) unless this flag is set.
        If set, validation will stop after the first warning.

        XML_FLAG_DTD_ONLY parse an external subset.  This is the same as
        calling xmlparsedtd; it parses an external subset (DTD) instead of
        a complete XML document.  Used primarily by the Class Generator so
        that it may generate classes from a DTD without need of a complete
        document.

        XML_FLAG_WARN_DUPLICATE_ENTITY causes a warning to be emitted if a
        duplicate entity declaration is found.  A duplicate entity declaration
        is usually silently ignored.  When set, this flag causes a warning to
        be emitted instead.

        XML_FLAG_FORCE_INCODING forced input documents to be interpreted
        in the encoding "incoding".  The default input encoding may be
        specified as incoding, which overrides the incoding given to xmlinit.
        If the input's encoding cannot be determined automatically (based on
        BOM, XMLDecl, etc) then it is assumed to be incoding.  IANA/Mime
        encoding names should be used, "UTF-8", "ASCII", etc.  If
        XML_FLAG_FORCE_INCODING is set, the document will be interpreted as
        "incoding" regardless.

        XML_FLAG_DONT_EXPAND_CHAR_REF when set causes character references
        to be left verbatim and not expanded.  For example, "&20;" normally
        turns into the space character, but when this flag is set, it would
        remain the four characters '&' '2' '0' and ';'.

    RETURNS
        (uword) numeric error code, XMLERR_OK [0] on success

    ARGUMENTS
        ctx      (I/O) XML context
        uri      (IN)  input URI
        incoding (IN)  default input encoding
        flags    (IN)  parse options [mask of flag bits]

    SEE ALSO
        xmlparsebuf, xmlparsefile, xmlparsestream, xmlparsedtd, xmlflags
*/
uword xmlparse(xmlctx *ctx, const oratext *uri,
               const oratext *incoding, xmlflags flags);

/*---------------------------------------------------------------------------
    NAME
        xmlparsebuf [DOM X]

    FUNCTION
        Parses XML document from a memory buffer

    DESCRIPTION
        Parses an XML given a buffer and its length.  The parser must have
        been initialized successfully with a call to xmlinit or xmlinitenc
        first.  See the notes for xmlparse for full documentation.

    RETURNS
        (uword) numeric error code, XMLERR_OK [0] on success

    ARGUMENTS
        ctx      (I/O) XML context
        buffer   (IN)  input buffer
        len      (IN)  length of buffer in bytes
        incoding (IN)  default input encoding
        flags    (IN)  parse options [mask of flag bits]

    SEE ALSO
        xmlparse, xmlparsefile, xmlparsestream, xmlparsedtd, xmlflags
*/
uword xmlparsebuf(xmlctx *ctx, const oratext *buffer, size_t len, 
                  const oratext *incoding, xmlflags flags);

/*---------------------------------------------------------------------------
    NAME
        xmlparsefile [DOM X]

    FUNCTION
        Parses XML document from a disk file

    DESCRIPTION
        Parses an XML given the path to a disk file.  The parser must have
        been initialized successfully with a call to xmlinit or xmlinitenc
        first.  See the notes for xmlparse for full documentation.

    RETURNS
        (uword) numeric error code, XMLERR_OK [0] on success

    ARGUMENTS
        ctx      (I/O)    XML context
        path     (IN)     disk file path
        incoding (IN)     default input encoding
        flags    (IN)     parse options [mask of flag bits]

    SEE ALSO
        xmlparse, xmlparsebuf, xmlparsestream, xmlparsedtd, xmlflags
*/
uword xmlparsefile(xmlctx *ctx, const oratext *path, 
                   const oratext *incoding, xmlflags flags);

/*---------------------------------------------------------------------------
    NAME
        xmlparsestream [DOM X]

    FUNCTION
        Parses XML document from a user-defined stream

    DESCRIPTION
        Parses an XML given the path from a user-defined stream.  The parser
        must have been initialized successfully with a call to xmlinit or
        xmlinitenc first.  See the notes for xmlparse for full documentation.

    RETURNS
        (uword) numeric error code, XMLERR_OK [0] on success

    ARGUMENTS
        ctx      (I/O) XML context
        stream   (IN)  user-defined stream
        incoding (IN)  default input encoding
        flags    (IN)  parse options [mask of flag bits]

    SEE ALSO
        xmlparse, xmlparsebuf, xmlparsefile, xmlparsedtd, xmlflags
*/
uword xmlparsestream(xmlctx *ctx, const void *stream, 
                     const oratext *incoding, xmlflags flags);

/*---------------------------------------------------------------------------
    NAME
        xmlparseorastream [DOM X]

    FUNCTION
        Parses XML document from an orastream

    DESCRIPTION
        Parses an XML given the path from an orastream (see orastruc.h).
        The parser must have been initialized successfully with a call to
        xmlinit or xmlinitenc first.  See the notes for xmlparse for full
        documentation.

    RETURNS
        (uword) numeric error code, XMLERR_OK [0] on success

    ARGUMENTS
        ctx      (I/O) XML context
        istream  (IN)  input orastream
        incoding (IN)  default input encoding
        flags    (IN)  parse options [mask of flag bits]

    SEE ALSO
        xmlparse, xmlparsebuf, xmlparsefile, xmlparsedtd, xmlflags
*/
uword xmlparseorastream(xmlctx *ctx, orastream *istream, 
                        const oratext *incoding, xmlflags flags);

/*---------------------------------------------------------------------------
    NAME
        xmlparsedtd [DOM X]

    FUNCTION
        Parses an external subset (DTD) specified by a URI

    DESCRIPTION
        Parses an external subset (DTD) specified by a URI.  Using this API
        is the same as calling xmlparse with XML_FLAG_DTD_ONLY set.  The
        parser must have been initialized successfully with a call to xmlinit
        or xmlinitenc first.  See the notes for xmlparse for full documentation.

    RETURNS
        (uword) numeric error code, XMLERR_OK [0] on success

    ARGUMENTS
        ctx      (I/O) XML context
        uri      (IN)  input URI
        name     (IN)  name of DTD
        incoding (IN)  default input encoding
        flags    (IN)  parse options [mask of flag bits]

    SEE ALSO
        xmlparse, xmlparsebuf, xmlparsefile, xmlparsestream, xmlflags
*/
uword xmlparsedtd(xmlctx *ctx, const oratext *filename,
                  oratext *name, const oratext *incoding, xmlflags flags);

/*---------------------------------------------------------------------------
    NAME
        xmlaccess [DOM X]

    FUNCTION
        Sets I/O callbacks for URI access method

    DESCRIPTION
        Sets the I/O callback functions for the given URI access type.  All
        three functions must be provided, and should be declared using
        the function prototype macros XML_OPENF, XML_CLOSEF and XML_READF.

    RETURNS
        (uword) numeric error code, XMLERR_OK [0] on success

    ARGUMENTS
        ctx    (IN) XML context
        access (IN) the XMLACCESS_xxx code
        openf  (IN) pointer to open function
        closef (IN) pointer to close function
        readf  (IN) pointer to read function

    SEE ALSO
        xmlacctype, XML_OPENF, XML_CLOSEF, XML_READF
*/
uword xmlaccess(xmlctx *ctx, xmlacctype access, XML_OPENF((*openf)),
                XML_CLOSEF((*closef)), XML_READF((*readf)));

/*---------------------------------------------------------------------------
    NAME
        xmlsetexternalaccess

    FUNCTION
        Enable or disable access to exteran files and protocols.

    DESCRIPTION
        This is a security feature. For internal use within servers, this
        should be called with ext_access == FALSE. That effectively
        disables the ability of the XML parser and other XML facilities to
        read from the file system, as well as to read from FTP or HTTP/HTTPS
        sources. Callers should provide access overrides if necessary to
        support these access modes via xmlaccess().

    RETURNS
        (void)

    ARGUMENTS
        ctx        (IN) XML context
        ext_access (IN) TRUE to enable access, FALSE to disable access.

    SEE ALSO
        xmlaccess
*/
void xmlsetexternalaccess(xmlctx *xctx, boolean ext_access);

/*---------------------------------------------------------------------------
    NAME
        isStandalone [DOM X]

    FUNCTION
        Returns value of standalone flag

    DESCRIPTION
        Returns the value of the standalone flag as specified in the
        document's XMLDecl.

    RETURNS
        (boolean) value of standalone flag

    ARGUMENTS
        ctx (IN) XML context
*/
boolean isStandalone(xmlctx *ctx);

/*---------------------------------------------------------------------------
    NAME
        isSingleChar [DOM X]

    FUNCTION
        Returns single-byte data encoding flag

    DESCRIPTION
        Returns a flag specifying if the data encoding is single-byte
        (ASCII, ISO-8859, EBCDIC, etc).  If FALSE, the data encoding
        may be multibyte or Unicode.

    RETURNS
        (boolean) value of single-char flag

    ARGUMENTS
        ctx (IN) XML context

    SEE ALSO
        isUnicode, getEncoding
*/
boolean isSingleChar(xmlctx *ctx);

/*---------------------------------------------------------------------------
    NAME
        isUnicode [DOM X]

    FUNCTION
        Returns Unicode data flag

    DESCRIPTION
        Returns a flag specifying whether the data encoding is Unicode
        (UCS2).  If FALSE, the data may be either single-byte or multibyte.

    RETURNS
        (boolean) value of Unicode flag

    ARGUMENTS
        ctx (IN) XML context

    SEE ALSO
        isSingleChar, getEncoding
*/
boolean isUnicode(xmlctx *ctx);

/*---------------------------------------------------------------------------
    NAME
        getEncoding [DOM X]

    FUNCTION
        Returns name of data encoding.

    DESCRIPTION
        This function returns the IANA/Mime name of the data encoding, e.g.
        "UTF-8", "ASCII", etc.

    RETURNS
        (oratext *) name of encoding

    ARGUMENTS
        ctx (IN) XML context

    SEE ALSO
        isSingleChar, isUnicode
*/
oratext *getEncoding(xmlctx *ctx);

/*---------------------------------------------------------------------------
    NAME
        saveString [DOM X]

    FUNCTION
        Saves single/multibyte string in XML context memory

    DESCRIPTION
        Allocates memory and saves the NULL-terminated single- or multi-byte
        string in the XML string pool.  Strings saved this way cannot be freed
        individually since they are stored head-to-tail in a single pool, for
        maximum compactness.  The memory is reused only when the entire pool is
        freed, after an xmlclean or xmlterm call.  Use saveString2 for saving
        Unicode strings.

    RETURNS
        (oratext *) pointer to saved string

    ARGUMENTS
        ctx (IN) XML context
        str (IN) pointer to single- or multi-byte string

    SEE ALSO
        saveString2
*/
oratext *saveString(xmlctx *ctx, oratext *str);

/*---------------------------------------------------------------------------
    NAME
        saveString2 [DOM X]

    FUNCTION
        Saves Unicode string in XML context memory

    DESCRIPTION
        Allocates memory and saves the NULL-terminated Unicode string in the
        XML string pool.  Note that a Unicode string is terminated with TWO
        NULL bytes, not just one!  Strings saved this way cannot be freed
        individually since they are stored head-to-tail in a single pool, for
        maximum compactness.  The memory is reused only when the entire pool
        is freed, after an xmlclean or xmlterm call.  Use saveString for
        saving single- or multi-byte strings.

    RETURNS
        (oratext *) pointer to saved Unicode string

    ARGUMENTS
        ctx  (IN) XML context
        ustr (IN) pointer to Unicode string

    SEE ALSO
        saveString
*/
ub2 *saveString2(xmlctx *ctx, ub2 *ustr);

/*---------------------------------------------------------------------------
    NAME
        printStream [DOM X]

    FUNCTION
        Prints XML tree to stdio stream

    DESCRIPTION
        Pretty-prints (formats) an XML document rooted at the given node
        to a stdio stream.  The top level is indented step*level spaces,
        the next level step*(level+1) spaces, etc.  Output will be in the
        data encoding.

    RETURNS
        (void)

    ARGUMENTS
        stream (IN) output stdio stream [stdout, etc]
        node   (IN) root node to print
        step   (IN) spaces to indent each level [0-4]
        level  (IN) initial nesting level [0 for root]

    SEE ALSO
        printStreamEnc, printOraStream, printBuffer, printSize
*/
void printStream(FILE *stream, xmlnode *node, uword step, uword level);

/*---------------------------------------------------------------------------
    NAME
        printStreamEnc [DOM X]

    FUNCTION
        Prints XML tree to stdio stream in specified encoding

    DESCRIPTION
        Pretty-prints (formats) an XML document rooted at the given node
        to a stdio stream.  The entire document will be in the encoding
        specified (by name); if no encoding is given, the document is
        output in its current (data) encoding.  The top level is indented
        step*level spaces, the next level step*(level+1) spaces, etc.  Flags:

        XML_FLAG_PRINT_XMLDECL says to always print an XMLDecl before
        the root element.  If (node==DOC), an XMLDecl is always printed.

    RETURNS
        (size_t) total number of bytes written to stream

    ARGUMENTS
        stream   (IN) output stdio stream [stdout, etc]
        node     (IN) root node to print
        step     (IN) spaces to indent each level [0-4]
        level    (IN) initial nesting level [0 for root]
        encoding (IN) printed encoding [NULL for data current encoding]
        flags    (IN) mask of flag bits

    SEE ALSO
        printStreamEnc, printOraStream, printBuffer, printSize
*/
size_t printStreamEnc(FILE *stream, xmlnode *node, uword step, uword level,
                      oratext *encoding, xmlpflags flags);

/*---------------------------------------------------------------------------
    NAME
        printOraStream [DOM X]

    FUNCTION
        Prints XML tree to orastream in specified encoding

    DESCRIPTION
        Pretty-prints (formats) an XML document rooted at the given node
        to an orastream.  The entire document will be in the encoding
        specified (by name); if no encoding is given, the document is
        output in its current (data) encoding.  The top level is indented
        step*level spaces, the next level step*(level+1) spaces, etc.  Flags:

        XML_FLAG_PRINT_XMLDECL says to always print an XMLDecl before
        the root element.  If (node==DOC), an XMLDecl is always printed.

    RETURNS
        (size_t) total number of bytes written to stream

    ARGUMENTS
        ostream  (IN) output orastream
        node     (IN) root node to print
        step     (IN) spaces to indent each level [0-4]
        level    (IN) initial nesting level [0 for root]
        encoding (IN) printed encoding [NULL for data current encoding]
        flags    (IN) mask of flag bits

    SEE ALSO
        printStream, printStreamEnc, printBuffer, printSize
*/
size_t printOraStream(orastream *ostream, xmlnode *node, uword step,
                      uword level, oratext *encoding, xmlpflags flags);

/*---------------------------------------------------------------------------
    NAME
        printBuffer [DOM X]

    FUNCTION
        Prints XML tree to memory buffer

    DESCRIPTION
        Pretty-prints an XML document rooted at the given node to a buffer.
        The top level is indented step*level spaces, the next level
        step*(level+1) spaces, etc.  Output will be in the data encoding.

    RETURNS
        (void)

    ARGUMENTS
        buffer (IN) output buffer
        bufsiz (IN) size of buffer
        node   (IN) root node to print
        step   (IN) spaces to indent each level [0-4]
        level  (IN) initial nesting level [0 for root]

    SEE ALSO
        printBufferEnc, printCallbackEnc, printStream,
        printStreamEnc, printOraStream, printSize
*/
void printBuffer(oratext *buffer, size_t bufsiz, xmlnode *node,
                 uword step, uword level);

/*---------------------------------------------------------------------------
    NAME
        printBufferEnc [DOM X]

    FUNCTION
        Prints XML tree to memory buffer in specified encoding

    DESCRIPTION
        Pretty-prints an XML document rooted at the given node to a buffer.
        The entire document will be in the encoding specified (by name); if
        no encoding is given, the document is output in its current (data)
        encoding.  The top level is indented step*level spaces, the next level
        step*(level+1) spaces, etc.  Output will be in the data encoding.
        Flags:

        XML_FLAG_PRINT_XMLDECL says to always print an XMLDecl before
        the root element.  If (node==DOC), an XMLDecl is always printed.

    RETURNS
        (size_t) total number of bytes written to stream

    ARGUMENTS
        buffer   (IN) output buffer
        bufsiz   (IN) size of buffer
        node     (IN) root node to print
        step     (IN) spaces to indent each level [0-4]
        level    (IN) initial nesting level [0 for root]
        encoding (IN) printed encoding [NULL for data current encoding]
        flags    (IN) mask of flag bits

    SEE ALSO
        printBuffer, printCallbackEnc, printStream,
        printStream, printStreamEnc, printOraStream, printSize
*/
size_t printBufferEnc(oratext *buffer, size_t bufsiz, xmlnode *node,
                      uword step, uword level, oratext *encoding,
                      xmlpflags flags);

/*---------------------------------------------------------------------------
    NAME
        printSize [DOM X]

    FUNCTION
        Returns size of formatted XML tree

    DESCRIPTION
        Returns the length of XML printed representation, without actually
        outputting anything.  Can be used to calculate the size of the buffer
        needed for allocation.  Caution, about as expensive as formatting it
        in the first place!  The top level is indented step*level spaces,
        the next level step*(level+1) spaces, etc.  Formatting is in the
        data encoding, however size is returned in BYTES, not characters.

    RETURNS
        (size_t) formatted length in bytes

    ARGUMENTS
        node   (IN) root node
        step   (IN) spaces to indent each level [0-4]
        level  (IN) initial nesting level [0 for root]

    SEE ALSO
        printSizeEnc, printStream, printBuffer
*/
size_t printSize(xmlnode *node, uword step, uword level);

/*---------------------------------------------------------------------------
    NAME
        printSizeEnc [DOM X]

    FUNCTION
        Returns size of formatted XML tree in specified encoding

    DESCRIPTION
        Returns the length of the XML printed representation of a
        subtree (in the specified encoding), without actually printing
        anything.  If no encoding is given, the current (data) encoding
        is used.  Caution, about as expensive as formatting it in the
        first place!  The top level is indented step*level spaces,
        the next level step*(level+1) spaces, etc.  Formatting is in the
        data encoding, however size is returned in BYTES, not characters.

        XML_FLAG_PRINT_XMLDECL says to always print an XMLDecl before
        the root element.  If (node==DOC), an XMLDecl is always printed.

    RETURNS
        (size_t) size of formatted subtree in bytes

    ARGUMENTS
        node     (IN) root node
        step     (IN) spaces to indent each level [0-4]
        level    (IN) initial nesting level [0 for root]
        encoding (IN) printed encoding [NULL for data current encoding]
        flags    (IN) mask of flag bits

    SEE ALSO
        printSize, printStream, printBuffer
*/
size_t printSizeEnc(xmlnode *node, uword step, uword level,
                    oratext *encoding, xmlpflags flags);

/*---------------------------------------------------------------------------
    NAME
        freeElements [DOM X]

    FUNCTION
        Frees an allocated list of element nodes

    DESCRIPTION
        Frees an allocated list of element nodes.  Used primarily to free the
        lists created by getElementsByTagName.

    RETURNS
        (void)

    ARGUMENTS
        ctx  (IN) XML context
        list (IN) list of nodes to free

    SEE ALSO
        getElementsByTagName
*/
void freeElements(xmlctx *ctx, xmlnodes *list);

/*---------------------------------------------------------------------------
    NAME
        setDocOrder [DOM X]

    FUNCTION
        Sets the document order for each node in a Document

    DESCRIPTION
        Sets the document order for each node in the current document.  Must
        be called once on the final document before XSLT processing can occur.
        Note this is called automatically by the XSLT processor, so ordinarily
        the user need not make this call.

    RETURNS
        (ub4) last ID# assigned

    ARGUMENTS
        ctx      (IN) XML context
        start_id (IN) initial ID# to assign

    SEE ALSO
        getDocOrder
*/
ub4 setDocOrder(xmlctx *ctx, ub4 start_id);

/*
** Stubs for deprecated routines,
** retained for link compatibility with old code.
*/
#ifdef ORAXML_STUBS
void printCallback(XML_PRINTF((*func)), void *ctx, xmlnode *node,
                   uword step, uword level);

size_t printCallbackEnc(XML_PRINTF((*func)), void *ctx, xmlnode *node,
                        uword step, uword level, oratext *encoding,
                        xmlpflags flags);
#endif

/*---------------------------------------------------------------------------
    NAME
        xmlWhere [DOM X]

    FUNCTION
        Returns the location where an error occured

    DESCRIPTION
        Returns the location (line#, path) where an error occurred.  The
        error location is a stack: to fetch all items, set index=0 and
        call repeatedly, incrementing index until FALSE is returned.
        This function must be called from the user's own error callback
        function, while the error stack is current.  It will NOT return
        valid information after the top-level parse functions have
        returned, only during the commission of an error!

    RETURNS
        (boolean) idx is valid, an error occurred

    ARGUMENTS
        ctx  (IN)  XML context
        line (OUT) line# where err occurred
        path (OUT) path/URL where err occurred
        idx  (IN)  location in error stack (starting at 0)

    SEE ALSO
        xmlLocation
*/
boolean xmlwhere(xmlctx *ctx, ub4 *line, oratext **path, uword idx);
#define xmlWhere xmlwhere

/*---------------------------------------------------------------------------
    NAME
        xmlLocation [DOM X]

    FUNCTION
        Returns the current location while parsing

    DESCRIPTION
        Returns the current location (line#, path) while parsing.  If
        called before or after parsing, 0 and NULL will be returned
        respectively.  The path will be either the URL or filespec;
        for other parsing methods (from stream or buffer), NULL is
        returned.

    RETURNS
        (void)

    ARGUMENTS
        ctx  (IN)  XML context
        line (OUT) current line#
        path (OUT) current path/URL

    SEE ALSO
        xmlWhere
*/
void xmlLocation(xmlctx *ctx, ub4 *line, oratext **path);

/*---------------------------------------------------------------------------
    NAME
        associateDomDocument [DOM X]

    FUNCTION
        Mechanism to associate xdkdomdoc with xmlctx

    DESCRIPTION
        Associates an xdkdomdoc to a given xmlctx.  This association allows
        usage of the XSLT, XPath, and XML Schema Validator APIs with an
        xdkdomdoc.   The XDB DOM (OCIDomDocument) object is one particular
        type of xdkdomdoc.  xdkdomdoc is defined in xdkdomdoc.h.

    RETURNS
        (void)

    ARGUMENTS
        ctx  (IN) XML context
        doc  (IN)  xdkdomdoc to be associated
*/
void associateDomDocument(xmlctx *ctx, void *doc);

/*---------------------------------------------------------------------------
                           Package DOM - DOM APIs

    Since the DOM standard is object-oriented, for this C adaptation, some
    changes had to be made:

    Reused function names had to be expanded, e.g. getValue in the
    attribute class is given the unique name getAttrValue, matching
    the pattern established by getNodeValue.

    Also, some functions were added to extend the DOM.  For example,
    there is no function defined which returns the number of children
    of a node, so numChildNodes was invented, etc.

    The implementation of this DOM interface follows REC-DOM-Level-1-19981001.

  ---------------------------------------------------------------------------*/

/****************************************************************************
                               Interface Node
 ****************************************************************************/

/*---------------------------------------------------------------------------
    NAME
        getNodeName [DOM 1]

    FUNCTION
        Returns name of a node

    DESCRIPTION
        Returns the (fully-qualified) name of a node (in the data encoding),
        for example "bar" or "foo:bar".  These functions are synonyms and
        return the same thing.

        Note that some node types have fixed names: "#text", "#cdata-section",
        "#comment", "#document", "#document-fragment".

    RETURNS
        (oratext *) node's QName [in data encoding]

    ARGUMENTS
        node (IN) node whose QName to return

    SEE ALSO
        getNodeQualifiedName, getNodeNamespace, getNodePrefix, getNodeLocal
*/
const oratext *getNodeName(const xmlnode *node);

/*---------------------------------------------------------------------------
    NAME
        getNodeQualifiedName [DOM 2 Node.nodeName attribute]

    FUNCTION
        Returns name of a node

    DESCRIPTION
        Returns the (fully-qualified) name of a node (in the data encoding),
        for example "bar" or "foo:bar".  These functions are synonyms and
        return the same thing.

        Note that some node types have fixed names: "#text", "#cdata-section",
        "#comment", "#document", "#document-fragment".

    RETURNS
        (oratext *) node's QName [in data encoding]

    ARGUMENTS
        node (IN) node whose QName to return

    SEE ALSO
        getNodeName, getNodeNamespace, getNodePrefix, getNodeLocal
*/
const oratext *getNodeQualifiedName(const xmlnode *node);

/*---------------------------------------------------------------------------
    NAME
        getNodeNamespace [DOM 2 Node.namespaceURI attribute]

    FUNCTION
        Returns namespace URI of a node

    DESCRIPTION
        Returns the namespace URI for a node (in the data encoding).  If
        the node's name is not qualified (does not contain a namespace
        prefix), it will have the default namespace in effect when the
        node was created (which may be NULL).

    EXAMPLE
        <pfx:foo xmlns:pfx="example_namespace"/>
        getNodeNamespace(element pfx:foo) --> "example_namespace"

    RETURNS
        (oratext *) node's namespace URI [in data encoding] or NULL

    ARGUMENTS
        node (IN) node whose namespace URI to return

    SEE ALSO
        getNodeQualifiedName, getNodePrefix, getNodeLocal
*/
const oratext *getNodeNamespace(const xmlnode *node);

/*---------------------------------------------------------------------------
    NAME
        getNodePrefix [DOM 2 Node.prefix attribute]

    FUNCTION
        Returns namespace prefix of a node

    DESCRIPTION
        Returns the namespace prefix for a node.  If the node's name is
        not fully qualified (has no prefix), NULL is returned.

    EXAMPLE
        <pfx:foo xmlns:pfx="example_namespace"/>
        getNodePrefix(element pfx:foo) --> "pfx"

    RETURNS
        (oratext *) node's namespace prefix [in data encoding] or NULL

    ARGUMENTS
        node (IN) node whose namespace prefix to return

    SEE ALSO
        getNodeQualifiedName, getNodeNamespace, getNodeLocal, prefixToURI
*/
const oratext *getNodePrefix(const xmlnode *node);

/*---------------------------------------------------------------------------
    NAME
        getNodeLocal [DOM 2 Node.localName attribute]

    FUNCTION
        Returns namespace local name of a node

    DESCRIPTION
        Returns the namespace local name for a node.  If the node's name
        is not fully qualified (has no prefix), then the local name is
        the same as the name.

    EXAMPLE
        <pfx:foo xmlns:pfx="example_namespace"/>
        getNodeLocal(element pfx:foo) --> "foo"

    RETURNS
        (oratext *) node's namespace local name [in data encoding]

    ARGUMENTS
        node (IN) node whose namespace local name to return

    SEE ALSO
        getNodeQualifiedName, getNodeNamespace, getNodeLocal
*/
const oratext *getNodeLocal(const xmlnode *node);

/*---------------------------------------------------------------------------
    NAME
        getNodeValue [DOM 1]

    FUNCTION
        Returns value (character data) of a node

    DESCRIPTION
        Returns the "value" (associated character data) for a node.
        Character and general entities will have been replaced.  Only
        Attr, CDATA, Comment, PI and Text nodes have values, all other
        node types have NULL value.

    EXAMPLE
        "<foo>data</foo>"
        getNodeValue(node foo) --> NULL
        getNodeValue(foo's first child) --> "data"

    RETURNS
        (oratext *) node's value [in data encoding] or NULL

    ARGUMENTS
        node (IN) node whose value to return

    SEE ALSO
        setNodeValue, getAttrValue, setAttrValue
*/
const oratext* getNodeValue(const xmlnode *node);

/*---------------------------------------------------------------------------
    NAME
        setNodeValue [DOM 1]

    FUNCTION
        Sets the new value (character data) of a node

    DESCRIPTION
        Sets a node's "value" (associated character data).  Does not allow
        setting the value to NULL.  Only Attr, CDATA, Comment, PI and Text
        nodes have values; trying to set the value of another type of node
        is a no-op.  Note that the new value must be in the data encoding!
        It is not verified, converted, or checked.  If bad data is set for
        a node, bad things may happen-- GIGO!

    RETURNS
        (void)

    ARGUMENTS
        node (IN) node whose value to set
        data (IN) new data for node [in data encoding]

    SEE ALSO
        getNodeValue, getAttrValue, setAttrValue
*/
void setNodeValue(xmlnode *node, const oratext *data);

/*---------------------------------------------------------------------------
    NAME
        getNodeType [DOM 1]

    FUNCTION
        Returns the type code of a node

    DESCRIPTION
        Returns the type code of a node.  The type names and numeric values
        match the DOM specification (and are #defined in oraxml.h):

        ELEMENT_NODE, ATTRIBUTE_NODE, TEXT_NODE, CDATA_SECTION_NODE,
        ENTITY_REFERENCE_NODE, ENTITY_NODE, PROCESSING_INSTRUCTION_NODE,
        COMMENT_NODE, DOCUMENT_NODE, DOCUMENT_TYPE_NODE,
        DOCUMENT_FRAGMENT_NODE, NOTATION_NODE

        Additional node types (not in the DOM standard) are as follows:

        ELEMENT_DECL_NODE, ATTR_DECL_NODE, CP_ELEMENT_NODE, CP_CHOICE_NODE,
        CP_SEQUENCE_NODE, CP_PCDATA_NODE, CP_STAR_NODE, CP_PLUS_NODE,
        CP_OPT_NODE

    RETURNS
        (xmlntype) node's type code

    ARGUMENTS
        node (IN) node whose type to return

    SEE ALSO
        xmlntype
*/
xmlntype getNodeType(const xmlnode *node);

/*---------------------------------------------------------------------------
    NAME
        getParentNode [DOM 1]

    FUNCTION
        Returns a node's parent

    DESCRIPTION
        Returns a node's parent node.  All nodes types except Attr, Document,
        DocumentFragment, Entity, and Notation may have a parent (these five
        exceptions have a NULL parent).  If a node has just been created and
        has not yet added to the tree, or if it has been removed from the tree,
        its parent is also NULL.

    RETURNS
        (xmlnode *) node's parent node

    ARGUMENTS
        node (IN) node whose parent to return
*/
xmlnode *getParentNode(const xmlnode *node);

/*---------------------------------------------------------------------------
    NAME
        getChildNodes [DOM 1]

    FUNCTION
        Returns a node's children

    DESCRIPTION
        Returns a list of the node's children, or NULL if it has no
        children.  Only Element, Document, DTD, and DocumentFragment
        nodes may have children; all other types will return NULL.

        Note that an empty list may be returned if the node once had
        children, but all have been removed!  So, presense of the list
        alone does not mean the node has children.  You must check the
        size of the list with numChildNodes or use hasChildNodes first.

        The xmlnodes structure is opaque and can only be manipulated
        with functions in the NamedNodeMap interface.

    RETURNS
        (xmlnodes *) list of node's children or NULL

    ARGUMENTS
        node  (IN) node whose children to return

    SEE ALSO
        hasChildNodes, numChildNodes, getNamedItem, setNamedItem,
        removeNamedItem, getChildNode, getNodeMapLength
*/
xmlnodes* getChildNodes(const xmlnode *node);

/*---------------------------------------------------------------------------
    NAME
        getFirstChild [DOM 1]

    FUNCTION
        Returns the first child of a node

    DESCRIPTION
        Returns the first child of a node, or NULL if the node has no
        children.  Only Element, Document, DTD, and DocumentFragment
        nodes may have children; all other types will return NULL.

    RETURNS
        (xmlnode *) node's first child

    ARGUMENTS
        node (IN) node whose first child to return

    SEE ALSO
        getChildNodes, getLastChild
*/
xmlnode *getFirstChild(const xmlnode *node);

/*---------------------------------------------------------------------------
    NAME
        getLastChild [DOM 1]

    FUNCTION
        Returns the last child of a node

    DESCRIPTION
        Returns the last child of a node, or NULL if the node has no
        children.  Only Element, Document, DTD, and DocumentFragment
        nodes may have children; all other types will return NULL.

    RETURNS
        (xmlnode *) node's last child

    ARGUMENTS
        node  (IN) node whose last child to return

    SEE ALSO
        getChildNodes, getFirstChild
*/
xmlnode *getLastChild(const xmlnode *node);

/*---------------------------------------------------------------------------
    NAME
        getPreviousSibling [DOM 1]

    FUNCTION
        Returns the previous sibling of a node

    DESCRIPTION
        Returns the node preceding a node at the same level in the DOM tree.
        That is, for each child of a parent node, the previous sibling of
        that child is the child which came before it.  If a node is the first
        child of its parent, or has no parent, NULL is returned.

    RETURNS
        (xmlnode *) node's previous sibling

    ARGUMENTS
        node (IN) node whose previous sibling to return

    SEE ALSO
        getNextSibling
*/
xmlnode *getPreviousSibling(const xmlnode *node);

/*---------------------------------------------------------------------------
    NAME
        getNextSibling [DOM 1]

    FUNCTION
        Returns the next sibling of a node

    DESCRIPTION
        Returns the node following a node at the same level in the DOM tree.
        That is, for each child of a parent node, the next sibling of that
        child is the child which comes after it.  If a node is the last child
        of its parent, or has no parent, NULL is returned.

    RETURNS
        (xmlnode *) node's next sibling

    ARGUMENTS
        node (IN) node whose next sibling to return

    SEE ALSO
        getPreviousSibling
*/
xmlnode *getNextSibling(const xmlnode *node);

/*---------------------------------------------------------------------------
    NAME
        getAttributes [DOM 1]

    FUNCTION
        Returns the attributes of an element

    DESCRIPTION
        Returns a NamedNodeMap of attributes of an element node, or NULL if
        it has no attributes.  For other node types, NULL is returned.  Note
        that if an element once had attributes, but they have all been removed,
        an empty list will be returned.  So, presense of the list does not mean
        the element has attributes.  You must check the size of the list with
        numAttributes or use hasChildNodes first.

    RETURNS
        (xmlnodes *) element node's attributes

    ARGUMENTS
        node  (IN) element node whose attributes to return
*/
xmlnodes *getAttributes(const xmlnode *node);

/*---------------------------------------------------------------------------
    NAME
        getOwnerDocument [DOM 1]

    FUNCTION
        Returns the document containing a node

    DESCRIPTION
        Returns the Document node associated with a node.  Each node belongs
        to a single xmlctx, and each xmlctx contains only one top-level
        Document node.  That Document node is returned, even if the given
        node has not yet been added to the DOM tree.  If no Document has been
        created yet, NULL is returned.

    RETURNS
        (xmlnode *) Document associated with the node

    ARGUMENTS
        node (IN) node whose associated Document to return

    SEE ALSO
        createDocument, createDocumentNS
*/
xmlnode *getOwnerDocument(xmlnode *node);

/*---------------------------------------------------------------------------
    NAME
        insertBefore [DOM 1]

    FUNCTION
        Inserts a new child into a parent

    DESCRIPTION
        Inserts the node newChild before the existing child node refChild
        in the parent node.  If refChild is NULL, appends to parent's
        children as per appendChild; otherwise it must be a child of the
        given parent.  If newChild is a DocumentFragment, all of its children
        are inserted (in the same order) before refChild; the DocumentFragment
        node itself is not.  If newChild is already in the DOM tree, it is
        first removed from its current position.

    RETURNS
        (xmlnode *) the new node inserted

    ARGUMENTS
        ctx      (IN) XML context
        parent   (IN) parent node
        newChild (IN) new node to insert
        refChild (IN) reference node, or NULL to append

    SEE ALSO
        appendChild, replaceChild, removeChild
*/
xmlnode *insertBefore(xmlctx *ctx, xmlnode *parent,
                      xmlnode *newChild, xmlnode *refChild);

/*---------------------------------------------------------------------------
    NAME
        replaceChild [DOM 1]

    FUNCTION
        Replaces one child node with another

    DESCRIPTION
        Replaces the child node oldChild with the new node newChild in
        oldChild's parent, and returns oldChild (which is now orphaned,
        with a NULL parent).  If newChild is a DocumentFragment, all of
        its children are inserted in place of oldChild; the DocumentFragment
        node itself is not.  If newChild is already in the DOM tree, it is
        first removed from its current position.

    RETURNS
        (xmlnode *) oldChild, the node replaced

    ARGUMENTS
        ctx      (IN) XML context
        newChild (IN) new node to replace with
        oldChild (IN) old node being replaced

    SEE ALSO
        insertBefore, appendChild, removeChild
*/
xmlnode *replaceChild(xmlctx *ctx, xmlnode *newChild, xmlnode *oldChild);

/*---------------------------------------------------------------------------
    NAME
        removeChild [DOM 1]

    FUNCTION
        Removes a child node

    DESCRIPTION
        Removes a node from its parent's list of children and returns it. 
        The node is orphaned; its parent will be NULL after removal.

    RETURNS
        (xmlnode *) the node removed

    ARGUMENTS
        node (IN) node being removed

    SEE ALSO
        appendChild, insertBefore, replaceChild
*/
xmlnode *removeChild(xmlnode *node);

/*---------------------------------------------------------------------------
    NAME
        appendChild [DOM 1]

    FUNCTION
        Appends a new child to a parent

    DESCRIPTION
        Appends the node to the end of the parent's list of children and
        returns the new node.  If newChild is a DocumentFragment, all of
        its children are appended in original order; the DocumentFragment
        node itself is not.

    RETURNS
        (xmlnode *) the node appended

    ARGUMENTS
        ctx      (IN) XML context
        parent   (IN) parent node
        newChild (IN) new node to append

    SEE ALSO
        insertBefore, replaceChild, removeChild
*/
xmlnode *appendChild(xmlctx *ctx, xmlnode *parent, xmlnode *newChild);

/*---------------------------------------------------------------------------
    NAME
        hasChildNodes [DOM 1]

    FUNCTION
        Determines if a node has children

    DESCRIPTION
        Test if a node has children.  Only Element, Document, DTD, and
        DocumentFragment nodes may have children.  Note that just because
        getChildNodes returns a list does not mean the node actually has
        children, since the list may be empty.

    RETURNS
        (boolean) TRUE if there are children, FALSE if not

    ARGUMENTS
        node (IN) node to test

    SEE ALSO
        getChildNodes, numChildNodes
*/
boolean hasChildNodes(const xmlnode *node);

/*---------------------------------------------------------------------------
    NAME
        numChildNodes [DOM X]

    FUNCTION
        Returns the number of children of a node

    DESCRIPTION
        Returns the number of children in a xmlnodes list (as returned by
        getChildNodes).  Only Element, Document, DTD, and DocumentFragment
        nodes may have children, all other types return 0.  Note that just
        because getChildNodes returns a list does not mean that it contains
        any children; it may be an empty list with zero length.

    RETURNS
        (size_t) number of children

    ARGUMENTS
        nodes (IN) list of nodes from getChildNodes

    SEE ALSO
        getChildNodes, hasChildNodes
*/
size_t numChildNodes(const xmlnodes *nodes);

/*---------------------------------------------------------------------------
    NAME
        hasAttributes [DOM X]

    FUNCTION
        Determines if an element has attributes

    DESCRIPTION
        Tests whether an element node has any defined attributes.  Only
        an element node may have attributes; all other types will return
        FALSE.

    RETURNS
        (boolean) TRUE if element has attributes, FALSE if not

    ARGUMENTS
        node (IN) element node to check for attributes

    SEE ALSO
        getAttributes, numAttributes, getAttribute, getAttributeIndex
*/
boolean hasAttributes(const xmlnode *node);

/*---------------------------------------------------------------------------
    NAME
        numAttributes [DOM X]

    FUNCTION
        Returns the number of attributes of an element

    DESCRIPTION
        Returns the number of attributes in an xmlnodes list.  Note that
        just because a list is returned by getAttributes does not necessarily
        mean it contains any attributes; it may be an empty list with zero
        length.

    RETURNS
        (size_t) number of attributes in list

    ARGUMENTS
        attrs (IN) list of attributes from getAttributes

    SEE ALSO
        getAttributes, hasAttributes, getAttribute, getAttributeIndex
*/
size_t numAttributes(const xmlnodes *attrs);

/*---------------------------------------------------------------------------
    NAME
        cloneNode [DOM 1]

    FUNCTION
        Creates a duplicate of a node

    DESCRIPTION
        Creates and returns a duplicate of a node.  The duplicate node has
        no parent.  Cloning an Element copies all attributes and their values,
        including those generated by the XML processor to represent defaulted
        attributes, but it does not copy any text it contains unless it is a
        deep clone, since the text is contained in a child Text node.  Cloning
        any other type of node simply returns a copy of the node.  Note that a
        clone of an unspecified Attr node is specified.  If deep is TRUE, all
        children of the node are recursively cloned, and the cloned node will
        have cloned children; a non-deep clone will have no children.

    RETURNS
        (xmlnode *) new, cloned node

    ARGUMENTS
        ctx  (IN) XML context
        old  (IN) node to clone
        deep (IN) recursively clone children?

    SEE ALSO
        importNode
*/
xmlnode *cloneNode(xmlctx *ctx, const xmlnode *old, boolean deep);

/*---------------------------------------------------------------------------
    NAME
        getDocOrder [DOM X]

    FUNCTION
        Returns the document order of a node

    DESCRIPTION
        Returns the document order cardinal for a node.  setDocOrder must
        have been called first or all nodes will have a 0 order!  Used
        primarily by the XSLT processor, which requires document order for
        node comparisons.  Nodes are numbered by the order in which they
        would be encountered in a preorder traversal of the tree.  The
        root node's order is specified to setDocOrder, but regardless of
        the absolute numbering, a node which comes first in the document
        will have a lower order than a node which comes later.

    RETURNS
        (ub4) document order cardinal for node

    ARGUMENTS
        node (IN) node whose doc order to return

    SEE ALSO
        setDocOrder
*/
ub4 getDocOrder(xmlnode *node);

/*---------------------------------------------------------------------------
    NAME
        prefixToURI [DOM X]

    FUNCTION
        Maps a namespace prefix to a URI

    DESCRIPTION
        Given a namespace prefix and a node, returns the namespace URI
        mapped to that prefix.  If the given node doesn't have a matching
        prefix, its parent is tried, then its parent, and so on, all the
        way to the root node.  If the prefix is undefined, NULL is returned.

    RETURNS
        (oratext *) namespace URI for prefix

    ARGUMENTS
        node   (IN) node whose prefix to return
        prefix (IN) prefix to match

    SEE ALSO
        getNodePrefix, getNodeNamespace, getAttrPrefix, getAttrNamespace
*/
oratext *prefixToURI(xmlnode *node, oratext *prefix);

/*---------------------------------------------------------------------------
    NAME
        freeNode [DOM X]

    FUNCTION
        Frees memory allocated by a node

    DESCRIPTION
        Frees a node.  That is, frees all the space allocated by a single
        node.  Does NOT recurse to children.  The node should already have
        been removed from the document.

    RETURNS
        (void)

    ARGUMENTS
        node (IN) node to be freed
*/
void freeNode(xmlnode *node);

/****************************************************************************
                            Interface Attr
 ****************************************************************************/

/*---------------------------------------------------------------------------
    NAME
        getAttrName [DOM 1 Attr.getName]

    FUNCTION
        Returns an attribute's name

    DESCRIPTION
        Returns the fully-qualified name of an attribute (in the data
        encoding), for example "bar" or "foo:bar".  These functions are
        synonyms and return the same thing.

    RETURNS
        (oratext *) attribute's QName [in data encoding]

    ARGUMENTS
        attr (IN) attribute node whose name to return

    SEE ALSO
        getAttrQualifiedName, getAttrNamespace, getAttrPrefix, getAttrLocal
*/
const oratext *getAttrName(const xmlnode *attr);

/*---------------------------------------------------------------------------
    NAME
        getAttrQualifiedName [DOM X]

    FUNCTION
        Returns an attribute's name

    DESCRIPTION
        Returns the fully-qualified name of an attribute (in the data
        encoding), for example "bar" or "foo:bar".  These functions are
        synonyms and return the same thing.

    RETURNS
        (oratext *) attribute's QName [in data encoding]

    ARGUMENTS
        attr (IN) attribute node whose name to return

    SEE ALSO
        getAttrName, getAttrNamespace, getAttrPrefix, getAttrLocal
*/
const oratext *getAttrQualifiedName(const xmlnode *attr);

/*---------------------------------------------------------------------------
    NAME
        getAttrNamespace [DOM X]

    FUNCTION
        Returns an attribute's namespace URI

    DESCRIPTION
        Returns an attribute's namespace URI (in the data encoding).  If
        the attribute's name is not qualified (does not contain a namespace
        prefix), it will have the default namespace in effect when the node
        was created (which may be NULL).

    EXAMPLE
        <foo xmlns:pfx="example_namespace" pfx:bar="bar_value"/>
        getAttrNamespace(attr pfx:bar) --> "example_namespace"

    RETURNS
        (oratext *) attribute's namespace URI [in data encoding] or NULL

    ARGUMENTS
        attr (IN) attribute whose namespace URI to return

    SEE ALSO
        getAttrQualifiedName, getAttrPrefix, getAttrLocal
*/
const oratext *getAttrNamespace(const xmlnode *attr);

/*---------------------------------------------------------------------------
    NAME
        getAttrPrefix [DOM X]

    FUNCTION
        Returns an attribute's namespace prefix

    DESCRIPTION
        Returns an attribute's namespace prefix (in the data encoding).  If
        the attribute's name is not fully qualified (has no prefix), NULL
        is returned.

    EXAMPLE
        <foo xmlns:pfx="example_namespace" pfx:bar="bar_value"/>
        getAttrPrefix(attr pfx:bar) --> "pfx"

    RETURNS
        (oratext *) attribute's namespace prefix [in data encoding] or NULL

    ARGUMENTS
        attr (IN) attribute whose namespace prefix to return

    SEE ALSO
        getAttrQualifiedName, getAttrNamespace, getAttrLocal
*/
const oratext *getAttrPrefix(const xmlnode *attr);

/*---------------------------------------------------------------------------
    NAME
        getAttrLocal [DOM X]

    FUNCTION
        Returns an attribute's namespace local name

    DESCRIPTION
        Returns an attribute's namespace local name (in the data encoding).
        If the attribute's name is not fully qualified (has no prefix), then
        the local name is the same as the name.

    EXAMPLE
        <foo xmlns:pfx="example_namespace" pfx:bar="bar_value"/>
        getAttrLocal(attr pfx:bar) --> "bar"

    RETURNS
        (oratext *) string of local name [in data encoding]

    ARGUMENTS
        attr (IN) attribute whose local name to return
*/
const oratext *getAttrLocal(const xmlnode *attr);

/*---------------------------------------------------------------------------
    NAME
        getAttrSpecified [DOM 1 Attr.getSpecified]

    FUNCTION
        Returns an attribute's specified flag

    DESCRIPTION
        Return the 'specified' flag for an attribute.  If the attribute was
        explicitly given a value in the original document, this is TRUE;
        otherwise, it is FALSE.  If the node is not an attribute, returns
        FALSE.  If the user sets an attribute's value through DOM, its
        specified flag will be TRUE.  To return an attribute to its default
        value (if it has one), the attribute should be deleted-- it will
        then be re-created automatically with the default value (and specified
        will be FALSE).

    RETURNS
        (boolean) value of attribute's specified flag

    ARGUMENTS
        attr (IN) attribute node whose specified flag to return

    SEE ALSO
        setAttrValue
*/
boolean getAttrSpecified(const xmlnode *attr);

/*---------------------------------------------------------------------------
    NAME
        setAttrValue [DOM 1 Attr.value attribute]

    FUNCTION
        Sets an attribute's value

    DESCRIPTION
        Sets the given attribute's value to data.  If the node is not an
        attribute, does nothing.  Note that the new value must be in the
        data encoding!  It is not verified, converted, or checked.  If bad
        data is set for the attribute, bad things may happen-- GIGO!  The
        attribute's specified flag will be TRUE after setting a new value.

    RETURNS
        (void)

    ARGUMENTS
        attr (IN) pointer to attribute node
        data (IN) new value for attribute [in data encoding]

    SEE ALSO
        getAttrSpecified, getAttrValue
*/
void setAttrValue(xmlnode *attr, const oratext *data);

/*---------------------------------------------------------------------------
    NAME
        getAttrValue [DOM 1 Attr.value attribute]

    FUNCTION
        Returns an attribute's value

    DESCRIPTION
        Returns the "value" (character data) of an attribute (in the data
        encoding).  Character and general entities will have been replaced.

    RETURNS
        (oratext *) value of attribute [in data encoding]

    ARGUMENTS
        attr (IN) attribute whose value to return

    SEE ALSO
        setAttrValue
*/
const oratext *getAttrValue(const xmlnode *attr);

/****************************************************************************
                            Interface Element
 ****************************************************************************/

/*---------------------------------------------------------------------------
    NAME
        nodeValid [DOM X]

    FUNCTION
        Determines if a node is valid again a DTD

    DESCRIPTION
        Validates an element node (and its children) against the current DTD.
        If the node is not an Element, returns XMLERR_NODE_TYPE and does
        nothing.  Only works on a DOM tree created by parsing with validation
        enabled; without initial validation, just returns success (no-op).

    RETURNS
        (uword) error code, XMLERR_OK [0] on success

    ARGUMENTS
        ctx  (IN) XML context
        node (IN) root node to validate

    SEE ALSO
        xmlparse
*/
uword nodeValid(xmlctx *ctx, xmlnode *node);

/*---------------------------------------------------------------------------
    NAME
        getTagName [DOM 1]

    FUNCTION
        Returns the tagName of a node

    DESCRIPTION
        Returns the "tagName" of a node, which is the same as its name for
        now.  The DOM says "...even though there is a generic nodeName
        attribute on the Node interface, there is still a tagName attribute
        on the Element interface; these two attributes must contain the same
        value, but the Working Group considers it worthwhile to support both,
        given the different constituencies the DOM API must satisfy."

    RETURNS
        (oratext *) node's tagname [in data encoding]

    ARGUMENTS
        node (IN) node whose tagname to return

    SEE ALSO
        getNodeName
*/
const oratext *getTagName(const xmlnode *node);

/*---------------------------------------------------------------------------
    NAME
        getAttribute [DOM 1]

    FUNCTION
        Returns an attribute's value given its name

    DESCRIPTION
        Returns the value of an element's attribute (specified by name).
        Note that an attribute may have the empty string as its value,
        but cannot be NULL.  If the element does not have an attribute
        with the given name, NULL is returned.

    RETURNS
        (oratext *) attribute's value [in data encoding] or NULL

    ARGUMENTS
        node (IN) element whose attributes to check
        name (IN) name of attribute

    SEE ALSO
        getAttributes, getAttributeIndex, getAttributeNode
*/
const oratext *getAttribute(const xmlnode *node, const oratext *name);

/*---------------------------------------------------------------------------
    NAME
        getAttributeIndex [DOM X]

    FUNCTION
        Returns an attribute by index

    DESCRIPTION
        Given an xmlnodes list of attributes (from getAttributes), returns
        an attribute specified by index (position).  The first attribute is
        index 0, the second is index 1, etc.  If specified attribute does
        not exist (index is too large), returns NULL.

    RETURNS
        (xmlnode *) specified attribute or NULL

    ARGUMENTS
        attrs (IN) list of attributes from getAttributes
        index (IN) index of attribute to return (zero-based)

    SEE ALSO
        getAttributes, numAttributes, getAttribute
*/
xmlnode *getAttributeIndex(const xmlnodes *attrs, size_t index);

/*---------------------------------------------------------------------------
    NAME
        setAttribute [DOM 1]

    FUNCTION
        Creates an attribute for an element

    DESCRIPTION
        Creates a new attribute for an element with the given name and value
        (which should be in the data encoding).  If the named attribute already
        exists, its value is simply replaced.  The name and value are not
        verified, converted, or checked.  If bad data is set, bad things may
        happen-- GIGO!  The value is not parsed, so entity references will not
        be expanded.  The attribute's Specified flag will be set.

    RETURNS
        (xmlnode *) pointer to (new or old) attribute node, or NULL

    ARGUMENTS
        ctx   (IN) XML context
        elem  (IN) element node
        name  (IN) new attribute's name [in data encoding]
        value (IN) new attribute's value [in data encoding]

    SEE ALSO
        createAttribute, setAttrValue, removeAttribute
*/
xmlnode *setAttribute(xmlctx *ctx, xmlnode *elem,
                      const oratext *name, const oratext *value);

/*---------------------------------------------------------------------------
    NAME
        setAttributeNode [DOM 1]

    FUNCTION
        Adds an attribute node to an element

    DESCRIPTION
        Adds a new attribute to an element.  If an attribute with newNode's
        name already exists, it is replaced and the old attribute is returned
        through oldNode.  If the attribute has a new name, it is added to the
        element's list and oldNode is set to NULL.

    RETURNS
        (boolean) success

    ARGUMENTS
        ctx     (IN)  XML context
        elem    (IN)  element node
        newNode (IN)  new attribute node
        oldNode (OUT) replaced attribute or NULL

    SEE ALSO
        createAttribute, setAttrValue
*/
boolean setAttributeNode(xmlctx *ctx, xmlnode *elem,
                         xmlnode *newNode, xmlnode **oldNode);

/*---------------------------------------------------------------------------
    NAME
        removeAttribute [DOM 1]

    FUNCTION
        Removes an attribute from an element (by name)

    DESCRIPTION
        Removes an attribute (specified by name).  If the removed attribute
        has a default value it is immediately re-created with that default.
        Note that the attribute is removed from the element's list of
        attributes, but the Attr node itself is not destroyed.

    RETURNS
        (void)

    ARGUMENTS
        elem (IN) element node
        name (IN) name of attribute to remove [in data encoding]

    SEE ALSO
        removeAttributeNode
*/
void removeAttribute(xmlnode *elem, const oratext *name);

/*---------------------------------------------------------------------------
    NAME
        getAttributeNode [DOM 1]

    FUNCTION
        Returns the named attribute of an element

    DESCRIPTION
        Returns an element's attribute specified by name.  If the node is
        not an element or the named attribute does not exist, returns NULL.

    RETURNS
        (xmlnode *) named attribute or NULL

    ARGUMENTS
        elem (IN) element node
        name (IN) name of attribute to get [in data encoding]

    SEE ALSO
        getAttribute, getAttributeIndex
*/
xmlnode *getAttributeNode(const xmlnode *elem, const oratext *name);

/*---------------------------------------------------------------------------
    NAME
        removeAttributeNode [DOM 1]

    FUNCTION
        Removes an attribute from an element (by pointer)

    DESCRIPTION
        Removes an attribute from an element.  If the attribute has a
        default value, it is immediately re-created with that value
        (Specified set to FALSE).  Returns the removed attribute on
        success, else NULL.

    RETURNS
        (xmlnode *) pointer to old attribute, or NULL on error

    ARGUMENTS
        elem (IN) element node
        attr (IN) removed attribute node or NULL

    SEE ALSO
        removeAttribute
*/
xmlnode *removeAttributeNode(xmlnode *elem, xmlnode *attr);

/*---------------------------------------------------------------------------
    NAME
        normalize [DOM 1]

    FUNCTION
        Merges adjacent text nodes in an element

    DESCRIPTION
        "Normalizes" the subtree rooted at an element, i.e. merges adjacent
        Text nodes children of elements.  Note that adjacent Text nodes will
        never be created during a normal parse, only after manipulation of
        the document with DOM calls.

    EXAMPLE
        <a>"XXX" "YYY"<b>"111" "222"</b></a>
        normalize(node a) -->
        <a>"XXXYYY"<b>"111222"</b></a>

    RETURNS
        (void)

    ARGUMENTS
        ctx  (IN) XML context
        elem (IN) root of subtree to normalize
*/
void normalize(xmlctx *ctx, xmlnode *elem);

/****************************************************************************
                            Interface DOMImplementation
 ****************************************************************************/

/*---------------------------------------------------------------------------
    NAME
        createDocument [DOM X]

    FUNCTION
        Creates a new Document node (non-namespace)

    DESCRIPTION
        Creates the top-level Document node in an xmlctx.  Each context can
        only contain a single document, rooted in a Document node.  If the
        context already contains a document, it is returned.  Otherwise a
        new Document node is created and set in the context.  This is a
        non-namespace-aware function; the Document will have NULL namespace
        URI and prefix and have the name "#document".  If a DTD (as created
        by createDocumentType) is specified (not NULL), it is added as a
        child of the document.  The DTD must not already be a child of
        another document!

    RETURNS
        (xmlnode *) existing or created Document node

    ARGUMENTS
        ctx   (IN) XML context

    SEE ALSO
        createDocumentNS, createDocumentType
*/
xmlnode* createDocument(xmlctx *ctx);

/*---------------------------------------------------------------------------
    NAME
        createDocumentNS [DOM 2 createDocument]

    FUNCTION
        Creates a new Document node (namespace-aware)

    DESCRIPTION
        Creates the top-level Document node in an xmlctx.  Each context can
        only contain a single document, rooted in a Document node.  If the
        context already contains a document, it is returned.  Otherwise a
        new Document node is created and set in the context.  This is a
        namespace-aware function; the Document's namespace URI and QName
        should be specified (but may be NULL).  If no name is provided, the
        document will be named "#document".  If a DTD (as created by
        createDocumentType) is specified (not NULL), it is added as a child
        of the document.  The DTD must not already be a child of another
        document!

    RETURNS
        (xmlnode *) existing or created Document node

    ARGUMENTS
        ctx   (IN) XML context
        imp   (IN) DOMImplementation structure
        uri   (IN) document's namespace URI [in data encoding] or NULL
        qname (IN) document's qualified name [in data encoding] or NULL
        dtd   (IN) document type (DTD)

    SEE ALSO
        createDocument, createDocumentType
*/
xmlnode* createDocumentNS(xmldomimp *imp, oratext *uri,
                          oratext *qname, xmlnode *dtd);

/*---------------------------------------------------------------------------
    NAME
        createDocumentType [DOM 2]

    FUNCTION
        Creates a new DTD node

    DESCRIPTION
        Creates and returns a DOCUMENT_TYPE (DTD) node.  Currently there is
        no way to populate the DTD; this function is provided in anticipation
        of future DOM versions.  The DTD may be associated with a new Document
        by createDocument or createDocumentNS.

    RETURNS
        (xmlnode *) created DTD node

    ARGUMENTS
        imp   (IN) DOMImplementation structure
        qname (IN) DTD's qualified name [in data encoding]
        pubid (IN) DTD's public identifier [in data encoding]
        sysid (IN) DTD's system identifier [in data encoding]

    SEE ALSO
        createDocument, createDocumentNS
*/
xmlnode* createDocumentType(xmldomimp *imp, oratext *qname,
                            oratext *pubid, oratext *sysid);

/*---------------------------------------------------------------------------
    NAME
        hasFeature [DOM 1]

    FUNCTION
        Tests if the DOM implementation supports a feature

    DESCRIPTION
        Tests if the DOM implementation supports a named feature with a
        certain version.  Note that version can only be "1.0" or "2.0"
        at present.  If omitted, supporting any version of the feature
        returns TRUE.

    DOM 1.0 FEATURES
        "XML"            XML module
        "HTML"           HTML module

    DOM 2.0 FEATURES
        "Core"           Core module
        "XML"            XML module
        "HTML"           HTML module
        "Views"          Views module
        "StyleSheets"    Style Sheets module
        "CSS"            CSS module
        "CSS2"           CSS2 module
        "Events"         Events module
        "UIEvents"       User interface Events module
        "MouseEvents"    Mouse Events module
        "MutationEvents" Mutation Events module
        "HTMLEvents"     HTML Events module
        "Range"          Range module
        "Traversal"      Traversal module

    RETURNS
        (boolean) Feature is implemented in the specified version

    ARGUMENTS
        imp     (IN) XML DOMImplementation structure
        feature (IN) case-insensitive name of the feature [in data encoding]
        version (IN) version of feature [in data encoding]
*/
boolean hasFeature(xmldomimp *imp, const oratext *feature, 
                   const oratext *version);

/****************************************************************************
                            Interface Document
 ****************************************************************************/

/*---------------------------------------------------------------------------
    NAME
        getDocType [DOM 1 Document.DocumentType attribute]

    FUNCTION
        Returns the current document's DTD

    DESCRIPTION
        Returns the DTD node associated with current document.  The DTD
        cannot be edited, but its children may be retrieved with getChildNodes
        as for other node types.

    RETURNS
        (xmlnode *) DTD node

    ARGUMENTS
        ctx (IN) XML context

    SEE ALSO
        createDocumentType, createDocument, createDocumentNS, getDocTypeName,
        getDocTypeEntities, getDocTypeNotations
*/
xmlnode* getDocType(xmlctx *ctx);

/*---------------------------------------------------------------------------
    NAME
        getImplementation [DOM 1]

    FUNCTION
        Returns a DOM implementation structure

    DESCRIPTION
        Returns the DOM-implementation-specific descriptor.  Used to probe
        the implementation with hasFeature, or to create new Document or
        DTDs.

    RETURNS
        (xmldomimp *) DOM implementation

    ARGUMENTS
        ctx (IN) XML context

    SEE ALSO
        createDocument, createDocumentNS, createDocumentType, hasFeature
*/
xmldomimp* getImplementation(xmlctx *ctx);

/*---------------------------------------------------------------------------
    NAME
        getDocument [DOM X]

    FUNCTION
        Returns the Document node

    DESCRIPTION
        Returns the root node of the DOM tree, which is a Document node, or
        NULL if there is none.  The Document node is created automatically
        when a document is parsed successfully, or manually by createDocument
        or createDocumentNS.

    RETURNS
        (xmlnode *) Document node or NULL

    ARGUMENTS
        ctx (IN) XML context

    SEE ALSO
        createDocument, createDocumentNS
*/
xmlnode* getDocument(xmlctx *ctx);

/*---------------------------------------------------------------------------
    NAME
        getDocumentElement [DOM 1]

    FUNCTION
        Returns the root element node

    DESCRIPTION
        Returns the root element (node) of the DOM tree, or NULL if there
        is none.  Each document has only one uppermost Element node, called
        the root element.  It is created after a document is parsed
        successfully, or manually by createElement then appendChild, etc.

    RETURNS
        (xmlnode *) root element node or NULL

    ARGUMENTS
        ctx (IN) XML context

    SEE ALSO
        createElement
*/
xmlnode* getDocumentElement(xmlctx *ctx);

/*---------------------------------------------------------------------------
    NAME
        createElement [DOM 1]

    FUNCTION
        Creates an element node (non-namespace)

    DESCRIPTION
        Creates an element node with the given tag name (which should be in
        the data encoding).  Note that the tag name of an element is CASE
        SENSITIVE.  This is the non-namespace aware function: the new node
        will have NULL namespace URI and prefix, and its local name will be the
        same as its tag name, even if the tag name specified is a QName.  The
        new node is an orphan with no parent; it must be added to the DOM
        tree with appendChild etc.

    RETURNS
        (xmlnode *) new element node

    ARGUMENTS
        ctx     (IN) XML context
        tagName (IN) new element's name [in data encoding]

    SEE ALSO
        createElementNS
*/
xmlnode *createElement(xmlctx *ctx, const oratext *tagName);

/*---------------------------------------------------------------------------
    NAME
        createElementNS [DOM 2]

    FUNCTION
        Creates an element node (namespace-aware)

    DESCRIPTION
        Creates an element with the given namespace URI and QName.  Note
        that element names are CASE SENSITIVE, and the QName is required
        though the URI may be NULL.  The QName will be split into prefix and
        local parts, retrievable with getNodePrefix, getNodeLocal, etc;
        the tagName will be the full QName.  The new node is an orphan with
        no parent; it must be added to the DOM tree with appendChild etc.

    RETURNS
        (xmlnode *) new element node

    ARGUMENTS
        ctx   (IN) XML context
        uri   (IN) new element's namespace URI [in data encoding]
        qname (IN) new element's QName [in data encoding]

    SEE ALSO
        createElement
*/
xmlnode *createElementNS(xmlctx *ctx, const oratext *uri, const oratext*qname);

/*---------------------------------------------------------------------------
    NAME
        createDocumentFragment [DOM 1]

    FUNCTION
        Creates an empty document fragment node

    DESCRIPTION
        Creates an empty Document Fragment node.  A document fragment is
        treated specially when it is inserted into a DOM tree: the children
        of the fragment are inserted in order instead of the fragment node
        itself.  After insertion, the fragment node will still exist, but
        have no children.  See insertBefore, replaceChild, appendChild, etc.
        The name of a fragment node is always "#document-fragment".

    RETURNS
        (xmlnode *) new document fragment node

    ARGUMENTS
        ctx (IN) XML context

    SEE ALSO
        insertBefore, replaceChild, appendChild
*/
xmlnode *createDocumentFragment(xmlctx *ctx);

/*---------------------------------------------------------------------------
    NAME
        createTextNode [DOM 1]

    FUNCTION
        Creates a text node

    DESCRIPTION
        Creates a Text node with the given initial data (which must be
        non-NULL and in the data encoding).  The data may be NULL; if
        provided, it is not verified, converted, checked, or parsed
        (entities will not be expanded).  If bad data is set, bad things
        may happen-- GIGO!  The name of a fragment node is always "#text".
        New data for a Text node can be set with setNodeValue; see the
        CharacterData interface for editing methods.  The new node is
        an orphan with no parent; it must be added to the DOM tree with
        appendChild etc.

    RETURNS
        (xmlnode *) new Text node

    ARGUMENTS
        ctx  (IN) XML context
        data (IN) initial data [in data encoding] or NULL

    SEE ALSO
        setNodeValue, getNodeValue, setCharData, getCharData, getCharLength,
        substringData, appendData, insertData, deleteData, replaceData
*/
xmlnode *createTextNode(xmlctx *ctx, const oratext *data);

/*---------------------------------------------------------------------------
    NAME
        createComment [DOM 1]

    FUNCTION
        Creates a comment node

    DESCRIPTION
        Creates a Comment node with the given initial data (which must be
        in the data encoding).  The data may be NULL; if provided, it is not
        verified, converted, or checked.  If bad data is set, bad things may
        happen-- GIGO!  The name of a Comment node is always "#comment".  The
        new node is an orphan with no parent; it must be added to the DOM
        tree with appendChild etc.

    RETURNS
        (xmlnode *) new Comment node

    ARGUMENTS
        ctx  (IN) XML context
        data (IN) initial data [in data encoding] or NULL
*/
xmlnode *createComment(xmlctx *ctx, const oratext *data);

/*---------------------------------------------------------------------------
    NAME
        createCDATASection [DOM 1]

    FUNCTION
        Creates a CDATA node

    DESCRIPTION
        Creates a CDATASection node with the given initial data (which
        should be in the data encoding).  A CDATA section is considered
        verbatim and is never parsed; it will not be joined with adjacent
        Text nodes by the normalize operation.  The initial data may be
        NULL; if provided, it is not verified, converted, or checked.  If
        bad data is set, bad things may happen-- GIGO!  The name of a
        CDATA node is always "#cdata-section".  The new node is an orphan
        with no parent; it must be added to the DOM tree with appendChild etc.

    RETURNS
        (xmlnode *) new CDATA node

    ARGUMENTS
        ctx  (IN) XML context
        data (IN) initial data [in data encoding]

    SEE ALSO
        createTextNode
*/
xmlnode *createCDATASection(xmlctx *ctx, const oratext *data);

/*---------------------------------------------------------------------------
    NAME
        createProcessingInstruction [DOM 1]

    FUNCTION
        Creates a PI node

    DESCRIPTION
        Creates a Processing Instruction node with the given target and
        data (which should be in the data encoding).  The data may be NULL
        initially, and may be changed later (with setPIData), but the
        target is required and cannot be changed.  Note the target and data
        are not verified, converted, or checked.  If bad data is set, bad
        things may happen-- GIGO!  The name of a PI node is the same as the
        target.  The new node is an orphan with no parent; it must be added
        to the DOM tree with appendChild etc.

    RETURNS
        (xmlnode *) new PI node

    ARGUMENTS
        ctx    (IN) XML context
        target (IN) PI's target [in data encoding]
        data   (IN) PI's data [in data encoding] or NULL

    SEE ALSO
        getPITarget, getPIData, setPIData
*/
xmlnode *createProcessingInstruction(xmlctx *ctx, const oratext *target,
                                     const oratext *data);

/*---------------------------------------------------------------------------
    NAME
        createAttribute [DOM 1]

    FUNCTION
        Creates an attribute node (non-namespace)

    DESCRIPTION
        Creates an attribute node with the given name and value (in the
        data encoding).  Note this function differs from the DOM
        specification, which does not allow the initial value of the
        attribute to be set (see setAttrValue).  The name is required,
        but the value may be NULL; neither is verified, converted, or
        checked.  If bad data is set, bad things may happen-- GIGO!
        This is the non-namespace aware function (see createAttributeNS):
        the new attribute will have NULL namespace URI and prefix, and
        its local name will be the same as its name, even if the name
        specified is a QName.  See the Attr interface for more attribute
        manipulation methods.  If given an initial value, the attribute's
        specified flag will be TRUE.  The new node is an orphan with no
        parent; it must be added to the DOM tree with appendChild etc.
        See setAttribute which creates and adds an attribute in a single
        operation.

    RETURNS
        (xmlnode *) new attribute node

    ARGUMENTS
        ctx   (IN) XML context
        name  (IN) attribute's name [in data encoding]
        value (IN) attribute's value [in data encoding] or NULL

    SEE ALSO
        setAttrValue, createAttributeNS, setAttribute
*/
xmlnode *createAttribute(xmlctx *ctx, const oratext *name,
                         const oratext *value);

/*---------------------------------------------------------------------------
    NAME
        createAttributeNS [DOM 2]

    FUNCTION
        Creates an attribute node (namespace-aware)

    DESCRIPTION
        Creates an attribute node with the given namespace URI and QName;
        this is the namespace-aware version of createAttribute.  Note this
        function differs from the DOM specification, which does not allow
        the initial value of the attribute to be set (see setAttrValue).
        The name is required, but the value may be NULL; neither is verified,
        converted, or checked.  If bad data is set, bad things may happen--
        GIGO!   If given an initial value, the attribute's specified flag
        will be TRUE.  The new node is an orphan with no parent; it must
        be added to the DOM tree with appendChild etc.  See setAttribute
        which creates and adds an attribute in a single operation.

    RETURNS
        (xmlnode *) new attribute node

    ARGUMENTS
        ctx   (IN) XML context
        uri   (IN) attribute's namespace URI [in data encoding] or NULL
        qname (IN) attribute's qualified name [in data encoding]
        value (IN) attribute's initial value [in data encoding] or NULL

    SEE ALSO
        setAttrValue, createAttribute, setAttribute
*/
xmlnode *createAttributeNS(xmlctx *ctx, const oratext *uri,
                           const oratext *qname, const oratext *value);

/*---------------------------------------------------------------------------
    NAME
        createEntityReference [DOM 1]

    FUNCTION
        Creates an entity reference node

    DESCRIPTION
        Creates an Entity Reference node; the name (which should be in the
        data encoding) is the name of the entity to be referenced.  The named
        entity does not have to exist.  The name is not verified, converted,
        or checked.  If bad data is set, bad things may happen-- GIGO!   
        Entity Reference nodes are never generated by the parser; instead,
        entity references are expanded as encountered.  On output, an entity
        reference node will turn into a "&name;" style reference.  The new
        node is an orphan with no parent; it must be added to the DOM tree
        with appendChild etc.

    RETURNS
        (xmlnode *) new entity reference node

    ARGUMENTS
        ctx  (IN) XML context
        name (IN) name of referenced entity [in data encoding]
*/
xmlnode *createEntityReference(xmlctx *ctx, const oratext *name);

/*---------------------------------------------------------------------------
    NAME
        getElementsByTagName [DOM 1]

    FUNCTION
        Returns the elements matching a name (non-namespace)

    DESCRIPTION
        Returns a list of all elements (in the document tree rooted at the
        root node) with a given tag name, in the order in which they would
        be encountered in a preorder traversal of the tree.  If root is NULL,
        the entire document is searched.  The tag name should be in the data
        encoding.  The special name "*" matches all tag names; a NULL name
        matches nothing.  Note that tag names are CASE SENSITIVE.  This
        function is not namespace aware; the full tag names are compared.
        If two prefixes which map to the same URI are compared, the
        comparison will fail.  See getElementsByTagNameNS for the namespace-
        aware version.  The returned list can be freed with freeElements.

    RETURNS
        (xmlnodes *) list of matching nodes

    ARGUMENTS
        ctx     (IN) XML context
        root    (IN) root of search tree
        tagName (IN) tagName to match or "*" [in data encoding]

    SEE ALSO
        getElementsByTagNameNS, freeElements
*/
xmlnodes *getElementsByTagName(xmlctx *ctx, xmlnode *node,
                               const oratext *tagName);

/*---------------------------------------------------------------------------
    NAME
        getElementsByTagNameNS [DOM 2]

    FUNCTION
        Returns the elements matching a namespace URI and local

    DESCRIPTION
        Returns a list of all elements (in the document tree rooted at the
        given node) with a given namespace URI and local name, in the order
        in which they would be encountered in a preorder traversal of the
        tree.  If root is NULL, the entire document is searched.  The URI
        and local name should be in the data encoding.  The special local
        name "*" matches all local names; a NULL local name matches nothing.
        Namespace URIs must always match, however.  Note that comparisons
        are CASE SENSITIVE.  See getElementsByTagName for the non-namespace
        aware version.  The returned list can be freed with freeElements.

    RETURNS
        (xmlnodes *) list of nodes containing all the matched elements

    ARGUMENTS
        ctx   (IN) XML context
        root  (IN) root node
        uri   (IN) namespace URI [in data encoding]
        local (IN) namespace local name or "*" [in data encoding]

    SEE ALSO
        getElementsByTagName, freeElements
*/
xmlnodes *getElementsByTagNameNS(xmlctx *ctx, xmlnode *node,
                                 const oratext *uri, const oratext *local);

/*---------------------------------------------------------------------------
    NAME
        getElementByID [DOM 2]

    FUNCTION
        Returns the element with the given ID

    DESCRIPTION
        Returns the element node which has the given ID.  If no such ID is
        defined, returns NULL.  Note that attributes named "ID" are not
        automatically of type ID; ID attributes (which can have any name)
        must be declared as type ID in the DTD.

    RETURNS
        (xmlnode *) matching element node or NULL

    ARGUMENTS
        ctx (IN) XML context
        id  (IN) element ID [in data encoding]
*/
xmlnode *getElementByID(xmlctx *ctx, oratext *id);

/*---------------------------------------------------------------------------
    NAME
        importNode [DOM 2]

    FUNCTION
        Copies a node from one document to another

    DESCRIPTION
        Imports a node from one Document to another.  The new node is an
        orphan and has no parent; it must be added to the DOM tree with
        appendChild, etc.  The original node is not modified in any way
        or removed from its document; instead, a new node is created with
        copies of all the original node's QName, prefix, namespace URI,
        and local name.  As with cloneNode, the deep controls whether the
        children of the node are recursively imported.  If FALSE, only the
        node itself is imported, and it will have no children.  If TRUE,
        all descendents of the node will be imported as well, and an
        entire new subtree created.

        Document and DocumentType nodes cannot be imported.  Imported
        attributes will have their specified flags set to TRUE.  Elements
        will have only their specified attributes imported; non-specified
        (default) attributes are omitted.  New default attributes (for the
        destination document) are then added.

    RETURNS
        (xmlnode *) element node with given ID

    ARGUMENTS
        ctx    (IN) XML context
        import (IN) node to import
        deep   (IN) recursively import children?

    SEE ALSO
        cloneNode
*/
xmlnode *importNode(xmlctx *ctx, xmlnode *import, boolean deep);

/****************************************************************************
                            Interface NamedNodeMap
 ****************************************************************************/

/*---------------------------------------------------------------------------
    NAME
        getNamedItem [DOM 1]

    FUNCTION
        Returns an item from a node map by name

    DESCRIPTION
        Retrieves an item from a NamedNodeMap, specified by name (which
        should be in the data encoding).  This is a non-namespace-aware
        function; it just matches (case sensitively) on the whole QName.
        Note this function differs from the DOM spec in that the index
        of the matching item is also returned.  

    RETURNS
        (xmlnode *) matching node or NULL

    ARGUMENTS
        nodes (IN)  node map
        name  (IN)  name of node to retrieve [in data encoding]
        index (OUT) index of node, if found

    SEE ALSO
        setNamedItem, removeNamedItem, getChildNode
*/
xmlnode *getNamedItem(const xmlnodes *nodes, const oratext *name, 
                      size_t *index);

/*---------------------------------------------------------------------------
    NAME
        setNamedItem [DOM 1]

    FUNCTION
        Add a new item to a node map

    DESCRIPTION
        Adds a new node to a NamedNodeMap.  If a node already exists with
        the given name, replaces the old node and returns it.  If no such
        named node exists, adds the new node to the map and sets old to
        NULL.  This is a non-namespace-aware function; it just matches
        (case sensitively) on the whole QName.  Since some node types have
        fixed names (Text, Comment, etc), trying to set another of the same
        type will always cause replacement.

    RETURNS
        (boolean) success, node added or replaced

    ARGUMENTS
        ctx    (IN)  XML context
        nodes  (IN)  node map
        node   (IN)  new node to set
        old    (OUT) replaced node or NULL

    SEE ALSO
        getNamedItem, removeNamedItem
*/
boolean setNamedItem(xmlctx *ctx, xmlnodes *nodes,
                     xmlnode *node, xmlnode **old);

/*---------------------------------------------------------------------------
    NAME
        removeNamedItem [DOM 1]

    FUNCTION
        Removes an item from a node map by name

    DESCRIPTION
        Removes a node from a NamedNodeMap, specified by name.  This is a
        non-namespace-aware function; it just matches (case sensitively)
        on the whole QName.  If the removed node is an attribute with default
        value (not specified), it is immediately replaced.  The removed node
        is returned; if no removal took place, NULL is returned.

    RETURNS
        (xmlnode *) the removed node or NULL

    ARGUMENTS
        nodes (IN) node map
        name  (IN) name of node to remove [in data encoding]

    SEE ALSO
        getNamedItem, setNamedItem
*/
xmlnode *removeNamedItem(xmlnodes *nodes, const oratext *name);

/*---------------------------------------------------------------------------
    NAME
        getChildNode [DOM 1 item]

    FUNCTION
        Return an item from a node map by index

    DESCRIPTION
        Returns the indexth node in a NamedNodeMap, or NULL if there is no
        such node.  Indexes are zero-based, so the first node is index 0.

    RETURNS
        (xmlnode *) indexth node or NULL

    ARGUMENTS
        nodes (IN) node map
        index (IN) index of node to return (0-based)

    SEE ALSO
        getNamedItem
*/
xmlnode *getChildNode(const xmlnodes *nodes, size_t index);

/*---------------------------------------------------------------------------
    NAME
        getNodeMapLength [DOM 1 NamedNodeMap.length attribute]

    FUNCTION
        Returns the length of a node map

    DESCRIPTION
        Returns the number of nodes in a NamedNodeMap (the length).
        Note that nodes are referred to by index, and the range of
        valid indexes is 0 through length-1.

    RETURNS
        (size_t) number of nodes in map

    ARGUMENTS
        nodes (IN) node map

    SEE ALSO
        getChildNode
*/
size_t getNodeMapLength(const xmlnodes *nodes);

/****************************************************************************
                            Interface CharacterData
 ****************************************************************************/

/*---------------------------------------------------------------------------
    NAME
        getCharData [DOM 1 getData]

    FUNCTION
        Returns the data for a character node

    DESCRIPTION
        Returns the data for a CharacterData node (type Text, Comment or
        CDATASection) in the data encoding.  For other node types, or if
        there is no data, returns NULL.

    RETURNS
        (oratext *) node's data [in the data encoding]

    ARGUMENTS
        node (IN) node whose data to return

    SEE ALSO
        setCharData, createTextNode, createComment, createCDATASection
*/
const oratext *getCharData(const xmlnode *node);

/*---------------------------------------------------------------------------
    NAME
        setCharData [DOM 1 setData]

    FUNCTION
        Sets the data for a character node

    DESCRIPTION
        Sets data for a CharacterData node (type Text, Comment or
        CDATASection), replacing the old data.  For other node types,
        does nothing.  The new data is not verified, converted, or
        checked-- it should be in the data encoding.  If bad data is
        set, bad things may happen-- GIGO!   

    RETURNS
        (void)

    ARGUMENTS
        node (IN) node whose data to set
        data (IN) new data for node [in data encoding]

    SEE ALSO
        getCharData
*/
void setCharData(xmlnode *node, const oratext *data);

/*---------------------------------------------------------------------------
    NAME
        getCharLength [DOM 1 getLength]

    FUNCTION
        Returns the length of a character node's data

    DESCRIPTION
        Returns the length of the data for a CharacterData node (type Text,
        Comment or CDATASection).  For other node types, returns 0.  Note
        the the data is the data encoding, and the returned length is in
        characters, not bytes.

    RETURNS
        (size_t) length of data [in characters]

    ARGUMENTS
        node (IN) node whose data length to return

    SEE ALSO
        getCharData
*/
size_t getCharLength(const xmlnode *node);

/*---------------------------------------------------------------------------
    NAME
        substringData [DOM 1]

    FUNCTION
        Returns a substring of a character node's data

    DESCRIPTION
        Returns a range of character data from a CharacterData node (type
        Text, Comment or CDATASection).  For other node types, or if count
        is zero, returns NULL.  Since the data is in the data encoding,
        offset and count are in characters, not bytes.  The beginning of
        the string is offset 0.  If the sum of offset and count exceeds the
        length, then all characters to the end of the data are returned.

    EXAMPLE
        Given a node with data "abcdefgh",
        substringData(offset=0, length=1) --> "a"
        substringData(offset=5, length=5) --> "fgh"

    RETURNS
        (oratext *) specified substring or NULL

    ARGUMENTS
        ctx    (IN) XML context
        node   (IN) node whose substring to extract
        offset (IN) starting offset of substring [zero based, in characters]
        count  (IN) length of substring [in characters]

    SEE ALSO
        appendData, insertData, deleteData, replaceData, splitText
*/
const oratext *substringData(xmlctx *ctx, const xmlnode *node,
                             ub4 offset, ub4 count);

/*---------------------------------------------------------------------------
    NAME
        appendData [DOM 1]

    FUNCTION
        Appends to a character node's data

    DESCRIPTION
        Append a string to the end of a CharacterData node's data.  If the
        node is not Text, Comment or CDATASection, or if the string to append
        is NULL, does nothing.  The appended data should be in the data
        encoding.  It will not be verified, converted, or checked.  If bad
        data is appended, bad things may happen-- GIGO!   

    RETURNS
        (void)

    ARGUMENTS
        ctx  (IN) XML context
        node (IN) node whose data to append to 
        arg  (IN) string to append [in data encoding]

    SEE ALSO
        getCharData, insertData, deleteData, replaceData, splitText
*/
void appendData(xmlctx *ctx, xmlnode *node, const oratext *arg);

/*---------------------------------------------------------------------------
    NAME
        insertData [DOM 1]

    FUNCTION
        Inserts into a character node's data

    DESCRIPTION
        Insert a string into a CharacterData node's data at the specified
        position.  If the node is not Text, Comment or CDATASection, or if
        the data to be inserted is NULL, or the offset is outside the
        original data, does nothing.  The inserted data must be in the
        data encoding.  It will not be verified, converted, or checked.
        If bad data is appended, bad things may happen-- GIGO!   The offset
        is specified as characters, not bytes.  The offset is zero-based,
        so inserting at offset zero prepends the data.

    EXAMPLE
        Given a node with data "abcdefgh",
        insertData(offset=0, arg="FOO") --> "FOOabcdefgh"
        insertData(offset=5, arg="*") --> "abcde*fgh"

    RETURNS
        (void)

    ARGUMENTS
        ctx    (IN) XML context
        node   (IN) node whose data to insert string to 
        offset (IN) offset at which to insert [in characters]
        arg    (IN) string to insert [in data encoding]

    SEE ALSO
        getCharData, appendData, deleteData, replaceData, splitText
*/
void insertData(xmlctx *ctx, xmlnode *node, ub4 offset, const oratext *arg);

/*---------------------------------------------------------------------------
    NAME
        deleteData [DOM 1]

    FUNCTION
        Removes part of a character node's data

    DESCRIPTION
        Remove a range of characters from a CharacterData node's data.
        If the node is not Text, Comment or CDATASection, or if the
        offset is outside of the original data, does nothing.  The offset
        is zero-based, so offset zero refers to the start of the data.
        Both offset and count are in characters, not bytes.  If the sum
        of offset and count exceeds the data length then all characters
        from offset to the end of the data are deleted.

    EXAMPLE
        Given a node with data "abcdefgh",
        deleteData(offset=0, count=2) --> "cdefgh"
        deleteData(offset=5, count=5) --> "abcde"

    RETURNS
        (void)

    ARGUMENTS
        ctx    (IN) XML context
        node   (IN) node whose data to delete string from
        offset (IN) offset to remove from [in characters]
        count  (IN) number of characters to remove

    SEE ALSO
        getCharData, appendData, insertData, replaceData, splitText
*/
void deleteData(xmlctx *ctx, xmlnode *node, ub4 offset, ub4 count);

/*---------------------------------------------------------------------------
    NAME
        replaceData [DOM 1]

    FUNCTION
        Replaces part of a character node's data

    DESCRIPTION
        Replaces a range of characters in a CharacterData node's data with
        a new string.  If the node is not Text, Comment or CDATASection, or
        if the offset is outside of the original data, or if the replacement
        string is NULL, does nothing.  If the count is zero, acts just as
        insertData.  The offset is zero-based, so offset zero refers to the
        start of the data.  The replacement data must be in the data encoding.
        It will not be verified, converted, or checked.  If bad data is set,
        bad things may happen-- GIGO!   The offset and count are both in
        characters, not bytes.  If the sum of offset and count exceeds
        length, then all characters to the end of the data are replaced.

    EXAMPLE
        Given a node with data "abcdefgh",
        replaceData(offset=0, count=1, arg="FOO") --> "FOObcdefgh"
        replaceData(offset=2, count=2, arg="*") --> "ab*efgh"
        replaceData(offset=5, count=5, arg="*") --> "abcde*"

    RETURNS
        (void)

    ARGUMENTS
        ctx    (IN) XML context
        node   (IN) node whose data to edit
        offset (IN) offset of original substring
        count  (IN) length of substring [in characters]
        arg    (IN) string to replace with [in data encoding]

    SEE ALSO
        getCharData, appendData, insertData, deleteData, splitText
*/
void replaceData(xmlctx *ctx, xmlnode *node, ub4 offset,
                 ub4 count, const oratext *arg);

/****************************************************************************
                            Interface Text
 ****************************************************************************/

/*---------------------------------------------------------------------------
    NAME
        splitText [DOM 1]

    FUNCTION
        Splits one text node into two

    DESCRIPTION
        Splits a single Text node into two Text nodes; the original data
        is split between them.  If the given node is not type Text, or the
        offset is outside of the original data, does nothing and returns
        NULL.  The offset is zero-based, and is in characters, not bytes.
        The original node is retained, its data is just truncated.  A new
        Text node is created which contains the remainder of the original
        data, and is inserted as the next sibling of the original.  The
        new Text node is returned.

    EXAMPLE
        Given a Text node with data "abcdefgh",
        splitText(1) --> Text("a"), Text("bcdefgh")
        splitText(5) --> Text("abcde"), Text("fgh")

    RETURNS
        (xmlnode *) new Text node or NULL

    ARGUMENTS
        ctx    (IN) XML context
        old    (IN) Text node to split
        offset (IN) split point [zero based]

    SEE ALSO
        getCharData, appendData, insertData, deleteData, replaceData
*/
xmlnode *splitText(xmlctx *ctx, xmlnode *old, uword offset);

/****************************************************************************
                            Interface DocumentType
 ****************************************************************************/

/*---------------------------------------------------------------------------
    NAME
        getDocTypeName [DOM 1 DocumentType.getName]

    FUNCTION
        Returns the name of a DTD

    DESCRIPTION
        Returns a DTD's name (specified immediately after the DOCTYPE
        keyword), or NULL if the node is not DocumentType.

    RETURNS
        (oratext *) name of DTD [in data encoding] or NULL

    ARGUMENTS
        node (IN) DTD node

    SEE ALSO
        getDocType, getDocTypeEntities, getDocTypeNotations, getContentModel
*/
const oratext *getDocTypeName(xmlnode* node);

/*---------------------------------------------------------------------------
    NAME
        getDocTypeEntities [DOM 1 DocumentType.getEntities]

    FUNCTION
        Returns a DTD's general entities

    DESCRIPTION
        Returns a NamedNodeMap of general entities defined by the DTD.
        If the node is not DocumentType, or has no general entities,
        returns NULL.  The nodes in the map will be of type Entity.

    RETURNS
        (xmlnodes *) pointer to (general) entities for DTD

    ARGUMENTS
        node (IN) map of DTD's entities

    SEE ALSO
        getDocType, getDocTypeName, getDocTypeNotations, getContentModel
*/
xmlnodes *getDocTypeEntities(xmlnode* node);

/*---------------------------------------------------------------------------
    NAME
        getDocTypeNotations [DOM 1 DocumentType.getNotations]

    FUNCTION
        Returns a DTD's notations

    DESCRIPTION
        Returns NamedNodeMap of notations declared by the DTD.  If the
        node is not DocumentType or has no notations, returns NULL.
        Nodes in the map will be of type Notation.

    RETURNS
        (xmlnodes *) map of DTD's notations

    ARGUMENTS
        node (IN) pointer to DTD node (as returned by getDocType)

    SEE ALSO
        getDocType, getDocTypeName, getDocTypeEntities, getContentModel
*/
xmlnodes *getDocTypeNotations(xmlnode* node);

/*---------------------------------------------------------------------------
    NAME
        getContentModel [DOM X]

    FUNCTION
        Returns the content model of the named element

    DESCRIPTION
        Returns the root node of the content model tree for a named
        element.  If an element by the given name does not exist,
        returns NULL.  The name must be in the data encoding, and is
        case sensitive.

        Nodes in the content model subtree will be one of the following
        Content Particle (CP) types: CP_ELEMENT_NODE, CP_CHOICE_NODE,
        CP_SEQUENCE_NODE, CP_PCDATA_NODE, CP_STAR_NODE, CP_PLUS_NODE,
        or CP_OPT_NODE

    RETURNS
        (xmlnode *) pointer to root CP node

    ARGUMENTS
        node (IN) DTD node
        name (IN) element's name [in data encoding]

    SEE ALSO
        getDocType, getDocTypeName, getDocTypeEntities, getDocTypeNotations
*/
xmlnode *getContentModel(xmlnode *node, oratext *name);

/****************************************************************************
                            Interface Notation
 ****************************************************************************/

/*---------------------------------------------------------------------------
    NAME
        getNotationPubID [DOM 1 Notation.publicId attribute]

    FUNCTION
        Returns the public ID of a notation

    DESCRIPTION
        Return a notation's public identifier (in the data encoding).  If
        the node is not a Notation, or has no defined public ID, returns NULL.

    RETURNS
        (oratext *) node's public ID [in data encoding] or NULL

    ARGUMENTS
        note (IN) Notation node

    SEE ALSO
        getNotationSysID
*/
const oratext *getNotationPubID(const xmlnode *note);

/*---------------------------------------------------------------------------
    NAME
        getNotationSysID [DOM 1 Notation.systemId attribute]

    FUNCTION
        Returns the system ID of a notation

    DESCRIPTION
        Return a notation's system identifier (in the data encoding).  If
        the node is not a Notation, or has no defined system ID, returns NULL.

    RETURNS
        (oratext *) node's system ID [in data encoding] or NULL

    ARGUMENTS
        note (IN) Notation node

    SEE ALSO
        getNotationPubID
*/
const oratext *getNotationSysID(const xmlnode *note);

/****************************************************************************
                            Interface Entity
 ****************************************************************************/

/*---------------------------------------------------------------------------
    NAME
        getEntityPubID [DOM 1 Entity.getPublicId]

    FUNCTION
        Returns the public ID of an entity

    DESCRIPTION
        Returns an entity's public identifier (in the data encoding).  If
        the node is not an Entity, or has no defined public ID, returns NULL.

    RETURNS
        (oratext *) entity's public ID [in data encoding] or NULL

    ARGUMENTS
        ent (IN) Entity node

    SEE ALSO
        getEntitySysID, getEntityNotation
*/
const oratext *getEntityPubID(const xmlnode *ent);

/*---------------------------------------------------------------------------
    NAME
        getEntitySysID [DOM 1 Entity.getSystemId]

    FUNCTION
        Returns the system ID of an entity

    DESCRIPTION
        Returns an entity's system identifier (in the data encoding).  If
        the node is not an Entity, or has no defined system ID, returns NULL.

    RETURNS
        (oratext *) entity's system ID [in data encoding] or NULL

    ARGUMENTS
        ent (IN) Entity node

    SEE ALSO
        getEntityPubID, getEntityNotation
*/
const oratext *getEntitySysID(const xmlnode *ent);

/*---------------------------------------------------------------------------
    NAME
        getEntityNotation [DOM 1 Entity.getNotationName]

    FUNCTION
        Returns the notation name of an entity

    DESCRIPTION
        For unparsed entities, returns the name of its notation (in the data
        encoding).  For parsed entities and other node types, returns NULL.

    RETURNS
        (oratext *) entity's NDATA [in data encoding] or NULL

    ARGUMENTS
        ent (IN) Entity node

    SEE ALSO
        getEntityPubID, getEntitySysID
*/
const oratext *getEntityNotation(const xmlnode *ent);

/****************************************************************************
                            Interface ProcessingInstruction
 ****************************************************************************/

/*---------------------------------------------------------------------------
    NAME
        getPITarget [DOM 1 ProcessingInstruction.getTarget]

    FUNCTION
        Returns the target of a PI

    DESCRIPTION
        Returns a Processing Instruction's (PI) target string.  If the
        node is not a PI, returns NULL.  The target is the first token
        following the markup that begins the PI.  All PIs must have a
        target, though the data part is optional.

    EXAMPLE
        <?rating extra saucy?>
        getPITarget --> "rating"

    RETURNS
        (oratext *) PI's target [in data encoding]

    ARGUMENTS
        pi (IN) PI node

    SEE ALSO
        getPIData, setPIData
*/
const oratext *getPITarget(const xmlnode *pi);

/*---------------------------------------------------------------------------
    NAME
        getPIData [DOM 1 ProcessingInstruction.getData]

    FUNCTION
        Returns the data of a PI

    DESCRIPTION
        Returns the content (data) of a Processing Instruction (in the data
        encoding).  If the node is not a PI, returns NULL.  The content is
        the part from the first non-whitespace character after the target
        until the ending "?>".

    EXAMPLE
        <?rating extra saucy?>
        getPIData --> "extra saucy"

    RETURNS
        (oratext *) PI's data [in data encoding]

    ARGUMENTS
        pi (IN) PI node

    SEE ALSO
        setPIData, getPITarget
*/
const oratext *getPIData(const xmlnode *pi);

/*---------------------------------------------------------------------------
    NAME
        setPIData [DOM 1 ProcessingInstruction.setData]

    FUNCTION
        Sets the data of a PI

    DESCRIPTION
        Set's a Processing Instruction's (PI) data (content), which must be
        in the data encoding.  It is not permitted to set the data to NULL.
        If the node is not a PI, does nothing.  The new data is not verified,
        converted, or checked.  If bad data is set, bad things may happen--
        GIGO!

    RETURNS
        (void)

    ARGUMENTS
        pi   (IN) PI node
        data (IN) new content [in data encoding]

    SEE ALSO
        getPIData, getPITarget
*/
void setPIData(xmlnode *pi, const oratext *data);

/*---------------------------------------------------------------------------
                      Package XSLT - XSLT and XPath APIs

    A software module called an XSL processor is used to read XML documents
    and transform them into other XML documents with different styles.

    The C implementation of the XSL processor follows the XSL Transformations 
    standard (version 1.0, November 16, 1999) and implements the required
    behavior of an XSL processor as specified in the XSLT specification.

  ---------------------------------------------------------------------------*/

/****************************************************************************
                            Interface XSLProcessor
 ****************************************************************************/

/*---------------------------------------------------------------------------
    NAME
        xslprocess

    FUNCTION
        Process XSL

    DESCRIPTION
        This function processes XSL Stylesheet with XML document source.
        Since this function returns a DOM document fragment node, it doesn't 
        support xsl:output.  To use xsl:output, call xslsetoutputstream and
        xslprocessxml.

    RETURNS
        (uword) error code, XMLERR_OK [0] on success

    ARGUMENTS
        xmlctx (I/O) XML document context
        xsl    (IN)  XSL stylesheet context
        resctx (IN)  result document fragment context        
        result (I/O) result document fragment node
*/
uword xslprocess(xmlctx *docctx, xmlctx *xsl,
                 xmlctx *resctx, xmlnode **result);

/*---------------------------------------------------------------------------
    NAME
        xslprocessex

    FUNCTION
        Process XSL

    DESCRIPTION
        This function processes XSL Stylesheet with XML document source.
        User can specify a list of top-level parameters to be set before
        parsing begins.  Since this function returns a DOM document fragment 
        node, it doesn't support xsl:output.  To use xsl:output, call 
        xslsetoutputstream and xslprocessxml.

    RETURNS
        (uword) error code, XMLERR_OK [0] on success

    ARGUMENTS
        xmlctx  (I/O) XML document context
        xsl     (IN)  XSL stylesheet context
        resctx  (IN)  result document fragment context
        nparams (IN)  number of [params,vals] pairs to be passed
        params  (IN)  array of name of pars [must be nparams in count]
        vals    (IN)  array of value of pars [must be nparams in count]
        baseuri (IN)  string giving the base uri for style sheet
        result  (I/O) result document fragment node
*/
uword xslprocessex(xmlctx *docctx, xmlctx *xsl, xmlctx *resctx,
                   size_t nparams, oratext * params[], oratext * vals[],
                   oratext * baseuri, xmlnode **result);

/*---------------------------------------------------------------------------
    NAME
        xslinit

    FUNCTION
        Initialize XSL context

    DESCRIPTION
        This routine is used to create the XSL processing context to 
        process one or more input XML DOM tree (specified through a 
        XML context). This context holds all the variables, states 
        and global parameters required to process a given XML DOM 
        tree. Depending on the choice of the user, processing can 
        generate its output in any one of following 3 forms: SAX  
        based events, a stream output or a DOM tree. During the 
        processing of the input XSL stylesheet is translated in to 
        an internal format and stored in xslctx.  This transformation 
        is done on as is needed basis and can be used again during 
        subsequent processing if the same xslctx is used again. 
        Hence the user can create a single xslctx for a stylesheet 
        which is being used again and again and cache it to speedup
        processing of successive input XML trees which are using 
        same XSL stylesheet again and again. The caching will
        ensure the usage of already translated intermediate format 
        of the XSL stylesheet available from a prior processing,
        instead of need to generate it again.

    RETURNS
        (xslctx *) pointer to new XSL context, or NULL

    ARGUMENTS
        err    (OUT) error code, XMLERR_OK [0] on success
        xsl    (IN)  XNL context of the input stylesheet
        baseURI(IN)  base URI for include and import statements
        resvd  (IN)  reserved for future use [must be set to 0]
*/
xslctx *xslinit(uword *err, xmlctx *xsl, const oratext *baseURI, ub4 resvd);

/*---------------------------------------------------------------------------
    NAME
        xslgetbaseuri

    FUNCTION
        Get base URI associated with XSL context

    DESCRIPTION
        Get base URI associated with XSL context

    RETURNS
        (const oratext *) base URI or NULL.

    ARGUMENTS
        xslSSctx (IN) the XSL processing context to be inquired upon
*/
const oratext *xslgetbaseuri(xslctx *xslSSctx);

/*---------------------------------------------------------------------------
    NAME
        xslgetxslctx

    FUNCTION
        Get the input stylesheet DOM context with XSL context

    DESCRIPTION
        Get XML context for XSL stylesheet associated with XSL context

    RETURNS
        (xmlctx *) XML context if successful, or NULL.

    ARGUMENTS
        xslSSctx (IN) the XSL processing context to be inquired upon
*/
xmlctx *xslgetxslctx(xslctx *xslSSctx);

/*---------------------------------------------------------------------------
    NAME
        xslsettextparam

    FUNCTION
        Set a top-level XSL parameter

    DESCRIPTION
        xslsettextparam is used to create a top level parameter 
        (or variable) to be used during the subsequent processing
        of XML DOM trees. User can call it any number of times, 
        to set as many variables as he wants. A subsequent call 
        with preexisting parameter name will override the old 
        value with the new value supplied. User can use the
        xslgettextparam to inquire the value of a preexisting 
        variable. A value specified to be NULL will reset
        the variable as if it was never added.

    RETURNS
        (uword) error code, XMLERR_OK [0] on success

    ARGUMENTS
        xslSSctx   (IN) XSL processing context 
        param_name (IN) name of parameter [variable] to be set.
        param_val  (IN) parameter value the parameter is to be set to. 
*/
uword xslsettextparam(xslctx *xslSSctx, const oratext *param_name, 
                      const oratext *param_value);

/*---------------------------------------------------------------------------
    NAME
        xslgettextparam

    FUNCTION
        Enquire about the preexisting top level parameter or value

    DESCRIPTION
        Given the parameter pull up the current value it is set to.

    RETURNS
        (const oratext *) String giving parameter value, or NULL

    ARGUMENTS
        xslSSctx   (IN) XSL processing context to be inquired upon
        param_name (IN) parameter whose value is to be inquired.
*/
const oratext *xslgettextparam(xslctx *xslSSctx, const oratext *param_name);

/*---------------------------------------------------------------------------
    NAME
       xslresetallparams

    FUNCTION
       Reset all the top level XSL parameters added

    DESCRIPTION
       Reset all the top level XSL parameters added

    RETURNS
       (uword) error code, XMLERR_OK [0] on success

    ARGUMENTS
        xslSSctx (IN) the XSL processing context to be inquired upon
*/
uword xslresetallparams(xslctx *xslSSctx);

/*---------------------------------------------------------------------------
    NAME
       xslsetoutputmethod

    FUNCTION
       Set overriding XSL output method

    DESCRIPTION
       This call is used to set an output method equivalent to XSL 
       processing direction <xsl:output method = "text/xml/html">.
       If the xsl stylesheet contains one or more similar statements 
       then this function will override the effect of any previously
       present statements in XSL stylesheet.

    RETURNS
       (uword) error code, XMLERR_OK [0] on success

    ARGUMENTS
        xslSSctx (IN) XSL processing context
        method   (IN) selected output method
        resvd    (IN) reserved for future use [must be set to 0]
*/
uword xslsetoutputmethod(xslctx *xslSSctx, xsloutputmethod method, ub4 resvd);

/*---------------------------------------------------------------------------
    NAME
       xslsetoutputencoding

    FUNCTION
       Set encoding for XSL output

    DESCRIPTION
       This call is used to set an output method equivalent to XSL 
       processing direction <xsl:output encoding = ".....">.
       If the xsl stylesheet contains one or more similar statements 
       then this function will override the effect of any previously
       present statements in XSL stylesheet.

    RETURNS
       (uword) error code, XMLERR_OK [0] on success

    ARGUMENTS
        xslSSctx  (IN) XSL processing context
        outcoding (IN) output encoding name
*/
uword xslsetoutputencoding(xslctx *xslSSctx, oratext *outcoding);

/*---------------------------------------------------------------------------
    NAME
        xslsetoutputdomctx

    FUNCTION
        Set XML context for storing XSL processing result

    DESCRIPTION
        xslsetoutputdomctx  is used for setting the xmlctx which will be
        used to store the output DOM tree.  A call to xslsetoutputdomctx
        overrides any previous output method set using any previous calls
        to xslsetoutputdomctx or xslsetoutputstream or xslsetoutputsax.
        Any subsequent processing of the XML input will be stored in form
        of a Document fragment in xmlctx specified by resctx.
        xslgetoutputdomctx can be used to inquire presently set resctx.
        If no DOM context is set or a call to xslsetoutputdomctx has been
        overridden by a subsequent call to xslsetoutputstream or 
        xslsetoutputsax then xslgetoutputdomctx will return NULL context
        indicating that present output mode is not a DOM output mode.

    RETURNS
        (uword) error code, XMLERR_OK [0] on success

    ARGUMENTS
        xslSSctx (IN) XSL processing context
        resCtx   (IN) XML context for output document fragment 
*/
uword xslsetoutputdomctx(xslctx *xslSSctx, xmlctx *resctx);

/*---------------------------------------------------------------------------
    NAME
        xslgetoutputdomctx

    FUNCTION
        Returns XML context for storing XSL output

    DESCRIPTION
        Returns the XML context set by a previous call to 
        xslsetoutputdomctx.

    RETURNS
        (xmlctx *) XML context if successful, Otherwise returns NULL.

    ARGUMENTS
        xslSSctx (IN) XSL processing context
*/
xmlctx *xslgetoutputdomctx(xslctx *xslSSctx);

/*---------------------------------------------------------------------------
    NAME 
        xslsetoutputstream

    FUNCTION 
        DEPRECATED Set the XSL output stream

    DESCRIPTION
        DEPRECATED - DO NOT USE THIS FUNCTION FOR NEW WORK.  ALL STREAMING
        CODE SHOULD USE THE NEW orastream STRUCTURE DEFINED IN orastruc.h
        xslsetoutputstream is used for setting the xmlstream to be 
        used for redirecting the text output of the XSL processing. A 
        call to xslsetoutputstream  overrides any previous output 
        method set using any previous calls to xslsetoutputdomctx 
        or xslsetoutputstream or xslsetoutputsax.  Any subsequent 
        processing of the XML input will be redirected in text format to
        the callbacks specified by stream. xslgetoutputstream  can be 
        used to inquire presently set stream. If no output stream is set 
        or a call to xslsetoutputstream  has been overridden by a 
        subsequent call to xslsetoutputdomctx or xslsetoutputsax  
        then xslgetoutputstream  will return NULL indicating that 
        present output mode is not a stream based output 
        mode.

    RETURNS
        (uword) error code, XMLERR_OK [0] on success

    ARGUMENTS
        xslSSctx (IN) XSL processing context
        stream   (IN) stream structure
*/
uword xslsetoutputstream(xslctx *xslSSctx, xmlstream *stream);

/*---------------------------------------------------------------------------
    NAME 
        xslsetoutputorastream

    FUNCTION 
        Set the XSL output orastream

    DESCRIPTION
        xslsetoutputstream is used for setting the orastream to be 
        used for redirecting the text output of the XSL processing. A 
        call to xslsetoutputstream  overrides any previous output 
        method set using any previous calls to xslsetoutputdomctx 
        or xslsetoutputstream or xslsetoutputsax.  Any subsequent 
        processing of the XML input will be redirected in text format to
        the callbacks specified by stream. xslgetoutputstream  can be 
        used to inquire presently set stream. If no output stream is set 
        or a call to xslsetoutputstream  has been overridden by a 
        subsequent call to xslsetoutputdomctx or xslsetoutputsax  
        then xslgetoutputstream  will return NULL indicating that 
        present output mode is not a stream based output mode.

    RETURNS
        (uword) error code, XMLERR_OK [0] on success

    ARGUMENTS
        xslSSctx (IN) XSL processing context
        stream   (IN) orastream structure
*/
uword xslsetoutputorastream(xslctx *xslSSctx, orastream *ostream);

/*---------------------------------------------------------------------------
    NAME
        xslgetoutputstream

    FUNCTION
        Returns the XSL output stream

    DESCRIPTION
        Returns the xmlstream set by a previous call to xslsetoutputstream

    RETURNS
        (xmlstream *) stream or NULL.

    ARGUMENTS
        xslSSctx (IN) XSL processing context
*/
xmlstream *xslgetoutputstream(xslctx *xslSSctx);

/*---------------------------------------------------------------------------
    NAME
        xslgetoutputorastream

    FUNCTION
        Returns the XSL output orastream

    DESCRIPTION
        Returns the orastream set by a previous call to xslsetoutputorastream

    RETURNS
        (orastream *) stream or NULL.

    ARGUMENTS
        xslSSctx (IN) XSL processing context
*/
orastream *xslgetoutputorastream(xslctx *xslSSctx);

/*---------------------------------------------------------------------------
    NAME
        xslsetoutputsax

    FUNCTION
        Sets the SAX based events to be generated

    DESCRIPTION
        Sets the SAX callback structure which describes SAX event to be
        be invoked as the processing progresses to generate output nodes.
        The context to be passed to the SAX event handler is set with a
        call to xslsetoutputsaxctx (or the context will be NULL).

        A call to xslsetoutputsax overrides any previous output method 
        set using any previous calls to xslsetoutputdomctx or 
        xslsetoutputstream or xslsetoutputsax.  Any subsequent processing
        of the XML input will generate SAX events specified by the SAX
        context.  xslgetoutputsax can be used to inquire presently set SAX
        context. If no SAX context is set or a call to xslsetoutputsax has
        been overridden by a subsequent call to xslsetoutputdomctx or 
        xslsetoutputstream then xslgetoutputsax will return NULL indicating
        that present output mode is not a SAX based output mode.

        Note: User can make a call to xslsetoutputsaxctx to set the SAX
        context to be used by the SAX callbacks. If none is set, then a NULL
        will be passed to the SAX callbacks as SAX context.

        Also note that just setting the callbacks is sufficient to invoke
        the SAX callbacks.

    RETURNS
        (uword) error code, XMLERR_OK [0] on success

    ARGUMENTS
        xslSSctx (IN) XSL processing context to be inquired upon
        s        (IN) SAX callbacks structure

    SEE ALSO
        xslsetoutputsaxctx
*/
uword xslsetoutputsax(xslctx *xslSSctx, xmlsaxcb *s);

/*---------------------------------------------------------------------------
    NAME
        xslsetoutputsaxctx

    FUNCTION
        Sets the context for SAX events

    DESCRIPTION
        Sets the context to be passed to SAX event handlers.  If not
        specified, it will be NULL.

    RETURNS
        (uword) error code, XMLERR_OK [0] on success

    ARGUMENTS
        xslSSctx (IN) XSL processing context to be inquired upon
        ctx      (IN) user-defined SAX context

    SEE ALSO
        xslsetoutputsax
*/
uword xslsetoutputsaxctx(xslctx *xslSSctx, void *ctx);

/*---------------------------------------------------------------------------
    NAME
        xslgetoutputsax

    FUNCTION
        Returns SAX callbacks

    DESCRIPTION
        Returns the xmlsaxcbset by a previous call to xslsetoutputsax.

    RETURNS
        (xmlsaxcb *) SAX callback structure or NULL

    ARGUMENTS
        xslSSctx (IN) XSL processing context
*/
xmlsaxcb *xslgetoutputsax(xslctx *xslSSctx);

/*---------------------------------------------------------------------------
    NAME
        xslprocessxml

    FUNCTION
        Processes XML

    DESCRIPTION
        Once user have created the xslctx (the XSL processing context 
        using LpxXSLCreatCtx), have set the base URI, set all the top 
        level parameters (using one or more calls to xslsettextparam),
        have selected an output method (using one of the calls to 
        xslsetoutputdomctx or xslsetoutputstream or 
        xslsetoutputsax).. .. .. then this is the function call to 
        make to carry out all the action. This function will process 
        the given XML file (specified by its DOM context) according to 
        the XSL stylesheet, parameters and, output method set in the 
        xslctx. Once this call returns, depending on the output scheme 
        which user might have selected:

        For Stream based output:
            The output text has been redirected using the stream callbacks.

        For SAX based output:
            User would have already got all the SAX events executed.

        For DOM based output:
            User can make a call to xslgetresultdocfrag function to 
            access the final Document Fragment created in context of
            the resCtx set by a call to xslsetoutputdomctx.

        The XSL processor expects a normalized DOM tree as an input.  If
        user is not sure whether the XML tree is normalized or not, then
        he should set this flag to TRUE and this call will normalize the DOM;
        Otherwise set it to FALSE.

        xsl:output is supported only for stream based output.

    RETURNS
        (uword) error code, XMLERR_OK [0] on success

    ARGUMENTS
        xslSSctx  (IN) XSL processing context
        xmlCtx    (IN) XML context of the input XML tree
        normalize (IN) normalize input document?
        resvd     (IN) reserved for future use [must be set to 0]
*/
uword xslprocessxml(xslctx *xslSSctx, xmlctx *xmlctx, 
                    boolean normalize, ub4 resvd);

/*---------------------------------------------------------------------------
    NAME
        xslprocessxmldocfrag

    FUNCTION
        Processs an XML document fragment

    DESCRIPTION
        Once user have created the xslctx (the XSL processing context 
        using LpxXSLCreatCtx), have set the base URI, set all the top 
        level parameters (using one or more calls to xslsettextparam),
        have selected an output method (using one of the calls to 
        xslsetoutputdomctx or xslsetoutputstream or 
        xslsetoutputsax).. .. .. then this is the function call to 
        make to carry out all the action. This function will process 
        the given XML document fragment according to the XSL stylesheet, 
        parameters and, output method set in the xslctx. Once this call 
        returns, depending on the output scheme which user might have 
        selected:

        For Stream based output:
            The output text has been redirected using the stream callbacks.

        For SAX based output:
            User would have already got all the SAX events executed.

        For DOM based output:
            User can make a call to xslgetresultdocfrag function to 
            access the final Document Fragment created in context of
            the resCtx set by a call to xslsetoutputdomctx.

        The XSL processor expects a normalized DOM tree as an input.  If
        user is not sure whether the XML tree is normalized or not, then
        he should set this flag to TRUE and this call will normalize the DOM;
        Otherwise set it to FALSE.

        xsl:output is supported only for stream based output.

    RETURNS
        (uword) error code, XMLERR_OK [0] on success

    ARGUMENTS
        xslSSctx  (IN) XSL processing context
        xmlCtx    (IN) XML context for the input XML Doc Frag
        docFrag   (IN) document fragment
        normalize (IN) normalize input document?
        resvd     (IN) reserved for future use [must be set to 0]
*/
uword xslprocessxmldocfrag(xslctx *xslSSctx, xmlctx *xmlctx, xmlnode *docFrag,
                           boolean normalize, /* ARGUSED */ ub4 resvd);

/*---------------------------------------------------------------------------
    NAME
        xslgetresultdocfrag

    FUNCTION
        Gets resulting document fragment

    DESCRIPTION
        This function should be called after a call to xslprocessxml  
        for the DOM based output method. If the output method for the 
        last call to xslprocessxml was DOM based, this function will 
        return the Document Fragment created by XSL processing of the 
        given XML tree, otherwise this function will return NULL 
        indicating that the output method selected was not DOM based or 
        no XML processing has been carried out since either the creation 
        of xslSSctx or since last selecting an output method for the 
        specified xslctx.

    RETURNS
        (xmlnode *) resulting DOM tree.

    ARGUMENTS
        xslSSctx (IN) XSL processing context
*/
xmlnode *xslgetresultdocfrag(xslctx *xslSSctx);

/*---------------------------------------------------------------------------
    NAME
        xslterm

    FUNCTION
        Terminates XSL context

    DESCRIPTION
        This function will terminate the XSL context and free all the
        intermediate data structures created for it during any previous
        processing using this context. Once terminated, the XSL context
        is no more valid and must not be used any further.

    RETURNS
        (uword) error code, XMLERR_OK [0] on success

    ARGUMENTS
        xslSSctx (IN) XSL processing context
*/
uword xslterm(xslctx *xslSSctx);

/****************************************************************************
                            Interface XPath
 ****************************************************************************/

/*---------------------------------------------------------------------------
    NAME
        xpmakexpathctx

    FUNCTION
        Makes an XPath context

    DESCRIPTION
        Creates, initializes and returns an XPath context.  The XPath
        context makes use of an internally created dummy XSL and XML
        context. When you are done executing the XPath related function
        calls, free this context with xpfreexpathctx.

        If ctx is NULL, we internally create a dummy xml context.  This
        context is used to obtain the encoding, memory management, access
        routines and error handlers.

        If xslnode is NULL, no name space expansion will be done at the
        time of parsing.  xml_node should be NULL for parsing.  baseURI
        sets the base URI for document function calls which might be
        made internally.  Set nctxels to the number of context nodes to
        follow (or 0 if no context list is available).

    RETURNS
        (xpctx *) the created XPath context

    ARGUMENTS
        ctx      (IN) XML parser context
        xslnode  (IN) XSL node used for namespace expansion
        xml_node (IN) context node for expression evaluation.
        baseURI  (IN) base URI for document
        nctxels  (IN) number of context nodes to follow
        ctxnodes (IN) array of nctxels nodes

    SEE ALSO
        xpfreexpathctx
*/
xpctx *xpmakexpathctx(xmlctx *ctx, xmlnode *xslnode,
                      xmlnode *xml_node, oratext *baseURI,
                      size_t nctxels, xmlnode **ctxnodes);

/*---------------------------------------------------------------------------
    NAME
        xpfreexpathctx

    FUNCTION
        Frees an XPath context

    DESCRIPTION
        Frees an XPath context allocated by a prior call to xpmakexpathctx.
        Once freed the xctx pointer is no more valid and must not be used
        further.

    RETURNS
        (void)

    ARGUMENTS
        xctx (I/O) XPath context to be freed.

    SEE ALSO
        xpmakexpathctx
*/
void xpfreexpathctx(xpctx *xctx);

/*---------------------------------------------------------------------------
    NAME
        xpparsexpathexpr

    FUNCTION
        Parses an XPath expression

    DESCRIPTION
        Take an expression in the form of a string and parses it.  The
        result is in the form of an expression tree.  The encoding should
        be in the data encoding.

    RETURNS
        (xpexpr *) expression tree or NULL

    ARGUMENTS
        xctx (I/O) XPath context
        expr (IN)  expression [in data encoding]
        err  (OUT) error code, XMLERR_OK [0] on success

    SEE ALSO
        xpevalxpathexpr
*/
xpexpr *xpparsexpathexpr(xpctx *xctx, oratext *expr, sword *err);

/*---------------------------------------------------------------------------
    NAME
        xpevalxpathexpr

    FUNCTION
        Evaluates an XPath expression

    DESCRIPTION
        Evaluates the previously parsed expression and returns the result.
        The exprtree is obtained by making a call to xpparsexpathexpr to parse
        the given XPath expression string.

    RETURNS
        (xpobj *) result object or NULL

    ARGUMENTS
        xctx (I/O) XPath context
        expr (IN)  parsed expression.
        err  (OUT) error code, XMLERR_OK [0] on success

    SEE ALSO
        xpparsexpathexpr
*/
xpobj *xpevalxpathexpr(xpctx *xctx, xpexpr *exprtree, sword *err);

/*---------------------------------------------------------------------------
    NAME
        xpgetxpobjtyp

    FUNCTION
        Returns the type code of an XPath object

    DESCRIPTION
        Returns the type code of an XPath object

    RETURNS
        (xpobjtyp) object type

    ARGUMENTS
        xobj (IN) XPath object

    SEE ALSO
        xpgetbooleanval, xpgetnumval, xpgetstrval, xpgetnsetval, xpgetrtfragval
*/
xpobjtyp xpgetxpobjtyp(xpobj * xobj);

/*---------------------------------------------------------------------------
    NAME
        xpgetbooleanval

    FUNCTION
        Returns the value of a boolean XPath object

    DESCRIPTION
        For bool XPath object (type XPOBJTYP_BOOL), returns its
        boolean value, TRUE or FALSE.

    RETURNS
        (boolean) boolean object's value

    ARGUMENTS
        xobj (IN) XPath object

    SEE ALSO
        xpgetnumval, xpgetstrval, xpgetnsetval, xpgetrtfragval
*/
boolean xpgetbooleanval(xpobj *xobj);

/*---------------------------------------------------------------------------
    NAME
        xpgetnumval

    FUNCTION
        Returns the value of a numeric XPath object

    DESCRIPTION
        For numeric XPath objects (type XPOBJTYP_NUM), returns its
        numeric value

    RETURNS
        (double) numeric object's value

    ARGUMENTS
        xobj (IN) XPath object

    SEE ALSO
        xpgetbooleanval, xpgetstrval, xpgetnsetval, xpgetrtfragval
*/
double xpgetnumval(xpobj *xobj);

/*---------------------------------------------------------------------------
    NAME
        xpgetstrval

    FUNCTION
        Returns the value of a string XPath object

    DESCRIPTION
        For string-type XPath objects (type XPOBJTYP_STR), returns
        its string value.

    RETURNS
        (oratext *) string object's value

    ARGUMENTS
        xobj (IN) XPath object

    SEE ALSO
        xpgetbooleanval, xpgetnumval, xpgetnsetval, xpgetrtfragval
*/
oratext *xpgetstrval(xpobj *xobj);

/*---------------------------------------------------------------------------
    NAME
        xpgetnsetval

    FUNCTION
        Returns the node-set of a node-set XPath object

    DESCRIPTION
        For node-set XPath objects (type XPOBJTYP_NSET), returns
        its node-set value.

    EXAMPLE
        nsetval = xpgetnsetval(xobj);
        for (nsetele = xpgetfirstnsetelem(nsetval);
             nsetelem; nsetelem = xpgetnextnsetelem(nsetval))
        {
            node = xpgetnsetelemnode(nsetele);
            ....use node ...
        }

    RETURNS
        (xpnset *) XPath object's node-set value

    ARGUMENTS
        xobj (IN) XPath object

    SEE ALSO
        xpgetfirstnsetelem, xpgetnextnsetelem, xpgetnsetelemnode
*/
xpnset *xpgetnsetval(xpobj *xobj);

/*---------------------------------------------------------------------------
    NAME
        xpgetfirstnsetelem

    FUNCTION
        Returns first element in a node-set

    DESCRIPTION
        Returns first element in a node-set.  See xpgetnsetval for
        typical usage.

    RETURNS
        (xpnsetele *) node-set element

    ARGUMENTS
        nset (IN) XPath node-set

    SEE ALSO
        xpgetnextnsetelem, xpgetnsetelemnode
*/
xpnsetele *xpgetfirstnsetelem(xpnset *nset);

/*---------------------------------------------------------------------------
    NAME
        xpgetnextnsetelem

    FUNCTION
        Returns next element in a node-set

    DESCRIPTION
        Returns next element in a node-set, given an element in that
        node-set.  See xpgetnsetval for sample usage.

    RETURNS
        (xpnsetele *) next node-set element

    ARGUMENTS
        nsetele (IN) current node-set element

    SEE ALSO
        xpgetfirstnsetelem, xpgetnsetelemnode
*/
xpnsetele *xpgetnextnsetelem(xpnsetele *nsetele);

/*---------------------------------------------------------------------------
    NAME
        xpgetnsetelemnode

    FUNCTION
        Returns the XML node for a node-set element

    DESCRIPTION
        Returns the XML node corresponding to a node-set element.  See
        xpgetnsetval for sample usage.

    RETURNS
        (xmlnode *) XML node

    ARGUMENTS
        nsetele (IN) node-set element

    SEE ALSO
        xpgetfirstnsetelem, xpgetnextnsetelem, xpgetnsetelemnode
*/
xmlnode *xpgetnsetelemnode(xpnsetele *nsetele);

/*---------------------------------------------------------------------------
    NAME
        xpgetrtfragval

    FUNCTION
        Returns the XML node of a result-tree fragment

    DESCRIPTION
        Returns the XML node corresponding to a result-tree fragment
        XPath object (type XPOBJTYP_RTFRAG).

    RETURNS
        (xmlnode *) XML node

    ARGUMENTS
        xobj (IN) XPath object
*/
xmlnode *xpgetrtfragval(xpobj *xobj);

#endif  /* ORAXML_ORACLE */
