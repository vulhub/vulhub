/* $RCSfile: xmlxvm.h $ $Date: 2017/12/04 19:06:51 $
 Copyright (c) 2001, 2017, Oracle and/or its affiliates. All rights reserved.

   FILE NAME
        xmlxvm.h - Oracle XML Virtual Machine APIs

   FILE DESCRIPTION
        This file defines the XSLT and XPath compiler and virtual machine
        interfaces.
*/


#ifndef XMLXVM_ORACLE
# define XMLXVM_ORACLE

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

#ifndef XMLERR_ORACLE
# include <xmlerr.h>
#endif

#ifndef XMLPROC_ORACLE
# include <xmlproc.h>
#endif

#ifndef XMLXSL_ORACLE
# include <xmlxsl.h>
#endif 

/*---------------------------------------------------------------------------
                   Package XSLTVM - XSLT Virtual Machine APIs

   XSLTVM Package implements the XSL Transformation (XSLT) language as  
   specified in W3C Recommendation 16 November 1999.  XSLT Virtual Machine is
   the software implementation of a "CPU" designed to run compiled XSLT code. 
   A concept of virtual machine assumes a compiler compiling XSLT stylesheets
   to a sequence of byte codes or machine instructions for the "XSLT CPU". 
   The byte-code program is a platform-independent sequence of 2-byte units.
   It can be stored, cashed and run on different XSLTVM.  The XSLTVM uses 
   the bytecode programs to transform instance XML documents.  This approach
   clearly separates compile(design)-time from run-time computations and 
   specifies a uniform way of exchanging data between instructions. 
     
   A typical scenario of using the package APIs has the following steps:

       (1) Create/Use an XML meta context object.
       xctx = XmlCreate(&err,...);

       (2) Create/Use an XSLT Compiler object.
       comp = XmlXvmCreateComp(xctx);

       (3) Compile an XSLT stylesheets and store/cash the result bytecode.
       code = XmlXvmCompileFile(comp, xslFile, baseuri, flags, &err);

       (4) Create/Use an XSLTVM object. The explicit stack size seting are
           needed when XSLTVM terminates with "... Stack Overflow" message
           or when smaller memory footprints are required (see XmlXvmCreate).
       vm = XmlXvmCreate(xctx, "StringStack", 32, "NodeStack", 24, NULL);

       (5) Set a stylesheet bytecode to the XSLTVM object.
       len = XmlXvmGetBytecodeLength(code, &err);
       err = XmlXvmSetBytecodeBuffer(vm, code, len);

       (6) Transform an instance XML document.
       err = XmlXvmTransformFile(vm, xmlFile, baseuri);

       (7) Clean.
        XmlXvmDestroy(vm);
        XmlXvmDestroyComp(comp);
        XmlDestroy(xctx);

  ---------------------------------------------------------------------------*/

/* DATATYPE xmlxvmcomp - The XSLTVM Compiler.
   SEE ALSO XmlXvmCreateComp, XmlXvmDestroyComp
    An object of type 'xmlxvmcomp' is used for compiling XSL stylesheets.
    Note: The contents of 'xmlxvmcomp' are private and must not be accessed by
    users.
*/
struct xmlxvmcomp; typedef struct xmlxvmcomp xmlxvmcomp;

/* DATATYPE xmlxvm - The XSLT Virtual Machine.
   SEE ALSO XmlXvmCreate, XmlXvmDestroy
    An object of type 'xmlxvm' is used for XML documents transformation.
    Note: The contents of 'xmlxvm' are private and must not be accessed by
    users.
*/
struct xmlxvm; typedef struct xmlxvm xmlxvm;

/* DATATYPE xvmobj - XSLVM processor run-time object. 
   SEE ALSO XmlXvmEvaluateXPath
    Note: The contents of 'xvmobj' are private and must not be accessed
    by users.
*/
struct xvmobj; typedef struct xvmobj xvmobj;

/* DATATYPE xmlxvmobjtype - XSL processor object type
   SEE ALSO XmlXvmGetObjectType
   Type of XSLTVM objects 
*/
typedef enum xmlxvmobjtype {
    XMLXVM_TYPE_UNKNOWN = 0,
    XMLXVM_TYPE_NDSET   = 1,
    XMLXVM_TYPE_BOOL    = 2,
    XMLXVM_TYPE_NUM     = 3,
    XMLXVM_TYPE_STR     = 4,
    XMLXVM_TYPE_FRAG    = 5
} xmlxvmobjtype;

/* DATATYPE xmlxvmflag - XSLT Compiler flags
   SEE ALSO XmlXvmCompileFile
   Control flags for the XSLT compiler
      XMLXVM_DEBUG - forces compiler to insert debug info into the bytecode.
      XMLXVM_STRIPSPACE - same as xsl:strip-space elements="*" 
      XMLXVM_INC_ANC_NSPFX - traverse DOM for obtaining NS prefixes in 
                             ancestor scope
      XMLXVM_NO_INDENT - output does not indent by default
*/
typedef ub4 xmlxvmflag;
#define XMLXVM_NOFLAG        0x00
#define XMLXVM_DEBUG         0x01 /* insert debug info into bytecode */
#define XMLXVM_STRIPSPACE    0x02 /* same as xsl:strip-space elements="*" */
#define XMLXVM_INC_ANC_NSPFX 0x04
#define XMLXVM_NO_INDENT     0x08

/****************************************************************************
                               Interface XSLTC
 ****************************************************************************/

