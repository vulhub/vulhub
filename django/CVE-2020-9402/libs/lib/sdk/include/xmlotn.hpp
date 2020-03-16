////////////////////////////////////////////////////////////////////////
// Copyright (c) 2003, 2009, Oracle and/or its affiliates.All rights reserved. 
//
// NAME
//   xmlotn.hpp - Header for Oracle C++ interfaces for OTN
//
//
// DESCRIPTION
//   public, generic C++ interfaces
//
////////////////////////////////////////////////////////////////////////

#ifndef XMLOTN_CPP_ORACLE
#define XMLOTN_CPP_ORACLE

extern "C" {

/* oratypes.h types are assumed to be known in all C++ interfaces and
   implementations
*/

#ifndef ORATYPES_ORACLE
# include <oratypes.h>
#endif
}

/*
  Multiple encodings are currently supported on the base of the oratext
type that is currently supposed to be used by all implementations.

  All strings are represented as oratext*. In general, it is possible
to represent all interfaces in this namespace as oratext* based
specializations of more general template interfaces with the type
parameter TString that is replaced with oratext* in these
specializations. Thus, it is possible to define a different set of
specializations based on the standard C++ class String. However,
the definition of such specializations requires significant
implementation work and is left for future, if ever is required.
*/

/*-------------------------------------------------------------------
           Package OracleXml - Package one-line doc

    OracleXml is the namespace for all XML C++ interaces.
    It includes class XmlException, root exception interface for all
    XML exceptions, and namespaces:
      _:: Ctx    - namespace for TCtx related declarations
      _:  Dom    - namespace for DOM related declarations
      _:  Parser - namespace for parser and schema validator declarations
      _:  IO     - namespace for input and output source declarations
      _:  Xsl    - namespace for XSLT related declarations
      _:  XPath  - namespace for XPath related declarations
      _:  Tools  - namespace for Tools::Factory related declarations
      _:_

    OracleXml is fully defined in this file, xml.hpp. Another
    namespace, XmlCtxNS, visible to users is defined in xmlctx.hpp.
    That namespace contains C++ definitions of datastructures
    corresponding to C level definitions of the xmlctx context and
    related data structures. While there is no need for users to
    know details of that namespace, xmlctx.hpp needs to be included
    in most application main modules (see demos for details)
--------------------------------------------------------------------*/

namespace OracleXml {

/**********************************************************************
            Interface XmlException - Interface one-line doc

    This is the root interface for all XML exceptions

**********************************************************************/

  class XmlException {
  public:

/*---------------------------------------------------------------------
    NAME
        getCode

    PURPOSE
        Get Oracle XML error code embedded in the exception 

    DESCRIPTION
        This is a virtual member function that defines a prototype
	for implementation defined functions returning Oracle XML
	error codes (like error codes defined in xml.h) of the
	exceptional situations during execution

    RETURNS
        (unsigned) numeric error code (0 on success)

    ARGUMENTS
        None
*/
    virtual unsigned getCode() const = 0;

/*--------------------------------------------------------------------
    NAME
        getMessage

    PURPOSE
        Get Oracle XML error message

    DESCRIPTION
        This is a virtual member function that defines a prototype for
	implementation defined functions returning Oracle XML
	error messages of the exceptional situations during execution

    RETURNS
        (oratext *) Error message

    ARGUMENTS
        None
*/
    virtual oratext* getMessage() const = 0;

/*--------------------------------------------------------------------
    NAME
        getMesLang

    PURPOSE
        Get current language (encoding) of error messages

    DESCRIPTION
        This is a virtual member function that defines a prototype for
	implementation defined functions returning current language
	(encoding) of error messages for the exceptional situations
	during execution

    RETURNS
        Current language (encoding) of error messages

    ARGUMENTS
        None
*/
    virtual oratext* getMesLang() const = 0;

  };

/*---------------------------------------------------------------------
                     Package Ctx - Package one-line doc

    Ctx is the namespace (sub-space of OracleXml) for TCtx
    (XML context) related types and interfaces.

-------------------------------------------------------------------*/

  namespace Ctx {

/* DATATYPE encoding - a particular supported encoding
    The following kinds of encodings (or encoding names) are supported:
    "data_encoding", "default_input_encoding", "input_encoding" - overwrites
    the previous one, "error_language" - gets overwritten by the language of
    the error handler, if any
*/
    typedef struct encoding {
      oratext *encname;
      oratext *encvalue;
    } encoding;

/* DATATYPE encodings - Array of encodings
    It defines an array of encodings
*/
    typedef struct encodings {
      unsigned  num;
      encoding *enc;
    } encodings;

/**********************************************************************
               Interface ErrorHandler - Interface one-line doc

    It is the root error handler class. It deals with local processing
    of errors, in particular in C implementation. It may throw
    XmlException in some implementations. But this is not specified
    in its signature in order to accomodate needs of all impementations.
    However, it can create exception objects. The error handler is
    passed to the TCtx constructor when TCtx is initialized.
    Implementations of this interface are provided by the user.

**********************************************************************/

    class ErrorHandler {
    public:

/*---------------------------------------------------------------------
    NAME
        handleError

    PURPOSE
        Handle current error

    DESCRIPTION
        This virtual member function defines a prototype for user
	defined functions implementing user defined actions for
	particular exceptional situations

    RETURNS
        (void)

    ARGUMENTS
        msg     (IN) Oracle error message
	errcode (IN) Oracle error code
*/
      virtual void handleError( oratext* msg, unsigned errcode) = 0;

/*---------------------------------------------------------------------
    NAME
        createException

    PURPOSE
        Create an exception for the last exceptional situation

    DESCRIPTION
        This virtual member function defines a prototype for user
	defined functions creating an XML exception based on the
	last exceptional situation processed by handleError()

    RETURNS
        (XmlException *) XmlException object

    ARGUMENTS
        None
*/
      virtual XmlException* createException() const = 0;

/*---------------------------------------------------------------------
    NAME
        getMesLang

    PURPOSE
        Get current language (encoding) of error messages

    DESCRIPTION
        This is a virtual member function that defines a prototype for
	user defined functions returning current language
	(encoding) of error messages for the exceptional situations
	during execution

    RETURNS
        (oratext *) Current language (encoding) of error messages

    ARGUMENTS
        None
*/
      virtual oratext* getMesLang() const = 0;

/*---------------------------------------------------------------------
    NAME
        ~ErrorHandler

    PURPOSE
        Virtual destructor - interface level handle to actual destructors

    DESCRIPTION
        It provides an interface level handle to actual destructors that
        can be invoked without knowing their names or implementations

    RETURNS
        (void)

    ARGUMENTS
        None
*/
      virtual ~ErrorHandler() {}

    };

/**********************************************************************
             Interface MemAllocator - Interface one-line doc

    It is a simple root interface to make the TCtx interface
    reasonably generic so that different allocator approaches can be
    used in the future. It is passed to the TCtx constructor
    when Tctx is initialized. It is a low level allocator that does not
    know the type of an object being allocated. The allocators with
    this interface can also be used directly. In this case the user
    is responsible for the explicit deallocation of objects (with
    dealloc)

**********************************************************************/

    class MemAllocator {
    public:

/*---------------------------------------------------------------------
    NAME
        alloc

    PURPOSE
        Allocates memory of given size

    DESCRIPTION
        This is a virtual member function that defines a prototype for
	user defined allocator functions

    RETURNS
        (void *) Pointer to memory allocated

    ARGUMENTS
        size (IN) memory size to be allocated
*/
      virtual void* alloc( ub4 size) = 0;

/*---------------------------------------------------------------------
    NAME
        dealloc

    PURPOSE
        deallocate memory pointed to by the argument

    DESCRIPTION
        This is a virtual member function that defines a prototype for
	user defined deallocator functions. Such deallocators are
	supposed to deallocate memory allocated by the alloc member
	functions

    RETURNS
        (void)

    ARGUMENTS
        ptr (IN) pointer to previously allocated memory
*/
      virtual void  dealloc( void* ptr) = 0;

/*---------------------------------------------------------------------
    NAME
        ~MemAllocator

    PURPOSE
        Virtual destructor - interface level handle to actual destructors

    DESCRIPTION
        It provides an interface level handle to actual destructors that
        can be invoked without knowing their names or implementations

    RETURNS
        (void)

    ARGUMENTS
        None
*/
      virtual ~MemAllocator() {}

    };

/**********************************************************************
                     Interface TCtx - Interface one-line doc

    This is an implicit interface to XML context implementations.
    It is primarily used for memory allocation, error (not exception)
    handling, and different encodings handling. The context interface
    is an implicit interface that is supposed to be used as type
    parameter. The name TCtx will be used as a corresponding
    type parameter name. Its actuall substitutions are instantiations
    of implementations parameterized (templatized) by real context
    implementations. In the case of errors XmlException might be
    thrown.

    All constructors create and initialize context implementations.
    In a multithreaded environment a separate context implementation
    has to be initialized for each thread.

    If the MemAllocator interface is passed as a parameter to the
    TCtx constructor, than, in many cases, it makes sense to overwrite
    the operator new. In this case all memory allocations in both C
    and C++ can be done by the same allocator. This works like this:

    EXAMPLE
      void* operator new( ub4 sz, MemAllocator* memalloc)
      {...return memalloc->alloc( sz);...}
      ...
      (extern) MemAllocator* mem_alloc (= initialize actual allocator) ;
      ...
      TCtx* ctxp = new( mem_alloc) TCtx( name, errh, mem_alloc, (...));
      ...
      ctxp->~TCtx();
      mem_alloc->dealloc( ctxp);
 **********************************************************************

/*---------------------------------------------------------------------
    NAME
        TCtx

    PURPOSE
        Default constructor

    DESCRIPTION
        This constructor creates the context object and initializes
	it with default values of parameters (see xml.h for
	a detailed description). It throws XmlException if fails
	to create a context object

    RETURNS
        (TCtx) Context object

    ARGUMENTS
        None
*/  /*
    TCtx() throw (XmlException);

/*---------------------------------------------------------------------
    NAME
        TCtx

    PURPOSE
        Same as above but takes parameters provided by the user

    DESCRIPTION
        This constructor creates the context object and initializes
	it with parameter values provided by the user. It throws
	XmlException if fails to create a context object

    RETURNS
        (TCtx) Context object

    ARGUMENTS
        name     (IN) user defined name of the context
	errh     (IN) user defined error handler
	memalloc (IN) user defined memory allocator
	encs     (IN) user specified encodings
*/  /*
    TCtx( oratext* name, ErrorHandler* errh = NULL,
          MemAllocator* memalloc = NULL, encodings* encs = NULL)
                                                throw (XmlException);

/*---------------------------------------------------------------------
    NAME
        TCtx

    PURPOSE
        Same as above.

    DESCRIPTION
        Same as above but takes an additinal parameter

    RETURNS
        Context object

    ARGUMENTS
        name       - user defined name of the context
	inpblksize - memory block size for input source
	errh       - user defined error handler
	memalloc   - user defined memory allocator
	encs       - user specified encodings

*/  /*
    TCtx( oratext* name, ub4 inpblksize, ErrorIfs* errh = NULL,
          MemAllocator* memalloc = NULL, encodings* encs = NULL)
                                                throw (XmlException);

/*---------------------------------------------------------------------
    NAME
        ~TCtx

    PURPOSE
        Destructor - clears space and destroys the implementation
 
    DESCRIPTION
        Destructor - should be called by the user the context object
	is no longer needed

    RETURNS
        (void)

    ARGUMENTS
        None
*/  /*
    ~TCtx();

/*---------------------------------------------------------------------
    NAME
        isSimple

    PURPOSE
        Get a flag saying if data encoding is simple

    DESCRIPTION
        Returns a flag saying whether the context's data encoding is
        "simple", i.e. single-byte per character, like ASCII or EBCDIC.

    RETURNS
        (boolean) TRUE of data encoding is "simple", FALSE otherwise

    ARGUMENTS
        None
*/  /*
    boolean isSimple() const;

/*---------------------------------------------------------------------
    NAME
        isUnicode

    PURPOSE
        Get a flag saying if data encoding is Unicode

    DESCRIPTION
        Returns a flag saying whether the context's data encoding is
        Unicode, i.e. UTF-16, with two-byte per character.

    RETURNS
        (boolean) TRUE if data encoding is Unicode, FALSE otherwise

    ARGUMENTS
        None
*/  /*
    boolean isUnicode() const;

/*---------------------------------------------------------------------
    NAME
        getEncoding

    PURPOSE
        Get data encoding in use by XML context

    DESCRIPTION
        Returns data encoding in use by XML context.  Ordinarily, the
        data encoding is chosen by the user, so this function is not
        needed.  However, if the data encoding is not specified, and
        allowed to default, this function can be used to return the
        name of that default encoding.

    RETURNS
        (oratext *) name of data encoding

    ARGUMENTS
        None
*/  /*
    oratext* getEncoding() const;

/*---------------------------------------------------------------------
    NAME
        getErrHandler

    PURPOSE
        Get Error Handler provided by the user

    DESCRIPTION
        This member functions returns Error Handler provided by
	the user when the context was created, or NULL if none
	were provided.

    RETURNS
        (ErrorHandler *) Pointer to the Error Handler object, or NULL

    ARGUMENTS
        None
*/  /*
    ErrorHandler* getErrHandler() const;

/*---------------------------------------------------------------------
    NAME
        getMemAllocator

    PURPOSE
        Get memory allocator

    DESCRIPTION
        This member function returns memory allocator provided by
	the user when the context was created, or default
	memory allocator. It is imortant that this memory
	allocator is used for all C level memory allocations

    RETURNS
        (MemAllocator*) Pointer to the memory allocator object

    ARGUMENTS
        None
*/  /*
    MemAllocator* getMemAllocator() const;

**********************************************************************/

  }

  using namespace Ctx;

/*---------------------------------------------------------------------
       Package Dom - namespace for DOM related types and interfaces

    DOM interfaces are represented as generic references to different
    implementations of the DOM spec. They are parameterized by Node
    that supports various specializatins and instantiations. Of them,
    the most important is xmlnode that corresponds to the current C
    implementation

    These generic references do not have a NULL like value. Any
    implementation should never create a reference with no state
    (like NULL). If there is need to signal that something has
    no state, an exception should be thrown.

    Many methods might throw the SYNTAX_ERR exception, if the DOM
    tree is incorrectly formed, or UNDEFINED_ERR, in the case
    of wrong parameters or unexpected NULL pointers .  If these are
    the only errors that a particular method might throw, it is not
    reflected in the method signature.

    Actual DOM trees do NOT dependent on the context (TCtx). However,
    manipulations on DOM trees in the current, xmlctx based implementation
    require access to the current context (TCtx). This is accomplished by
    passing the context pointer to the constructor of DOMImplRef. In
    multithreaded environment DOMImplRef is always created in the thread
    context and, so, has the pointer to the right context.

    DOMImplRef providea a way to create DOM trees. DomImplRef is a
    reference to the actual DOMImplementation object that is created when
    a regular, non-copy constructor of DomImplRef is invoked. This works
    well in multithreaded environment where DOM trees need to be shared,
    and each thread has a separate TCtx associated with it. This works
    equally well in a single threaded environment.

    DOMString is only one of encodings supported by Oracle
    implementations. The support of other encodings is Oracle's
    extension. The oratext* data type is used for all encodings.

    Interfaces in this package represent DOM level 2 Core interfaces
    according to http://www.w3.org/TR/DOM-Level-2-Core/core.html

----------------------------------------------------------------------*/

  namespace Dom {

/* DATATYPE DOMNodeType - DOM node types
    It defines types of DOM nodes
*/
    typedef enum DOMNodeType {
      UNDEFINED_NODE              = 0, // just some node, maybe erroneous
      ELEMENT_NODE                = 1,
      ATTRIBUTE_NODE              = 2,
      TEXT_NODE                   = 3,
      CDATA_SECTION_NODE          = 4,
      ENTITY_REFERENCE_NODE       = 5,
      ENTITY_NODE                 = 6,
      PROCESSING_INSTRUCTION_NODE = 7,
      COMMENT_NODE                = 8,
      DOCUMENT_NODE               = 9,
      DOCUMENT_TYPE_NODE          = 10,
      DOCUMENT_FRAGMENT_NODE      = 11,
      NOTATION_NODE               = 12
    } DOMNodeType;

/* DATATYPE DOMExceptionCode - codes for DOM exceptions
    It defines DOM exception codes. The UNDEFINED_ERR code
    indicates that there is no specific C++ code defined.
    However, in most cases there is a C level code defined
    in xmlerr.h.
*/
    typedef enum DOMExceptionCode {
      UNDEFINED_ERR               = 0,
      INDEX_SIZE_ERR              = 1,
      DOMSTRING_SIZE_ERR          = 2,
      HIERARCHY_REQUEST_ERR       = 3,
      WRONG_DOCUMENT_ERR          = 4,
      INVALID_CHARACTER_ERR       = 5,
      NO_DATA_ALLOWED_ERR         = 6,
      NO_MODIFICATION_ALLOWED_ERR = 7,
      NOT_FOUND_ERR               = 8,
      NOT_SUPPORTED_ERR           = 9,
      INUSE_ATTRIBUTE_ERR         = 10,
      INVALID_STATE_ERR           = 11,
      SYNTAX_ERR                  = 12,
      INVALID_MODIFICATION_ERR    = 13,
      NAMESPACE_ERR               = 14,
      INVALID_ACCESS_ERR          = 15,
      WRONG_NODE_TYPE_ERR       = 16
    } DOMExceptionCode;

/* DATATYPE AcceptNodeCodes - values returned by node filters
    Node filters are used by node iterators and tree walkers
*/
    typedef enum AcceptNodeCode {
      FILTER_ACCEPT   = 1,
      FILTER_REJECT   = 2,
      FILTER_SKIP     = 3
    } AcceptNodeCode;

/* DATATYPE WhatToShowCode - codes to filter certain node types
    They are used by node iterators and tree walkers
*/
    typedef unsigned long WhatToShowCode;

    const unsigned long   SHOW_ALL                       = 0xFFFFFFFF;
    const unsigned long   SHOW_ELEMENT                   = 0x00000001;
    const unsigned long   SHOW_ATTRIBUTE                 = 0x00000002;
    const unsigned long   SHOW_TEXT                      = 0x00000004;
    const unsigned long   SHOW_CDATA_SECTION             = 0x00000008;
    const unsigned long   SHOW_ENTITY_REFERENCE          = 0x00000010;
    const unsigned long   SHOW_ENTITY                    = 0x00000020;
    const unsigned long   SHOW_PROCESSING_INSTRUCTION    = 0x00000040;
    const unsigned long   SHOW_COMMENT                   = 0x00000080;
    const unsigned long   SHOW_DOCUMENT                  = 0x00000100;
    const unsigned long   SHOW_DOCUMENT_TYPE             = 0x00000200;
    const unsigned long   SHOW_DOCUMENT_FRAGMENT         = 0x00000400;
    const unsigned long   SHOW_NOTATION                  = 0x00000800;

/* DATATYPE RangeExceptionCode - codes for DOM 2 Range exceptions
    The Range interface throws both DOM and Range exceptions
*/
    typedef enum RangeExceptionCode {
      RANGE_UNDEFINED_ERR         = 0,
      BAD_BOUNDARYPOINTS_ERR      = 1,
      INVALID_NODE_TYPE_ERR       = 2
    } RangeExceptionCode;

/* DATATYPE CompareHowCode - codes to identify the comparison kind
    It defines which points of two ranges to compare
*/
    typedef enum CompareHowCode {
      START_TO_START  = 0,
      START_TO_END    = 1,
      END_TO_END      = 2,
      END_TO_START    = 3
    } CompareHowCode;

/**********************************************************************/

/**********************************************************************
       Interface DOMException - Exception class for DOM operations

    DOM operations raise exceptions only when an operation is impossible
    to perform for DOM logic related reasons. Implementations might
    need to handle a wider range of error conditions. In these cases
    inherited XMLException functionality can be used.

**********************************************************************/

    class DOMException : public XmlException {
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
        getDOMCode

    PURPOSE
        Get DOM exception code embedded in the exception 

    DESCRIPTION
        This is a virtual member function that defines a prototype
	for implementation defined member functions returning DOM
	exception codes, defined in DOMExceptionCode, of the
	exceptional situations during execution

    RETURNS
        (DOMExceptionCode) exception code

    ARGUMENTS
        None
*/
      virtual DOMExceptionCode getDOMCode() const = 0;
    };

    // forward declarations of all DOM interfaces

    template< typename Node> class NodeRef;
    template< typename Node> class DocumentRef;
    template< typename Node> class DocumentFragmentRef;
    template< typename Node> class ElementRef;
    template< typename Node> class AttrRef;
    template< typename Node> class CharacterDataRef;
    template< typename Node> class TextRef;
    template< typename Node> class CDATASectionRef;   
    template< typename Node> class CommentRef;
    template< typename Node> class ProcessingInstructionRef;
    template< typename Node> class EntityRef;
    template< typename Node> class EntityReferenceRef;
    template< typename Node> class NotationRef;
    template< typename Node> class DocumentTypeRef;
    template< typename Node> class DOMImplementation;
    template< typename Context, typename Node> class DOMImplRef;

    template< typename Node> class NodeList;
    template< typename Node> class NodeListRef;
    template< typename Node> class NamedNodeMap;
    template< typename Node> class NamedNodeMapRef;

    // Interface NodeData - not visible to the user

    template< typename Node> class NodeData {
    protected:
      NodeData();
      ~NodeData();
    };

/**********************************************************************
          Interface NodeRef - reference to generic DOM node
**********************************************************************/

