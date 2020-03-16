/*
 * 
 */

/* Copyright (c) 1991, 2005, Oracle. All rights reserved.  */
/* Copyright (c) 1991, 2005, Oracle. All rights reserved.  */
/*
   NAME
     ocidem.h - OCI demo header
   MODIFIED   (MM/DD/YY)
    dmukhin    06/29/05  - ANSI prototypes; miscellaneous cleanup 
    whe        04/07/99 -  bug#810071
    whe        03/19/99 -  lrg 32079 etc.: putting static back for oci_func_tab
    nmacnaug   02/02/99 -  static declarations should not be in header files
    mygopala   09/22/97 -  Fix for bug 550351
    surman     03/14/97 -  Merge 413362 to 8.0.3
    surman     11/08/96 -  413362: Add SS_64BIT_SERVER macro
    emendez    04/07/94 -  merge changes from branch 1.6.710.1
    emendez    02/02/94 -  Fix for bug 157576
    jnlee      01/05/93 -  include oratypes.h once, make oci_func_tab static
    rkooi2     10/26/92 -  More portability mods 
    rkooi2     10/22/92 -  Change text back to char to avoid casts 
    rkooi2     10/20/92 -  Changes to make it portable 
    sjain      03/16/92 -  Creation 
*/

/*
 *  ocidem.h
 *
 *  Declares additional functions and data structures
 *  used in the OCI C sample programs.
 */


#ifndef ORATYPES
#include <oratypes.h>
#endif /* ORATYPES */

#ifndef OCIDFN
#include <ocidfn.h>
#endif /* OCIDFN */

#ifndef OCIDEM
#define OCIDEM


/*  internal/external datatype codes */
#define VARCHAR2_TYPE            1
#define NUMBER_TYPE              2
#define INT_TYPE                 3
#define FLOAT_TYPE               4
#define STRING_TYPE              5
#define ROWID_TYPE              11
#define DATE_TYPE               12

/*  ORACLE error codes used in demonstration programs */
#define VAR_NOT_IN_LIST       1007
#ifndef NO_DATA_FOUND
#  define NO_DATA_FOUND         1403
#endif
#define NULL_VALUE_RETURNED   1405

/*  some SQL and OCI function codes */
#define FT_INSERT                3
#define FT_SELECT                4
#define FT_UPDATE                5
#define FT_DELETE                9

#define FC_OOPEN                14

/*
 *  OCI function code labels,
 *  corresponding to the fc numbers
 *  in the cursor data area.
 */
static const text  *oci_func_tab[] =  {(text *) "not used",
/* 1-2 */       (text *) "not used", (text *) "OSQL",
/* 3-4 */       (text *) "not used", (text *) "OEXEC, OEXN",
/* 5-6 */       (text *) "not used", (text *) "OBIND",
/* 7-8 */       (text *) "not used", (text *) "ODEFIN",
/* 9-10 */      (text *) "not used", (text *) "ODSRBN",
/* 11-12 */     (text *) "not used", (text *) "OFETCH, OFEN",
/* 13-14 */     (text *) "not used", (text *) "OOPEN",
/* 15-16 */     (text *) "not used", (text *) "OCLOSE",
/* 17-18 */     (text *) "not used", (text *) "not used",
/* 19-20 */     (text *) "not used", (text *) "not used",
/* 21-22 */     (text *) "not used", (text *) "ODSC",
/* 23-24 */     (text *) "not used", (text *) "ONAME",
/* 25-26 */     (text *) "not used", (text *) "OSQL3",
/* 27-28 */     (text *) "not used", (text *) "OBNDRV",
/* 29-30 */     (text *) "not used", (text *) "OBNDRN",
/* 31-32 */     (text *) "not used", (text *) "not used",
/* 33-34 */     (text *) "not used", (text *) "OOPT",
/* 35-36 */     (text *) "not used", (text *) "not used",
/* 37-38 */     (text *) "not used", (text *) "not used",
/* 39-40 */     (text *) "not used", (text *) "not used",
/* 41-42 */     (text *) "not used", (text *) "not used",
/* 43-44 */     (text *) "not used", (text *) "not used",
/* 45-46 */     (text *) "not used", (text *) "not used",
/* 47-48 */     (text *) "not used", (text *) "not used",
/* 49-50 */     (text *) "not used", (text *) "not used",
/* 51-52 */     (text *) "not used", (text *) "OCAN",
/* 53-54 */     (text *) "not used", (text *) "OPARSE",
/* 55-56 */     (text *) "not used", (text *) "OEXFET",
/* 57-58 */     (text *) "not used", (text *) "OFLNG",
/* 59-60 */     (text *) "not used", (text *) "ODESCR",
/* 61-62 */     (text *) "not used", (text *) "OBNDRA",
/* 63-64 */     (text *) "OBINDPS",  (text *) "ODEFINPS",
/* 65-66 */     (text *) "OGETPI",   (text *) "OSETPI"
};

#endif      /* OCIDEM */

