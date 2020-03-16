/* $RCSfile: xmlxsl.h $ $Date: 2015/08/12 07:16:56 $
 Copyright (c) 2001, 2015, Oracle and/or its affiliates. All rights reserved.

   FILE NAME
        xmlxsl.h - Oracle XML XSLT APIs

   FILE DESCRIPTION
        This file defines the XSLT and XPath interfaces.
*/


#ifndef XMLXSL_ORACLE
# define XMLXSL_ORACLE

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

/*---------------------------------------------------------------------------
                     PUBLIC TYPES AND CONSTANTS
  ---------------------------------------------------------------------------*/

/* DATATYPE xslctx - XSL top-level context
   SEE ALSO XmlXslCreate, XmlXslDestroy
   XSLT context
*/
struct xslctx;
#ifndef ORAXML_ORACLE
typedef struct xslctx xslctx;
#endif

/* DATATYPE xpctx - XPath top-level context
   SEE ALSO XmlXPathCreateCtx, XmlXPathDestroyCtx
   XPath context
*/
struct xpctx;
#ifndef ORAXML_ORACLE
typedef struct xpctx xpctx;
#endif

/* DATATYPE xpexpr - XPath expression
   SEE ALSO XmlXPathParse, XmlXPathEval
   XPath expression
*/
struct xpexpr;
#ifndef ORAXML_ORACLE
typedef struct xpexpr xpexpr;
#endif

/* DATATYPE xpobj - XPath object
   SEE ALSO XmlXPathEval, XmlXPathGetObjectType
   XPath object
*/
struct xpobj;
#ifndef ORAXML_ORACLE
typedef struct xpobj xpobj;
#endif

/*---------------------------------------------------------------------------
                           DEPRECATED User-Defined Stream
  ---------------------------------------------------------------------------*/

/* DATATYPE xmlostream - DEPRECATED User-defined output stream
   SEE ALSO XmlDomGetNodeValueStream, XmlDomGetAttrValueStream, XmlXslSetOutputStream, XmlXvmSetOutputStream
    DEPRECATED Generic user-defined output stream.  The three function
    pointers are required (but may be stubs).  The context pointer is
    entirely user-defined; point it to whatever state information is
    required to manage the stream; it will be passed as first argument
    to the user functions.  DEPRECATED, use orastream interfaces instead.
*/
typedef struct xmlostream {
    XML_STREAM_OPEN_F((*open_xmlostream), xctx, sctx, path, parts, length);
    XML_STREAM_WRITE_F((*write_xmlostream), xctx, sctx, path, src, size);
    XML_STREAM_CLOSE_F((*close_xmlostream), xctx, sctx);
    void *ctx_xmlostream;                         /* user's stream context */
} xmlostream;

/*---------------------------------------------------------------------------
                           Package XPath - XPath APIs
  ---------------------------------------------------------------------------*/

/* DATATYPE xmlxslobjtype - XSL processor object type
   SEE ALSO XmlXPathGetObjectType, XmlXvmGetObjectType
   Type of XSLT object that may be returned.
*/
typedef enum xmlxslobjtype {
    XMLXSL_TYPE_UNKNOWN = 0, /* Not a defined type */
    XMLXSL_TYPE_NDSET   = 1, /* Node-set */
    XMLXSL_TYPE_BOOL    = 2, /* Boolean value */
    XMLXSL_TYPE_NUM     = 3, /* Numeric value (double) */
    XMLXSL_TYPE_STR     = 4, /* String */
    XMLXSL_TYPE_FRAG    = 5  /* Document Fragment */
} xmlxslobjtype;

/****************************************************************************
                               Interface XPath
 ****************************************************************************/

