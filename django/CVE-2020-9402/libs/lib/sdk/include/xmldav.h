/* $RCSfile: xmldav.h $ $Date: 2016/08/09 13:29:38 $
 Copyright (c) 2001, 2016, Oracle and/or its affiliates. All rights reserved.

  #####################################################################
  # !!!!WARNING!!!!   THIS HEADER GENERATES DOCUMENTATION DIRECTLY!   #
  # The syntax of Package/Interface/function block, etc is specific & #
  # must be maintained properly or documentation will not generate!!  #
  #####################################################################

   FILE NAME
        xmldav.h - Oracle WebDAV Client APIs

   FILE DESCRIPTION
        This header file implements the WebDAV client interface.  This
        front end can be used to access or author remote resources via
        HTTP 1.1 requests.

        Unlike HTTP/1.1, WebDAV encodes method parameter information 
        either in an Extensible Markup Language (XML) [REC-XML] request 
        entity body, or in an HTTP header.  

        This implementation is built on top of XDK's XML parser. 
        To begin with, user needs to create an xmlctx to be used in
        this interface.  This meta context (i.e., xmlctx) contains encoding
        information, low-level registered memory callbacks, error message
        language/encoding and optional handler function, etc. This context
        is constantly referenced in all DAV client APIs directly or 
        indirectly.
        
        User can choose the data_encoding for the new xmlctx.  This is the 
        encoding that any data to be inserted into an XML document or to be 
        retrieved from an XML DOM tree will be in.  Note that an entity body 
        of xml type can be retrieved as an XML DOM tree by calling 
        XmlDavRespGetXmlDoc.  This encoding will be referenced as DAV's XML 
        encoding in this document.

        This implementation always uses "UTF-8" as the encodings of the wire
        data and IRI's.  So, for best performance, it is recommended that 
        "UTF-8" is used as the data encoding of the provided xmlctx (i.e.,
        DAV's data encoding).  Note that although data encoding is associated
        with an xmlctx, its application is not limited to xml type data only.
        A better name for xmlctx is probably oractx. But, before things are
        straighten out, xmlctx is here to stay.

        Next, a DAV context (i.e., xmlDavCtx) needs to be created using 
        an xml context (i.e., xmlctx). This DAV context will be used in all 
        WebDAV client APIs directly or indirectly.  Data structures such 
        as xmlDavReqDesc, xmlDavResDesc, xmlDavResponse, etc. all belong
        to a single xmlDavCtx that was used to create them.  If XmlDavDestroy
        is invoked on it, the given xmlDavCtx and all its subordinate objects
        will be deleted if not yet so.  All xmlDavResponse's belong to a 
        xmlDavResDesc that was used to create them when the resource-related
        methods were invoked.  If XmlDavDestroy is invoked on it, the given
        xmlDavResDesc and all its subordinate objects will be deleted if not
        yet so.

        The life span of each object depends on its ancestor's.  The lineage 
        of each object defined in this API is shown as follows:

            xmlctx
               |
               +---xmldocnode (from XmlCreateDocument)
               |       |
               |       +---xmlelemnode
               |
               +---xmlDavCtx
               |       |
               |       +---xmlDavIri
               |       |
               |       +---xmlDavCond
               |       |
               |       +---xmlDavCondFac
               |       |
               |       +---xmlDavCondTerm
               |       |
               |       +---xmlDavHdrDesc
               |       |
               |       +---xmlDavHostDesc
               |       |
               |       +---xmlDavHttpReq
               |       |
               |       +---xmlDavList
               |       |
               |       +---xmlDavPropDesc
               |       |
               |       +---xmlDavPropGrp
               |       |
               |       +---xmlDavReqDesc
               |       |
               |       +---xmlDavResDesc
               |       |       |
               |       |       +---xmlDavResponse
               |       |            |
               |       |            +---xmlDavXmlDoc
               |       |               (from XmlDavRespGetXmlDoc)
               |       +---xmlDavUri
               |       |
               |       +---xmlDavXmlDoc (from XmlDavCreateXmlDoc)
               |
               +---xmlDavCtx
               |      .
               |      .
               |      .

        The data type of "owner" attribute or property value is ANY which
        means it can be anything encoded in well-formed XML elements.  
        That's why we take a data of (xmlelemnode *) type as their values.
        When XML elements need to be constructed, an XML context and a 
        Document object are needed.  The XML context used must match the
        one stored in the DAV context which is referenced in methods that 
        consume these attributes or property values.  Application can 
        create a Document object using XmlCreateDocument.  No root element
        or dtd object need to be provided when it is created.  This Document
        object can be used as a container for holding any xml element nodes
        to be created in it.
        
        New elements can be created using XmlDomCreateElemNS which requires
        a namespace URI and qname.  If the new element exists in the 
        "DAV:" namespace, the recommended prefix used in the qname
        specification is "D:".  This is because a new Document Object that
        represents the entity body in XML will be created when the 
        resource-related APIs are invoked.  The new Document object 
        will be constructed using attribute name/value pairs provided 
        by the user.  Which means that the XML element representing
        "owner" or property value need to be imported into this Document
        object.  To be consistent in the prefix using, we therefore 
        recommend user choose "D:" prefix in creating their qnames if they
        exist in the "DAV:" namespace.

        On EBCDIC platforms, compiler encoding is in EBCDIC and UTF-E is used
        instead of UTF-8 at interface.  However, internally, it still converts
        EBCDIC to ASCII for header data and UTF-E to UTF-8 for XML payloads.
        
        A lock token is a type of state token, represented as a URI, which 
        identifies a particular lock.  Each lock has only one unique lock
        token.  Lock support in server is optional.  A client can determine
        what lock types the server supports by retrieving the "supportedlock"
        property.  Locks are associated with a principal, not a program.  If
        another principal locks a resource that a principal wishes to access,
        the second principal can find out whom the first principal is by 
        retrieving "lockdiscovery" property.  Note that it is illegal for a 
        principal to request the same lock twice. 

        WebDAV defines the protocol between a client and a server.  This
        set of APIs is defined for client side only.  Functions within this
        implementation are organized into two categories:

        1) Request related
           
           Functions are further divided into three groups: top level,
           resource related, and condition related.  This set of APIs 
           takes method parameters from the user and composes an HTTP request
           for the user.  Some method parameters become HTTP headers and 
           some of them are composed into XML elements and stored as entity
           body. This set of APIs aims to provide a user-friendly interface 
           for ease of use.

        2) Response related

           To achieve maximum flexibility and support for extensibility, this
           set of APIs allows the user examine the response at the HTTP level.
           For example, status line can be examined by XmlDavRespGetCode and 
           XmlDavRespGetReason; header fields can be viewed by 
           XmlDavRespGetHdrNam and XmlDavRespGetHdrVal; XML contents 
           can be retrieved by XmlDavRespGetXmlDoc and traversed by XDK DOM 
           API.  The caveat of providing this flexibility is the client 
           application has to be aware of all headers that can be returned by
           a method and of all XML elements can appear in an entity body.
          
        Because some interoperability issues were not covered or covered
        correctly in the original RFC 2518, a new "RFC 2518 bis" has being
        working on by the IETF.  The differences between RFC 2518 and 
        RFC 2518 bis is deemed significant.  Therefore, this API 
        specification tries to use the latest "RFC 2518 bis" as guideline. 
        The latest known version that is referenced by this implementation
        is:
                  http://greenbytes.de/tech/webdav/
                  draft-ietf-webdav-rfc2518bis-05.txt


        There is a one-to-one mapping between some of the methods provided 
        in this API and methods defined in either HTTP 1.1 or WebDAV.  The 
        mapping is presented here:

            XmlDavResCopy     <------>    COPY      method
            XmlDavResDelete   <------>    DELETE    method
            XmlDavResGetProp  <------>    PROPFIND  method
            XmlDavResLock     <------>    LOCK      method
            XmlDavResMkCol    <------>    MKCOL     method
            XmlDavResMove     <------>    MOVE      method
            XmlDavResOptions  <------>    OPTIONS   method
            XmlDavResPut      <------>    PUT       method
            XmlDavResUnlock   <------>    UNLOCK    method
            XmlDavResUpdProp  <------>    PROPPATCH method

        There is one-to-one mapping between some of the attributes provided
        in this API and header fields defined in HTTP 1.1 and its extensions.
        If there is such connection.  The attribute name is chosen to be the
        same as the header name defined in HTTP 1.1 and its extensions.  To 
        distinguish attributes that have such connection from the ones that
        don't have, we use this convention:
            1. Attributes mapped to HTTP or its extensions
               The first letter of each word is capitalized and "-" is used
               to connect words.  Example: "Transfer-Encoding", "Connection",
               etc.
            2. Attributes not mapped to HTTP or its extensions
               The first letter of each word is NOT capitalized and "_" is
               used to connect words.  Example: "io_buffer", "max_xml_size",
               etc.

        Most routines in this API have a variable-length argument list. NULL
        is used to indicate the end of argument list. Duplicate attribute
        name/value pairs are allowed on the argument list.  But, only the
        setting from the last specification is taken.

        For data provided to this API, their pointers are saved and their
        values are NOT duplicated.  Therefore, data either belong to
        user's application or this implementation.  As long as DAV objects 
        remain active, data they point to must remain intact.  For data
        retrieved from DAV objects, users need to duplicate a copy of them if
        they want to keep them longer than DAV objects' life span.

        This implementation is part of Oracore and XDK packages.  These
        components are tightly integrated.  For example, to compose/simplify/
        split an IRI or URI, LPU component can be used.  To traverse the
        XML DOM tree returned by XmlDavRespGetXmlDoc, XDK's DOM API can be
        utilized.  Since WebDAV is an extension of HTTP 1.1, a new HTTP 
        package is to be implemented and this package will be integrated with
        it too.  To deal with various charsets and encodings, NLS library
        will be utilized in this implementation.  Same can be done by any 
        client applications. 

        Example
            xmlctx             *xctx;
            xmlDavCtx          *dctx1, *dctx2;
            xmlerr              xerr;
            xmlDavUri          *uri4SmallCol;
            xmlDavIri          *iri4LargeCol;
            xmlDavResDesc      *smallDir;
            xmlDavResDesc      *largeDir;
            xmlDavResponse     *sResp;
            xmlDavXmlDoc       *ddoc;
            xmldocnode         *sdoc;
            ...
            // Create a top-level meta context to be used in all DAV 
            // operations.  In the example, DAV context 1 and 2 share
            // the same meta context.  They can also have different meta
            // contexts if the user chooses.  The meta context allows the
            // user to set up data encoding, error handler, memory handler,
            // etc. See XmlCreate for details.
            xctx = XmlCreate(&xerr, (oratext *) "myApplication",
                             "data_encoding", "utf-8", NULL);
            ...
            // Create two DAV contexts to deal with two collection resources:
            //   dctx1 is used for a collection with small set of internal
            //         members.
            //   dctx2 is used for a collection with large set of internal 
            //         members.
            dctx1 = XmlDavCreate(xctx, &xerr, "max_xml_size", 10, NULL);
            dctx2 = XmlDavCreate(xctx, &xerr, "max_xml_size", 100, NULL);
            ...
            // Create two resource descriptors for accessing the collection
            smallDir = XmlDavCreateResDesc(dctx1, &xerr, "uri", uri4SmallCol,
                                           NULL);
            largeDir = XmlDavCreateResDesc(dctx2, &xerr, "iri", iri4LargeCol,
                                           NULL);
            ...
            // Create two request descriptors to provide extra information 
            // needed for method invocation.
            sreq = XmlDavCreateReqDesc(dctx1, &xerr, 
                                       "Content-Language", "en", 
                                       "Authorization", 
                                       "Basic QWxhZGRpbjpvcGVuIHNlc2FtZQ=="
                                        NULL);
            lreq = XmlDavCreateReqDesc(dctx2, &xerr, 
                                       "Content-Language", "en", 
                                        NULL);
            ...
            // Set request descriptor in the resource descriptor
            XmlDavUpdResDesc(smallDir, "request_descriptor", 
                                       sreq, NULL);
            XmlDavUpdResDesc(largeDir, "request_descriptor", 
                                       lreq, NULL);
            ...
            // Invoke resource-related operations
            if (!XmlDavResGetProp(smallDir, "allprop", TRUE,
                                            "Depth", XMLDAV_DEPTH_INF,
                                            "response", &sResp))
            {
                if (sResp)   // sResp can be NULL
                {
                    xmlctx      xctx;

                    statusCode = XmlDavRespGetCode(sResp, &xerr);
                    switch (statusCode)
                    {
                    case 401:  // Unauthorized
                        break;
                    case 403:  // Infinite depth forbidden (see 
                                         // RFC 2518 bis)
                        break;
                    case 404:  // Not found
                        break;
                    case 207:  // Multi-status
                        ddoc = XmlDavRespGetXmlDoc(sResp, &xerr);
                        sdoc = XmlDavDocGetNode(ddoc, &xerr);
                        xctx = XmlDavDocGetXmlCtx(ddoc, &xerr);
                        if (sdoc)
                        {
                            xmlelemnode *elem;
                            xmlnode     *sub;
                            ...
                            // Use the meta context retrieved from DAV's
                            // document here.  Although, in this case, it's
                            // same as the one used to create DAV's context.
                            elem=XmlDomGetDocElem(xctx, sdoc);
                            ...
                            // Can also use DOM 2 Range or DOM 2 Traversal 
                            // implementation to traverse the DOM tree.
                            sub = XmlDomGetFirstChild(xctx, (xmlnode *)elem);
                            do {
                                name = XmlDomGetNodeName(xctx, sub);
                                ...
                            } while (sub = XmlDomGetNextSibling(xctx, sub));
                        }
                        break;
                    default:
                        break;
                    }
                }
            }
            ...
            ...
            // Destroy DAV context and all its subordinate objects (i.e., 
            // xmlDavResDesc, xmlDavReqDesc, xmlDavResponse, xmlDavXmlDoc,
            // etc.).
            XmlDavDestroy(&xerr, "dav_context", dctx1, NULL);
            XmlDavDestroy(&xerr, "dav_context", dctx2, NULL);
            ...
            // User can continue using its meta context for other XDK
            // supported operations.
            ...
            // All XDK-related operations are done.
            XmlDestroy(xctx);
*/

#ifndef XMLDAV_ORACLE
# define XMLDAV_ORACLE

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

#ifndef XML_ORACLE
# include <xml.h>
#endif


 

/*---------------------------------------------------------------------------
                         PUBLIC TYPES AND CONSTANTS
  ---------------------------------------------------------------------------*/

#define XMLDAV_MAX_HOST       256
#define XMLDAV_MAX_PORT        16 

/* DATATYPE xmlDavAce - Access Control Element
   SEE ALSO XmlDavAclCreateAce, XmlDavDestroy
   This structure defines an access control element (ACE).  An ACE either 
   grants or denies a particular set of (non-abstract) privileges for a 
   particular principal.
*/
struct xmlDavAce; 
typedef struct xmlDavAce xmlDavAce;

/* DATATYPE xmlDavCtx - webDAV ConTeXt
   SEE ALSO XmlDavCreate, XmlDavDestroy
   This structure provides global information such as data encoding, i/o
   buffer specification, xml size limitation, etc. to be used in all methods
   invoked using it.  All DAV objects (e.g., xmlDavResDesc, xmlDavReqDesc, 
   xmlDavResponse, etc.) dealt with in this implementation are managed by a
   single DAV context.  To free all DAV objects associated with a DAV context
   that have not been explicitly deleted by the user, call XmlDavDetroy with
   the DAV context.
*/
struct xmlDavCtx; 
typedef struct xmlDavCtx xmlDavCtx;

/* DATATYPE xmlDavCond - Conditions to be met for a successful method execution
   SEE ALSO xmlDavCondTerm, xmlDavCondFac, XmlDavResMove, XmlDavResCopy
   This structure provides state tokens (i.e., lock token) and/or entity tags
   for a successful method execution.
*/
struct xmlDavCond; 
typedef struct xmlDavCond xmlDavCond;

/* DATATYPE xmlDavCondTerm - CONDition TERM
   SEE ALSO xmlDavCond, xmlDavCondFac, XmlDavResMove, XmlDavResCopy
   This structure defines some state configuration of a resource that must
   be satisfied in order for the associated request to be valid. Condition 
   terms in a condition are OR'd together.  In other words, one only needs 
   to match the state of the resource for the method to be allowed to
   continue.
*/
struct xmlDavCondTerm; 
typedef struct xmlDavCondTerm xmlDavCondTerm;

/* DATATYPE xmlDavCondFac - CONDition FACtor
   SEE ALSO xmlDavCondTerm, xmlDavCond, XmlDavResMove, XmlDavResCopy
   This structure defines some state of a resource that must be satisfied
   in order for the associated request to be valid.  The condition factors
   in condition term must all match with states of the resource, i.e., 
   they are AND'ed together.
*/
struct xmlDavCondFac; 
typedef struct xmlDavCondFac xmlDavCondFac;

/* DATATYPE xmlDavHdrDesc - HeaDeR DESCriptor
   SEE ALSO XmlDavCreateHdrDesc
   This structure represents an application-specific header that has two 
   parts: header name and its value.  Both parts are in compiler encoding.
*/
struct xmlDavHdrDesc; 
typedef struct xmlDavHdrDesc xmlDavHdrDesc;

/* DATATYPE xmlDavHostDesc - HOST DESCriptor
   SEE ALSO xmlDavHttpReq, XmlDavGetHttpReq
   This structure specifies the Internet host and port number of a resource.
*/
struct xmlDavHostDesc; 
typedef struct xmlDavHostDesc xmlDavHostDesc;

/* DATATYPE xmlDavHttpReq - HTTP REQuest
   SEE ALSO XmlDavGetHttpReq, xmlDavHostDesc
   This structure represents an HTTP request message that has two 
   major parts: connection information and request content.  Connection
   information can be used to set up HTTP connections, compose an HTTP
   "CONNECT" method for SSL tunneling, etc.  Request content part is 
   composed of two sub parts: "head" and "body".  The "head" subpart includes
   HTTP request line and headers.
   
   {{
   Steps of setting up a connection and sending the request:
     1. Make a connection to peer (using peer_host and peer_port)
     2. For HTTPS scheme, 
          2.1) if proxy is used:
               Send a CONNECT method through the connection
                 (using srv_host and srv_port)
               Wait until tunneling is established
          2.2) Start SSL
     3. Send HTTP request head through the connection 
     4. Send HTTP body if it exists through the connection
   }}
*/
struct xmlDavHttpReq; 
typedef struct xmlDavHttpReq xmlDavHttpReq;