/*---------------------------------------------------------------------------
    NAME
        XmlXvmCreateComp 

    PURPOSE
        Create an XSLT compiler

    DESCRIPTION
        Create an XSLT compiler object. The XSLT compiler is used to compile
        XSLT stylesheets into bytecode.

    RETURNS
        (xmlxvmcomp *) XSLT compiler object, or NULL on error

    ARGUMENTS
        xctx (IN) XML context

    SEE ALSO
        XmlXvmDestroyComp

    EXAMPLE
        comp = XmlXvmCreateComp(xctx);
        code = XmlXvmCompileFile(comp, xslFile1, baseuri, 0, &err);
        code = XmlXvmCompileFile(comp, xslFile2, baseuri, 0, &err);
        ...
        -- the following is an example of bytecode instructions sequence:
        ...
        14.           <xsl:apply-templates select="@* | node()"/>
    39    PUSHCUR          
    40    ATTRIBUTE        node()       node()       
    43    PUSHCUR                                        
    44    CHILD            node()       node()       
    47    UNION            
    48    PUSHCTX          [loop][context]
    49    FOREACH          code: @-8    
    51    PUSHCUR          
    52    MATCH             0            
    54    CALLTEMPLATE      
    55    BRA              code: @6     
    57    TESTCURNDTYPE    1            
    59    BNO              code: @-3   
        ...

*/
xmlxvmcomp *XmlXvmCreateComp(xmlctx *xctx);

/*---------------------------------------------------------------------------
    NAME
        XmlXvmDestroyComp

    PURPOSE
        Destroys an XSLT compiler object

    DESCRIPTION
        Destroys an XSLT compiler object

    RETURNS
        (void)

    ARGUMENTS
        comp (IN) XSLT compiler object

   SEE ALSO
        XmlXvmCreateComp
*/
void XmlXvmDestroyComp(xmlxvmcomp *comp);

/*---------------------------------------------------------------------------
    NAME
        XmlXvmResetComp

    PURPOSE
        Resets an XSLT compiler object

    DESCRIPTION
        Resets an XSLT compiler object

    RETURNS
        (void)

    ARGUMENTS
        comp (IN) XSLT compiler object

   SEE ALSO
        XmlXvmCreateComp
*/
void XmlXvmResetComp(xmlxvmcomp *comp);

/*---------------------------------------------------------------------------
    NAME
        XmlXvmCompileFile

    PURPOSE
        Compile XSLT stylesheet from file into bytecode

    DESCRIPTION
        Compile XSLT stylesheet from file into bytecode.

        Compiler flags could be one or more of the following:

            XMLXVM_DEBUG      - forces compiler to include debug
                                information into the bytecode

            XMLXVM_STRIPSPACE - same as <xsl:strip-space elements="*"/>.

        The generated bytecode resides in a compiler buffer which is freed 
        when next stylesheet is compiled or when compiler object is deleted. 
        Hence, if the bytecode is to be reused it should be copied into 
        another location. 


    RETURNS
        (ub2 *) bytecode or NULL on error

    ARGUMENTS
        comp    (IN)  compiler object
        path    (IN)  path of XSL stylesheet file
        baseuri (IN)  base URI of the document
        flags   (IN)  flags for the current compilation
        error   (OUT) returned error code

    SEE ALSO
        XmlXvmCompileURI, XmlXvmCompileBuffer, XmlXvmCompileDom

    EXAMPLE
        comp = XmlXvmCreateComp(xctx);
        code = XmlXvmCompileFile(comp, xslFile1, baseuri, 0, &err);
        ...  (copy the code)
        code = XmlXvmCompileFile(comp, xslFile2, baseuri, 0, &err);
        ...  (copy the code)

*/
ub2 *XmlXvmCompileFile(xmlxvmcomp *comp, oratext *path,
                       oratext *baseURI, xmlxvmflag flags, xmlerr *error);

/*---------------------------------------------------------------------------
    NAME
        XmlXvmCompileURI

    PURPOSE
        Compile XSLT stylesheet from URI into bytecode

    DESCRIPTION
        Compile XSLT stylesheet from URI into bytecode

        Compiler flags could be one or more of the following:

            XMLXVM_DEBUG      - forces compiler to include debug
                                information into the bytecode

            XMLXVM_STRIPSPACE - same as <xsl:strip-space elements="*"/>.

        The generated bytecode resides in a compiler buffer which is freed 
        when next stylesheet is compiled or when compiler object is deleted. 
        Hence, if the bytecode is to be reused it should be copied into 
        another location. 

    RETURNS
        (ub2 *) bytecode or NULL on error

    ARGUMENTS
        comp  (IN)  compiler object
        uri   (IN)  URI of the file contining the XSL stylesheet
        flags (IN)  flags for the current compilation
        error (OUT) returned error code

    SEE ALSO
        XmlXvmCompileFile, XmlXvmCompileBuffer, XmlXvmCompileDom

    EXAMPLE
        comp = XmlXvmCreateComp(xctx);
        code = XmlXvmCompileURI(comp, xslFile1, uri, XMLXVM_DEBUG, &err);
        ...  (copy the code)
        code = XmlXvmCompileURI(comp, xslFile2, uri, XMLXVM_STRIPSPACE, &err);
        ...  (copy the code)
*/
ub2 *XmlXvmCompileURI(xmlxvmcomp *comp, oratext *uri, xmlxvmflag flags, 
                      xmlerr *error);
