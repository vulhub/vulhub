/* Copyright (c) 2006, 2016, Oracle and/or its affiliates. 
All rights reserved.*/
 
/* 
   NAME 
     ocixstream.h - OCI XStream APIs

   DESCRIPTION 
     OCI APIs for XStream

   RELATED DOCUMENTS 
 
   EXPORT FUNCTION(S) 

   INTERNAL FUNCTION(S)

   EXAMPLES

   NOTES

   MODIFIED   (MM/DD/YY)
   huntran     08/01/16 - Flush wait for sync error handling
   jathyaga    04/11/16 - Added comment for reserved OCI_ROWLCR flag.
   thoang      11/13/15 - Add XOut attach flags for local undo and appcont
   baswamy     07/21/15 - Bug# 21420307: Added comment for reserved 
                          OCI_ROWLCR flags. 
   romorale    07/19/14 - BigSCN.
   huntran     11/07/13 - reserve OCI_ROWLCR flags
   yurxu       12/12/12 - Bug-16000459: New flag for Object Col
   huntran     08/15/12 - private column flag
   vgerard     04/20/12 - comment for private LCR compare column flags
   elu         03/06/12 - 32k
   tianli      11/30/11 - add control lcr subcode macro
   huntran     11/02/11 - attribute name for old owner and old name
   thoang      12/04/11 - Add OCI_ROWLCR_IS_INFLIGHT_TXN
   elu         11/30/11 - remove knxinXMLInfoSet
   vchandar    10/10/11 - Bug 13058458
   huntran     09/28/11 - make SessionSet apis public
   tianli      05/13/11 - add pdb fields
   elu         05/25/11 - remove xml schema
   elu         04/20/11 - xmlschema
   elu         03/15/11 - add current_user
   tianli      03/08/11 - Add OCIXSTREAM_IN_DETACH_RESTART_INBOUND mode
   tianli      03/03/11 - add control lcr
   thoang      03/10/10 - Add OCIXSTREAM_IN_ATTACH_RESTART_INBOUND mode
   thoang      12/28/09 - Update comments
   elu         01/07/10 - stmt lcr
   thoang      12/15/09 - Remove non-oracle src mode 
   thoang      12/09/09 - Add note to OCIXStreamInErrorGet
   thoang      12/04/09 - Remove ProcessedLWMGet2/Set2 
   rmao        11/20/09 - add OCI_ROWLCR_SEQ_LCR
   thoang      10/20/09 - Add rollback and start_tx cmd.  
   bpwang      10/09/09 - Add OCI_ROWLCR_HAS_ID_KEY_ONLY
   elu         10/05/09 - add stmt LCR
   rihuang     10/06/09 - Add OCIXSTREAM_IN_FLUSH_WAIT_FOR_COMPLETE
   thoang      05/08/09 - Add OCILCR_NEW_ONLY_MODE 
   praghuna    05/11/09 - removed 'TODO' comments 
   thoang      02/15/09 - Change lob_column_* to chunk_column_*
   thoang      01/27/09 - 8216105 - add OLD/NEW column parms to OCILCRHeaderGet
   rihuang     01/05/09 - Add OCI_LCR_ATTR_TRACKING_LABEL
   tianli      11/28/08 - add DDL flags
   tianli      11/20/08 - add OCILCRAttribute methods
   thoang      11/20/08 - Define OCI_LCR_MAX_TXID_LEN
   tianli      11/07/08 - add edition
   thoang      11/10/08 - change return type to sword for consistency
   thoang      10/16/08 - remove commit position arg
   tianli      08/26/08 - rename client_name in XStreamIn attach call
   thoang      06/30/08 - Support XStream APIs using two callbacks.
   praghuna    05/14/08 - charset id is ub2, OCILcrGetRowStmtWithBindVar
   thoang      06/02/08 - Define reserved attach mode for internal clients
   elu         05/08/08 - add pos functions
   thoang      04/29/08 - API changes 
   jinwu       04/28/08 - add OCILcrGetExtraAttributes
   elu         04/14/08 - add OCI_LCR_MAX_POSITION_LEN
   juyuan      03/27/08 - add flag for Set/GetHeader and Set/GetColumnInfo
   thoang      02/25/08 - Add GetNextChunk and SetNextChunk
   rihuang     03/24/08 - Signature change for OCILcrNew
   elu         03/05/08 - add lcr id
   praghuna    02/26/08 - Added OCILcrGetRowStmt
   thoang      01/25/08 - Add wm_time parameter to XApply APIs
   thoang      12/28/07 - Add mode parameter to XApplyDetach
   thoang      11/07/07 - Change extapp apis to return ub1[] watermark
   juyuan      05/23/07 - XStream In
   thoang      11/13/06 - Add XStream Out methods
   thoang      11/13/06 - Add LCR getter methods 
   nshodhan    05/12/06 - xstream OCI APIs 
   nshodhan    05/12/06 - Creation

*/

#ifndef OCIXSTREAM_ORACLE
# define OCIXSTREAM_ORACLE

#ifndef ORATYPES
# include <oratypes.h>
#endif

#ifndef OCI_ORACLE
# include <oci.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

/*---------------------------------------------------------------------------
                     PUBLIC TYPES AND CONSTANTS
  ---------------------------------------------------------------------------*/
/* LCR Types -- must match with values defined in kngo.h */
#define OCI_LCR_XROW                   (3)               /* External Row LCR */
#define OCI_LCR_XDDL                   (4)               /* External DDL LCR */

/* DML Command Types -- must match with values defined in kngl.h */
#define OCI_LCR_ROW_CMD_INSERT          "INSERT"
#define OCI_LCR_ROW_CMD_DELETE          "DELETE"
#define OCI_LCR_ROW_CMD_UPDATE          "UPDATE"
#define OCI_LCR_ROW_CMD_COMMIT          "COMMIT"
#define OCI_LCR_ROW_CMD_LOB_WRITE       "LOB WRITE"
#define OCI_LCR_ROW_CMD_LOB_TRIM        "LOB TRIM"
#define OCI_LCR_ROW_CMD_LOB_ERASE       "LOB ERASE"
#define OCI_LCR_ROW_CMD_ROLLBACK        "ROLLBACK"
#define OCI_LCR_ROW_CMD_START_TX        "START_TX"      /* start transaction */
#define OCI_LCR_ROW_CMD_CTRL_INFO       "CONTROL INFO"        /* contorl lcr */

#define OCI_LCR_CTRLINFO_MEMORY_PRESSURE (32768)
#define OCI_LCR_CTRLINFO_MISSING_LOGFILE (32769)
#define OCI_LCR_CTRLINFO_UNSUPPORTED_LCR (32770)

/* LCR Extra Attribute Name -- must match with values defined in knll.h */
#define OCI_LCR_ATTR_THREAD_NO         "THREAD#"
#define OCI_LCR_ATTR_ROW_ID            "ROW_ID"
#define OCI_LCR_ATTR_SESSION_NO        "SESSION#"
#define OCI_LCR_ATTR_SERIAL_NO         "SERIAL#"
#define OCI_LCR_ATTR_USERNAME          "USERNAME"
#define OCI_LCR_ATTR_TX_NAME           "TX_NAME"

/* below are non first class LCR field specific */
#define OCI_LCR_ATTR_EDITION_NAME      "EDITION_NAME"
#define OCI_LCR_ATTR_MESSAGE_TRACKING_LABEL "MESSAGE_TRACKING_LABEL"
#define OCI_LCR_ATTR_CURRENT_USER      "CURRENT_USER"
#define OCI_LCR_ATTR_ROOT_NAME         "ROOT_NAME"
#define OCI_LCR_ATTR_OLD_OWNER         "OLD_OWNER"
#define OCI_LCR_ATTR_OLD_ONAME         "OLD_ONAME"

/* Row LCR column value types used in OCILCRRowColumnInfoGet/Set functions. */
#define OCI_LCR_ROW_COLVAL_OLD         0                      /* OLD columns */
#define OCI_LCR_ROW_COLVAL_NEW         1                     /*  NEW columns */

/* maximum length for position
 * NOTE: This MUST be consistent with DefaultRowLCRCache.MaxLowWaterMarkLength
 * in DefaultRowLCRCache.java
 */
#define OCI_LCR_MAX_POSITION_LEN        64

/* maximum length for txid */
#define OCI_LCR_MAX_TXID_LEN            128

/* Valid column flags used in OCILCRRowColumnInfoSet, OCILCRRowColumnInfoGet,
 * OCILCRLobInfoSet, OCILCRLobInfoGet, OCIXStreamOutChunkReceive, 
 * OCIXStreamInChunkSend calls.
 * NOTE: last byte reserved for private OCIP_LCR flags.
 */
#define OCI_LCR_COLUMN_LOB_DATA     (0x00000001)    /* col contains lob data */
#define OCI_LCR_COLUMN_LONG_DATA    (0x00000002)    /* col contains long data*/
#define OCI_LCR_COLUMN_EMPTY_LOB    (0x00000004)    /* col has an empty lob  */
#define OCI_LCR_COLUMN_LAST_CHUNK   (0x00000008) /* last chunk of current col*/
#define OCI_LCR_COLUMN_AL16UTF16    (0x00000010)  /* col is in AL16UTF16 fmt */
#define OCI_LCR_COLUMN_NCLOB        (0x00000020)       /* col has NCLOB data */
#define OCI_LCR_COLUMN_XML_DATA     (0x00000040)    /* col contains xml data */
#define OCI_LCR_COLUMN_XML_DIFF     (0x00000080)/* col contains xmldiff data */
#define OCI_LCR_COLUMN_ENCRYPTED    (0x00000100)         /* col is encrypted */

/* OCI_LCR_COLUMN_UPDATED is set only for the modified columns in the NEW 
 * column list of an update LCR. 
 */
#define OCI_LCR_COLUMN_UPDATED      (0x00000200)           /* col is updated */
#define OCI_LCR_COLUMN_32K_DATA     (0x00000400)    /* col contains 32k data */
#define OCI_LCR_COLUMN_OBJ_XML      (0x00000800)    /* col is UDT, rep as XML*/
    
/* Valid bit values for the flag parameter in the following APIS:
 * - OCILCRHeaderGet
 * - OCILCRHeaderSet
 */
#define OCI_ROWLCR_HAS_ID_KEY_ONLY  (0x00000001)     /* only has ID key cols */
                                                 /* (0x00000002) is RESERVED */
                                                 /* (0x00000004) is RESERVED */
                                                 /* (0x00000008) is RESERVED */
#define OCI_ROWLCR_SEQ_LCR          (0x00000010)             /* sequence lcr */

/* LCR belongs to an inflight transaction, i.e., transaction was started
 * before the outbound server's starting position.
 */ 
#define OCI_ROWLCR_IS_INFLIGHT_TXN  (0x00000020)  
                                                 /* (0x00000040) is RESERVED */
                                                 /* (0x00000080) is RESERVED */
                                                 /* (0x00000100) is RESERVED */
                                                 /* (0x00000200) is RESERVED */
                                                 /* (0x00000400) is RESERVED */
                                                 /* (0x00000800) is RESERVED */

/* LCR replayed from application container sync statement. */
#define OCI_LCR_APPCON_REPLAY       (0x00001000)  
                                                 /* (0x00002000) is RESERVED */

/* Valid bit values for flag parameter in the following APIs: 
 * - OCIXStreamOutChunkReceive & OCIXStreamOutLCRReceive 
 * - OCIXStreamInChunkSend & OCIXStreamInLCRSend  
 */
#define OCI_XSTREAM_MORE_ROW_DATA    (0x00000001)  /* LCR contains more data */

/* Valid mode flag for OCILCRHeaderGet and OCILCRRowColumnInfoGet functions */
#define OCILCR_NEW_ONLY_MODE       (0x0001)      /* NEW columns only -- dont */
                                                 /* include OLD columns      */

/* Valid mode flag for OCIXStreamInFlush */
                                   /* Synchronous mode for OCIXStreamInFlush */
#define OCIXSTREAM_IN_FLUSH_WAIT_FOR_COMPLETE  (0x00000001)
/* internal use */
#define OCIXSTREAM_IN_FLUSH_RESERVED_1         (0x00000002)

