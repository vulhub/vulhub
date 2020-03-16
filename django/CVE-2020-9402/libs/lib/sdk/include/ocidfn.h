/* Copyright (c) 1991, 2012, Oracle and/or its affiliates. 
All rights reserved. */
/*
   NAME
     ocidfn.h - OCI Definations
   NOTES
     Shipped to users.
   MODIFIED   (MM/DD/YY)
    kkverma    12/19/12 - define host area type
    sagrawal   12/03/10 - Boolean Binds
    mbastawa   09/16/05 - dbhygiene
    dmukhin    06/29/05 - ANSI prototypes; miscellaneous cleanup 
    srseshad   11/25/02 - change binary float/double codes
    srseshad   11/14/02 - Add SQLT_IBFLOAT, SQLT_IBDOUBLE
    mxyang     09/17/02 - grabtrans 'mmorsi_obj_float'
    srseshad   09/06/02 - Add binary float/double
    aahluwal   06/04/02 - bug 2360115
    kmuthukk   05/02/00 - add SQLT_PNTY
    amangal    07/30/99 - Merge into 8.1.6 : Bug 879031
    tnbui      07/28/99 - Remove SQLT_TIMESTAMP_ITZ                            
    tnbui      07/21/99 - SQLT_TIMESTAMP_LTZ                                   
    tnbui      06/16/99 - TIMESTAMP WITH IMPLICIT TIME ZONE                    
    whe        04/07/99 - bug#810075
    whe        03/19/99 - lrg 32079 etc.: move HDA def from ocidem.h to ocidfn.
    skmishra   05/10/98 -
    vyanaman   04/16/98 - update sql92 datatypes
    khnguyen   01/16/98 -
    khnguyen   12/23/97 - SQLT* for datetimes and intervals
    tanguyen   08/19/97 -
    dchatter   03/18/97 -  porting exception 390897
    dchatter   05/02/97 -   merge porting exception
    dalpern    12/04/96 -   SQLCS_LIT_NULL added
    cxcheng    11/14/96 -   add SQLT_BFILE/SQLT_CFILE to fix compile prob
    cxcheng    11/12/96 -   add SQLT_NCO for named collection
    lchidamb   10/17/96 -   add SQLT_VST and SQLT_ODT
    sgollapu   10/14/96 -   Mutual exclusion of ocidfn and sqldef
    sgollapu   10/07/96 -   OCI Simplification
    aroy       09/09/96 -   add SQLCS* definitions
    slari      08/07/96 -  add SQLT_RDD, rowid descriptor
    slari      06/12/96 -  remove SQLT_TTBL
    dchatter   04/21/96 -  prepare for merge into main
    slari      08/24/95 -  b299432, define CDA_SIZE
    zwalcott   02/28/96 -  add SQLT_BFILEE and SQLT_CFILEE.
    lchidamb   02/22/96 -  make dtys consistent with dtydef.h
    lchidamb   02/16/96 -  add SQLT_BFILEE and SQLT_CFILEE
    lchidamb   01/30/96 -  rename new datatypes for v8
    lchidamb   09/06/95 -  add new datatypes
    slari      05/11/95 -  add OCI_EV_DEF and OCI_EV_TSF
    dchatter   04/06/95 -  add ifdef flags around OCI_flags
    dchatter   03/08/95 -  piece values
    dchatter   03/06/95 -  merge changes from branch 1.2.720.3
    jfbrown    02/17/95 -  merge changes from branch 1.2.720.2
    dchatter   02/08/95 -  olog call modes
    jfbrown    02/03/95 -  remove non-printable characters
    lchidamb   12/06/94 -  merge changes from branch 1.2.720.1
    lchidamb   10/04/94 -  added field chk to cda_head, cda_def
    dchatter   07/05/94 -  SQLT_CUR added
    rkooi2     11/27/92 -  Changing e* datatypes to s* 
    rkooi2     10/26/92 -  More portability mods 
    rkooi2     10/22/92 -  Added #ifndef ORATYPES ... 
    rkooi2     10/18/92 -  Changes to make it portable. 
    sjain      03/16/92 -  Creation 
*/