ub2 *XmlXvmCompileURIWithBU(xmlxvmcomp *comp, oratext *uri, xmlxvmflag flags, 
                      oratext *baseURI, xmlerr *error);

/*---------------------------------------------------------------------------
    NAME
        XmlXvmCompileBuffer

    PURPOSE
        Compile an XSLT stylesheet from buffer into bytecode

    DESCRIPTION
        Compile an XSLT stylesheet from buffer into bytecode

        Compiler flags could be one or more of the following:

            XMLXVM_DEBUG      - forces compiler to include debug
                                information into the bytecode

            XMLXVM_STRIPSPACE - same as <xsl:strip-space elements="*"/>.

        The generated bytecode resides in a compiler buffer which is freed 
        when next stylesheet is compiled or when compiler object is deleted. 
        Hence, if the bytecode is to be reused it should be copied into 
        another location. 

    RETURNS
        (ub2 *) bytecode or NULL on error

    ARGUMENTS
        comp    (IN)  compiler object
        buffer  (IN)  pointer to buffer containing stylesheet document
        length  (IN)  the length of the stylesheet document in bytes
        baseuri (IN)  base URI of the document
        flags   (IN)  flags for the current compilation
        error   (OUT) returned error code

    SEE ALSO
        XmlXvmCompileFile, XmlXvmCompileURI, XmlXvmCompileDom
*/
ub2 *XmlXvmCompileBuffer(xmlxvmcomp *comp, oratext *buffer, ub4 length, 
                         oratext *baseURI, xmlxvmflag flags, xmlerr *error);

/*---------------------------------------------------------------------------
    NAME
        XmlXvmCompileDom

    PURPOSE
        Compile an XSLT stylesheet from DOM into bytecode

    DESCRIPTION
        Compile an XSLT stylesheet from DOM into bytecode 

        Compiler flags could be one or more of the following:

            XMLXVM_DEBUG      - forces compiler to include debug
                                information into the bytecode

            XMLXVM_STRIPSPACE - same as <xsl:strip-space elements="*"/>.

        The generated bytecode resides in a compiler buffer which is freed 
        when next stylesheet is compiled or when compiler object is deleted. 
        Hence, if the bytecode is to be reused it should be copied into 
        another location. 

    RETURNS
        (ub2 *) bytecode or NULL on error

    ARGUMENTS
        comp  (IN)  compiler object
        root  (IN)  root element of the stylesheet DOM
        flags (IN)  flags for the current compilation
        error (OUT) returned error code

    SEE ALSO
        XmlXvmCompileFile, XmlXvmCompileBuffer, XmlXvmCompileURI

    EXAMPLE
        comp   = XmlXvmCreateComp(xctx);
        code   = XmlXvmCompileDom(comp, root, XMLXVM_DEBUG, &err);
*/
ub2 *XmlXvmCompileDom(xmlxvmcomp *comp, xmldocnode *root, xmlxvmflag flags, 
                      xmlerr *error);
ub2 *XmlXvmCompileDomWithBU(xmlxvmcomp *comp, xmldocnode *root,
                            xmlxvmflag flags, oratext *baseURI, xmlerr *error);

/*---------------------------------------------------------------------------
    NAME
        XmlXvmCompileXPath

    PURPOSE
        Compiles an XPath expression

    DESCRIPTION
        Compiles an XPath expression.  The optional pfxmap is used
        to map namespace prefixes to URIs in the XPath expression.
        It is an array of prefix, URI values, endinging in NULL,
        for example:

        oratext *my_pfx_map[] = {
            "pfx1", "/this/that/pfx1/uri",
            "pfx2", "/this/that/another/pfx2/uri",
            NULL
        };

    RETURNS
        (ub2 *) XPath expression bytecode or NULL on error

    ARGUMENTS
        comp   (IN)  Compiler object
        xpath  (IN)  XPath expression
        pfxmap (IN)  array of prefix-URI mappings
        error  (OUT) returned error code

    EXAMPLE
        comp = XmlXvmCreateComp(xctx);
        code = XmlXvmCompileXPath(comp, "pfx1:doc/employee[4]",
                                  my_pfx_map, &err);

*/
ub2 *XmlXvmCompileXPath(xmlxvmcomp *comp, oratext *xpath,
                        oratext **pfxmap, xmlerr *error);
ub2 *XmlXvmCompileXPathEx(xmlxvmcomp *comp, oratext *xpath,
                          oratext **pfxmap, oratext **pfext, xmlerr *error);

/*---------------------------------------------------------------------------
    NAME
        XmlXvmGetBytecodeLength

    PURPOSE
        Returns the bytecode length

    DESCRIPTION
        The bytecode length is needed when the bytecode is to be copied 
        or when it is set into XSLTVM.

    RETURNS
        (ub4) The bytecode length in bytes.

    ARGUMENTS
        bytecode (IN)  bytecode buffer
        error    (OUT) returned error code

    EXAMPLE
        len = XmlXvmGetBytecodeLength(code, &err);
        err = XmlXvmSetBytecodeBuffer (vm, code, len);
*/
ub4 XmlXvmGetBytecodeLength(ub2 *bytecode, xmlerr *error);

/****************************************************************************
                               Interface XSLTVM
 ****************************************************************************/