/* SessionSet attributes */
#define  OCIXSTREAM_SESSION_SET_MAX_PARAM_LEN   128
#define  OCIXSTREAM_ATTR_ATTACH_TIMEOUT        "ATTACH_TIMEOUT_SECS"
#define  OCIXSTREAM_ATTR_MAX_ATTACH_RETRIES    "MAX_ATTACH_RETRIES"

/* BigSCN. These flags identify the LCRID position version */
#define  OCI_LCRID_V1                           1
#define  OCI_LCRID_V2                           2
  
/*---------------------------------------------------------------------------
                     PRIVATE TYPES AND CONSTANTS
  ---------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------
                           EXPORT FUNCTIONS
  ---------------------------------------------------------------------------*/
/*
------------------------------------------------------------------------------=
NAME
  OCILCRNew - OCI LCR NEW
DESCRIPTION
  Create a new XStream LCR for the user specified duration and type
PARAMETERS
  svchp        (IN)      - OCI service context
  errhp        (IN)      - OCI Error Handle
  duration     (IN)      - allocation duration for LCR memory
  lcrtype      (IN)      - LCR type (OCI_LCR_XROW / OCI_LCR_XDDL)
  lcrp         (IN/OUT)  - XStream LCR. (*lcrp must be initialized to null.)
  mode         (IN)      - mode
NOTES
  - memory will be based on the duration specified by the user
  - For now, specify OCI_DEFAULT for mode
------------------------------------------------------------------------------=
*/
sword OCILCRNew(OCISvcCtx    *svchp, 
                OCIError     *errhp, 
                OCIDuration   duration, 
                ub1           lcrtype,
                void        **lcrp,
                ub4           mode);

/*
------------------------------------------------------------------------------=
NAME
  OCILCRFree - OCI LCR FREE
DESCRIPTION
  Free XStream LCR specified by the user
PARAMETERS
  svchp        (IN)      - OCI service context
  errhp        (IN)      - OCI Error Handle
  lcrp         (IN/OUT)  - XStream LCR
  mode         (IN)      - mode
NOTES
  - For now, specify OCI_DEFAULT for mode
------------------------------------------------------------------------------=
*/
sword OCILCRFree(OCISvcCtx    *svchp, 
                 OCIError     *errhp, 
                 void         *lcrp,
                 ub4           mode);


/*
------------------------------------------------------------------------------=
NAME
  OCILCRHeaderSet - OCI LCR Set Header
DESCRIPTION
  Initialize elements of XStream LCR's header 
PARAMETERS
  svchp                 (IN) - OCI service context
  errhp                 (IN) - OCI Error Handle
  src_db_name           (IN) - Pointer to Canonicalized source database name. 
                               Must be non-NULL. 
  src_db_name_len       (IN) - Length of source database name in bytes
                               excluding NULL terminator. Should follow Oracle
                               naming conventions and size limitations.
  cmd_type              (IN) - For ROW LCRs: OCI_LCR_ROW_CMD_XXXXXXX 
                               For DDL LCRs: One of the command types 
                               corresponding to OCI Reference manual
  cmd_type_len          (IN) - Length of cmd_type.
  owner                 (IN) - Canonicalized table owner name. Not required
                               for COMMIT LCR.
  owner_len             (IN) - Length of owner name in bytes excluding the 
                               NULL terminator. Should follow Oracle naming 
                               conventions and size limitations.
  oname                 (IN) - Canonicalized table name. Not required for 
                               COMIT LCR. 
  oname_len             (IN) - Length of table name in bytes excluding the 
                               NULL terminator. Should follow Oracle naming 
                               conventions and size limitations.
  tag                   (IN) - A binary tag that enables tracking of the LCR. 
                               For example, this tag can be used to determine
                               the original source database of the DML 
                               statement if apply forwarding is used. 
  tag_len               (IN) - Number of bytes in the tag. Cannot exceed 2000
                               bytes
  txid                  (IN) - Transaction ID.
  txid_len              (IN) - Length of transaction id in bytes excluding the 
                               NULL terminator. Should not exceeed
                               OCI_LCR_MAX_TXID_LEN bytes.
  src_time              (IN) - The time when the change was generated at the 
                               source database.
  position              (IN) - position for LCR. Must be byte-comparable.
  position_len          (IN) - Length of position. Must be non-zero. 
  flag                  (IN) - LCR flag.
  lcrp              (IN/OUT) - XStream LCR
  mode                  (IN) - mode
NOTES
  - For now, specify OCI_DEFAULT for mode
  - This function clears the current contents of the input LCR before
    setting the header to the new values.
------------------------------------------------------------------------------=
*/
sword OCILCRHeaderSet(OCISvcCtx   *svchp, 
                       OCIError    *errhp, 
                       oratext     *src_db_name,
                       ub2          src_db_name_len,
                       oratext     *cmd_type,
                       ub2          cmd_type_len,
                       oratext     *owner,
                       ub2          owner_len,
                       oratext     *oname,
                       ub2          oname_len,
                       ub1         *tag,
                       ub2          tag_len,
                       oratext     *txid,
                       ub2          txid_len, 
                       OCIDate     *src_time,
                       ub1         *position,
                       ub2          position_len,
                       oraub8       flag,
                       void        *lcrp,
                       ub4          mode);

/*
------------------------------------------------------------------------------=
NAME
  OCILCRHeaderGet - OCI LCR Get Header
DESCRIPTION
  Get header information from XStream LCR 
PARAMETERS
  svchp                  (IN) - OCI service context
  errhp                  (IN) - OCI Error Handle
  src_db_name           (OUT) - Pointer to Canonicalized source database name. 
                                Optional, if src_db_name is specified then 
                                must specify src_db_name_len as well.
  src_db_name_len       (OUT) - Length of source database name in bytes
                                excluding NULL terminator. 
                                Optional, if specified src_db_name_len then 
                                must specify src_db_name as well.
  cmd_type              (OUT) - Command type. Must be non-null if
                                cmd_type_len is non-null. Must be null if
                                cmd_type_len is NULL.
  cmd_type_len          (OUT) - Length of cmd_type. Optional.
  owner                 (OUT) - Canonicalized table owner name. 
                                Optional, if owner is specified then 
                                must specify owner_len as well.
  owner_len             (OUT) - Length of owner name in bytes excluding the 
                                NULL terminator. 
                                Optional, if owner_len is specified then 
                                must specify owner as well.
  oname                 (OUT) - Canonicalized table name. 
                                Optional, if oname is specified then 
                                must specify oname_len as well.
  oname_len             (OUT) - Length of table name in bytes excluding the 
                                NULL terminator. 
                                Optional, if oname_len is specified then 
                                must specify oname as well.
  tag                   (OUT) - A binary tag that enables tracking of the LCR. 
                                For example, this tag can be used to determine
                                the original source database of the 
                                DML statement if apply forwarding is used. 
                                Optional, if tag is specified then 
                                must specify tag_len as well.
  tag_len               (OUT) - Number of bytes in the tag.
                                Optional, if tag_len is specified then 
                                must specify tag as well.
  txid                  (OUT) - Transaction ID. 
                                Optional, if txid is specified then 
                                must specify txid_len as well.
  txid_len              (OUT) - Length of transaction id in bytes excluding
                                the NULL terminator. 
                                Optional, if txid_len is specified then 
                                must specify txid as well.
  src_time              (OUT) - The time when the change was generated at the 
                                source database. Optional.
  old_columns           (OUT) - Number of columns in the OLD column list.
                                Return 0 if input lcr is DDL LCR. Optional.
  new_columns           (OUT) - Number of columns present in either 
                                the OLD or NEW column list. 
                                Return 0 if input lcr is DDL LCR. Optional.
                                See NOTES below for the special mode supported
                                by this function.
  position              (OUT) - LCR position. Optional.
  position_len          (OUT) - Length of position. Must be non-null if
                                position is non-null. Must be null if
                                position is null.
  flag                  (OUT) - LCR flag. Optional.
  lcrp                   (IN) - XStream LCR
  mode                   (IN) - mode (see NOTES)
NOTES
  - Parameter src_time is optional. If specified the appropriate return 
    structure must be pre-allocated before calling OCILCRHeaderGet.
  - The return values for src_db_name, cmd_type, owner, oname, tag, txid and 
    position are shallow-copied (i.e., they point directly into the LCR 
    structure).  
  - Valid mode flags:
    - OCILCR_NEW_ONLY_MODE: if this mode is specified then the new_columns 
      returned is the count of the columns in the NEW column list only.  
      Otherwise, the new_columns returned is the number of distinct 
      columns present in either the NEW or the OLD column list of the given 
      ROW LCR.
------------------------------------------------------------------------------=
*/

sword OCILCRHeaderGet(OCISvcCtx   *svchp, 
                       OCIError    *errhp, 
                       oratext    **src_db_name,
                       ub2         *src_db_name_len,
                       oratext    **cmd_type,
                       ub2         *cmd_type_len,
                       oratext    **owner,
                       ub2         *owner_len,
                       oratext    **oname,
                       ub2         *oname_len,
                       ub1        **tag,
                       ub2         *tag_len,
                       oratext    **txid,
                       ub2         *txid_len, 
                       OCIDate     *src_time,
                       ub2         *old_columns,
                       ub2         *new_columns,
                       ub1        **position,
                       ub2         *position_len,
                       oraub8      *flag, 
                       void        *lcrp,
                       ub4          mode);

/*
------------------------------------------------------------------------------=
NAME
  OCILCRRowColumnInfoSet - OCI LCR ROW SET COLUMN INFO
DESCRIPTION
  Populates column information as specified by the user.
PARAMETERS
  svchp             (IN)    - OCI service context
  errhp             (IN)    - OCI Error Handle
  column_value_type (IN)    - ROW LCR column value type: 
                              - OCI_LCR_ROW_COLVAL_OLD        
                              - OCI_LCR_ROW_COLVAL_NEW    
  num_columns       (IN)    - Number of columns to be populated
  column_names      (IN)    - Pointer to an array of column names. Column 
                              names must be canonicalized. Column names should 
                             follow Oracle naming conventions
  column_name_lens (IN)    - Pointer to an array of column name lengths
                             in bytes, excluding the NULL terminator.
  column_dtyp      (IN)    - Pointer to an array of column datatypes. 
  column_valuesp   (IN)    - Pointer to an array of column data values.
  column_indp      (IN)    - Pointer to an indicator array. For all datatypes,
                             this is a pointer to an array of  OCIInd values 
                             (OCI_IND_NULL/OCI_IND_NOTNULL).
  column_alensp    (IN)    - Pointer to an array of actual column lengths in 
                             bytes.
  column_csetfp    (IN)    - Pointer to an array of character set forms for 
                             the columns. The default form is SQLCS_IMPLICIT. 
                             Setting this attribute will cause the database or 
                             national character set to be used on the client 
                             side. Set this attribute to SQLCS_NCHAR for the 
                             national character set or SQLCS_IMPLICIT for the 
                             database character set. 
                             Pass 0 for non-character columns.
  column_flags     (IN)    - Pointer to an array of column flags.
                             Possible bit values are OCI_LCR_COLUMN_* flags 
                             listed above.
  column_csid      (IN)    - Pointer to an array of column character set id.
                             The character set id is only required for
                             XMLType column; otherwise, the csid is ignored.
  row_lcrp         (IN/OUT)- XStream Row LCR pointer
  mode             (IN)    - mode
NOTES
  - For now, specify OCI_DEFAULT for mode
------------------------------------------------------------------------------=
*/
sword OCILCRRowColumnInfoSet(OCISvcCtx   *svchp, 
                             OCIError    *errhp, 
                             ub2          column_value_type,
                             ub2          num_columns,
                             oratext    **column_names,
                             ub2         *column_name_lens,
                             ub2         *column_dtyp,
                             void       **column_valuesp,
                             OCIInd      *column_indp,
                             ub2         *column_alensp,
                             ub1         *column_csetfp,
                             oraub8      *column_flags,
                             ub2         *column_csid,
                             void        *row_lcrp,
                             ub4          mode);

