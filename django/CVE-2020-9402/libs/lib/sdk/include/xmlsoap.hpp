////////////////////////////////////////////////////////////////////////
// Copyright (c) 2004, 2005, Oracle. All rights reserved.  
//
// NAME
//   xmlsoap.hpp - Header for Oracle C++ interfaces for SOAP 
//                 
//
// DESCRIPTION
//   
//
////////////////////////////////////////////////////////////////////////

#ifndef XMLSOAP_CPP_ORACLE
#define XMLSOAP_CPP_ORACLE

#ifndef XML_CPP_ORACLE
#include <xml.hpp>
#endif

namespace OracleXml {

/*------------------------------------------------------------------------
                Package SOAP - Simple Object Access Protocol APIs

    See xmlsoap.h for details

    ----------------------------------------------------------------------*/

  namespace Soap {

/* DATATYPE SoapExceptionCode - Soap related exception codes
    It defines Soap exception codes
*/
    typedef enum SoapExceptionCode {
      SOAP_UNDEFINED_ERR       = 0,
      SOAP_OTHER_ERR           = 1
    } SoapExceptionCode;

/* DATATYPE SoapBinding - SOAP connection binding
    Binding for SOAP connections.  HTTP is currently the only choice.
*/
    typedef enum SoapBinding {
      BIND_NONE  = 0,  /* none */
      BIND_HTTP  = 1   /* HTTP */
    } SoapBinding;

/* DATATYPE SoapRole - Roles for SOAP nodes
*/
    typedef enum SoapRole {
      ROLE_UNSET = 0,  /* not specified */
      ROLE_NONE  = 1,  /* "none" */
      ROLE_NEXT  = 2,  /* "next" */
      ROLE_ULT   = 3   /* "ultimateReceiver" */
    } SoapRole;

/**********************************************************************
  Interface SoapException - Exceptions for SOAP API
**********************************************************************/

    class SoapException : public XmlException {
    public:

/*---------------------------------------------------------------------
    NAME
        getCode

    PURPOSE
        Get Oracle XML error code embedded in the exception 

    DESCRIPTION
        Virtual member function inherited from XMLException

    RETURNS
        (unsigned) numeric error code (0 on success)

    ARGUMENTS
        None
*/
      // virtual unsigned getCode() const = 0;

/*--------------------------------------------------------------------
    NAME
        getMessage

    PURPOSE
        Get Oracle XML error message

    DESCRIPTION
        Virtual member function inherited from XMLException

    RETURNS
        (oratext *) Error message

    ARGUMENTS
        None
*/
      // virtual oratext* getMessage() const = 0;

/*--------------------------------------------------------------------
    NAME
        getMesLang

    PURPOSE
        Get current language (encoding) of error messages

    DESCRIPTION
        Virtual member function inherited from XMLException

    RETURNS
        (oratext*) Current language (encoding) of error messages

    ARGUMENTS
        None
*/
      // virtual oratext* getMesLang() const = 0;

/*---------------------------------------------------------------------
    NAME
        getSoapCode

    PURPOSE
        Get Soap exception code embedded in the exception 

    DESCRIPTION
        This is a virtual member function that defines a prototype
	for implementation defined member functions returning SOAP
	API exception codes, defined in SoapExceptionCode, of the
	exceptional situations during execution

    RETURNS
        (SoapExceptionCode) exception code

    ARGUMENTS
        None
*/
      virtual SoapExceptionCode getSoapCode() const = 0;
    };

/**********************************************************************
             Interface ConnectRef - Interface one-line doc
**********************************************************************/

