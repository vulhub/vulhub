/* Copyright (c) 1981, 2005, Oracle. All rights reserved.  */
/* Copyright (c) 1984, 2005, Oracle. All rights reserved.  */
 
/*
NAME
  ocidef
CONTENTS
  Oracle Call Interface cursor area and LDA definitions
NOTES
  none
OWNER
  Oates
DATE
  09/07/82
MODIFIED
    mbastawa   09/16/05  - dbhygiene
    dmukhin    06/29/05  - ANSI prototypes; miscellaneous cleanup 
    srseshad   03/25/03  - convert oci public api to ansi
    csteinba   11/05/02  - bug 2521931: redefine OTYACL
    aahluwal   06/03/02  - bug 2360115
    bpalaval   02/08/01  - Change text to oratext.
    chliang    02/01/01  - fix olint error.
    bpalaval   11/16/00  - Bug 1244685 : Fix ALTER CLUSTER issue..
    slari      11/08/00  - remove functions duplicated in ociapr.h
    whe        09/01/99 -  976457:check __cplusplus for C++ code
    skmishra   04/23/97 -  Provide C++ compatibility
    lchidamb   06/26/96 -  put upidef.h, riddef.h within #ifndef
    slari      06/12/96 -  add ocigft_getFcnType
    dchatter   11/10/95 -  ocignfd - oci get native file descriptor
    slari      05/11/95 -  change OCIEVDEF/OCIEVTSF to UPIEVDEF/UPIEVTSF
    dchatter   04/06/95 -  add ifdef flags around OCI_flags
    lchidamb   04/06/95 -  drop maxdsz from ocibndps/ocidfnps
    slari      04/07/95 -  rename opinit to ocipin
    slari      03/13/95 -  thread safety changes
    dchatter   03/08/95 -  piece definitions OCI_*_PIECE
    lchidamb   12/06/94 -  add support for binding/defining with skips
    dchatter   03/06/95 -  merge changes from branch 1.12.720.1
    dchatter   02/06/95 -  add defines for login mode parameters
    dchatter   07/06/94 -  Deleting CRSCHK, with refcursor, no way to set this
    dchatter   06/13/94 -  add a new LDA flag LDANBL
    rkooi      11/18/92 -  update ocidpr interface 
    mmoore     10/31/92 -  add ocidpr 
    gpongrac   11/17/92 -  fix oexfet prototype 
    sjain      01/03/92 -  Add ocibra 
    rjenkins   11/04/91 -  adding prototypes for oparse and oexfet 
    sjain      04/15/91 -         Change ocistf proto 
    sjain      04/01/91 -         Rearrange oty codes. Add new ones 
    Jain       12/03/90 - Add #define for new describe call
    Jain       11/29/90 - Add new function code for the new oci calls
    Mendels    01/20/89 - fix 19170: make ocitbl CONST_DATA
    Kabcene    01/27/88 - change interfaces to match V5
    Navab      12/09/87 - add a parameter to ocierr call
    Navab      11/30/87 - add ocierr, rename ocioer
    Navab      10/08/87 - add prototypes for procedure declarations
    Howard     09/07/87 - endif blah
    Howard     05/11/87 - Add OTY types
    Howard     04/27/87 - move ocldef defines here
    Oates      10/15/85 - Add OCANCEL
    Oates      09/30/85 - Implement ORA*Net
    Oates      06/27/85 - Make datatype compatible with upidef.h
    Andy       05/07/85 - delete CSRFBPIC
*/

#ifndef UPIDEF 
#include <upidef.h>
#endif

#ifndef RIDDEF
#include <riddef.h>
#endif

#include <ociapr.h>

#ifndef OCIDEF
#define OCIDEF
 
 
#define CSRCHECK 172                                   /* csrdef is a cursor */
#define LDACHECK 202                          /* csrdef is a login data area */
struct csrdef
{
   sb2     csrrc;                /* return code: v2 codes, v4 codes negative */
   ub2     csrft;                                           /* function type */
   ub4     csrrpc;                                   /* rows processed count */
   ub2     csrpeo;                                     /* parse error offset */
   ub1     csrfc;                                           /* function code */
   ub1     csrlfl;                     /* lda flag to indicate type of login */
   ub2     csrarc;                        /* actual untranslated return code */
   ub1     csrwrn;                                          /* warning flags */
   ub1     csrflg;                                           /* error action */
   sword   csrcn;                                           /* cursor number */
   riddef  csrrid;                                        /* rowid structure */
   sword   csrose;                                /* os dependent error code */
   ub1     csrchk;                      /* check byte = CSRCHECK - in cursor */
                                        /* check byte = LDACHECK - in    LDA */
   struct hstdef *csrhst;                              /* pointer to the hst */
};
typedef struct csrdef csrdef;
typedef struct csrdef ldadef;                    /* lda is the same as a csr */
 
 
/* values for csrlfl */
#define LDAFLG 1                                            /* ...via ologon */
#define LDAFLO 2                                     /* ...via olon or orlon */
#define LDANBL 3                                  /* ...nb logon in progress */
 