/*---------------------------------------------------------------------------
    NAME
        XMLXVM_DEBUG_F

    PURPOSE
        XML XSLT VM debug function

    DESCRIPTION
        Debug callback function for XSLT VM

    RETURNS
        (void)

    ARGUMENTS
        line (IN) source stylesheet line#
        file (IN) stylesheet filename
        obj  (IN) current VM object
        n    (IN) index of current node

    SEE ALSO
        XmlXvmSetDebugFunc
*/
#define XMLXVM_DEBUG_F(func, line, file, obj, n) \
    void func(ub2 line, oratext *file, xvmobj *obj, ub4 n)

/*---------------------------------------------------------------------------
    NAME
        XmlXvmCreate

    PURPOSE
        Create an XSLT virtual machine

    DESCRIPTION
        Create an XSLT virtual machine.
        Zero or more of the following XSLTVM properties could be set by
        using this API:

            "VMStack", size,     - sets the size[Kbyte] of the main VM stack.
                                   Default size is 4K.

            "NodeStack", size,   - sets the size[Kbyte] of the node-stack
                                   Default size is 16K.

            "StringStack", size  - sets the size[Kbyte] of the string-stack
                                   Default size is 64K.

        If the stack size is not specified the default size is used.  The
        explicit stack size seting is needed when XSLTVM terminates with
        "Stack Overflow" message or when smaller memory footprints are
        required.

    RETURNS
        (xmlxvm *) XSLT virtual machine object, or NULL on error

    ARGUMENTS
        xctx (IN) XML context
        ...  (IN) list of zero or more properties to be set, NULL-terminated
        
    SEE ALSO
        XmlXvmDestroy

    EXAMPLE
        vm = XmlXvmCreate(xctx);
        XmlXvmDestroy(vm);
        ...
        vm = XmlXvmCreate(xctx, "VMStack", 10, "NodeStack", 24, 
                                "StringStack", 24, NULL);
*/
xmlxvm *XmlXvmCreate(xmlctx *xctx, ...);

/*---------------------------------------------------------------------------
    NAME
        XmlXvmDestroy

    PURPOSE
        Destroys an XSLT virtual machine

    DESCRIPTION
        Destroys an XSLT virtual machine

    RETURNS
        (void)

    ARGUMENTS
        vm   (IN) VM object
        comp (IN) XSLT virtual machine object

    SEE ALSO
        XmlXvmCreate
*/
void XmlXvmDestroy(xmlxvm *vm);

/*---------------------------------------------------------------------------
    NAME
        XmlXvmTransformFile

    PURPOSE
        Run compiled XSLT stylesheet on XML document in file

    DESCRIPTION
        Run compiled XSLT stylesheet on XML document in file.  The compiled
        XSLT stylesheet (bytecode) should be set using XmlXvmSetBytecode ...()
        prior to this call.  xsl:output is supported only if the output is
        a stream instead of DOM or SAX.

    RETURNS
        (xmlerr) error code

    ARGUMENTS
        vm   (IN) VM object
        path (IN) path of XML document to transform

    SEE ALSO
        XmlXvmTransformURI, XmlXvmTransformBuffer, XmlXvmTransformDom

    EXAMPLE
        code = XmlXvmCompileFile(comp, xslFile, baseuri, 0, &err);
        ...
        vm   = XmlXvmCreate(xctx, NULL);
        len  = XmlXvmGetBytecodeLength(code, &err);
        err  = XmlXvmSetBytecodeBuffer(vm, code, len);
        err  = XmlXvmTransformFile(vm, xmlFile[i], baseuri);
*/
xmlerr XmlXvmTransformFile(xmlxvm *vm, oratext *path, oratext *baseURI);

/*---------------------------------------------------------------------------
    NAME
        XmlXvmTransformURI

    PURPOSE
        Run compiled XSLT stylesheet on XML document from URI

    DESCRIPTION
        Run compiled XSLT stylesheet on XML document from URI.  The compiled
        XSLT stylesheet (bytecode) should be set using XmlXvmSetBytecode ...() 
        prior to this call.  xsl:output is supported only if the output is
        a stream instead of DOM or SAX.

    RETURNS
        (xmlerr) error code.

    ARGUMENTS
        vm      (IN) VM object
        uri     (IN) URI of XML document to transform
        baseuri (IN) base URI of XML document

    SEE ALSO
        XmlXvmTransformFile, XmlXvmTransformBuffer, XmlXvmTransformDom
*/
xmlerr XmlXvmTransformURI(xmlxvm *vm, oratext *uri);

/*---------------------------------------------------------------------------
    NAME
        XmlXvmTransformBuffer

    PURPOSE
        Run compiled XSLT stylesheet on XML document in memory

    DESCRIPTION
        Run compiled XSLT stylesheet on XML document in memory.  The
        compiled XSLT stylesheet (bytecode) should be set using
        XmlXvmSetBytecode...() prior to this call.  xsl:output is supported 
        only if the output is a stream instead of DOM or SAX.

    RETURNS
        (xmlerr) error code.

    ARGUMENTS
        vm     (IN) VM object
        buffer (IN) NULL-terminated buffer containing the XML document
        length (IN) the length of the XML document
        baseURI(IN) base URI of the XML document

    SEE ALSO
        XmlXvmTransformFile, XmlXvmTransformURI, XmlXvmTransformDom
*/
xmlerr XmlXvmTransformBuffer(xmlxvm *vm, oratext *buffer,
                               ub4 length, oratext *baseURI);