    template< typename Node> class NodeRef :
      public NodeData< Node> {
    public:

/*---------------------------------------------------------------------
    NAME
        NodeRef

    PURPOSE
        Constructor

    DESCRIPTION
        This constructor is used to create references
	to a given node when at least one reference to this node or
	another node is already available. The node deletion flag is
	not copied and is set to FALSE.

    RETURNS
        (NodeRef) Node reference object

    ARGUMENTS
        nref (IN) reference to the node reference object
	nptr (IN) node to be referenced
*/
      NodeRef( const NodeRef< Node>& nref, Node* nptr);

/*---------------------------------------------------------------------
    NAME
        NodeRef

    PURPOSE
        Copy constructor

    DESCRIPTION
        This copy constructor is used to create additional references
	to the node when at least one reference is already available.
	The node deletion flag is not copied and is set to FALSE.

    RETURNS
        (NodeRef) Node reference object

    ARGUMENTS
        nref (IN) reference to the reference object to be copied
*/
      NodeRef( const NodeRef< Node>& nref);

/*---------------------------------------------------------------------
    NAME
        ~NodeRef

    PURPOSE
        Public default destructor

    DESCRIPTION
        This is the default destructor. It cleans the reference to
	the node and, if the node is marked for deletion, deletes
	the node. If the node was marked for deep deletion, the tree
	under the node is also deleted (deallocated). It is usually
	called by the environment. But it can be called by the user
	directly if necessary.

    RETURNS
        (void)

    ARGUMENTS
        None
*/
      ~NodeRef();

/*---------------------------------------------------------------------
    NAME
        resetNode

    PURPOSE
        Reset NodeRef to reference another node

    DESCRIPTION
        This function resets NodeRef to reference Node given
	as an argument

    RETURNS
        (void) 

    ARGUMENTS
	nptr (IN) node to be referenced
*/
      void resetNode( Node* nptr);

/*---------------------------------------------------------------------
    NAME
        getNodeName

    PURPOSE
         Get node's name as NULL-terminated string

    DESCRIPTION
        Returns the (fully-qualified) name of the node (in the data
	encoding) as a NULL-terminated string, for example "bar\0"
	or "foo:bar\0". Some node kinds have fixed names: "#text",
	"#cdata-section", "#comment", "#document", "#document-fragment".
	The name of a node cannot changed once it is created.

    RETURNS
        (oratext*) name of node in data encoding

    ARGUMENTS
        None
*/
      oratext* getNodeName() const;

/*---------------------------------------------------------------------
    NAME
        getNodeValue

    PURPOSE
        Get node's value as NULL-terminated string

    DESCRIPTION
        Returns the "value" (associated character data) for a node as a
        NULL-terminated string.  Character and general entities will have
        been replaced.  Only Attr, CDATA, Comment, PI and Text nodes have
        values, all other node types have NULL value.

    RETURNS
        (oratext *) value of node

    ARGUMENTS
        None
*/
      oratext* getNodeValue() const;

/*---------------------------------------------------------------------
    NAME
        setNodeValue

    PURPOSE
        Set node's value as NULL-terminated string

    DESCRIPTION
        Sets a node's value (character data) as a NULL-terminated
	string. Does not allow setting the value to NULL.  Only Attr,
	CDATA, Comment, PI, and Text nodes have values. Trying to set
	the value of another kind of node is a no-op.  The new value
	must be in the data encoding! It is not verified, converted,
	or checked. The value is NOT copied, its pointer is just
	stored. The user is responsible for persistence and freeing of
	that data.

	It throws the NO_MODIFICATION_ALLOWED_ERR exception, if no
	modifications are allowed, or UNDEFINED_ERR, with an
	appropriate Oracle XML error code (see xml.h), in the case of
	an implementation defined error.

    RETURNS
        (void) 

    ARGUMENTS
        data (IN) node's new value
*/
      void setNodeValue( oratext* data) throw (DOMException);

/*---------------------------------------------------------------------
    NAME
        getNodeType

    PURPOSE
        get DOMNodeType of the node

    DESCRIPTION
        Returns DOMNodeType of the node

    RETURNS
        (DOMNodeType) of the node

    ARGUMENTS
        None
*/
      DOMNodeType getNodeType() const;

/*---------------------------------------------------------------------
    NAME
        getParentNode

    PURPOSE
        Get parent node of this node

    DESCRIPTION
        Returns the parent node, or NULL, if this 
	node has no parent

    RETURNS
        (Node*) the parent node, or NULL

    ARGUMENTS
        None
*/
      Node* getParentNode() const;

/*---------------------------------------------------------------------
    NAME
        hasChildNodes

    PURPOSE
        Test if this node has children

    DESCRIPTION
        Tests if this node has children.  Only Element, Document,
	DTD, and DocumentFragment nodes may have children.

    RETURNS
        (boolean) TRUE if this node has any children

    ARGUMENTS
        None
*/
      boolean hasChildNodes() const;

/*---------------------------------------------------------------------
    NAME
        getChildNodes

    PURPOSE
        Get children of this node

    DESCRIPTION
        Returns the list of child nodes, or NULL
	if this node has no children. Only Element, Document, DTD,
	and DocumentFragment nodes may have children; all other types
	will return NULL. In the current implementation, the list of
	child nodes is live; all changes in the original node are
	reflected immediately. Because of this, side effects can be
	present for some DOM tree manipulation styles, in particular,
	in multi-threaded environments.

    RETURNS
        (NodeList*) the list of child nodes

    ARGUMENTS
        None
*/
      NodeList< Node>* getChildNodes() const;

/*---------------------------------------------------------------------------
    NAME
        numChildNodes

    PURPOSE
        Return number of children of node

    DESCRIPTION
        Returns the number of children of a node.  Only Element, Document,
        DTD, and DocumentFragment nodes may have children, all other types
        return 0.  

    RETURNS
        (ub4) number of children of node

    ARGUMENTS
        none
*/
      ub4 numChildNodes() const;

/*---------------------------------------------------------------------
    NAME
        getFirstChild

    PURPOSE
        Get the first child node of this node

    DESCRIPTION
        Returns the first child node, or NULL, if this 
	node has no children

    RETURNS
        (Node*) the first child node, or NULL

    ARGUMENTS
        None
*/
      Node* getFirstChild() const;

/*---------------------------------------------------------------------
    NAME
        getLastChild

    PURPOSE
        Get the last child node of this node

    DESCRIPTION
        Returns the last child node, or NULL, if this 
	node has no children

    RETURNS
        (Node*) the last child node, or NULL

    ARGUMENTS
        None
*/
      Node* getLastChild() const;

/*---------------------------------------------------------------------
    NAME
        getPreviousSibling

    PURPOSE
        Get the previous sibling node of this node

    DESCRIPTION
        Returns the previous sibling node, or NULL,
	if this node has no previous siblings

    RETURNS
        (Node*) the previous sibling node, or NULL

    ARGUMENTS
        None
*/
      Node* getPreviousSibling() const;

/*---------------------------------------------------------------------
    NAME
        getNextSibling

    PURPOSE
        Get the next sibling node of this node

    DESCRIPTION
        Returns the next sibling node, or NULL,
	if this node has no next sibling

    RETURNS
        (Node*) the next sibling node, or NULL

    ARGUMENTS
        None
*/
      Node* getNextSibling() const;

/*---------------------------------------------------------------------
    NAME
        insertBefore

    PURPOSE
        Insert new child into node's list of children

    DESCRIPTION
        Inserts the node newChild before the existing child node
	refChild in this node.  refChild must be a child of this
        node.  If newChild is a DocumentFragment, all of its children
        are inserted (in the same order) before refChild; the
	DocumentFragment node itself is not.  If newChild is already in
	the DOM tree, it is first removed from its current position.

    RETURNS
        (Node*) pointer to the node being inserted

    ARGUMENTS
        newChild (IN) reference to the node to insert
	refChild (IN) reference to the node to insert before
*/
      Node* insertBefore( NodeRef& newChild, NodeRef& refChild)
	throw (DOMException);

/*---------------------------------------------------------------------
    NAME
        insertBeforeWithPtr

    PURPOSE
        Insert new child into node's list of children

    DESCRIPTION
        Same as insertBefore except that inserted node is passed by
	pointer

    RETURNS
        (Node*) pointer to the node being inserted

    ARGUMENTS
        newChildPtr (IN) pointer to the node to insert
	refChild    (IN) reference to the node to insert before
*/
      Node* insertBeforeWithPtr( Node* newChildPtr, NodeRef& refChild)
	throw (DOMException);

/*---------------------------------------------------------------------
    NAME
        replaceChild

    PURPOSE
        Replace an existing child of a node

    DESCRIPTION
        Replaces the child node oldChild with the new node newChild in
        this node's children list, and returns oldChild (which is now
	orphaned, with a NULL parent).  If newChild is a DocumentFragment,
	all of its children are inserted in place of oldChild; the
	DocumentFragment node itself is not.  If newChild is already in
	the DOM tree, it is first removed from its current position.

    RETURNS
        (Node*) pointer to the node replaced

    ARGUMENTS
        newChild (IN) reference to new node being substituted
        oldChild (IN) reference to the node to be substituted
*/
      Node* replaceChild( NodeRef& newChild, NodeRef& oldChild)
	throw (DOMException);

/*---------------------------------------------------------------------
    NAME
        replaceChildWithPtr

    PURPOSE
        Replace an existing child of a node

    DESCRIPTION
        Same as replaceChild except that new node is passed by pointer

    RETURNS
        (Node*) pointer to the node replaced

    ARGUMENTS
        newChildPtr (IN) pointer to new node being substituted
        oldChild    (IN) reference to the node to be substituted
*/
      Node* replaceChildWithPtr( Node* newChildPtr, NodeRef& oldChild)
	throw (DOMException);

/*---------------------------------------------------------------------
    NAME
        removeChild

    PURPOSE
        Remove an existing child node

    DESCRIPTION
        Removes the node from this node's list of children and returns
	it. The node is orphaned; its parent will be NULL after removal.

    RETURNS
        (Node*) pointer to the node removed

    ARGUMENTS
        oldChild (IN) reference to the node to be removed
*/
      Node* removeChild( NodeRef& oldChild) throw (DOMException);

/*---------------------------------------------------------------------
    NAME
        appendChild

    PURPOSE
        Append new child to node's list of children

    DESCRIPTION
        Appends the node to the end of this node's list of children and
        returns the new node.  If newChild is a DocumentFragment, all of
        its children are appended in original order; the
	DocumentFragment node itself is not. If newChild is already in
	the DOM tree, it is first removed from its current position.

    RETURNS
        (Node*) pointer to the node appended

    ARGUMENTS
        newChild (IN) reference to the node to add
*/
      Node* appendChild( NodeRef& newChild) throw (DOMException);

/*---------------------------------------------------------------------
    NAME
        appendChildWithPtr

    PURPOSE
        Append new child to node's list of children

    DESCRIPTION
        Same as appendChild except that new node is passed by pointer

    RETURNS
        (Node*) pointer to the node appended

    ARGUMENTS
        newChildPtr (IN) pointer to the node to add
*/
      Node* appendChildWithPtr( Node* newChildPtr) throw (DOMException);

/*---------------------------------------------------------------------
    NAME
        hasAttributes

    PURPOSE
        Tests if this node has attributes

    DESCRIPTION
        Returns TRUE if this node has attributes, if it is an element.
	Otherwise, it returns FALSE. Note that for nodes that are not
	elements, it always returns FALSE.

    RETURNS
        (boolean) TRUE is this node is an element and has attributes

    ARGUMENTS
        None
*/
      boolean hasAttributes() const;

/*---------------------------------------------------------------------
    NAME
        getAttributes

    PURPOSE
        Get attributes of this node

    DESCRIPTION
        Returns NamedNodeMap of attributes of this
	node, or NULL if it has no attributes. Only element nodes
	can have attribute nodes. For other node kinds, NULL is
	always returned. In the current implementation, the node map
	of child nodes is live; all changes in the original node are
	reflected immediately. Because of this, side effects can be
	present for some DOM tree manipulation styles, in particular,
	in multi-threaded environments.

    RETURNS
        (NamedNodeMap*) NamedNodeMap of attributes

    ARGUMENTS
        None
*/
      NamedNodeMap< Node>* getAttributes() const;

/*---------------------------------------------------------------------------
    NAME
        numAttributes

    PURPOSE
        Return number of attributes of element

    DESCRIPTION
        Returns the number of attributes of an element.

    RETURNS
        (ub4) number of attributes of node

    ARGUMENTS
        none
*/
      ub4 numAttributes() const;

/*---------------------------------------------------------------------
    NAME
        getOwnerDocument

    PURPOSE
        Get the owner document of this node

    DESCRIPTION
        Returns the document node associated with this node.  It is
	assumed that the document node type is derived from the node
	type. Each node may belong to only one document, or may not
	be associated with any document at all, e.g. immediately after
	it was created on user's request. The "owning" document [node]
	is returned, or the WRONG_DOCUMENT_ERR exception is thrown.

    RETURNS
        (Node*) the owning document node

    ARGUMENTS
        None
*/
      Node* getOwnerDocument() const throw (DOMException);

/*---------------------------------------------------------------------
    NAME
        cloneNode

    PURPOSE
        Clone this node

    DESCRIPTION
        Creates and returns a duplicate of this node. The duplicate
	node has no parent.  Cloning an Element copies all attributes
	and their values, including those generated by the XML
	processor to represent defaulted attributes, but it does not
	copy any text it contains unless it is a deep clone, since the
	text is contained in a child Text node.  Cloning any other type
	of node simply returns a copy of the node. If deep is TRUE, all
	children of the node are recursively cloned, and the cloned
	node will have cloned children; a non-deep clone will have no
	children. If the cloned node is not inserted into another tree
	or fragment, it probably should be marked, through its
	reference, for deletion (by the user).

    RETURNS
        (Node*) duplicate (cloned) node

    ARGUMENTS
        deep (IN) indicates if children should be recursively cloned
*/
      Node* cloneNode( boolean deep);

/*---------------------------------------------------------------------
    NAME
        normalize

    PURPOSE
        Merge adjacent text nodes

    DESCRIPTION
        "Normalizes" the subtree rooted at an element, i.e. merges
	adjacent Text nodes children of elements.  Note that adjacent
	Text nodes will never be created during a normal parse, only
	after manipulation of the document with DOM calls.

    RETURNS
        (void)

    ARGUMENTS
        None
*/
      void normalize();

/*---------------------------------------------------------------------
    NAME
        getNamespaceURI

    PURPOSE
        Get namespace URI of this node as a NULL-terminated string

    DESCRIPTION
        Returns the namespace URI of this node (in the data encoding)
	as a NULL-terminated string.  If the node's name is not
	qualified (does not contain a namespace prefix), it will have
	the default namespace in effect when the node was created
	(which may be NULL).

    RETURNS
        (oratext*) namespace URI of this node

    ARGUMENTS
        None
*/
      oratext* getNamespaceURI() const;

/*---------------------------------------------------------------------
    NAME
        getPrefix      

    PURPOSE
        Get namespace prefix of this node

    DESCRIPTION
        Returns the namespace prefix of this node (in the data encoding)
	(as a NULL-terminated string). If this node's name is not fully
	qualified (has no prefix), NULL is returned.

    RETURNS
        (oratext*) namespace prefix of this node

    ARGUMENTS
        None
*/
      oratext* getPrefix() const;

/*---------------------------------------------------------------------
    NAME
        setPrefix

    PURPOSE
        Set namespace prefix of this node

    DESCRIPTION
        Sets the namespace prefix of this node (as NULL-terminated
	string). Does not verify the prefix is defined! And does not
	verify that the prefix is in the current data encoding. Just
	causes a new QName to be formed from the new prefix and the old
	local name.

	It throws the NO_MODIFICATION_ALLOWED_ERR exception, if no
	modifications are allowed. Or it throws NAMESPACE_ERR if the
	namespaceURI of this node is null, or if the specified prefix
	is "xml" and the namespaceURI of this node is different from
	"http://www.w3.org/XML/1998/namespace", or if this node is an
	attribute and the specified prefix is "xmlns" and the
	namespaceURI of this node is different from
	"http://www.w3.org/2000/xmlns/". Note that the
	INVALID_CHARACTER_ERR exception is never thrown since it is
	not checked how the prefix is formed

    RETURNS
        (void)

    ARGUMENTS
        prefix (IN) new namespace prefix
*/
      void setPrefix( oratext* prefix) throw (DOMException);

/*---------------------------------------------------------------------
    NAME
        getLocalName

    PURPOSE
        Get local name of this node

    DESCRIPTION
        Returns local name (local part of the qualified name) of this
	node (in the data encoding) as a NULL-terminated string.  If
	this node's name is not fully qualified (has no prefix), then
	the local name is the same as the name.

    RETURNS
        (oratext*) local name of this node

    ARGUMENTS
        None
*/
      oratext* getLocalName() const;

/*---------------------------------------------------------------------------
    NAME
        prefixToURI

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
        prefix (IN) prefix to map
*/
      oratext* prefixToURI( oratext* prefix) const;

/*---------------------------------------------------------------------
    NAME
        isSupported

    PURPOSE
        Tests if specified feature is supported by the implementation

    DESCRIPTION
        Tests if the feature, specified by the arguments, is supported
	by the DOM implementation of this node.

    RETURNS
        (boolean) TRUE is specified feature is supported

    ARGUMENTS
	feature (IN) package name of the feature to test
        version (IN) version # of the package name to test
*/
      boolean isSupported( oratext* feature, oratext* version) const;

/*---------------------------------------------------------------------
    NAME
        markToDelete

    PURPOSE
        Sets the mark to delete the referenced node

    DESCRIPTION
        Sets the mark indicating that the referenced node should be
	deleted at the time when destructor of this reference is called.
	All other references to the node become invalid. This behavior
	is inherited by all other reference classes. This member
	function is Oracle extension.

    RETURNS
        (void)

    ARGUMENTS
        None
*/
      void markToDelete();

/*---------------------------------------------------------------------
    NAME
        getNoMod

    PURPOSE
        Tests if no modifications are allowed for this node

    DESCRIPTION
        Tests if no modifications are allowed for this node and the
	DOM tree it belongs to. This member function is Oracle
	extension.

    RETURNS
        (boolean) TRUE if no modifications are allowed

    ARGUMENTS
        None
*/
      boolean getNoMod() const;

    protected:

      // DOM nodes are created explicitly only by calling create member
      // functions on DocumentRef and DOMImplRef objects So, the user
      // should have no access to default constructors. This means
      // that such constructors should be explicitly private or protected.
      // This default constructor is explicitly specified as protected
      // because it is implicitly used by the environment when objects
      // of derived classes are created. Default constructors of
      // references to other nodes are private.
      NodeRef();
    };

/**********************************************************************
         Interface DocumentRef - reference to Document nodes
**********************************************************************/

    template< typename Node> class DocumentRef
      : public NodeRef< Node> {
    public:

      //      ~DocumentRef();

/*---------------------------------------------------------------------
    NAME
        DocumentRef

    PURPOSE
        Constructor

    DESCRIPTION
        This is a constructor.

    RETURNS
        (DocumentRef) Node reference object

    ARGUMENTS
        nref (IN) reference to the object to provide the context
	nptr (IN) document node to be referenced
*/
      DocumentRef( const NodeRef< Node>& nref, Node* nptr);

/*---------------------------------------------------------------------
    NAME
        DocumentRef

    PURPOSE
        Copy constructor

    DESCRIPTION
        This is a copy constructor.

    RETURNS
        (DocumentRef) Node reference object

    ARGUMENTS
        nref (IN) reference to the reference object to be copied
*/
      DocumentRef( const DocumentRef< Node>& nref);

/*---------------------------------------------------------------------
    NAME
        getDoctype

    PURPOSE
        Get DTD (Document Type) associated with the document

    DESCRIPTION
        Returns the DTD node associated with this document.
	After this call, a DocumentTypeRef object needs to be created
	with an appropriate constructor in order to call its member
	functions. The DTD tree cannot be edited.

    RETURNS
        (Node*) DTD node

    ARGUMENTS
        None
*/
      Node* getDoctype() const;

/*---------------------------------------------------------------------
    NAME
        getImplementation

    PURPOSE
        Get DOMImplementation object associated with the document

    DESCRIPTION
        Returns DOMImplementation object that was used to create
	this document. When the DOMImplementation object is
	destructed, all document trees associated with it are
	also destructed.

    RETURNS
        (DOMImplementation*) DOMImplementation object

    ARGUMENTS
        None
*/
      DOMImplementation< Node>* getImplementation() const;

/*---------------------------------------------------------------------
    NAME
        getDocumentElement

    PURPOSE
        Get top-level element of this document

    DESCRIPTION
        Returns the root element (node) of the DOM tree.
        Each document has only one uppermost Element node, called
        the root element. If there is no root element, NULL is
	returned. This can happen when the document tree is
	being constructed.

    RETURNS
        (Node*) Root element

    ARGUMENTS
        None
*/
      Node* getDocumentElement() const;

/*---------------------------------------------------------------------
    NAME
        createElement

    PURPOSE
        Create an element node

    DESCRIPTION
        Creates an element node with the given tag name (which should
	be in the data encoding). The new node is an orphan with no
	parent. The tagname is NOT copied, its pointer is just stored.
	The user is responsible for persistence and freeing of that data.

	Note that the tag name of an element is CASE SENSITIVE.  This
	is the non-namespace aware function: the new node will have
	NULL namespace URI and prefix, and its local name will be the
	same as its tag name, even if the tag name specified is a QName.

    RETURNS
        (Node*) New element node

    ARGUMENTS
        tagname (IN) tag name
*/
      Node* createElement( oratext* tagname)
	throw (DOMException);

/*---------------------------------------------------------------------
    NAME
        createDocumentFragment

    PURPOSE
        Create a document fragment

    DESCRIPTION
        Creates an empty Document Fragment node.  A document fragment
	is treated specially when it is inserted into a DOM tree:
	the children of the fragment are inserted in order instead of
	the fragment node itself.  After insertion, the fragment node
	will still exist, but have no children. The name of a fragment
	node is always "#document-fragment".

    RETURNS
        (Node*) new document fragment node

    ARGUMENTS
        None
*/
      Node* createDocumentFragment()
	  throw (DOMException);

/*---------------------------------------------------------------------
    NAME
        createTextNode

    PURPOSE
        Create a text node

    DESCRIPTION
        Creates a text node with the given initial data (which must be
        non-NULL and in the data encoding).  The data may be NULL; if
        provided, it is not verified, converted, checked, or parsed
        (entities will not be expanded). The name of the node is always
	"#text". The new node is an orphan with no parent. The text data
	is NOT copied, its pointer is just stored.  The user is
	responsible for persistence and freeing of that data.

    RETURNS
        (Node*) new text node

    ARGUMENTS
        data (IN) data for new text node
*/
      Node* createTextNode( oratext* data)
	throw (DOMException);

/*---------------------------------------------------------------------
    NAME
        createComment

    PURPOSE
        Create a comment node

    DESCRIPTION
        Creates a comment node with the given initial data (which must
	be in the data encoding).  The data may be NULL, if provided;
	it is not verified, converted, or checked. The name of the comment
	node is always "#comment". The new node is an orphan with no
	parent. The comment data is NOT copied, its pointer is just
	stored.  The user is responsible for persistence and freeing
	of that data.

    RETURNS
        (Node*) New comment node

    ARGUMENTS
        data (IN) data for new comment node
*/
      Node* createComment( oratext* data)
	throw (DOMException);

/*---------------------------------------------------------------------
    NAME
        createCDATASection

    PURPOSE
        Create a CDATA node

    DESCRIPTION
        Creates a CDATA section node with the given initial data (which
        should be in the data encoding).  A CDATA section is considered
        verbatim and is never parsed; it will not be joined with adjacent
        text nodes by the normalize operation.  The initial data may be
        NULL, if provided; it is not verified, converted, or checked.
	The name of a CDATA node is always "#cdata-section". The new node
	is an orphan with no parent. The CDATA is NOT copied, its pointer
	is just stored.  The user is responsible for persistence and
	freeing of that data.

    RETURNS
        (Node*) New CDATA node

    ARGUMENTS
        data (IN) data for new CDATA node
*/
      Node* createCDATASection( oratext* data)
	throw (DOMException);

/*---------------------------------------------------------------------
    NAME
        createProcessingInstruction

    PURPOSE
        Create a processing instruction node

    DESCRIPTION
        Creates a processing instruction node with the given target and
        data (which should be in the data encoding). The data may be
	NULL, but the target is required and cannot be changed.  The
	target and data are not verified, converted, or checked. The
	name of the node is the same as the target. The new node is an
	orphan with no parent. The target and data are NOT copied;
	their pointers are just stored.  The user is responsible for
	persistence and freeing of that data.

    RETURNS
        (Node*) New PI node

    ARGUMENTS
        target (IN) target
        data   (IN) data for new PI node
*/
      Node* createProcessingInstruction( oratext* target, oratext* data)
	throw (DOMException);

/*---------------------------------------------------------------------
    NAME
        createAttribute

    PURPOSE
        Create an attribute node

    DESCRIPTION
        Creates an attribute node with the given name. This is the
	non-namespace aware function. The new attribute will have NULL
	namespace URI and prefix, and its local name will be the same as
	its name, even if the name specified is a QName. The new node is
	an orphan with no parent. The name is NOT copied, its pointer is
	just stored. The user is responsible for persistence and freeing
	of that data.

    RETURNS
        (Node*) New attribute node

    ARGUMENTS
        name (IN) name
*/
      Node* createAttribute( oratext* name) throw (DOMException);

/*---------------------------------------------------------------------
    NAME
        createEntityReference

    PURPOSE
        Create an entity reference node

    DESCRIPTION
        Creates an entity reference node; the name (which should be in
	the data encoding) is the name of the entity to be referenced.
	The named entity does not have to exist. The name is not verified,
	converted, or checked. The new node is an orphan with no parent.
	The entity reference name is NOT copied; its pointer is just
	stored. The user is responsible for persistence and freeing of
	that data.

	Note that entity reference nodes are never generated by the
	parser; instead, entity references are expanded as encountered.
	On output, an entity reference node will turn into a "&name;"
	style reference.

    RETURNS
        (Node*) New entity reference node

    ARGUMENTS
        name (IN) name
*/
      Node* createEntityReference( oratext* name) throw (DOMException);

/*---------------------------------------------------------------------
    NAME
        getElementsByTagName

    PURPOSE
        Get elements in the document by tag name

    DESCRIPTION
        Returns a list of all elements in the document with a given tag
	name, in document order (i.e. the order in which they would be
	encountered in a preorder traversal of the tree). The list should
	be freed by the user when it is no longer needed. The list is NOT
	live, it is a snapshot.  That is, if a new node which matched the
	tag name were added to the DOM after the list was returned, the
	list would not automatically be updated to include the node.

        The special name "*" matches all tag names; a NULL name matches
        nothing.  Note that tag names are CASE SENSITIVE, and should be
	in the data encoding or a mismatch might occur.

        This function is not namespace aware; the full tag names are
	compared. If two QNames with two different prefixes both of
	which map to the same URI are compared, the comparison will fail.

    RETURNS
        (NodeList< Node>*) List of nodes

    ARGUMENTS
        tagname (IN) tag name
*/
      NodeList< Node>* getElementsByTagName( oratext* tagname) const;

/*---------------------------------------------------------------------
    NAME
        createElementNS

    PURPOSE
        Create an element node with namespace information

    DESCRIPTION
        Creates an element with the given namespace URI and QName.  
        The new node is an orphan with no parent. The URI and QName
	are NOT copied, their pointers are just stored. The user is
	responsible for persistence and freeing of that data.

	Note that element names are CASE SENSITIVE, and the QName is
	required though the URI may be NULL.  The QName will be split
	into prefix and local parts. The tagName will be the full
	QName.

    RETURNS
        (Node*) New element node

    ARGUMENTS
        namespaceURI  (IN) new node's namespace URI
	qualifiedName (IN) new node's qualified name
*/
      Node* createElementNS( oratext* namespaceURI,
			     oratext* qualifiedName)
	throw (DOMException);

/*---------------------------------------------------------------------
    NAME
        createAttributeNS

    PURPOSE
        Create an attribute node with namespace information

    DESCRIPTION
        Creates an attribute node with the given namespace URI and QName.
	The new node is an orphan with no parent. The URI and QName
	are NOT copied, their pointers are just stored.  The user is
	responsible for persistence and freeing of that data.

    RETURNS
        (Node*) New attribute node

    ARGUMENTS
        namespaceURI  (IN) node's namespace URI
	qualifiedName (IN) node's qualified name
*/
      Node* createAttributeNS( oratext* namespaceURI,
			       oratext* qualifiedName)
	throw (DOMException);

/*---------------------------------------------------------------------
    NAME
        getElementsByTagNameNS

    PURPOSE
        Get elements in the document by tag name (namespace aware version)

    DESCRIPTION
        Returns a list of all elements in the document with a given
	namespace URI and local name, in document order (i.e. the order
	in which they would be encountered in a preorder traversal of the
	tree). The list should be freed by the user when it is no longer
	needed. The list is NOT	live, it is a snapshot.  That is, if a new
	node which matches the URI and local name were added to the DOM
	after the list was returned, the list would not automatically be
	updated to include the node.

        The URI and local name should be in the data encoding. The special
	name "*" matches all local names; a NULL local name matches
        nothing. Namespace URIs must always match, however, no wildcard is
        allowed.  Note that comparisons are CASE SENSITIVE.

    RETURNS
        (NodeList< Node>*) List of nodes containing all matched elements

    ARGUMENTS
        namespaceURI (IN) namespace URI to match
	localName    (IN) local name to match
*/
      NodeList< Node>* getElementsByTagNameNS( oratext* namespaceURI,
					       oratext* localName);

/*---------------------------------------------------------------------
    NAME
        getElementById

    PURPOSE
        Get an element given its ID

    DESCRIPTION
        Returns the element node which has the given ID. Throws
	NOT_FOUND_ERR if no element is found. The given ID should be
	in the data encoding or it might not match.

	Note that attributes named "ID" are not automatically of type ID;
	ID attributes (which can have any name) must be declared as type
	ID in the DTD or XML schema associated with the document.

    RETURNS
        (Node*)Element node

    ARGUMENTS
        elementId (IN) element id
*/
      Node* getElementById( oratext* elementId);

/*---------------------------------------------------------------------
    NAME
        importNode

    PURPOSE
        Import a node from another DOM

    DESCRIPTION
        Imports a node from one Document to another.  The new node is an
        orphan and has no parent. The original node is not modified in
	any way or removed from its document; instead, a new node is
	created with copies of all the original node's QName, prefix,
	namespace URI, and local name.

	The deep controls whether the children of the node are
	recursively imported.  If FALSE, only the node itself is imported,
	and it will have no children.  If TRUE, all descendents of the
	node will be imported as well, and an entire new subtree created.
	Elements will have only their specified attributes imported;
	non-specified (default) attributes are omitted.  New default
	attributes (for the destination document) are then added.
	Document and DocumentType nodes cannot be imported.

    RETURNS
        (Node*) New imported node

    ARGUMENTS
        importedNode (IN) node to import
	deep         (IN) recursively import subtree?
*/
      Node* importNode( NodeRef< Node>& importedNode,
			boolean deep) const
	throw (DOMException);

    private:

      // This is the default constructor.
      DocumentRef();
    };

/**********************************************************************
      Interface DocumentFragmentRef - reference to Fragment nodes
**********************************************************************/