/* DATATYPE xmlDavList - list of objects
   SEE ALSO XmlDavListAppendObj, XmlDavDestroy
   This structure provides a list of objects to be used in a method.
*/
struct xmlDavList; 
typedef struct xmlDavList xmlDavList;

/* DATATYPE xmlDavPrin - PRINcipal
   SEE ALSO XmlDavAclCreatePrin, XmlDavDestroy
   This structure defines a distinct human or computational actor that 
   initiates access to network resources.  In this implementation, a 
   principal is an HTTP resource that represents such an actor.
*/
struct xmlDavPrin; 
typedef struct xmlDavPrin xmlDavPrin;

/* DATATYPE xmlDavPriv - PRIVilege
   SEE ALSO XmlDavAclCreatePriv, XmlDavDestroy
   This structure defines a set of privileges.  A privilege controls access
   to a particular set of HTTP operations on a resource.
*/
struct xmlDavPriv; 
typedef struct xmlDavPriv xmlDavPriv;

/* DATATYPE xmlDavPropDesc - PROPerty DESCriptor
   SEE ALSO XmlDavCreatePropDesc
   This structure represents a property that has two parts: property's
   local name and its value. Note that a property's namespace URI is 
   recorded in its parent (i.e., xmlDavPropGrp).  So, each property must
   belong to a property group that is the entity to be provided to
   property-related operations.
*/
struct xmlDavPropDesc; 
typedef struct xmlDavPropDesc xmlDavPropDesc;

/* DATATYPE xmlDavPropGrp - PROPerty GRouP
   SEE ALSO XmlDavCreatePropGrp
   This structure represents a property group that is consisted of three
   parts: namespace uri, language specification and a list of properties
   (represented in xmlDavPropDesc).
*/
struct xmlDavPropGrp; 
typedef struct xmlDavPropGrp xmlDavPropGrp;

/* DATATYPE xmlDavReqDesc - REQuest DESCriptor
   SEE ALSO XmlDavCreateReqDesc, XmlDavDestroy
   This structure provides information to be used in formulating a request
   when a resource's method is invoked.  It is referenced from a resource
   descriptor.
*/
struct xmlDavReqDesc; 
typedef struct xmlDavReqDesc xmlDavReqDesc;

/* DATATYPE xmlDavResDesc - RESource DESCriptor
   SEE ALSO XmlDavCreateResDesc
   This structure represents a resource to be used in all resource-related
   routines.
*/
struct xmlDavResDesc; 
typedef struct xmlDavResDesc xmlDavResDesc;

/* DATATYPE xmlDavResponse - RESPONSE
   SEE ALSO XmlDavResCopy, XmlDavResGetProp, XmlDavRespGetXmlDoc
   This structure represents a response from the server to be used in all 
   response-related routines.
*/
struct xmlDavResponse; 
typedef struct xmlDavResponse xmlDavResponse;

/* DATATYPE xmlDavUri - URI representation
   SEE ALSO XmlDavResCopy
   This structure represents a URI specification.
*/
struct xmlDavUri; 
typedef struct xmlDavUri xmlDavUri;

/* DATATYPE xmlDavIri - IRI representation
   SEE ALSO XmlDavResCopy
   This structure represents an IRI specification.  For the definition of 
   IRI, please reference:
        http://www.ietf.org/internet-drafts/draft-duerst-iri-04.txt. 
*/
struct xmlDavIri;
typedef struct xmlDavIri xmlDavIri;

/* DATATYPE xmlDavXmlDoc - DAV's XML DOCument
   SEE ALSO XmlDavDestroy, XmlDavCreateXmlDoc, XmlDavRespGetXmlDoc
   This structure represents a DAV created XML Document.  APIs are provided
   to retrieve its xml context and/or xml document node.
*/
struct xmlDavXmlDoc; 
typedef struct xmlDavXmlDoc xmlDavXmlDoc;

/* DATATYPE xmlDavDepth - Depth of hierarchy
   SEE ALSO XmlDavResType
   This is an enumeration of the known depth values for manipulating resource
   in a hierarchy.
*/
typedef enum {
    XMLDAV_DEPTH_0    = 0,  /* target URL itself */
    XMLDAV_DEPTH_1       ,  /* target and its immediate members */
    XMLDAV_DEPTH_INF        /* target and recursively all its children */
} xmlDavDepth;

/* DATATYPE xmlDavLockScope - LOCK SCOPE
   SEE ALSO XmlDavResLock
   This is an enumeration of the known scope values for a lock.
*/
typedef enum {
    XMLDAV_LOCKSCOPE_UNKNOWN    = 0,  /* unknown */
    XMLDAV_LOCKSCOPE_EXCLUSIVE     ,  /* "exclusive" */
    XMLDAV_LOCKSCOPE_SHARED           /* "shared" */
} xmlDavLockScope;

/* DATATYPE xmlDavLockType - LOCK TYPE
   SEE ALSO XmlDavResLock
   This is an enumeration of the known lock type values.
*/
typedef enum {
    XMLDAV_LOCKTYPE_UNKNOWN    = 0,  /* unknown */
    XMLDAV_LOCKTYPE_WRITE            /* "write" */
} xmlDavLockType;

/* DATATYPE xmlDavPrinTyp - PRINcipal TYPe
   SEE ALSO XmlDavAclCreatePrin
   This is an enumeration of the predefined principal type values.
*/
typedef enum {
    XMLDAV_PRN_UNKNOWN    = 0,      /* unknown */
    XMLDAV_PRN_ALL,                 /* all */
    XMLDAV_PRN_AUTH,                /* authenticated */
    XMLDAV_PRN_UNAUTH,              /* unauthenticated */
    XMLDAV_PRN_SELF                 /* self */
} xmlDavPrinTyp;

/* DATATYPE xmlDavPrivTyp - PRIVilege TYPe
   SEE ALSO XmlDavAclCreatePriv
   This is an enumeration of the predefined privilege type values.
*/
typedef enum {
    XMLDAV_PT_UNKNOWN    = 0,       /* unknown */
    XMLDAV_PT_READ,                 /* read */
    XMLDAV_PT_WRITE,                /* write */
    XMLDAV_PT_WRITE_PROPERTIES,     /* write-properties */
    XMLDAV_PT_WRITE_CONTENT,        /* write-content */
    XMLDAV_PT_UNLOCK,               /* unlock */
    XMLDAV_PT_READ_ACL,             /* read-acl */
    XMLDAV_PT_READ_CUSR_PRIV_SET,   /* read-current-user-privilege-set */
    XMLDAV_PT_WRITE_ACL,            /* write-acl */
    XMLDAV_PT_BIND,                 /* bind */
    XMLDAV_PT_UNBIND,               /* unbind */
    XMLDAV_PT_ALL                   /* all */
} xmlDavPrivTyp;

/* DATATYPE xmlDavRscTyp - ReSourCe TYPe
   SEE ALSO XmlDavResType
   This is an enumeration of the known resource type values.
*/
typedef enum {
    XMLDAV_RT_UNKNOWN    = 0,       /* unknown */
    XMLDAV_RT_RSC,                  /* regular resource (or non-collection) */
    XMLDAV_RT_COL                   /* collection */
} xmlDavRscTyp;


/*---------------------------------------------------------------------------
                   Package WebDAV - WebDAV Client APIs
  ---------------------------------------------------------------------------*/

/****************************************************************************
                            Interface AccessControl
 ****************************************************************************/
/*---------------------------------------------------------------------------
    NAME
        XmlDavAclCreateAce

    PURPOSE
        Create an access control element (ACE)

    DESCRIPTION
        Creates an access control element (ACE) to be used in XmlDavResAcl
        method.

        {{
        Currently supported name/value attribute pairs:

        ("dav_deny", list of <xmlDavPriv *> type)
            The dav_deny attribute specifies the list of privileges to be
            denied to the specified principal.

        ("dav_grant", list of <xmlDavPriv *> type)
            The dav_grant attribute specifies the list of privileges to be
            granted to the specified principal.

        ("dav_invert", value of <xmlDavPrin *> type)
            The dav_invert attribute identifies those users NOT matching
            the current pricipal and to whom this ACE applies.

        ("dav_principal", value of <xmlDavPrin *> type)
            The dav_principal attribute identifies the principal to which this
            ACE applies.  

        }}
    

    RETURNS
        (xmlDavAce *) created ACE [or NULL on error with err set]

    ARGUMENTS
        dctx   (IN)     DAV context
        err    (IN/OUT) returned error code
        ...    (IN)     variable arguments, with final NULL

    SEE ALSO
        XmlDavDestroy, XmlDavAclCreatePriv
*/
xmlDavAce *XmlDavAclCreateAce(xmlDavCtx *dctx, xmlerr *err, ...);

/*---------------------------------------------------------------------------
    NAME
        XmlDavAclCreatePrin

    PURPOSE
        Create a principal object

    DESCRIPTION
        Creates a principal object to be used in XmlDavAclCreateAce.

        {{
        Currently supported name/value attribute pairs:

        ("dav_href", value of <oratext *> type in DAV's data encoding)
            The dav_href attribute specifies the URL to be matched against.
            The current user matches DAV:href only if that user is
            authenticated as being (or being a member of) the principal
            identified by the URL contained by that DAV:href.

        ("dav_property", value of <xmlelemnode *> type in DAV's data encoding)
            The dav_property attribute identified a property of the resource.
            The current user matches a DAV:property principal in a DAV:acl
            property of a resource only if the value of the identified 
            property of that resource contains at most one DAV:href XML
            element, the URI value of DAV:href identifies a principal, and
            the current user is authenticated as being (or being a member of)
            that principal.  For example, if the DAV:property element 
            contained <DAV:owner/>, the current user would match the 
            DAV:property principal only if the current user is authenticated
            as matching the principal identified by the DAV:owner property
            of the resource.  If this attribute is specified, "xmlctx" 
            attribute needs to be specified too.

        ("predefined_principal", value of <xmlDavPrinTyp> type)
            The predefined attribute specifies one of the following choices:
            XMLDAV_PRN_ALL, XMLDAV_PRN_AUTH, XMLDAV_PRN_UNAUTH, 
            XMLDAV_PRN_SELF.  These choices are mapped to the corresponding 
            DAV:all, DAV:authenticated, DAV:unauthenticated, and DAV:self 
            XML elements.  The current user always matches DAV:all; it matches
            DAV:authenticated only if authenticated; it matches 
            DAV:unauthenticated only if not authenticated; it matches DAV:self
            in a DAV:acl property of the resource only if that resource is a 
            principal and that principal matches the current user or, if the
            principal is a group, a member of that group matches the current
            user.

        ("xmlctx", value of <xmlctx *> type)
            The xmlctx attribute is used to specify the xmlctx used to 
            create element nodes provided in dav_property attribute.

        }}
    

    RETURNS
        (xmlDavPrin *) created principal object

    ARGUMENTS
        dctx   (IN)     DAV context
        err    (IN/OUT) returned error code
        ...    (IN)     variable arguments, with final NULL

    SEE ALSO
        XmlDavDestroy, XmlDavAclCreateAce
*/
xmlDavPrin *XmlDavAclCreatePrin(xmlDavCtx *dctx, xmlerr *err, ...);

/*---------------------------------------------------------------------------
    NAME
        XmlDavAclCreatePriv

    PURPOSE
        Create a privilege object

    DESCRIPTION
        Creates a privilege object to be used in XmlDavAclCreateAce.

        {{
        Currently supported name/value attribute pairs:

        ("predefined_privilege", value of <xmlDavPrivTyp> type)
            The predefined_privilege attribute specifies the set of 
            predefined privileges which control access to a particular set 
            of HTTP operations on a resource.

        }}
    

    RETURNS
        (xmlDavPriv *) created privilege object

    ARGUMENTS
        dctx   (IN)     DAV context
        err    (IN/OUT) returned error code
        ...    (IN)     variable arguments, with final NULL

    SEE ALSO
        XmlDavDestroy, XmlDavAclCreateAce
*/
xmlDavPriv *XmlDavAclCreatePriv(xmlDavCtx *dctx, xmlerr *err, ...);

/****************************************************************************
                            Interface Condition
 ****************************************************************************/
/*---------------------------------------------------------------------------
    NAME
        XmlDavCondCreate

    PURPOSE
        Create a new condition

    DESCRIPTION
        Create a new condition.  A condition represents some state 
        configuration of a resource that must be satisfied in order for 
        the associated request to be valid. 
        
        It represents some condition term or collection of condition
        terms representing states of a resource. If the state of the 
        resource does not match any of the specified states in any of 
        the condition terms, then the method would fail. Condition terms
        in a condition are OR'd together while condition factors in a 
        condition term are AND'ed. 

        When performing comparison, the definition of a matching state
        token or entity tag is as follows:
          Matching entity tag: Where the entity tag matches an entity tag
                               associated with that resource.
          Matching state token: Where there is an exact match between the
                                state token in the condition and any state
                                token on the resource.

        {{
        Currently supported name/value attribute pairs:

        ("list", list of <xmlDavCondTerm *> type)
            The list attribute specifies that a list of condition terms
            to be OR'ed together to form a condition.  Objects on the
            list are of xmlDavCondTerm type.  

        ("condition_term", value of <xmlDavCondTerm *> type)
            The condition_term attribute specifies that a single condition
            term to be included in a condition.

        }}
    
    EXAMPLE


    RETURNS
        (xmlDavCond *) created condition [or NULL on error with err set]

    ARGUMENTS
        dctx   (IN)     DAV context
        err    (IN/OUT) returned error code
        ...    (IN)     variable arguments, with final NULL

    SEE ALSO
        XmlDavDestroy, XmlDavCondCreateTerm
*/
xmlDavCond *XmlDavCondCreate(xmlDavCtx *dctx, xmlerr *err, ...);

/*---------------------------------------------------------------------------
    NAME
        XmlDavCondCreateFac

    PURPOSE
        Create a new condition factor

    DESCRIPTION
        Create a new condition factor.  A condition factor represents 
        some state of a resource that must be satisfied in order for the 
        associated request to be valid. The condition factors in a
        condition term must all match with states of the resource, i.e., 
        they are AND'ed together.

        {{
        Currently supported name/value attribute pairs:

        ("Etag", value of <oratext *> type in compiler encoding)
            The Etag attribute is used to indicate an EntityTag 
            describing the state of the body or contents of a resource 
            represented as an opaque string. See section 3.11 of the 
            HTTP/1.1 specification. 

        ("not_state_token", value of <oratext *> type in compiler encoding)
            The not_state_token attribute is used to indicate some state 
            information represented as a URI. The only example defined
            in WebDAV is lock token.  This attribute is similar to 
            "state_token" attribute. However, the comparison on this 
            condition factor is negated.

        ("not_entity_tag", value of <oratext *> type in compiler encoding)
            The not_entity_tag attribute is used to indicate an EntityTag 
            describing the state of the body or contents of a resource 
            represented as an opaque string. See section 3.11 of the 
            HTTP/1.1 specification. This attribute is similar to Etag 
            attribute.  However, the comparison on this condition factor is 
            negated.

        ("state_token", value of <oratext *> type in compiler encoding)
            The state_token attribute is used to indicate some state 
            information represented as a URI. The only example defined
            in WebDAV is lock token.  If specified, it means that the 
            resource must be locked with the provided lock token for the
            comparison to succeed.

        }}
    
    EXAMPLE
        xmlDavCtx        *dctx;
        xmlerr            xerr;
        xmlDavPropDesc   *propDesc;
        xmlDavPropGrp    *propGrp;
        xmlDavResponse   *resp;
        xmlDavResDesc    *resDesc;
        oratext          *etagInComp;  // etag in compiler encoding
        oratext           etag[MAX_ETAG_LEN];
        oratext          *tokenInComp; // lock token in compiler encoding

        // Get a resource's Etag
        if (xerr = XmlDavResHead(resDesc, "response", &resp, NULL))
            return xerr;
        etagInComp = NULL;
        if (resp)    // resp can be NULL
        {
            oratext    *hdrNam;
            sb4         statusCode;
            ub4         index;

            statusCode = XmlDavRespGetCode(resp, &xerr);
            switch (statusCode)
            {
            case 204:    // OK
                index = 0;
                while (hdrNam = XmlDavRespGetHdrNam(resp, &xerr, index))
                {
                    if (!XmlDavEquals(hdrNam, (oratext *) "Etag", FALSE))
                    {
                        etagInComp = XmlDavRespGetHdrVal(resp, &xerr, index);
                        DISCARD lstcpy(etag, (const oratext *) etagInComp);
                        break;
                    }
                    index++;
                }
                break;
            default:
                break;
            }
        }
        // Free response object.  After this, etagInComp cannot be referenced
        // anymore.
        if (resp)
            XmlDavDestroy(&xerr, "response", resp, NULL);
        ...
        // Lock the resource.  "jim_whitehead@yahoo.com" is in DAV's encoding
        // In this example, our encoding is assumed to be ISO-8859-1.
        // 
        if (xerr = XmlDavResLock(res, "owner_text", "jim_whitehead@yahoo.com",
                   "scope", XMLDAV_LOCKSCOPE_EXCLUSIVE,
                   "lock_type",  XMLDAV_LOCKTYPE_WRITE,
                   "lock_timeout", 0, 
                   "use_if_header", TRUE, // for backward compatibility
                   "response", &resp, 
                   NULL))
            return xerr;

        tokenInComp = NULL;
        if (resp)    // resp can be NULL
        {
            oratext    *hdrNam;
            sb4         statusCode;
            ub4         index;

            statusCode = XmlDavRespGetCode(resp, &xerr);
            switch (statusCode)
            {
            case 200:    // OK
                index = 0;
                while (hdrNam = XmlDavRespGetHdrNam(resp, &xerr, index))
                {
                    if (XmlDavEquals(hdrNam,(oratext *)"Lock-Token", FALSE))
                    {
                        tokenInComp = XmlDavRespGetHdrVal(resp, &xerr, index);
                        DISCARD lstcpy(token, (const oratext *) tokenInComp);
                    }
                    index++;
                }
                break;
            default:
                break;
            }
        }
        // Free the response object.  After this, tokenInComp cannot be 
        // referenced anymore.
        XmlDavDestroy(&xerr, "response", resp, NULL);
        ...
        // Create a condition factor
        if (etagInComp && tokenInComp)
            condFac = XmlDavCondCreateFac(dctx, &xerr,"Etag", etag, 
                                                      "state_token", token,
                                                       NULL);
        ...

    RETURNS
        (xmlDavCondFac *) condition factor [or NULL on error with err set]

    ARGUMENTS
        dctx   (IN)     DAV context
        err    (IN/OUT) returned error code
        ...    (IN)     variable arguments, with final NULL

    SEE ALSO
        XmlDavDestroy, XmlDavCondCreateTerm, XmlDavCondCreate
*/
xmlDavCondFac *XmlDavCondCreateFac(xmlDavCtx *dctx, xmlerr *err, ...);

