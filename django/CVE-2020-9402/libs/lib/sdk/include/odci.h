/*
 * 
 */

/* Copyright (c) 1998, 2018, Oracle and/or its affiliates. 
All rights reserved.*/
 
/* 
   NAME 
     odci.h - Oracle Data Cartridge Interface definitions

   DESCRIPTION 
     This file contains Oracle Data Cartridge Interface definitions. These
     include the ODCI Types and Constants.

   RELATED DOCUMENTS 
 
   INSPECTION STATUS 
     Inspection date: 
     Inspection status: 
     Estimated increasing cost defects per page: 
     Rule sets: 
 
   ACCEPTANCE REVIEW STATUS 
     Review date: 
     Review status: 
     Reviewers: 
 
   PUBLIC FUNCTION(S) 
     None.

   PRIVATE FUNCTION(S)
     None.

   EXAMPLES

   NOTES
     - The constants defined here are replica of the constants defined 
       in ODCIConst Package defined as part of catodci.sql. If you change
       these do make the similar change in catodci.sql.

   MODIFIED   (MM/DD/YY)
   rphillip    11/27/18 - XbranchMerge rphillip_bug-28449437_main from main
   rphillip    03/21/18 - Bug 27254684 Add ODCI_EXTTABLE_OPEN_FLAGS_OLD_CA
   jstenois    07/25/17 - 26527267: track xt queries that reference no cols
   rodfuent    07/13/17 - RTI 20418540: Add mutable fields to trusted types
   timedina    01/04/17 - Bug 25261796: allow Spatial's index in sharded tables
   rodfuent    10/18/16 - bug 21692960: Trusted callout types
   mwjohnso    09/14/16 - Bug 24307258: add ODCI_EXTTABLE_OPEN_FLAGS_VALIDATE
   cochang     04/29/16 - Bug 22627249: add ODCIExtTableClose Flag
   mjjones     03/21/16 - Bug 22950991: forward port Predicate PushDown txns
                          mjjones_bigdatasql_ppd 
                          mjjones_bug-22486603 
                          mjjones_bug-22519704 
                          mjjones_bug-22761713 
                          mjjones_lrg-18956213 
   cochang     03/15/15 - Proj 47082: Partitioned external table partition
                          pruning
   sdoraisw    01/28/15 - proj 47082:add ODCIExtTableInfo.AccessParmMod
   echong      12/05/14 - local domain index support on composite partitioned
                          tables
   acolunga    10/16/14 - bug 19354925: add ODCI_INDEX_NAMED_PARTS
   echong      06/30/14 - support domain index on ref-partitioned tables
   dpotapov    01/12/16 - Add internal table flag
   abrumm      05/12/14 - LRG 10020665: add ODCI_EXTTABLE_OPEN_FLAGS_SILENT
   evoss       04/08/14 - fix public rdbms dependencies
   abrumm      03/20/14 - ExaDoop/BigSQL: add new ODCI_EXTTABLE_FETCH flags
   evoss       03/22/14 - lint
   abrumm      03/04/14 - ExaDoop/BigSQL: add new ODCI_EXTTABLE_OPEN flags
   dpotapov    03/06/14 - xtss merge
   echong      08/26/13 - add event flag for row migration
   spsundar    09/29/11 - add a new flag ODCI_USER_PARAM_STR
   yhu         02/03/10 - add a new flag ODCI_INDEX_UGI
   spsundar    09/13/07 - 
   yhu         06/02/06 - add callproperty for statistics 
   yhu         05/22/06 - add ODCI_NODATA to speed rebuild empty index or ind. 
                          part. 
   srirkris    05/09/06 - change ODCIOrderByInfo_ind
   srirkris    02/06/06 - add definitions for CDI query.
   spsundar    02/17/06 - add fields/types for system managed domain idx
   yhu         02/08/06 - add RenameCol Na d RenameTopADT 
   yhu         03/11/05 - add flags for rename column and rename table 
   spsundar    11/28/05 - add fields/types for composite domain idx
   yhu         12/06/05 - mapping table for local text indexes 
   dmukhin     06/29/05 - ANSI prototypes; miscellaneous cleanup 
   ayoaz       04/21/03 - add CursorNum to ODCIEnv
   abrumm      12/30/02 - Bug #2223225: add define for
                          ODCI_ARG_DESC_LIST_MAXSIZE
   ayoaz       10/14/02 - Add Cardinality to ODCIArgDesc
   ayoaz       09/11/02 - add ODCIQueryInfo to ODCIIndexCtx
   yhu         09/19/02 - add ODCI_DEBUGGING_ON for ODCIEnv.EnvFlags
   hsbedi      10/10/02 - add object number into ODCIExtTableInfo
   ayoaz       08/30/02 - add ODCITable2 types
   tchorma     07/29/02 - Add ODCIFuncCallInfo type for WITH COLUMN CONTEXT
   hsbedi      06/29/02 - External table populate
   yhu         07/20/01 - add parallel degree in ODCIIndexInfo.
   abrumm      02/20/01 - ODCIExtTableInfo: add AccessParmBlob attribute
   abrumm      01/18/01 - ODCIExtTableInfo: add default directory
   spsundar    08/24/00 - Update attrbiute positions
   abrumm      08/04/00 - external tables changes: ODCIExtTableInfo, constants
   tchorma     09/11/00 - Add return code ODCI_FATAL
   tchorma     08/08/00 - Add Update Block References Option for Alter Index
   ayoaz       08/01/00 - Add ODCI_AGGREGATE_REUSE_CTX
   spsundar    06/19/00 - add ODCIEnv type
   abrumm      06/27/00 - add defines for ODCIExtTable flags
   abrumm      06/04/00 - external tables: ODCIExtTableInfo change; add ODCIEnv
   ddas        04/28/00 - extensible optimizer enhancements for 8.2
   yhu         06/05/00 - add a bit in IndexInfoFlags for trans. tblspc
   yhu         04/10/00 - add ODCIPartInfo & remove ODCIIndexPartList
   abrumm      03/29/00 - external table support
   spsundar    02/14/00 - update odci definitions for 8.2
   nagarwal    03/07/99 - bug# 838308 - set estimate_stats=1
   rmurthy     11/09/98 - add blocking flag
   ddas        10/31/98 - add ODCI_QUERY_SORT_ASC and ODCI_QUERY_SORT_DESC
   ddas        05/26/98 - fix ODCIPredInfo flag bits
   rmurthy     06/03/98 - add macro for RegularCall
   spsundar    05/08/98 - add constants related to ODCIIndexAlter options
   rmurthy     04/30/98 - remove include s.h
   rmurthy     04/20/98 - name fixes
   rmurthy     04/13/98 - add C mappings for odci types
   alsrivas    04/10/98 - adding defines for ODCI_INDEX1
   jsriniva    04/04/98 - Creation

*/