/*
------------------------------------------------------------------------------=
NAME
  OCILCRRowColumnInfoGet - OCI LCR ROW GET COLUMN INFO
DESCRIPTION
  Returns column information as requested by the user.
PARAMETERS
  svchp            (IN)    - OCI service context
  errhp            (IN)    - OCI Error Handle
  column_value_type (IN)   - ROW LCR column value type: 
                             - OCI_LCR_ROW_COLVAL_OLD        
                             - OCI_LCR_ROW_COLVAL_NEW    
                             (See NOTES for special mode supported by this
                              function.)
  num_columns      (OUT)   - Number of columns in requested column list
  column_names     (IN/OUT)- Pointer to an array of column names. 
                             Optional. If specified then column_namesl must
                             be specified as well, and both arrays must be the 
                             size specified by array_size parameter. 
  column_name_lens (IN/OUT)- Pointer to an array of column name lengths
                             in bytes, excluding the NULL terminator.
                             Optional. If specified then column_names must
                             be specified as well, and both arrays must be the 
                             size specified by array_size parameter. 
  column_dtyp      (IN/OUT)- Pointer to an array of column datatypes. 
                             Optional. If specified then this array must be 
                             the size specified by array_size parameter. 
  column_valuesp   (IN/OUT)- Pointer to an array of column data values.
                             Optional. If specified then this array must be 
                            the  size specified by array_size parameter. 
  column_indp      (IN/OUT)- Pointer to an indicator array. For all datatypes,
                             this is a pointer to an array of  OCIInd values 
                             (OCI_IND_NULL/OCI_IND_NOTNULL).
                             Optional. If specified then this array must be 
                             the size specified by array_size parameter. 
  column_alensp    (IN/OUT)- Pointer to an array of actual column lengths in 
                             bytes.
                             Optional. If specified then this array must be 
                             the size specified by array_size parameter. 
  column_csetfp    (IN/OUT)- Pointer to an array of character set forms for 
                             the columns. 
                             Optional. If specified then this array must be 
                             the size specified by array_size parameter. 
  column_flags     (IN/OUT)- Pointer to an array of column flags for 
                             the columns. 
                             Optional. If specified then this array must be 
                             the size specified by array_size parameter. 
                             Possible bit values are OCI_LCR_COLUMN_* flags 
                             listed above.
  column_csid      (IN/OUT)- Pointer to an array of column character set id for
                             the columns. 
                             Optional. If specified then this array must be 
                             the size specified by array_size parameter. 
                             The column csid is returned only for XMLType 
                             column. 
  row_lcrp         (IN)    - XStream Row LCR pointer
  array_size       (IN)    - Size of each of above arrays   
  mode             (IN)    - mode (see NOTES)
NOTES
  - For now, specify OCI_DEFAULT for mode
  - If array_size is not large enough to accommodate the number of columns
    in the requested column list then OCI_ERROR is returned. Parameter 
    num_columns will have the number of columns in the requested column list.
  - The return values for column_names and column_valuesp will be shallow
    copied (i.e., they reference directly into the LCR structure). 
    Client should not modify those pointers directly.  
  - Valid mode flags:
    - OCILCR_NEW_ONLY_MODE: this mode is valid only for OCI_LCR_ROW_COLVAL_NEW 
      column_value_type; otherwise, an error is raised. 
      If this mode is specified then the columns returned include only the
      columns in the NEW column list.
      If this mode is not specified then the columns returned is the union
      of the NEW columns plus the OLD columns that are not present in the 
      NEW column list.
------------------------------------------------------------------------------=
*/
sword OCILCRRowColumnInfoGet(OCISvcCtx   *svchp, 
                             OCIError    *errhp, 
                             ub2          column_value_type,
                             ub2         *num_columns,
                             oratext    **column_names,
                             ub2         *column_name_lens,
                             ub2         *column_dtyp,
                             void       **column_valuesp,
                             OCIInd      *column_indp,
                             ub2         *column_alensp,
                             ub1         *column_csetfp,
                             oraub8      *column_flags,
                             ub2         *column_csid,
                             void        *row_lcrp,
                             ub2          array_size,
                             ub4          mode);


/*
------------------------------------------------------------------------------=
NAME
  OCILCRDDLInfoSet - OCI LCR SET DDL INFO
DESCRIPTION
  populates DDL information as sepcified by the user.
PARAMETERS
  svchp             (IN)  - OCI service context
  errhp             (IN)  - OCI Error Handle
  object_type       (IN)  - The type of object on which the DDL statement was 
                            executed. The following are valid object types:
                            CLUSTER, FUNCTION, INDEX, LINK, OUTLINE,
                            PACKAGE, PACKAGE BODY, PROCEDURE, SEQUENCE,
                            SYNONYM, TABLE, TRIGGER, TYPE, USER, VIEW
                            LINK represents a database link.
                            NULL is also a valid object type. Specify NULL 
                            for all object types not listed.
  object_type_len   (IN)  - Length of object_type without the NULL terminator.
  ddl_text          (IN)  - The text of the DDL statement. This parameter 
                            should be set to a non-NULL value. 
                            DDL text must be in Oracle DDL format.
  ddl_text_len      (IN)  - DDL text length in bytes without NULL terminator.
  logon_user        (IN)  - Canonicalized name of the user whose session 
                            executed the DDL statement. Should follow Oracle 
                            naming conventions and size limitations.
  logon_user_len    (IN)  - logon user name length in bytes without NULL 
                            terminator.
  current_schema    (IN)  - The canonicalized schema name that is used if no 
                            schema is specified explicitly for the modified 
                            database objects in ddl_text. If a schema is 
                            specified in ddl_text that differs from the one 
                            specified for current_schema, then the schema 
                            specified in ddl_text will be used. 
                            This parameter should be set to a non-NULL value.
                            Should follow Oracle naming conventions and size 
                            limitations.
  current_schema_len (IN) - schema name length in bytes without NULL terminator
  base_table_owner  (IN)  - If the DDL statement is a table related DDL 
                            (such as CREATE TABLE and ALTER TABLE), or if the 
                            DDL statement involves a table (such as creating 
                            a trigger on a table), then base_table_owner 
                            specifies the canonicalized owner of the table 
                            involved. Otherwise, base_table_owner is NULL.
                            Should follow Oracle naming conventions and size 
                            limitations.
  base_table_owner_len (IN)- base table owner name length in bytes without NULL
                            terminator.
  base_table_name   (IN)  - If the DDL statement is a table related DDL (such
                            as CREATE TABLE and ALTER TABLE), or if the DDL 
                            statement involves a table (such as creating a 
                            trigger on a table), then base_table_name 
                            specifies the canonicalized name of the table 
                            involved. Otherwise, base_table_name is NULL.
                            Length of the above string without the NULL 
                            terminator. Should follow Oracle naming 
                            conventions and size limitations.
                            Should follow Oracle naming conventions and size 
                            limitations.
  base_table_name_len (IN)- base table name length in bytes without NULL
                            terminator.
  flag               (IN) - DDL LCR flag.
  ddl_lcrp       (IN/OUT) - XStream Ddl LCR pointer
  mode              (IN)  - mode
NOTES
  - For now, specify OCI_DEFAULT for mode
------------------------------------------------------------------------------=
*/
sword OCILCRDDLInfoSet(OCISvcCtx   *svchp,
                       OCIError    *errhp,
                       oratext     *object_type,
                       ub2          object_type_len,
                       oratext     *ddl_text,
                       ub4          ddl_text_len, 
                       oratext     *logon_user,
                       ub2          logon_user_len,
                       oratext     *current_schema,
                       ub2          current_schema_len,
                       oratext     *base_table_owner,
                       ub2          base_table_owner_len,
                       oratext     *base_table_name,
                       ub2          base_table_name_len,
                       oraub8       flag,
                       void        *ddl_lcrp,
                       ub4          mode);

/*
------------------------------------------------------------------------------=
NAME
  OCILCRDDLInfoGet - OCI LCR GET DDL INFO
DESCRIPTION
  Returns DDL information from specified lcr.
PARAMETERS
  svchp                (IN)  - OCI service context
  errhp                (IN)  - OCI Error Handle
  object_type          (OUT)  - The type of object on which the DDL statement 
                               was executed. 
                               Optional, if object_type is specified then
                               must specify object_type_len as well.
  object_type_len      (OUT)  - Length of object_type without the NULL 
                                terminator.
  ddl_text             (OUT) - The text of the DDL statement. 
                               Optional, if ddl_text is specified then
                               must specify ddl_text_len as well.
  ddl_text_len         (OUT) - DDL text length in bytes without NULL 
                               terminator.
  logon_user           (OUT) - Canonicalized name of the user whose session 
                               executed the DDL statement. 
                               Optional, if logon_user is specified then
                               must specify logon_user_len as well.
  logon_user_len       (OUT) - logon user name length in bytes without NULL 
                               terminator.
  current_schema       (OUT) - The canonicalized schema name that is used if 
                               no schema is specified explicitly for the
                               modified database objects in ddl_text.
                               Optional, if current_schema is specified then
                               must specify current_schema_len as well.
  current_schema_len   (OUT)- schema name length in bytes without NULL 
                              terminator
  base_table_owner     (OUT) - If the DDL statement is a table related DDL 
                               (such as CREATE TABLE and ALTER TABLE), or if
                               the DDL statement involves a table (such as
                               creating a trigger on a table), then
                               base_table_owner specifies the canonicalized
                               owner of the table involved. Otherwise,
                               base_table_owner is NULL.  Optional, if
                               base_table_owner is specified then must specify
                               base_table_owner_len as well.
  base_table_owner_len (OUT) - base table owner name length in bytes without 
                               NULL terminator.
  base_table_name      (OUT) - If the DDL statement is a table related DDL 
                               (such as CREATE TABLE and ALTER TABLE), or if
                               the DDL statement involves a table (such as
                               creating a trigger on a table), then
                               base_table_name specifies the canonicalized name
                               of the table involved. Otherwise,
                               base_table_name is NULL.  Optional, if
                               base_table_name is specified then must specify
                               base_table_name_len as well.
  base_table_name_len  (OUT) - base table name length in bytes without NULL
                               terminator.
  flag                 (OUT) - DDL LCR flag. Optional, data not returned if 
                               NULL.
  ddl_lcrp             (IN)  - XStream DDL LCR pointer
  mode                 (IN)  - mode (for future extention - not used currently)
RETURNS
  OCI_SUCCESS or OCI_ERROR.
NOTES
  - For now, specify OCI_DEFAULT for mode
------------------------------------------------------------------------------=
*/
sword OCILCRDDLInfoGet(OCISvcCtx   *svchp,
                       OCIError    *errhp,
                       oratext    **object_type,
                       ub2         *object_type_len,
                       oratext    **ddl_text,
                       ub4         *ddl_text_len, 
                       oratext    **logon_user,
                       ub2         *logon_user_len,
                       oratext    **current_schema,
                       ub2         *current_schema_len,
                       oratext    **base_table_owner,
                       ub2         *base_table_owner_len,
                       oratext    **base_table_name,
                       ub2         *base_table_name_len,
                       oraub8      *flag,
                       void        *ddl_lcrp,
                       ub4          mode);

/*
------------------------------------------------------------------------------=
NAME
  OCILCRAttributesSet - OCI LCR SET ATTRIBUTES
DESCRIPTION
  populates extra attribute information in ROW/DDL LCR, as well as any 
  non first class attributes that can not be set through
  OCILCRHeaderSet, OCILCRDDLInfoSet, or OCILCRRowColumnInfoSet.
  e.g. edition name
PARAMETERS
  svchp           (IN)    - OCI service context
  errhp           (IN)    - OCI Error Handle
  num_attrs       (IN)    - Number of extra attributes to be populated
  attr_names      (IN)    - Pointer to an array of attribute names. Attribute
                            names must be canonicalized and should follow 
                            Oracle naming conventions
  attr_names_lens (IN)    - Pointer to an array of attribute name lengths
                            in bytes, excluding the NULL terminator.
  attr_dtyp       (IN)    - Pointer to an array of attribute datatypes. 
  attr_valuesp    (IN)    - Pointer to an array of attribute data values.
  attr_indp       (IN)    - Pointer to an indicator array. For all datatypes,
                            this is a pointer to an array of  OCIInd values 
                            (OCI_IND_NULL/OCI_IND_NOTNULL).
  attr_alensp     (IN)    - Pointer to an array of actual attribute lengths in 
                            bytes.
  lcrp            (IN/OUT)- XStream (Row/DDL) LCR pointer
  mode            (IN)    - mode
NOTES
  - For now, specify OCI_DEFAULT for mode
------------------------------------------------------------------------------=
*/
sword OCILCRAttributesSet(OCISvcCtx   *svchp, 
                          OCIError    *errhp, 
                          ub2          num_attrs,
                          oratext    **attr_names,
                          ub2         *attr_name_lens,
                          ub2         *attr_dtyp,
                          void       **attr_valuesp,
                          OCIInd      *attr_indp,
                          ub2         *attr_alensp,
                          void        *lcrp,
                          ub4          mode);

