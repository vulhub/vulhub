////////////////////////////////////////////////////////////////////////
// Copyright (c) 2003, 2018, Oracle and/or its affiliates. All rights reserved.
//
// NAME
//   xmlctx.hpp - Header for C, xmlctx based implementation of TCtx
//                 
//
// DESCRIPTION
//   
//
////////////////////////////////////////////////////////////////////////

#ifndef XMLCTX_CPP_ORACLE
#define XMLCTX_CPP_ORACLE

extern "C" {

/* xml.h is the main C header that this implementation relies on
*/

#ifndef XML_ORACLE
#include <xml.h>
#endif
}

#ifndef XML_CPP_ORACLE
#include <xml.hpp>
#endif

namespace XmlCtxNS {

  void ProcessError( void* objp, oratext* msg, xmlerr err);
  void* AllocMem( void* obj, ub4 size);
  void FreeMem( void* obj, void* ptr);

  class XmlExceptionCImpl : public XmlException {
  public:
    // Constructors

    XmlExceptionCImpl( unsigned xdk_code)
    { xdkCode = xdk_code; xdkMessage = NULL;
      xdkMesLang = (oratext*)"American"; }

    XmlExceptionCImpl( unsigned xdk_code, oratext* xdk_mes)
    { xdkCode = xdk_code; xdkMessage = xdk_mes;
      xdkMesLang = (oratext*)"American"; }

    XmlExceptionCImpl( unsigned xdk_code, oratext* xdk_mes, oratext* xdk_lang)
    { xdkCode = xdk_code; xdkMessage = xdk_mes; xdkMesLang = xdk_lang; }

    // member functions

    unsigned getCode() const { return xdkCode; }
    oratext* getMessage() const { return xdkMessage; }
    oratext* getMesLang() const { return xdkMesLang; }

  private:
    // Default constructor
    XmlExceptionCImpl()
    { xdkCode = 0; xdkMessage = NULL; xdkMesLang = (oratext*)"American"; }

    unsigned         xdkCode;
    oratext*         xdkMessage;
    oratext*         xdkMesLang;
  };

  class CXmlAllocator : public MemAllocator {
  public:
    void* alloc( ub4 size);
    void  dealloc( void* ptr);
    ~CXmlAllocator() {}
  };

  class CXmlCtx {
  public:
    CXmlCtx() throw (XmlException);
    CXmlCtx( bool no_ri_open) throw (XmlException);
    CXmlCtx( oratext* name, ErrorHandler* errh_ptr = NULL,
	     MemAllocator* mem_ptr = NULL, encodings* encs_ptr = NULL)
                                                    throw( XmlException);
    CXmlCtx( oratext* name, ub4 inpblksize, ErrorHandler* errh_ptr = NULL,
	     MemAllocator* mem_ptr = NULL, encodings* encs_ptr = NULL)
                                                    throw( XmlException);
    CXmlCtx( xmlctx* cctxp);
    ~CXmlCtx();

    boolean isSimple() const;
    boolean isUnicode() const;
    oratext* getEncoding() const;
    ErrorHandler* getErrHandler() const;
    MemAllocator* getMemAllocator() const;

    // This implementation special

    xmlctx* getCCtx() const { return xctxp; }

  private:
    xmlctx* xctxp;
    ErrorHandler* errhp;
    MemAllocator* memp;
  };

  void getcreatepars( void** pars, ErrorHandler* errh_ptr,
		    MemAllocator* mem_ptr, encodings* encs_ptr);

};

using namespace XmlCtxNS;

namespace OracleXml {

namespace Dom {

  // internal data

  template<> class DOMImplData< xmlnode> {
  protected:
    DOMImplData() {}
    ~DOMImplData() {} 

    void* docs;
    boolean nomod;
  };

  template<> class NodeData< xmlnode> {
  protected:
    NodeData() {}
    ~NodeData() {}

    CXmlCtx* ctxp;
    DOMImplData< xmlnode>* implp;
    xmlnode* nodep;
    boolean delnode;
  };

  template<> class NodeListData< xmlnode> {
  protected:
    NodeListData() {}
    ~NodeListData() {}

    xmlnodelist* listp;
    CXmlCtx* ctxp;
    DOMImplData< xmlnode>* implp;
    boolean dellist;
  };

  template<> class NamedNodeMapData< xmlnode> {
  protected:
    NamedNodeMapData() {}
    ~NamedNodeMapData() {}

    xmlnamedmap* mapp;
    CXmlCtx* ctxp;
    DOMImplData< xmlnode>* implp;
    boolean delmap;
  };

}

using namespace Dom;

};

using namespace OracleXml;

#endif