    template< typename Node> class DocumentFragmentRef
      : public NodeRef< Node> {
    public:

      // ~DocumentFragmentRef() {}

/*---------------------------------------------------------------------
    NAME
        DocumentFragmentRef

    PURPOSE
        Constructor

    DESCRIPTION
        This constructor is used to create references to a given
	fragment node after a call to createDocumentFragment.

    RETURNS
        (DocumentFragmentRef) Node reference object

    ARGUMENTS
        node_ref (IN) reference to provide the context
	nptr     (IN) document fragment node to be referenced
*/
      DocumentFragmentRef( const NodeRef< Node>& node_ref,
			   Node* nptr);

/*---------------------------------------------------------------------
    NAME
        DocumentFragmentRef

    PURPOSE
        Copy constructor

    DESCRIPTION
        This is a copy constructor.

    RETURNS
        (DocumentFragmentRef) Node reference object

    ARGUMENTS
        nref (IN) reference to the reference object to be copied
*/
      DocumentFragmentRef( const DocumentFragmentRef< Node>& nref);

    private:

      // This is the default constructor.
      DocumentFragmentRef();
    };

/**********************************************************************
          Interface ElementRef - reference to Element nodes
**********************************************************************/

    template< typename Node> class ElementRef
      : public NodeRef< Node> {
    public:

      //      ~ElementRef();

/*---------------------------------------------------------------------
    NAME
        ElementRef

    PURPOSE
        Constructor

    DESCRIPTION
        This constructor is used to create references
	to a given element node after a call to createElement.

    RETURNS
        (ElementRef) Node reference object

    ARGUMENTS
        node_ref (IN) reference to provide the context
	nptr     (IN) element node to be referenced
*/
      ElementRef( const NodeRef< Node>& node_ref, Node* nptr);

/*---------------------------------------------------------------------
    NAME
        ElementRef

    PURPOSE
        Copy constructor

    DESCRIPTION
        This is a copy constructor.

    RETURNS
        (ElementRef) Node reference object

    ARGUMENTS
        nref (IN) reference to the reference object to be copied
*/
      ElementRef( const ElementRef< Node>& nref);

/*---------------------------------------------------------------------
    NAME
        getTagName

    PURPOSE
        Get element's tag name

    DESCRIPTION
        Returns the tag name of an element node which is supposed
	to have the same value as the node name from the node
	interface

    RETURNS
        (oratext*) element's name [in data encoding]

    ARGUMENTS
        None
*/
      oratext* getTagName() const;

/*---------------------------------------------------------------------
    NAME
        getAttribute

    PURPOSE
        Get attribute's value given its name

    DESCRIPTION
        Returns the value of an element's attribute (specified by name).
        Note that an attribute may have the empty string as its value,
        but cannot be NULL.

    RETURNS
        (oratext*) named attribute's value (in data encoding)

    ARGUMENTS
        name (IN) attribute's name (in data encoding)
*/
      oratext* getAttribute( oratext* name) const;

/*---------------------------------------------------------------------
    NAME
        setAttribute

    PURPOSE
        Set new attribute for this element and /or new value

    DESCRIPTION
        Creates a new attribute for an element with the given name and
	value (which should be in the data encoding). If the named attribute
	already exists, its value is simply replaced.  The name and value
	are not verified, converted, or checked. The value is not parsed,
	so entity references will not be expanded.

    RETURNS
        (void)

    ARGUMENTS
        name  (IN) attribute's name (data encoding)
        value (IN) attribute's value (data encoding)
*/
      void setAttribute( oratext* name, oratext* value)
                                              throw (DOMException);

/*---------------------------------------------------------------------
    NAME
        removeAttribute

    PURPOSE
        Remove attribute with specified name

    DESCRIPTION
        Removes an attribute specified by name. The attribute is removed
	from the element's list of attributes, but the attribute node
	itself is not destroyed.

    RETURNS
        (void)

    ARGUMENTS
        name (IN) attribute's name [data encoding]
*/
      void removeAttribute( oratext* name) throw (DOMException);

/*---------------------------------------------------------------------
    NAME
        getAttributeNode

    PURPOSE
        Get the attribute node given its name

    DESCRIPTION
        Returns the attribute node given its name. If the node
        is not an element or the named attribute does not exist,
        returns NULL.

    RETURNS
        (Node*) the attribute node

    ARGUMENTS
        name (IN) attribute's name (data encoding)
*/
      Node* getAttributeNode( oratext* name) const;

/*---------------------------------------------------------------------------
    NAME
        getAttributeNodeNS

    PURPOSE
        Get attribute by name (namespace aware version)

    DESCRIPTION
        Returns an element's attribute specified by URI and localname. If
        the node is not an element or the named attribute does not exist,
        returns NULL.

    RETURNS
        (Node *) the attribute

    ARGUMENTS
        uri   (IN) attribute's namespace URI [data encoding]
        local (IN) attribute's local name [data encoding]
*/
      Node* getAttributeNodeNS( oratext* namespaceURI,
				oratext* localName) const;
/*---------------------------------------------------------------------
    NAME
        setAttributeNode

    PURPOSE
        Set attribute node

    DESCRIPTION
        Adds a new attribute to an element.  If an attribute with the
	given name already exists, it is replaced and a pointer to the
	old attribute returned. If the attribute is new, it is added to
	the element's list and a pointer to the new attribute is
	returned.

    RETURNS
        (Node*) the attribute node (old or new)

    ARGUMENTS
        newAttr (IN) attribute node to add
*/
      Node* setAttributeNode( AttrRef< Node>& newAttr)
	throw (DOMException);

/*---------------------------------------------------------------------
    NAME
        setAttributeNodeNS

    PURPOSE
        Set attribute node (namespace aware version)

    DESCRIPTION
        Adds a new attribute to an element.  If an attribute with
	{newNode}'s URI and localname already exists, it is replaced
	and the old attribute returned.  If the attribute is new,
	it is added to the element's list and a pointer to the new
	attribute is returned

    RETURNS
        (Node*) the attribute node (old or new)

    ARGUMENTS
        newAttr (IN) attribute node to add
*/
      Node* setAttributeNodeNS( AttrRef< Node>& newAttr)
	throw (DOMException);

/*---------------------------------------------------------------------
    NAME
        removeAttributeNode

    PURPOSE
        Remove attribute node

    DESCRIPTION
        Removes an attribute from an element. Returns a pointer to
	the removed attribute or NULL

    RETURNS
        (Node*) the attribute node (old) or NULL

    ARGUMENTS
        oldAttr (IN) attribute node to remove
*/
      Node* removeAttributeNode( AttrRef< Node>& oldAttr)
	throw (DOMException);

/*---------------------------------------------------------------------
    NAME
        getElementsByTagName

    PURPOSE
        Get elements with given tag name

    DESCRIPTION
        Returns a list of all elements (in the document subtree rooted
	at the given node) with a given tag name, in the order in which
	they would be encountered in a preorder traversal of the
	osubtree. The tag name should be in the data encoding. The
	special name {"*"} matches all tag names; a NULL name matches
	nothing.  Tag names are CASE SENSITIVE.  This function is not
	namespace aware; the full tag names are compared. 

        The list should be freed when it is no longer needed. The list
	is NOT live, it is a snapshot.  That is, if a new node which
	matched the tag name were added to the DOM after the list was
	returned, the list would not automatically be updated to include
	the node.

    RETURNS
        (NodeList< Node>*) the list of elements

    ARGUMENTS
        name (IN) tag name to match (data encoding; "*" for any tag)
*/
      NodeList< Node>* getElementsByTagName( oratext* name);

/*---------------------------------------------------------------------------
    NAME
        getElementsByTagNameNS

    PURPOSE
        Obtain doc elements (namespace aware version)

    DESCRIPTION
        Returns a list of all elements (in the document subtree rooted at
	the given node) with a given namespace URI and local name, in the
	order in which they would be encountered in a preorder traversal
	of the tree. The URI and local name should be in the data encoding.
	The special local name "*" matches all local names; a NULL local
	name matches nothing.  Namespace URIs must always match, however,
	no wildcard is allowed.  Comparisons are CASE SENSITIVE.

        Like in getElementsByTagName the list should be freed when it is
	no longer needed. 

    RETURNS
        (NodeList< Node>*) new NodeList containing all matched elements

    ARGUMENTS
        namespaceURI (IN) namespace URI to match [data encoding]
        localName    (IN) local name to match [data encoding; "*" for all]
*/
      NodeList< Node>*
      getElementsByTagNameNS( oratext* namespaceURI,
			      oratext* localName);

/*---------------------------------------------------------------------
    NAME
        getAttributeNS

    PURPOSE
        Get attribute's value given its URI and localname

    DESCRIPTION
        Returns the value of an element's attribute (specified by URI and
        localname).  Note that an attribute may have the empty string as
        its value, but cannot be NULL.

    RETURNS
        (oratext *) named attribute's value (in data encoding)

    ARGUMENTS
        namespaceURI   (IN) attribute's namespace URI (data encoding)
        localName      (IN) attribute's local name (data encoding)

*/
      oratext* getAttributeNS( oratext* namespaceURI,
			       oratext* localName);

/*---------------------------------------------------------------------
    NAME
        setAttributeNS

    PURPOSE
        Set new attribute for the element and /or new value

    DESCRIPTION
        Creates a new attribute for an element with the given URI,
	localname and value (which should be in the data encoding).
	If the named attribute already exists, its value is simply
	replaced.  The name and value are not verified, converted, or
	checked. The value is not parsed, so entity references will not
	be expanded.

    RETURNS
        (void)

    ARGUMENTS
        namespaceURI  (IN) attribute's namespace URI (data encoding)
        qualifiedName (IN) attribute's qualified name (data encoding)
        value         (IN) attribute's value (data encoding)
*/
      void setAttributeNS( oratext* namespaceURI,
			   oratext* qualifiedName, oratext* value)
	throw (DOMException);

/*---------------------------------------------------------------------
    NAME
        removeAttributeNS

    PURPOSE
        Remove attribute with specified URI and local name

    DESCRIPTION
        Removes an attribute specified by URI and local name. The
	attribute is removed from the element's list of attributes, but
	the attribute node itself is not destroyed.

    RETURNS
        (void)

    ARGUMENTS
        namespaceURI (IN) attribute's namespace URI
        localName    (IN) attribute's local name
*/
      void removeAttributeNS( oratext* namespaceURI, oratext* localName)
	throw (DOMException);

/*---------------------------------------------------------------------
    NAME
        hasAttribute

    PURPOSE
        Check if named attribute exist

    DESCRIPTION
        Determines if an element has a attribute with the given name

    RETURNS
        (boolean) TRUE if element has attribute with given name

    ARGUMENTS
        name (IN) attribute's name (data encoding)
*/
      boolean hasAttribute( oratext* name);

/*---------------------------------------------------------------------
    NAME
        hasAttributeNS

    PURPOSE
        Check if named attribute exist (namespace aware version)

    DESCRIPTION
        Determines if an element has a attribute with the given URI
	and local name

    RETURNS
        (boolean) TRUE if element has such attribute

    ARGUMENTS
        namespaceURI (IN) attribute's namespace URI (data encoding)
        localName    (IN) attribute's local name (data encoding)

*/
      boolean hasAttributeNS( oratext* namespaceURI, oratext* localName);

    private:

      // This is the default constructor.
      ElementRef();
    };

/**********************************************************************
          Interface AttrRef - reference to Attribute nodes
**********************************************************************/

    template< typename Node> class AttrRef
      : public NodeRef< Node> {
    public:

      //      ~AttrRef();

/*---------------------------------------------------------------------
    NAME
        AttrRef

    PURPOSE
        Constructor

    DESCRIPTION
        This constructor is used to create references
	to a given attribute node after a call to createAttribute.

    RETURNS
        (AttrRef) Node reference object

    ARGUMENTS
        node_ref (IN) reference to provide the context
	nptr     (IN) attribute node to be referenced
*/
      AttrRef( const NodeRef< Node>& node_ref, Node* nptr);

/*---------------------------------------------------------------------
    NAME
        AttrRef

    PURPOSE
        Copy constructor

    DESCRIPTION
        This is a copy constructor.

    RETURNS
        (AttrRef) Node reference object

    ARGUMENTS
        nref (IN) reference to the reference object to be copied
*/
      AttrRef( const AttrRef< Node>& nref);

/*---------------------------------------------------------------------
    NAME
        getName

    PURPOSE
        Return attribute's name

    DESCRIPTION
        Returns the fully-qualified name of an attribute (in the data
        encoding) as a NULL-terminated string.

    RETURNS
        (oratext *) name of attribute

    ARGUMENTS
        None
*/
      oratext* getName() const;

/*---------------------------------------------------------------------
    NAME
        getSpecified

    PURPOSE
        Return boolean indicating if an attribute was explicitly created

    DESCRIPTION
        Returns the 'specified' value for an attribute. If the attribute
	was explicitly given a value in the original document, it is
	TRUE; otherwise, it is FALSE.  If the node is not an attribute,
	returns FALSE.  If the user sets attribute's value through DOM,
	its 'specified' value will be TRUE.

    RETURNS
        (boolean) attribute's "specified" value

    ARGUMENTS
        None
*/
      boolean getSpecified() const;

/*---------------------------------------------------------------------
    NAME
        getValue

    PURPOSE
        Return attribute's value

    DESCRIPTION
        Returns the "value" (character data) of an attribute (in the
	data encoding) as NULL-terminated string.  Character and general
	entities will have been replaced.

    RETURNS
        (oratext*) attribute's value

    ARGUMENTS
        None
*/
      oratext* getValue() const;

/*---------------------------------------------------------------------
    NAME
        setValue

    PURPOSE
        Set attribute's value

    DESCRIPTION
        Sets the given attribute's value to data. The new value must be
	in the data encoding. It is not verified, converted, or checked.
	The attribute's 'specified' flag will be TRUE after setting a new
	value.

    RETURNS
        (void)

    ARGUMENTS
        data (IN) new value of attribute
*/
      void setValue( oratext* data) throw (DOMException);

/*---------------------------------------------------------------------
    NAME
        getOwnerElement

    PURPOSE
        Return attribute's owning element

    DESCRIPTION
        Returns attribute's owning element

    RETURNS
        (Node*) attribute's owning element node.

    ARGUMENTS
        None
*/
      Node* getOwnerElement();

    private:

      // This is the default constructor.
      AttrRef();
    };

/**********************************************************************
                 Interface CharacterDataRef

    This interface does not have a corresponding node kind. It just
    introduces some common functionality for Text, Comment, and PI
    nodes.
**********************************************************************/

    template< typename Node> class CharacterDataRef
      : public NodeRef< Node> {
    public:

/*---------------------------------------------------------------------
    NAME
        getData

    PURPOSE
        Return node's data

    DESCRIPTION
        Returns the data for a CharacterData node (type text, comment or
        CDATA) in the data encoding.

    RETURNS
        (oratext*) node's data

    ARGUMENTS
        None
*/
      oratext* getData() const;

/*---------------------------------------------------------------------
    NAME
        setData

    PURPOSE
        Set node's data

    DESCRIPTION
        Sets data for a CharacterData node (type text, comment or CDATA),
        replacing the old data. The new data is not verified, converted,
	or checked -- it should be in the data encoding.

    RETURNS
        (void)

    ARGUMENTS
        data (IN) new node's data
*/
      void setData( oratext* data) throw (DOMException);

/*---------------------------------------------------------------------
    NAME
        getLength

    PURPOSE
        Return length of node's data

    DESCRIPTION
        Returns the length of the data for a CharacterData node (type
	text, comment or CDATA) in characters (not bytes!).

    RETURNS
        (ub4) length in characters (not bytes!) of node's data

    ARGUMENTS
        None
*/
      ub4 getLength() const;

      // member functions - DOM1

/*---------------------------------------------------------------------
    NAME
        substringData

    PURPOSE
        Get substring of node's data

    DESCRIPTION
        Returns a range of character data from a CharacterData node
	(type text, comment or CDATA). Since the data is in the data
	encoding, offset and count are in characters, not bytes.  The
	beginning of the string is offset 0.  If the sum of offset and
	count exceeds the length, then all characters to the end of the
	data are returned. The substring is permanently allocated in the
	context managed memory and should be explicitly deallocated by
	freeString().

    RETURNS
        (oratext *) specified substring

    ARGUMENTS
        offset (IN) start offset of substring to extract
        count  (IN) number of characters to extract
*/
      oratext* substringData( ub4 offset, ub4 count) throw (DOMException);

/*---------------------------------------------------------------------
    NAME
        appendData

    PURPOSE
        Append data to end of node's current data

    DESCRIPTION
        Append a string to the end of a CharacterData node's data. The
	appended data should be in the data encoding.  It will not be
	verified, converted, or checked.

    RETURNS
        (void)

    ARGUMENTS
        data (IN) data to append
*/
      void appendData( oratext* data) throw (DOMException);

/*---------------------------------------------------------------------
    NAME
        insertData

    PURPOSE
        Insert string into node's current data

    DESCRIPTION
        Insert a string into a CharacterData node's data at the specified
        position. The inserted data must be in the data encoding.
	It will not be verified, converted, or checked. The offset
        is specified as characters, not bytes.  The offset is zero-based,
        so inserting at offset zero prepends the data.

    RETURNS
        (void)

    ARGUMENTS
        offset (IN)  character offset at which to insert
        data   (IN)  data to insert
*/
      void insertData( ub4 offset, oratext* data) throw (DOMException);

/*---------------------------------------------------------------------
    NAME
        deleteData

    PURPOSE
        Remove part of node's data

    DESCRIPTION
        Remove a range of characters from a CharacterData node's data.
        The offset is zero-based, so offset zero refers to the start of
	the data. Both offset and count are in characters, not bytes.
	If the sum of offset and count exceeds the data length then all
	characters from offset to the end of the data are deleted.

    RETURNS
        (void)

    ARGUMENTS
        offset (IN)  character offset from which to start removing
        count  (IN)  number of characters to delete
*/
      void deleteData( ub4 offset, ub4 count) throw (DOMException);

/*---------------------------------------------------------------------
    NAME
        replaceData

    PURPOSE
        Replace part of node's data

    DESCRIPTION
        Replaces a range of characters in a CharacterData node's data
	with a new string. The offset is zero-based, so offset zero
	refers to the start of the data.  The replacement data must be
	in the data encoding.  It will not be verified, converted, or
	checked. The offset and count are both in characters, not bytes.
	If the sum of offset and count exceeds length, then all
	characters to the end of the data are replaced.

    RETURNS
        

    ARGUMENTS
        offset (IN)  character offset from which to start replacing
        count  (IN)  number of characters to replace
        data   (IN)  replacement substring [data encoding]
*/
      void replaceData( ub4 offset, ub4 count, oratext* data)
                                                  throw (DOMException);

/*---------------------------------------------------------------------
    NAME
        freeString

    PURPOSE
        Deallocate the string allocated by substringData()

    DESCRIPTION
        Deallocates the string allocated by substringData(). It is
	Oracle's extension.

    RETURNS
        (void)

    ARGUMENTS
        str (IN) string to free
*/
      void freeString( oratext* str);

    protected:

      CharacterDataRef();
      //      ~CharacterDataRef();

    };

/**********************************************************************
          Interface TextRef - reference to Text nodes
**********************************************************************/

