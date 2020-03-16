
/* Copyright (c) 2004, 2018, Oracle and/or its affiliates. 
All rights reserved.*/

/*    NAME
     xmlurl.h - Xml URL parser/loader public header

   DESCRIPTION
     URL-based APIs.  Provides several function groups:
        (1) parsing, composition, resolution
        (2) one-shot data loading
        (3) session (multi-connection) data loading.

     This package is designed to work with URLs.  That is, it wants to
     actually load data.  You can parse a URIs since it has the same
     syntax as a URL, but you cannot load data from it.  It is just an
     identifier, NOT a locator.  Where the matching data is actually
     located is anyone's guess.  You need a URL to locate it.

   URL vs URI
     There is much confusion about "URL" vs "URI".  Both have the same
     syntax; in fact, both look identical.  But the 'L' in URL stands
     for LOCATOR, meaning you can use the URL to actually locate and
     retrieve data.  A URI is just an identifier, it locates nothing.

        A common mistake is choosing URIs that look like URLs,
        down to the access method.

        The URL "http://foo.com/bar.txt" means you can use HTTP
        to retrieve file bar.txt from host foo.com.  A locator!

        But the URI "http://foo.com/bar.txt" is simply an identifier,
        a unique sequence of characters MASQUERADING AS A URL!

        You CAN'T go to host foo.com; it doesn't necessarily exist.
        If did does, it doesn't necessarily talk HTTP, or have file
        bar.txt.

        This is why your URIs should not be made to look like URLs:
        because you cannot tell them apart and it's very confusing.

   ENCODING
     URLs have a very restricted character set, see RFC-2396.  Not good
     for Internationalization.  What's coming is IRIs, which are like URIs
     but encoded in UTF-8 instead.  The IRI spec is still in draft stage,
     but the basic idea of a UTF-8 encoded URL we have adopted.  Note as
     IRI is to URI, IRL is to URL.

   RELATED DOCUMENTS
     See RFC-2396, "Uniform Resource Identifiers (URI): Generic Syntax"

     See "Internationalized Resource Identifiers (IRIs)",
        <http://www.ietf.org/internet-drafts/draft-duerst-iri-05.txt>
     (as of Jan 23, 2004; IRI is being updated regularly at this time)

   PARSER FUNCTIONS
     This set of function deals with URL syntax; no context is needed.

     XmlUrlPath     - turn URL into OSD path
     XmlUrlParse    - parse URL into components
     XmlUrlParseIrl - parse IRL into components

     XmlUrlSimplify - simplify URL/IRL (reduce path to canonical form)
     XmlUrlDecode   - decode URL %FF-style encodings in place
     XmlUrlEncode   - encode URL %FF-style to buffer
     XmlUrlResolve  - resolve relative URL/IRI reference
     XmlUrlCompose  - compose URL/IRI from components

     DEPRECATED: wrong name maintained for backwards compatability
     XmlUrlParseiri - parse IRL

   ONE-SHOT PUBLIC LOADER FUNCTIONS
     This set of functions forms a one-shot loader, used to perform
     "wget"-like action.  A context is needed, but it can only support a
     single read at a time.  For multiple simultaneous reads, see the
     next function group.

     XmlUrlInit     - initialize LPU in one-shot mode & return context
     XmlUrlTerm     - terminate an LPU context
     XmlUrlOpen     - open URL for reading
     XmlUrlOpenIrl  - open IRL for reading
     XmlUrlRead     - read data from open URL
     XmlUrlClose    - close open URL
     XmlUrlError    - return error response (after XmlUrlOpen)

     DEPRECATED: wrong name maintained for backwards compatability
     XmlUrlOpeniri  - open IRL

   MULTI-CONNECTION APIs
     This set forms a persist-use session.  A context is created once at
     initialization time with proxy/no-proxy data and other parameters.
     Then to load data, a connection object is made based on the context
     and a URL.  After reading, the connection objects is destroyed.  Any
     number of simultaneous connections may be open using a single context.

     Note that the same termination function XmlUrlTerm() is used to destroy
     contexts created with both XmlUrlInit() and XmlUrlInitCon().

     XmlUrlInitCon    - initialize LPU in multi-connection mode & return context
     XmlUrlMakeCon    - make a new connection object
     XmlUrlTermCon    - terminate a connection object
     XmlUrlHandleCon  - return inputs handles for connection object
     XmlUrlOpenCon    - open connection
     XmlUrlReadCon    - read data from open connection
     XmlUrlCloseCon   - close open connection
     XmlUrlErrorc     - get error message for connection
     XmlUrlFlushCon   - flush connection buffer
     XmlUrlHeaderCon  - set additional HTTP header value

   ACCESS-METHOD HOOKS
     URLs specify the access method which is to be used to retrieve the
     data; it's the first thing in the URL, ending with colon (':').
     Each method has a set of hooks to open, read, and close.  LPU has
     built-in hooks for file, HTTP, and FTP, however all methods' hooks
     can be replaced with user functions.

     *********** DEPREPCATED DO NOT USE THIS FUNCTIONS ***********
     XmlUrlGetCallback    - DEPRECATED get access-method callbacks
     XmlUrlSetCallback    - DEPRECATED set access-method callbacks

     ********* THESE ARE THE NEW orastream-based FUNCTIONS *******
     NOTE: orastream hook SIDs will be replaced with the components
     (xmlurlpart) for the connection!

     XmlUrlGetStream     - get orastream for access-method (context-wide)
     XmlUrlGetStreamc    - get orastream for access-method (per-connection)
     XmlUrlSetStream     - set orastream for access-method (context-wide)
     XmlUrlSetStreamCon  - set orastream for access-method (per-connection)

   MODIFIED   (MM/DD/YY)
   dmcmahon    06/12/18 - Bug 28173446: break dependency on s.h/lml.h
   dmcmahon    08/29/16 - Bug 24561442: restore XMLURLACCESS_MAX
   dmcmahon    06/26/16 - Bug 23705085 - back out XMLURLACCESS_MAX
   haiswang    06/21/16 - parfait issue for access size
   dmcmahon    03/30/16 - Fix incorrect property "http_no_proxy"
   dmcmahon    11/07/15 - Bug 22169571: add XmlUrlSetStreamFlag
   mjaeger     07/08/15 - bug 20604110: handle proxy error
   haiswang    02/06/15 - bug19459392: HTTPS access needs extend MAX
   vmedi       08/30/10 - bug 10036116 fix
   vmedi       03/28/06 - bugfix 4200153
   imacky      07/09/04 - imacky_loader
   imacky      06/30/04 - Creation
*/