/*---------------------------------------------------------------------------
    NAME
        XmlDavCondCreateTerm

    PURPOSE
        Create a new condition term

    DESCRIPTION
        Create a new condition term.  A condition term represents some 
        state configuration of a resource that must be satisfied in order 
        for the associated request to be valid. 
        
        The condition factors in a condition term must all match with 
        states of the resource, i.e., they are AND'ed together. Condition
        terms are contained in a condition that is used in the precondition 
        attribute. 

        {{
        Currently supported name/value attribute pairs:

        ("condition_factor", value of <xmlDavCondFac *> type)
            The condition_factor attribute specifies that a single condition
            factor is to be included in a condition term.

        ("list", list of <xmlDavCondFac *> type)
            The list attribute specifies that a list of condition factors
            to be AND'ed together to form a condition term.  Objects on the
            list are xmlDavCondFac type.

        ("iri", value of <xmlDavIri *> type)
            The iri attribute scopes the list of condition factors.
            That is, it specifies that the list of condition factors only 
            apply to the specified resource.  If not specified, it indicates
            the list of condition factors applies to the resource receiving 
            the request.

        ("uri", value of <xmlDavUri *> type)
            The uri attribute scopes the list of condition factors.
            That is, it specifies that the list of condition factors only 
            apply to the specified resource.  If not specified, it indicates
            the list of condition factors applies to the resource receiving 
            the request.

        }}
    
    EXAMPLE


    RETURNS
        (xmlDavCondTerm *) condition term [or NULL on error with err set]

    ARGUMENTS
        dctx   (IN)     DAV context
        err    (IN/OUT) returned error code
        ...    (IN)     variable arguments, with final NULL

    SEE ALSO
        XmlDavDestroy, XmlDavCondCreateFac, XmlDavCondCreate
*/
xmlDavCondTerm *XmlDavCondCreateTerm(xmlDavCtx *dctx, xmlerr *err, ...);

/****************************************************************************
                            Interface Document
 ****************************************************************************/
/*---------------------------------------------------------------------------
    NAME
        XmlDavDocGetNode

    PURPOSE
        Retrieve the document node from a DAV document

    DESCRIPTION
        Retrieve the document node from a DAV document.  Don't free this
        document node with XmlFreeDocument.  Instead, free the whole DAV
        document (i.e., xmlDavXmlDoc) with XmlDavDestroy.
    
    RETURNS
        (xmldocnode *) returned document node [or NULL on error with err set]

    ARGUMENTS
        dctx   (IN)     DAV context
        err    (IN/OUT) returned error code

    SEE ALSO
        XmlDavDestroy, XmlDavCreateXmlDoc, XmlDavRespGetXmlDoc
*/
xmldocnode *XmlDavDocGetNode(xmlDavXmlDoc *doc, xmlerr *err);

/*---------------------------------------------------------------------------
    NAME
        XmlDavDocGetXmlCtx

    PURPOSE
        Retrieve the XML context (i.e., xmlctx) from a DAV's document

    DESCRIPTION
        Retrieve the XML context (i.e., xmlctx) from a DAV's document.
    
    RETURNS
        (xmlctx *) returned XML context [or NULL on error with err set]

    ARGUMENTS
        doc    (IN)     DAV's document
        err    (IN/OUT) returned error code

    SEE ALSO
        XmlDavCreateXmlDoc, XmlDavRespGetXmlDoc
*/
xmlctx *XmlDavDocGetXmlCtx(xmlDavXmlDoc *doc, xmlerr *err);

/****************************************************************************
                            Interface Host
 ****************************************************************************/
/*---------------------------------------------------------------------------
    NAME
        XmlDavHostGetNamLen

    PURPOSE
        Retrieve the length of host name from a host descriptor

    DESCRIPTION
        Retrieve the length of host name from a host descriptor. 
    
    RETURNS
        (ub4) returned host name length

    ARGUMENTS
        host   (IN)     host descriptor
        err    (IN/OUT) returned error code

    SEE ALSO
        XmlDavHostGetPortLen, XmlDavHostGetNamStr
*/
ub4 XmlDavHostGetNamLen(xmlDavHostDesc *host, xmlerr *err);

/*---------------------------------------------------------------------------
    NAME
        XmlDavHostGetNamStr

    PURPOSE
        Retrieve the host name from a host descriptor

    DESCRIPTION
        Retrieve the host name from a host descriptor. 
    
    RETURNS
        (oratext *) returned host name

    ARGUMENTS
        host   (IN)     host descriptor
        err    (IN/OUT) returned error code

    SEE ALSO
        XmlDavHostGetPortStr, XmlDavHostGetNamLen
*/
oratext *XmlDavHostGetNamStr(xmlDavHostDesc *host, xmlerr *err);

/*---------------------------------------------------------------------------
    NAME
        XmlDavHostGetPortLen

    PURPOSE
        Retrieve the port length from a host descriptor

    DESCRIPTION
        Retrieve the length of port number string from a host descriptor. 
        Note that port number string is optional.
    
    RETURNS
        (ub4) returned port length

    ARGUMENTS
        host   (IN)     host descriptor
        err    (IN/OUT) returned error code

    SEE ALSO
        XmlDavHostGetNamLen, XmlDavHostGetPortStr
*/
ub4 XmlDavHostGetPortLen(xmlDavHostDesc *host, xmlerr *err);

/*---------------------------------------------------------------------------
    NAME
        XmlDavHostGetPortStr

    PURPOSE
        Retrieve the port number string from a host descriptor

    DESCRIPTION
        Retrieve the optional port number string from a host descriptor. 
    
    RETURNS
        (oratext *) returned port number string

    ARGUMENTS
        host   (IN)     host descriptor
        err    (IN/OUT) returned error code

    SEE ALSO
        XmlDavHostGetPortLen, XmlDavHostGetNamStr
*/
oratext *XmlDavHostGetPortStr(xmlDavHostDesc *host, xmlerr *err);

/****************************************************************************
                            Interface List
 ****************************************************************************/
/*---------------------------------------------------------------------------
    NAME
        XmlDavListAppendObj

    PURPOSE
        Append a new object onto the list

    DESCRIPTION
        
        Append a new object onto the list.  When objects are appended, only 
        their pointers are saved on the list.  So, objects on the list must 
        exist until the list is freed.

    EXAMPLE
        xmlDavCtx         *dctx;
        xmlerr             xerr;
        xmlDavList        *propList;
        xmlDavPropDesc    *prop;
        ...
        // Create a list
        if (!(propList = XmlDavCreateList(dctx, &xerr)))
            return xerr;
        ...
        // Append properties to the list
        if (xerr = XmlDavListAppendObj(propList, (void *)prop))
            return xerr;
        ...
        // Clean up
        if (propList)
            XmlDavDestroy(&xerr, "list", propList, NULL);

    RETURNS
        (xmlerr) numeric error code

    ARGUMENTS
        dlist  (IN)  DAV list object
        obj    (IN)  object to be appended

    SEE ALSO
        XmlDavCreateList, XmlDavDestroy
*/
xmlerr XmlDavListAppendObj(xmlDavList *dlist, void *obj);

/*---------------------------------------------------------------------------
    NAME
        XmlDavListDeleteObj

    PURPOSE
        Delete an object from the list

    DESCRIPTION
        
        Delete an object from the list.  When objects are appended, only 
        their pointers are saved on the list.  So, objects on the list must 
        exist until the list is destroyed or until they are deleted from the
        list.

    EXAMPLE
        xmlDavCtx         *dctx;
        xmlerr             xerr;
        xmlDavList        *propList;
        xmlDavPropDesc    *prop;
        ...
        // Create a list
        if (!(propList = XmlDavCreateList(dctx, &xerr)))
            return xerr;
        ...
        // Append properties to the list
        if (xerr = XmlDavListAppendObj(propList, (void *)prop))
            return xerr;
        ...
        // Delete objects from the list
        if (xerr = XmlDavListDeleteObj(propList, (void *) prop))
            return xerr;
        ...
        // Clean up
        if (propList)
            XmlDavDestroy(&xerr, "list", propList, NULL);

    RETURNS
        (xmlerr) numeric error code

    ARGUMENTS
        dlist  (IN)  DAV list object
        obj    (IN)  object to be deleted

    SEE ALSO
        XmlDavCreateList, XmlDavListAppendObj, XmlDavDestroy
*/
xmlerr XmlDavListDeleteObj(xmlDavList *dlist, void *obj);

/****************************************************************************
                            Interface TopLevel
 ****************************************************************************/
/*---------------------------------------------------------------------------
    NAME
        XMLDAV_HTTP_RESP1_F

    PURPOSE
        User-defined HTTP response callback #1

    DESCRIPTION
        This macro defines a prototype for the HTTP response function 
        callback, which is called once to send an HTTP request and receive
        the response via user's HTTP implementation.  This function should 
        return an xmlDavXmlDoc object on success.  XmlDavCreateXmlDoc is 
        available for converting the XML content embedded in the response
        body to an xmlDavXmlDoc object.

    RETURNS
        (xmlDavXmlDoc *) DAV's document on success; NULL, otherwise

    ARGUMENTS
        dctx    (IN)  DAV's context
        httpReq (IN)  HTTP request object

    SEE ALSO
        XMLDAV_HTTP_RESP2_F, XmlDavCreateXmlDoc
*/
#define XMLDAV_HTTP_RESP1_F(func) \
    xmlDavXmlDoc *func(xmlDavCtx *dctx, xmlDavHttpReq *httpReq)

/*---------------------------------------------------------------------------
    NAME
        XMLDAV_HTTP_RESP2_F

    PURPOSE
        User-defined HTTP response callback #2

    DESCRIPTION
        This macro defines a prototype for the HTTP response function 
        callback, which is called once to send an HTTP request and receive
        the response via user's HTTP implementation.  This function should 
        return an xmlDavResponse object on success.

    RETURNS
        (xmlDavResponse *) DAV's response object on success; NULL, otherwise

    ARGUMENTS
        dctx    (IN)  DAV's context
        httpReq (IN)  HTTP request object

    SEE ALSO
        XMLDAV_HTTP_RESP1_F
*/
#define XMLDAV_HTTP_RESP2_F(func) \
    xmlDavResponse *func(xmlDavCtx *dctx, xmlDavHttpReq *httpReq)


/*---------------------------------------------------------------------------
    NAME
        XmlDavCreate

    PURPOSE
        Create an xmlDavCtx used for WebDAV API

    DESCRIPTION
        Create an xmlDavCtx to be used in all other WebDAV APIs.  Note that
        there is always a back pointer from resource descriptor, request
        descriptor, or response object to this DAV context.  At the end of 
        application, XmlDavDestroy must be called to free up the DAV context
        and all its subordinate objects (e.g., xmlDavResDesc, xmlDavResponse,
        xmlDavReqDesc, etc.) allocated directly or indirectly using this DAV 
        context.
        
        Data encoding of the provided xmlctx is used to determine the
        encoding (unless stated otherwise) of the string data to be provided
        to this set of new WebDAV APIs and the encoding of the XML DOM 
        document returned from a response in in.  "utf-8" will always be used
        as the encoding for the wire data. Appropriate character conversion 
        will be done between input data and wire data or wire data and 
        output data.

        {{
        Currently supported name/value attribute pairs:

        ("http_proxy", value of <oratext *> type in compiler encoding)
            The http_proxy attribute is used to provide new proxy server
            setting.  This will override what is specified in the environment
            variable.  If specified, any request with target URI does not
            match any entry on the "no proxy" list when provided, will be sent
            via the specified proxy server. See also environment variable 
            "http_proxy" for this attribute's format.

        ("io_buffer", value of <oratext *> type)
            The io_buffer can be provided by the user to be used in the
            low-level transport mechanism.  If not provided, system will
            allocate one for the user.  If provided, the io_buffer_size
            attribute needs to be provided too.

        ("io_buffer_size", value of ub4 type)
            The io_buffer_size indicates that the size of io_buffer in KBytes
            provided by the user via "io_buffer" attribute.

        ("max_redirect, value of ub4 type)
            The max_redirect attribute is used to indicate the maximum
            number of redirections before a connection fail.  Note that
            for some cases (e.g., status 302), no automatic redirection 
            will be done.

        ("max_xml_size", value of ub4 type)
            The max_xml_size attribute is used to indicate that the maximum
            size of the XML document allowed in the entity body in Kbytes.
            If the size of XML document exceeds the limit, 
            XmlDavRespGetXmlDoc or XmlDavCreateXmlDoc would fail and no DOM 
            will be created.

        ("no_proxy", value of <oratext *> type in compiler encoding)
            The no_proxy attribute is used to provide a list comma- or 
            space-separated list of machine or domain names, with an optional 
            port part.  If no port is present, it applies to all ports on 
            that domain.  Any request with target URI matching one of the 
            entry on the list will not be using the proxy server as provided
            by "http_proxy" attribute or specified via environment variable
            "http_proxy".  If there is no proxy server specified, this 
            attribute will be ingored.  See also environment variable 
            "no_proxy" for this attribute's format.

        ("open_timeout", value of ub4 type)
            The open_timeout attribute is used to indicate that the number
            of milliseconds before an open operation times out.

        ("read_timeout", value of ub4 type)
            The read_timeout attribute is used to indicate that the number
            of milliseconds before a read operation times out.

        }}


    EXAMPLE
        xmlctx     *xctx;
        xmlDavCtx  *dctx;
        xmlerr      xerr;
        ...
        // Create a meta context to be used in all WebDAV operations.
        //
        //     "lowAllocMem" is a low-level memory alloc function using 
        //     XML_ALLOC_F macro as its prototype. Similarly, "lowFreeMem"
        //     is a low-level memory free function using XML_FREE_F macro as
        //     its prototype.
        //
        //     "msghdlr" is user-provided error handler using XML_ERRMSG_F 
        //     macro as its prototype.
        if (!(xctx = XmlCreate(&xerr, (oratext *) "myApplication",
                               "data_encoding",   "utf-8",
                               "error_language",  lang,
                               "error_handler",   msghdlr,
                               "error_context",   msgctx,
                               "memory_alloc",    lowAllocMem,
                               "memory_free",     lowFreeMem,
                               "memory_context",  memcbctx,
                               NULL)))
            return xerr;
        // Create a DAV context to be used in all WebDAV operations.
        if (!(dctx = XmlDavCreate(xctx, &xerr, "max_xml_size", 10, NULL)))
            return xerr;
        // All WebDav operations come next.
        ...
        // Clean up
        if (dctx)
            XmlDavDestroy(&xerr, "dav_context", dctx, NULL);
        if (xctx)
            XmlDestroy(xctx);

    RETURNS
        (xmlDavCtx *) created xmlDavCtx [or NULL on error with err set]

    ARGUMENTS
        xctx (IN)     xmlctx with appropriate data encoding
        err  (IN/OUT) returned error code
        ...  (IN)     variable arguments, with final NULL

    SEE ALSO
        XmlCreate, XmlDavDestroy
*/
xmlDavCtx *XmlDavCreate(xmlctx *xctx, xmlerr *err, ...);

/*---------------------------------------------------------------------------
    NAME
        XmlDavCreateHdrDesc

    PURPOSE
        Create a new header descriptor

    DESCRIPTION
        Create a new header descriptor.  Header descriptor is consisted of
        two parts: header name and its value. This structure allows an
        application to provide its own headers.
        
        {{
        Currently supported name/value attribute pairs:

        ("name", value of <oratext *> type in compiler encoding)
            The "name" attribute is used to specify the name of an 
            application-specific header.

        ("value", value of <oratext *> type in compiler encoding)
            The "value" attribute is used to specify the value of an 
            application-specific header.

        }}
    
    EXAMPLE
        xmlDavCtx         *dctx;
        xmlerr             xerr;
        xmlDavList        *hdrList;
        xmlDavHdrDesc     *hdrDesc;
        ...
        // Create a header descriptor
        //
        //   Note that "name" must be in DAV context's data encoding.  
        if (!(hdrDesc = XmlDavCreateHdrDesc(dctx, &xerr,
                                      "name", "Proxy-Connection",
                                      "value", "Keep-Alive",
                                      NULL)))
            return xerr;
        // Create a list
        if (!(hdrList = XmlDavCreateList(dctx, &xerr)))
            return xerr;
        ...
        // Append application-specific headers to the list
        if (xerr = XmlDavListAppendObj(elemList, (void *)hdrDesc))
            return xerr;
        ...
        if (!(reqDesc = XmlDavCreateReqDesc(dctx, &xerr, 
                            "User-Agent", "CERN-LineMode/2.15 libwww/2.17b3",
                            "app_hdr_list", hdrList,
                            NULL)))
        ...
        // Clean up
        if (hdrList)
            XmlDavDestroy(&xerr, "list", hdrList, NULL);
        if (hdrDesc)
            XmlDavDestroy(&xerr, "header_descriptor", propDesc, NULL);

    RETURNS
        (xmlDavHdrDesc *) created header descriptor [or NULL on error]

    ARGUMENTS
        dctx   (IN)     DAV context
        err    (IN/OUT) returned error code
        ...    (IN)     variable arguments, with final NULL

    SEE ALSO
        XmlDavDestroy, XmlDavCreateList, xmlDavHdrDesc
*/
xmlDavHdrDesc *XmlDavCreateHdrDesc(xmlDavCtx *dctx, xmlerr *err,...);

