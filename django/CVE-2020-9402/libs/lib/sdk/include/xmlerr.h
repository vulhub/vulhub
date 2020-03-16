
/* $RCSfile: xmlerr.h $ $Date: 2018/06/22 02:22:57 $
 Copyright (c) 2001, 2018, Oracle and/or its affiliates. All rights reserved.

   FILE NAME
        xmlerr.h - Oracle XML error codes

   FILE DESCRIPTION
        This file lists XML error codes.
*/

#ifndef XMLERR_ORACLE
# define XMLERR_ORACLE

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

/*---------------------------------------------------------------------------
                     PUBLIC TYPES AND CONSTANTS
  ---------------------------------------------------------------------------*/

#ifndef ORAXML_ORACLE

/*---------------------------------------------------------------------------

                      ERROR CODES, XMLERR_xxx

                        0000 0099 Generic
                        0100 0199 Validity Constraint (VC) and other Warnings
                        0200 0299 Parser
                        0300 0399 XSL
                        0400 0499 XPath
                        0500-0550 Iterator, TreeWalker, Range
                        0600-0699 XSLVM
                        0700-0749 SOAP
                        0750-0799 WebDAV
                        0900-0999 XmlDiff

  ---------------------------------------------------------------------------*/

/* Errors in the following range are considered "warnings" and may continue */

#define XMLERR_WARN_MIN         100     /* minimum warning code */
#define XMLERR_WARN_MAX         199     /* maximum warning code */

/* DATATYPE xmlerr - XML numeric error return code
   SEE ALSO XmlDomGetLastError
   Numeric error code returned by many functions.  A zero value indicates
   success; a non-zero value indicates error.
*/