/*
------------------------------------------------------------------------------=
NAME
  OCILCRAttributesGet - OCI LCR GET EXTRA ATTRIBUTES
DESCRIPTION
  Gets extra attribute information in (ROW/DDL) LCR, as well as any 
  non first class attributes that are not populated through
  OCILCRHeaderGet, OCILCRDDLInfoGet, or OCILCRRowColumnInfoGet
  e.g. edition name
PARAMETERS
  svchp          (IN)    - OCI service context
  errhp          (IN)    - OCI Error Handle
  num_attrs      (OUT)   - Number of extra attributes to be populated
  attr_names     (IN/OUT)- Pointer to an array of attribute names. Attribute
                           names must be canonicalized and should follow 
                           Oracle naming conventions
  attr_namesl    (IN/OUT)- Pointer to an array of attribute name lengths
                           in bytes, excluding the NULL terminator.
  attr_dtyp      (IN/OUT)- Pointer to an array of attribute datatypes. 
  attr_valuesp   (IN/OUT)- Pointer to an array of attribute data values.
  attr_indp      (IN/OUT)- Pointer to an indicator array. For all datatypes,
                           this is a pointer to an array of OCIInd values 
                           (OCI_IND_NULL/OCI_IND_NOTNULL).
  attr_alensp    (IN/OUT)- Pointer to an array of actual attribute lengths in 
                           bytes.
  lcrp           (IN)    - XStream (Row/DDL) LCR pointer
  array_size     (IN)    - Size of each of above arrays, use at least the size
                           defined by OCI_LCR_MAX_ATTRIBUTES
  mode           (IN)    - mode
NOTES
  - For now, specify OCI_DEFAULT for mode
  - If array_size is not large enough to accommodate the number of attributes
    in the requested attribute list then OCI_ERROR is returned. Parameter
    num_attrs will return the suggested size. 
------------------------------------------------------------------------------=
*/
sword OCILCRAttributesGet(OCISvcCtx   *svchp,
                          OCIError    *errhp,
                          ub2         *num_attrs,
                          oratext    **attr_names,
                          ub2         *attr_namesl,
                          ub2         *attr_dtyp,
                          void       **attr_valuesp,
                          OCIInd      *attr_indp,
                          ub2         *attr_alensp,
                          void        *lcrp,
                          ub2          array_size,
                          ub4          mode);

/*--------------------- OCILCRWhereClauseGet ----------------------------*/
/*
   NAME
       OCILCRWhereClauseGet - OCI Get Where Clause
   DESCRIPTION
     Gets the Where clause  statement for the given ROW LCR.
   PARAMETERS
       svchp                      (IN/OUT) - OCI service handle
       errhp                      (IN/OUT) - Error Handle to which errors
                                             should be reported
       wc_stmt                    (OUT)    - Sql Statement equivalent to the 
                                             LCR has applied
       wc_stmt_len                (IN/OUT) - length of wc_stmt buffer
       row_lcrp                   (IN)     - row LCR to be converted to SQL
       mode                       (IN)     - Mode flags (For future extension. 
                                             Not used currently)
   RETURNS
       OCI_SUCCESS or OCI_ERROR.
   NOTES
     - For now, specify OCI_DEFAULT for mode
     - WHERE clause generated for INSERT lcr will have all the columns that
       are being inserted. This WHERE clause could be used to identify the 
       inserted row after inserting. (like "returning ROWID").
       INSERT INTO TAB(COL1) VALUES (10) -> WHERE COL1=10
     - WHERE clause generated for UPDATE will have all the columns in the
       old column list. However the values of the columns will be that of 
       new value if it exist in the new column list
       of the UPDATE. If the column doesnt have new value then the old column
       value will be used.
       UPDATE TAB SET COL1 = 10 WHERE COL1 = 20 -> WHERE COL1 = 10
       UPDATE TAB SET COL2 = 20 WHERE COL1 = 20 -> WHERE COL1 = 20
     - WHERE clause for DELETE will use the columns and values from 
       old column lst
     - LOB piecewise operations would use the new columns and values for
       generating the WHERE clause.
*/

sword OCILCRWhereClauseGet(
     OCISvcCtx  *svchp,
     OCIError   *errhp,
     oratext    *wc_stmt,
     ub4        *wc_stmt_len,
     void       *row_lcrp,
     ub4         mode);

/*--------------------- OCILCRRowStmtGet ----------------------------*/
/*
   NAME
       OCILCRRowStmtGet - OCI Get Row Statement 
   DESCRIPTION
     Gets the SQL statement for the given ROW LCR.
   PARAMETERS
       svchp                      (IN/OUT) - OCI service handle
       errhp                      (IN/OUT) - Error Handle to which errors
                                             should be reported
       row_stmt                   (OUT)    - Sql Statement equivalent to the 
                                             LCR has applied
       row_stmt_len               (IN/OUT) - length of row_stmt buffer
       row_lcrp                   (IN)     - row LCR to be converted to SQL
       mode                       (IN)     - Mode flags (For future extension. 
                                             Not used currently)
   RETURNS
       OCI_SUCCESS or OCI_ERROR.
   NOTES
       None
*/
sword OCILCRRowStmtGet(
     OCISvcCtx  *svchp,
     OCIError   *errhp,
     oratext    *row_stmt,
     ub4        *row_stmt_len,
     void       *row_lcrp,
     ub4         mode);

/*--------------------- OCILCRWhereClauseWithBindVarGet ----------------------*/
/*
   NAME
       OCILCRWhereClauseWithBindVarGet - OCI Get Where clause with binds
   DESCRIPTION
     Gets the where clause statement with bind variables for the given ROW LCR. 
   PARAMETERS
       svchp                      (IN/OUT) - OCI service handle
       errhp                      (IN/OUT) - Error Handle to which errors
                                             should be reported
       wc_stmt                    (OUT)    - Sql Stmt equivalent to the LCR
       wc_stmt_len                (IN/OUT) - length of wc_stmt  buffer
       num_bind_var               (OUT)    - Number of bind variables
       bind_var_dtyp              (OUT)    - Array of Data types of bind 
                                             variables
       bind_var_valuesp           (OUT)    - Array of Values of bind variables
       bind_var_indp              (OUT)    - Array of null indicators of 
                                             bind variables
       bind_var_alensp            (OUT)    - Array of lengths of bind values
       bind_var_csetidp           (OUT)    - Array of char set id of binds
       bind_var_csetfp            (OUT)    - Array of char set form of binds
       row_lcrp                   (IN)     - row LCR to be converted to SQL
       array_size                 (IN)     - Size of the array of bind values
       bind_var_syntax            (IN)     - Native syntax to be used for binds
       mode                       (IN)     - Mode flags (For future extension. 
                                             Not used currently)
   RETURNS
       OCI_SUCCESS or OCI_ERROR.
   NOTES
     - For now, specify OCI_DEFAULT for mode
     - If array_size is not large enough to accommodate the number of columns
       in the requested column list then OCI_ERROR is returned. Expected 
       array_size is returned through num_bind_var parameter. 
     - bind_var_syntax for oracle should contain ":". This will generate 
       positional binds such as :1, :2, :3 etc. For other non-oracle databases
       they can give the string that needs to be used for binds. 
     - WHERE clause generated for INSERT lcr will have all the columns that
       are being inserted. This WHERE clause could be used to identify the 
       inserted row after inserting. (like "returning ROWID").
       INSERT INTO TAB(COL1) VALUES (10) -> WHERE COL1=10
     - WHERE clause generated for UPDATE will have all the columns in the
       old column list. However the values of the columns will be that of 
       new column value of the column if it exist in the new values
       of the UPDATE. If the column appears only in the old column then
       old column value will be used.
       UPDATE TAB SET COL1 = 10 WHERE COL1 = 20 -> WHERE COL1 = 10
       UPDATE TAB SET COL2 = 20 WHERE COL1 = 20 -> WHERE COL1 = 20
     - WHERE clause for DELETE will use the columns and values from 
       old column lst
     - LOB piecewise operations would use the new columns and values for
       generating the WHERE clause.
*/
sword OCILCRWhereClauseWithBindVarGet(
     OCISvcCtx *svchp,
     OCIError  *errhp,
     oratext   *wc_stmt,
     ub4       *wc_stmt_len,
     ub2       *num_bind_var,
     ub2       *bind_var_dtyp,
     void     **bind_var_valuesp,
     OCIInd    *bind_var_indp,
     ub2       *bind_var_alensp,
     ub2       *bind_var_csetidp,
     ub1       *bind_var_csetfp,
     void      *row_lcrp,
     ub2        array_size,
     oratext   *bind_var_syntax,
     ub4        mode);

/*--------------------- OCILCRRowStmtWithBindVarGet ----------------------*/
/*
   NAME
       OCILCRRowStmtWithBindVarGet - OCI Get Row Statement 
   DESCRIPTION
     Gets the SQL statement with bind variables for the given ROW LCR.
   PARAMETERS
       svchp                      (IN/OUT) - OCI service handle
       errhp                      (IN/OUT) - Error Handle to which errors
                                             should be reported
       row_stmt                   (OUT)    - Sql Stmt equivalent to the LCR
       row_stmt_len               (IN/OUT) - length of row_stmt buffer
       num_bind_var               (OUT)    - Number of bind variables
       bind_var_dtyp              (OUT)    - Array of Data types of bind 
                                             variables
       bind_var_valuesp           (OUT)    - Array of Values of bind variables
       bind_var_indp              (OUT)    - Array of null indicators of 
                                             bind variables
       bind_var_alensp            (OUT)    - Array of lengths od bind values
       bind_var_csetidp           (OUT)    - Array of char set id of binds
       bind_var_csetfp            (OUT)    - Array of char set form of binds
       row_lcrp                   (IN)     - row LCR to be converted to SQL
       chunk_column_names         (OUT)    - Array of chunked column names in 
                                             lcr
       chunk_column_namesl        (OUT)    - Length of chunk_column_names 
       chunk_column_flags         (OUT)    - flags of chunked columns in lcr
                                             Possible bit values are 
                                             OCI_LCR_COLUMN_* flags listed 
                                             above.
       array_size                 (IN)     - Size of the array of bind values
       bind_var_syntax            (IN)     - Native syntax to be used for binds
       mode                       (IN)     - Mode flags (For future extension. 
                                             Not used currently)
   RETURNS
       OCI_SUCCESS or OCI_ERROR.
   NOTES
     - For now, specify OCI_DEFAULT for mode
     - If array_size is not large enough to accommodate the number of columns
       in the requested column list then OCI_ERROR is returned. Expected 
       array_size is returned through num_bind_var parameter. 
     - bind_var_syntax for oracle should contain ":". This will generate 
       positional binds such as :1, :2, :3 etc. For other non-oracle databases
       they can give the string that needs to be used for binds. 
*/
sword OCILCRRowStmtWithBindVarGet(
     OCISvcCtx *svchp,
     OCIError  *errhp,
     oratext   *row_stmt,
     ub4       *row_stmt_len,
     ub2       *num_bind_var,
     ub2       *bind_var_dtyp,
     void     **bind_var_valuesp,
     OCIInd    *bind_var_indp,
     ub2       *bind_var_alensp,
     ub2       *bind_var_csetidp,
     ub1       *bind_var_csetfp,
     void      *row_lcrp,
     oratext  **chunk_column_names,
     ub2       *chunk_column_namesl,
     oraub8    *chunk_column_flags,
     ub2        array_size,
     oratext   *bind_var_syntax,
     ub4        mode); 

/*
-------------------------------------------------------------------------------
  NAME
    OCILCRSCNsFromPosition - Get SCNs From Position
                        
  DESCRIPTION
    Returns the SCN and commit SCN from the given position. 
  PARAMETERS
    svchp          (IN)  -  OCI service context
    errhp          (IN)  -  OCI Error Handle
    position       (IN)  -  LCR position
    position_len   (IN)  -  length of position 
    scn            (OUT) -  the SCN stored in position
    commit_scn     (OUT) -  the commit SCN stored in position 
    mode           (IN)  -  Mode flags (For future extension. Not used 
                            currently)
  RETURN
    OCI_SUCCESS if the conversion succeeds, OCI_ERROR otherwise.
  NOTE
    The user must allocate memory for the return numbers.
    The input position must conform to the format generated by an XStream 
    server.
-------------------------------------------------------------------------------
*/
sword OCILCRSCNsFromPosition(OCISvcCtx  *svchp,
                            OCIError   *errhp,
                            ub1        *position,
                            ub2         position_len,
                            OCINumber  *scn, 
                            OCINumber  *commit_scn,  
                            ub4         mode); 

