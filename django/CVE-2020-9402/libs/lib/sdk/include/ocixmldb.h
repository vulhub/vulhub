/* Copyright (c) 2003, 2014, Oracle and/or its affiliates. 
All rights reserved.*/
 
/* 
   NAME 
     ocixmldb.h - XDB public functions

   DESCRIPTION 
     This file contains XDB specific public functions required for DOM C-API. 

   RELATED DOCUMENTS 

 
   EXPORT FUNCTION(S) 
   struct xmlctx *OCIXmlDbInitXmlCtx(OCIEnv *, OCISvcCtx *, OCIError *,
                               ocixmldbparam *params, int num_params);

   void    OCIXmlDbFreeXmlCtx(struct xmlctx *xctx);


  ------------------------------------------------------------------------
   EXAMPLES
   
   NOTES

   MODIFIED   (MM/DD/YY)
   stirmizi    06/05/14 - OCI properties for client side binary XML processing
   alejgarc    05/29/14 - 18857660 - added OCIXmlGetDiffBndVersion.
   tojhuan     07/10/13 - Update OCIXmlDr_DEF revision
   tojhuan     06/05/13 - 16318092: add csform and csid to OCIXmlDiffBnd
   tojhuan     12/17/12 - fwd merge 15980280 expose table alias parameters of
                          qmudxRewriteXMLDiffRaw for OCIXmlDbRewriteXMLDiff
   srirkris    06/15/11 - Add OCIXmlDr_DEF 
   srirkris    03/01/11 - OCIXmlDbRewriteXMLDiff definition change
   vmedi       01/14/11 - OCIXmlDbGetFullyQualifiedSchemaUrl
   sipatel     03/08/10 - add lob arg to OCIXmlDbRewriteXMLDiff
   samane      01/20/10 - Bug 9302227
   yifeng      11/05/09 - add OCIXmlDbRewriteXMLDiff
   samane      08/05/09 - Bug 8661204
   ataracha    12/11/03 - remove redundant definitions
   ataracha    05/28/03 - change names
   ataracha    02/18/03 - add oratypes, remove XMLERR_*
   imacky      02/01/03 - remove xml.h; xdbs fix
   ataracha    01/24/03 - use "struct xmlctx" instead of xmlctx
   imacky      01/28/03 - fix XMLERR defs
   ataracha    01/21/03 - ataracha_uni_capi_cleanup
   ataracha    01/09/03 - Creation

*/

#ifndef ORATYPES 
#include <oratypes.h> 
#endif
 
#ifndef OCI_ORACLE
# include <oci.h>
#endif

#ifndef OCIXMLDB_ORACLE
# define OCIXMLDB_ORACLE

/*---------------------------------------------------------------------------
                     PUBLIC TYPES AND CONSTANTS
  ---------------------------------------------------------------------------*/

#ifndef XMLCTX_DEFINED
# define XMLCTX_DEFINED
/* DATATYPE xmlctx - XML top-level context
*/
struct xmlctx; typedef struct xmlctx xmlctx;
#endif


typedef struct OCIXmlDiffBnd {
 oratext *bndnmp;
 ub1      bndnml;
 ub2      bndpos; 
 ub2      bndnum;
 ub2      bnddty;
 void    *bndvalp;
 ub4      bndvallen;
 ub2      bndcsid;
 ub1      bndcsform;
 
} OCIXmlDiffBnd;

struct xmldrctx; typedef struct xmldrctx xmldrctx;

typedef enum 
{
  XCTXINIT_OCIDUR  = 1,
  XCTXINIT_ERRHDL  = 2
} ocixmldbpname;

typedef struct ocixmldbparam
{
  ocixmldbpname   name_ocixmldbparam;
  void           *value_ocixmldbparam;
} ocixmldbparam;

#define NUM_OCIXMLDBPARAMS 2

#define OCIXMLDB_BINDBYNAME 1
#define OCIXMLDB_BINDBYNUM 2

#define OCIMAXXQUBNDLMT 1000

#define OCIXmlDr_DEF 2

/* property names for client-side encoding/decoding of binary XML */
#define OCIXMLDB_ENCODE    "XML_ENCODE_ON"
#define OCIXMLDB_DECODEL   13
#define OCIXMLDB_DECODE    "XML_DECODE_ON"
#define OCIXMLDB_DECODEL   13

/*
 * At version 2, OCIXmlDiffBnd has attributes bndcsid, bndcsform
 */

/*---------------------------------------------------------------------------
                     PRIVATE TYPES AND CONSTANTS
  ---------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------
                           EXPORT FUNCTIONS
  ---------------------------------------------------------------------------*/
