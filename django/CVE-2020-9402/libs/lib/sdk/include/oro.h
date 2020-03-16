/* Copyright (c) 1994, 2014, Oracle and/or its affiliates. 
All rights reserved.*/

/*
  NAME
    OCI - Oracle Object Interface for External/Internal/Kernel Clients

  DESCRIPTION
    This header file contains Oracle object interface definitions which
    can be included by external user applications, tools, as well as
    the kernel.  It defines types and constants that are common to all
    object interface which is being defined in several other header files
    (e.g., ori.h, ort.h, and orl.h).

  RELATED DOCUMENTS
    TBD

  INSPECTION STATUS [[ deletable if no inspection ]]
    Inspection date:    [[ date of the last logging meeting ]]
    Inspection status:  [[ exited, not exited, or N/A if exit is not a goal ]]
    Estimated increasing cost defects per page:
    Rule sets:          [[ rule sets inspected against or planned to be 
                           inspected against ]]

  ACCEPTANCE REVIEW STATUS [[ deletable if no approval review ]]
    Review date:    [[ date of the meeting where issues were logged and the 
                       approval status was decided ]] 
    Review status:  [[ current status:  accepted, conditionally accepted, 
                       major revision required, rejected ]]
    Reviewers:      [[ names of the members on the review team ]]

  PUBLIC FUNCTIONS

  EXAMPLES
    Examples are given in the description of each function or macro where
    relevant.

  MODIFIED
    dpotapov   04/24/14 - Fix oci ind string raw
    dpotapov   03/06/14 - xtss merge
    mnanal     06/09/03 - backout of fix 2836388
    mnanal     05/14/03 - bug-2836388
    srseshad   11/27/02 - Change OCI_TYPECODE_BFLOAT/BDOUBLE codes
    rxgovind   10/09/02 - add OCI_TYPECODE_UROWID
    mxyang     09/17/02 - grabtrans 'mmorsi_obj_float'
    srseshad   09/11/02 - 
    srseshad   09/01/02 - 
    aahluwal   06/03/02 - bug 2360115
    celsbern   10/19/01 - merge LOG to MAIN
    rxgovind   10/16/01 - update typecodes
    rxgovind   09/19/01 - add typecodes
    rkasamse   08/15/01 - add OCI_DURATION_USER_CALLBACK
    jchai      09/24/01 - add type code for PLS_INTEGER
    porangas   08/22/01 - Fix bug#1776434
    schatter   04/09/01 - merge 1456235: define OCI_DURATION_INVALID
    rdani      10/12/00 - 1449943 NOCOPY and PIPELINE
    ciyer      05/26/00 - short names for abstract, overriding
    rkasamse   05/25/00 - OCCI enhancements
    smuralid   05/11/00 - OCITypeMethodFlags - add NOT INSTANTIABLE, OVERRIDING
    rxgovind   05/09/00 - add OCI_TYPECODE_NONE
    tnbui      07/28/99 - Remove OCI_TYPECODE_TIMESTAMP_ITZ                    
    tnbui      07/21/99 - TS LOCAL TZ                                          
    thoang     06/21/99 - Add OCI_TYPECODE_TIMESTAMP_ITZ
    thoang     03/04/99 - Add datetime datatypes
    rkasamse   10/20/98 - add OCI_ATTR_CACHE_ARRAYFLUSH
    rkasamse   10/29/98 - add OCI_DURATION_CALLOUT
    rkasamse   04/28/98 - OCI_OBJECT_DETECTCHANGE -> OCI_ATTR_OBJECT_DETECTCHAN
    rkasamse   04/28/98 - OCI_OBJECT_NEWNOTNULL -> OCI_ATTR_OBJECT_NEWNOTNULL  
    rkasamse   04/23/98 - add OCI_OBJECT_DETECTCHANGE
    rkasamse   04/03/98 - add OCI_OBJECT_NEWNOTNULL                            
    pmitra     04/01/98 - OCI_LOCK_X_NOWAIT added                              
    rxgovind   02/18/98 - add OCI_TYPECODE_OPAQUE
    rkasamse   02/13/98 - Add OCI_DURATION_PROCESS
    cxcheng    07/28/97 - fix compile with SLSHORTNAME
    skrishna   07/14/97 - add OCIObjectGetProperty
    cxcheng    04/30/97 - make OCITypeParamMode values consistent with PL/SQL
    skrishna   04/28/97 - undocument OCIObjectProperty & OCIObjectEvent
    cxcheng    03/29/97 - remove all remaining short names
    sthakur    03/20/97 - add casts to constants
    cxcheng    02/21/97 - temporarily put SLSHORTNAME for PL/SQL
    cxcheng    02/06/97 - take out short name support except with SLSHORTNAME
    lchidamb   01/20/97 - update OCIRef comments
    sgollapu   11/19/96 - Add OCI type codes for BOOL,REC,and TAB
    cxcheng    11/19/96 - more typecode changes
    cxcheng    11/13/96 - add #include for ocidfn.h
    cxcheng    11/13/96 - add OCI_TYPECODE_ADT for compatibility
    cxcheng    11/12/96 - add SQLT_NCO for named collection
    cxcheng    11/11/96 - more changes to typecodes
    cxcheng    11/07/96 - #define OCI_TYPECODE_MLSLABEL to SQLT_LAB
    cxcheng    11/06/96 - fix #define omission for OROTCNAT
    cxcheng    10/30/96 - move OCI_TYPECODE_* to ocidfn.h as SQLT_*
    cxcheng    10/28/96 - more beautification changes
    jboonleu   10/29/96 - add flags for freeing object
    dchatter   10/26/96 - delete redef of OCISvcCtx, OCIError, OCIEnv
    cxcheng    10/15/96 - more changes
    cxcheng    10/14/96 - more final fixes to constants
    mluong     10/11/96 -
    mluong     10/11/96 -  KOCON and KONSP are in lowercase
    mluong     10/11/96 -  add some define per Calvin
    cxcheng    10/09/96 -  add #define for OROOCOSFN to OCI_COPY_NOREF
    jboonleu   10/08/96 -  change OROOCOSFN to OCICopyFlag
    jboonleu   10/07/96 -  use new OCI names for cache options
    cxcheng    10/07/96 -  add OROTCS02 for KOTTCBRI and OROTCS03 as spare
    cxcheng    10/07/96 -  more lint fixes
    cxcheng    10/02/96 -  move oronsp to ko.h as konsp
    cxcheng    10/01/96 -  add long names for readability
    cxcheng    10/01/96 -  remove orotty and orotal
    rjenkins   09/28/96 -  2k char 4k varchar2
    jboonleu   09/27/96 -  add macro used only in beta2
    cxcheng    09/27/96 -  move oroenv to oroenv.h
    cxcheng    09/24/96 -  remove unnecessary orotyp
    cxcheng    09/25/96 -  add typecode OROTCS01 as placeholder for lob pointer
    cxcheng    09/20/96 -  add TDO load option orotgo
    jboonleu   09/18/96 -  add OROOPOREC
    jboonleu   09/10/96 -  add OROOPODFL
    jweisz     08/27/96 -  add SQL internal typecode OROTCS00
    cxcheng    08/02/96 -  add PLSQL internal typecodes OROTCP..
    cxcheng    08/01/96 -  add OROTCFAR to fill up space left by OROTCCAR
    jboonleu   07/16/96 -  new pin option
    cxcheng    06/18/96 -  add casts to OROTNOPRE and OROTNOSCL
    cxcheng    05/29/96 -  change OROTCNPT back to OROTCDOM
    vkrishna   05/27/96 -  add OROTCCAR
    cxcheng    05/17/96 -  replace OROTCFAR with OROTCCAR
    cxcheng    05/08/96 -  change orotmf from ub1 to ub2
    cxcheng    05/07/96 -  fix public defines for method types
    cxcheng    04/30/96 -  change OROTCDOM to OROTCNPT
    cxcheng    04/15/96 -  remove obsolete OROTTYICT
    jboonleu   04/12/96 -  add new pin option
    sthakur    04/12/96 -  add indicator type and indicator status
    cxcheng    04/10/96 -  add function parameter codes for ORT/KOT
    cxcheng    04/03/96 -  replace OROTCFAR as OROTCCAR
    jwijaya    03/29/96 -  add OROTTCCAR
    jwijaya    03/27/96 -  better comments for orotc
    cxcheng    02/23/96 -  add typecodes for SMALLINT and VARCHAR2
    skrishna   02/22/96 -  add oroind - null indicator type
    cxcheng    02/21/96 -  change lob character codes to OROTCCLB, OROTCBLB...
    jboonleu   02/06/96 -  new value for predefined duration
    cxcheng    01/12/96 -  add OROTCCLO, OROTCBLO, OROTCFIL to orotc
    cxcheng    12/05/95 -  add OROTCDOM and OROTCAAT to orotc
    skotsovo   10/30/95 -  reserve space for internal 'oid' type
    jwijaya    10/20/95 -  support variable-length ref
    cxcheng    10/03/95 -  add OROTMFOR for ordering function to orotmf
    cxcheng    10/03/95 -  Adding the ordering function type to orotmf
    jboonleu   09/28/95 -  set OROODTPRE 
    jboonleu   09/25/95 -  add oroodt
    skotsovo   03/10/95 -  update to only include release 1 
    jboonleu   02/15/95 -  add OROOPOREC, remove orocro, oroolo
    skotsovo   01/30/95 -  add default max lengths for varrays and vstrings 
    skotsovo   01/24/95 -  categorize sint32, double, and real as number types 
                           (with precision and scale) instead of scalar types.
    skotsovo   12/20/94 -  add release 1 types 
    skotsovo   12/12/94 -  update according to new ots doc
    skotsovo   12/01/94 -  add default precision and scale 
    jwijaya    11/15/94 -  rename ORONSPTAB to ORONSPEXT 
    jwijaya    10/25/94 -  tint 
    jwijaya    10/06/94 -  add namespace 
    jwijaya    10/02/94 -  connection handle -> connection number 
    skotsovo   09/12/94 -  keep 0 as uninitialized value for ORT consts 
    skotsovo   08/24/94 -  fix orotec 
    skotsovo   08/17/94 -  modify type code names 
    skotsovo   08/12/94 -  fix 141 lint errors 
    skotsovo   07/25/94 -  modify categorization of complex types (orotc) 
    skotsovo   07/07/94 -  change typecode enum values & add decimal type
    skotsovo   07/01/94 -  change order of typecodes 
    jwijaya    06/15/94 -  review 
    jboonleu   06/13/94 -  add comments for the object cache options
    jwijaya    06/13/94 -  adhere to the header file template 
    skotsovo   06/09/94 -  make ots scalar type names consistent with the ots 
                           document 
    jwijaya    06/07/94 -  include oratypes.h instead of s.h 
    skotsovo   05/24/94 -  change typecodes 
    jwijaya    05/23/94 -  fix comments of ororef 
    skotsovo   05/19/94 -  remove type composition 
    skotsovo   05/09/94 -  modified orotc according to new OTS document
    jwijaya    05/03/94 -  oroid and ororef
    jwijaya    01/26/94 -  Creation
*/