/*---------------------------------------------------------------------------
    NAME
        XmlXPathCreateCtx

    PURPOSE
        Create an XPath context

    DESCRIPTION
        Create an XPath context

    RETURNS
        (xpctx *) XPath context or NULL on error

    ARGUMENTS
        xsl     (IN) XSL stylesheet as xmldoc object.
        baseuri (IN) base URI used by document [if any]
        ctxpos  (IN) current context position.
        ctxsize (IN) current context size
        ctxnode (IN) current context node
*/
xpctx* XmlXPathCreateCtx(xmlctx *xsl, oratext *baseuri,
                         xmlnode *ctxnode, ub4 ctxpos, ub4 ctxsize);

/*---------------------------------------------------------------------------
    NAME
        XmlXPathDestroyCtx

    PURPOSE
        Destroy an XPath context

    DESCRIPTION
        Destroy an XPath context.

    RETURNS
        (void)

    ARGUMENTS
        xslxpctx (IN) XPath context object.
*/
void XmlXPathDestroyCtx(xpctx *xslxpctx);

/*---------------------------------------------------------------------------
    NAME
        XmlXPathParse

    PURPOSE
        Parse XPath expression

    DESCRIPTION
        Parse XPath expression.

    RETURNS
        (xpexpr *) XPath expression parse tree or NULL on error
 
    ARGUMENTS
        xpctx (IN)  XPath context object
        expr  (IN)  XPath expression
        err   (OUT) error code
*/
xpexpr* XmlXPathParse(xpctx *xctx, oratext *expr,  xmlerr * err);

/*---------------------------------------------------------------------------
    NAME
        XmlXPathEval

    PURPOSE
        Evaluate XPath expression

    DESCRIPTION
        Evaluate XPath expression.

    RETURNS
        (xpobj *) result XPath object or NULL on error
 
    ARGUMENTS
        xpctx    (IN)  XPath context
        exprtree (IN)  Parsed XPath expression tree
        err      (OUT) error code
*/
xpobj *XmlXPathEval(xpctx *xctx, xpexpr *exprtree, xmlerr *err);

/*---------------------------------------------------------------------------
    NAME
        XmlXPathGetObjectType
        
    PURPOSE
        Get XPath object type

    DESCRIPTION
        Get XPath object type

    RETURNS
        (xmlxslobjtype) type-code for object
 
    ARGUMENTS
        xobj (IN) XPath object

    SEE ALSO
        XmlXPathGetObjectNSetNum, XmlXPathGetObjectNSetNode,
        XmlXPathGetObjectString, XmlXPathGetObjectNumber,
        XmlXPathGetObjectBoolean
*/
xmlxslobjtype XmlXPathGetObjectType(xpobj *obj);

/*---------------------------------------------------------------------------
    NAME
        XmlXPathGetObjectNSetNum
        
    PURPOSE
        Get number of of nodes in nodeset-type XPath object

    DESCRIPTION
        Get number of of nodes in nodeset-type XPath object

    RETURNS
        (ub4) number of nodes in nodeset
 
    ARGUMENTS
        xobj (IN) XPath object (nodeset)

    SEE ALSO
        XmlXPathGetObjectType, XmlXPathGetObjectNSetNode,
        XmlXPathGetObjectString, XmlXPathGetObjectNumber,
        XmlXPathGetObjectBoolean
*/
ub4 XmlXPathGetObjectNSetNum(xpobj *obj);

/*---------------------------------------------------------------------------
    NAME
        XmlXPathGetObjectNSetNode
        
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
        XmlXPathGetObjectType, XmlXPathGetObjectNSetNum,
        XmlXPathGetObjectString, XmlXPathGetObjectNumber,
        XmlXPathGetObjectBoolean
*/
xmlnode *XmlXPathGetObjectNSetNode(xpobj *obj, ub4 i);

/*---------------------------------------------------------------------------
    NAME
        XmlXPathGetObjectString
        
    PURPOSE
        Get string from XPath object

    DESCRIPTION
        Get string from XPath object

    RETURNS
        (oratext *) string
 
    ARGUMENTS
        xobj (IN) XPath object.

    SEE ALSO
        XmlXPathGetObjectType, XmlXPathGetObjectNSetNum,
        XmlXPathGetObjectNSetNode, XmlXPathGetObjectNumber,
        XmlXPathGetObjectBoolean
*/
oratext *XmlXPathGetObjectString(xpobj *obj);