    template< typename Node> class TextRef
      : public CharacterDataRef< Node> {
    public:

      //      ~TextRef();

/*---------------------------------------------------------------------
    NAME
        TextRef

    PURPOSE
        Constructor

    DESCRIPTION
        This constructor is used to create references
	to a given text node after a call to createtext.

    RETURNS
        (TextRef) Node reference object

    ARGUMENTS
        node_ref (IN) reference to provide the context
	nptr    ( IN) text node to be referenced
*/
      TextRef( const NodeRef< Node>& node_ref, Node* nptr);

/*---------------------------------------------------------------------
    NAME
        TextRef

    PURPOSE
        Copy constructor

    DESCRIPTION
        This is a copy constructor.

    RETURNS
        (TextRef) Node reference object

    ARGUMENTS
        nref (IN) reference to the reference object to be copied
*/
      TextRef( const TextRef< Node>& nref);

/*---------------------------------------------------------------------
    NAME
        splitText

    PURPOSE
        Split text node into two

    DESCRIPTION
        Splits a single text node into two text nodes; the original data
        is split between them. The offset is zero-based, and is in
	characters, not bytes. The original node is retained, its data
	is just truncated.  A new text node is created which contains
	the remainder of the original data, and is inserted as the next
	sibling of the original.  The new text node is returned.

    RETURNS
        (Node*) new node

    ARGUMENTS
        offset (IN) character offset at which to split
*/
      Node* splitText( ub4 offset) throw (DOMException);

    protected:

      // This is the default constructor.
      TextRef();
    };

/**********************************************************************
      Interface CDATASectionRef - reference to CDATASection nodes
**********************************************************************/

    template< typename Node> class CDATASectionRef
      : public TextRef< Node> {
    public:

      //      ~CDATASectionRef();

/*---------------------------------------------------------------------
    NAME
        CDATASectionRef

    PURPOSE
        Constructor

    DESCRIPTION
        This constructor is used to create references
	to a given CDATA node after a call to createCDATASection.

    RETURNS
        (CDATASectionRef) Node reference object

    ARGUMENTS
        node_ref (IN) reference to provide the context
	nptr     (IN) CDATASection node to be referenced
*/
      CDATASectionRef( const NodeRef< Node>& node_ref, Node* nptr);

/*---------------------------------------------------------------------
    NAME
        CDATASectionRef

    PURPOSE
        Copy constructor

    DESCRIPTION
        This is a copy constructor.

    RETURNS
        (CDATASectionRef) Node reference object

    ARGUMENTS
        nref (IN) reference to the reference object to be copied
*/
      CDATASectionRef( const CDATASectionRef< Node>& nref);

    private:

      // This is the default constructor.
      CDATASectionRef();
    };

/**********************************************************************
          Interface CommentRef - reference to Comment nodes
**********************************************************************/

    template< typename Node> class CommentRef
      : public CharacterDataRef< Node> {
    public:

      //      ~CommentRef();

/*---------------------------------------------------------------------
    NAME
        CommentRef

    PURPOSE
        Constructor

    DESCRIPTION
        This constructor is used to create references
	to a given comment node after a call to createComment.

    RETURNS
        (CommentRef) Node reference object

    ARGUMENTS
        node_ref (IN) reference to provide the context
	nptr     (IN) comment node to be referenced
*/
      CommentRef( const NodeRef< Node>& node_ref, Node* nptr);

/*---------------------------------------------------------------------
    NAME
        CommentRef

    PURPOSE
        Copy constructor

    DESCRIPTION
        This is a copy constructor.

    RETURNS
        (CommentRef) Node reference object

    ARGUMENTS
        nref (IN) reference to the reference object to be copied
*/
      CommentRef( const CommentRef< Node>& nref);

    private:

      // This is the default constructor.
      CommentRef();
    };

/**********************************************************************
      Interface ProcessingInstructionRef - reference to PI nodes
**********************************************************************/

    template< typename Node> class ProcessingInstructionRef
      : public NodeRef< Node> {
    public:

      //      ~ProcessingInstructionRef();

/*---------------------------------------------------------------------
    NAME
        ProcessingInstructionRef

    PURPOSE
        Constructor

    DESCRIPTION
        This constructor is used to create references to a given PI node
	after a call to createProcessingInstruction.

    RETURNS
        (ProcessingInstructionRef) Node reference object

    ARGUMENTS
        node_ref (IN) reference to provide the context
	nptr     (IN) PI node to be referenced
*/
      ProcessingInstructionRef( const NodeRef< Node>& node_ref,
				Node* nptr);

/*---------------------------------------------------------------------
    NAME
        ProcessingInstructionRef

    PURPOSE
        Copy constructor

    DESCRIPTION
        This is a copy constructor.

    RETURNS
        (ProcessingInstructionRef) Node reference object

    ARGUMENTS
        nref (IN) reference to the reference object to be copied
*/
      ProcessingInstructionRef(
			 const ProcessingInstructionRef< Node>& nref);

/*---------------------------------------------------------------------
    NAME
        getTarget

    PURPOSE
        Get processing instruction's target

    DESCRIPTION
        Returns a processing instruction's target string. The target is
	the first token following the markup that begins the PI.  All PIs
	must have a target, though the data part is optional.

    RETURNS
        (oratext*) processing instruction's target

    ARGUMENTS
        None
*/
      oratext* getTarget() const;

/*---------------------------------------------------------------------
    NAME
        getData

    PURPOSE
        Get processing instruction's data

    DESCRIPTION
        Returns the content (data) of a processing instruction (in the
	data encoding). The content is the part from the first
	non-whitespace character after the target until the ending "?>".

    RETURNS
        (oratext*) processing instruction's data

    ARGUMENTS
        None
*/
      oratext* getData() const;

/*---------------------------------------------------------------------
    NAME
        setData

    PURPOSE
        Set processing instruction's data

    DESCRIPTION
        Sets processing instruction's data (content), which must be
        in the data encoding.  It is not permitted to set the data to
	NULL. The new data is not verified, converted, or checked.

    RETURNS
        (void)

    ARGUMENTS
        data (IN) processing instruction's new data
*/
      void setData( oratext* data) throw (DOMException);

    private:

      // This is the default constructor.
      ProcessingInstructionRef();
    };

/**********************************************************************
          Interface EntityRef - reference to Entity nodes
**********************************************************************/

    template< typename Node> class EntityRef
      : public NodeRef< Node> {
    public:

      //      ~EntityRef();

/*---------------------------------------------------------------------
    NAME
        EntityRef

    PURPOSE
        Constructor

    DESCRIPTION
        This constructor is used to create references
	to a given entity node after a call to createEntity.

    RETURNS
        (EntityRef) Node reference object

    ARGUMENTS
        node_ref (IN) reference to provide the context
	nptr     (IN) entity node to be referenced
*/
      EntityRef( const NodeRef< Node>& node_ref, Node* nptr);

/*---------------------------------------------------------------------
    NAME
        EntityRef

    PURPOSE
        Copy constructor

    DESCRIPTION
        This is a copy constructor.

    RETURNS
        (EntityRef) Node reference object

    ARGUMENTS
        nref (IN) reference to the reference object to be copied
*/
      EntityRef( const EntityRef< Node>& nref);

/*---------------------------------------------------------------------
    NAME
        getPublicId

    PURPOSE
        Get entity's public ID

    DESCRIPTION
        Returns an entity's public identifier (in the data encoding).

    RETURNS
        (oratext*) entity's public identifier

    ARGUMENTS
        None
*/
      oratext* getPublicId() const;

/*---------------------------------------------------------------------
    NAME
        getSystemId

    PURPOSE
        Get entity's system ID

    DESCRIPTION
        Returns an entity's system identifier (in the data encoding).

    RETURNS
        (oratext*) entity's system identifier

    ARGUMENTS
        None
*/
      oratext* getSystemId() const;

/*---------------------------------------------------------------------
    NAME
        getNotationName

    PURPOSE
        Get entity's notation

    DESCRIPTION
        For unparsed entities, returns the name of its notation (in the
	data encoding).  For parsed entities and other node types,
	returns NULL.

    RETURNS
        (oratext*) entity's notation

    ARGUMENTS
        None
*/
      oratext* getNotationName() const;

/*---------------------------------------------------------------------
    NAME
        getType

    PURPOSE
        Get entity's type

    DESCRIPTION
        Returns a boolean for an entity describing whether it is general
        (TRUE) or parameter (FALSE).

    RETURNS
        (boolean) TRUE for general entity, FALSE for parameter entity

    ARGUMENTS
        None
*/
      boolean getType() const;

    private:

      // This is the default constructor.
      EntityRef();
    };

/**********************************************************************
   Interface EntityReferenceRef - reference to EntityReference nodes
**********************************************************************/

    template< typename Node> class EntityReferenceRef
      : public NodeRef< Node> {
    public:

      //      ~EntityReferenceRef();

/*---------------------------------------------------------------------
    NAME
        EntityReferenceRef

    PURPOSE
        Constructor

    DESCRIPTION
        This constructor is used to create references to a given
	entity reference node after a call to createEntityReference.

    RETURNS
        (EntityReferenceRef) Node reference object

    ARGUMENTS
        node_ref (IN) reference to provide the context
	nptr     (IN) entity reference node to be referenced
*/
      EntityReferenceRef( const NodeRef< Node>& node_ref, Node* nptr);

/*---------------------------------------------------------------------
    NAME
        EntityReferenceRef

    PURPOSE
        Copy constructor

    DESCRIPTION
        This is a copy constructor.

    RETURNS
        (EntityReferenceRef) Node reference object

    ARGUMENTS
        nref (IN) reference to the reference object to be copied
*/
      EntityReferenceRef( const EntityReferenceRef< Node>& nref);

    private:

      // This is the default constructor.
      EntityReferenceRef();
    };

/**********************************************************************
          Interface NotationRef - reference to Notation nodes
**********************************************************************/

    template< typename Node> class NotationRef
      : public NodeRef< Node> {
    public:

      //      ~NotationRef();

/*---------------------------------------------------------------------
    NAME
        NotationRef

    PURPOSE
        Constructor

    DESCRIPTION
        This constructor is used to create references
	to a given notation node after a call to createNotation.

    RETURNS
        (NotationRef) Node reference object

    ARGUMENTS
        node_ref (IN) reference to provide the context
	nptr     (IN) notation node to be referenced
*/
      NotationRef( const NodeRef< Node>& node_ref, Node* nptr);

/*---------------------------------------------------------------------
    NAME
        NotationRef

    PURPOSE
        Copy constructor

    DESCRIPTION
        This is a copy constructor.

    RETURNS
        (NotationRef) Node reference object

    ARGUMENTS
        nref (IN) reference to the reference object to be copied
*/
      NotationRef( const NotationRef< Node>& nref);

/*---------------------------------------------------------------------
    NAME
        getPublicId

    PURPOSE
        Get publicId

    DESCRIPTION
        

    RETURNS
        (oratext*) publicId

    ARGUMENTS
        None
*/
      oratext* getPublicId() const;

/*---------------------------------------------------------------------
    NAME
        getSystemId

    PURPOSE
        Get systemId

    DESCRIPTION
        

    RETURNS
        (oratext*) systemId

    ARGUMENTS
        None
*/
      oratext* getSystemId() const;

    private:

      // This is the default constructor.
      NotationRef();
    };

/**********************************************************************
        Interface DocumentTypeRef - reference to DTD nodes
**********************************************************************/

    template< typename Node> class DocumentTypeRef
      : public NodeRef< Node> {
    public:

      //      ~DocumentTypeRef();

/*---------------------------------------------------------------------
    NAME
        DocumentTypeRef

    PURPOSE
        Constructor

    DESCRIPTION
        This is a constructor.

    RETURNS
        (DocumentTypeRef) Node reference object

    ARGUMENTS
        nref (IN) reference to the object to provide the context
	nptr (IN) document type node to be referenced
*/
      DocumentTypeRef( const NodeRef< Node>& nref, Node* nptr);

/*---------------------------------------------------------------------
    NAME
        DocumentTypeRef

    PURPOSE
        Copy constructor

    DESCRIPTION
        This is a copy constructor.

    RETURNS
        (DocumentTypeRef) Node reference object

    ARGUMENTS
        nref (IN) reference to the reference object to be copied
*/
      DocumentTypeRef( const DocumentTypeRef< Node>& nref);

/*---------------------------------------------------------------------
    NAME
        getName

    PURPOSE
        Get name of DTD

    DESCRIPTION
        Returns DTD's name (specified immediately after the {DOCTYPE}
        keyword)

    RETURNS
        (oratext*) name of DTD

    ARGUMENTS
        None
*/
      oratext* getName() const;

/*---------------------------------------------------------------------
    NAME
        getEntities

    PURPOSE
        Get DTD's entities

    DESCRIPTION
        Returns a named node map of general entities defined by the DTD.

    RETURNS
        (NamedNodeMap< Node>*) map containing entities

    ARGUMENTS
        None
*/
      NamedNodeMap< Node>* getEntities() const;

/*---------------------------------------------------------------------
    NAME
        getNotations

    PURPOSE
        Get DTD's notations

    DESCRIPTION
        Returns a named node map of notations declared by the DTD.

    RETURNS
        (NamedNodeMap< Node>*) map containing notations

    ARGUMENTS
        None
*/
      NamedNodeMap< Node>* getNotations() const;

/*---------------------------------------------------------------------
    NAME
        getPublicId

    PURPOSE
        Get DTD's public ID

    DESCRIPTION
        Returns DTD's public identifier

    RETURNS
        (oratext*) DTD's public identifier

    ARGUMENTS
        None
*/
      oratext* getPublicId() const;

/*---------------------------------------------------------------------
    NAME
        getSystemId

    PURPOSE
        Get DTD's system ID

    DESCRIPTION
        Returns DTD's system identifier

    RETURNS
        (oratext*) DTD's system identifier

    ARGUMENTS
        None
*/
      oratext* getSystemId() const;

/*---------------------------------------------------------------------
    NAME
        getInternalSubset

    PURPOSE
        Get DTD's internal subset

    DESCRIPTION
        Returns the content model for an element.  If there is no DTD,
        returns NULL.

    RETURNS
        (xmlnode*) content model subtree

    ARGUMENTS
        name (IN) name of the element
*/
      Node* getInternalSubset( oratext* name);

    private:

      // This is the default constructor.
      DocumentTypeRef();
    };

    template< typename Node> class DOMImplData {
    protected:
      DOMImplData();
      ~DOMImplData();
    };

/***********************************************************************
          Interface DOMImplementation - handles DOM trees

    DOMImplementation - is fundamental for manipulating DOM trees.
    Every DOM tree is attached to a particular DOM implementation object.
    Several DOM trees can be attached to the same DOM implementation
    object. Each DOM tree can be deleted and deallocated by deleting
    the document object. All DOM trees attached to a particular DOM
    implementation object are deleted when this object is deleted.
    DOMImplementation object is visible to the user directly. The
    user explicitly creates and deletes DOMIMplementation objects.
    However, the functionality of DOMImplementation is available
    through DOMImplRef objects only.

************************************************************************/

    template< typename Node> class DOMImplementation :
      public DOMImplData< Node> {
    public:

/*---------------------------------------------------------------------
    NAME
        DOMImplementation

    PURPOSE
        Constructor

    DESCRIPTION
        Creates DOMImplementation object. Sets the
	'no modifications allowed' flag to the parameter value.

    RETURNS
        (DOMImplementation) implementation object

    ARGUMENTS
        no_mod  (IN) value of the 'no modifications allowed' flag
*/
      DOMImplementation( boolean no_mod);

/*---------------------------------------------------------------------
    NAME
        ~DOMImplementation

    PURPOSE
        Public default destructor

    DESCRIPTION
        This is the default destructor. It removes all DOM trees
	associated with this object.

    RETURNS
        (void)

    ARGUMENTS
        None
*/
      ~DOMImplementation(); 

/*---------------------------------------------------------------------
    NAME
        getNoMod

    PURPOSE
        Get the 'no modification allowed' flag value

    DESCRIPTION
        It is an Oracle extension

    RETURNS
        TRUE if flag's value is TRUE, FALSE otherwise

    ARGUMENTS
        None
*/
      boolean getNoMod() const;
    };

/***********************************************************************
          Interface DOMImplRef - reference to DOMImplementation

    DOMImplRef is explicitly bound to a particular context. It
    provides access to all standard DOMImplementation functionality
************************************************************************/

    template< typename Context, typename Node> class DOMImplRef {
    public:

/*---------------------------------------------------------------------
    NAME
        DOMImplRef

    PURPOSE
        Constructor

    DESCRIPTION
        Creates reference object to DOMImplementation object in a given
	context. Returns reference to the implementation object.

    RETURNS
        (DOMImplRef) reference to the implementation object

    ARGUMENTS
        ctx_ptr (IN) context pointer
        
*/
      DOMImplRef( Context* ctx_ptr, DOMImplementation< Node>* impl_ptr);

/*---------------------------------------------------------------------
    NAME
        ~DOMImplRef

    PURPOSE
        Public default destructor

    DESCRIPTION
        This is the default destructor. It cleans the reference to
	the implementation object. It is usually
	called by the environment. But it can be called by the user
	directly if necessary.

    RETURNS
        (void)

    ARGUMENTS
        None
*/
      ~DOMImplRef(); 

/*---------------------------------------------------------------------
    NAME
        DOMImplRef

    PURPOSE
        Copy constructor

    DESCRIPTION
        It is needed to create other references to the implementation
	object; deletion flags are not copied

    RETURNS
        (DOMImplRef) new reference to the implementation object

    ARGUMENTS
        iref (IN) reference to the reference object to be copied
*/
      DOMImplRef( const DOMImplRef< Context, Node>& iref);

/*---------------------------------------------------------------------
    NAME
        DOMImplRef

    PURPOSE
        Copy constructor

    DESCRIPTION
        It is needed to create references to the implementation
	object in a different context; deletion flags are not copied

    RETURNS
        (DOMImplRef) new reference to the implementation object

    ARGUMENTS
        iref    (IN) reference to the reference object to be copied
	ctx_ptr (IN) different context
*/
      DOMImplRef( const DOMImplRef< Context, Node>& iref,
		  Context* ctx_ptr);

/*---------------------------------------------------------------------
    NAME
        hasFeature

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

	DOM 2.0 features are "Core", "XML", "HTML", "Views",
	"StyleSheets", "CSS", "CSS2", "Events", "UIEvents", "MouseEvents",
	"MutationEvents", "HTMLEvents", "Range", "Traversal"

    RETURNS
        (boolean) is feature implemented?

    ARGUMENTS
        feature (IN) package name of the feature to test.
        version (IN) version # of the package name to test.
*/
      boolean hasFeature( oratext* feature, oratext* version);

/*---------------------------------------------------------------------
    NAME
        createDocumentType

    PURPOSE
        Create DTD reference

    DESCRIPTION
        Creates DTD reference

    RETURNS
        (DocumentTypeRef< Node>*) DTD reference

    ARGUMENTS
        qualifiedName (IN)  qualified name
        publicId      (IN)  external subset public identifier
        systemId      (IN)  external subset system identifier
*/
      DocumentTypeRef< Node>* createDocumentType(
                           oratext* qualifiedName, oratext* publicId,
			   oratext* systemId)
	throw (DOMException);

/*---------------------------------------------------------------------
    NAME
        formDocumentType

    PURPOSE
        Form document type reference given a document pointer

    DESCRIPTION
        Forms document type reference given a document pointer

    RETURNS
        (DocumentTypeRef*) pointer to the document reference

    ARGUMENTS
        node (IN) pointer to the document type node
*/
      DocumentTypeRef< Node>* formDocumentType( Node* node)
	throw (DOMException);

/*---------------------------------------------------------------------
    NAME
        createDocument

    PURPOSE
        Create document reference

    DESCRIPTION
        Creates document reference

    RETURNS
        (DocumentRef< Node>*) document reference

    ARGUMENTS
        namespaceURI  (IN)  namespace URI of root element to create
        qualifiedName (IN)  QName of root element
        doctype       (IN)  associated DTD node
*/
      DocumentRef< Node>* createDocument( oratext* namespaceURI,
    	    oratext* qualifiedName, DocumentTypeRef< Node>& doctype)
	throw (DOMException);

/*---------------------------------------------------------------------
    NAME
        formDocument

    PURPOSE
        Form document reference given a document pointer

    DESCRIPTION
        Forms document reference given a document pointer

    RETURNS
        (DocumentRef*) pointer to the document reference

    ARGUMENTS
        node (IN) pointer to the document node
*/
      DocumentRef< Node>* formDocument( Node* node)
	throw (DOMException);

/*---------------------------------------------------------------------
    NAME
        getImplementation

    PURPOSE
        Get DOMImplementation object associated with the document

    DESCRIPTION
        Returns DOMImplementation object that was used to create
	this document. When the DOMImplementation object is
	destructed, all document trees associated with it are
	also destructed.

    RETURNS
        (DOMImplementation) DOMImplementation reference object

    ARGUMENTS
        None
*/
      DOMImplementation< Node>* getImplementation() const;

/*---------------------------------------------------------------------
    NAME
        getNoMod

    PURPOSE
        Get the 'no modification allowed' flag value

    DESCRIPTION
        It is an Oracle extension

    RETURNS
        TRUE if flag's value is TRUE, FALSE otherwise

    ARGUMENTS
        None
*/
      boolean getNoMod() const;

/*---------------------------------------------------------------------
    NAME
        setContext

    PURPOSE
        Set another context to a node

    DESCRIPTION
        It is needed to create node references in a different context

    RETURNS
        (void)

    ARGUMENTS
        nref    (IN) node reference
	ctx_ptr (IN) different context
*/
      void setContext( NodeRef< Node>& nref, Context* ctx_ptr);

    };

/**********************************************************************
          Interface NodeList - represents node lists
**********************************************************************/

    template< typename Node> class NodeList {

    private:
      // default constructor and destructor - should never be called
      NodeList() {}
      ~NodeList() {}
    };

