/* Copyright (c) 1992, 2006, Oracle. All rights reserved.  */

/*
   NAME
     xa.h - <one-line expansion of the name>
   DESCRIPTION
     <short description of component this file declares/defines>
   PUBLIC FUNCTION(S)
     <list of external functions declared/defined - with one-line descriptions>
   PRIVATE FUNCTION(S)
     <list of static functions defined in .c file - with one-line descriptions>
   RETURNS
     <function return values, for .c file with single function>
   NOTES
     <other useful comments, qualifications, etc.>
     
     This is the public XA .h file

   MODIFIED   (MM/DD/YY)
    yohu       08/27/06  - XA/RAC project changes: XAER_AFFINITY
    dmukhin    06/29/05  - ANSI prototypes; miscellaneous cleanup 
    whe        09/01/99 -  976457:check __cplusplus for C++ code
    ntang      10/20/98 -  Remove TMCACHE & TMFORCL
    abhide     08/04/97 -  implement xaoforcl
    abhide     07/23/97 -  XA OTS project changes
    schandra   02/20/96 -  lint
    abhide     04/07/94 -  merge changes from branch 1.1.710.1
    abhide     02/14/94 -  Creation
    abhide     02/10/94 -  Creation
    abhide     02/10/94 -  Creation
*/
/*
 * xa.h header
 *      Typed in from X/Open doc of March 13, 1990
 *      Updated to Parsippany II draft, March, 1991
 *      Updated to Co Review draft, 19 Sep 1991
 */

#ifndef XA_H
#define XA_H


/* 
 * Transaction branch identification: XID and NULLXID:
 */


#define XIDDATASIZE 128         /* size in bytes */
#define MAXGTRIDSIZE    64      /* maximum size in bytes of gtrid */
#define MAXBQUALSIZE    64      /* maximum size in bytes of bqual */
struct xid_t {
    long        formatID;       /* format identifier */
    long        gtrid_length;   /* value from 1 through 64 */
    long        bqual_length;   /* value from 1 through 64 */
    char        data[XIDDATASIZE];
};
typedef struct xid_t XID;

/*
 * A value of -1 in formatID means that the XID is null.
 */
/*
 * Declarations of routines by which RMs call TMs:
 */

int ax_reg(int, XID *, long);
int ax_unreg(int, long);
/*
 * XA Switch Data Structure
 */
#define RMNAMESZ 32             /* length of resource manager name, */
                                /* including the null terminator */
#define MAXINFOSIZE 256         /* maximum size in bytes of xa_info strings, */
                                /* including the null terminator */
struct xa_switch_t {
    char name[RMNAMESZ];        /* name of resource manager */
    long flags;                 /* resource manager specific options */
    long version;               /* must be 0 */

    int  (*xa_open_entry)(char *, int, long);   /*xa_open function pointer*/
    int  (*xa_close_entry)(char *, int, long);  /*xa_close function pointer*/
    int  (*xa_start_entry)(XID *, int, long);   /*xa_start function pointer*/
    int  (*xa_end_entry)(XID *, int, long);     /*xa_end function pointer*/
    int  (*xa_rollback_entry)(XID *, int, long);
                                               /*xa_rollback function pointer*/
    int  (*xa_prepare_entry)(XID *, int, long); /*xa_prepare function pointer*/
    int  (*xa_commit_entry)(XID *, int, long);  /*xa_commit function pointer*/
    int  (*xa_recover_entry)(XID *, long, int, long);
                                                /*xa_recover function pointer*/
    int  (*xa_forget_entry)(XID *, int, long);  /*xa_forget function pointer*/
    int  (*xa_complete_entry)(int *, int *, int, long);
};

/*
 * Flag definition for the RM switch
 */
#define TMNOFLAGS       0x00000000L     /* no resource manager features
                                           selected */  
#define TMREGISTER      0x00000001L     /* resource manager dynamically
                                           registers */
#define TMNOMIGRATE     0x00000002L     /* resource manager does not support
                                           association migration */
#define TMUSEASYNC      0x00000004L     /* resource manager supports
                                           asynchronous operations */
/*
 * Flag definitions for xa_ and ax_ routines
 */
/* Use TMNOFLAGS, defined above, when not specifying other flags */
#define TMASYNC         0x80000000L     /* perform routine asynchronously */
#define TMONEPHASE      0x40000000L     /* caller is using one-phase commit
                                        optimisation */