/*---------------------------------------------------------------------------
    NAME
        XmlDavCreateIri

    PURPOSE
        Create a new IRI descriptor

    DESCRIPTION
        Create a new IRI descriptor to be used in all resource-related 
        operations. For the definition of IRI, please reference:
        http://www.ietf.org/internet-drafts/draft-duerst-iri-05.txt. 
        Internally, a conversion from IRI to URI will be done before it can
        be used in the HTTP headers.

        {{
        Currently supported name/value attribute pairs:

        ("absolute_uri", value of <oratext *> type in compiler encoding)
            The absolute_uri attribute is a Uniform Resource Identifier and
            identifies the resource upon which to apply the request.

        }}
    
    EXAMPLE
        xmlDavResDesc     *smallDir;
        xmlerr             xerr;
        xmlDavIri         *iri4SmallCol;
        xmlDavReqDesc     *sreq;
        ...
        // Create an IRI descriptor
        //
        //   Both "www.w3.org" and "/pub/WWW/" are in UTF-8 encoding.  
        if (!(iri4SmallCol = XmlDavCreateIri(dctx, &xerr, 
                              "absolute_uri", "http://www.w3.org/pub/WWW/",
                              NULL)))
            return xerr;
        // Create a resource descriptor
        if (!(smallDir = XmlDavCreateResDesc(dctx, &xerr, "iri", iri4SmallCol,
                               NULL)))
            return xerr;
        ...
        // Set request descriptor in the resource descriptor
        if (xerr = XmlDavUpdResDesc(smallDir, "request_descriptor", 
                                   sreq, NULL)))
            return xerr;
        // All resource-related operations come here 
        ...
        // Clean up
        if (smallDir)
            XmlDavDestroy(&xerr, "resource_descriptor", smallDir, NULL);
        if (iri4SmallCol)
            XmlDavDestroy(&xerr, "iri", iri4SmallCol, NULL);
        if (sreq)
            XmlDavDestroy(&xerr, "request_descriptor", sreq, NULL);


    RETURNS
        (xmlDavIri *) created IRI descriptor [or NULL on error with err set]

    ARGUMENTS
        dctx   (IN)     DAV context
        err    (IN/OUT) returned error code
        ...    (IN)     variable arguments, with final NULL

    SEE ALSO
        XmlDavDestroy, XmlDavCreateUri
*/
xmlDavIri *XmlDavCreateIri(xmlDavCtx *dctx, xmlerr *err, ...);

/*---------------------------------------------------------------------------
    NAME
        XmlDavCreateList

    PURPOSE
        Create a new linked list

    DESCRIPTION
        Create a new linked list.  When objects are appended onto a list, only
        their pointers are saved on the list.  So, objects that are appended
        to the list must exist until the list is free.

    
    EXAMPLE
        xmlDavCtx         *dctx;
        xmlerr             xerr;
        xmlDavList        *propList;
        xmlDavPropDesc    *prop;
        ...
        // Create a list
        if (!(propList = XmlDavCreateList(dctx, &xerr)))
            return xerr;
        ...
        // Append properties to the list
        if (xerr = XmlDavListAppendObj(propList, (void *)prop))
            return xerr;
        ...
        // Clean up
        if (propList)
            XmlDavDestroy(&xerr, "list", propList, NULL);

    RETURNS
        (xmlDavList *) created list object [or NULL on error with err set]

    ARGUMENTS
        dctx   (IN)     DAV context
        err    (IN/OUT) returned error code

    SEE ALSO
        XmlDavDestroy, XmlDavListAppendObj
*/
xmlDavList *XmlDavCreateList(xmlDavCtx *dctx, xmlerr *err);

/*---------------------------------------------------------------------------
    NAME
        XmlDavCreatePropDesc

    PURPOSE
        Create a new property descriptor

    DESCRIPTION
        Create a new property descriptor.  Property descriptor is consisted of
        two parts: property's local name and potentially its value.  If a
        property value is provided, it can be used to update a property's 
        value via XmlDavResUpdProp.  Attributes "text_value", "elem_value" and
        "list_value" are exclusive.  Specify only one of them.
        
        Each property descriptor must be assigned to a property group before
        it can be used in this Client APIs.  If a user-created XML element 
        node (for example, <Z:Author>Stanley Guan</Z:Author>) is provided 
        via "elem_value" or "list_value" attribute, its namespace declaration 
        must be appropriately declared based on the following guidelines:
           1. If the new element's namespace uri is the same as property 
              group's in which this property descriptor belongs to, no
              namespace declaration attribute needs to be specified. However,
              the new element's prefix must be chosen the same as the 
              property group's.
           2. Otherwise, the new element must be created like:
                <Z:Author xmlns:Z="http://www.w3.com/standards/z39.50">
                Stanley Guan
                </Z:Author>
              with its own namespace declaration attribute specified.

        {{
        Currently supported name/value attribute pairs:

        ("elem_value", value of <xmlelemnode *> type in DAV's data encoding)
            The elem_value attribute is used to specify the value of a 
            property whose content is a single XML sub-element node.  If
            specified, xmlctx attribute must also be specified.

        ("list_value", list of xmlelemnode in DAV's data encoding)
            The list_value attribute is used to specify the value of a 
            property whose content is a list of XML sub-element nodes.
            Example: <Z:Author>Stanley Guan</Z:Author>, 
                     <Z:Author>Jim Whitehead</Z:Author>, ...
            If specified, xmlctx attribute must also be specified.

        ("local_name", value of <oratext *> type in DAV's data encoding)
            The local_name attribute identifies the local name of a property.
            This attribute is required.

        ("text_value", value of <oratext *> type in DAV's data encoding)
            The text_value attribute is used to specify the value of a 
            property whose content is text only (vs. XML element(s)).

        ("xmlctx", value of <xmlctx *> type)
            The xmlctx attribute is used to specify the xmlctx used to 
            create element nodes provided in list_value or elem_value
            attributes.

        }}
    
    EXAMPLE
        xmlctx            *xctx;
        xmlDavCtx         *dctx;
        xmlDavResDesc     *res;
        xmldocnode        *doc;
        xmlerr             xerr;
        xmltextnode       *nam1, *nam2;
        xmlelemnode       *elem1, *elem2;
        xmlDavList        *elemList;
        xmlDavPropDesc    *propDesc;
        xmlDavPropGrp     *propGrp;
        xmlDavResponse    *resp;
        ...
        if (!(dctx = XmlDavCreate(xctx, &xerr, "max_xml_size", 10, NULL)))
            return xerr;
        doc = XmlCreateDocument(xctx, (oratext *)0, (oratext *)0, 
                                (xmldtdnode *)0, &xerr);
        ...
        // Note that all namespace URIs, QNames, and text data are in DAV 
        // context's data encoding.  In this example, our encoding is 
        // assumed to be ISO-8859-1.
        elem1 = XmlDomCreateElemNS(xctx, doc, 
                    "http://www.w3.com/standards/z39.50", "Z:Author");
        elem2 = XmlDomCreateElemNS(xctx, doc, 
                    "http://www.w3.com/standards/z39.50", "Z:Author");
        nam1 = XmlDomCreateTextNode(xctx, doc, "Stanley Guan");
        nam2 = XmlDomCreateTextNode(xctx, doc, "Jim Whitehead");
        DISCARD XmlDomAppendChild(xctx, elem1, nam1);
        DISCARD XmlDomAppendChild(xctx, elem2, nam2);
        ...
        // Create a list
        if (!(elemList = XmlDavCreateList(dctx, &xerr)))
            return xerr;
        ...
        // Append elements to the list
        if (xerr = XmlDavListAppendObj(elemList, (void *)elem1))
            return xerr;
        if (xerr = XmlDavListAppendObj(elemList, (void *)elem2))
            return xerr;
        ...
        // Create a property descriptor
        //
        //   Note that "authors" must be in DAV context's data encoding.  
        if (!(propDesc = XmlDavCreatePropDesc(dctx, &xerr,
                                      "local_name", "authors",
                                      "list_value", elemList,
                                      NULL)))
            return xerr;
        ...
        // Create a property group object
        //
        //   Note that both namespace URI and "en" need to be in DAV 
        //   context's data encoding. 
        if (!(propGrp = XmlDavCreatePropGrp(dctx, &xerr,
                                      "namespace_uri", 
                                        "http://www.w3.com/standards/z39.50",
                                      "prefix", "Z",  /o match "Z:Author" o/
                                      "natural_language", "en",
                                      "prop", propDesc,
                                      NULL)))
            return xerr;
        ...
        //
        if (xerr = XmlDavResUpdProp(res, "set", propGrp, 
                                         "response", &resp, NULL))
            return xerr;
        ...
        // Clean up
        if (propGrp)
            XmlDavDestroy(&xerr, "prop_group", propGrp, NULL);
        if (propDesc)
            XmlDavDestroy(&xerr, "prop_descriptor", propDesc, NULL);
        if (elemList)
            XmlDavDestroy(&xerr, "list", elemList, NULL);
        if (doc)
            XmlFreeDocument(xctx, doc);

    RETURNS
        (xmlDavPropDesc *) created property descriptor [or NULL on error]

    ARGUMENTS
        dctx   (IN)     DAV context
        err    (IN/OUT) returned error code
        ...    (IN)     variable arguments, with final NULL

    SEE ALSO
        XmlDavDestroy, XmlDavCreateList, XmlDavResGetProp, XmlDavCreatePropGrp
*/
xmlDavPropDesc *XmlDavCreatePropDesc(xmlDavCtx *dctx, xmlerr *err,...);

/*---------------------------------------------------------------------------
    NAME
        XmlDavCreatePropGrp

    PURPOSE
        Create a new property group object

    DESCRIPTION
        Create a new property group object.  Property group is consisted of
        four parts: namespace uri, prefix, natural language specification and 
        a list of name/value pairs.  Attributes "prop" and "prop_list" are
        exclusive. Specify only one of them.

        {{
        Currently supported name/value attribute pairs:

        ("namespace_uri", value of <oratext *> type in DAV's data encoding)
            The namespace_uri attribute identifies the namespace of a 
            property group is in.  This attribute is required.

        ("natural_language", value of <oratext *> type in DAV's data encoding)
            The natural_language attribute is used to indicate the natural 
            language of the intended audience of the property values in
            this group.  If set, "xml:lang" will be tagged at property values
            before sent to the server.

        ("prefix", value of <oratext *> type in DAV's data encoding)
            The "prefix" attribute identifies a prefix to go with the
            namespace uri as specified in "namespace_uri" attribute.  This
            attribute is required.  If "namespace_uri" attribute specifies
            DAV's namespace (i.e., "DAV:"), this attribute must have a value
            of "D".

        ("prop", value of <xmlDavPropDesc *> type>
            The prop attribute identifies a single property in the
            group.  The property descriptor is consisted of two parts:
            property's local name and potentially its value.

        ("prop_list", list of <xmlDavPropDesc *> type)
            The prop_list attribute identifies the list of properties located
            in the same namespace.  Each property is defined by a property 
            descriptor.

        }}
    
    EXAMPLE
        xmlDavResDesc     *res;
        xmlerr             xerr;
        xmlDavList        *propList;
        xmlDavPropDesc    *prop;
        xmlDavPropGrp     *propGrp;
        xmlDavResponse    *resp;
        ...
        // Create a list
        if (!(propList = XmlDavCreateList(dctx, &xerr)))
            return xerr;
        ...
        // Append properties to the list
        if (xerr = XmlDavListAppendObj(propList, (void *)prop))
            return xerr;
        ...
        // Create a property group object
        //
        //   Note that both "DAV:" and "en" need to be in DAV context's data
        //   encoding.  In this example, our encoding is assumed to be
        //   ISO-8859-1.
        if (!(propGrp = XmlDavCreatePropGrp(dctx, &xerr,
                                      "namespace_uri", "DAV:",
                                      "prefix", "D",  /o must be "D" for DAV's
                                                       o namespace
                                                       o/
                                      "natural_language", "en",
                                      "prop_list", propList,
                                      NULL)))
            return xerr;
            
        //
        if (xerr = XmlDavResGetProp(res, "prop_group", propGrp, 
                                         "response", &resp, NULL))
            return xerr;
        ...
        // Clean up
        if (propGrp)
            XmlDavDestroy(&xerr, "prop_group", propGrp, NULL);
        if (propList)
            XmlDavDestroy(&xerr, "list", propList, NULL);

    RETURNS
        (xmlDavPropGrp *) created property group object [or NULL on error]

    ARGUMENTS
        dctx   (IN)     DAV context
        err    (IN/OUT) returned error code
        ...    (IN)     variable arguments, with final NULL

    SEE ALSO
        XmlDavDestroy, XmlDavCreateList, XmlDavCreatePropDesc
*/
xmlDavPropGrp *XmlDavCreatePropGrp(xmlDavCtx *dctx, xmlerr *err,...);

/*---------------------------------------------------------------------------
    NAME
        XmlDavCreateReqDesc

    PURPOSE
        Create a new request descriptor to be used in a resource descriptor

    DESCRIPTION
        Create a new request descriptor.  This routine can be paired 
        with XmlDavDestroy to free up memory or other system resources.
        Multiple resources can share the same request descriptor.  However,
        application needs to make sure not to free a request descriptor until
        all resource descriptors that reference it have finished using it. 
        A request descriptor provides information that can help to formulate
        a request when a resource's method is invoked.  All attributes listed
        here have a one-to-one mapping to the HTTP header fields.  So, you
        can reference RFC 2616 and RFC 2617 for how to compose a valid value
        to be used in them.

        {{
        Currently supported name/value attribute pairs:

        ("Accept", value of <oratext *> type in compiler encoding)
            Set what media types are acceptable for a response. Note that
            this implementation understands text/xml and application/xml.
            If "Accept" attribute is specified and the returned entity
            body in the response is in media types other than "text/xml"
            and "application/xml", user can use XmlDavRespGetBody()
            to retrieve it.
        
        ("Accept-Language", value of <oratext *> type in compiler encoding) 
            Set what natural languages are acceptable for a response. 

        ("Authorization", value of <oratext *> type in compiler encoding) 
            Set authentication information of the user agent for the
            realm of the resource being requested. HTTP access authentication
            is described in "HTTP Authentication: Basic and Digest Access 
            Authentication" (RFC 2617). If a request is authenticated and 
            a realm specified, the same credentials SHOULD be valid for all 
            other requests within this realm (assuming that the 
            authentication scheme itself does not require otherwise, such 
            as credentials that vary according to a challenge value or using
            synchronized clocks).
        
        ("Cache-Control", value of <oratext *> type in compiler encoding) 
            Non-DAV compliant proxies will not honor the If header, 
            since they will not understand the If header, and HTTP 
            requires non-understood headers to be ignored.  When 
            communicating with HTTP/1.1 proxies, the "Cache-Control: 
            no-cache" request header MUST be used so as to prevent the 
            proxy from improperly trying to service the request from
            its cache. 

        ("Connection", value of <oratext *> type in compiler encoding)
            This Connection attribute allows the client to specify options
            that are desired for that particular connection and MUST NOT
            be communicated by proxies over further connections.

        ("Content-Language", value of <oratext *> type in compiler encoding)
            Set the natural language of the intended audience for the 
            entity body. 

        ("Content-Type", value of <oratext *> type in compiler encoding)
            The Content-Type attribute is used to indicate the media type of
            the entity-body sent to the server.  Example: 
            "text/html; charset=ISO-8859-4" (See section 3.7 in RFC 2616).

        ("Date", value of <oratext *> type in compiler encoding)
            This Date attribute is used by the system to provide the date and
            time at which the next method is to be invoked on a resource.  If
            there is no entity body in the next method, this attribute will be
            ignored.  This attribute is optional.

        ("Max-Forwards", value of ub4 integer)
            The max_forwards attribute may be used to target a specific
            proxy in the request chain. When a proxy receives this request
            on an absoluteURI for which request forwarding is permitted,
            the proxy MUST check for a Max-Forwards field. If the 
            Max-Forwards field-value is zero ("0"), the proxy MUST NOT 
            forward the message; instead, the proxy SHOULD respond with 
            its own communication options.  If the Max-Forwards field-value 
            is an integer greater than zero, the proxy MUST decrement the 
            field-value when it forwards the request. If no Max-Forwards 
            field is present in the request, then the forwarded request 
            MUST NOT include a Max-Forwards field.  This attribute is 
            optional.

        ("Pragma", value of <oratext *> type in compiler encoding) 
            Non-DAV compliant proxies will not honor the If header, 
            since they will not understand the If header, and HTTP 
            requires non-understood headers to be ignored.  When 
            communicating with HTTP/1.0 proxies, the "Pragma: 
            no-cache" request header MUST be used so as to prevent the 
            proxy from improperly trying to service the request from
            its cache. 

        ("Proxy-Authorization", value of <oratext *> type in compiler encoding)
            The Proxy-Authorization attribute allows the client to identify
            itself (or its user) to a proxy that requires authentication. 
            Its value consists of credentials containing the authentication 
            information of the user agent for the proxy and/or realm of 
            the resource being requested.  Unlike authorization attribute,
            the Proxy-Authorization attribute applies only to the next 
            outbound proxy that demanded authentication using the Proxy-
            Authenticate response header field. When multiple proxies are 
            used in a chain, the Proxy-Authorization attribute is consumed by 
            the first outbound proxy that was expecting to receive 
            credentials. A proxy MAY relay the credentials from the client 
            request to the next proxy if that is the mechanism by which the 
            proxies cooperatively authenticate a given request.

        ("referer_iri", value of <xmlDavIri *> type)
            The Referer attribute allows the client to specify the address
            of the resource from which the request uri was obtained.

        ("referer_uri", value of <xmlDavUri *> type)
            The Referer attribute allows the client to specify the address
            of the resource from which the request uri was obtained.

        ("Transfer-Encoding", value of <oratext *> type in compiler encoding)
            This Transfer-Encoding attribute is used to indicate that an 
            encoding transformation has been applied to the entity body in
            order to ensure "safe transport" through the network.  See also
            message_body attribute.

        ("User-Agent", value of <oratext *> type in compiler encoding)
            The User-Agent attribute contains information about the user
            agent originating the request.  The attribute can contain
            multiple product tokens and comments identifying the agent
            and any sub products which form a significant part of the 
            user agent.  Example: "CERN-LineMode/2.15 libwww/2.17b3".

        ("app_hdr", value of <xmlDavHdrDesc *> type)
            The app_hdr attribute can be used to provide an application-
            specific header.

        ("app_hdr_list", list of xmlDavHdrDesc objects)
            The app_hdr_list attribute can be used to provide a list of 
            application-specific headers.
        }}

    EXAMPLE
        xmlDavCtx     *dctx;
        xmlerr         xerr;
        xmlDavReqDesc *reqDesc;
        xmlDavResDesc *resDesc;
        ...
        if (!(reqDesc = XmlDavCreateReqDesc(dctx, &xerr, 
                            "User-Agent", "CERN-LineMode/2.15 libwww/2.17b3",
                            NULL)))
            return xerr;
        if (xerr = XmlDavUpdResDesc(resDesc, "request_descriptor", reqDesc,
                                    NULL)))
            return xerr;
        // All WebDAV operations come here //
        ...
        // Clean up
        if (reqDesc)
            XmlDavDestroy(&xerr, "request_descriptor", reqDesc, NULL);

    RETURNS
        (xmlDavReqDesc *) created xmlDavReqDesc [or NULL on error with err set]

    ARGUMENTS
        dctx   (IN)     DAV context
        err    (IN/OUT) returned error code
        ...    (IN)     variable arguments, with final NULL

    SEE ALSO
        XmlDavDestroy, XmlDavUpdReqDesc
*/
xmlDavReqDesc *XmlDavCreateReqDesc(xmlDavCtx *dctx, xmlerr *err, ...);

