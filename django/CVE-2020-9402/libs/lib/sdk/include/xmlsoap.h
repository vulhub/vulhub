/* $RCSfile: xmlsoap.h $ $Date: 2016/11/14 13:24:18 $
 Copyright (c) 2004, 2016, Oracle and/or its affiliates. All rights reserved.

   FILE NAME
        xmlsoap.h - XML SOAP APIs

   FILE DESCRIPTION
        XML SOAP Public APIs

   PUBLIC FUNCTIONS
        XmlSoapCreateCtx         - Create and return a SOAP context
        XmlSoapDestroyCtx        - Destroy a SOAP context

        XmlSoapCreateConnection  - Create a SOAP connection object
        XmlSoapDestroyConnection - Destroy a SOAP connection object

        XmlSoapCall              - Send a SOAP message & wait for reply

        XmlSoapCreateMsg         - Create and return an empty SOAP message
        XmlSoapDestroyMsg        - Destroy a SOAP message created w/XmlSoapCreateMsg

        XmlSoapGetEnvelope       - Return a SOAP message's envelope
        XmlSoapGetHeader         - Return a SOAP message's envelope header
        XmlSoapGetBody           - Return a SOAP message's envelope body

        XmlSoapAddHeaderElement  - Adds an element to a SOAP header
        XmlSoapGetHeaderElement  - Gets an element from a SOAP header

        XmlSoapAddBodyElement    - Adds an element to a SOAP message body
        XmlSoapGetBodyElement    - Gets an element from a SOAP message body

        XmlSoapSetMustUnderstand - Set mustUnderstand attr for SOAP hdr elem
        XmlSoapGetMustUnderstand - Get mustUnderstand attr from SOAP hdr elem

        XmlSoapSetRole           - Set role for SOAP header element
        XmlSoapGetRole           - Get role from SOAP header element

        XmlSoapSetRelay          - Set relay Header element property
        XmlSoapGetRelay          - Get relay Header element property

        XmlSoapSetFault          - Set Fault in SOAP message
        XmlSoapHasFault          - Does SOAP message have a Fault?
        XmlSoapGetFault          - Return Fault code, reason, and details

        XmlSoapAddFaultReason    - Add additional Reason to Fault
        XmlSoapAddFaultSubDetail - Add additional child to Fault Detail
        XmlSoapGetReasonNum      - Get number of Reasons in Fault element
        XmlSoapGetReasonLang     - Get a lang of a reasons with a
                                       particular index.

        XmlSoapError             - Get error message(s)

*/

#ifndef XMLSOAP_ORACLE
# define XMLSOAP_ORACLE

# ifndef XML_ORACLE
#  include <xml.h>
# endif

/*---------------------------------------------------------------------------
                Package SOAP - Simple Object Access Protocol APIs

     W3C: "SOAP is a lightweight protocol for exchange of information
     in a decentralized, distributed environment.  It is an XML based
     protocol that consists of three parts: an envelope that defines a
     framework for describing what is in a message and how to process
     it, a set of encoding rules for expressing instances of
     application-defined datatypes, and a convention for representing
     remote procedure calls and responses."
     Atachments are only allowed in Soap 1.1     
     In Soap 1.2 body may not have other elements if Fault is present.
  
     Structure of a SOAP message:

     [SOAP message (XML document)
        [SOAP envelope
             [SOAP header?
                 element*
             ]
             [SOAP body
                 (element* | Fault)?
             ]
        ]
     ]
---------------------------------------------------------------------------*/

#define XMLSOAP_VER_1_2         "http://www.w3.org/2003/05"
#define XMLSOAP_VER_1_1         "http://schemas.xmlsoap.org"
#define XMLSOAP_URI             XMLSOAP_VER_1_2 "/soap"

#define XMLSOAP_ENV_URI_1_1     XMLSOAP_VER_1_1 "/soap/envelope/"

#define XMLSOAP_ENV_URI_1_2     XMLSOAP_VER_1_2 "/soap-envelope"

#define XMLSOAP_PROP_FEATURES   XMLSOAP_URI "/features"

