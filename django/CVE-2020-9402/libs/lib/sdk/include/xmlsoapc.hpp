////////////////////////////////////////////////////////////////////////
// Copyright (c) 2004, 2005, Oracle. All rights reserved.  
//
// NAME
//   xmlsoapc.hpp - header of C xmlctx and xmlnode based implementation
//                  of SOAP
//                 
//
// DESCRIPTION
//   
//
////////////////////////////////////////////////////////////////////////

#ifndef XMLSOAPC_CPP_ORACLE
#define XMLSOAPC_CPP_ORACLE

extern "C" {

#ifndef XMLSOAP_ORACLE
#include <xmlsoap.h>
#endif
}

#ifndef XMLSOAP_CPP_ORACLE
#include <xmlsoap.hpp>
#endif

#ifndef XMLD_CPP_ORACLE
#include <xmld.hpp>
#endif

namespace OracleXml {

  namespace Soap {

    class SoapExceptionCImpl : public SoapException {
    public:

      // Constructors
      SoapExceptionCImpl( SoapExceptionCode Soap_code, unsigned xdk_code)
      {
	SoapCode = Soap_code;
	xdkCode = xdk_code;
	xdkMessage = NULL;
	xdkMesLang = (oratext*)"American";
      }

      SoapExceptionCImpl( SoapExceptionCode Soap_code, unsigned xdk_code,
			  oratext* Soap_msg)
      {
	SoapCode = Soap_code;
	xdkCode = xdk_code;
	xdkMessage = Soap_msg;
	xdkMesLang = (oratext*)"American";
      }

      SoapExceptionCImpl( SoapExceptionCode Soap_code, XmlException* excp)
      {
	SoapCode = Soap_code;
	xdkCode = excp->getCode();
	xdkMessage = excp->getMessage();
	xdkMesLang = excp->getMesLang();
      }

      // member functions

      unsigned getCode() const { return xdkCode; }
      oratext* getMessage() const { return xdkMessage; }
      oratext* getMesLang() const { return xdkMesLang; }

      SoapExceptionCode getSoapCode() const { return SoapCode; }

    private:
      // Default constructor
      SoapExceptionCImpl();

      SoapExceptionCode SoapCode;
      unsigned         xdkCode;
      oratext*         xdkMessage;
      oratext*         xdkMesLang;
    };

    template<> class ConnectRef < xmlnode> {
    public:
      ConnectRef( xmlsoapctx* soapctx_ptr, xmlsoapcon* soapcon_ptr) {
	soapctxp = soapctx_ptr, soapconp = soapcon_ptr; }

      ~ConnectRef() throw (SoapException); 

      DocumentRef< xmlnode>* call( DocumentRef< xmlnode>& msg)
	throw (SoapExceptionCImpl);

      xmlsoapcon* getConPtr() { return soapconp; }

      xmlsoapctx* soapctxp;
      xmlsoapcon* soapconp;
    };

    template<> class MsgFactory< CXmlCtx, xmlnode> {
    public:

      MsgFactory( CXmlCtx* ctx_ptr) throw (SoapExceptionCImpl);
      ~MsgFactory() throw (SoapExceptionCImpl);

      DocumentRef< xmlnode>* CreateMessage() throw (SoapExceptionCImpl);
      void destroyMessage(  DocumentRef< xmlnode>& msg)
	throw (SoapExceptionCImpl);
      ConnectRef< xmlnode>*
      createConnection( SoapBinding bind, void *endp, oratext *buf,
			ubig_ora bufsiz, oratext *msgbuf,
			ubig_ora msgbufsiz) throw (SoapExceptionCImpl);
      /*
      void setProperty( DocumentRef< xmlnode>& msg, oratext *name,
			oratext *value) throw (SoapExceptionCImpl);
      oratext* getProperty( DocumentRef< xmlnode>& msg, oratext *name)
	throw (SoapExceptionCImpl);
      xmlnode* getPart( DocumentRef<xmlnode>& msg)
	throw (SoapExceptionCImpl);
      oratext* getAttachment( DocumentRef< xmlnode>& msg, size_t num)
	throw (SoapExceptionCImpl);
      void addAttachment( DocumentRef< xmlnode>& msg, oratext *att)
	throw (SoapExceptionCImpl);
      */
      xmlnode* getEnvelope( DocumentRef<xmlnode>& msg)
	throw (SoapExceptionCImpl);
      xmlnode* getHeader( DocumentRef< xmlnode>& msg)
	throw (SoapExceptionCImpl);
      xmlnode* getBody( DocumentRef< xmlnode>& msg)
	throw (SoapExceptionCImpl);
      xmlnode* addHeaderElement( DocumentRef< xmlnode>& msg,
					  oratext *qname, oratext *uri);
      xmlnode* getHeaderElement( DocumentRef< xmlnode>& msg,
					  oratext *uri, oratext *local)
	throw (SoapExceptionCImpl);
      xmlnode* addBodyElement( DocumentRef< xmlnode>& msg,
					oratext *qname, oratext *uri); 
      xmlnode* getBodyElement( DocumentRef< xmlnode>& msg,
					oratext *uri, oratext *local)
	throw (SoapExceptionCImpl);
      void setRole( ElementRef< xmlnode>& elem, SoapRole role)
	throw (SoapException);
      SoapRole getRole( ElementRef< xmlnode>& elem)
	throw (SoapException);
      boolean hasFault( DocumentRef< xmlnode>& msg)
	throw (SoapException);
      void setFault( DocumentRef< xmlnode>& msg, oratext *node,
		     oratext *code, oratext *reason, oratext *lang,
		     oratext *role, ElementRef< xmlnode>& detail)
	throw (SoapException);
      void addFaultReason( DocumentRef< xmlnode>& msg,
			   oratext *reason, oratext *lang)
	throw (SoapException);
      xmlnode* getFault( DocumentRef< xmlnode>& msg, oratext **code,
			 oratext **reason, oratext **lang,
			 oratext **node, oratext **role)
	throw (SoapException);
      ub4 getReasonNum( DocumentRef< xmlnode>& msg);
      oratext* getreasonLang( DocumentRef< xmlnode>& msg, ub4 idx)
	throw (SoapException);
      void setMustUnderstand( ElementRef< xmlnode>& elem,
			      boolean mustUnderstand)
	throw (SoapException);
      boolean getMustUnderstand( ElementRef< xmlnode>& elem)
	throw (SoapException);

    private:
      xmlsoapctx* soapctxp;
      CXmlCtx* ctxp;
    };

  }

  using namespace Soap;

}

using namespace OracleXml;

#endif