/*
-------------------------------------------------------------------------------
  NAME
    OCILCRSCNToPosition - Converts SCN To Position

  DESCRIPTION
    Converts an SCN to a position. The generated position can be passed as the
    last_position to OCIXStreamOutAttach function to filter the LCRs
    with commit SCN less than the given SCN and the LCR's SCN less than the
    given SCN. This means the first LCR sent by the Outbound server is either
     - a commit LCR at the given SCN, or 
     - the first LCR of the subsequent transaction with commit SCN greater 
       than or equal to the given SCN.
  PARAMETERS
    svchp          (IN)  -  OCI service context
    errhp          (IN)  -  OCI Error Handle
    position       (OUT) -  Result position. Must pre-allocate 
                            OCI_LCR_MAX_POSITION_LEN bytes.
    position_len   (OUT) -  Length of position
    scn            (IN)  -  The SCN to be stored in position
    mode           (IN)  -  Mode flags (for future extension)
  RETURN
    OCI_SUCCESS if the conversion succeeds, OCI_ERROR otherwise.
-------------------------------------------------------------------------------
*/
sword OCILCRSCNToPosition(OCISvcCtx  *svchp,
                          OCIError   *errhp,
                          ub1        *position,
                          ub2        *position_len,
                          OCINumber  *scn,
                          ub4         mode);

/*
------------------------------------------------------------------------------=
NAME
  OCILCRLobInfoGet - OCI LCR GET LOB INFO
DESCRIPTION
  Returns the LOB information for a given piece-wise LOB LCR.
PARAMETERS
  svchp          (IN)    - OCI service context
  errhp          (IN)    - OCI Error Handle
  column_name    (OUT)   - Pointer to the LOB column name. 
                           Optional. If specified then column_name_len must
                           be specified as well.
  column_name_len(OUT)   - Length of LOB column name without the NULL 
                           terminator.
  column_dty     (OUT)   - LOB column dty - either SQLT_CHR (for CLOB) or 
                           SQLT_BIN (for BLOB).
  column_flag    (OUT)   - LOB column flag.
                           Possible bit values are OCI_LCR_COLUMN_* flags 
                           listed above.
  offset         (OUT)   - LOB operation offset in code points. Returned only
                           for LOB_TRIM and LOB_WRITE operations; otherwise,
                           a zero is returned.
                           This is the same as the 'soffset' parameter for 
                           OCILobErase or the 'offset' parameter in 
                           OCILobWrite functions.
  size           (OUT)   - LOB operation size in code points. Returned only
                           for LOB_TRIM and LOB_ERASE operations; otherwise,
                           a zero is returned.
                           This is the same as the 'new_length' parameter in 
                           OCILobTrim or the 'amtp' parameter in OCILobErase 
                           functions.
  row_lcrp       (IN)    - XStream Row LCR pointer
  mode           (IN)    - mode
NOTES
  - For now, specify OCI_DEFAULT for mode
------------------------------------------------------------------------------=
*/
sword OCILCRLobInfoGet(OCISvcCtx   *svchp, 
                       OCIError    *errhp, 
                       oratext    **column_name,
                       ub2         *column_name_len,
                       ub2         *column_dty,
                       oraub8      *column_flag,
                       ub4         *offset,
                       ub4         *size,
                       void        *row_lcrp,
                       ub4          mode);

/*
------------------------------------------------------------------------------=
NAME
  OCILCRLobInfoSet - OCI LCR SET LOB INFO
DESCRIPTION
  Sets the LOB information for a given piece-wise LOB LCR.
PARAMETERS
  svchp          (IN)    - OCI service context
  errhp          (IN)    - OCI Error Handle
  column_name    (IN)    - Pointer to the LOB column name. 
  column_name_len(IN)    - Length of LOB column name without the NULL 
                           terminator.
  column_dty     (IN)    - LOB column dty - either SQLT_CHR (for CLOB) or 
                           SQLT_BIN (for BLOB).
  column_flag    (IN)    - LOB column flag.
                           Possible bit values are OCI_LCR_COLUMN_* flags 
                           listed above.
  offset         (IN)    - LOB operation offset in code points. Returned only
                           for LOB_TRIM and LOB_WRITE operations; otherwise,
                           a zero is returned.
                           This is the same as the 'soffset' parameter for 
                           OCILobErase or the 'offset' parameter in 
                           OCILobWrite functions.
  size           (IN)    - LOB operation size in code points. Returned only
                           for LOB_TRIM and LOB_ERASE operations; otherwise,
                           a zero is returned.
                           This is the same as the 'new_length' parameter in 
                           OCILobTrim or the 'amtp' parameter in OCILobErase 
                           functions.
  row_lcrp       (IN/OUT)- XStream Row LCR pointer
  mode           (IN)    - mode
NOTES
  - For now, specify OCI_DEFAULT for mode
------------------------------------------------------------------------------=
*/
sword OCILCRLobInfoSet(OCISvcCtx   *svchp, 
                       OCIError    *errhp, 
                       oratext     *column_name,
                       ub2          column_name_len,
                       ub2          column_dty,
                       oraub8       column_flag,
                       ub4          offset,
                       ub4          size,
                       void        *row_lcrp,
                       ub4          mode);

/*---------------------------------------------------------------------------
                        XSTREAM OUT FUNCTIONS
  ---------------------------------------------------------------------------*/

/*------------------------- OCIXStreamOutAttach -----------------------------*/
/*
   NAME
       OCIXStreamOutAttach - OCI Attach to XStream Out
   DESCRIPTION
       Given the name of the server process, attach to the outbound server.
   PARAMETERS
       svchp                  (IN/OUT) - OCI service handle
       errhp                  (IN/OUT) - Error Handle for error reporting
       server_name                (IN) - Server name.
       server_name_len            (IN) - Length of server name.
       last_position              (IN) - last rcv position. (Optional)
       last_position_len          (IN) - Length of last_position.
       mode                       (IN) - Mode flags (see below) 
   RETURNS
       OCI_SUCCESS or OCI_ERROR.
   NOTES
       Specify OCI_DEFAULT for the mode parameter.

       The name of the outbound server must be provided because multiple 
       outbound servers can be configured in one Oracle instance. This call 
       returns OCI_ERROR if it encounters any error while attaching to the 
       outbound server.
       
       The last_position parameter is used to establish the starting point 
       of the stream. This call returns OCI_ERROR if the specified position 
       is non-null and less than the server's processed low-watermark; 
       otherwise, LCRs with position greater than last_position will be
       sent to the user.

       If last_position is null then the stream will start from the processed 
       low-watermark maintained in the server.
*/

sword OCIXStreamOutAttach (OCISvcCtx *svchp, OCIError *errhp, 
                           oratext *server_name, ub2 server_name_len, 
                           ub1 *last_position, 
                           ub2  last_position_len, 
                           ub4  mode);  

/* Valid modes for OCIXStreamOutAttach call */
#define OCIXSTREAM_OUT_ATTACH_RESERVED_1              (0x00000001) 
/* Application is in charge of freeing the LCRs from the outbound server */ 
#define OCIXSTREAM_OUT_ATTACH_APP_FREE_LCR            (0x00000002) 

/* Capture app container statements */
#define OCIXSTREAM_OUT_ATTACH_APP_CONTAINER           (0x00000100) 

/* Request for extended transaction id format */ 
#define OCIXSTREAM_OUT_ATTACH_EXTENDED_TXID           (0x00000200) 

/*---------------------- OCIXStreamOutProcessedLWMSet ----------------------*/
/*
   NAME
       OCIXStreamOutProcessedLWMSet - Set Processed Low-Watermark
   DESCRIPTION
       Sets the processed low-watermark maintained at the client.
   PARAMETERS
       svchp                  (IN/OUT) - OCI service handle
       errhp                  (IN/OUT) - Error Handle for error reporting
       processed_low_position     (IN) - processed low position.
       processed_low_position_len (IN) - processed low position length.
       mode                       (IN) - mode for future extension. (Not used 
                                         currently).
   RETURNS
       OCI_SUCCESS or OCI_ERROR.

   NOTES
       The processed low-watermark denotes all LCRs at or below this position
       have been processed. After successfully attaching to an XStream 
       outbound server, a local copy of the processed low-watermark is 
       maintained at the client. Periodically, this watermark is sent to the 
       server so that archived logs containing already processed transactions 
       can be purged. 

       The following API is used to update the local copy of the processed 
       low-watermark. It can be called anytime between OCIXStreamOutAttach 
       and OCIXStreamOutDetach calls. Clients, using the callback mechanism 
       to stream LCRs from the server, can invoke this API while 
       in the callback functions.
*/

sword OCIXStreamOutProcessedLWMSet (OCISvcCtx *svchp, OCIError *errhp, 
                                    ub1 *processed_low_position, 
                                    ub2  processed_low_position_len, 
                                    ub4  mode);  


/*-------------------- OCICallbackXStreamOutLCRProcess ----------------------*/
/*
   NAME
       OCICallbackXStreamOutLCRProcess - Callback to process each LCR received 
   DESCRIPTION
       This callback is invoked during OCIXStreamOutLCRCallbackReceive
       to process each LCR received from the outbound server.
   PARAMETERS
       usrctxp                (IN/OUT) - Ptr to the user context.
       lcrp                       (IN) - Pointer to the LCR just received.
       lcrtyp                     (IN) - LCR type (OCI_LCR_XROW / OCI_LCR_XDDL)
       flag                       (IN) - If OCI_XSTREAM_MORE_ROW_DATA is set,
                                         this means the current LCR has more
                                         chunk data.
   RETURNS
    This callback function must return OCI_CONTINUE to continue processing
    OCIXStreamOutLCRCallbackReceive call.  Any return code other than 
    OCI_CONTINUE signals that the client wants to terminate 
    OCIXStreamOutLCRCallbackReceive immediately.
*/
typedef sb4 (*OCICallbackXStreamOutLCRProcess) (void *usrctxp, void *lcrp,
                                                ub1 lcrtyp, oraub8 flag);


/*-------------------- OCICallbackXStreamOutChunkProcess --------------------*/
/*
   NAME
       OCICallbackXStreamOutChunkProcess - Callback to process each chunk 
   DESCRIPTION
       This callback is invoked during OCIXStreamOutLCRCallbackReceive
       to process each chunk in an LCR.
   PARAMETERS
       usrctxp     (IN/OUT) - Ptr to the user context.
       column_name     (IN) - Column name for the current chunk.
       column_name_len (IN) - Length of column name.
       column_dty      (IN) - Chunk data type (SQLT_CHR or SQLT_BIN).
       column_flag     (IN) - LCR column flags. Possible bit values are
                              OCI_LCR_COLUMN_* flags listed above.
       column_csid     (IN) - Column character set id. Relevant only if 
                              the column is an XMLType column (i.e., 
                              column_flag has OCI_LCR_COLUMN_XML_DATA bit set).
       chunk_bytes     (IN) - Chunk data length in bytes.
       chunk_data      (IN) - Chunk data buffer.
       flag            (IN) - If OCI_XSTREAM_MORE_ROW_DATA is set, this means
                              the current LCR has more chunks.
   RETURNS
    This callback function must return OCI_CONTINUE to continue processing
    OCIXStreamOutLCRCallbackReceive call.  Any return code other than 
    OCI_CONTINUE signals that the client wants to terminate 
    OCIXStreamOutLCRCallbackReceive immediately.
*/
typedef sb4 (*OCICallbackXStreamOutChunkProcess)
                  (void  *usrctxp, oratext *column_name, ub2 column_name_len,
                   ub2 column_dty, oraub8 column_flag, ub2 column_csid,
                   ub4 chunk_bytes, ub1 *chunk_data, oraub8 flag);