#define XMLSOAP_PROP_WEB_METHOD XMLSOAP_PROP_FEATURES "/web-method/Method"
#define XMLSOAP_WEB_METHOD_GET  "GET"
#define XMLSOAP_WEB_METHOD_POST "POST"

#define XMLSOAP_MIME_TYPE_1_1   (oratext *) "text/xml"
#define XMLSOAP_MIME_TYPE_1_2   (oratext *) "application/soap+xml"

/****************************************************************************
                           Interface SOAP
 ****************************************************************************/

/* DATATYPE xmlsoapctx - SOAP context
    Context for SOAP operations.  Only a single context is needed and can
    be shared for multiple SOAP messages.
*/
struct xmlsoapctx; typedef struct xmlsoapctx xmlsoapctx;

/* DATATYPE xmlsoapcon - SOAP connection
    SOAP connection object.  One is needed for each distinct connection, and
    contains binding and endpoint information.
*/
struct xmlsoapcon; typedef struct xmlsoapcon xmlsoapcon;

/* DATATYPE xmlsoapbind - SOAP connection binding
    Binding for SOAP connections.  SOAP does not dictate the binding
    (transport) used for conveying messages; however HTTP is well-defined
    and currently the only choice.
*/
typedef enum xmlsoapbind {
    XMLSOAP_BIND_NONE  = 0,  /* none */
    XMLSOAP_BIND_HTTP  = 1   /* HTTP */
} xmlsoapbind;

/*---------------------------------------------------------------------------
    NAME
        XmlSoapCreateCtx

    PURPOSE
        Create and return a SOAP context

    DESCRIPTION
        Create and return a SOAP context.  This context must be passed to
        all XmlSoap APIs.  Note the name provided should be unique and is
        used to identify the context when debugging.

        Options are specified as (name, value) pairs, ending with a NULL,
        same as XmlCreate().  If no options are desired, the NULL is still
        needed.  Options are:

        ("debug_level", numeric level, 0 for no debug)
            Enables SOAP debug output to stderr.  Level 0 (the default)
            means no debug statements; the higher the level, the more
            detailed (and volumnous) the output.

    RETURNS
        (xmlsoapctx *) SOAP context, or NULL on failure (w/xerr set)

    ARGUMENTS
        xctx (IN)  XML context
        xerr (OUT) error return code on failure
        name (IN)  name of context (for debugging)
        ...  (IN)  options, as (name, value) pairs, followed by NULL

    SEE ALSO
        XmlSoapDestroyCtx
*/
xmlsoapctx *XmlSoapCreateCtx(xmlctx *xctx, xmlerr *xerr, oratext *name, ...);
xmlsoapctx *XmlSoapCreateCtxVA(xmlctx *xctx, xmlerr *xerr, oratext *name,
                               va_list va);

/*---------------------------------------------------------------------------
    NAME
        XmlSoapDestroyCtx

    PURPOSE
        Destroy a SOAP context

    DESCRIPTION
        Destroy a SOAP context created with XmlSoapCreateCtx.  All memory
        allocated will be freed, and all connections closed.

    RETURNS
        (xmlerr) numeric error code, XMLERR_OK [0] on success

    ARGUMENTS
        ctx (IN) SOAP context

    SEE ALSO
        XmlSoapCreateCtx
*/
xmlerr XmlSoapDestroyCtx(xmlsoapctx *ctx);