#ifndef ORATYPES
#include <oratypes.h>
#endif

#ifndef OCIDFN_ORACLE
#include <ocidfn.h>
#endif

#ifndef ORO_ORACLE
#define ORO_ORACLE

#ifdef K3_ORACLE
#ifndef KOL3_ORACLE
#  include <kol3.h>
#endif
#endif /* K3_ORACLE */

/*---------------------------------------------------------------------------*/
/*                         SHORT NAMES SUPPORT SECTION                       */
/*---------------------------------------------------------------------------*/

#ifdef SLSHORTNAME

/* the following are short names that are only supported on IBM mainframes
   with the SLSHORTNAME defined.
   With this all subsequent long names will actually be substituted with
   the short names here */

#define OCIDuration                     oroodt
#define OCIInd                          oroind
#define OCILockOpt                      oroolm
#define OCIMarkOpt                      oroomo
#define OCIObjectEvent                  orocev
#define OCIObjectProperty               oroopr
#define OCIPinOpt                       oroopo
#define OCIRef                          ororef
#define OCIRefreshOpt                   orooro
#define OCITypeCode                     orotc
#define OCITypeEncap                    orotec
#define OCITypeGetOpt                   orotgo
#define OCITypeMethodFlag               orotmf
#define OCITypeParamMode                orotpm
#define OCIObjectPropId                 oroopi
#define OCIObjectLifetime               oroolft
#define OCIObjectMarkstatus             oroomst
#define OCI_LOCK_NONE                   OROOLMNUL
#define OCI_LOCK_X                      OROOLMX
#define OCI_LOCK_X_NOWAIT               OROOLMXNW
#define OCI_MARK_DEFAULT                OROOMODFL
#define OCI_MARK_NONE                   OROOMONON
#define OCI_MARK_UPDATE                 OROOMOUPD
#define OCI_OBJECTEVENT_AFTER_FLUSH     OROCEVAFL
#define OCI_OBJECTEVENT_AFTER_REFRESH   OROCEVARF
#define OCI_OBJECTEVENT_BEFORE_FLUSH    OROCEVBFL
#define OCI_OBJECTEVENT_BEFORE_REFRESH  OROCEVBRF
#define OCI_OBJECTEVENT_WHEN_LOCK       OROCEVWLK
#define OCI_OBJECTEVENT_WHEN_MARK_DELETED OROCEVWDL
#define OCI_OBJECTEVENT_WHEN_MARK_UPDATED OROCEVWUP
#define OCI_OBJECTEVENT_WHEN_UNMARK     OROCEVWUM
#define OCI_OBJECTPROP_DIRTIED          OROOPRDRT
#define OCI_OBJECTPROP_LOADED           OROOPRLOD
#define OCI_OBJECTPROP_LOCKED           OROOPRLCK
#define OCI_PIN_ANY                     OROOPOANY
#define OCI_PIN_DEFAULT                 OROOPODFL
#define OCI_PIN_LATEST                  OROOPOLST
#define OCI_PIN_RECENT                  OROOPOREC
#define OCI_REFRESH_LOADED              OROOROLOD
#define OCI_TYPEENCAP_PRIVATE           OROTECPVT
#define OCI_TYPEENCAP_PUBLIC            OROTECPUB
#define OCI_TYPEGET_ALL                 OROTGOALL
#define OCI_TYPEGET_HEADER              OROTGOHDR
#define OCI_TYPEMETHOD_CONSTANT         OROTMCON
#define OCI_TYPEMETHOD_CONSTRUCTOR      OROTMCSTR
#define OCI_TYPEMETHOD_DESTRUCTOR       OROTMDSTR
#define OCI_TYPEMETHOD_INLINE           OROTMINL
#define OCI_TYPEMETHOD_MAP              OROTMMAP
#define OCI_TYPEMETHOD_OPERATOR         OROTMOP
#define OCI_TYPEMETHOD_ORDER            OROTMOR
#define OCI_TYPEMETHOD_RNDS             OROTMRDS
#define OCI_TYPEMETHOD_RNPS             OROTMRPS
#define OCI_TYPEMETHOD_SELFISH          OROTMSLF
#define OCI_TYPEMETHOD_VIRTUAL          OROTMVRT
#define OCI_TYPEMETHOD_WNDS             OROTMWDS
#define OCI_TYPEMETHOD_WNPS             OROTMWPS
#define OCI_TYPEMETHOD_ABSTRACT         OROTMABSTRACT
#define OCI_TYPEMETHOD_OVERRIDING       OROTMOVERRIDING
#define OCI_TYPEMETHOD_PIPELINED        OROTMPIPELINED
#define OCI_TYPEPARAM_BYREF             OROTPMREF
#define OCI_TYPEPARAM_IN                OROTPMIN
#define OCI_TYPEPARAM_INOUT             OROTPMIO
#define OCI_TYPEPARAM_OUT               OROTPMOUT
#define OCI_TYPEPARAM_OUTNCPY           OROTPMOUTNCPY
#define OCI_TYPEPARAM_INOUTNCPY         OROTPMIONCPY