/*---------------------------------------------------------------------------
    NAME
        XmlDavCreateResDesc

    PURPOSE
        Create a new resource descriptor

    DESCRIPTION
        Create a new resource descriptor  to be used in all 
        resource-related operations.  This routine can be paired with 
        XmlDavDestroy to free up xmlDavResDesc and all its subordinate objects
        if no more resource operations on it are needed.  All response 
        objects belong to a resource descriptor that was referenced in the
        methods that generated them and are considered as a resource 
        descriptor's subordinate objects.

        {{
        Currently supported name/value attribute pairs:

        ("iri", value of <xmlDavIri *> type)
            This iri attribute is used by the system to identify the resource.
            Either this attribute or uri attribute, but not both, must be 
            specified.

        ("request_descriptor", value of <xmlDavReqDesc *> type)
            This request_descriptor attribute is used to provide information 
            that can help to formulate a request when a resource's method 
            is invoked. This attribute is optional.  If not specified at 
            creation time, it can still be set using XmlDavUpdResDesc() later.

        ("uri", value of <xmlDavUri *> type)
            This uri attribute is used by the system to identify the resource.
            Either this attribute or iri attribute, but not both, must be 
            specified.
        }}
    
    EXAMPLE
        xmlDavResDesc     *smallDir;
        xmlerr             xerr;
        xmlDavUri         *uri4SmallCol;
        xmlDavReqDesc     *sreq;
        ...
        // Create a resource descriptor
        if (!(smallDir = XmlDavCreateResDesc(dctx, &xerr, "uri", uri4SmallCol,
                               NULL)))
            return xerr;
        ...
        // Set request descriptor in the resource descriptor
        if (xerr = XmlDavUpdResDesc(smallDir, "request_descriptor", 
                                   sreq, NULL)))
            return xerr;
        // All resource-related operations come here 
        ...
        // Clean up
        if (smallDir)
            XmlDavDestroy(&xerr, "resource_descriptor", smallDir, NULL);
        if (sreq)
            XmlDavDestroy(&xerr, "request_descriptor", sreq, NULL);


    RETURNS
        (xmlDavResDesc *) created xmlDavResDesc [or NULL on error with err set]

    ARGUMENTS
        dctx   (IN)     DAV context
        err    (IN/OUT) returned error code
        ...    (IN)     variable arguments, with final NULL

    SEE ALSO
        XmlDavDestroy, XmlDavCreateReqDesc
*/
xmlDavResDesc *XmlDavCreateResDesc(xmlDavCtx *dctx, xmlerr *err, ...);

/*---------------------------------------------------------------------------
    NAME
        XmlDavCreateUri

    PURPOSE
        Create a new URI descriptor

    DESCRIPTION
        Create a new URI descriptor to be used in all resource-related 
        operations.

        {{
        Currently supported name/value attribute pairs:

        ("absolute_uri", value of <oratext *> type in compiler encoding)
            The absolute_uri attribute is a Uniform Resource Identifier and
            identifies the resource upon which to apply the request.

        }}
    
    EXAMPLE
        xmlDavResDesc     *smallDir;
        xmlerr             xerr;
        xmlDavUri         *uri4SmallCol;
        xmlDavReqDesc     *sreq;
        ...
        // Create a URI descriptor
        if (!(uri4SmallCol = XmlDavCreateUri(dctx, &xerr, 
                              "absolute_uri", "http://www.w3.org/pub/WWW/",
                              NULL)))
            return xerr;
        // Create a resource descriptor
        if (!(smallDir = XmlDavCreateResDesc(dctx, &xerr, "uri", uri4SmallCol,
                               NULL)))
            return xerr;
        ...
        // Set request descriptor in the resource descriptor
        if (xerr = XmlDavUpdResDesc(smallDir, "request_descriptor", 
                                   sreq, NULL)))
            return xerr;
        // All resource-related operations come here 
        ...
        // Clean up
        if (uri4SmallCol)
            XmlDavDestroy(&xerr, "uri", uri4SmallCol, NULL);
        if (smallDir)
            XmlDavDestroy(&xerr, "resource_descriptor", smallDir, NULL);
        if (sreq)
            XmlDavDestroy(&xerr, "request_descriptor", sreq, NULL);


    RETURNS
        (xmlDavUri *) created URI descriptor [or NULL on error with err set]

    ARGUMENTS
        dctx   (IN)     DAV context
        err    (IN/OUT) returned error code
        ...    (IN)     variable arguments, with final NULL

    SEE ALSO
        XmlDavDestroy, XmlDavCreateIri
*/
xmlDavUri *XmlDavCreateUri(xmlDavCtx *dctx, xmlerr *err, ...);

/*---------------------------------------------------------------------------
    NAME
        XmlDavCreateXmlDoc

    PURPOSE
        Create a DAV's document out of an XML document stored in a buffer

    DESCRIPTION
        This method can be used to create a DAV's document out of an 
        XML document stored in the input buffer.  The final data encoding
        of the DOM tree will be based on DAV's data encoding.  All other 
        relevant information (e.g., memory management, error handling, etc.)
        stored in DAV's context all also apply.  This method is added to 
        support a potential foreigh implementation of the underneath HTTP 
        layer.  For this kind of layering, user can still pass the entity 
        body of "text/xml" or "application/xml" type to this API to generate
        an XML DOM representation for further processing.

        If content type of the response is not either "text/xml" or 
        "application/xml", NULL is returned.  If an ill-formed XML is 
        encoded in a response, NULL is also returned.

        The memory space used in the DOM tree belongs to its parent--DAV's
        context.  To free the DOM tree, use XmlFreeDocument.
        {{
        Currently supported name/value attribute pairs:

        ("buffer", value of <oratext *> type)
            The "buffer" attribute is used to point to the XML document to 
            be parsed.  This could be the content of the entity body returned
            in the response.  This attribute is required.

        ("buffer_length", value of ubig_ora type)
            The "buffer_length" attribute is used to indicate the length of
            the input buffer holding the XML document.  This attribute is 
            required.

        ("default_input_encoding", value of <oratext *> type in compiler
            encoding)
            If the encoding of an input document cannot be automatically
            determined through BOM, XMLDecl, protocol header, etc, then
            this encoding will be assumed.

        ("input_encoding", value of <oratext *> type in compiler encoding)
            The forced input encoding for input documents.  Used to
            override a document's XMLDecl, etc, and always interpret
            it in the given encoding.  USE OF THIS FEATURE IS
            DISCOURAGED!  It should be not necessary in normal use,
            as BOMs, XMLDecls, etc, when existing, should be correct.

        }}

    RETURNS
        (xmlDavXmlDoc *) DAV's document on success

    ARGUMENTS
        dctx   (IN)     DAV context
        err    (IN/OUT) returned error code
        ...    (IN)     variable arguments, with final NULL

    SEE ALSO
        XmlDavDestroy, XmlDavRespGetXmlDoc 
*/
xmlDavXmlDoc *XmlDavCreateXmlDoc(xmlDavCtx *dctx, xmlerr *err, ...);

/*---------------------------------------------------------------------------
    NAME
        XmlDavDestroy

    PURPOSE
        Free a DAV object and all its subordinate objects

    DESCRIPTION
        Free a DAV object and all its subordinate objects that are not
        yet freed.  Consult the lineage relationship among DAV objects
        to find out what subordinate objects that each DAV object can have.
        For example, a resource descriptor's progeny includes response 
        objects and returned xmlDavXmlDoc's.  Data referenced from this object
        or its subordinate objects are NOT freed if they were provided by 
        the caller when objects were created.  To free any objects that 
        do not belong to DAV's implementation use the destroy method in its
        own package.  For example, to free an xmlelemnode, use XmlDomFreeNode.
        Multiple objects can be specified in a free operation and the order
        of freeing them is the same as the order they are specified on the
        argument list.  A DAV object cannot be freed twice.  For example,
        if a response object "A" belong to a resource "B", then "A" and "B"
        can only appear on the list simultaneously if "A" appears before "B". 

        {{
        Currently supported name/value attribute pairs:
        ("ace", value of <xmlDavAce *> type)
            The ace attribute is used to indicate a DAV access control element
            to be freed.
BEGIN_NO_DOC
        ("dav_buffer", value of <xmlDavBuf *> type)
            The dav_buffer attribute is used to indicate a DAV buffer 
            to be freed.
END_NO_DOC

        ("dav_context", value of <xmlDavCtx *> type)
            The dav_context attribute is used to indicate a DAV context 
            to be freed.

        ("dav_principal", value of <xmlDavPrin *> type)
            The dav_principal attribute is used to indicate a DAV principal 
            object to be freed.

        ("dav_privilege", value of <xmlDavPriv *> type)
            The dav_privilege attribute is used to indicate a DAV privilege 
            object to be freed.

        ("iri", value of <xmlDavIri *> type)
            The iri attribute is used to indicate a DAV IRI object 
            to be freed.

        ("list", value of <xmlDavList *> type)
            The list attribute is used to indicate a DAV list object 
            to be freed.
        
        ("condition", value of <xmlDavCond *> type)
            The condition attribute is used to indicate a DAV condition
            object to be freed.

        ("condition_factor", value of <xmlDavCondFac *> type)
            The condition_factor attribute is used to indicate a DAV condition
            factor object to be freed.

        ("condition_term", value of <xmlDavCondTerm *> type)
            The condition_term attribute is used to indicate a DAV condition
            term object to be freed.

        ("hdr_descriptor", value of <xmlDavHdrDesc *> type)
            The hdr_descriptor attribute is used to indicate a header 
            descriptor to be freed.

BEGIN_NO_DOC
        ("host_descriptor", value of <xmlDavHostDesc *> type)
            The host_descriptor attribute is used to indicate a host 
            descriptor to be freed.

END_NO_DOC
        ("http_request", value of <xmlDavHttpReq *> type)
            The http_request attribute is used to indicate an HTTP request 
            object to be freed.

        ("list", value of <xmlDavList *> type)
            The "list" attribute is used to indicate an DAV's list 
            object to be freed.

        ("prop_descriptor", value of <xmlDavPropDesc *> type)
            The prop_descriptor attribute is used to indicate a property 
            descriptor to be freed.

        ("prop_group", value of <xmlDavPropGrp *> type)
            The prop_group attribute is used to indicate a property group
            object to be freed.

        ("request_descriptor", value of <xmlDavReqDesc *> type)
            The request_descriptor attribute is used to indicate a request 
            descriptor to be freed.

        ("resource_descriptor", value of <xmlDavResDesc *> type)
            The resource_descriptor attribute is used to indicate a resource 
            descriptor to be freed.

        ("response", value of <xmlDavResponse *> type)
            The response attribute is used to indicate a response object
            to be freed.  After freeing, no reference to the response object
            itself or any data provided via XmlDavRespXxx APIs are allowed. 
            So, if the data returned by the XmlDavRespXxx APIs need to be 
            persistent, they need to be copied into user's buffer.

        ("uri", value of <xmlDavUri *> type)
            The uri attribute is used to indicate a DAV URI object to be 
            freed.

        ("xml_doc", value of <xmlDavXmlDoc *> type)
            The xml_doc attribute is used to indicate a DAV document object 
            to be freed.

        }}
    RETURNS
        (void)

    ARGUMENTS
        err    (IN/OUT) returned error code
        ...    (IN)     variable arguments, with final NULL

    SEE ALSO
        XmlDavCreate, XmlDavCreateResDesc, XmlDavCreateReqDesc
*/
void XmlDavDestroy(xmlerr *err, ...);

/*---------------------------------------------------------------------------
    NAME
        XmlDavEquals

    PURPOSE
        Check if two strings are equal 

    DESCRIPTION
        Return TRUE if two strings are equal.  Two strings are assumed to be 
        in compiler encoding.

    RETURNS
        (boolean)   TRUE if equal; FALSE, otherwise

    ARGUMENTS
        str1   (IN)     string 1
        str2   (IN)     string 2
        caseSensitive (IN) TRUE if case is sensitive; FALSE, otherwise 
*/
boolean XmlDavEquals(oratext *str1, oratext *str2, boolean caseSensitive);

/*---------------------------------------------------------------------------
    NAME
        XmlDavGetHttpReq

    PURPOSE
        Retrieve individual fields from an HTTP request

    DESCRIPTION
        Retrieve individual fields from an HTTP request.  Then, user can
        use the information to do the following:
        {{
        1. Make a connection to peer (using peer_host and peer_port)
        2. For HTTPS scheme, 
              2.1) if a proxy is used:
                   Send a CONNECT method through the connection, and
                     (using srv_host and srv_port)
                   Wait until tunneling is established
              2.2) Start SSL (using wallet information)
        3. Send HTTP request head (including HTTP request line and headers)
            through the connection
        4. Send HTTP message body through the connection if it exists 

        Currently supported name/value attribute pairs:

        ("peer", address of a pointer of <xmlDavHostDesc *> type)
            The attribute "peer" can be used to retrieve the peer information.
            An HTTP connection is to be set up between client and peer to 
            begin a session.  "peer" could be either a proxy or the direct
            remote server that owns the requested resource.  See also 
            "use_proxy" attribute.
        ("request_head", address of a pointer of <oratext *> type)
            The "request_head" attribute can be used to retrieve the content
            of the HTTP head which is composed of an HTTP request line and
            HTTP headers.
        ("request_head_length", address of a ub4 variable)
            The "request_head_length" attribute can be used to retrieve the
            length of the HTTP request head (as returned in the "request_head"
            attribute).
        ("request_body", address of a pointer of <oratext *> type)
            The "request_body" attribute can be used to retrieve the content
            of the HTTP message body.
        ("request_body_length", address of a ubig_ora variable)
            The "request_body_length" can be used to retrieve the length
            of the message body as returned in the "request_body" attribute.
        ("server", address of a pointer of <xmlDavHostDesc *> type)
            The attribute "server" can be used to retrieve the remote server
            information.  "peer" and "server" will be different when a proxy
            is used in the connection.  Otherwise, they are the same.
        ("use_proxy", address of a boolean variable)
            The attribute "use_proxy" can be used to retrieve a boolean
            variable.  If the value of the boolean vairable is TRUE, it means
            that a proxy is to be used in the connection.
        ("use_ssl", address of a boolean vairable)
            The attribute "use_ssl" can be used to retrieve a boolean
            variable.  If the value of the boolean vairable is TRUE, it means
            an SSL session is to be set up.
        
        }}

    RETURNS
        (xmlerr) numeric error code

    ARGUMENTS
        req   (IN)   HTTP request
        ...   (IN)   variable arguments, with final NULL

    SEE ALSO
        xmlDavHttpReq
*/
xmlerr XmlDavGetHttpReq(xmlDavHttpReq *req, ...);

/*---------------------------------------------------------------------------
    NAME
        XmlDavGetXmlCtx

    PURPOSE
        Retrieve XML context (i.e., xmlctx) from a DAV's context

    DESCRIPTION
        Retrieve the XML context (i.e., xmlctx) from a DAV's context.

    RETURNS
        (xmlctx *) returned XML context [or NULL on error with err set]

    ARGUMENTS
        dctx   (IN)     DAV context
        err    (IN/OUT) returned error code

    SEE ALSO
        XmlDavCreate
*/
xmlctx *XmlDavGetXmlCtx(xmlDavCtx *dctx, xmlerr *err);