/*---------------------------------------------------------------------------
    NAME
        XmlSoapCreateConnection

    PURPOSE
        Create a SOAP connection object

    DESCRIPTION
        Create a SOAP connection object, specifying the binding (transport)
        and endpoint.  The binding is an enum of type xmlsoapbind, and the
        endpoint depends on the binding.

        Currently only HTTP binding is supported, and the endpoint is a URL.
        That URL should be active, i.e. a cgi-bin script or some mechanism
        to trigger SOAP message processing based on the Content-type of the
        incoming message ("application/soap+xml").

        To control the HTTP access method (GET or POST), use the web-method
        property named XMLSOAP_PROP_WEB_METHOD which can have possible
        values XMLSOAP_WEB_METHOD_GET and XMLSOAP_WEB_METHOD_POST.

        (conbuf, consiz) is the connection buffer used with LPU; for sending,
        it contains only the HTTP header, but on reception it holds the entire
        reply, including HTTP header and the full SOAP body.  If no buffer is
        provided, one will be allocated for you.  If size if zero, the default
        size (64K) will be used.

        (msgbuf, msgsiz) is the message buffer used to form SOAP messages for
        sending.  It needs to be large enough to contain the largest message
        which will be sent.  If no buffer is specified, one will be allocated
        for you.  If the size is zero, the default size (64K) will be used.

        Two buffers are needed for sending since the SOAP message needs to be
        formed first in order to determine it's size; then, the HTTP header
        can be formed, since the Content-Length is now known.

    RETURNS
        (xmlsoapcon *) connect object, or NULL on error with xerr set

    ARGUMENTS
        ctx    (IN)  SOAP context
        xerr   (OUT) numeric error code on failure
        bind   (IN)  connection binding
        endp   (IN)  connection endpoint
        conbuf (I/O) connection buffer (or NULL to have one allocated)
        consiz (IN)  size of connection buffer (or 0 for default size)
        msgbuf (I/O) message buffer (or NULL to have one allocated)
        msgsiz (IN)  size of message buffer (or 0 for default size)
        ...    (IN)  additional HTTP headers to set, followed by NULL

    SEE ALSO
        XmlSoapDestroyConnection, XmlSoapCall
*/
xmlsoapcon *XmlSoapCreateConnection(xmlsoapctx *ctx, xmlerr *xerr,
                                    xmlsoapbind bind, void *endp,
                                    oratext *conbuf, ubig_ora consiz,
                                    oratext *msgbuf, ubig_ora msgsiz, ...);
xmlsoapcon *XmlSoapCreateConnectionVA(xmlsoapctx *ctx, xmlerr *xerr,
                                      xmlsoapbind bind, void *endp,
                                      oratext *conbuf, ubig_ora consiz,
                                      oratext *msgbuf, ubig_ora msgsiz,
                                      va_list va);

/*---------------------------------------------------------------------------
    NAME
        XmlSoapDestroyConnection

    PURPOSE
        Destroy a SOAP connection object

    DESCRIPTION
        Destroy a SOAP connection object made with XmlSoapCreateConnection
        and free any resources.

    RETURNS
        (xmlerr) numeric error code, XMLERR_OK [0] on success

    ARGUMENTS
        ctx (IN) SOAP context
        con (IN) SOAP connection object

    SEE ALSO
        XmlSoapCreateConnection, XmlSoapCall
*/
xmlerr XmlSoapDestroyConnection(xmlsoapctx *ctx, xmlsoapcon *con);

/*---------------------------------------------------------------------------
    NAME
        XmlSoapCall

    PURPOSE
        Send a SOAP message then wait for and return replied message

    DESCRIPTION
        Send a SOAP message over a connection and wait for the response;
        the message reply (an XML document) is parsed and returned as
        a SOAP message (equivalent to a DOM).

        The message buffer is first used to serialize the outgoing message;
        if it's too small (overflow occurs), xerr gets XMLERR_SAVE_OVERFLOW
        and NULL is returned.  The same buffer is then re-used to receive
        the replied SOAP message.

        Opening the connection object is expected to cause an active SOAP
        handler to appear on the end-point; how this happens is up to the
        user.  For HTTP, the URL should invoke a cgi-bin, or detect the
        application/soap+xml content-type.

    RETURNS
        (xmldocnode *) returned message, or NULL on failure with xerr set

    ARGUMENTS
        ctx  (IN)  SOAP context
        con  (IN)  SOAP connection object
        msg  (IN)  SOAP message to send
        xerr (OUT) numeric error code on failure

    SEE ALSO
        XmlSoapCreateMsg, XmlSoapCreateConnection, XmlSoapDestroyConnection
*/
xmldocnode *XmlSoapCall(xmlsoapctx *ctx, xmlsoapcon *con,
                        xmldocnode *msg, xmlerr *xerr);

