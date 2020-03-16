/*
 * 
 */

/* Copyright (c) 1998, 2005, Oracle. All rights reserved.  */
 
/* 
   NAME 
     oci8dp.h - OCI: Direct Path API interface prototypes.

   DESCRIPTION 
     Public types, constants, and interfaces to the direct path API.

   RELATED DOCUMENTS 
 
   NOTES
     This file is not directly included by the application, this file
     is included by "oci.h", which the application should include.

   MODIFIED   (MM/DD/YY)
   dmukhin     06/29/05 - ANSI prototypes; miscellaneous cleanup 
   rphillip    02/27/04 - Add OCI_DIRPATH_COL_ERROR 
   srseshad    03/12/03 - convert oci public api to ansi
   msakayed    10/28/02 - Bug #2643907: add OCI_ATTR_DIRPATH_SKIPINDEX_METHOD
   cmlim       04/13/01 - remove OCIDirPathStreamToStream - not used by dpapi
   cmlim       04/02/01 - OCI_DIRPATH_EXPR_OPQ_SQL_FN to OCI_DIRPATH_EXPR_SQL
   ebatbout    01/22/01 - PARTIAL value for OCIDirPathDataSave action parameter
   cmlim       07/20/00 - support opaques/sql strings in 8.2 dpapi
   cmlim       08/14/00 - support refs in 8.2 dpapi
   cmlim       04/17/00 - add defines for OCIDirPathFuncCtx handle & OCI_ATTR_D
   whe         09/01/99 - 976457:check __cplusplus for C++ code
   abrumm      04/16/99 - dpapi: more attributes
   abrumm      02/26/99 - add defines for DataSave action
   abrumm      10/04/98 - clen must be a ub4
   abrumm      05/27/98 - add column array flag values
   abrumm      05/12/98 - direct path api support
   abrumm      03/31/98 - OCI direct path interface support
   abrumm      03/18/98 - Creation

*/

#ifndef OCI8DP_ORACLE
# define OCI8DP_ORACLE

#ifndef ORATYPES
#include <oratypes.h>
#endif

#ifndef OCIDFN
#include <ocidfn.h>
#endif

#ifndef OCI_ORACLE
#include <oci.h>
#endif


/*---------------------------------------------------------------------------
                     PUBLIC TYPES AND CONSTANTS
  ---------------------------------------------------------------------------*/

/*----- Handles and descriptors for direct path operations (OCIDirPath*) ----*/

typedef struct OCIDirPathCtx      OCIDirPathCtx;                  /* context */
typedef struct OCIDirPathFuncCtx  OCIDirPathFuncCtx;     /* function context */
typedef struct OCIDirPathColArray OCIDirPathColArray;        /* column array */
typedef struct OCIDirPathStream   OCIDirPathStream;                /* stream */
typedef struct OCIDirPathDesc     OCIDirPathDesc;  /* direct path descriptor */

                 /*----- Defines for Direct Path Options -----*/

                 /* values for OCI_ATTR_DIRPATH_MODE attribute */
#define OCI_DIRPATH_LOAD         1             /* direct path load operation */
#define OCI_DIRPATH_UNLOAD       2           /* direct path unload operation */
#define OCI_DIRPATH_CONVERT      3     /* direct path convert only operation */

    /*----- values for OCI_ATTR_DIRPATH_INDEX_MAINT_METHOD attribute -----*/
#define OCI_DIRPATH_INDEX_MAINT_SINGLE_ROW     1

/* Note that there are two attributes dealing with index maintenance -
 * OCI_ATTR_DIRPATH_INDEX_MAINT_METHOD and OCI_ATTR_DIRPATH_SKIPINDEX_METHOD.
 * OCI_ATTR_DIRPATH_SKIPINDEX_METHOD exists to isolate the behavior for
 * skipping index maintenance since maintenance of unusable indexes is 
 * orthogonal to that of single row insertion.
 * For backwards compatibility we still allow users to specify skip
 * methods in OCI_ATTR_DIRPATH_INDEX_MAINT_METHOD so make sure the
 * enumerations for the two attributes are distinct.
 */
    /*----- values for OCI_ATTR_DIRPATH_SKIPINDEX_METHOD attribute -----*/