/*---------------------------------------------------------------------------
    NAME
        XmlDavUpdReqDesc

    PURPOSE
        Update information stored in a request descriptor

    DESCRIPTION
        Update information stored in a request descriptor.  After update, the
        new information will affect all new methods invoked using it.  If
        request descriptor is shared, it will affect all resources that share
        it.

        {{
        Currently supported name/value attribute pairs:

        ("Accept", value of <oratext *> type in compiler encoding)
            Set what media types are acceptable for a response. Note that
            this implementation understands text/xml and application/xml.
            If "Accept" attribute is specified and the returned entity
            body in the response is in media types other than "text/xml"
            and "application/xml", user can use XmlDavRespGetBody()
            to retrieve it.
        
        ("Accept-Language", value of <oratext *> type in compiler encoding) 
            Set what natural languages are acceptable for a response. 

        ("Authorization", value of <oratext *> type in compiler encoding) 
            Set authentication information of the user agent for the
            realm of the resource being requested. HTTP access authentication
            is described in "HTTP Authentication: Basic and Digest Access 
            Authentication" (RFC 2617). If a request is authenticated and 
            a realm specified, the same credentials SHOULD be valid for all 
            other requests within this realm (assuming that the 
            authentication scheme itself does not require otherwise, such 
            as credentials that vary according to a challenge value or using
            synchronized clocks).
        
        ("Cache-Control", value of <oratext *> type in compiler encoding) 
            Non-DAV compliant proxies will not honor the If header, 
            since they will not understand the If header, and HTTP 
            requires non-understood headers to be ignored.  When 
            communicating with HTTP/1.1 proxies, the "Cache-Control: 
            no-cache" request header MUST be used so as to prevent the 
            proxy from improperly trying to service the request from
            its cache. 

        ("Connection", value of <oratext *> type in compiler encoding)
            This Connection attribute allows the client to specify options
            that are desired for that particular connection and MUST NOT
            be communicated by proxies over further connections.

        ("Content-Language", value of <oratext *> type in compiler encoding)
            Set the natural language of the intended audience for the 
            entity body. 

        ("Content-Type", value of <oratext *> type in compiler encoding)
            The Content-Type attribute is used to indicate the media type of
            the entity-body sent to the server.  Example: 
            "text/html; charset=ISO-8859-4" (See section 3.7 in RFC 2616).

        ("Date", value of <oratext *> type in compiler encoding)
            This Date attribute is used by the system to provide the date and
            time at which the next method is to be invoked on a resource.  If
            there is no entity body in the next method, this attribute will be
            ignored.  This attribute is optional.

        ("Max-Forwards", value of ub4 integer)
            The max_forwards attribute may be used to target a specific
            proxy in the request chain. When a proxy receives this request
            on an absoluteURI for which request forwarding is permitted,
            the proxy MUST check for a Max-Forwards field. If the 
            Max-Forwards field-value is zero ("0"), the proxy MUST NOT 
            forward the message; instead, the proxy SHOULD respond with 
            its own communication options.  If the Max-Forwards field-value 
            is an integer greater than zero, the proxy MUST decrement the 
            field-value when it forwards the request. If no Max-Forwards 
            field is present in the request, then the forwarded request 
            MUST NOT include a Max-Forwards field.  This attribute is 
            optional.

        ("Pragma", value of <oratext *> type in compiler encoding) 
            Non-DAV compliant proxies will not honor the If header, 
            since they will not understand the If header, and HTTP 
            requires non-understood headers to be ignored.  When 
            communicating with HTTP/1.0 proxies, the "Pragma: 
            no-cache" request header MUST be used so as to prevent the 
            proxy from improperly trying to service the request from
            its cache. 

        ("Proxy-Authorization", value of <oratext *> type in compiler encoding)
            The Proxy-Authorization attribute allows the client to identify
            itself (or its user) to a proxy that requires authentication. 
            Its value consists of credentials containing the authentication 
            information of the user agent for the proxy and/or realm of 
            the resource being requested.  Unlike authorization attribute,
            the Proxy-Authorization attribute applies only to the next 
            outbound proxy that demanded authentication using the Proxy-
            Authenticate response header field. When multiple proxies are 
            used in a chain, the Proxy-Authorization attribute is consumed by 
            the first outbound proxy that was expecting to receive 
            credentials. A proxy MAY relay the credentials from the client 
            request to the next proxy if that is the mechanism by which the 
            proxies cooperatively authenticate a given request.

        ("referer_uri", value of <xmlDavUri *> type)
            The Referer attribute allows the client to specify the address
            of the resource from which the request uri was obtained.

        ("referer_iri", value of <xmlDavIri *> type)
            The Referer attribute allows the client to specify the address
            of the resource from which the request uri was obtained.

        ("Transfer-Encoding", value of <oratext *> type in compiler encoding)
            This Transfer-Encoding attribute is used to indicate that an 
            encoding transformation has been applied to the entity body in
            order to ensure "safe transport" through the network.  See also
            message_body attribute.

        ("User-Agent", value of <oratext *> type in compiler encoding)
            The User-Agent attribute contains information about the user
            agent originating the request.  The attribute can contain
            multiple product tokens and comments identifying the agent
            and any sub products which form a significant part of the 
            user agent.  Example: "CERN-LineMode/2.15 libwww/2.17b3".
                
        ("app_hdr", value of <xmlDavHdrDesc *> type)
            The app_hdr attribute can be used to provide an application-
            specific header.  If specified, the existing application-specific
            headers are all replaced.

        ("app_hdr_list", list of <xmlDavHdrDesc *> type)
            The app_hdr_list attribute can be used to provide application-
            specific headers.  If specified, the existing application-specific
            headers are all replaced.

        }}

    RETURNS
        (xmlerr) error code

    ARGUMENTS
        desc (IN/OUT) request descriptor to be updated
        ...  (IN)  variable arguments, with final NULL

    SEE ALSO
        xmlDavReqDesc, XmlDavDestroy
*/
xmlerr XmlDavUpdReqDesc(xmlDavReqDesc *desc, ...);

/*---------------------------------------------------------------------------
    NAME
        XmlDavUpdResDesc

    PURPOSE
        Update information stored in a resource descriptor

    DESCRIPTION
        Update information stored in a resource descriptor (i.e., 
        xmlDavResDesc).  No round-trip communication to the server is involved.

        {{
        Currently supported name/value attribute pairs:

        ("Etag", value of <oratext *> type in compiler encoding) 
            This Etag attribute sets the etag for the resource.  Etag header
            can be retrieved from the responses received with XmlDavResPut or
            can be retrieved from the DAV:getetag property of the resource.
            The ETag represents the state of the body or contents of the 
            resource.  There is no similar way to tell if properties have
            changed.

        ("request_descriptor", value of <xmlDavReqDesc *>)
            This ctx attribute is used to provide information that can help 
            to formulate a request when a resource's method is invoked.

        ("Lock-Token", value of <oratext *> type in compiler encoding)
            This Lock-Token attribute sets the lock token for the resource.

        }}

    RETURNS
        (xmlerr) numeric error node

    ARGUMENTS
        res  (IN) resource whose descriptor is to be updated
        ...  (IN) variable arguments, with final NULL

    SEE ALSO
        XmlDavCreateResDesc, XmlDavDestroy
*/
xmlerr XmlDavUpdResDesc(xmlDavResDesc *res, ...);

/****************************************************************************
                            Interface Resource
 ****************************************************************************/

/*---------------------------------------------------------------------------
    NAME
        XmlDavResAcl

    PURPOSE
        modify the access control list of a resource

    DESCRIPTION
        An access control element (ACE) grants or denies a particular  set
        of (non-abstract) privileges for a particular principal.  

        The ACL method modifies the access control list (which can be read
        via the DAV:acl property) of a resource. Specifically, the ACL method
        only permits modification to ACEs that are not inherited, and are not
        protected. An ACL method invocation modifies all non-inherited and 
        non-protected ACEs in a resource's access control list to exactly 
        match the ACEs contained within in the DAV:acl XML element of the 
        request body. 

        ACE(s) can be provided via attributes: "ace" or "ace_list".  These two
        attributes are exclusive.  Specify only one of them.  All XML elements
        within the ACE declarations inherit the DAV's namespace declaration
        (i.e., "DAV:").  So, all XML sub-elements in the DAV's namespace need
        not to redeclare DAV's namespace (i.e., "DAV:") and must use "D" as 
        their prefixes.

        It is possible that the ACEs visible to the current user in the 
        DAV:acl property may only be a portion of the complete set of ACEs 
        on that resource. If this is the case, an ACL request only modifies
        the set of ACEs visible to the current user, and does not affect 
        any non-visible ACE.

        In order to avoid overwriting DAV:acl changes by another client, a 
        client SHOULD acquire a WebDAV lock on the resource before retrieving
        the DAV:acl property of a resource that it intends on updating.

        Two common operations are to add or remove an ACE from an existing 
        access control list. To accomplish this, a client uses the PROPFIND 
        method to retrieve the value of the DAV:acl property, then parses 
        the returned access control list to remove all inherited and 
        protected ACEs (these ACEs are tagged with the DAV:inherited and
        DAV:protected XML elements). In the remaining set of non-inherited, 
        non-protected ACEs, the client can add or remove one or more ACEs 
        before submitting the final ACE set in the request body of the ACL 
        method.


        {{
        Currently supported name/value attribute pairs:

        ("ace", value of <xmlDavAce*> type)
            The "ace" attribute is used to specify an access control element.

        ("ace_list", list of <xmlDavAce *> type)
            The "ace_list" attribute is used to provide a list of access 
            control elements.

        ("http_request", address of pointer of <xmlDavHttpReq *> type)
            The http_request attribute is used to provide user the final
            constructed request message in some system-generated buffer(s).
            See also XmlDavGetHttpReq().

        ("precondition", value of <xmlDavCond *> type)
            This precondition attribute provides state tokens or entity
            tags needed for a successful execution of this method.

        ("response", address of pointer of <xmlDavResponse *> type)
            The response attribute is used to provide user the response from
            the server.  The returned value can be NULL if error occurs.

        }}

    RETURNS
        (xmlerr) numeric error code

    ARGUMENTS
        res  (IN)  resource to be created or modified
        ...  (IN)  variable arguments, with final NULL

    SEE ALSO
        XmlDavCreateResDesc, XmlDavUpdResDesc, XmlDavDestroy
*/
xmlerr XmlDavResAcl(xmlDavResDesc *res, ...);

/*---------------------------------------------------------------------------
    NAME
        XmlDavResCopy

    PURPOSE
        Copy a resource from source to destination

    DESCRIPTION
        This method creates a duplicate of the source resource at the location
        specified with "destination_uri/iri" attribute.  After copy, any write
        locks active on the source will not be duplicated at destination. And,
        subsequent alterations to the destination resource will not modify 
        the source resource.  Subsequent alterations to the source resource 
        will not modify the destination resource. If the client wishes to 
        access newly created destination resource, use XmlCreateResDesc() to
        create a new resource descriptor for it.

        {{
        Currently supported name/value attribute pairs:

        ("Depth", value of xmlDavDepth type)
            The Depth attribute is used to specify what level of children
            are to be copied.  Valid values allowed are: XMLDAV_DEPTH_0 and
            XMLDAV_DEPTH_INF.  The default value is XMLDAV_DEPTH_INF.

        ("Overwrite", value of boolean type)
            If a resource exists at the destination and the Overwrite 
            attribute is TRUE then prior to performing the copy the server
            MUST perform a DELETE with "Depth: infinity" on the destination 
            resource.  If the Overwrite attribute is set to FALSE then the 
            operation will fail. If not specified, default is TRUE.

        ("destination_iri", value of <xmlDavIri *> type)
            The destination_iri specifies the IRI which identifies a
            destination resource.  This must be an absolute IRI.  If the 
            source and destination resource are on different servers, this
            method may fail with a HTTP 502 (Bad Gateway) response.  Either 
            this attribute or destination_uri, but not both, must be specified.

        ("destination_uri", value of <xmlDavUri *> type)
            The destination_uri specifies the URI which identifies a
            destination resource.  This must be an absolute URI.  If the 
            source and destination resource are on different servers, this
            method may fail with a 502 (Bad Gateway) response.  Either this
            attribute or destination_iri, but not both, must be specified.

        ("http_request", address of pointer of <xmlDavHttpReq *> type)
            The http_request attribute is used to provide user the final
            constructed request message in some system-generated buffer(s).
            See also XmlDavGetHttpReq().

        ("precondition", value of <xmlDavCond *> type)
            This precondition attribute provides state tokens or entity
            tags needed for a successful execution of this method.

        ("response", address of pointer of <xmlDavResponse *> type)
            The response attribute is used to provide user the response from
            the server.  The returned value can be NULL if error occurs.
        }}

    RETURNS
        (xmlerr)   numeric error code

    ARGUMENTS
        res  (IN)  resource to be copied
        ...  (IN)  variable arguments, with final NULL

    SEE ALSO
        xmlDavDepth, XmlDavCreateResDesc
*/
xmlerr XmlDavResCopy(xmlDavResDesc *res, ...);

/*---------------------------------------------------------------------------
    NAME
        XmlDavResDelete

    PURPOSE
        Delete this resource and potentially all its members from the server

    DESCRIPTION
        This method deletes a resource from the server.  If the resource is
        a collection resource, delete it and all its internal members.  If
        any resource identified by a member URL cannot be deleted then all
        the member's ancestors will not be deleted.

        {{
        Currently supported name/value attribute pairs:

        ("http_request", address of pointer of <xmlDavHttpReq *> type)
            The http_request attribute is used to provide user the final
            constructed request message in some system-generated buffer(s).
            See also XmlDavGetHttpReq().

        ("precondition", value of <xmlDavCond *> type)
            This precondition attribute provides state tokens or entity
            tags needed for a successful execution of this method.

        ("response", address of pointer of <xmlDavResponse *> type)
            The response attribute is used to provide user the response from
            the server.  The returned value can be NULL if error occurs.
        }}

    RETURNS
        (xmlerr)   numeric error code

    ARGUMENTS
        res  (IN)  resource to be deleted
        ...  (IN)  variable arguments, with final NULL

    SEE ALSO
        xmlDavDepth, XmlDavResType
*/
xmlerr XmlDavResDelete(xmlDavResDesc *res, ...);

/*---------------------------------------------------------------------------
    NAME
        XmlDavResGet

    PURPOSE
        Retrieve information in the form of an entity from a resource

    DESCRIPTION
        Retrieve whatever information (in the form of an entity) is 
        identified by the resource descriptor. XmlDavResGet when applied 
        to a collection may return the contents of an "index.html" 
        resource, a human-readable view of the contents of the collection,
        or something else altogether by the server.
        
        {{
        Currently supported name/value attribute pairs:

        ("http_request", address of pointer of <xmlDavHttpReq *> type)
            The http_request attribute is used to provide user the final
            constructed request message in some system-generated buffer(s).
            See also XmlDavGetHttpReq().

        ("response", address of pointer of <xmlDavResponse *> type)
            The response attribute is used to provide user the response from
            the server.  The returned value can be NULL if error occurs.

        }}

    RETURNS
        (xmlerr) numeric error code

    ARGUMENTS
        res  (IN)  resource descriptor
        ...  (IN)  variable arguments, with final NULL

    SEE ALSO
        XmlDavResPut, XmlDavResHead, xmlDavResDesc
*/
xmlerr XmlDavResGet(xmlDavResDesc *res, ...);

/*---------------------------------------------------------------------------
    NAME
        XmlDavResGetProp

    PURPOSE
        Retrieve properties defined on the resource

    DESCRIPTION
        This method retrieves properties defined on the resource
        and potentially its member resources.  Note that attributes
        "propname", "allprop", and "prop_group" are exclusive.
        Specify only one of them.  However, attribute "deadprops"
        can be combined with "prop_group" to retrieve specific live 
        properties.

        {{
        Currently supported name/value attribute pairs:

        ("allprop", value of boolean type)
            The allprop attribute is used to specify that all property
            names and values on the resource are to be returned.

        ("deadprops", value of boolean type)
            The deadprops attribute is used to specify that all dead
            property names and values on the resource are to be returned. Note
            that a request using this attribute is only supported on "bis"
            compliant servers.

        ("Depth", value of xmlDavDepth type)
            The Depth attribute is used to specify what level of children
            whose properties are to be returned.  The default value is 
            XMLDAV_DEPTH_INF.

        ("http_request", address of pointer of <xmlDavHttpReq *> type)
            The http_request attribute is used to provide user the final
            constructed request message in some system-generated buffer(s).
            See also XmlDavGetHttpReq().

        ("prop_group", value of <xmlDavPropGrp *> type)
            The prop_group attribute is used to specify that a list of
            properties in a group whose name and value are to be 
            returned.

        ("propname", value of boolean type)
            The propname attribute is used to specify that only a list 
            of property names subject to Depth attribute value on 
            the resource is to be returned.

        ("response", address of pointer of <xmlDavResponse *> type)
            The response attribute is used to provide user the response from
            the server.  The returned value can be NULL if error occurs.
        }}


    EXAMPLE
        xmlDavCtx        *dctx;
        xmlerr            xerr;
        xmlDavPropDesc   *propDesc;
        xmlDavPropGrp    *propGrp;
        xmlDavResDesc    *res;
        oratext          *nameInDav;  // name in DAV context's data encoding

        // Create a property descriptor
        //
        //   Note that "displayname" must be in DAV context's data encoding.  
        if (!(propDesc = XmlDavCreatePropDesc(dctx, &xerr,
                                      "local_name", "displayname",
                                      NULL)))
            return xerr;
        ...
        // Create a property group object
        //
        //   Note that namespace "DAV:" needs to be in DAV 
        //   context's data encoding. 
        if (!(propGrp = XmlDavCreatePropGrp(dctx, &xerr,
                                      "namespace_uri", "DAV:",
                                      "prefix", "D", /o this must be "D" if
                                                      o it maps to DAV's 
                                                      o namespace
                                                      o/
                                      "prop", propDesc,
                                      NULL)))
            return xerr;
        ...
        nameInData = NULL;
        // Get DAV:displayname property
        if (!XmlDavResGetProp(resDesc, "prop_group", propGrp, 
                                       "response", &resp, 
                                       NULL))
        {
            if (resp)    // resp can be NULL
            {
                statusCode = XmlDavRespGetCode(resp);
                switch (statusCode)
                {
                case 207:    // Multi-status
                case 200:    // OK
                    ddoc = XmlDavRespGetXmlDoc(resp, &xerr);
                    if (ddoc)
                    {
                        xmlnodelist *nodeList;
                        xmldocnode  *doc = XmlDavDocGetNode(ddoc, &xerr);
                        xmlnode     *node;
                        ...
                        // Use the meta context that was used to create
                        // the DAV context here
                        nodeList = XmlDomGetDocElemsByTagNS(xctx, doc, 
                            "DAV:", "displayname");
                        node = XmlDomGetNodeListItem(xctx, nodeList, 0);
                        if (XmlDomHasChildNodes(xctx, node))
                        {
                            childList = XmlDomGetChildNodes(xctx, node);
                            textNode = XmlDomGetNodeListItem(xctx, childList,
                                                             0);
                            nameInData = XmlDomGetNodeValue(xctx, textNode);
                            XmlDomFreeNodeList(xctx, childList);
                        }
                        XmlDomFreeNodeList(xctx, nodeList);
                    }
                    break;
                ...
                default:
                    break;
                }
            }
        }

    RETURNS
        (xmlerr)   numeric error code

    ARGUMENTS
        res  (IN)  resource to be queried
        ...  (IN)  variable arguments, with final NULL

    SEE ALSO
        XmlDavResType, xmlDavDepth
*/
xmlerr XmlDavResGetProp(xmlDavResDesc *res, ...);

/*---------------------------------------------------------------------------
    NAME
        XmlDavResHead

    PURPOSE
        Retrieve metainformation from a resource without an entity body

    DESCRIPTION
        This method can be used for obtaining metainformation about the 
        entity implied by the request without transferring the entity-body 
        itself.  For example, this method can be used to retrieve entity
        tag by checking the ETag response header field.
        
        {{
        Currently supported name/value attribute pairs:

        ("http_request", address of pointer of <xmlDavHttpReq *> type)
            The http_request attribute is used to provide user the final
            constructed request message in some system-generated buffer(s).
            See also XmlDavGetHttpReq().

        ("response", address of pointer of <xmlDavResponse *> type)
            The response attribute is used to provide user the response from
            the server.  The returned value can be NULL if error occurs.

        }}
    
    EXAMPLE
        xmlerr          xerr;
        xmlDavResponse *resp;
        oratext        *etagInComp;    // Etag in compiler encoding 

        if (xerr = XmlDavResHead(res, "response", &resp, NULL))
            return xerr;
        etagInComp = NULL;
        if (resp)    // resp can be NULL
        {
            oratext    *hdrNam;
            sb4         statusCode;
            ub4         index;

            statusCode = XmlDavRespGetCode(resp, &xerr);
            switch (statusCode)
            {
            case 204:    // OK
                index = 0;
                while (hdrNam = XmlDavRespGetHdrNam(resp, &xerr, index))
                {
                    if (XmlDavEquals(hdrNam, (oratext *) "Etag", FALSE))
                    {
                        etagInComp = XmlDavRespGetHdrVal(resp, &xerr, index);
                    }
                    index++;
                }
                break;
            default:
                break;
            }
        }


    RETURNS
        (xmlerr) numeric error code

    ARGUMENTS
        res  (IN)  resource descriptor
        ...  (IN)  variable arguments, with final NULL

    SEE ALSO
        XmlDavResPut, XmlDavResGet, xmlDavResDesc
*/
xmlerr XmlDavResHead(xmlDavResDesc *res, ...);