#ifndef OCI_ORACLE
# include <oci.h>
#endif
#ifndef ODCI_ORACLE
# define ODCI_ORACLE
# define ODCI_ORACLE_DEFINED

/*---------------------------------------------------------------------------*/
/*                         SHORT NAMES SUPPORT SECTION                       */
/*---------------------------------------------------------------------------*/

#ifdef SLSHORTNAME

/* The following are short names that are only supported on IBM mainframes
 *   with the SLSHORTNAME defined.
 * With this all subsequent long names will actually be substituted with
 *  the short names here
 */

#define ODCIColInfo_ref             odcicir
#define ODCIColInfoList             odcicil
#define ODCIColInfoList2            odcicil2
#define ODCIIndexInfo_ref           odciiir
#define ODCIPredInfo_ref            odcipir
#define ODCIRidList                 odcirl
#define ODCIIndexCtx_ref            odciicr
#define ODCIObject_ref              odcior
#define ODCIObjectList              odciol
#define ODCIQueryInfo_ref           odciqir
#define ODCIFuncInfo_ref            odcifir
#define ODCICost_ref                odcicr
#define ODCIArgDesc_ref             odciadr
#define ODCIArgDescList             odciadl
#define ODCIStatsOptions_ref        odcisor
#define ODCIColInfo                 odcici
#define ODCIColInfo_ind             odcicii
#define ODCIIndexInfo               odciii
#define ODCIIndexInfo_ind           odciiii
#define ODCIPredInfo                odcipi
#define ODCIPredInfo_ind            odcipii
#define ODCIIndexCtx                odciic
#define ODCIIndexCtx_ind            odciici
#define ODCIObject                  odcio
#define ODCIObject_ind              odcioi
#define ODCIQueryInfo               odciqi
#define ODCIQueryInfo_ind           odciqii
#define ODCIFuncInfo                odcifi
#define ODCIFuncInfo_infd           odcifii
#define ODCICost                    odcic
#define ODCICost_ind                odcici
#define ODCIArgDesc                 odciad
#define ODCIArgDesc_ind             odciadi
#define ODCIStatsOptions            odciso
#define ODCIStatsOptions_ind        odcisoi
#define ODCIPartInfo                odcipti
#define ODCIPartInfo_ind            odciptii
#define ODCIPartInfo_ref            odciptir
#define ODCIExtTableInfo            odcixt
#define ODCIExtTableInfo_ind        odcixti
#define ODCIExtTableInfo_ref        odcixtr
#define ODCIExtTableQCInfo          odcixq
#define ODCIExtTableQCInfo_ind      odcixqi
#define ODCIExtTableQCInfo_ref      odcixqr
#define ODCIFuncCallInfo            odcifc
#define ODCIFuncCall_ind            odcifci
#define ODCIFuncCall_ref            odcifcr
#define ODCIColValList              odcicvl
#define ODCIColArrayList            odcical
#define ODCIFilterInfoList          odciflil
#define ODCIOrderByInfoList         odciobil
#define ODCIFilterInfo_ref          odciflir
#define ODCIOrderByInfo_ref         odciobir
#define ODCICompQueryInfo_ref       odcicqir
#define ODCIFilterInfo              odcifli
#define ODCIOrderByInfo             odciobi
#define ODCICompQueryInfo           odcicqi
#define ODCIFilterInfo_ind          odciflii
#define ODCIOrderByInfo_ind         odciobii
#define ODCICompQueryInfo_ind       odcicqii

#endif                                                        /* SLSHORTNAME */

/*---------------------------------------------------------------------------
                     PUBLIC TYPES AND CONSTANTS
  ---------------------------------------------------------------------------*/

/* Constants for Return Status */
#define ODCI_SUCCESS             0
#define ODCI_ERROR               1
#define ODCI_WARNING             2
#define ODCI_ERROR_CONTINUE      3
#define ODCI_FATAL               4

/* Constants for ODCIPredInfo.Flags */
#define ODCI_PRED_EXACT_MATCH    0x0001
#define ODCI_PRED_PREFIX_MATCH   0x0002
#define ODCI_PRED_INCLUDE_START  0x0004
#define ODCI_PRED_INCLUDE_STOP   0x0008
#define ODCI_PRED_OBJECT_FUNC    0x0010
#define ODCI_PRED_OBJECT_PKG     0x0020
#define ODCI_PRED_OBJECT_TYPE    0x0040
#define ODCI_PRED_MULTI_TABLE    0x0080
#define ODCI_PRED_NOT_EQUAL      0x0100

/* Constants for QueryInfo.Flags */
#define ODCI_QUERY_FIRST_ROWS    0x01
#define ODCI_QUERY_ALL_ROWS      0x02
#define ODCI_QUERY_SORT_ASC      0x04
#define ODCI_QUERY_SORT_DESC     0x08
#define ODCI_QUERY_BLOCKING      0x10