/*---------------------------------------------------------------------------
    NAME
        XmlXvmTransformDom

    PURPOSE
        Run compiled XSLT stylesheet on XML document as DOM

    DESCRIPTION
        Run compiled XSLT stylesheet on XML document as DOM.  The compiled
        XSLT stylesheet (bytecode) should be set using XmlXvmSetBytecode...()
        prior to this call.  xsl:output is supported only if the output is
        a stream instead of DOM or SAX.

    RETURNS
        (xmlerr) error code.

    ARGUMENTS
        vm   (IN) VM object
        root (IN) root element of the XML document's DOM

    SEE ALSO
        XmlXvmTransformFile, XmlXvmTransformURI, XmlXvmTransformBuffer

    EXAMPLE
        code   = XmlXvmCompileFile(comp, xslFile, baseuri, 0, &err);
        vm     = XmlXvmCreate(xctx, NULL);
        len    = XmlXvmGetBytecodeLength(code, &err);
        err    = XmlXvmSetBytecodeBuffer (vm, code, len); 
        domdoc = XmlLoadDom(xctx, &err, "uri", xmlFile, NULL);
        err    = XmlXvmTransformDom(vm, domdoc);
        err    = XmlXvmTransformDom (vm, domdoc);
*/
xmlerr XmlXvmTransformDom(xmlxvm *vm, xmldocnode *root);

/*---------------------------------------------------------------------------
    NAME
        XmlXvmSetOutputDom

    PURPOSE
        Sets the XSLTVM to output document node.

    DESCRIPTION
        Sets the XSLTVM to output DOM. If (xmldocnode == NULL) then the 
        result DOM tree belongs to the VM object and is deleted when 
        a new transformation is performed or when the VM object is deleted.
        If the result DOM tree is to be used for longer period of time then
        an xmldocnode has to be created and set to the VM object.         

    RETURNS
        (xmlerr) error code

    ARGUMENTS
        vm  (IN) VM object
        doc (IN) empty document

    EXAMPLE
        doc  = XmlCreateDocument(xctx,  NULL, NULL, NULL, &err);
        err  = XmlXvmSetOutputDom(vm, doc);
        err  = XmlXvmTransformFile(vm, ...); 
        root = XmlXvmGetOutputDom(vm);
*/
xmlerr XmlXvmSetOutputDom(xmlxvm *vm, xmldocnode *doc);

/*---------------------------------------------------------------------------
    NAME
        XmlXvmSetOutputSax

    PURPOSE
        Set XSLTVM to output SAX

    DESCRIPTION
        Set XSLTVM to output SAX. If the SAX callback interface object is
        provided the VM outputs the result document in a form of SAX events
        using the user specified callback functions. 

    RETURNS
        (xmlerr) error code

    ARGUMENTS
        vm     (IN) VM object
        saxcb  (IN) SAX callback object
        saxctx (IN) SAX context
*/
xmlerr XmlXvmSetOutputSax(xmlxvm *vm, xmlsaxcb *saxcb, void *saxctx);

/*---------------------------------------------------------------------------
    NAME
        XmlXvmSetOutputStream

    PURPOSE
        Set the XSLTVM output to a user-defined stream

    DESCRIPTION
        Set the XSLTVM output to a user-defined stream. The default XSLTVM
        output is a stream. This APIs overrides the default stream with 
        user spesified APIs for writing. 

    RETURNS
        (xmlerr) error code.

    ARGUMENTS
        vm      (IN) VM object
        ostream (IN) stream object
*/
xmlerr XmlXvmSetOutputStream(xmlxvm *vm, xmlostream *ostream);

/*---------------------------------------------------------------------------
    NAME
        XmlXvmSetOutputOraStream

    PURPOSE
        Set the XSLTVM output to an OraStream

    DESCRIPTION
        Set the XSLTVM output to an OraStream. The default XSLTVM
        output is a stream. This APIs overrides the default stream with 
        user spesified APIs for writing. 

    RETURNS
        (xmlerr) error code.

    ARGUMENTS
        vm      (IN) VM object
        ostream (IN) OraStream object
*/
xmlerr XmlXvmSetOutputOraStream(xmlxvm *vm, orastream *ostream);

/*---------------------------------------------------------------------------
    NAME
        XmlXvmSetOutputEncoding

    PURPOSE
        Sets the encoding for the XSLTVM output

    DESCRIPTION
        Sets the encoding for the XSLTVM stream output. If the input (data) 
        encoding is different from the one set by this APIs then encoding
        conversion is performed. This APIs overrides the encoding set in the
        XSLT stylesheet (if any).

    RETURNS
        (xmlerr) error code.

    ARGUMENTS
        vm       (IN) VM object
        encoding (IN) output encoding
*/
xmlerr XmlXvmSetOutputEncoding(xmlxvm *vm, oratext *encoding);

/*---------------------------------------------------------------------------
    NAME
        XmlXvmSetBaseURI

    PURPOSE
        Sets the base URI for the XSLTVM

    DESCRIPTION
        Sets the base URI for the XSLTVM. The baseuri is used by VM to 
        the compose the path XML documents to be loaded for transformation
        using document() or XmlXvmTransformFile().

    RETURNS
        (xmlerr) error code.

    ARGUMENTS
        vm      (IN) VM object
        baseuri (IN) VM baseuri for reading and writing documents

   EXAMPLE
        err  = XmlXvmSetBaseURI(vm, "/doc/myxmlfiles/");
        err  = XmlXvmTransformFile(vm, "xmlfile1", NULL);
        err  = XmlXvmTransformFile(vm, "xmlfile2", NULL);
        ...
*/
xmlerr XmlXvmSetBaseURI(xmlxvm *vm, oratext *baseuri);