/*---------------------------------------------------------------------------
    NAME
        XmlSoapCreateMsg

    PURPOSE
        Create and return an empty SOAP message

    DESCRIPTION
        Create and return an empty SOAP message.  The SOAP message will
        consist of an Envelope.  The Envelope contains
        an empty Header and Body.  A SOAP message is an XML document
        represented by a DOM, and is no different from any other XML
        document.  All DOM operations are valid on the document, but be
        sure not to harm the overall structure!  Changes should be
        restricted to creating and modifying elements inside the Header
        and Body.

    RETURNS
        (xmldocnode *) SOAP message, or NULL on failure (w/xerr set)

    ARGUMENTS
        ctx  (IN)  SOAP context
        xerr (OUT) error return code on failure

    SEE ALSO
        XmlSoapDestroyMsg
*/
xmldocnode *XmlSoapCreateMsg(xmlsoapctx *ctx, xmlerr *xerr);

/*---------------------------------------------------------------------------
    NAME
        XmlSoapDestroyMsg

    PURPOSE
        Destroy a SOAP message created with XmlSoapCreateMsg

    DESCRIPTION
        Destroy a SOAP message created with XmlSoapCreateMsg; this is the
        same as calling XmlFreeDocument.

    RETURNS
        (xmlerr) numeric error code, XMLERR_OK [0] on success

    ARGUMENTS
        ctx (IN) SOAP context
        msg (IN) SOAP message

    SEE ALSO
        XmlSoapCreateMsg
*/
xmlerr XmlSoapDestroyMsg(xmlsoapctx *ctx, xmldocnode *msg);

/****************************************************************************
                           Interface Message
 ****************************************************************************/

/* DATATYPE xmlsoaprole - Role for SOAP node
   Role for SOAP node
*/
typedef enum xmlsoaprole {
    XMLSOAP_ROLE_UNSET = 0,  /* not specified */
    XMLSOAP_ROLE_NONE  = 1,  /* "none" */
    XMLSOAP_ROLE_NEXT  = 2,  /* "next" */
    XMLSOAP_ROLE_ULT   = 3,   /* "ultimateReceiver" */
    XMLSOAP_ROLE_OTHER = 4    /* other - user defined */
} xmlsoaprole;

/*---------------------------------------------------------------------------
    NAME
        XmlSoapGetEnvelope

    PURPOSE
        Return a SOAP message's envelope

    DESCRIPTION
        Return a SOAP message's envelope

    RETURNS
        (xmlelemnode *) SOAP envelope

    ARGUMENTS
        ctx  (IN) SOAP context
        msg  (IN) SOAP message
        xerr (I/O) error code        

    SEE ALSO

*/
xmlelemnode *XmlSoapGetEnvelope(xmlsoapctx *ctx, xmldocnode *msg, xmlerr *xerr);

/*---------------------------------------------------------------------------
    NAME
        XmlSoapGetHeader

    PURPOSE
        Return a SOAP message's envelope header

    DESCRIPTION
        Return a SOAP message's envelope header

    RETURNS
        (xmlelemnode *) SOAP header

    ARGUMENTS
        ctx  (IN) SOAP context
        msg  (IN) SOAP message
        xerr (I/O) error code        
 
    SEE ALSO
        XmlSoapGetBody
*/
xmlelemnode *XmlSoapGetHeader(xmlsoapctx *ctx, xmldocnode *msg, xmlerr *xerr);

/*---------------------------------------------------------------------------
    NAME
        XmlSoapGetBody

    PURPOSE
        Return a SOAP message's envelope body

    DESCRIPTION
        Return a SOAP message's envelope body

    RETURNS
        (xmlelemnode *) SOAP Body

    ARGUMENTS
        ctx  (IN) SOAP context
        msg  (IN) SOAP message
        xerr (I/O) error code        

    SEE ALSO
        XmlSoapGetHeader
*/
xmlelemnode *XmlSoapGetBody(xmlsoapctx *ctx, xmldocnode *msg, xmlerr *xerr);

