/* $RCSfile: xmlproc.h $ $Date: 2018/06/14 16:52:54 $
 Copyright (c) 2001, 2018, Oracle and/or its affiliates. All rights reserved.

   FILE NAME
        xmlproc.h - Oracle XML Processing APIs

   FILE DESCRIPTION
        This file defines the XML processing interfaces.

        DOM class diagram:                   XML type name:

          Node                                  xmlnode
           \
            +----Entity                         xmlentnode
            |----DocumentFragment               xmlfragnode
            |----EntityReference                xmlentrefnode
            |----Document                       xmldocnode
            |----CharacterData
            |     \---Text                      xmltextnode
            |         \--CDATASection           xmlcdatanode
            |          \-Comment                xmlcommentnode
            |----Attr                           xmlattrnode
            |----ProcessingInstruction          xmlpinode
            |----Element                        xmlelemnode
            |----Notation                       xmlnotenode
            +----DocumentType                   xmldtdnode

         DOMImplementation                      NOT USED

         NodeList                               xmlnodelist
         NamedNodeMap                           xmlnamedmap

         NodeIterator      \                    xmliter
         NodeFilter         \   DOM 2           xmlnodefilt
         TreeWalker         / Traversal         xmlwalk
         DocumentTraversal /

         Range             \    DOM 2           xmlrange
         DocumentRange     /    Range                   

 
   The W3C DOM specification says all data is a DOMString, defined as
   UTF-16 or "Unicode".  This implementation allows the data encoding
   to be chosen; it can be Unicode if desired, or anything else (UTF-8,
   Shift-JIS, ASCII, EBCDIC, etc).

   It is the user's responsibility to make sure strings passed to DOM
   are in the appropriate encoding.  If you set a node's value, that
   value will NOT be validated, verified, or re-encoded.  It will just
   be set; the onus is on the API user to provide correct data.  The
   initial DOM data from a parse will be in the chosen data encoding.
*/

#ifndef XMLPROC_ORACLE
# define XMLPROC_ORACLE

#ifndef STDIO_ORACLE
# define STDIO_ORACLE
# include <stdio.h>
#endif

#ifndef STDARG_ORACLE
# define STDARG_ORACLE
# include <stdarg.h>
#endif

#ifndef ORASTRUC
# include <orastruc.h>
#endif

#ifndef XMLERR_ORACLE
# include <xmlerr.h>
#endif

/*---------------------------------------------------------------------------
                     PUBLIC TYPES AND CONSTANTS
  ---------------------------------------------------------------------------*/

/* Forward declarations */

struct xmlschtype; typedef struct xmlschtype xmlschtype;
struct xmlcb;    typedef struct xmlcb xmlcb;
struct xmldomcb; typedef struct xmldomcb xmldomcb;

/* xmlMemU is used to allow users to create DOM document
 * using user's own memory management mechanism: which
 * has its memory allocator and free with its own memory
 * region. Furthermore, this allows users to control memory
 * allocation and free at the granuality of per document level.
 *
 * User can call XmlCreateDocumentWithMem() or
 * XmlLoadDom() API to pass in the pointer to xmlMemU.
 */
typedef struct xmlMemU
{
  /* void * that application owns */
  void *usrctx_xmlMemU; 
  /* The initMemCtx_xmlMemU function is called by DOM implemetopr
   * by passing the usrctx_xmlMemU as input argumet and
   * it returns a void * which represents the user memory context
   * that is passed in as the first parameter to subsequent
   * alloc_xmlMemU() and free_xmlMemU() functions. This function is
   * called first to get a valid memctx that is stashed subquently for
   * allocation and free.
   */
  void *(*initMemCtx_xmlMemU)(void *usrctx);
  /* The  alloc_xmlMemU function is called by DOM implemetopr to
   * allocate memory associated with the current DOM doc tree. 
   * The first parameter
   * memctx is what (*initMemCtx_xmlMemU) has returned.
   * The second parameter is the size to be allocated.
   * The third parameter is TRUE which means allocated memory needs to be
   * memzeroed. FALSE means there is NO need to memzeroed allocated memory.
   */
  void *(*alloc_xmlMemU)(void *memctx, ub4 size, boolean init);
  /* The  free_xmlMemU function is called by DOM implemetopr to
   * free memory associated with this current DOM doc tree. 
   * The first parameter
   * memctx is what (*initMemCtx_xmlMemU) has returned.
   * The second parameter is the pointer to piece of memory to be freed.
   */
  void (*free_xmlMemU)(void *memctx, void *memtobefree);
   /* The  termMemCtx_xmlMemU function is called by DOM implemetopr to
    * free the entire memory associated with this current DOM doc tree when
    * this DOM doc tree is freed. 
    * The first parameter is usrctx_xmlMemU.
    * The second parameter
    * memctx is what (*initMemCtx_xmlMemU) has returned.
    */
  void (*termMemCtx_xmlMemU)(void *usrctx, void *memctx);
} xmlMemU;

#ifndef XMLCTX_DEFINED
# define XMLCTX_DEFINED
/* DATATYPE xmlctx - XML top-level context
   SEE ALSO XmlCreate, XmlDestroy
   Context shared for all documents in an XML session.  Contains encoding
   information, low-level memory allocation function pointers, error message
   language/encoding and optional handler function, etc.  Required to load
   (parse) documents and create DOM, generate SAX, etc.
*/
struct xmlctx; typedef struct xmlctx xmlctx;
#endif

/* DATATYPE xmlnodetype - DOM node type enumeration
   SEE ALSO XmlDomGetNodeType
   The numeric type code of a node.  0 means invalid, 1-13 are the
   standard numberings from DOM 1.0, and higher numbers are for
   internal use only.
*/
typedef enum {
    XMLDOM_NONE     = 0,  /* bogus node */
    XMLDOM_ELEM     = 1,  /* element */
    XMLDOM_ATTR     = 2,  /* attribute */
    XMLDOM_TEXT     = 3,  /* char data not escaped by CDATA */
    XMLDOM_CDATA    = 4,  /* char data escaped by CDATA */
    XMLDOM_ENTREF   = 5,  /* entity reference */
    XMLDOM_ENTITY   = 6,  /* entity */
    XMLDOM_PI       = 7,  /* <?processing instructions?> */
    XMLDOM_COMMENT  = 8,  /* <!-- Comments --> */
    XMLDOM_DOC      = 9,  /* Document */
    XMLDOM_DTD      = 10, /* DTD */
    XMLDOM_FRAG     = 11, /* Document fragment */
    XMLDOM_NOTATION = 12, /* notation */
    /* === Oracle extensions from here on === */
    XMLDOM_ELEMDECL = 13, /* DTD element declaration */
    XMLDOM_ATTRDECL = 14, /* DTD attribute declaration */
    /* --- Content Particles (nodes in element's Content Model) --- */
    XMLDOM_CPELEM   = 15, /* element */
    XMLDOM_CPCHOICE = 16, /* choice (a|b) */
    XMLDOM_CPSEQ    = 17, /* sequence (a,b) */
    XMLDOM_CPPCDATA = 18, /* #PCDATA */
    XMLDOM_CPSTAR   = 19, /* '*' (zero or more) */
    XMLDOM_CPPLUS   = 20, /* '+' (one or more) */
    XMLDOM_CPOPT    = 21, /* '?' (optional) */
    XMLDOM_CPEND    = 22  /* end marker */
} xmlnodetype;

/* XML DOM types */
#ifndef XML_TYPES
# define XML_TYPES
typedef struct xmlnode       xmlnode;
typedef xmlnode              xmlentnode;
typedef xmlnode              xmlfragnode;
typedef xmlnode              xmlentrefnode;
typedef xmlnode              xmldocnode;
typedef xmlnode              xmltextnode;
typedef xmlnode              xmlcdatanode;
typedef xmlnode              xmlcommentnode;
typedef xmlnode              xmlattrnode;
typedef xmlnode              xmlpinode;
typedef xmlnode              xmlelemnode;
typedef xmlnode              xmlnotenode;
typedef xmlnode              xmldtdnode;
typedef struct xmlnodelist   xmlnodelist;
typedef struct xmlnamedmap   xmlnamedmap;
typedef struct xmlpfnspair   xmlpfnspair;
typedef struct xmlnodefilt   xmlnodefilt;
#endif

/* DATATYPE xmlrange - DOM Range control structure
   SEE ALSO XmlDomCreateRange
   Control structure for DOM 2 Range 
*/
typedef struct xmlrange {
    xmlnode *startnode_xmlrange;  /* start point container */
    ub4      startofst_xmlrange;  /* start point index */
    xmlnode *endnode_xmlrange;    /* end point container */
    ub4      endofst_xmlrange;    /* end point index */
    xmlnode *doc_xmlrange;        /* document node */
    xmlnode *root_xmlrange;       /* root node of the range */
    boolean  collapsed_xmlrange;  /* is range collapsed? */
    boolean  detached_xmlrange;   /* is range invalid or invalidated? */
} xmlrange;

/* DATATYPE xmliter - Control structure for Node Iterator and Tree Walker
   SEE ALSO XmlDomCreateNodeIter, XmlDomCreateTreeWalker
   Control structure for DOM 2 Node Iterator and Tree Walker
*/
struct xmliter {
    xmlnode *root_xmliter;   /* root node of the iteration space */
    xmlnode *cur_xmliter;    /* current position (ref) node of the iterator */
    ub4      show_xmliter;   /* node filter mask */
    void    *filt_xmliter;   /* node filter function */
    boolean  attach_xmliter; /* is iterator valid? */
    boolean  expan_xmliter;  /* are external entities expanded? */
    boolean  before_xmliter; /* is the iter position before the ref node? */
};
typedef struct xmliter xmliter;
typedef struct xmliter xmlwalk;

/* DATATYPE xmlurlacc - URL access methods
   SEE ALSO XmlAccess
   This is an enumeration of the known access methods for retrieving data
   from a URL.  Open/read/close functions may be plugged in to override the
   default behavior (see XmlAccess)
*/
typedef enum {
    XML_ACCESS_NONE    = 0, /* not specified */
    XML_ACCESS_UNKNOWN = 1, /* specified but unknown */
    XML_ACCESS_FILE    = 2, /* filesystem access */
    XML_ACCESS_HTTP    = 3, /* HTTP */
    XML_ACCESS_FTP     = 4, /* FTP */
    XML_ACCESS_GOPHER  = 5, /* Gopher */
    XML_ACCESS_ORADB   = 6, /* Oracle DB */
    XML_ACCESS_STREAM  = 7, /* (deprecated) */
    XML_ACCESS_HTTPS   = 8  /* Secure HTTP */
} xmlurlacc;

#define XML_ACCESS_MIN  0
#define XML_ACCESS_MAX  8
#define XML_N_ACCESS    (XML_ACCESS_MAX - XML_ACCESS_MIN + 1)

/* -----------------------------------------------------------------------
                Constants for range return values/errors
   ----------------------------------------------------------------------- */

/* DATATYPE xmlshowbits - Constants for whatToShow etc
   SEE ALSO XmlDomCreateNodeIter, XmlDomCreateTreeWalker
   Bit flags used to select which nodes types to show
*/
typedef ub4 xmlshowbits;
#define XMLDOM_SHOW_ALL         ~(xmlshowbits)0
#define XMLDOM_SHOW_BIT(ntype)  ((xmlshowbits)1 << (ntype))
#define XMLDOM_SHOW_ELEM        XMLDOM_SHOW_BIT(XMLDOM_ELEM)
#define XMLDOM_SHOW_ATTR        XMLDOM_SHOW_BIT(XMLDOM_ATTR)
#define XMLDOM_SHOW_TEXT        XMLDOM_SHOW_BIT(XMLDOM_TEXT)
#define XMLDOM_SHOW_CDATA       XMLDOM_SHOW_BIT(XMLDOM_CDATA)
#define XMLDOM_SHOW_ENTREF      XMLDOM_SHOW_BIT(XMLDOM_ENTREF)
#define XMLDOM_SHOW_ENTITY      XMLDOM_SHOW_BIT(XMLDOM_ENTITY)
#define XMLDOM_SHOW_PI          XMLDOM_SHOW_BIT(XMLDOM_PI)
#define XMLDOM_SHOW_COMMENT     XMLDOM_SHOW_BIT(XMLDOM_COMMENT)
#define XMLDOM_SHOW_DOC         XMLDOM_SHOW_BIT(XMLDOM_DOC)
#define XMLDOM_SHOW_DTD         XMLDOM_SHOW_BIT(XMLDOM_DTD)
#define XMLDOM_SHOW_FRAG        XMLDOM_SHOW_BIT(XMLDOM_FRAG)
#define XMLDOM_SHOW_NOTATION    XMLDOM_SHOW_BIT(XMLDOM_NOTATION)
#define XMLDOM_SHOW_DOC_TYPE    XMLDOM_SHOW_BIT(XMLDOM_DOC_TYPE)

/* DATATYPE xmlcmphow - Constants for range comparison
   SEE ALSO XmlDomRangeCompareBoundaryPoints
   Constant used for DOM Range comparisons
*/
typedef enum {
    XMLDOM_START_TO_START = 0,
    XMLDOM_START_TO_END   = 1,
    XMLDOM_END_TO_END     = 2, 
    XMLDOM_END_TO_START   = 3
} xmlcmphow;

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
        XML_STREAM_OPEN_F

    PURPOSE
        DEPRECATED User-defined stream open callback

    DESCRIPTION
        DEPRECATED - DO NOT USE THIS FUNCTION FOR NEW WORK.  ALL STREAMING
        CODE SHOULD USE THE NEW orastream STRUCTURE DEFINED IN orastruc.h
        This macro defines a prototype for the open function callback, which
        is called once to open the input source.  This function should return
        XMLERR_OK on success.

    RETURNS
        (xmlerr) numeric error code, 0 on success

    ARGUMENTS
        xctx   (IN)  XML context
        sctx   (IN)  user-defined stream context
        path   (IN)  full path of the URI to be opened
        parts  (IN)  URI broken down into components (opaque pointer)
        length (OUT) total length of input data if known, 0 if not known

    SEE ALSO
        XML_STREAM_CLOSE_F, XML_STREAM_WRITE_F
*/
#define XML_STREAM_OPEN_F(func, xctx, sctx, path, parts, length) \
    xmlerr func(xmlctx *xctx, void *sctx, oratext *path, \
                void *parts, ubig_ora *length)

/*---------------------------------------------------------------------------
    NAME
        XML_STREAM_CLOSE_F

    PURPOSE
        DEPRECATED User-defined stream close callback

    DESCRIPTION
        DEPRECATED - DO NOT USE THIS FUNCTION FOR NEW WORK.  ALL STREAMING
        CODE SHOULD USE THE NEW orastream STRUCTURE DEFINED IN orastruc.h
        This macro defines a prototype for the close function callback,
        called to close an open source and free its resources.

    RETURNS
        (void)

    ARGUMENTS
        xctx (IN) XML context
        sctx (IN) user-defined stream context

    SEE ALSO
        XML_STREAM_OPEN_F, XML_STREAM_WRITE_F
*/
#define XML_STREAM_CLOSE_F(func, xctx, sctx) \
    void func(xmlctx *xctx, void *sctx) 

/*---------------------------------------------------------------------------
    NAME
        XML_STREAM_WRITE_F

    PURPOSE
        DEPRECATED User-defined stream write callback

    DESCRIPTION
        DEPRECATED - DO NOT USE THIS FUNCTION FOR NEW WORK.  ALL STREAMING
        CODE SHOULD USE THE NEW orastream STRUCTURE DEFINED IN orastruc.h
        This macro defines a prototype for the write function callback,
        called to write data to a user-defined stream.

    RETURNS
        (xmlerr) numeric error code, 0 on success

    ARGUMENTS
        xctx (IN) XML context
        sctx (IN) user-defined stream context
        path (IN) full URI of the open source (for error messages)
        src  (IN) source buffer to read data from
        size (IN) size of source in bytes

    SEE ALSO
        XML_STREAM_OPEN_F, XML_STREAM_CLOSE_F
*/
#define XML_STREAM_WRITE_F(func, xctx, sctx, path, src, size) \
    xmlerr func(xmlctx *xctx, void *sctx, oratext *path, \
                oratext *src, size_t size)

/*---------------------------------------------------------------------------
    NAME
        XML_ERRMSG_F

    PURPOSE
        Handle error message

    DESCRIPTION
        This macro defines a prototype for the error message handling
        function.  If no error message callback is provided at XML
        initialization time, errors will be printed to stderr.  If
        a handler is provided, it will be invoked instead of printing
        to stderr.

    RETURNS
        (void)

    ARGUMENTS
        etcx (IN) error message context
        msg  (IN) text of error message
        err  (IN) numeric error code

    SEE ALSO
        XmlCreate
*/
#define XML_ERRMSG_F(func, ectx, msg, err) \
    void func(void *ectx, oratext *msg, xmlerr err)
/*---------------------------------------------------------------------------
    NAME
        XML_TRACE_F

    PURPOSE
        Handle xdk tracing

    DESCRIPTION
        This macro defines a prototype for the xdk tracing mechanism.  
        if a tracing handler is provided, it will be invoked at err 
        and provide tracing information to caller.

    RETURNS
        (void)

    ARGUMENTS
        etcx (IN) error message context
        buf  (OUT) tracing data

    SEE ALSO
        XmlCreate
*/
#define XML_TRACE_F(func, ectx, buf) \
   void func(void *ectx, oratext *buf)
/*---------------------------------------------------------------------------
    NAME
        XML_ALLOC_F

    PURPOSE
        Low-level memory allocation

    DESCRIPTION
        This macro defines a prototype for the low-level memory alloc
        function provided by the user.  If no allocator is provided,
        malloc() is used.  Memory should NOT be zeroed by this func!
        Matches XML_FREE_F.

    RETURNS
        (void *) allocated memory

    ARGUMENTS
        mctx (IN) low-level memory context
        size (IN) number of bytes to allocate

    SEE ALSO
        XML_FREE_F
*/
#define XML_ALLOC_F(func, mctx, size) \
    void *func(void *mctx, size_t size)

/*---------------------------------------------------------------------------
    NAME
        XML_FREE_F

    PURPOSE
        Low-level memory freer

    DESCRIPTION
        This macro defines a prototype for the low-level memory free
        function provided by the user.  If no allocator is provided,
        free() is used.  Matches XML_ALLOC_F.

    RETURNS
        (void)

    ARGUMENTS
        mctx (IN) low-level memory context
        ptr  (IN) memory to be freed

    SEE ALSO
        XML_FREE_F
*/
#define XML_FREE_F(func, mctx, ptr) \
    void func(void *mctx, void *ptr)

#ifndef XMLSAXCB_DEFINED
# define XMLSAXCB_DEFINED

/*---------------------------------------------------------------------------
                           Package SAX - SAX APIs

    SAX is a standard interface for event-based XML parsing, developed
    collaboratively by the members of the XML-DEV mailing list.

    To use SAX, an xmlsaxcb structure is initialized with function
    pointers and passed to one of the XmlLoadSax calls.  A pointer to a
    user-defined context structure is also provided, and will be passed
    to each SAX function.

  ---------------------------------------------------------------------------*/

/****************************************************************************
                               Interface SAX
 ****************************************************************************/

/* SAX callback structure.  Set function pointers for those events you wish
   to receive, and make the rest NULL.  The entire structure should be
   initialized to zeros so that unused pointers at the end of the struct
   are NULL (to avoid future problems if they are defined to new events)

   All SAX functions return a (sword) error code.  A zero value (XMLERR_OK)
   means success and processing will continue; any non-zero value indicates
   a failure and parsing stops.

   Pointers passed to SAX callbacks (names, character data, etc), are only
   valid for the duration of the callback!  Once it has returned, the memory
   pointed to will be recycled and written over.  If you need to persistently
   keep the datums, you must copy them.
*/

struct xmlsaxcb
{
/*---------------------------------------------------------------------------
    NAME
        XmlSaxStartDocument

    STANDARD
        SAX 2: ContentHandler.startDocument()

    PURPOSE
        Receives SAX start-of document notification

    DESCRIPTION
        The first SAX event, called once per document, indicating the
        start of the document.  Matching event is XmlSaxEndDocument.

    EXAMPLE
        <top/>
          --SAX Parse-->
        XmlSaxStartDocument()
        ...

    RETURNS
        (xmlerr) error code, XMLERR_OK [0] for success

    ARGUMENTS
        ctx (IN) user's SAX context [see XmlLoadSax functions]

    SEE ALSO
        XmlSaxEndDocument
*/
#define XMLSAX_START_DOC_CB XmlSaxStartDocument_xmlsaxcb

#define XMLSAX_START_DOC_F(func, ctx) \
    xmlerr func(void *ctx)

    XMLSAX_START_DOC_F((*XMLSAX_START_DOC_CB), ctx);

/*---------------------------------------------------------------------------
    NAME
        XmlSaxEndDocument

    STANDARD
        SAX 2: ContentHandler.endDocument()

    PURPOSE
        Receives SAX end-of-document notification

    DESCRIPTION
        The last SAX event, called once per document, indicating the
        end of the document.  Matching event is XmlSaxStartDocument.

    EXAMPLE
        <top/>
          --SAX Parse-->
        ...
        XmlSaxEndDocument()

    RETURNS
        (xmlerr) error code, XMLERR_OK [0] for success

    ARGUMENTS
        ctx (IN) user's SAX context [see XmlLoadSax functions]

    SEE ALSO
        XmlSaxStartDocument
*/
#define XMLSAX_END_DOC_CB XmlSaxEndDocument_xmlsaxcb

#define XMLSAX_END_DOC_F(func, ctx) \
    xmlerr func(void *ctx)


    XMLSAX_END_DOC_F((*XMLSAX_END_DOC_CB), ctx);

/*---------------------------------------------------------------------------
    NAME
        XmlSaxStartElement

    STANDARD
        SAX 1: DocumentHandler.startElement()

    PURPOSE
        Receives SAX start-of-element notification

    DESCRIPTION
        This event marks the start of an element.  Note this is the original
        SAX 1 non-namespace-aware version; XmlSaxStartElementNS is the SAX 2
        namespace-aware version.  If both are registered, only the NS version
        will be called.  The element's name will be in the data encoding,
        as are all the attribute parts.  See the functions in the NamedNodeMap
        interface for operating on the attributes map.  The matching function
        is XmlSaxEndElement (there is no XmlSaxEndElementNS).

    EXAMPLE
        <top/>
          --SAX Parse-->
        ...
        XmlSaxStartElement(name="top", attrs=NULL)
        XmlSaxEndElement(name="top")
        ...

    RETURNS
        (xmlerr) error code, XMLERR_OK [0] for success

    ARGUMENTS
        ctx   (IN) user's SAX context [see XmlLoadSax functions]
        name  (IN) name of element [data encoding]
        attrs (IN) NamedNodeMap of element's attributes, or NULL

    SEE ALSO
        XmlSaxEndElement, XmlDomGetNodeMapLength, XmlDomGetNamedItem
*/
#define XMLSAX_START_ELEM_CB XmlSaxStartElement_xmlsaxcb

#define XMLSAX_START_ELEM_F(func, ctx, name, attrs) \
    xmlerr func(void *ctx, oratext *name, xmlnodelist *attrs)

    XMLSAX_START_ELEM_F((*XMLSAX_START_ELEM_CB), ctx, name, attrs);

/*---------------------------------------------------------------------------
    NAME
        XmlSaxEndElement

    STANDARD
        SAX 1: DocumentHandler.endElement()

    PURPOSE
        Receives SAX end-of-element notification

    DESCRIPTION
        This event marks the close of an element; it matches the
        XmlSaxStartElement or XmlSaxStartElementNS events.  The name is the
        tagName of the element (which may be a QName for namespace-aware
        elements) and is in the data encoding.

    EXAMPLE
        <top/>
          --SAX Parse-->
        ...
        XmlSaxStartElement(name="top", attrs=NULL)
        XmlSaxEndElement(name="top")
        ...

    RETURNS
        (xmlerr) error code, XMLERR_OK [0] for success

    ARGUMENTS
        ctx  (IN) user's SAX context [see XmlLoadSax functions]
        name (IN) name of ending element [data encoding]

    SEE ALSO
        XmlSaxEndElement
*/
#define XMLSAX_END_ELEM_CB XmlSaxEndElement_xmlsaxcb

#define XMLSAX_END_ELEM_F(func, ctx, name) \
    xmlerr func(void *ctx, oratext *name)

    XMLSAX_END_ELEM_F((*XMLSAX_END_ELEM_CB), ctx, name);

/*---------------------------------------------------------------------------
    NAME
        XmlSaxCharacters

    STANDARD
        SAX 2: ContentHandler.characters()

    PURPOSE
        Receives SAX notification of character data

    DESCRIPTION
        This event marks character data, either Text or CDATA.  If an
        XmlSaxCDATA callback is provided, then CDATA will be send to that
        instead; with no XmlSaxCDATA callback, both Text and CDATA go to
        the XmlSaxCharacters callback.  The data will be in the data
        encoding, and the returned length is in characters, not bytes.
        See also XmlSaxWhitespace, which receiving notification about
        ignorable (whitespace formatting) character data.

    EXAMPLE
        <top>junk</top>
          --SAX Parse-->
        ...
        XmlSaxCharacters(ch="junk", len=4)
        ...

    RETURNS
        (xmlerr) error code, XMLERR_OK [0] for success

    ARGUMENTS
        ctx (IN) user's SAX context [see XmlLoadSax functions]
        ch  (IN) pointer to data [data encoding]
        len (IN) length of data [in characters]

    SEE ALSO
        XmlSaxWhitespace
*/
#define XMLSAX_CHARACTERS_CB XmlSaxCharacters_xmlsaxcb

#define XMLSAX_CHARACTERS_F(func, ctx, ch, len) \
    xmlerr func(void *ctx, oratext *ch, size_t len)

    XMLSAX_CHARACTERS_F((*XMLSAX_CHARACTERS_CB), ctx, ch, len);

/*---------------------------------------------------------------------------
    NAME
        XmlSaxWhitespace

    STANDARD
        SAX 2: ContentHandler.ignorableWhitespace()

    PURPOSE
        Receives SAX notification of ignorable (whitespace) data

    DESCRIPTION
        This event marks ignorable whitespace data such as newlines, and
        indentation between lines.  The matching function is XmlSaxCharacters,
        which receives notification of normal character data.  The data
        is in the data encoding, and the returned length is in characters,
        not bytes.

    EXAMPLE
        <top>
            <sub>junk</sub>
        </top>
          --SAX Parse-->
        ...
        XmlSaxStartElement(name="top", attrs=NULL)
        XmlSaxWhitespace(ch="\n    ", len=5)
        XmlSaxStartElement(name="sub", attrs=NULL)
        ...

    RETURNS
        (xmlerr) error code, XMLERR_OK [0] for success

    ARGUMENTS
        ctx (IN) user's SAX context [see XmlLoadSax functions]
        ch  (IN) pointer to data [data encoding]
        len (IN) length of data [in characters]

    SEE ALSO
        XmlSaxCharacters
*/
#define XMLSAX_WHITESPACE_CB XmlSaxWhitespace_xmlsaxcb

#define XMLSAX_WHITESPACE_F(func, ctx, ch, len) \
    xmlerr func(void *ctx, oratext *ch, size_t len)

    XMLSAX_WHITESPACE_F((*XMLSAX_WHITESPACE_CB), ctx, ch, len);

/*---------------------------------------------------------------------------
    NAME
        XmlSaxPI

    STANDARD
        SAX 2: ContentHandler.processingInstruction()

    PURPOSE
        Receives SAX notification of a processing instruction

    DESCRIPTION
        This event marks a processing instruction.  The PI's target and
        data will be in the data encoding.  There is always a target, but
        the data may be NULL.

    EXAMPLE
        <?keywords sax example?>
        <top/>
          --SAX Parse-->
        XmlSaxPI(target="keywords", data="sax example")
        ...

    RETURNS
        (xmlerr) error code, XMLERR_OK [0] for success

    ARGUMENTS
        ctx    (IN) user's SAX context [see XmlLoadSax functions]
        target (IN) PI's target [data encoding]
        data   (IN) PI's data [data encoding] or NULL
*/
#define XMLSAX_PI_CB XmlSaxPI_xmlsaxcb

#define XMLSAX_PI_F(func, ctx, target, data) \
    xmlerr func(void *ctx, oratext *target, oratext *data)

    XMLSAX_PI_F((*XMLSAX_PI_CB), ctx, target, data);

/*---------------------------------------------------------------------------
    NAME
        XmlSaxNotationDecl

    STANDARD
        SAX 1: DTDHandler.notationDecl()

    PURPOSE
        Receives SAX notification of a notation declaration

    DESCRIPTION
        The even marks the declaration of a notation in the DTD.  The
        notation's name, public ID, and system ID will all be in the
        data encoding.  Both IDs are optional and may be NULL.

    EXAMPLE
        <!DOCTYPE foo [ <!NOTATION foo PUBLIC "[" "null.ent"> ]>
        ...
          --SAX Parse-->
        XmlSaxNotationDecl(name="foo", publicID="[", systemID="null.ent")
        ...

    RETURNS
        (xmlerr) error code, XMLERR_OK [0] for success

    ARGUMENTS
        ctx      (IN) user's SAX context [see XmlLoadSax functions]
        name     (IN) notation's name [data encoding]
        publicID (IN) notation's public ID [data encoding] or NULL
        systemID (IN) notation's system ID [data encoding] or NULL
*/
#define XMLSAX_NOTATION_DECL_CB XmlSaxNotationDecl_xmlsaxcb

#define XMLSAX_NOTATION_DECL_F(func, ctx, name, pubId, sysId) \
    xmlerr func(void *ctx, oratext *name, oratext *pubId, oratext *sysId)

    XMLSAX_NOTATION_DECL_F((*XMLSAX_NOTATION_DECL_CB), ctx, name, pubId,sysId);

/*---------------------------------------------------------------------------
    NAME
        XmlSaxUnparsedEntityDecl

    STANDARD
        SAX 1: DTDHandler.unparsedEntityDecl()

    PURPOSE
        Receives SAX notification of a unparsed entity declaration

    DESCRIPTION
        Marks an unparsed entity declaration in the DTD, see
        XmlSaxParsedEntityDecl for the parsed entity version.  The unparsed
        entity's name, public ID, system ID, and notation name will all be
        in the data encoding.

    EXAMPLE
        <!DOCTYPE foo [
            <!ENTITY e PUBLIC "p-p-pub-id" 'entity.dat' NDATA endayta>
        ]>
        ...
          --SAX Parse-->
        XmlSaxUnparsedEntityDecl(name="e", publicID="p-p-pub-id",
                                 systemID="entity.dat", notationName="endayta")

    RETURNS
        (xmlerr) error code, XMLERR_OK [0] for success

    ARGUMENTS
        ctx   (IN) user's SAX context [see XmlLoadSax functions]
        name  (IN) entity's name [data encoding]
        pubId (IN) entity's public ID [data encoding] or NULL
        sysId (IN) entity's system ID [data encoding]
        note  (IN) entity's notation name [data encoding]

    SEE ALSO
        XmlSaxParsedEntityDecl
*/
#define XMLSAX_UNPARSED_ENT_DECL_CB XmlSaxUnparsedEntityDecl_xmlsaxcb

#define XMLSAX_UNPARSED_ENT_DECL_F(func, ctx, name, pubId, sysId, note) \
    xmlerr func(void *ctx, oratext *name, oratext *pubId, oratext *sysId, \
               oratext *note)

    XMLSAX_UNPARSED_ENT_DECL_F((*XMLSAX_UNPARSED_ENT_DECL_CB),
                               ctx, name, pubId, sysId, note);

/*---------------------------------------------------------------------------
    NAME
        XmlSaxStartElementNS

    STANDARD
        SAX 2: ContentHandler.startElement()

    PURPOSE
        Receives SAX namespace-aware start-of-element notification

    DESCRIPTION
        This event marks the start of an element.  Note this is the new
        SAX 2 namespace-aware version; XmlSaxStartElement is the SAX 1
        non-namespace-aware version.  If both are registered, only the NS
        version will be called.  The element's QName, local name, and
        namespace URI will be in the data encoding, as are all the attribute
        parts.  See the functions in the NamedNodeMap interface for
        operating on the attributes map.  The matching function is
        XmlSaxEndElement (there is no XmlSaxEndElementNS).

    EXAMPLE
        <foo:top xmlns:foo="/foo/bar"/>
          --SAX Parse-->
        ...
        XmlSaxStartElement(qname="foo:top", local="top",
                           nsp="/foo/bar", attrs=NULL)
        XmlSaxEndElement(name="foo:top")
        ...

    RETURNS
        (xmlerr) error code, XMLERR_OK [0] for success

    ARGUMENTS
        ctx   (IN) user's SAX context [see XmlLoadSax functions]
        qname (IN) element's qualified name [data encoding]
        local (IN) element's namespace local name [data encoding]
        nsp   (IN) element's namespace URI [data encoding]
        attrs (IN) NodeList of element's attributes, or NULL

    SEE ALSO
        XmlSaxStartElement, XmlSaxEndElement, XmlDomGetNodeMapLength,
        XmlDomGetNamedItem
*/
#define XMLSAX_START_ELEM_NS_CB XmlSaxStartElementNS_xmlsaxcb

#define XMLSAX_START_ELEM_NS_F(func, ctx, qname, local, nsp, attrs) \
    xmlerr func(void *ctx, oratext *qname, oratext *local, \
               oratext *nsp, xmlnodelist *attrs)

    XMLSAX_START_ELEM_NS_F((*XMLSAX_START_ELEM_NS_CB),
                           ctx, qname, local, nsp, attrs);

/*---------------------------------------------------------------------------
    NAME
        XmlSaxComment

    PURPOSE
        Receives SAX notification of a comment

    DESCRIPTION
        This event marks a comment in the XML document.  The comment's data
        will be in the data encoding.  Oracle extension, not in SAX standard.

    EXAMPLE
        <!--Just a comment-->
        <foo/>
          --SAX Parse-->
        XmlSaxComment(data="Just a comment")
        ...

    RETURNS
        (xmlerr) error code, XMLERR_OK [0] for success

    ARGUMENTS
        ctx  (IN) user's SAX context [see XmlLoadSax functions]
        data (IN) comment's data [data encoding]
*/
#define XMLSAX_COMMENT_CB XmlSaxComment_xmlsaxcb

#define XMLSAX_COMMENT_F(func, ctx, data) \
    xmlerr func(void *ctx, oratext *data)

    XMLSAX_COMMENT_F((*XMLSAX_COMMENT_CB), ctx, data);

/*---------------------------------------------------------------------------
    NAME
        XmlSaxElementDecl

    PURPOSE
        Receives SAX notification of an element's declaration.  Oracle
        extension, not in SAX standard.

    DESCRIPTION
        This event marks an element declaration in the DTD.  The element's
        name and content will be in the data encoding.

    EXAMPLE
        <!DOCTYPE foo [ <!ELEMENT e (#PCDATA)> ]>
        ...
          --SAX Parse-->
        XmlSaxElementDecl(name="e", content="(#PCDATA)")
        ...

    RETURNS
        (xmlerr) error code, XMLERR_OK [0] for success

    ARGUMENTS
        ctx     (IN) user's SAX context [see XmlLoadSax functions]
        name    (IN) element's name
        content (IN) element's context model

    SEE ALSO
        XmlSaxAttributeDecl
*/
#define XMLSAX_ELEM_DECL_CB XmlSaxElementDecl_xmlsaxcb

#define XMLSAX_ELEM_DECL_F(func, ctx, name, content) \
    xmlerr func(void *ctx, oratext *name, oratext *content)

    XMLSAX_ELEM_DECL_F((*XMLSAX_ELEM_DECL_CB), ctx, name, cont);

/*---------------------------------------------------------------------------
    NAME
        XmlSaxAttributeDecl

    PURPOSE
        Receives SAX notification of an attribute's declaration.  Oracle
        extension, not in SAX standard.

    DESCRIPTION
        This event marks an element declaration in the DTD.  The element's
        name and content will be in the data encoding.  Note that an
        attribute may be declared before the element it belongs to!

    EXAMPLE
        <!DOCTYPE foo [ <!ATTLIST a id ID #IMPLIED> ]>
        ...
          --SAX Parse-->
        XmlSaxAttributeDecl(elem="a", name="id", body="ID #IMPLIED")
        ...

    RETURNS
        (xmlerr) error code, XMLERR_OK [0] for success

    ARGUMENTS
        ctx  (IN) user's SAX context [see XmlLoadSax functions]
        elem (IN) element that attribute is declared for [data encoding]
        attr (IN) attribute's name [data encoding]
        body (IN) body of attribute declaration [data encoding]

    SEE ALSO
        XmlSaxAttributeDecl
*/
#define XMLSAX_ATTR_DECL_CB XmlSaxAttributeDecl_xmlsaxcb

#define XMLSAX_ATTR_DECL_F(func, ctx, elem, attr, body) \
    xmlerr func(void *ctx, oratext *elem, oratext *attr, oratext *body)

    XMLSAX_ATTR_DECL_F((*XMLSAX_ATTR_DECL_CB), ctx, elem, attr, body);

/*---------------------------------------------------------------------------
    NAME
        XmlSaxXmlDecl

    PURPOSE
        Receives SAX notification of an XML declaration.  Oracle extension,
        not in SAX standard.

    DESCRIPTION
        This event marks an XML declaration (XMLDecl).  The XmlSaxStartDocument
        event is always first; if this callback is registered and an XMLDecl
        exists, it will be the second event.  The encoding flag says whether
        an encoding was specified.  Since the document's own encoding
        specification may be overridden (or wrong), and the input will be
        converted to the data encoding anyway, the actual encoding specified
        in the document is not provided.  For the standalone flag, -1 will be
        returned if it was not specified, otherwise 0 for FALSE, 1 for TRUE.

    EXAMPLE
        <?xml version="1.0"?>
        <top/>
          --SAX Parse-->
        XmlSaxXmlDecl(version="1.0", encoding=FALSE)
        ...

    RETURNS
        (xmlerr) error code, XMLERR_OK [0] for success

    ARGUMENTS
        ctx        (IN) user's SAX context [see XmlLoadSax functions]
        version    (IN) version string from XMLDecl [data encoding]
        encoding   (IN) encoding was specified?
        standalone (IN) value of standalone-document flag [< 0 not specified]
*/
#define XMLSAX_XML_DECL_CB XmlSaxXmlDecl_xmlsaxcb

#define XMLSAX_XML_DECL_F(func, ctx, version, encoding, standalone) \
   xmlerr func(void *ctx, oratext *version, boolean encoding, sword standalone)

    XMLSAX_XML_DECL_F((*XMLSAX_XML_DECL_CB), ctx, ver, enc, std);

/*---------------------------------------------------------------------------
    NAME
        XmlSaxCDATA

    PURPOSE
        Receives SAX notification of CDATA.  Oracle extension, not in SAX
        standard.

    DESCRIPTION
        This event handles CDATA, as distinct from Text.  If no XmlSaxCDATA
        callback is provided, the Text callback will be invoked.  The data
        will be in the data encoding, and the returned length is in
        characters, not bytes.  See also XmlSaxWhitespace, which receiving
        notification about ignorable (whitespace formatting) character data.

    EXAMPLE
        <doc><![CDATA [ stuff]]></doc>
          --SAX Parse-->
        ...
        XmlSaxCDATA(ch=" stuff", len=6)
        ...

    RETURNS
        (xmlerr) error code, XMLERR_OK [0] for success

    ARGUMENTS
        ctx (IN) user's SAX context [see XmlLoadSax functions]
        ch  (IN) pointer to CDATA [data encoding]
        len (IN) length of CDATA [in characters]

    SEE ALSO
        XmlSaxWhitespace
*/
#define XMLSAX_CDATA_CB XmlSaxCDATA_xmlsaxcb

#define XMLSAX_CDATA_F(func, ctx, ch, len) \
    xmlerr func(void *ctx, oratext *ch, size_t len)

    XMLSAX_CDATA_F((*XMLSAX_CDATA_CB), ctx, ch, len);

/*---------------------------------------------------------------------------
    NAME
        XmlSaxParsedEntityDecl

    PURPOSE
        Receives SAX notification of a parsed entity declaration.  Oracle
        extension, not in SAX standard.

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
          --SAX Parse-->
        XmlSaxXmlDecl(version="1.0", encoding=NULL)
        XmlSaxParsedEntityDecl(name="e1", value="foobar", pubID=NULL,
                                 sysID=NULL, general=TRUE)
        XmlSaxParsedEntityDecl(name="e2", value=NULL, pubID=NULL,
                                 sysID="nop.ent", general=TRUE)
        XmlSaxParsedEntityDecl(name="32", value="parameter_entity_value",
                                pubID=NULL, sysID=NULL, general=FALSE)
        ...


    RETURNS
        (xmlerr) error code, XMLERR_OK [0] for success

    ARGUMENTS
        ctx     (IN) user's SAX context [see XmlLoadSax functions]
        name    (IN) entity's name [data encoding]
        value   (IN) entity's value if internal [data encoding]
        pubId   (IN) entity's public ID [data encoding] or NULL
        sysId   (IN) entity's system ID [data encoding]
        general (IN) general entity?  FALSE if parameter entity

    SEE ALSO
        XmlSaxUnparsedEntityDecl
*/
#define XMLSAX_PARSED_ENT_DECL_CB XmlSaxParsedEntityDecl_xmlsaxcb

#define XMLSAX_PARSED_ENT_DECL_F(func, ctx, name, value, pubId,sysId,general) \
    xmlerr func(void *ctx, oratext *name, oratext *value, \
               oratext *pubId, oratext *sysId, boolean general)

    XMLSAX_PARSED_ENT_DECL_F((*XMLSAX_PARSED_ENT_DECL_CB),
                             ctx, name, val, pubId, sysId, general);

/* The following 2 fields are reserved for future use. */
   void (*empty1_xmlsaxcb)();
   void (*empty2_xmlsaxcb)();
};

typedef struct xmlsaxcb xmlsaxcb;

#endif /* ifndef XMLSAXCB_DEFINED */

/*---------------------------------------------------------------------------
                                XML Context
  ---------------------------------------------------------------------------*/

/* This is the fixed header part of a conforming Unified DOM XML context
   which is exposed to the outside world; each implementation's XML context
   MUST BEGIN with one of these header parts, so that the function callbacks
   can be invoked.  The other parts of the header are private.
*/

typedef struct xmlctxhead
{
    ub4           cw_xmlctxhead;        /* checkword */
    oratext      *name_xmlctxhead;      /* name for context */
    xmlcb        *cb_xmlctxhead;        /* top-level function callbacks */
    xmldomcb     *domcb_xmlctxhead;     /* DOM function callbacks */
} xmlctxhead;

#define XML_CW(xctx)    ((xmlctxhead *) xctx)->cw_xmlctxhead
#define XML_NAME(xctx)  ((xmlctxhead *) xctx)->name_xmlctxhead
#define XML_CB(xctx)    ((xmlctxhead *) xctx)->cb_xmlctxhead
#define XML_DOMCB(xctx) ((xmlctxhead *) xctx)->domcb_xmlctxhead

/*---------------------------------------------------------------------------
                      Package XML - Top-Level APIs

    This C implementation of the XML processor (or parser) follows the W3C
    XML specification (rev REC-xml-19980210) and implements the required
    behavior of an XML processor in terms of how it must read XML data and
    the information it must provide to the application.

  ---------------------------------------------------------------------------*/

/****************************************************************************
                                Interface XML
 ****************************************************************************/

/*---------------------------------------------------------------------------
    NAME
        XmlCreateNew

    PURPOSE
        Create an XML Developer's Toolkit xmlctx

    DESCRIPTION
        Create an XML Developer's Toolkit xmlctx.  Properties common
        to all xmlctx's (both XDK and XMLType) are:

        ("data_encoding", name of data encoding)
            The encoding in which XML data will be presented through
            DOM and SAX.  If not specified, the default is UTF-8 (or
            UTF-E on EBCDIC platforms).  Note that single-byte encodings
            such as EBCDIC or ISO-8859 are substantially faster than
            multibyte encodings like UTF-8; Unicode (UTF-16) uses more
            memory but has better performance than multibyte.

BEGIN_NO_DOC
        ("data_lid", data encoding lid)
            The data encoding specified as an NLS lx_langid; the
            matching NLS global area must also be specified.
END_NO_DOC

        ("default_input_encoding", name of default input encoding)
            If the encoding of an input document cannot be automatically
            determined through BOM, XMLDecl, protocol header, etc, then
            this encoding will be assumed.

BEGIN_NO_DOC
        ("default_input_lid", default input encoding lid)
            The default input encoding specified as an NLS lx_langid;
            the matching NLS global area must also be specified.
END_NO_DOC

        ("error_language", error language or language.encoding)
            The language (and optional encoding) in which error
            messages are created.  The default is American with
            UTF-8 encoding.  To specify just the language, give
            the name of the language and nothing else ("American");
            To also specify the encoding, add a dot and the Oracle
            name of the encoding ("American.WE8ISO8859P1").

        ("error_handler", function pointer, see XML_ERRMSG_F)
            Default behavior on errors is to output the formatted
            message to stderr.  If an error handler is provided,
            the formatted message will be passed to it instead of
            being printed.

        ("error_context", user-defined context for error handler)
            This is a context pointer to be passed to the error
            handler function.  It's meaning is user-defined; it
            is just specified here and passed along when an error
            occurs.
        ("tracing_handler", function pointer, see XML_ERRTRC_F)
            If a tracing handler is provided, the tracing information
            will be formatted to a string and passed to the caller.

        ("input_encoding", name of forced input encoding)
            The forced input encoding for input documents.  Used to
            override a document's XMLDecl, etc, and always interpret
            it in the given encoding.  USE OF THIS FEATURE IS
            DISCOURAGED!  It should be not necessary in normal use,
            as BOMs, XMLDecls, etc, when existing, should be correct.

        ("memory_context",           MEMCTX,         POINTER),
            The oramem context used for low-level memory allocation.
            If not provided, one will be made.  From the outside,
            end-users have to set memory_alloc, memory_free, etc.

BEGIN_NO_DOC
        ("input_lid",                INLID,          POINTER),
            The forced input encopding

        ("lpu_context", lpu context)
            The LPU context used for URL data loading and
            access-method hooking.  If one is not provided, it
            will be made for you.  DEPRECATED!  Use XmlUrl and
            "url_context" instead!)

        ("url_context", XmlUrl context)
            The XmlUrl context used for URL data loading and
            access-method hooking.  If one is not provided, it
            will be made for you.)
END_NO_DOC

        ("memory_alloc", low-level memory allocation function)
            Low-level memory allocation function, if malloc is
            not to be used.  If provided, the matching free function
            must also be given.  See XML_ALLOC_F.

        ("memory_free", low-level memory freeing function)
            Low-level memory freeing function, if free is not to be
            used.  Matches the alloc function.

        ("memory_context", user-defined memory context)
            User-defined memory context which is passed to the alloc
            and free functions.  Its definition and use is entirely
            up to the user; it is just set here and passed to the
            callbacks.

BEGIN_NO_DOC
        ("nls_global_area", NLS global area, lx_glo)
            If any encoding are specified as NLS lids, the matching
            NLS global area must also be specified.
END_NO_DOC

        The XDK has properites of its own, that only apply to an XDK
        type xmlctx (the previous properies were all general and
        applied to all xmlctx's).  XDK properties must follow generic
        properties, with the symbol ORAPROP_SEP used as as separator,
        for example:

          ...,  "generic-name1", value1,
                "generic-name2", value2,
                ORAPROP_SEP,
                "xdk-specific-name1", value1,
                "xdk-specific-name2", value2,
                NULL

        ("input_buffer_size", size in characters of input buffer)
            This is the basic I/O buffer size.  Default is 256K,
            minimum is 4K and maximum is 4MB.  Depending on the
            encoding, 1, 2 or 3 of these buffers may be needed.
            Note size is in characters, not bytes.  If the buffer
            holds Unicode data, it will be twice as large.

        ("memory_block_size", size in bytes of memory allocation unit)
            This is the size of chunk the high-level memory package
            will request from the low-level allocator; i.e., the basic
            unit of memory allocation.  Default is 64K, minimum is 16K
            and maximum is 256K.

    RETURNS
        (xmlctx *) created xmlctx [or NULL on error with err set]

    ARGUMENTS
        err       (OUT) returned error code
        name      (IN)  name of context [for debug]
        wlist     (IN)  whitelist
        wcount    (IN)  # of strings in the whitelist
        wencoding (IN)  encoding of the whitelist (default UTF-8)
        ...       (IN)  variable arguments, with final NULL

    SEE ALSO
        XmlDestroy, XML_ERRMSG_F

    MEMORY USE
        Memory requirements for documents can be computed by the following
        formulas.  These formulas are used during testing, and memory use
        is verified to be within 2% of the expected use.

        Terms:  N = number of nodes (excluding attributes),
                A = number of attributes,
                L = number of lists (one for each node with children, one
                        for each element with attributes),
                D = data size in requested encoding (node and attribute
                        names & values, including terminating NULLs;
                        usually roughly input file size)
                B = buffer overhead, which is formed from:
                I = I/O buffer size (default 256K),
                M = memory block size (default 64K)
                S = SAX buffer (initial size 4K, grows to fit largest
                        set of event datums)

        DOM: ((N + A) * 60) + (L * 16) + D + B

            Single-byte encoding: B = I + (M * 2)
            Unicode (UTF-16):     B = (I * 3) + (M * 3)
            Multi-byte encoding:  B = (I * 5) + (M * 2)

        SAX: (N * 60) + (L * 16) + B

            Single-byte encoding: B = (I * 2) + (M * 2) + S
            Unicode (UTF-16):     B = (I * 5) + (M * 3) + S
            Multi-byte encoding:  B = (I * 5) + (M * 2) + S

        Note that for SAX, the only nodes used are for the DTD.  However,
        initial parse nodes are allocated in blocks of 1000.  Similarly,
        list structures used to maintain node children are allocated in
        blocks of 500.  If there is no DTD, there will be no node or list
        storage.
*/
xmlctx *XmlCreateNew(xmlerr *err, oratext *name, oratext **wlist, ub4 wcount,
                  oratext *wencoding, ...);
 
/*
 * XmlCreate API is deprecated. Please use XmlCreateNew 
 */
xmlctx *XmlCreate(xmlerr *err, oratext *name, ...);

/*---------------------------------------------------------------------------
    NAME
        XmlVersion

    PURPOSE
        Returns version string for XDK

    DESCRIPTION
        Returns the version string for the XDK

    RETURNS
        (oratext *) version string

    ARGUMENTS
        void
*/
oratext *XmlVersion(void);

/*---------------------------------------------------------------------------
    NAME
        XmlGetEncoding

    PURPOSE
        Returns data encoding in use by XML context

    DESCRIPTION
        Returns data encoding in use by XML context.  Ordinarily, the
        data encoding is chosen by the user, so this function is not
        needed.  However, if the data encoding is not specified, and
        allowed to default, this function can be used to return the
        name of that default encoding.

    RETURNS
        (oratext *) name of data encoding

    ARGUMENTS
        xctx (IN)  XML context

    SEE ALSO
        XmlDomGetDecl, XmlIsSimple, XmlIsUnicode
*/
oratext *XmlGetEncoding(xmlctx *xctx);

/*---------------------------------------------------------------------------
    NAME
        XmlIsSimple

    PURPOSE
        Returns single-byte (simple) character set flag

    DESCRIPTION
        Returns a flag saying whether the context's data encoding is
        "simple", i.e. single-byte per character, like ASCII or EBCDIC.

    RETURNS
        (boolean) TRUE of data encoding is "simple", FALSE otherwise

    ARGUMENTS
        xctx (IN)  XML context

    SEE ALSO
        XmlGetEncoding, XmlIsUnicode
*/
boolean XmlIsSimple(xmlctx *xctx);

/*---------------------------------------------------------------------------
    NAME
        XmlIsUnicode

    PURPOSE
        Returns XmlIsUnicode (simple) character set flag

    DESCRIPTION
        Returns a flag saying whether the context's data encoding is
        Unicode, i.e. UTF-16, with two-byte per character.

    RETURNS
        (boolean) TRUE of data encoding is Unicode, FALSE otherwise

    ARGUMENTS
        xctx (IN)  XML context

    SEE ALSO
        XmlGetEncoding, XmlIsSimple
*/
boolean XmlIsUnicode(xmlctx *xctx);

/*---------------------------------------------------------------------------
    NAME
        XmlSetExternalAccess

    PURPOSE
        Enable or disable file system and HTTP access.

    DESCRIPTION
        When the XML context is used to load documents, it is normally
        able to load them from the file system subject to operating system
        permissions, as well as via socket connections to HTTP sources.
        This allows callers to disable this type of access, limiting
        access to buffers and streams supplied by the program.

    RETURNS

    ARGUMENTS
        xctx       (IN)  XML context
        ext_access (IN)  TRUE to allow access, FALSE to disable access

    SEE ALSO
*/
void XmlSetExternalAccess(xmlctx *xctx, boolean ext_access);

/*---------------------------------------------------------------------------
    NAME
        XmlLoadDom

    STANDARD
        DOM 3: DOMBuilder.parse()

    PURPOSE
        Load (parse) an XML document and produce a DOM

    DESCRIPTION
        Loads (parses) an XML document from an input source and
        creates a DOM.  The root document node is returned on
        success, or NULL on failure (with err set).

        The function takes two fixed arguments, the xmlctx and an
        error return code, then zero or more (property, value)
        pairs, then NULL.

    SOURCE
        Input source is set by one of the following mutually
        exclusive properties (i.e. choose one):

        ("uri", document URI) [compiler encoding]

        ("file", document filesystem path) [compiler encoding]

        ("buffer", address of buffer,
         "buffer_length", # bytes in buffer)

        ("stream", address of stream object,
         "stream_context", pointer to stream object's context)

        ("events", pointer to xmlevctx structure)
        ("need_datacopy", if a datacopy is needed)  
          
        ("stdio", FILE* stream)

    PROPERTIES
        Additional properties:

        ("dtd", DTD node)
            DTD for document

        ("base_uri", document base URI)
            for documents loaded from other sources than a URI, sets
            the effective base URI.  the document's base URI is needed
            in order to resolve relative URI include, import, etc.

        ("input_encoding", encoding name)
            forced input encoding [name]

        ("default_input_encoding", encoding_name)
            default input encoding to assume if document is not
            self-describing (i.e. no BOM, protocol header, XMLDecl, etc)

        ("schema_location", string)
            schemaLocation of schema for this document.  used to figure
            optimal layout when loading documents into a database

        ("validate", boolean)
            when TRUE, turns on DTD validation; by default, only
            well-formedness is checked.  note that schema validation is a
            separate beast.

        ("discard_whitespace", boolean)
            when TRUE, formatting whitespace between elements (newlines
            and indentation) in input documents is discarded.  by default,
            ALL input characters are preserved.

        ("dtd_only", boolean)
            when TRUE, parses an external DTD, not a complete XML document.

        ("stop_on_warning", boolean)
            when TRUE, warnings are treated the same as errors and cause
            parsing, validation, etc, to stop immediately.  by default,
            warnings are issued but the game continues.

        ("warn_duplicate_entity", boolean)
            when TRUE, entities which are declared more than once will
            cause warnings to be issued.  the default is to accept the
            first declaration and silently ignore the rest.

        ("no_expand_char_ref", boolean)
            when TRUE, causes character references to be left unexpanded
            in the DOM data.  ordinarily, character references are replaced
            by the character they represent.  however, when a document is
            saved those characters entities do not reappear.  to way to
            ensure they remain through load and save is to not expand them.

        ("no_check_chars", boolean)
            when TRUE, omits the test of XML [2] Char production: all input
            characters will be accepted as valid

    RETURNS
        (xmldocnode *) document node on success [NULL on failure with err set]

    ARGUMENTS
        xctx (IN)  XML context
        err  (OUT) error code on failure
        ...  (IN)  variable arguments, with final NULL

    SEE ALSO
        XmlSaveDom
*/
xmldocnode *XmlLoadDom(xmlctx *xctx, xmlerr *err, ...);

/*---------------------------------------------------------------------------
    NAME
        XmlSaveDom

    STANDARD
        DOM 3: DOMWriter.writeNode

    PURPOSE
        Saves (serializes, formats) an XML document

    DESCRIPTION
        Serializes document or subtree to the given destination and
        returns the number of bytes written; if no destination is
        provided, just returns formatted size but does not output.

        If an output encoding is specified, the document will be re-encoded
        on output; otherwise, it will be in its existing encoding.

        The top level is indented step*level spaces, the next level
        step*(level+1) spaces, etc.

        When saving to a buffer, if the buffer overflows, 0 is returned
        and err is set to XMLERR_SAVE_OVERFLOW.

    DESTINATION
        Output destination is set by one of the following mutually
        exclusive properties (i.e. choose one):

        ("uri", document URI) POST, PUT? [compiler encoding]

        ("file", document filesystem path) [compiler encoding]

        ("buffer", address of buffer,
         "buffer_length", # bytes in buffer)

        ("stream", address of stream object,
         "stream_context", pointer to stream object's context)

        ("stdio", FILE* stream)

    PROPERTIES
        Additional properties:

        ("output_encoding", encoding name)
            name of final encoding for document.  unless specified, saved
            document will be in same encoding as xmlctx.

        ("indent_step", unsigned)
            spaces to indent each level of output.  default is 4, 0 means
            no indentation.

        ("indent_level", unsigned)
            initial indentation level.  default is 0, which means no
            indentation, flush left.

        ("no_format", boolean)
            Pretty print for DOM. default is FALSE. When this property 
            is set to be TRUE, the above two properties will be invalid.

        ("xmldecl", boolean)
            include an XMLDecl in the output document.  ordinarily an
            XMLDecl is onput output for a compete document (i.e. root
            node is DOC).

        ("bom", boolean)
            input a BOM in the output document.  usually the BOM is
            only needed for certain encodings (UTF-16), and optional
            for others (UTF-8).  causes optional BOMs to be output.

        ("prune", boolean)
            prunes the output like the unix 'find' command; does not
            not descend to children, just prints the one node given.

        ("eol", EOL string)
            specifies a custom string to be used at End of Line
            instead of the default <newline> character.

    RETURNS
        (ubig_ora) number of bytes written to destination

    ARGUMENTS
        xctx (IN)  XML context
        err  (OUT) error code on failure
        root (IN)  root node of subtree to save
        ...  (IN)  variable arguments, with final NULL

    SEE ALSO
        XmlLoadDom
*/
ubig_ora XmlSaveDom(xmlctx *xctx, xmlerr *err, xmlnode *root, ...);

/*---------------------------------------------------------------------------
    NAME
        XmlLoadSax

    PURPOSE
        Load (parse) an XML document from and produce SAX events

    DESCRIPTION
        Loads (parses) an XML document from an input source and
        generates a set of SAX events (as user callbacks).  Input
        sources and basic set of properties is the same as for
        XmlLoadDom.

    RETURNS
        (xmlerr) numeric error code, XMLERR_OK [0] on success

    ARGUMENTS
        xctx   (IN) XML context
        saxcb  (IN) SAX callback structure
        saxctx (IN) context to be passed to SAX callbacks
        ...    (IN) variable arguments, with final NULL
*/
xmlerr XmlLoadSax(xmlctx *xctx, xmlsaxcb *saxcb, void *saxctx, ...);

/*---------------------------------------------------------------------------
    NAME
        XmlLoadSaxVA

    PURPOSE
        Load (parse) an XML document from and produce SAX events [varargs]

    DESCRIPTION
        Loads (parses) an XML document from an input source and
        generates a set of SAX events (as user callbacks).  Input
        sources and basic set of properties is the same as for
        XmlLoadDom.

    RETURNS
        (xmlerr) numeric error code, XMLERR_OK [0] on success

    ARGUMENTS
        xctx   (IN) XML context
        saxcb  (IN) SAX callback structure
        saxctx (IN) context to pass to callbacks
        va     (IN) variable arguments, with final NULL
*/
xmlerr XmlLoadSaxVA(xmlctx *xctx, xmlsaxcb *saxcb, void *saxctx, va_list va);

/*---------------------------------------------------------------------------
    NAME
        XmlAccess

    PURPOSE
        Set access-method callbacks for URL

    DESCRIPTION
        Sets the stream used to load data for a specific URL access method.
        Overrides the built-in data loading functions for HTTP, FTP, etc, or
        provides functions to handle new types (UNKNOWN, etc).

    RETURNS
        (xmlerr) numeric error code, XMLERR_OK [0] on success

    ARGUMENTS
        xctx   (IN) XML context
        access (IN) URL access method
        stream (IN) stream data source

    SEE ALSO
        XmlLoadDom, XmlLoadSax
*/
xmlerr XmlAccess(xmlctx *xctx, xmlurlacc access, orastream *stream);

/*---------------------------------------------------------------------------
    NAME
        XmlConvert

    PURPOSE
        Convert compiler-encoding string to data encoding

    DESCRIPTION
        Converts a given string in the compiler encoding (ASCII or
        EBCDIC) to the XML context's data encoding, writing it to
        the given destination buffer.  The converted string will be
        NULL-terminated (or double-NULL terminated for UCS2/UTF-16),
        the dest pointer will be updated to point to the next position
        in the buffer, and size is updated to reflect the space used.
        The converted string is returned, but if the given string is
        already in the right encoding, it is just returned back and
        dest and size are not updated.

    RETURNS
        (oratext *) converted string, or NULL on error

    ARGUMENTS
        xctx (IN)  XML context
        src  (IN)  source string (in compiler encoding)
        dest (I/O) destination buffer
        size (I/O) size of destination buffer in bytes

    SEE ALSO
        XmlCreate
*/
oratext *XmlConvert(xmlctx *xctx, oratext *src, oratext **dest, ub4 *size);

/****************************************************************************
                           CALLBACK SECTION
 ****************************************************************************/

struct xmlcb
{
/*---------------------------------------------------------------------------
    NAME
        XmlDestroy

    PURPOSE
        Destroy an xmlctx

    DESCRIPTION
        Destroys an xmlctx

    RETURNS
        (void)

    ARGUMENTS
        xctx (IN) XML context

    SEE ALSO
        XmlCreate
*/
#define XML_DESTROY_CB destroy_xmlcb

#define XML_DESTROY_F(func, xctx) \
    void func(xmlctx *xctx)

    XML_DESTROY_F((*XML_DESTROY_CB), xctx);

#define XmlDestroy(xctx) \
    (*XML_CB(xctx)->XML_DESTROY_CB)(xctx)

/*---------------------------------------------------------------------------
    NAME
        XmlLoadDomVA

    PURPOSE
        Load (parse) an XML document and produce a DOM [varargs]

    STATUS
        Private

    RETURNS
        (xmldocnode *) document node on success [NULL on failure with err set]

    ARGUMENTS
        xctx (IN)  XML context
        err  (OUT) error code on failure
        va   (IN)  variable arguments

    SEE ALSO
        XmlLoadDOM
*/
#define XML_LOAD_DOM_VA_CB loadDomVA_xmlcb

#define XML_LOAD_DOM_VA_F(func, xctx, err, va) \
    xmldocnode *func(xmlctx *xctx, xmlerr *err, va_list va)

    XML_LOAD_DOM_VA_F((*XML_LOAD_DOM_VA_CB), xctx, err, va);

#define XmlLoadDomVA(xctx, err, va) \
    (*XML_CB(xctx)->XML_LOAD_DOM_VA_CB)((xctx), (err), (va))

/*---------------------------------------------------------------------------
    NAME
        XmlSaveDomVA

    PURPOSE
        Saves (serializes, formats) an XML document [varargs]

    STATUS
        Private

    RETURNS
        (ubig_ora) number of bytes written to destination

    ARGUMENTS
        xctx (IN)  XML context
        err  (OUT) error code on failure
        root (IN)  root node of subtree to save
        va   (IN)  variable arguments

    SEE ALSO
        XmlSaveDOM
*/
#define XML_SAVE_DOM_VA_CB saveDomVA_xmlcb

#define XML_SAVE_DOM_VA_F(func, xctx, err, root, va) \
    ubig_ora func(xmlctx *xctx, xmlerr *err, xmlnode *root, va_list va)

    XML_SAVE_DOM_VA_F((*XML_SAVE_DOM_VA_CB), xctx, err, root, va);

#define XmlSaveDomVA(xctx, err, root, va) \
    (*XML_CB(xctx)->XML_SAVE_DOM_VA_CB)((xctx), (err), (root), (va))

/*---------------------------------------------------------------------------
    NAME
        XmlHasFeature

    STANDARD
        DOM 2: DOMImplementation.hasFeature()

    PURPOSE
        Determine if DOM feature is implemented

    DESCRIPTION
        Determine if a DOM feature is implemented.  Returns TRUE if the
        feature is implemented in the specified version, FALSE otherwise.

        In level 1, the legal values for package are 'HTML' and 'XML'
        (case-insensitive), and the version is the string "1.0".  If the
        version is not specified, supporting any version of the feature
        will cause the method to return true.

        DOM 1.0 features are "XML" and "HTML".

        DOM 2.0 features are "Core", "XML", "HTML", "Views", "StyleSheets",
        "CSS", "CSS2", "Events", "UIEvents", "MouseEvents", "MutationEvents",
        "HTMLEvents", "Range", "Traversal"

     RETURNS
        (boolean) feature is implemented?

    ARGUMENTS
        xctx    (IN) XML context
        feature (IN) package name of the feature to test.
        version (IN) version # of the package name to test.
*/
#define XML_HAS_FEATURE_CB hasFeature_xmlcb

#define XML_HAS_FEATURE_F(func, xctx, feature, version) \
    boolean func(xmlctx *xctx, oratext *feature, oratext *version)

    XML_HAS_FEATURE_F((*XML_HAS_FEATURE_CB), xctx, feature, version);

#define XmlHasFeature(xctx, feature, version) \
    (*XML_CB(xctx)->XML_HAS_FEATURE_CB)((xctx), (feature), (version))

/*---------------------------------------------------------------------------
    NAME
        XmlCreateDTD

    STANDARD
        DOM 2: DOMImplementation.createDocumentType()

    PURPOSE
        Create DTD

    DESCRIPTION
        Create DTD.

    RETURNS
        (xmldtdnode *) new DTD node

    ARGUMENTS
        xctx  (IN)  XML context
        qname (IN)  qualified name
        pubid (IN)  external subset public identifier
        sysid (IN)  external subset system identifier
        err   (OUT) returned error code
*/
#define XML_CREATE_DTD_CB createDTD_xmlcb

#if 0 /* xxx TBD XXX */
#define XML_CREATE_DTD_F(func, xctx, qname, pubid, sysid, err) \
    xmldtdnode* func(xmlctx *xctx, oratext *qname, oratext *pubid, \
                     oratext *sysid, xmlerr *err)

    XML_CREATE_DTD_F((*XML_CREATE_DTD_CB), xctx, qname, pubid, sysid, err);
#else
#define XML_CREATE_DTD_F(func, xctx, qname, pubid, sysid) \
    xmldtdnode* func(xmlctx *xctx, oratext *qname, oratext *pubid, \
                     oratext *sysid, xmlerr *err)

    XML_CREATE_DTD_F((*XML_CREATE_DTD_CB), xctx, qname, pubid, sysid);
#endif

#define XmlCreateDTD(xctx, qname, pubid, sysid, err) \
    (*XML_CB(xctx)->XML_CREATE_DTD_CB) \
        ((xctx), (qname), (pubid), (sysid), (err))

/*---------------------------------------------------------------------------
    NAME
        XmlCreateDocument

    STANDARD
        DOM 2: DOMImplementation.createDocument()

    PURPOSE
        Create Document (node)

    DESCRIPTION
        Creates the initial top-level DOCUMENT node and its supporting
        infrastructure.  If a qname is provided, a an element with that
        name is created and set as the document's root element.

    RETURNS
        (xmldocnode *) new Document object.

    ARGUMENTS
        xctx  (IN)  XML context
        uri   (IN)  namespace URI of root element to create [or NULL]
        qname (IN)  QName of root element [or NULL if none]
        dtd   (IN)  associated DTD node
        err   (OUT) returned error code
*/
#define XML_CREATE_DOCUMENT_CB createDocument_xmlcb

#define XML_CREATE_DOCUMENT_F(func, xctx, uri, qname, dtd, err) \
    xmldocnode* func(xmlctx *xctx, oratext *uri, oratext *qname, \
                     xmldtdnode *dtd, xmlerr *err)

    XML_CREATE_DOCUMENT_F((*XML_CREATE_DOCUMENT_CB), xctx, uri, qname,dtd,err);

#define XmlCreateDocument(xctx, uri, qname, dtd, err) \
    (*XML_CB(xctx)->XML_CREATE_DOCUMENT_CB) \
        ((xctx), (uri), (qname), (dtd), (err))

/*---------------------------------------------------------------------------
    NAME
        XmlFreeDocument

    PURPOSE
        Free a document (releases all resources)

    DESCRIPTION
        Destroys a document created by XmlCreateDocument or through one
        of the Load functions.  Releases all resources associated with
        the document, which is then invalid.

    RETURNS
        (void)

    ARGUMENTS
        xctx (IN) XML context
        doc  (IN) document to free

    SEE ALSO
        XmlCreateDocument, XmlLoadDom
*/
#define XML_FREE_DOCUMENT_CB freeDocument_xmlcb

#define XML_FREE_DOCUMENT_F(func, xctx, doc) \
    void func(xmlctx *xctx, xmldocnode *doc)

    XML_FREE_DOCUMENT_F((*XML_FREE_DOCUMENT_CB), xctx, doc);

#define XmlFreeDocument(xctx, doc) \
    (*XML_CB(xctx)->XML_FREE_DOCUMENT_CB)((xctx), (doc))


/*---------------------------------------------------------------------------
    NAME
        XmlCreateDocumentWithMem

    STANDARD
        DOM 2: DOMImplementation.createDocumentWithMem()

    PURPOSE
        Create Document (node) using different memory management structure.

    DESCRIPTION
        Creates the initial top-level DOCUMENT node and its supporting
        infrastructure.  If a qname is provided, a an element with that
        name is created and set as the document's root element. However
        the memory that holds the DOM nodes for this document is controlled by 
        the input parameter xmlMemU that allows user to control where the
        memory region in which the doc tree resides in.

    RETURNS
        (xmldocnode *) new Document object.

    ARGUMENTS
        xctx  (IN)  XML context
        uri   (IN)  namespace URI of root element to create [or NULL]
        qname (IN)  QName of root element [or NULL if none]
        dtd   (IN)  associated DTD node
        err   (OUT) returned error code
        xmu   (IN)  Memory Ctx structure. See xmlMemU 
*/
#define XML_CREATE_DOCUMENT_WITHMEM_CB createDocumentWithMem_xmlcb

#define XML_CREATE_DOCUMENT_WITHMEM_F(func, xctx, uri, qname, dtd, xmu, err) \
    xmldocnode* func(xmlctx *xctx, oratext *uri, oratext *qname, \
                     xmldtdnode *dtd, xmlMemU *xmu, xmlerr *err)

    XML_CREATE_DOCUMENT_WITHMEM_F((*XML_CREATE_DOCUMENT_WITHMEM_CB), xctx, uri, qname,dtd,xmu,err);

#define XmlCreateDocumentWithMem(xctx, uri, qname, dtd, err, xmu) \
    (*XML_CB(xctx)->XML_CREATE_DOCUMENT_WITHMEM_CB) \
        ((xctx), (uri), (qname), (dtd), (xmu), (err))



/*---------------------------------------------------------------------------
    NAME
        XmlDocSetSecurityFunction

    STANDARD
        
    PURPOSE
        Set up the security violation callback function

    DESCRIPTION
        This func is called to set up security violation callback function
        for the DOM document node.
        When security violation occurs on a node that is part of the DOM
        document, this callback func is called
        with the following arguments (xmlnode *, void *obj, oratext *candVal).
        The xmlnode * points to the node that is subject to security check
        and void * points to the user callback context that is passed
        into XmlDocSetSecurityFunction() as the last argument.
        The oratext *candVal points to the candidate value that is 
        the real node value if security check were not applied.
        
        When callf() is NULL for the callback function, this means it
        clears any previous callback set for the DOM, and revert to the 
        default behaviour.


    RETURNS
        
        

    ARGUMENTS
        xctx (IN) XML document context
        doc (IN)  document node to which the SecurityFunction is applied
        callf(IN) callback function, the signature of the callback function
                   is oratext *(*)(xmlnode *, void *, oratext *)
        obj (IN)  callback function context

    SEE ALSO
        
*/

#define XML_DOCSET_SECURITY_FUNC_CB docset_security_func_xmlcb

#define XML_DOCSET_SECURITY_FUNC_F(func, xctx, doc, callf, obj) \
    void func(xmlctx *xctx, xmldocnode *doc, oratext *(*callf)(xmlnode *, void *, oratext *), void *obj)

    XML_DOCSET_SECURITY_FUNC_F((*XML_DOCSET_SECURITY_FUNC_CB), xctx, doc,callf, obj);

#define XmlDocSetSecurityFunction(xctx, doc, callf, obj)\
    (*XML_CB(xctx)->XML_DOCSET_SECURITY_FUNC_CB) \
        ((xctx), (doc), (callf), (obj))

/*---------------------------------------------------------------------------
    NAME
        XmlCreateDocumentWithMemFlag

    STANDARD
        DOM 2: DOMImplementation.createDocumentWithMemFlag()

    PURPOSE
        Create Document (node) using different memory management structure.

    DESCRIPTION
        Creates the initial top-level DOCUMENT node and its supporting
        infrastructure.  If a qname is provided, a an element with that
        name is created and set as the document's root element. However
        the memory that holds the DOM nodes for this document is controlled by 
        the input parameter xmlMemU that allows user to control where the
        memory region in which the doc tree resides in.

    RETURNS
        (xmldocnode *) new Document object.

    ARGUMENTS
        xctx  (IN)  XML context
        uri   (IN)  namespace URI of root element to create [or NULL]
        qname (IN)  QName of root element [or NULL if none]
        dtd   (IN)  associated DTD node
        err   (OUT) returned error code
        xmu   (IN)  Memory Ctx structure. See xmlMemU 
        flag  (IN)  flag (ONEFILE_DOM_MODE)
*/

#define ONEFILE_DOM_MODE 0x1

#define XML_CREATE_DOCUMENT_WITHMEM_FLAG_CB createDocumentWithMemFlag_xmlcb

#define XML_CREATE_DOCUMENT_WITHMEM_FLAG_F(func, xctx, uri, qname, dtd, xmu, err,flag) \
    xmldocnode* func(xmlctx *xctx, oratext *uri, oratext *qname, \
                     xmldtdnode *dtd, xmlMemU *xmu, xmlerr *err, ub4 flag)

    XML_CREATE_DOCUMENT_WITHMEM_FLAG_F((*XML_CREATE_DOCUMENT_WITHMEM_FLAG_CB), xctx, uri, qname,dtd,xmu,err, flag);

#define XmlCreateDocumentWithMemFlag(xctx, uri, qname, dtd, err, xmu, flag) \
    (*XML_CB(xctx)->XML_CREATE_DOCUMENT_WITHMEM_FLAG_CB) \
        ((xctx), (uri), (qname), (dtd), (xmu), (err), (flag))

};

/*---------------------------------------------------------------------------
                           Package DOM - DOM APIs

    Since the DOM standard is object-oriented, for this C adaptation, some
    changes had to be made:

    Reused function names had to be expanded, e.g. {getValue} in the
    attribute class is given the unique name XmlDomGetAttrValue, matching
    the pattern established by DOM 2's "{getNodeValue}".

    Also, some functions were added to extend the DOM.  For example,
    there is no function defined which returns the number of children
    of a node, so XmlDomNumChildNodes was invented, etc.

    The implementation of this DOM interface follows REC-DOM-Level-1-19981001.

  ---------------------------------------------------------------------------*/

/****************************************************************************
                                Interface Node
 ****************************************************************************/

/*---------------------------------------------------------------------------
    NAME
        XmlDomGetNodeValueStream

    PURPOSE
        Get node's value via orastream

    STATUS
        Oracle extension

    DESCRIPTION
        Returns the "value" (associated character data) for a node (in
        the data encoding) as an orastream, in either push or pull modes.
        Data may be retrieved as bytes or characters.

        Character and general entities will have been replaced.  Only Attr,
        CDATA, Comment, PI and Text nodes have values, all other node types
        have NULL value and XMLERR_NODE_TYPE will be returned if they are
        attempted to be read.

    PROPERTIES
        Control over the stream details is specified through a variable
        property list.  Any number of (name, value) pairs are specified,
        ending with a final NULL.

        ("character", boolean)
            By default, data is returned as raw bytes.  If the character
            property is set to TRUE, data will be returned in complete
            characters only.  That is, in character-mode, no character
            will be split over a buffer boundry, and all characters are
            guaranteed to be complete and contiguous within a buffer:
            instead of splitting a character which would have crossed a
            boundry, a short read will happen first, then the complete
            character is returned in the next buffer.  In byte mode, each
            buffer is completely filled, and characters may be split
            between two reads.  For single-byte encodings, there is no
            difference between byte and character modes.

        ("pull", orastream **)
            In pull-mode, a read-only orastream is returned; the property's
            value is a pointer to an (orastream *), which is set to the
            created orastream.  The node's value is then read with successive
            calls to OraStreamRead().  The stream will already have been
            opened, so no call to OraStreamOpen() is needed, and after the
            final piece is read, the stream will automatically be closed
            and destroyed.

        ("push", orastream *)
            In push-mode, a write-only orastream is supplied as the
            property value.  That stream will be successively written with
            OraStreamWrite() until the complete value is passed.  The stream
            will be opened with OraStreamOpen() before the first write, and
            closed with OraStreamClose() after the last write, but the user
            is responsible for creating and destroying the stream.

        ("stream_context", void *)
            Specifies the stream context for the pull-mode created stream.
            If not given, the stream's context will be NULL.

        Either "pull" or "push" must be specified, but not both.

    EXAMPLE
        "<foo>data</foo>"
        ORASTREAM_WRITE_F(my_writer, sctx, sid, hdl, src, size, written);
        oraerr     oerr;
        orastream *os = OraStreamInit(0xDEAD, 0xBEEF, &oerr,
                                      "write", my_writer, NULL);
        XmlDomGetNodeValueStream(xctx, <foo's first child, a TEXT node>,
                                 "character", TRUE, "push", os, NULL) -> 0 [OK]
        Which causes the following invocation:
            my_writer(0xDEAD, 0xBEEF, hdl, "data", 4, written);
        The writer is responsible for setting <written>, see orastruc.h.

    RETURNS
        (xmlerr) error code, XMLERR_OK [0] for success

    ARGUMENTS
        xctx (IN) XML context
        node (IN) XML node
        ...  (IN) variable arguments specifying retrival mode, ending in NULL

    SEE ALSO
        XmlDomGetNodeValue, XmlDomGetNodeValueLen
*/
xmlerr XmlDomGetNodeValueStream(xmlctx *xctx, xmlnode *node, ...);

/*---------------------------------------------------------------------------
    NAME
        XmlDomSetNodeValueStream

    PURPOSE
        Set node's value via orastream

    STATUS
        Oracle extension

    DESCRIPTION
        Sets the "value" (associated character data) for a node with an
        orastream in either push or pull modes.  Data may be set as bytes
        or characters, but must be in the data encoding.

        Only Attr, CDATA, Comment, PI and Text nodes have values; trying
        to set the value of another type of node is an error and
        XMLERR_NODE_TYPE will be returned.

    PROPERTIES
        Control over the stream details is specified through a variable
        property list.  Any number of (name, value) pairs are specified,
        ending with a final NULL.

        ("character", boolean)
            By default, data is set as raw bytes.  If the character
            property is set to TRUE, data will be passed in complete
            characters only.  That is, in character-mode, no character
            may be split over a buffer boundry, and all characters must
            be complete and contiguous within a buffer: instead of
            splitting a character which would cross a boundry, a short
            write should happen first, then the complete character is
            set in the next buffer.  In byte mode, each buffer is
            completely filled, and characters may be split between writes.
            For single-byte encodings, there is no difference between
            byte and character modes.

        ("pull", orastream *)
            In pull-mode, a read-only orastream is provided by the user.
            The node's value is then read with successive calls to
            OraStreamRead().  The stream will be opened with OraStreamOpen()
            before the first read, and closed with OraStreamClose() after
            the last read, but the user is responsible for creating and
            destroying the stream.

        ("push", orastream **)
            In push-mode, a write-only orastream is returned; the property's
            value is a pointer to an (orastream *), which is set to the
            created orastream.  The node's value is then written with
            successive calls to OraStreamWrite().  The stream will already
            have been opened, so no call to OraStreamOpen() is needed, and
            after the final piece is written, the stream will automatically
            be closed and destroyed.

        ("stream_context", void *)
            Specifies the stream context for the push-mode created stream.
            If not given, the stream's context will be NULL.

        Either "pull" or "push" must be specified, but not both.

    EXAMPLE
        "<foo>data</foo>"
        ORASTREAM_READ_F(my_reader, func, sctx, sid, hdl,
                         dest, size, start, nread, eoi);
        oraerr     oerr;
        orastream *is = OraStreamInit(0xDEAD, 0xBEEF, &oerr,
                                      "read", my_reader, NULL);
        XmlDomSetNodeValueStream(xctx, <foo's first child, a TEXT node>,
                                 "pull", is, NULL) -> 0 [OK]
        Which causes the following invocation:
            my_reader(0xDEAD, 0xBEEF, hdl, <buffer>, <buffer_size>,
                      start, nread, eoi);
        The reader is responsible for setting <start>, <nread>, and <eoi>,
        (see orastruc.h).

    RETURNS
        (xmlerr) error code, XMLERR_OK [0] for success

    ARGUMENTS
        xctx (IN) XML context
        node (IN) XML node
        ...  (IN) variable arguments specifying retrival mode, ending in NULL

    SEE ALSO
        XmlDomSetNodeValue, XmlDomSetNodeValueLen
*/
xmlerr XmlDomSetNodeValueStream(xmlctx *xctx, xmlnode *node, ...);

/****************************************************************************
                                Interface Attr
 ****************************************************************************/

/*---------------------------------------------------------------------------
    NAME
        XmlDomGetAttrValueStream

    PURPOSE
        Returns an attribute's value via orastream

    STATUS
        Oracle extension

    DESCRIPTION
        Returns the "value" (associated character data) of an attribute
        (in the data encoding) as an orastream, in either push or pull
        modes.  Data may be retrieved as bytes or characters, but must
        be in the data encoding.

    PROPERTIES
        Control over the stream details is specified through a variable
        property list.  Any number of (name, value) pairs are specified,
        ending with a final NULL.

        ("character", boolean)
            By default, data is returned as raw bytes.  If the character
            property is set to TRUE, data will be returned in complete
            characters only.  That is, in character-mode, no character
            will be split over a buffer boundry, and all characters are
            guaranteed to be complete and contiguous within a buffer:
            instead of splitting a character which would have crossed a
            boundry, a short read will happen first, then the complete
            character is returned in the next buffer.  In byte mode, each
            buffer is completely filled, and characters may be split
            between two reads.  For single-byte encodings, there is no
            difference between byte and character modes.

        ("pull", orastream **)
            In pull-mode, a read-only orastream is returned; the property's
            value is a pointer to an (orastream *), which is set to the
            created orastream.  The attribute's value is then read with
            successive calls to OraStreamRead().  The stream will already
            have been opened, so no call to OraStreamOpen() is needed, and
            after the final piece is read, the stream will automatically be
            closed and destroyed.

        ("push", orastream *)
            In push-mode, a write-only orastream is supplied as the
            property value.  That stream will be successively written with
            OraStreamWrite() until the complete value is passed.  The stream
            will be opened with OraStreamOpen() before the first write, and
            closed with OraStreamClose() after the last write, but the user
            is responsible for creating and destroying the stream.

        ("stream_context", void *)
            Specifies the stream context for the pull-mode created stream.
            If not given, the stream's context will be NULL.

        Either "pull" or "push" must be specified, but not both.

    EXAMPLE
        "<foo x='data'/>"
        ORASTREAM_READ_F(my_reader, func, sctx, sid, hdl,
                         dest, size, start, nread, eoi);
        orastream *is;
        XmlDomGetAttrValueStream(xctx, <foo's attribute "x">,
                                 "character", TRUE, "pull", &is, NULL) -> 0
        The data is then read with calls like:
            OraStreamRead(is, dest, size, &start, &nread, &eoi);
        See orastruc.h for orastream details.

    RETURNS
        (xmlerr) error code, XMLERR_OK [0] for success

    ARGUMENTS
        xctx (IN) XML context
        attr (IN) XML attribute node
        ...  (IN) variable arguments specifying retrival mode, ending in NULL

    SEE ALSO
        XmlDomGetAttrValue, XmlDomGetAttrValueLen
*/
xmlerr XmlDomGetAttrValueStream(xmlctx *xctx, xmlattrnode *attr, ...);

/*---------------------------------------------------------------------------
    NAME
        XmlDomSetAttrValueStream

    PURPOSE
        Sets an attribute's value via orastream

    STATUS
        Oracle extension

    DESCRIPTION
        Sets the "value" (associated character data) for an attribute with
        an orastream in either push or pull modes.  Data may be set as bytes
        or characters, but must be in the data encoding.

    PROPERTIES
        Control over the stream details is specified through a variable
        property list.  Any number of (name, value) pairs are specified,
        ending with a final NULL.

        ("character", boolean)
            By default, data is set as raw bytes.  If the character
            property is set to TRUE, data will be passed in complete
            characters only.  That is, in character-mode, no character
            may be split over a buffer boundry, and all characters must
            be complete and contiguous within a buffer: instead of
            splitting a character which would cross a boundry, a short
            write should happen first, then the complete character is
            set in the next buffer.  In byte mode, each buffer is
            completely filled, and characters may be split between writes.
            For single-byte encodings, there is no difference between
            byte and character modes.

        ("pull", orastream *)
            In pull-mode, a read-only orastream is provided by the user.
            The attribute's value is then read with successive calls to
            OraStreamRead().  The stream will be opened with OraStreamOpen()
            before the first read, and closed with OraStreamClose() after
            the last read, but the user is responsible for creating and
            destroying the stream.

        ("push", orastream **)
            In push-mode, a write-only orastream is returned; the property's
            value is a pointer to an (orastream *), which is set to the
            created orastream.  The attribute's value is then written with
            successive calls to OraStreamWrite().  The stream will already
            have been opened, so no call to OraStreamOpen() is needed, and
            after the final piece is written, the stream will automatically
            be closed and destroyed.

        ("stream_context", void *)
            Specifies the stream context for the push-mode created stream.
            If not given, the stream's context will be NULL.

        Either "pull" or "push" must be specified, but not both.

    EXAMPLE
        "<foo x='data'/>"
        ORASTREAM_READ_F(my_reader, func, sctx, sid, hdl,
                         dest, size, start, nread, eoi);
        oraerr     oerr;
        orastream *is = OraStreamInit(0xDEAD, 0xBEEF, &oerr,
                                      "read", my_reader, NULL);
        XmlDomSetAttrValueStream(xctx, <foo's attribute "x">,
                                 "pull", is, NULL) -> 0 [OK]
        Which causes the following invocation:
            my_reader(0xDEAD, 0xBEEF, hdl, <buffer>, <buffer_size>,
                      start, nread, eoi);
        The reader is responsible for setting <start>, <nread>, and <eoi>,
        (see orastruc.h).

    RETURNS
        (xmlerr) error code, XMLERR_OK [0] for success

    ARGUMENTS
        xctx (IN) XML context
        attr (IN) XML attribute node
        ...  (IN) variable arguments specifying retrival mode, ending in NULL

    SEE ALSO
        XmlDomSetAttrValue
*/
xmlerr XmlDomSetAttrValueStream(xmlctx *xctx, xmlattrnode *attr, ...);

/****************************************************************************
                               CALLBACK SECTION
 ****************************************************************************/

struct xmldomcb
{

/****************************************************************************
                               Interface Document
 ****************************************************************************/

/*---------------------------------------------------------------------------
    NAME
        XmlDomGetDecl

    PURPOSE
        Returns a document's XMLDecl information

    DESCRIPTION
        Returns the information from a document's XMLDecl.  If there is
        no XMLDecl, returns XMLERR_NO_DECL.  Returned are the XML version#
        ("1.0" or "2.0"), the specified encoding, and the standalone value.
        If encoding is not specified, NULL will be set.  The standalone flag
        is three-state: < 0 if standalone was not specified, 0 if it was
        specified and FALSE, > 0 if it was specified and TRUE.

    RETURNS
        (xmlerr) XML error code, perhaps version/encoding/standalone set

    ARGUMENTS
        xctx (IN)  XML context
        doc  (IN)  XML document [node]
        ver  (OUT) XML version
        enc  (OUT) encoding specification
        std  (OUT) standalone specification
*/
#define XMLDOM_GET_DECL_CB getDecl_xmldomcb

#define XMLDOM_GET_DECL_F(func, xctx, doc, ver, enc, std) \
    xmlerr func(xmlctx *xctx, xmldocnode *doc, oratext **ver, \
                oratext **enc, sb4 *std)

    XMLDOM_GET_DECL_F((*XMLDOM_GET_DECL_CB), xctx, doc, ver, enc, std);

#define XmlDomGetDecl(xctx, doc, ver, enc, std) \
    (*XML_DOMCB(xctx)->XMLDOM_GET_DECL_CB) \
        ((xctx), (doc), (ver), (enc), (std))

/*---------------------------------------------------------------------------
    NAME
        XmlDomGetBaseURI

    STANDARD
        DOM 3: Node.baseURI

    PURPOSE
        Returns the base URI for a document

    DESCRIPTION
        Returns the base URI for a document.  Usually only documents that
        were loaded from a URI will automatically have a base URI; documents
        loaded from other sources (stdin, buffer, etc) will not naturally
        have a base URI, but a base URI may have been set for them using
        XmlDomSetBaseURI, for the purposes of resolving relative URIs in
        inclusion.

    RETURNS
        (oratext *) document's base URI [or NULL]

    ARGUMENTS
        xctx (IN) XML context
        doc  (IN) XML document [node]

    SEE ALSO
        XmlDomSetBaseURI
*/
#define XMLDOM_GET_BASE_URI_CB getBaseURI_xmldomcb

#define XMLDOM_GET_BASE_URI_F(func, xctx, doc) \
    oratext *func(xmlctx *xctx, xmldocnode *doc)

    XMLDOM_GET_BASE_URI_F((*XMLDOM_GET_BASE_URI_CB), xctx, doc);

#define XmlDomGetBaseURI(xctx, doc) \
    (*XML_DOMCB(xctx)->XMLDOM_GET_BASE_URI_CB) \
        ((xctx), (doc))

/*---------------------------------------------------------------------------
    NAME
        XmlDomSetBaseURI

    STANDARD
        DOM 3: Document.setBaseURI()

    PURPOSE
        Sets base URI for document

    DESCRIPTION
        Only documents that were loaded from a URI will automatically
        have a base URI; documents loaded from other sources (stdin,
        buffer, etc) will not naturally have a base URI, so this API
        is used to set a base URI, for the purposes of relative URI
        resolution in includes.  The base URI should be in the data
        encoding, and a copy will be made.

    EXAMPLE
        A document is loaded with XmlLoadDomStdio from stdin; that
        document as a DOCTYPE declaration, which is always a URI.
        The DTD's URI is relative, i.e. "subdir/junk.dtd".  Where
        is the DTD to be found?  Its URI is resolved against the
        parent's URI, but the parent doesn't have a URI in this
        case, so no resolution takes place, and "subdir/junk.dtd"
        is tried directly, which may or may not work depending on
        the working directory.  To make this work always, set the
        base URI of the top-level document to an absolute path,
        for example to "/root/dir1/dir2/topdoc.html"; then the URI
        resolution of the included DTD would produce
        "/root/dir1/dir2/subdir/junk.dtd" every time.

    RETURNS
        (xmlerr) XML error code

    ARGUMENTS
        xctx (IN) XML context
        doc  (IN) XML document [node]
        uri  (IN) base URI to set [data encoding]

    SEE ALSO
        XmlDomGetBaseURI
*/
#define XMLDOM_SET_BASE_URI_CB setBaseURI_xmldomcb

#define XMLDOM_SET_BASE_URI_F(func, xctx, doc, uri) \
    xmlerr func(xmlctx *xctx, xmldocnode *doc, oratext *uri)

    XMLDOM_SET_BASE_URI_F((*XMLDOM_SET_BASE_URI_CB), xctx, doc, uri);

#define XmlDomSetBaseURI(xctx, doc, uri) \
    (*XML_DOMCB(xctx)->XMLDOM_SET_BASE_URI_CB) \
        ((xctx), (doc), (uri))

/*---------------------------------------------------------------------------
    NAME
        XmlDomGetDTD

    STANDARD
        DOM 2: Document.doctype

    PURPOSE
        Get DTD (Document Type) for document

    DESCRIPTION
        Returns the DTD node associated with current document; if there
        is no DTD, returns NULL.  The DTD cannot be edited, but its
        children may be retrieved with XmlDomGetChildNodes as for other
        node types.

    RETURNS
        (xmldtdnode *) DTD node for document [or NULL]

    ARGUMENTS
        xctx (IN) XML context
        doc  (IN) XML document [node]

    SEE ALSO
        XmlDomSetDTD, XmlCreateDTD, XmlCreateDocument, XmlDomGetDTDName,
        XmlDomGetDTDEntities, XmlDomGetDTDNotations
*/
#define XMLDOM_GET_DTD_CB getDTD_xmldomcb

#define XMLDOM_GET_DTD_F(func, xctx, doc) \
    xmldtdnode* func(xmlctx *xctx, xmldocnode *doc)

    XMLDOM_GET_DTD_F((*XMLDOM_GET_DTD_CB), xctx, doc);

#define XmlDomGetDTD(xctx, doc) \
    (*XML_DOMCB(xctx)->XMLDOM_GET_DTD_CB) \
        ((xctx), (doc))

/*---------------------------------------------------------------------------
    NAME
        XmlDomSetDTD

    PURPOSE
        Sets DTD (Document Type Definition) for document

    DESCRIPTION
        Sets the DTD for document.  Note this call may only be used
        for a blank document, BEFORE any parsing has taken place.  A
        single DTD can be set for multiple documents, so when a document
        with a set DTD is freed, the set DTD is NOT also freed.

    RETURNS
        (xmlerr) numeric error code, 0 on success

    ARGUMENTS
        xctx    (IN) XML context
        doc     (IN) XML document [node]
        dtdnode (IN) Document Type to set [node]

    SEE ALSO
        XmlDomGetDTD, XmlDomGetDTDName, XmlDomGetDTDEntities,
        XmlDomGetDTDNotations
*/
#define XMLDOM_SET_DTD_CB setDTD_xmldomcb

#define XMLDOM_SET_DTD_F(func, xctx, doc, dtdnode) \
    xmlerr func(xmlctx *xctx, xmldocnode *doc, xmldtdnode *dtdnode)

    XMLDOM_SET_DTD_F((*XMLDOM_SET_DTD_CB), xctx, doc, dtdnode);

#define XmlDomSetDTD(xctx, doc, dtdnode) \
    (*XML_DOMCB(xctx)->XMLDOM_SET_DTD_CB) \
        ((xctx), (doc), (dtdnode))

/*---------------------------------------------------------------------------
    NAME
        XmlDomGetDocElem

    STANDARD
        DOM 2: Document.documentElement

    PURPOSE
        Get top-level element for document

    DESCRIPTION
        Returns the root element (node) of the DOM tree, or NULL if there
        is none.  Each document has only one uppermost Element node, called
        the root element.  It is created after a document is parsed
        successfully, or manually by XmlDomCreateElem then XmlDomAppendChild,
        etc.

    RETURNS
        (xmlelemnode *) root element [or NULL]

    ARGUMENTS
        xctx (IN) XML context
        doc  (IN) XML document [node]

    SEE ALSO
        XmlDomCreateElem
*/
#define XMLDOM_GET_DOCUMENT_ELEM_CB getDocumentElem_xmldomcb

#define XMLDOM_GET_DOCUMENT_ELEM_F(func, xctx, doc) \
    xmlelemnode* func(xmlctx *xctx, xmldocnode *doc)

    XMLDOM_GET_DOCUMENT_ELEM_F((*XMLDOM_GET_DOCUMENT_ELEM_CB), xctx, doc);

#define XmlDomGetDocElem(xctx, doc) \
    (*XML_DOMCB(xctx)->XMLDOM_GET_DOCUMENT_ELEM_CB) \
        ((xctx), (doc))

/*---------------------------------------------------------------------------
    NAME
        XmlDomSync

    PURPOSE
        Synchronizes the persistent version of a document with its DOM.

    DESCRIPTION
        Causes a modified DOM to be written back out to its original
        source, synchronizing the persistent store and in-memory versions.

    RETURNS
        (xmlerr) numeric error code, 0 on success

    ARGUMENTS
        xctx (IN) XML context
        doc  (IN) XML document [node]
*/
#define XMLDOM_SYNC_CB syncDoc_xmldomcb

#define XMLDOM_SYNC_F(func, xctx, doc) \
    xmlerr func(xmlctx *xctx, xmldocnode *doc)

    XMLDOM_SYNC_F((*XMLDOM_SYNC_CB), xctx, doc);

#define XmlDomSync(xctx, doc) \
    (*XML_DOMCB(xctx)->XMLDOM_SYNC_CB) \
        ((xctx), (doc))

/*---------------------------------------------------------------------------
    NAME
        XmlDomCreateElem

    STANDARD
        DOM 2: Document.createElement()

    PURPOSE
        Create an element node

    DESCRIPTION
        Creates an element node with the given tag name (which should be in
        the data encoding).  Note that the tag name of an element is CASE
        SENSITIVE.  This is the non-namespace aware function: the new node
        will have NULL namespace URI and prefix, and its local name will be
        the same as its tag name, even if the tag name specified is a QName.

        The new node is an orphan with no parent; it must be added to the DOM
        tree with XmlDomAppendChild etc.

        The tagname is NOT copied, its pointer is just stored.  The user is
        responsible for persistence and freeing of that data.

    RETURNS
        (xmlelemnode *) new Element node.

    ARGUMENTS
        xctx    (IN) XML context
        doc     (IN) XML document [node]
        tagname (IN) new node's name [data encoding; user control]

    SEE ALSO
        XmlDomCreateElemNS, XmlDomCleanNode, XmlDomFreeNode
*/
#define XMLDOM_CREATE_ELEM_CB createElem_xmldomcb

#define XMLDOM_CREATE_ELEM_F(func, xctx, doc, tagname) \
    xmlelemnode* func(xmlctx *xctx, xmldocnode *doc, oratext *tagname)

    XMLDOM_CREATE_ELEM_F((*XMLDOM_CREATE_ELEM_CB), xctx, doc, tagname);

#define XmlDomCreateElem(xctx, doc, tagname) \
    (*XML_DOMCB(xctx)->XMLDOM_CREATE_ELEM_CB) \
        ((xctx), (doc), (tagname))

/*---------------------------------------------------------------------------
    NAME
        XmlDomCreateElemNS

    STANDARD
        DOM 2: Document.createElementNS()

    PURPOSE
        Create an element node with namespace information

    DESCRIPTION
        Creates an element with the given namespace URI and QName.  Note
        that element names are CASE SENSITIVE, and the QName is required
        though the URI may be NULL.  The QName will be split into prefix and
        local parts, retrievable with XmlDomGetNodePrefix, XmlDomGetNodeLocal,
        etc; the tagName will be the full QName.

        The new node is an orphan with no parent; it must be added to the
        DOM tree with XmlDomAppendChild etc.

        The URI and QName are NOT copied, their pointers are just stored.
        The user is responsible for persistence and freeing of that data.

    RETURNS
        (xmlelemnode *) new Element node.

    ARGUMENTS
        xctx  (IN) XML context
        doc   (IN) XML document [node]
        uri   (IN) new node's namespace URI [data encoding; user control]
        qname (IN) new node's qualified name [data encoding; user control]

    SEE ALSO
        XmlDomCreateElem, XmlDomCleanNode, XmlDomFreeNode
*/
#define XMLDOM_CREATE_ELEM_NS_CB createElemNS_xmldomcb

#define XMLDOM_CREATE_ELEM_NS_F(func, xctx, doc, uri, qname) \
    xmlelemnode* func(xmlctx *xctx, xmldocnode *doc, \
                      oratext *uri, oratext *qname)

    XMLDOM_CREATE_ELEM_NS_F((*XMLDOM_CREATE_ELEM_NS_CB), xctx, doc, uri,qname);

#define XmlDomCreateElemNS(xctx, doc, uri, qname) \
    (*XML_DOMCB(xctx)->XMLDOM_CREATE_ELEM_NS_CB) \
        ((xctx),(doc),(uri),(qname))

/*---------------------------------------------------------------------------
    NAME
        XmlDomCreateFragment

    STANDARD
        DOM 2: Document.createDocumentFragment()

    PURPOSE
        Create a document fragment

    DESCRIPTION
        Creates an empty Document Fragment node.  A document fragment is
        treated specially when it is inserted into a DOM tree: the children
        of the fragment are inserted in order instead of the fragment node
        itself.  After insertion, the fragment node will still exist, but
        have no children.  See XmlDomInsertBefore, XmlDomReplaceChild,
        XmlDomAppendChild, etc.  The name of a fragment node is always
        "#document-fragment".

    RETURNS
        (xmlfragnode *) new empty DocumentFragment node

    ARGUMENTS
        xctx (IN) XML context
        doc  (IN) XML document [node]

    SEE ALSO
        XmlDomInsertBefore, XmlDomReplaceChild, XmlDomAppendChild
*/
#define XMLDOM_CREATE_FRAGMENT_CB createFragment_xmldomcb

#define XMLDOM_CREATE_FRAGMENT_F(func, xctx, doc) \
    xmlfragnode* func(xmlctx *xctx, xmldocnode *doc)

    XMLDOM_CREATE_FRAGMENT_F((*XMLDOM_CREATE_FRAGMENT_CB), xctx, doc);

#define XmlDomCreateFragment(xctx, doc) \
    (*XML_DOMCB(xctx)->XMLDOM_CREATE_FRAGMENT_CB) \
        ((xctx), (doc))

/*---------------------------------------------------------------------------
    NAME
        XmlDomCreateText

    STANDARD
        DOM 2: Document.createTextNode()

    PURPOSE
        Create text node

    DESCRIPTION
        Creates a Text node with the given initial data (which must be
        non-NULL and in the data encoding).  The data may be NULL; if
        provided, it is not verified, converted, checked, or parsed
        (entities will not be expanded).  If bad data is set, bad things
        will happen-- GIGO!  The name of a fragment node is always "#text".
        New data for a Text node can be set with XmlDomSetNodeValue; see
        the CharacterData interface for editing methods.

        The new node is an orphan with no parent; it must be added to
        the DOM tree with XmlDomAppendChild etc.

        The text data is NOT copied, its pointer is just stored.  The user
        is responsible for persistence and freeing of that data.

    RETURNS
        (xmltextnode *) new Text node.

    ARGUMENTS
        xctx (IN) XML context
        doc  (IN) XML document [node]
        data (IN) new node's text [data encoding; user control]

    SEE ALSO
        XmlDomCreateCDATA, XmlDomSetNodeValue, XmlDomGetNodeValue,
        XmlDomSetCharData, XmlDomGetCharData, XmlDomGetCharDataLength,
        XmlDomSubstringData, XmlDomAppendData, XmlDomInsertData,
        XmlDomDeleteData, XmlDomReplaceData, XmlDomCleanNode, XmlDomFreeNode
*/
#define XMLDOM_CREATE_TEXT_NODE_CB createText_xmldomcb

#define XMLDOM_CREATE_TEXT_NODE_F(func, xctx, doc, data) \
    xmltextnode* func(xmlctx *xctx, xmldocnode *doc, oratext *data)

    XMLDOM_CREATE_TEXT_NODE_F((*XMLDOM_CREATE_TEXT_NODE_CB), xctx, doc, data);

#define XmlDomCreateText(xctx, doc, data) \
    (*XML_DOMCB(xctx)->XMLDOM_CREATE_TEXT_NODE_CB) \
        ((xctx), (doc), (data))

/*---------------------------------------------------------------------------
    NAME
        XmlDomCreateComment

    STANDARD
        DOM 2: Document.createComment()

    PURPOSE
        Create comment node

    DESCRIPTION
        Creates a Comment node with the given initial data (which must be
        in the data encoding).  The data may be NULL; if provided, it is not
        verified, converted, or checked.  If bad data is set, bad things may
        happen-- GIGO!  The name of a Comment node is always "#comment".

        The new node is an orphan with no parent; it must be added to the DOM
        tree with XmlDomAppendChild etc.

        The comment data is NOT copied, its pointer is just stored.  The user
        is responsible for persistence and freeing of that data.

    RETURNS
        (xmlcommentnode *) new Comment node.

    ARGUMENTS
        xctx (IN) XML context
        doc  (IN) XML document [node]
        data (IN) new node's comment [data encoding; user control]

    SEE ALSO
        XmlDomCleanNode, XmlDomFreeNode
*/
#define XMLDOM_CREATE_COMMENT_CB createComment_xmldomcb

#define XMLDOM_CREATE_COMMENT_F(func, xctx, doc, data) \
    xmlcommentnode* func(xmlctx *xctx, xmldocnode *doc, oratext *data)

    XMLDOM_CREATE_COMMENT_F((*XMLDOM_CREATE_COMMENT_CB), xctx, doc, data);

#define XmlDomCreateComment(xctx, doc, data) \
    (*XML_DOMCB(xctx)->XMLDOM_CREATE_COMMENT_CB) \
        ((xctx), (doc), (data))

/*---------------------------------------------------------------------------
    NAME
        XmlDomCreateCDATA

    STANDARD
        DOM 2: Document.createCDATASection()

    PURPOSE
        Create CDATA node

    DESCRIPTION
        Creates a CDATASection node with the given initial data (which
        should be in the data encoding).  A CDATA section is considered
        verbatim and is never parsed; it will not be joined with adjacent
        Text nodes by the normalize operation.  The initial data may be
        NULL; if provided, it is not verified, converted, or checked.  If
        bad data is set, bad things will happen-- GIGO!  The name of a
        CDATA node is always "#cdata-section".

        The new node is an orphan with no parent; it must be added to the
        DOM tree with XmlDomAppendChild etc.

        The CDATA is NOT copied, its pointer is just stored.  The user is
        responsible for persistence and freeing of that data.

    RETURNS
        (xmlcdatanode *) new CDATA node.

    ARGUMENTS
        xctx (IN) XML context
        doc  (IN) XML document [node]
        data (IN) new node's CDATA [data encoding; user control]

    SEE ALSO
        XmlDomCreateText, XmlDomCleanNode, XmlDomFreeNode
*/
#define XMLDOM_CREATE_CDATA_CB createCDATA_xmldomcb

#define XMLDOM_CREATE_CDATA_F(func, xctx, doc, data) \
    xmlcdatanode* func(xmlctx *xctx, xmldocnode *doc, oratext *data)

    XMLDOM_CREATE_CDATA_F((*XMLDOM_CREATE_CDATA_CB), xctx, doc, data);

#define XmlDomCreateCDATA(xctx, doc, data) \
    (*XML_DOMCB(xctx)->XMLDOM_CREATE_CDATA_CB) \
        ((xctx), (doc), (data))

/*---------------------------------------------------------------------------
    NAME
        XmlDomCreatePI

    STANDARD
        DOM 2: Document.createProcessingInstruction()

    PURPOSE
        Create PI node

    DESCRIPTION
        Creates a Processing Instruction node with the given target and
        data (which should be in the data encoding).  The data may be NULL
        initially, and may be changed later (with XmlDomSetPIData), but the
        target is required and cannot be changed.  Note the target and data
        are not verified, converted, or checked.  If bad data is set, bad
        things will happen-- GIGO!  The name of a PI node is the same as the
        target.

        The new node is an orphan with no parent; it must be added to the
        DOM tree with XmlDomAppendChild etc.

        The PI's target and data are NOT copied, their pointers are just
        stored.  The user is responsible for persistence and freeing of
        that data.

    RETURNS
        (xmlpinode *) new PI node.

    ARGUMENTS
        xctx   (IN) XML context
        doc    (IN) XML document [node]
        target (IN) new node's target [data encoding; user control]
        data   (IN) new node's data [data encoding; user control]

    SEE ALSO
        XmlDomGetPITarget, XmlDomGetPIData, XmlDomSetPIData,
        XmlDomCleanNode, XmlDomFreeNode
*/
#define XMLDOM_CREATE_PI_CB createPI_xmldomcb

#define XMLDOM_CREATE_PI_F(func, xctx, doc, target, data) \
    xmlpinode* func(xmlctx *xctx, xmldocnode *doc, \
                    oratext *target, oratext *data)

    XMLDOM_CREATE_PI_F((*XMLDOM_CREATE_PI_CB), xctx, doc, target, data);

#define XmlDomCreatePI(xctx, doc, target, data) \
    (*XML_DOMCB(xctx)->XMLDOM_CREATE_PI_CB) \
        ((xctx), (doc), (target), (data))

/*---------------------------------------------------------------------------
    NAME
        XmlDomCreateAttr

    STANDARD
        DOM 2: Document.createAttribute()

    PURPOSE
        Create attribute node

    DESCRIPTION
        Creates an attribute node with the given name and value (in the
        data encoding).  Note this function differs from the DOM
        specification, which does not allow the initial value of the
        attribute to be set (see XmlDomSetAttrValue).  The name is required,
        but the value may be NULL; neither is verified, converted, or
        checked.  If bad data is set, bad things will happen-- GIGO!

        This is the non-namespace aware function (see XmlDomCreateAttrNS):
        the new attribute will have NULL namespace URI and prefix, and
        its local name will be the same as its name, even if the name
        specified is a QName.

        If given an initial value, the attribute's specified flag will
        be TRUE.

        The new node may be added to the DOM tree with XmlDomSetAttrNode.

        See XmlDomSetAttr which creates and adds an attribute in a
        single operation.

        The name and value are NOT copied, their pointers are just stored.
        The user is responsible for persistence and freeing of that data.

    RETURNS
        (xmlattrnode *) new Attr node.

    ARGUMENTS
        xctx  (IN) XML context
        doc   (IN) XML document [node]
        name  (IN) new node's name [data encoding; user control]
        value (IN) new node's value [data encoding; user control]

    SEE ALSO
        XmlDomSetAttrValue, XmlDomCreateAttrNS, XmlDomSetAttr,
        XmlDomCleanNode, XmlDomFreeNode, XmlDomSetAttrNode
*/
#define XMLDOM_CREATE_ATTR_CB createAttr_xmldomcb

#define XMLDOM_CREATE_ATTR_F(func, xctx, doc, name, value) \
    xmlattrnode* func(xmlctx *xctx, xmldocnode *doc, \
                      oratext *name, oratext *value)

    XMLDOM_CREATE_ATTR_F((*XMLDOM_CREATE_ATTR_CB), xctx, doc, name, value);

#define XmlDomCreateAttr(xctx, doc, name, value) \
    (*XML_DOMCB(xctx)->XMLDOM_CREATE_ATTR_CB) \
        ((xctx), (doc), (name), (value))

/*---------------------------------------------------------------------------
    NAME
        XmlDomCreateAttrNS

    STANDARD
        DOM 2: Document.createAttributeNS()

    PURPOSE
        Create attribute node with namespace information

    DESCRIPTION
        Creates an attribute node with the given namespace URI and QName;
        this is the namespace-aware version of XmlDomCreateAttr.  Note
        this function differs from the DOM specification, which does not allow
        the initial value of the attribute to be set (see XmlDomSetAttrValue).
        The name is required, but the value may be NULL; neither is verified,
        converted, or checked.  If bad data is set, bad things will happen--
        GIGO!

        If given an initial value, the attribute's specified flag will be
        TRUE.

        The new node may be added to the DOM tree with XmlDomSetAttrNodeNS.

        See XmlDomSetAttrNS which creates and adds an attribute in a single
        operation.

        The URI, QName and value are NOT copied, their pointers are just
        stored.  The user is responsible for persistence and freeing of
        that data.

    RETURNS
        (xmlattrnode *) new Attr node.

    ARGUMENTS
        xctx  (IN) XML context
        doc   (IN) XML document [node]
        uri   (IN) node's namespace URI [data encoding; user control]
        qname (IN) node's qualified name [data encoding; user control]
        value (IN) node's value [data encoding; user control]

    SEE ALSO
        XmlDomSetAttrValue, XmlDomCreateAttr, XmlDomSetAttrNS,
        XmlDomCleanNode, XmlDomFreeNode, XmlDomSetAttrNodeNS
*/
#define XMLDOM_CREATE_ATTR_NS_CB createAttrNS_xmldomcb

#define XMLDOM_CREATE_ATTR_NS_F(func, xctx, doc, uri, qname, value) \
    xmlattrnode* func(xmlctx *xctx, xmldocnode *doc, oratext *uri, \
                      oratext *qname, oratext *value)

    XMLDOM_CREATE_ATTR_NS_F((*XMLDOM_CREATE_ATTR_NS_CB),
                            xctx, doc, uri, qname, value);

#define XmlDomCreateAttrNS(xctx, doc, uri, qname, value) \
    (*XML_DOMCB(xctx)->XMLDOM_CREATE_ATTR_NS_CB) \
        ((xctx), (doc), (uri), (qname), (value))

/*---------------------------------------------------------------------------
    NAME
        XmlDomCreateEntityRef

    STANDARD
        DOM 2: Document.createEntityReference()

    PURPOSE
        Create entity reference node

    DESCRIPTION
        Creates an Entity Reference node; the name (which should be in the
        data encoding) is the name of the entity to be referenced.  The named
        entity does not have to exist.  The name is not verified, converted,
        or checked.  If bad data is set, bad things will happen-- GIGO!

        Entity Reference nodes are never generated by the parser; instead,
        entity references are expanded as encountered.  On output, an entity
        reference node will turn into a "&name;" style reference.

        The new node is an orphan with no parent; it must be added to the
        DOM tree with XmlDomAppendChild etc.

        The entity reference name is NOT copied, its pointer is just stored.
        The user is responsible for persistence and freeing of that data.

    RETURNS
        (xmlentrefnode *) new EntityReference node.

    ARGUMENTS
        xctx (IN) XML context
        doc  (IN) XML document [node]
        name (IN) name of referenced entity [data encoding; user control]
*/
#define XMLDOM_CREATE_ENTITY_REF_CB createEntityRef_xmldomcb

#define XMLDOM_CREATE_ENTITY_REF_F(func, xctx, doc, name) \
    xmlentrefnode* func(xmlctx *xctx, xmldocnode *doc, oratext *name)

    XMLDOM_CREATE_ENTITY_REF_F((*XMLDOM_CREATE_ENTITY_REF_CB), xctx, doc,name);

#define XmlDomCreateEntityRef(xctx, doc, name) \
    (*XML_DOMCB(xctx)->XMLDOM_CREATE_ENTITY_REF_CB) \
        ((xctx), (doc), (name))

/*---------------------------------------------------------------------------
    NAME
        XmlDomGetDocElemByID

    STANDARD
        DOM 2: Document.getElementById()

    PURPOSE
        Get document element given ID

    DESCRIPTION
        Returns the element node which has the given ID.  If no such ID is
        defined, returns NULL.  Note that attributes named "ID" are not
        automatically of type ID; ID attributes (which can have any name)
        must be declared as type ID in the DTD.

        The given ID should be in the data encoding or it might not match.

    RETURNS
        (xmlelemnode *) matching element.

    ARGUMENTS
        xctx (IN) XML context
        doc  (IN) XML document [node]
        id   (IN) element's unique ID [data encoding]

    SEE ALSO
        XmlDomGetDocElemsByTag, XmlDomGetDocElemsByTagNS
*/
#define XMLDOM_GET_DOC_ELEM_BY_ID_CB getDocElemByID_xmldomcb

#define XMLDOM_GET_DOC_ELEM_BY_ID_F(func, xctx, doc, id) \
    xmlelemnode* func(xmlctx *xctx, xmldocnode *doc, oratext *id)

    XMLDOM_GET_DOC_ELEM_BY_ID_F((*XMLDOM_GET_DOC_ELEM_BY_ID_CB), xctx, doc,id);

#define XmlDomGetDocElemByID(xctx, doc, id) \
    (*XML_DOMCB(xctx)->XMLDOM_GET_DOC_ELEM_BY_ID_CB) \
        ((xctx), (doc), (id))

/*---------------------------------------------------------------------------
    NAME
        XmlDomGetDocElemsByTag

    STANDARD
        DOM 2: Document.getElementsByTagName()

    PURPOSE
        Obtain doc elements

    DESCRIPTION
        Returns a list of all elements in the document tree rooted at the
        root node with a given tag name, in document order (i.e. the order
        in which they would be encountered in a preorder traversal of the
        tree).  If root is NULL, the entire document is searched.

        The special name "*" matches all tag names; a NULL name matches
        nothing.  Note that tag names are CASE SENSITIVE, and should be in
        the data encoding or a mismatch might occur.

        This function is not namespace aware; the full tag names are compared.
        If two QNames with two different prefixes both of which map to the
        same URI are compared, the comparison will fail.  See
        XmlDomGetElemsByTagNS for the namespace-aware version.

        The list should be freed with XmlDomFreeNodeList when it is no
        longer needed.

        The list is NOT live, it is a snapshot.  That is, if a new node
        which matched the tag name were added to the DOM after the list
        was returned, the list would not automatically be updated to
        include the node.

    RETURNS
        (xmlnodelist *) new NodeList containing all matched Elements.

    ARGUMENTS
        xctx (IN) XML context
        doc  (IN) XML document [node]
        name (IN) tagname to match [data encoding; "*" for all]

    SEE ALSO
        XmlDomGetDocElemByID, XmlDomGetDocElemsByTagNS, XmlDomFreeNodeList
*/
#define XMLDOM_GET_DOC_ELEMS_BY_TAG_CB getDocElemsByTag_xmldomcb

#define XMLDOM_GET_DOC_ELEMS_BY_TAG_F(func, xctx, doc, name) \
    xmlnodelist* func(xmlctx *xctx, xmldocnode *doc, oratext *name)

    XMLDOM_GET_DOC_ELEMS_BY_TAG_F((*XMLDOM_GET_DOC_ELEMS_BY_TAG_CB),
                                  xctx, doc, name);

#define XmlDomGetDocElemsByTag(xctx, doc, name) \
    (*XML_DOMCB(xctx)->XMLDOM_GET_DOC_ELEMS_BY_TAG_CB) \
        ((xctx), (doc), (name))

/*---------------------------------------------------------------------------
    NAME
        XmlDomGetDocElemsByTagNS

    STANDARD
        DOM 2: Document.getElementsByTagNameNS()

    PURPOSE
        Obtain doc elements (namespace aware version)

    DESCRIPTION
        Returns a list of all elements (in the document tree rooted at the
        given node) with a given namespace URI and local name, in the order
        in which they would be encountered in a preorder traversal of the
        tree.  If root is NULL, the entire document is searched.

        The URI and local name should be in the data encoding.  The special
        local name "*" matches all local names; a NULL local name matches
        nothing.  Namespace URIs must always match, however, no wildcard is
        allowed.  Note that comparisons are CASE SENSITIVE.  See
        XmlDomGetDocElemsByTag for the non-namespace aware version.

        The list should be freed with XmlDomFreeNodeList when it is no
        longer needed.

        The list is NOT live, it is a snapshot.  That is, if a new node
        which matched the tag name were added to the DOM after the list
        was returned, the list would not automatically be updated to
        include the node.

    RETURNS
        (xmlnodelist *) new NodeList containing all matched Elements.

    ARGUMENTS
        xctx  (IN) XML context
        doc   (IN) XML document [node]
        uri   (IN) namespace URI to match [data encoding]
        local (IN) local name to match [data encoding; "*" for all]

    SEE ALSO
        XmlDomGetDocElemByID, XmlDomGetDocElemsByTag, XmlDomFreeNodeList
*/
#define XMLDOM_GET_DOC_ELEMS_BY_TAG_NS_CB getDocElemsByTagNS_xmldomcb

#define XMLDOM_GET_DOC_ELEMS_BY_TAG_NS_F(func, xctx, doc, uri, local) \
    xmlnodelist* func(xmlctx *xctx, xmldocnode *doc, \
                      oratext *uri, oratext *local)

    XMLDOM_GET_DOC_ELEMS_BY_TAG_NS_F((*XMLDOM_GET_DOC_ELEMS_BY_TAG_NS_CB),
                                     xctx, doc, uri, local);

#define XmlDomGetDocElemsByTagNS(xctx, doc, uri, local) \
    (*XML_DOMCB(xctx)->XMLDOM_GET_DOC_ELEMS_BY_TAG_NS_CB) \
        ((xctx), (doc), (uri), (local))

/*---------------------------------------------------------------------------
    NAME
        XmlDomGetChildren

    PURPOSE
        Obtain children elements (namespace aware version)

    STATUS
        Oracle extension [Private]

    DESCRIPTION
        Returns a list of all children elements (in the document order)
        complaining with node-test.

    RETURNS
        (ub2) number of children in the list.

    ARGUMENTS
        xctx  (IN) XML context
        node  (IN) parent XML element
        uri   (IN) namespace URI to match [NULL for all]
        local (IN) local name to match [NULL for all]
        test  (IN) XPath node-test for the child
        dst   (OUT) array for children 
        size  (IN) the size of dst[]
        deep  (IN) if TRUE do deep recursive search

    SEE ALSO
        XmlDomNodeTest
*/
#define XMLDOM_GET_CHILDREN_CB getChildren_xmldomcb

#define XMLDOM_GET_CHILDREN_F(func, xctx, node, uri,local,test,dst,size,deep) \
    ub4 func(xmlctx *xctx, xmlnode *node, \
                      oratext *uri, oratext *local, ub2 test, \
                      xmlnode **dst, ub4 size, boolean deep)
    XMLDOM_GET_CHILDREN_F((*XMLDOM_GET_CHILDREN_CB), xctx, node, uri, local,
                          test, dst, size, deep);

#define XmlDomGetChildren(xctx, node, uri, local, test, dst, size, deep) \
    (*XML_DOMCB(xctx)->XMLDOM_GET_CHILDREN_CB) \
        ((xctx), (node), (uri), (local), (test), \
         (dst), (size), (deep))

/*---------------------------------------------------------------------------
    NAME
        XdkDomNodeTest

    PURPOSE
        Returns TRUE if node complay with XPath node-test.

    STATUS
        Oracle extension [Private]

    DESCRIPTION
        Returns TRUE if node complay with XPath node-test.

    RETURNS
        (ub2) number of children in the list.

    ARGUMENTS
        xctx  (IN) XML context
        node  (IN) parent XML element
        uri   (IN) namespace URI to match [NULL for all]
        local (IN) local name to match [NULL for all]
        test  (IN) XPath node-test for the child

    SEE ALSO
        XmlDomGetChildren
*/
#define XMLDOM_NODETEST_CB nodeTest_xmldomcb

#define XMLDOM_NODETEST_F(func, xctx, node, uri, local, test) \
    ub2 func(xmlctx *xctx, xmlnode *node, \
                      oratext *uri, oratext *local, ub2 test)
    XMLDOM_NODETEST_F((*XMLDOM_NODETEST_CB), xctx, node, uri, local, test);

#define XmlDomNodeTest(xctx, node, uri, local, test) \
    (*XML_DOMCB(xctx)->XMLDOM_NODETEST_CB) \
        ((xctx), (node), (uri), (local), (test))

/*---------------------------------------------------------------------------
    NAME
        XmlDomIsNamespaceNode

    PURPOSE
        Returns TRUE if node is namespace attribute.

    STATUS
        Oracle extension [Private]

    DESCRIPTION
        Returns TRUE if node is namespace attribute.

    RETURNS
        Returns TRUE if node is namespace attribute.

    ARGUMENTS
        xctx  (IN) XML context
        attr  (IN) attribute node

    SEE ALSO
        
*/
#define XMLDOM_IS_NAMESPACE_NODE_CB isNamespaceNode_xmldomcb

#define XMLDOM_IS_NAMESPACE_NODE_F(func, xctx, attr) \
    ub2 func(xmlctx *xctx, xmlnode *attr)

    XMLDOM_IS_NAMESPACE_NODE_F((*XMLDOM_IS_NAMESPACE_NODE_CB), xctx, attr);

#define XmlDomIsNamespaceNode(xctx, attr) \
    (*XML_DOMCB(xctx)->XMLDOM_IS_NAMESPACE_NODE_CB) \
        ((xctx), (attr))

/*---------------------------------------------------------------------------
    NAME
        XmlDomSetDocOrder

    PURPOSE
        Set document order for all nodes

    DESCRIPTION
        Sets the document order for each node in the current document.  Must
        be called once on the final document before XSLT processing can occur.
        Note this is called automatically by the XSLT processor, so ordinarily
        the user need not make this call.

    RETURNS
        (ub4) highest ordinal assigned

    ARGUMENTS
        xctx     (IN) XML context
        doc      (IN) XML document [node]
        start_id (IN) starting ID#

    SEE ALSO
        XmlDomGetDocOrder, XmlDomCompareDocOrder
*/
#define XMLDOM_SET_DOC_ORDER_CB setDocOrder_xmldomcb

#define XMLDOM_SET_DOC_ORDER_F(func, xctx, doc, start_id) \
    ub4 func(xmlctx *xctx, xmldocnode *doc, ub4 start_id)

    XMLDOM_SET_DOC_ORDER_F((*XMLDOM_SET_DOC_ORDER_CB), xctx, doc, start_id);

#define XmlDomSetDocOrder(xctx, doc, start_id) \
    (*XML_DOMCB(xctx)->XMLDOM_SET_DOC_ORDER_CB) \
        ((xctx), (doc), (start_id))

/*---------------------------------------------------------------------------
    NAME
        XmlDomGetLastError

    PURPOSE
        Return last error code for document

    DESCRIPTION
        Returns the error code of the last error which occured in
        the given document.

    RETURNS
        (xmlerr) numeric error code, 0 if no error

    ARGUMENTS
        xctx (IN) XML context
        doc  (IN) XML document [node]
*/
#define XMLDOM_GET_LAST_ERROR_CB getLastError_xmldomcb

#define XMLDOM_GET_LAST_ERROR_F(func, xctx, doc) \
    xmlerr func(xmlctx *xctx, xmldocnode *doc)

    XMLDOM_GET_LAST_ERROR_F((*XMLDOM_GET_LAST_ERROR_CB), xctx, doc);

#define XmlDomGetLastError(xctx, doc) \
    (*XML_DOMCB(xctx)->XMLDOM_GET_LAST_ERROR_CB) \
        ((xctx), (doc))

/* BEGIN_NO_DOC */
#if 0
/*---------------------------------------------------------------------------
    NAME
        XmlDomSetLastError

    PURPOSE
        Sets last error code for document.

    DESCRIPTION
        Sets the Last Error code for the given document.  If <doc> is
        NULL, sets the error code for the XML context.

    RETURNS
        (xmlerr) original error code

    ARGUMENTS
        xctx    (IN) XML context
        doc     (IN) XML document [node]
        errcode (IN) error code to set, 0 to clear error
*/
#define XMLDOM_SET_LAST_ERROR_CB setLastError_xmldomcb

#define XMLDOM_SET_LAST_ERROR_F(func, xctx, doc, errcode) \
    xmlerr func(xmlctx *xctx, xmldocnode *doc, xmlerr errcode)

    XMLDOM_SET_LAST_ERROR_F((*XMLDOM_SET_LAST_ERROR_CB), xctx, doc, errcode);

#define XmlDomSetLastError(xctx, doc, errcode) \
    (*XML_DOMCB(xctx)->XMLDOM_SET_LAST_ERROR_CB) \
        ((xctx), (doc), (errcode))
#endif
/* END_NO_DOC */

/*---------------------------------------------------------------------------
    NAME
        XmlDomImportNode

    STANDARD
        DOM 2: Document.importNode()

    PURPOSE
        Import a node from another DOM

    DESCRIPTION
        Imports a node from one Document to another.  The new node is an
        orphan and has no parent; it must be added to the DOM tree with
        XmlDomAppendChild, etc.  The original node is not modified in any
        way or removed from its document; instead, a new node is created
        with copies of all the original node's QName, prefix, namespace URI,
        and local name.

        As with XmlDomCloneNode, the deep controls whether the children of
        the node are recursively imported.  If FALSE, only the node itself
        is imported, and it will have no children.  If TRUE, all descendents
        of the node will be imported as well, and an entire new subtree
        created.

        Document and DocumentType nodes cannot be imported.  Imported
        attributes will have their specified flags set to TRUE.  Elements
        will have only their specified attributes imported; non-specified
        (default) attributes are omitted.  New default attributes (for the
        destination document) are then added.

    RETURNS
        (xmlnode *) newly imported node (in this Document).

    ARGUMENTS
        xctx (IN) XML document context
        doc  (IN) XML document [node]
        nctx (IN) XML context of imported node
        node (IN) node to import
        deep (IN) recursively import subtree?

    SEE ALSO
        XmlDomCloneNode
*/
#define XMLDOM_IMPORT_NODE_CB importNode_xmldomcb

#define XMLDOM_IMPORT_NODE_F(func, xctx, doc, nctx, node, deep) \
    xmlnode* func(xmlctx *xctx, xmldocnode *doc, \
                  xmlctx *nctx, xmlnode *node, boolean deep)

    XMLDOM_IMPORT_NODE_F((*XMLDOM_IMPORT_NODE_CB), xctx, doc, nctx, node,deep);

#define XmlDomImportNode(xctx, doc, nctx, node, deep) \
    (*XML_DOMCB(xctx)->XMLDOM_IMPORT_NODE_CB) \
        ((xctx), (doc), (nctx), (node), (deep))

/*---------------------------------------------------------------------------
    NAME
        XmlDomIsSchemaBased

    PURPOSE
        Indicate whether a schema is associated with a document

    DESCRIPTION
        Returns flag saying whether there is a schema associated with this
        document.   The XmlLoadDom functions take a schema location hint
        (URI); the schema is used for efficient layout of XMLType data.
        If a schema was provided at load time, this function returns TRUE.

    RETURNS
        (boolean) TRUE if there is a schema associated with the document

    ARGUMENTS
        xctx (IN) XML context
        doc  (IN) XML document [node]

    SEE ALSO
        XmlDomGetSchema, XmlLoadDom
*/
#define XMLDOM_IS_SCHEMA_BASED_CB isSchemaBased_xmldomcb

#define XMLDOM_IS_SCHEMA_BASED_F(func, xctx, doc) \
    boolean func(xmlctx *xctx, xmldocnode *doc)

    XMLDOM_IS_SCHEMA_BASED_F((*XMLDOM_IS_SCHEMA_BASED_CB), xctx, doc);

#define XmlDomIsSchemaBased(xctx, doc) \
    (*XML_DOMCB(xctx)->XMLDOM_IS_SCHEMA_BASED_CB) \
        ((xctx), (doc))

/*---------------------------------------------------------------------------
    NAME
        XmlDomGetSchema

    PURPOSE
        Returns URI of schema associated with document

    DESCRIPTION
        Returns URI of schema associated with document, if there is one,
        else returns NULL.  The XmlLoadDom functions take a schema location
        hint (URI); the schema is used for efficient layout of XMLType data.
        If a schema was provided at load time, this function returns TRUE.

    RETURNS
        (oratext *) Schema URI or NULL

    ARGUMENTS
        xctx (IN) XML context
        doc  (IN) XML document [node]

    SEE ALSO
        XmlDomIsSchemaBased, XmlLoadDom
*/
#define XMLDOM_GET_SCHEMA_CB getSchema_xmldomcb

#define XMLDOM_GET_SCHEMA_F(func, xctx, doc) \
    oratext* func(xmlctx *xctx, xmldocnode *doc)

    XMLDOM_GET_SCHEMA_F((*XMLDOM_GET_SCHEMA_CB), xctx, doc);

#define XmlDomGetSchema(xctx, doc) \
    (*XML_DOMCB(xctx)->XMLDOM_GET_SCHEMA_CB) \
        ((xctx), (doc))

/*---------------------------------------------------------------------------
    NAME
        XmlDomSetXSLOutput

    PURPOSE
        Handle XSL's xsl:output directive

    STATUS
        Private

    DESCRIPTION
        Handle XSL's xsl:output directive.  Called by the XSL processor
        when xsl:output is encountered.  Used to stash the arguments to
        xsl:output in the DOM structure for use later if/when the DOM is
        serialized.

    RETURNS
        (xmlerr) XML error code

    ARGUMENTS
        xctx     (IN) XML context
        doc      (IN) XML document [node]
        method   (IN) "method" argument [NULL, or "xml|html|text|name"]
        version  (IN) "version" argument [NULL or "string"]
        encoding (IN) "encoding" argument [NULL or "string"]
        omitdecl (IN) "omit-xml-declaration" argument [NULL or "yes|no"]
        stdalone (IN) "standalone" argument [NULL or "yes|no"]
        docpubid (IN) "doctype-public" argument [NULL or "string"]
        docsysid (IN) "doctype-system" argument [NULL or "string"]
        cdelems  (IN) "cdata-section-elements" [NULL or "namelist"]
        indent   (IN) "indent" argument [NULL or "yes|no"]
        media    (IN) "media-type" argument [NULL or "string"]
*/
#define XMLDOM_SET_XSL_OUTPUT_CB setXSLoutput_xmldomcb

#define XMLDOM_SET_XSL_OUTPUT_F(func, xctx, doc, method, version, encoding, \
                                omitdecl, stdalone, docpubid, docsysid, \
                                cdelems, indent, media) \
    xmlerr func(xmlctx *xctx, xmldocnode *doc, oratext *method, \
                oratext *version, oratext *encoding, oratext *omitdecl, \
                oratext *stdalone, oratext *docpubid, oratext *docsysid, \
                oratext *cdelems, oratext *indent, oratext *media)

    XMLDOM_SET_XSL_OUTPUT_F((*XMLDOM_SET_XSL_OUTPUT_CB),
                            xctx, doc, meth, ver, enc, odecl, std,
                            pubid, sysid, cdelems, indent, media);

#define XmlDomSetXSLOutput(xctx, doc, method, version, encoding, omitdecl, \
                           stdalone, docpubid, docsysid,cdelems,indent,media) \
    (*XML_DOMCB(xctx)->XMLDOM_SET_XSL_OUTPUT_CB) \
        ((xctx), (doc), (method), (version), (encoding), \
         (omitdecl), (stdalone), (docpubid), (docsysid), (cdelems), \
         (indent), (media))

/*---------------------------------------------------------------------------
    NAME
        XmlDomSaveString

    PURPOSE
        Saves a string permanently in a document's memory pool 

    DESCRIPTION
        Copies the given string into the document's memory pool, so that
        it persists for the life of the document.  The individual string
        WILL NOT BE FREEABLE, and the storage will be returned only when
        the entire document is freed.  Works on single-byte or multi-byte
        encodings; for Unicode strings, use XmlDomSaveString2

    RETURNS
        (oratext *) saved copy of string

    ARGUMENTS
        xctx (IN) XML context
        doc  (IN) XML document
        str  (IN) string to save [data encoding; single- or multi-byte only]

    SEE ALSO
        XmlDomSaveString2, XmlFreeDocument
*/
#define XMLDOM_SAVE_STRING_CB saveString_xmldomcb

#define XMLDOM_SAVE_STRING_F(func, xctx, doc, str) \
    oratext* func(xmlctx *xctx, xmldocnode *doc, oratext *str)

    XMLDOM_SAVE_STRING_F((*XMLDOM_SAVE_STRING_CB), xctx, doc, str);

#define XmlDomSaveString(xctx, doc, str) \
    (*XML_DOMCB(xctx)->XMLDOM_SAVE_STRING_CB) \
        ((xctx), (doc), (str))

/*---------------------------------------------------------------------------
    NAME
        XmlDomSaveString2

    PURPOSE
        Saves a Unicode string permanently in a document's memory pool 

    DESCRIPTION
        Copies the given string into the document's memory pool, so that
        it persists for the life of the document.  The individual string
        will not be freeable, and the storage will be returned only when
        the entire document is free.  Works on Unicode strings only; for
        single-byte or multi-byte strings, use XmlDomSaveString

    RETURNS
        (ub2 *) saved copy of string

    ARGUMENTS
        xctx (IN) XML context
        doc  (IN) XML document
        ustr (IN) string to save [data encoding; Unicode only]

    SEE ALSO
        XmlDomSaveString, XmlFreeDocument
*/
#define XMLDOM_SAVE_STRING2_CB saveString2_xmldomcb

#define XMLDOM_SAVE_STRING2_F(func, xctx, doc, ustr) \
    ub2* func(xmlctx *xctx, xmldocnode *doc, ub2 *ustr)

    XMLDOM_SAVE_STRING2_F((*XMLDOM_SAVE_STRING2_CB), xctx, doc, ustr);

#define XmlDomSaveString2(xctx, doc, ustr) \
    (*XML_DOMCB(xctx)->XMLDOM_SAVE_STRING2_CB) \
        ((xctx), (doc), (ustr))

/*---------------------------------------------------------------------------
    NAME
        XmlDomFreeString

    PURPOSE
        Frees a string allocate by XmlDomSubstringData etc

    DESCRIPTION
        Frees the string allocated by XmlDomSubstringData or similar
        functions.  Note that strings explicitly saved with XmlDomSaveString
        et al are NOT freeable individually.

    RETURNS
        (void)

    ARGUMENTS
        xctx (IN) XML context
        doc  (IN) document string belongs to
        str  (IN) string to free

    SEE ALSO
        XmlDomSaveString, XmlDomSaveString2
*/
#define XMLDOM_FREE_STRING_CB freeString_xmldomcb

#define XMLDOM_FREE_STRING_F(func, xctx, doc, str) \
    void func(xmlctx *xctx, xmldocnode *doc, oratext *str)

    XMLDOM_FREE_STRING_F((*XMLDOM_FREE_STRING_CB), xctx, doc, str);

#define XmlDomFreeString(xctx, doc, str) \
    (*XML_DOMCB(xctx)->XMLDOM_FREE_STRING_CB) \
        ((xctx), (doc), (str))

/****************************************************************************
                               Interface Node
 ****************************************************************************/

/*---------------------------------------------------------------------------
    NAME
        XmlDomGetNodeName

    STANDARD
        DOM 2: Node.getNodeName()

    PURPOSE
        Get node's name as NULL-terminated string

    DESCRIPTION
        Returns the (fully-qualified) name of a node (in the data encoding)
        as a NULL-terminated string, for example "bar\0" or "foo:bar\0".

        Note that some node types have fixed names: "#text", "#cdata-section",
        "#comment", "#document", "#document-fragment".

        A node's name cannot be changed once it is created, so there is
        no matching SetNodeName function.

        A length-based version is available as XmlDomGetNodeNameLen which
        returns the node name as a pointer and length, for use if the data
        is known to use XMLType backing store.

    RETURNS
        (oratext *) name of node [data encoding]

    ARGUMENTS
        xctx (IN) XML context
        node (IN) XML node

    SEE ALSO
        XmlDomGetNodeNameLen
*/
#define XMLDOM_GET_NODE_NAME_CB getNodeName_xmldomcb

#define XMLDOM_GET_NODE_NAME_F(func, xctx, node) \
    oratext* func(xmlctx *xctx, xmlnode *node)

    XMLDOM_GET_NODE_NAME_F((*XMLDOM_GET_NODE_NAME_CB), xctx, node);

#define XmlDomGetNodeName(xctx, node) \
    (*XML_DOMCB(xctx)->XMLDOM_GET_NODE_NAME_CB) \
        ((xctx), (node))

/*---------------------------------------------------------------------------
    NAME
        XmlDomGetNodeNameLen

    PURPOSE
        Get node's name as length-encoded string

    DESCRIPTION
        Returns the (fully-qualified) name of a node (in the data encoding)
        as a length-encoded string, for example {"bar", 3} or {"foo:bar", 7}.

        Note that some node types have fixed names: "#text", "#cdata-section",
        "#comment", "#document", "#document-fragment".

        A node's name cannot be changed once it is created, so there is
        no matching SetNodeName function.

        A NULL-terminated version is available as XmlDomGetNodeName which
        returns the node name as NULL-terminated string.  If the backing
        store is known to be XMLType, then the node's name will be stored
        internally as length-encoded.  Using the length-encoded Get
        functions will avoid having to copy and NULL-terminate the name.

        If both the input buffer is non-NULL and the input buffer length is
        non-zero, then the value will be stored in the input buffer.  Else,
        the implementation will return its own buffer.

        If the actual length is greater than buflen, then a truncated value
        will be copied into the buffer and len will return the actual length.

    RETURNS
        (oratext *) name of node, with length of name set in 'len'

    ARGUMENTS
        xctx   (IN)  XML context
        node   (IN)  XML node
        buf    (IN)  input buffer [optional]
        buflen (IN)  input buffer length [optional]
        len    (OUT) length of name [in characters]

    SEE ALSO
        XmlDomGetNodeName
*/
#define XMLDOM_GET_NODE_NAME_LEN_CB getNodeNameLen_xmldomcb

#define XMLDOM_GET_NODE_NAME_LEN_F(func, xctx, node, buf, buflen, len) \
    oratext* func(xmlctx *xctx, xmlnode *node, oratext *buf, ub4 buflen, \
                  ub4 *len)

    XMLDOM_GET_NODE_NAME_LEN_F((*XMLDOM_GET_NODE_NAME_LEN_CB), xctx, node, 
                               buf, buflen, len);

#define XmlDomGetNodeNameLen(xctx, node, buf, buflen, len) \
    (*XML_DOMCB(xctx)->XMLDOM_GET_NODE_NAME_LEN_CB) \
        ((xctx), (node), (buf), (buflen), (len))

/*---------------------------------------------------------------------------
    NAME
        XmlDomGetNodeType

    STANDARD
        DOM 2: Node.getNodeType()

    PURPOSE
        Get node's numeric type code

    DESCRIPTION
        Returns the type code of a node.  The type names and numeric values
        match the DOM specification:

        ELEMENT_NODE=1, ATTRIBUTE_NODE=2, TEXT_NODE=3, CDATA_SECTION_NODE=4,
        ENTITY_REFERENCE_NODE=5, ENTITY_NODE=6, PROCESSING_INSTRUCTION_NODE=7,
        COMMENT_NODE=8, DOCUMENT_NODE=9, DOCUMENT_TYPE_NODE=10,
        DOCUMENT_FRAGMENT_NODE=11, NOTATION_NODE=12

        Additional Oracle extension node types are as follows:

        ELEMENT_DECL_NODE, ATTR_DECL_NODE, CP_ELEMENT_NODE, CP_CHOICE_NODE,
        CP_SEQUENCE_NODE, CP_PCDATA_NODE, CP_STAR_NODE, CP_PLUS_NODE,
        CP_OPT_NODE

    RETURNS
        (xmlnodetype) numeric type-code of the node

    ARGUMENTS
        xctx (IN) XML context
        node (IN) XML node
*/
#define XMLDOM_GET_NODE_TYPE_CB getNodeType_xmldomcb

#define XMLDOM_GET_NODE_TYPE_F(func, xctx, node) \
    xmlnodetype func(xmlctx *xctx, xmlnode *node)

    XMLDOM_GET_NODE_TYPE_F((*XMLDOM_GET_NODE_TYPE_CB), xctx, node);

#define XmlDomGetNodeType(xctx, node) \
    (*XML_DOMCB(xctx)->XMLDOM_GET_NODE_TYPE_CB) \
        ((xctx), (node))

/*---------------------------------------------------------------------------
    NAME
        XmlDomGetNodeValue

    STANDARD
        DOM 2: Node.getNodeValue()

    PURPOSE
        Get node's value as NULL-terminated string

    DESCRIPTION
        Returns the "value" (associated character data) for a node as a
        NULL-terminated string.  Character and general entities will have
        been replaced.  Only Attr, CDATA, Comment, PI and Text nodes have
        values, all other node types have NULL value.

        A length-encoded version is available as XmlDomGetNodeValueLen which
        returns the node value as a pointer and length, for use if the data
        is known to use XMLType backing store.

    EXAMPLE
        "<foo>data</foo>"
        XmlDomGetNodeValue(node foo) --> NULL
        XmlDomGetNodeValue(foo's first child, a TEXT node) --> "data\0"

    RETURNS
        (oratext *) value of node

    ARGUMENTS
        xctx (IN) XML context
        node (IN) XML node

    SEE ALSO
        XmlDomSetNodeValue, XmlDomGetNodeValueLen
*/
#define XMLDOM_GET_NODE_VALUE_CB getNodeValue_xmldomcb

#define XMLDOM_GET_NODE_VALUE_F(func, xctx, node) \
    oratext* func(xmlctx *xctx, xmlnode *node)

    XMLDOM_GET_NODE_VALUE_F((*XMLDOM_GET_NODE_VALUE_CB), xctx, node);

#define XmlDomGetNodeValue(xctx, node) \
    (*XML_DOMCB(xctx)->XMLDOM_GET_NODE_VALUE_CB) \
        ((xctx), (node))

/*---------------------------------------------------------------------------
    NAME
        XmlDomGetNodeValueLen

    PURPOSE
        Get node's value as length-encoded string

    DESCRIPTION
        Returns the "value" (associated character data) for a node as a
        length-encoded string.  Character and general entities will have
        been replaced.  Only Attr, CDATA, Comment, PI and Text nodes have
        values, all other node types have NULL value.

        A NULL-terminated version is available as XmlDomGetNodeValue which
        returns the node value as NULL-terminated string.  If the backing
        store is known to be XMLType, then the node's data will be stored
        internally as length-encoded.  Using the length-based Get functions
        will avoid having to copy and NULL-terminate the data.

        If both the input buffer is non-NULL and the input buffer length is
        non-zero, then the value will be stored in the input buffer.  Else,
        the implementation will return its own buffer without allocating
        any new buffer, thus this returned value can be the
        underlying DOM inernal storage buffer itself. This is the most
        efficient way to get the value as there is no char conversion required
        and no null termination is required.

        If the actual length is greater than buflen, then a truncated value
        will be copied into the buffer and len will return the actual length.

    EXAMPLE
        "<foo>data</foo>"
        XmlDomGetNodeValue(node foo) --> NULL
        XmlDomGetNodeValue(foo's first child) --> {"data", 4}

    RETURNS
        (oratext *) value of node

    ARGUMENTS
        xctx   (IN)  XML context
        node   (IN)  XML node
        buf    (IN)  input buffer [optional]
        buflen (IN)  input buffer length [optional]
        len    (OUT) length of value [in bytes]

    SEE ALSO
        XmlDomSetNodeValueLen, XmlDomGetNodeValue
*/
#define XMLDOM_GET_NODE_VALUE_LEN_CB getNodeValueLen_xmldomcb

#define XMLDOM_GET_NODE_VALUE_LEN_F(func, xctx, node, buf, buflen, len) \
    oratext* func(xmlctx *xctx, xmlnode *node, oratext *buf, ub4 buflen, \
                  ub4 *len)

    XMLDOM_GET_NODE_VALUE_LEN_F((*XMLDOM_GET_NODE_VALUE_LEN_CB),
                                xctx, node, buf, buflen, len);

#define XmlDomGetNodeValueLen(xctx, node, buf, buflen, len) \
    (*XML_DOMCB(xctx)->XMLDOM_GET_NODE_VALUE_LEN_CB) \
        ((xctx), (node), (buf), (buflen), (len))

/*---------------------------------------------------------------------------
    NAME
        XmlDomGetNodeValueStreamVA

    PURPOSE
        Get node's value via orastream

    STATUS
        Oracle extension [Private]

    DESCRIPTION
        Same as XmlDomGetNodeValueStream() but takes the variable
        arguments as a va_list instead of ...

    RETURNS
        (xmlerr) error code, XMLERR_OK [0] for success

    ARGUMENTS
        xctx (IN) XML context
        node (IN) XML node
        va   (IN) variable arguments as va_list (ending in NULL)

    SEE ALSO
        XmlDomGetNodeValue, XmlDomGetNodeValueLen, XmlDomGetNodeValueStream
*/
#define XMLDOM_GET_NODE_VALUE_STREAM_VA_CB getNodeValueStreamVA_xmldomcb

#define XMLDOM_GET_NODE_VALUE_STREAM_VA_F(func, xctx, node, va) \
    xmlerr func(xmlctx *xctx, xmlnode *node, va_list va)

    XMLDOM_GET_NODE_VALUE_STREAM_VA_F((*XMLDOM_GET_NODE_VALUE_STREAM_VA_CB),
                                        xctx, node, va);

#define XmlDomGetNodeValueStreamVA(xctx, node, va) \
    (*XML_DOMCB(xctx)->XMLDOM_GET_NODE_VALUE_STREAM_VA_CB) \
        ((xctx), (node), (va))

/*---------------------------------------------------------------------------
    NAME
        XmlDomSetNodeValue

    STANDARD
        DOM 2: Node.setNodeValue()

    PURPOSE
        Set node's value

    DESCRIPTION
        Sets a node's value (character data) as a NULL-terminated string.
        Does not allow setting the value to NULL.  Only Attr, CDATA, Comment,
        PI and Text nodes have values; trying to set the value of another
        type of node is a no-op.  The new value must be in the data encoding!
        It is not verified, converted, or checked.  If bad data is set for
        a node, bad things will happen-- GIGO!

        The value is NOT copied, its pointer is just stored.  The user is
        responsible for persistence and freeing of that data.

    RETURNS
        (xmlerr) numeric error code, 0 on success

    ARGUMENTS
        xctx  (IN) XML context
        node  (IN) XML node
        value (IN) node's new value [data encoding; user control]

    SEE ALSO
        XmlDomGetNodeValue, XmlDomSetNodeValueLen
*/
#define XMLDOM_SET_NODE_VALUE_CB setNodeValue_xmldomcb

#define XMLDOM_SET_NODE_VALUE_F(func, xctx, node, value) \
    xmlerr func(xmlctx *xctx, xmlnode *node, oratext *value)

    XMLDOM_SET_NODE_VALUE_F((*XMLDOM_SET_NODE_VALUE_CB), xctx, node, value);

#define XmlDomSetNodeValue(xctx, node, value) \
    (*XML_DOMCB(xctx)->XMLDOM_SET_NODE_VALUE_CB) \
        ((xctx), (node), (value))

/*---------------------------------------------------------------------------
    NAME
        XmlDomSetNodeValueLen

    PURPOSE
        Set node's value as length-encoded string

    DESCRIPTION
        Sets the "value" (associated character data) for a node as a
        length-encoded string.

        A NULL-terminated version is available as XmlDomSetNodeValue which
        takes the node value as a NULL-terminated string.  If the backing
        store is known to be XMLType, then the node's data will be stored
        internally as length-encoded.  Using the length-based Set functions
        will avoid having to copy and NULL-terminate the data.

    EXAMPLE
        "<foo>data</foo>"
        XmlDomSetNodeValue(foo's text node, "z", 1)
        "<foo>z</foo>"

    RETURNS
        (xmlerr) numeric error code, 0 on success

    ARGUMENTS
        xctx  (IN) XML context
        node  (IN) XML node
        value (IN) new value for node
        len   (IN) length of value [in bytes]

    SEE ALSO
        XmlDomSetNodeValueLen, XmlDomSetNodeValue
*/
#define XMLDOM_SET_NODE_VALUE_LEN_CB setNodeValueLen_xmldomcb

#define XMLDOM_SET_NODE_VALUE_LEN_F(func, xctx, node, value, len) \
    xmlerr func(xmlctx *xctx, xmlnode *node, oratext *value, ub4 len)

    XMLDOM_SET_NODE_VALUE_LEN_F((*XMLDOM_SET_NODE_VALUE_LEN_CB),
                                xctx, node, value, len);

#define XmlDomSetNodeValueLen(xctx, node, value, len) \
    (*XML_DOMCB(xctx)->XMLDOM_SET_NODE_VALUE_LEN_CB) \
        ((xctx), (node), (value), (len))

/*---------------------------------------------------------------------------
    NAME
        XmlDomSetNodeValueStreamVA

    PURPOSE
        Set node's value via orastream

    STATUS
        Oracle extension [Private]

    DESCRIPTION
        Same as XmlDomSetNodeValueStream() but takes the variable
        arguments as a va_list instead of ...

    RETURNS
        (xmlerr) error code, XMLERR_OK [0] for success

    ARGUMENTS
        xctx (IN) XML context
        node (IN) XML node
        va   (IN) variable arguments as va_list (ending in NULL)

    SEE ALSO
        XmlDomSetNodeValue, XmlDomSetNodeValueLen, XmlDomSetNodeValueStream
*/
#define XMLDOM_SET_NODE_VALUE_STREAM_VA_CB setNodeValueStreamVA_xmldomcb

#define XMLDOM_SET_NODE_VALUE_STREAM_VA_F(func, xctx, node, va) \
    xmlerr func(xmlctx *xctx, xmlnode *node, va_list va)

    XMLDOM_SET_NODE_VALUE_STREAM_VA_F((*XMLDOM_SET_NODE_VALUE_STREAM_VA_CB),
                                        xctx, node, va);

#define XmlDomSetNodeValueStreamVA(xctx, node, va) \
    (*XML_DOMCB(xctx)->XMLDOM_SET_NODE_VALUE_STREAM_VA_CB) \
        ((xctx), (node), (va))

/*---------------------------------------------------------------------------
    NAME
        XmlDomGetParentNode

    STANDARD
        DOM 2: Node.getParentNode()

    PURPOSE
        Get parent node

    DESCRIPTION
        Returns a node's parent node.  All nodes types except Attr, Document,
        DocumentFragment, Entity, and Notation may have a parent (these five
        exceptions always have a NULL parent).  If a node has just been
        created but not yet added to the DOM tree, or if it has been removed
        from the DOM tree, its parent is also NULL.

    RETURNS
        (xmlnode *) parent of node

    ARGUMENTS
        xctx (IN) XML context
        node (IN) XML node
*/
#define XMLDOM_GET_PARENT_NODE_CB getParentNode_xmldomcb

#define XMLDOM_GET_PARENT_NODE_F(func, xctx, node) \
    xmlnode* func(xmlctx *xctx, xmlnode *node)

    XMLDOM_GET_PARENT_NODE_F((*XMLDOM_GET_PARENT_NODE_CB), xctx, node);

#define XmlDomGetParentNode(xctx, node) \
    (*XML_DOMCB(xctx)->XMLDOM_GET_PARENT_NODE_CB) \
        ((xctx), (node))

/*---------------------------------------------------------------------------
    NAME
        XmlDomGetXSLParentNode

    PURPOSE
        Get parent node (per XSL spec)

    STATUS
        Private

    DESCRIPTION
        This function differs from the normal XmlDomGetParentNode in that
        for DOM, attribute nodes have no parents; but for XSL, attributes
        have their elements as parents.  Used only by the XSLT processor.

    RETURNS
        (xmlnode *) parent of node (XSL rules)

    ARGUMENTS
        xctx (IN) XML context
        node (IN) XML node

    SEE ALSO
        XmlDomGetParentNode
*/
#define XMLDOM_GET_XSL_PARENT_NODE_CB getXSLParentNode_xmldomcb

#define XMLDOM_GET_XSL_PARENT_NODE_F(func, xctx, node) \
    xmlnode* func(xmlctx *xctx, xmlnode *node)

    XMLDOM_GET_XSL_PARENT_NODE_F((*XMLDOM_GET_XSL_PARENT_NODE_CB), xctx, node);

#define XmlDomGetXSLParentNode(xctx, node) \
    (*XML_DOMCB(xctx)->XMLDOM_GET_XSL_PARENT_NODE_CB) \
        ((xctx), (node))

/*---------------------------------------------------------------------------
    NAME
        XmlDomHasChildNodes

    STANDARD
        DOM 2: Node.hasChildNodes()

    PURPOSE
        Test if node has children

    DESCRIPTION
        Test if a node has children.  Only Element, Document, DTD, and
        DocumentFragment nodes may have children.  Note that just because
        XmlDomGetChildNodes returns a list does not mean the node actually
        has children, since the list may be empty, so a non-NULL return
        from XmlDomGetChildNodes should not be used as a test.

    RETURNS
        (boolean) TRUE if the node has any children

    ARGUMENTS
        xctx (IN) XML context
        node (IN) XML node
*/
#define XMLDOM_HAS_CHILD_NODES_CB hasChildNodes_xmldomcb

#define XMLDOM_HAS_CHILD_NODES_F(func, xctx, node) \
    boolean func(xmlctx *xctx, xmlnode *node)

    XMLDOM_HAS_CHILD_NODES_F((*XMLDOM_HAS_CHILD_NODES_CB), xctx, node);

#define XmlDomHasChildNodes(xctx, node) \
    (*XML_DOMCB(xctx)->XMLDOM_HAS_CHILD_NODES_CB) \
        ((xctx), (node))

/*---------------------------------------------------------------------------
    NAME
        XmlDomGetChildNodes

    STANDARD
        DOM 2: Node.getChildNodes()

    PURPOSE
        Return children of node

    DESCRIPTION
        Returns a list of the node's children, or NULL if it has no
        children.  Only Element, Document, DTD, and DocumentFragment
        nodes may have children; all other types will return NULL.

        Note that an empty list may be returned if the node once had
        children, but all have been removed!  That is, the list may
        exist but have no members.  So, presense of the list alone does
        not mean the node has children.  You must check the size of the
        list with XmlDomNumChildNodes or use XmlDomHasChildNodes first.

        The xmlnodelist structure is opaque and can only be manipulated
        with functions in the NodeList interface.

        The returned list is live; all changes in the original node are
        reflected immediately.

    RETURNS
        (xmlnodelist *) live NodeList containing all children of node

    ARGUMENTS
        xctx (IN) XML context
        node (IN) XML node
*/
#define XMLDOM_GET_CHILD_NODES_CB getChildNodes_xmldomcb

#define XMLDOM_GET_CHILD_NODES_F(func, xctx, node) \
    xmlnodelist* func(xmlctx *xctx, xmlnode *node)

    XMLDOM_GET_CHILD_NODES_F((*XMLDOM_GET_CHILD_NODES_CB), xctx, node);

#define XmlDomGetChildNodes(xctx, node) \
    (*XML_DOMCB(xctx)->XMLDOM_GET_CHILD_NODES_CB) \
        ((xctx), (node))

/*---------------------------------------------------------------------------
    NAME
        XmlDomNumChildNodes

    PURPOSE
        Return number of children of node

    DESCRIPTION
        Returns the number of children of a node.  Only Element, Document,
        DTD, and DocumentFragment nodes may have children, all other types
        return 0.  Note that just because XmlDomGetChildNodes returns a list
        does not mean that it contains any children; it may be an empty list
        with zero length.

    RETURNS
        (ub4) number of children of node

    ARGUMENTS
        xctx (IN) XML context
        node (IN) XML node
*/
#define XMLDOM_NUM_CHILD_NODES_CB numChildNodes_xmldomcb

#define XMLDOM_NUM_CHILD_NODES_F(func, xctx, node) \
    ub4 func(xmlctx *xctx, xmlnode *node)

    XMLDOM_NUM_CHILD_NODES_F((*XMLDOM_NUM_CHILD_NODES_CB), xctx, node);

#define XmlDomNumChildNodes(xctx, node) \
    (*XML_DOMCB(xctx)->XMLDOM_NUM_CHILD_NODES_CB) \
        ((xctx), (node))

/*---------------------------------------------------------------------------
    NAME
        XmlDomGetFirstChild

    STANDARD
        DOM 2: Node.getFirstChild()

    PURPOSE
        Returns first child of node

    DESCRIPTION
        Returns the first child of a node, or NULL if the node has no
        children.  Only Element, Document, DTD, and DocumentFragment
        nodes may have children; all other types will return NULL.

    RETURNS
        (xmlnode *) first child of node

    ARGUMENTS
        xctx (IN) XML context
        node (IN) XML node

    SEE ALSO
        XmlDomGetLastChild, XmlDomHasChildNodes, XmlDomGetChildNodes,
        XmlDomNumChildNodes
*/
#define XMLDOM_GET_FIRST_CHILD_CB getFirstChild_xmldomcb

#define XMLDOM_GET_FIRST_CHILD_F(func, xctx, node) \
    xmlnode* func(xmlctx *xctx, xmlnode *node)

    XMLDOM_GET_FIRST_CHILD_F((*XMLDOM_GET_FIRST_CHILD_CB), xctx, node);

#define XmlDomGetFirstChild(xctx, node) \
    (*XML_DOMCB(xctx)->XMLDOM_GET_FIRST_CHILD_CB) \
        ((xctx), (node))

/*---------------------------------------------------------------------------
    NAME
        XmlDomGetLastChild

    STANDARD
        DOM 2: Node.getLastChild()

    PURPOSE
        Returns last child of node

    DESCRIPTION
        Returns the last child of a node, or NULL if the node has no
        children.  Only Element, Document, DTD, and DocumentFragment
        nodes may have children; all other types will return NULL.

    RETURNS
        (xmlnode *) last child of node

    ARGUMENTS
        xctx (IN) XML context
        node (IN) XML node

    SEE ALSO
        XmlDomGetFirstChild, XmlDomHasChildNodes, XmlDomGetChildNodes,
        XmlDomNumChildNodes
*/
#define XMLDOM_GET_LAST_CHILD_CB getLastChild_xmldomcb

#define XMLDOM_GET_LAST_CHILD_F(func, xctx, node) \
    xmlnode* func(xmlctx *xctx, xmlnode *node)

    XMLDOM_GET_LAST_CHILD_F((*XMLDOM_GET_LAST_CHILD_CB), xctx, node);

#define XmlDomGetLastChild(xctx, node) \
    (*XML_DOMCB(xctx)->XMLDOM_GET_LAST_CHILD_CB) \
        ((xctx), (node))

/*---------------------------------------------------------------------------
    NAME
        XmlDomAppendChild

    STANDARD
        DOM 2: Node.appendChild()

    PURPOSE
        Append new child to node's list of children

    DESCRIPTION
        Appends the node to the end of the parent's list of children and
        returns the new node.  If newChild is a DocumentFragment, all of
        its children are appended in original order; the DocumentFragment
        node itself is not.

    RETURNS
        (xmlnode *) node added

    ARGUMENTS
        xctx     (IN) XML context
        parent   (IN) parent to receive new node
        newChild (IN) node to add

    SEE ALSO
        XmlDomInsertBefore, XmlDomReplaceChild
*/
#define XMLDOM_APPEND_CHILD_CB appendChild_xmldomcb

#define XMLDOM_APPEND_CHILD_F(func, xctx, parent, newChild) \
    xmlnode* func(xmlctx *xctx, xmlnode *parent, xmlnode *newChild)

    XMLDOM_APPEND_CHILD_F((*XMLDOM_APPEND_CHILD_CB), xctx, parent, newChild);

#define XmlDomAppendChild(xctx, parent, child) \
    (*XML_DOMCB(xctx)->XMLDOM_APPEND_CHILD_CB) \
        ((xctx), (parent), (child))

/*---------------------------------------------------------------------------
    NAME
        XmlDomInsertBefore

    STANDARD
        DOM 2: Node.insertBefore()

    PURPOSE
        Insert new child into node's list of children

    DESCRIPTION
        Inserts the node newChild before the existing child node refChild
        in the parent node.  If refChild is NULL, appends to parent's
        children as per XmlDomAppendChild; otherwise it must be a child of the
        given parent.  If newChild is a DocumentFragment, all of its children
        are inserted (in the same order) before refChild; the DocumentFragment
        node itself is not.  If newChild is already in the DOM tree, it is
        first removed from its current position.

    RETURNS
        (xmlnode *) node being inserted

    ARGUMENTS
        xctx     (IN) XML context
        parent   (IN) parent to receive new child
        newChild (IN) node to insert
        refChild (IN) reference node to insert before

    SEE ALSO
        XmlDomAppendChild, XmlDomReplaceChild, XmlDomRemoveChild
*/
#define XMLDOM_INSERT_BEFORE_CB insertBefore_xmldomcb

#define XMLDOM_INSERT_BEFORE_F(func, xctx, parent, newChild, refChild) \
    xmlnode* func(xmlctx *xctx, xmlnode *parent, \
                  xmlnode *newChild, xmlnode *refChild)

    XMLDOM_INSERT_BEFORE_F((*XMLDOM_INSERT_BEFORE_CB),
                           xctx, parent, newChild, refChild);

#define XmlDomInsertBefore(xctx, parent, child, ref) \
    (*XML_DOMCB(xctx)->XMLDOM_INSERT_BEFORE_CB) \
        ((xctx), (parent), (child), (ref))

/*---------------------------------------------------------------------------
    NAME
        XmlDomReplaceChild

    STANDARD
        DOM 2: Node.replaceChild()

    PURPOSE
        Replace an existing child of a node

    DESCRIPTION
        Replaces the child node oldChild with the new node newChild in
        oldChild's parent, and returns oldChild (which is now orphaned,
        with a NULL parent).  If newChild is a DocumentFragment, all of
        its children are inserted in place of oldChild; the DocumentFragment
        node itself is not.  If newChild is already in the DOM tree, it is
        first removed from its current position.

    RETURNS
        (xmlnode *) node replaced

    ARGUMENTS
        xctx     (IN)  XML context
        newChild (IN)  new node being substituted
        oldChild (OUT) old node being replaced

    SEE ALSO
        XmlDomAppendChild, XmlDomInsertBefore, XmlDomRemoveChild
*/
#define XMLDOM_REPLACE_CHILD_CB replaceChild_xmldomcb

#define XMLDOM_REPLACE_CHILD_F(func, xctx, newChild, oldChild) \
    xmlnode* func(xmlctx *xctx, xmlnode *newChild, xmlnode *oldChild)

    XMLDOM_REPLACE_CHILD_F((*XMLDOM_REPLACE_CHILD_CB), xctx, newC, oldC);

#define XmlDomReplaceChild(xctx, newChild, oldChild) \
    (*XML_DOMCB(xctx)->XMLDOM_REPLACE_CHILD_CB) \
        ((xctx),(newChild),(oldChild))

/*---------------------------------------------------------------------------
    NAME
        XmlDomRemoveChild

    STANDARD
        DOM 2: Node.removeChild()

    PURPOSE
        Remove an existing child node

    DESCRIPTION
        Removes a node from its parent's list of children and returns it.
        The node is orphaned; its parent will be NULL after removal.

    RETURNS
        (xmlnode *) node removed

    ARGUMENTS
        xctx     (IN) XML context
        oldChild (IN) node to remove

    SEE ALSO
        XmlDomAppendChild, XmlDomInsertBefore, XmlDomReplaceChild
*/
#define XMLDOM_REMOVE_CHILD_CB removeChild_xmldomcb

#define XMLDOM_REMOVE_CHILD_F(func, xctx, oldChild) \
    xmlnode* func(xmlctx *xctx, xmlnode *oldChild)

    XMLDOM_REMOVE_CHILD_F((*XMLDOM_REMOVE_CHILD_CB), xctx, oldChild);

#define XmlDomRemoveChild(xctx, oldChild) \
    (*XML_DOMCB(xctx)->XMLDOM_REMOVE_CHILD_CB) \
        ((xctx), (oldChild))

/*---------------------------------------------------------------------------
    NAME
        XmlDomGetPrevSibling

    STANDARD
        DOM 2: Node.previousSibling

    PURPOSE
        Return previous sibling of node

    DESCRIPTION
        Returns the node preceding a node at the same level in the DOM tree.
        That is, for each child of a parent node, the previous sibling of
        that child is the child which came before it.  If a node is the first
        child of its parent, or has no parent, NULL is returned.

    RETURNS
        (xmlnode *) node immediately preceding node at same level

    ARGUMENTS
        xctx (IN) XML context
        node (IN) XML node

    SEE ALSO
        XmlDomGetNextSibling
*/
#define XMLDOM_GET_PREV_SIBLING_CB getPrevSibling_xmldomcb

#define XMLDOM_GET_PREV_SIBLING_F(func, xctx, node) \
    xmlnode* func(xmlctx *xctx, xmlnode *node)

    XMLDOM_GET_PREV_SIBLING_F((*XMLDOM_GET_PREV_SIBLING_CB), xctx, node);

#define XmlDomGetPrevSibling(xctx, node) \
    (*XML_DOMCB(xctx)->XMLDOM_GET_PREV_SIBLING_CB) \
        ((xctx), (node))

/*---------------------------------------------------------------------------
    NAME
        XmlDomGetNextSibling

    STANDARD
        DOM 2: Node.nextSibling

    PURPOSE
        Return next sibling of node

    DESCRIPTION
        Returns the node following a node at the same level in the DOM tree.
        That is, for each child of a parent node, the next sibling of that
        child is the child which comes after it.  If a node is the last child
        of its parent, or has no parent, NULL is returned.

    RETURNS
        (xmlnode *) node immediately following node at same level

    ARGUMENTS
        xctx (IN) XML context
        node (IN) XML node

    SEE ALSO
        XmlDomGetPrevSibling
*/
#define XMLDOM_GET_NEXT_SIBLING_CB getNextSibling_xmldomcb

#define XMLDOM_GET_NEXT_SIBLING_F(func, xctx, node) \
    xmlnode* func(xmlctx *xctx, xmlnode *node)

    XMLDOM_GET_NEXT_SIBLING_F((*XMLDOM_GET_NEXT_SIBLING_CB), xctx, node);

#define XmlDomGetNextSibling(xctx, node) \
    (*XML_DOMCB(xctx)->XMLDOM_GET_NEXT_SIBLING_CB) \
        ((xctx), (node))

/*---------------------------------------------------------------------------
    NAME
        XmlDomHasAttrs

    PURPOSE
        Test if element has attributes

    DESCRIPTION
        Test if an element has attributes.  Returns TRUE if any
        attributes of any sort are defined (namespace or regular).

    RETURNS
        (boolean) TRUE if element has attributes

    ARGUMENTS
        xctx (IN) XML context
        elem (IN) XML element node
*/
#define XMLDOM_HAS_ATTRS_CB hasAttrs_xmldomcb

#define XMLDOM_HAS_ATTRS_F(func, xctx, elem) \
    boolean func(xmlctx *xctx, xmlelemnode *elem)

    XMLDOM_HAS_ATTRS_F((*XMLDOM_HAS_ATTRS_CB), xctx, elem);

#define XmlDomHasAttrs(xctx, elem) \
    (*XML_DOMCB(xctx)->XMLDOM_HAS_ATTRS_CB) \
        ((xctx), (elem))

/*---------------------------------------------------------------------------
    NAME
        XmlDomGetAttrs

    STANDARD
        DOM 2: Node.attributes

    PURPOSE
        Return attributes of node

    DESCRIPTION
        Returns a NamedNodeMap of attributes of an element node, or NULL
        if it has no attributes.  For other node types, NULL is returned.
        Note that if an element once had attributes, but they have all been
        removed, an empty list will be returned.  So, presense of the list
        does not mean the element has attributes.  You must check the size
        of the list with XmlDomNumAttrs or use XmlDomHasChildNodes first.

    RETURNS
        (xmlnamedmap *) NamedNodeMap of node's attributes

    ARGUMENTS
        xctx (IN) XML context
        elem (IN) XML element node

    SEE ALSO
        XmlDomNumAttrs, XmlDomHasChildNodes
*/
#define XMLDOM_GET_ATTRS_CB getAttrs_xmldomcb

#define XMLDOM_GET_ATTRS_F(func, xctx, elem) \
    xmlnamedmap* func(xmlctx *xctx, xmlelemnode *elem)

    XMLDOM_GET_ATTRS_F((*XMLDOM_GET_ATTRS_CB), xctx, elem);

#define XmlDomGetAttrs(xctx, elem) \
    (*XML_DOMCB(xctx)->XMLDOM_GET_ATTRS_CB) \
        ((xctx), (elem))

/*---------------------------------------------------------------------------
    NAME
        XmlDomNumAttrs

    PURPOSE
        Return number of attributes of element

    DESCRIPTION
        Returns the number of attributes of an element.  Note that just
        because a list is returned by XmlDomGetAttrs does not mean
        it contains any attributes; it may be an empty list with zero
        length.

    RETURNS
        (ub4) number of attributes of node

    ARGUMENTS
        xctx (IN) XML context
        elem (IN) XML element node
*/
#define XMLDOM_NUM_ATTRS_CB numAttrs_xmldomcb

#define XMLDOM_NUM_ATTRS_F(func, xctx, elem) \
    ub4 func(xmlctx *xctx, xmlelemnode *elem)

    XMLDOM_NUM_ATTRS_F((*XMLDOM_NUM_ATTRS_CB), xctx, elem);

#define XmlDomNumAttrs(xctx, elem) \
    (*XML_DOMCB(xctx)->XMLDOM_NUM_ATTRS_CB) \
        ((xctx), (elem))

/*---------------------------------------------------------------------------
    NAME
        XmlDomGetNodeURI

    STANDARD
        DOM 2: Node.namespaceURI

    PURPOSE
        Return namespace URI of node as a NULL-terminated string

    DESCRIPTION
        Returns the namespace URI for a node (in the data encoding) as a
        NULL-terminated string.  If the node's name is not qualified (does
        not contain a namespace prefix), it will have the default namespace
        in effect when the node was created (which may be NULL).

        A length-encoded version is available as XmlDomGetNodeURILen which
        returns the URI as a pointer and length, for use if the data is
        known to use XMLType backing store.

    EXAMPLE
        <pfx:foo xmlns:pfx="example_namespace"/>
        XmlDomGetNodeURI(element pfx:foo) --> "example_namespace\0"

    RETURNS
        (oratext *) namespace URI of node [data encoding; may be NULL]

    ARGUMENTS
        xctx (IN) XML context
        node (IN) XML node

    SEE ALSO
        XmlDomGetNodeURILen, XmlDomGetNodePrefix, XmlDomGetNodeLocal
*/
#define XMLDOM_GET_NODE_URI_CB getNodeURI_xmldomcb

#define XMLDOM_GET_NODE_URI_F(func, xctx, node) \
    oratext* func(xmlctx *xctx, xmlnode *node)

    XMLDOM_GET_NODE_URI_F((*XMLDOM_GET_NODE_URI_CB), xctx, node);

#define XmlDomGetNodeURI(xctx, node) \
    (*XML_DOMCB(xctx)->XMLDOM_GET_NODE_URI_CB) \
        ((xctx), (node))

/*---------------------------------------------------------------------------
    NAME
        XmlDomGetNodeURILen

    PURPOSE
        Return namespace URI of node as length-encoded string

    DESCRIPTION
        Returns the namespace URI for a node (in the data encoding) as
        length-encoded string.  If the node's name is not qualified (does
        not contain a namespace prefix), it will have the default namespace
        in effect when the node was created (which may be NULL).

        A NULL-terminated version is available as XmlDomGetNodeURI which
        returns the URI value as NULL-terminated string.  If the backing
        store is known to be XMLType, then the node's data will be stored
        internally as length-encoded.  Using the length-based Get functions
        will avoid having to copy and NULL-terminate the data.

        If both the input buffer is non-NULL and the input buffer length is
        non-zero, then the value will be stored in the input buffer.  Else,
        the implementation will return its own buffer.

        If the actual length is greater than buflen, then a truncated value
        will be copied into the buffer and len will return the actual length.

    EXAMPLE
        <pfx:foo xmlns:pfx="example_namespace"/>
        XmlDomGetNodeURILen(element pfx:foo) --> ("example_namespace", 17)

    RETURNS
        (oratext *) namespace URI of node [data encoding; may be NULL]

    ARGUMENTS
        xctx   (IN)  XML context
        node   (IN)  XML node
        buf    (IN)  input buffer [optional]
        buflen (IN)  input buffer length [optional]
        len    (OUT) length of URI [in characters]

    SEE ALSO
        XmlDomGetNodeURI, XmlDomGetNodePrefix, XmlDomGetNodeLocal
*/
#define XMLDOM_GET_NODE_URI_LEN_CB getNodeURILen_xmldomcb

#define XMLDOM_GET_NODE_URI_LEN_F(func, xctx, node, buf, buflen, len) \
    oratext* func(xmlctx *xctx, xmlnode *node, oratext *buf, ub4 buflen, \
                  ub4 *len)

    XMLDOM_GET_NODE_URI_LEN_F((*XMLDOM_GET_NODE_URI_LEN_CB), xctx, node, 
                              buf, buflen, len);

#define XmlDomGetNodeURILen(xctx, node, buf, buflen, len) \
    (*XML_DOMCB(xctx)->XMLDOM_GET_NODE_URI_LEN_CB) \
        ((xctx), (node), (buf), (buflen), (len))

/*---------------------------------------------------------------------------
    NAME
        XmlDomGetNodePrefix

    STANDARD
        DOM 2: Node.prefix

    PURPOSE
        Return namespace prefix of node

    DESCRIPTION
        Returns the namespace prefix for a node (as a NULL-terminated
        string).  If the node's name is not fully qualified (has no
        prefix), NULL is returned.

    EXAMPLE
        <pfx:foo xmlns:pfx="example_namespace"/>
        XmlDomGetNodePrefix(element pfx:foo) --> "pfx"

    RETURNS
        (oratext *) namespace prefix of node [data encoding; may be NULL]

    ARGUMENTS
        xctx (IN) XML context
        node (IN) XML node
*/
#define XMLDOM_GET_NODE_PREFIX_CB getNodePrefix_xmldomcb

#define XMLDOM_GET_NODE_PREFIX_F(func, xctx, node) \
    oratext* func(xmlctx *xctx, xmlnode *node)

    XMLDOM_GET_NODE_PREFIX_F((*XMLDOM_GET_NODE_PREFIX_CB), xctx, node);

#define XmlDomGetNodePrefix(xctx, node) \
    (*XML_DOMCB(xctx)->XMLDOM_GET_NODE_PREFIX_CB) \
        ((xctx), (node))

/*---------------------------------------------------------------------------
    NAME
        XmlDomSetNodePrefix

    PURPOSE
        Set namespace prefix of node

    DESCRIPTION
        Sets the namespace prefix of node (as NULL-terminated string).
        Does not verify the prefix is defined!  Just causes a new QName
        to be formed from the new prefix and the old local name; the new
        QName will be under DOM control and should not be managed by the
        user.

    RETURNS
        (void)

    ARGUMENTS
        xctx   (IN) XML context
        node   (IN) XML node
        prefix (IN) new namespace prefix
*/
#define XMLDOM_SET_NODE_PREFIX_CB setNodePrefix_xmldomcb

#define XMLDOM_SET_NODE_PREFIX_F(func, xctx, node, prefix) \
    void func(xmlctx *xctx, xmlnode *node, oratext *prefix)

    XMLDOM_SET_NODE_PREFIX_F((*XMLDOM_SET_NODE_PREFIX_CB), xctx, node, prefix);

#define XmlDomSetNodePrefix(xctx, node, prefix) \
    (*XML_DOMCB(xctx)->XMLDOM_SET_NODE_PREFIX_CB) \
        ((xctx), (node), (prefix))

/*---------------------------------------------------------------------------
    NAME
        XmlDomGetNodeLocal

    STANDARD
        DOM 2: Node.localName

    PURPOSE
        Get local part of node's qualified name as NULL-terminated string

    DESCRIPTION
        Returns the namespace local name for a node as a NULL-terminated
        string.  If the node's name is not fully qualified (has no prefix),
        then the local name is the same as the name.

        A length-encoded version is available as XmlDomGetNodeLocalLen which
        returns the local name as a pointer and length, for use if the data is
        known to use XMLType backing store.

    EXAMPLE
        <pfx:foo xmlns:pfx="example_namespace"/>
        XmlDomGetNodeLocal(element pfx:foo) --> "foo\0"

    RETURNS
        (oratext *) local name of node [data encoding]

    ARGUMENTS
        xctx (IN) XML context
        node (IN) XML node

    SEE ALSO
        XmlDomGetNodeLocalLen, XmlDomGetNodePrefix, XmlDomGetNodeURI
*/
#define XMLDOM_GET_NODE_LOCAL_CB getNodeLocal_xmldomcb

#define XMLDOM_GET_NODE_LOCAL_F(func, xctx, node) \
    oratext* func(xmlctx *xctx, xmlnode *node)

    XMLDOM_GET_NODE_LOCAL_F((*XMLDOM_GET_NODE_LOCAL_CB), xctx, node);

#define XmlDomGetNodeLocal(xctx, node) \
    (*XML_DOMCB(xctx)->XMLDOM_GET_NODE_LOCAL_CB) \
        ((xctx), (node))

/*---------------------------------------------------------------------------
    NAME
        XmlDomGetNodeLocalLen

    PURPOSE
        Get local part of node's qualified name as length-encoded string

    DESCRIPTION
        Returns the namespace local name for a node as a length-encoded
        string.  If the node's name is not fully qualified (has no prefix),
        then the local name is the same as the name.

        A NULL-terminated version is available as XmlDomGetNodeLocal which
        returns the local name as NULL-terminated string.  If the backing
        store is known to be XMLType, then the node's data will be stored
        internally as length-encoded.  Using the length-based Get functions
        will avoid having to copy and NULL-terminate the data.

        If both the input buffer is non-NULL and the input buffer length is
        non-zero, then the value will be stored in the input buffer.  Else,
        the implementation will return its own buffer.

        If the actual length is greater than buflen, then a truncated value
        will be copied into the buffer and len will return the actual length.

    EXAMPLE
        <pfx:foo xmlns:pfx="example_namespace"/>
        XmlDomGetNodeLocalLen(element pfx:foo) --> ("foo", 3)

    RETURNS
        (oratext *) local name of node [data encoding]

    ARGUMENTS
        xctx   (IN)  XML context
        node   (IN)  XML node
        buf    (IN)  input buffer [optional]
        buflen (IN)  input buffer length [optional]
        len    (OUT) length of local name [in characters]

    SEE ALSO
        XmlDomGetNodeLocal, XmlDomGetNodePrefix, XmlDomGetNodeURILen
*/
#define XMLDOM_GET_NODE_LOCAL_LEN_CB getNodeLocalLen_xmldomcb

#define XMLDOM_GET_NODE_LOCAL_LEN_F(func, xctx, node, buf, buflen, len) \
    oratext* func(xmlctx *xctx, xmlnode *node, oratext *buf, ub4 buflen, \
                  ub4 *len)

    XMLDOM_GET_NODE_LOCAL_LEN_F((*XMLDOM_GET_NODE_LOCAL_LEN_CB),
                                xctx, node, buf, buflen, len);

#define XmlDomGetNodeLocalLen(xctx, node, buf, buflen, len) \
    (*XML_DOMCB(xctx)->XMLDOM_GET_NODE_LOCAL_LEN_CB) \
        ((xctx), (node), (buf), (buflen), (len))

/*---------------------------------------------------------------------------
    NAME
        XmlDomGetOwnerDocument

    STANDARD
        DOM 2: Node.ownerDocument

    PURPOSE
        Get the owner document of node

    DESCRIPTION
        Returns the Document node associated with a node.  Each node may
        belong to only one document, or may not be associated with any
        document at all (e.g. immediately after XmlDomCreateElem, etc).
        The "owning" document [node] is returned, or NULL for an orphan
        node.

    RETURNS
        (xmldocnode *) document node is in

    ARGUMENTS
        xctx (IN) XML context
        node (IN) XML node
*/
#define XMLDOM_GET_OWNER_DOCUMENT_CB getOwnerDocument_xmldomcb

#define XMLDOM_GET_OWNER_DOCUMENT_F(func, xctx, node) \
    xmldocnode* func(xmlctx *xctx, xmlnode *node)

    XMLDOM_GET_OWNER_DOCUMENT_F((*XMLDOM_GET_OWNER_DOCUMENT_CB), xctx, node);

#define XmlDomGetOwnerDocument(xctx, node) \
    (*XML_DOMCB(xctx)->XMLDOM_GET_OWNER_DOCUMENT_CB) \
        ((xctx), (node))

/*---------------------------------------------------------------------------
    NAME
        XmlDomCloneNode

    STANDARD
        DOM 2: Node.cloneNode()

    PURPOSE
        Clone a node

    DESCRIPTION
        Creates and returns a duplicate of a node.  The duplicate node has
        no parent.  Cloning an element copies all attributes and their values,
        including those generated by the XML processor to represent defaulted
        attributes, but it does not copy any text it contains unless it is a
        deep clone, since the text is contained in a child text node.  Cloning
        any other type of node simply returns a copy of the node.  Note that a
        clone of an unspecified attribute node is specified.  If deep is TRUE,
        all children of the node are recursively cloned, and the cloned node
        will have cloned children; a non-deep clone will have no children.

        Note that although the new duplicate node is an orphan, it is still
        bound to the document that contained the node it was cloned from.
        Thus, this interface cannot be used to duplicate the document node.
        To duplicate an entire document, create the new document and use
        XmlDomImportNode to duplicate the root element with a deep copy,
        then tie the duplicate to the new document as its root.

    RETURNS
        (xmlnode *) duplicate (cloned) node

    ARGUMENTS
        xctx (IN) XML context
        node (IN) XML node
        deep (IN) recursively clone children?

    SEE ALSO
        XmlDomImportNode
*/
#define XMLDOM_CLONE_NODE_CB cloneNode_xmldomcb

#define XMLDOM_CLONE_NODE_F(func, xctx, node, deep) \
    xmlnode* func(xmlctx *xctx, xmlnode *node, boolean deep)

    XMLDOM_CLONE_NODE_F((*XMLDOM_CLONE_NODE_CB), xctx, node, deep);

#define XmlDomCloneNode(xctx, node, deep) \
    (*XML_DOMCB(xctx)->XMLDOM_CLONE_NODE_CB) \
        ((xctx), (node), (deep))

/*---------------------------------------------------------------------------
    NAME
        XmlDomNormalize

    STANDARD
        DOM 2: Element.normalize()

    PURPOSE
        "Normalize" a node, that is, merge adjacent text nodes

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
        xctx (IN) XML context
        node (IN) XML node
*/
#define XMLDOM_NORMALIZE_CB normalize_xmldomcb

#define XMLDOM_NORMALIZE_F(func, xctx, node) \
    void func(xmlctx *xctx, xmlnode *node)

    XMLDOM_NORMALIZE_F((*XMLDOM_NORMALIZE_CB), xctx, node);

#define XmlDomNormalize(xctx, node) \
    (*XML_DOMCB(xctx)->XMLDOM_NORMALIZE_CB) \
        ((xctx), (node))

/*---------------------------------------------------------------------------
    NAME
        XmlDomGetSourceLine

    STANDARD
        DOM 2: DOMLocator.getLineNumber()

    PURPOSE
        Return source line# of node

    DESCRIPTION
        Returns the line# in the original source where the node
        started.  The first line in every input is line #1.

    RETURNS
        (ub4) line number of node in original input source

    ARGUMENTS
        xctx (IN) XML context
        node (IN) XML node
*/
#define XMLDOM_GET_SOURCE_LINE_CB getSourceLine_xmldomcb

#define XMLDOM_GET_SOURCE_LINE_F(func, xctx, node) \
    ub4 func(xmlctx *xctx, xmlnode *node)

    XMLDOM_GET_SOURCE_LINE_F((*XMLDOM_GET_SOURCE_LINE_CB), xctx, node);

#define XmlDomGetSourceLine(xctx, node) \
    (*XML_DOMCB(xctx)->XMLDOM_GET_SOURCE_LINE_CB) \
        ((xctx), (node))

/*---------------------------------------------------------------------------
    NAME
        XmlDomGetSourceEntity

    PURPOSE
        Return the entity node if the input file is an external entity

    DESCRIPTION
        Returns the extern entity node whose inclusion caused the creation
        of the given node.

    RETURNS
        (xmlentnode *) entity node if the input is from an external entity

    ARGUMENTS
        xctx (IN) XML context
        node (IN) XML node
*/
#define XMLDOM_GET_SOURCE_ENTITY_CB getSourceEntity_xmldomcb

#define XMLDOM_GET_SOURCE_ENTITY_F(func, xctx, node) \
    xmlentnode* func(xmlctx *xctx, xmlnode *node)

    XMLDOM_GET_SOURCE_ENTITY_F((*XMLDOM_GET_SOURCE_ENTITY_CB), xctx, node);

#define XmlDomGetSourceEntity(xctx, node) \
    (*XML_DOMCB(xctx)->XMLDOM_GET_SOURCE_ENTITY_CB) \
        ((xctx), (node))

/*---------------------------------------------------------------------------
    NAME
        XmlDomGetSourceLocation

    PURPOSE
        Return source location (path, URI, etc) of node

    DESCRIPTION
        Return source location (path, URI, etc) of node.  Note this will
        be in the compiler encoding, not the data encoding!

    RETURNS
        (oratext *) full path of input source [in compiler encoding]

    ARGUMENTS
        xctx (IN) XML context
        node (IN) XML node
*/
#define XMLDOM_GET_SOURCE_LOCATION_CB getSourceLocation_xmldomcb

#define XMLDOM_GET_SOURCE_LOCATION_F(func, xctx, node) \
    oratext* func(xmlctx *xctx, xmlnode *node)

    XMLDOM_GET_SOURCE_LOCATION_F((*XMLDOM_GET_SOURCE_LOCATION_CB), xctx, node);

#define XmlDomGetSourceLocation(xctx, node) \
    (*XML_DOMCB(xctx)->XMLDOM_GET_SOURCE_LOCATION_CB) \
        ((xctx), (node))

/*---------------------------------------------------------------------------
    NAME
        XmlDomGetDocOrder

    PURPOSE
        Get document order for a node

    STATUS
        Private

    DESCRIPTION
        Returns the document order ordinal for a node.  XmlDomSetDocOrder
        must have been called first or all nodes will have a 0 order!  Used
        primarily by the XSLT processor, which requires document order for
        node comparisons.  Nodes are numbered by the order in which they
        would be encountered in a preorder traversal of the tree.  The
        root node's order is specified to XmlDomSetDocOrder, but regardless
        of the absolute numbering, a node which comes first in the document
        will have a lower order than a node which comes later.

    RETURNS
        (ub4) document order ordinal for a node

    ARGUMENTS
        xctx (IN) XML context
        node (IN) XML node

    SEE ALSO
        XmlDomSetDocOrder, XmlDomCompareDocOrder
*/
#define XMLDOM_GET_DOC_ORDER_CB getDocOrder_xmldomcb

#define XMLDOM_GET_DOC_ORDER_F(func, xctx, node) \
    ub4 func(xmlctx *xctx, xmlnode *node)

    XMLDOM_GET_DOC_ORDER_F((*XMLDOM_GET_DOC_ORDER_CB), xctx, node);

#define XmlDomGetDocOrder(xctx, node) \
    (*XML_DOMCB(xctx)->XMLDOM_GET_DOC_ORDER_CB) \
        ((xctx), (node))

/*---------------------------------------------------------------------------
    NAME
        XmlDomCompareDocOrder

    PURPOSE
        Compare the document order of two nodes

    STATUS
        Private

    DESCRIPTION
        Compares the document order of two nodes, returning a strcmp-like
        signed comparison: < 0 if node1 is before node2, 0 if node1 and
        node2 are the same; > 0 if node1 comes after node2.

    RETURNS
        (sb4) three-state comparison value

    ARGUMENTS
        xctx  (IN) XML context
        node1 (IN) first node to test
        node2 (IN) second node to test
*/
#define XMLDOM_CMP_DOC_ORDER_CB compareDocOrder_xmldomcb

#define XMLDOM_CMP_DOC_ORDER_F(func, xctx, node1, node2) \
    sb4 func(xmlctx *xctx, xmlnode *node1, xmlnode *node2)

    XMLDOM_CMP_DOC_ORDER_F((*XMLDOM_CMP_DOC_ORDER_CB), xctx, node1, node2);

#define XmlDomCompareDocOrder(xctx, node1, node2) \
    (*XML_DOMCB(xctx)->XMLDOM_CMP_DOC_ORDER_CB) \
        ((xctx), (node1), (node2))

/*---------------------------------------------------------------------------
    NAME
        XmlDomPrefixToURI

    STANDARD
        DOM 3: Node.lookupNamespacePrefix()

    PURPOSE
        Get namespace URI for prefix

    DESCRIPTION
        Given a namespace prefix and a node, returns the namespace URI
        mapped to that prefix.  If the given node doesn't have a matching
        prefix, its parent is tried, then its parent, and so on, all the
        way to the root node.  If the prefix is undefined, NULL is returned.

    RETURNS
        (oratext *) URI for prefix [data encoding; NULL if no match]

    ARGUMENTS
        xctx   (IN) XML context
        node   (IN) XML node
        prefix (IN) prefix to map
*/
#define XMLDOM_PREFIX_TO_URI_CB prefixToURI_xmldomcb

#define XMLDOM_PREFIX_TO_URI_F(func, xctx, node, prefix) \
    oratext* func(xmlctx *xctx, xmlnode *node, oratext *prefix)

    XMLDOM_PREFIX_TO_URI_F((*XMLDOM_PREFIX_TO_URI_CB), xctx, node, prefix);

#define XmlDomPrefixToURI(xctx, node, prefix) \
    (*XML_DOMCB(xctx)->XMLDOM_PREFIX_TO_URI_CB) \
        ((xctx), (node), (prefix))

/*---------------------------------------------------------------------------
    NAME
        XmlDomFreeNode

    PURPOSE
        Free a node allocated with XmlDomCreateXXX

    DESCRIPTION
        Free a node allocated with XmlDomCreateXXX.  Frees all resources
        associated with a node, then frees the node itself.  Certain parts
        of the node are under DOM control, and some parts may be under
        user control.  DOM keeps flags tracking who owns what, and only
        frees its own allocations.  The user is responsible for freeing
        their own parts of the node before calling XmlDomFreeNode.

    RETURNS
        (void)

    ARGUMENTS
        xctx (IN) XML context
        node (IN) node to free

    SEE ALSO
        XmlDomCleanNode
*/
#define XMLDOM_FREE_NODE_CB freeNode_xmldomcb

#define XMLDOM_FREE_NODE_F(func, xctx, node) \
    void func(xmlctx *xctx, xmlnode *node)

    XMLDOM_FREE_NODE_F((*XMLDOM_FREE_NODE_CB), xctx, node);

#define XmlDomFreeNode(xctx, node) \
    (*XML_DOMCB(xctx)->XMLDOM_FREE_NODE_CB) \
        ((xctx), (node))

/*---------------------------------------------------------------------------
    NAME
        XmlDomCleanNode

    PURPOSE
        "Clean" a node (free DOM allocations)

    DESCRIPTION
        Frees parts of the node which were allocated by DOM itself, but does
        not recurse to children or touch the node's attributes.  After
        freeing part of the node (e.g. name), a DOM call to get that part
        (e.g. XmlDomGetNodeName) should return a NULL pointer.  Used to
        manage the allocations of a node parts of which are controlled by
        DOM, and part by the user.  Calling clean frees all allocations
        may by DOM and leaves the user's allocations alone.  The user is
        responsible for freeing their own allocations.

    RETURNS
        (void)

    ARGUMENTS
        xctx (IN) XML context
        node (IN) node to clean

    SEE ALSO
        XmlDomFreeNode
*/
#define XMLDOM_CLEAN_NODE_CB cleanNode_xmldomcb

#define XMLDOM_CLEAN_NODE_F(func, xctx, node) \
    void func(xmlctx *xctx, xmlnode *node)

    XMLDOM_CLEAN_NODE_F((*XMLDOM_CLEAN_NODE_CB), xctx, node);

#define XmlDomCleanNode(xctx, node) \
    (*XML_DOMCB(xctx)->XMLDOM_CLEAN_NODE_CB) \
        ((xctx), (node))

/*---------------------------------------------------------------------------
    NAME
        XmlDomGetDefaultNS

    PURPOSE
        Get default namespace for node

    DESCRIPTION
        Gets the default namespace for a node.

    RETURNS
        (oratext *) default namespace for node [data encoding; may be NULL]

    ARGUMENTS
        xctx (IN) XML context
        node (IN) element/attribute DOM node
*/
#define XMLDOM_GET_DEFAULT_NS_CB getDefaultNS_xmldomcb

#define XMLDOM_GET_DEFAULT_NS_F(func, xctx, node) \
    oratext* func(xmlctx *xctx, xmlnode *node)

    XMLDOM_GET_DEFAULT_NS_F((*XMLDOM_GET_DEFAULT_NS_CB), xctx, node);

#define XmlDomGetDefaultNS(xctx, node) \
    (*XML_DOMCB(xctx)->XMLDOM_GET_DEFAULT_NS_CB) \
        ((xctx), (node))
    
/*---------------------------------------------------------------------------
    NAME
        XmlDomSetDefaultNS

    PURPOSE
        Set default namespace for node

    DESCRIPTION
        Set the default namespace for a node

    RETURNS
        (void)

    ARGUMENTS
        xctx  (IN) XML context
        node  (IN) element/attribute DOM node
        defns (IN) new default namespace for node
*/
#define XMLDOM_SET_DEFAULT_NS_CB setDefaultNS_xmldomcb

#define XMLDOM_SET_DEFAULT_NS_F(func, xctx, node, defns) \
    void func(xmlctx *xctx, xmlnode *node, oratext *defns)

    XMLDOM_SET_DEFAULT_NS_F((*XMLDOM_SET_DEFAULT_NS_CB), xctx, node, defns);

#define XmlDomSetDefaultNS(xctx, node, defns) \
    (*XML_DOMCB(xctx)->XMLDOM_SET_DEFAULT_NS_CB) \
        ((xctx), (node), (defns))

/*---------------------------------------------------------------------------
    NAME
        XmlDomGetFirstPfnsPair

    PURPOSE
        Get first prefix namespace pair

    DESCRIPTION
        This function is to allow implementations an opportunity to
        speedup the iteration of all available prefix-Uri bindings
        available on a given node.  It returns a state structure and
        the prefix and URI of the first prefix-URI mapping.  The
        state structure should be passed to XmlDomGetNextPfnsPair to
        fetch the remaining pairs.

    RETURNS
        (xmlpfnspair *) iterating object or NULL of no prefixes

    ARGUMENTS
        xctx   (IN)  XML context
        node   (IN)  Node whose prefix/URI pairs are to be iterated
        prefix (OUT) prefix of first mapping [data encoding]
        uri    (OUT) URI of first mapping [data encoding]
*/
#define XMLDOM_GET_FIRST_PFNS_PAIR_CB getFirstPfnsPair_xmldomcb

#define XMLDOM_GET_FIRST_PFNS_PAIR_F(func, xctx, node, prefix, uri) \
    xmlpfnspair* func(xmlctx *xctx, xmlnode *node, \
                      oratext **prefix, oratext **uri)

    XMLDOM_GET_FIRST_PFNS_PAIR_F((*XMLDOM_GET_FIRST_PFNS_PAIR_CB),
                                 xctx, node, prefix, uri);

#define XmlDomGetFirstPfnsPair(xctx, node, prefix, uri) \
    (*XML_DOMCB(xctx)->XMLDOM_GET_FIRST_PFNS_PAIR_CB) \
        ((xctx), (node), (prefix), (uri))

/*---------------------------------------------------------------------------
    NAME
        XmlDomGetNextPfnsPair

    PURPOSE
        Get subsequent prefix namespace pair

    DESCRIPTION
        This function is to allow implementations an opportunity to
        speedup the iteration of all available prefix-URI bindings
        available on a given node.  Given an iterator structure from
        XmlDomGetFirstPfnsPair, returns the next prefix-URI mapping;
        repeat calls to XmlDomGetNextPfnsPair until NULL is returned.

    RETURNS
        (xmlpfnspair *) iterating object, NULL when no more pairs

    ARGUMENTS
        xctx   (IN)  XML context
        pfns   (IN)  iterator object returned by XmlDomGetFirstPfnsPair
        prefix (OUT) prefix of next mapping [data encoding]
        uri    (OUT) URI of next mapping [data encoding]
*/
#define XMLDOM_GET_NEXT_PFNS_PAIR_CB getNextPfnsPair_xmldomcb

#define XMLDOM_GET_NEXT_PFNS_PAIR_F(func, xctx, pfnx, prefix, uri) \
    xmlpfnspair* func(xmlctx *xctx, xmlpfnspair *pfns, \
                      oratext **prefix, oratext **uri)

    XMLDOM_GET_NEXT_PFNS_PAIR_F((*XMLDOM_GET_NEXT_PFNS_PAIR_CB),
                                xctx, pfnx, prefix, uri);

#define XmlDomGetNextPfnsPair(xctx, pfns, prefix, uri) \
    (*XML_DOMCB(xctx)->XMLDOM_GET_NEXT_PFNS_PAIR_CB) \
        ((xctx), (pfns), (prefix), (uri))

/*---------------------------------------------------------------------------
    NAME
        XmlDomValidate

    PURPOSE
        Validate a node against current DTD

    DESCRIPTION
        Given a root node, validates it against the current DTD.

    RETURNS
        (xmlerr) error code, XMLERR_OK [0] means node is valid

    ARGUMENTS
        xctx   (IN) XML context
        node   (IN) node to validate
*/
#define XMLDOM_VALIDATE_CB validate_xmldomcb

#define XMLDOM_VALIDATE_F(func, xctx, node) \
    xmlerr func(xmlctx *xctx, xmlnode *node)

    XMLDOM_VALIDATE_F((*XMLDOM_VALIDATE_CB), xctx, node);

#define XmlDomValidate(xctx, node) \
    (*XML_DOMCB(xctx)->XMLDOM_VALIDATE_CB)((xctx), (node))

/****************************************************************************
                               Interface NodeList
 ****************************************************************************/

/*---------------------------------------------------------------------------
    NAME
        XmlDomGetNodeListItem

    STANDARD
        DOM 2: NodeList.item()

    PURPOSE
        Return nth node in list

    DESCRIPTION
        Return nth node in a node list.  The first item is index 0.

    RETURNS
        (xmlnode *) node at the nth position in node list [or NULL]

    ARGUMENTS
        xctx  (IN) XML context
        list  (IN) node list
        index (IN) index into list

    SEE ALSO
        XmlDomGetNodeListLength, XmlDomFreeNodeList
*/
#define XMLDOM_GET_NODELIST_ITEM_CB getNodeListItem_xmldomcb

#define XMLDOM_GET_NODELIST_ITEM_F(func, xctx, list, index) \
    xmlnode* func(xmlctx *xctx, xmlnodelist *list, ub4 index)

    XMLDOM_GET_NODELIST_ITEM_F((*XMLDOM_GET_NODELIST_ITEM_CB),
                               xctx, list, index);

#define XmlDomGetNodeListItem(xctx, list, index) \
    (*XML_DOMCB(xctx)->XMLDOM_GET_NODELIST_ITEM_CB) \
        ((xctx), (list), (index))

/*---------------------------------------------------------------------------
    NAME
        XmlDomGetNodeListLength

    STANDARD
        DOM 2: NodeList.length

    PURPOSE
        Return length of node list

    DESCRIPTION
        Returns the number of nodes in a node list (i.e. its length).
        Note that nodes are referred to by index, so the range of
        valid indexes is 0 through length-1.

    RETURNS
        (ub4) number of nodes in node list

    ARGUMENTS
        xctx (IN) XML context
        list (IN) node list

    SEE ALSO
        XmlDomGetNodeListItem, XmlDomFreeNodeList
*/
#define XMLDOM_GET_NODELIST_LENGTH_CB getNodeListLength_xmldomcb

#define XMLDOM_GET_NODELIST_LENGTH_F(func, xctx, list) \
    ub4 func(xmlctx *xctx, xmlnodelist *list)

    XMLDOM_GET_NODELIST_LENGTH_F((*XMLDOM_GET_NODELIST_LENGTH_CB), xctx, list);

#define XmlDomGetNodeListLength(xctx, list) \
    (*XML_DOMCB(xctx)->XMLDOM_GET_NODELIST_LENGTH_CB) \
        ((xctx), (list))

/*---------------------------------------------------------------------------
    NAME
        XmlDomFreeNodeList

    PURPOSE
        Free a node list returned by XmlDomGetElemsByTag etc

    DESCRIPTION
        Free a node list returned by XmlDomGetElemsByTag or related
        functions, releasing all resources associated with it.  If
        given a node list that is part of the DOM proper (such as
        the children of a node), does nothing.

    RETURNS
        (void)

    ARGUMENTS
        xctx (IN) XML context
        list (IN) node list to free

    SEE ALSO
        XmlDomGetElemsByTag, XmlDomGetElemsByTagNS, XmlDomGetChildrenByTag,
        XmlDomGetChildrenByTagNS
*/
#define XMLDOM_FREE_NODELIST_CB freeNodeList_xmldomcb

#define XMLDOM_FREE_NODELIST_F(func, xctx, list) \
    void func(xmlctx *xctx, xmlnodelist *list)

    XMLDOM_FREE_NODELIST_F((*XMLDOM_FREE_NODELIST_CB), xctx, list);

#define XmlDomFreeNodeList(xctx, list) \
    (*XML_DOMCB(xctx)->XMLDOM_FREE_NODELIST_CB) \
        ((xctx), (list))

/****************************************************************************
                               Interface NamedNodeMap
 ****************************************************************************/

/*---------------------------------------------------------------------------
    NAME
        XmlDomGetNodeMapItem

    STANDARD
        DOM 2: NamedNodeMap.item()

    PURPOSE
        Return nth node in list

    DESCRIPTION
        Retrieves an item from a NamedNodeMap, specified by name (which
        should be in the data encoding).  This is a non-namespace-aware
        function; it just matches (case sensitively) on the whole QName.
        Note this function differs from the DOM spec in that the index
        of the matching item is also returned.  Named "item" in W3C spec.

    RETURNS
        (xmlnode *) node at the nth position in the map (or NULL)

    ARGUMENTS
        xctx  (IN) XML context
        map   (IN) NamedNodeMap
        index (IN) index into this map, starts from 0

    SEE ALSO
        XmlDomGetNamedItem, XmlDomSetNamedItem, XmlDomRemoveNamedItem,
        XmlDomGetNodeMapLength
*/
#define XMLDOM_GET_NODEMAP_ITEM_CB getNodeMapItem_xmldomcb

#define XMLDOM_GET_NODEMAP_ITEM_F(func, xctx, map, index) \
    xmlnode* func(xmlctx *xctx, xmlnamedmap *map, ub4 index)

    XMLDOM_GET_NODEMAP_ITEM_F((*XMLDOM_GET_NODEMAP_ITEM_CB), xctx, map, index);

#define XmlDomGetNodeMapItem(xctx, map, index) \
    (*XML_DOMCB(xctx)->XMLDOM_GET_NODEMAP_ITEM_CB) \
        ((xctx), (map), (index))

/*---------------------------------------------------------------------------
    NAME
        XmlDomGetNodeMapLength

    STANDARD
        DOM 2: NamedNodeMap.length

    PURPOSE
        Return length of named node map

    DESCRIPTION
        Returns the number of nodes in a NamedNodeMap (the length).
        Note that nodes are referred to by index, and the range of
        valid indexes is 0 through length-1.

    RETURNS
        (ub4) number of nodes in NamedNodeMap

    ARGUMENTS
        xctx (IN) XML context
        map  (IN) NamedNodeMap

    SEE ALSO
        XmlDomGetNodeMapItem, XmlDomGetNamedItem
*/
#define XMLDOM_GET_NODEMAP_LENGTH_CB getNodeMapLength_xmldomcb

#define XMLDOM_GET_NODEMAP_LENGTH_F(func, xctx, map) \
    ub4 func(xmlctx *xctx, xmlnamedmap *map)

    XMLDOM_GET_NODEMAP_LENGTH_F((*XMLDOM_GET_NODEMAP_LENGTH_CB), xctx, map);

#define XmlDomGetNodeMapLength(xctx, map) \
    (*XML_DOMCB(xctx)->XMLDOM_GET_NODEMAP_LENGTH_CB) \
        ((xctx), (map))

/*---------------------------------------------------------------------------
    NAME
        XmlDomGetNamedItem

    STANDARD
        DOM 2: NamedNodeMap.getNamedItem()

    PURPOSE
        Return named node from list

    DESCRIPTION
        Retrieves an item from a NamedNodeMap, specified by name (which
        should be in the data encoding).  This is a non-namespace-aware
        function; it just matches (case sensitively) on the whole QName.
        Note this function differs from the DOM spec in that the index
        of the matching item is also returned.

    RETURNS
        (xmlnode *) Node with the specified name [or NULL]

    ARGUMENTS
        xctx (IN) XML context
        map  (IN) NamedNodeMap
        name (IN) name of node to retrieve [data encoding]

    SEE ALSO
        XmlDomGetNamedItemNS, XmlDomGetNodeMapItem, XmlDomGetNodeMapLength
*/
#define XMLDOM_GET_NAMED_ITEM_CB getNamedItem_xmldomcb

#define XMLDOM_GET_NAMED_ITEM_F(func, xctx, map, name) \
    xmlnode* func(xmlctx *xctx, xmlnamedmap *map, oratext *name)

    XMLDOM_GET_NAMED_ITEM_F((*XMLDOM_GET_NAMED_ITEM_CB), xctx, map, name);

#define XmlDomGetNamedItem(xctx, map, name) \
    (*XML_DOMCB(xctx)->XMLDOM_GET_NAMED_ITEM_CB) \
        ((xctx), (map), (name))

/*---------------------------------------------------------------------------
    NAME
        XmlDomGetNamedItemNS

    PURPOSE
        Return named node from list (namespace aware version)

    DOM 2 NamedNodeMap.getNamedItemNS()

    DESCRIPTION
        Retrieves an item from a NamedNodeMap, specified by URI and
        localname (which should be in the data encoding).  Note this
        function differs from the DOM spec in that the index of the
        matching item is also returned.

    RETURNS
        (xmlnode *) node with given local name and namespace URI [or NULL]

    ARGUMENTS
        xctx  (IN) XML context
        map   (IN) NamedNodeMap
        uri   (IN) namespace URI of node to retrieve [data encoding]
        local (IN) local name of node to retrieve [data encoding]

    SEE ALSO
        XmlDomGetNamedItem, XmlDomGetNodeMapItem, XmlDomGetNodeMapLength
*/
#define XMLDOM_GET_NAMED_ITEM_NS_CB getNamedItemNS_xmldomcb

#define XMLDOM_GET_NAMED_ITEM_NS_F(func, xctx, map, uri, local) \
    xmlnode* func(xmlctx *xctx, xmlnamedmap *map, \
                  oratext *uri, oratext *local)

    XMLDOM_GET_NAMED_ITEM_NS_F((*XMLDOM_GET_NAMED_ITEM_NS_CB),
                               xctx, map, uri, local);

#define XmlDomGetNamedItemNS(xctx, map, uri, local) \
    (*XML_DOMCB(xctx)->XMLDOM_GET_NAMED_ITEM_NS_CB) \
        ((xctx), (map), (uri), (local))

/*---------------------------------------------------------------------------
    NAME
        XmlDomSetNamedItem

    STANDARD
        DOM 2: NamedNodeMap.setNamedItem()

    PURPOSE
        Set node in named node list

    DESCRIPTION
        Adds a new node to a NamedNodeMap.  If a node already exists with
        the given name, replaces the old node and returns it.  If no such
        named node exists, adds the new node to the map and sets old to
        NULL.  This is a non-namespace-aware function; it just matches
        (case sensitively) on the whole QName.  Since some node types have
        fixed names (Text, Comment, etc), trying to set another of the same
        type will always cause replacement.

    RETURNS
        (xmlnode *) the replaced node (or NULL)

    ARGUMENTS
        xctx    (IN) XML context
        map     (IN) NamedNodeMap
        newNode (IN) new node to store in map

    SEE ALSO
        XmlDomSetNamedItemNS, XmlDomGetNamedItem, XmlDomGetNamedItemNS,
        XmlDomGetNodeMapItem, XmlDomGetNodeMapLength
*/
#define XMLDOM_SET_NAMED_ITEM_CB setNamedItem_xmldomcb

#define XMLDOM_SET_NAMED_ITEM_F(func, xctx, map, newNode) \
    xmlnode* func(xmlctx *xctx, xmlnamedmap *map, xmlnode *newNode)

    XMLDOM_SET_NAMED_ITEM_F((*XMLDOM_SET_NAMED_ITEM_CB), xctx, map, newNode);

#define XmlDomSetNamedItem(xctx, map, node) \
    (*XML_DOMCB(xctx)->XMLDOM_SET_NAMED_ITEM_CB) \
        ((xctx), (map), (node))

/*---------------------------------------------------------------------------
    NAME
        XmlDomSetNamedItemNS

    STANDARD
        DOM 2: NamedNodeMap.setNamedItemNS()

    PURPOSE
        Set node in named node list (namespace aware version)

    DESCRIPTION
        Adds a new node to a NamedNodeMap.  If a node already exists with
        the given URI and localname, replaces the old node and returns it.
        If no such named node exists, adds the new node to the map and sets
        old to NULL.  Since some node types have fixed names (Text, Comment,
        etc), trying to set another of the same type will always cause
        replacement.

    RETURNS
        (xmlnode *) replaced Node [or NULL]

    ARGUMENTS
        xctx    (IN) XML context
        map     (IN) NamedNodeMap
        newNode (IN) new node to store in map

    SEE ALSO
        XmlDomSetNamedItem, XmlDomGetNamedItem, XmlDomGetNamedItemNS,
        XmlDomGetNodeMapItem, XmlDomGetNodeMapLength
*/
#define XMLDOM_SET_NAMED_ITEM_NS_CB setNamedItemNS_xmldomcb

#define XMLDOM_SET_NAMED_ITEM_NS_F(func, xctx, map, newNode) \
    xmlnode* func(xmlctx *xctx, xmlnamedmap *map, xmlnode *newNode)

    XMLDOM_SET_NAMED_ITEM_NS_F((*XMLDOM_SET_NAMED_ITEM_NS_CB),
                               xctx, map, newNode);

#define XmlDomSetNamedItemNS(xctx, map, newNode) \
    (*XML_DOMCB(xctx)->XMLDOM_SET_NAMED_ITEM_NS_CB) \
        ((xctx), (map), (newNode))

/*---------------------------------------------------------------------------
    NAME
        XmlDomRemoveNamedItem

    STANDARD
        DOM 2: NamedNodeMap.removeNamedItem()

    PURPOSE
        Remove node from named node map

    DESCRIPTION
        Removes a node from a NamedNodeMap, specified by name.  This is a
        non-namespace-aware function; it just matches (case sensitively)
        on the whole QName.  If the removed node is an attribute with default
        value (not specified), it is immediately replaced.  The removed node
        is returned; if no removal took place, NULL is returned.

    RETURNS
        (xmlnode *) node removed from this map

    ARGUMENTS
        xctx (IN) XML context
        map  (IN) NamedNodeMap
        name (IN) name of node to remove [data encoding]

    SEE ALSO
        XmlDomRemoveNamedItemNS, XmlDomGetNamedItem, XmlDomGetNamedItemNS,
        XmlDomSetNamedItem, XmlDomSetNamedItemNS
*/
#define XMLDOM_REMOVE_NAMED_ITEM_CB removeNamedItem_xmldomcb

#define XMLDOM_REMOVE_NAMED_ITEM_F(func, xctx, map, name) \
    xmlnode* func(xmlctx *xctx, xmlnamedmap *map, oratext *name)

    XMLDOM_REMOVE_NAMED_ITEM_F((*XMLDOM_REMOVE_NAMED_ITEM_CB), xctx,map,name);

#define XmlDomRemoveNamedItem(xctx, map, name) \
    (*XML_DOMCB(xctx)->XMLDOM_REMOVE_NAMED_ITEM_CB) \
        ((xctx), (map), (name))

/*---------------------------------------------------------------------------
    NAME
        XmlDomRemoveNamedItemNS

    STANDARD
        DOM 2: NamedNodeMap.removeNamedItemNS()

    PURPOSE
        Remove node from named node map (namespace aware version)

    DESCRIPTION
        Removes a node from a NamedNodeMap, specified by URI and localname.
        If the removed node is an attribute with default value (not specified),
        it is immediately replaced.  The removed node is returned; if no
        removal took place, NULL is returned.

    RETURNS
        (xmlnode *) node removed from this map

    ARGUMENTS
        xctx  (IN) XML context
        map   (IN) NamedNodeMap
        uri   (IN) namespace URI of node to remove [data encoding]
        local (IN) local name of node to remove [data encoding]

    SEE ALSO
        XmlDomRemoveNamedItem, XmlDomGetNamedItem, XmlDomGetNamedItemNS,
        XmlDomSetNamedItem, XmlDomSetNamedItemNS
*/
#define XMLDOM_REMOVE_NAMED_ITEM_NS_CB removeNamedItemNS_xmldomcb

#define XMLDOM_REMOVE_NAMED_ITEM_NS_F(func, xctx, map, uri, local) \
    xmlnode* func(xmlctx *xctx, xmlnamedmap *map, \
                  oratext *uri, oratext *local)

    XMLDOM_REMOVE_NAMED_ITEM_NS_F((*XMLDOM_REMOVE_NAMED_ITEM_NS_CB),
                                  xctx, map, uri, local);

#define XmlDomRemoveNamedItemNS(xctx, map, uri, local) \
    (*XML_DOMCB(xctx)->XMLDOM_REMOVE_NAMED_ITEM_NS_CB) \
        ((xctx), (map), (uri), (local))

/****************************************************************************
                               Interface CharacterData
 ****************************************************************************/

/*---------------------------------------------------------------------------
    NAME
        XmlDomGetCharData

    STANDARD
        DOM 2: CharacterData.data

    PURPOSE
        Return data for node

    DESCRIPTION
        Returns the data for a CharacterData node (type text, comment or
        CDATA) in the data encoding.  For other node types, or if there
        is no data, returns NULL.

    RETURNS
        (oratext *) character data of node [data encoding]

    ARGUMENTS
        xctx (IN) XML context
        node (IN) CharacterData node [text, comment, or CDATA]

    SEE ALSO
        XmlDomSetCharData, XmlDomCreateText, XmlDomCreateComment,
        XmlDomCreateCDATA
*/
#define XMLDOM_GET_CHAR_DATA_CB getCharData_xmldomcb

#define XMLDOM_GET_CHAR_DATA_F(func, xctx, node) \
    oratext* func(xmlctx *xctx, xmlnode *node)

    XMLDOM_GET_CHAR_DATA_F((*XMLDOM_GET_CHAR_DATA_CB), xctx, node);

#define XmlDomGetCharData(xctx, node) \
    (*XML_DOMCB(xctx)->XMLDOM_GET_CHAR_DATA_CB) \
        ((xctx), (node))

/*---------------------------------------------------------------------------
    NAME
        XmlDomSetCharData

    STANDARD
        DOM 2: CharacterData.data

    PURPOSE
        Set data for node

    DESCRIPTION
        Sets data for a CharacterData node (type text, comment or CDATA),
        replacing the old data.  For other node types, does nothing.  The
        new data is not verified, converted, or checked-- it should be in
        the data encoding.  If bad data is set, bad things will happen-- GIGO!

    RETURNS
        (void)

    ARGUMENTS
        xctx (IN) XML context
        node (IN) CharacterData node [text, comment, or CDATA]
        data (IN) new data for node

    SEE ALSO
        XmlDomGetCharData
*/
#define XMLDOM_SET_CHAR_DATA_CB setCharData_xmldomcb

#define XMLDOM_SET_CHAR_DATA_F(func, xctx, node, data) \
    void func(xmlctx *xctx, xmlnode *node, oratext *data)

    XMLDOM_SET_CHAR_DATA_F((*XMLDOM_SET_CHAR_DATA_CB), xctx, node, data);

#define XmlDomSetCharData(xctx, node, data) \
    (*XML_DOMCB(xctx)->XMLDOM_SET_CHAR_DATA_CB) \
        ((xctx), (node), (data))

/*---------------------------------------------------------------------------
    NAME
        XmlDomGetCharDataLength

    STANDARD
        DOM 2: CharacterData.length

    PURPOSE
        Return length of data for node

    DESCRIPTION
        Returns the length of the data for a CharacterData node (type text,
        comment or CDATA) in characters (not bytes!).  For other node types,
        returns 0.

    RETURNS
        (ub4) length in characters (not bytes!) of node's data

    ARGUMENTS
        xctx (IN) XML context
        node (IN) CharacterData node [text, comment, or CDATA]

    SEE ALSO
        XmlDomGetCharData
*/
#define XMLDOM_GET_CHAR_DATA_LENGTH_CB getCharDataLength_xmldomcb

#define XMLDOM_GET_CHAR_DATA_LENGTH_F(func, xctx, cdata) \
    ub4 func(xmlctx *xctx, xmlnode *cdata)

    XMLDOM_GET_CHAR_DATA_LENGTH_F((*XMLDOM_GET_CHAR_DATA_LENGTH_CB),
                                  xctx, cdata);

#define XmlDomGetCharDataLength(xctx, node) \
    (*XML_DOMCB(xctx)->XMLDOM_GET_CHAR_DATA_LENGTH_CB) \
        ((xctx), (node))

/*---------------------------------------------------------------------------
    NAME
        XmlDomSubstringData

    STANDARD
        DOM 2: CharacterData.substringData()

    PURPOSE
        Return substring of node's data

    DESCRIPTION
        Returns a range of character data from a CharacterData node (type
        text, comment or CDATA).  For other node types, or if count is zero,
        returns NULL.  Since the data is in the data encoding, offset and
        count are in characters, not bytes.  The beginning of the string is
        offset 0.  If the sum of offset and count exceeds the length, then
        all characters to the end of the data are returned.

        The substring is permanently allocated in the node's document's
        memory pool.  To free the substring, use XmlDomFreeString.

    EXAMPLE
        Given a node with data "abcdefgh",
        XmlDomSubstringData(offset=0, length=1) --> "a"
        XmlDomSubstringData(offset=5, length=5) --> "fgh"

    RETURNS
        (oratext *) specified substring.

    ARGUMENTS
        xctx   (IN) XML context
        node   (IN) CharacterData node [text, comment, or CDATA]
        offset (IN) start offset of substring to extract
        count  (IN) number of characters to extract

    SEE ALSO
        XmlDomAppendData, XmlDomInsertData, XmlDomDeleteData,
        XmlDomReplaceData, XmlDomSplitText, XmlDomFreeString
*/
#define XMLDOM_SUBSTRING_DATA_CB substringData_xmldomcb

#define XMLDOM_SUBSTRING_DATA_F(func, xctx, node, offset, count) \
    oratext* func(xmlctx *xctx, xmlnode *node, ub4 offset, ub4 count)

    XMLDOM_SUBSTRING_DATA_F((*XMLDOM_SUBSTRING_DATA_CB),
                            xctx, node, offset, count);

#define XmlDomSubstringData(xctx, node, offset, count) \
    (*XML_DOMCB(xctx)->XMLDOM_SUBSTRING_DATA_CB) \
        ((xctx), (node), (offset), (count))

/*---------------------------------------------------------------------------
    NAME
        XmlDomAppendData

    STANDARD
        DOM 2: Node.appendData()

    PURPOSE
        Append data to end of node's current data

    DESCRIPTION
        Append a string to the end of a CharacterData node's data.  If the
        node is not text, comment or CDATA, or if the string to append
        is NULL, does nothing.  The appended data should be in the data
        encoding.  It will not be verified, converted, or checked.  If bad
        data is appended, bad things will happen-- GIGO!

        The new node data will be allocated and managed by DOM, but if the
        previous node value was allocated and manager by the user, they
        are responsible for freeing it (which is why it's returned).

    RETURNS
        (void)

    ARGUMENTS
        xctx (IN)  XML context
        node (IN)  CharacterData node [text, comment, or CDATA]
        data (IN)  data to append [data encoding]
        old  (OUT) previous data for node [data encoding]

    SEE ALSO
        XmlDomGetCharData, XmlDomInsertData, XmlDomDeleteData,
        XmlDomReplaceData, XmlDomSplitText
*/
#define XMLDOM_APPEND_DATA_CB appendData_xmldomcb

#define XMLDOM_APPEND_DATA_F(func, xctx, node, data, old) \
    void func(xmlctx *xctx, xmlnode *node, oratext *data, oratext **old)

    XMLDOM_APPEND_DATA_F((*XMLDOM_APPEND_DATA_CB), xctx, node, data, old);

#define XmlDomAppendData(xctx, node, data, old) \
    (*XML_DOMCB(xctx)->XMLDOM_APPEND_DATA_CB) \
        ((xctx), (node), (data), (old))

/*---------------------------------------------------------------------------
    NAME
        XmlDomInsertData

    STANDARD
        DOM 2: CharacterData.insertData()

    PURPOSE
        Insert string into node's current data

    DESCRIPTION
        Insert a string into a CharacterData node's data at the specified
        position.  If the node is not text, comment or CDATAS, or if
        the data to be inserted is NULL, or the offset is outside the
        original data, does nothing.  The inserted data must be in the
        data encoding.  It will not be verified, converted, or checked.
        If bad data is appended, bad things will happen-- GIGO!   The offset
        is specified as characters, not bytes.  The offset is zero-based,
        so inserting at offset zero prepends the data.

        The new node data will be allocated and managed by DOM, but if the
        previous node value was allocated and managed by the user, they
        are responsible for freeing it (which is why it's returned).

    EXAMPLE
        {Given a node with data} "abcdefgh"
        InsertData(offset=0, arg="FOO") --> "FOOabcdefgh"
        InsertData(offset=5, arg="*") --> "abcde*fgh"

    RETURNS
        (void)

    ARGUMENTS
        xctx   (IN)  XML context
        node   (IN)  CharacterData node [Text, Comment, or CDATA]
        offset (IN)  character offset at which to insert
        arg    (IN)  data to insert [data encoding]
        old    (OUT) previous data for node [data encoding]

    SEE ALSO
        XmlDomGetCharData, XmlDomAppendData, XmlDomDeleteData,
        XmlDomReplaceData, XmlDomSplitText
*/
#define XMLDOM_INSERT_DATA_CB insertData_xmldomcb

#define XMLDOM_INSERT_DATA_F(func, xctx, node, offset, arg, old) \
    void func(xmlctx *xctx, xmlnode *node, ub4 offset, \
              oratext *arg, oratext **old)

    XMLDOM_INSERT_DATA_F((*XMLDOM_INSERT_DATA_CB), xctx, node, offset,arg,old);

#define XmlDomInsertData(xctx, node, offset, data, old) \
    (*XML_DOMCB(xctx)->XMLDOM_INSERT_DATA_CB) \
        ((xctx), (node), (offset), (data), (old))

/*---------------------------------------------------------------------------
    NAME
        XmlDomDeleteData

    STANDARD
        DOM 2: CharacterData.deleteData()

    PURPOSE
        Remove part of node's data

    DESCRIPTION
        Remove a range of characters from a CharacterData node's data.
        If the node is not text, comment or CDATA, or if the offset is
        outside of the original data, does nothing.  The offset is
        zero-based, so offset zero refers to the start of the data.
        Both offset and count are in characters, not bytes.  If the sum
        of offset and count exceeds the data length then all characters
        from offset to the end of the data are deleted.

        The new node data will be allocated and managed by DOM, but if the
        previous node value was allocated and managed by the user, they
        are responsible for freeing it (which is why it's returned).

    EXAMPLE
        {Given a node with data} "abcdefgh",
        DeleteData(offset=0, count=2) --> "cdefgh"
        DeleteData(offset=5, count=5) --> "abcde"

    RETURNS
        (void)

    ARGUMENTS
        xctx   (IN)  XML context
        node   (IN)  CharacterData node [Text, Comment, or CDATA]
        offset (IN)  character offset from which to start removing
        count  (IN)  number of characters to delete
        out    (OUT) previous data for node [data encoding]

    SEE ALSO
        XmlDomGetCharData, XmlDomAppendData, XmlDomInsertData,
        XmlDomReplaceData, XmlDomSplitText
*/
#define XMLDOM_DELETE_DATA_CB deleteData_xmldomcb

#define XMLDOM_DELETE_DATA_F(func, xctx, node, offset, count, old) \
    void func(xmlctx *xctx, xmlnode *node, ub4 offset, \
              ub4 count, oratext **old)

    XMLDOM_DELETE_DATA_F((*XMLDOM_DELETE_DATA_CB),
                         xctx, node, offset, count, old);

#define XmlDomDeleteData(xctx, node, offset, count, old) \
    (*XML_DOMCB(xctx)->XMLDOM_DELETE_DATA_CB) \
        ((xctx), (node), (offset), (count), (old))

/*---------------------------------------------------------------------------
    NAME
        XmlDomReplaceData

    STANDARD
        DOM 2: CharacterData.replaceData()

    PURPOSE
        Replace part of node's data

    DESCRIPTION
        Replaces a range of characters in a CharacterData node's data with
        a new string.  If the node is not text, comment or CDATA, or
        if the offset is outside of the original data, or if the replacement
        string is NULL, does nothing.  If the count is zero, acts just as
        XmlDomInsertData.  The offset is zero-based, so offset zero refers
        to the start of the data.  The replacement data must be in the data
        encoding.  It will not be verified, converted, or checked.  If bad
        data is set, bad things will happen-- GIGO!   The offset and count
        are both in characters, not bytes.  If the sum of offset and count
        exceeds length, then all characters to the end of the data are
        replaced.

        The new node data will be allocated and managed by DOM, but if the
        previous node value was allocated and managed by the user, they
        are responsible for freeing it (which is why it's returned).

    EXAMPLE
        Given a node with data "abcdefgh",
        XmlDomReplaceData(offset=0, count=1, arg="FOO") --> "FOObcdefgh"
        XmlDomRreplaceData(offset=2, count=2, arg="*") --> "ab*efgh"
        XmlDomRreplaceData(offset=5, count=5, arg="*") --> "abcde*"

    RETURNS
        (void)

    ARGUMENTS
        xctx   (IN)  XML context
        node   (IN)  CharacterData node [text, comment, or CDATA]
        offset (IN)  character offset from which to start replacing
        count  (IN)  number of characters to replace
        arg    (IN)  replacement substring [data encoding]
        out    (OUT) previous data for node [data encoding]

    SEE ALSO
        XmlDomGetCharData, XmlDomAppendData, XmlDomInsertData,
        XmlDomDeleteData, XmlDomSplitText
*/
#define XMLDOM_REPLACE_DATA_CB replaceData_xmldomcb

#define XMLDOM_REPLACE_DATA_F(func, xctx, node, offset, count, arg, old) \
    void func(xmlctx *xctx, xmlnode *node, ub4 offset, \
              ub4 count, oratext *arg, oratext **old)

    XMLDOM_REPLACE_DATA_F((*XMLDOM_REPLACE_DATA_CB),
                          xctx, node, offset, count, arg, old);

#define XmlDomReplaceData(xctx, node, offset, count, data, old) \
    (*XML_DOMCB(xctx)->XMLDOM_REPLACE_DATA_CB) \
        ((xctx), (node), (offset), (count), (data), (old))

/****************************************************************************
                              Interface Attr
 ****************************************************************************/

/*---------------------------------------------------------------------------
    NAME
        XmlDomGetAttrName

    STANDARD
        DOM 2: Attr.name

    PURPOSE
        Returns an attribute's name as NULL-terminated string

    DESCRIPTION
        Returns the fully-qualified name of an attribute (in the data
        encoding) as a NULL-terminated string, for example "bar\0" or
        "foo:bar\0".

        A length-encoded version is available as XmlDomGetAttrNameLen which
        returns the attribute name as a pointer and length, for use if the 
        data is known to use XMLType backing store.

    RETURNS
        (oratext *) name of attribute [data encoding]

    ARGUMENTS
        xctx (IN) XML context
        attr (IN) attribute node

    SEE ALSO
        XmlDomGetAttrNameLen, XmlDomGetAttrURI, XmlDomGetAttrPrefix,
        XmlDomGetAttrLocal
*/
#define XMLDOM_GET_ATTR_NAME_CB getAttrName_xmldomcb

#define XMLDOM_GET_ATTR_NAME_F(func, xctx, attr) \
    oratext* func(xmlctx *xctx, xmlattrnode *attr)

    XMLDOM_GET_ATTR_NAME_F((*XMLDOM_GET_ATTR_NAME_CB), xctx, attr);

#define XmlDomGetAttrName(xctx, attr) \
    (*XML_DOMCB(xctx)->XMLDOM_GET_ATTR_NAME_CB) \
        ((xctx), (attr))

/*---------------------------------------------------------------------------
    NAME
        XmlDomGetAttrNameLen

    PURPOSE
        Returns an attribute's name as length-encoded string

    DESCRIPTION
        Returns the fully-qualified name of an attribute (in the data
        encoding) as a length-encoded string, for example ("bar", 3) or
        ("foo:bar", 7).

        A NULL-terminated version is available as XmlDomGetAttrName which
        returns the attribute name as NULL-terminated string.  If the backing
        store is known to be XMLType, then the attribute's data will be stored
        internally as length-encoded.  Using the length-based Get functions
        will avoid having to copy and NULL-terminate the data.

        If both the input buffer is non-NULL and the input buffer length is
        non-zero, then the value will be stored in the input buffer.  Else,
        the implementation will return its own buffer.

        If the actual length is greater than buflen, then a truncated value
        will be copied into the buffer and len will return the actual length.

    RETURNS
        (oratext *) name of attribute [data encoding]

    ARGUMENTS
        xctx   (IN)  XML context
        attr   (IN)  attribute node
        buf    (IN)  input buffer [optional]
        buflen (IN)  input buffer length [optional]
        len    (OUT) length of name [in characters]

    SEE ALSO
        XmlDomGetAttrName, XmlDomGetAttrURI, XmlDomGetAttrPrefix,
        XmlDomGetAttrLocal
*/
#define XMLDOM_GET_ATTR_NAME_LEN_CB getAttrNameLen_xmldomcb

#define XMLDOM_GET_ATTR_NAME_LEN_F(func, xctx, attr, buf, buflen, len) \
    oratext* func(xmlctx *xctx, xmlattrnode *attr, oratext *buf, ub4 buflen, \
                  ub4 *len)

    XMLDOM_GET_ATTR_NAME_LEN_F((*XMLDOM_GET_ATTR_NAME_LEN_CB), xctx, attr, buf,
                               buflen, len);

#define XmlDomGetAttrNameLen(xctx, attr, buf, buflen, len) \
    (*XML_DOMCB(xctx)->XMLDOM_GET_ATTR_NAME_LEN_CB) \
        ((xctx), (attr), (buf), (buflen), (len))

/*---------------------------------------------------------------------------
    NAME
        XmlDomGetAttrURI

    STANDARD
        DOM 2: Node.namespaceURI

    PURPOSE
        Returns an attribute's namespace URI as NULL-terminated string

    DESCRIPTION
        Returns an attribute's namespace URI (in the data encoding).  If
        the attribute's name is not qualified (does not contain a namespace
        prefix), it will have the default namespace in effect when the node
        was created (which may be NULL).

        A length-encoded version is available as XmlDomGetAttrURILen which
        returns the URI as a pointer and length, for use if the 
        data is known to use XMLType backing store.

    EXAMPLE
        <foo xmlns:pfx="example_namespace" pfx:bar="bar_value"/>
        XmlDomGetAttrURI(attr pfx:bar) --> "example_namespace\0"

    RETURNS
        (oratext *) attribute's namespace URI [data encoding] or NULL

    ARGUMENTS
        xctx (IN) XML context
        attr (IN) attribute node

    SEE ALSO
        XmlDomGetAttrURILen, XmlDomGetAttrPrefix, XmlDomGetAttrLocal
*/
#define XMLDOM_GET_ATTR_URI_CB getAttrURI_xmldomcb

#define XMLDOM_GET_ATTR_URI_F(func, xctx, attr) \
    oratext* func(xmlctx *xctx, xmlattrnode *attr)

    XMLDOM_GET_ATTR_URI_F((*XMLDOM_GET_ATTR_URI_CB), xctx, attr);

#define XmlDomGetAttrURI(xctx, attr) \
    (*XML_DOMCB(xctx)->XMLDOM_GET_ATTR_URI_CB) \
        ((xctx), (attr))

/*---------------------------------------------------------------------------
    NAME
        XmlDomGetAttrURILen

    PURPOSE
        Returns an attribute's namespace URI as length-encoded string

    DESCRIPTION
        Returns an attribute's namespace URI (in the data encoding) as
        length-encoded string.  If the attribute's name is not qualified
        (does not contain a namespace prefix), it will have the default
        namespace in effect when the node was created (which may be NULL).

        A NULL-terminated version is available as XmlDomGetAttrURI which
        returns the URI as NULL-terminated string.  If the backing
        store is known to be XMLType, then the attribute's data will be stored
        internally as length-encoded.  Using the length-based Get functions
        will avoid having to copy and NULL-terminate the data.

        If both the input buffer is non-NULL and the input buffer length is
        non-zero, then the value will be stored in the input buffer.  Else,
        the implementation will return its own buffer.

        If the actual length is greater than buflen, then a truncated value
        will be copied into the buffer and len will return the actual length.

    EXAMPLE
        <foo xmlns:pfx="example_namespace" pfx:bar="bar_value"/>
        XmlDomGetAttrURILen(attr pfx:bar) --> ("example_namespace", 17)

    RETURNS
        (oratext *) attribute's namespace URI [data encoding] or NULL

    ARGUMENTS
        xctx   (IN)  XML context
        attr   (IN)  attribute node
        buf    (IN)  input buffer [optional]
        buflen (IN)  input buffer length [optional]
        len    (OUT) length of URI [in characters]

    SEE ALSO
        XmlDomGetAttrURI, XmlDomGetAttrPrefix, XmlDomGetAttrLocal
*/
#define XMLDOM_GET_ATTR_URI_LEN_CB getAttrURILen_xmldomcb

#define XMLDOM_GET_ATTR_URI_LEN_F(func, xctx, attr, buf, buflen, len) \
    oratext* func(xmlctx *xctx, xmlattrnode *attr, oratext *buf, ub4 buflen, \
                  ub4 *len)

    XMLDOM_GET_ATTR_URI_LEN_F((*XMLDOM_GET_ATTR_URI_LEN_CB), xctx, attr, 
                              buf, buflen, len);

#define XmlDomGetAttrURILen(xctx, attr, buf, buflen, len) \
    (*XML_DOMCB(xctx)->XMLDOM_GET_ATTR_URI_LEN_CB) \
        ((xctx), (attr), (buf), (buflen), (len))

/*---------------------------------------------------------------------------
    NAME
        XmlDomGetAttrPrefix

    STANDARD
        DOM 2: Node.prefix

    PURPOSE
        Returns an attribute's namespace prefix

    DESCRIPTION
        Returns an attribute's namespace prefix (in the data encoding).  If
        the attribute's name is not fully qualified (has no prefix), NULL
        is returned.

    EXAMPLE
        <foo xmlns:pfx="example_namespace" pfx:bar="bar_value"/>
        XmlDomGetAttrPrefix(attr pfx:bar) --> "pfx\0"

    RETURNS
        (oratext *) attribute's namespace prefix [data encoding] or NULL

    ARGUMENTS
        xctx (IN) XML context
        attr (IN) attribute node

    SEE ALSO
        XmlDomGetAttrName, XmlDomGetAttrURI, XmlDomGetAttrLocal
*/
#define XMLDOM_GET_ATTR_PFX_CB getAttrPrefix_xmldomcb

#define XMLDOM_GET_ATTR_PFX_F(func, xctx, attr) \
    oratext* func(xmlctx *xctx, xmlattrnode *attr)

    XMLDOM_GET_ATTR_PFX_F((*XMLDOM_GET_ATTR_PFX_CB), xctx, attr);

#define XmlDomGetAttrPrefix(xctx, attr) \
    (*XML_DOMCB(xctx)->XMLDOM_GET_ATTR_PFX_CB) \
        ((xctx), (attr))

/*---------------------------------------------------------------------------
    NAME
        XmlDomGetAttrLocal

    STANDARD
        DOM 2: Node.localName

    PURPOSE
        Returns an attribute's namespace local name as NULL-terminated string

    DESCRIPTION
        Returns an attribute's namespace local name (in the data encoding).
        If the attribute's name is not fully qualified (has no prefix), then
        the local name is the same as the name.

        A length-encoded version is available as XmlDomGetAttrURILen which
        returns the local name as a pointer and length, for use if the 
        data is known to use XMLType backing store.

    EXAMPLE
        <foo xmlns:pfx="example_namespace" pfx:bar="bar_value"/>
        XmlDomGetAttrLocal(attr pfx:bar) --> "bar\0"

    RETURNS
        (oratext *) attribute's local name [data encoding]

    ARGUMENTS
        xctx (IN) XML context
        attr (IN) attribute node

    SEE ALSO
        XmlDomGetAttrLocalLen, XmlDomGetAttrName, XmlDomGetAttrURI,
        XmlDomGetAttrPrefix
*/
#define XMLDOM_GET_ATTR_LOCAL_CB getAttrLocal_xmldomcb

#define XMLDOM_GET_ATTR_LOCAL_F(func, xctx, attr) \
    oratext* func(xmlctx *xctx, xmlattrnode *attr)

    XMLDOM_GET_ATTR_LOCAL_F((*XMLDOM_GET_ATTR_LOCAL_CB), xctx, attr);

#define XmlDomGetAttrLocal(xctx, attr) \
    (*XML_DOMCB(xctx)->XMLDOM_GET_ATTR_LOCAL_CB) \
        ((xctx), (attr))

/*---------------------------------------------------------------------------
    NAME
        XmlDomGetAttrLocalLen

    PURPOSE
        Returns an attribute's namespace local name as length-encoded string

    DESCRIPTION
        Returns an attribute's namespace local name (in the data encoding).
        If the attribute's name is not fully qualified (has no prefix), then
        the local name is the same as the name.

        A NULL-terminated version is available as XmlDomGetAttrLocal which
        returns the local name as NULL-terminated string.  If the backing
        store is known to be XMLType, then the attribute's data will be stored
        internally as length-encoded.  Using the length-based Get functions
        will avoid having to copy and NULL-terminate the data.

        If both the input buffer is non-NULL and the input buffer length is
        non-zero, then the value will be stored in the input buffer.  Else,
        the implementation will return its own buffer.

        If the actual length is greater than buflen, then a truncated value
        will be copied into the buffer and len will return the actual length.

    EXAMPLE
        <foo xmlns:pfx="example_namespace" pfx:bar="bar_value"/>
        XmlDomGetAttrLocalLen(attr pfx:bar) --> ("bar", 3)

    RETURNS
        (oratext *) attr's local name [data encoding]

    ARGUMENTS
        xctx   (IN)  XML context
        attr   (IN)  attribute node
        buf    (IN)  input buffer [optional]
        buflen (IN)  input buffer length [optional]
        len    (OUT) length of local name [in characters]

    SEE ALSO
        XmlDomGetAttrLocal, XmlDomGetAttrName, XmlDomGetAttrURI,
        XmlDomGetAttrPrefix
*/
#define XMLDOM_GET_ATTR_LOCAL_LEN_CB getAttrLocalLen_xmldomcb

#define XMLDOM_GET_ATTR_LOCAL_LEN_F(func, xctx, attr, buf, buflen, len) \
    oratext* func(xmlctx *xctx, xmlattrnode *attr, oratext *buf, ub4 buflen, \
                  ub4 *len)

    XMLDOM_GET_ATTR_LOCAL_LEN_F((*XMLDOM_GET_ATTR_LOCAL_LEN_CB),
                                xctx, attr, buf, buflen, len);

#define XmlDomGetAttrLocalLen(xctx, attr, buf, buflen, len) \
    (*XML_DOMCB(xctx)->XMLDOM_GET_ATTR_LOCAL_LEN_CB) \
        ((xctx), (attr), (buf), (buflen), (len))

/*---------------------------------------------------------------------------
    NAME
        XmlDomGetAttrSpecified

    STANDARD
        DOM 2: Attr.specified

    PURPOSE
        Returns a flag saying whether an attribute was explicitly created

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
        (boolean) attribute's "specified" flag

    ARGUMENTS
        xctx (IN) XML context
        attr (IN) attribute node

    SEE ALSO
        XmlDomSetAttrValue
*/
#define XMLDOM_GET_ATTR_SPECIFIED_CB getAttrSpecified_xmldomcb

#define XMLDOM_GET_ATTR_SPECIFIED_F(func, xctx, attr) \
    boolean func(xmlctx *xctx, xmlattrnode *attr)

    XMLDOM_GET_ATTR_SPECIFIED_F((*XMLDOM_GET_ATTR_SPECIFIED_CB), xctx, attr);

#define XmlDomGetAttrSpecified(xctx, attr) \
    (*XML_DOMCB(xctx)->XMLDOM_GET_ATTR_SPECIFIED_CB) \
        ((xctx), (attr))

/*---------------------------------------------------------------------------
    NAME
        XmlDomGetAttrValue

    STANDARD
        DOM 2: Attr.value

    PURPOSE
        Returns an attribute's value as NULL-terminated string

    DESCRIPTION
        Returns the "value" (character data) of an attribute (in the data
        encoding) as NULL-terminated string.  Character and general entities
        will have been replaced.

        A length-encoded version is available as XmlDomGetAttrValueLen which
        returns the attribute value as a pointer and length, for use if the 
        data is known to use XMLType backing store.

    RETURNS
        (oratext *) attribute's value

    ARGUMENTS
        xctx (IN) XML context
        attr (IN) attribute node

    SEE ALSO
        XmlDomGetAttrValueLen, XmlDomSetAttrValue
*/
#define XMLDOM_GET_ATTR_VALUE_CB getAttrValue_xmldomcb

#define XMLDOM_GET_ATTR_VALUE_F(func, xctx, attr) \
    oratext* func(xmlctx *xctx, xmlattrnode *attr)

    XMLDOM_GET_ATTR_VALUE_F((*XMLDOM_GET_ATTR_VALUE_CB), xctx, attr);

#define XmlDomGetAttrValue(xctx, attr) \
    (*XML_DOMCB(xctx)->XMLDOM_GET_ATTR_VALUE_CB) \
        ((xctx), (attr))

/*---------------------------------------------------------------------------
    NAME
        XmlDomGetAttrValueLen

    PURPOSE
        Returns an attribute's value as length-encoded string

    DESCRIPTION
        Returns the "value" (character data) of an attribute (in the data
        encoding) as length-encoded string.  Character and general entities
        will have been replaced.

        A NULL-terminated version is available as XmlDomGetAttrValue which
        returns the attribute value as NULL-terminated string.  If the backing
        store is known to be XMLType, then the attribute's data will be stored
        internally as length-encoded.  Using the length-based Get functions
        will avoid having to copy and NULL-terminate the data.

        If both the input buffer is non-NULL and the input buffer length is
        non-zero, then the value will be stored in the input buffer.  Else,
        the implementation will return its own buffer.

        If the actual length is greater than buflen, then a truncated value
        will be copied into the buffer and len will return the actual length.

    RETURNS
        (oratext *) attribute's value

    ARGUMENTS
        xctx   (IN)  XML context
        attr   (IN)  attribute node
        buf    (IN)  input buffer [optional]
        buflen (IN)  input buffer length [optional]
        len    (OUT) length of attr's value [in characters]

    SEE ALSO
        XmlDomGetAttrValue, XmlDomSetAttrValue
*/
#define XMLDOM_GET_ATTR_VALUE_LEN_CB getAttrValueLen_xmldomcb

#define XMLDOM_GET_ATTR_VALUE_LEN_F(func, xctx, attr, buf, buflen, len) \
    oratext* func(xmlctx *xctx, xmlattrnode *attr, oratext *buf, ub4 buflen, \
                  ub4 *len)

    XMLDOM_GET_ATTR_VALUE_LEN_F((*XMLDOM_GET_ATTR_VALUE_LEN_CB),
                                xctx, attr, buf, buflen, len);

#define XmlDomGetAttrValueLen(xctx, attr, buf, buflen, len) \
    (*XML_DOMCB(xctx)->XMLDOM_GET_ATTR_VALUE_LEN_CB) \
        ((xctx), (attr), (buf), (buflen), (len))

/*---------------------------------------------------------------------------
    NAME
        XmlDomGetAttrValueStreamVA

    PURPOSE
        Get attribute's value via orastream

    STATUS
        Oracle extension [Private]

    DESCRIPTION
        Same as XmlDomGetAttrValueStream() but takes the variable
        arguments as a va_list instead of ...

    RETURNS
        (xmlerr) error code, XMLERR_OK [0] for success

    ARGUMENTS
        xctx (IN) XML context
        attr (IN) XML attriibute node
        va   (IN) variable arguments as va_list (ending in NULL)

    SEE ALSO
        XmlDomGetAttrValue, XmlDomGetAttrValueLen, XmlDomGetAttrValueStream
*/
#define XMLDOM_GET_ATTR_VALUE_STREAM_VA_CB getAttrValueStreamVA_xmldomcb

#define XMLDOM_GET_ATTR_VALUE_STREAM_VA_F(func, xctx, attr, va) \
    xmlerr func(xmlctx *xctx, xmlnode *attr, va_list va)

    XMLDOM_GET_ATTR_VALUE_STREAM_VA_F((*XMLDOM_GET_ATTR_VALUE_STREAM_VA_CB),
                                      xctx, attr, va);

#define XmlDomGetAttrValueStreamVA(xctx, attr, va) \
    (*XML_DOMCB(xctx)->XMLDOM_GET_ATTR_VALUE_STREAM_VA_CB) \
        ((xctx), (attr), (va))

/*---------------------------------------------------------------------------
    NAME
        XmlDomSetAttrValue

    STANDARD
        DOM 2: Attr.value

    PURPOSE
        Sets an attribute's value

    DESCRIPTION
        Sets the given attribute's value to data.  If the node is not an
        attribute, does nothing.  Note that the new value must be in the
        data encoding!  It is not verified, converted, or checked.  If bad
        data is set for the attribute, bad things will happen-- GIGO!  The
        attribute's specified flag will be TRUE after setting a new value.

    RETURNS
        (void)

    ARGUMENTS
        xctx  (IN) XML context
        attr  (IN) attribute node
        value (IN) new value of attribute [data encoding]

    SEE ALSO
        XmlDomGetAttrValue
*/
#define XMLDOM_SET_ATTR_VALUE_CB setAttrValue_xmldomcb

#define XMLDOM_SET_ATTR_VALUE_F(func, xctx, attr, value) \
    void func(xmlctx *xctx, xmlattrnode *attr, oratext *value)

    XMLDOM_SET_ATTR_VALUE_F((*XMLDOM_SET_ATTR_VALUE_CB), xctx, attr, value);

#define XmlDomSetAttrValue(xctx, attr, value) \
    (*XML_DOMCB(xctx)->XMLDOM_SET_ATTR_VALUE_CB) \
        ((xctx), (attr), (value))

/*---------------------------------------------------------------------------
    NAME
        XmlDomSetAttrValueStreamVA

    PURPOSE
        Set attribute's value via orastream

    STATUS
        Oracle extension [Private]

    DESCRIPTION
        Same as XmlDomSetAttrValueStream() but takes the variable
        arguments as a va_list instead of ...

    RETURNS
        (xmlerr) error code, XMLERR_OK [0] for success

    ARGUMENTS
        xctx (IN) XML context
        attr (IN) XML attribute node
        va   (IN) variable arguments as va_list (ending in NULL)

    SEE ALSO
        XmlDomSetAttrValue, XmlDomSetAttrValueStream
*/
#define XMLDOM_SET_ATTR_VALUE_STREAM_VA_CB setAttrValueStreamVA_xmldomcb

#define XMLDOM_SET_ATTR_VALUE_STREAM_VA_F(func, xctx, attr, va) \
    xmlerr func(xmlctx *xctx, xmlnode *attr, va_list va)

    XMLDOM_SET_ATTR_VALUE_STREAM_VA_F((*XMLDOM_SET_ATTR_VALUE_STREAM_VA_CB),
                                      xctx, attr, istream);

#define XmlDomSetAttrValueStreamVA(xctx, attr, va) \
    (*XML_DOMCB(xctx)->XMLDOM_SET_ATTR_VALUE_STREAM_VA_CB) \
        ((xctx), (attr), (va))

/*---------------------------------------------------------------------------
    NAME
        XmlDomGetOwnerElem

    STANDARD
        DOM 2: Attr.ownerElement

    PURPOSE
        Returns an attribute's owning element

    DESCRIPTION
        Returns the Element node associated with an attribute.  Each attr
        either belongs to an element (one and only one), or is detached
        and not yet part of the DOM tree.  In the former case, the element
        node is returned; if the attr is unassigned, NULL is returned.

    RETURNS
        (xmlelemnode *) attribute's element node [or NULL]

    ARGUMENTS
        xctx (IN) XML context
        attr (IN) attribute node

    SEE ALSO
        XmlDomGetOwnerDocument
*/
#define XMLDOM_GET_OWNER_ELEM_CB getOwnerElem_xmldomcb

#define XMLDOM_GET_OWNER_ELEM_F(func, xctx, attr) \
    xmlelemnode* func(xmlctx *xctx, xmlattrnode *attr)

    XMLDOM_GET_OWNER_ELEM_F((*XMLDOM_GET_OWNER_ELEM_CB), xctx, attr);

#define XmlDomGetOwnerElem(xctx, node) \
    (*XML_DOMCB(xctx)->XMLDOM_GET_OWNER_ELEM_CB) \
        ((xctx), (node))

/****************************************************************************
                               Interface Element
 ****************************************************************************/

/*---------------------------------------------------------------------------
    NAME
        XmlDomGetTag

    STANDARD
        DOM 2: Element.tagName

    PURPOSE
        Return an element node's tagname

    DESCRIPTION
        Returns the {tagName} of a node, which is the same as its name.
        DOM 1.0 says "...even though there is a generic {nodeName} attribute
        on the {Node} interface, there is still a {tagName} attribute on the
        {Element} interface; these two attributes must contain the same value,
        but the Working Group considers it worthwhile to support both, given
        the different constituencies the DOM API must satisfy."

    RETURNS
        (oratext *) element's name [data encoding]

    ARGUMENTS
        xctx (IN) XML context
        elem (IN) element node

    SEE ALSO
        XmlDomGetNodeName
*/
#define XMLDOM_GET_TAG_CB getTag_xmldomcb

#define XMLDOM_GET_TAG_F(func, xctx, elem) \
    oratext* func(xmlctx *xctx, xmlelemnode *elem)

    XMLDOM_GET_TAG_F((*XMLDOM_GET_TAG_CB), xctx, elem);

#define XmlDomGetTag(xctx, elem) \
    (*XML_DOMCB(xctx)->XMLDOM_GET_TAG_CB) \
        ((xctx), (elem))

/*---------------------------------------------------------------------------
    NAME
        XmlDomGetAttr

    STANDARD
        DOM 2: Element.getAttribute()

    PURPOSE
        Return attribute's value given its name

    DESCRIPTION
        Returns the value of an element's attribute (specified by name).
        Note that an attribute may have the empty string as its value,
        but cannot be NULL.  If the element does not have an attribute
        with the given name, NULL is returned.

    RETURNS
        (oratext *) named attribute's value [data encoding; may be NULL]

    ARGUMENTS
        xctx (IN) XML context
        elem (IN) element node
        name (IN) attribute's name [data encoding]

    SEE ALSO
        XmlDomGetAttrNS, XmlDomGetAttrs, XmlDomGetAttrNode
*/
#define XMLDOM_GET_ATTR_CB getAttr_xmldomcb

#define XMLDOM_GET_ATTR_F(func, xctx, elem, name) \
    oratext* func(xmlctx *xctx, xmlelemnode *elem, oratext *name)

    XMLDOM_GET_ATTR_F((*XMLDOM_GET_ATTR_CB), xctx, elem, name);

#define XmlDomGetAttr(xctx, elem, name) \
    (*XML_DOMCB(xctx)->XMLDOM_GET_ATTR_CB) \
        ((xctx), (elem), (name))

/*---------------------------------------------------------------------------
    NAME
        XmlDomGetAttrNS

    STANDARD
        DOM 2: Element.getAttributeNS()

    PURPOSE
        Return attribute's value given its URI and localname

    DESCRIPTION
        Returns the value of an element's attribute (specified by URI and
        localname).  Note that an attribute may have the empty string as
        its value, but cannot be NULL.  If the element does not have an
        attribute with the given name, NULL is returned.

    RETURNS
        (oratext *) named attribute's value [data encoding; may be NULL]

    ARGUMENTS
        xctx  (IN) XML context
        elem  (IN) element node
        uri   (IN) attribute's namespace URI [data encoding]
        local (IN) attribute's local name [data encoding]

    SEE ALSO
        XmlDomGetAttr, XmlDomGetAttrs, XmlDomGetAttrNode
*/
#define XMLDOM_GET_ATTR_NS_CB getAttrNS_xmldomcb

#define XMLDOM_GET_ATTR_NS_F(func, xctx, elem, uri, local) \
    oratext* func(xmlctx *xctx, xmlelemnode *elem, \
                  oratext *uri, oratext *local)

    XMLDOM_GET_ATTR_NS_F((*XMLDOM_GET_ATTR_NS_CB), xctx, elem, uri, local);

#define XmlDomGetAttrNS(xctx, elem, uri, local) \
    (*XML_DOMCB(xctx)->XMLDOM_GET_ATTR_NS_CB) \
        ((xctx), (elem), (uri), (local))

/*---------------------------------------------------------------------------
    NAME
        XmlDomSetAttr

    STANDARD
        DOM 2: Element.setAttribute()

    PURPOSE
        Set new attribute for element

    DESCRIPTION
        Creates a new attribute for an element with the given name and value
        (which should be in the data encoding).  If the named attribute already
        exists, its value is simply replaced.  The name and value are not
        verified, converted, or checked.  If bad data is set, bad things may
        happen-- GIGO!  The value is not parsed, so entity references will not
        be expanded.  The attribute's Specified flag will be set.

    RETURNS
        (void)

    ARGUMENTS
        xctx  (IN) XML context
        elem  (IN) element node
        name  (IN) attribute's name [data encoding]
        value (IN) attribute's value [data encoding]

    SEE ALSO
        XmlDomSetAttrNS, XmlDomCreateAttr, XmlDomSetAttrValue,
        XmlDomRemoveAttr
*/
#define XMLDOM_SET_ATTR_CB setAttr_xmldomcb

#define XMLDOM_SET_ATTR_F(func, xctx, elem, name, value) \
    void func(xmlctx *xctx, xmlelemnode *elem, oratext *name, oratext *value)

    XMLDOM_SET_ATTR_F((*XMLDOM_SET_ATTR_CB), xctx, elem, name, value);

#define XmlDomSetAttr(xctx, elem, name, value) \
    (*XML_DOMCB(xctx)->XMLDOM_SET_ATTR_CB) \
        ((xctx), (elem), (name), (value))

/*---------------------------------------------------------------------------
    NAME
        XmlDomSetAttrNS

    STANDARD
        DOM 2: Element.setAttributeNS()

    PURPOSE
        Set new attribute for element (namespace aware version)

    DESCRIPTION
        Creates a new attribute for an element with the given URI, localname
        and value (which should be in the data encoding).  If the named
        attribute already exists, its value is simply replaced.  The name
        and value are not verified, converted, or checked.  If bad data is
        set, bad things will happen-- GIGO!

        The value is not parsed, so entity references will not be expanded.

        The attribute's {specified} flag will be set.

    RETURNS
        (void)

    ARGUMENTS
        xctx  (IN) XML context
        elem  (IN) element node
        uri   (IN) attribute's namespace URI [data encoding]
        qname (IN) attribute's qualified name [data encoding]
        value (IN) attribute's value [data encoding]

    SEE ALSO
        XmlDomSetAttr, XmlDomCreateAttr, XmlDomSetAttrValue,
        XmlDomRemoveAttr
*/
#define XMLDOM_SET_ATTR_NS_CB setAttrNS_xmldomcb

#define XMLDOM_SET_ATTR_NS_F(func, xctx, elem, uri, qname, value) \
    void func(xmlctx *xctx, xmlelemnode *elem, oratext *uri, \
              oratext *qname, oratext *value)

    XMLDOM_SET_ATTR_NS_F((*XMLDOM_SET_ATTR_NS_CB),
                         xctx, elem, uri, qname, value);

#define XmlDomSetAttrNS(xctx, elem, uri, qname, value) \
    (*XML_DOMCB(xctx)->XMLDOM_SET_ATTR_NS_CB) \
        ((xctx), (elem), (uri), (qname), (value))

/*---------------------------------------------------------------------------
    NAME
        XmlDomRemoveAttr

    STANDARD
        DOM 2: Element.removeAttribute()

    PURPOSE
        Remove attribute with specified name

    DESCRIPTION
        Removes an attribute (specified by name).  If the removed attribute
        has a default value it is immediately re-created with that default.
        Note that the attribute is removed from the element's list of
        attributes, but the attribute node itself is not destroyed.

    RETURNS
        (void)

    ARGUMENTS
        xctx (IN) XML context
        elem (IN) element node
        name (IN) attribute's name [data encoding]

    SEE ALSO
        XmlDomRemoveAttrNS, XmlDomRemoveAttrNode
*/
#define XMLDOM_REMOVE_ATTR_CB removeAttr_xmldomcb

#define XMLDOM_REMOVE_ATTR_F(func, xctx, elem, name) \
    void func(xmlctx *xctx, xmlelemnode *elem, oratext *name)

    XMLDOM_REMOVE_ATTR_F((*XMLDOM_REMOVE_ATTR_CB), xctx, elem, name);

#define XmlDomRemoveAttr(xctx, elem, name) \
    (*XML_DOMCB(xctx)->XMLDOM_REMOVE_ATTR_CB) \
        ((xctx), (elem), (name))

/*---------------------------------------------------------------------------
    NAME
        XmlDomRemoveAttrNS

    PURPOSE
        Remove attribute with specified URI and local name

    STANDARD
        DOM 2: Element.removeAttributeNS()

    DESCRIPTION
        Removes an attribute (specified by URI and localname).  If the
        removed attribute has a default value it is immediately re-created
        with that default.  Note that the attribute is removed from the
        element's list of attributes, but the attribute node itself is not
        destroyed.

    RETURNS
        (void)

    ARGUMENTS
        xctx  (IN) XML context
        elem  (IN) element node
        uri   (IN) attribute's namespace URI
        local (IN) attribute's local name

    SEE ALSO
        XmlDomRemoveAttr, XmlDomRemoveAttrNode
*/
#define XMLDOM_REMOVE_ATTR_NS_CB removeAttrNS_xmldomcb

#define XMLDOM_REMOVE_ATTR_NS_F(func, xctx, elem, uri, local) \
    void func(xmlctx *xctx, xmlelemnode *elem, oratext *uri, oratext *local)

    XMLDOM_REMOVE_ATTR_NS_F((*XMLDOM_REMOVE_ATTR_NS_CB),
                            xctx, elem, uri, local);

#define XmlDomRemoveAttrNS(xctx, elem, uri, local) \
    (*XML_DOMCB(xctx)->XMLDOM_REMOVE_ATTR_NS_CB) \
        ((xctx),(elem),(uri),(local))

/*---------------------------------------------------------------------------
    NAME
        XmlDomGetAttrNode

    STANDARD
        DOM 2: Element.getAttributeNode()

    PURPOSE
        Get attribute by name

    DESCRIPTION
        Returns an element's attribute specified by name.  If the node is
        not an element or the named attribute does not exist, returns NULL.

    RETURNS
        (xmlattrnode *) attribute with the specified name [or NULL]

    ARGUMENTS
        xctx (IN) XML context
        elem (IN) element node
        name (IN) attribute's name [data encoding]

    SEE ALSO
        XmlDomGetAttrNodeNS, XmlDomGetAttr
*/
#define XMLDOM_GET_ATTR_NODE_CB getAttrNode_xmldomcb

#define XMLDOM_GET_ATTR_NODE_F(func, xctx, elem, name) \
    xmlattrnode* func(xmlctx *xctx, xmlelemnode *elem, oratext *name)

    XMLDOM_GET_ATTR_NODE_F((*XMLDOM_GET_ATTR_NODE_CB), xctx, elem, name);

#define XmlDomGetAttrNode(xctx, elem, name) \
    (*XML_DOMCB(xctx)->XMLDOM_GET_ATTR_NODE_CB) \
        ((xctx), (elem), (name))

/*---------------------------------------------------------------------------
    NAME
        XmlDomGetAttrNodeNS

    STANDARD
        DOM 2: Element.getAttributeNodeNS()

    PURPOSE
        Get attribute by name (namespace aware version)

    DESCRIPTION
        Returns an element's attribute specified by URI and localname.  If
        the node is not an element or the named attribute does not exist,
        returns NULL.

    RETURNS
        (xmlattrnode *) attribute node with the given URI/local name [or NULL]

    ARGUMENTS
        xctx  (IN) XML context
        elem  (IN) element node
        uri   (IN) attribute's namespace URI [data encoding]
        local (IN) attribute's local name [data encoding]

    SEE ALSO
        XmlDomGetAttrNode, XmlDomGetAttr
*/
#define XMLDOM_GET_ATTR_NODE_NS_CB getAttrNodeNS_xmldomcb

#define XMLDOM_GET_ATTR_NODE_NS_F(func, xctx, elem, uri, local) \
    xmlattrnode* func(xmlctx *xctx, xmlelemnode *elem, \
                      oratext *uri, oratext *local)

    XMLDOM_GET_ATTR_NODE_NS_F((*XMLDOM_GET_ATTR_NODE_NS_CB),
                              xctx, elem, uri, local);

#define XmlDomGetAttrNodeNS(xctx, elem, uri, local) \
    (*XML_DOMCB(xctx)->XMLDOM_GET_ATTR_NODE_NS_CB) \
        ((xctx), (elem), (uri), (local))

/*---------------------------------------------------------------------------
    NAME
        XmlDomSetAttrNode

    STANDARD
        DOM 2: Element.setAttributeNode()

    PURPOSE
        Set attribute node

    DESCRIPTION
        Adds a new attribute to an element.  If an attribute with the given
        name already exists, it is replaced and the old attribute returned
        through {oldNode}.  If the attribute is new, it is added to the
        element's list and {oldNode} set to NULL.

    RETURNS
        (xmlattrnode *) replaced attribute node (or NULL)

    ARGUMENTS
        xctx    (IN) XML context
        elem    (IN) element node
        newAttr (IN) attribute node to add

    SEE ALSO
        XmlDomSetAttrNodeNS, XmlDomCreateAttr, XmlDomSetAttrValue
*/
#define XMLDOM_SET_ATTR_NODE_CB setAttrNode_xmldomcb

#define XMLDOM_SET_ATTR_NODE_F(func, xctx, elem, newAttr) \
    xmlattrnode* func(xmlctx *xctx, xmlelemnode *elem, xmlattrnode *newAttr)

    XMLDOM_SET_ATTR_NODE_F((*XMLDOM_SET_ATTR_NODE_CB), xctx, elem, newAttr);

#define XmlDomSetAttrNode(xctx, elem, attr) \
    (*XML_DOMCB(xctx)->XMLDOM_SET_ATTR_NODE_CB) \
        ((xctx), (elem), (attr))

/*---------------------------------------------------------------------------
    NAME
        XmlDomSetAttrNodeNS

    STANDARD
        DOM 2: Element.setAttributeNodeNS()

    PURPOSE
        Set attribute node (namespace aware version)

    DESCRIPTION
        Adds a new attribute to an element.  If an attribute with {newNode}'s
        URI and localname already exists, it is replaced and the old attribute
        returned through {oldNode}.  If the attribute is new, it is added to
        the element's list and {oldNode} set to NULL.

    RETURNS
        (xmlattrnode *) replaced attribute node [or NULL]

    ARGUMENTS
        xctx    (IN) XML context
        elem    (IN) element node
        newAttr (IN) attribute node to add

    SEE ALSO
        XmlDomSetAttrNode, XmlDomCreateAttr, XmlDomSetAttrValue
*/
#define XMLDOM_SET_ATTR_NODE_NS_CB setAttrNodeNS_xmldomcb

#define XMLDOM_SET_ATTR_NODE_NS_F(func, xctx, elem, newAttr) \
    xmlattrnode* func(xmlctx *xctx, xmlelemnode *elem, xmlattrnode *newAttr)

    XMLDOM_SET_ATTR_NODE_NS_F((*XMLDOM_SET_ATTR_NODE_NS_CB),
                              xctx, elem, newAttr);

#define XmlDomSetAttrNodeNS(xctx, elem, attr) \
    (*XML_DOMCB(xctx)->XMLDOM_SET_ATTR_NODE_NS_CB) \
        ((xctx), (elem), (attr))

/*---------------------------------------------------------------------------
    NAME
        XmlDomRemoveAttrNode

    PURPOSE
        Remove attribute node

    STANDARD
        DOM 2: Element.removeAttributeNode()

    DESCRIPTION
        Removes an attribute from an element.  If the attribute has a
        default value, it is immediately re-created with that value
        (Specified set to FALSE).  Returns the removed attribute on
        success, else NULL.

    RETURNS
        (xmlattrnode *) replaced attribute node [or NULL]

    ARGUMENTS
        xctx    (IN) XML context
        elem    (IN) element node
        oldAttr (IN) attribute node to remove

    SEE ALSO
        XmlDomRemoveAttr
*/
#define XMLDOM_REMOVE_ATTR_NODE_CB removeAttrNode_xmldomcb

#define XMLDOM_REMOVE_ATTR_NODE_F(func, xctx, elem, oldAttr) \
    xmlattrnode* func(xmlctx *xctx, xmlelemnode *elem, xmlattrnode *oldAttr)

    XMLDOM_REMOVE_ATTR_NODE_F((*XMLDOM_REMOVE_ATTR_NODE_CB),
                              xctx, elem, oldAttr);

#define XmlDomRemoveAttrNode(xctx, elem, old) \
    (*XML_DOMCB(xctx)->XMLDOM_REMOVE_ATTR_NODE_CB) \
        ((xctx), (elem), (old))

/*---------------------------------------------------------------------------
    NAME
        XmlDomHasAttr

    PURPOSE
        Does named attribute exist?

    STANDARD
        DOM 2: Element.hasAttribute()

    DESCRIPTION
        Determines if an element has a attribute with the given name.
        Returns TRUE if so, FALSE if not.

    RETURNS
        (boolean) TRUE if element has attribute with given name

    ARGUMENTS
        xctx (IN) XML context
        elem (IN) element node
        name (IN) attribute's name [data encoding]

    SEE ALSO
        XmlDomHasAttrNS
*/
#define XMLDOM_HAS_ATTR_CB hasAttr_xmldomcb

#define XMLDOM_HAS_ATTR_F(func, xctx, elem, name) \
    boolean func(xmlctx *xctx, xmlelemnode *elem, oratext *name)

    XMLDOM_HAS_ATTR_F((*XMLDOM_HAS_ATTR_CB), xctx, elem, name);

#define XmlDomHasAttr(xctx, elem, name) \
    (*XML_DOMCB(xctx)->XMLDOM_HAS_ATTR_CB) \
        ((xctx), (elem), (name))

/*---------------------------------------------------------------------------
    NAME
        XmlDomHasAttrNS

    STANDARD
        DOM 2 Element.hasAttributeNS()

    PURPOSE
        Does named attribute exist?  (namespace aware version)

    DESCRIPTION
        Determines if an element has an attribute with the given URI and
        localname.  Returns TRUE if so, FALSE if not.

    RETURNS
        (boolean) TRUE if element has attribute with given URI/localname

    ARGUMENTS
        xctx  (IN) XML context
        elem  (IN) element node
        uri   (IN) attribute's namespace URI [data encoding]
        local (IN) attribute's local name [data encoding]

    SEE ALSO
        XmlDomHasAttr
*/
#define XMLDOM_HAS_ATTR_NS_CB hasAttrNS_xmldomcb

#define XMLDOM_HAS_ATTR_NS_F(func, xctx, elem, uri, local) \
    boolean func(xmlctx *xctx, xmlelemnode *elem, oratext *uri, oratext *local)

    XMLDOM_HAS_ATTR_NS_F((*XMLDOM_HAS_ATTR_NS_CB), xctx, elem, uri, local);

#define XmlDomHasAttrNS(xctx, elem, uri, local) \
    (*XML_DOMCB(xctx)->XMLDOM_HAS_ATTR_NS_CB) \
        ((xctx), (elem), (uri), (local))

/*---------------------------------------------------------------------------
    NAME
        XmlDomGetElemsByTag

    STANDARD
        DOM 2: Element.getElementsByTagName()

    PURPOSE
        Get elements with given tag name [non-namespace-aware version]

    DESCRIPTION
        Returns a list of all elements (in the document tree rooted at the
        root node) with a given tag name, in the order in which they would
        be encountered in a preorder traversal of the tree.  If root is NULL,
        the entire document is searched.  The tag name should be in the data
        encoding.  The special name {"*"} matches all tag names; a NULL name
        matches nothing.  Note that tag names are CASE SENSITIVE.  This
        function is not namespace aware; the full tag names are compared.
        If two prefixes which map to the same URI are compared, the
        comparison will fail.  See XmlDomGetElemsByTagNS for the
        namespace-aware version.  The returned list can be freed with
        XmlDomFreeNodeList.

    RETURNS
        (xmlnodelist *) node list of matching elements

    ARGUMENTS
        xctx (IN) XML context
        elem (IN) element node
        name (IN) tag name to match [data encoding; {"*"} for any tag]

    SEE ALSO
        XmlDomGetElemsByTagNS, XmlDomFreeNodeList
*/
#define XMLDOM_GET_ELEMS_BY_TAG_CB getElemsByTag_xmldomcb

#define XMLDOM_GET_ELEMS_BY_TAG_F(func, xctx, elem, name) \
    xmlnodelist* func(xmlctx *xctx, xmlelemnode *elem, oratext *name)

    XMLDOM_GET_ELEMS_BY_TAG_F((*XMLDOM_GET_ELEMS_BY_TAG_CB), xctx, elem, name);

#define XmlDomGetElemsByTag(xctx, elem, name) \
    (*XML_DOMCB(xctx)->XMLDOM_GET_ELEMS_BY_TAG_CB) \
        ((xctx), (elem), (name))

/*---------------------------------------------------------------------------
    NAME
        XmlDomGetElemsByTagNS

    STANDARD
        DOM 2: Element.getElementsByTagNameNS()

    PURPOSE
        Get elements with given URI and localname [namespace aware version]

    DESCRIPTION
        Returns a list of all elements (in the document tree rooted at the
        root node) with a given URI and localname, in the order in which they
        would be encountered in a preorder traversal of the tree.  If root is
        NULL, the entire document is searched.  The tag name should be in the
        data encoding.  The special name {"*"} matches all tag names; a NULL
        name matches nothing.  Note that tag names are CASE SENSITIVE.  This
        function is not namespace aware; the full tag names are compared.
        If two prefixes which map to the same URI are compared, the
        comparison will fail.  The returned list can be freed with
        XmlDomFreeNodeList.

    RETURNS
        (xmlnodelist *) node list of matching elements

    ARGUMENTS
        xctx  (IN) XML context
        elem  (IN) element node
        uri   (IN) namespace URI to match [data encoding; {"*"} matches any]
        local (IN) local name to match [data encoding; {"*"} matches any]

    SEE ALSO
        XmlDomGetDocElemsByTag, XmlDomFreeNodeList
*/
#define XMLDOM_GET_ELEMS_BY_TAG_NS_CB getElemsByTagNS_xmldomcb

#define XMLDOM_GET_ELEMS_BY_TAG_NS_F(func, xctx, elem, uri, local) \
    xmlnodelist* func(xmlctx *xctx, xmlelemnode *elem, \
                      oratext *uri, oratext *local)

    XMLDOM_GET_ELEMS_BY_TAG_NS_F((*XMLDOM_GET_ELEMS_BY_TAG_NS_CB),
                                 xctx, elem, uri, local);

#define XmlDomGetElemsByTagNS(xctx, elem, uri, local) \
    (*XML_DOMCB(xctx)->XMLDOM_GET_ELEMS_BY_TAG_NS_CB) \
        ((xctx), (elem), (uri), (local))

/*---------------------------------------------------------------------------
    NAME
        XmlDomGetChildrenByTag

    PURPOSE
        Get children of element with given tag name [non-namespace-aware]

    DESCRIPTION
        Returns a list of children of an element with the given tag name,
        in the order in which they would be encountered in a preorder
        traversal of the tree. The tag name should be in the data encoding.
        The special name {"*"} matches all tag names; a NULL name matches
        nothing.  Note that tag names are CASE SENSITIVE.  This function
        is not namespace aware; the full tag names are compared.  If two
        prefixes which map to the same URI are compared, the comparison
        will fail.  See XmlDomGetChildrenByTagNS for the namespace-aware
        version.  The returned list can be freed with XmlDomFreeNodeList.

    RETURNS
        (xmlnodelist *) node list of matching children

    ARGUMENTS
        xctx (IN) XML context
        elem (IN) element node
        name (IN) tag name to match [data encoding; {"*"} for any]

    SEE ALSO
        XmlDomGetChildrenByTagNS, XmlDomFreeNodeList
*/
#define XMLDOM_GET_CHILDREN_BY_TAG_CB getChildrenByTag_xmldomcb

#define XMLDOM_GET_CHILDREN_BY_TAG_F(func, xctx, elem, name) \
    xmlnodelist* func(xmlctx *xctx, xmlelemnode *elem, oratext *name)

    XMLDOM_GET_CHILDREN_BY_TAG_F((*XMLDOM_GET_CHILDREN_BY_TAG_CB),
                                 xctx, elem, name);

#define XmlDomGetChildrenByTag(xctx, elem, name) \
    (*XML_DOMCB(xctx)->XMLDOM_GET_CHILDREN_BY_TAG_CB) \
        ((xctx),(elem),(name))

/*---------------------------------------------------------------------------
    NAME
        XmlDomGetChildrenByTagNS

    PURPOSE
        Get children of element with tag name [namespace-aware version]

    DESCRIPTION
        Returns a list of children of an element with the given URI and
        local name, in the order in which they would be encountered in a
        preorder traversal of the tree.  The URI and local name should be
        in the data encoding.  The special name {"*"} matches all URIs or
        tag names; a NULL name matches nothing.  Note that names are
        CASE SENSITIVE.  See XmlDomGetChildrenByTag for the non-namespace
        version.  The returned list can be freed with XmlDomFreeNodeList.

    RETURNS
        (xmlnodelist *) node list of matching children

    ARGUMENTS
        xctx  (IN) XML context
        elem  (IN) element node
        uri   (IN) namespace URI to match [data encoding; {"*"} for]
        local (IN) local name to match [data encoding; {"*"} for any]

    SEE ALSO
        XmlDomGetChildrenByTag, XmlDomFreeNodeList
*/
#define XMLDOM_GET_CHILDREN_BY_TAG_NS_CB getChildrenByTagNS_xmldomcb

#define XMLDOM_GET_CHILDREN_BY_TAG_NS_F(func, xctx, elem, uri, local) \
    xmlnodelist* func(xmlctx *xctx, xmlelemnode *elem, \
                      oratext *uri, oratext *local)

    XMLDOM_GET_CHILDREN_BY_TAG_NS_F((*XMLDOM_GET_CHILDREN_BY_TAG_NS_CB),
                                    xctx, elem, uri, local);

#define XmlDomGetChildrenByTagNS(xctx, elem, uri, local) \
    (*XML_DOMCB(xctx)->XMLDOM_GET_CHILDREN_BY_TAG_NS_CB) \
        ((xctx), (elem), (uri), (local))

/****************************************************************************
                               Interface Text
 ****************************************************************************/

/*---------------------------------------------------------------------------
    NAME
        XmlDomSplitText

    STANDARD
        DOM 2: Text.splitText()

    PURPOSE
        Split text node into two

    DESCRIPTION
        Splits a single text node into two text nodes; the original data
        is split between them.  If the given node is not type text, or the
        offset is outside of the original data, does nothing and returns
        NULL.  The offset is zero-based, and is in characters, not bytes.
        The original node is retained, its data is just truncated.  A new
        text node is created which contains the remainder of the original
        data, and is inserted as the next sibling of the original.  The
        new text node is returned.

    EXAMPLE
        (Given a Text node with data) "abcdefgh",
        XmlDomSplitText(1) --> Text("a"), Text("bcdefgh")
        XmlDomSplitText(5) --> Text("abcde"), Text("fgh")

    RETURNS
        (xmltextnode *) new text node

    ARGUMENTS
        xctx     (IN) XML context
        textnode (IN) text node
        offset   (IN) character offset at which to split, starting from 0.

    SEE ALSO
        XmlDomGetCharData, XmlDomAppendData, XmlDomInsertData,
        XmlDomDeleteData, XmlDomReplaceData
*/
#define XMLDOM_SPLIT_TEXT_CB splitText_xmldomcb

#define XMLDOM_SPLIT_TEXT_F(func, xctx, textnode, offset) \
    xmltextnode* func(xmlctx *xctx, xmltextnode *textnode, ub4 offset)

    XMLDOM_SPLIT_TEXT_F((*XMLDOM_SPLIT_TEXT_CB), xctx, textnode, offset);

#define XmlDomSplitText(xctx, textnode, offset) \
    (*XML_DOMCB(xctx)->XMLDOM_SPLIT_TEXT_CB) \
        ((xctx), (textnode), (offset))

/****************************************************************************
                               Interface DocumentType
 ****************************************************************************/

/*---------------------------------------------------------------------------
    NAME
        XmlDomGetDTDName

    STANDARD
        DOM 2: DocumentType.name

    PURPOSE
        Get name of DTD

    DESCRIPTION
        Returns a DTD's name (specified immediately after the {DOCTYPE}
        keyword), or NULL if the node is not type DTD.

    RETURNS
        (oratext *) name of DTD

    ARGUMENTS
        xctx (IN) XML context
        dtd  (IN) DTD node

    SEE ALSO
        XmlDomGetDTD, XmlDomGetDTDEntities, XmlDomGetDTDNotations,
        XmlDomGetDTDSysID, XmlDomGetDTDInternalSubset
*/
#define XMLDOM_GET_DTD_NAME_CB getDTDName_xmldomcb

#define XMLDOM_GET_DTD_NAME_F(func, xctx, dtd) \
    oratext* func(xmlctx *xctx, xmldtdnode *dtd)

    XMLDOM_GET_DTD_NAME_F((*XMLDOM_GET_DTD_NAME_CB), xctx, dtd);

#define XmlDomGetDTDName(xctx, dtd) \
    (*XML_DOMCB(xctx)->XMLDOM_GET_DTD_NAME_CB) \
        ((xctx), (dtd))

/*---------------------------------------------------------------------------
    NAME
        XmlDomGetDTDEntities

    STANDARD
        DOM 2: DocumentType.entities

    PURPOSE
        Get entities of DTD

    DESCRIPTION
        Returns a named node map of general entities defined by the DTD.
        If the node is not a DTD, or has no general entities, returns NULL.

    RETURNS
        (xmlnamedmap *) named node map containing entities declared in DTD

    ARGUMENTS
        xctx (IN) XML context
        dtd  (IN) DTD node

    SEE ALSO
        XmlDomGetDTD, XmlDomGetDTDName, XmlDomGetDTDNotations,
        XmlDomGetDTDSysID, XmlDomGetDTDInternalSubset
*/
#define XMLDOM_GET_DTD_ENTITIES_CB getDTDEntities_xmldomcb

#define XMLDOM_GET_DTD_ENTITIES_F(func, xctx, dtd) \
    xmlnamedmap* func(xmlctx *xctx, xmldtdnode *dtd)

    XMLDOM_GET_DTD_ENTITIES_F((*XMLDOM_GET_DTD_ENTITIES_CB), xctx, dtd);

#define XmlDomGetDTDEntities(xctx, dtd) \
    (*XML_DOMCB(xctx)->XMLDOM_GET_DTD_ENTITIES_CB) \
        ((xctx), (dtd))

/*---------------------------------------------------------------------------
    NAME
        XmlDomGetDTDNotations

    STANDARD
        DOM 2: DocumentType.notations

    PURPOSE
        Get notations of DTD

    DESCRIPTION
        Returns named node map of notations declared by the DTD.  If the
        node is not a DTD or has no notations, returns NULL.

    RETURNS
        (xmlnamedmap *) named node map containing notations declared in DTD

    ARGUMENTS
        xctx (IN) XML context
        dtd  (IN) DTD node

    SEE ALSO
        XmlDomGetDTD, XmlDomGetDTDName, XmlDomGetDTDEntities,
        XmlDomGetDTDSysID, XmlDomGetDTDInternalSubset
*/
#define XMLDOM_GET_DTD_NOTATIONS_CB getDTDNotations_xmldomcb

#define XMLDOM_GET_DTD_NOTATIONS_F(func, xctx, dtd) \
    xmlnamedmap* func(xmlctx *xctx, xmldtdnode *dtd)

    XMLDOM_GET_DTD_NOTATIONS_F((*XMLDOM_GET_DTD_NOTATIONS_CB), xctx, dtd);

#define XmlDomGetDTDNotations(xctx, dtd) \
    (*XML_DOMCB(xctx)->XMLDOM_GET_DTD_NOTATIONS_CB) \
        ((xctx), (dtd))

/*---------------------------------------------------------------------------
    NAME
        XmlDomGetDTDPubID

    STANDARD
        DOM 2: DocumentType.publicID

    PURPOSE
        Get DTD's public ID

    DESCRIPTION
        Returns a DTD's public identifier.

    RETURNS
        (oratext *) DTD's public identifier [data encoding]

    ARGUMENTS
        xctx (IN) XML context
        dtd  (IN) DTD node

    SEE ALSO
        XmlDomGetDTD, XmlDomGetDTDName, XmlDomGetDTDEntities,
        XmlDomGetDTDSysID, XmlDomGetDTDInternalSubset
*/
#define XMLDOM_GET_DTD_PUBID_CB getDTDPubID_xmldomcb

#define XMLDOM_GET_DTD_PUBID_F(func, xctx, dtd) \
    oratext* func(xmlctx *xctx, xmldtdnode *dtd)

    XMLDOM_GET_DTD_PUBID_F((*XMLDOM_GET_DTD_PUBID_CB), xctx, dtd);

#define XmlDomGetDTDPubID(xctx, dtd) \
    (*XML_DOMCB(xctx)->XMLDOM_GET_DTD_PUBID_CB) \
        ((xctx), (dtd))

/*---------------------------------------------------------------------------
    NAME
        XmlDomGetDTDSysID

    STANDARD
        DOM 2: DocumentType.systemID

    PURPOSE
        Get DTD's system ID

    DESCRIPTION
        Returns a DTD's system identifier.

    RETURNS
        (oratext *) DTD's system identifier [data encoding]

    ARGUMENTS
        xctx (IN) XML context
        dtd  (IN) DTD node

    SEE ALSO
        XmlDomGetDTD, XmlDomGetDTDName, XmlDomGetDTDEntities,
        XmlDomGetDTDPubID, XmlDomGetDTDInternalSubset
*/
#define XMLDOM_GET_DTD_SYSID_CB getDTDSysID_xmldomcb

#define XMLDOM_GET_DTD_SYSID_F(func, xctx, dtd) \
    oratext* func(xmlctx *xctx, xmldtdnode *dtd)

    XMLDOM_GET_DTD_SYSID_F((*XMLDOM_GET_DTD_SYSID_CB), xctx, dtd);

#define XmlDomGetDTDSysID(xctx, dtd) \
    (*XML_DOMCB(xctx)->XMLDOM_GET_DTD_SYSID_CB) \
        ((xctx), (dtd))

/*---------------------------------------------------------------------------
    NAME
        XmlDomGetDTDInternalSubset

    STANDARD
        DOM 2: DocumentType.internalSubset

    PURPOSE
        Get DTD's internal subset

    DESCRIPTION
        Returns the content model for an element.  If there is no DTD,
        returns NULL.

    RETURNS
        (xmlnode *) content model subtree

    ARGUMENTS
        xctx (IN) XML context
        dtd  (IN) DTD node
        name (IN) name of Element [data encoding]

    SEE ALSO
        XmlDomGetDTD, XmlDomGetDTDName, XmlDomGetDTDEntities,
        XmlDomGetDTDNotations, XmlDomGetDTDPubID
*/
#define XMLDOM_GET_DTD_INTERNAL_SUBSET_CB getDTDInternalSubset_xmldomcb

#define XMLDOM_GET_DTD_INTERNAL_SUBSET_F(func, xctx, dtd, name) \
    xmlnode* func(xmlctx *xctx, xmldtdnode *dtd, oratext *name)

    XMLDOM_GET_DTD_INTERNAL_SUBSET_F((*XMLDOM_GET_DTD_INTERNAL_SUBSET_CB),
                                     xctx, dtd, name);

#define XmlDomGetDTDInternalSubset(xctx, dtd, name) \
    (*XML_DOMCB(xctx)->XMLDOM_GET_DTD_INTERNAL_SUBSET_CB) \
        ((xctx), (dtd), (name))

/****************************************************************************
                               Interface Notation
 ****************************************************************************/

/*---------------------------------------------------------------------------
    NAME
        XmlDomGetNotationPubID

    STANDARD
        DOM 2: Notation.publicId

    PURPOSE
        Get notation's public ID

    DESCRIPTION
        Return a notation's public identifier (in the data encoding).  If
        the node is not a notation, or has no defined public ID, returns
        NULL.

    RETURNS
        (oratext *) notation's public identifier [data encoding; may be NULL]

    ARGUMENTS
        xctx (IN) XML context
        note (IN) notation node

    SEE ALSO
        XmlDomGetNotationSysID
*/
#define XMLDOM_GET_NOTATION_PUBID_CB getNotationPubID_xmldomcb

#define XMLDOM_GET_NOTATION_PUBID_F(func, xctx, note) \
    oratext* func(xmlctx *xctx, xmlnotenode *note)

    XMLDOM_GET_NOTATION_PUBID_F((*XMLDOM_GET_NOTATION_PUBID_CB), xctx, note);

#define XmlDomGetNotationPubID(xctx, note) \
    (*XML_DOMCB(xctx)->XMLDOM_GET_NOTATION_PUBID_CB) \
        ((xctx), (note))

/*---------------------------------------------------------------------------
    NAME
        XmlDomGetNotationSysID

    STANDARD
        DOM 2: Notation.systemId

    PURPOSE
        Get notation's system ID

    DESCRIPTION
        Return a notation's system identifier (in the data encoding).  If
        the node is not a notation, or has no defined system ID, returns
        NULL.

    RETURNS
        (oratext *) notation's system identifier [data encoding; may be NULL]

    ARGUMENTS
        xctx (IN) XML context
        note (IN) notation node

    SEE ALSO
        XmlDomGetNotationPubID
*/
#define XMLDOM_GET_NOTATION_SYSID_CB getNotationSysID_xmldomcb

#define XMLDOM_GET_NOTATION_SYSID_F(func, xctx, note) \
    oratext* func(xmlctx *xctx, xmlnotenode *note)

    XMLDOM_GET_NOTATION_SYSID_F((*XMLDOM_GET_NOTATION_SYSID_CB), xctx, note);

#define XmlDomGetNotationSysID(xctx, note) \
    (*XML_DOMCB(xctx)->XMLDOM_GET_NOTATION_SYSID_CB) \
        ((xctx), (note))

/****************************************************************************
                               Interface Entity
 ****************************************************************************/

/*---------------------------------------------------------------------------
    NAME
        XmlDomGetEntityPubID

    STANDARD
        DOM 2: Entity.publicId

    PURPOSE
        Get entity's public ID

    DESCRIPTION
        Returns an entity's public identifier (in the data encoding).  If
        the node is not an entity, or has no defined public ID, returns NULL.

    RETURNS
        (oratext *) entity's public identifier [data encoding; may be NULL]

    ARGUMENTS
        xctx (IN) XML context
        ent  (IN) entity node

    SEE ALSO
        XmlDomGetEntitySysID, XmlDomGetEntityNotation
*/
#define XMLDOM_GET_ENTITY_PUBID_CB getEntityPubID_xmldomcb

#define XMLDOM_GET_ENTITY_PUBID_F(func, xctx, ent) \
    oratext* func(xmlctx *xctx, xmlentnode *ent)

    XMLDOM_GET_ENTITY_PUBID_F((*XMLDOM_GET_ENTITY_PUBID_CB), xctx, ent);

#define XmlDomGetEntityPubID(xctx, ent) \
    (*XML_DOMCB(xctx)->XMLDOM_GET_ENTITY_PUBID_CB) \
        ((xctx), (ent))

/*---------------------------------------------------------------------------
    NAME
        XmlDomGetEntitySysID

    STANDARD
        DOM 2: Entity.systemId

    PURPOSE
        Get entity's system ID

    DESCRIPTION
        Returns an entity's system identifier (in the data encoding).  If
        the node is not an entity, or has no defined system ID, returns
        NULL.

    RETURNS
        (oratext *) entity's system identifier [data encoding; may be NULL]

    ARGUMENTS
        xctx (IN) XML context
        ent  (IN) entity node

    SEE ALSO
        XmlDomGetEntityPubID, XmlDomGetEntityNotation
*/
#define XMLDOM_GET_ENTITY_SYSID_CB getEntitySysID_xmldomcb

#define XMLDOM_GET_ENTITY_SYSID_F(func, xctx, ent) \
    oratext* func(xmlctx *xctx, xmlentnode *ent)

    XMLDOM_GET_ENTITY_SYSID_F((*XMLDOM_GET_ENTITY_SYSID_CB), xctx, ent);

#define XmlDomGetEntitySysID(xctx, ent) \
    (*XML_DOMCB(xctx)->XMLDOM_GET_ENTITY_SYSID_CB) \
        ((xctx), (ent))

/*---------------------------------------------------------------------------
    NAME
        XmlDomGetEntityNotation

    STANDARD
        DOM 2: Entity.notationName

    PURPOSE
        Get entity's notation

    DESCRIPTION
        For unparsed entities, returns the name of its notation (in the data
        encoding).  For parsed entities and other node types, returns NULL.

    RETURNS
        (oratext *) entity's notation [data encoding; may be NULL]

    ARGUMENTS
        xctx (IN) XML context
        ent  (IN) entity node

    SEE ALSO
        XmlDomGetEntityPubID, XmlDomGetEntitySysID
*/
#define XMLDOM_GET_ENTITY_NOTATION_CB getEntityNotation_xmldomcb

#define XMLDOM_GET_ENTITY_NOTATION_F(func, xctx, ent) \
    oratext* func(xmlctx *xctx, xmlentnode *ent)

    XMLDOM_GET_ENTITY_NOTATION_F((*XMLDOM_GET_ENTITY_NOTATION_CB), xctx, ent);

#define XmlDomGetEntityNotation(xctx, ent) \
    (*XML_DOMCB(xctx)->XMLDOM_GET_ENTITY_NOTATION_CB) \
        ((xctx), (ent))

/*---------------------------------------------------------------------------
    NAME
        XmlDomGetEntityType

    PURPOSE
        Get entity's type

    DESCRIPTION
        Returns a boolean for an entity describing whether it is general
        (TRUE) or parameter (FALSE).

    RETURNS
        (boolean) TRUE for general entity, FALSE for parameter entity

    ARGUMENTS
        xctx (IN) XML context
        ent  (IN) entity node

    SEE ALSO
        XmlDomGetEntityPubID, XmlDomGetEntitySysID, XmlDomGetEntityNotation
*/
#define XMLDOM_GET_ENTITY_TYPE_CB getEntityType_xmldomcb

#define XMLDOM_GET_ENTITY_TYPE_F(func, xctx, ent) \
    boolean func(xmlctx *xctx, xmlentnode *ent)

    XMLDOM_GET_ENTITY_TYPE_F((*XMLDOM_GET_ENTITY_TYPE_CB), xctx, ent);

#define XmlDomGetEntityType(xctx, ent) \
    (*XML_DOMCB(xctx)->XMLDOM_GET_ENTITY_TYPE_CB) \
        ((xctx), (ent))

/****************************************************************************
                        Interface ProcessingInstruction
 ****************************************************************************/

/*---------------------------------------------------------------------------
    NAME
        XmlDomGetPITarget

    STANDARD
        DOM 2: ProcessingInstruction.target

    PURPOSE
        Get PI's target

    DESCRIPTION
        Returns a processing instruction's target string.  If the
        node is not a PI, returns NULL.  The target is the first token
        following the markup that begins the PI.  All PIs must have a
        target, though the data part is optional.

    EXAMPLE
        <?rating extra saucy?>
        XmlDomGetPITarget --> "rating"

    RETURNS
        (oratext *) processing instruction's target [data encoding]

    ARGUMENTS
        xctx (IN) XML context
        pi   (IN) processing instruction node

    SEE ALSO
        XmlDomGetPIData, XmlDomSetPIData
*/
#define XMLDOM_GET_PI_TARGET_CB getPITarget_xmldomcb

#define XMLDOM_GET_PI_TARGET_F(func, xctx, pi) \
    oratext* func(xmlctx *xctx, xmlpinode *pi)

    XMLDOM_GET_PI_TARGET_F((*XMLDOM_GET_PI_TARGET_CB), xctx, pi);

#define XmlDomGetPITarget(xctx, pi) \
    (*XML_DOMCB(xctx)->XMLDOM_GET_PI_TARGET_CB) \
        ((xctx), (pi))

/*---------------------------------------------------------------------------
    NAME
        XmlDomGetPIData

    STANDARD
        DOM 2: ProcessingInstruction.data

    PURPOSE
        Get processing instruction's data

    DESCRIPTION
        Returns the content (data) of a processing instruction (in the data
        encoding).  If the node is not a PI, returns NULL.  The content is
        the part from the first non-whitespace character after the target
        until the ending "?>".

    EXAMPLE
        <?rating extra saucy?>
        XmlDomGetPIData --> "extra saucy"

    RETURNS
        (oratext *) processing instruction's data [data encoding]

    ARGUMENTS
        xctx (IN) XML context
        pi   (IN) processing instruction node

    SEE ALSO
        XmlDomGetPITarget, XmlDomSetPIData
*/
#define XMLDOM_GET_PI_DATA_CB getPIData_xmldomcb

#define XMLDOM_GET_PI_DATA_F(func, xctx, pi) \
    oratext* func(xmlctx *xctx, xmlpinode *pi)

    XMLDOM_GET_PI_DATA_F((*XMLDOM_GET_PI_DATA_CB), xctx, pi);

#define XmlDomGetPIData(xctx, pi) \
    (*XML_DOMCB(xctx)->XMLDOM_GET_PI_DATA_CB) \
        ((xctx), (pi))

#define XMLDOM_SET_PI_DATA_CB setPIData_xmldomcb

#define XMLDOM_SET_PI_DATA_F(func, xctx, pi, data) \
    void func(xmlctx *xctx, xmlpinode *pi, oratext *data)

    XMLDOM_SET_PI_DATA_F((*XMLDOM_SET_PI_DATA_CB), xctx, pi, data);

#define XmlDomSetPIData(xctx, pi, data) \
    (*XML_DOMCB(xctx)->XMLDOM_SET_PI_DATA_CB) \
        ((xctx), (pi), (data))

/*---------------------------------------------------------------------------
    NAME

        XmlDomClearLastError

    PURPOSE
        Clear the last error for document

    DESCRIPTION
        Clears the last error for a given document.

    RETURNS
        (void)
    
    ARGUMENTS
        xctx (IN) XML context
        doc  (IN) XML document [node]
*/
#define XMLDOM_CLEAR_LAST_ERROR_CB clearLastError_xmldomcb

#define XMLDOM_CLEAR_LAST_ERROR_F(func, xctx, doc) \
    void func(xmlctx *xctx, xmldocnode *doc)

    XMLDOM_CLEAR_LAST_ERROR_F((*XMLDOM_CLEAR_LAST_ERROR_CB), xctx, doc);

#define XmlDomClearLastError(xctx, doc) \
    (*XML_DOMCB(xctx)->XMLDOM_CLEAR_LAST_ERROR_CB) \
        ((xctx), (doc))

/*---------------------------------------------------------------------------
    NAME
        XmlDomGetLastErrorStr

    PURPOSE
        Return last error message for document

    DESCRIPTION
        Returns the error message of the last error which occured in
        the given document.

    RETURNS
        (oratext *) error message

    ARGUMENTS
        xctx (IN) XML context
        doc  (IN) XML document [node]
*/
#define XMLDOM_GET_LAST_ERROR_STR_CB getLastErrorStr_xmldomcb

#define XMLDOM_GET_LAST_ERROR_STR_F(func, xctx, doc) \
    oratext *func(xmlctx *xctx, xmldocnode *doc)

    XMLDOM_GET_LAST_ERROR_STR_F((*XMLDOM_GET_LAST_ERROR_STR_CB), xctx, doc);

#define XmlDomGetLastErrorStr(xctx, doc) \
    (*XML_DOMCB(xctx)->XMLDOM_GET_LAST_ERROR_STR_CB) \
        ((xctx), (doc))

/*---------------------------------------------------------------------------
    NAME
        XmlDomGetNodeMiscInfo

    PURPOSE
        Return info about a node

    DESCRIPTION
        Returns info about a node. The return type is ub4. So that 
        the each bit can be used to indicate certain info. 

    RETURNS
        ub4

    ARGUMENTS
        xctx  (IN) XML context
        node  (IN) XML node [node]
*/
#define XMLDOM_GET_NODE_MISC_INFO_CB getNodeMiscInfo_xmldomcb

#define XMLDOM_GET_NODE_MISC_INFO_F(func, xctx, node) \
    ub4 func(xmlctx *xctx, xmlnode *node)

    XMLDOM_GET_NODE_MISC_INFO_F((*XMLDOM_GET_NODE_MISC_INFO_CB), xctx, node);

#define XmlDomGetNodeMiscInfo(xctx, node) \
    (*XML_DOMCB(xctx)->XMLDOM_GET_NODE_MISC_INFO_CB) \
        ((xctx), (node))
  
/* NOTE...NOTE...PLEASE ADD SOME COMMENTS. This file is modified by more than
 * one person. So comments will help!
 */
#define XMLDOM_GETPULL_NODE_VALUE_BSTREAM_CB \
        getPullNodeValueAsBinaryStream_xmldomcb

#define XMLDOM_GETPULL_NODE_VALUE_BSTREAM_F(func,xctx,node) \
        orastream *func(xmlctx *xctx,xmlnode* node)

XMLDOM_GETPULL_NODE_VALUE_BSTREAM_F((*XMLDOM_GETPULL_NODE_VALUE_BSTREAM_CB),xctx,node);

#define XmlDomGetPullNodeAsBinaryStream(xctx,node)\
(*XML_DOMCB(xctx)->XMLDOM_GETPULL_NODE_VALUE_BSTREAM_CB)\
((xctx),(node))
                                    
#define XMLDOM_GETPULL_NODE_VALUE_CSTREAM_CB\
        getPullNodeValueAsCharacterStream_xmldomcb

#define XMLDOM_GETPULL_NODE_VALUE_CSTREAM_F(func,xctx,node)\
        orastream *func(xmlctx *xctx,xmlnode* node)

XMLDOM_GETPULL_NODE_VALUE_CSTREAM_F((*XMLDOM_GETPULL_NODE_VALUE_CSTREAM_CB),xctx,node);

#define XmlDomGetPullNodeAsCharacterStream(xctx,node)\
(*XML_DOMCB(xctx)->XMLDOM_GETPULL_NODE_VALUE_CSTREAM_CB)\
((xctx),(node)) 


#define XMLDOM_GETPUSH_NODE_VALUE_BSTREAM_CB\
        getPushNodeValueAsBinaryStream_xmldomcb

#define XMLDOM_GETPUSH_NODE_VALUE_BSTREAM_F(func,xctx,node,ostream)\
        xmlerr *func(xmlctx *xctx,xmlnode* node,orastream* ostream)

XMLDOM_GETPUSH_NODE_VALUE_BSTREAM_F((*XMLDOM_GETPUSH_NODE_VALUE_BSTREAM_CB),xctx,node,ostream);

#define XmlDomGetPushNodeAsBinaryStream(xctx,node,ostream)\
(*XML_DOMCB(xctx)->XMLDOM_GETPUSH_NODE_VALUE_BSTREAM_CB)\
((xctx),(node),(ostream)) 


#define XMLDOM_GETPUSH_NODE_VALUE_CSTREAM_CB\
        getPushNodeValueAsCharacterStream_xmldomcb

#define XMLDOM_GETPUSH_NODE_VALUE_CSTREAM_F(func,xctx,node,ostream)\
        xmlerr *func(xmlctx *xctx,xmlnode* node,orastream* ostream)

XMLDOM_GETPUSH_NODE_VALUE_CSTREAM_F((*XMLDOM_GETPUSH_NODE_VALUE_CSTREAM_CB),xctx,node,ostream);

#define XmlDomGetPushNodeAsCharacterStream(xctx,node,ostream)\
(*XML_DOMCB(xctx)->XMLDOM_GETPUSH_NODE_VALUE_CSTREAM_CB)\
((xctx),(node),(ostream))       




#define XMLDOM_SETPULL_NODE_VALUE_BSTREAM_CB\
        setPullNodeValueAsBinaryStream_xmldomcb

#define XMLDOM_SETPULL_NODE_VALUE_BSTREAM_F(func,xctx,node,istream)\
        xmlerr func(xmlctx *xctx,xmlnode* node,orastream* istream)

XMLDOM_SETPULL_NODE_VALUE_BSTREAM_F((*XMLDOM_SETPULL_NODE_VALUE_BSTREAM_CB),xctx,node,istream);

#define XmlDomSetPullNodeAsBinaryStream(xctx,node,istream)\
(*XML_DOMCB(xctx)->XMLDOM_GETPULL_NODE_VALUE_BSTREAM_CB)\
((xctx),(node),(istream)) 


#define XMLDOM_SETPULL_NODE_VALUE_CSTREAM_CB\
        setPullNodeValueAsCharacterStream_xmldomcb

#define XMLDOM_SETPULL_NODE_VALUE_CSTREAM_F(func,xctx,node,istream)\
        xmlerr func(xmlctx *xctx,xmlnode* node,orastream* istream)

XMLDOM_SETPULL_NODE_VALUE_CSTREAM_F((*XMLDOM_SETPULL_NODE_VALUE_CSTREAM_CB),xctx,node,istream);

#define XmlDomSetPullNodeAsCharacterStream(xctx,node,istream)\
(*XML_DOMCB(xctx)->XMLDOM_GETPULL_NODE_VALUE_CSTREAM_CB)\
((xctx),(node),(istream)) 



#define XMLDOM_SETPUSH_NODE_VALUE_BSTREAM_CB\
        setPushNodeValueAsBinaryStream_xmldomcb

#define XMLDOM_SETPUSH_NODE_VALUE_BSTREAM_F(func,xctx,node)\
        orastream *func(xmlctx *xctx,xmlnode* node)

XMLDOM_SETPUSH_NODE_VALUE_BSTREAM_F((*XMLDOM_SETPUSH_NODE_VALUE_BSTREAM_CB),xctx,node);

#define XmlDomSetPushNodeAsBinaryStream(xctx,node)\
(*XML_DOMCB(xctx)->XMLDOM_SETPUSH_NODE_VALUE_BSTREAM_CB)\
((xctx),(node)) 


#define XMLDOM_SETPUSH_NODE_VALUE_CSTREAM_CB\
        setPushNodeValueAsCharacterStream_xmldomcb

#define XMLDOM_SETPUSH_NODE_VALUE_CSTREAM_F(func,xctx,node)\
        orastream *func(xmlctx *xctx,xmlnode* node)

XMLDOM_SETPUSH_NODE_VALUE_CSTREAM_F((*XMLDOM_SETPUSH_NODE_VALUE_CSTREAM_CB),xctx,node);

#define XmlDomSetPushNodeAsCharacterStream(xctx,node)\
(*XML_DOMCB(xctx)->XMLDOM_SETPUSH_NODE_VALUE_CSTREAM_CB)\
((xctx),(node)) 

/*---------------------------------------------------------------------------
    NAME
        XmlDomRenameNode

    PURPOSE
        Update the name of a node (element/attribute nodes only)
        1. If the prefix does not have a current mapping. The user should
           add the mapping by creating a xmlns attribute and associating it
           with this element node by calling XmlDomSetAttributeNodeNS
        2. namespace attribute nodes cannot be modified

    DESCRIPTION
         

    RETURNS
        xmlnode 

    ARGUMENTS
        xctx    (IN) XML context
        node    (IN) XML node [node]
        tagname (IN) new tagname
*/
#define XMLDOM_RENAME_NODE_CB renameNode_xmldomcb

#define XMLDOM_RENAME_NODE_F(func, xctx, node, tagname) \
    xmlnode  *func(xmlctx *xctx, xmlnode *node, oratext *tagname)

XMLDOM_RENAME_NODE_F((*XMLDOM_RENAME_NODE_CB), xctx, node, tagname);

#define XmlDomRenameNode(xctx, node, tagname) \
    (*XML_DOMCB(xctx)->XMLDOM_RENAME_NODE_CB) \
        ((xctx), (node), (tagname))

/*---------------------------------------------------------------------------
    NAME
        XmlDomRenameNodeNS

    PURPOSE
        Update the name, uri of a node (element/attribute)
        1. If the prefix does not have a current mapping. The user should
           add the mapping by creating a xmlns attribute and associating it
           with this element node by calling XmlDomSetAttributeNodeNS
        2. namespace attribute nodes cannot be modified

    DESCRIPTION
         

    RETURNS
        xmlnode 

    ARGUMENTS
        xctx    (IN) XML context
        node    (IN) XML node [node]
        uri     (IN) the new uri. If NULL, the existing uri is retained.
        tagname (IN) new tagname
        
*/
#define XMLDOM_RENAME_NODE_NS_CB renameNodeNS_xmldomcb

#define XMLDOM_RENAME_NODE_NS_F(func, xctx, node, uri, tagname) \
    xmlnode  *func(xmlctx *xctx, xmlnode *node, oratext *uri, oratext *tagname)

XMLDOM_RENAME_NODE_NS_F((*XMLDOM_RENAME_NODE_NS_CB), xctx, node, uri, tagname);

#define XmlDomRenameNodeNS(xctx, node, uri, tagname) \
    (*XML_DOMCB(xctx)->XMLDOM_RENAME_NODE_NS_CB) \
        ((xctx), (node), (uri), (tagname))


/*---------------------------------------------------------------------------
    NAME
        XmlDomImportNodeWithXQSem

    STANDARD
        

    PURPOSE
        Create a new DOM node from another DOM with XQuery semantics
        and import it into the target node.

    DESCRIPTION
        This DOM API is called by XVM attaching node for xquery construction
        expression with xquery semantics.

    RETURNS
        (xmlnode *) newly copied node (in this Document).

    ARGUMENTS
        xctx (IN) XML document context
        node (IN) node to copy
        tdocnode (IN) target document node to which import is applied
        boolean preserve (IN) xquery constructor preserve semantics
        boolean inherit (IN) xquery constructor inherit semantics

    SEE ALSO
        
*/
#define XMLDOM_IMPORT_NODE_WXQSEM_CB ImportNodeWithXQSem_xmldomcb

#define XMLDOM_IMPORT_NODE_WXQSEM_F(func, xctx, node, tdocnode, \
                                    preserve, inherit) \
    xmlnode* func(xmlctx *xctx, xmlnode *node,\
                  xmlnode *tdocnode, boolean preserve, boolean inherit)

    XMLDOM_IMPORT_NODE_WXQSEM_F((*XMLDOM_IMPORT_NODE_WXQSEM_CB), xctx, node, tdocnode, preserve, inherit);

#define XmlDomImportNodeWithXQSem(xctx, node, tdocnode, preserve, inherit) \
    (*XML_DOMCB(xctx)->XMLDOM_IMPORT_NODE_WXQSEM_CB) \
        ((xctx), (node), tdocnode, (preserve), (inherit))

/*---------------------------------------------------------------------------
    NAME
        XmlDomGetNodeQNameId

    STANDARD
        

    PURPOSE
        Get the qname of the node as ub8 integer id

    DESCRIPTION
        This DOM API is called by XVM NodeTest to use qname id (ub8) instead of
        qname string to do qname matching. Only DOM implementation which
        assigns qname id may implement this.

    RETURNS
        qname id (as ub8) of the node

    ARGUMENTS
        xctx (IN) XML document context
        node (IN) node to copy

    SEE ALSO
        
*/
#define XMLDOM_GET_NODE_QNAMEID_CB get_node_qnameid_xmldomcb

#define XMLDOM_GET_NODE_QNAMEID_F(func, xctx, node) \
    ub8 func(xmlctx *xctx, xmlnode *node)

    XMLDOM_GET_NODE_QNAMEID_F((*XMLDOM_GET_NODE_QNAMEID_CB), xctx, node);

#define XmlDomGetNodeQNameId(xctx, node)\
    (*XML_DOMCB(xctx)->XMLDOM_GET_NODE_QNAMEID_CB) \
        ((xctx), (node))

/*---------------------------------------------------------------------------
    NAME
        XmlDomSetNodeSecurity

    STANDARD
        

    PURPOSE
        Set/clear the security check flag for the node.

    DESCRIPTION
        This DOM API is called to set/clear security check flag for the node

    RETURNS
        

    ARGUMENTS
        xctx (IN) XML document context
        node (IN) node to set security flag
        mode (IN) 0 means clear the security flag
                  otherwise it is XMLDOM_NODE_SECURE flag

    SEE ALSO
        
*/
#define XMLDOM_NODE_SECURE 0x1

#define XMLDOM_SET_NODE_SECURITY_CB set_node_security_xmldomcb

#define XMLDOM_SET_NODE_SECURITY_F(func, xctx, node, mode) \
    void func(xmlctx *xctx, xmlnode *node, ub4 mode)

    XMLDOM_SET_NODE_SECURITY_F((*XMLDOM_SET_NODE_SECURITY_CB), xctx, node,mode);

#define XmlDomSetNodeSecurity(xctx, node, mode)\
    (*XML_DOMCB(xctx)->XMLDOM_SET_NODE_SECURITY_CB) \
        ((xctx), (node), (mode))



/*---------------------------------------------------------------------------
    NAME
        XmlDomGetNodeSecurity

    STANDARD
        

    PURPOSE
        Get the security check flag of the node.

    DESCRIPTION
        This DOM API is called to get security check flag for the node

    RETURNS
        ub4 flag. 0 means no security check; otherwise it is 
        XMLDOM_NODE_SECURE flag
        

    ARGUMENTS
        xctx (IN) XML document context
        node (IN) node to get security flag

    SEE ALSO
        
*/
#define XMLDOM_GET_NODE_SECURITY_CB get_node_security_xmldomcb

#define XMLDOM_GET_NODE_SECURITY_F(func, xctx, node) \
    ub4 func(xmlctx *xctx, xmlnode *node)

    XMLDOM_GET_NODE_SECURITY_F((*XMLDOM_GET_NODE_SECURITY_CB), xctx, node);

#define XmlDomGetNodeSecurity(xctx, node)\
    (*XML_DOMCB(xctx)->XMLDOM_GET_NODE_SECURITY_CB) \
        ((xctx), (node))


/****************************************************************************
                               Interface DOM PSVI
 ****************************************************************************/

/*---------------------------------------------------------------------------
    NAME
        XmlDomGetNodeSchType

    STANDARD
        

    PURPOSE
        Get node XMLSchema type

    DESCRIPTION
        XML DOM nodes get types after XMLSchema validation. 

    RETURNS
        (xmlschtype *) a pointer to XMLSchema type. NULL if not validated.

    ARGUMENTS
        xctx  (IN) XML context
        node  (IN) the DOM node.
*/

#define XMLDOM_GET_NODE_SCHTYPE_CB getNodeSchType_xmldomcb

#define XMLDOM_GET_NODE_SCHTYPE_F(func, xctx, node) \
    xmlschtype* func(xmlctx *xctx, xmlnode *node)

    XMLDOM_GET_NODE_SCHTYPE_F((*XMLDOM_GET_NODE_SCHTYPE_CB), xctx, node);

#define XmlDomGetNodeSchType(xctx, node) \
    (*XML_DOMCB(xctx)->XMLDOM_GET_NODE_SCHTYPE_CB) \
        ((xctx), (node))

/*---------------------------------------------------------------------------
    NAME
        XmlDomGetNodeTypedValue

    STANDARD
        

    PURPOSE
        Get node XMLSchema typed value

    DESCRIPTION
        XML DOM node value after XMLSchema validation. 

    RETURNS
        (ub1 *) a pointer to node typed value. NULL if the node value 
                is not typed.

    ARGUMENTS
        xctx  (IN)  XML context
        node  (IN)  the DOM node.
        len   (OUT) the length of returned value.
        num   (IN)  the value component number. Some values (like QName)
                    have more than one component. In that case each
                    value component is obtained with its number.  
        rsltbuf (IN/OUT) pointer to a result buffer of at most 
                         XMLDOM_GET_NODE_TYPED_VALUE_TMPBUF_SIZE (22) size. 
                     It is used 
                     for caller to pass in a tmp buffer so that callee can
                     put the node value into this tmp buffer and set return
                     value to be pointer to this tmp buffer. That way, callee
                     does need to allocate memory to hold return value smaller
                     than  XMLDOM_GET_NODE_TYPED_VALUE_TMPBUF_SIZE size because
                     once the memory is allocated by callee, callee does not
                     know when to free the memory. This helps for most of the
                     built-in fixed size schema type value (xs:decimal, 
                     xs:double, xs:integer) whose binary value
                     requires at most  XMLDOM_GET_NODE_TYPED_VALUE_TMPBUF_SIZE
                     storage.
*/

#define XMLDOM_GET_NODE_TYPED_VALUE_TMPBUF_SIZE 22
#define XMLDOM_GET_NODE_TYPED_VALUE_CB getNodeTypedValue_xmldomcb

#define XMLDOM_GET_NODE_TYPED_VALUE_F(func, xctx, node, len, num, rsltbuf)\
    ub1* func(xmlctx *xctx, xmlnode *node, ub4 *len, ub4 num, ub1 *rsltbuf)

    XMLDOM_GET_NODE_TYPED_VALUE_F((*XMLDOM_GET_NODE_TYPED_VALUE_CB), xctx, node, len, num, rsltbuf);

#define XmlDomGetNodeTypedValue(xctx, node, len, num, rsltbuf) \
    (*XML_DOMCB(xctx)->XMLDOM_GET_NODE_TYPED_VALUE_CB) \
        ((xctx), (node), (len), (num), (rsltbuf))

/*---------------------------------------------------------------------------
    NAME
        XmlDomIsNodeNilled

    STANDARD
        

    PURPOSE
        Informs if the node is XMLSchema declared as "nilled"

    DESCRIPTION
        XML DOM nodes are annotated after XMLSchema validation. 

    RETURNS
        (boolean) TRUE if node is declared as "nilled".

    ARGUMENTS
        xctx  (IN) XML context
        node  (IN) the DOM node.
*/

#define XMLDOM_IS_NODE_NILLED_CB isNodeNilled_xmldomcb

#define XMLDOM_IS_NODE_NILLED_F(func, xctx, node) \
    boolean func(xmlctx *xctx, xmlnode *node)

    XMLDOM_IS_NODE_NILLED_F((*XMLDOM_IS_NODE_NILLED_CB), xctx, node);

#define XmlDomIsNodeNilled(xctx, node) \
    (*XML_DOMCB(xctx)->XMLDOM_IS_NODE_NILLED_CB) \
        ((xctx), (node))

/*---------------------------------------------------------------------------
    NAME
        XmlDomGetNodePrefixLen

    PURPOSE
        Get node's prefix as length-encoded string

    DESCRIPTION
        Returns the prefix of a node (in the data encoding)
        as a length-encoded string,.

        If both the input buffer is non-NULL and the input buffer length is
        non-zero, then the value will be stored in the input buffer and null
        terminated.  Else, the implementation will return a 
        non NULL terminated prefix value from the storage.

        If the actual length is greater than buflen, then a truncated value
        will be copied into the buffer and len will return the actual length.
        The trunctated value will be NULL terminated as well.

    RETURNS
        (oratext *) prefix of node, with length of prefix set in 'len'

    ARGUMENTS
        xctx   (IN)  XML context
        node   (IN)  XML node
        buf    (IN)  input buffer [optional]
        buflen (IN)  input buffer length [optional]
        len    (OUT) length of prefix [in characters]

    SEE ALSO
        XmlDomGetNodePrefixLen
*/
#define XMLDOM_GET_NODE_PREFIX_LEN_CB getNodePrefixLen_xmldomcb

#define XMLDOM_GET_NODE_PREFIX_LEN_F(func, xctx, node, buf, buflen, len) \
    oratext* func(xmlctx *xctx, xmlnode *node, oratext *buf, ub4 buflen, \
                  ub4 *len)

    XMLDOM_GET_NODE_PREFIX_LEN_F((*XMLDOM_GET_NODE_PREFIX_LEN_CB), xctx, node, 
                               buf, buflen, len);

#define XmlDomGetNodePrefixLen(xctx, node, buf, buflen, len) \
    (*XML_DOMCB(xctx)->XMLDOM_GET_NODE_PREFIX_LEN_CB) \
        ((xctx), (node), (buf), (buflen), (len))
/*---------------------------------------------------------------------------
    NAME
        XmlDomSetEnvHdl

    PURPOSE
        Store External Env hanlder into the root doc node.

    DESCRIPTION

        This allows the DOM API impelmentation to store external env handlder
        to which it is associated with.

    RETURNS
        nothing

    ARGUMENTS
        xctx   (IN)  XML context
        node   (IN)  XML doc node
        envhdl (IN)  env handler

    SEE ALSO
        
*/
#define XMLDOM_SET_ENV_HDL_CB setEnvHdl_xmldomcb

#define XMLDOM_SET_ENV_HDL_F(func, xctx, node, envhdl) \
    void func(xmlctx *xctx, xmldocnode *node, void *envhdl)

    XMLDOM_SET_ENV_HDL_F((*XMLDOM_SET_ENV_HDL_CB), xctx, node, envhdl);

#define XmlDomSetEnvHdl(xctx, node, envhdl) \
    (*XML_DOMCB(xctx)->XMLDOM_SET_ENV_HDL_CB) \
        ((xctx), (node), (envhdl))

/*---------------------------------------------------------------------------
    NAME
        XmlDomGetEnvHdl

    PURPOSE
        Get the External Env hanlder from the root doc node.

    DESCRIPTION

        This allows the DOM API impelmentation to get external env handlder
        to which it is associated with.

    RETURNS
        void *envhdl

    ARGUMENTS
        xctx   (IN)  XML context
        node   (IN)  XML doc node

    SEE ALSO
        
*/
#define XMLDOM_GET_ENV_HDL_CB getEnvHdl_xmldomcb

#define XMLDOM_GET_ENV_HDL_F(func, xctx, node) \
    void* func(xmlctx *xctx, xmldocnode *node)

    XMLDOM_GET_ENV_HDL_F((*XMLDOM_GET_ENV_HDL_CB), xctx, node);

#define XmlDomGetEnvHdl(xctx, node) \
    (*XML_DOMCB(xctx)->XMLDOM_GET_ENV_HDL_CB) \
        ((xctx), (node))



/*---------------------------------------------------------------------------
    NAME
        XmlDomSetDocFragOrder

    PURPOSE
        Set document order for all nodes within a fragment.

    DESCRIPTION
        Sets the document order for each node in the current fragment.  This
        is called for document fragment constructed by xquery expression
        constructor where the input node may not be a document node, but
        rather element node, text node etc.

    RETURNS
        (ub4) highest ordinal assigned

    ARGUMENTS
        xctx     (IN) XML context
        node      (IN) XML node
        start_id (IN) starting ID#

    SEE ALSO
        XmlDomGetDocOrder, XmlDomCompareDocOrder
*/
#define XMLDOM_SET_DOCFRAG_ORDER_CB setDocFragOrder_xmldomcb

#define XMLDOM_SET_DOCFRAG_ORDER_F(func, xctx, node, start_id) \
    ub4 func(xmlctx *xctx, xmlnode *node, ub4 start_id)

    XMLDOM_SET_DOCFRAG_ORDER_F((*XMLDOM_SET_DOCFRAG_ORDER_CB), xctx, node, start_id);

#define XmlDomSetDocFragOrder(xctx, node, start_id) \
    (*XML_DOMCB(xctx)->XMLDOM_SET_DOCFRAG_ORDER_CB) \
        ((xctx), (node), (start_id))


/*---------------------------------------------------------------------------
    NAME
        XmlDomGetFirstAttr

    STANDARD
        

    PURPOSE
        Return first attribute node of an element node

    DESCRIPTION
        Returns the first attribute node of an element node, or NULL
        if it has no attributes.  For other node types, NULL is returned.

    RETURNS
        (xmlattrnode *) attribute node

    ARGUMENTS
        xctx (IN) XML context
        elem (IN) XML element node

    SEE ALSO
        XmlDomNumAttrs, XmlDomHasChildNodes
*/
#define XMLDOM_GET_FIRST_ATTR_CB getFirstAttr_xmldomcb

#define XMLDOM_GET_FIRST_ATTR_F(func, xctx, elem) \
    xmlattrnode* func(xmlctx *xctx, xmlelemnode *elem)

    XMLDOM_GET_FIRST_ATTR_F((*XMLDOM_GET_FIRST_ATTR_CB), xctx, elem);

#define XmlDomGetFirstAttr(xctx, elem) \
    (*XML_DOMCB(xctx)->XMLDOM_GET_FIRST_ATTR_CB) \
        ((xctx), (elem))


/*---------------------------------------------------------------------------
    NAME
        XmlDomGetNextAttr

    STANDARD
        

    PURPOSE
        Return the next attribute node of the input attribute node

    DESCRIPTION
        Returns the next attribute node of an attribute node, or NULL
        if it is the last attribute node. For other node types, 
        NULL is returned.

    RETURNS
        (xmlattrnode *) attribute node

    ARGUMENTS
        xctx (IN) XML context
        attr (IN) XML attribute node

    SEE ALSO
        XmlDomNumAttrs, XmlDomHasChildNodes
*/
#define XMLDOM_GET_NEXT_ATTR_CB getNextAttr_xmldomcb

#define XMLDOM_GET_NEXT_ATTR_F(func, xctx, attr) \
    xmlattrnode* func(xmlctx *xctx, xmlattrnode *attr)

    XMLDOM_GET_NEXT_ATTR_F((*XMLDOM_GET_NEXT_ATTR_CB), xctx, attr);

#define XmlDomGetNextAttr(xctx, attr) \
    (*XML_DOMCB(xctx)->XMLDOM_GET_NEXT_ATTR_CB) \
        ((xctx), (attr))

/*---------------------------------------------------------------------------
    NAME
        XmlDomCreateAttrLen

    STANDARD
        DOM 2: Document.createAttribute()

    PURPOSE
        Create attribute node with len

    DESCRIPTION
        Same as  XmlDomCreateAttr but the text arguments are passed in with 
        length
    
    RETURNS:
        (xmlattrnode *) new Attr node.

    ARGUMENTS
        xctx  (IN) XML context
        doc   (IN) XML document [node]
        name  (IN) new node's name [data encoding; user control]
        namelen (IN) name's length in bytes
        value (IN) new node's value [data encoding; user control]
        valuelen (IN) value's length in bytyes

    SEE ALSO
        XmlDomSetAttrValue, XmlDomCreateAttrNS, XmlDomSetAttr,
        XmlDomCleanNode, XmlDomFreeNode, XmlDomSetAttrNode
*/
#define XMLDOM_CREATE_ATTR_LEN_CB createAttrLen_xmldomcb

#define XMLDOM_CREATE_ATTR_LEN_F(func, xctx, doc, name, namelen, \
                                 value, valuelen) \
    xmlattrnode* func(xmlctx *xctx, xmldocnode *doc, \
                      oratext *name, ub4 namelen, oratext *value, ub4 valuelen)

    XMLDOM_CREATE_ATTR_LEN_F((*XMLDOM_CREATE_ATTR_LEN_CB), xctx, doc, name, namelen,
                             value, valuelen);

#define XmlDomCreateAttrLen(xctx, doc, name, namelen, value, valuelen) \
    (*XML_DOMCB(xctx)->XMLDOM_CREATE_ATTR_LEN_CB) \
        ((xctx), (doc), (name), (namelen), (value), (valuelen))

/*---------------------------------------------------------------------------
    NAME
        XmlDomCreateElemLen

    STANDARD
        DOM 2: Document.createElement()

    PURPOSE
        Create an element node

    DESCRIPTION
        Same as XmlDomCreateElem with length for the text arguments
 
    RETURNS
        (xmlelemnode *) new Element node.

    ARGUMENTS
        xctx    (IN) XML context
        doc     (IN) XML document [node]
        tagname (IN) new node's name [data encoding; user control]
        tagnamelen (IN) node' name length

    SEE ALSO
        XmlDomCreateElemNS, XmlDomCleanNode, XmlDomFreeNode
*/
#define XMLDOM_CREATE_ELEM_LEN_CB createElemLen_xmldomcb

#define XMLDOM_CREATE_ELEM_LEN_F(func, xctx, doc, tagname, tagnamelen) \
    xmlelemnode* func(xmlctx *xctx, xmldocnode *doc, oratext *tagname, \
                      ub4 tagnamelen)                                  

    XMLDOM_CREATE_ELEM_LEN_F((*XMLDOM_CREATE_ELEM_LEN_CB), xctx, doc, tagname,
                              tagnamelen);

#define XmlDomCreateElemLen(xctx, doc, tagname, tagnamelen) \
    (*XML_DOMCB(xctx)->XMLDOM_CREATE_ELEM_LEN_CB) \
        ((xctx), (doc), (tagname), (tagnamelen))

/*---------------------------------------------------------------------------
    NAME
        XmlDomCreateAttrNSLen

    STANDARD
        DOM 2: Document.createAttributeNS()

    PURPOSE
        Create attribute node with namespace information

    DESCRIPTION
        Same  as XmlDomCreateAttrNS with non-NULL terminated input strings

    RETURNS
        (xmlattrnode *) new Attr node.

    ARGUMENTS
        xctx  (IN) XML context
        doc   (IN) XML document [node]
        uri   (IN) node's namespace URI [data encoding; user control]
        urilen (IN) uri length
        qname (IN) node's qualified name [data encoding; user control]
        qnamelen (IN) qname length
        value (IN) node's value [data encoding; user control]
        valuelen (IN) value length

    SEE ALSO
        XmlDomSetAttrValue, XmlDomCreateAttr, XmlDomSetAttrNS,
        XmlDomCleanNode, XmlDomFreeNode, XmlDomSetAttrNodeNS
*/
#define XMLDOM_CREATE_ATTR_NS_LEN_CB createAttrNSLen_xmldomcb

#define XMLDOM_CREATE_ATTR_NS_LEN_F(func, xctx, doc, uri, urilen, \
                                    qname, qnamelen, value, valuelen) \
    xmlattrnode* func(xmlctx *xctx, xmldocnode *doc, oratext *uri, ub4 urilen, \
                      oratext *qname, ub4 qnamelen,  oratext *value, ub4 valuelen)

    XMLDOM_CREATE_ATTR_NS_LEN_F((*XMLDOM_CREATE_ATTR_NS_LEN_CB),
                            xctx, doc, uri, urilen, qname, qnamelen, 
                            value, valuelen);

#define XmlDomCreateAttrNSLen(xctx, doc, uri, urilen, qname, qnamelen, \
                              value, valuelen) \
    (*XML_DOMCB(xctx)->XMLDOM_CREATE_ATTR_NS_LEN_CB) \
        ((xctx), (doc), (uri), (urilen), (qname), (qnamelen), (value), (valuelen))


/*---------------------------------------------------------------------------
    NAME
        XmlDomCreateElemNSLen

    STANDARD
        DOM 2: Document.createElementNS()

    PURPOSE
        Create an element node with namespace information

    DESCRIPTION
        Same as XmlDomCreateElemLen
 
    RETURNS
        (xmlelemnode *) new Element node.

    ARGUMENTS
        xctx  (IN) XML context
        doc   (IN) XML document [node]
        uri   (IN) new node's namespace URI [data encoding; user control]
        urilen (IN) uri length
        qname (IN) new node's qualified name [data encoding; user control]
        qnamelen (IN) qname length 

    SEE ALSO
        XmlDomCreateElem, XmlDomCleanNode, XmlDomFreeNode
*/
#define XMLDOM_CREATE_ELEM_NS_LEN_CB createElemNSLen_xmldomcb

#define XMLDOM_CREATE_ELEM_NS_LEN_F(func, xctx, doc, uri, urilen, \
                                    qname, qnamelen) \
    xmlelemnode* func(xmlctx *xctx, xmldocnode *doc, \
                      oratext *uri, ub4 urilen, oratext *qname, ub4 qnamelen)

    XMLDOM_CREATE_ELEM_NS_LEN_F((*XMLDOM_CREATE_ELEM_NS_LEN_CB), xctx, doc, 
                                uri, urilen, qname, qnamelen);

#define XmlDomCreateElemNSLen(xctx, doc, uri, urilen, qname, qnamelen) \
    (*XML_DOMCB(xctx)->XMLDOM_CREATE_ELEM_NS_LEN_CB) \
        ((xctx),(doc),(uri),(urilen), (qname), (qnamelen))

/*---------------------------------------------------------------------------
    NAME
        XmlDomInsertAfter


    PURPOSE
        Insert new child into node's list of children

    DESCRIPTION
        Inserts the node newChild after the existing child node refChild
        in the parent node.  If refChild is NULL, appends to parent's
        children as per XmlDomAppendChild; otherwise it must be a child of the
        given parent.  If newChild is a DocumentFragment, all of its children
        are inserted (in the same order) after refChild; the DocumentFragment
        node itself is not.  If newChild is already in the DOM tree, it is
        first removed from its current position.

    RETURNS
        (xmlnode *) node being inserted

    ARGUMENTS
        xctx     (IN) XML context
        parent   (IN) parent to receive new child
        newChild (IN) node to insert
        refChild (IN) reference node to insert after

    SEE ALSO
        XmlDomAppendChild, XmlDomReplaceChild, XmlDomRemoveChild
*/
#define XMLDOM_INSERT_AFTER_CB insertAfter_xmldomcb

#define XMLDOM_INSERT_AFTER_F(func, xctx, parent, newChild, refChild) \
    xmlnode* func(xmlctx *xctx, xmlnode *parent, \
                  xmlnode *newChild, xmlnode *refChild)

    XMLDOM_INSERT_AFTER_F((*XMLDOM_INSERT_AFTER_CB),
                           xctx, parent, newChild, refChild);

#define XmlDomInsertAfter(xctx, parent, child, ref) \
    (*XML_DOMCB(xctx)->XMLDOM_INSERT_AFTER_CB) \
        ((xctx), (parent), (child), (ref))

/*---------------------------------------------------------------------------
    NAME
        XmlDomCreateTextLen

    STANDARD
        DOM 2: Document.createTextNode()

    PURPOSE
        Create text node with length

    DESCRIPTION
        
        See XmlDomCreateText
    RETURNS
        (xmltextnode *) new Text node.

    ARGUMENTS
        xctx (IN) XML context
        doc  (IN) XML document [node]
        data (IN) new node's text [data encoding; user control]
        datalen (IN) length of data

    SEE ALSO
        XmlDomCreateCDATA, XmlDomSetNodeValue, XmlDomGetNodeValue,
        XmlDomSetCharData, XmlDomGetCharData, XmlDomGetCharDataLength,
        XmlDomSubstringData, XmlDomAppendData, XmlDomInsertData,
        XmlDomDeleteData, XmlDomReplaceData, XmlDomCleanNode, XmlDomFreeNode
*/
#define XMLDOM_CREATE_TEXT_NODE_LEN_CB createTextLen_xmldomcb

#define XMLDOM_CREATE_TEXT_NODE_LEN_F(func, xctx, doc, data, datalen) \
    xmltextnode* func(xmlctx *xctx, xmldocnode *doc, oratext *data, ub4 datalen)

    XMLDOM_CREATE_TEXT_NODE_LEN_F((*XMLDOM_CREATE_TEXT_NODE_LEN_CB), 
                                   xctx, doc, data, datalen);

#define XmlDomCreateTextLen(xctx, doc, data, datalen) \
    (*XML_DOMCB(xctx)->XMLDOM_CREATE_TEXT_NODE_LEN_CB) \
        ((xctx), (doc), (data), (datalen))


/*---------------------------------------------------------------------------
    NAME
        XmlDomSetDecl

    PURPOSE
        Sets a document's XMLDecl information

    DESCRIPTION
        Set the information for a document's XMLDecl.  See XmlDomGetDecl 
        for details on the parameters.

    RETURNS
        NOTHING

    ARGUMENTS
        xctx (IN)  XML context
        doc  (IN)  XML document [node]
        ver  (IN)  XML version
        enc  (IN)  encoding specification
        std  (IN)  standalone specification
*/
#define XMLDOM_SET_DECL_CB setDecl_xmldomcb

#define XMLDOM_SET_DECL_F(func, xctx, doc, ver, enc, std) \
    xmlerr func(xmlctx *xctx, xmldocnode *doc, oratext *ver, \
                oratext *enc, sb4 std)

    XMLDOM_SET_DECL_F((*XMLDOM_SET_DECL_CB), xctx, doc, ver, enc, std);

#define XmlDomSetDecl(xctx, doc, ver, enc, std) \
    (*XML_DOMCB(xctx)->XMLDOM_SET_DECL_CB) \
        ((xctx), (doc), (ver), (enc), (std))

/*---------------------------------------------------------------------------
    NAME
        XmlDomCreateCommentLen

    STANDARD
        DOM 2: Document.createCommentLen()

    PURPOSE
        Create comment node with the data length given

    DESCRIPTION
        Creates a Comment node with the given initial data (which must be
        in the data encoding).  The data may be NULL; if provided, it is not
        verified, converted, or checked.  If bad data is set, bad things may
        happen-- GIGO!  The name of a Comment node is always "#comment".

        The new node is an orphan with no parent; it must be added to the DOM
        tree with XmlDomAppendChild etc.

        The comment data is NOT copied, its pointer is just stored.  The user
        is responsible for persistence and freeing of that data.

    RETURNS
        (xmlcommentnode *) new Comment node.

    ARGUMENTS
        xctx (IN) XML context
        doc  (IN) XML document [node]
        data (IN) new node's comment [data encoding; user control]
        data (IN) new node's comment len 

    SEE ALSO
        XmlDomCleanNode, XmlDomFreeNode
*/
#define XMLDOM_CREATE_COMMENT_LEN_CB createCommentLen_xmldomcb

#define XMLDOM_CREATE_COMMENT_LEN_F(func, xctx, doc, data, datalen) \
    xmlcommentnode* func(xmlctx *xctx, xmldocnode *doc, oratext *data, ub4 datalen)

    XMLDOM_CREATE_COMMENT_LEN_F((*XMLDOM_CREATE_COMMENT_LEN_CB), xctx, doc, data, datalen);

#define XmlDomCreateCommentLen(xctx, doc, data, datalen) \
    (*XML_DOMCB(xctx)->XMLDOM_CREATE_COMMENT_LEN_CB) \
        ((xctx), (doc), (data), (datalen))

/*---------------------------------------------------------------------------
    NAME
        XmlDomCreateCDATALen

    STANDARD
        DOM 2: Document.createCDATASectionLen()

    PURPOSE
        Create CDATA node with its data content length passed in

    DESCRIPTION
        Creates a CDATASection node with the given initial data (which
        should be in the data encoding).  A CDATA section is considered
        verbatim and is never parsed; it will not be joined with adjacent
        Text nodes by the normalize operation.  The initial data may be
        NULL; if provided, it is not verified, converted, or checked.  If
        bad data is set, bad things will happen-- GIGO!  The name of a
        CDATA node is always "#cdata-section".

        The new node is an orphan with no parent; it must be added to the
        DOM tree with XmlDomAppendChild etc.

        The CDATA is NOT copied, its pointer is just stored.  The user is
        responsible for persistence and freeing of that data.

    RETURNS
        (xmlcdatanode *) new CDATA node.

    ARGUMENTS
        xctx (IN) XML context
        doc  (IN) XML document [node]
        data (IN) new node's CDATA [data encoding; user control]
        datalen (IN) new node's CDATA length

    SEE ALSO
        XmlDomCreateText, XmlDomCleanNode, XmlDomFreeNode
*/
#define XMLDOM_CREATE_CDATA_LEN_CB createCDATALen_xmldomcb

#define XMLDOM_CREATE_CDATA_LEN_F(func, xctx, doc, data, datalen) \
    xmlcdatanode* func(xmlctx *xctx, xmldocnode *doc, oratext *data,ub4 datalen)

    XMLDOM_CREATE_CDATA_LEN_F((*XMLDOM_CREATE_CDATA_LEN_CB), xctx, doc, data, datalen);

#define XmlDomCreateCDATALen(xctx, doc, data, datalen) \
    (*XML_DOMCB(xctx)->XMLDOM_CREATE_CDATA_LEN_CB) \
        ((xctx), (doc), (data), (datalen))

/*---------------------------------------------------------------------------
    NAME
        XmlDomCreatePILen

    STANDARD
        DOM 2: Document.createProcessingInstructionLen()

    PURPOSE
        Create PI node with its target and data length passed in

    DESCRIPTION
        Creates a Processing Instruction node with the given target and
        data (which should be in the data encoding).  The data may be NULL
        initially, and may be changed later (with XmlDomSetPIData), but the
        target is required and cannot be changed.  Note the target and data
        are not verified, converted, or checked.  If bad data is set, bad
        things will happen-- GIGO!  The name of a PI node is the same as the
        target.

        The new node is an orphan with no parent; it must be added to the
        DOM tree with XmlDomAppendChild etc.

        The PI's target and data are NOT copied, their pointers are just
        stored.  The user is responsible for persistence and freeing of
        that data.

    RETURNS
        (xmlpinode *) new PI node.

    ARGUMENTS
        xctx   (IN) XML context
        doc    (IN) XML document [node]
        target (IN) new node's target [data encoding; user control]
        targetlen (IN) new node's target content length
        data   (IN) new node's data [data encoding; user control]
        datalen (IN) new node's data content length

    SEE ALSO
        XmlDomGetPITarget, XmlDomGetPIData, XmlDomSetPIData,
        XmlDomCleanNode, XmlDomFreeNode
*/
#define XMLDOM_CREATE_PI_LEN_CB createPILen_xmldomcb

#define XMLDOM_CREATE_PI_LEN_F(func, xctx, doc, target,targetlen,data,datalen) \
    xmlpinode* func(xmlctx *xctx, xmldocnode *doc, \
                    oratext *target, ub4 targetlen, oratext *data, ub4 datalen)

    XMLDOM_CREATE_PI_LEN_F((*XMLDOM_CREATE_PI_LEN_CB), xctx, doc, target, targetlen, data, datalen);

#define XmlDomCreatePILen(xctx, doc, target, targetlen, data, datalen) \
    (*XML_DOMCB(xctx)->XMLDOM_CREATE_PI_LEN_CB) \
        ((xctx), (doc), (target), (targetlen), (data), (datalen))

/*---------------------------------------------------------------------------
    NAME
        XmlDomMrgWithPrevTxtOrCrtTxtNodeLen

    STANDARD
        DOM 2: 

    PURPOSE
        Either create a text node or merge with previous text node.
        Either create a cdata node or merge with previous cdata node.
        Either create a comment node or merge with previous comment node.

    DESCRIPTION

    RETURNS
        (xmlpinode *) new text(cdata/cmment) node or the 
                      merged text(cdata/comment) node.

    ARGUMENTS
        xctx   (IN) XML context
        doc    (IN) XML document [node]
        elem   (IN) XML element [node] to which this text nodes needs to be
                    under
        data   (IN) new node's text/cdata/comment data 
        datalen (IN) new node's text/cdata/comment data length
        xmlnodetype (IN)has to be either XMLDOM_TEXT,XMLDOM_CDATA,XMLDOM_COMMENT
        flag    (IN/OUT) in/out flag


    SEE ALSO
*/
#define XMLPROC_OUTFLG_MRGED_WITH_PREV_TXTNODE 0x1
#define XMLDOM_MRGWPREVORCRT_TXT_LEN_CB MrgWithPrevTxtOrCrtTxtNodeLen_xmldomcb

#define XMLDOM_MRGWPREVORCRT_TXT_LEN_F(func, xctx, doc, elem, data,datalen,ndtyp,flag) \
    xmlpinode* func(xmlctx *xctx, xmldocnode *doc, xmlnode *elem,\
                    oratext *data, ub4 datalen, xmlnodetype ndtyp,ub4 *flag)

    XMLDOM_MRGWPREVORCRT_TXT_LEN_F((*XMLDOM_MRGWPREVORCRT_TXT_LEN_CB), xctx, doc,elem, data, datalen, ndtyp, flag);

#define XmlDomMrgWithPrevTxtOrCrtTxtNodeLen(xctx, doc, elem, data, datalen, ndtyp, flag) \
    (*XML_DOMCB(xctx)->XMLDOM_MRGWPREVORCRT_TXT_LEN_CB) \
        ((xctx), (doc), (elem), (data), (datalen), (ndtyp), (flag))

/*---------------------------------------------------------------------------
    NAME
        XmlDomTreatDocAsNonSchemBased

    STANDARD
        DOM 2: 

    PURPOSE
        Treat the XML DOM as non-schema based.

    DESCRIPTION

    RETURNS
        NONE

    ARGUMENTS
        xctx   (IN) XML context
        node   (IN) XML node 

    SEE ALSO
*/

#define XMLDOM_TREAT_DOC_AS_NON_SCHM_CB TreatDocAsNonSchemBased

#define XMLDOM_TREAT_DOC_AS_NON_SCHM_F(func, xctx, node) \
    void func(xmlctx *xctx, xmlnode *node)

    XMLDOM_TREAT_DOC_AS_NON_SCHM_F((*XMLDOM_TREAT_DOC_AS_NON_SCHM_CB), xctx, node);

#define XmlDomTreatDocAsNonSchemBased(xctx, node) \
    (*XML_DOMCB(xctx)->XMLDOM_TREAT_DOC_AS_NON_SCHM_CB) \
        ((xctx), (node))

/*---------------------------------------------------------------------------
    NAME
        XmlDomSetPropertiesVA

    PURPOSE
        Set DOM properties

    STATUS
        Oracle extension [Private]

    DESCRIPTION

    RETURNS
        NONE

    ARGUMENTS
        xctx        (IN) XML context
        doc         (IN) XML document [node]
        va          (IN) va_list

    PROPERTIES
        Control over the stream details is specified through a variable
        property list.  Any number of (name, value) pairs are specified,
        ending with a final NULL.

    SEE ALSO
      XmlDomSetProperties
*/
#define XMLDOM_SET_PROPERTIES_CB setProperties_xmldomcb

#define XMLDOM_SET_PROPERTIES_F(func, xctx, doc, autocreate) \
  xmlerr func(xmlctx *xctx, xmldocnode *doc, va_list va)

    XMLDOM_SET_PROPERTIES_F((*XMLDOM_SET_PROPERTIES_CB), xctx, doc, va);

#define XmlDomSetPropertiesVA(xctx, doc, va) \
    (*XML_DOMCB(xctx)->XMLDOM_SET_PROPERTIES_CB) \
        ((xctx), (doc), (va))
};

xmlerr XmlDomSetProperties(xmlctx *xctx, xmldocnode *doc, ...);

/****************************************************************************
                               Interface Document
 ****************************************************************************/

/*---------------------------------------------------------------------------
    NAME
        XmlDomAddTextElem

    PURPOSE
        Adds a text-containing element

    DESCRIPTION
        This is a shorthand function which performs a common idiom:
        adding a text node to an element.  Given the URI, QName, and
        text string, creates an element with the given URI/QName,
        which in turn has a text child containing the given string.
        The created element is appended to the given parent node.

    RETURNS
        (xmlelemnode *) created element

    ARGUMENTS
        xctx   (IN) XML context
        parent (IN) parent to receive new element
        uri    (IN) URI of created element
        qname  (IN) QName of created element
        value  (IN) value of Text node
*/
xmlelemnode *XmlDomAddTextElem(xmlctx *xctx, xmlelemnode *parent,
                               oratext *uri, oratext *qname, oratext *value);

/*---------------------------------------------------------------------------
    NAME
        XmlDomGetChildText

    PURPOSE
        Return text of named element

    DESCRIPTION
        Searched the given element for a sub-element with the given
        URI and local name.  If found, and the first child of that
        node is TEXT, then the value of the text is returned.  In all
        other cases, NULL is returned.

    RETURNS
        (oratext *) text of named child

    ARGUMENTS
        xctx   (IN) XML context
        elem   (IN) parent node
        uri    (IN) URI of child node
        local  (IN) localname of child node
*/
oratext *XmlDomGetChildText(xmlctx *xctx, xmlelemnode *elem,
                            oratext *uri, oratext *local);

/*---------------------------------------------------------------------------
                   Package Traversal - DOM Traversal APIs
  ---------------------------------------------------------------------------*/

/****************************************************************************
                               Interface NodeIterator
 ****************************************************************************/

/*---------------------------------------------------------------------------
    NAME
        XmlDomIterDetach

    STANDARD
        DOM 2: NodeIterator.detach()

    PURPOSE
        Detach a node iterator (deactivate it)

    DESCRIPTION
        Detaches the NodeIterator from the set which it iterated over,
        releasing any resources and placing the iterator in the INVALID
        state.  After detach has been invoked, calls to XmlDomIterNextNode
        or XmlDomIterPrevNode will raise the exception XMLERR_ITER_DETACHED.

    RETURNS
        (void)

    ARGUMENTS
        xctx (IN) XML context
        iter (IN) NodeIterator object

    SEE ALSO
        XmlDomIterNextNode, XmlDomIterPrevNode
*/
xmlerr XmlDomIterDetach(xmlctx *xctx, xmliter *iter);

/*---------------------------------------------------------------------------
    NAME
        XmlDomIterNextNode

    STANDARD
        DOM 2: TreeWalker.nextNode()

    PURPOSE
        Returns next node for iterator

    DESCRIPTION
        Returns the next node in the set and advances the position of
        the iterator in the set.  After a node iterator is created, the
        first call to XmlDomIterNextNode returns the first node in the set.
        It assumed that the reference node (current iterator position)
        is never deleted.  Otherwise, changes in the underlying DOM tree
        do not invalidate the iterator.

    RETURNS
        (xmlnode *) next node in set being iterated over [or NULL]

    ARGUMENTS
        xctx (IN)  XML context
        iter (IN)  node iterator object
        xerr (OUT) error return code

    SEE ALSO
        XmlDomIterPrevNode, XmlDomIterDetach
*/
xmlnode* XmlDomIterNextNode(xmlctx *xctx, xmliter *iter, xmlerr *xerr);

/*---------------------------------------------------------------------------
    NAME
        XmlDomIterPrevNode

    STANDARD
        DOM 2: TreeWalker.previousNode()

    PURPOSE
        Returns previous node for iterator

    DESCRIPTION
        Returns the previous node in the set and moves the position of
        the iterator backwards in the set.

    RETURNS
        (xmlnode *) previous node in set being iterated over [or NULL]

    ARGUMENTS
        xctx (IN)  XML context
        iter (IN)  node iterator object
        xerr (OUT) error return code

    SEE ALSO
        XmlDomIterNextNode, XmlDomIterDetach
*/
xmlnode* XmlDomIterPrevNode(xmlctx *xctx, xmliter *iter, xmlerr *xerr);

/****************************************************************************
                               Interface NodeFilter
 ****************************************************************************/

/*---------------------------------------------------------------------------
    NAME
        XMLDOM_ACCEPT_NODE_F

    STANDARD
        DOM 2: NodeFilter

    PURPOSE
        Perform user-defined filtering action on node

    DESCRIPTION
        Sole method of {NodeFilter} interface.  Given a node and a
        filter, determines the filtering action to perform.

        This function pointer is passed to the node iterator/tree
        walker methods, as needed.

        Values for xmlerr are:

             XMLERR_OK
                     Accept the node.  Navigation methods defined for
                     NodeIterator or TreeWalker will return this node.

             XMLERR_FILTER_REJECT
                     Reject the node.  Navigation methods defined for
                     NodeIterator or TreeWalker will NOT return this
                     node.  For TreeWalker, the children of this node
                     will also be rejected.  NodeIterators treat this
                     as a synonym for XMLDOM_FILTER_SKIP

             XMLERR_FILTER_SKIP
                     Skip this single node.  Navigation methods defined
                     for NodeIterator or TreeWalker will NOT return this
                     node.  For both NodeIterator and TreeWalker, the
                     children of this node will still be considered.

    RETURNS
        (xmlerr) filtering result

    ARGUMENTS
        xctx (IN) XML context
        node (IN) node to test
*/
#define XMLDOM_ACCEPT_NODE_F(func, xctx, node) \
    xmlerr func(xmlctx *xctx, xmlnode *node)

/****************************************************************************
                               Interface TreeWalker
 ****************************************************************************/

/*---------------------------------------------------------------------------
    NAME
        XmlDomWalkerGetRoot

    PURPOSE
        Return (get) root node

    DESCRIPTION
        Return (get) root node, or NULL on error.
        Since the current node can be removed from under the root node
        together with a subtree where it belongs to, the current root node
        in a walker might have no relation to the current node any more.
        The TreeWalker iterations are based on the current node. However,
        the root node defines the space of an iteration. This function
        checks if the root node is still in the root node (ancestor) relation 
        to the current node. If so, it returns this root node. Otherwise,
        it finds the root of the tree where the current node belongs to,
        and sets and returns this root as the root node of the walker.
        It returns NULL if the walker is a NULL pointer.

    RETURNS
        (xmlnode *) root node

    ARGUMENTS
        walker (IN)  TreeWalker object
        xerr   (OUT) numeric return value
*/
xmlnode* XmlDomWalkerGetRoot(xmlctx *xctx, xmlwalk *walker, xmlerr *xerr);

/*---------------------------------------------------------------------------
    NAME
        XmlDomWalkerSetRoot

    PURPOSE
        Set the root node

    DESCRIPTION
        Set the root node.
        Returns new root node if it is an ancestor of the current
        node.  If not it signals an error and checks if the cuurent root
        node is an ancestor of the current node.  If yes it returns it.
        Otherwise it sets the root node to and returns the root of
        the tree where the current node belongs to.  It returns NULL
        if the walker or the root node parameter is a NULL pointer.

    RETURNS
        (xmlnode *) new root node

    ARGUMENTS
        walker (IN)  TreeWalker object
        node   (IN)  new root node
        xerr   (OUT) numeric return code
*/
xmlnode* XmlDomWalkerSetRoot(xmlctx *xctx, xmlwalk *walker, xmlnode *node, 
                             xmlerr *xerr);

/*---------------------------------------------------------------------------
    NAME
        XmlDomWalkerGetCurrentNode

    PURPOSE
        Return (get) current node

    DESCRIPTION
        Return (get) current node, or NULL on error.

    RETURNS
        (xmlnode *) current node

    ARGUMENTS
        walker (IN)  TreeWalker object
        xerr   (OUT) numeric return code
*/
xmlnode* XmlDomWalkerGetCurrentNode(xmlctx *xctx, xmlwalk *walker, 
                                    xmlerr *xerr);

/*---------------------------------------------------------------------------
    NAME
        XmlDomWalkerSetCurrentNode

    PURPOSE
        Set current node

    DESCRIPTION
        Sets and returns new current node.  It also checks if the root node
        is an ancestor of the new current node.  If not it does not set the
        current node, returns NULL, and sets retval to
        XMLDOM_WALKER_BAD_NEW_CUR.  Returns NULL if an error.

    RETURNS
        (xmlnode *) new current node

    ARGUMENTS
        walker (IN)  TreeWalker object
        node   (IN)  new current node
        xerr   (OUT) numeric return code
*/
xmlnode* XmlDomWalkerSetCurrentNode(xmlctx *xctx, xmlwalk *walker, 
                                    xmlnode *node, xmlerr *xerr);

/*---------------------------------------------------------------------------
    NAME
        XmlDomWalkerFirstChild

    STANDARD
        DOM 2: TreeWalker.firstChild()

    PURPOSE
        Return first visible child of current node

    DESCRIPTION
        Moves the TreeWalker to the first visible child of the current
        node, and returns the new node.  If the current node has no
        visible children, returns NULL, and retains the current node.

    RETURNS
        (xmlnode *) first visible child [or NULL]

    ARGUMENTS
        xctx   (IN)  XML context
        walker (IN)  TreeWalker object
        xerr   (OUT) numeric return code

    SEE ALSO
        XmlDomWalkerLastChild
*/
xmlnode* XmlDomWalkerFirstChild(xmlctx *xctx, xmlwalk *walker, xmlerr *xerr);

/*---------------------------------------------------------------------------
    NAME
        XmlDomWalkerLastChild

    STANDARD
        DOM 2: TreeWalker.lastChild()

    PURPOSE
        Return last visible child of current node

    DESCRIPTION
        Moves the TreeWalker to the last visible child of the current
        node, and returns the new node.  If the current node has no
        visible children, returns NULL, and retains the current node.

    RETURNS
        (xmlnode *) last visible children [or NULL]

    ARGUMENTS
        xctx   (IN)  XML context
        walker (IN)  TreeWalker object
        xerr   (OUT) numeric return code
*/
xmlnode* XmlDomWalkerLastChild(xmlctx *xctx, xmlwalk *walker, xmlerr *xerr);

/*---------------------------------------------------------------------------
    NAME
        XmlDomWalkerNextNode

    STANDARD
        DOM 2: TreeWalker.nextNode()

    PURPOSE
        Return next visible node

    DESCRIPTION
        Moves the TreeWalker to the next visible node in document order
        relative to the current node, and returns the new node.  If the
        current node has no next node, or if the search for the next node
        attempts to step upward from the TreeWalker's root node, returns
        NULL, and retains the current node.

    RETURNS
        (xmlnode *) next node [or NULL]

    ARGUMENTS
        xctx   (IN)  XML context
        walker (IN)  TreeWalker object
        xerr   (OUT) numeric return code

    SEE ALSO
        XmlDomWalkerPrevNode, XmlDomWalkerNextSibling, XmlDomWalkerPrevSibling
*/
xmlnode* XmlDomWalkerNextNode(xmlctx *xctx, xmlwalk *walker, xmlerr *xerr);

/*---------------------------------------------------------------------------
    NAME
        XmlDomWalkerNextSibling

    STANDARD
        DOM 2: TreeWalker.nextSibling()

    PURPOSE
        Return next sibling node

    DESCRIPTION
        Moves the TreeWalker to the next sibling of the current node, and
        returns the new node.  If the current node has no visible next
        sibling, returns NULL, and retains the current node.

    RETURNS
        (xmlnode *) next sibling [or NULL]

    ARGUMENTS
        xctx   (IN)  XML context
        walker (IN)  TreeWalker object
        xerr   (OUT) numeric return code

    SEE ALSO
        XmlDomWalkerNextNode, XmlDomWalkerPrevNode, XmlDomWalkerPrevSibling
*/
xmlnode* XmlDomWalkerNextSibling(xmlctx *xctx, xmlwalk *walker, xmlerr *xerr);

/*---------------------------------------------------------------------------
    NAME
        XmlDomWalkerParentNode

    STANDARD
        DOM 2: TreeWalker.parentNode()

    PURPOSE
        Return parent node

    DESCRIPTION
        Moves to and returns the closest visible ancestor node of the 
        current node.  If the search for the parent node attempts to step
        upward from the TreeWalker's root node, or if it fails to find a
        visible ancestor node, this method retains the current position
        and returns null.

    RETURNS
        (xmlnode *) parent node [or NULL]

    ARGUMENTS
        xctx   (IN)  XML context
        walker (IN)  TreeWalker object
        xerr   (OUT) numeric return code
*/
xmlnode* XmlDomWalkerParentNode(xmlctx *xctx, xmlwalk *walker, xmlerr *xerr);

/*---------------------------------------------------------------------------
    NAME
        XmlDomWalkerPrevNode

    STANDARD
        DOM 2: TreeWalker.previousNode()

    PURPOSE
        Return previous node

    DESCRIPTION
        Moves the TreeWalker to the previous visible node in document order
        relative to the current node, and returns the new node.  If the
        current node has no previous node, or if the search for the
        previous node attempts to step upward from the TreeWalker's root
        node, returns NULL, and retains the current node.

    RETURNS
        (xmlnode *) previous node [or NULL]

    ARGUMENTS
        xctx   (IN)  XML context
        walker (IN)  TreeWalker object
        xerr   (OUT) numeric return code

    SEE ALSO
        XmlDomWalkerNextNode, XmlDomWalkerNextSibling, XmlDomWalkerPrevSibling
*/
xmlnode* XmlDomWalkerPrevNode(xmlctx *xctx, xmlwalk *walker, xmlerr *xerr);

/*---------------------------------------------------------------------------
    NAME
        XmlDomWalkerPrevSibling

    STANDARD
        DOM 2: TreeWalker.previousSibling()

    PURPOSE
        Return previous sibling node

    DESCRIPTION
        Moves the TreeWalker to the previous sibling of the current node,
        and returns the new node.  If the current node has no visible
        previous sibling, returns NULL, and retains the current node.

    RETURNS
        (xmlnode *) previous sibling [or NULL]

    ARGUMENTS
        xctx   (IN)  XML context
        walker (IN)  TreeWalker object
        xerr   (OUT) numeric return code

    SEE ALSO
        XmlDomWalkerNextNode, XmlDomWalkerPrevNode, XmlDomWalkerNextSibling
*/
xmlnode* XmlDomWalkerPrevSibling(xmlctx *xctx, xmlwalk *walker, xmlerr *xerr);

/****************************************************************************
                           Interface DocumentTraversal
 ****************************************************************************/

/*---------------------------------------------------------------------------
    NAME
        XmlDomCreateNodeIter

    STANDARD
        DOM 2: DocumentTraversal.createNodeIterator()

    PURPOSE
        Create node iterator object

    DESCRIPTION
        One of two methods of DocumentTraversal interface, used to
        create a NodeIterator object.  This method is identical to
        XmlDomCreateTreeWalker except for the type of object returned.

        The whatToShow argument is a mask of flag bits, one for each
        node type.  The value XMLDOM_SHOW_ALL passes all node types
        through, otherwise only the types whose bits are set will be
        passed.

        Entity reference expansion is controlled by the entrefExpansion
        flag.  If TRUE, entity references are replaced with their final
        content; if FALSE, entity references are left as nodes.

    RETURNS
        (xmliter *) original or new NodeIterator object

    ARGUMENTS
        xctx         (IN) XML context
        iter         (IN) existing NodeIterator to set [NULL to create]
        root         (IN) root node to be iterated
        whatToShow   (IN) mask of XMLDOM_SHOW_xxx flag bits
        nodeFilter   (IN) node filter to be used [NULL if none]
        entrefExpand (IN) expand entity reference nodes?

    SEE ALSO
        XmlDomCreateTreeWalker
*/
xmliter* XmlDomCreateNodeIter(xmlctx *xctx, xmliter *iter, xmlnode *root,
                              xmlshowbits whatToShow, 
                              XMLDOM_ACCEPT_NODE_F((*nodeFilter), xctx, node),
                              boolean entrefExpand);

/*---------------------------------------------------------------------------
    NAME
        XmlDomCreateTreeWalker

    STANDARD
        DOM 2: DocumentTraversal.createTreeWalker()

    PURPOSE
        Create tree walker object

    DESCRIPTION
        One of two methods of DocumentTraversal interface, used to
        create a TreeWalker object.  This method is identical to
        XmlDomCreateNodeIter except for the type of object returned.

        The {whatToShow} argument is a mask of flag bits, one for each
        node type.  The value XMLDOM_SHOW_ALL passes all node types
        through, otherwise only the types whose bits are set will be
        passed.

        Entity reference expansion is controlled by the {entrefExpansion}
        flag.  If TRUE, entity references are replaced with their final
        content; if FALSE, entity references are left as nodes.

    RETURNS
        (xmlwalk *) new TreeWalker object

    ARGUMENTS
        xctx         (IN) XML context
        walker       (IN) existing TreeWalker to set [NULL to create]
        root         (IN) root node which for TreeWalker
        whatToShow   (IN) mask of XMLDOM_SHOW_xxx flag bits
        nodeFilter   (IN) node filter to be used [NULL if none]
        entrefExpand (IN) expand entity reference nodes?

    SEE ALSO
        XmlDomCreateNodeIter
*/
xmlwalk* XmlDomCreateTreeWalker(xmlctx *xctx, xmlwalk* walker, xmlnode *root,
                                xmlshowbits whatToShow,
                               XMLDOM_ACCEPT_NODE_F((*nodeFilter), xctx, node),
                                boolean entrefExpansion);

/*---------------------------------------------------------------------------
                      Package Range - DOM Range APIs
  ---------------------------------------------------------------------------*/

/****************************************************************************
                               Interface Range
 ****************************************************************************/

/*
   RANGE:

   All functions defined in the Range specification are implemented
   here and have exactly the same semantics as described in W3C DOM
   Level 2 Spec.  In addition to the specified functions this
   implementation adds two more functions, XmlDomRangeGetDetached and
   XmlDomRangeIsConsistent.  These functions are Oracle extensions.
   If the document is modified by an API different than the Range API,
   it is assumed, in this case, that the consistency of the range is
   maintained by using appropriate SET, SELECT, and COLLAPSE functions
   from the Range API.
*/

/*---------------------------------------------------------------------------
    NAME
        XmlDomRangeGetStartContainer

    STANDARD
        DOM 2: Range.startContainer

    PURPOSE
        Return range start container node

    DESCRIPTION
        Returns range start container node if the range is valid and
        is not detached, otherwise returns NULL.

    RETURNS
        (xmlnode *) range start container node

    ARGUMENTS
        xctx  (IN)  XML context
        range (IN)  range object
        xerr  (OUT) numeric return code
*/
xmlnode* XmlDomRangeGetStartContainer(xmlctx *xctx, xmlrange *range, 
                                      xmlerr *xerr);

/*---------------------------------------------------------------------------
    NAME
        XmlDomRangeGetStartOffset

    STANDARD
        DOM 2: Range.startOffset

    PURPOSE
        Return range start offset

    DESCRIPTION
        Returns range start offset if the range is not detached, otherwise
        returns {~(ub4)0} [the maximum {ub4} value].

    RETURNS
        (ub4) range start offset [or {ub4} maximum]

    ARGUMENTS
        xctx  (IN)  XML context
        range (IN)  range object
        xerr  (OUT) numeric return code
*/
ub4 XmlDomRangeGetStartOffset(xmlctx *xctx, xmlrange *range, xmlerr *xerr);

/*---------------------------------------------------------------------------
    NAME
        XmlDomRangeGetEndContainer

    STANDARD
        DOM 2: Range.endContainer

    PURPOSE
        Return range end container node

    DESCRIPTION
        Returns range end container node if the range is not detached,
        otherwise returns NULL.

    RETURNS
        (xmlnode *) range end container node [or NULL]

    ARGUMENTS
        xctx  (IN)  XML context
        range (IN)  range object
        xerr  (OUT) numeric return code
*/
xmlnode* XmlDomRangeGetEndContainer(xmlctx *xctx, xmlrange *range, 
                                    xmlerr *xerr);

/*---------------------------------------------------------------------------
    NAME
        XmlDomRangeGetEndOffset

    STANDARD
        DOM 2: Range.endOffset

    PURPOSE
        Return range end offset

    DESCRIPTION
        Returns range end offset if the range is not detached, otherwise
        returns {~(ub4)0} [the maximum {ub4} value].

    RETURNS
        (ub4) range end offset [or {ub4} maximum]

    ARGUMENTS
        xctx  (IN)  XML context
        range (IN)  range object
        xerr  (OUT) numeric return code
*/
ub4 XmlDomRangeGetEndOffset(xmlctx *xctx, xmlrange *range, xmlerr *xerr);

/*---------------------------------------------------------------------------
    NAME
        XmlDomRangeGetCollapsed

    STANDARD
        DOM 2: Range.collapsed

    PURPOSE
        Return whether the range is collapsed

    DESCRIPTION
        Returns TRUE if the range is collapsed and is not detached,
        otherwise returns FALSE.

    RETURNS
        (boolean) TRUE if the range is collapsed, FALSE otherwise

    ARGUMENTS
        xctx  (IN)  XML context
        range (IN)  range object
        xerr  (OUT) numeric return code
*/
boolean XmlDomRangeGetCollapsed(xmlctx *xctx, xmlrange *range, xmlerr *xerr);

/*---------------------------------------------------------------------------
    NAME
        XmlDomRangeGetCommonAncestor

    PURPOSE
        Return deepest common ancestor node of two boundary points

    DESCRIPTION
        Returns deepest common ancestor node of two boundary points
        of the range if the range is not detached, otherwise returns
        NULL.  It is assumed that the range is in a consistent state.

    RETURNS
        (xmlnode *) deepest common ancestor node [or NULL]

    ARGUMENTS
        xctx  (IN)  XML context
        range (IN)  range object
        xerr  (OUT) numeric return code
*/
xmlnode* XmlDomRangeGetCommonAncestor(xmlctx *xctx, xmlrange *range, 
                                      xmlerr *xerr);

/*---------------------------------------------------------------------------
    NAME
        XmlDomRangeSetStart

    STANDARD
        DOM 2: Range.setStart()

    PURPOSE
        Set the start point

    DESCRIPTION
        Sets the range start point.  If it has a root container other
        than the current one for the range, the range is collapsed
        to the new position. If the start is set to be at a position
        after the end, the range is collapsed to that position.  Returns
        xmlerr value according to the description above where this type
        is defined.  It is assumed that the end point of the range is a
        valid end point.

    RETURNS
        (xmlerr) numeric return code

    ARGUMENTS
        xctx   (IN) XML context
        range  (IN) range object
        node   (IN) XML node
        offset (IN) starting offset
*/
xmlerr XmlDomRangeSetStart(xmlctx *xctx, xmlrange *range, xmlnode *node, 
                           ub4 offset);

/*---------------------------------------------------------------------------
    NAME
        XmlDomRangeSetEnd

    STANDARD
        DOM 2: Range.setEnd()

    PURPOSE
        Set the end point

    DESCRIPTION
        Sets the range end point. If it has a root container other
        than the current one for the range, the range is collapsed
        to the new position.  If the end is set to be at a position
        before the start, the range is collapsed to that position.
        Returns xmlerr value according to the description above
        where this type is defined.  It is assumed that the start
        point of the range is a valid start point.

    RETURNS
        (xmlerr) numeric return code

    ARGUMENTS
        xctx   (IN) XML context
        range  (IN) range object
        node   (IN) node node
        offset (IN) ending offset
*/
xmlerr XmlDomRangeSetEnd(xmlctx *xctx, xmlrange *range, xmlnode *node, 
                         ub4 offset);

/*---------------------------------------------------------------------------
    NAME
        XmlDomRangeSetStartBefore

    STANDARD
        DOM 2: Range.setStartBefore()

    PURPOSE
        Set the start point before a node

    DESCRIPTION
        Sets the range start point before a node.  If it has a root
        container other than the current one for the range, the range
        is collapsed to the new position with offset == 0.  If the
        before node sets the start to be at a position after the end,
        the range is collapsed to new position.  Returns xmlerr value
        according to the description above where this type is defined.
        It is assumed that the end point of the range is a valid end point.

    RETURNS
        (xmlerr) numeric return code

    ARGUMENTS
        xctx  (IN) XML context
        range (IN) range object
        node  (IN) XML node
*/
xmlerr XmlDomRangeSetStartBefore(xmlctx *xctx, xmlrange *range, xmlnode *node);

/*---------------------------------------------------------------------------
    NAME
        XmlDomRangeSetStartAfter

    STANDARD
        DOM 2: Range.setStartAfter()

    PURPOSE
        Set the start point after a node

    DESCRIPTION
        Sets the range start point after a node. If it has a root
        container other than the current one for the range, the range
        is collapsed to the new position.  If the after node sets the
        start to be at a position after the end, the range is collapsed
        to new position.  Returns xmlerr value according to the
        description above where this type is defined.  It is assumed
        that the end point of the range is a valid end point.

    RETURNS
        (xmlerr) numeric return code

    ARGUMENTS
        xctx  (IN) XML context
        range (IN) range object
        node  (IN) XML node
*/
xmlerr XmlDomRangeSetStartAfter(xmlctx *xctx, xmlrange *range, xmlnode *node);

/*---------------------------------------------------------------------------
    NAME
        XmlDomRangeSetEndBefore

    STANDARD
        DOM 2: Range.setEndBefore()

    PURPOSE
        Set the end point before a node

    DESCRIPTION
        Sets the range end point before a node. If it has a root
        container other than the current one for the range, the range
        is collapsed to the new position.  If the before node sets the
        end to be at a position before the start, the range is collapsed
        to new position.  Returns xmlerr value according to the
        description above where this type is defined.  It is assumed that
        the start point of the range is a valid start point.

    RETURNS
        (xmlerr) numeric return code

    ARGUMENTS
        xctx  (IN) XML context
        range (IN) range object
        node  (IN) XML node
*/
xmlerr XmlDomRangeSetEndBefore(xmlctx *xctx, xmlrange *range, xmlnode *node);
    
/*---------------------------------------------------------------------------
    NAME
        XmlDomRangeSetEndAfter

    STANDARD
        DOM 2: Range.setEndAfter()

    PURPOSE
        Set the end point after a node

    DESCRIPTION
        Sets the range end point after a node. If it has a root
        container other than the current one for the range, the range
        is collapsed to the new position.  If the after node sets the
        end to be at a position before the start, the range is collapsed
        to new position.  Returns xmlerr value according to the
        description above where this type is defined.  It is assumed
        that the start point of the range is a valid start point.

    RETURNS
        (xmlerr) numeric return code

    ARGUMENTS
        xctx  (IN) XML context
        range (IN) range object
        node  (IN) XML node
*/
xmlerr XmlDomRangeSetEndAfter(xmlctx *xctx, xmlrange *range, xmlnode *node);

/*---------------------------------------------------------------------------
    NAME
        XmlDomRangeCollapse

    STANDARD
        DOM 2: Range.collapse()

    PURPOSE
        Collapse range to either start point or end point

    DESCRIPTION
        Collapses the range to either start point or end point.
        The point where it is collapsed to is assumed to be a valid
        point in the document which this range is attached to.

    RETURNS
        (xmlerr) numeric return code

    ARGUMENTS
        xctx    (IN) XML context
        range   (IN) range object
        tostart (IN) flag indicating whether collapse is to start or end
*/
xmlerr XmlDomRangeCollapse(xmlctx *xctx, xmlrange *range, boolean tostart);

/*---------------------------------------------------------------------------
    NAME
        XmlDomRangeSelectNode

    STANDARD
        DOM 2: Range.selectNode()

    PURPOSE
        Select a node as a range

    DESCRIPTION
        Sets the range end point and start point so that the parent
        node of this node becomes the container node, and the offset is
        the offset of this node among the children of its parent.  The
        range becomes collapsed.  It is assumed that the node is a valid
        node of its document.  If the range is detached, it is ignored,
        and the range becomes attached.

    RETURNS
        (xmlerr) numeric return code

    ARGUMENTS
        xctx  (IN) XML context
        range (IN) range object
        node  (IN) XML node
*/
xmlerr XmlDomRangeSelectNode(xmlctx *xctx, xmlrange *range, xmlnode *node);

/*---------------------------------------------------------------------------
    NAME
        XmlDomRangeSelectNodeContents

    STANDARD
        DOM 2: Range.selectNodeContents()

    PURPOSE
        Define range to select node contents

    DESCRIPTION
        Sets the range start point to the start of the node contents and
        the end point to the end of the node contents.  It is assumed that
        the node is a valid document node.  If the range is detached, it is
        ignored, and the range becomes attached.

    RETURNS
        (xmlerr) numeric return code

    ARGUMENTS
        xctx  (IN) XML context
        range (IN) range object
        node  (IN) XML node
*/
xmlerr XmlDomRangeSelectNodeContents(xmlctx *xctx, xmlrange *range, 
                                     xmlnode *node);

/*---------------------------------------------------------------------------
    NAME
        XmlDomRangeCompareBoundaryPoints

    STANDARD
        DOM 2: Range.compareBoundaryPoints()

    PURPOSE
        Compare boundary points of two ranges

    DESCRIPTION
        Compares two boundary points of two different ranges.  Returns
        -1,0,1 depending on whether the corresponding boundary point of
        the range (range) is before, equal, or after the corresponding
        boundary point of the second range ({srange}).  It returns {~(int)0}
        if two ranges are attached to two different documents or if one
        of them is detached.

    RETURNS
        (sb4) strcmp-like comparison result

    ARGUMENTS
        xctx   (IN)  XML context
        range  (IN)  range object
        how    (IN)  xmlcmphow value [how to compare]
        srange (IN)  range object to be compared with
        xerr   (OUT) numeric return code
*/
sb4 XmlDomRangeCompareBoundaryPoints(xmlctx *xctx, xmlrange *range, 
                                     xmlcmphow how, xmlrange *srange, 
                                     xmlerr *xerr);

/*---------------------------------------------------------------------------
    NAME
        XmlDomRangeDeleteContents

    STANDARD
        DOM 2: Range.deleteContents()

    PURPOSE
        Delete content selected by a range

    DESCRIPTION
        Delete content selected by a range.  Deletes content selected by
        a range.  Performs the range consistency check and sets retval
        to an error code if an error.

    RETURNS
        (xmlerr) numeric return code

    ARGUMENTS
        xctx  (IN)  XML context
        range (IN)  range object
*/
xmlerr XmlDomRangeDeleteContents(xmlctx *xctx, xmlrange *range);

/*---------------------------------------------------------------------------
    NAME
        XmlDomRangeExtractContents

    STANDARD
        DOM 2: Range.extractContents()

    PURPOSE
        Extract contents selected by a range

    DESCRIPTION
        Extract contents selected by a range.  Clones and deletes contents
        selected by a range.  Performs the range consistency check and sets
        retval to an error code if an error.

    RETURNS
        (xmlnode *) extracted

    ARGUMENTS
        xctx  (IN)  XML context
        range (IN)  range object
        xerr  (OUT) numeric return code
*/
xmlnode* XmlDomRangeExtractContents(xmlctx *xctx, xmlrange *range,
                                    xmlerr *xerr);

/*---------------------------------------------------------------------------
    NAME
        XmlDomRangeCloneContents

    STANDARD
        DOM 2: Range.cloneContents()

    PURPOSE
        Clone contents selected by a range

    DESCRIPTION
        Clone contents selected by a range.  Clones but does not delete
        contents selected by a range.  Performs the range consistency
        check and sets retval to an error code if an error.

    RETURNS
        (xmlnode *) cloned contents

    ARGUMENTS
        xctx  (IN)  XML context
        range (IN)  range object
        xerr  (OUT) numeric return code
*/
xmlnode* XmlDomRangeCloneContents(xmlctx *xctx, xmlrange *range, xmlerr *xerr);

/*---------------------------------------------------------------------------
    NAME
        XmlDomRangeClone

    PURPOSE
        Clone a range

    STANARD
        DOM 2: Range.cloneRange()

    DESCRIPTION
        Clone a range.  Clones the range without affecting the content
        selected by the original range.  Returns NULL if an error.

    RETURNS
        (xmlrange *) new range that clones the old one

    ARGUMENTS
        xctx  (IN)  XML context
        range (IN)  range object
        xerr  (OUT) numeric return code
*/
xmlrange* XmlDomRangeClone(xmlctx *xctx, xmlrange *range, xmlerr *xerr);

/*---------------------------------------------------------------------------
    NAME
        XmlDomRangeDetach

    STANDARD
        DOM 2: Range.detach()

    PURPOSE
        Detach a range

    DESCRIPTION
        Detaches the range from the document and places it (range)
        in invalid state.

    RETURNS
        (xmlerr) numeric return code

    ARGUMENTS
        xctx  (IN) XML context
        range (IN) range object
*/
xmlerr XmlDomRangeDetach(xmlctx *xctx, xmlrange *range);

/*---------------------------------------------------------------------------
    NAME
        XmlDomRangeGetDetached

    PURPOSE
        Return whether the range is detached

    DESCRIPTION
        Return whether the range is detached.  Returns TRUE if the range
        is detached and is not NULL.  Otherwise returns FALSE.

    RETURNS
        (ub1) TRUE if the range is detached, FALSE otherwise

    ARGUMENTS
        xctx  (IN) XML context
        range (IN) range object
*/
ub1 XmlDomRangeGetDetached(xmlctx *xctx, xmlrange *range);

/*---------------------------------------------------------------------------
    NAME
        XmlDomRangeIsConsistent

    PURPOSE
        Return whether the range is consistent

    DESCRIPTION
        Return whether the range is consistent.  Returns TRUE if the
        range is consistent: both points are under the same root and
        the start point is before or equal to the end point.  Otherwise
        returns FALSE.

    RETURNS
        (ub1) TRUE if the range is consistent, FALSE otherwise

    ARGUMENTS
        xctx  (IN)  XML context
        range (IN)  range object
        xerr  (OUT) numeric return code
*/
boolean XmlDomRangeIsConsistent(xmlctx *xctx, xmlrange *range, xmlerr *xerr);

/****************************************************************************
                               Interface DocumentRange
 ****************************************************************************/

/*---------------------------------------------------------------------------
    NAME
        XmlDomCreateRange

    STANDARD
        DOM 2: DocumentRange.createRange()

    PURPOSE
        Create Range object

    DESCRIPTION
        The only one method of DocumentRange interface, used to create
        a Range object. 

    RETURNS
        (xmlrange *) original or new Range object.

    ARGUMENTS
        xctx  (IN) XML context
        range (IN) existing NodeIterator or NULL to allocate new.
                   If a new object is allocated the caller must free it using
                   the memory free function associated with xctx.
        doc   (IN) the document which new (set) range is attached to.
*/
xmlrange* XmlDomCreateRange(xmlctx *xctx, xmlrange *range, xmldocnode *doc);


/*---------------------------------------------------------------------------
    NAME
        XmlSetWhiteList

    PURPOSE
        Set a new white list for the xmlctx

    DESCRIPTION
        Destroy any previous white list and set a new one for this
        xmlctx. Similar to the white list information passed to
        XmlCreateNew.

    RETURNS
        (xmlerr) error code from the operation

    ARGUMENTS
        xctx      (IN)  xml context to change
        wlist     (IN)  whitelist
        wcount    (IN)  # of strings in the whitelist
        wencoding (IN)  encoding of the whitelist (default UTF-8)
        ...       (IN)  variable arguments, with final NULL

    SEE ALSO
        XmlCreateNew

*/
xmlerr XmlSetWhiteList(xmlctx *xctx, oratext **wlist,
                       ub4 wcount, oratext *wencoding);

#endif /* XMLPROC_ORACLE */