#endif                                                        /* SLSHORTNAME */


/*---------------------------------------------------------------------------*/
/*                    PUBLIC TYPES, CONSTANTS AND MACROS                     */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                  GENERAL OBJECT TYPES, CONSTANTS, MACROS                  */
/*---------------------------------------------------------------------------*/

/*------------------------- OBJECT REFERENCE (REF) --------------------------*/

typedef struct OCIRef OCIRef;
/*
 * OCIRef - OCI object REFerence
 *
 * In the Oracle object runtime environment, an object is identified by an 
 * object reference (ref) which contains the object identifier plus other 
 * runtime information.  The contents of a ref is opaque to clients.  Use
 * OCIObjectNew() to construct a ref.
 */


/*--------------------------- OBJECT INDICATOR ------------------------------*/

#ifndef KUTYXTT3_ORACLE
typedef sb2 OCIInd;
#define ORO_OCIIND_DEFINED
#endif
/*
 * OCIInd -- a variable of this type contains (null) indicator information
 */

#define OCI_IND_NOTNULL (OCIInd)0                                /* not NULL */
#define OCI_IND_NULL (OCIInd)(-1)                                    /* NULL */
#define OCI_IND_BADNULL (OCIInd)(-2)                             /* BAD NULL */
#define OCI_IND_NOTNULLABLE (OCIInd)(-3)                     /* not NULLable */

/*---------------------------------------------------------------------------*/
/*                               OBJECT CACHE                                */
/*---------------------------------------------------------------------------*/

/* To enable object change detection mode, set this to TRUE */
#define OCI_ATTR_OBJECT_DETECTCHANGE            0x00000020

/* To enable object creation with  non-NULL attributes by default, set the
   following to TRUE.
   By default, object is created with NULL attributes
*/
#define OCI_ATTR_OBJECT_NEWNOTNULL   0x00000010

/* To enable sorting of the objects that belong to the same table
   before being flushed through OCICacheFlush.
   Please note that by enabling this object cache will not be flushing
   the objects in the same order they were dirtied */
#define OCI_ATTR_CACHE_ARRAYFLUSH 0x00000040

/*--------------------------- OBJECT PIN OPTION -----------------------------*/

enum OCIPinOpt
{
  /* 0 = uninitialized */
  OCI_PIN_DEFAULT = 1,                                 /* default pin option */
  OCI_PIN_ANY = 3,                          /* pin any copy of the object */
  OCI_PIN_RECENT = 4,                    /* pin recent copy of the object */
  OCI_PIN_LATEST = 5                     /* pin latest copy of the object */
};
typedef enum OCIPinOpt OCIPinOpt;