#define TMFAIL          0x20000000L     /* dissociates caller and marks
                                           transaction branch rollback-only */
#define TMNOWAIT        0x10000000L     /* return if blocking condition
                                           exists */
#define TMRESUME        0x08000000L     /* caller is resuming association
                                           with suspended transaction branch */
#define TMSUCCESS       0x04000000L     /* dissociate caller from transaction
                                        branch */
#define TMSUSPEND       0x02000000L     /* caller is suspending, not ending,
                                           association */
#define TMSTARTRSCAN    0x01000000L     /* start a recovery scan */
#define TMENDRSCAN      0x00800000L     /* end a recovery scan */
#define TMMULTIPLE      0x00400000L     /* wait for any asynchronous
                                           operation */
#define TMJOIN          0x00200000L     /* caller is joining existing
                                        transaction branch */
#define TMMIGRATE       0x00100000L     /* caller intends to perform
                                        migration */

/* 
 * ax_() return codes (transaction manager reports to resource manager)
 */
#define TM_JOIN         2       /* caller is joining existing transaction
                                branch */
#define TM_RESUME       1       /* caller is resuming association with
                                   suspended transaction branch */
#define TM_OK   0               /* normal execution */
#define TMER_TMERR      -1      /* an error occurred in the transaction
                                manager */
#define TMER_INVAL      -2      /* invalid arguments were given */
#define TMER_PROTO      -3      /* routine invoked in an improper context */

/* 
 * xa_() return codes (resource manager reports to transaction manager)
 */
#define XA_RBBASE       100             /* The inclusive lower bound of the
                                           rollback codes */
#define XA_RBROLLBACK   XA_RBBASE       /* The rollback was caused by an
                                           unspecified reason */
#define XA_RBCOMMFAIL   XA_RBBASE+1     /* The rollback was caused by a
                                           communication failure */
#define XA_RBDEADLOCK   XA_RBBASE+2     /* A deadlock was detected */
#define XA_RBINTEGRITY  XA_RBBASE+3     /* A condition that violates the
                                           integrity of the resources was
                                           detected */
#define XA_RBOTHER      XA_RBBASE+4     /* The resource manager rolled back the
                                           transaction for a reason not on this
                                           list */
#define XA_RBPROTO      XA_RBBASE+5     /* A protocal error occurred in the
                                           resource manager */
#define XA_RBTIMEOUT    XA_RBBASE+6     /* A transaction branch took too long*/
#define XA_RBTRANSIENT  XA_RBBASE+7     /* May retry the transaction branch */
#define XA_RBEND        XA_RBTRANSIENT  /* The inclusive upper bound of the
                                           rollback codes */

#define XA_NOMIGRATE    9               /* resumption must occur where
                                           suspension occurred */
#define XA_HEURHAZ      8               /* the transaction branch may have been
                                           heuristically completed */
#define XA_HEURCOM      7               /* the transaction branch has been
                                           heuristically comitted */
#define XA_HEURRB       6               /* the transaction branch has been
                                           heuristically rolled back */
#define XA_HEURMIX      5               /* the transaction branch has been
                                           heuristically committed and rolled
                                           back */
#define XA_RETRY        4               /* routine returned with no effect
                                           and may be re-issued */
#define XA_RDONLY       3               /* the transaction was read-only
                                           and has been committed */
#define XA_OK           0               /* normal execution */
#define XAER_ASYNC      -2              /* asynchronous operation already
                                           outstanding */
#define XAER_RMERR      -3              /* a resource manager error occurred
                                        in the transaction branch */
#define XAER_NOTA       -4              /* the XID is not valid */
#define XAER_INVAL      -5              /* invalid arguments were given */
#define XAER_PROTO      -6              /* routine invoked in an improper
                                           context */
#define XAER_RMFAIL     -7              /* resource manager unavailable */
#define XAER_DUPID      -8              /* the XID already exists */
#define XAER_OUTSIDE    -9              /* resource manager doing work */
                                        /* outside global transaction */

#define XAER_AFFINITY   -10             /* XA on RAC: resumption must occur on
                                           RAC instance where the transaction
                                           branch was created */
                                          
#endif /* ifndef XA_H */