/* valuses for crsfc */
#define csrfpa 2                                                  /* ...OSQL */
#define csrfex 4                                                 /* ...OEXEC */
#define csrfbi 6                                                 /* ...OBIND */
#define csrfdb 8                                                /* ...ODFINN */
#define csrfdi 10                                               /* ...ODSRBN */
#define csrffe 12                                               /* ...OFETCH */
#define csrfop 14                                                /* ...OOPEN */
#define csrfcl 16                                               /* ...OCLOSE */
#define csrfds 22                                                 /* ...ODSC */
#define csrfnm 24                                                /* ...ONAME */
#define csrfp3 26                                                /* ...OSQL3 */
#define csrfbr 28                                               /* ...OBNDRV */
#define csrfbx 30                                               /* ...OBNDRN */
/*#defe csrfdf 32*/                                                  /* ???? */
#define csrfso 34                                                 /* ...OOPT */
#define csrfre 36                                               /* ...ORESUM */
#define csrfbn 50                                               /* ...OBINDN */
#define csrfca 52                                               /* ..OCANCEL */
#define csrfsd 54                                                 /* ..OSQLD */
#define csrfef 56                                                /* ..OEXFEN */
#define csrfln 58                                                 /* ..OFLNG */
#define csrfdp 60                                                /* ..ODSCSP */
#define csrfba 62                                                /* ..OBNDRA */
#define csrfbps 63                                               /*..OBINDPS */
#define csrfdps 64                                              /*..ODEFINPS */
#define csrfgpi 65                                              /* ...OGETPI */
#define csrfspi 66                                              /* ...OSETPI */

/* values for csrwrn */
#define CSRWANY  0x01                         /* there is a warning flag set */
#define CSRWTRUN 0x02                           /* a data item was truncated */
#define CSRWNVIC 0x04      /* NULL values were used in an aggregate function */
#define CSRWITCE 0x08           /* column count not equal to into list count */
#define CSRWUDNW 0x10               /* update or delete without where clause */
#define CSRWRSV0 0x20
#define CSRWROLL 0x40                                   /* rollback required */
#define CSRWRCHG 0x80       /* change after query start on select for update */
 
/* values fro csrflg */
#define CSRFSPND 0x01                         /* current operation suspended */
#define CSRFATAL 0x02            /* fatal operation: transaction rolled back */
#define CSRFBROW 0x04                              /* current row backed out */
#define CSRFREFC 0x08     /* ref cursor type CRSCHK disabled for this cursor */
#define CSRFNOAR 0x10     /* ref cursor type binds, so no array bind/execute */
 
/* define function codes; in order of octdef.h */
#define OTYCTB 1                                             /* CREATE TABLE */
#define OTYSER 2                                                 /* set role */
#define OTYINS 3                                                   /* INSERT */
#define OTYSEL 4                                                   /* SELECT */
#define OTYUPD 5                                                   /* UPDATE */
#define OTYDRO 6                                                /* drop role */
#define OTYDVW 7                                                /* DROP VIEW */
                                                  /* once was validate index */
                                                /* once was create partition */
                                                 /* once was alter partition */
#define OTYDTB 8                                               /* DROP TABLE */
                                                     /* once was alter space */
                                                      /* once was drop space */
#define OTYDEL 9                                                   /* DELETE */
#define OTYCVW 10                                             /* create view */
#define OTYDUS 11                                               /* drop user */
#define OTYCRO 12                                             /* create role */
#define OTYCSQ 13                                         /* create sequence */
#define OTYASQ 14                                          /* alter sequence */
#define OTYACL 15                                           /* alter cluster */
#define OTYDSQ 16                                           /* drop sequence */
#define OTYCSC 17                                           /* create schema */
#define OTYCCL 18                                          /* CREATE CLUSTER */
                                                   /* once was alter cluster */
#define OTYCUS 19                                             /* create user */
#define OTYCIX 20                                            /* CREATE INDEX */
#define OTYDIX 21                                              /* DROP INDEX */
#define OTYDCL 22                                            /* DROP CLUSTER */
#define OTYVIX 23                                          /* validate index */
#define OTYCPR 24                                        /* create procedure */
#define OTYAPR 25                                         /* alter procedure */
#define OTYATB 26                                             /* alter table */
                                                        /* once was evaluate */