/* Constants for ScnFlg(Func /w Index Context) */
#define ODCI_CLEANUP_CALL        1
#define ODCI_REGULAR_CALL        2

/* Constants for ODCIFuncInfo.Flags */
#define ODCI_OBJECT_FUNC         0x01
#define ODCI_OBJECT_PKG          0x02
#define ODCI_OBJECT_TYPE         0x04

/* Constants for ODCIArgDesc.ArgType */
#define ODCI_ARG_OTHER           1
#define ODCI_ARG_COL             2                                 /* column */
#define ODCI_ARG_LIT             3                                /* literal */
#define ODCI_ARG_ATTR            4                       /* object attribute */
#define ODCI_ARG_NULL            5
#define ODCI_ARG_CURSOR          6

/* Maximum size of ODCIArgDescList array */
#define ODCI_ARG_DESC_LIST_MAXSIZE 32767

/* Constants for ODCIStatsOptions.Options */
#define ODCI_PERCENT_OPTION      1
#define ODCI_ROW_OPTION          2

/* Constants for ODCIStatsOptions.Flags */
#define ODCI_ESTIMATE_STATS     0x01
#define ODCI_COMPUTE_STATS      0x02
#define ODCI_VALIDATE           0x04

/* Constants for ODCIIndexAlter parameter alter_option */
#define ODCI_ALTIDX_NONE               0
#define ODCI_ALTIDX_RENAME             1
#define ODCI_ALTIDX_REBUILD            2
#define ODCI_ALTIDX_REBUILD_ONL        3
#define ODCI_ALTIDX_MODIFY_COL         4
#define ODCI_ALTIDX_UPDATE_BLOCK_REFS  5
#define ODCI_ALTIDX_RENAME_COL         6
#define ODCI_ALTIDX_RENAME_TAB         7
#define ODCI_ALTIDX_MIGRATE            8

/* Constants for ODCIIndexInfo.IndexInfoFlags */
#define ODCI_INDEX_LOCAL         0x0001
#define ODCI_INDEX_RANGE_PARTN   0x0002
#define ODCI_INDEX_HASH_PARTN    0x0004
#define ODCI_INDEX_ONLINE        0x0008
#define ODCI_INDEX_PARALLEL      0x0010
#define ODCI_INDEX_UNUSABLE      0x0020
#define ODCI_INDEX_ONIOT         0x0040
#define ODCI_INDEX_TRANS_TBLSPC  0x0080
#define ODCI_INDEX_FUNCTION_IDX  0x0100
#define ODCI_INDEX_LIST_PARTN    0x0200
#define ODCI_INDEX_UGI           0x0400
#define ODCI_INDEX_REF_PARTN     0x0800
#define ODCI_INDEX_NAMED_PARTS   0x1000
#define ODCI_INDEX_COMP_PARTN    0x2000
#define ODCI_INDEX_SUB_PARTN     0x4000
#define ODCI_INDEX_SHARD_TABLE   0x8000

/* Constants for ODCIIndexInfo.IndexParaDegree */
#define ODCI_INDEX_DEFAULT_DEGREE 32767

/* Constants for ODCIEnv.EnvFlags */
#define ODCI_DEBUGGING_ON        0x01
#define ODCI_NODATA              0x02
#define ODCI_USER_PARAM_STR      0x04
#define ODCI_ROWMIG              0x08
#define ODCI_IKEYCHG             0x10
#define ODCI_SHARD_COORDINATOR   0x20

/* Constants for ODCIEnv.CallProperty */
#define ODCI_CALL_NONE           0
#define ODCI_CALL_FIRST          1
#define ODCI_CALL_INTERMEDIATE   2
#define ODCI_CALL_FINAL          3
#define ODCI_CALL_REBUILD_INDEX  4
#define ODCI_CALL_REBUILD_PMO    5
#define ODCI_CALL_STATSGLOBAL    6
#define ODCI_CALL_STATSGLOBALANDPARTITION    7
#define ODCI_CALL_STATSPARTITION             8

/* Constants for ODCIExtTableInfo.OpCode */
#define ODCI_EXTTABLE_INFO_OPCODE_FETCH           1
#define ODCI_EXTTABLE_INFO_OPCODE_POPULATE        2

/* Constants (bit definitions) for ODCIExtTableInfo.Flag */
    /* sampling type: row or block */
#define ODCI_EXTTABLE_INFO_FLAG_SAMPLE           0x00000001
#define ODCI_EXTTABLE_INFO_FLAG_SAMPLE_BLOCK     0x00000002
    /* AccessParmClob, AccessParmBlob discriminator */
#define ODCI_EXTTABLE_INFO_FLAG_ACCESS_PARM_CLOB 0x00000004
#define ODCI_EXTTABLE_INFO_FLAG_ACCESS_PARM_BLOB 0x00000008
    /* set if no columns values are referenced */
#define ODCI_EXTTABLE_INFO_FLAG_NO_REF_COL       0x00000010

/* Constants for ODCIExtTableInfo.IntraSourceConcurrency */
#define ODCI_TRUE  1
#define ODCI_FALSE 0

/* Constants (bit definitions) for ODCIExtTable{Open,Fetch,Populate,Close}
 * Flag argument.
 */
/* ------------------------- ODCIExtTableOpen Flags ------------------------ */
                                              /* Query Coordinator Mode call */
#define ODCI_EXTTABLE_OPEN_FLAGS_QC              0x00000001

                                                         /* Shadow Mode call */
#define ODCI_EXTTABLE_OPEN_FLAGS_SHADOW          0x00000002 

                                                         /* Slave  Mode call */
#define ODCI_EXTTABLE_OPEN_FLAGS_SLAVE           0x00000004

                                          /* GET GRANULES from access driver */