#ifndef XMLURL_ORACLE
# define XMLURL_ORACLE

#ifndef ORATYPES_ORACLE
# include <oratypes.h>
#endif

#ifndef ORASTRUC_ORACLE
# include <orastruc.h>  /* for oramemctx */
#endif

/*---------------------------------------------------------------------------
                     PUBLIC TYPES AND CONSTANTS
  ---------------------------------------------------------------------------*/

/* forward declare (opaque to user) */

typedef struct xmlurlctx xmlurlctx;             /* LPU top-level context */
typedef struct xmlurlcon xmlurlcon;             /* LPU connection object */

#define XMLURL_MAX_RI   2048            /* max size of Resource Identifier */
#define XMLURL_MAX_URI     XMLURL_MAX_RI        /* synonyms */
#define XMLURL_MAX_URL     XMLURL_MAX_RI
#define XMLURL_MAX_IRI     XMLURL_MAX_RI
#define XMLURL_MAX_IRL     XMLURL_MAX_RI

#define XMLURL_MIN_BUF  64              /* Minimum data buffer size */

/*---------------------------------------------------------------------------
                            PATH LOCALIZATION

   The different systems for which XmlUrlPath knows how to turn URL paths
   into OSD filesystem paths.  Note that any OS's path can be asked for
   regardless of the actual executing OS, so the path formation code is
   portable and not OSD branched.

  ---------------------------------------------------------------------------*/

typedef enum {                          /* given URL "foo/bar/baz.txt": */
    XMLURLOS_CURRENT = 0,               /* same as the executing machine */
    XMLURLOS_UNIX    = 1,               /* "foo/bar/baz.txt" */
    XMLURLOS_VMS     = 2,               /* "[.foo.bar]baz.txt" */
    XMLURLOS_WINDOWS = 3                /* "foo\bar\baz.txt" */
} xmlurlos;

#if defined(VMS)
# define XMLURLOS_DEFAULT       XMLURLOS_VMS
#elif defined(WIN32COMMON)
# define XMLURLOS_DEFAULT       XMLURLOS_WINDOWS
#else
# define XMLURLOS_DEFAULT       XMLURLOS_UNIX
#endif

/* -----------------------------------------------------------------------
                           Error Codes
   ----------------------------------------------------------------------- */

#define XMLURLERR_OK            0       /* success code */

#define XMLURLERR_NULL_PTR      1       /* null pointer */
#define XMLURLERR_BAD_CTX       2       /* invalid xmlurl context */
#define XMLURLERR_PARSE_FAILED  3       /* failed to parse URL */
#define XMLURLERR_NO_HOST       4       /* no host specified in URL */
#define XMLURLERR_NO_ACCESS     5       /* can't handle access type */
#define XMLURLERR_BAD_ACCESS    6       /* invalid access code */
#define XMLURLERR_NOT_OPEN      7       /* tried to close when not open */
#define XMLURLERR_BIG_ENCODING  8       /* encoding name is too large */
#define XMLURLERR_BAD_PROXY     9       /* bad proxy configuration */
#define XMLURLERR_NOT_ABS       10      /* URL must be absolute */
#define XMLURLERR_BAD_URL       11      /* invalid URL */
#define XMLURLERR_OUT_OF_MEMORY 12      /* memory allocation failed */
#define XMLURLERR_BUF_TOO_SMALL 13      /* provided data buffer too small */
#define XMLURLERR_INTERNAL      14      /* internal error */
#define XMLURLERR_BAD_PROPERTY  15      /* no such property */
#define XMLURLERR_NO_LXGLO      16      /* an NLS global area is required */
#define XMLURLERR_NO_UTF8       17      /* couldn't get UTF-8 lx_langid */
#define XMLURLERR_NO_LML        18      /* couldn't allocate LML context */
#define XMLURLERR_NO_RI         19      /* no URL/IRL given to XmlUrlMakeCon */
#define XMLURLERR_ARG_CONFLICT  20      /* conflicting arguments */
#define XMLURLERR_BUF_OVERFLOW  21      /* buffer overflow */
#define XMLURLERR_BAD_SOCKET    22      /* invalid socket */
#define XMLURLERR_BAD_HTTP_PROXY 23     /* bad configuration of http_proxy */
#define XMLURLERR_BAD_NO_PROXY  24      /* bad configuration of no_proxy */