#define OTYXPL 27                                                 /* explain */
#define OTYGRA 28                                                   /* grant */
#define OTYREV 29                                                  /* revoke */
#define OTYCSY 30                                          /* create synonym */
#define OTYDSY 31                                            /* drop synonym */
#define OTYASY 32                                 /* alter system switch log */
#define OTYSET 33                                         /* set transaction */
#define OTYPLS 34                                          /* pl/sql execute */
#define OTYLTB 35                                                    /* lock */
#define OTYNOP 36                                                    /* noop */
#define OTYRNM 37                                                  /* rename */
#define OTYCMT 38                                                 /* comment */
#define OTYAUD 39                                                   /* audit */
#define OTYNOA 40                                                /* no audit */
#define OTYAIX 41                                             /* ALTER INDEX */
#define OTYCED 42                                /* create external database */
#define OTYDED 43                                  /* drop external database */
#define OTYCDB 44                                         /* create database */
#define OTYADB 45                                          /* alter database */
#define OTYCRS 46                                 /* create rollback segment */
#define OTYARS 47                                  /* alter rollback segment */
#define OTYDRS 48                                   /* drop rollback segment */
#define OTYCTS 49                                       /* create tablespace */
#define OTYATS 50                                        /* alter tablespace */
#define OTYDTS 51                                         /* drop tablespace */
#define OTYASE 52                                           /* alter session */
#define OTYAUR 53                                              /* alter user */
#define OTYCWK 54                                           /* commit (work) */
#define OTYROL 55                                                /* rollback */
#define OTYSPT 56                                               /* savepoint */
 
/* For number greater than 56 the the type is the same as defined in 
** octdef.h for that number. So for completion look at octdef.h 
*/

#define OTYDEV OTYCVW                       /* old DEFINE VIEW = create view */
 
/* FUNCTION CODES */
#define OCLFPA  2                                            /* parse - OSQL */
#define OCLFEX  4                                         /* execute - OEXEC */
#define OCLFBI  6                                    /* BIND by name - OBIND */
#define OCLFDB  8                                 /* define buffer -  ODEFIN */
#define OCLFDI  10                                   /* describe item - ODSC */
#define OCLFFE  12                                         /* fetch - OFETCH */
#define OCLFOC  14                                    /* open cursor - OOPEN */
#       define OCLFLI  OCLFOC            /* old name for open cursor - OOPEN */
#define OCLFCC  16                                  /* close cursor - OCLOSE */
#       define OCLFLO  OCLFCC          /* old name for close cursor - OCLOSE */
#define OCLFDS  22                                        /* describe - ODSC */
#define OCLFON  24                     /* get table and column names - ONAME */
#define OCLFP3  26                                          /* parse - OSQL3 */
#define OCLFBR  28                        /* bind reference by name - OBNDRV */
#define OCLFBX  30                        /* bind referecne numeric - OBNDRN */
#define OCLFSO  34                                /* special function - OOPT */
#define OCLFRE  36                                        /* resume - ORESUM */
#define OCLFBN  50                                                  /* bindn */
#define OCLFMX  52                                /* maximum function number */

#ifdef NEVER                                                 /* unused codes */
# define OCLFLK  18                           /* open  for kernel operations */
# define OCLFEK  20                             /* execute kernel operations */
# define OCLFOK  22                                          /* kernel close */
# define OCLFIN  28                                       /* logon to oracle */
# define OCLFOF  30                                    /* logoff from oracle */
# define OCLFAX  32                               /* allocate a context area */
# define OCLFPI  34                                  /* page in context area */
# define OCLFIS  36                                  /* special system logon */
# define OCLFCO  38                          /* cancel the current operation */
# define OCLFGI  40                                       /* get database id */
# define OCLFJN  42                                     /* journal operation */
# define OCLFCL  44                       /* cleanup prior execute operation */
# define OCLFMC  46                                     /* map a cursor area */
# define OCLFUC  48                  /* unmap cursor and restore user maping */
#endif /*NEVER                                           *//* obsolete codes */
 

/* values for ocimode in ocipin call */

#define OCIEVDEF UPIEVDEF           /* default : non-thread safe enivronment */
#define OCIEVTSF UPIEVTSF                         /* thread-safe environment */


/* OCIL* flags used to determine the mode of login, using ocilog().
** Currently defined only for non-blocking and thread-safe logins.
*/

#define OCILMDEF        UPILMDEF                   /* default, regular login */
#define OCILMNBL        UPILMNBL                       /* non-blocking logon */
#define OCILMESY        UPILMESY            /* thread safe but external sync */
#define OCILMISY        UPILMISY                  /* internal sync, we do it */
#define OCILMTRY        UPILMTRY        /* try to, but do not block on mutex */


/* 
 * since sqllib uses both ocidef and ocidfn the following defines
 * need to be guarded
 */
#ifndef OCI_FLAGS 
#define OCI_FLAGS

/* OCI_*_PIECE defines the piece types that are returned or set
*/

#define OCI_ONE_PIECE   UPI_ONE_PIECE     /* there or this is the only piece */
#define OCI_FIRST_PIECE UPI_FIRST_PIECE          /* the first of many pieces */
#define OCI_NEXT_PIECE  UPI_NEXT_PIECE            /* the next of many pieces */
#define OCI_LAST_PIECE  UPI_LAST_PIECE      /* the last piece of this column */
#endif