/*-------------------- OCIXStreamOutLCRCallbackReceive ----------------------*/
/*
   NAME
       OCIXStreamOutLCRCallbackReceive - OCI Receive LCR stream using Callbacks
   DESCRIPTION
       This API is used to get the LCR stream from the outbound server using 
       callbacks to gain better performance. The user must supply a callback 
       function to be invoked for each LCR received. If some row changes 
       in the stream may contain LOB/LONG/XMLType columns then the data for
       those columns are returned to the user in chunks. To receive those row
       changes, the user must provide a second callback to be invoked to
       process each chunk data.

       If there is an LCR available in the stream, the processlcr_cb function 
       is invoked immediately. After the processlcr_cb function exits, if the 
       current LCR contains additional chunks then the processchunk_cb function
       is invoked for each chunk belonging to that LCR.  

       If there is no LCR in the stream when the idle timeout expires (see
       OCI_ATTR_XSTREAM_IDLE_TIMEOUT), this call returns a null LCR with 
       OCI_SUCCESS code.
   PARAMETERS
       svchp                  (IN/OUT) - OCI service handle
       errhp                  (IN/OUT) - Error Handle for error reporting
       processlcr_cb              (IN) - Client callback function for each LCR.
       processchunk_cb            (IN) - Client callback function for each 
                                         chunk.
       usrctxp                    (IN) - Client context. (Optional)
       fetch_low_position         (OUT)- Fetch low watermark. (Optional)
       fetch_low_position_len     (OUT)- Fetch low watermark length.
       mode                       (IN) - mode for future extension. (Not used 
                                         currently).
   RETURNS
       OCI_SUCCESS or OCI_ERROR.
   NOTES
      - The fetch low watermark is used to indicate all transactions
        with commit position below this have been received by the XStream
        outbound server.

      - If the LCR contains non-chunked column(s), the duration of that LCR is
        limited to the processlcr_cb function. If the LCR contains some 
        chunk data then the duration of the LCR is extended until all the 
        chunks have been processed (that is, when the flag passing to
        processchunk_cb function does not have OCI_XSTREAM_MORE_ROW_DATA flag
        set). If the user wants to access the LCR data at a later time, a 
        copy of the LCR must be made.  The client callback should not modify 
        or free the LCR passing to the callback. 

        If the OCIXSTREAM_OUT_ATTACH_APP_FREE_LCR mode is passed to 
        OCIXStreamOutAttach call then the application is in charge of calling
        OCILCRFree to free each LCR. Note, this mode does not apply to the 
        chunk data associated with each LCR. Each chunk data is freed 
        immediately after each OCIXStreamOutChunkReceive call.
        
      - The given usrctxp is passed to both callbacks.

      - An ACK interval is the interval in seconds which the outbound
        server receives the processed LWM or the inbound server sends 
        the processed LWM.  The default ACK interval is 30 seconds. This 
        value can be changed by setting the OCI_ATTR_XSTREAM_ACK_INTERVAL 
        attribute using OCIAttrSet API.  This attribute is checked only 
        during the Attach call; thus, it must be set before invoking this API. 

      - The idle timeout is the interval in seconds after which the current 
        call will terminate if there is no LCR in the stream. The default 
        idle timeout is one second. This value can be changed by setting the 
        OCI_ATTR_XSTREAM_IDLE_TIMEOUT attribute using OCIAttrSet API.  This 
        attribute is checked only during the Attach call; thus, it must be 
        set before invoking this API. 

      - The outbound server ends each call at the transaction boundary 
        after an ACK interval has elapsed since the start of the call
        or when the idle timeout expires. This API returns the fetch 
        low watermark at the end of each call. 
*/
sword OCIXStreamOutLCRCallbackReceive(
  OCISvcCtx *svchp, OCIError *errhp, 
  OCICallbackXStreamOutLCRProcess processlcr_cb,
  OCICallbackXStreamOutChunkProcess processchunk_cb, void *usrctxp, 
  ub1 *fetch_low_position, ub2 *fetch_low_position_len, ub4 mode);

/*---------------------- OCIXStreamOutLCRReceive -------------------------*/
/*
   NAME
       OCIXStreamOutLCRReceive - Receive LCR without using callback 
   DESCRIPTION
       This API is used to receive an LCR from an outbound stream. If there 
       is an LCR available, this API immediately returns that LCR. 
       When there is no LCR available in the stream, this call returns a 
       null LCR after the idle timeout (see OCI_ATTR_XSTREAM_IDLE_TIMEOUT)
       has expired.

       The client must not modify the LCR received from the outbound server.
       The duration of each LCR is until the next OCIXStreamOutLCRReceive call.
       If the OCIXSTREAM_OUT_ATTACH_APP_FREE_LCR mode is passed to 
       OCIXStreamOutAttach call then the application is in charge of calling
       OCILCRFree to free each LCR. Note, this mode does not apply to the 
       chunk data associated with each LCR. Each chunk data is freed 
       immediately after each OCIXStreamOutChunkReceive call.

       To avoid network round trip for every OCIXStreamOutLCRReceive call, 
       the connection is tied to this call to let the server fill up 
       the network buffer with LCRs so subsequent calls can quickly receive 
       the LCRs from the network. The server ends each call at the 
       transaction boundary after an ACK interval has elapsed since the start
       of the call or when the idle timeout expires. See 
       OCI_ATTR_XSTREAM_ACK_INTERVAL & OCI_ATTR_XSTREAM_IDLE_TIMEOUT
       attributes.
   PARAMETERS
       svchp                  (IN/OUT) - OCI service handle
       errhp                  (IN/OUT) - Error Handle for error reporting
       lcrp                      (OUT) - Pointer to the LCR received from the
                                         stream. 
       lcrtype                   (OUT) - LCR type (OCI_LCR_XROW / OCI_LCR_XDDL)
       flag                      (OUT) - If OCI_XSTREAM_MORE_ROW_DATA is set, 
                                         it means the current LCR has more 
                                         chunk data.
       fetch_low_position         (OUT)- Fetch low watermark. (Optional)
       fetch_low_position_len     (OUT)- Fetch low watermark length.
       mode                       (IN) - mode for future extension. (Not used 
                                         currently).
   RETURNS
     -  OCI_STILL_EXECUTING means the current call is still in progress. The 
        connection associated with the specified service context handle is 
        still tied to this call for streaming the LCRs from the server. An 
        error is returned if the user attempts to use the same connection to 
        execute any OCI calls that require database round trip, for example, 
        OCIStmtExecute, OCIStmtFetch, OCILobRead, etc. OCILcr* calls are 
        local calls; thus, they are valid while the stream is in progress.
     -  OCI_SUCCESS means the current call is completed. User is free to 
        execute OCIStmt*, OCILob*, etc. from the same service context.
     -  OCI_ERROR means the current call encounters some errors. Use 
        OCIErrorGet to obtain information about the error.

   NOTES
     This call always returns a null LCR when the return code is OCI_SUCCESS. 
     In addition, it returns the fetch low position to denote the outbound 
     server has received all transactions with commit position lower than or 
     equal to this value.

     The fetch low watermark is used to indicate all transactions
     with commit position below this have been received by the XStream
     outbound server.
*/

sword OCIXStreamOutLCRReceive(
  OCISvcCtx *svchp, OCIError *errhp, 
  void     **lcrp,
  ub1       *lcrtype,
  oraub8    *flag,
  ub1       *fetch_low_position,
  ub2       *fetch_low_position_len,
  ub4        mode);

/*-------------------------- OCIXStreamOutChunkReceive ---------------------*/
/*
   NAME
       OCIXStreamOutChunkReceive - Receive Chunk data
   DESCRIPTION
       Receives next chunk of LCR data from XStream Outbound server.
       This API can only be called while OCIXStreamOutLCRReceive call is
       in progress.
   PARAMETERS
       svchp        (IN/OUT) - OCI service handle
       errhp        (IN/OUT) - Error Handle to which errors should be reported
       column_name     (OUT) - Name of column for which data is retrieved.
       column_name_len (OUT) - Length of column name.
       column_dty      (OUT) - LCR column data type.
       column_flag     (OUT) - LCR column flag. Possible bit values are
                                OCI_LCR_COLUMN_LOB_DATA     
                                OCI_LCR_COLUMN_LONG_DATA   
                                OCI_LCR_COLUMN_EMPTY_LOB  
                                OCI_LCR_COLUMN_LAST_CHUNK
                                OCI_LCR_COLUMN_AL16UTF16
                                OCI_LCR_COLUMN_ENCRYPTED   
                                OCI_LCR_COLUMN_NCLOB      
                                OCI_LCR_COLUMN_XML_DATA  
                                OCI_LCR_COLUMN_XML_DIFF 
       column_csid     (OUT) - Column character set id. This is returned only
                               if the column is an XMLType column (i.e., 
                               column_flag has OCI_LCR_COLUMN_XML_DATA bit 
                               set).
       chunk_bytes     (OUT) - Number of bytes in output buffer.
       chunk_data      (OUT) - Pointer to the chunk data in the LCR. 
                               Client must not de-allocate this pointer.
       flag            (OUT) - If OCI_XSTREAM_MORE_ROW_DATA is set, it means
                               the current LCR has more data coming.
       mode             (IN) - mode for future extension. (Not used currently).
   RETURNS
       OCI_SUCCESS - Check colname_len and chunk_bytes to determine the 
                     data just read.
       OCI_ERROR   - Error encountered. Execute OCIErrorGet to get information
                     about the error.
   NOTES
      - If the return code is OCI_SUCCESS, client should check chunk_bytes to 
        determine the # of bytes read and check column_name to determine 
        which LCR column the data associated with.

      - All the chunks from one LOB/LONG/XMLType column are returned entirely 
        before the chunk value for the next LOB/LONG/XMLType column is 
        returned.

      - The is no fixed ordering on how the LOB/LONG/XMLType columns is 
        returned. Users must check the column name to determine which column. 
        The column_flag will have OCI_LCR_COLUMN_LAST_CHUNK bit set when this 
        function returns the last chunk of each column.

      - This call returns a null column name and null chunk data if it's 
        invoked when the current LCR contains only non-chunked columns.

      - If OCIXStreamOutLCRReceive call returns OCI_XSTREAM_MORE_ROW_DATA flag 
        then the user must iteratively call OCIXStreamOutChunkReceive to 
        retrieve all the chunks belonging to the current row change before
        calling the next OCIXStreamOutLCRReceive.

*/
sword OCIXStreamOutChunkReceive(OCISvcCtx *svchp, OCIError *errhp, 
                                oratext **column_name, ub2 *column_name_len, 
                                ub2 *column_dty, oraub8 *column_flag, 
                                ub2 *column_csid, ub4 *chunk_bytes, 
                                ub1 **chunk_data, oraub8 *flag, ub4 mode);

/*------------------------- OCIXStreamOutDetach -----------------------------*/
/*
   NAME
       OCIXStreamOutDetach - OCI Detach from XStream Out
   DESCRIPTION
      Detaches from the attached XStream outbound server. This call sends the 
      current local processed low-watermark to the server before detaching 
      from the outbound server. The outbound server automatically restarts 
      after this call. This API returns OCI_ERROR if it is invoked while a 
      ReceiveLCR call is in progress.
   PARAMETERS
       svchp    (IN/OUT) - OCI service handle
       errhp    (IN/OUT) - Error Handle to which errors should be reported
       mode     (IN)     - mode for future extension. (Not used currently).
   RETURNS
       OCI_SUCCESS or OCI_ERROR.
   NOTES
      - The processed_low_position is passed to the server so it can update its
        copy.  This value if provided must be greater than or equal to the
        value maintained in the server; otherwise, an error is returned.
*/
sword OCIXStreamOutDetach (OCISvcCtx *svchp, OCIError *errhp, ub4 mode);

/*---------------------------------------------------------------------------
                        XSTREAM IN FUNCTIONS
  ---------------------------------------------------------------------------*/

/*------------------------ OCIXStreamInAttach -------------------------------*/
/*
   NAME
       OCIXStreamInAttach - OCI XStream In Attach
   DESCRIPTION
       Attaches to the specified XStream inbound server.
   PARAMETERS
       svchp                 (IN/OUT) - OCI service handle
       errhp                 (IN/OUT) - Error Handle to which errors
                                        should be reported
       server_name               (IN) - XStream inbound server name.
       server_name_len           (IN) - Length of server name.
       source_name               (IN) - source name to identify the data src.
       source_name_len           (IN) - Length of source name.
       last_position            (OUT) - Last position received by inbound
                                        server. Optional. If specified must 
                                        pre-allocate OCI_LCR_MAX_POSITION_LEN 
                                        bytes for return value.
       last_position_len        (OUT) - Length of last_position. Must be 
                                        non-NULL if last_position is non-NULL.
       mode                      (IN) - Mode flags (For future extension. 
                                        (Not used currently)
   RETURNS
       OCI_SUCCESS or OCI_ERROR.
   NOTES
       The last_position parameter is returned to establish the starting point 
       to resume the inbound stream. The client should start sending LCRs with 
       positions greater than the last_position since the inbound server will
       ignore all LCRs with positions less than or equal to this value. 
*/