#define OCI_DIRPATH_INDEX_MAINT_SKIP_UNUSABLE       2
#define OCI_DIRPATH_INDEX_MAINT_DONT_SKIP_UNUSABLE  3
#define OCI_DIRPATH_INDEX_MAINT_SKIP_ALL            4

           /* values for OCI_ATTR_STATE attribute of OCIDirPathCtx */
#define OCI_DIRPATH_NORMAL       1     /* can accept rows, last row complete */
#define OCI_DIRPATH_PARTIAL      2                   /* last row was partial */
#define OCI_DIRPATH_NOT_PREPARED 3    /* direct path context is not prepared */

     /*----- values for cflg argument to OCIDirpathColArrayEntrySet -----*/
#define OCI_DIRPATH_COL_COMPLETE 0                /* column data is complete */
#define OCI_DIRPATH_COL_NULL     1                         /* column is null */
#define OCI_DIRPATH_COL_PARTIAL  2                 /* column data is partial */
#define OCI_DIRPATH_COL_ERROR    3               /* column error, ignore row */
          /*----- values for action parameter to OCIDirPathDataSave -----*/
#define OCI_DIRPATH_DATASAVE_SAVEONLY 0              /* data save point only */
#define OCI_DIRPATH_DATASAVE_FINISH   1           /* execute finishing logic */
/* save portion of input data (before space error occurred) and finish */
#define OCI_DIRPATH_DATASAVE_PARTIAL  2       

  /*- OCI_ATTR_DIRPATH_EXPR_TYPE values (describes OCI_ATTR_NAME expr type) -*/
#define OCI_DIRPATH_EXPR_OBJ_CONSTR  1      /* NAME is an object constructor */
#define OCI_DIRPATH_EXPR_SQL         2  /* NAME is an opaque or sql function */
#define OCI_DIRPATH_EXPR_REF_TBLNAME 3 /* NAME is table name if ref is scoped*/


/*---------------------------------------------------------------------------
                           PUBLIC FUNCTIONS
  ---------------------------------------------------------------------------*/

/*------------------------ OCIDirPathCtx Operations -------------------------*/

/*
  NAME
    OCIDirPathAbort - OCI: Abort a direct path operation.

  DESCRIPTION
    Aborts a direct path operation.  Upon successful completion
    the direct path context is no longer valid.

  RETURNS
    An OCI error code, Oracle errors are returned via the error handle.
  NOTES
 */
sword
OCIDirPathAbort( OCIDirPathCtx *dpctx, OCIError *errhp );

/*
  NAME
    OCIDirPathDataSave - OCI: Execute a data save point.

  DESCRIPTION
    Successful return of this function indicates that a data save
    point has been executed.

  RETURNS
    An OCI error code, Oracle errors are returned via the error handle.
  NOTES
 */
sword
OCIDirPathDataSave( OCIDirPathCtx *dpctx, OCIError *errhp, ub4 action );

/*
  NAME
    OCIDirPathFinish - OCI: Finish a direct path operation.

  DESCRIPTION
    Finishes a direct path operation.

  RETURNS
    An OCI error code, Oracle errors are returned via the error handle.
  NOTES
 */
sword
OCIDirPathFinish( OCIDirPathCtx *dpctx, OCIError  *errhp );

/*
  NAME
    OCIDirPathFlushRow - OCI: Flush a partial row from the server.

  DESCRIPTION
    Flushes a partially loaded row from the server.

  RETURNS
    An OCI error code, Oracle errors are returned via the error handle.
  NOTES
 */
sword
OCIDirPathFlushRow( OCIDirPathCtx *dpctx, OCIError  *errhp );