#define ODCI_EXTTABLE_OPEN_FLAGS_GET_GRANULES    0x00000008

                                                /* GET Access Driver ConTeXt */
#define ODCI_EXTTABLE_OPEN_FLAGS_GET_AD_CTX      0x00000010

                                                /* SET Access Driver ConTeXt */
#define ODCI_EXTTABLE_OPEN_FLAGS_SET_AD_CTX      0x00000020

                                                      /* GET Storage Objects */
#define ODCI_EXTTABLE_OPEN_FLAGS_GET_STGOBJ      0x00000040

                                         /* GET Storage Object Sizes (bytes) */
#define ODCI_EXTTABLE_OPEN_FLAGS_GET_STGOBJ_SIZE 0x00000080

                               /* GET Oracle column types from access driver */
#define ODCI_EXTTABLE_OPEN_FLAGS_GET_COLS        0x00000100

                               /* GET Native column types from access driver */
#define ODCI_EXTTABLE_OPEN_FLAGS_GET_NATIVE_COLS 0x00000200

                                /* GET partition metadata from access driver */
#define ODCI_EXTTABLE_OPEN_FLAGS_GET_PARTITIONS  0x00000400

               /* access driver should be "silent" (don't write to log file) */
#define ODCI_EXTTABLE_OPEN_FLAGS_SILENT          0x00000800

                                     /* SET absolute partition number vector */
#define ODCI_EXTTABLE_OPEN_FLAGS_SET_APNUM_VEC   0x00001000

                                               /* Partitioned external table */
#define ODCI_EXTTABLE_OPEN_FLAGS_PET             0x00002000

                                                      /* validate table data */
#define ODCI_EXTTABLE_OPEN_FLAGS_VALIDATE        0x00004000
                                              /* use old column array format */
#define ODCI_EXTTABLE_OPEN_FLAGS_OLD_CA          0x00008000
                  /* internal external table, =ODCI_EXTTABLE_FETCH_FLAGS_INT */
#define ODCI_EXTTABLE_OPEN_FLAGS_INT             0x80000000

/* ------------------------ ODCIExtTableFetch Flags ------------------------ */
/* ODCI_EXTTABLE_FETCH_FLAGS_EOS is an OUT parameter FROM the
 * Access Driver to indicate that end-of-stream has been reached.
 */
#define ODCI_EXTTABLE_FETCH_FLAGS_EOS   0x00000001 /* end-of-stream on fetch */

/* ODCI_EXTTABLE_FETCH_OPAQUE_GRAN is an IN parameter TO the
 * Access Driver to indicate that the RDBMS is suppling opaque granules.
 */
#define ODCI_EXTTABLE_FETCH_OPAQUE_GRAN 0x00000002   /* opaque granule given */

/* ODCI_EXTTABLE_FETCH_BIND_DOC is an IN parameter TO the
 * Access Driver to indicate that the RDBMS is supplying xadbinddoc XML document
 */
#define ODCI_EXTTABLE_FETCH_BIND_DOC    0x00000004          /* binddoc given */
#define ODCI_EXTTABLE_FETCH_FLAGS_INT ODCI_EXTTABLE_OPEN_FLAGS_INT

/* ------------------------ ODCIExtTableClose Flags ------------------------ */
#define ODCI_EXTTABLE_CLOSE_FLAGS_ERR   0x00000001
                                     /* rowsource close with error signalled */

/* Constants for Flags argument to ODCIAggregateTerminate */
#define ODCI_AGGREGATE_REUSE_CTX  1

/* Constants for ODCIColInfo.Flags */
#define ODCI_COMP_FILTERBY_COL     0x0001
#define ODCI_COMP_ORDERBY_COL      0x0002
#define ODCI_COMP_ORDERDSC_COL     0x0004
#define ODCI_COMP_UPDATED_COL      0x0008
#define ODCI_COMP_RENAMED_COL      0x0010
#define ODCI_COMP_RENAMED_TOPADT   0x0020
#define ODCI_COMP_REFERENCED_COL   0x0040

/* Constants for ODCIOrderByInfo.ExprType */
#define ODCI_COLUMN_EXPR   1
#define ODCI_ANCOP_EXPR    2

/* Constants for ODCIOrderByInfo.SortOrder */
#define ODCI_SORT_ASC    1
#define ODCI_SORT_DESC   2
#define ODCI_NULLS_FIRST 4

/* Constants for ODCIPartInfo.PartOp */
#define  ODCI_ADD_PARTITION   1
#define  ODCI_DROP_PARTITION  2

/*---------------------------------------------------------------------------
                     ODCI TYPES
  ---------------------------------------------------------------------------*/
/*
 * These are C mappings for the OTS types defined in catodci.sql
 */

typedef OCIRef   ODCIColInfo_ref;
typedef OCIRef   ODCIIndexInfo_ref;
typedef OCIRef   ODCIPredInfo_ref;
typedef OCIArray ODCIRidList;
typedef OCIRef   ODCIIndexCtx_ref;
typedef OCIRef   ODCIObject_ref;
typedef OCIArray ODCIObjectList;
typedef OCIRef   ODCIQueryInfo_ref;
typedef OCIRef   ODCIFuncInfo_ref;
typedef OCIRef   ODCICost_ref;
typedef OCIRef   ODCIArgDesc_ref;
typedef OCIRef   ODCIStatsOptions_ref;
typedef OCIRef   ODCIPartInfo_ref;
typedef OCIRef   ODCIEnv_ref;
typedef OCIRef   ODCIExtTableInfo_ref;             /* external table support */
typedef OCIRef   ODCIExtTableQCInfo_ref;           /* external table support */
typedef OCIRef   ODCIFuncCallInfo_ref;
typedef OCIArray ODCINumberList;
typedef OCIArray ODCIPartInfoList;
typedef OCIArray ODCIColValList;
typedef OCIArray ODCIColArrayList;
typedef OCIArray ODCIFilterInfoList;
typedef OCIArray ODCIOrderByInfoList;
typedef OCIRef   ODCIFilterInfo_ref;
typedef OCIRef   ODCIOrderByInfo_ref;
typedef OCIRef   ODCICompQueryInfo_ref;
typedef OCIArray ODCIColInfoList;
typedef OCIArray ODCIColInfoList2;
typedef OCIArray ODCIArgDescList;
typedef OCIArray ODCIGranuleList;
 