/*
 *  ocidfn.h
 *
 *  Common header file for OCI C sample programs.
 *  This header declares the cursor and logon data area structure.
 *  The types used are defined in <oratypes.h>.
 *
 */
 
#ifndef OCIDFN
#define OCIDFN

#include <oratypes.h>

/* The cda_head struct is strictly PRIVATE.  It is used
   internally only. Do not use this struct in OCI programs. */

struct cda_head {
    sb2          v2_rc;
    ub2          ft;
    ub4          rpc;
    ub2          peo;
    ub1          fc;
    ub1          rcs1;
    ub2          rc;
    ub1          wrn;
    ub1          rcs2;
    sword        rcs3;
    struct {
        struct {
           ub4    rcs4;
           ub2    rcs5;
           ub1    rcs6;
        } rd;
        ub4    rcs7;
        ub2    rcs8;
    } rid;
    sword        ose;
    ub1           chk;
    void         *rcsp;
};

/*
** Size of HDA area:
** 512 for 64 bit arquitectures
** 256 for 32 bit arquitectures
*/

#if defined(SS_64BIT_SERVER) || defined(__64BIT__)
# define HDA_SIZE 512
#else
# define HDA_SIZE 256
#endif

#if defined(SS_64BIT_SERVER) || defined(__64BIT__)
#define CDA_SIZE 88
#else
# define CDA_SIZE 64
#endif

/* the real CDA, padded to CDA_SIZE bytes in size */
struct cda_def {
    sb2          v2_rc;                                    /* V2 return code */
    ub2          ft;                                    /* SQL function type */
    ub4          rpc;                                /* rows processed count */
    ub2          peo;                                  /* parse error offset */
    ub1          fc;                                    /* OCI function code */
    ub1          rcs1;                                        /* filler area */
    ub2          rc;                                       /* V7 return code */
    ub1          wrn;                                       /* warning flags */
    ub1          rcs2;                                           /* reserved */
    sword        rcs3;                                           /* reserved */
    struct {                                              /* rowid structure */
        struct {
           ub4    rcs4;
           ub2    rcs5;
           ub1    rcs6;
        } rd;
        ub4    rcs7;
        ub2    rcs8;
    } rid;
    sword        ose;                                 /* OSD dependent error */
    ub1           chk;
    void         *rcsp;                          /* pointer to reserved area */
    ub1          rcs9[CDA_SIZE - sizeof (struct cda_head)];        /* filler */
};

typedef struct cda_def Cda_Def;

/* the logon data area (LDA)
   is the same shape as the CDA */
typedef struct cda_def Lda_Def;

/* define host area type */
typedef ub8 Hda_AlignType;
typedef Hda_AlignType Hda_Def[HDA_SIZE/sizeof(Hda_AlignType)];

/* OCI Environment Modes for opinit call */
#define OCI_EV_DEF 0                  /* default single-threaded environment */
#define OCI_EV_TSF 1                              /* thread-safe environment */

/* OCI Logon Modes for olog call */
#define OCI_LM_DEF 0                                        /* default login */
#define OCI_LM_NBL 1                                   /* non-blocking logon */

/* 
 * since sqllib uses both ocidef and ocidfn the following defines
 * need to be guarded
 */
#ifndef OCI_FLAGS 
#define OCI_FLAGS

/* OCI_*_PIECE defines the piece types that are returned or set
*/
#define OCI_ONE_PIECE   0                 /* there or this is the only piece */
#define OCI_FIRST_PIECE 1                        /* the first of many pieces */
#define OCI_NEXT_PIECE  2                         /* the next of many pieces */
#define OCI_LAST_PIECE  3                   /* the last piece of this column */
#endif

#ifndef SQLDEF 