/*---------------------------------------------------------------------------
    NAME
        XmlSoapAddHeaderElement

    PURPOSE
        Adds an element to a SOAP header

    DESCRIPTION
        Adds an element to a SOAP header
        Set (xmlerr) numeric error code.

    RETURNS
        (xmlelemnode *) created element

    ARGUMENTS
        ctx   (IN)  SOAP context
        msg   (I/O) SOAP message
        qname (IN)  QName of element to add
        uri   (IN)  Namespace URI of element to add
        xerr  (I/O) error code        

    SEE ALSO
        XmlDomCreateElemNS, XmlSoapGetHeaderElement, XmlSoapAddBodyElement
*/
xmlelemnode *XmlSoapAddHeaderElement(xmlsoapctx *ctx, xmldocnode *msg,
                        oratext *qname, oratext *uri, xmlerr *xerr);

/*---------------------------------------------------------------------------
    NAME
        XmlSoapGetHeaderElement

    PURPOSE
        Gets an element from a SOAP header

    DESCRIPTION
        Gets an element from a SOAP header.
        Set (xmlerr) numeric error code.        

    RETURNS
        (xmlelemnode *) named element, or NULL on error

    ARGUMENTS
        ctx   (IN) SOAP context
        msg   (IN) SOAP message
        uri   (IN) Namespace URI of element to get
        local (IN) Local name of element to get
        xerr  (I/O) error code        

    SEE ALSO
        XmlDomCreateElemNS, XmlSoapAddHeaderElement, XmlSoapGetBodyElement
*/
xmlelemnode *XmlSoapGetHeaderElement(xmlsoapctx *ctx, xmldocnode *msg,
                        oratext *uri, oratext *local, xmlerr *xerr);

/*---------------------------------------------------------------------------
    NAME
        XmlSoapAddBodyElement

    PURPOSE
        Adds an element to a SOAP message body.

    DESCRIPTION
        Adds an element to a SOAP message body.
        Set (xmlerr) numeric error code
        
    RETURNS
        (xmlelemnode *) created element

    ARGUMENTS
        ctx   (IN)  SOAP context
        msg   (I/O) SOAP message
        qname (IN)  QName of element to add
        uri   (IN)  Namespace URI of element to add
        xerr  (I/O) error code        

    SEE ALSO
        XmlDomCreateElemNS, XmlSoapAddHeaderElement
*/
xmlelemnode *XmlSoapAddBodyElement(xmlsoapctx *ctx, xmldocnode *msg,
                                   oratext *qname, oratext *uri, xmlerr *xerr);

/*---------------------------------------------------------------------------
    NAME
        XmlSoapGetBodyElement

    PURPOSE
        Gets an element from a SOAP body

    DESCRIPTION
        Gets an element from a SOAP body

    RETURNS
        (xmlelemnode *) named element, or NULL on error

    ARGUMENTS
        ctx   (IN) SOAP context
        msg   (IN) SOAP message
        uri   (IN) Namespace URI of element to get
        local (IN) Local name of element to get
        xerr (I/O) error code        

    SEE ALSO
        XmlDomCreateElemNS, XmlSoapAddBodyElement
*/
xmlelemnode *XmlSoapGetBodyElement(xmlsoapctx *ctx, xmldocnode *msg,
                                   oratext *uri, oratext *local, xmlerr *xerr);

/*---------------------------------------------------------------------------
    NAME
        XmlSoapSetMustUnderstand

    PURPOSE
        Set mustUnderstand attribute for SOAP header element

    DESCRIPTION
        Set mustUnderstand attribute for SOAP header element.
        According to the standard, if the value is FALSE, the
        attribute is not set.

    RETURNS
        (xmlerr) numeric error code, XMLERR_OK [0] on success

    ARGUMENTS
        ctx             (IN)  SOAP context
        elem            (I/O) SOAP header element
        mustUnderstand  (IN)  mustUnderstand value (TRUE|FALSE)

    SEE ALSO
        XmlSoapSetRole
*/
xmlerr XmlSoapSetMustUnderstand(xmlsoapctx *ctx, xmlelemnode *elem,
                                boolean mustUnderstand);