#endif /* ODCI_ORACLE */

#ifndef ODCI_KUTYXTT
#  define ODCI_KUTYXTT

#  ifndef KOL3_KUTYXTT
#   ifdef K3_ORACLE
#    include <kol3.h>
#   endif
#  endif

/*---------- External Tables ----------*/

/* Begin structure */

#ifndef ODCI_KUTY_BS
#  define ODCI_KUTY_BS(s) struct s {
#endif 

/* End structure */

#ifndef ODCI_KUTY_ES
#  define ODCI_KUTY_ES(s) }; typedef struct s s;
#endif

/* Scalar attr */

#ifndef ODCI_KUTY_SA
#  define ODCI_KUTY_SA( t, m, s ) t m;
#endif

/* Pointer attr */

#ifndef ODCI_KUTY_PA
#  define ODCI_KUTY_PA( t, m, s ) t *m;
#endif

/* large attr by value */
#ifndef ODCI_KUTY_LA
#  define ODCI_KUTY_LA( t, m, s ) t *m;
#endif

/* array attr */
#ifndef ODCI_KUTY_AA
#  define ODCI_KUTY_AA( t, m, s, e ) t *m;
#endif

ODCI_KUTY_BS( ODCIColInfo )
  ODCI_KUTY_PA( OCIString, TableSchema,         ODCIColInfo )
  ODCI_KUTY_PA( OCIString, TableName,           ODCIColInfo )
  ODCI_KUTY_PA( OCIString, ColName,             ODCIColInfo )
  ODCI_KUTY_PA( OCIString, ColTypName,          ODCIColInfo )
  ODCI_KUTY_PA( OCIString, ColTypSchema,        ODCIColInfo )
  ODCI_KUTY_PA( OCIString, TablePartition,      ODCIColInfo )
  ODCI_KUTY_SA( OCINumber, ColFlags,            ODCIColInfo )
  ODCI_KUTY_SA( OCINumber, ColOrderPos,         ODCIColInfo )
  ODCI_KUTY_SA( OCINumber, TablePartitionIden,  ODCIColInfo )
  ODCI_KUTY_SA( OCINumber, TablePartitionTotal, ODCIColInfo )
ODCI_KUTY_ES( ODCIColInfo )

ODCI_KUTY_BS( ODCIColInfo_ind )
  ODCI_KUTY_SA( OCIInd, atomic,              ODCIColInfo_ind )
  ODCI_KUTY_SA( OCIInd, TableSchema,         ODCIColInfo_ind )
  ODCI_KUTY_SA( OCIInd, TableName,           ODCIColInfo_ind )
  ODCI_KUTY_SA( OCIInd, ColName,             ODCIColInfo_ind )
  ODCI_KUTY_SA( OCIInd, ColTypName,          ODCIColInfo_ind )
  ODCI_KUTY_SA( OCIInd, ColTypSchema,        ODCIColInfo_ind )
  ODCI_KUTY_SA( OCIInd, TablePartition,      ODCIColInfo_ind )
  ODCI_KUTY_SA( OCIInd, ColFlags,            ODCIColInfo_ind )
  ODCI_KUTY_SA( OCIInd, ColOrderPos,         ODCIColInfo_ind )
  ODCI_KUTY_SA( OCIInd, TablePartitionIden,  ODCIColInfo_ind )
  ODCI_KUTY_SA( OCIInd, TablePartitionTotal, ODCIColInfo_ind )
ODCI_KUTY_ES( ODCIColInfo_ind )

ODCI_KUTY_BS( ODCIArgDesc )
  ODCI_KUTY_SA( OCINumber, ArgType,             ODCIArgDesc )
  ODCI_KUTY_PA( OCIString, TableName,           ODCIArgDesc )
  ODCI_KUTY_PA( OCIString, TableSchema,         ODCIArgDesc )
  ODCI_KUTY_PA( OCIString, ColName,             ODCIArgDesc )
  ODCI_KUTY_PA( OCIString, TablePartitionLower, ODCIArgDesc )
  ODCI_KUTY_PA( OCIString, TablePartitionUpper, ODCIArgDesc )
  ODCI_KUTY_SA( OCINumber, Cardinality,         ODCIArgDesc )
ODCI_KUTY_ES( ODCIArgDesc )

ODCI_KUTY_BS( ODCIArgDesc_ind )
  ODCI_KUTY_SA( OCIInd, atomic,              ODCIArgDesc_ind )
  ODCI_KUTY_SA( OCIInd, ArgType,             ODCIArgDesc_ind )
  ODCI_KUTY_SA( OCIInd, TableName,           ODCIArgDesc_ind )
  ODCI_KUTY_SA( OCIInd, TableSchema,         ODCIArgDesc_ind )
  ODCI_KUTY_SA( OCIInd, ColName,             ODCIArgDesc_ind )
  ODCI_KUTY_SA( OCIInd, TablePartitionLower, ODCIArgDesc_ind )
  ODCI_KUTY_SA( OCIInd, TablePartitionUpper, ODCIArgDesc_ind )
  ODCI_KUTY_SA( OCIInd, Cardinality,         ODCIArgDesc_ind )
ODCI_KUTY_ES( ODCIArgDesc_ind )