#define XMLURLERR_TCP_INIT      100     /* TCP/IP initialization failed */
#define XMLURLERR_TCP_OPEN      101     /* failed to open TCP connection */
#define XMLURLERR_TCP_SEND      102     /* failed to send TCP data */
#define XMLURLERR_TCP_READ      103     /* failed to read TCP connection */

#define XMLURLERR_FILE_OPEN     200     /* failed to open file */
#define XMLURLERR_FILE_READ     201     /* failed to read from file */

#define XMLURLERR_HTTP_PROTO    300     /* HTTP protocol error */
#define XMLURLERR_HTTP_NO_REDIR 301     /* redirection not allowed */
#define XMLURLERR_HTTP_MAX_REDIR 302    /* maximum redirections exceeded */
#define XMLURLERR_HTTP_INFO     303     /* can't handle Informational (1xx) */
#define XMLURLERR_HTTP_CLIENT   304     /* client request error (4xx) */
#define XMLURLERR_HTTP_SERVER   305     /* misc server problem (5xx) */
#define XMLURLERR_HTTP_NOT_FOUND 306    /* page not found (505) */
#define XMLURLERR_HTTP_MIME     307     /* bad Mime header */
#define XMLURLERR_HTTP_MAX_HEADER 308   /* too many headers added */

#define XMLURLERR_FTP_LOGIN     400     /* FTP login failed */
#define XMLURLERR_FTP_UNAVAIL   401     /* FTP server unavailable */
#define XMLURLERR_FTP_PROTO     402     /* FTP general protocol failure */

/* -----------------------------------------------------------------------
                           Access Codes
   ----------------------------------------------------------------------- */

typedef enum {
    XMLURLACCESS_BAD =   -1,               /* invalid URL */
    XMLURLACCESS_NONE =   0,               /* not specified */
    XMLURLACCESS_UNK =    1,               /* specified but unknown */
    XMLURLACCESS_FILE =   2,               /* filesystem access */
    XMLURLACCESS_HTTP =   3,               /* HTTP */
    XMLURLACCESS_FTP =    4,               /* FTP */
    XMLURLACCESS_GOPHER = 5,               /* Gopher */
    XMLURLACCESS_ORADB =  6,               /* Oracle DB */
/* #### STREAM ACCESS IS DEPRECATED - SEE XmlUrlSetStream() FOR NEW API #### */
    XMLURLACCESS_STREAM = 7,               /* user-defined stream */
    XMLURLACCESS_HTTPS  = 8                /* Secure HTTP */
} xmlurlaccess;

/* bug19459392: the access max below might need to extend, since there is a
 * over limit enum as 8, XMLURLACCESS_HTTPS. In order to make HTTPS as a valid
 * value for stream mapping from con, the extension might be necessary and
 * a copy change in lpu.h is also needed, lpuaccess. ####
 */
#define XMLURLACCESS_MAX   8            /* highest LPUACCESS code */

/* -----------------------------------------------------------------------
                           URL broken into Parts
   ----------------------------------------------------------------------- */

typedef struct xmlurlpart {
    xmlurlaccess access_xmlurlpart;     /* XMLURLACCESS_xxx code */
    oratext  *accbuf_xmlurlpart;        /* access name */
    oratext  *host_xmlurlpart;          /* host name */
    oratext  *dir_xmlurlpart;           /* directory */
    oratext  *file_xmlurlpart;          /* filename */
    oratext  *uid_xmlurlpart;           /* userid (username) */
    oratext  *passwd_xmlurlpart;        /* password */
    oratext  *port_xmlurlpart;          /* port */
    oratext  *frag_xmlurlpart;          /* fragment */
    oratext  *query_xmlurlpart;         /* query */
    oratext  *param_xmlurlpart;         /* parameter */
    ub2       portnum_xmlurlpart;       /* port as a # */
    ub1       abs_xmlurlpart;           /* absolute path reference? */
} xmlurlpart;

/* -----------------------------------------------------------------------
                              Input Handle Union
   ----------------------------------------------------------------------- */

typedef union xmlurlihdl xmlurlihdl;

/*---------------------------------------------------------------------------
                           DYNAMIC POST CALLBACK
  ---------------------------------------------------------------------------*/

#define XMLURL_POST_BODYF(func, con, ctx, buf, bufsiz) \
    ubig_ora func(xmlurlcon *con, void *ctx, oratext *buf, ubig_ora bufsiz)

/* -----------------------------------------------------------------------
        _                                               _                _
     __| |   ___   _ __    _ __    ___    ___    __ _  | |_    ___    __| |
    / _` |  / _ \ | '_ \  | '__|  / _ \  / __|  / _` | | __|  / _ \  / _` |
   | (_| | |  __/ | |_) | | |    |  __/ | (__  | (_| | | |_  |  __/ | (_| |
    \__,_|  \___| | .__/  |_|     \___|  \___|  \__,_|  \__|  \___|  \__,_|
                  |_|

                             I/O callback functions

       DEPRECATED -- DO NOT USE THIS FUNCTIONS -- USE orastream INSTEAD

   ----------------------------------------------------------------------- */

#define XMLURL_OPENF(func) \
    uword func(void *ctx, oratext *path, xmlurlpart *parts, \
                ubig_ora *length, xmlurlihdl *ih)