/* input data types */
#define SQLT_CHR  1                        /* (ORANET TYPE) character string */
#define SQLT_NUM  2                          /* (ORANET TYPE) oracle numeric */
#define SQLT_INT  3                                 /* (ORANET TYPE) integer */
#define SQLT_FLT  4                   /* (ORANET TYPE) Floating point number */
#define SQLT_STR  5                                /* zero terminated string */
#define SQLT_VNU  6                        /* NUM with preceding length byte */
#define SQLT_PDN  7                  /* (ORANET TYPE) Packed Decimal Numeric */
#define SQLT_LNG  8                                                  /* long */
#define SQLT_VCS  9                             /* Variable character string */
#define SQLT_NON  10                      /* Null/empty PCC Descriptor entry */
#define SQLT_RID  11                                                /* rowid */
#define SQLT_DAT  12                                /* date in oracle format */
#define SQLT_VBI  15                                 /* binary in VCS format */
#define SQLT_BFLOAT 21                                /* Native Binary float*/
#define SQLT_BDOUBLE 22                             /* NAtive binary double */
#define SQLT_BIN  23                                  /* binary data(DTYBIN) */
#define SQLT_LBI  24                                          /* long binary */
#define SQLT_UIN  68                                     /* unsigned integer */
#define SQLT_SLS  91                        /* Display sign leading separate */
#define SQLT_LVC  94                                  /* Longer longs (char) */
#define SQLT_LVB  95                                   /* Longer long binary */
#define SQLT_AFC  96                                      /* Ansi fixed char */
#define SQLT_AVC  97                                        /* Ansi Var char */
#define SQLT_IBFLOAT  100                          /* binary float canonical */
#define SQLT_IBDOUBLE 101                         /* binary double canonical */
#define SQLT_CUR  102                                        /* cursor  type */
#define SQLT_RDD  104                                    /* rowid descriptor */
#define SQLT_LAB  105                                          /* label type */
#define SQLT_OSL  106                                        /* oslabel type */

#define SQLT_NTY  108                                   /* named object type */
#define SQLT_REF  110                                            /* ref type */
#define SQLT_CLOB 112                                       /* character lob */
#define SQLT_BLOB 113                                          /* binary lob */
#define SQLT_BFILEE 114                                   /* binary file lob */
#define SQLT_CFILEE 115                                /* character file lob */
#define SQLT_RSET 116                                     /* result set type */
#define SQLT_NCO  122      /* named collection type (varray or nested table) */
#define SQLT_VST  155                                      /* OCIString type */
#define SQLT_ODT  156                                        /* OCIDate type */

/* datetimes and intervals */
#define SQLT_DATE                      184                      /* ANSI Date */
#define SQLT_TIME                      185                           /* TIME */
#define SQLT_TIME_TZ                   186            /* TIME WITH TIME ZONE */
#define SQLT_TIMESTAMP                 187                      /* TIMESTAMP */
#define SQLT_TIMESTAMP_TZ              188       /* TIMESTAMP WITH TIME ZONE */
#define SQLT_INTERVAL_YM               189         /* INTERVAL YEAR TO MONTH */
#define SQLT_INTERVAL_DS               190         /* INTERVAL DAY TO SECOND */
#define SQLT_TIMESTAMP_LTZ             232        /* TIMESTAMP WITH LOCAL TZ */



#define SQLT_PNTY   241              /* pl/sql representation of named types */

/* some pl/sql specific types */
#define SQLT_REC    250                     /* pl/sql 'record' (or %rowtype) */
#define SQLT_TAB    251                            /* pl/sql 'indexed table' */
#define SQLT_BOL    252                                  /* pl/sql 'boolean' */

/* cxcheng: this has been added for backward compatibility -
   it needs to be here because ocidfn.h can get included ahead of sqldef.h */
#define SQLT_FILE SQLT_BFILEE                             /* binary file lob */
#define SQLT_CFILE SQLT_CFILEE
#define SQLT_BFILE SQLT_BFILEE
 
/* CHAR/NCHAR/VARCHAR2/NVARCHAR2/CLOB/NCLOB char set "form" information */
#define SQLCS_IMPLICIT 1     /* for CHAR, VARCHAR2, CLOB w/o a specified set */
#define SQLCS_NCHAR    2                  /* for NCHAR, NCHAR VARYING, NCLOB */
#define SQLCS_EXPLICIT 3   /* for CHAR, etc, with "CHARACTER SET ..." syntax */
#define SQLCS_FLEXIBLE 4                 /* for PL/SQL "flexible" parameters */
#define SQLCS_LIT_NULL 5      /* for typecheck of NULL and empty_clob() lits */

#endif  /* SQLDEF */
#endif  /* OCIDFN */