/*---------------------------------------------------------------------------
    NAME
        XmlXPathGetObjectNumber
        
    PURPOSE
        Get number from XPath object

    DESCRIPTION
        Get number from XPath object

    RETURNS
        (double) number
 
    ARGUMENTS
        xobj (IN) XPath object.

    SEE ALSO
        XmlXPathGetObjectType, XmlXPathGetObjectNSetNum,
        XmlXPathGetObjectNSetNode, XmlXPathGetObjectString,
        XmlXPathGetObjectBoolean
*/
double XmlXPathGetObjectNumber(xpobj *obj);

/*---------------------------------------------------------------------------
    NAME
        XmlXPathGetObjectBoolean
        
    PURPOSE
        Get boolean value of XPath object

    DESCRIPTION
        Get boolean value of XPath object

    RETURNS
        (boolean) truth value
 
    ARGUMENTS
        xobj (IN) XPath object.

    SEE ALSO
        XmlXPathGetObjectType, XmlXPathGetObjectNSetNum,
        XmlXPathGetObjectNSetNode, XmlXPathGetObjectNumber,
        XmlXPathGetObjectBoolean
*/
boolean XmlXPathGetObjectBoolean(xpobj *obj);

/*---------------------------------------------------------------------------
    NAME
        XmlXPathGetObjectFragment
        
    PURPOSE
        Get fragment value of XPath object

    DESCRIPTION
        Get boolean value of XPath object

    RETURNS
        (boolean) truth value
 
    ARGUMENTS
        xobj (IN) XPath object.

    SEE ALSO
        XmlXPathGetObjectType, XmlXPathGetObjectNSetNum,
        XmlXPathGetObjectNSetNode, XmlXPathGetObjectNumber,
        XmlXPathGetObjectBoolean
*/
xmlnode* XmlXPathGetObjectFragment(xpobj *obj);

/*---------------------------------------------------------------------------
    NAME
        XmlXPathSetPrecisionCheck

    FUNCTION
        Enable double precision check

    DESCRIPTION

    RETURNS

    ARGUMENTS
        xctx      (IN)  XPath context object
        precision (IN)  TRUE or FALSE
*/
void XmlXPathSetPrecisionCheck(xpctx *xctx, boolean precision);

/*---------------------------------------------------------------------------
                           Package XSLT - XSLT APIs
  ---------------------------------------------------------------------------*/

/* DATATYPE xmlxslomethod - XSL processor output method
   SEE ALSO XmlXslSetOutputMethod
   Type of output to be produced by the XSLT processor.
*/
typedef enum xmlxslomethod {
    XMLXSL_OUTPUT_UNKNOWN   = 0, /* Not defined */
    XMLXSL_OUTPUT_XML       = 1, /* Produce a Document Fragment */
    XMLXSL_OUTPUT_STREAM    = 2, /* Stream out formatted result */
    XMLXSL_OUTPUT_HTML      = 3  /* Stream out HTML-ized formatted result */
} xmlxslomethod;

/****************************************************************************
                               Interface XSLT
 ****************************************************************************/

/*---------------------------------------------------------------------------
    NAME
        XmlXslCreate

    PURPOSE
        Create an XSLT context

    DESCRIPTION
        Create an XSLT context

    RETURNS
        (xslctx *) XSLT context

    ARGUMENTS
        ctx     (IN)  XML context object.
        xsl     (IN)  XSL stylesheet document object.
        baseuri (IN)  baseuri for include/import/document().
        err     (I/O) the error returned.

    SEE ALSO
        XmlXslDestroy
*/
xslctx *XmlXslCreate(xmlctx *ctx, xmldocnode *xsl, 
                     oratext *baseuri, xmlerr *err);