sword OCIXStreamInAttach(
      OCISvcCtx *svchp,
      OCIError  *errhp,
      oratext   *server_name,
      ub2        server_name_len,
      oratext   *source_name,
      ub2        source_name_len,
      ub1       *last_position, 
      ub2       *last_position_len, 
      ub4        mode);

/*--------- Valid modes for OCIXStreamInAttach -------------*/
/* Restart inbound server regardless if it's in DISABLED or ABORTED state. */
#define OCIXSTREAM_IN_ATTACH_RESTART_INBOUND          (0x00000001)

/*-------------------- OCICallbackXStreamInLCRCreate ------------------------*/
/*
   NAME
       OCICallbackXStreamInLCRCreate - Callback to create an LCR 
   DESCRIPTION
       This callback is invoked during OCIXStreamInLCRCallbackSend
       to create each LCR to be sent to the inbound server.
   PARAMETERS
       usrctxp                (IN/OUT) - Ptr to the user context
       lcrp                      (OUT) - Pointer to the LCR to be sent
       lcrtyp                    (OUT) - LCR type (OCI_LCR_XROW / OCI_LCR_XDDL)
       flag                      (OUT) - If OCI_XSTREAM_MORE_ROW_DATA is set,
                                         this means the current LCR has more
                                         chunk data.
   RETURNS
    This callback function must return OCI_CONTINUE to continue processing
    OCIXStreamInLCRCallbackSend call.  Any return code other than 
    OCI_CONTINUE signals that the client wants to terminate 
    OCIXStreamInLCRCallbackSend immediately.
*/
typedef sb4  (*OCICallbackXStreamInLCRCreate)(
               void   *usrctxp,
               void  **lcrp,  
               ub1    *lcrtyp,  
               oraub8 *flag);

/*-------------------- OCICallbackXStreamInChunkCreate --------------------*/
/*
   NAME
       OCICallbackXStreamInChunkCreate - Callback to create each chunk 
   DESCRIPTION
       This callback is invoked during OCIXStreamInLCRCallbackSend
       to create each chunk to be sent to the inbound server.
   PARAMETERS
       usrctxp      (IN/OUT) - Ptr to the user context.
       column_name     (OUT) - Column name for the current chunk.
       column_name_len (OUT) - Length of column name.
       column_dty      (OUT) - Chunk data type (SQLT_CHR or SQLT_BIN).
       column_flag     (OUT) - LCR column flags. Possible bit values are
                               OCI_LCR_COLUMN_* flags listed above.
       column_csid     (OUT) - Column character set id. Relevant only if 
                               the column is an XMLType column (i.e., 
                               column_flag has OCI_LCR_COLUMN_XML_DATA bit 
                               set).
       chunk_bytes     (OUT) - Chunk data length in bytes.
       chunk_data      (OUT) - Chunk data buffer.
       flag            (OUT) - If OCI_XSTREAM_MORE_ROW_DATA is set, this means
                               the current LCR has more chunks.
   RETURNS
    This callback function must return OCI_CONTINUE to continue processing
    OCIXStreamInLCRCallbackSend call.  Any return code other than 
    OCI_CONTINUE signals that the client wants to terminate 
    OCIXStreamInLCRCallbackSend immediately.
*/
typedef sb4  (*OCICallbackXStreamInChunkCreate)(
               void      *usrctxp,
               oratext  **column_name, 
               ub2       *column_name_len, 
               ub2       *column_dty, 
               oraub8    *column_flag,
               ub2       *column_csid,
               ub4       *chunk_bytes, 
               ub1      **chunk_data, 
               oraub8    *flag);

/*--------------------- OCIXStreamInLCRCallbackSend ------------------------*/
/*
   NAME
       OCIXStreamInLCRCallbackSend - OCI XStream In Send LCR to Inbound Server
   DESCRIPTION
       Sends LCR stream to XStream inbound server using callbacks.
       The API invokes createlcr_cb function to obtain each LCR to send to the 
       server. If the return flag from the createlcr_cb function has 
       OCI_XSTREAM_MORE_ROW_DATA bit set, then it invokes createchunk_cb 
       procedure to obtain each chunk. It repeatedly calls createchunk_cb 
       function while the flag returned from this callback has 
       OCI_XSTREAM_MORE_ROW_DATA bit set. When this bit is not set, this API 
       cycles back to invoke createlcr_cb function to get the next LCR. 
       This cycle is repeated until the createlcr_cb function returns a null 
       LCR or when an ACK interval has elapsed since the start of the call.
       See OCI_ATTR_XSTREAM_ACK_INTERVAL attribute.
   PARAMETERS
       svchp                 (IN/OUT) - OCI service handle
       errhp                 (IN/OUT) - Error Handle to which errors
                                        should be reported
       createlcr_cb              (IN) - Callback function to be invoked
                                        to generate an LCR for streaming. 
                                        Cannot be null.
       createchunk_cb            (IN) - Callback function to be invoked to 
                                        create each chunk. Can be null if the 
                                        user does not need to send any LCR with
                                        LOB/LONG/XMLType columns. OCI_ERROR 
                                        will be returned if this argument is 
                                        null and the user attempts to send an 
                                        LCR with additional chunk data.
       usrctxp                   (IN) - Client context to pass to both
                                        callback functions.
       mode                      (IN) - Mode flags (For future extension. 
                                        Not used currently)
   RETURNS
       OCI_SUCCESS or OCI_ERROR.
   NOTES
       None
*/

sword OCIXStreamInLCRCallbackSend(
       OCISvcCtx                              *svchp,
       OCIError                               *errhp,
       OCICallbackXStreamInLCRCreate           createlcr_cb,
       OCICallbackXStreamInChunkCreate         createchunk_cb,
       void                                   *userctxp,
       ub4                                     mode);

/*---------------------------- OCIXStreamInLCRSend --------------------------*/
/*
   NAME
       OCIXStreamInLCRSend - OCI XStream In Send LCR to Inbound Server
   DESCRIPTION
       Sends LCR stream to XStream inbound server without using callbacks.
       To avoid a network round trip for every OCIXStreamInLCRSend call,    
       the connection is tied to this call for at least the duration 
       specified by the OCI_ATTR_XSTREAM_ACK_INTERVAL attribute.
   PARAMETERS
       svchp                 (IN/OUT) - OCI service handle
       errhp                 (IN/OUT) - Error Handle to which errors
                                        should be reported
       lcrp                      (IN) - Pointer to the LCR to send. Cannot
                                        be null.
       lcrtype                   (IN) - LCR type (OCI_LCR_XROW / OCI_LCR_XDDL)
       flag                      (IN) - If OCI_XSTREAM_MORE_ROW_DATA is set,
                                        it means the current LCR has more
                                        chunk data.
       mode                      (IN) - Mode flags (For future extension. 
                                        Not used currently)
   RETURNS
     -  OCI_STILL_EXECUTING means the current call is still in progress. The 
        connection associated with the specified service context handle is 
        still tied to this call for streaming the LCRs to the server. An error 
        is returned if the user attempts to use the same connection to 
        execute any OCI calls that require database round trip, for example, 
        OCIStmtExecute, OCIStmtFetch, OCILobRead, etc. OCILcr* calls are 
        local calls; thus, they are valid while this call is in progress.
     -  OCI_SUCCESS means the current call is completed. User is free to 
        execute OCIStmt*, OCILob*, etc. from the same service context.
     -  OCI_ERROR means this call encounters some errors. Use OCIErrorGet to 
        obtain information about the error.
*/
sword OCIXStreamInLCRSend(
       OCISvcCtx     *svchp,
       OCIError      *errhp,
       void          *lcrp,
       ub1            lcrtype,
       oraub8         flag,
       ub4            mode);

/*----------------------------- OCIXStreamInChunkSend -----------------------*/
/*
   NAME
       OCIXStreamInChunkSend - Send Chunk
   DESCRIPTION
       Sends the given chunk of column data to XStream Inbound server.
       This chunk is associated with the LCR that is sent by the 
       most recent OCIXStreamInLCRSend call prior to this call.
   PARAMETERS
       svchp       (IN/OUT) - OCI service handle
       errhp       (IN/OUT) - Error Handle to which errors should be reported
       column_name     (IN) - Name of column for which data is sent.
                              Column names must be canonicalized and must 
                              follow Oracle naming conventions.
       column_name_len (IN) - Length of column name.
       column_dty      (IN) - LCR column data type (must be SQLT_CHR or 
                              SQLT_BIN).
       column_flag     (IN) - LCR column flags. Possible bit values are
                                OCI_LCR_COLUMN_LOB_DATA
                                OCI_LCR_COLUMN_LONG_DATA
                                OCI_LCR_COLUMN_EMPTY_LOB
                                OCI_LCR_COLUMN_LAST_CHUNK
                                OCI_LCR_COLUMN_AL16UTF16
                                OCI_LCR_COLUMN_ENCRYPTED
                                OCI_LCR_COLUMN_NCLOB
                                OCI_LCR_COLUMN_XML_DATA
                                OCI_LCR_COLUMN_XML_DIFF
       column_csid     (IN) - Column character set id. This is required only
                              if the column is an XMLType column (i.e., 
                              column_flag has OCI_LCR_COLUMN_XML_DATA bit set).
       chunk_bytes     (IN) - Chunk data length in bytes.
       chunk_data      (IN) - Chunk data buffer.
       flag            (IN) - If OCI_XSTREAM_MORE_ROW_DATA is set, it means
                              the current LCR has more data coming.
       mode            (IN) - mode for future extension. (Not used currently).
RETURNS
   OCI_SUCCESS - Successful call.
   OCI_ERROR   - Error encountered. Execute OCIErrorGet to get information
                 about the error.
NOTES
 - This function must be called while OCIXStreamInLCRSend is in progress.
   
 - This function is valid only if the associated LCR's cmd type is
   INSERT, UPDATE or LOB_WRITE. It can be invoked multiple times for the
   same LCR. 

 - This API is not valid for LOB_ERASE and LOB_TRIM LCRs.

 - The chunk values for different columns can not be interleaved. If a 
   column contains multiple chunks, this procedure must be called 
   consecutively using the same column name before proceeding to a new column. 
   The ordering in which the LOB/LONG/XMLType column values are set is 
   irrelevant. 

 - The OCI_LCR_COLUMN_LAST_CHUNK must be specified for the last chunk of
   each column. 

 - Only one column can be specified for LOB_WRITE operation.

 - For NCLOB or varying width CLOB, the input buffer must be in 
   AL16UTF16 format.

 - For INSERT operation, each LOB/LONG/XMLType column, with value set using 
   OCIXStreamInChunkSend, must be included in the current LCR's NEW 
   column list. The value of that LOB/LONG/XMLType column must be set to 
   null and must have OCI_LCR_COLUMN_EMPTY_LOB flag defined.
 
*/
sword OCIXStreamInChunkSend (OCISvcCtx *svchp, OCIError *errhp,
                             oratext  *column_name, ub2 column_name_len, 
                             ub2 column_dty, oraub8 column_flag,
                             ub2 column_csid, ub4 chunk_bytes, 
                             ub1 *chunk_data, oraub8 flag, ub4 mode);

/*--------------------- OCIXStreamInDetach ----------------------------*/
/*
   NAME
       OCIXStreamInDetach - OCI XStream In Detach from Inbound Server
   DESCRIPTION
       Detaches from XStream inbound server and returns the inbound server's 
       processed low-watermark.
   PARAMETERS
       svchp                 (IN/OUT) - OCI service handle
       errhp                 (IN/OUT) - Error Handle to which errors
                                        should be reported
       processed_low_position   (OUT) - Inbound server's processed low
                                        position. Must pre-allocate 
                                        OCI_LCR_MAX_POSITION_LEN bytes for
                                        output buffer.
       processed_low_position_len(OUT)- Processed_low_position length.
       mode                      (IN) - Mode flags (For future extension. 
                                        Not used currently)
   RETURNS
       OCI_SUCCESS or OCI_ERROR.
   NOTES
       None
*/
sword OCIXStreamInDetach(
       OCISvcCtx *svchp, 
       OCIError  *errhp,
       ub1       *processed_low_position,
       ub2       *processed_low_position_len,
       ub4        mode);