/*
**  OCITAB: define return code pairs for version 2 to 3 conversions
*/
struct  ocitab
{
   sb2  ocitv3;                                   /* Version 3/4 return code */
   sb2  ocitv2;                          /* Version 2 equivalent return code */
};
typedef struct ocitab ocitab;
 
externref const ocitab ocitbl[];
 
/* macros to check cursors and LDA's.  */
/* macros to set error codes           */

# define CRSCHK(c)     if ((c->csrchk != CSRCHECK)\
                            && !bit(c->csrflg, CSRFREFC))\
                          return(ocir32(c, OER(1001)))
# define ldaerr(l, e)  ( l->csrrc = (sb2)(-( l->csrarc = (ub2)(e)) ) )
# define LDACHK(l)     if (l->csrchk != LDACHECK) \
                          return(ldaerr(l, OER(1001)))
 

/************************************************/
/*         OCI PROCEDURE DECLARATIONS           */
/************************************************/ 




/*****************************/
/*  Database logon/logout    */
/*****************************/
sword ocilog(  ldadef *lda, struct hstdef *hst, oratext *uid, sword uidl, 
                  oratext *psw, sword pswl, oratext* conn, sword connl,
                  ub4 mode );
sword ocilon(  ldadef *lda, oratext *uid, sword uidl, oratext *psw, 
                  sword pswl, sword audit );
sword  ocilgi(  ldadef *lda, sb2 areacount  );
sword ocirlo(  ldadef *lda, struct hstdef *hst, oratext *uid, sword uidl,
            oratext *psw, sword pswl, sword audit  );
     /* ocilon - logon to oracle
     ** ocilgi - version 2 compatible ORACLE logon call.
     **          no login to ORACLE is performed: the LDA is initialized
     ** ocirlo - version 5 compatible ORACLE Remote Login call,
     **          oracle login is executed.
     **   lda     - pointer to ldadef
     **   uid     - user id [USER[/PASSWORD]]
     **   uidl    - length of uid, if -1 strlen(uid) is used
     **   psw     - password string; ignored if specified in uid
     **   pswl    - length of psw, if -1 strlen(psw) is used
     **   audit   - is not supported; the only permissible value is 0
     **   areacount - unused
     */

sword ocilof(  ldadef *lda  );
     /*
     ** ocilof - disconnect from ORACLE
     **   lda     - pointer to ldadef
     */


/*********************/
/*   Error Messages  */
/*********************/
sword ocierr(  ldadef *lda, sb2 rcode, oratext *buffer, sword bufl  );
sword ocidhe(  sb2 rcode, oratext *buffer  );
    /* 
    ** Move the text explanation for an ORACLE error to a user defined buffer
    **  ocierr - will return the message associated with the hstdef stored 
    **           in the lda.
    **  ocidhe - will return the message associated with the default host.
    **    lda    - lda associated with the login session
    **    rcode  - error code as returned by V3 call interface
    **    buffer - address of a user buffer of at least 132 characters
    */


/***********************/
/*  Cursor Open/Close  */
/***********************/
sword ociope(  struct csrdef *cursor, ldadef *lda, oratext *dbn, sword dbnl, 
                 sword areasize, oratext *uid, sword uidl  );

sword ociclo(  struct csrdef *cursor  );
   /* 
   ** open or close a cursor.
   **   cursor - pointer to csrdef
   **   ldadef - pointer to ldadef
   **   dbn    - unused
   **   dbnl   - unused
   **   areasize - if (areasize == -1)  areasize <- system default initial size
   **              else if (areasize IN [1..256]) areasize <- areasize * 1024;
   **              most applications should use the default size since context
   **              areas are extended as needed until memory is exhausted.
   **   uid    - user id
   **   uidl   - userid length
   */

/***********************************/
/*      CONTROL AND OPTIONS        */
/***********************************/
sword ocibre(  ldadef *lda  );
   /*
   **  ocibrk - Oracle Call Interface send BReaK Sends a break to
   **  oracle.  If oracle is  active,  the  current  operation  is
   **  cancelled.  May be called  asynchronously.   DOES  NOT  SET
   **  OERRCD in the hst.  This is because ocibrk  may  be  called
   **  asynchronously.  Callers must test the return code.
   **    lda  - pointer to a ldadef 
   */

sword ocican(  struct csrdef *cursor  );
   /*
   **  cancel the operation on the cursor, no additional OFETCH calls
   **  will be issued for the existing cursor without an intervening 
   **  OEXEC call.
   **   cursor  - pointer to csrdef
   */

sword ocisfe(  struct csrdef *cursor, sword erropt, sword waitopt  );
   /* 
   ** ocisfe - user interface set error options
   ** set the error and cursor options.
   ** allows user to set the options for dealing with fatal dml errors
   ** and other cursor related options
   ** see oerdef for valid settings
   **   cursor  - pointer to csrdef
   **   erropt  - error optionsn
   **   waitopr - wait options
   */