typedef enum
{

    XMLERR_OK                   = 0,    /* success return */
    XMLERR_NULL_PTR             = 1,    /* NULL pointer */
    XMLERR_NO_MEMORY            = 2,    /* out of memory */
    XMLERR_HASH_DUP             = 3,    /* duplicate entry in hash table */
    XMLERR_INTERNAL             = 4,    /* internal error */
    XMLERR_BUFFER_OVERFLOW      = 5,    /* name/quoted string too long */
    XMLERR_BAD_CHILD            = 6,    /* invalid child for parent */
    XMLERR_EOI                  = 7,    /* unexpected End Of Information */
    XMLERR_BAD_MEMCB            = 8,    /* invalid memory callbacks */
    XMLERR_BANNER_URI           = 9,    /* banner messages (source=URI) */
    XMLERR_BANNER_URI_PENT      = 10,   /* 9+1 (parmemeter entity version) */
    XMLERR_BANNER_URI_GENT      = 11,   /* 9+2 (general entity version) */
    XMLERR_UNICODE_ALIGN        = 12,   /* Unicode data misalignment */
    XMLERR_NODE_TYPE            = 13,   /* wrong node type */
    XMLERR_UNCLEAN              = 14,   /* context is not clean */
    /* --- Errors 15-17 are for internal use */
    XMLERR_NESTED_STRINGS       = 18,   /* internal err: nested open strs */
    XMLERR_PROP_NOT_FOUND       = 19,   /* property not found */
    XMLERR_SAVE_OVERFLOW        = 20,   /* save output overflowed */
    XMLERR_NOT_IMP              = 21,   /* feature not implemented */
    XMLERR_INTERNAL_SUB         = 22,   /* internal error w/subcode */
    XMLERR_SHORT_WRITE          = 23,   /* short write (not all bytes) */
    XMLERR_PREFIX_LENGTH        = 24,   /* qname prefix too long */
    /* --- Errors 50-59 are initialization errors; they have no associated
           error messages since they occur before the error message system
           has been set up.  */
    XMLERR_NLS_MISMATCH         = 50,   /* specify both lxglo/lxd or neither */
    XMLERR_NLS_INIT             = 51,   /* error during NLS initialization */
    XMLERR_LEH_INIT             = 52,   /* error during LEH initialization */
    XMLERR_MEM_INIT             = 53,   /* error during Mem initialization */
    XMLERR_URL_INIT             = 54,   /* error during XmlUrl initialization */

    /* --- Errors 60-69 are for Load/Save */
    XMLERR_LOAD_NO_IO           = 60,   /* no I/Os specified */
    XMLERR_LOAD_TOO_IO          = 61,   /* too many I/Os specified */
    /* --- Errors 100-149 are for warnings (DTD Validity Constraint checks) */
    XMLERR_VC_ROOT_ELEM         = 100,  /* root element mismatch */
    XMLERR_VC_DECL_PE_NEST      = 101,  /* improper decl/pe nesting */
    XMLERR_VC_STDALONE_DOC      = 102,  /* bogus standalone doc decl */
    XMLERR_VC_BAD_ELEM          = 103,  /* invalid element (mismatch DTD) */
    XMLERR_VC_UNDEF_ELEM        = 104,  /* element is not defined */
    XMLERR_VC_NOT_EMPTY         = 105,  /* element should be empty */
    XMLERR_VC_UNDEF_ATTR        = 106,  /* undefined attribute */
    XMLERR_VC_UNIQUE_ELEM       = 107,  /* element multiply defined */
    XMLERR_VC_GROUP_PE_NEST     = 108,  /* improper group/pe nesting */
    XMLERR_VC_DUP_TYPE          = 109,  /* duplicate name in mixed decl */
    XMLERR_VC_BAD_NAME          = 110,  /* bad Name */
    XMLERR_VC_BAD_NMTOKEN       = 111,  /* bad Nmtoken */
    XMLERR_VC_ONE_ID            = 112,  /* multiple ID attributes */
    XMLERR_VC_ID_ATTR_DEF       = 113,  /* ID not IMPLIED or REQUIRED */
    XMLERR_VC_ENUM              = 114,  /* attr val not in enumeration */
    XMLERR_VC_REQ_ATTR          = 115,  /* missing required attribute */
    XMLERR_VC_FIXED_ATTR        = 116,  /* wrong val for fixed attribute */
    XMLERR_VC_UNDEF_ENTITY      = 118,  /* undefined entity */
    XMLERR_VC_ENTITY_PARSE      = 119,  /* entity attribute not unparsed */
    XMLERR_VC_ENTITY_NDATA      = 120,  /* undefined entity NDATA */
    XMLERR_VC_UNDEF_NOTE        = 121,  /* undefined notation */
    XMLERR_VC_UNDEF_ID          = 122,  /* undefined ID (in IDREF) */
    XMLERR_VC_DUP_ID            = 123,  /* duplicate ID */
    XMLERR_VC_ATTR_REFS         = 124,  /* attr value missing refs */
    XMLERR_DUP_ENTITY           = 125,  /* duplicate entity */
    XMLERR_NO_DECL              = 126,  /* asked for XMLDecl but no got */
    /* --- Errors 200-299 are Parser */
    XMLERR_CONVERT              = 200,  /* encoding conversion problem */
    XMLERR_BAD_ENCODING         = 201,  /* invalid encoding */
    XMLERR_OPEN_INPUT           = 202,  /* could not open input */
    XMLERR_READ_INPUT           = 203,  /* could not read from input */
    XMLERR_SYNTAX               = 204,  /* generic syntax error */
    XMLERR_COMMENT_SYNTAX       = 205,  /* comment syntax error */
    XMLERR_CDATA_SYNTAX         = 206,  /* CDATA syntax error */
    XMLERR_COND_SYNTAX          = 207,  /* conditional section syntax error */
    XMLERR_DTD_KEYWORD          = 208,  /* unknown keyword found in DTD */
    XMLERR_XML_RESERVED         = 209,  /* XML cannot be used in a PI */
    XMLERR_NOT_EXPECTED         = 210,  /* syntax error */
    XMLERR_DEF_DECL             = 211,  /* error in default declaration */
    XMLERR_COMMENT_END          = 212,  /* comment syntax error */
    XMLERR_COMMENT_NOEND        = 213,  /* comment syntax error */
    XMLERR_CDATA_NOEND          = 214,  /* CDATA syntax error */
    XMLERR_PIDATA_NOEND         = 215,  /* PIDATA syntax error */
    XMLERR_BAD_WIDE_CHAR        = 216,  /* Invalid lxwchar */
    XMLERR_BAD_UNICODE_CHAR     = 217,  /* Invalid lxuchar */
    XMLERR_BAD_NATIVE_CHAR      = 218,  /* Invalid ASCII/EBCDIC character */
    XMLERR_BAD_CHARREF          = 219,  /* Invalid character */
    XMLERR_CHARDATA             = 220,  /* Error while processing text */
    XMLERR_ATTR_VALUE           = 221,  /* Invalid char in attribute value */
    XMLERR_SAX                  = 222,  /* Error from SAX callback */
    XMLERR_WFC_EXT_ENTITY       = 223,  /* No external entity in attrib val */
    XMLERR_WFC_UNIQUE_ATTR      = 224,  /* Attributes must be unique */
    XMLERR_WFC_ELEM_MATCH       = 225,  /* Start tag and end tag of elem */
    XMLERR_WFC_ENTITY_DECL      = 226,  /* Entity not declared */
    XMLERR_WFC_PARSED_ENT       = 227,  /* Must be a parsed entity */
    XMLERR_WFC_RECUR            = 228,  /* No recursion in entity refs */
    XMLERR_EMPTY_FILE           = 229,  /* Empty file */
    XMLERR_BAD_NM_UNI_CHAR      = 230,  /* Invalid character in NAME/NMTOKEN */
    XMLERR_BAD_NM_CHAR          = 231,  /* Invalid character in NAME/NMTOKEN */
    XMLERR_WFC_BAD_PE           = 232,  /* PE improperly used in internal DTD*/
    XMLERR_NSC_LEADING_XML      = 233,  /* Leading XML for namespace prefix */
    XMLERR_NSC_PREFIX_DECL      = 234,  /* Namespace prefix not declared */
    XMLERR_BAD_VERSION          = 235,  /* XML Version not supported */
    XMLERR_BAD_PUB_CHAR         = 236,  /* Invalid pubid character */
    XMLERR_COND_KEYWORD         = 237,  /* condition section keyword invalid */
    XMLERR_COND_UNFINISHED      = 238,  /* condition is unfinished (open) */
    XMLERR_ATTR_TYPE            = 239,  /* invalid attribute type */
    XMLERR_NWF_ELEM_START       = 240,  /* element-start tag is NWF */
    XMLERR_NWF_ENTREF           = 241,  /* entity reference is NWF */
    XMLERR_AMPERSAND            = 242,  /* invalid use of ampersand char */
    XMLERR_ATTR_QUOTE           = 243,  /* elem attr value not in quotes */
    XMLERR_LESSTHAN             = 244,  /* invalid use of '<' character */
    XMLERR_EXTRA_DATA           = 245,  /* extra data at end of document */
    XMLERR_NO_SYSID             = 246,  /* missing SYSID after PUBID */
    XMLERR_BAD_DTD              = 247,  /* bad DTD declaration */
    XMLERR_BAD_ENTITY_DECL      = 248,  /* bad entity declaration */
    XMLERR_BAD_EXTID            = 249,  /* bad external ID declaration */
    XMLERR_BAD_ATTR_DECL        = 250,  /* bad attribute declaration */
    XMLERR_INT_COND             = 251,  /* no conditionals in internal DTD */
    XMLERR_ENTITY_NESTING       = 252,  /* improper entity nesting */
    XMLERR_NO_VERSION           = 253,  /* missing required version# */
    XMLERR_BAD_XML_DECL         = 254,  /* bad XML declaration */
    XMLERR_STANDALONE_YN        = 255,  /* invalid standalone yes/no */
    XMLERR_ELEMENT_DECL         = 256,  /* invalid element declaration */
    XMLERR_CHILDREN_DECL        = 257,  /* invalid children declaration */
    XMLERR_MIXED_DECL           = 258,  /* invalid mixed declaration */
    XMLERR_NOTATION_DECL        = 259,  /* invalid notation declaration */
    XMLERR_XMLSPACE_DECL        = 260,  /* invalid xml:space declaration */
    XMLERR_BAD_URL              = 261,  /* invalid URL */
    XMLERR_UNSUP_PROT           = 262,  /* unsupported protocol */
    XMLERR_OPEN_TCP             = 263,  /* failed to open TCP connection */
    XMLERR_SEND_TCP             = 264,  /* failed to send to TCP connection */
    XMLERR_RECV_TCP             = 265,  /* failed to read from TCP conn. */
    XMLERR_BAD_LANG             = 266,  /* invalid language specification */
    XMLERR_RESOLVE_URL          = 267,  /* couldn't resolve relative URL */
    XMLERR_BAD_ACCESS           = 268,  /* invalid access method */
    XMLERR_ACCESS_FUNCS         = 269,  /* all access funcs must be provided */
    XMLERR_FTP_CMD              = 270,  /* FTP command failed */
    XMLERR_FTP_LOGIN            = 271,  /* FTP login failed */
    XMLERR_FTP_UNAVAIL          = 272,  /* FTP server unavailable */
    XMLERR_INIT_TCP             = 273,  /* failed to initialize TCP/IP */
    XMLERR_CANT_IMPORT          = 274,  /* can't import that node type */
    XMLERR_CANT_SET_ENC         = 275,  /* can't set output encoding */
    XMLERR_BAD_HTTP_MIME        = 276,  /* bad Mime header in HTTP reply */
    XMLERR_END_QUOTE            = 277,  /* No ending quote was seen */
    XMLERR_INVALID_PROXY        = 278,  /* bad value for "http_proxy" */
    XMLERR_INVALID_NO_PROXY     = 279,  /* bad value for "no_proxy" */
    XMLERR_HTTP_ERROR           = 280,  /* HTTP prococol error */
    XMLERR_UNSUP_ENCODING       = 281,  /* unsupported encoding */
    XMLERR_SHARED_DTD_MIX       = 282,  /* doc can't have DTD if set shared */
    XMLERR_WRONG_ENCODING       = 283,  /* input document in wrong encoding */
    XMLERR_NULL_URI             = 284,  /* NULL URI in namespace pfx decl */
    XMLERR_MAXDEPTH             = 286,  /* Exceeded max depth for recursion */
    XMLERR_MAXLIMIT             = 287,  /* Generic max limit error */
    XMLERR_MAX_PATGRPTBL_LIMIT  = 291,  /* Max limit for pattern group table */

    /* --- XSL error codes */
    XMLERR_NONAMEINATTRSET        = 300, /* no name in attribute set */
    XMLERR_ERROR_IN_XPATH         = 301, /* XPATH function returns an error */
    XMLERR_CANNOT_TRANSFORM       = 302, /* child node has invalid type/name */
    XMLERR_ATTRIBUTE_NOT_EXPECTED = 303, /* Attr found but wrong value */
    XMLERR_NULL_INPUT_STRING      = 304, /* input string null */
    XMLERR_MISSING_TOKEN          = 305, /* expected token missing */
    XMLERR_INCOM_ATTR_VAL_TMPL    = 306, /* inp string missing closing brace */
    XMLERR_NS_PREFIX_NOT_DECLARED = 307, /* nsp prefix used but not declared */
    XMLERR_ATTRIBUTE_NOT_FOUND    = 308, /* expected attr node not found */
    XMLERR_CANNOT_INIT_XPATH      = 309, /* XPATH ctx could not be init'd */
    XMLERR_ELEMENT_NOT_FOUND      = 310, /* expected element not found. */
    XMLERR_FEATURE_NOT_SUPPORTED  = 311, /* feature not supported */
    XMLERR_CANNOT_CONS_PI         = 312, /* cont of PI node might be invalid */
    XMLERR_CANNOT_CONS_COMMENT    = 313, /* cont of XML comment might be inv */
    XMLERR_FAIL                   = 314, /* internal error occurred */
    XMLERR_EXT_FUNC_NOT_SUPPORTED = 315, /* extension function not supported */
    XMLERR_BAD_ATTR_VALUE         = 316, /* invalid value for attr */
    XMLERR_UNDEF_DECIMAL_FORMAT   = 317, /* undefined decimal-format */
    XMLERR_DUP_DEC_FORMAT         = 318, /* duplicate decimal-format */
    XMLERR_BAD_DATA               = 319, /* The data is not allowed */
    XMLERR_OUTOFORDERATTRIBUTE    = 320, /* Cant add more attributes */
    XMLERR_NULL_OUTPUT            = 321, /* No mechanism for output */
    XMLERR_DOCUMENTNOTFOUND       = 322, /* Could not open doc */
    XMLERR_APPLY_IMPORTS          = 323, /* Cant apply imports */
    XMLERR_INV_LANG               = 324, /* Invalid LANG for xsl:sort */
    XMLERR_RECUR_IMP_INCLUDE      = 325, /* recursive import/include */
    XMLERR_RECUR_ATTR_SET         = 326, /* recursive attribute sets */
    XMLERR_UNKNOWN_PARAM          = 327, /* unknown or undefined parameter */
    XMLERR_PARAM_IS_SET           = 328, /* parameter is already set */
    /* --- XPATH error codes */
    XMLERR_XPATH_INTERNAL         = 400, /* Internal error */
    XMLERR_XPATH_INVNAME          = 401, /* Invalid QName */
    XMLERR_XPATH_INVAXIS          = 402, /* Invalid axis name */
    XMLERR_XPATH_QTNOTMATCH       = 403, /* Unmatched quote */
    XMLERR_XPATH_NSERR            = 404, /* Unable to resolve namespace */
    XMLERR_XPATH_MEMERR           = 405, /* Unable to allocate memory */
    XMLERR_XPATH_INVOBJTYP        = 406, /* Incorrect object type */
    XMLERR_XPATH_NOTRSQB          = 407, /* Right square bracket missing */
    XMLERR_XPATH_NOTRPAREN        = 408, /* Right parenthesis missing */
    XMLERR_XPATH_INVTOK           = 409, /* Invalid token */
    XMLERR_XPATH_VARREF           = 410, /* Unable to resolve the variable */
    XMLERR_XPATH_UNKFUNC          = 411, /* Unknown function */
    XMLERR_ENCODING_MISMATCH      = 412, /* encoding mismatch in Inc/Import*/
    XMLERR_XPATH_TOOBIGNUMBER     = 413, /* Too big numerical constant */
    /* --- XPath internal internal error codes (470-499) */
    XMLERR_XPATH_INVEXPRTREE      = 470, /* Invalid expression tree */
    XMLERR_XPATH_INVOP            = 471, /* Invalid operator */
    XMLERR_XPATH_INVCTX           = 472, /* Invalid context list/node */
    /* --- Errors 500-509 are Iterator */
    XMLERR_ITER_NULL              = 500,
    XMLERR_ITER_DETACHED          = 501,
    XMLERR_ITER_CUR_REMOVED       = 502,
    /* --- Errors 510-519 are TreeWalker */
    XMLERR_WALKER_NULL            = 510,
    XMLERR_WALKER_NODE_PAR_NULL   = 511,
    XMLERR_WALKER_BAD_NEW_CUR     = 512,
    XMLERR_WALKER_BAD_NEW_ROOT    = 513,
    XMLERR_WALKER_NO_ROOT         = 514,
    /* --- Errors 520-539 are Range */
    XMLERR_RANGE_NULL             = 520, /* range pointer is NULL */
    XMLERR_RANGE_DETACHED         = 521, /* range is detached */
    XMLERR_RANGE_BAD_NODE         = 522, /* node parameter is invalid */
    XMLERR_RANGE_BAD_INDEX        = 523, /* offset parameter is invalid */
    XMLERR_RANGE_BAD_DOC          = 524, /* start/end node not from this doc */
    XMLERR_RANGE_START_AFTER_END  = 525, /* start after end */
    XMLERR_RANGE_NO_ROOT          = 526, /* new start/end point has no root */
    XMLERR_RANGE_DIFF_ROOTS       = 527, /* ranges cmp'd have dif roots */
    XMLERR_RANGE_NO_ANCESTOR      = 528, /* bad error, implementation error */
    XMLERR_RANGE_NONE             = 529, /* used by private functions */
    XMLERR_RANGE_COLLAPSE         = 530, /* used by private functions */
    XMLERR_RANGE_ERROR            = 531, /* consistency error */
    /* --- Errors 540-549 are Filter return codes */
    XMLERR_FILTER_REJECT          = 540,
    XMLERR_FILTER_SKIP            = 541,

    /* --- Errors 600-690 are XVM return codes */
    XMLERR_XVM_INVTOKEN           = 601, /* Invalid token  */
    XMLERR_XVM_INVCHILDELEM       = 602, /* Invalid child element  */
    XMLERR_XVM_INVATTRVALINELEM   = 603, /* Invalid attribute value in elem */
    XMLERR_XVM_INVATTRVAL         = 604, /* Invalid attribute value  */
    XMLERR_XVM_INVATTR            = 605, /* Invalid attribute  */
    XMLERR_XVM_MISSINGATTR        = 606, /* Missing attribute  */
    XMLERR_XVM_INVREFERENCE       = 607, /* Invalid reference */
    XMLERR_XVM_DUPDECL            = 608, /* Repeated declaration  */
    XMLERR_XVM_INVARGNUM          = 609, /* Invalid number of arguments  */
    XMLERR_XVM_NULLPOINTER        = 610, /* Null node */
    XMLERR_XVM_INTERNAL           = 611, /* Internal error */
    XMLERR_XVM_RECURSIVE          = 650, /* Template recursion error */
    XMLERR_XVM_STACKOVERFLOW      = 651, /* VM Stack overflow */
    XMLERR_XVM_SAXRETURNERROR     = 652, /* SAX callback returns with error */
    XMLERR_XVM_INVRESULTATTR      = 653, /* Invalid result attribute node*/
    XMLERR_XVM_INVRESULTNSATTR    = 654, /* Invalid result namespace node */
    XMLERR_XVM_INVRESCOMMENT      = 655, /* Invalid result comment  */
    XMLERR_XVM_TERMINATE          = 656, /* XSLTVM terminate  */
    XMLERR_XVM_INVRESPI           = 657, /* Invalid result PI  */
    XMLERR_XVM_INVOBJTYPE         = 658, /* Invalid XSLT object type */
    XMLERR_XVM_WRITEFAILD         = 659, /* Output write failed */
    XMLERR_XVM_NOTWELLFORMED      = 660, /* Not a well-formed document */
    XMLERR_XVM_LOADFAILED         = 661, /* Failed to load  */
    XMLERR_XVM_INVENCODING        = 662, /* Invalid encoding  */
    XMLERR_XVM_STRSTACKOVERFLOW   = 663, /* VM String-Stack overflow  */
    XMLERR_XVM_NODESTACKOVERFLOW  = 664, /* VM Node-Stack overflow  */
    XMLERR_XVM_INVALIDARG         = 690, /* Invalid argument  */
    /* --- Errors 691-699 are XTI return codes */
    XMLERR_XTI_GENERAL_ASSERT      = 691, /* general assertion failure  */
    XMLERR_XTI_PAGE_FULL_ERR       = 692, /* out of space error */
    XMLERR_XTI_PIN_FULL_ERR        = 693, /* too many pinned pages in cache */
    XMLERR_XTI_PG_CORRUPT_ERR      = 694, /* page data corruption error */

    /* --- Errors 700-749 are SOAP return codes */
    XMLERR_SOAP_BAD_CTX           = 700, /* invalid SOAP context */
    XMLERR_SOAP_BAD_ROLE          = 701, /* invalid role */
    XMLERR_SOAP_BAD_BINDING       = 702, /* invalid connection binding */
    XMLERR_SOAP_CALL_FAILED       = 703, /* SOAP Call failed */
    XMLERR_SOAP_NO_MUST_UNDERSTAND= 704, /* elem has no mustUnderstand */
    XMLERR_SOAP_NO_ROLE           = 705, /* elem has no role */
    XMLERR_SOAP_NO_FAULT          = 706, /* message has no fault */
    XMLERR_SOAP_NO_LANG           = 707, /* no Fault reason w/given language */
    XMLERR_SOAP_HTTP_CON          = 708, /* failed to make HTTP connection */
    XMLERR_SOAP_REPLY_NWF         = 709, /* reply to SOAP Call was NWF XML */
    XMLERR_SOAP_UNSUP_WEB_METHOD  = 710, /* unsupported Web Method */
    XMLERR_SOAP_BAD_VERSION       = 711, /* invalid SOAP version */
    XMLERR_SOAP_BAD_HEADER        = 712, /* failed to set HTTP header */
    XMLERR_SOAP_MULTI_FAULTS      = 713, /* body has more than one fault */
    XMLERR_SOAP_FAULT_NOT_SINGLE  = 714, /* fault is not a single child */
    XMLERR_SOAP_BAD_FAULT         = 715, /* badly formed fault element*/
    XMLERR_SOAP_BAD_TEXT          = 716, /* badly formed Text subelement */
    XMLERR_SOAP_BAD_VALUE         = 717, /* badly formed Value subelement */
    XMLERR_SOAP_NO_ENV            = 718, /* message has no envelope */
    XMLERR_SOAP_PREF              = 719, /* qname too long */
    XMLERR_SOAP_NO_HEADER         = 720, /* envelope has no header */
    XMLERR_SOAP_NO_BODY           = 721, /* envelope has no body */
    XMLERR_SOAP_NO_RELAY          = 722, /* elem has no relay */
    XMLERR_SOAP_NO_ELEMENT        = 723, /* no such element */
    XMLERR_SOAP_DUPL_LANG         = 724, /* duplicate language */


    /* --- Errors 750-799 are for WebDAV errors */
    XMLERR_DAV_NUL_PTR            = 750, /* NULL argument pointer */
    XMLERR_DAV_EXC_ARGS           = 751, /* exclusive arguments */
    XMLERR_DAV_INV_VAL            = 752, /* invalid value */
    XMLERR_DAV_INV_PRX            = 753, /* invalid proxy */
    XMLERR_DAV_INV_NO_PRX         = 754, /* invalid no-proxy */
    XMLERR_DAV_NO_REQ_ARG         = 755, /* missing required argument */
    XMLERR_DAV_NO_MIX_TAG         = 756, /* all tagged or all untagged */
    XMLERR_DAV_MAX_XML_SIZ        = 757, /* max xml size exceeded */
    XMLERR_DAV_CB_RET_NUL         = 758, /* callback resturns NULL */
    XMLERR_DAV_TCP_INIT           = 759, /* TCP/IP initialization failed */
    XMLERR_DAV_TCP_OPEN           = 760, /* failed to open TCP connection */
    XMLERR_DAV_TCP_SEND           = 761, /* failed to send TCP data */
    XMLERR_DAV_TCP_READ           = 762, /* failed to read TCP data */
    XMLERR_DAV_MAX_CONS           = 763, /* max connections exceeded */
    XMLERR_DAV_BAD_CONN           = 764, /* connection broken */
    XMLERR_DAV_HTTP_PROTO         = 765, /* malformed HTTP header */
    XMLERR_DAV_BAD_TRAN_ENC       = 766, /* bad transfer encoding */
    XMLERR_DAV_NOT_TEXT           = 767, /* not text media type */
    XMLERR_DAV_NO_BODY            = 768, /* no entity body */
    XMLERR_DAV_NOT_XML            = 769, /* not xml media type */
    XMLERR_DAV_NO_LOK_TOK         = 770, /* missing lock token in lock refresh
                                          */
    XMLERR_DAV_NO_LT_UNLOCK       = 771, /* missing lock token in UNLOCK */
    XMLERR_DAV_OBJ_TYPE           = 772, /* wrong object type */
    XMLERR_DAV_BAD_PROT           = 773, /* protocol violation */

    /* --- Errors 800-849 are XQuery Parser return codes */
    XMLERR_XQ_INVTOKEN            = 800, /* invalid XQuery token */
    XMLERR_XQ_SYNTAXERR           = 801, /* XQuery syntax error */

    /*  Erros 900-950 are for Xmldiff */
    XMLERR_DF_ILLEGAL_FLAGS       = 903, /* illegal flag bits for XmlDiff() */
    XMLERR_DF_ILLEGAL_INPUTS      = 904, /* illegal sources for XmlDiff() */
    XMLERR_DF_ILLEGAL_INSERT      = 905, /* illegal context while inserting */
    XMLERR_DF_ILLEGAL_OP          = 906, /* illegal context while inserting */
    XMLERR_DF_ILLEGAL_STATE       = 907, /* problem with FSM */
    XMLERR_DF_ILLEGAL_DMATRIX     = 908, /* inconsistency in distance matrix */
    XMLERR_DF_RENAME_FAIL         = 909, /* rename node failed in patch */
    XMLERR_DF_ROOT_MISMATCH       = 910, /* root nodes do not match */
    XMLERR_DF_DHASH_ERR           = 911, /* error in calculating domhash */
    XMLERR_DF_MANY_NODES          = 912, /* error in calculating domhash */
    XMLERR_PA_PI_FAIL             = 913, /* oracle-diff PI does not assert
                                            values correctly to be able to
                                            patch */
    XMLERR_PA_IMP_FAIL            = 914, /* dom copy/import failed */
    XMLERR_DF_UNKNOWN_NODE        = 915, /* node type not handled */
    XMLERR_DF_NSMAP_ERR           = 916, /* inconsistent NS maps */
    XMLERR_PA_XVM_ERR             = 917, /* error invoking xvm */
    XMLERR_PA_XPTRANS_ERR         = 918, /* cant translate xpath while patching
                                         */

    /* Errors 950-960 are for JSON, currenlty only used for asserts */
    XMLERR_JSON_GENERAL           = 950,

    XMLERR_DF_INTERNAL            = 999, /* illegal context while inserting */

    /* --- Last error --- */

    XMLERR_LASTERROR              = 9999

} xmlerr;

#define XMLERR_LPU_INIT XMLERR_URL_INIT

#endif /* ifndef ORAXML_ORACLE */

#endif /* XMLERR_ORACLE */