    template< typename Node> class ConnectRef {
    public:

/*---------------------------------------------------------------------
    NAME
        ~ConnectRef

    PURPOSE
         Destructor - destroys a SOAP connection object 

    DESCRIPTION
        Destroy a SOAP connection object made with createConnection 
	in MsgFactory and free any resources associated with it.

    RETURNS
        (void)

    ARGUMENT 
        none
*/
      ~ConnectRef() throw (SoapException); 


/*---------------------------------------------------------------------
    NAME
        call

    PURPOSE
        Send a SOAP message then wait for and return replied message 

    DESCRIPTION
        Sends a SOAP message over a connection and waits for the
	response. For more details see XmlSoapCall in xmlsoap.h

    RETURNS
        (DocumentRef) returned message

    ARGUMENTS 
        msg (IN) SOAP message to send 
*/
      DocumentRef< Node>* call( DocumentRef< Node>& msg)
	throw (SoapException);
    };

/**********************************************************************
             Interface MsgFactory - Interface one-line doc
**********************************************************************/

    template< typename Context, typename Node> class MsgFactory {
    public:

/*---------------------------------------------------------------------
    NAME
        MsgFactory

    PURPOSE
        Constructor

    DESCRIPTION
        Create and return SOAP Message Factory.  

    RETURNS
        (MsgFactory) object

    ARGUMENTS 
        ctx_ptr (IN) TContext object 
*/
      MsgFactory( Context* ctx_ptr) throw (SoapException);

/*---------------------------------------------------------------------
    NAME
        ~MsgFactory

    PURPOSE
        Destructor - destroy the SOAP message factory

    DESCRIPTION
        Destroy the SOAP factory. All memory allocated will be freed,
	and all connections closed. 

    RETURNS
        (void)

    ARGUMENTS
        none
*/
      ~MsgFactory() throw (SoapException);

/*---------------------------------------------------------------------
    NAME
        createMessage

    PURPOSE
        Create and return an empty SOAP message 

    DESCRIPTION
        See details for XmlSoapCreateMsg in xmlsoap.h.
	It returns an empty message as a pointer to its reference.
	The reference object should be explicitly deleted by the user
	when no longer needed.

    RETURNS
        (DocumentRef*) SOAP message, or an exception

    ARGUMENTS 
        None
*/
      DocumentRef< Node>* CreateMessage() throw (SoapException);

/*---------------------------------------------------------------------
    NAME
        destroyMessage

    PURPOSE
        Destroy a SOAP message created with createMessage

    DESCRIPTION
        Destroy a SOAP message created with createMessage.

    RETURNS
        (void)

    ARGUMENTS 
        msg (IN) SOAP message 

*/
      void destroyMessage(  DocumentRef< Node>& msg) throw (SoapException);

/*---------------------------------------------------------------------
    NAME
        createConnection

    PURPOSE
        Create a SOAP connection object 

    DESCRIPTION
        Create a SOAP connection object. See details on bindings
	and other parameteres in xmlsoap.h for XmlSoapCreateConnection.
	The connection reference object should explicitly deleted
	by the user.

    RETURNS
        (ConnectRef) connect object

    ARGUMENTS  
        bind (IN) connection binding 
	endp (IN) connection endpoint 
	buf  (I/O) data buffer (or NULL to have one allocated) 
	bufsiz (IN) size of data buffer (or 0 for default size)
	msgbuf (I/O) message buffer (or NULL to have one allocated)
        msgbufsiz (IN) size of message buffer (or 0 for default size)

*/
      ConnectRef< Node>*
      createConnection( SoapBinding bind, void *endp, oratext *buf,
			ubig_ora bufsiz, oratext *msgbuf,
			ubig_ora msgbufsiz) throw (SoapException);

/*---------------------------------------------------------------------
    NAME
        getEnvelope

    PURPOSE
        Return a SOAP part's envelope 

    DESCRIPTION
        Returns a SOAP part's envelope as a pointer to envelope
	element node

    RETURNS
        (Node*) pointre to the SOAP part's envelope

    ARGUMENTS 
        msg (IN) SOAP message
*/
      Node* getEnvelope( DocumentRef<Node>& msg)
	throw (SoapException);

/*---------------------------------------------------------------------
    NAME
        getHeader

    PURPOSE
        Return a SOAP message's envelope header 

    DESCRIPTION
        Returns a SOAP message's envelope header as a pointer
	to the header element node.

    RETURNS
        (Node*) pointer to the SOAP message's envelope header 

    ARGUMENTS 
        msg (IN) SOAP message 
*/
      Node* getHeader( DocumentRef< Node>& msg)
	throw (SoapException);

/*---------------------------------------------------------------------
    NAME
        getBody

    PURPOSE
        Return a SOAP message's envelope body 

    DESCRIPTION
        Returns a SOAP message's envelope body as a pointer to
	the body's element node.

    RETURNS
        (Node*) pointer to the SOAP message's envelope body 

    ARGUMENTS
        msg (IN) SOAP message 
*/
      Node* getBody( DocumentRef< Node>& msg)
	throw (SoapException);


/*---------------------------------------------------------------------
    NAME
        addHeaderElement

    PURPOSE
        Adds an element to SOAP header 

    DESCRIPTION
        Adds an element to SOAP header 

    RETURNS
        (Node*) pointer to the created element

    ARGUMENTS 
        msg (I/O) SOAP message 
        qname (IN) QName of element to add 
        uri (IN) Namespace URI of element to add 
*/
      Node* addHeaderElement( DocumentRef< Node>& msg,
			      oratext *qname, oratext *uri)
	throw (SoapException);

/*---------------------------------------------------------------------
    NAME
        getHeaderElement

    PURPOSE
        Gets an element from a SOAP header 

    DESCRIPTION
        Gets an element from a SOAP header as a pointer to its
	element node

    RETURNS
        (Node*) pointer to the named element

    ARGUMENTS 
        msg (IN) SOAP message 
	uri (IN) Namespace URI of element to get 
        local (IN) Local name of element to get 
*/
      Node* getHeaderElement( DocumentRef< Node>& msg,
			      oratext *uri, oratext *local)
	throw (SoapException);

/*---------------------------------------------------------------------
    NAME
        addBodyElement

    PURPOSE
        Adds an element to a SOAP message body. 

    DESCRIPTION
        Adds an element to a SOAP message body. 

    RETURNS
        (Node*) pointer to the created element 

    ARGUMENTS 
        msg (I/O) SOAP message 
        qname (IN) QName of element to add 
        uri (IN) Namespace URI of element to add 
*/
      Node* addBodyElement( DocumentRef< Node>& msg,
			    oratext *qname, oratext *uri)
	throw (SoapException);

/*---------------------------------------------------------------------
    NAME
        getBodyElement

    PURPOSE
        Get an element from a SOAP body 

    DESCRIPTION
        Gets an element from a SOAP body as a pointer to its element
	node

    RETURNS
        (Node*) pointer to the named element

    ARGUMENTS 
        msg (IN) SOAP message 
        uri (IN) Namespace URI of element to get 
        local (IN) Local name of element to get 
*/
      Node* getBodyElement( DocumentRef< Node>& msg,
			    oratext *uri, oratext *local)
	throw (SoapException);

/*---------------------------------------------------------------------
    NAME
        setRole

    PURPOSE
        Set role for SOAP header element

    DESCRIPTION
        Sets role for SOAP header element

    RETURNS
        (void)

    ARGUMENTS
        elem (IN) reference to the header element
	role (IN) role value
*/
      void setRole( ElementRef< Node>& elem, SoapRole role)
	throw (SoapException);

/*---------------------------------------------------------------------
    NAME
        getRole

    PURPOSE
        Get role from SOAP header element

    DESCRIPTION
        Gets role from SOAP header element

    RETURNS
        (SoapRole) header element's role

    ARGUMENTS
        elem (IN) reference to the header element
*/
      SoapRole getRole( ElementRef< Node>& elem)
	throw (SoapException);

/*---------------------------------------------------------------------
    NAME
        hasFault

    PURPOSE
        Determine if SOAP message contains Fault object

    DESCRIPTION
        Determines if SOAP message contains Fault object

    RETURNS
        (boolean) TRUE if there's a Fault, FALSE if not

    ARGUMENTS
        msg (I/O) SOAP message
*/
      boolean hasFault( DocumentRef< Node>& msg)
	throw (SoapException);

/*---------------------------------------------------------------------
    NAME
        setFault

    PURPOSE
        Set Fault in SOAP message

    DESCRIPTION
        Sets Fault in SOAP message. See xmlsoap.h for details

    RETURNS
        (void)

    ARGUMENTS
        msg    (I/O) SOAP message
	node   (IN)  URI of SOAP node which faulted
        code   (IN)  Fault code
	reason (IN)  Human-readable fault reason
	lang   (IN)  Language of reason
	role   (IN)  URI representing role, Role (1.2), unused (1.1)
	detail (IN)  detail elements (user-defined)
*/
      void setFault( DocumentRef< Node>& msg, oratext *node,
		     oratext *code, oratext *reason, oratext *lang,
		     oratext *role, ElementRef< Node>& detail)
	throw (SoapException);

/*---------------------------------------------------------------------
    NAME
        addFaultReason

    PURPOSE
        Add additional Reason to Fault

    DESCRIPTION
        Add additional Reason to Fault.  The same reason text may be
	provided in different languages.  When the fault is created,
	the primary language/reason is added at that time; use this
	function to add additional translations of the reason.

    RETURNS
        (void)

    ARGUMENTS
        msg    (I/O) SOAP message
	reason (IN)  Human-readable fault reason
	lang   (IN)  Language of reason
*/
      void addFaultReason( DocumentRef< Node>& msg,
			   oratext *reason, oratext *lang)
	throw (SoapException);

/*---------------------------------------------------------------------------
    NAME
        getFault

    PURPOSE
	Return Fault code, reason, and details

    DESCRIPTION
	Return Fault code, reason, and details.  Fetches the Fault information
	and returns through user variables.  NULL may be supplied for any part
	which is not needed.  For lang, if the pointed-to variable is NULL, it
	will be set to the default language (that of the first reason).

    RETURNS
        (Node) pointer to the details in the message, if any

    ARGUMENTS
        msg    (I/O) SOAP message
	code   (OUT) Fault code
	reason (OUT) Human-readable fault reason
	lang   (IN)  Desired reason language (or NULL)
        node   (OUT) Fault Node
        role   (OUT) Role (next, none, or ulitmate receiver) (unused in 1.1)
*/
    Node* getFault( DocumentRef< Node>& msg, oratext **code,
		    oratext **reason, oratext **lang,
		    oratext **node, oratext **role)
	throw (SoapException);

    ub4 getReasonNum( DocumentRef< Node>& msg);
    oratext* getreasonLang( DocumentRef< Node>& msg, ub4 idx)
	throw (SoapException);


/*---------------------------------------------------------------------------
    NAME
        setMustUnderstand

    PURPOSE
        Set mustUnderstand attribute for the SOAP header element

    DESCRIPTION
        Sets mustUnderstand attribute for the SOAP header element

    RETURNS
        (void)

    ARGUMENTS
        elem            (I/O) SOAP header element
        mustUnderstand  (IN)  mustUnderstand value (TRUE|FALSE)
*/
      void setMustUnderstand( ElementRef< Node>& elem,
			      boolean mustUnderstand)
	throw (SoapException);

/*---------------------------------------------------------------------------
    NAME
        getMustUnderstand

    PURPOSE
        Get mustUnderstand attribute from SOAP header element

    DESCRIPTION
        Gets mustUnderstand attribute from SOAP header element

    RETURNS
        (boolean) value of the mustUnderstand attribute

    ARGUMENTS
        elem           (IN)  SOAP header element
*/
      boolean getMustUnderstand( ElementRef< Node>& elem)
	throw (SoapException);

    };

  }

  using namespace Soap;

}

using namespace OracleXml;

#endif