#define XMLURL_CLOSEF(func) \
    uword func(void *ctx, xmlurlihdl *ih)

#define XMLURL_READF(func) \
    uword func(void *ctx, xmlurlihdl *ih, oratext **data, \
                ubig_ora *nraw, ub1 *eoi)

typedef struct xmlurlcb {
    void *ctx_xmlurlcb;                 /* user-defined callback context */
    XMLURL_OPENF((*open_xmlurlcb));     /* open input */
    XMLURL_CLOSEF((*close_xmlurlcb));   /* read from input */
    XMLURL_READF((*read_xmlurlcb));     /* close input */
} xmlurlcb;

/*---------------------------------------------------------------------------
                           PUBLIC FUNCTIONS
  ---------------------------------------------------------------------------*/

/*
 * NAME:
 *   XmlUrl64Encode - return Base64 encoded input string
 * PARAMETERS:
 *   dest (OUT) - destination buffer (make sure it's big enough)
 *   src  (IN)  - source buffer
 * RETURNS:
 *   (void)
 * DESCRIPTION:
 *   Does Base64 encoding of src to dst, per RFC-2046
 */
void XmlUrl64Encode(oratext *dst, oratext *src);

/*
 * NAME:
 *   XmlUrlPath - return localized path part of URL
 * PARAMETERS:
 *   url   (IN)  - incoming URL
 *   path  (OUT) - destination buffer
 *   os    (IN)  - OS to xform to (0 for current system)
 * RETURNS:
 *   (oratext *) localized path or NULL on error
 * DESCRIPTION:
 *   Change a URL path into an OS path for the given OS.  This is a
 *   portable function, and any OS's path may be generated on any
 *   other OS.  Use XMLURLOS_CURRENT to select the executing OS.
 */
oratext *XmlUrlPath(oratext *url, oratext *path, xmlurlos os);

/*
 * NAME:
 *   XmlUrlParse - parse an URL into parts
 * PARAMETERS:
 *   url   (IN)  - incoming URL
 *   work  (I/O) - work buffer where URL is split apart
 *   parts (I/O) - structure of pointers to URL parts
 * RETURNS:
 *   (boolean) success
 * DESCRIPTION:
 *   Split <url> in parts, setting <parts>' pointers to the components.
 *   See the definition of xmlurlpart in xmlurl.h for the breakdown.
 *   Working buffer should be XMLURL_MAX_URL bytes.
 */
boolean XmlUrlParse(oratext *url, oratext *work, xmlurlpart *part);

/*
 * NAME:
 *   XmlUrlParseIrl - parse a IRL into parts
 * PARAMETERS:
 *   irl   (IN)  - incoming IRL
 *   work  (I/O) - work buffer where IRL is split apart
 *   parts (I/O) - structure of pointers to IRL parts
 * RETURNS:
 *   (boolean) success
 * DESCRIPTION:
 *   Split <irl> in parts, setting <parts>' pointers to the components.
 *   See the definition of xmlurlpart in xmlurl.h for the breakdown.
 */
boolean XmlUrlParseIrl(oratext *irl, oratext *work, xmlurlpart *part);

/*
 * NAME:
 *   XmlUrlCompose - turns parts into final URL
 * PARAMETERS:
 *   url  (IN)  - URL to recompose
 *   dest (OUT) - destination buffer (should be XMLURL_MAX_URL in size)
 * RETURNS:
 *   (boolean) - TRUE on success
 * DESCRIPTION:
 *   Turns an xmlurlpart struct into a full URL in the given destination buffer
 */
boolean XmlUrlCompose(xmlurlpart *url, oratext *dest);

/*
 * NAME:
 *   XmlUrlCopy - copys an xmlurlpart
 * PARAMETERS:
 *   destpart (OUT) - destination xmlurlpart
 *   destwork (OUT) - destination working buffer
 *   srcpart  (IN)  - source xmlurlpart
 * RETURNS:
 *   (boolean) - TRUE on success
 * DESCRIPTION:
 *   Copies an xmlurlpart and its data to a new xmlurlpart/buffer.
 *   New working buffer should be XMLURL_MAX_URL bytes.
 */
boolean XmlUrlCopy(xmlurlpart *destpart, oratext *destwork, xmlurlpart *srcpart);

/*
 * NAME:
 *   XmlUrlSimplify - simplify URL paths
 * PARAMETERS:
 *   url (I/O) - incoming URL
 * RETURNS:
 *   none
 * DESCRIPTION:
 *   simplifies an URL by removing "../foo" and "/./" sequences.  the URL
 *   is modified in place, rewriting the old URL, since all operations
 *   shrink the URL
 * NOTES:
 *   A URL is allowed to contain the sequence "xxx/../" which may be replaced
 *   by "", and the sequence "/./" which may be replaced by "/".
 *   Simplification helps us recognize duplicate URLs.
 * EXAMPLES:
 *      /etc/junk/../fred               /etc/fred
 *      /etc/junk/../../fred            /fred
 *      /etc/junk/./fred                /etc/junk/fred
 *      /etc/junk/././fred              /etc/junk/fred
 *      http://fred.xxx.edu/../..       <unchanged>
 *      ../../albert.html               <unchanged>
 *   In order to avoid empty URLs the following URLs become:
 *      /fred/..                        /
 *      /fred/././..                    /
 *      /fred/.././junk/.././           /
 *   If more than one set of `://' is found (several proxies in cascade)
 *   then only the part after the last `://' is simplified.
 */