ODCI_KUTY_BS( ODCIExtTableInfo )

   ODCI_KUTY_PA( OCIString,       TableSchema,      ODCIExtTableInfo )
   ODCI_KUTY_PA( OCIString,       TableName,        ODCIExtTableInfo )
   ODCI_KUTY_AA( ODCIColInfoList, RefCols,          ODCIExtTableInfo, ODCIColInfo )
   ODCI_KUTY_LA( OCIClobLocator,  AccessParmClob,   ODCIExtTableInfo )
   ODCI_KUTY_LA( OCIBlobLocator,  AccessParmBlob,   ODCIExtTableInfo )
   ODCI_KUTY_AA( ODCIArgDescList, Locations,        ODCIExtTableInfo, ODCIArgDesc )
   ODCI_KUTY_AA( ODCIArgDescList, Directories,      ODCIExtTableInfo, ODCIArgDesc )
   ODCI_KUTY_PA( OCIString,       DefaultDirectory, ODCIExtTableInfo )
   ODCI_KUTY_PA( OCIString,       DriverType,       ODCIExtTableInfo )
   ODCI_KUTY_SA( OCINumber,       OpCode,           ODCIExtTableInfo )
   ODCI_KUTY_SA( OCINumber,       AgentNum,         ODCIExtTableInfo )
   ODCI_KUTY_SA( OCINumber,       GranuleSize,      ODCIExtTableInfo )
   ODCI_KUTY_SA( OCINumber,       Flag,             ODCIExtTableInfo )
   ODCI_KUTY_SA( OCINumber,       SamplePercent,    ODCIExtTableInfo )
   ODCI_KUTY_SA( OCINumber,       MaxDoP,           ODCIExtTableInfo )
   ODCI_KUTY_PA( OCIRaw,          SharedBuf,        ODCIExtTableInfo )
   ODCI_KUTY_PA( OCIString,       MTableName,       ODCIExtTableInfo )
   ODCI_KUTY_PA( OCIString,       MTableSchema,     ODCIExtTableInfo )
   ODCI_KUTY_SA( OCINumber,       TableObjNo,       ODCIExtTableInfo )
   ODCI_KUTY_PA( OCIString,       AccessParmMod,    ODCIExtTableInfo )

ODCI_KUTY_ES( ODCIExtTableInfo )

ODCI_KUTY_BS( ODCIExtTableInfo_ind )

   ODCI_KUTY_SA( OCIInd, _atomic,          ODCIExtTableInfo_ind )
   ODCI_KUTY_SA( OCIInd, TableSchema,      ODCIExtTableInfo_ind )
   ODCI_KUTY_SA( OCIInd, TableName,        ODCIExtTableInfo_ind )
   ODCI_KUTY_SA( OCIInd, RefCols,          ODCIExtTableInfo_ind )
   ODCI_KUTY_SA( OCIInd, AccessParmClob,   ODCIExtTableInfo_ind )
   ODCI_KUTY_SA( OCIInd, AccessParmBlob,   ODCIExtTableInfo_ind )
   ODCI_KUTY_SA( OCIInd, Locations,        ODCIExtTableInfo_ind )
   ODCI_KUTY_SA( OCIInd, Directories,      ODCIExtTableInfo_ind )
   ODCI_KUTY_SA( OCIInd, DefaultDirectory, ODCIExtTableInfo_ind )
   ODCI_KUTY_SA( OCIInd, DriverType,       ODCIExtTableInfo_ind )
   ODCI_KUTY_SA( OCIInd, OpCode,           ODCIExtTableInfo_ind )
   ODCI_KUTY_SA( OCIInd, AgentNum,         ODCIExtTableInfo_ind )
   ODCI_KUTY_SA( OCIInd, GranuleSize,      ODCIExtTableInfo_ind )
   ODCI_KUTY_SA( OCIInd, Flag,             ODCIExtTableInfo_ind )
   ODCI_KUTY_SA( OCIInd, SamplePercent,    ODCIExtTableInfo_ind )
   ODCI_KUTY_SA( OCIInd, MaxDoP,           ODCIExtTableInfo_ind )
   ODCI_KUTY_SA( OCIInd, SharedBuf,        ODCIExtTableInfo_ind )
   ODCI_KUTY_SA( OCIInd, MTableName,       ODCIExtTableInfo_ind )
   ODCI_KUTY_SA( OCIInd, MTableSchema,     ODCIExtTableInfo_ind )
   ODCI_KUTY_SA( OCIInd, TableObjNo,       ODCIExtTableInfo_ind )
   ODCI_KUTY_SA( OCIInd, AccessParmMod,    ODCIExtTableInfo_ind )

ODCI_KUTY_ES( ODCIExtTableInfo_ind )

/* Proj 47082: GranuleInfo is currently not in use. Granule info is subsumed
 * by attributes in the xtArgs structure */
ODCI_KUTY_BS( ODCIExtTableQCInfo )

   ODCI_KUTY_SA( OCINumber,       NumGranules,            ODCIExtTableQCInfo )
   ODCI_KUTY_SA( OCINumber,       NumLocations,           ODCIExtTableQCInfo )
   ODCI_KUTY_AA( ODCIGranuleList, GranuleInfo,            ODCIExtTableQCInfo, OCINumber )
   ODCI_KUTY_SA( OCINumber,       IntraSourceConcurrency, ODCIExtTableQCInfo )
   ODCI_KUTY_SA( OCINumber,       MaxDoP,                 ODCIExtTableQCInfo )
   ODCI_KUTY_PA( OCIRaw,          SharedBuf,              ODCIExtTableQCInfo )

ODCI_KUTY_ES( ODCIExtTableQCInfo )