/*---------------------------------------------------------------------------
    NAME
        XmlDavResType

    PURPOSE
        Return a resource's type

    DESCRIPTION
        Returns XMLDAV_RT_COL if the given resource is a collection. Returns
        XMLDAV_RT_RSC if the given resource is a regular resource.  Otherwise,
        XMLDAV_RT_UNKNOWN is returned if error occurs.  After query, this 
        attribute will be recorded in the resource.  If a resource is not 
        a collection, "Depth" attribute is not allowed in some resource
        methods.

        {{
        Currently supported name/value attribute pairs:

        ("callback", value of XMLDAV_HTTP_RESP1_F type)
            The callback attribute is used to provide a user-defined 
            response function callback. Within XmlDavResType, it formulates
            an HTTP request, sends the request to destination URI, and
            receives response from the server.  If this API is built on top
            of a privately-implemented HTTP server, user can provide a 
            response callback to handle the above-mentioned tasks. If this
            callback is not provided, HTTP server provided by CORE will be
            utilized instead.
        }}
        
    RETURNS
        (xmlDavRscTyp) resoruce type

    ARGUMENTS
        res  (IN)     resource to be queried
        err  (IN/OUT) returned error code
        ...  (IN)     variable arguments, with final NULL

    SEE ALSO
        XmlDavResMkCol, XmlDavResPut, xmlDavRscTyp
*/
xmlDavRscTyp XmlDavResType(xmlDavResDesc *res, xmlerr *err, ...);

/*---------------------------------------------------------------------------
    NAME
        XmlDavResLock

    PURPOSE
        Take out a lock of any access type or to refresh an existing lock

    DESCRIPTION
        This method is used to take out a lock of any access type or to
        refresh an existing lock.  The resource will become the root of the
        lock at a successful execution.  
        
        If a new lock is to be created, owner attribute and scope attribute
        are required. The owner attribute identifies the owner of the lock.
        It could be an href to a home page, an email address, phone number, 
        etc. Owner attribute can be specified in one of the following format:
        owner_elem, owner_elem_list or owner_text. Internally, this 
        implementation, check whether "owner" and "scope" attributes are
        specified or not.  If both of them are not specified, it will treat 
        this invokation as a lock refreshing invokation.

        If an existing lock is to be refreshed, the attribute "Lock-Token" 
        must be set in xmlDavResDesc using XmlDavUpdResDesc().  Also, set
        "use_if_header" attribute to be TRUE, if you want this implementation
        to construct an "If" header instead of "Lock-Token" Header.

        If a new lock is successfully created, a new lock token can be 
        retrieved from the Lock-Token response header.  The scope of a lock 
        is the entire state of the resource, including its body and 
        associated properties.

        For collections, a lock also affects the ability to add or remove 
        members.  The nature of the effect depends upon the type of access 
        control involved.  This means that if a collection is locked, its 
        lock-token is required in all these cases: 
        {{
            - DELETE a collection's direct  internal member 
            - MOVE a member out of the collection 
            - MOVE a member into the collection, unless it overwrites a pre-
              existing member 
            - MOVE to rename it within a collection, 
            - COPY a member into a collection, unless it overwrites a pre-
              existing member 
            - PUT or MKCOL request that would create a new member.   
        }}
        The collection's lock token is required in addition to the lock 
        token on the internal member itself, if it exists. 
        
        To provide lock token(s) in this API, either "precondition" 
        attribute needs to be set in each method that may require a lock 
        token for its successful execution or Lock-Token attribute needs to
        be set in a resource with XmlDavUpdResDesc() when XmlDavResUnlock() 
        is invoked.

        Lock support in server is optional.  A client can determine
        what lock types the server supports by retrieving the supportedlock
        property.  Locks are associated with a principal, not a program.  If
        another principal locks a resource that a principal wishes to access,
        the second principal can find out who the first principal is by 
        retrieving lockdiscovery property.  Note that it is illegal for a 
        principal to request the same lock twice.

        If a resource is "bis" compliant, a locking on an unmapped URLs 
        will result in the creation of an empty resource that is locked
        (and which is not a collection).  Later on, the lock may go away but 
        the empty resource remains.  The client is expected to update the 
        locked empty resource shortly after locking it, using XmlDavResPut
        and possibly XmlDavResUpdProp.  When the client uses XmlDavResPut to
        overwrite a locked empty resource the client MUST supply a 
        Content-Type if any is known.

        If a user-created XML element node is provided via "owner_elem"
        or "owner_elem_list", its namespace declaration must be 
        appropriately specified.  If the new element is in DAV's namespace,
        its prefix must be chosen to be "D".  For example, a owner element's
        content may look like:
             <D:href>http://www.ics.uci.edu/~ejw/contact.html</D:href>.
        Otherwise, a namespace declaration attribute must be provided in the
        new element.  For example, a owner element's content may look like:
             <Z:contact xmlns:Z="boo">Stanley Guan</Z:contact>.

        {{
        Currently supported name/value attribute pairs:

        ("Depth", value of xmlDavDepth type)
            The Depth attribute is used to specify what level of children
            are to be locked.  Valid values allowed are: XMLDAV_DEPTH_0 and
            XMLDAV_DEPTH_INF.  This attribute is optional.  If not set, the
            default is XMLDAV_DEPTH_INF.  A zero-depth lock on a collection
            affects changes to the direct membership of that collection.
        
        ("http_request", address of pointer of <xmlDavHttpReq *> type)
            The http_request attribute is used to provide user the final
            constructed request message in some system-generated buffer(s).
            See also XmlDavGetHttpReq().

        ("lock_timeout", value of ubig_ora type)
            The lock_timeout attribute is used to indicate the number of 
            seconds before the lock times out or 0 for infinite timeout.  
            This attribute is optional.

        ("lock_type", value of xmlDavLockType type)
            The lock_type attribute is used to indicate the access type of a
            lock. Currently only one type is allowed: XMLDAV_LOCKTYPE_WRITE.

        ("owner_elem", value of <xmlelemnode *> type in DAV's data encoding)
            The owner_elem attribute is used to specify the content of an
            "owner" XML element is to be a single XML sub-element node.  If
            specified, xmlctx attribute must be also specified.

        ("owner_elem_list", list of xmlelemnode in DAV's data encoding)
            The owner_elem_list attribute is used to specify the content of 
            an "owner" XML element is to be a list of XML sub-element nodes. 
            For example: 
                <D:href>http://www.ics.uci.edu/~ejw/contact.html</D:href>
                <Z:workphone>650-888-1111</Z:workphone>
            If specified, xmlctx attribute must be also specified.

        ("owner_text", value of <oratext *> type in DAV's data encoding)
            The owner_text attribute is used to specify the content of an
            "owner" XML element is to be text only. 
                      
        ("response", address of pointer of <xmlDavResponse *> type)
            The response attribute is used to provide user the response from
            the server.  The returned value can be NULL if error occurs.

        ("scope", value of xmlDavLockScope type)
            The scope attribute is used to indicate the scope of the lock.
            Valid values are: XMLDAV_LOCKSCOPE_EXCLUSIVE or 
            XMLDAV_LOCKSCOPE_SHARED.  Default is XMLDAV_LOCKSCOPE_SHARED if
            it's a lock creating command.

        ("use_if_header", value of boolean type)
            The use_if_header attribute is used to indicate an "If" header,
            instead of "Lock-Token" header, should be sent for a lock 
            refreshing.  "bis" compliant implementations will support 
            "Lock-Token" header in a lock refreshing method.  But, for
            backward compatibility, "If" header could be chosen to be 
            sent instead.  The default is FALSE.

        ("xmlctx", value of <xmlctx *> type)
            The xmlctx attribute is used to specify the xmlctx used to 
            create element nodes provided in owner_elem or owner_elem_list
            attributes.

        }}

    EXAMPLE
        xmlctx        *xctx;
        xmlDavCtx     *dctx;
        xmlerr         xerr;
        xmldocnode    *doc;
        xmltextnode   *info;
        ...
        if (!(dctx = XmlDavCreate(xctx, &xerr, "max_xml_size", 10, NULL)))
            return xerr;
        doc = XmlCreateDocument(xctx, (oratext *)0, (oratext *)0, 
                                (xmldtdnode *)0, &xerr);
        ...
        // Note that both "DAV:" and "D:href" need to be in DAV context's data
        // encoding.  In this example, our encoding is assumed to be
        // ISO-8859-1.
        ownerInfo = XmlDomCreateElemNS(xctx, doc, "DAV:", "D:href");
        ...
        // "http://www.ics.uci.edu/~ejw/contact.html" also needs to be in
        // DAV context's data encoding.
        info = XmlDomCreateTextNode(xctx, doc, 
                             "http://www.ics.uci.edu/~ejw/contact.html");
        DISCARD XmlDomAppendChild(xctx, ownerInfo, info);
        ...
        // must provide namespace declaration 
        XmlDomSetAttrNS(xctx, ownerInfo, "DAV:", "xmlns:D", "DAV:");
        ....
        //
        if (xerr = XmlDavResLock(res, "owner_elem", ownerInfo, 
                           "xmlctx", xctx,
                           "scope", XMLDAV_LOCKSCOPE_EXCLUSIVE,
                           "lock_type",  XMLDAV_LOCKTYPE_WRITE,
                           "lock_timeout", 0, 
                            "use_if_header", TRUE, // for backward comp.
                           "response", &resp, 
                           NULL))
            return xerr;
        ...
        // Clean up
        if (doc)
            XmlFreeDocument(xctx, doc);

    RETURNS
        (xmlerr) numeric error node

    ARGUMENTS
        res  (IN) resource to be locked
        ...  (IN) variable arguments, with final NULL

    SEE ALSO
        XmlDavResUnlock
*/
xmlerr XmlDavResLock(xmlDavResDesc *res, ...);

/*---------------------------------------------------------------------------
    NAME
        XmlDavResMkCol

    PURPOSE
        Create a new collection resource at the server

    DESCRIPTION
        Create a new collection resource at the location specified by the
        uri/iri attribute.  If the resource identified by the uri/iri 
        attribute is non-null, this method will fail.  When this operation 
        creates a new collection resource, all ancestors MUST already exist,
        or the method will fail. 
        
        {{
        Currently supported name/value attribute pairs:

        ("http_request", address of pointer of <xmlDavHttpReq *> type)
            The http_request attribute is used to provide user the final
            constructed request message in some system-generated buffer(s).
            See also XmlDavGetHttpReq().

        ("precondition", value of <xmlDavCond *> type)
            This precondition attribute provides state tokens or entity
            tags needed for a successful execution of this method.

        ("response", address of pointer of <xmlDavResponse *> type)
            The response attribute is used to provide user the response from
            the server.  The returned value can be NULL if error occurs.

        }}

    RETURNS
        (xmlerr) numeric error code

    ARGUMENTS
        res  (IN)  resource descriptor
        ...  (IN)  variable arguments, with final NULL

    SEE ALSO
        XmlDavResPut, XmlDavResType, XmlDavDestroy
*/
xmlerr XmlDavResMkCol(xmlDavResDesc *res, ...);

/*---------------------------------------------------------------------------
    NAME
        XmlDavResMove

    PURPOSE
        Move this resource to the destination URL

    DESCRIPTION
        Move this resource and potentially its internal members and 
        their properties to the destination URL. Partial results are 
        possible, check the "response" for details.  If a resource exists
        at the destination,  the destination resource will be DELETED as a
        side-effect of the MOVE operation, subject to the restrictions of 
        the Overwrite attribute.  Any write locks active on the resource 
        will not be moved.  After a successful move, the original resource 
        descriptor becomes obsolete.  So, call XmlDavDestroy to free it. 
        If the client wish to access the newly moved resource, a new 
        resource descriptor can be created with XmlDavCreateResDesc.

        {{
        Currently supported name/value attribute pairs:

        ("Overwrite", value of boolean type)
            If a resource exists at the destination and the Overwrite 
            attribute is TRUE then prior to performing the move the server
            MUST perform a DELETE with "Depth: infinity" on the destination 
            resource.  If the Overwrite attribute is set to FALSE then the 
            operation will fail. If not specified, default is TRUE.

        ("destination_iri", value of <xmlDavIri *> type)
            The destination_iri specifies the IRI which identifies a
            destination resource. If the  source and destination resource 
            are on different servers, this method may fail with a 502 (Bad 
            Gateway) response.  Either this attribute or destination_uri, 
            but not both, must be specified.

        ("destination_uri", value of <xmlDavUri *> type)
            The destination_uri specifies the URI which identifies a
            destination resource. If the source and destination resource 
            are on different servers, this method may fail with a 502 (Bad 
            Gateway) response.  Either this attribute or destination_iri, 
            but not both, must be specified.

        ("http_request", address of pointer of <xmlDavHttpReq *> type)
            The http_request attribute is used to provide user the final
            constructed request message in some system-generated buffer(s).
            See also XmlDavGetHttpReq().

        ("precondition", value of <xmlDavCond *> type)
            This precondition attribute provides state tokens or entity
            tags needed for a successful execution of this method.

        ("response", address of pointer of <xmlDavResponse *> type)
            The response attribute is used to provide user the response from
            the server.  The returned value can be NULL if error occurs.
        }}

    RETURNS
        (xmlerr)   numeric error code

    ARGUMENTS
        res  (IN)  resource to be moved
        ...  (IN)  variable arguments, with final NULL

    SEE ALSO
        XmlDavCreateResDesc, XmlDavDestroy
*/
xmlerr XmlDavResMove(xmlDavResDesc *res, ...);

/*---------------------------------------------------------------------------
    NAME
        XmlDavResOptions

    PURPOSE
        Get the options supported by the resource

    DESCRIPTION
        This method allows the client to determine the options and/or
        requirements associated with a resource, or the capabilities of
        a server, without implying a resource action or initiating a 
        resource retrieval.  A client can discover the compliance 
        classes of a WebDav-compliant resource by executing this method 
        on the resource, and examining the "DAV" header which is returned.

        HTTP header fields such as "Max-Forwards", "Transfer-Encoding" or 
        "Content-Type" can be specified by setting them in the request 
        descriptor.

        {{
        Currently supported name/value attribute pairs:

        ("http_request", address of pointer of <xmlDavHttpReq *> type)
            The http_request attribute is used to provide user the final
            constructed request message in some system-generated buffer(s).
            See also XmlDavGetHttpReq().

        ("message_body", value of <oratext *> type)
            The message_body attribute is used to provide a message body to 
            be shipped in the request.  This data will be shipped as is which
            means all encodings have been applied. This attribute is optional.
            If specified, it needs to be paired with "message_length" 
            attribute.  See also "Transfer-Encoding" or "Content-Type"
            attribute defined in the request descriptor.

        ("message_length", value of ubig_ora type)
            The message_length attribute is used to specify the length of the
            data pointed to by message_body attribute in bytes.  This 
            attribute is required when message_body attribute is specified.

        ("response", address of pointer of <xmlDavResponse *> type)
            The response attribute is used to provide user the response from
            the server.  The returned value can be NULL if error occurs.

        }}

    RETURNS
        (xmlerr) numeric error node

    ARGUMENTS
        res  (IN) resource to be queried
        ...  (IN) variable arguments, with final NULL

    SEE ALSO
        XmlDavRespGetHdrNam, XmlDavRespGetHdrVal, XmlDavRespGetCode
*/
xmlerr XmlDavResOptions(xmlDavResDesc *res, ...);

/*---------------------------------------------------------------------------
    NAME
        XmlDavResPut

    PURPOSE
        Create or modify a non-collection resource at the server

    DESCRIPTION
        Create a new non-collection resource at the location specified by
        the uri/iri attribute stored in xmlDavResDesc if the uri/iri doesn't
        point to an existing resource.  Otherwise, the operation is to 
        modify an existing resource.

        HTTP header fields such as "Transfer-Encoding" or "Content-Type" 
        can be specified by setting them in the request descriptor.

        {{
        Currently supported name/value attribute pairs:

        ("http_request", address of pointer of <xmlDavHttpReq *> type)
            The http_request attribute is used to provide user the final
            constructed request message in some system-generated buffer(s).
            See also XmlDavGetHttpReq().

        ("message_body", value of <oratext *> type)
            The message_body attribute is used to provide a message body to 
            be shipped in the request.  This data will be shipped as is which
            means all encodings have been applied. This attribute is optional.
            If specified, it needs to be paired with "message_length" 
            attribute.  See also "Transfer-Encoding" or "Content-Type"
            attribute defined in the request descriptor.

        ("message_length", value of ubig_ora type)
            The message_length attribute is used to specify the length of the
            data pointed to by message_body attribute in bytes.  This 
            attribute is required when message_body attribute is specified.

        ("precondition", value of <xmlDavCond *> type)
            This precondition attribute provides state tokens or entity
            tags needed for a successful execution of this method.

        ("response", address of pointer of <xmlDavResponse *> type)
            The response attribute is used to provide user the response from
            the server.  The returned value can be NULL if error occurs.

        }}

    RETURNS
        (xmlerr) numeric error code

    ARGUMENTS
        res  (IN)  resource to be created or modified
        ...  (IN)  variable arguments, with final NULL

    SEE ALSO
        XmlDavResMkCol, XmlDavCreateResDesc, XmlDavUpdResDesc, XmlDavDestroy
*/
xmlerr XmlDavResPut(xmlDavResDesc *res, ...);