/*---------------------------------------------------------------------------
    NAME
        XmlSoapGetMustUnderstand

    PURPOSE
        Get mustUnderstand attribute from SOAP header element

    DESCRIPTION
        Get mustUnderstand attribute from SOAP header element.
        The absence of this attribute is not an error and treated
        as value FALSE.  To indicate the absence of an attribute,
        the error code XMLERR_SOAP_NO_MUST_UNDERSTAND is returned
        in this case, XMLERR_OK (0) is returned if the attribute
        is present.  Other appropriate error codes might be
        returned. User supplied *mustUnderstand value is set
        accordingly. 

    RETURNS
        (xmlerr) numeric error code, XMLERR_OK [0] on success

    ARGUMENTS
        ctx            (IN)  SOAP context
        elem           (IN)  SOAP header element
        mustUnderstand (OUT) mustUnderstand value (boolean)

    SEE ALSO
        XmlDomCreateElemNS, XmlSoapAddBodyElement, XmlSoapSetMustUnderstand
*/
xmlerr XmlSoapGetMustUnderstand(xmlsoapctx *ctx, xmlelemnode *elem,
                                boolean *mustUnderstand);

/*---------------------------------------------------------------------------
    NAME
        XmlSoapSetRole

    PURPOSE
        Set role for SOAP header element

    DESCRIPTION
        Set role for SOAP header element. If the role specified is
        XMLSOAP_ROLE_ULT, then according to the standard the
        attribute is not set.

    RETURNS
        (xmlerr) numeric error code, XMLERR_OK [0] on success

    ARGUMENTS
        ctx  (IN)  SOAP context
        elem (I/O) SOAP header element
        role (IN)  role value (xmlsoaprole)

    SEE ALSO
        XmlSoapSetMustUnderstand, xmlsoaprole
*/
xmlerr XmlSoapSetRole(xmlsoapctx *ctx, xmlelemnode *elem, xmlsoaprole role);

/*---------------------------------------------------------------------------
    NAME
        XmlSoapGetRole

    PURPOSE
        Get role from SOAP header element

    DESCRIPTION
        Get role from SOAP header element.  If the element has no role,
        XMLERR_SOAP_NO_ROLE is returned, otherwise XMLERR_OK (0) is returned.
        The user's role is set accordingly, if the element has no role,
        then according to the standard, the user's role is set to XMLSOAP_ROLE_ULT.

    RETURNS
        (xmlerr) numeric error code, XMLERR_OK [0] on success

    ARGUMENTS
        ctx  (IN)  SOAP context
        elem (IN)  SOAP header element
        role (OUT) role value (xmlsoaprole)

    SEE ALSO
        XmlSoapSetMustUnderstand, xmlsoaprole, XmlSoapSetRole
*/
xmlerr XmlSoapGetRole(xmlsoapctx *ctx, xmlelemnode *elem, xmlsoaprole *role);

/*---------------------------------------------------------------------------
    NAME
        XmlSoapSetRelay

    PURPOSE
        Set Relay attribute for SOAP header element

    DESCRIPTION
        Set Relay attribute for SOAP header element
        According to the standard, if the value is FALSE, the
        attribute is not set.

    RETURNS
        (xmlerr) numeric error code, XMLERR_OK [0] on success

    ARGUMENTS
        ctx             (IN)  SOAP context
        elem            (I/O) SOAP header element
        Relay           (IN)  Relay (TRUE|FALSE)

    SEE ALSO
        XmlSoapSetRelay
*/
xmlerr XmlSoapSetRelay(xmlsoapctx *ctx, xmlelemnode *elem,
                                boolean Relay);

/*---------------------------------------------------------------------------
    NAME
        XmlSoapGetRelay

    PURPOSE
        Get Relay attribute from SOAP header element

    DESCRIPTION
        Get Relay attribute from SOAP header element.  If the
        element has no mustUnderstand attr, XMLERR_SOAP_NO_MUST_UNDERSTAND
        is returned, otherwise XMLERR_OK (0) is returned and the user's
        mustUnderstand value is set accordingly.

    RETURNS
        (xmlerr) numeric error code, XMLERR_OK [0] on success

    ARGUMENTS
        ctx            (IN)  SOAP context
        elem           (IN)  SOAP header element
        Relay          (OUT) Relay value (boolean)

    SEE ALSO
        XmlDomCreateElemNS, XmlSoapAddBodyElement, XmlSoapSetRelay
*/
xmlerr XmlSoapGetRelay(xmlsoapctx *ctx, xmlelemnode *elem,
                                boolean *Relay);