    // Interface NodeListData - not visible to the user

    template< typename Node> class NodeListData {
    protected:
      NodeListData() {}
      ~NodeListData() {}
    };

/**********************************************************************
          Interface NodeListRef - reference to a node list

    The node list referenced can be marked for deletion by the
    implementation and, then, is deleted when the destructor of this 
    class is called
**********************************************************************/

    template< typename Node> class NodeListRef :
    public NodeListData< Node>{

    public:

      //      ~NodeListRef();


/*---------------------------------------------------------------------
    NAME
        NodeListRef

    PURPOSE
        Constructor

    DESCRIPTION
        This constructor is used to create references to a given
	NodeList node.

    RETURNS
        (NodeListRef) Node reference object

    ARGUMENTS
        node_ref (IN) node to provide the context
	lptr     (IN) node list node to be referenced
*/
      NodeListRef( const NodeRef< Node>& node_ref,
		   NodeList< Node>* lptr);

/*---------------------------------------------------------------------
    NAME
        NodeListRef

    PURPOSE
        Copy constructor

    DESCRIPTION
        This is a copy constructor.

    RETURNS
        (NodeListRef) Node reference object

    ARGUMENTS
        lref (IN) reference to the reference object to be copied
*/
      NodeListRef( const NodeListRef< Node>& lref);

/*---------------------------------------------------------------------
    NAME
        getLength

    PURPOSE
        Get list's length

    DESCRIPTION
        

    RETURNS
        (ub4) list's length

    ARGUMENTS
        None
*/
      ub4 getLength() const;

/*---------------------------------------------------------------------
    NAME
        item

    PURPOSE
        Get item given its index

    DESCRIPTION
        

    RETURNS
        (Node*) pointer to the item

    ARGUMENTS
        index (IN) index of the item
*/
      Node* item( ub4 index) const;

/*---------------------------------------------------------------------
    NAME
        getCCtx

    PURPOSE
        Get associated C context 

    DESCRIPTION
        

    RETURNS
        (xmlctx*) pointer to the C context

    ARGUMENTS

*/
      xmlctx* getCCtx() const;

    protected:
      // default constructor - should not be called publically
      NodeListRef();
    };

/**********************************************************************
          Interface NamedNodeMap - represents named node maps
**********************************************************************/

    template< typename Node> class NamedNodeMap {

    private:
      // default constructor and destructor - should never be called
      NamedNodeMap() {}
      ~NamedNodeMap() {}
    };

    // Interface NamedNodeMapData - not visible to the user

    template< typename Node> class NamedNodeMapData {

    protected:
      // default constructor and destructor - should not be called
      // publically
      NamedNodeMapData() {}
      ~NamedNodeMapData() {}
    };

/**********************************************************************
         Interface NamedNodeMapRef - reference to a node map 

    The node map referenced can be marked for deletion by the
    implementation and, then, is deleted when the destructor
    of this class is called
**********************************************************************/

    template< typename Node> class NamedNodeMapRef :
    public NamedNodeMapData< Node>{

    public:

      //      ~NamedNodeMapRef();

/*---------------------------------------------------------------------
    NAME
        NamedNodeMapRef

    PURPOSE
        Constructor

    DESCRIPTION
        This constructor is used to create references to a given
	NamedNodeMap node.

    RETURNS
        (NamedNodeMapRef) Node reference object

    ARGUMENTS
        node_ref (IN) node to provide the context
	mptr     (IN) node map to be referenced
*/
      NamedNodeMapRef( const NodeRef< Node>& node_ref,
		   NamedNodeMap< Node>* mptr);

/*---------------------------------------------------------------------
    NAME
        NamedNodeMapRef

    PURPOSE
        Copy constructor

    DESCRIPTION
        This is a copy constructor.

    RETURNS
        (NamedNodeMapRef) Node reference object

    ARGUMENTS
        mref (IN) reference to the reference object to be copied
*/
      NamedNodeMapRef( const NamedNodeMapRef< Node>& mref);

/*---------------------------------------------------------------------
    NAME
        getLength

    PURPOSE
        Get map's length

    DESCRIPTION
        

    RETURNS
        (ub4) map's length

    ARGUMENTS
        None
*/
      ub4 getLength() const;

/*---------------------------------------------------------------------
    NAME
        item

    PURPOSE
        Get item given its index

    DESCRIPTION
        

    RETURNS
        (Node*) pointer to the item

    ARGUMENTS
        index (IN) item's index
*/
      Node* item( ub4 index) const;

/*---------------------------------------------------------------------
    NAME
        getNamedItem

    PURPOSE
        Get item given its name

    DESCRIPTION
        

    RETURNS
        (Node*) pointer to the item

    ARGUMENTS
        name (IN) item's name
*/
      Node* getNamedItem( oratext* name) const;

/*---------------------------------------------------------------------
    NAME
        setNamedItem

    PURPOSE
        Add new item to the map

    DESCRIPTION
        

    RETURNS
        (Node*) pointer to new item

    ARGUMENTS
        newItem (IN) item to add to the map
*/
      Node* setNamedItem( NodeRef< Node>& newItem)
                                                 throw (DOMException);

/*---------------------------------------------------------------------
    NAME
        removeNamedItem

    PURPOSE
        Remove an item given its name

    DESCRIPTION
        

    RETURNS
        (Node*) pointer to the removed item

    ARGUMENTS
        name (IN) name of the item to be removed
*/
      Node* removeNamedItem( oratext* name) throw (DOMException);

/*---------------------------------------------------------------------
    NAME
        getNamedItemNS

    PURPOSE
        Get item given its namespace URI and local name

    DESCRIPTION
        

    RETURNS
        (Node*) pointer to the item

    ARGUMENTS
        namespaceURI (IN) namespace URI of the item
	localName    (IN) local name of th item
*/
      Node* getNamedItemNS( oratext* namespaceURI,
			    oratext* localName) const;

/*---------------------------------------------------------------------
    NAME
        setNamedItemNS

    PURPOSE
        Set named item to the map

    DESCRIPTION
        

    RETURNS
        (Node*) pointer to the item

    ARGUMENTS
        newItem (IN) item to be set to the map
*/
      Node* setNamedItemNS( NodeRef< Node>& newItem)
	throw (DOMException);

/*---------------------------------------------------------------------
    NAME
        removeNamedItemNS

    PURPOSE
        Remove the item from the map

    DESCRIPTION
        

    RETURNS
        (Node*) pointer to the removed item

    ARGUMENTS
        namespaceURI (IN) namespace URI of the item
	localName    (IN) local name of the item
*/
      Node* removeNamedItemNS( oratext* namespaceURI,
			       oratext* localName)
	throw (DOMException);

    protected:
      // default constructor - should not be called publically
      NamedNodeMapRef();
    };

/**********************************************************************
          Interface NodeFilter - DOM 2 Node Filter

    NodeFilter implementations are provided by users.

**********************************************************************/

/*---------------------------------------------------------------------
    NAME
        acceptNode

    PURPOSE
        Execute it for a given node and use its return value

    DESCRIPTION
        This function is used as a test by NodeIterator and
	TreeWalker

    RETURNS
        (AcceptNodeCode) result returned by the filter function

    ARGUMENTS
        nref (IN) reference to the node to be evaluated
*/
    template< typename Node> AcceptNodeCode AcceptNode(
					       NodeRef< Node>& nref);

/**********************************************************************
          Interface NodeIterator - DOM 2 Node Iterator

**********************************************************************/

    template< typename Node> class NodeIterator {
    public:

/*---------------------------------------------------------------------
    NAME
        nextNode 

    PURPOSE
        Go to the next node

    DESCRIPTION
        

    RETURNS
        (Node*) pointer to the next node

    ARGUMENTS
        None
*/
      Node* nextNode() throw (DOMException);

/*---------------------------------------------------------------------
    NAME
        previousNode

    PURPOSE
        Go to the previous node

    DESCRIPTION
        

    RETURNS
        (Node*) pointer to the previous node

    ARGUMENTS
        None
*/
      Node* previousNode() throw (DOMException);

/*---------------------------------------------------------------------
    NAME
        detach

    PURPOSE
        invalidate the iterator

    DESCRIPTION
        

    RETURNS
        (void)

    ARGUMENTS
        None
*/
      void detach();

/*---------------------------------------------------------------------
    NAME
        adjustCtx

    PURPOSE
        Attach this iterator to the another context

    DESCRIPTION
        Attaches this iterator to the context associated with
	a given node reference

    RETURNS
        (void)

    ARGUMENTS
        nref (IN) given node reference
*/
      void adjustCtx( NodeRef< Node>& nref);

    private:
      // Default constructor
      NodeIterator() {}
      ~NodeIterator();
    };

/**********************************************************************
          Interface TreeWalker - DOM 2 Tree Walker

**********************************************************************/

    template< typename Node> class TreeWalker {
    public:

/*---------------------------------------------------------------------
    NAME
        parentNode

    PURPOSE
        Get the parent of the current node

    DESCRIPTION
        

    RETURNS
        (Node*) pointer to the parent node

    ARGUMENTS
        None
*/
      Node* parentNode();

/*---------------------------------------------------------------------
    NAME
        firstChild

    PURPOSE
        Get the first child of the current node

    DESCRIPTION
        

    RETURNS
        (Node*) pointer to first child node

    ARGUMENTS
        None
*/
      Node* firstChild();

/*---------------------------------------------------------------------
    NAME
        lastChild

    PURPOSE
        Get the last child of the current node

    DESCRIPTION
        

    RETURNS
        (Node*) pointer to last child node

    ARGUMENTS
        None
*/
      Node* lastChild();

/*---------------------------------------------------------------------
    NAME
        nextSibling

    PURPOSE
        Get the next sibling node

    DESCRIPTION
        

    RETURNS
        (Node*) pointer to the next sibling node

    ARGUMENTS
        None
*/
      Node* nextSibling();

/*---------------------------------------------------------------------
    NAME
        previousSibling

    PURPOSE
        Get the previous sibling node

    DESCRIPTION
        

    RETURNS
        (Node*) pointer to the previous sibling node

    ARGUMENTS
        None
*/
      Node* previousSibling();

/*---------------------------------------------------------------------
    NAME
        nextNode

    PURPOSE
        Get the next node

    DESCRIPTION
        

    RETURNS
        (Node*) pointer to the next node

    ARGUMENTS
        None
*/
      Node* nextNode();

/*---------------------------------------------------------------------
    NAME
        previousNode

    PURPOSE
        Get the previous node

    DESCRIPTION
        

    RETURNS
        (Node*) pointer to te previous node

    ARGUMENTS
        None
*/
      Node* previousNode();

/*---------------------------------------------------------------------
    NAME
        adjustCtx

    PURPOSE
        Attach this tree walker to the another context

    DESCRIPTION
        Attaches this tree walker to the context associated with
	a given node reference

    RETURNS
        (void)

    ARGUMENTS
        nref (IN) given node reference
*/
      void adjustCtx( NodeRef< Node>& nref);

    private:
      // Default constructor and destructor
      TreeWalker() {}
      ~TreeWalker();
    };

/**********************************************************************
          Interface DocumentTraversal - DOM 2 interface

    This is an interface to the factory producing NodeIterator
    and TreeWalker objects. It keeps track of all objects that
    it created and destructs them when itself is destructed.
**********************************************************************/

    template< typename Node> class DocumentTraversal {
    public:

/*---------------------------------------------------------------------
    NAME
        ~DocumentTraversal

    PURPOSE
        Default destructor

    DESCRIPTION
        

    RETURNS
        (void)

    ARGUMENTS
        None
*/
      ~DocumentTraversal();

/*---------------------------------------------------------------------
    NAME
        DocumentTraversal

    PURPOSE
        Constructor

    DESCRIPTION
        Constructs the factory

    RETURNS
        (DocumentTraversal) new factory object

    ARGUMENTS
        none
*/
      DocumentTraversal();

/*---------------------------------------------------------------------
    NAME
        createNodeIterator

    PURPOSE
        Create new iterator object

    DESCRIPTION
        

    RETURNS
        (NodeIterator*) Pointer to new iterator

    ARGUMENTS
        root                     (IN) root of subtree to iterate on
	whatToShow               (IN) node types filter
	filter                   (IN) user defined node filter
	entityReferenceExpansion (IN) expand entity references if TRUE
*/
      NodeIterator< Node>* createNodeIterator( NodeRef< Node>& root,
	               WhatToShowCode whatToShow,
				   boolean entityReferenceExpansion)
	throw (DOMException);

/*---------------------------------------------------------------------
    NAME
        destroyNodeIterator

    PURPOSE
        Destroy node iterator object

    DESCRIPTION
        

    RETURNS
        (void)

    ARGUMENTS
        iter (IN) iterator to be destroyed
*/
      void destroyNodeIterator( NodeIterator< Node>* iter)
	throw (DOMException);

/*---------------------------------------------------------------------
    NAME
        createTreeWalker

    PURPOSE
        Create new tree walker object

    DESCRIPTION
        

    RETURNS
        (TreeWalker*) Pointer to new tree walker

    ARGUMENTS
        root                     (IN) root of subtree to traverse
	whatToShow               (IN) node types filter
	filter                   (IN) user defined node filter
	entityReferenceExpansion (IN) expand entity references if TRUE
*/
      TreeWalker< Node>* createTreeWalker( NodeRef< Node>& root,
	                      WhatToShowCode whatToShow,
				  boolean entityReferenceExpansion)
	throw (DOMException);

/*---------------------------------------------------------------------
    NAME
        destroyTreeWalker

    PURPOSE
        Destroy tree walker object

    DESCRIPTION
        

    RETURNS
        (void)

    ARGUMENTS
        walker (IN) tree walker to be destroyed
*/
      void destroyTreeWalker( TreeWalker< Node>* walker)
	throw (DOMException);

    };

/**********************************************************************
   Interface RangeException - Exceptions for DOM 2 Range operations

    Range operations raise exceptions only when an operation is
    impossible to perform for Range logic related reasons.
    Implementations might need to handle a wider range of error
    conditions. In these cases inherited XMLException functionality
    can be used.

**********************************************************************/

    class RangeException : public XmlException {
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
        getRangeCode

    PURPOSE
        Get Range exception code embedded in the exception 

    DESCRIPTION
        This is a virtual member function that defines a prototype
	for implementation defined member functions returning Range
	exception codes, defined in RangeExceptionCode, of the
	exceptional situations during execution

    RETURNS
        (RangeExceptionCode) exception code

    ARGUMENTS
        None
*/
      virtual RangeExceptionCode getRangeCode() const = 0;
    };

/**********************************************************************
          Interface Range - DOM 2 Range

**********************************************************************/

    template< typename Node> class Range {
    public:

/*---------------------------------------------------------------------
    NAME
        getStartContainer

    PURPOSE
        Get start container node

    DESCRIPTION
        

    RETURNS
        (Node*) start container node

    ARGUMENTS
        None
*/
      Node* getStartContainer() const;

/*---------------------------------------------------------------------
    NAME
        getStartOffset

    PURPOSE
        Get offset of the start point

    DESCRIPTION
        

    RETURNS
        (long) offset

    ARGUMENTS
        None
*/
      long getStartOffset() const;

/*---------------------------------------------------------------------
    NAME
        getEndContainer

    PURPOSE
        Get end container node

    DESCRIPTION
        

    RETURNS
        (Node*) end container node

    ARGUMENTS
        None
*/
      Node* getEndContainer() const;

/*---------------------------------------------------------------------
    NAME
        getEndOffset

    PURPOSE
        Get offset of the end point

    DESCRIPTION
        

    RETURNS
        (long) offset

    ARGUMENTS
        None
*/
      long getEndOffset() const;

/*---------------------------------------------------------------------
    NAME
        getCollapsed

    PURPOSE
        Check if the range is collapsed

    DESCRIPTION
        

    RETURNS
        (boolean) TRUE if the range is collapsed, FALSE otherwise

    ARGUMENTS
        None
*/
      boolean getCollapsed() const;

/*---------------------------------------------------------------------
    NAME
        getCommonAncestorContainer

    PURPOSE
        Get the deepest common ancestor node

    DESCRIPTION
        

    RETURNS
        (Node*) common ancestor node

    ARGUMENTS
        None
*/
      Node* getCommonAncestorContainer() const;

/*---------------------------------------------------------------------
    NAME
        setStart

    PURPOSE
        Set start point

    DESCRIPTION
        

    RETURNS
        (void)

    ARGUMENTS
        refNode (IN) start container
	offset  (IN) start offset
*/
      void setStart( NodeRef< Node>& refNode, long offset)
                        throw (RangeException, DOMException);

/*---------------------------------------------------------------------
    NAME
        setEnd

    PURPOSE
        Set end point

    DESCRIPTION
        

    RETURNS
        (void)

    ARGUMENTS
        refNode (IN) end container
	offset  (IN) end offset

*/
      void setEnd( NodeRef< Node>& refNode, long offset)
                        throw (RangeException, DOMException);

/*---------------------------------------------------------------------
    NAME
        setStartBefore

    PURPOSE
        

    DESCRIPTION
        

    RETURNS
        (void)

    ARGUMENTS
        refNode (IN) reference node
*/
      void setStartBefore( NodeRef< Node>& refNode)
                                        throw (RangeException);

/*---------------------------------------------------------------------
    NAME
        setStartAfter

    PURPOSE
        

    DESCRIPTION
        

    RETURNS
        (void)

    ARGUMENTS
        refNode (IN) reference node
*/
      void setStartAfter( NodeRef< Node>& refNode)
                                        throw (RangeException);

/*---------------------------------------------------------------------
    NAME
        setEndBefore

    PURPOSE
        

    DESCRIPTION
        

    RETURNS
        (void)

    ARGUMENTS
        refNode (IN) reference node
*/
      void setEndBefore( NodeRef< Node>& refNode)
                                         throw (RangeException);

/*---------------------------------------------------------------------
    NAME
        setEndAfter

    PURPOSE
        

    DESCRIPTION
        

    RETURNS
        (void)

    ARGUMENTS
        refNode (IN) reference node
*/
      void setEndAfter( NodeRef< Node>& refNode)
                                         throw (RangeException);
/*---------------------------------------------------------------------
    NAME
        collapse

    PURPOSE
        

    DESCRIPTION
        

    RETURNS
        (void)

    ARGUMENTS
        toStart (IN) TRUE if collapse to start
*/
      void collapse( boolean toStart);

/*---------------------------------------------------------------------
    NAME
        selectNode

    PURPOSE
        

    DESCRIPTION
        

    RETURNS
        (void)

    ARGUMENTS
        refNode (IN) reference node
*/
      void selectNode( NodeRef< Node>& refNode)
                                         throw (RangeException);

/*---------------------------------------------------------------------
    NAME
        select node content

    PURPOSE
        

    DESCRIPTION
        

    RETURNS
        (void)

    ARGUMENTS
        refNode (IN) reference node
*/
      void selectNodeContent( NodeRef< Node>& refNode)
                                         throw (RangeException);

/*---------------------------------------------------------------------
    NAME
        CompareBoundaryPoints

    PURPOSE
        

    DESCRIPTION
        

    RETURNS
        (CompareHowCode) result of comparison

    ARGUMENTS
        how         (IN) how to compare
	sourceRange (IN) range to compare with
*/
      CompareHowCode compareBoundaryPoints( unsigned short how,
				      Range< Node>* sourceRange)
                                             throw (DOMException);

/*---------------------------------------------------------------------
    NAME
        deleteContents

    PURPOSE
        

    DESCRIPTION
        

    RETURNS
        (void)

    ARGUMENTS
        None
*/
      void deleteContents() throw (DOMException);

/*---------------------------------------------------------------------
    NAME
        extractContent

    PURPOSE
        

    DESCRIPTION
        

    RETURNS
        (Node*) subtree extracted

    ARGUMENTS
        None
*/
      Node* extractContents() throw (DOMException);

/*---------------------------------------------------------------------
    NAME
        cloneContent

    PURPOSE
        

    DESCRIPTION
        

    RETURNS
        (Node*) subtree cloned

    ARGUMENTS
        None
*/
      Node* cloneContents() throw (DOMException);

/*---------------------------------------------------------------------
    NAME
        insertNode

    PURPOSE
        

    DESCRIPTION
        

    RETURNS
        (void)

    ARGUMENTS
        newNode (IN) node to insert
*/
      void insertNode( NodeRef< Node>& newNode)
                        throw (RangeException, DOMException);

/*---------------------------------------------------------------------
    NAME
        surroundContent

    PURPOSE
        

    DESCRIPTION
        

    RETURNS
        (void)

    ARGUMENTS
        newParent (IN) parent node
*/
      void surroundContents( NodeRef< Node>& newParent)
                        throw (RangeException, DOMException);

/*---------------------------------------------------------------------
    NAME
        cloneRange

    PURPOSE
        

    DESCRIPTION
        

    RETURNS
        (Range< Node>*) new cloned range

    ARGUMENTS
        
*/
      Range< Node>* cloneRange();

/*---------------------------------------------------------------------
    NAME
        toString

    PURPOSE
        

    DESCRIPTION
        

    RETURNS
        (oratext*) string representation of the range

    ARGUMENTS
        None
*/
      oratext* toString();

/*---------------------------------------------------------------------
    NAME
        detach

    PURPOSE
        invalidate the range

    DESCRIPTION
        Invalidates the range. It is not recommended to use
	this method since it leaves the object in invalid state.
	The preferable way is to call the destructor.

    RETURNS
        (void)

    ARGUMENTS
        None
*/
      void detach();

    private:
      // Default constructor
      Range() {}
      ~Range();
    };

/**********************************************************************
          Interface DocumentRange - DOM 2 interface

    This is an interface to the factory producing Range objects. It
    keeps track of all objects that it created and destructs them when
    itself is destructed.
**********************************************************************/

    template< typename Node> class DocumentRange {
    public:

/*---------------------------------------------------------------------
    NAME
        ~DocumentRange

    PURPOSE
        Default destructor

    DESCRIPTION
        

    RETURNS
        (void)

    ARGUMENTS
        None
*/
      ~DocumentRange();

/*---------------------------------------------------------------------
    NAME
        DocumentRange

    PURPOSE
        Constructor

    DESCRIPTION
        Constructs the factory

    RETURNS
        (DocumentRange) new factory object

    ARGUMENTS
        none
*/
      DocumentRange();

/*---------------------------------------------------------------------
    NAME
        createRange

    PURPOSE
        Create new range object

    DESCRIPTION
        

    RETURNS
        (Range*) Pointer to new range

    ARGUMENTS
        doc (IN) reference to a document node
*/
      Range< Node>* createRange( DocumentRef< Node>& doc);

/*---------------------------------------------------------------------
    NAME
        destroyRange

    PURPOSE
        Destroy range object

    DESCRIPTION
        

    RETURNS
        (void)

    ARGUMENTS
        range (IN) range to be destroyed
*/
      void destroyRange( Range< Node>* range)
	throw (DOMException);

    };

  }

  using namespace Dom;

/*----------------------------------------------------------------------
                  Package IO - Input Source options
-----------------------------------------------------------------------*/

  namespace IO {

/* DATATYPE InputSourceType - input source types
    It defines different input source types
*/
    typedef enum InputSourceType {
      ISRC_URI             = 1,
      ISRC_FILE            = 2,
      ISRC_BUFFER          = 3,
      ISRC_CSTREAM         = 4,
      ISRC_DOM             = 5
    } InputSourceType;

/* DATATYPE OutputSourceType - output source types
    It defines different output source types to save
    DOM or other binary representations of XML documents
    as the textual representation
*/
    typedef enum OutputSourceType {
      OSRC_URI             = 1,
      OSRC_FILE            = 2,
      OSRC_BUFFER          = 3,
      OSRC_CSTREAM         = 4,
      OSRC_CSTD            = 5
    } OutputSourceType;

/**********************************************************************
   Interface InputSource - top level class for various input sources
***********************************************************************/

