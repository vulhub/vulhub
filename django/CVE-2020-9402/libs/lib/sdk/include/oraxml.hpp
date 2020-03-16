#ifndef LINT /* don't try to olint this file, it can't hack C++ */

//////////////////////////////////////////////////////////////////////////////
// Copyright (c) 1999, 2018, Oracle and/or its affiliates. All rights reserved.
//
// NAME
//   oradom.h - Header for C++ DOM interface to C XML parser
//
// DESCRIPTION
//   C++ DOM interface
//
//////////////////////////////////////////////////////////////////////////////

#ifndef ORAXML_CPP_ORACLE
# define ORAXML_CPP_ORACLE

extern "C" {

#include <stdio.h>

#ifndef ORASTRUC
# include <orastruc.h>
#endif

#ifndef ORAXML_ORACLE
# include <oraxml.h>
#endif

}

// DOM standard string types.  Note that "String" was introduced in error
// and we should not be using it.  We'll drop out own usage of it but still
// maintain the typedef for now (in case the user is using it), then drop
// the typedef later when the user has had time to change over.

typedef oratext*        DOMString;
typedef oratext*        String;

class Node;             // forward references
class Element;
class Document;
class DocumentType;

class XMLParser
{
    public:
        uword			xmlinit(DOMString encoding = 0,
				        XML_MSGHDLRF((*msghdlr)) = 0,
					void *msgctx = 0, xmlsaxcb *saxcb = 0,
					void *saxcbctx = 0, DOMString lang = 0);
        uword			xmlinitenc(DOMString incoding = 0,
					DOMString outcoding = 0,
				        XML_MSGHDLRF((*msghdlr)) = 0,
					void *msgctx = 0, xmlsaxcb *saxcb = 0,
					void *saxcbctx = 0, DOMString lang = 0);
        void			xmlterm();
        uword			xmlparse(DOMString doc, DOMString encoding,
					ub4 flags);
        uword			xmlparseDTD(DOMString doc, DOMString name,
					    DOMString encoding, ub4 flags);
	uword			xmlparseBuffer(DOMString buffer, size_t len,
					       DOMString encoding, ub4 flags);
	uword			xmlparseFile(DOMString path, size_t len,
					     DOMString encoding, ub4 flags);
	uword			xmlparseStream(DOMString path, void *stream,
					     DOMString encoding, ub4 flags);
	uword			xmlparseStream(DOMString path, orastream *os,
					     DOMString encoding, ub4 flags);
	void			xmlclean();
	boolean			xmlwhere(ub4 *line, DOMString *path,
					 uword idx);
	uword			setAccess(xmlacctype access, xml_openf openf,
					  xml_closef closef, xml_readf readf);
	Node*			getContent(Node *node);
        Document*		getDocument();
        Element*		getDocumentElement();
        DocumentType*		getDocType();
        DOMString	        getEncoding();
        boolean			isStandalone();
        boolean		        isSingleChar();
	uword			validate(Node *root);
	xmlctx*			context();

	// Needed to resolve circular createDocument/getImplementation
	// circularity.
	Document*		createDocument(DOMString uri, DOMString qname,
						DocumentType *dtd);

//    private:
        xmlctx      *ctx;		// XML (LPX) context
};

// Since xslprocess creates a DOM document fragment node as its result, 
// it doesn't support xsl:output.
class XSLProcessor
{
    public:
        uword        xslprocess(XMLParser *xmltree, XMLParser *stylesheet,
                                XMLParser *resulttree, Node **result);
};

class XPObject
{
    private:
       xpobj *xobj;
 
    public:
        XPObject(xpobj *xo);

        xpobjtyp  getxpobjtyp();
        boolean   getbooleanval();
        double    getnumval();
        oratext*  getstrval();
        xpnset*   getnsetval();
};

class XPath
{
    private:
        xpctx  *xctx;

    public:
        XPath(xmlctx *ctx, Node *xslnode, Node *xml_node,
	      oratext *baseURI, size_t nctxels, Node **ctxnodes);
        ~XPath();

        xpexpr*   parsexpathexpr(oratext *expr, sword *err);
        XPObject* evalxpathexpr(xpexpr *exprtree, sword *err);
};

class NamedNodeMap
{
    public:
        Node*        item(size_t index);
        size_t       getLength();
	Node*	     getNamedItem(DOMString name);
	boolean	     setNamedItem(Node *node, Node **old);
	Node*	     removeNamedItem(DOMString name);
	void         free();
};

class NodeList
{
    public:
        Node*        item(size_t index);
        size_t       getLength();
	void         free();
};

class ListLink
{
    public:
	ListLink *next;		// next item in list
	ListLink *previous;	// previous item in list
	union {
	    void  *object;	// pointer to obj or 0 if builtin
	    ub4    size;	// size of user part if mem block
	};
};

// Work around 9.5.2 anonymous union problem on AIX platforms

typedef struct {
    void          *master;	// master definition
    NamedNodeMap  *attributes;	// ELEM's attributes
    union {
	NodeList  *children;	// node's children
	DOMString  value;	// U: node's data
    };
} NodeU1;

typedef struct {
    DOMString pubid;		// U: PubID for notations
    DOMString sysid;		// U: SysID for DTD/notation
    NodeList *children2;	// DTD's children
} NodeU2;

class Node
{
    protected:
	ListLink    glue;	// linked-list glue
	void	   *ctx;	// context backpointer
	ub2	    flags;	// mask of flag bits
	ub1         type;	// element, CData, etc (ub1)
	ub1	    loff;	// qname offset of local part
	DOMString   name;	// U: element's (q)name
	DOMString   uri;	// U: namespace URI of this node
	Node       *parent;     // up-pointer in tree