/*---------------------------------------------------------------------------
    NAME
        XmlSoapSetFault

    PURPOSE
        Set Fault in SOAP message

    DESCRIPTION
        Set Fault in SOAP message.  
        Ver 1.2 Only one Fault is allowed per message,
        and it must be the only child of the Body.  If the Body has children,
        they are first removed and freed.  The Fault is then added with
        children code - "env:Code" (required), reason - "env:Reason" (required),
        node - "env:Node" (optional), role - "env:role"(optional),
        and detail - "env:Detail" (optional),
        The primary-language reason should be added first; calls to
        XmlSoapGetFault which pass a NULL language will pick this reason.
        Detail is the user-defined subtree to be spliced into the Fault.

        Ver 1.1.  Only one Fault is allowed 
        per message. If the Body already has Fault, it is first removed 
        and freed.  The Fault is then added with
        children code - "faultcode" (required), reason - "faultstring" (required),
        node - "faultactor" (optional),  and detail - "detail" (optional).
        Detail is the user-defined subtree to be spliced into the Fault.
        role and lang are unused in ver 1.1

     RETURNS
        (xmlerr) numeric error code, XMLERR_OK [0] on success

    ARGUMENTS
        ctx    (IN)  SOAP context
        msg    (I/O) SOAP message
        node   (IN)  URI of SOAP node which faulted, Node (1.2), faultactor(1.1)
        code   (IN)  Code (1.2), faultcode (1.1)
        reason (IN)  Human-readable fault Reason (1.2), faultreason (1.1)
        lang   (IN)  Language of reason (1.2), unused (1.1)
        role   (IN)  URI representing role, Role (1.2), unused (1.1)
        detail (IN)  detail elements (user-defined)


    SEE ALSO
        XmlSoapAddFaultReason
*/
xmlerr XmlSoapSetFault(xmlsoapctx *ctx, xmldocnode *msg,oratext *node, 
                       oratext *code, oratext *reason, oratext *lang, 
                       oratext *role, xmlelemnode *detail);

/*---------------------------------------------------------------------------
    NAME
        XmlSoapHasFault

    PURPOSE
        Determine if SOAP message contains Fault object

    DESCRIPTION
        Determine if SOAP message contains Fault object

    RETURNS
        (boolean) TRUE if there's a Fault, FALSE if not

    ARGUMENTS
        ctx    (IN)  SOAP context
        msg    (IN) SOAP message
        xerr   (I/O) error code        

    SEE ALSO
        XmlSoapGetFault
*/
boolean XmlSoapHasFault(xmlsoapctx *ctx, xmldocnode *msg, xmlerr *xerr);

/*---------------------------------------------------------------------------
    NAME
        XmlSoapGetFault

    PURPOSE
        Return Fault code, reason, and details

    DESCRIPTION
        Return Fault code, reason, node, role, and details.  Fetches the Fault information
        and returns through user variables.  NULL may be supplied for any part
        which is not needed.  For lang, if the pointed-to variable is NULL, it
        will be set to the default language (that of the first reason).

    RETURNS
        (xmlerr) numeric error code, XMLERR_OK [0] on success

    ARGUMENTS
        ctx    (IN)  SOAP context
        msg    (I/O) SOAP message
        code   (OUT) Fault code
        reason (OUT) Human-readable fault reason
        lang   (IN)  Desired reason language (or NULL)
        node   (OUT) Fault Node
        role   (OUT) Role (next, none, or ulitmate receiver)
                           (unused in 1.1)
        detail (OUT) user-defined details

    SEE ALSO
        XmlSoapSetFault
*/
xmlerr XmlSoapGetFault(xmlsoapctx *ctx, xmldocnode *msg, oratext **code,
                       oratext **reason, oratext **lang,
                       oratext **node, oratext **role,
                       xmlelemnode **detail);