ODCI_KUTY_BS( ODCIExtTableQCInfo_ind )

   ODCI_KUTY_SA( OCIInd, _atomic,                ODCIExtTableQCInfo_ind )
   ODCI_KUTY_SA( OCIInd, NumGranules,            ODCIExtTableQCInfo_ind )
   ODCI_KUTY_SA( OCIInd, NumLocations,           ODCIExtTableQCInfo_ind )
   ODCI_KUTY_SA( OCIInd, GranuleInfo,            ODCIExtTableQCInfo_ind )
   ODCI_KUTY_SA( OCIInd, IntraSourceConcurrency, ODCIExtTableQCInfo_ind )
   ODCI_KUTY_SA( OCIInd, MaxDoP,                 ODCIExtTableQCInfo_ind )
   ODCI_KUTY_SA( OCIInd, SharedBuf,              ODCIExtTableQCInfo_ind )

ODCI_KUTY_ES( ODCIExtTableQCInfo_ind )

#ifdef KUTYXTTE_ORACLE
KUTYSA( ODCIColInfo_ptr_t )
KUTYSA( ODCIArgDesc_ptr_t )
KUTYSA( OCINumber_ptr_t )
#endif

#endif /* ODCI_KUTYXTT */

#ifdef ODCI_ORACLE_DEFINED

struct ODCIFuncCallInfo
{
   struct ODCIColInfo ColInfo;
};

struct ODCIFuncCallInfo_ind
{
  struct ODCIColInfo_ind ColInfo;
};

struct ODCITrustedColInfo
{
 oratext*  TableSchema;    /* QX_CINFO_TSCHEMA_POS */
 oratext*  TableName;      /* QX_CINFO_TNAME_POS */
 oratext*  ColName;        /* QX_CINFO_CNAME_POS */
 oratext*  ColTypeName;    /* QX_CINFO_CTYPNAME_POS */
 oratext*  ColTypeSchema;  /* QX_CINFO_CTYPSCHEMA_POS */
 oratext*  TablePartition; /* QX_CINFO_TPART_POS */
 ub1       ColFlags;          /* QX_CINFO_FLAGS_POS */
 ub2       ColOrderBy;        /* QX_CINFO_ORDERBY_POS */
 void*     TablePartitionIden;     /* QX_CINFO_TPIDEN_POS */
 ub4       TablePartitionIdenLen;
 void*     TablePartitionTotal;      /* QX_CINFO_TPTOTAL_POS */
 ub4       TablePartitionTotalLen;
 void*     cTableMut;
 void*     TableMut;
};
typedef struct ODCITrustedColInfo ODCITrustedColInfo;

struct ODCITrustedIndexInfo
{
   oratext*               IndexSchema;
   oratext*               IndexName;
   ODCITrustedColInfo**   IndexCols;
   ub4                    NumIndexCols;
   oratext*               IndexPartition;
   ub2                    IndexInfoFlags;
   ub2                    IndexParaDegree;
   void*                  IndexPartitionIden;
   ub4                    IndexPartitionIdenLen;
   void*                  IndexPartitionTotal;
   ub4                    IndexPartitionTotalLen;
   void*                  IndexMut;
};
typedef struct ODCITrustedIndexInfo ODCITrustedIndexInfo;

struct ODCITrustedUpdateInfo
{
  ODCITrustedIndexInfo *iinfo;
  oratext              *rid;
  ub4                   ridlen;
  void                 *oldval;
  void                 *newval;
  ub1                   EnvFlags;
  ub2                   CallProperty;
  ub4                   DebugLevel;
  sword                 CursorNum;
};
typedef struct ODCITrustedUpdateInfo ODCITrustedUpdateInfo;

struct ODCIIndexInfo
{
   OCIString*       IndexSchema;
   OCIString*       IndexName;
   ODCIColInfoList* IndexCols;
   OCIString*       IndexPartition;
   OCINumber        IndexInfoFlags;
   OCINumber        IndexParaDegree;
   OCINumber        IndexPartitionIden;
   OCINumber        IndexPartitionTotal;
};
typedef struct ODCIIndexInfo ODCIIndexInfo;
 
struct ODCIIndexInfo_ind
{
   OCIInd atomic;
   OCIInd IndexSchema;
   OCIInd IndexName;
   OCIInd IndexCols;
   OCIInd IndexPartition;
   OCIInd IndexInfoFlags;
   OCIInd IndexParaDegree;
   OCIInd IndexPartitionIden;
   OCIInd IndexPartitionTotal;
};
typedef struct ODCIIndexInfo_ind ODCIIndexInfo_ind;
 
struct ODCIPredInfo
{
   OCIString* ObjectSchema;
   OCIString* ObjectName;
   OCIString* MethodName;
   OCINumber  Flags;
};
typedef struct ODCIPredInfo ODCIPredInfo;
 
struct ODCIPredInfo_ind
{
   OCIInd atomic;
   OCIInd ObjectSchema;
   OCIInd ObjectName;
   OCIInd MethodName;
   OCIInd Flags;
};
typedef struct ODCIPredInfo_ind ODCIPredInfo_ind;

struct ODCIFilterInfo
{
  ODCIColInfo ColInfo;
  OCINumber Flags;
  OCIAnyData *strt;
  OCIAnyData *stop;
};
typedef struct ODCIFilterInfo ODCIFilterInfo;

struct ODCIFilterInfo_ind
{
  OCIInd atomic;
  ODCIColInfo_ind ColInfo;
  OCIInd  Flags;
  OCIInd  strt;
  OCIInd  stop;
};
typedef struct ODCIFilterInfo_ind ODCIFilterInfo_ind;


struct ODCIOrderByInfo
{
  OCINumber ExprType;
  OCIString *ObjectSchema;
  OCIString *TableName;
  OCIString *ExprName;
  OCINumber SortOrder;
};
typedef struct ODCIOrderByInfo ODCIOrderByInfo;

struct ODCIOrderByInfo_ind
{
  OCIInd atomic;
  OCIInd ExprType;
  OCIInd ObjectSchema;
  OCIInd TableName;
  OCIInd ExprName;
  OCIInd SortOrder;
};
typedef struct ODCIOrderByInfo_ind ODCIOrderByInfo_ind;