void XmlUrlSimplify(oratext *url);

/*
 * NAME:
 *   XmlUrlResolve - resolve relative URL reference
 * PARAMETERS:
 *   base (IN)  - base URL
 *   rel  (IN)  - relative URL
 *   res  (OUT) - resolved URL
 *   work (IN)  - working buffer
 * RETURNS:
 *   (boolean) - TRUE on success
 * DESCRIPTION:
 *   Resolves a relative URL reference (against a base URL).  See appendix
 *   C of RFC-2396 for discussion.
 * EXAMPLES:
 *   With a base URL of "http://a/b/c/d;p?q", the following relative URLs
 *   will be resolved as shown:
 *
 *      Relative URL    Resolution
 *      g:h             g:h
 *      g               http://a/b/c/g
 *      ./g             http://a/b/c/g
 *      g/              http://a/b/c/g/
 *      /g              http://a/g
 *      //g             http://g
 *      ?y              http://a/b/c/?y
 *      g?y             http://a/b/c/g?y
 *      #s              (current document)#s
 *      g#s             http://a/b/c/g#s
 *      g?y#s           http://a/b/c/g?y#s
 *      ;x              http://a/b/c/;x
 *      g;x             http://a/b/c/g;x
 *      g;x?y#s         http://a/b/c/g;x?y#s
 *      .               http://a/b/c/
 *      ./              http://a/b/c/
 *      ..              http://a/b/
 *      ../             http://a/b/
 *      ../g            http://a/b/g
 *      ../..           http://a/
 *      ../../          http://a/
 *      ../../g         http://a/g
 */
boolean XmlUrlResolve(xmlurlpart *base, xmlurlpart *rel, xmlurlpart *res, oratext *work);

/*
 * NAME:
 *   XmlUrlDecode - decode hex encoding
 * PARAMETERS:
 *   str (I/O) - the string to be decoded
 * RETURNS:
 *   str (OUT) - returns str back again
 * DESCRIPTION:
 *   This function rewrites the given string in place, decoding all the
 *   hex-encoded %FF style character sequences and replacing each with
 *   the intended character.
 * NOTES:
 *   We can decode the string in place since we know it's going to be
 *   shrinking.  Each 3-character sequence '%' 'X' 'X' is replaced by
 *   the single character it represents.  The final, shorter string is
 *   null-terminated.  Note that only printing characters (accord to
 *   lchisprint) are converted.  Other 3-character patterns are invalid,
 *   but left as-is.
 */
oratext *XmlUrlDecode(oratext *str);

/*
 * NAME:
 *   XmlUrlEncode - hex-encode URL
 * PARAMETERS:
 *   url    (IN)  - URL to encode (single or multibyte, but not Unicode)
 *   buf    (OUT) - destination buffer
 *   buflen (IN)  - size of dest buffer
 * RETURNS:
 *   (oratext *) encoded URL
 * DESCRIPTION:
 *   This function encodes a URL into a destination buffer.  It performs
 *   the opposite function to XmlUrlDecode.
 */
oratext *XmlUrlEncode(oratext *url, oratext *buf, size_t buflen);

/*---------------------------------------------------------------------------
                           PUBLIC LOADER FUNCTIONS
  ---------------------------------------------------------------------------*/

/*
 * NAME:
 *   XmlUrlInit - initialize an LPU context
 * PARAMETERS:
 *   memctx   (IN)  - context for memory allocation (NULL for malloc/free)
 *   open_to  (IN)  - open connection timeout (in ms)  [NOT USED CURRENTLY]
 *   read_to  (IN)  - read timeout (in ms)  [NOT USED CURRENTLY]
 *   maxredir (IN)  - maximum number of redirections before fail (default=5)
 *   err      (OUT) - returned error code (on failure)
 * RETURNS:
 *   (xmlurlctx *) created context, NULL on failure (with err set)
 * DESCRIPTION:
 *    Creates and returns a new LPU context, in preparation for reading data.
 */
xmlurlctx *XmlUrlInit(oramemctx *memctx, ub4 open_to,
                      ub4 read_to, ub2 maxredir, uword *err);

/*
 * NAME:
 *   XmlUrlTerm - terminate an LPU context
 * PARAMETERS:
 *   ctx (IN) - LPU context
 * RETURNS:
 *   (void)
 * DESCRIPTION:
 *    Terminate an LPU context and frees any allocated memory, including
 *    the context itself.
 */
void XmlUrlTerm(xmlurlctx *ctx);

/*
 * NAME:
 *   XmlUrlError - return HTTP server error message
 * PARAMETERS:
 *   ctx      (IN)  - LPU context
 * RETURNS:
 *   (oratext *) pointer to error message buffer
 * DESCRIPTION:
 *    Returns a pointer to the error message buffer.  After an unsuccessful
 *    XmlUrlOpen() of an HTTP UTL, this buffer will contain the 3-digit error
 *    code and information message returned by the HTTP server.
 */
oratext *XmlUrlError(xmlurlctx *ctx);

/*
 * NAME:
 *   XmlUrlOpen - open URL for reading
 * PARAMETERS:
 *   ctx      (IN)  - LPU context
 *   url      (IN)  - URL to open
 *   parts    (IN)  - if URL is already parsed, the xmlurlpart
 *   buffer   (IN)  - data buffer
 *   buflen   (IN)  - size of data buffer
 * RETURNS:
 *   (uword) error code, 0 on success
 * DESCRIPTION:
 *    Opens the given URL for reading.  The given buffer will be used for
 *    all subsequent read operations.
 */