/*---------------------------------------------------------------------------
    NAME
        XmlXvmSetTextParam

    PURPOSE
        Set the stylesheet top-level text param.

    DESCRIPTION
        Set the stylesheet top-level text param. The param value set in the
        XSLT stylesheet is overritten. Since the top-level param are reset
        with stylesheet values after each transformation this APIs has to be 
        called again.

    RETURNS
        (xmlerr) error code, XMLERR_SUCC [0] on success.
        This returns XMLERR_NULL_PTR [1] if the parameter isn't defined
        by the style sheet.

    ARGUMENTS
        vm    (IN) VM object
        name  (IN) name of top-level param
        value (IN) value of top-level param
*/
xmlerr XmlXvmSetTextParam(xmlxvm *vm, oratext *name, oratext *value);

/*---------------------------------------------------------------------------
    NAME
        XmlXvmSetTextParamEx

    PURPOSE
        Set the stylesheet top-level text param.

    DESCRIPTION
        Set the stylesheet top-level text param. The param value set in the
        XSLT stylesheet is overritten. Since the top-level param are reset
        with stylesheet values after each transformation this APIs has to be 
        called again.

    RETURNS
        (xmlerr) error code, XMLERR_SUCC [0] on success.
        This returns XMLERR_UNKNOWN_PARAM if the parameter isn't defined
        by the style sheet. It return XMLERR_PARAM_IS_SET if an attempt is
        made to set a parameter that's already been set.

    ARGUMENTS
        vm    (IN) VM object
        name  (IN) name of top-level param
        value (IN) value of top-level param
*/
xmlerr XmlXvmSetTextParamEx(xmlxvm *vm, oratext *name, oratext *value);

/*---------------------------------------------------------------------------
    NAME
        XmlXvmResetParams
        
    PURPOSE
        Resets the stylesheet top-level text params.

    DESCRIPTION
        Resets the stylesheet top-level param with their default values.

    RETURNS
        No returns.

    ARGUMENTS
        vm    (IN) VM object
*/
void XmlXvmResetParams(xmlxvm *vm);

/*---------------------------------------------------------------------------
    NAME
        XmlXvmGetOutputDom

    PURPOSE
        Returns the output DOM

    DESCRIPTION
        Returns the root node of the result DOM tree (if any).
        XmlXvmSetOutputDom() has to be used before transformation to set 
        the VM to output a DOM tree (the default VM output is a stream). 

    RETURNS
        (xmlfragnode *) output DOM, or NULL in a case of SAX or Stream output.

    ARGUMENTS
        vm (IN) VM object

    SEE ALSO
        XmlXvmSetOutputDom

    EXAMPLE
        doc  = XmlCreateDocument(xctx,  NULL, NULL, NULL, &err);
        err  = XmlXvmSetOutputDom(vm, doc);
        err  = XmlXvmTransformFile(vm, ...); 
        root = XmlXvmGetOutputDom(vm);
*/
xmlfragnode *XmlXvmGetOutputDom(xmlxvm *vm);

/*---------------------------------------------------------------------------
    NAME
       XmlXvmSetDebugFunc

    PURPOSE
       Set a callback function for debuging

    DESCRIPTION
       The user callback function is invoked by VM every time the execution
       reaches a new line in the XSLT stylesheet. The VM passes to the user 
       the stylesheet file name, the line number, the current context nodes-set
       and the current node index in the node-set. 
       IMPORTANT - the stylesheet has to be compiled with flag XMLXVM_DEBUG.

    RETURNS
        (xmlerr) numeric error code, XMLERR_OK [0] on success

    ARGUMENTS
        vm   (IN) XSLT VM context
        func (IN) the callback function

    EXAMPLE
        XMLXVM_DEBUG_F(MyCallbackFunction, line, file, obj, n);
        ...
        code = XmlXvmCompileFile(comp, xslFile, NULL, XMLXVM_DEBUG, &err);
        ...
        vm   = XmlXvmCreate(xctx, NULL);
        len  = XmlXvmGetBytecodeLength(code, &err);
        err  = XmlXvmSetBytecodeBuffer(vm, code, len);
        err  = XmlXvmSetDebugFunc(vm, MyCallbackFunction);
        err  = XmlXvmTransformFile(vm, xmlFile, baseuri);
*/
#define XMLXVM_DEBUG_FUNC(func) \
    void (*func) (ub2 line, oratext *filename, xvmobj *obj, ub4 n)
xmlerr XmlXvmSetDebugFunc(xmlxvm *vm, XMLXVM_DEBUG_FUNC(debugcallback));

/*---------------------------------------------------------------------------
    NAME
       XmlXvmSetDebugFlags

    PURPOSE
       Set debug flags

    DESCRIPTION
       Sets flags for debug mode, in lieu of setting a callback function.

    RETURNS
        (xmlerr) numeric error code, XMLERR_OK [0] on success

    ARGUMENTS
        vm    (IN) XSLT VM context
        flags (IN) the flags to set

    EXAMPLE
*/
xmlerr XmlXvmSetDebugFlags(xmlxvm *vm, ub2 flags);