struct ODCICompQueryInfo
{
  ODCIFilterInfoList  *PredInfo;
  ODCIOrderByInfoList *ObyInfo;
};
typedef struct ODCICompQueryInfo ODCICompQueryInfo;

struct ODCICompQueryInfo_ind
{
  OCIInd atomic;
  OCIInd PredInfo;
  OCIInd ObyInfo;
};
typedef struct ODCICompQueryInfo_ind ODCICompQueryInfo_ind;

 
struct ODCIObject
{
   OCIString* ObjectSchema;
   OCIString* ObjectName;
};
typedef struct ODCIObject ODCIObject;
 
struct ODCIObject_ind
{
   OCIInd atomic;
   OCIInd ObjectSchema;
   OCIInd ObjectName;
};
typedef struct ODCIObject_ind ODCIObject_ind;
 
struct ODCIQueryInfo
{
   OCINumber       Flags;
   ODCIObjectList* AncOps;
   ODCICompQueryInfo CompInfo;
};
typedef struct ODCIQueryInfo ODCIQueryInfo;

 
struct ODCIQueryInfo_ind
{
   OCIInd atomic;
   OCIInd Flags;
   OCIInd AncOps;
   ODCICompQueryInfo_ind CompInfo;
};
typedef struct ODCIQueryInfo_ind ODCIQueryInfo_ind;
 
struct ODCIIndexCtx
{
   struct ODCIIndexInfo IndexInfo;
   OCIString*           Rid;
   struct ODCIQueryInfo QueryInfo;
};
typedef struct ODCIIndexCtx ODCIIndexCtx;
 
struct ODCIIndexCtx_ind
{
   OCIInd                   atomic;
   struct ODCIIndexInfo_ind IndexInfo;
   OCIInd                   Rid;
   struct ODCIQueryInfo_ind QueryInfo;
};
typedef struct ODCIIndexCtx_ind ODCIIndexCtx_ind;
 
struct ODCIFuncInfo
{
   OCIString* ObjectSchema;
   OCIString* ObjectName;
   OCIString* MethodName;
   OCINumber Flags;
};
typedef struct ODCIFuncInfo ODCIFuncInfo;
 
struct ODCIFuncInfo_ind
{
   OCIInd atomic;
   OCIInd ObjectSchema;
   OCIInd ObjectName;
   OCIInd MethodName;
   OCIInd Flags;
};
typedef struct ODCIFuncInfo_ind ODCIFuncInfo_ind;
 
struct ODCICost
{
   OCINumber  CPUcost;
   OCINumber  IOcost;
   OCINumber  NetworkCost;
   OCIString* IndexCostInfo;
};
typedef struct ODCICost ODCICost;
 
struct ODCICost_ind
{
   OCIInd atomic;
   OCIInd CPUcost;
   OCIInd IOcost;
   OCIInd NetworkCost;
   OCIInd IndexCostInfo;
};
typedef struct ODCICost_ind ODCICost_ind;
 
struct ODCIStatsOptions
{
   OCINumber Sample;
   OCINumber Options;
   OCINumber Flags;
};
typedef struct ODCIStatsOptions ODCIStatsOptions;
 
struct ODCIStatsOptions_ind
{
   OCIInd atomic;
   OCIInd Sample;
   OCIInd Options;
   OCIInd Flags;
};
typedef struct ODCIStatsOptions_ind ODCIStatsOptions_ind;

struct ODCIEnv
{
   OCINumber EnvFlags;
   OCINumber CallProperty;
   OCINumber DebugLevel;
   OCINumber CursorNum;
};
typedef struct ODCIEnv ODCIEnv;

struct ODCIEnv_ind
{
   OCIInd _atomic;
   OCIInd EnvFlags;
   OCIInd CallProperty;
   OCIInd DebugLevel;
   OCIInd CursorNum;
};
typedef struct ODCIEnv_ind ODCIEnv_ind;
 
struct ODCIPartInfo
{
   OCIString* TablePartition;
   OCIString* IndexPartition;
   OCINumber  IndexPartitionIden;
   OCINumber  PartOp;
};
typedef struct ODCIPartInfo ODCIPartInfo;
 
struct ODCIPartInfo_ind
{
   OCIInd atomic;
   OCIInd TablePartition;
   OCIInd IndexPartition;
   OCIInd IndexPartitionIden;
   OCIInd PartOp;
};
typedef struct ODCIPartInfo_ind ODCIPartInfo_ind;

/*********************************************************/
/* Table Function Info types (used by ODCITablePrepare)  */
/*********************************************************/

struct ODCITabFuncInfo
{
  ODCINumberList*  Attrs;
  OCIType*         RetType;
};
typedef struct ODCITabFuncInfo ODCITabFuncInfo;

struct ODCITabFuncInfo_ind
{
  OCIInd _atomic;
  OCIInd Attrs;
  OCIInd RetType;
};
typedef struct ODCITabFuncInfo_ind ODCITabFuncInfo_ind;

/*********************************************************************/
/* Table Function Statistics types (used by ODCIStatsTableFunction)  */
/*********************************************************************/

struct ODCITabFuncStats
{
  OCINumber num_rows;
};
typedef struct ODCITabFuncStats ODCITabFuncStats;

struct ODCITabFuncStats_ind
{
  OCIInd _atomic;
  OCIInd num_rows;
};
typedef struct ODCITabFuncStats_ind ODCITabFuncStats_ind;

/*---------------------------------------------------------------------------
                     PRIVATE TYPES AND CONSTANTS
  ---------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------
                           PUBLIC FUNCTIONS
  ---------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------
                          PRIVATE FUNCTIONS
  ---------------------------------------------------------------------------*/


#undef ODCI_ORACLE_DEFINED
#endif                                              /* ODCI_ORACLE_DEFINED */