/*
 * OCIPinOpt - OCI object Pin Option 
 *
 * In the Oracle object runtime environment, the program has the option to
 * specify which copy of the object to pin.  
 *
 * OCI_PINOPT_DEFAULT pins an object using the default pin option.  The default
 * pin option can be set as an attribute of the OCI environment handle 
 * (OCI_ATTR_PINTOPTION).  The value of the default pin option can be
 * OCI_PINOPT_ANY, OCI_PINOPT_RECENT, or OCI_PIN_LATEST. The default option
 * is initialized to OCI_PINOPT_ANY.
 *
 * OCI_PIN_ANY pins any copy of the object.  The object is pinned 
 * using the following criteria:
 *   If the object copy is not loaded, load it from the persistent store.
 *   Otherwise, the loaded object copy is returned to the program. 
 * 
 * OCI_PIN_RECENT pins the latest copy of an object.  The object is 
 * pinned using the following criteria:
 *   If the object is not loaded, load the object from the persistent store
 *       from the latest version.
 *   If the object is not loaded in the current transaction and it is not 
 *       dirtied, the object is refreshed from the latest version.
 *   Otherwise, the loaded object copy is returned to the program. 
 *
 * OCI_PINOPT_LATEST pins the latest copy of an object.  The object copy is 
 * pinned using the following criteria:
 *   If the object copy is not loaded, load it from the persistent store.
 *   If the object copy is loaded and dirtied, it is returned to the program.
 *   Otherwise, the loaded object copy is refreshed from the persistent store.
 */



/*--------------------------- OBJECT LOCK OPTION ----------------------------*/

enum OCILockOpt
{
  /* 0 = uninitialized */
  OCI_LOCK_NONE = 1,                               /* null (same as no lock) */
  OCI_LOCK_X = 2,                                          /* exclusive lock */
  OCI_LOCK_X_NOWAIT = 3                      /* exclusive lock, do not wait  */
};
typedef enum OCILockOpt OCILockOpt;
/*
 * OCILockOpt - OCI object LOCK Option 
 *
 * This option is used to specify the locking preferences when an object is
 * loaded from the server.
 */


/*------------------------- OBJECT MODIFYING OPTION -------------------------*/

enum OCIMarkOpt
{
  /* 0 = uninitialized */
  OCI_MARK_DEFAULT = 1,        /* default (the same as OCI_MARK_NONE) */
  OCI_MARK_NONE = OCI_MARK_DEFAULT,   /* object has not been modified */
  OCI_MARK_UPDATE                               /* object is to be updated */
};
typedef enum OCIMarkOpt OCIMarkOpt;
/*
 * OCIMarkOpt - OCI object Mark option
 *
 * When the object is marked updated, the client has to specify how the
 * object is intended to be changed.  
 */

/*-------------------------- OBJECT Duration --------------------------------*/
 
typedef ub2 OCIDuration;

#define  OCI_DURATION_INVALID 0xFFFF                     /* Invalid duration */
#define  OCI_DURATION_BEGIN (OCIDuration)10
                                           /* beginning sequence of duration */
#define  OCI_DURATION_NULL (OCIDuration)(OCI_DURATION_BEGIN-1)
                                                            /* null duration */
#define  OCI_DURATION_DEFAULT (OCIDuration)(OCI_DURATION_BEGIN-2) /* default */
#define  OCI_DURATION_USER_CALLBACK (OCIDuration)(OCI_DURATION_BEGIN-3)
#define  OCI_DURATION_NEXT (OCIDuration)(OCI_DURATION_BEGIN-4)
                                                    /* next special duration */
#define  OCI_DURATION_SESSION (OCIDuration)(OCI_DURATION_BEGIN)
                                                  /* the end of user session */
#define  OCI_DURATION_TRANS (OCIDuration)(OCI_DURATION_BEGIN+1)
                                              /* the end of user transaction */
/******************************************************************************
**  DO NOT USE OCI_DURATION_CALL. IT  IS UNSUPPORTED                         **
**  WILL BE REMOVED/CHANGED IN A FUTURE RELEASE                              **
******************************************************************************/
#define  OCI_DURATION_CALL (OCIDuration)(OCI_DURATION_BEGIN+2)
                                       /* the end of user client/server call */
#define  OCI_DURATION_STATEMENT (OCIDuration)(OCI_DURATION_BEGIN+3)

/* This is to be used only during callouts.  It is similar to that 
of OCI_DURATION_CALL, but lasts only for the duration of a callout.
Its heap is from PGA */
#define  OCI_DURATION_CALLOUT (OCIDuration)(OCI_DURATION_BEGIN+4)

#define  OCI_DURATION_LAST OCI_DURATION_CALLOUT 
                                             /* last of predefined durations */

/* This is not being treated as other predefined durations such as 
   SESSION, CALL etc, because this would not have an entry in the duration
   table and its functionality is primitive such that only allocate, free,
   resize memory are allowed, but one cannot create subduration out of this
*/ 
#define  OCI_DURATION_PROCESS (OCIDuration)(OCI_DURATION_BEGIN-5) 
 
/*
 * OCIDuration - OCI object duration
 *
 * A client can specify the duration of which an object is pinned (pin
 * duration) and the duration of which the object is in memory (allocation
 * duration).  If the objects are still pinned at the end of the pin duration,
 * the object cache manager will automatically unpin the objects for the
 * client. If the objects still exist at the end of the allocation duration,
 * the object cache manager will automatically free the objects for the client.
 *
 * Objects that are pinned with the option OCI_DURATION_TRANS will get unpinned
 * automatically at the end of the current transaction.
 *
 * Objects that are pinned with the option OCI_DURATION_SESSION will get
 * unpinned automatically at the end of the current session (connection).
 *
 * The option OCI_DURATION_NULL is used when the client does not want to set
 * the pin duration.  If the object is already loaded into the cache, then the
 * pin duration will remain the same.  If the object is not yet loaded, the
 * pin duration of the object will be set to OCI_DURATION_DEFAULT.
 */

/*----------------------------- OBJECT PROPERTY -----------------------------*/
 
/******************************************************************************
**  DO NOT USE OCIObjectProperty. IT IS UNSUPPORTED                          **
**  WILL BE REMOVED/CHANGED IN A FUTURE RELEASE                              **
******************************************************************************/
enum OCIObjectProperty
{
  /* 0 = uninitialized */
  OCI_OBJECTPROP_DIRTIED = 1,                               /* dirty objects */
  OCI_OBJECTPROP_LOADED,                /* objects loaded in the transaction */
  OCI_OBJECTPROP_LOCKED                                    /* locked objects */
};
typedef enum OCIObjectProperty OCIObjectProperty;
/*
 * OCIObjectProperty -- OCI Object Property
 * This specifies the properties of objects in the object cache.
 */

/*------------------------- CACHE REFRESH OPTION ---------------------------*/

enum OCIRefreshOpt
{
  /* 0 = uninitialized */
  OCI_REFRESH_LOADED = 1        /* refresh objects loaded in the transaction */
};
typedef enum OCIRefreshOpt OCIRefreshOpt; 
/*
 * OCIRefreshOpt - OCI cache Refresh Option
 * This option is used to specify the set of objects to be refreshed. 
 *
 * OCI_REFRESH_LOAD refreshes the objects that are loaded in the current  
 * transaction. 
 */