/***************************************/
/* COMMIT/ROLLBACK/AUTOCOMMIT          */
/***************************************/
sword   ocicom(  ldadef *lda  );
sword   ocirol(  ldadef *lda  );
   /*
   ** ocicom - commit the current transaction
   ** ocirol - roll back the current transaction
   */
 
sword   ocicon(  ldadef *lda  );
sword   ocicof(  ldadef *lda  );
   /*
   ** ocicon - auto Commit ON
   ** ocicof - auto Commit OFf
   */


 
/************************/
/*     parsing          */
/************************/
sword    ocisq3(struct csrdef *cursor, oratext * /* sqlstm */, sword sqllen);
   /*
   ** ocisq3 - user interface parse sql statement
   **   cursor - pointer to csrdef
   **   sqlstm - pointer to SQL statement
   **   sqllen - length of SQL statement.  if -1, strlen(sqlstm) is used
   */



/***************************/
/*      BINDING            */
/***************************/
/* these are for the opcode in ocibndps, ocidfnps */
#define OCI_PCWS 0
#define OCI_SKIP 1

sword ocibin(  struct csrdef *cursor, oratext *sqlvar, sword sqlvl, 
                  ub1 *progv, sword progvl, sword ftype, sword scale, 
                  oratext *fmt, sword fmtl, sword fmtt  );
sword  ocibrv(  struct csrdef *cursor, oratext *sqlvar, sword sqlvl, 
                ub1 *progv, sword progvl, sword ftype, sword scale, sb2 *indp,
                oratext *fmt, sword fmtl, sword fmtt  );
sword  ocibra(  struct csrdef *cursor, oratext *sqlvar, sword sqlvl, 
                   ub1 *progv, sword progvl, sword ftype, sword scale, 
                   sb2 *indp, ub2 *aln, ub2 *rcp, ub4 mal, ub4 *cal, 
                   oratext *fmt, sword fmtl, sword fmtt  );
sword  ocibndps(  struct csrdef *cursor, ub1 opcode, oratext *sqlvar, 
                     sb4 sqlvl, ub1 *progv, sb4 progvl, sword ftype, 
                     sword scale, sb2 *indp, ub2 *aln, ub2 *rcp, sb4 pv_skip, 
                     sb4 ind_skip, sb4 len_skip, sb4 rc_skip, ub4 mal, 
                     ub4 *cal, oratext *fmt, sb4 fmtl, sword fmtt  );
sword ocibnn (  struct csrdef *cursor, ub2 sqlvn, ub1 *progv, sword progvl,
                   sword ftype, sword scale, oratext *fmt, sword fmtl, 
                   sword fmtt  );
sword  ocibrn(  struct csrdef *cursor, sword sqlvn, ub1 *progv, sword progvl,
                sword ftype, sword scale, sb2 *indp, oratext *fmt, sword fmtl, 
                sword fmtt  );
    /*
    ** ocibin - bind by value by name
    ** ocibrv - bind by reference by name
    ** ocibra - bind by reference by name (array)
    ** ocibndps - bind by reference by name (array) piecewise or with skips
    ** ocibnn - bind by value numeric
    ** ocibrn - bind by reference numeric
    **
    ** the contents of storage specified in bind-by-value calls are
    ** evaluated immediately.
    ** the addresses of storage specified in bind-by-reference calls are
    ** remembered, and the contents are examined at every execute.
    **
    **  cursor  - pointer to csrdef
    **  sqlvn   - the number represented by the name of the bind variables
    **            for variables of the form :n or &n for n in [1..256)
    **            (i.e. &1, :234).  unnecessarily using larger numbers
    **            in the range wastes space.
    **  sqlvar  - the name of the bind variable (:name or &name)
    **  sqlval  - the length of the name;
    **            in bindif -1, strlen(bvname) is used
    **  progv   - pointer to the object to bind.
    **  progvl  - length of object to bind.
    **            in bind-by-value if specified as -1 then strlen(bfa) is
    **              used (really only makes sends with character types)
    **            in bind-by-value, if specified as -1 then UB2MAXVAL
    **              is used.  Again this really makes sense only with
    **              SQLT_STR.
    **  ftype   - datatype of object
    **  indp    - pointer to indicator variable.
    **              -1     means to ignore bfa/bfl and bind NULL;
    **              not -1 means to bind the contents of bfa/bfl
    **              bind the contents pointed to by bfa
    **  aln     - Alternate length pointer
    **  rcp     - Return code pointer
    **  mal     - Maximum array length
    **  cal     - Current array length pointer
    **  fmt     - format string
    **  fmtl    - length of format string; if -1, strlen(fmt) is used
    **  fmtt    - desired output type after applying forat mask. Not
    **            really yet implemented
    **  scale   - number of decimal digits in a cobol packed decimal (type 7)
    **
    ** Note that the length of bfa when bound as SQLT_STR is reduced
    ** to strlen(bfa).
    ** Note that trailing blanks are stripped of storage of SQLT_STR.
    */
    