    class InputSource {
    public:

/*---------------------------------------------------------------------
    NAME
        getISrcType

    PURPOSE
        Get the type of an input source

    DESCRIPTION
        Gets the type of an input source

    RETURNS
        (InputSourceType) input source type

    ARGUMENTS
        None
*/
      InputSourceType getISrcType() const { return isrctype; }

/*---------------------------------------------------------------------
    NAME
        setBaseURI

    PURPOSE
        Set the base URI

    DESCRIPTION
        Sets the base URI. It is used by some input sources
	such as File and URI

    RETURNS
        (void)

    ARGUMENTS
        baseURI (IN) base URI
*/
      void setBaseURI( oratext* base_URI) { baseURI = base_URI; }

/*---------------------------------------------------------------------
    NAME
        getBaseURI

    PURPOSE
        Get the base URI

    DESCRIPTION
        Gets the base URI. It is used by some input sources
	such as File and URI

    RETURNS
        (oratext*) base URI

    ARGUMENTS
        none
*/
      oratext* getBaseURI() { return baseURI; }

    protected:
      // default constructor - should never be called publically
      InputSource() {}

      InputSourceType isrctype;
      oratext* baseURI;
      boolean isBinary;
    };

    class URISource : public InputSource {

    public:
      // constructor
      URISource( oratext* uri_par, boolean is_bin = FALSE) : uri( uri_par)
      { isrctype = ISRC_URI; isBinary = is_bin; baseURI = NULL; }

      oratext* getURI() const { return uri; }

    private:
      oratext* uri;

      // should never be called publically
      URISource() {}
    };

    class FileSource : public InputSource {

    public:
      // constructor
      FileSource( oratext* filepath_par, boolean is_bin = FALSE) :
	fpath( filepath_par)
      { isrctype = ISRC_FILE; isBinary = is_bin; baseURI = NULL; }

      oratext* getFilePath() const { return fpath; }

    private:
      oratext* fpath;

      // should never be called publically
      FileSource() {}
    };

    class BufferSource : public InputSource {

    public:
      // constructor
      BufferSource( oratext* buf_par, ub4 len_par,
		    boolean is_bin = FALSE) :
	buf( buf_par), length( len_par)
      { isrctype = ISRC_BUFFER; isBinary = is_bin; baseURI = NULL; }

      oratext* getBufPtr() const { return buf; }
      ub4 getLength() const { return length; }

    private:
      oratext* buf;
      ub4 length;

      // should never be called publically
      BufferSource() {}
    };

    template< typename Node> class DOMSource : public InputSource {

    public:
      // constructor
      DOMSource( DocumentRef< Node>& doc_ref) : docref( doc_ref)
      { isrctype = ISRC_DOM; isBinary = FALSE; baseURI = NULL; }

      DocumentRef< Node>* getDocRef() const { return &docref; }

    private:
      DocumentRef< Node>& docref;

      DOMSource();
    };

    class CStreamSource : public InputSource {

    public:
      // constructor
      CStreamSource( void* stream_par, void* sctx_par) :
	stream( stream_par), sctx( sctx_par)
      { isrctype = ISRC_CSTREAM; isBinary = FALSE; baseURI = NULL; }

      void* getStream() const { return stream; }
      void* getStreamCtx() const { return sctx; }

    private:
      void* stream;
      void* sctx;

      // should never be called publically
      CStreamSource() {}
    };

/**********************************************************************
   Interface OutputSource - top level class for various output sources
***********************************************************************/

    class OutputSource {
    public:

/*---------------------------------------------------------------------
    NAME
        getOSrcType

    PURPOSE
        Get the type of an output source

    DESCRIPTION
        Gets the type of an output source

    RETURNS
        (OutputSourceType) output source type

    ARGUMENTS
        None
*/
      OutputSourceType getOSrcType() const { return osrctype; }

    protected:
      // default constructor - should never be called publically
      OutputSource() {}

      OutputSourceType osrctype;
    };

    class URIOSource : public OutputSource {

    public:
      // constructor
      URIOSource( oratext* uri_par) : uri( uri_par)
      { osrctype = OSRC_URI; }

      oratext* getURI() const { return uri; }

    private:
      oratext* uri;

      // should never be called publically
      URIOSource() {}
    };

    class FileOSource : public OutputSource {

    public:
      // constructor
      FileOSource( oratext* filepath_par) : fpath( filepath_par)
      { osrctype = OSRC_FILE; }

      oratext* getFilePath() const { return fpath; }

    private:
      oratext* fpath;

      // should never be called publically
      FileOSource() {}
    };

    class BufferOSource : public OutputSource {

    public:
      // constructor
      BufferOSource( oratext* buf_par, ub4 len_par) :
	buf( buf_par), length( len_par)
      { osrctype = OSRC_BUFFER; }

      oratext* getBufPtr() const { return buf; }
      ub4 getLength() const { return length; }

    private:
      oratext* buf;
      ub4 length;

      // should never be called publically
      BufferOSource() {}
    };

    class CStreamOSource : public OutputSource {

    public:
      // constructor
      CStreamOSource( void* stream_par, void* sctx_par) :
	stream( stream_par), sctx( sctx_par)
      { osrctype = OSRC_CSTREAM; }

      void* getStream() const { return stream; }
      void* getStreamCtx() const { return sctx; }

    private:
      void* stream;
      void* sctx;

      // should never be called publically
      CStreamOSource() {}
    };

    class CStdOSource : public OutputSource {

    public:
      // constructor
      CStdOSource()
      { osrctype = OSRC_CSTD; }
    };
  }

  using namespace IO;

/*----------------------------------------------------------------------
                  Package Parser - Parser Interfaces

    Parser interfaces include: Parser exceptions, Validator, Parser,
    DOMParser, SAXParser, DOMWriter interfaces

----------------------------------------------------------------------*/

  namespace Parser {

/* DATATYPE ParserExceptionCode - parser exceptions
    It defines parser exception codes. PARSER_UNDEFINED_ERR refers to
    non-documented errors. PARSER_OTHER_ERR refers to errors documented
    outside of the C++ framework, currently in xmlerr.h only.
*/
    typedef enum ParserExceptionCode {
      PARSER_UNDEFINED_ERR               = 0,
      PARSER_VALIDATION_ERR              = 1,
      PARSER_VALIDATOR_ERR               = 2,
      PARSER_BAD_ISOURCE_ERR             = 3,
      PARSER_CONTEXT_ERR                 = 4,
      PARSER_PARAMETER_ERR               = 5,
      PARSER_PARSE_ERR                   = 6,
      PARSER_SAXHANDLER_SET_ERR          = 7,
      PARSER_VALIDATOR_SET_ERR           = 8,
      PARSER_WRITE_ERR                   = 9,
      PARSER_OTHER_ERR                   = 10
    } ParserExceptionCode;

/* DATATYPE DOMParserIdType - DOM parser identifiers
    It defines DOM parser identifiers
*/
    typedef enum DOMParserIdType {
      DOMParCXml         = 1
    } DOMParserIdType;

/* DATATYPE SAXParserIdType - SAX parser identifiers
    It defines SAX parser identifiers
*/
    typedef enum SAXParserIdType {
      SAXParCXml         = 1
    } SAXParserIdType;

/* DATATYPE SchValidatorIdType - defines validator identifiers
   Defines identifiers for the validator implementations. These
   identifiers are used as parameters to the XML tools factory
   when a particular validator object has to be created.
*/
    typedef enum SchValidatorIdType {
      SchValCXml        = 1
    } SchValidatorIdType;

/* DATATYPE DOMWriterIdType - DOM writer identifiers
    It defines DOM writer identifiers
*/
    typedef enum DOMWriterIdType {
      DOMWrCXml         = 1
    } DOMWriterIdType;

/**********************************************************************
   Interface ParserException - Exception class for parser and validator
**********************************************************************/

    class ParserException : public XmlException {
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
        getParserCode

    PURPOSE
        Get parser exception code embedded in the exception 

    DESCRIPTION
        This is a virtual member function that defines a prototype
	for implementation defined member functions returning parser
	and validator exception codes, defined in ParserExceptionCode,
	of the exceptional situations during execution

    RETURNS
        (ParserExceptionCode) exception code

    ARGUMENTS
        None
*/
      virtual ParserExceptionCode getParserCode() const = 0;
    };

/**********************************************************************
         Interface SchemaValidator - XML schema aware validator

    This interface assumes that validator implementations follow the 
    W3C XML Schema specification (rev REC-xmlschema-1-20010502) and
    exibit the required behavior of an schema validator in terms of how
    multiple schema documents can be assembled into a schema and be used
    to validate a specific instance document.
**********************************************************************/

    template< typename Node> class SchemaValidator {
    public:

      virtual ~SchemaValidator() {}

/*---------------------------------------------------------------------
    NAME
        loadSchema

    PURPOSE
        Load up a schema document

    DESCRIPTION
        Load up a schema document to be used in the next validation
	session. Throws an exception in the case of an error.

    RETURNS
        (void)

    ARGUMENTS
        schema_URI (IN) URL of schema document [compiler encoding]
*/
      virtual void loadSchema( oratext* schema_URI)
	throw (ParserException) = 0;

/*---------------------------------------------------------------------
    NAME
        unloadSchema

    PURPOSE
        Unload a schema document

    DESCRIPTION
        Unload a schema document and all its descendants (included or
	imported nestedly from the validator. All previously
	loaded schema documents will remain loaded until they are
	unloaded. To unload all loaded schema documents, set
	schema_URI to be NULL. Throws an exception in the case of
	an error.

    RETURNS
        (void)

    ARGUMENTS
        schema_URI (IN) URL of schema document [compiler encoding]
*/
      virtual void unloadSchema( oratext* schema_URI)
	throw (ParserException) = 0;

/*----------------------------------------------------------------------
    NAME
        getSchemaList

    FUNCTION
        Return the size and/or list of loaded schema documents. 

    DESCRIPTION
        Return only the size of loaded schema list documents if "list"
	is NULL. If "list" is NOT NULL, a list of URL pointers is
	returned in the user-provided pointer buffer.  Note that its
	user's responsibility to provide a buffer with big enough size.  

    RETURNS
        (ub4) list size and list of loaded schemas (I/O parameter)

    ARGUMENTS
        list (I/O) address of a pointer buffer
*/
      virtual ub4 getSchemaList( oratext **list) const = 0;

/*---------------------------------------------------------------------
    NAME
        validate

    PURPOSE
        Validate an element node against loaded schema(s)

    DESCRIPTION
        Validates an element node against loaded schema(s).  The set of
	Schemas used in the current session consists of all schema
	documents specified through loadSchema() and provided as hint(s)
	through schemaLocation or noNamespaceSchemaLocation in the
	instance document. After the invokation of this routine, all
	loaded schema documents remain loaded and can be queried by
	getSchemaList.  However, they will remain inactive. In the
	next validation session, inactive schema documents can be
	activated by specifying them via loadSchema or providing
        them as hint(s) via schemaLocation or noNamespaceSchemaLocation
	in the new instance document.  To unload a schema document and
	all its descendants (documents included or imported nestedly),
	use unloadSchema. Throws an exception in the case of an error.

    RETURNS
        void

    ARGUMENTS
        elem_ref (IN) an element node in "doc" to be validated
*/
      virtual void validate( ElementRef< Node>& elem_ref)
	throw (ParserException) = 0;

/*---------------------------------------------------------------------
    NAME
        validate

    PURPOSE
        Validate an element node against loaded schema(s)

    DESCRIPTION
        Validates a document against loaded schema(s).  The set of
	Schemas used in the current session consists of all schema
	documents specified through loadSchema() and provided as hint(s)
	through schemaLocation or noNamespaceSchemaLocation in the
	instance document. After the invokation of this routine, all
	loaded schema documents remain loaded and can be queried by
	getSchemaList.  However, they will remain inactive. In the
	next validation session, inactive schema documents can be
	activated by specifying them via loadSchema or providing
        them as hint(s) via schemaLocation or noNamespaceSchemaLocation
	in the new instance document.  To unload a schema document and
	all its descendants (documents included or imported nestedly),
	use unloadSchema. Throws an exception in the case of an error.

    RETURNS
        void

    ARGUMENTS
        isrc_ptr (IN) input document
*/
      virtual void validate( InputSource* isrc_ptr)
	throw (ParserException) = 0;

/*---------------------------------------------------------------------
    NAME
        getValidatorId

    PURPOSE
        Get validator identifier

    DESCRIPTION
        Get the validator identifier corresponding to the implementation
	of this validator object

    RETURNS
        (SchValidatorIdType) validator identifier

    ARGUMENTS
        (none)
*/
      virtual SchValidatorIdType getValidatorId() const = 0;
    };

/**********************************************************************
             Interface GParser - Root class for XML parsers

    This is the root class for all Oracle XML parsers. It handles all
    options that can be set to any Oracle parser.
**********************************************************************/

    class GParser {
    public:

/*---------------------------------------------------------------------
    NAME
        getBaseURI

    PURPOSE
        Returns the base URI for the document

    DESCRIPTION
        Returns the base URI for the document. Usually only documents
	loaded from a URI will automatically have a base URI.
	Documents loaded from other sources (stdin, buffer, etc) will
	not naturally have a base URI, but a base URI may have been set
	for them using setBaseURI, for the purposes of resolving relative
	URIs in inclusion.

    RETURNS
        (oratext *) current document's base URI [or NULL]

    ARGUMENTS
        none
*/
      oratext* getBaseURI() const
	{ return base_uri; }

/*---------------------------------------------------------------------
    NAME
        getSchemaLocation

    PURPOSE
        Get schema location for this document

    DESCRIPTION
        Gets schema location for this document. It is used to figure
        out the optimal layout when loading documents into a database

    RETURNS
        (oratext*) schema location

    ARGUMENTS
        none
*/
      oratext* getSchemaLocation() const
	{ return schema_loc; }

/*---------------------------------------------------------------------
    NAME
        getDiscardWhitespaces

    PURPOSE
        Checks if whitespaces between elements are discarded

    DESCRIPTION
        Checks if formatting whitespaces between elements, such as
	newlines and indentation in input documents are discarded.
	By default, all input characters are preserved.

    RETURNS
        (boolean) TRUE if whitespace between elements are discarded

    ARGUMENTS
        none
*/
      boolean getDiscardWhitespaces() const
	{ return discard_white_spaces; }

/*---------------------------------------------------------------------
    NAME
        getExpandCharRefs

    PURPOSE
        Checks if character references are expanded

    DESCRIPTION
        Checkss if character references are expanded in the DOM data.
	By default, character references are replaced by the character
	they represent. However, when a document is saved those
	characters entities do not reappear. To ensure they remain
	through load and save, they should not be expanded.

    RETURNS
        (boolean) TRUE if character references are expanded

    ARGUMENTS
        none
*/
      boolean getExpandCharRefs() const
	{ return expand_char_refs; }

/*---------------------------------------------------------------------
    NAME
        getStopOnWarning

    PURPOSE
        Get if document processing stops on warnings

    DESCRIPTION
        When TRUE is returned, warnings are treated the same as errors
	and cause parsing, validation, etc, to stop immediately.
	By default, warnings are issued but the processing continues.

    RETURNS
        (boolean) TRUE if document processing stops on warnings

    ARGUMENTS
        none
*/
      boolean getStopOnWarning() const
	{ return stop_on_warning; }

/*---------------------------------------------------------------------
    NAME
        getWarnDuplicateEntity

    PURPOSE
        Get if multiple entity declarations cause a warning

    DESCRIPTION
        Get if entities which are declared more than once will
        cause warnings to be issued

    RETURNS
        (boolean) TRUE if multiple entity declarations cause a warning

    ARGUMENTS
        none
*/
      boolean getWarnDuplicateEntity() const
	{ return warn_duplicate_entity; }

/*---------------------------------------------------------------------
    NAME
        setBaseURI

    PURPOSE
        Sets the base URI for the document

    DESCRIPTION
        Sets the base URI for the document. Usually only documents
	that were loaded from a URI will automatically have a base URI.
	Documents loaded from other sources (stdin, buffer, etc) will
	not naturally have a base URI, but a base URI may have been set
	for them using setBaseURI, for the purposes of resolving relative
	URIs in inclusion.

    RETURNS
        (void)

    ARGUMENTS
        par (IN) base URI
*/
      void setBaseURI( oratext* par)
	{ base_uri = par; }

/*---------------------------------------------------------------------
    NAME
        setSchemaLocation

    PURPOSE
        Set schema location for this document

    DESCRIPTION
        Sets schema location for this document. It is used to figure
        out the optimal layout when loading documents into a database

    RETURNS
        (void)

    ARGUMENTS
        par (IN) schema location
*/
      void setSchemaLocation( oratext* par)
	{ schema_loc = par; }

/*---------------------------------------------------------------------
    NAME
        setDiscardWhitespaces

    PURPOSE
        Sets if formatting whitespaces should be discarded

    DESCRIPTION
        Sets if formatting whitespaces between elements (newlines
        and indentation) in input documents are discarded. By default,
        ALL input characters are preserved.

    RETURNS
        (void)

    ARGUMENTS
        par_bool (IN) TRUE if whitespaces should be discarded
*/
      void setDiscardWhitespaces( boolean par_bool)
	{ discard_white_spaces = par_bool; }

/*---------------------------------------------------------------------
    NAME
        setExpandCharRefs

    PURPOSE
        Get if character references are expanded

    DESCRIPTION
        Sets if character references should be expanded in the DOM data.
	Ordinarily, character references are replaced by the character
	they represent. However, when a document is saved those
	characters entities do not reappear. To ensure they remain
	through load and save is to not expand them.

    RETURNS
        (void)

    ARGUMENTS
        par_bool (IN) TRUE if character references should be expanded
*/
      void setExpandCharRefs( boolean par_bool)
	{expand_char_refs = par_bool; }

/*---------------------------------------------------------------------
    NAME
        setStopOnWarning

    PURPOSE
        Sets if document processing stops on warnings

    DESCRIPTION
        When TRUE is set, warnings are treated the same as errors
	and cause parsing, validation, etc, to stop immediately.
	By default, warnings are issued but the processing continues.

    RETURNS
        (void)

    ARGUMENTS
        par_bool (IN) TRUE if document processing should stop on warnings
*/
      void setStopOnWarning( boolean par_bool)
	{stop_on_warning = par_bool; }

/*---------------------------------------------------------------------
    NAME
        SetWarnDuplicateEntity

    PURPOSE
        Specifies if multiple entity declarations result in a warning

    DESCRIPTION
        Specifies if entities that are declared more than once will
	cause warnings to be issued

    RETURNS
        (void)

    ARGUMENTS
        par_bool (IN) TRUE if multiple entity declarations cause a warning
*/
      void setWarnDuplicateEntity( boolean par_bool)
	{ warn_duplicate_entity = par_bool; }


    protected:
      // Sets default values
      GParser()
      {
	base_uri = NULL;
	schema_loc = NULL;
	discard_white_spaces = FALSE;
	expand_char_refs = TRUE;
	stop_on_warning = FALSE;
	warn_duplicate_entity = FALSE;
      }

      oratext* base_uri;
      oratext* schema_loc;
      boolean discard_white_spaces;
      boolean expand_char_refs;
      boolean stop_on_warning;
      boolean warn_duplicate_entity;
    };

/**********************************************************************
            Interface DOMParser - DOM parser root class
**********************************************************************/

    template< typename Context, typename Node> class DOMParser :
                                       public GParser {
    public:

/*---------------------------------------------------------------------
    NAME
        parseDTD

    PURPOSE
        Parse DTD document

    DESCRIPTION
        Parses an external DTD document and returns the document
	node of the DTD tree.

    RETURNS
        (DocumentRef) DTD document tree

    ARGUMENTS
        src_ptr  (IN) input source of the document to parse
	no_mod   (IN) if TRUE, no modification allowed on result tree
	impl_ptr (IN) DOMImplementation to be used for result tree
*/
      virtual DocumentRef< Node>*
      parseDTD( InputSource* src_ptr, boolean no_mod = FALSE,
		DOMImplementation< Node>* impl_ptr = NULL)
	throw (ParserException) = 0;

/*---------------------------------------------------------------------
    NAME
        parse

    PURPOSE
        Parse the document

    DESCRIPTION
        Parses the document and returns the tree document node

    RETURNS
        (DocumentRef) document tree

    ARGUMENTS
        isrc_ptr    (IN) input source of the document to parse
	DTDvalidate (IN) if TRUE, document is validated
	dtd_ptr     (IN) DTD reference pointer
	no_mod      (IN) if TRUE, no modification allowed on result tree
	new_domimpl (IN) DOMImplementation to be used for result tree
*/
      virtual DocumentRef< Node>*
      parse( InputSource* isrc_ptr, boolean DTDvalidate = FALSE,
	     DocumentTypeRef< Node>* dtd_ptr = NULL,
	     boolean no_mod = FALSE,
	     DOMImplementation< Node>* impl_ptr = NULL)
	throw (ParserException) = 0;

/*---------------------------------------------------------------------
    NAME
        setValidator

    PURPOSE
        Set the validator for this parser
        
    DESCRIPTION
        Sets the validator for all validations except
        when another one is given in parseSchVal

    RETURNS
        (void)

    ARGUMENTS
        tor_ptr (IN) pointer to the schema validator
*/
      virtual void setValidator( SchemaValidator< Node>* tor_ptr) = 0;

/*---------------------------------------------------------------------
    NAME
        parseSchVal

    PURPOSE
        Parse and validate the document

    DESCRIPTION
        Parses and validates the document. Sets the validator if
	the corresponding parameter is NULL.

    RETURNS
        (DocumentRef) document tree

    ARGUMENTS
        src_ptr     (IN) input source of the document to parse
	no_mod      (IN) if TRUE, no modification allowed on result tree
	new_domimpl (IN) DOMImplementation to be used for result tree
	tor_ptr     (IN) pointer to the schema validator
*/
      virtual DocumentRef< Node>*
      parseSchVal( InputSource* src_par, boolean no_mod = FALSE,
		   DOMImplementation< Node>* impl_ptr = NULL,
		   SchemaValidator< Node>* tor_ptr = NULL)
        throw (ParserException) = 0;

/*---------------------------------------------------------------------
    NAME
        getParserId

    PURPOSE
        Get parser's Id

    DESCRIPTION
        Gets parser's Id

    RETURNS
        (DOMParserIdType) Parser Id

    ARGUMENTS
        None
*/
      virtual DOMParserIdType getParserId() const = 0;

/*---------------------------------------------------------------------
    NAME
        getContext

    PURPOSE
        Returns parser's XML context (allocation and encodings)

    DESCRIPTION
        Each parser object is allocated and executed in a particular
	Oracle XML context. This member function returns a pointer
	to this context.

    RETURNS
        (Context*) pointer to parser's context

    ARGUMENTS
        none
*/
      virtual Context* getContext() const = 0;

      virtual ~DOMParser() {}
    };

/**********************************************************************
       Interface SAXHandlerRoot - root class for all SAX handlers

    This class provides the base for future non-virtual derivations
**********************************************************************/

    class SAXHandlerRoot {

    protected:
      // default constructor - should never be called publically
      SAXHandlerRoot() {}
    };

/************************************************************************
Interface SAXHandler - root class for current SAX handler implementations
************************************************************************/

