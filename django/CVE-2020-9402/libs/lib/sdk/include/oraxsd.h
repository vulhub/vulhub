/* $RCSfile: oraxsd.h $ $Date: 2016/03/15 21:33:26 $
 Copyright (c) 1999, 2016, Oracle and/or its affiliates. All rights reserved.

   NAME
     oraxsd.h - XML Schema

   DESCRIPTION
     Public header for the XML Schema

   PUBLIC FUNCTION(S) 
     schemaInitialize
     schemaLoad
     schemaValidate
     schemaTerminate
*/

#ifndef ORAXSD_ORACLE
# define ORAXSD_ORACLE

#ifndef ORATYPES_ORACLE
# include <oratypes.h>
#endif

#ifndef ORAXML_ORACLE
# include <oraxml.h>
#endif

/*---------------------------------------------------------------------------
                     PUBLIC TYPES AND CONSTANTS
  ---------------------------------------------------------------------------*/

/* DATATYPE loadflags - XSD load flags
    Flag bits for schemaLoadFlags
*/
#define XSD_LOAD_FORCE_ASSESSMENT   0x01  /* assess loaded schema docs 
                                           * immediately
                                           */
/* DATATYPE initflags - XSD initialization flags
    Flag bits for schemaInitializeFlags
*/
#define XSD_INIT_NO_CONV            0x01  /* avoid char conversion */

/*---------------------------------------------------------------------------
                            XML opaque structures
  ---------------------------------------------------------------------------*/

#ifndef XSDCTX_DEFINED
# define XSDCTX_DEFINED
struct xsdctx; typedef struct xsdctx xsdctx;
#endif

typedef struct xsd     xsd;

/*---------------------------------------------------------------------------
                                API
  ---------------------------------------------------------------------------*/

/*
 * NAME:
 *   schemaInitialize
 * PARAMETERS:
 *   ctx (IN)  - XML (instance document?) context
 *   err (OUT) - failure code
 * RETURNS:
 *   (xsdctx *) schema context on success, (xsdctx *) 0 on failure with
 *   err set to indicate the problem.
 * DESCRIPTION:
 *   Create and return a schema context.  The incoming ctx is needed to
 *   perform memory allocation.
 */
xsdctx *schemaInitialize(xmlctx *ctx, uword *err);

/*
 * NAME:
 *   schemaInitializeFlags
 * PARAMETERS:
 *   ctx (IN)  - XML (instance document?) context
 *   err (OUT) - failure code
 *   initflags(IN) - initialization flags
 * RETURNS:
 *   (xsdctx *) schema context on success, (xsdctx *) 0 on failure with
 *   err set to indicate the problem.
 * DESCRIPTION:
 *   Create and return a schema context.  The incoming ctx is needed to
 *   perform memory allocation.
 */
xsdctx *schemaInitializeFlags(xmlctx *ctx, uword *err, ub4 initflags);

/*
 * NAME:
 *   schemaSetErrHandler
 * PARAMETERS:
 *   ctx     (IN)  - schema context
 *   msghdlr (IN)  - user-define message handler
 *   msgctx  (IN)  - user-define message handler context
 * RETURNS:
 *   (uword) error code
 * DESCRIPTION:
 *   Sets an error-message-handling function and its associated context in a
 *   schema context.
 */
uword schemaSetErrHandler(xsdctx *ctx, XML_MSGHDLRF((*msghdlr)),
                          void *msgctx);
/*
 * NAME:
 *   schemaErrWhere
 * PARAMETERS:
 *   ctx     (IN)  - schema context
 *   line    (OUT) - line# where error occured
 *   path    (OUT) - URL/filespec where error occured
 * RETURNS:
 *   (uword ) error code
 * DESCRIPTION:
 *   Returns the location (line#, path) where an error occured.
 */
uword schemaErrWhere(xsdctx *ctx, ub4 *line, oratext **path);

/*
 * NAME:
 *   schemaLoad
 * PARAMETERS:
 *   ctx    (IN)  - schema context
 *   uri    (IN)  - URI of schema
 *   nsp    (IN)  - namespace of schema (optional)
 *   schema (OUT) - returned pointer to loaded schema
 * RETURNS:
 *   (uword) error code
 * DESCRIPTION:
 *   Loads a schema; sets the user's schema pointer to it.
 */
uword schemaLoad(xsdctx *ctx, oratext *uri, oratext *nsp, xsd **schema);

/*
 * NAME:
 *   schemaLoadFlags
 * PARAMETERS:
 *   ctx    (IN)  - schema context
 *   uri    (IN)  - URI of schema
 *   nsp    (IN)  - namespace of schema (optional)
 *   schema (OUT) - returned pointer to loaded schema
 *   flags  (IN)  - loading flags 
 * RETURNS:
 *   (uword) error code
 * DESCRIPTION:
 *   Loads a schema; sets the user's schema pointer to it.
 */
uword schemaLoadFlags(xsdctx *ctx, oratext *uri, oratext *nsp, xsd **schema,
                      ub4 flags);

/*
 * NAME:
 *   schemaValidate
 * PARAMETERS:
 *   scctx (IN) - schema context
 *   root  (IN) - root node to validate
 *   url   (IN) - optional default schema
 * RETURNS:
 *   (uword) error code
 * DESCRIPTION:
 *   Validates a document against a schema.  If provided, the named
 *   schema is loaded and will be the default.
 */
uword schemaValidate(xsdctx *scctx, xmlnode *root, oratext *schema);

/*
 * NAME:
 *   schemaClean
 * PARAMETERS:
 *   scctx (IN) - schema context
 * RETURNS:
 *   (uword)   error code
 * DESCRIPTION:
 *   Remove loaded schemas and recycle the schema context.
 */
uword schemaClean(xsdctx *scctx);

/*
 * NAME:
 *   schemaTerminate
 * PARAMETERS:
 *   scctx (IN) - schema context
 * RETURNS:
 *   (void)
 * DESCRIPTION:
 *   Release storage associated with schemas.
 */
void schemaTerminate(xsdctx *scctx);

/*
 * NAME:
 *   schemaTarget
 * PARAMETERS:
 *   schema (IN) - schema data strucure (e.g., returned by schemaLoad())
 * RETURNS:
 *   Target namespace URI
 * DESCRIPTION:
 *   Returns associated target namespace URI.
 */
oratext *schemaTarget(xsd *schema);

#endif  /* ORAXSD_ORACLE */