/***************************/
/*        DESCRIBING       */
/***************************/
sword ocidsc (  struct csrdef *cursor, sword pos, sb2 *dbsize, sb2 *fsize,
             sb2 *rcode, sb2 *dtype, sb1 *buf, sb2 *bufl, sb2 *dsize  );
sword ocidsr(  struct csrdef *cursor, sword pos, sb2 *dbsize, sb2 *dtype, 
            sb2 *fsize  );
sword   ocinam(  struct csrdef *cursor, sword pos, sb1 *tbuf, sb2 *tbufl,
                sb1 *buf, sb2 *bufl );
    /*
    **  ocidsc, ocidsr: Obtain information about a column
    **  ocinam : get the name of a column
    **   cursor  - pointer to csrdef
    **   pos     - position in select list from [1..N]
    **   dbsize  - place to store the database size
    **   fsize   - place to store the fetched size
    **   rcode   - place to store the fetched column returned code
    **   dtype   - place to store the data type
    **   buf     - array to store the column name
    **   bufl    - place to store the column name length
    **   dsize   - maximum display size
    **   tbuf    - place to store the table name
    **   tbufl   - place to store the table name length
    */

sword ocidsp (  struct csrdef *cursor, sword pos, sb4 *dbsize, sb2 *dbtype,
                   sb1 *cbuf, sb4 *cbufl, sb4 *dsize, sb2 *pre, sb2 *scl,
                   sb2 *nul );

sword ocidpr( ldadef *lda, oratext *object_name, size_t object_length,
                void * reserved1, size_t reserved1_length, void * reserved2,
                size_t reserved2_length, ub2 *overload, ub2 *position,
                ub2 *level, oratext **argument_name, ub2 *argument_length, 
                ub2 *datatype, ub1 *default_supplied, ub1 *in_out, 
                ub4 *length, sb2 *precision, sb2 *scale, ub1 *radix, 
                ub4 *spare, ub4 *total_elements );
   /*
   ** OCIDPR - User Program Interface: Describe Stored Procedure
   **
   ** This routine is used to obtain information about the calling
   ** arguments of a stored procedure.  The client provides the 
   ** name of the procedure using "object_name" and "database_name"
   ** (database name is optional).  The client also supplies the 
   ** arrays for OCIDPR to return the values and indicates the 
   ** length of array via the "total_elements" parameter.  Upon return
   ** the number of elements used in the arrays is returned in the 
   ** "total_elements" parameter.  If the array is too small then 
   ** an error will be returned and the contents of the return arrays 
   ** are invalid.
   **
   **
   **   EXAMPLE :
   **
   **   Client provides -
   **
   **   object_name    - SCOTT.ACCOUNT_UPDATE@BOSTON
   **   total_elements - 100
   **   
   **
   **   ACCOUNT_UPDATE is an overloaded function with specification :
   **
   **     type number_table is table of number index by binary_integer;
   **     table account (account_no number, person_id number,
   **                    balance number(7,2))
   **     table person  (person_id number(4), person_nm varchar2(10))
   **
   **      function ACCOUNT_UPDATE (account number, 
   **         person person%rowtype, amounts number_table,
   **         trans_date date) return accounts.balance%type;
   **
   **      function ACCOUNT_UPDATE (account number, 
   **         person person%rowtype, amounts number_table,
   **         trans_no number) return accounts.balance%type;
   **
   **
   **   Values returned -
   **
   **   overload position   argument  level  datatype length prec scale rad
   **   -------------------------------------------------------------------
   **          0        0                0   NUMBER     22    7     2   10
   **          0        1   ACCOUNT      0   NUMBER     22    0     0    0
   **          0        2   PERSON       0   RECORD      0    0     0    0
   **          0        2     PERSON_ID  1   NUMBER     22    4     0   10
   **          0        2     PERSON_NM  1   VARCHAR2   10    0     0    0
   **          0        3   AMOUNTS      0   TABLE       0    0     0    0
   **          0        3                1   NUMBER     22    0     0    0
   **          0        4   TRANS_NO     0   NUMBER     22    0     0    0
   **
   **          1        0                0   NUMBER     22    7     2   10
   **          1        1   ACCOUNT      0   NUMBER     22    0     0    0
   **          1        2   PERSON       0   RECORD      0    0     0    0
   **          1        2    PERSON_ID   1   NUMBER     22    4     0   10
   **          1        2    PERSON_NM   1   VARCHAR2   10    0     0    0
   **          1        3   AMOUNTS      0   TABLE       0    0     0    0
   **          1        3                1   NUMBER     22    0     0    0
   **          1        4   TRANS_DATE   0   NUMBER     22    0     0    0
   **
   **
   **  OCIDPR Argument Descriptions -
   **
   **  ldadef           - pointer to ldadef
   **  object_name      - object name, synonyms are also accepted and will 
   **                     be translate, currently only procedure and function
   **                     names are accepted, also NLS names are accepted.
   **                     Currently, the accepted format of a name is 
   **                     [[part1.]part2.]part3[@dblink] (required)
   **  object_length    - object name length (required)
   **  reserved1        - reserved for future use
   **  reserved1_length - reserved for future use
   **  reserved2        - reserved for future use
   **  reserved2_length - reserved for future use
   **  overload         - array indicating overloaded procedure # (returned)
   **  position         - array of argument positions, position 0 is a 
   **                     function return argument (returned)
   **  level            - array of argument type levels, used to describe
   **                     sub-datatypes of data structures like records
   **                     and arrays (returned)
   **  argument_name    - array of argument names, only returns first 
   **                     30 characters of argument names, note storage
   **                     for 30 characters is allocated by client (returned)
   **  argument_length  - array of argument name lengths (returned)
   **  datatype         - array of oracle datatypes (returned)
   **  default_supplied - array indicating parameter has default (returned)
   **                     0 = no default, 1 = default supplied
   **  in_out           - array indicating if argument is IN or OUT (returned
   **                     0 = IN param, 1 = OUT param, 2 = IN/OUT param
   **  length           - array of argument lengths (returned)
   **  precision        - array of precisions (if number type)(returned)
   **  scale            - array of scales (if number type)(returned)
   **  radix            - array of radix (if number type)(returned)
   **  spare            - array of spares.
   **  total_elements   - size of arrays supplied by client (required),
   **                     total number of elements filled (returned)
   */