/*---------------------------------------------------------------------------
    NAME
        XmlDavResReport

    PURPOSE
        perform the complex request to return information about a resource

    DESCRIPTION
        A REPORT request is an extensible mechanism for obtaining information
        about a resource.  Unlike a resource property, which has a single
        value, the value of a report can depend on additional information
        specified in the REPORT request body and in the REPORT request
        headers.

        The body of a REPORT request specifies which report is being
        requested, as well as any additional information that will be used
        to customize the report.
        
        The request MAY include a Depth header.  If no Depth header is
        included, Depth:0 is assumed.

        REPORT reqeust is not affected by a write-lock because the REPORT 
        method cannot change the content or dead properties of any resource.

        {{
        Currently supported name/value attribute pairs:

        ("Depth", value of xmlDavDepth type)
            The Depth attribute is used to specify what level of children
            whose properties are to be returned.  The default value is 
            XMLDAV_DEPTH_0.

        ("http_request", address of pointer of <xmlDavHttpReq *> type)
            The http_request attribute is used to provide user the final
            constructed request message in some system-generated buffer(s).
            See also XmlDavGetHttpReq().

        ("response", address of pointer of <xmlDavResponse *> type)
            The response attribute is used to provide user the response from
            the server.  The returned value can be NULL if error occurs.

        ("report_name", value of <oratext *> type in DAV's data encoding)
            The report_name attribute is used to specify the name of the 
            report request.  This name must be defined in the DAV's namespace
            (i.e., "DAV:").  For example, "D:principal-property-search", 
            "D:principal-match", etc. are acceptable if server supports access
            control.  This information will be used to compose a document 
            node with its Qname same as the "report_name".  DAV's namespace
            URI will be declared in the new document node using prefix "D".
            This attribute is required.

        ("report_value", list of <xmlelemnode *> type in DAV's data encoding)
            The report_value attribute is used to specify the content of the
            document node which will be used to customize the report.  Refer
            to WebDAV Access Control Protocol specification for allowable 
            subelements for each specific report.  The  Qname of the document 
            node is provided via "report_name" attribute.  This attribute is
            required.  

        ("xmlctx", value of <xmlctx *> type)
            The xmlctx attribute is used to specify the xmlctx used to 
            create element nodes provided in report_value attribute.  This
            attribute is required.
        }}

    RETURNS
        (xmlerr) numeric error code

    ARGUMENTS
        res  (IN)  resource to be created or modified
        ...  (IN)  variable arguments, with final NULL

    SEE ALSO
        XmlDavCreateResDesc, XmlDavUpdResDesc, XmlDavDestroy
*/
xmlerr XmlDavResReport(xmlDavResDesc *res, ...);

/*---------------------------------------------------------------------------
    NAME
        XmlDavResUnlock

    PURPOSE
        Remove the lock from the resource

    DESCRIPTION
        This method removes the lock identified by its lock token from 
        this resource and all other resources included in the lock.  The 
        lock token retrieved from the response of XmlDavResLock must be
        set in the resource with XmlDavUpdResDesc() before calling this 
        method.

        HTTP header fields such as "Authorization", or "Proxy-Authorization"
        can be specified by setting them in the request descriptor.

        {{
        Currently supported name/value attribute pairs:

        ("http_request", address of pointer of <xmlDavHttpReq *> type)
            The http_request attribute is used to provide user the final
            constructed request message in some system-generated buffer(s).
            See also XmlDavGetHttpReq().

        ("response", address of pointer of <xmlDavResponse *> type)
            The response attribute is used to provide user the response from
            the server.  The returned value can be NULL if error occurs.

        }}

    RETURNS
        (xmlerr) numeric error node

    ARGUMENTS
        res  (IN) resource to be unlocked
        ...  (IN) variable arguments, with final NULL

    SEE ALSO
        XmlDavResLock
*/
xmlerr XmlDavResUnlock(xmlDavResDesc *res, ...);

/*---------------------------------------------------------------------------
    NAME
        XmlDavResUpdProp

    PURPOSE
        Set and/or remove properties defined on the resource

    DESCRIPTION
        This method sets and/or removes properties defined on the resource.
        Note that properties to be updated are limited to the resource 
        itself, not including its internal members'. This method must contain
        at least one remove or set attribute.  However, it may contain any 
        number of remove and set attributes.  "set" or "remove" instructions
        will be executed in the order as they are specified on the argument 
        list.  Some read-only properties, e.g. getetag property, cannot be
        set.

        {{
        Currently supported name/value attribute pairs:

        ("http_request", address of pointer of <xmlDavHttpReq *> type)
            The http_request attribute is used to provide user the final
            constructed request message in some system-generated buffer(s).
            See also XmlDavGetHttpReq().

        ("precondition", value of <xmlDavCond *> type)
            This precondition attribute provides state tokens or entity
            tags needed for a successful execution of this method.

        ("remove_props", value of <xmlDavPropGrp *> type)
            The remove attribute is used to specify that a list of properties
            in a group are to be removed.  Attributes "remove_props" and 
            "remove_props_list" are exclusive.  Specify only one of them.  

        ("remove_props_list", list of <xmlDavPropGrp *> type)
            The remove_props_list attribute is used to specify that a list of
            properties in multiple groups whose name and value are to be 
            removed.  Attributes "remove_props" and "remove_props_list" are
            exclusive.  Specify only one of them.  

        ("response", address of pointer of <xmlDavResponse *> type)
            The response attribute is used to provide user the response from
            the server.  The returned value can be NULL if error occurs.

        ("set_props", value of <xmlDavPropGrp *> type)
            The set attribute is used to specify that a group of properties
            whose values are to be set with the provided new values.  If a 
            property already exists then its value is replaced.  The natural
            language of the  intended audience of the property value can be 
            specified by setting "natural_language" attribute in the property
            group object.  Attributes "set_props" and "set_props_list" are
            exclusive.  Specify only one of them.  

             
        ("set_props_list", list of <xmlDavPropGrp *> type)
            The set_props_list attribute is used to specify that a list of
            properties in multiple groups whose values are to be set with the
            provided new values.  If a property already exists then its value
            is replaced.  The natural language of the  intended audience of 
            the property value can be specified by setting "natural_language"
            attribute in the property group object.  Attributes "set_props" 
            and "set_props_list" are exclusive.  Specify only one of them.  
             
        }}

    RETURNS
        (xmlerr)   numeric error code

    ARGUMENTS
        res  (IN)  resource to be queried
        ...  (IN)  variable arguments, with final NULL

    SEE ALSO
        XmlDavResGetProp, xmlDavPropGrp, XmlDavCreatePropGrp 
*/
xmlerr XmlDavResUpdProp(xmlDavResDesc *res, ...);

/****************************************************************************
                            Interface Response
 ****************************************************************************/

/*---------------------------------------------------------------------------
    NAME
        XmlDavRespEnd

    PURPOSE
        Terminate a request-response session

    DESCRIPTION
        This method signals that application has done with the response
        object.  Therefore, the socket reserved for this session can be
        released for reuse.  To free up response object itself, use 
        XmlDavDestroy.  If XmlDavDestroy is called before XmlDavRespEnd,
        it will implicitly invoke XmlDavRespEnd on this response object.

    RETURNS
        (xmlerr)   error code

    ARGUMENTS
        resp  (IN)  response object whose session is to be terminated

    SEE ALSO
        XmlDavDestroy
*/
xmlerr XmlDavRespEnd(xmlDavResponse *resp);

/*---------------------------------------------------------------------------
    NAME
        XmlDavRespGetCode

    PURPOSE
        Get the status code for this response

    DESCRIPTION
        This method retrieves status code for this response.  See RFC 2518 and
        RFC 2616 for a list of possible status codes.  If return value is one
        of 200, 207..., XmlDavRespGetXmlDoc can be used to retrieved an entity
        body encoded in XML.  Based on returned status code, the client can 
        always take a reasonable course of action. If 0 is returned, it 
        signifies an error condition.

        {{

        1) If a method returns 207, the content type MUST be either text/xml,
           or application/xml and the response must have a DAV:multistatus 
           root element.

        2) Servers *may* return a text/xml or application/xml response for 
           4xx and 5xx status codes, in which case a client can test for a 
           DAV:error root element or a DAV:multistatus element to extract 
           more information.

        3) Servers MAY return text/xml or application/xml on any request. 
           However, the spec only specifies what this means for 200, 207, 
           4xx and 5xx.

        }}

    RETURNS
        (sb4)   status code

    ARGUMENTS
        resp  (IN)     response to be queried
        err   (IN/OUT) returned error code

    SEE ALSO
        XmlDavRespGetReason, XmlDavDestroy, XmlDavRespEnd
*/
sb4 XmlDavRespGetCode(xmlDavResponse *resp, xmlerr *err);

/*---------------------------------------------------------------------------
    NAME
        XmlDavRespGetBody

    PURPOSE
        Get the entity body in binary from a response message

    DESCRIPTION
        This method retrieves the entity body of a response in binary format.
        No character conversion is involved. The entity body is held in an
        internal buffer.  This internal buffer exits until this reponse or 
        its ancestor(s) are destroyed.

        {{
        Currently supported name/value attribute pairs:
            TBA
        }}

    RETURNS
        (oratext *) pointer to entity body

    ARGUMENTS
        resp   (IN)     response to be queried
        err    (IN/OUT) error code on failure
        outlen (IN/OUT) length of the entity body returned in bytes
        ...    (IN)     variable arguments, with final NULL

    SEE ALSO
        XmlDavRespGetHdrNam, XmlDavRespGetHdrSize
*/
oratext *XmlDavRespGetBody(xmlDavResponse *resp, xmlerr *err, 
                           ubig_ora *outlen, ...);

/*---------------------------------------------------------------------------
    NAME
        XmlDavRespGetBodyLine

    PURPOSE
        Get the next text line from the entity body of a response message

    DESCRIPTION
        This method retrieves the next text line from the entity body of 
        a response.  User can read the text line until an empty line is 
        reached (i.e., NULL is return). Character conversion may be done 
        from wire encoding (i.e., UTF-8) to the specified encoding.  An 
        optional output buffer can be specified. If buffer is not provided 
        or if it's provided and buffer is not big enough, system will 
        provide an internal buffer for the user.  This internal buffer 
        exits until this reponse or its ancestor(s) are destroyed.  
        Application can determine the size of buffer needed by calling this 
        routine with size_only attribute set to be TRUE.  The length of 
        the text line is returned in outlen argument.  Note that NULL char
        is not added at the end of text line.
        
        {{
        Currently supported name/value attribute pairs:

        ("buffer", value of <oratext *> type)
            The "buffer" attribute is used to provide a user buffer for
            the output of entity body.  If specified, "buffer_length" must
            be also specified.  If not specified, an internal buffer will
            be provided and this buffer has the same life span as response's. 

        ("buffer_length", value of ub4 type)
            The buffer_length attribute is used to indicate the length of
            user-provided output buffer.  This must be specified if "buffer"
            attribute is specified.

        ("content_encoding", value of <oratext *> type in compiler encoding)
            The "content_encoding" attribute is used to specify the 
            final encoding of the entity body to be presented to the user.
            If not specified, default is DAV's data encoding.

        ("remove_crlf", value of boolean type)
            The remove_crlf attribute indicates that whether the carriage
            return and/or line feed chracters are to be removed or not.
            Default value is FALSE (i.e., no CR or LF chars are to be 
            removed).

        ("size_only", value of boolean type)
            The size_only attribute is used to indicate whether the request
            is for size only.  If TRUE, size of the output in bytes will be
            returned in "outlen".  Otherwise, both data and its size will 
            be returned.  Default value for this attribute is FALSE;

        }}

    RETURNS
        (oratext *) pointer to returned text line

    ARGUMENTS
        resp   (IN)     response to be queried
        err    (IN/OUT) error code on failure
        outlen (IN/OUT) length of the entity body returned in bytes
        ...    (IN)     variable arguments, with final NULL

    SEE ALSO
        XmlDavRespGetHdrNam, XmlDavRespGetHdrSize, XmlDavDestroy
*/
oratext *XmlDavRespGetBodyLine(xmlDavResponse *resp, xmlerr *err, 
                               ub4 *outlen, ...);

/*---------------------------------------------------------------------------
    NAME
        XmlDavRespGetHdrNam

    PURPOSE
        Get the name of a header field in this response given an index

    DESCRIPTION
        This method retrieves the name of a header field for this response.
        A field name at the indexed position is returned.  If none exists at
        the indexed position, NULL is returned.  User can loop through all
        field name(s) until a NULL is returned by incrementing indexes 
        starting at zero.  Header name is encoded in compiler encoding.

    EXAMPLE
        xmlerr          xerr;
        xmlDavResponse *resp;
        oratext        *etagInComp;    // Etag in compiler encoding 

        if (xerr = XmlDavResHead(res, "response", &resp, NULL))
            return xerr;
        etagInComp = NULL;
        if (resp)    // resp can be NULL
        {
            oratext    *hdrNam;
            sb4         statusCode;
            ub4         index;

            statusCode = XmlDavRespGetCode(resp, &xerr);
            switch (statusCode)
            {
            case 200:    // OK
                index = 0;
                while (hdrNam = XmlDavRespGetHdrNam(resp, &xerr, index))
                {
                    if (XmlDavEquals(hdrNam, (oratext *) "Etag", FALSE))
                    {
                        etagInComp = XmlDavRespGetHdrVal(resp, &xerr, index);
                    }
                    index++;
                }
                break;
            default:
                break;
            }
        }

    RETURNS
        (oratext *)   name of a header field; NULL if not known

    ARGUMENTS
        resp  (IN)     response to be queried
        err   (IN/OUT) returned error code
        idx   (IN)     index starting at 0

    SEE ALSO
        XmlDavRespGetHdrVal, XmlDavRespGetHdrSize, XmlDavDestroy
*/
oratext *XmlDavRespGetHdrNam(xmlDavResponse *resp, xmlerr *err, ub4 idx);

/*---------------------------------------------------------------------------
    NAME
        XmlDavRespGetHdrSize

    PURPOSE
        Get the size of header fields for this response

    DESCRIPTION
        This method retrieves the size of header field(s) for this response.

    RETURNS
        (ub4)   the total number of header field(s) for this response

    ARGUMENTS
        resp  (IN)     response to be queried
        err   (IN/OUT) returned error code

    SEE ALSO
        XmlDavRespGetHdrNam, XmlDavRespGetHdrVal
*/
ub4 XmlDavRespGetHdrSize(xmlDavResponse *resp, xmlerr *err);

/*---------------------------------------------------------------------------
    NAME
        XmlDavRespGetHdrVal

    PURPOSE
        Get the value of a header field in this response given an index

    DESCRIPTION
        This method retrieves the value of a header field for this response.
        A field value at the indexed position is returned.  If none exists at
        the indexed position, NULL is returned.  Header value is encoded in 
        compiler encoding.

    RETURNS
        (oratext *)   value of a header field; NULL if not known

    ARGUMENTS
        resp  (IN)     response to be queried
        err   (IN/OUT) returned error code
        idx   (IN)     index starting at 0

    SEE ALSO
        XmlDavRespGetHdrNam, XmlDavDestroy, XmlDavRespGetHdrIriVal
*/
oratext *XmlDavRespGetHdrVal(xmlDavResponse *resp, xmlerr *err, ub4 idx);

/*---------------------------------------------------------------------------
    NAME
        XmlDavRespGetHdrIriVal

    PURPOSE
        Get the value of the indexed header field as an IRI from the response

    DESCRIPTION
        This method retrieves the value of a header field as an IRI from a
        response.  An IRI at the indexed position is returned.  If none 
        exists at the indexed position, NULL is returned.  Before returning,
        a URI at the indexed header position is converted to IRI.  Note that
        the value returned by XmlDavRespGetHdrVal is always a URI.  IRI's
        encoding is always UTF-8.

        {{
        Currently supported name/value attribute pairs:

        ("buffer", value of <oratext *> type)
            The "buffer" attribute is used to provide a user buffer for
            the output of IRI.  If specified, "buffer_length" must
            be also specified.  If not specified, an internal buffer will
            be provided and this buffer has the same life span as response's. 

        ("buffer_length", value of ub4 type)
            The buffer_length attribute is used to indicate the length of
            user-provided output buffer.  This must be specified if "buffer"
            attribute is specified.

        }}

    RETURNS
        (oratext *)   IRI; NULL if not known


    ARGUMENTS
        resp  (IN)     response to be queried
        err   (IN/OUT) returned error code
        idx   (IN)     index starting at 0
        ...   (IN)     variable arguments, with final NULL

    SEE ALSO
        XmlDavRespGetHdrNam,XmlDavDestroy, XmlDavRespGetHdrVal
*/
oratext *XmlDavRespGetHdrIriVal(xmlDavResponse *resp, xmlerr *err, 
                                ub4 idx, ...);

/*---------------------------------------------------------------------------
    NAME
        XmlDavRespGetReason

    PURPOSE
        Get the reason phrase for this response

    DESCRIPTION
        This method retrieves reason phrase for this response.  Reason phrase
        is a short textual description of the status code.

    RETURNS
        (oratext *)   reason phrase encoded in compiler encoding

    ARGUMENTS
        resp  (IN)     response to be queried
        err   (IN/OUT) returned error code

    SEE ALSO
        XmlDavRespGetCode, XmlDavDestroy
*/
oratext *XmlDavRespGetReason(xmlDavResponse *resp, xmlerr *err);

/*---------------------------------------------------------------------------
    NAME
        XmlDavRespGetVersion

    PURPOSE
        Get the protocol version for this response

    DESCRIPTION
        This method retrieves the protocol version for this response.  
        It could be one of the following: "HTTP/1.0", "HTTP/1.1", etc.

    RETURNS
        (oratext *)   HTTP version encoded in compiler encoding

    ARGUMENTS
        resp  (IN)     response to be queried
        err   (IN/OUT) returned error code

    SEE ALSO
        XmlDavRespGetCode, XmlDavDestroy
*/
oratext *XmlDavRespGetVersion(xmlDavResponse *resp, xmlerr *err);

/*---------------------------------------------------------------------------
    NAME
        XmlDavRespGetXmlDoc

    PURPOSE
        Get the XML DOM document encoded in the entity body

    DESCRIPTION
        This method retrieves the entity body of the response that is
        encoded in XML.  An XML DOM Document is returned and can be examined
        using XDK provided DOM API.  If content type of the response is not
        either "text/xml" or "application/xml", NULL is returned.  If an 
        ill-formed XML is encoded in a response, NULL is also returned. To
        free the returned DAV's document, use XmlDavDestroy.

    RETURNS
        (xmlDavXmlDoc *) DAV's document on success

    ARGUMENTS
        resp  (IN)     response to be queried
        err   (IN/OUT) error code on failure

    SEE ALSO
        XmlDavDestroy
*/
xmlDavXmlDoc *XmlDavRespGetXmlDoc(xmlDavResponse *resp, xmlerr *err);

#endif /* XMLDAV_ORACLE */