/*---------------------------------------------------------------------------
    NAME
       XmlXvmSetBytecodeFile

    PURPOSE
       Set the compiled bytecode from file

    DESCRIPTION
       Set the compiled bytecode from file. Any previously set bytecode 
       is replaced. An XML transformation can't be performed if the stylesheet 
       bytecode is not set.

    RETURNS
        (xmlerr) numeric error code, XMLERR_OK [0] on success

    ARGUMENTS
        vm   (IN) XSLT VM context
        path (IN) path of bytecode file

    SEE ALSO
        XmlXvmSetBytecodeURI, XmlXvmSetBytecodeBuffer

    EXAMPLE
        err = XmlXvmSetBytecodeFile(vm, codefile);
        err = XmlXvmTransformFile (vm, xmlFile, baseuri);
*/
xmlerr XmlXvmSetBytecodeFile(xmlxvm *vm, oratext *path);

/*---------------------------------------------------------------------------
    NAME
       XmlXvmSetBytecodeURI

    PURPOSE
       Set the compiled byte-code

    DESCRIPTION
       Set the compiled bytecode from URI. Any previously set bytecode 
       is replaced. An XML transformation can't be performed if the stylesheet 
       bytecode is not set.

    RETURNS
        (xmlerr) numeric error code, XMLERR_OK [0] on success

    ARGUMENTS
        vm  (IN) XSLT VM context
        uri (IN) URI

    SEE ALSO
        XmlXvmSetBytecodeFile, XmlXvmSetBytecodeBuffer
*/
xmlerr XmlXvmSetBytecodeURI(xmlxvm *vm, oratext *uri);

/*---------------------------------------------------------------------------
    NAME
       XmlXvmSetBytecodeBuffer

    PURPOSE
       Set the compiled byte-code

    DESCRIPTION
       Set the compiled bytecode from buffer. Any previously set bytecode 
       is replaced. An XML transformation can't be performed if the stylesheet 
       bytecode is not set. The VM doesn't copy the bytecode into internal 
       buffer, hence the it shouldn't be freed before VM finishes using it. 

    RETURNS
        (xmlerr) numeric error code, XMLERR_OK [0] on success

    ARGUMENTS
        vm     (IN) XSLT VM context
        buffer (IN) user's buffer
        buflen (IN) size of buffer in bytes

    SEE ALSO
        XmlXvmSetBytecodeFile, XmlXvmSetBytecodeURI

    EXAMPLE
        code = XmlXvmCompileFile(comp, xslFile, baseuri, flags, &err);
        len = XmlXvmGetBytecodeLength(code, &err);
        err = XmlXvmSetBytecodeBuffer(vm, code, len);
*/
xmlerr XmlXvmSetBytecodeBuffer(xmlxvm *vm, ub2 *buffer, size_t buflen);

/*---------------------------------------------------------------------------
    NAME
        XmlXvmGetObjectType
        
    PURPOSE
        Get XPath object type

    DESCRIPTION
        Get XPath object type

    RETURNS
        (xmlxvmobjtype) type-code for object
 
    ARGUMENTS
        xobj (IN) XPath object

    SEE ALSO
        XmlXvmGetObjectNSetNum, XmlXvmGetObjectNSetNode,
        XmlXvmGetObjectString, XmlXvmGetObjectNumber,
        XmlXvmGetObjectBoolean

    EXAMPLE
        switch (XmlXvmGetObjectType(obj)) {
            case XMLXVM_TYPE_NDSET: ...
            case XMLXVM_TYPE_BOOL: ...  
            case XMLXVM_TYPE_NUM: ... 
            case XMLXVM_TYPE_FRAG: ...
            case XMLXVM_TYPE_STR: ...
        }
*/
xmlxvmobjtype XmlXvmGetObjectType(xvmobj *obj);

/*---------------------------------------------------------------------------
    NAME
        XmlXvmGetObjectNSetNum
        
    PURPOSE
        Get number of of nodes in nodeset-type XPath object

    DESCRIPTION
        Get number of of nodes in nodeset-type XPath object

    RETURNS
        (ub4) number of nodes in nodeset
 
    ARGUMENTS
        xobj (IN) XPath object (nodeset)

    SEE ALSO
        XmlXvmGetObjectType, XmlXvmGetObjectNSetNode,
        XmlXvmGetObjectString, XmlXvmGetObjectNumber,
        XmlXvmGetObjectBoolean

    EXAMPLE
        obj = XmlXvmEvaluateXPath(vm, code, 1, 1, node);
        switch (XmlXvmGetObjectType(obj)) {
            case XMLXVM_TYPE_NDSET:
                objnum = XmlXvmGetObjectNSetNum(obj);
                for (i = 0; i < objnum; i++) 
                    node = XmlXvmGetObjectNSetNode(obj, i);
                    ...
        }
*/
ub4 XmlXvmGetObjectNSetNum(xvmobj *obj);

/*---------------------------------------------------------------------------
    NAME
        XmlXvmGetObjectNSetNode
        
    PURPOSE
        Get node from nodeset-type XPath object

    DESCRIPTION
        Get node from nodeset-type XPath object

    RETURNS
        (xmlnode *) The object type or values.
 
    ARGUMENTS
        xobj (IN) XPath object
        i    (IN) node index in nodeset

    SEE ALSO
        XmlXvmGetObjectType, XmlXvmGetObjectNSetNum,
        XmlXvmGetObjectString, XmlXvmGetObjectNumber,
        XmlXvmGetObjectBoolean

    EXAMPLE
        obj = XmlXvmEvaluateXPath(vm, code, 1, 1, node);
        switch (XmlXvmGetObjectType(obj)) {
            case XMLXVM_TYPE_NDSET:
                objnum = XmlXvmGetObjectNSetNum(obj);
                for (i = 0; i < objnum; i++) 
                    node = XmlXvmGetObjectNSetNode(obj, i);
                    ...
        }
*/
xmlnode *XmlXvmGetObjectNSetNode(xvmobj *obj, ub4 i);