uword XmlUrlOpen(xmlurlctx *ctx, oratext *url, xmlurlpart *parts,
              oratext *buffer, ubig_ora buflen);

/*
 * NAME:
 *   XmlUrlOpenIrl - open IRL for reading
 * PARAMETERS:
 *   ctx      (IN)  - LPU context
 *   irl      (IN)  - IRL to open
 *   parts    (IN)  - if IRI is already parsed, the xmlurlpart
 *   buffer   (IN)  - data buffer
 *   buflen   (IN)  - size of data buffer
 * RETURNS:
 *   (uword) error code, 0 on success
 * DESCRIPTION:
 *    Opens the given IRL for reading.  The given buffer will be used for
 *    all subsequent read operations.
 */
uword XmlUrlOpenIrl(xmlurlctx *ctx, oratext *irl, xmlurlpart *parts,
                 oratext *buffer, ubig_ora buflen);

/*
 * NAME:
 *   XmlUrlRead - read data from open URL
 * PARAMETERS:
 *   ctx      (IN)  - LPU context
 *   data     (OUT) - returned data
 *   nread    (OUT) - number of bytes actually read
 *   eoi      (OUT) - End of Information flag; if set, no more data follows
 * RETURNS:
 *   (uword) error code, 0 on success
 * DESCRIPTION:
 *    Reads data from open URL into the buffer specified in XmlUrlOpen.  Note
 *    that the returned data pointer will usually be to the head of this
 *    buffer, except for the very first HTTP read.  In the HTTP case, the
 *    first data returned from the HTTP server includes an HTTP header
 *    (which is read for the encoding information).  For the first read,
 *    the data pointer will point into the already-read buffer, after the
 *    HTTP header, at the beginning of the actual data.
 */
uword XmlUrlRead(xmlurlctx *ctx, oratext **data, ubig_ora *nread, ub1 *eoi);

/*
 * NAME:
 *   XmlUrlClose - close open URL
 * PARAMETERS:
 *   ctx (IN) - LPU context
 * RETURNS:
 *   (uword) error code, 0 on success
 * DESCRIPTION:
 *    Close open URL
 */
uword XmlUrlClose(xmlurlctx *ctx);

/*
 * NAME:
 *   XmlUrlSetCallback - set access callbacks
 * PARAMETERS:
 *   ctx     (IN) - LPU context
 *   access  (IN) - access method code
 *   userctx (IN) - user-defined context
 *   openf   (IN) - open function
 *   closef  (IN) - close function
 *   readf   (IN) - read function
 * RETURNS:
 *   (uword) error code, 0 on error
 * DESCRIPTION:
 *    Set I/O access callback functions
 */
uword XmlUrlSetCallback(xmlurlctx *ctx, xmlurlaccess access, void *userctx,
                        XMLURL_OPENF((*openf)), XMLURL_CLOSEF((*closef)),
                        XMLURL_READF((*readf)));

/*
 * NAME:
 *   XmlUrlGetCallback - get access callbacks
 * PARAMETERS:
 *   ctx    (IN) - LPU context
 *   access (IN) - access code
 * RETURNS:
 *   (xmlurlcb *) pointer to I/O callback structure
 * DESCRIPTION:
 *    Opens the given URL (specified as xmlurlpart) for reading.
 */
xmlurlcb *XmlUrlGetCallback(xmlurlctx *ctx, xmlurlaccess access);

/*
 * NAME:
 *   XmlUrlInitCon - initialize LPU in multi-connection mode
 *   XmlUrlInitConVA - same but takes va_list instead of ...
 *
 * PARAMETERS:
 *   err    (OUT) - error code
 *   ...    (IN)  - additional name, value pairs plus final NULL
 *
 * RETURNS:
 *   (xmlurlctx *) LPU context or NULL w/err set
 *
 * DESCRIPTION:
 *    Create a new LPU context in multi-connection mode.  Following the
 *    error return argument are any number of name, value pairs, with a
 *    final NULL at the end.  Note the NULL is required.  XmlUrlInitConVA is
 *    the same function but taking the variable args as a va_list instead.
 *
 * PROPERTIES:
 *    "debug_level"             enables debug output; the higher the
 *                              value the more detail; range is 1-3
 *
 *    "nls_global_area"         specifies NLS global area (lxglo) to
 *                              use.  if not specified, a new one will
 *                              be made if necessary.
 *
 *    "mem_context"             specifies the OraMem context used
 *                              for memory allocations
 *
 *    "open_timeout"            TCP/IP open timeout in ms [not used]
 *
 *    "read_timeout"            TCP/IP read timeout in ms [not used]
 *
 *    "http_max_redirects"      HTTP maximum number of redirects before
 *                              giving up (default is 5).
 *
 *    "http_proxy"              proxy for HTTP connections.  if not
 *                              provided, the environment variable
 *                              of the same name is used.
 *
 *    "http_no_proxy"           comma- or space-separated list of domains
 *                              NOT to apply http_proxy to.  if not
 *                              provided, the environment variable "no_proxy"
 *                              is used.
 *
 * EXAMPLE:
 *    xmlurlctx *ctx;
 *    uword   err;
 *    ctx = XmlUrlInitCon(&err, "http_max_redirects", 10, NULL);
 */