/*
  NAME
    OCIDirPathPrepare - OCI: Prepare a direct path operation.

  DESCRIPTION
    Prepares a table/partition for a direct path operation.

  RETURNS
    An OCI error code, Oracle errors are returned via the error handle.
  NOTES
 */
sword
OCIDirPathPrepare( OCIDirPathCtx *dpctx, OCISvcCtx *svchp,
                      OCIError  *errhp );

/*
  NAME
    OCIDirPathLoadStream - OCI: Load a direct path stream.

  DESCRIPTION
    Load a direct path stream to the object associated with
    the direct path context.

  RETURNS
    An OCI error code, Oracle errors are returned via the error handle.
  NOTES
 */
sword
OCIDirPathLoadStream( OCIDirPathCtx *dpctx, OCIDirPathStream *dpstr,
                         OCIError      *errhp );


/*---------------------- OCIDirPathColArray Operations ----------------------*/

/*
  NAME
    OCIDirPathColArrayEntryGet - OCI:  Get column array entry.

  DESCRIPTION
    Column array function which is used to get a specified entry in
    a column array.

  RETURNS
    An OCI error code, Oracle errors are returned via the error handle.
  NOTES
 */
sword
OCIDirPathColArrayEntryGet( OCIDirPathColArray *dpca, OCIError *errhp,
                              ub4 rownum, ub2 colIdx, ub1 **cvalpp, ub4 *clenp,
                              ub1 *cflgp );

/*
  NAME
    OCIDirPathColArrayEntrySet - OCI:  Set column array entry.

  DESCRIPTION
    Column array function which is used to set a specified entry in
    a column array.

  RETURNS
    An OCI error code, Oracle errors are returned via the error handle.
  NOTES
 */
sword
OCIDirPathColArrayEntrySet( OCIDirPathColArray *dpca, OCIError *errhp,
                               ub4 rownum, ub2 colIdx, ub1 *cvalp, ub4 clen,
                               ub1 cflg );

/*
  NAME
    OCIDirPathColArrayRowGet - OCI:  Get column array row pointers.

  DESCRIPTION
    Column array function which is used to get the base row pointers
    for a specified row in a column array.
    To be used in lieu of OCIDirPathColArrayEntryGet() and
    OCIDirPathColArrayEntrySet().

  RETURNS
    An OCI error code, Oracle errors are returned via the error handle.
  NOTES
 */
sword
OCIDirPathColArrayRowGet( OCIDirPathColArray *dpca, OCIError *errhp,
                         ub4 rownum, ub1 ***cvalppp, ub4 **clenpp,
                         ub1 **cflgpp );

/*
  NAME
    OCIDirPathColArrayReset - OCI: Reset Column Array State

  DESCRIPTION
    Function which resets the column array state.

  RETURNS
    An OCI error code, Oracle errors are returned via the error handle.
  NOTES
    Resetting the column array state is necessary when piecing in a large
    column and an error occurs in the middle of loading the column.
 */
sword
OCIDirPathColArrayReset( OCIDirPathColArray *dpca, OCIError *errhp );

/*
  NAME
    OCIDirPathColArrayToStream - OCI: Convert Column Array to Stream Format.

  DESCRIPTION
    Convert from column array format to stream format which is suitable
    for loading via OCIDirPathLoadStream().

  RETURNS
    An OCI error code, Oracle errors are returned via the error handle.
  NOTES
 */
sword
OCIDirPathColArrayToStream( OCIDirPathColArray *dpca,  OCIDirPathCtx *dpctx,
                               OCIDirPathStream   *dpstr, OCIError      *errhp,
                               ub4 rowcnt, ub4 rowoff );



/*----------------------- OCIDirPathStream Operations -----------------------*/

/*
  NAME
    OCIDirPathStreamReset - OCI:

  DESCRIPTION

  RETURNS
    An OCI error code, Oracle errors are returned via the error handle.
  NOTES
 */
sword
OCIDirPathStreamReset( OCIDirPathStream *dpstr, OCIError *errhp );

#endif                                              /* OCI8DP_ORACLE */