/*---------------------------------------------------------------------------
    NAME
        XmlXvmGetObjectString
        
    PURPOSE
        Get string from XPath object

    DESCRIPTION
        Get string from XPath object

    RETURNS
        (oratext *) string
 
    ARGUMENTS
        xobj (IN) XPath object.

    SEE ALSO
        XmlXvmGetObjectType, XmlXvmGetObjectNSetNum,
        XmlXvmGetObjectNSetNode, XmlXvmGetObjectNumber,
        XmlXvmGetObjectBoolean

    EXAMPLE
        obj = XmlXvmEvaluateXPath(vm, code, 1, 1, node);
        switch (XmlXvmGetObjectType(obj)) 
        {
            case XMLXVM_TYPE_STR:
                str = XmlXvmGetObjectString(obj);
                ...
        }
*/
oratext *XmlXvmGetObjectString(xvmobj *obj);

/*---------------------------------------------------------------------------
    NAME
        XmlXvmGetObjectNumber
        
    PURPOSE
        Get number from XPath object

    DESCRIPTION
        Get number from XPath object

    RETURNS
        (double) number
 
    ARGUMENTS
        xobj (IN) XPath object.

    SEE ALSO
        XmlXvmGetObjectType, XmlXvmGetObjectNSetNum,
        XmlXvmGetObjectNSetNode, XmlXvmGetObjectString,
        XmlXvmGetObjectBoolean

    EXAMPLE
        obj = XmlXvmEvaluateXPath(vm, code, 1, 1, node);
        switch (XmlXvmGetObjectType(obj)) 
        {
            case XMLXVM_TYPE_NUM:
                num = XmlXvmGetObjectNumber(obj);
                ...
        }
*/
double XmlXvmGetObjectNumber(xvmobj *obj);

/*---------------------------------------------------------------------------
    NAME
        XmlXvmGetObjectBoolean
        
    PURPOSE
        Get boolean value of XPath object

    DESCRIPTION
        Get boolean value of XPath object

    RETURNS
        (boolean) value of an XPath object
 
    ARGUMENTS
        xobj (IN) XPath object.

    SEE ALSO
        XmlXvmGetObjectType, XmlXvmGetObjectNSetNum,
        XmlXvmGetObjectNSetNode, XmlXvmGetObjectNumber,
        XmlXvmGetObjectBoolean

    EXAMPLE
        obj = XmlXvmEvaluateXPath(vm, code, 1, 1, node);
        switch (XmlXvmGetObjectType(obj)) 
        {
            case XMLXVM_TYPE_BOOL:
                bool = XmlXvmGetObjectBoolean(obj);
                ...
        }
*/
boolean XmlXvmGetObjectBoolean(xvmobj *obj);

/*---------------------------------------------------------------------------
    NAME
        XmlXvmGetObjectNSetArray
        
    PURPOSE
        Get all nodes from nodeset-type XPath object

    DESCRIPTION
        Get node from nodeset-type XPath object

    RETURNS
        (xmlnode **) The array of nodes in the node set
 
    ARGUMENTS
        xsltvm (IN) VM object
        xobj   (IN) XPath object

    SEE ALSO
        XmlXvmGetObjectType, XmlXvmGetObjectNSetNum

    EXAMPLE
        obj = XmlXvmEvaluateXPath(vm, code, 1, 1, node);
        switch (XmlXvmGetObjectType(obj)) {
            case XMLXVM_TYPE_NDSET:
                objnum = XmlXvmGetObjectNSetNum(obj);
                nodes = XmlXvmGetObjectNSetArray(vm, obj);
                for (i = 0; i < objnum; i++) 
                    node = nodes[i];
                    ...
        }
*/
xmlnode **XmlXvmGetObjectNSetArray (xmlxvm *xsltvm, xvmobj *obj);

/*---------------------------------------------------------------------------
    NAME
        XmlXvmEvaluateXPath

    PURPOSE
        Evaluate already-compiled XPath expression

    DESCRIPTION
        Evaluate already-compiled XPath expression

    RETURNS
        (xvmobj *) XPath object

    ARGUMENTS
        vm       (IN) XSLTVM object
        bytecode (IN) XPath expression bytecode
        ctxpos   (IN) current context position
        ctxsize  (IN) current context size
        ctxnode  (IN) current context node

   EXAMPLE
        code   = XmlXvmCompileXPath(comp, xpath, NULL, &err);
        obj    = XmlXvmEvaluateXPath(vm, code, 1, 1, node);
        switch (XmlXvmGetObjectType(obj))
        ...
*/
xvmobj *XmlXvmEvaluateXPath(xmlxvm *vm, ub2 *bytecode, ub4 ctxpos,
                            ub4 ctxsize, xmlnode *ctxnode);


/*--------------------------------------------------
 NAME
     XmlXvmSetXMLCtx
 PURPOSE
     Reset the xmlctx for the xmlxvm
*/
void XmlXvmSetXMLCtx(xmlxvm *vm, xmlctx *xctx);


/*--------------------------------------------------
 NAME
     XmlXvmGetXMLCtx
 PURPOSE
     Get the xmlctx for the xmlxvm
*/
xmlctx * XmlXvmGetXMLCtx(xmlxvm *vm);
#endif                                              /* XMLXVM_ORACLE */