/*-------------------------------- OBJECT EVENT -----------------------------*/

/******************************************************************************
**  DO NOT USE OCIObjectEvent. IT IS UNSUPPORTED                             **
**  WILL BE REMOVED/CHANGED IN A FUTURE RELEASE                              **
******************************************************************************/
enum OCIObjectEvent
{
   /* 0 = uninitialized */
   OCI_OBJECTEVENT_BEFORE_FLUSH = 1,            /* before flushing the cache */
   OCI_OBJECTEVENT_AFTER_FLUSH,                  /* after flushing the cache */
   OCI_OBJECTEVENT_BEFORE_REFRESH,            /* before refreshing the cache */
   OCI_OBJECTEVENT_AFTER_REFRESH,              /* after refreshing the cache */
   OCI_OBJECTEVENT_WHEN_MARK_UPDATED,    /* when an object is marked updated */
   OCI_OBJECTEVENT_WHEN_MARK_DELETED,    /* when an object is marked deleted */
   OCI_OBJECTEVENT_WHEN_UNMARK,          /* when an object is being unmarked */
   OCI_OBJECTEVENT_WHEN_LOCK               /* when an object is being locked */
};
typedef enum OCIObjectEvent OCIObjectEvent;
/*
 * OCIObjectEvent -- OCI Object Event 
 * This specifies the kind of event that is supported by the object
 * cache.  The program can register a callback that is invoked when the  
 * specified event occurs.
 */

/*----------------------------- OBJECT COPY OPTION --------------------------*/
#define OCI_OBJECTCOPY_NOREF (ub1)0x01 
/*
 * OCIObjectCopyFlag - Object copy flag
 *
 * If OCI_OBJECTCOPY_NOREF is specified when copying an instance, the 
 * reference and lob will not be copied to the target instance.
 */

/*----------------------------- OBJECT FREE OPTION --------------------------*/
#define OCI_OBJECTFREE_FORCE      (ub2)0x0001
#define OCI_OBJECTFREE_NONULL     (ub2)0x0002
#define OCI_OBJECTFREE_HEADER     (ub2)0x0004
/*
 * OCIObjectFreeFlag - Object free flag
 *
 * If OCI_OBJECTCOPY_FORCE is specified when freeing an instance, the instance
 * is freed regardless it is pinned or diritied.
 * If OCI_OBJECTCOPY_NONULL is specified when freeing an instance, the null 
 * structure is not freed.
 */

/*----------------------- OBJECT PROPERTY ID -------------------------------*/

typedef ub1 OCIObjectPropId;
#define OCI_OBJECTPROP_LIFETIME 1       /* persistent or transient or value */
#define OCI_OBJECTPROP_SCHEMA 2   /* schema name of table containing object */
#define OCI_OBJECTPROP_TABLE 3     /* table name of table containing object */
#define OCI_OBJECTPROP_PIN_DURATION 4             /* pin duartion of object */
#define OCI_OBJECTPROP_ALLOC_DURATION 5         /* alloc duartion of object */
#define OCI_OBJECTPROP_LOCK 6                      /* lock status of object */
#define OCI_OBJECTPROP_MARKSTATUS 7                /* mark status of object */
#define OCI_OBJECTPROP_VIEW 8            /* is object a view object or not? */

/*
 * OCIObjectPropId - OCI Object Property Id
 * Identifies the different properties of objects.
 */

/*----------------------- OBJECT LIFETIME ----------------------------------*/

enum OCIObjectLifetime
{
   /* 0 = uninitialized */
   OCI_OBJECT_PERSISTENT = 1,                          /* persistent object */
   OCI_OBJECT_TRANSIENT,                                /* transient object */
   OCI_OBJECT_VALUE                                         /* value object */
};
typedef enum OCIObjectLifetime OCIObjectLifetime;
/*
 * OCIObjectLifetime - OCI Object Lifetime
 * Classifies objects depending upon the lifetime and referenceability 
 * of the object.
 */

/*----------------------- OBJECT MARK STATUS -------------------------------*/

typedef uword OCIObjectMarkStatus;
#define OCI_OBJECT_NEW     0x0001                             /* new object */
#define OCI_OBJECT_DELETED 0x0002                  /* object marked deleted */
#define OCI_OBJECT_UPDATED 0x0004                  /* object marked updated */
/*
 * OCIObjectMarkStatus - OCI Object Mark Status
 * Status of the object - new or updated or deleted
 */

/* macros to test the object mark status */ 
#define OCI_OBJECT_IS_UPDATED(flag) bit((flag), OCI_OBJECT_UPDATED)
#define OCI_OBJECT_IS_DELETED(flag) bit((flag), OCI_OBJECT_DELETED)
#define OCI_OBJECT_IS_NEW(flag) bit((flag), OCI_OBJECT_NEW)
#define OCI_OBJECT_IS_DIRTY(flag) \
  bit((flag), OCI_OBJECT_UPDATED|OCI_OBJECT_NEW|OCI_OBJECT_DELETED)

/*---------------------------------------------------------------------------*/
/*                               TYPE MANAGER                                */
/*---------------------------------------------------------------------------*/

/*------------------------------ TYPE CODE ----------------------------------*/

/*
 * Type manager typecodes
 *
 * These are typecodes designed to be used with the type manager;
 * they also include longer, more readable versions of existing SQLT names.
 * Those types that are directly related to existing SQLT types are #define'd
 * to their SQLT equivalents.
 *
 * The type manager typecodes are designed to be useable for all OCI calls.
 * They are in the range from 192 to 320 for typecodes, so as not to conflict
 * with existing OCI SQLT typecodes (see ocidfn.h).
 */

#define OCI_TYPECODE_REF         SQLT_REF        /* SQL/OTS OBJECT REFERENCE */
#define OCI_TYPECODE_DATE        SQLT_DAT              /* SQL DATE  OTS DATE */
#define OCI_TYPECODE_SIGNED8     27      /* SQL SIGNED INTEGER(8)  OTS SINT8 */
#define OCI_TYPECODE_SIGNED16    28    /* SQL SIGNED INTEGER(16)  OTS SINT16 */
#define OCI_TYPECODE_SIGNED32    29    /* SQL SIGNED INTEGER(32)  OTS SINT32 */
#define OCI_TYPECODE_REAL        21                /* SQL REAL  OTS SQL_REAL */
#define OCI_TYPECODE_DOUBLE      22  /* SQL DOUBLE PRECISION  OTS SQL_DOUBLE */
#define OCI_TYPECODE_BFLOAT      SQLT_IBFLOAT       /* Binary float */ 
#define OCI_TYPECODE_BDOUBLE     SQLT_IBDOUBLE  /* Binary double */
#define OCI_TYPECODE_FLOAT       SQLT_FLT      /* SQL FLOAT(P)  OTS FLOAT(P) */
#define OCI_TYPECODE_NUMBER      SQLT_NUM/* SQL NUMBER(P S)  OTS NUMBER(P S) */
#define OCI_TYPECODE_DECIMAL     SQLT_PDN
                                       /* SQL DECIMAL(P S)  OTS DECIMAL(P S) */