xmlurlctx *XmlUrlInitCon(oraerr *err, ...);
xmlurlctx *XmlUrlInitConVA(oraerr *err, va_list va);

/*
 * NAME:
 *   XmlUrlMakeCon - make connection object
 *   XmlUrlMakeConVA - same but takes va_list instead of ...
 *
 * PARAMETERS:
 *   ctx (IN)  - LPU multi-connection context
 *   err (OUT) - access code
 *   ... (IN)  - additional name, value pairs plus final NULL
 *
 * RETURNS:
 *   (xmlurlcon *) new connection object or NULL w/err set
 *
 * DESCRIPTION:
 *    Creates and returns a new connection object.
 *
 * PROPERTIES:
 *    "base_url"        Base URL used for resolving relative URLs against
 *    "base_irl"        Same but as IRL
 *    "base_parts"      Same but specified as xmlurlpart
 *
 *    "buffer"          I/O buffer
 *    "buffer_size"     Size in bytes of I/O buffer
 *
 *    "charset"         Character set of data [Mime/IANA]
 *    "mime_type"       Mime type of data
 *
 *    "debug_level"     Enables additional debug output for this connection
 *                      only.  Range is 1-3, higher number gives more detail.
 *
 *    "irl"             Endpoint of connection, as IRL, URL, or xmlurlpart.
 *    "parts"
 *    "url"
 */
xmlurlcon *XmlUrlMakeCon(xmlurlctx *ctx, uword *err, ...);
xmlurlcon *XmlUrlMakeConVA(xmlurlctx *ctx, uword *err, va_list va);

/*
 * NAME:
 *   XmlUrlTermCon - terminate a connection
 *
 * PARAMETERS:
 *   con (I/O) - LPU connection object
 *
 * RETURNS:
 *   (uword) error return
 *
 * DESCRIPTION:
 *    Destroys a connection object.  To destroy the context, use XmlUrlTerm().
 */
uword XmlUrlTermCon(xmlurlcon *con);

/*
 * NAME:
 *   XmlUrlHandleCon - return handles for connection
 *
 * PARAMETERS:
 *   con (I/O) - LPU connection object
 *
 * RETURNS:
 *   (xmlurlihdl *) handles
 *
 * DESCRIPTION:
 *    Returns the union structure containing connection handles
 */
xmlurlihdl *XmlUrlHandleCon(xmlurlcon *con);

/*
 * NAME:
 *   XmlUrlOpenCon - open a connection
 *   XmlUrlOpenConVA - same but takes va_list instead of ...
 *
 * PARAMETERS:
 *   con (I/O) - LPU connection object
 *
 * RETURNS:
 *   (uword) error return
 *
 * DESCRIPTION:
 *    Opens a connection object (i.e. actually makes the connection).
 *
 * PROPERTIES:
 *   "post_body"                When opening an HTTP connection, if post_body
 *   "post_body_len"            or post_body_stream is supplied, POST will be
 *   "post_body_stream"         used instead of GET.  post_body and len give
 *                              a static POST body; alternately a stream may
 *                              be used to dynamically provide the POST body
 *                              at open time.
 */
uword XmlUrlOpenCon(xmlurlcon *con, ...);
uword XmlUrlOpenConVA(xmlurlcon *con, va_list va);

/*
 * NAME:
 *   XmlUrlReadCon - read data from connection
 *
 * PARAMETERS:
 *   con   (I/O) - LPU connection object
 *   data  (OUT) - buffer (IN) - updated to start of data (OUT)
 *   nread (OUT) - number of bytes read
 *   eoi   (OIT) - last data?
 *
 * RETURNS:
 *   (uword) error return
 *
 * DESCRIPTION:
 *    Reads data from a connection object.  data is set to the start of
 *    the actual data in the connection's I/O buffer [perhaps skipping
 *    over some first-buffer protocol overhead]; nread is the matching
 *    actual number of data bytes; eoi indicates if this is the last
 *    block of data (End of Information).
 */
uword XmlUrlReadCon(xmlurlcon *con, oratext **data, ubig_ora *nread, ub1 *eoi);

/*
 * NAME:
 *   XmlUrlIsDefault - Is there a default resolver in effect?
 *
 * PARAMETERS:
 *   ctx   (IN) - XMLURL context
 *   con   (IN) - XMLURL connection object
 *   acc   (IN) - Access method
 *
 * RETURNS:
 *   boolean
 *
 * DESCRIPTION:
 *   Returns TRUE if the default URL resolver is used. False otherwise
 */
boolean XmlUrlIsDefault(xmlurlctx *ctx, xmlurlcon *con, xmlurlaccess acc);

/*
 * NAME:
 *   XmlUrlCloseCon - close a connection
 *
 * PARAMETERS:
 *   con (I/O) - LPU connection object
 *
 * RETURNS:
 *   (uword) error return
 *
 * DESCRIPTION:
 *    Closes an open connection.
 */
uword XmlUrlCloseCon(xmlurlcon *con);

/*
 * NAME:
 *   XmlUrlComposeCon - compose final URL from connection's components
 *
 * PARAMETERS:
 *   con  (I/O) - LPU connection object
 *   dest (OUT) - destination buffer
 *
 * RETURNS:
 *   (boolean) success
 *
 * DESCRIPTION:
 *    Forms the full URL for a connection into the given destination buffer.
 */
boolean XmlUrlComposeCon(xmlurlcon *con, oratext *dest);