    template< typename Node> class SAXHandler : public SAXHandlerRoot {
    public:

/*---------------------------------------------------------------------
    NAME
        startDocument

    PURPOSE
        Receive notification of the start of the document

    DESCRIPTION
        Receives notification of the start of the document

    RETURNS
        (void)

    ARGUMENTS
        None
*/
      virtual void startDocument() = 0;

/*---------------------------------------------------------------------
    NAME
        endDocument

    PURPOSE
        Receive notification of the end of the document

    DESCRIPTION
        Receives notification of the end of the document

    RETURNS
        (void)

    ARGUMENTS
        None
*/
      virtual void endDocument() = 0;

/*---------------------------------------------------------------------
    NAME
        startElement

    PURPOSE
        Receive notification of element's start

    DESCRIPTION
        This event marks the start of an element.

    RETURNS
        (void)

    ARGUMENTS
        name      (IN) name of element
        attrs_ref (IN) list of element's attributes
*/
      virtual void startElement( oratext* name,
				 NodeListRef< Node>* attrs_ptr) = 0;

/*---------------------------------------------------------------------
    NAME
        startElementNS

    PURPOSE
        Receive namespace aware notification of element's start

    DESCRIPTION
        This event marks the start of an element. Note this is the new
        SAX 2 namespace-aware version. The element's QName, local name,
	and namespace URI will be in the data encoding, as are all the
	attribute parts.

    RETURNS
        (void)

    ARGUMENTS
        qname     (IN) element's qualified name
        local     (IN) element's namespace local name
        ns_URI    (IN) element's namespace URI
        attrs_ref (IN) NodeList of element's attributes

*/
      virtual void startElementNS( oratext* qname, oratext* local,
				   oratext* ns_URI,
				   NodeListRef< Node>* attrs_ptr) = 0;

/*---------------------------------------------------------------------
    NAME
        endElement

    PURPOSE
        Receive notification of element's end

    DESCRIPTION
        This event marks the end of an element. The name is the
        tagName of the element (which may be a QName for namespace-aware
        elements) and is in the data encoding.

    RETURNS
        (void)

    ARGUMENTS
        name (IN) name of element
*/
      virtual void endElement( oratext* name) = 0;

/*---------------------------------------------------------------------
    NAME
        characters

    PURPOSE
        Receive notification of character data

    DESCRIPTION
        This event marks character data

    RETURNS
        (void)

    ARGUMENTS
        ch   (IN) pointer to data
        size (IN) length of data

*/
      virtual void characters( oratext* ch, ub4 size) = 0;

/*---------------------------------------------------------------------
    NAME
        whitespace

    PURPOSE
        Receive notification of whitespace characters

    DESCRIPTION
        This event marks ignorable whitespace data such as newlines,
	and indentation between lines.

    RETURNS
        (void)

    ARGUMENTS
        data (IN) pointer to data
        size (IN) length of data
*/
      virtual void whitespace( oratext* data, ub4 size) = 0;

/*---------------------------------------------------------------------
    NAME
        processingInstruction

    PURPOSE
        Receive notification of a processing instruction

    DESCRIPTION
        This event marks a processing instruction.  The PI's target and
        data will be in the data encoding. There is always a target, but
        the data may be NULL.

    RETURNS
        (void)

    ARGUMENTS
        target (IN) PI's target
        data   (IN) PI's data
*/
      virtual void processingInstruction( oratext* target,
					oratext* data) = 0;

/*---------------------------------------------------------------------
    NAME
        notationDecl

    PURPOSE
        Receive notification of a notation declaration

    DESCRIPTION
        The even marks the declaration of a notation in the DTD.  The
        notation's name, public ID, and system ID will all be in the
        data encoding.  Both IDs are optional and may be NULL.

    RETURNS
        (void)

    ARGUMENTS
        name      (IN) notation's name
        public_iD (IN) notation's public ID
        system_iD (IN) notation's system ID
*/
      virtual void notationDecl( oratext* name,
			    oratext* public_id, oratext* system_id) = 0;

/*---------------------------------------------------------------------
    NAME
        comment

    PURPOSE
        Receive notification of a comment

    DESCRIPTION
        This event marks a comment in the XML document. The comment's
	data will be in the data encoding. It is an Oracle extension,
	not in SAX standard.

    RETURNS
        (void)

    ARGUMENTS
        data (IN) comment's data
*/
      virtual void comment( oratext* data) = 0;

/*---------------------------------------------------------------------
    NAME
        elementDecl

    PURPOSE
        Receive notification of element's declaration

    DESCRIPTION
        This event marks an element declaration in the DTD. It is
	an Oracle extension; not in SAX standard

    RETURNS
        (void)

    ARGUMENTS
        name    (IN) element's name
	content (IN) element's content model
*/
      virtual void elementDecl( oratext *name, oratext *content) = 0;

/*---------------------------------------------------------------------
    NAME
        attributeDecl

    PURPOSE
        Receive notification of attribute's declaration

    DESCRIPTION
        This event marks an attribute declaration in the DTD. It is
	an Oracle extension; not in SAX standard

    RETURNS
        (void)

    ARGUMENTS
        name      (IN) element that attribute is declared for
        attr_name (IN) attribute's name
        content   (IN) body of attribute declaration
*/
      virtual void attributeDecl( oratext* attr_name,
				oratext *name, oratext *content) = 0;

/*---------------------------------------------------------------------
    NAME
        XMLDecl

    PURPOSE
        Receive notification of an XML declaration

    DESCRIPTION
        This event marks an XML declaration (XMLDecl). The startDocument
        event is always first; this event will be the second event.
	The encoding flag says whether an encoding was specified. For
	the standalone flag, -1 will be returned if it was not specified,
	otherwise 0 for FALSE, 1 for TRUE. This member function is an
	Oracle extension.

    RETURNS
        (void)

    ARGUMENTS
        version       (IN) version string from XMLDecl
        is_encoding   (IN) encoding was specified?
        standalone    (IN) value of standalone-document flag
*/
      virtual void XMLDecl( oratext* version,
			    boolean is_encoding, sword standalone) = 0;

/*---------------------------------------------------------------------
    NAME
        CDATA

    PURPOSE
        Receive notification of CDATA

    DESCRIPTION
        This event handles CDATA, as distinct from Text. The data
        will be in the data encoding, and the returned length is in
        characters, not bytes. This is an Oracle extension.

    RETURNS
        (void)

    ARGUMENTS
        data (IN) pointer to CDATA
	size (IN) size of CDATA
*/
      virtual void CDATA( oratext* data, ub4 size) = 0;

/*---------------------------------------------------------------------
    NAME
        parsedEntityDecl

    PURPOSE
        Receive notification of a parsed entity declaration

    DESCRIPTION
        Marks a parsed entity declaration in the DTD. The parsed entity's
        name, public ID, system ID, and notation name will all be in the
        data encoding. This is an Oracle extension.

    RETURNS
        (void)

    ARGUMENTS
        name       (IN) entity's name
        value      (IN) entity's value if internal
        public_id  (IN) entity's public ID
        system_id  (IN) entity's system ID
        is_general (IN) is general entity?  FALSE if parameter entity

*/
      virtual void parsedEntityDecl( oratext* name, oratext* value,
          oratext* public_id, oratext* system_id, boolean general) = 0;

/*---------------------------------------------------------------------
    NAME
        unparsedEntityDecl

    PURPOSE
        Receive notification of a unparsed entity declaration

    DESCRIPTION
        Marks an unparsed entity declaration in the DTD. The unparsed
        entity's name, public ID, system ID, and notation name will all
	be in the data encoding.

    RETURNS
        

    ARGUMENTS
        name          (IN) entity's name
        public_id     (IN) entity's public ID
        system_id     (IN) entity's system ID
        notation_name (IN) entity's notation name
*/
      virtual void unparsedEntityDecl( oratext* name,
		       oratext* public_id, oratext* system_id,
				       oratext* notation_name) = 0;
      void *clntctx; 
    };

/************************************************************************
          Interface SAXParser - root class for all SAX parsers
************************************************************************/


    template< typename Context> class SAXParser :
                                       public GParser {
    public:

/*---------------------------------------------------------------------
    NAME
        setSAXHandler

    PURPOSE
        Set SAX handler

    DESCRIPTION
        Sets SAX handler for all parser invokations except when
	another SAX handler is specified in the parser call.

    RETURNS
        (void)

    ARGUMENTS
        hdlr_ptr (IN) SAX handler pointer
*/
      virtual void setSAXHandler( SAXHandlerRoot* hdlr_ptr) = 0;

/*---------------------------------------------------------------------
    NAME
        parseDTD

    PURPOSE
        Parse DTD document

    DESCRIPTION
        Parses an external DTD document and generates DTD parsing
	events

    RETURNS
        (void)

    ARGUMENTS
        src_ptr  (IN)
	hdlr_ptr (IN)
*/
      virtual void parseDTD( InputSource* src_ptr,
			     SAXHandlerRoot* hdlr_ptr = NULL)
	throw (ParserException) = 0;

/*---------------------------------------------------------------------
    NAME
        parse

    PURPOSE
        Parse the document

    DESCRIPTION
        Parses the document and returns SAX events
    RETURNS
        (void)

    ARGUMENTS
        src_ptr     (IN)
	DTDvalidate (IN)
	hdlr_ptr    (IN)
*/
      virtual void parse( InputSource* src_ptr,
			  boolean DTDvalidate = FALSE,
			  SAXHandlerRoot* hdlr_ptr = NULL)
	throw (ParserException) = 0;

/*---------------------------------------------------------------------
    NAME
        getParserId

    PURPOSE
        Get parsers's Id

    DESCRIPTION
        Gets parser's Id

    RETURNS
        (SAXParserIdType) Parser Id

    ARGUMENTS
        None
*/
      virtual SAXParserIdType getParserId() const = 0;

/*---------------------------------------------------------------------
    NAME
        getContext

    PURPOSE
        Returns parser's XML context (allocation and encodings)

    DESCRIPTION
        Each parser object is allocated and executed in a particular
	Oracle XML context. This member function returns a pointer
	to this context.

    RETURNS
        (Context*) pointer to parser's context

    ARGUMENTS
        none
*/
      virtual Context* getContext() const = 0;

      virtual ~SAXParser() {}

    protected:
    };

/**********************************************************************
             Interface GWriter - Root class for XML writers

    This is the root class for all Oracle XML writerss. It handles all
    options that can be set to any Oracle writer.
**********************************************************************/

    class GWriter {
    public:

/*---------------------------------------------------------------------
    NAME
        setOutputEncoding

    PURPOSE
        Set output encoding

    DESCRIPTION
        Sets the name of final encoding for the document. Unless
	specified, the saved document will be in the same encoding as
	in Context

    RETURNS
        (void)

    ARGUMENTS
        out_enc (IN) output encoding
*/
      void setOutputEncoding( oratext* out_enc)
	{ output_encoding = out_enc; }

/*---------------------------------------------------------------------
    NAME
        getOutputEncoding

    PURPOSE
        Get output encoding

    DESCRIPTION
        Gets output encoding

    RETURNS
        (oratext*) output encoding

    ARGUMENTS
        (IN)
*/
      oratext* getOutputEncoding() const
	{ return output_encoding; }

/*---------------------------------------------------------------------
    NAME
        setIndentStep

    PURPOSE
        Set the indentation step

    DESCRIPTION
        Sets the number of spaces to indent each level of output.
	The default is 4; 0 means no indentation

    RETURNS
        (void)

    ARGUMENTS
        ind_step (IN) indentation step
*/
      void setIndentStep( unsigned ind_step)
	{ indent_step = ind_step; }

/*---------------------------------------------------------------------
    NAME
        getIndentStep

    PURPOSE
        Get the indentation step

    DESCRIPTION
        Gets the number of spaces that is used to indent each level
	of output.

    RETURNS
        (unsigned) indentation step

    ARGUMENTS
        none
*/
      unsigned getIndentStep() const
	{ return indent_step; }

/*---------------------------------------------------------------------
    NAME
        setIndentLevel

    PURPOSE
        Set the indentation level

    DESCRIPTION
        Sets the initial indentation level. The default is 0, which
	means no indentation, flush left

    RETURNS
        (void)

    ARGUMENTS
        ind_level (IN) initial indentation level
*/
      void setIndentLevel( unsigned ind_level)
	{ indent_level = ind_level; }

/*---------------------------------------------------------------------
    NAME
        getIndenLevel

    PURPOSE
        Get initial indentation level

    DESCRIPTION
        Gets initial indentation level

    RETURNS
        (unsigned) initial indentation level

    ARGUMENTS
        none
*/
      unsigned getIndentLevel() const
	{ return indent_level; }

/*---------------------------------------------------------------------
    NAME
        setEOL

    PURPOSE
        Set the EOL string

    DESCRIPTION
        Sets a custom string to be used at End of Line
	instead of the default <newline> character

    RETURNS
        (void)

    ARGUMENTS
        eol_str (IN) EOL string
*/
      void setEOL( oratext* eol_str)
	{ eol_string = eol_str; }

/*---------------------------------------------------------------------
    NAME
        getEOL

    PURPOSE
        Get the EOL string

    DESCRIPTION
        Gets the EOL string

    RETURNS
        (oratext*) EOL string

    ARGUMENTS
        none
*/
      oratext* getEOL() const
	{ return eol_string; }

    protected:
      // Sets default values
      GWriter()
      {
	output_encoding = NULL;
	indent_step = 4;
	indent_level = 0;
	eol_string = NULL;
      }

      oratext* output_encoding;
      unsigned indent_step;
      unsigned indent_level;
      oratext* eol_string;
    };

/**********************************************************************
            Interface DOMWriter - DOM writer root class
**********************************************************************/

    template< typename Node> class DOMWriter :
                                       public GWriter {
    public:

/*---------------------------------------------------------------------
    NAME
        writeNode

    PURPOSE
        Write the document to the output

    DESCRIPTION
        Writes the document or any its subtree to the output.
	If no output source is provided, just returns the size but
	does not output. When writing to a buffer, if the buffer
	overflows, an exception is thrown.

    RETURNS
        (ubig_ora) number of bytes written to the output

    ARGUMENTS
        osrc_ptr    (IN) output source
	node_ref    (IN) root of the document or subtree to
*/
      virtual ubig_ora
      writeNode( OutputSource* osrc_ptr, NodeRef< Node>& node_ref)
	throw (ParserException) = 0;

/*---------------------------------------------------------------------
    NAME
        getWriterId

    PURPOSE
        Get writer ID

    DESCRIPTION
        Gets Writer ID

    RETURNS
        (DOMWriterIdType) writer Id

    ARGUMENTS
        None
*/
      virtual DOMWriterIdType getWriterId() const = 0;

      virtual ~DOMWriter() {}
    };
  }

  using namespace Parser;

/*---------------------------------------------------------------------
                    Package Xsl - Package one-line doc

    Xsl is the namespace (sub-space of OracleXml) for XSLT related
types and interfaces.

-------------------------------------------------------------------*/

  namespace Xsl{

/* DATATYPE XslExceptionCode - XSLT related exceptions
    It defines XSLT exception codes. XSL_UNDEFINED_ERR refers to
    non-documented errors. XSL_OTHER_ERR refers to errors documented
    outside of the C++ framework, currently in xmlerr.h only.
*/
    typedef enum XslExceptionCode {
      XSL_UNDEFINED_ERR               = 0,
      XSL_OTHER_ERR                   = 1
    } XslExceptionCode;

/* DATATYPE XslTrIdType - XSL transformer identifiers
    It defines XSL transformer identifiers
*/
    typedef enum XslTrIdType {
      XslTrCXml         = 1,
      XvmTrCXml         = 2
    } XslTrIdType;

/* DATATYPE XslCompIdType - XSL compiler identifiers
    It defines XSL compiler identifiers
*/
    typedef enum XslCompIdType {
      XvmCompCXml         = 1
    } XslCompIdType;

/**********************************************************************
  Interface XslException - Exceptions for XSL compilers & transformers
**********************************************************************/

    class XslException : public XmlException {
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
        getXslCode

    PURPOSE
        Get XSL exception code embedded in the exception 

    DESCRIPTION
        This is a virtual member function that defines a prototype
	for implementation defined member functions returning XSL
	transformer and compiler exception codes, defined in
	XslExceptionCode, of the exceptional situations during
	execution

    RETURNS
        (XslExceptionCode) exception code

    ARGUMENTS
        None
*/
      virtual XslExceptionCode getXslCode() const = 0;
    };

/**********************************************************************
            Interface Transformer - basic XSLT Processor
**********************************************************************/

    template< typename Context, typename Node> class Transformer {
    public:

/*---------------------------------------------------------------------
    NAME
        setXSL

    PURPOSE
        Set XSLT document for this transformer

    DESCRIPTION
        Set XSLT document for this transformer. Should be called
	before the transform member function is called

    RETURNS
        (void)

    ARGUMENTS
        isrc_ptr (IN) instance document to process
*/
      virtual void setXSL (InputSource* isrc_ptr)
	throw (XslException) = 0;

/*---------------------------------------------------------------------
    NAME
        transform

    PURPOSE
        Transform the document and return DOM

    DESCRIPTION
        Transforms the document and returns DOM. Throws an exception
	if an XSLT document is not set by a previous call to setXSL.
        
    RETURNS
        (DocumentRef) document tree of new document

    ARGUMENTS
        isrc_ptr (IN) instance document to process
	no_mod   (IN) if TRUE, no modification alloed on result tree
	impl_ptr (IN) DOMImplementation to be used for result tree
*/
      virtual NodeRef< Node>*
      transform( InputSource* isrc_ptr, boolean no_mod = FALSE,
		 DOMImplementation< Node>* impl_ptr = NULL)
	throw (XslException) = 0;

/*---------------------------------------------------------------------
    NAME
        setSAXHandler

    PURPOSE
        Set SAX handler

    DESCRIPTION
        Sets the SAX handler for following calls to the transform
	member function with hdlr_ptr being NULL

    RETURNS
        (void)

    ARGUMENTS
        hdlr_ptr (IN) SAX handler pointer
*/
      virtual void setSAXHandler( SAXHandlerRoot* hdlr_ptr) = 0;

/*---------------------------------------------------------------------
    NAME
        transform

    PURPOSE
        Transform the document and return SAX events

    DESCRIPTION
        Transforms the document and returns SAX events. If
	hdlr_ptr is not NULL, the SAX handler is used just for
	this call only.

    RETURNS
        (void)

    ARGUMENTS
        isrc_ptr    (IN) instance document tp process
	hdlr_ptr    (IN) SAX handler pointer
*/
      virtual void transform( InputSource* isrc_ptr,
			      SAXHandlerRoot* hdlr_ptr)
	throw (XslException) = 0;

/*---------------------------------------------------------------------------
    NAME
        resetAllParams
        
    PURPOSE
        Reset XSL processor parameters.

    DESCRIPTION
        Reset all the top level parameters added

    RETURNS
        (void)

    ARGUMENTS
        none
*/
      virtual void resetAllParams() throw (XslException) = 0;

/*---------------------------------------------------------------------------
    NAME
        setTextParam
        
    PURPOSE
        Set the output text param.

    DESCRIPTION
        Sets the output text param.

    RETURNS
        (void)

    ARGUMENTS
        name  (IN) name of top-level param
        value (IN) value of top-level param
*/
      virtual void setTextParam( oratext *name, oratext *value)
	throw (XslException) = 0;

/*---------------------------------------------------------------------------
    NAME
        setOutputEncoding
        
    PURPOSE
        Set the output encoding

    DESCRIPTION
        Sets the output encoding

    RETURNS
        (void)

    ARGUMENTS
        encoding (IN) output encoding
*/
      virtual void setOutputEncoding( oratext* encoding)
	throw (XslException) = 0;

/*---------------------------------------------------------------------
    NAME
        getTransformerId

    PURPOSE
        Get transformer's Id

    DESCRIPTION
        Gets transformer's Id
        
    RETURNS
        (XslTrIdType) Transformer's Id

    ARGUMENTS
        None
*/
      virtual XslTrIdType getTransformerId() const = 0;

      virtual ~Transformer() {}

    protected:
    };


/**********************************************************************
        Interface CompTransformer - Extended XSLT Processor
**********************************************************************/

    template< typename Context, typename Node> class CompTransformer :
                                public Transformer< Context, Node>{
    public:

/*---------------------------------------------------------------------
    NAME
        setXSL

    PURPOSE
        Set XSLT document for this transformer

    DESCRIPTION
        Set XSLT document for this transformer. Should be called
	before the transform member function is called. It is
	inherited from Transform

    RETURNS
        (void)

    ARGUMENTS
        isrc_ptr    (IN) instance document tp process
*/
      // virtual void setXSL (InputSource* isrc_ptr)
      //   throw (XslException) = 0;

/*---------------------------------------------------------------------
    NAME
        setBinXsl

    PURPOSE
        Set compiled XSLT document

    DESCRIPTION
        Sets the compiled XSLT document

    RETURNS
        (void)

    ARGUMENTS
        binxsl_ptr (IN) compiled Xsl
*/
      virtual void setBinXsl (ub2* binxsl_ptr)
	throw (XslException) = 0;

/*---------------------------------------------------------------------
    NAME
        transform

    PURPOSE
        Transform the document and return DOM

    DESCRIPTION
        Inherited from Transform

    RETURNS
        (DocumentRef) document tree of new document

    ARGUMENTS
        isrc_ptr (IN) instance document to process
	no_mod   (IN) if TRUE, no modification alloed on result tree
	impl_ptr (IN) DOMImplementation to be used for result tree
*/
      // virtual NodeRef< Node>*
      // transform( InputSource* isrc_ptr, boolean no_mod = FALSE,
      //            DOMImplementatioin< Node>* impl_ptr = NULL)
      //   throw (XslException) = 0;

/*---------------------------------------------------------------------
    NAME
        setSAXHandler

    PURPOSE
        Set SAX handler

    DESCRIPTION
        Inherited from Transformer

    RETURNS
        (void)

    ARGUMENTS
        hdlr_ptr (IN) SAX handler pointer
*/
      // virtual void setSAXHandler( SAXHandlerRoot* hdlr_ptr) = 0;

/*---------------------------------------------------------------------
    NAME
        transform

    PURPOSE
        Transform the document and return SAX events

    DESCRIPTION
        Inherited from Transform

    RETURNS
        (void)

    ARGUMENTS
        isrc_ptr    (IN) instance document to process
	hdlr_ptr    (IN) SAX handler pointer
*/
      // virtual void transform( InputSource* isrc_ptr,
      //			 SAXHandlerRoot* hdlr_ptr)
      //	throw (XslException) = 0;

/*---------------------------------------------------------------------------
    NAME
        resetAllParams
        
    PURPOSE
        Reset XSL processor parameters.

    DESCRIPTION
        Inherited from class Transformer

    RETURNS
        (void)

    ARGUMENTS
        none
*/
      // virtual void resetAllParams() throw (XslException) = 0;

/*---------------------------------------------------------------------------
    NAME
        setTextParam
        
    PURPOSE
        Set the output text param.

    DESCRIPTION
        Inherited from class Transformer

    RETURNS
        (void)

    ARGUMENTS
        name  (IN) name of top-level param
        value (IN) value of top-level param
*/
      // virtual void setTextParam( oratext *name, oratext *value)
      //   throw (XslException) = 0;

/*---------------------------------------------------------------------------
    NAME
        setOutputEncoding
        
    PURPOSE
        Set the output encoding

    DESCRIPTION
        Inherited from class Transformer

    RETURNS
        (void)

    ARGUMENTS
        encoding (IN) output encoding
*/
      // virtual void setOutputEncoding( oratext* encoding)
      //   throw (XslException) = 0;

/*---------------------------------------------------------------------
    NAME
        getTransformerId

    PURPOSE
        Get transformer's Id

    DESCRIPTION
        
    RETURNS
        (XslTrIdType) Transformer's Id

    ARGUMENTS
        None
*/
      // virtual XslTrIdType getTransformerId() const = 0;

      virtual ~CompTransformer() {}

    protected:
    };

/**********************************************************************
       Interface Xsl_Compiler - Xsl compiler into binary representation
**********************************************************************/