/*---------------------------------------------------------------------------
    NAME
        XmlXslDestroy

    PURPOSE
        Destroy an XSL context

    DESCRIPTION
        Destroy an XSL context

    RETURNS
        (xmlerr) error code

    ARGUMENTS
        ctx (IN) XSL context

    SEE ALSO
        XmlXslCreate
*/
xmlerr XmlXslDestroy(xslctx *ctx);

/*---------------------------------------------------------------------------
    NAME
        XmlXslSetFlags

    PURPOSE
        Set FLAGs to process XSL on an instance document using
        different behaviour.

    DESCRIPTION
        Do XSL processing on an instance document.  xsl:output is supported 
        only if XmlXslSetOutputStream has been called previously.

    RETURNS
        

    ARGUMENTS
        ctx       (IN) XSL context object.
        flags     (IN) XMLXSL_FALG_EVALPARAMS
*/
void XmlXslSetFlags(xslctx *ctx, ub4 flags);

   /* While processing with-params for xsl:apply-templates,
    * current behaviour doesnt treat source node as current node.
    * instead it process with-params using the node selected by
    * xsl:apply-templates. This will be fixed in later version.
    * Till then support is provided to acheive correct behaviour
    * by setting flags using XmlXslSetFlags(ctx, XMLXSL_FLAG_EVALPARAMS).
    */
#define XMLXSL_FLAG_EVALPARAMS 1

/*---------------------------------------------------------------------------
    NAME
        XmlXslProcess

    PURPOSE
        Do XSL processing on an instance document

    DESCRIPTION
        Do XSL processing on an instance document.  xsl:output is supported 
        only if XmlXslSetOutputStream has been called previously.

    RETURNS
        (xmlerr) error code

    ARGUMENTS
        ctx       (IN) XSL context object.
        xml       (IN) instance document to process
        normalize (IN) if TRUE forces the XSL processor to normalize the doc.
*/
xmlerr XmlXslProcess(xslctx *ctx, xmldocnode *xml, boolean normalize);

/*---------------------------------------------------------------------------
    NAME
        XmlXslSetOutputMethod
        
    PURPOSE
        Set the xslctx output method

    DESCRIPTION
        Set the xslctx output method

    RETURNS
        (xmlerr) error code, XMLERR_SUCC [0] on success.

    ARGUMENTS
        ctx      (IN) XSL context object
        method   (IN) XSL output metod (xmlxslomethod)
*/
xmlerr XmlXslSetOutputMethod(xslctx *ctx, xmlxslomethod method);

/*---------------------------------------------------------------------------
    NAME
        XmlXslSetOutputEncoding
        
    PURPOSE
        Set the xslctx output encoding

    DESCRIPTION
        Set the xslctx output encoding

    RETURNS
        (xmlerr) error code, XMLERR_SUCC [0] on success.

    ARGUMENTS
        ctx      (IN) XSL context object
        encoding (IN) output encoding
*/
xmlerr XmlXslSetOutputEncoding(xslctx *ctx, oratext* encoding);

/*---------------------------------------------------------------------------
    NAME
        XmlXslSetOutputSax
        
    PURPOSE
        Set the xslctx output SAX

    DESCRIPTION
        Set the xslctx output SAX

    RETURNS
        (xmlerr) error code, XMLERR_SUCC [0] on success.

    ARGUMENTS
        ctx      (IN) XSL context object
        saxcb    (IN) SAX callback object
        saxctx   (IN) SAX callback context
*/
xmlerr XmlXslSetOutputSax(xslctx *ctx, xmlsaxcb* saxcb, void *saxctx);

/*---------------------------------------------------------------------------
    NAME
        XmlXslSetOutputStream
        
    PURPOSE
        DEPRECATED Set the xslctx output stream

    DESCRIPTION
        Deprecated, do not use.  See XmlXslSetOutputOraStream()

    RETURNS
        (xmlxsl) error code, XMLXSL_SUCC (0) on success.

    ARGUMENTS
        ctx      (IN) - XSL context object.
        stream   (IN) - output stream object.
*/
xmlerr XmlXslSetOutputStream(xslctx *ctx, xmlostream *stream);