/*
 * NAME:
 *   XmlUrlSetStream - set access-method stream (context-wide)
 *
 * PARAMETERS:
 *   ctx    (I/O) - LPU context
 *   acc    (IN)  - access method
 *   stream (I/O) - stream to read data from
 *
 * RETURNS:
 *   (uword) error return
 *
 * DESCRIPTION:
 *    Sets access-method stream prototype for context.  Note per-connection
 *    streams will override.  This stream prototype will be used to load
 *    data for the given access method, with the stream's SID being set to
 *    the ctx before each invocation.
 */
uword XmlUrlSetStream(xmlurlctx *ctx, xmlurlaccess acc, orastream *stream);

/*
 * NAME:
 *   XmlUrlGetStream - get access-method stream (context-wide)
 *
 * PARAMETERS:
 *   ctx    (I/O) - LPU context
 *   acc    (IN)  - access method
 *
 * RETURNS:
 *   (orastream *) stream for access method (or NULL)
 *
 * DESCRIPTION:
 *    Returns access-method stream previously set by XmlUrlSetStream()
 */
orastream *XmlUrlGetStream(xmlurlctx *ctx, xmlurlaccess acc);

/*
 * NAME:
 *   XmlUrlSetStreamCon - set access-method stream (per-connection)
 *
 * PARAMETERS:
 *   con    (I/O) - LPU connection object
 *   acc    (IN)  - access method
 *   stream (I/O) - stream to read data from
 *
 * RETURNS:
 *   (uword) error return
 *
 * DESCRIPTION:
 *    Sets access-method stream for connection.  Overrides per-context
 *    streams.  This stream prototype will be used to load data for the
 *    given access method for the given connection only.  The stream's
 *    SID will be set to the ctx before each invocation.
 */
uword XmlUrlSetStreamCon(xmlurlcon *con, xmlurlaccess acc, orastream *stream);

/*
 * NAME:
 *   XmlUrlBufferCon - return I/O buffer (and size) for connection
 *
 * PARAMETERS:
 *   con (I/O) - LPU connection object
 *
 * RETURNS:
 *   (oratext *) buffer and size set
 *
 * DESCRIPTION:
 *    Returns the I/O buffer for a connection and its size.
 */
oratext *XmlUrlBufferCon(xmlurlcon *con, ubig_ora *size);

/*
 * NAME:
 *   XmlUrlSetBufferCon - set the buffer
 *
 * PARAMETERS:
 *   con (I/O) - LPU connection object
 *
 * DESCRIPTION:
 *    Returns the I/O buffer for a connection and its size.
 */
void XmlUrlSetBufferCon(xmlurlcon *con, oratext *buffer);

/*
 * NAME:
 *   XmlUrlErrorCon - return last error code for connection
 *
 * PARAMETERS:
 *   con (I/O) - LPU connection object
 *
 * RETURNS:
 *   (uword) last error for connection
 *
 * DESCRIPTION:
 *    Returns the last numeric error code for the connection, or
 *    0 if there have been no errors.
 */
uword XmlUrlErrorCon(xmlurlcon *con);

/*
 * NAME:
 *   XmlUrlErrMsgCon - return last error message for connection
 *
 * PARAMETERS:
 *   con (I/O) - LPU connection object
 *
 * RETURNS:
 *   (oratext *) last error message for connection
 *
 * DESCRIPTION:
 *    Returns the last error message for the connection, or
 *    NULL if there have been no errors.
 */
oratext *XmlUrlErrMsgCon(xmlurlcon *con);

/*
 * NAME:
 *   XmlUrlFlushCon - flush connection buffer
 *
 * PARAMETERS:
 *   con (I/O) - LPU connection object
 *   len (IN)  - number of bytes to flush
 *
 * RETURNS:
 *   (uword) error code
 *
 * DESCRIPTION:
 *    Flushes the connection buffer.  This function is only for use
 *    by a XMLURL_POST_BODYF function, which calls it when the connection
 *    buffer becomes full, in order to flush and chunk and make new
 *    space available for the next part of the POST body.
 */
uword XmlUrlFlushCon(xmlurlcon *con, ubig_ora len);

/*
 * NAME:
 *   XmlUrlHeaderCon - set additional HTTP header value
 *
 * PARAMETERS:
 *   con    (I/O) - LPU connection object
 *   header (IN)  - HTTP header value
 *
 * RETURNS:
 *   (uword) error code
 *
 * DESCRIPTION:
 *    Sets an additional HTTP header value used when opening the
 *    connection.  The header value should be in standard HTTP
 *    format, "Header: Value", e.g. "User-Agent: Oracle Foo".
 */
uword XmlUrlHeaderCon(xmlurlcon *con, oratext *header);

/*
 * NAME:
 *   XmlUrlSetStreamFlag
 * PARAMETERS:
 *   ctx    (IN) - xmlurl context
 *   acctyp (IN) - access type to change
 *   flag   (IN) - the new value for the stream access flag
 * RETURNS:
 *   void
 * DESCRIPTION:
 *   Set the stream callback flag for an access type to TRUE or FALSE.
 */
void XmlUrlSetStreamFlag(xmlurlctx *ctx, xmlurlaccess acctyp, boolean flag);

/* ### DEPRECATED ### */
uword XmlUrlOpeniri(xmlurlctx *ctx, oratext *irl, xmlurlpart *parts,
                    oratext *buffer, ubig_ora bufsiz);

#endif /* XMLURL_ORACLE */