/*--------- Valid modes for OCIXStreamInDetach -------------*/
/* Restart inbound server when calling detach. */
#define OCIXSTREAM_IN_DETACH_RESTART_INBOUND          (0x00000001)

/*--------------------- OCIXStreamInProcessedLWMGet -------------------------*/
/*
   NAME
     OCIXStreamInProcessedLWMGet - OCI XStream In Get LowWatermark 
   DESCRIPTION
     Returns XStream inbound server's processed low watermark 
     cached at the client.
   PARAMETERS
     svchp                      (IN/OUT) - OCI service handle
     errhp                      (IN/OUT) - Error Handle to which errors
                                           should be reported
     processed_low_position        (OUT) - Inbound server's cached processed
                                           low position. Must pre-
                                           allocate OCI_LCR_MAX_POSITION_LEN 
                                           bytes for output buffer.
     processed_low_position_len    (OUT) - Processed_low_position length.
     mode                           (IN) - Mode flags (For future extension.
                                           Not used currently)
   RETURNS
       OCI_SUCCESS or OCI_ERROR.
   NOTES
       None
*/
sword OCIXStreamInProcessedLWMGet(
       OCISvcCtx *svchp,
       OCIError  *errhp,
       ub1       *processed_low_position,
       ub2       *processed_low_position_len,
       ub4        mode);

/*-------------------------- OCIXStreamInFlush ------------------------------*/
/*
   NAME
     OCIXStreamInFlush - OCI XStream In Flush network
   DESCRIPTION
     Flushes network and terminates any in-progress OCIXStreamInLCRSend or
     OCIXStreamInLCRCallbackSend call associated with the given service handle.
   PARAMETERS
     svchp                      (IN/OUT) - OCI service handle
     errhp                      (IN/OUT) - Error Handle to which errors
                                           should be reported
     mode                           (IN) - Mode flags (see below)
   RETURNS
     OCI_SUCCESS or OCI_ERROR.
   NOTES
     Each call will incur a database round trip to get the server's processed 
     low-watermark, which the user can retrieve afterward using 
     OCIXStreamInProcessedLWMGet API. This API should be called only when 
     there is no LCR to send to the server and the client wants to know the 
     progress of the attached inbound server.

     This call returns OCI_ERROR if it is invoked from the callback functions
     of OCIXStreamInLCRCallbackSend API.

     Client must have attached to an XStream inbound server prior to calling
     this API. 

  Valid Mode flags:
  - OCIXSTREAM_IN_FLUSH_WAIT_FOR_COMPLETE :  flush network and wait for all
    complete and rollback transactions sent to the inbound server to complete
    before returning control to the client.
      
*/
sword OCIXStreamInFlush(
       OCISvcCtx *svchp,
       OCIError  *errhp,
       ub4        mode);


/*-------------------------- OCIXStreamInCommit -----------------------------*/
/*
   NAME
       OCIXStreamInCommit - OCI XStream In Commit 
   DESCRIPTION
       Commits current transaction
   PARAMETERS
       svchp                      (IN/OUT) - OCI service handle
       errhp                      (IN/OUT) - Error Handle to which errors
                                             should be reported
       lcrp                           (IN) - Pointer to the LCR to send. Must
                                             be a commit LCR.
       mode                           (IN) - Mode flags (For future extension.
                                             Not used currently)
   RETURNS
       OCI_SUCCESS or OCI_ERROR.
   NOTES
       The position of the input LCR must be higher than
       DBA_XSTREAM_INBOUND_PROGRESS.APPLIED_HIGH_POSITION and the LCR's source
       database must match DBA_APPLY_PROGRESS.SOURCE_DATABASE of the attached
       inbound server.

       Upon receiving this LCR, the inbound server will check if there is any
       unapplied complete or rollback transaction remaining. If none is found
       it inserts a row to the apply progress table based on the input LCR and
       commit the current transaction; otherwise, it returns an error.

       If there is any pre-commit handler defined, it will be executed when
       this commit LCR is executed.
*/
sword OCIXStreamInCommit(
  OCISvcCtx *svchp,
  OCIError  *errhp,
  void      *lcrp,
  ub4        mode);

/*-------------------------- OCIXStreamInErrorGet ---------------------------*/
/*
   NAME
     OCIXStreamInErrorGet - OCI XStream In Get Error Info
   DESCRIPTION
     Returns the first error encountered by the inbound server since the 
     Attach call.
   PARAMETERS
     svchp                      (IN/OUT) - OCI service handle
     errhp                      (IN/OUT) - Error Handle
     errcodep                   (OUT)    - Error code
     msgbuf                     (IN/OUT) - Pre-allocated message buffer
     msg_bufsize                (IN)     - Message buffer size
     msg_len                    (OUT)    - Length of returned error message
     txn_id                     (IN/OUT) - Pre-allocated txn id buffer
     txn_id_bufsize             (IN)     - Txn_id buffer size
     txn_id_len                 (OUT)    - Length of returned txn id
   RETURNS
     OCI_SUCCESS or OCI_ERROR.
   NOTES
     The maximum size for the returned txn id is OCI_LCR_MAX_TXID_LEN. If the 
     allocated buffer for txn_id is too small, this routine returns ORA-29258.
     The maximum size for the returned error msg is OCI_ERROR_MAXMSG_SIZE. If
     the allocated size for msgbuf is too small, the returned message will be
     truncated.
*/
sword OCIXStreamInErrorGet(
       OCISvcCtx *svchp,
       OCIError  *errhp,
       sb4       *errcodep,   
       oratext   *msgbuf,    
       ub2        msg_bufsize,
       ub2       *msg_len,
       oratext   *txn_id,
       ub2        txn_id_bufsize, 
       ub2       *txn_id_len);

/*
------------------------------------------------------------------------------=
NAME
  OCIXStreamOutSessionSet - OCI XStream Out Session Set attribute
DESCRIPTION
  Sets session attributes for XStream Out
PARAMETERS
  svchp                   (IN) - OCI service context
  errhp                   (IN) - OCI Error Handle
  attribute_name          (IN) - Attribute name
  attribute_name_len      (IN) - Attribute name length
  attribute_value         (IN) - Attribute value
  attribute_value_len     (IN) - Attribute value length
  attribute_dty           (IN) - Attribute dty
  mode                    (IN) - mode
RETURNS
  OCI_SUCCESS if successful, OCI_ERROR otherwise
NOTES
------------------------------------------------------------------------------=
*/
sword OCIXStreamOutSessionSet(OCISvcCtx   *svchp,
                              OCIError    *errhp,
                              oratext     *attribute_name,
                              ub2          attribute_name_len,
                              void        *attribute_value,
                              ub2          attribute_value_len,
                              ub2          attribute_dty,
                              ub4          mode);

/*
------------------------------------------------------------------------------=
NAME
  OCIXStreamInSessionSet - OCI XStream In Session Set attribute
DESCRIPTION
  Sets session attributes for XStream In
PARAMETERS
  svchp                   (IN) - OCI service context
  errhp                   (IN) - OCI Error Handle
  attribute_name          (IN) - Attribute name
  attribute_name_len      (IN) - Attribute name length
  attribute_value         (IN) - Attribute value
  attribute_value_len     (IN) - Attribute value length
  attribute_dty           (IN) - Attribute dty
  mode                    (IN) - mode
RETURNS
  OCI_SUCCESS if successful, OCI_ERROR otherwise
NOTES
------------------------------------------------------------------------------=
*/
sword OCIXStreamInSessionSet(OCISvcCtx   *svchp,
                             OCIError    *errhp,
                             oratext     *attribute_name,
                             ub2          attribute_name_len,
                             void        *attribute_value,
                             ub2          attribute_value_len,
                             ub2          attribute_dty,
                             ub4          mode);

/*
------------------------------------------------------------------------------=
NAME
  OCILCRComparePosition
FUNCTION
  Will compare two LCRID values. These LCRIDs can have different versions.
  The provided position must be a valid LCRID for 12.2.
PARAMETERS
  svchp             -  (IN) OCI service context.
  errhp             -  (IN) OCI Error handle.
  position1         -  (IN) The first position to compare.
  position1_len     -  (IN) The first position's length. 
  position2         -  (IN) The second position to compare.
  position2_len     -  (IN) The second position's length.
  mode              -  (IN) Mode flags.
  result            -  (OUT) 0 if both values are equal,
                             -1 if position1  is less than position2,
                             1 if position1 is greater than position 2.
RETURNS
  OCI_SUCCESS if the conversion succeeds, OCI_ERROR otherwise.
NOTES
  Supported modes:
    0 - Complete byte comparison.
    2 - Smaller length is smaller value
------------------------------------------------------------------------------=
*/
  sword OCILCRComparePosition(OCISvcCtx  *svchp,
                              OCIError   *errhp,
                              ub1        *position1,
                              ub2         position1_len,
                              ub1        *position2,
                              ub2         position2_len,
                              ub4         mode,
                              sb2        *result);

/*
------------------------------------------------------------------------------=
NAME
  OCILCRConvertPosition
FUNCTION
  Converts an LCRID value to the specified version (1 or 2). The provided LCRID
  must be valid for 12.2.
PARAMETERS
  svchp             -  (IN) OCI service context.
  errhp             -  (IN) OCI Error handle.
  in_position       -  (IN) The position to be converted.
  in_position_len   -  (IN) The length of the position to be converted.
  out_position      -  (OUT) The converted position's value.
  out_position_len  -  (OUT) The converted postition's length.
  to_version        -  (IN) The LCRID version we want to convert to.
  mode              -  (IN) Mode flags.
RETURNS
  OCI_SUCCESS if the conversion succeeds.
  OCI_SUCCESS if the LCRID is already in the desired version.
  OCI_ERROR if the conversion fails.
NOTES
------------------------------------------------------------------------------=
*/
  sword OCILCRConvertPosition(OCISvcCtx  *svchp,
                              OCIError   *errhp,
                              ub1        *in_position,
                              ub2         in_position_len,
                              ub1        *out_position,
                              ub2        *out_position_len,
                              ub1         to_version,
                              ub4         mode);

/*
------------------------------------------------------------------------------=
NAME
  OCILCRSCNToPosition2
FUNCTION
  Converts SCN to position (LCRID), version 1 and version 2 compatible.
PARAMETERS
  svchp             -  (IN) OCI service context.
  errhp             -  (IN) OCI Error handle.
  position          -  (OUT) The converted position's value.
  position_len      -  (OUT) The converted position's length.
  scn               -  (IN) The SCN to be stored in position.
  version           -  (IN) The converted position's LCRID version.
  mode              -  (IN) Mode flags.
RETURNS
  OCI_SUCCESS if the conversion succeeds, OCI_ERROR otherwise.
NOTES
  The given SCN is assumed to be the commit SCN. 
------------------------------------------------------------------------------=
*/
  sword OCILCRSCNToPosition2(OCISvcCtx  *svchp,
                             OCIError   *errhp,
                             ub1        *position,
                             ub2        *position_len,
                             OCINumber  *scn,
                             ub1         version,
                             ub4         mode);

/*
------------------------------------------------------------------------------=
NAME
  OCILCRGetLCRIDVersion
FUNCTION
  Determines the LCRID Version of a given position.
PARAMETERS
  svchp             -  (IN) OCI service context.
  errhp             -  (IN) OCI Error handle.
  position          -  (IN) The position.
  position_len      -  (IN) The position's length.
  version           -  (OUT) LCRID version.
RETURNS
  OCI_SUCCESS if the LCRID version is valid, OCI_ERROR otherwise.
NOTES
------------------------------------------------------------------------------=
*/
  sword OCILCRGetLCRIDVersion(OCISvcCtx  *svchp,
                              OCIError   *errhp,
                              ub1        *position,
                              ub2         position_len,
                              ub1        *version);
    
/*---------------------------------------------------------------------------
                          INTERNAL FUNCTIONS
  ---------------------------------------------------------------------------*/


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif                                                  /* OCIXSTREAM_ORACLE */