#define OCI_TYPECODE_UNSIGNED8   SQLT_BIN
                                       /* SQL UNSIGNED INTEGER(8)  OTS UINT8 */
#define OCI_TYPECODE_UNSIGNED16  25  /* SQL UNSIGNED INTEGER(16)  OTS UINT16 */
#define OCI_TYPECODE_UNSIGNED32  26  /* SQL UNSIGNED INTEGER(32)  OTS UINT32 */
#define OCI_TYPECODE_OCTET       245                   /* SQL ???  OTS OCTET */
#define OCI_TYPECODE_SMALLINT    246           /* SQL SMALLINT  OTS SMALLINT */
#define OCI_TYPECODE_INTEGER     SQLT_INT        /* SQL INTEGER  OTS INTEGER */
#define OCI_TYPECODE_RAW         SQLT_LVB          /* SQL RAW(N)  OTS RAW(N) */
#define OCI_TYPECODE_PTR         32              /* SQL POINTER  OTS POINTER */
#define OCI_TYPECODE_VARCHAR2    SQLT_VCS
                                     /* SQL VARCHAR2(N)  OTS SQL_VARCHAR2(N) */
#define OCI_TYPECODE_CHAR        SQLT_AFC    /* SQL CHAR(N)  OTS SQL_CHAR(N) */
#define OCI_TYPECODE_VARCHAR     SQLT_CHR
                                       /* SQL VARCHAR(N)  OTS SQL_VARCHAR(N) */
#define OCI_TYPECODE_MLSLABEL    SQLT_LAB                    /* OTS MLSLABEL */
#define OCI_TYPECODE_VARRAY      247         /* SQL VARRAY  OTS PAGED VARRAY */
#define OCI_TYPECODE_TABLE       248              /* SQL TABLE  OTS MULTISET */
#define OCI_TYPECODE_OBJECT      SQLT_NTY       /* SQL/OTS NAMED OBJECT TYPE */
#define OCI_TYPECODE_OPAQUE       58                /*  SQL/OTS Opaque Types */
#define OCI_TYPECODE_NAMEDCOLLECTION  SQLT_NCO
                                            /* SQL/OTS NAMED COLLECTION TYPE */
#define OCI_TYPECODE_BLOB        SQLT_BLOB    /* SQL/OTS BINARY LARGE OBJECT */
#define OCI_TYPECODE_BFILE       SQLT_BFILE    /* SQL/OTS BINARY FILE OBJECT */
#define OCI_TYPECODE_CLOB        SQLT_CLOB /* SQL/OTS CHARACTER LARGE OBJECT */
#define OCI_TYPECODE_CFILE       SQLT_CFILE /* SQL/OTS CHARACTER FILE OBJECT */

/* the following are ANSI datetime datatypes added in 8.1 */
#define OCI_TYPECODE_TIME        SQLT_TIME                   /* SQL/OTS TIME */
#define OCI_TYPECODE_TIME_TZ     SQLT_TIME_TZ             /* SQL/OTS TIME_TZ */
#define OCI_TYPECODE_TIMESTAMP   SQLT_TIMESTAMP         /* SQL/OTS TIMESTAMP */
#define OCI_TYPECODE_TIMESTAMP_TZ  SQLT_TIMESTAMP_TZ /* SQL/OTS TIMESTAMP_TZ */

#define OCI_TYPECODE_TIMESTAMP_LTZ  SQLT_TIMESTAMP_LTZ /* TIMESTAMP_LTZ */

#define OCI_TYPECODE_INTERVAL_YM SQLT_INTERVAL_YM   /* SQL/OTS INTRVL YR-MON */
#define OCI_TYPECODE_INTERVAL_DS SQLT_INTERVAL_DS  /* SQL/OTS INTRVL DAY-SEC */
#define OCI_TYPECODE_UROWID      SQLT_RDD                     /* Urowid type */


#define OCI_TYPECODE_OTMFIRST    228     /* first Open Type Manager typecode */
#define OCI_TYPECODE_OTMLAST     320                    /* last OTM typecode */
#define OCI_TYPECODE_SYSFIRST    228     /* first OTM system type (internal) */
#define OCI_TYPECODE_SYSLAST     235      /* last OTM system type (internal) */
#define OCI_TYPECODE_PLS_INTEGER   266     /* type code for PLS_INTEGER */

/* the following are PL/SQL-only internal. They should not be used */
#define OCI_TYPECODE_ITABLE      SQLT_TAB             /* PLSQL indexed table */
#define OCI_TYPECODE_RECORD      SQLT_REC                    /* PLSQL record */
#define OCI_TYPECODE_BOOLEAN     SQLT_BOL                   /* PLSQL boolean */

/* NOTE : The following NCHAR related codes are just short forms for saying
   OCI_TYPECODE_VARCHAR2 with a charset form of SQLCS_NCHAR. These codes are
   intended for use in the OCIAnyData API only and nowhere else. */
#define OCI_TYPECODE_NCHAR       286
#define OCI_TYPECODE_NVARCHAR2   287
#define OCI_TYPECODE_NCLOB       288


/* To indicate absence of typecode being specified */
#define OCI_TYPECODE_NONE          0
/* To indicate error has to be taken from error handle - reserved for
   sqlplus use */
#define OCI_TYPECODE_ERRHP         283

/* The OCITypeCode type is interchangeable with the existing SQLT type
   which is a ub2 */
typedef ub2 OCITypeCode;


/*----------------------- GET OPTIONS FOR TDO  ------------------------------*/

enum OCITypeGetOpt
{
  OCI_TYPEGET_HEADER,
                /* load only the header portion of the TDO when getting type */
  OCI_TYPEGET_ALL       /* load all attribute and method descriptors as well */
};
typedef enum OCITypeGetOpt OCITypeGetOpt;