/*************************************/
/* DEFINING                          */
/*************************************/
sword ocidfi(  struct csrdef *cursor, sword pos, ub1 *buf, sword bufl,
                  sword ftype, sb2 *rc, sword scale  );
sword ocidfn(  struct csrdef *cursor, sword pos, ub1 *buf, sword bufl,
                 sword ftype, sword scale, sb2 *indp, oratext *fmt, sword fmtl,
                 sword fmtt, ub2 *rl, ub2 *rc  );
sword ocidfnps(  struct csrdef *cursor, ub1 opcode, sword pos, ub1 *buf, 
                    sb4 bufl, sword ftype, sword scale,  
                    sb2 *indp, oratext *fmt, sb4 fmtl,
                    sword fmtt, ub2 *rl, ub2 *rc,
                    sb4 pv_skip, sb4 ind_skip, sb4 len_skip, 
                    sb4 rc_skip  );


   /*  Define a user data buffer using upidfn
   **   cursor  - pointer to csrdef
   **   pos     - position of a field or exp in the select list of a query
   **   bfa/bfl - address and length of client-supplied storage
                  to receive data
   **   ftype   - user datatype
   **   scale   - number of fractional digits for cobol packed decimals
   **   indp    - place to store the length of the returned value. If returned
   **             value is:
   **             negative, the field fetched was NULL
   **             zero    , the field fetched was same length or shorter than
   **               the buffer provided
   **             positive, the field fetched was truncated
   **   fmt    - format string
   **   fmtl   - length of format string, if -1 strlent(fmt) used
   **   rl     - place to store column length after each fetch
   **   rc     - place to store column error code after each fetch
   **   fmtt   - fomat type
   */
 
/********************************/
/*    PIECE INFORMATION GET/SET */
/********************************/
sword           ocigetpi(  struct csrdef *cursor, ub1 *piecep,
                              void  **ctxpp, ub4 *iterp, ub4 *indexp  );
sword           ocisetpi(  struct csrdef *cursor, ub1 piece,
                              void  *bufp, ub4 *lenp  );


/********************************/
/*   EXECUTE                    */
/********************************/
sword ociexe(  struct csrdef *cursor  );
sword ociexn(  struct csrdef *cursor, sword iters, sword roff  );
sword ociefn(  struct csrdef *cursor, ub4 nrows, sword can, sword exact );
    /* 
    ** ociexe  - execute a cursor
    ** ociexn  - execute a cursosr N times
    **  cursor   - pointer to a csrdef
    **  iters    - number of times to execute cursor
    **  roff     - offset within the bind variable array at which to begin 
    **             operations.
    */


/*********************************/
/*     FETCHING                  */
/*********************************/
sword ocifet(  struct csrdef *cursor  );
sword ocifen(  struct csrdef *cursor, sword nrows  );
    /* ocifet - fetch the next row
    ** ocifen - fetch n rows 
    ** cursor   - pointer to csrdef
    ** nrows    - number of rows to be fetched
    */

sword ocilng( struct csrdef *cursor, sword posit, ub1 *bfa, sb4 bfl,
                 sword dty, ub4 *rln, sb4 off );

/*********************************/
/*         CONVERSION            */
/*********************************/
sword ocic32(  struct csrdef *cursor  );
    /*
    **   Convert selected version 3 return codes to the equivalent
    **   version 2 code.
    **   csrdef->csrrc is set to the converted code
    **   csrdef->csrft is set to v2 oracle statment type
    **   csrdef->csrrpc is set to the rows processed count
    **   csrdef->csrpeo is set to error postion
    ** 
    **     cursor - pointer to csrdef
    */