/*---------------------------------------------------------------------------
    NAME
        XmlSoapAddFaultReason

    PURPOSE
        Add additional Reason to Fault

    DESCRIPTION
        Add additional Reason to Fault.  The same reason text may be
        provided in different languages.  When the fault is created,
        the primary language/reason is added at that time; use this
        function to add additional translations of the reason.

    RETURNS
        (xmlerr) numeric error code, XMLERR_OK [0] on success

    ARGUMENTS
        ctx    (IN)  SOAP context
        msg    (I/O) SOAP message
        reason (IN)  Human-readable fault reason
        lang   (IN)  Language of reason

    SEE ALSO
        XmlSoapSetFault
*/
xmlerr XmlSoapAddFaultReason(xmlsoapctx *ctx, xmldocnode *msg,
                             oratext *reason, oratext *lang);

/*---------------------------------------------------------------------------
    NAME
        XmlSoapAddFaultSubDetail

    PURPOSE
        Add additional child to Fault Detail

    DESCRIPTION
        Add additional child to Fault Detail.  XmlSoapSetFault allows
        for creation of Deatail element with only one child. Extra
        children could be added with this function.

    RETURNS
        (xmlerr) numeric error code, XMLERR_OK [0] on success

    ARGUMENTS
        ctx    (IN)  SOAP context
        msg    (I/O) SOAP message
        sub    (IN)  subdetail tree.

    SEE ALSO
        XmlSoapSetFault
*/
xmlerr XmlSoapAddFaultSubDetail(xmlsoapctx *ctx, xmldocnode *msg,
                                xmlelemnode *sub);

/*---------------------------------------------------------------------------
    NAME
        XmlSoapGetReasonNum

    PURPOSE
        Determine the number of reasons in Fault element

    DESCRIPTION
        Determine the number of reasons in Fault element.
        Returns 0 if Fault is not present

    RETURNS
        (ub4) number of reasons in Fault element

    ARGUMENTS
        ctx    (IN)  SOAP context
        msg    (IN) SOAP message

    SEE ALSO
        XmlSoapGetFault, XmlSoapHasFault
*/
ub4 XmlSoapGetReasonNum(xmlsoapctx *ctx, xmldocnode *msg);

/*---------------------------------------------------------------------------
    NAME
        XmlSoapGetReasonLang

    PURPOSE
        Get a lang of a reasons with a particular index.

    DESCRIPTION
        Returns lang of the Reason through user variable.
        NULL may be supplied. The reason is selected based on index.

    RETURNS
        Error code

    ARGUMENTS
        ctx    (IN)  SOAP context
        msg    (IN)  SOAP message
        indx   (IN)  index of fault reason
        lang   (OUT  Reason language

    SEE ALSO
        XmlSoapGetFault, XmlSoapHasFault, XmlSoapGetReasonNum
*/
xmlerr XmlSoapGetReasonLang(xmlsoapctx *ctx, xmldocnode *msg,
                          ub4 index, oratext **lang);

/*---------------------------------------------------------------------------
    NAME
        XmlSoapError

    PURPOSE
        To retrieve human readable representation of the error code.
        And optionally to retrieve the info about the error code
        of underlying layer. 

    DESCRIPTION
        Retrive human readable representation of the error code.
        Retrive (optionally) info about the error code
        of underlying layer. 

    RETURNS
        None

    ARGUMENTS
        ctx    (IN)  SOAP context
        con    (IN)  connection about which additional info is requested
        err    (IN)  error code for which human readable info will be returned
        suberr (OUT) error code from con
        submsg (OUT) human readable info about con error

    SEE ALSO
        XmlSoapError, XmlSoapGetErrorCode, XmlSoapGetError
*/
oratext *XmlSoapError(xmlsoapctx *ctx, xmlsoapcon *con,
                      xmlerr err, uword *suberr, oratext **submsg);

xmlerr XmlSoapGetBoolean(xmlsoapctx *ctx, xmlelemnode *elem,
                         oratext *name, boolean *value);
xmlerr XmlSoapSetBoolean(xmlsoapctx *ctx, xmlelemnode *elem,
                         boolean value, oratext *name);

#endif /* XMLSOAP_ORACLE */