/*
 * OCITypeGetOpt
 *
 * This is the flag passed to OCIGetTypeArray() to indicate how the TDO is
 * going to be loaded into the object cache.
 * OCI_TYPEGET_HEADER implies that only the header portion is to be loaded
 * initially, with the rest loaded in on a 'lazy' basis. Only the header is
 * needed for PL/SQL and OCI operations. OCI_TYPEGET_ALL implies that ALL
 * the attributes and methods belonging to a TDO will be loaded into the
 * object cache in one round trip. Hence it will take much longer to execute,
 * but will ensure that no more loading needs to be done when pinning ADOs
 * etc. This is only needed if your code needs to examine and manipulate
 * attribute and method information.
 *
 * The default is OCI_TYPEGET_HEADER.
 */


/*------------------------ TYPE ENCAPSULTATION LEVEL ------------------------*/

enum OCITypeEncap
{
  /* 0 = uninitialized */
  OCI_TYPEENCAP_PRIVATE,                /* private: only internally visible */
  OCI_TYPEENCAP_PUBLIC /* public: visible to both internally and externally */
};
typedef enum OCITypeEncap OCITypeEncap;
/*
 * OCITypeEncap - OCI Encapsulation Level
 */


/*---------------------------- TYPE METHOD FLAGS ----------------------------*/

enum OCITypeMethodFlag
{
  OCI_TYPEMETHOD_INLINE = 0x0001,                                  /* inline */
  OCI_TYPEMETHOD_CONSTANT = 0x0002,                              /* constant */
  OCI_TYPEMETHOD_VIRTUAL = 0x0004,                                /* virtual */
  OCI_TYPEMETHOD_CONSTRUCTOR = 0x0008,                        /* constructor */
  OCI_TYPEMETHOD_DESTRUCTOR = 0x0010,                          /* destructor */
  OCI_TYPEMETHOD_OPERATOR  = 0x0020,                             /* operator */
  OCI_TYPEMETHOD_SELFISH = 0x0040,     /* selfish method (generic otherwise) */

  OCI_TYPEMETHOD_MAP = 0x0080,                    /* map (relative ordering) */
  OCI_TYPEMETHOD_ORDER  = 0x0100,               /* order (relative ordering) */
  /* OCI_TYPEMETHOD_MAP and OCI_TYPEMETHOD_ORDER are mutually exclusive */

  OCI_TYPEMETHOD_RNDS= 0x0200,               /* Read no Data State (default) */
  OCI_TYPEMETHOD_WNDS= 0x0400,                        /* Write no Data State */
  OCI_TYPEMETHOD_RNPS= 0x0800,                      /* Read no Process State */
  OCI_TYPEMETHOD_WNPS= 0x1000,                     /* Write no Process State */
  OCI_TYPEMETHOD_ABSTRACT = 0x2000,    /* abstract (not instantiable) method */
  OCI_TYPEMETHOD_OVERRIDING = 0x4000,                   /* overriding method */
  OCI_TYPEMETHOD_PIPELINED = 0x8000                   /* method is pipelined */
};
typedef enum OCITypeMethodFlag OCITypeMethodFlag;

/* macros to test the type method flags */
#define OCI_METHOD_IS_INLINE(flag) bit((flag), OCI_TYPEMETHOD_INLINE)
#define OCI_METHOD_IS_CONSTANT(flag) bit((flag), OCI_TYPEMETHOD_CONSTANT)
#define OCI_METHOD_IS_VIRTUAL(flag) bit((flag), OCI_TYPEMETHOD_VIRTUAL)
#define OCI_METHOD_IS_CONSTRUCTOR(flag) bit((flag), OCI_TYPEMETHOD_CONSTRUCTOR)
#define OCI_METHOD_IS_DESTRUCTOR(flag) bit((flag), OCI_TYPEMETHOD_DESTRUCTOR)
#define OCI_METHOD_IS_OPERATOR(flag) bit((flag), OCI_TYPEMETHOD_OPERATOR)
#define OCI_METHOD_IS_SELFISH(flag) bit((flag), OCI_TYPEMETHOD_SELFISH)
#define OCI_METHOD_IS_MAP(flag) bit((flag), OCI_TYPEMETHOD_MAP)
#define OCI_METHOD_IS_ORDER(flag) bit((flag), OCI_TYPEMETHOD_ORDER)
#define OCI_METHOD_IS_RNDS(flag) bit((flag), OCI_TYPEMETHOD_RNDS)
#define OCI_METHOD_IS_WNDS(flag) bit((flag), OCI_TYPEMETHOD_WNDS)
#define OCI_METHOD_IS_RNPS(flag) bit((flag), OCI_TYPEMETHOD_RNPS)
#define OCI_METHOD_IS_WNPS(flag) bit((flag), OCI_TYPEMETHOD_WNPS)
#define OCI_METHOD_IS_ABSTRACT(flag) bit((flag), OCI_TYPEMETHOD_ABSTRACT)
#define OCI_METHOD_IS_OVERRIDING(flag) bit((flag), OCI_TYPEMETHOD_OVERRIDING)
#define OCI_METHOD_IS_PIPELINED(flag) bit((flag), OCI_TYPEMETHOD_PIPELINED)

#define OCI_TYPEMETHOD_IS_INLINE(flag) bit((flag), OCI_TYPEMETHOD_INLINE)
#define OCI_TYPEMETHOD_IS_CONSTANT(flag) bit((flag), OCI_TYPEMETHOD_CONSTANT)
#define OCI_TYPEMETHOD_IS_VIRTUAL(flag) bit((flag), OCI_TYPEMETHOD_VIRTUAL)
#define OCI_TYPEMETHOD_IS_CONSTRUCTOR(flag) \
  bit((flag), OCI_TYPEMETHOD_CONSTRUCTOR)
#define OCI_TYPEMETHOD_IS_DESTRUCTOR(flag) \
  bit((flag), OCI_TYPEMETHOD_DESTRUCTOR)
#define OCI_TYPEMETHOD_IS_OPERATOR(flag) bit((flag), OCI_TYPEMETHOD_OPERATOR)
#define OCI_TYPEMETHOD_IS_SELFISH(flag) bit((flag), OCI_TYPEMETHOD_SELFISH)
#define OCI_TYPEMETHOD_IS_MAP(flag) bit((flag), OCI_TYPEMETHOD_MAP)
#define OCI_TYPEMETHOD_IS_ORDER(flag) bit((flag), OCI_TYPEMETHOD_ORDER)
#define OCI_TYPEMETHOD_IS_RNDS(flag) bit((flag), OCI_TYPEMETHOD_RNDS)
#define OCI_TYPEMETHOD_IS_WNDS(flag) bit((flag), OCI_TYPEMETHOD_WNDS)
#define OCI_TYPEMETHOD_IS_RNPS(flag) bit((flag), OCI_TYPEMETHOD_RNPS)
#define OCI_TYPEMETHOD_IS_WNPS(flag) bit((flag), OCI_TYPEMETHOD_WNPS)
#define OCI_TYPEMETHOD_IS_ABSTRACT(flag) bit((flag), OCI_TYPEMETHOD_ABSTRACT)
#define OCI_TYPEMETHOD_IS_OVERRIDING(flag) \
  bit((flag), OCI_TYPEMETHOD_OVERRIDING)