sword ocir32(  struct csrdef *cursor, sword retcode  );
   /*   
   ** Convert selected version 3 return codes to the equivalent version 2 
   ** code.
   **
   **    cursor - pointer to csrdef
   **    retcode - place to store the return code
   */


void ociscn(  sword **arglst, char *mask_addr, sword **newlst  );
   /*
   ** Convert call-by-ref to call-by-value:
   ** takes an arg list and a mask address, determines which args need 
   ** conversion to a value, and creates a new list begging at the address
   ** of newlst.
   **
   **    arglst    - list of arguments
   **    mast_addr _ mask address determines args needing conversion
   **    newlst    - new list of args
   */

sword    ocistf (  sword typ, sword bufl, sword rdig, oratext *fmt, 
                      struct csrdef *cursor, sword *err  );
/*  Convert a packed  decimal buffer  length  (bytes) and scale to a format
**  string of the form mm.+/-nn, where  mm is the number of packed 
**  decimal digits, and nn is the scaling factor.   A positive scale name 
**  nn digits to the rights of the decimal; a negative scale means nn zeros 
**  should be supplied to the left of the decimal.
**     bufl   - length of the packed decimal buffer
**     rdig   - number of fractional digits
**     fmt    - pointer to a string holding the conversion format
**     cursor - pointer to csrdef 
**     err    - pointer to word storing error code
*/ 


/******************************************/
/*         Non-blocking operations        */
/******************************************/
sword ocinbs(  ldadef *lda  );  /* set a connection to non-blocking   */
sword ocinbt(  ldadef *lda  );  /* test if connection is non-blocking */
sword ocinbc(  ldadef *lda  );  /* clear a connection to blocking     */
sword ocinlo(  ldadef *lda, struct hstdef *hst, oratext *conn,
                  sword connl, oratext *uid, sword uidl,
                  oratext *psw, sword pswl, sword audit  );  
              /* logon in non-blocking fashion */
/* ocinlo allows an application to logon in non-blocking fashion.
**   lda     - pointer to ldadef
**   hst     - pointer to a 256 byte area, must be cleared to zero before call
**   conn    - the database link (if specified @LINK in uid will be ignored)
**   connl   - length of conn; if -1 strlen(conn) is used   
**   uid     - user id [USER[/PASSWORD][@LINK]]
**   uidl    - length of uid, if -1 strlen(uid) is used
**   psw     - password string; ignored if specified in uid
**   pswl    - length of psw, if -1 strlen(psw) is used
**   audit   - is not supported; the only permissible value is 0
*/

/***************************************************/
/*         Procedure Declaration for Pro*C         */
/***************************************************/
/* Note: The following routines are used in Pro*C and have the
   same interface as their couterpart in OCI. 
   Althought the interface follows for more details please refer 
   to the above routines */

/******************************************/
/*         initialization/logon/logof     */
/******************************************/
sword ocipin(  ub4 mode  );

sword ologin(  ldadef *lda, sb2 areacount  );
sword ologon(  ldadef *lda, sb2 areacount  );

/*****************************************/
/*        Open/Close/Parse Cursor        */
/*****************************************/

/* 
** ocisqd - oci delayed parse (Should be used only with deferred upi/oci)
** FUNCTION: Call upidpr to delay the parse of the sql statement till the
**           time that a call needs to be made to the kernel (execution or
**           describe time )
** RETURNS: Oracle return code.
*/ 
sword ocisq7( struct csrdef *cursor, oratext * /* sqlstm */, sb4 sqllen,
                 sword defflg, ub4 sqlt );

/*****************************************/
/*            Bind                       */
/*****************************************/
sword obind(  struct csrdef *cursor, oratext *sqlvar, sword sqlvl, 
                 ub1 *progv, sword progvl, sword ftype, sword scale, 
                 oratext *fmt, sword fmtl, sword fmtt  );
sword obindn(  struct csrdef *cursor, ub2 sqlvn, ub1 *progv, sword progvl,
                  sword ftype, sword scale, oratext *fmt, sword fmtl, 
                  sword fmtt  );

/**********************************************/
/*                 Define                     */
/**********************************************/
sword odfinn(  struct csrdef *cursor, sword pos, ub1 *buf, sword bufl,
                  sword ftype, sb2 *rc, sword scale  );

/**********************************************/
/*                 Describe                   */
/**********************************************/
sword odsrbn(  struct csrdef *cursor, sword pos, sb2 *dbsize, sb2 *dtype, 
            sb2 *fsize  );


/******************************************/
/*         Non-blocking operations        */
/******************************************/
sword onblon(  ldadef *lda, struct hstdef *hst, oratext *conn,
                  sword connl, oratext *uid, sword uidl,
                  oratext *psw, sword pswl, sword audit  );  
              /* logon in non-blocking fashion */
sword ocignfd(  ldadef *lda, void  *nfdp );           /* get native fd */

ub2   ocigft_getFcnType(  ub2 oertyp  );      /* get sql function code */

#endif