	union {
	    NodeU1 s1;		// All node types use one triplet
	    NodeU2 s2;		// or the other.  Compress node size.
	};

        void     *pfns;		// U: default nsp, prefixes/URIs
	void	 *loc;		// URL/entity source information
	ub4       linenum;	// source line number
	ub4	  xslid;	// ID of DocOrderList
        void     *xsdtype;

    public:
	~Node();			// destructor

	xmlctx*      getContext();
        DOMString    getName();
        DOMString    getQualifiedName();
        DOMString    getPrefix();
        DOMString    getLocal();
        DOMString    getNamespace();
        DOMString    getValue();
	void	     setValue(DOMString data);
        short        getType();
        Node*        getParentNode();
        boolean      hasChildNodes();
        NodeList*    getChildNodes();
        size_t       numChildNodes();
        Node*        getChildNode(uword index);
        Node*        getFirstChild();
        Node*        getLastChild();
	Node*	     getPreviousSibling();
	Node*	     getNextSibling();
        boolean      hasAttributes();
        NamedNodeMap*getAttributes();
	Document*    getOwnerDocument();
	Node*	     insertBefore(Node *newChild, Node *refChild);
	Node*	     replaceChild(Node *newChild);
	Node*	     removeChild();
	Node*	     appendChild(Node *newChild);
	Node*	     cloneNode(boolean deep);
	size_t	     print(FILE *out = stdout, uword level = 0,
			   uword step = 4, DOMString encoding = NULL,
			   ub4 flags = 0);
	size_t	     print(DOMString buffer, size_t bufsize,
			    uword level = 0, uword step = 4,
			    DOMString encoding = NULL, ub4 flags = 0);
	size_t	     printSize(uword level = 0, uword step = 4,
			    DOMString encoding = NULL, ub4 flags = 0);
};

class Attr : public Node
{
    public:
	DOMString    getName();
	DOMString    getValue();
        boolean      getSpecified();
        void         setValue(DOMString value);
};

class DocumentType : public Node
{
    public:
        DOMString     getName();
	NamedNodeMap* getEntities();
	NamedNodeMap* getNotations();
};

class CharacterData : public Node
{
    public:
	DOMString	getData();
	size_t		getLength();
	void		setData(DOMString data);
	DOMString	substringData(unsigned long offset,
                                      unsigned long count);
	void		appendData(DOMString arg);
	void		insertData(unsigned long offset, DOMString arg);
	void		deleteData(unsigned long offset,
                                   unsigned long count);
	void		replaceData(unsigned long offset,
                                    unsigned long count, DOMString arg);
};

class Comment : public CharacterData
{
};

class Text : public CharacterData
{
    public:
        Node* splitText(unsigned long offset);
};

class CDATASection : public Text
{
};

class DOMImplementation
{
    protected:
        xmlctx  *ctx;

    public:
        boolean 	hasFeature(DOMString feature, DOMString version);
	Document*	createDocument(DOMString uri, DOMString qname,
				       DocumentType *dtd);
	DocumentType*	createDocumentType(DOMString qname, DOMString pubid,
					   DOMString sysid);
};

class DocumentFragment : public Node
{
};

class Notation : public Node
{
    public:
	DOMString getPublicId();
	DOMString getSystemId();
};

class Element : public Node
{
    public:
        DOMString getTagName();
        DOMString getAttribute(DOMString name);
        Attr*     setAttribute(DOMString name, DOMString value);
        void      removeAttribute(DOMString name);
        Attr*     getAttributeNode(DOMString name);
        boolean   setAttributeNode(Attr* newAttr, Attr** oldAttr);
        Attr*     removeAttributeNode(Attr* oldAttr);
        NodeList* getElementsByTagName(DOMString name);
        NodeList* getElementsByTagNameNS(DOMString nspuri, DOMString local);
        void      normalize(void);
	uword	  initialize(Document *doc, DOMString nspuri, DOMString qname);
};

class Entity : public Node
{
    public:
	DOMString getPublicId();
        DOMString getSystemId();
        DOMString getNotationName();
};

class EntityReference : public Node
{
};

class ProcessingInstruction : public Node
{
    public:
        DOMString  getTarget();
        DOMString  getData();
        void       setData(DOMString data);
};

class Document : public Node
{
    public:
	Document();		// constructor

        DOMImplementation*    getImplementation();
        Element*              createElement(DOMString tagName);
	Element*              createElementNS(DOMString nspuri,
					      DOMString qname);
        DocumentFragment*     createDocumentFragment(void);
        Text*                 createTextNode(DOMString data);
        Comment*              createComment(DOMString data);
        CDATASection*         createCDATASection(DOMString data);
        ProcessingInstruction*createProcessingInstruction(DOMString target,
                                        DOMString data);
        Attr*                 createAttribute(DOMString name, DOMString value);
        Attr*                 createAttributeNS(DOMString nspuri,
					DOMString qname, DOMString value);
        EntityReference*      createEntityReference(DOMString name);
        NodeList*             getElementsByTagName(Element *elem,
					DOMString tagname);
        NodeList*             getElementsByTagNameNS(Element *elem,
					DOMString nspuri, DOMString local);
        Element*              getElementByID(DOMString name);
	DOMString	      saveString(DOMString str);
	Node*                 importNode(Node *importedNode, boolean deep);
};

#endif // ORAXML_CPP_ORACLE

#endif /* ifndef LINT */