    template< typename Context, typename Node> class Compiler {
    public:

/*---------------------------------------------------------------------
    NAME
        compile

    PURPOSE
        Compile XSLT and return its compiled binary representation.

    DESCRIPTION
        Compiles XSLT and returns its compiled binary representation

    RETURNS
        XSLT document in compiled binary representation

    ARGUMENTS
        isrc_ptr (IN) XSLT document
*/
      virtual ub2* compile( InputSource* isrc_ptr)
	throw (XslException) = 0;

/*---------------------------------------------------------------------
    NAME
        getLength

    PURPOSE
        Get length of compiled XSL document

    DESCRIPTION
        Returns length of compiled XSL document

    RETURNS
        (ub4) length of the document

    ARGUMENTS
        binxsl_ptr (IN) compiled XSL document
*/
      virtual ub4 getLength( ub2* binxsl_ptr)
	throw (XslException) = 0;

/*---------------------------------------------------------------------
    NAME
        getCompilerId

    PURPOSE
        Get compiler's Id

    DESCRIPTION
        Returns compiler's Id
        
    RETURNS
        (XslCompIdType) Compiler's Id

    ARGUMENTS
        None
*/
      virtual XslCompIdType getCompilerId() const = 0;

      virtual ~Compiler() {}

    protected:
    };

  }

/*---------------------------------------------------------------------
                   Package XPath - Package one-line doc

    XPath is the namespace (sub-space of OracleXml) for XPath
    processing related types and interfaces.

-------------------------------------------------------------------*/

  namespace XPath {

/* DATATYPE XPathExceptionCode - XPath related exception codes
    It defines XPath exception codes. XPATH_UNDEFINED_ERR refers to
    non-documented errors. XPATH_OTHER_ERR refers to errors documented
    outside of the C++ framework, currently in xmlerr.h only.
*/
    typedef enum XPathExceptionCode {
      XPATH_UNDEFINED_ERR       = 0,
      XPATH_OTHER_ERR           = 1
    } XPathExceptionCode;

/* DATATYPE XPathPrIdType - XPath processor identifiers
    It defines XPath processor identifiers
*/
    typedef enum XPathPrIdType {
      XPathPrCXml       = 1,
      XvmPrCXml         = 2
    } XPathPrIdType;

/* DATATYPE XPathCompIdType - XPath compiler identifiers
    It defines XPath compiler identifiers
*/
    typedef enum XPathCompIdType {
      XvmXPathCompCXml         = 1
    } XPathCompIdType;

/* DATATYPE XPathObjType - object types
    It defines XPath 1.0 object types
*/
    typedef enum XPathObjType {
      XPOBJ_TYPE_UNKNOWN = 0,
      XPOBJ_TYPE_NDSET   = 1,
      XPOBJ_TYPE_BOOL    = 2,
      XPOBJ_TYPE_NUM     = 3,
      XPOBJ_TYPE_STR     = 4
    } XPathObjType;

/**********************************************************************
  Interface XPathException - Exceptions for XPath compilers & processors
**********************************************************************/

    class XPathException : public XmlException {
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
        getXPathCode

    PURPOSE
        Get XPath exception code embedded in the exception 

    DESCRIPTION
        This is a virtual member function that defines a prototype
	for implementation defined member functions returning XPath
	processor and compiler exception codes, defined in
	XPathExceptionCode, of the exceptional situations during
	execution

    RETURNS
        (XPathExceptionCode) exception code

    ARGUMENTS
        None
*/
      virtual XPathExceptionCode getXPathCode() const = 0;
    };

/**********************************************************************
                 Interface NodeSet - Interface one-line doc
**********************************************************************/

    template< typename Node > class NodeSet {
    public:

      ~NodeSet();

/*---------------------------------------------------------------------
    NAME
        getSize

    PURPOSE
        Get node set size
 
    DESCRIPTION
        The size of the node set

    RETURNS
        (ub4) node set size

    ARGUMENTS
        None
*/
      ub4 getSize() const;

/*---------------------------------------------------------------------
    NAME
        getNode

    PURPOSE
        Get node given its index
 
    DESCRIPTION
        Returns a reference to the node

    RETURNS
        (NodeRef) reference to the node

    ARGUMENTS
        idx (IN) index of the node in the set
*/
      NodeRef< Node>* getNode( ub4 idx) const;

    private:
      // default constructor - should not be called publically
      NodeSet();
    };

/**********************************************************************
     Interface XPathObject -  interface for XPath 1.0 objects
**********************************************************************/

    template< typename Node> class XPathObject {
    public:

      ~XPathObject();

/*---------------------------------------------------------------------
    NAME
        XPathObject

    PURPOSE
        Copy constructor

    DESCRIPTION
        Copies the object

    RETURNS
        (XPathObject) new object

    ARGUMENTS
        (XPathObject&) referene to the object to be copied
*/
      XPathObject( XPathObject< Node>&);

/*---------------------------------------------------------------------
    NAME
        getObjType

    PURPOSE
        Get Xpath 1.0 object type

    DESCRIPTION
        Gets Xpath 1.0 object type

    RETURNS
        (XPathObjType) object type

    ARGUMENTS
        none
*/
      XPathObjType getObjType() const;

/*---------------------------------------------------------------------
    NAME
        getObjString

    PURPOSE
        Get the string value of the object

    DESCRIPTION
        Gets the string value of the object

    RETURNS
        (oratext*) string value

    ARGUMENTS
        none
*/
      oratext* getObjString() const;

/*---------------------------------------------------------------------
    NAME
        getObjNumber

    PURPOSE
        Get the number value of the object

    DESCRIPTION
        Gets the number value of the object

    RETURNS
        (double) number value

    ARGUMENTS
        none
*/
      double getObjNumber() const;

/*---------------------------------------------------------------------
    NAME
        getObjBoolean

    PURPOSE
        Get the boolean value of the object

    DESCRIPTION
        Gets the boolean value of the object

    RETURNS
        (boolean) boolean value

    ARGUMENTS
        none
*/
      boolean getObjBoolean() const;

/*---------------------------------------------------------------------
    NAME
        getNodeSet

    PURPOSE
        Get the node set value of the object

    DESCRIPTION
        Gets the node set value of the object

    RETURNS
        (NodeSet*) node set

    ARGUMENTS
        none
*/
      NodeSet< Node>* getNodeSet() const;

    protected:
      // Default constructor - never to be called by the user
      XPathObject() {}
    };

/**********************************************************************
            Interface XPath_Processor - basic XPath Processsor
**********************************************************************/

    template< typename Context, typename Node> class Processor {
    public:

/*---------------------------------------------------------------------
    NAME
        process

    PURPOSE
        Evaluate XPath expression against given document

    DESCRIPTION
        Evaluates XPath expression against given document and
	returns result XPath object
        
    RETURNS
        (XPathObject*) XPath object

    ARGUMENTS
        isrc_ptr  (IN) instance document to process
	xpath_exp (IN) XPath expression
*/
      virtual XPathObject< Node>*
      process( InputSource* isrc_ptr, oratext* xpath_exp)
	throw (XPathException) = 0;

/*---------------------------------------------------------------------
    NAME
        getProcessorId

    PURPOSE
        Get processor's Id

    DESCRIPTION
        Gets processor's Id

    RETURNS
        (XPathPrIdType) Processor's Id

    ARGUMENTS
        None
*/
      virtual XPathPrIdType getProcessorId() const = 0;

      virtual ~Processor() {}

    protected:
    };


/**********************************************************************
        Interface XPath_CompProcessor - Extended XPath Processor
**********************************************************************/

    template< typename Context, typename Node> class CompProcessor :
                                    public Processor< Context, Node>{
    public:

/*---------------------------------------------------------------------
    NAME
        process

    PURPOSE
        Evaluate XPath expression against given document

    DESCRIPTION
        Inherited from Processor
        
    RETURNS
        (XPathObject*) XPath object

    ARGUMENTS
        isrc_ptr  (IN) instance document to process
	xpath_exp (IN) XPath expression
*/
      // virtual XPathObject< Node>*
      // process( InputSource* isrc_ptr, oratext* xpath_exp)
      //   throw (XPathException) = 0;

/*---------------------------------------------------------------------
    NAME
        processWithBinXPath

    PURPOSE
        Evaluate compiled XPath expression against given document

    DESCRIPTION
        Evaluates compiled XPath expression against given document

    RETURNS
        (XPathGenObject*) XPath object

    ARGUMENTS
        isrc_ptr  (IN) instance document to process
	bin_xpath (IN) compiled xpath expression
*/
      virtual XPathObject< Node>*
      processWithBinXPath( InputSource* isrc_ptr, ub2* bin_xpath)
	throw (XPathException) = 0;

/*---------------------------------------------------------------------
    NAME
        getProcessorId

    PURPOSE
        Get processor's Id

    DESCRIPTION
        Gets processor's Id
        
    RETURNS
        (XPathPrIdType) Processor's Id

    ARGUMENTS
        None
*/
      // virtual XPathPrIdType getProcessorId() const = 0;

      virtual ~CompProcessor() {}

    };

/**********************************************************************
       Interface XPath_Compiler - XPath compiler into binary representation
**********************************************************************/

    template< typename Context, typename Node> class Compiler {
    public:

/*---------------------------------------------------------------------
    NAME
        compile

    PURPOSE
        Compile XPath and return its compiled binary representation.

    DESCRIPTION
        Compiles XPath and returns its compiled binary representation

    RETURNS
        (ub2) XPath expression in compiled binary representation

    ARGUMENTS
        xpath_exp    (IN) XPath expression
*/
      virtual ub2* compile( oratext* xpath_exp)
	throw (XPathException) = 0;

/*---------------------------------------------------------------------
    NAME
        getCompilerId

    PURPOSE
        Get compiler's Id

    DESCRIPTION
        Gets compiler's Id

    RETURNS
        (XPathCompIdType) Compiler's Id

    ARGUMENTS
        None
*/
      virtual XPathCompIdType getCompilerId() const = 0;

      virtual ~Compiler() {}

    };

  }

/*---------------------------------------------------------------------
                Package XPointer - Package one-line doc

    XPointer is the namespace (sub-space of OracleXml) for XPointer
    processing related types and interfaces.

-------------------------------------------------------------------*/

  namespace XPointer {

/* DATATYPE XppExceptionCode - XPointer related exception codes
    It defines XPointer exception codes
*/
    typedef enum XppExceptionCode {
      XPP_UNDEFINED_ERR       = 0,
      XPP_OTHER_ERR           = 1
    } XppExceptionCode;

/* DATATYPE XppPrIdType - XPointer processor identifiers
    It defines XPointer processor identifiers
*/
    typedef enum XppPrIdType {
      XPtrPrCXml         = 1
    } XppPrIdType;

/* DATATYPE XppLocType - XPointer location types
    It defines XPointer location types
*/
    typedef enum XppLocType {
      XPPLOC_TYPE_UNKNOWN = 0,
      XPPLOC_TYPE_NODE    = 1,
      XPPLOC_TYPE_POINT   = 2,
      XPPLOC_TYPE_RANGE   = 3,
      XPPLOC_TYPE_BOOL    = 4,
      XPPLOC_TYPE_NUM     = 5,
      XPPLOC_TYPE_STR     = 6
    } XppLocType;

/**********************************************************************
  Interface XppException - Exceptions for XPointer processors
**********************************************************************/

    class XppException : public XmlException {
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
        getXppCode

    PURPOSE
        Get XPointer exception code embedded in the exception 

    DESCRIPTION
        This is a virtual member function that defines a prototype
	for implementation defined member functions returning XPointer
	processor and compiler exception codes, defined in
	XppExceptionCode, of the exceptional situations during
	execution

    RETURNS
        (XppExceptionCode) exception code

    ARGUMENTS
        None
*/
      virtual XppExceptionCode getXppCode() const = 0;
    };

/**********************************************************************
     Interface XppLocation -  interface for XPointer locations
**********************************************************************/

    template< typename Node> class XppLocation {
    public:

      ~XppLocation();

/*---------------------------------------------------------------------
    NAME
        getLocType

    PURPOSE
        Get location type

    DESCRIPTION
        Gets location type

    RETURNS
        (XppLocType) location type

    ARGUMENTS
        None
*/
      XppLocType getLocType() const;

/*---------------------------------------------------------------------
    NAME
        getNode

    PURPOSE
        Get node location

    DESCRIPTION
        Gets node location

    RETURNS
        (Node*) pointer to the node

    ARGUMENTS
        None
*/
      Node* getNode() const;

/*---------------------------------------------------------------------
    NAME
        getRange

    PURPOSE
        Get range location

    DESCRIPTION
        Gets range location

    RETURNS
        (Range*) pointer to the range

    ARGUMENTS
        None
*/
      Range< Node>* getRange() const;

    protected:
      // Default constructor - never to be called by the user
      XppLocation() {}
    };

/**********************************************************************
              Interface XppLocSet - Location set interface
**********************************************************************/

    template< typename Node > class XppLocSet {
    public:

      ~XppLocSet();

/*---------------------------------------------------------------------
    NAME
        getSize

    PURPOSE
        Get location set size
 
    DESCRIPTION
        The size of the node set

    RETURNS
        (ub4) node set size

    ARGUMENTS
        None
*/
      ub4 getSize() const;

/*---------------------------------------------------------------------
    NAME
        getItem

    PURPOSE
        Get item given its index
 
    DESCRIPTION
        Returns a reference to the item

    RETURNS
        (XppLocation*) reference to the item

    ARGUMENTS
        None
*/
      XppLocation< Node>* getItem( ub4 index) const;

    private:
      // default constructor - should not be called publically
      XppLocSet();
    };

/**********************************************************************
       Interface XPointer_Processor - basic XPointer Processsor
**********************************************************************/

    template< typename Context, typename Node> class Processor {
    public:

      virtual ~Processor() { }

/*---------------------------------------------------------------------
    NAME
        process

    PURPOSE
        Evaluate XPointer expression against given document

    DESCRIPTION
        Evaluates XPointer expression against given document and
	returns result XPointer location set object
        
    RETURNS
        (XppLocSet*) XPath object

    ARGUMENTS
        isrc_ptr  (IN) instance document to process
	xpp_exp   (IN) XPointer expression
*/
      virtual XppLocSet< Node>* process (InputSource* isrc_ptr,
					    oratext* xpp_exp)
                                          throw (XppException) = 0;

/*---------------------------------------------------------------------
    NAME
        getProcessorId

    PURPOSE
        Get processor's Id

    DESCRIPTION
        Gets processor's Id

    RETURNS
        (XppPrIdType) Processor's Id

    ARGUMENTS
        None
*/
      virtual XppPrIdType getProcessorId() const = 0;

    };

  }

  using namespace Xsl;
  using namespace XPath;
  using namespace XPointer;

/*---------------------------------------------------------------------
                 Package Tools - Package one-line doc

    Tools is the namespace (sub-space of OracleXml) for types and
    interfaces related to the creation and instantiation of Oracle
    XML tools.

-------------------------------------------------------------------*/

  namespace Tools {

/* DATATYPE FactoryExceptionCode - Tool Factory exceptions
    It defines tool factory exception codes. FACTORY_UNDEFINED_ERR
    refers to non-documented errors. FACTORY_OTHER_ERR refers to
    errors documented outside of the C++ framework, currently in
    xmlerr.h only.
*/
    typedef enum FactoryExceptionCode {
      FACTORY_UNDEFINED_ERR               = 0,
      FACTORY_OTHER_ERR                   = 1
    } FactoryExceptionCode;

/**********************************************************************
     Interface FactoryException - Exceptions for Tools namespace
**********************************************************************/

    class FactoryException : public XmlException {
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
        getFactoryCode

    PURPOSE
        Get Factory exception code embedded in the exception 

    DESCRIPTION
        This is a virtual member function that defines a prototype
	for implementation defined member functions returning
	exception codes specific to the Tools namespace, defined
	in FactoryExceptionCode, of the exceptional situations during
	execution

    RETURNS
        (FactoryExceptionCode) exception code

    ARGUMENTS
        None
*/
      virtual FactoryExceptionCode getFactoryCode() const = 0;
    };

/**********************************************************************
            Interface Factory - XML tools factory
**********************************************************************/

    template< typename Context, typename Node> class Factory {
    public:

/*---------------------------------------------------------------------
    NAME
        Factory

    PURPOSE
        Default constructor

    DESCRIPTION
        Creates a factory object

    RETURNS
        (Factory) object

    ARGUMENTS
        none
*/
      Factory() throw (FactoryException);

/*---------------------------------------------------------------------
    NAME
        Factory

    PURPOSE
        Constructor

    DESCRIPTION
        Creates a factory object given a Context object

    RETURNS
        (Factory) object

    ARGUMENTS
        ctx_ptr (IN) pointer to a Context object
*/
      Factory( Context* ctx_ptr) throw (FactoryException);

/*---------------------------------------------------------------------
    NAME
        ~Factory

    PURPOSE
        Default destructor

    DESCRIPTION
        Destroys the factory object

    RETURNS
        (void)

    ARGUMENTS
        none
*/
      ~Factory();

/*---------------------------------------------------------------------
    NAME
        getContext

    PURPOSE
        Get factory's context

    DESCRIPTION
        Returns factory's context

    RETURNS
        (Context*) pointer to the contextobject

    ARGUMENTS
        none
*/
      Context* getContext() const;

/*---------------------------------------------------------------------
    NAME
        createDOMParser

    PURPOSE
        Create DOMParser

    DESCRIPTION
        Creates DOM parser
        
    RETURNS
        (DOMParser*) pointer to the parser object

    ARGUMENTS
        id_type (IN) parser Id type
        ctx_ptr (IN) pointer to a Context object
*/
      DOMParser< Context, Node>* createDOMParser (
	      DOMParserIdType id_type, Context* ctx_ptr = NULL)
                                          throw (FactoryException);

/*---------------------------------------------------------------------
    NAME
        createDOMWriter

    PURPOSE
        Create DOMWriter

    DESCRIPTION
        Creates DOM writer
        
    RETURNS
        (DOMWriter*) pointer to the writer object

    ARGUMENTS
        id_type (IN) parser Id type
*/
      DOMWriter< Node>* createDOMWriter( DOMWriterIdType id_type)
	throw (FactoryException);

/*---------------------------------------------------------------------
    NAME
        createSAXParser

    PURPOSE
        Create SAXParser

    DESCRIPTION
        Creates SAX parser
        
    RETURNS
        (SAXParser*) pointer to the parser object

    ARGUMENTS
        id_type (IN) parser Id type
        ctx_ptr (IN) pointer to a Context object
*/
      SAXParser< Context>* createSAXParser ( SAXParserIdType id_type,
				   Context* ctx_ptr = NULL)
                                          throw (FactoryException);

/*---------------------------------------------------------------------
    NAME
        createSchemaValidator

    PURPOSE
        Create schema validator

    DESCRIPTION
        Creates schema validator
        
    RETURNS
        (SchemaValidator*) pointer to the validator object

    ARGUMENTS
        id_type (IN) validator Id type
        ctx_ptr (IN) pointer to a Context object
*/
      SchemaValidator< Node>* createSchemaValidator (
		  SchValidatorIdType id_type, Context* ctx_ptr = NULL)
                                         throw (FactoryException);

/*---------------------------------------------------------------------
    NAME
        createXslTransformer

    PURPOSE
        Create XSL trnasformer

    DESCRIPTION
        Creates XSL trnasformer
        
    RETURNS
        (Trasformer*) pointer to the transformer object

    ARGUMENTS
        id_type (IN) transformer Id type
        ctx_ptr (IN) pointer to a Context object
*/
      Transformer< Context, Node>* createXslTransformer (
	      XslTrIdType id_type, Context* ctx_ptr = NULL)
                                          throw (FactoryException);

/*---------------------------------------------------------------------
    NAME
        createXslExtendedTransformer

    PURPOSE
        Create XSL extended trnasformer

    DESCRIPTION
        Creates XSL extended trnasformer; takes XvmTrCXml value only 
        
    RETURNS
        (CompTrasformer*) pointer to the transformer object

    ARGUMENTS
        id_type (IN) transformer Id type
        ctx_ptr (IN) pointer to a Context object
*/
      CompTransformer< Context, Node>* createXslExtendedTransformer (
	     XslTrIdType id_type, Context* ctx_ptr = NULL)
                                         throw (FactoryException);

/*---------------------------------------------------------------------
    NAME
        createXslCompiler

    PURPOSE
        Create Xsl compiler

    DESCRIPTION
        Creates Xsl compiler
        
    RETURNS
        (Compiler*) pointer to the compiler object

    ARGUMENTS
        id_type (IN) compiler Id type
        ctx_ptr (IN) pointer to a Context object
*/
      Xsl::Compiler< Context, Node>* createXslCompiler (
		XslCompIdType id_type, Context* ctx_ptr = NULL)
                                          throw (FactoryException);

/*---------------------------------------------------------------------
    NAME
        createXPathProcessor

    PURPOSE
        Create XPath processor

    DESCRIPTION
        Creates XPath processor
        
    RETURNS
        (Processor*) pointer to the processor object

    ARGUMENTS
        id_type (IN) processor Id type
        ctx_ptr (IN) pointer to a Context object
*/
      XPath::Processor< Context, Node>* createXPathProcessor (
	      XPathPrIdType id_type, Context* ctx_ptr = NULL)
                                          throw (FactoryException);

/*---------------------------------------------------------------------
    NAME
        createXPathCompProcessor

    PURPOSE
        Create extended XPath processor

    DESCRIPTION
        Creates extended XPath processor; takes XvmPrCXml value only
        
    RETURNS
        (CompProcessor*) pointer to the processor object

    ARGUMENTS
        id_type (IN) processor Id type
        ctx_ptr (IN) pointer to a Context object
*/
      CompProcessor< Context, Node>* createXPathCompProcessor (
                 XPathPrIdType id_type, Context* ctx_ptr = NULL)
                                          throw (FactoryException);

/*---------------------------------------------------------------------
    NAME
        createXPathCompiler

    PURPOSE
        Create XPath compiler

    DESCRIPTION
        Creates XPath compiler
        
    RETURNS
        (XPathCompiler*) pointer to the compiler object

    ARGUMENTS
        id_type (IN) compiler Id type
        ctx_ptr (IN) pointer to a Context object
*/
      XPath::Compiler< Context, Node>* createXPathCompiler (
               XPathCompIdType id_type, Context* ctx_ptr = NULL)
                                          throw (FactoryException);


/*---------------------------------------------------------------------
    NAME
        createXPointerProcessor

    PURPOSE
        Create XPointer processor

    DESCRIPTION
        Creates XPointer processor
        
    RETURNS
        (Processor*) pointer to the processor object

    ARGUMENTS
        id_type (IN) processor Id type
        ctx_ptr (IN) pointer to a Context object
*/
      XPointer::Processor< Context, Node>* createXPointerProcessor (
	      XppPrIdType id_type, Context* ctx_ptr = NULL)
                                          throw (FactoryException);

      private:
       void*   ctxp;
       boolean delctx;

    };
  }

  using namespace Tools;

};

using namespace OracleXml;

#endif
