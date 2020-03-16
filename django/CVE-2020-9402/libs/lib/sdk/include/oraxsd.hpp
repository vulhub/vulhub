#ifndef LINT /* don't try to olint this file, it can't hack C++ */

// $RCSfile: $
// $Date: $
// $Revision: $

// Copyright (c) 1999, 2004, Oracle. All rights reserved.  

//////////////////////////////////////////////////////////////////////////////
// NAME
//   oraxsd.hpp - Header for C++ interface to C XML Schema processor
//
// DESCRIPTION
//   C++ Schema interface
//////////////////////////////////////////////////////////////////////////////

#ifndef ORAXSD_CPP_ORACLE
# define ORAXSD_CPP_ORACLE

extern "C" {

#ifndef ORATYPES
# include <oratypes.h>
#endif

#ifndef ORAXSD_ORACLE
# include <oraxsd.h>
#endif

}

#ifndef ORAXML_CPP_ORACLE
# include <oraxml.hpp>
#endif

typedef xsd Schema;

class XMLSchema
{
    public:
        uword    clean(void); 
        uword    initialize(XMLParser *parser);
        uword    initialize(XMLParser *parser, ub4 flags);
        uword    setErrHandler(XML_MSGHDLRF((*msghdlr)), void *msgctx);
        uword    xmlwhere(ub4 *line, oratext **path);
	uword    load(oratext *uri, oratext *nsp, Schema **schema);
	uword    load(oratext *uri, oratext *nsp, Schema **schema, ub4 flags);
        uword    validate(Element *root, oratext *url);
        oratext *target(Schema *schema);

        void     terminate(void);

    private:
	xsdctx *ctx;
};

#endif // ORAXSD_CPP_ORACLE

#endif /* ifndef LINT */