/*---------------------------------------------------------------------------
    NAME
        XmlXslSetOutputOraStream
        
    PURPOSE
        Set the xslctx output stream.

    DESCRIPTION

    RETURNS
        (xmlxsl) error code, XMLXSL_SUCC (0) on success.

    ARGUMENTS
        ctx      (IN) - XSL context object.
        stream   (IN) - output stream object.
*/
xmlerr XmlXslSetOutputOraStream(xslctx *ctx, orastream *stream);

/*---------------------------------------------------------------------------
    NAME
        XmlXslSetOutputDom
        
    PURPOSE
        Set the xslctx output DOM

    DESCRIPTION
        Set the xslctx output DOM

    RETURNS
        (xmlerr) error code, XMLERR_SUCC [0] on success.

    ARGUMENTS
        ctx      (IN) XSL context object
        stream   (IN) output stream object
*/
xmlerr XmlXslSetOutputDom(xslctx *ctx, xmldocnode *doc);

/*---------------------------------------------------------------------------
    NAME
        XmlXslGetTextParam

    PURPOSE
        Get the XSL text parameter value

    DESCRIPTION
        Get the XSL text parameter value

    RETURNS
        (oratext *) parameter value

    ARGUMENTS
        ctx  (IN) XSL context
        name (IN) name of the top-level param

    SEE ALSO
        XmlXslSetTextParam
*/
oratext *XmlXslGetTextParam(xslctx *ctx, oratext *name);

/*---------------------------------------------------------------------------
    NAME
        XmlXslResetAllParams
        
    PURPOSE
        Reset XSL processor parameters.

    DESCRIPTION
        Reset all the top level parameters added

    RETURNS
        (xmlerr) error code, XMLERR_SUCC [0] on success.

    ARGUMENTS
        ctx   (IN) XSL context object

    SEE ALSO
        XmlXslSetTextParam
*/
xmlerr XmlXslResetAllParams(xslctx *ctx);

/*---------------------------------------------------------------------------
    NAME
        XmlXslSetTextParam
        
    PURPOSE
        Set the xslctx output text param.

    DESCRIPTION
        Set the xslctx output text param.

    RETURNS
        (xmlerr) error code, XMLERR_SUCC [0] on success.

    ARGUMENTS
        ctx   (IN) XSL context object
        name  (IN) name of top-level param
        value (IN) value of top-level param

    SEE ALSO
        XmlXslGetTextParam
*/
xmlerr XmlXslSetTextParam(xslctx *ctx, oratext *name, oratext *value);

/*---------------------------------------------------------------------------
    NAME
        XmlXslGetStylesheetDom

    PURPOSE
        Get the XSL stylesheet document

    DESCRIPTION
        Get the XSL stylesheet document

    RETURNS
        (xmldocnode *) stylesheet document

    ARGUMENTS
        ctx (IN) XSL context
*/
xmldocnode *XmlXslGetStylesheetDom(xslctx *ctx);

/*---------------------------------------------------------------------------
    NAME
        XmlXslGetOutput

    PURPOSE
        Get the XSL result fragment

    DESCRIPTION
        Get the XSL result fragment

    RETURNS
        (xmlfragnode *) result fragment

    ARGUMENTS
        ctx (IN) XSL context
*/
xmlfragnode *XmlXslGetOutput(xslctx *ctx);

/*---------------------------------------------------------------------------
    NAME
        XmlXslGetBaseURI

    PURPOSE
        Get the XSL processor base URI

    DESCRIPTION
        Get the XSL processor base URI

    RETURNS
        (oratext *) base URI

    ARGUMENTS
        ctx (IN) XSL context
*/
oratext *XmlXslGetBaseURI(xslctx *ctx);

#endif                                              /* XMLXSL_ORACLE */