/*****************************************************************************
                              DESCRIPTION

-----------------------------OCIXmlDbInitXmlCtx---------------------------------
Name
OCIXmlDbInitXmlCtx
Purpose
To get a xmlctx structure initialized with error-handler and XDB callbacks. 
Syntax
struct xmlctx *OCIXmlDbInitXmlCtx (OCIEnv           *envhp,
                                 OCISvcCtx        *svchp, 
                                 OCIError         *err, 
                                 params_ocixmldb *params,
                                 int               num_params);
Parameters
envhp (IN) - The OCI environment handle
svchp (IN) - The OCI service handle
errhp (IN) - The OCI error handle
params (IN)- This contains the following optional parameters :
   (a) OCIDuration dur (IN - The OCI Duration (Default: OCI_DURATION_SESSION)
   (b) void (*err_handler) (sword, (const oratext *) (IN) - 
       Pointer to the error handling function (Default: null)
num_params (IN) - Number of parameters to be read from parameter params.
                  If the value of num_params exceeds the size of array 
                  "params", unexpected behavior will result.

Returns
A pointer to xmlctx structure, with xdb context, error handler and callbacks 
populated with appropriate values. This is later used for all API calls. NULL
if no database connection available.

-----------------------------OCIXmlDbFreeXmlCtx----------------------------
Name
OCIXmlDbFreeXmlCtx
Pupose
To free any allocations done during OCIXmlDbInitXmlCtx.
Syntax
void OCIXmlDbFreeXmlCtx (struct xmlctx *xctx)
Parameters
xctx (IN) - The xmlctx to terminate
Returns
-
------------------------OCIXmlDbOrastreamFromLob---------------------------
Name
OCIXmlDbOrastreamFromLob
Pupose
To create an orastream from a lob. This orastream can be used by functions like XMLLoadDom().
Syntax
sword OCIXmlDbOrastreamFromLob(OCIError *errhp, xmlctx *xctx,
                               void **stream, OCILobLocator *lobloc)
Parameters
envhp  (IN)     - The OCI environment handle
xctx   (IN)     - XML context
stream (IN/OUT) - A pointer to orastream
lobloc (IN)     - The OCI lob locator
Returns
The orastream created on top of the lob is returned in the parameter 'stream'.
******************************************************************************/

struct xmlctx *OCIXmlDbInitXmlCtx(OCIEnv *, OCISvcCtx *, OCIError *, 
                            ocixmldbparam *, int);

void    OCIXmlDbFreeXmlCtx(struct xmlctx *xctx);
sword OCIXmlDbStreamFromXMLType(OCIError   *errhp, void  **stream, 
                                OCIXMLType *doc, ub4 mode);
sword OCIXmlDbOrastreamFromLob(OCIError *errhp, xmlctx *xctx, 
                               void **stream, OCILobLocator *lobloc);
sword OCIXmlDbStreamRead(OCIError   *errhp, void  *stream, 
                         void *bufp, sb8 *len, ub4 mode);
sword OCIXmlDbStreamClose(OCIError *errhp, void *stream);

/*---------------------------------------------------------------------------
                          INTERNAL FUNCTIONS
  ---------------------------------------------------------------------------*/
/* This function is for internal usage only */
sword OCIXmlDbRewriteXMLDiff(OCIEnv *envhp, OCIError *errhp, OCISvcCtx *svchp, 
                             xmldrctx *xctx , oratext* colname, ub4 colnamelen,  
                             const void* xmldiff, ub4 xmldifflen, 
                             OCILobLocator *xdiff_locator, oratext** updstmt, 
                             ub2 binditerator, ub2 *colvalbndcount,  
                             OCIXmlDiffBnd **bindlist,
                             oratext *obj_tab_prefix, ub2 obj_tab_prefix_len);

struct xmldrctx *OCIXmlInitDRCtx(OCIEnv *env, OCISvcCtx *svc, OCIError *err, 
                                 ub1 bindtyp);

void OCIXmlFreeDRCtx(xmldrctx *xctx);

ub1 OCIXmlGetDiffBndVersion();

sword OCIXmlDbGetFullyQualifiedSchemaUrl(OCIError *errhp,
                                         oratext  *schema_url,
                                         ub2       schema_url_len,
                                         oratext  *schema_owner,
                                         ub2       schema_owner_len,
                                         oratext **fq_schema_url,
                                         ub4      *fs_schema_url_len);

void *OCIXmlDbMemCallback(void *ctx, size_t size);

#endif                                              /* OCIXMLDB_ORACLE */