#define OCI_TYPEMETHOD_IS_PIPELINED(flag) bit((flag), OCI_TYPEMETHOD_PIPELINED)

/* macros to set the type method flags */
#define OCI_TYPEMETHOD_SET_INLINE(flag) bis((flag), OCI_TYPEMETHOD_INLINE)
#define OCI_TYPEMETHOD_SET_CONSTANT(flag) bis((flag), OCI_TYPEMETHOD_CONSTANT)
#define OCI_TYPEMETHOD_SET_VIRTUAL(flag) bis((flag), OCI_TYPEMETHOD_VIRTUAL)
#define OCI_TYPEMETHOD_SET_CONSTRUCTOR(flag) \
  bis((flag), OCI_TYPEMETHOD_CONSTRUCTOR)
#define OCI_TYPEMETHOD_SET_DESTRUCTOR(flag) \
  bis((flag), OCI_TYPEMETHOD_DESTRUCTOR)
#define OCI_TYPEMETHOD_SET_OPERATOR(flag) bis((flag), OCI_TYPEMETHOD_OPERATOR)
#define OCI_TYPEMETHOD_SET_SELFISH(flag) bis((flag), OCI_TYPEMETHOD_SELFISH)
#define OCI_TYPEMETHOD_SET_MAP(flag) bis((flag), OCI_TYPEMETHOD_MAP)
#define OCI_TYPEMETHOD_SET_ORDER(flag) bis((flag), OCI_TYPEMETHOD_ORDER)
#define OCI_TYPEMETHOD_SET_RNDS(flag) bis((flag), OCI_TYPEMETHOD_RNDS)
#define OCI_TYPEMETHOD_SET_WNDS(flag) bis((flag), OCI_TYPEMETHOD_WNDS)
#define OCI_TYPEMETHOD_SET_RNPS(flag) bis((flag), OCI_TYPEMETHOD_RNPS)
#define OCI_TYPEMETHOD_SET_WNPS(flag) bis((flag), OCI_TYPEMETHOD_WNPS)

/* macros to clear the type method flags */
#define OCI_TYPEMETHOD_CLEAR_INLINE(flag) bic((flag), OCI_TYPEMETHOD_INLINE)
#define OCI_TYPEMETHOD_CLEAR_CONSTANT(flag) \
  bic((flag), OCI_TYPEMETHOD_CONSTANT)
#define OCI_TYPEMETHOD_CLEAR_VIRTUAL(flag) bic((flag), OCI_TYPEMETHOD_VIRTUAL)
#define OCI_TYPEMETHOD_CLEAR_CONSTRUCTOR(flag) \
  bic((flag), OCI_TYPEMETHOD_CONSTRUCTOR)
#define OCI_TYPEMETHOD_CLEAR_DESTRUCTOR(flag) \
  bic((flag), OCI_TYPEMETHOD_DESTRUCTOR)
#define OCI_TYPEMETHOD_CLEAR_OPERATOR(flag) \
  bic((flag), OCI_TYPEMETHOD_OPERATOR)
#define OCI_TYPEMETHOD_CLEAR_SELFISH(flag) bic((flag), OCI_TYPEMETHOD_SELFISH)
#define OCI_TYPEMETHOD_CLEAR_MAP(flag) bic((flag), OCI_TYPEMETHOD_MAP)
#define OCI_TYPEMETHOD_CLEAR_ORDER(flag) bic((flag), OCI_TYPEMETHOD_ORDER)
#define OCI_TYPEMETHOD_CLEAR_RNDS(flag) bic((flag), OCI_TYPEMETHOD_RNDS)
#define OCI_TYPEMETHOD_CLEAR_WNDS(flag) bic((flag), OCI_TYPEMETHOD_WNDS)
#define OCI_TYPEMETHOD_CLEAR_RNPS(flag) bic((flag), OCI_TYPEMETHOD_RNPS)
#define OCI_TYPEMETHOD_CLEAR_WNPS(flag) bic((flag), OCI_TYPEMETHOD_WNPS)

/*--------------------------- TYPE PARAMETER MODE ---------------------------*/

enum OCITypeParamMode
{
  /* PL/SQL starts this from 0 */
  OCI_TYPEPARAM_IN = 0,                                                /* in */
  OCI_TYPEPARAM_OUT,                                                  /* out */
  OCI_TYPEPARAM_INOUT,                                             /* in-out */
  OCI_TYPEPARAM_BYREF,              /* call by reference (implicitly in-out) */
  OCI_TYPEPARAM_OUTNCPY,                         /* OUT with NOCOPY modifier */
  OCI_TYPEPARAM_INOUTNCPY                     /* IN OUT with NOCOPY modifier */
};
typedef enum OCITypeParamMode OCITypeParamMode;


/*-------------------------------- DEFAULTS ---------------------------------*/

/* default binary and decimal precision and scale */

#define OCI_NUMBER_DEFAULTPREC ((ub1)0)            /* no precision specified */
#define OCI_NUMBER_DEFAULTSCALE ((sb1)MAXSB1MINVAL)
                                        /* no binary/decimal scale specified */

/* default maximum length for varrays and vstrings (used in sql.bsq) */

#define OCI_VARRAY_MAXSIZE 4000
                          /* default maximum number of elements for a varray */
#define OCI_STRING_MAXLEN  4000     /* default maximum length of a vstring */

/*---------------------------------------------------------------------------*/
/* This set of macro is used only in beta2. They should be removed as soon as
 * PLSQL has made the changes of not using these macros.
 */
 
/* Special duration for allocating memory only. No instance can be allocated
 * given these durations.
 */
#define OCICoherency OCIRefreshOpt
#define OCI_COHERENCY_NONE   (OCIRefreshOpt)2
#define OCI_COHERENCY_NULL   (OCIRefreshOpt)4
#define OCI_COHERENCY_ALWAYS (OCIRefreshOpt)5

#endif /* ORO_ORACLE */

