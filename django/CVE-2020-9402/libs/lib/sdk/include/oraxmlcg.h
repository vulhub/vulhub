#ifndef LINT /* don't try to olint this file, it can't hack C++ */

// Copyright (c) 1999, 2003, Oracle.  All rights reserved.  

//////////////////////////////////////////////////////////////////////////////
// NAME
//   lgx.h
//
// DESCRIPTION
//   Header for C++ Class Generator
//
// PUBLIC CLASSES(S)
//   XMLClassGenerator [generate]
//////////////////////////////////////////////////////////////////////////////

#ifndef LGX_ORACLE
# define LGX_ORACLE

extern "C" {

#ifndef ORATYPES
# include <oratypes.h>
#endif

}

#ifndef ORAXML_CPP_ORACLE
# include <oraxml.hpp>
#endif

#ifndef ORAXSD_CPP_ORACLE
# include <oraxsd.hpp>
#endif

// Error codes

#define LGX_SUCC        0
#define LGX_ERR         1

// Declare generator class

class XMLClassGenerator
{
    public:
        oratext*version(void);
        uword   generate(DocumentType *dtd, DOMString outdir = 0);
        uword   generate(Schema *schema, DOMString name, DOMString outdir = 0);
};

#endif // LGX_ORACLE

#endif /* ifndef LINT */
