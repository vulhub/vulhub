/* Copyright (c) 1993, 2014, Oracle and/or its affiliates. 
All rights reserved.*/

/*
  Author:             Srinath Krishnaswamy
  Date:               11/24/93
  Source documents:   "Functional Specification for C Language Mapping of
                      OTS Types, Object Management Subsystem", "Oracle C
                      Coding Standards version 2.2", and the header file
                      template
  Rule sets:          the generic and .h file rule sets
  Quality status:     not exited
  Identification tag: [ one or more letters to identify the .h file ]
  Revision code:      11/24/93
 
  NAME

    ORL - ORacle's external C Language interface to primitive OTS types
 
  DESCRIPTION

    This header file contains C langauge interface to the OTS primitive
    types. The interface includes C mapping of OTS primitive types and
    the prototype of the pre-defined operations on the primitive types.
 
    ***********************************************************************
    *** NOTE: The OCI functions for objects requires the                ***
    ***       application to be running in OBJECT mode. That is, during ***
    ***       process initialization OCIInitialize(), the mode          ***
    ***       specified should be OBJECT mode.                          ***
    **                OCIInitialize(OCI_OBJECT, ..);                    ***
    ***********************************************************************

  RELATED DOCUMENTS

    [1] Krishnaswamy, Srinath and Nguyen, Tin A., "Functional Specification
        for C Language Mapping of OTS Types, Object Management Subsystem",
        March 1994.
    [2] Nguyen, Tin A., "The Open Type System", Oracle Corporation,
        February 1994.
    [3] Klein, Jonathan D., "Large Field Management", Oracle Corporation,
        October 1993.
 
  PUBLIC FUNCTIONS
 
    OCI - OCI functions to manipulate Oracle Number, float and decimal
      ARITHMETIC
        OCINumberInc - OCINumber INCrement (optimized)
        OCINumberDec - OCINumber DECrement (optimized)
        OCINumberAdd - OCINumber ADD numbers
        OCINumberSub - OCINumber SUBtract numbers
        OCINumberMul - OCINumber MULtiply numbers
        OCINumberDiv - OCINumber DIVide numbers
        OCINumberMod - OCINumber MODulo division
        OCINumberIntPower - OCINumber integer PoWeR
        OCINumberShift - OCINumber decimal ShiFT number
        OCINumberNeg - OCINumber NEGate number
        OCINumberAbs - OCINumber ABSolute value
        OCINumberCeil - OCINumber CEiling of number
        OCINumberFloor - OCINumber FLooR of number
        OCINumberSqrt - OCINumber SQuare Root of number
        OCINumberSign - OCINumber get SIGN of number
      NATIVE TYPE CONVERSION
        OCINumberToInt  - OCINumber convert number TO machine-format Integer
        OCINumberFromInt - OCINumber convert machine-format Integer TO Number
        OCINumberToReal  - OCINumber convert number TO machine-format Real
        OCINumberToRealArray  - OCINumber convert array of numbers TO
                                machine-format Real
        OCINumberFromReal - OCINumber convert machine-format Real TO Number
      TEXT STRING CONVERSION
        OCINumberToText  - OCINumber convert number TO String
        OCINumberFromText - OCINumber convert String TO Number
      COMPARISON
        OCINumberCmp - OCINumber CoMPare numbers
        OCINumberIsZero - OCINumber comparison with ZERo
        OCINumberIsInt - OCINumber Is an Integer
      ASSIGNMENT
        OCINumberAssign - OCINumber ASsiGn number
        OCINumberSetZero - OCINumber Set number to Zero value
        OCINumberSetPi - OCINumber Set number to Pi
      ROUNDING
        OCINumberTrunc - OCINumber TRUncate an Oracle number
        OCINumberRound - OCINumber ROUnd number
        OCINumberPrec - OCINumber round to Precision digits
      TRANSCENDENTAL
        OCINumberSin - OCINumber SINe
        OCINumberArcSin - OCINumber Arc SINe
        OCINumberHypSin - OCINumber SiNe Hyperbolic
        OCINumberCos - OCINumber COSine
        OCINumberArcCos - OCINumber Arc COSine
        OCINumberHypCos - OCINumber CoSine Hyperbolic
        OCINumberTan - OCINumber TANgent
        OCINumberArcTan - OCINumber Arc TANgent
        OCINumberArcTan2 - OCINumber Arc TaNgent 2
        OCINumberHypTan - OCINumber TaNgent Hyperbolic
        OCINumberPower - OCINumber arbitrary Base EXponentiation
        OCINumberExp - OCINumber EXPonentiation to base e
        OCINumberLn  - OCINumber Logarithm Natural
        OCINumberLog - OCINumber LOGarithm to arbitrary base

    OCIDate - OCI functions to manipulate OCI Date
      OCIDateToExternal - OCIDate convert date to external form
      OCIDateFromExternal - OCIDate convert external form of date into OCIDate
      OCIDateAssign - OCIDate Assignment
      OCIDateToText  - OCIDate convert date TO String
      OCIDateFromText - OCIDate convert String TO Date
      OCIDateZoneToZone - OCIDate convert date from one time 
                        Zone TO another Zone
      OCIDateCompare - OCIDate CoMPare dates
      OCIDateAddMonths - OCIDate ADd or subtract Months
      OCIDateAddDays - OCIDate ADd or subtract Days
      OCIDateLastDay - OCIDate get date of LaST day of month
      OCIDateDaysBetween - OCIDate get number of days BeTWeen two dates
      OCIDateNextDay - OCIDate get date of Next DaY
      OCIDateCheck - OCIDate CHecK if the given date is valid
      OCIDateSysDate - OCIDate get current SYStem date and time

    OCIString - OCI String functions to manipulate Variable-length string
      OCIStringAssign - OCIString Assign string to string
      OCIStringAssignText - OCIString Assign Text string to string
      OCIStringResize - OCIString ReSiZe string
      OCIStringSize - OCIString get String Size
      OCIStringPtr - OCIString get String PoinTeR
      OCIStringAllocSize - OCIString get Allocated SiZe
 
    OCIRaw - OCI Raw functions to manipulate variable-length raW
      OCIRawAssignRaw - OCIRaw Assign Raw (of type OCIRaw*) to raw
                        (of type OCIRaw*)
      OCIRawResize - OCIRaw Resize raw
      OCIRawSize - OCIRaw get Raw Size
      OCIRawPtr  - OCIRaw get Raw data Pointer
      OCIRawAllocSize - OCIRaw get Allocated Size

    OCIColl - OCI Collection generic functions. These functions can be 
                used to manipulate both variable-length array (varray) and 
                nested table.
      OCICollSize  - OCIColl return current SIZe of the given collection
                (in number of elements)
      OCICollMax  - OCIColl return the MAXimum number of elements in the
                collection (i.e. upper-bound)
      OCICollGetElem  - OCIColl GET pointer to the element at the given 
                position
      OCICollAssignElem  - OCIColl assign to element at given index
      OCICollAssign  - OCIColl ASsiGn collection; perform deep-copy of source 
                collection to target collection
      OCICollAppend  - OCIColl aPPend the given element to the end of the
                collection
      OCICollTrim  - OCIColl trim (delete) the given number of elements 
                from the end of the collection
      OCICollIsLocator - OCIColl indicates whether a collection is locator
                         based or not.
      OCIIterCreate  - OCIColl Create an ITerator to scan the collection 
                elements
      OCIIterDelete  - OCIColl Delete ITerator
      OCIIterInit  - OCIColl Initialize ITerator to scan the given collection
      OCIIterGetCurrent - OCIColl Iterator based, get CURrent 
                collection element
      OCIIterNext - OCIColl Iterator based, get NeXT collection element
      OCIIterPrev - OCIColl Iterator based, get PReVious collection element
 
      OCITable - OCI functions to manipulate nested Table. The OCIColl*() and 
                OCITable*() functions can be used to manipulate nested table
      OCITableDelete(i) - OCITable if element(i) exists then the element is 
                marked as deleted else the function returns false. So 
                delete's create "holes".
      OCITableExists(i) - OCITable return true iff an element at
                position i EXIsts
      OCITableFirst  - OCITable return the smallest value of i for which
                exists(i) is true.
      OCITableLast  - OCITable return the largest value of i for which 
                exists(i) is true.
      OCITableNext(i) - OCITable return pointer to the smallest position j, 
                greater than i, such that OCITableExists(j) is true
      OCITablePrev(i) - OCITable return pointer to the largest position j,
                less than i, such that OCITableExists(j) is true
      OCITableSize - OCITable return current SIZe of the given nested table not
                including deleted elements

    OCIRef - OCI functions to manipulate object Reference
      OCIRefClear  - OCIRef CLeaR or nullify a ref
      OCIRefAssign  - OCIRef ASsiGn a ref to another
      OCIRefIsEqual  - OCIRef compare two refs for EQUality
      OCIRefIsNull  - OCIRef test if a ref is NULl 
      OCIRefFromHex  - OCIRef convert a Hexadecimal string TO a Ref
      OCIRefToHex   - OCIRef convert a ref to a Hexadecimal string
      OCIRefHexSize  - OCIRef get size of buffer in bytes to store hexadecimal
                string

  OBSOLETE: to be replaced by functions from oci.h:

    ORLL - ORL functions to manipulate lob Locators
      orllasg   - ORLL AsiGn one locator to another
      orllequ   - ORLL compare two locators for EQUality
      orlliini  - ORLL Is the locator INItialized?
      orllgsz   - ORLL Get locator SiZe
      orllgcid  - ORLL Get Character set ID

      NOTE: The following are specific to FILE lobs:

      orllsnm - ORLL Set directory alias and file NaMe in the locator
      orllgnm - ORLL Get directory alias and file NaMe from the locator
 
  EXAMPLES

    Examples are given in the description of each function where
    relevant.
 
  NOTES

    This file has been subsetted to contain ONLY the routines that will
    be in the first release.
 
  QUESTIONS
     
  MODIFIED
    dpotpaov   04/24/14 - Fix oci ind string raw
    dpotapov   03/06/14 - xtss merge
    bkhaladk   08/01/05 - add defn for OCIBinXmlReposCtx 
    dmukhin    06/29/05 - ANSI prototypes; miscellaneous cleanup 
    srseshad   03/12/03 - convert oci public api to ansi
    rpingte    11/21/02 - Add OCICollGetElemArray and OCINumberToRealArray
    aahluwal   06/03/02 - bug 2360115
    gayyappa   02/01/02 - fix 2210776 : change Dom to DOM 
    whe        09/25/01 - add OCIXMLType & OCIDomDocument opaque types
    bpalaval   02/09/01   - Change text to oratext.
    rkasamse   09/20/99 -   lint changes
    whe        09/01/99 -   976457:check __cplusplus for C++ code
    hsbedi     08/11/99 -   Add macro
    rxgovind   10/14/98 -  make non exposed calls (OCIRowType etc) private
    rxgovind   06/09/98 -  update OCIRowTypeCreate
    nmantrav   05/11/98 -  add OCIRowTypeGetCount
    rxgovind   03/29/98 -  add OCIRowType and OCIRowData interfaces
    jwijaya    05/06/98  - add OCICollIsLocator                                
    rxgovind   03/18/98 -  opaque types: move to kolo.h
    etucker    02/02/98 -  add comments for Dec and Inc
    etucker    01/29/98 -  Finish core5 integration
    rxgovind   11/11/97 -  opaque types
    etucker    10/28/97 -  add ORLN functions for SDK
    cxcheng    07/28/97 -  remove OCILobLocator #define
    skmishra   05/13/97 -  stdcc compatibility changes
    skrishna   04/25/97 -  rename OCINumber*(): Exp Power TanHyp Zero Init
                           TanToArc Sqr Truncate and Compare 
    skotsovo   03/31/97 -  remove OCILobLocatorSize
    skrishna   03/25/97 -  remove orld2i and orldi2d
    skrishna   03/18/97 -  fix ifdef for supporting ansi and k&r proto-types
    cxcheng    02/06/97 -  take out short name support except with SLSHORTNAME
    skrishna   01/06/97 -  update OCITableSize() comments
    skrishna   12/27/96 -  fix OCIDateGet/OCIDateSet
    skrishna   12/12/96 -  update OCICollGelElem comments
    skrishna   11/07/96 -  OCICollGetElem: interface change
    skrishna   11/05/96 -  add OCIDate Get/Set and OCIDateAssign
    cxcheng    10/31/96 -  change OCINumberTanHyp to OCINumberHypTan
    cxcheng    10/30/96 -  #define orll short names to long names
    dchatter   10/26/96 -  fix some OCI file long names
    cxcheng    10/24/96 -  remove unnecessary comment in front
    cxcheng    10/14/96 -  disable long name mapping for LOB functions
    skrishna   10/13/96 -  continue beautification
    skotsovo   10/16/96 -  update ocilob names
    cxcheng    10/09/96 -  add structure members in #define for date/time
    cxcheng    10/09/96 -  more lint fixes
    skrishna   10/09/96 -  continue beautification
    cxcheng    10/09/96 -  more fixes
    skrishna   10/09/96 -  change fixed-char rep. to orlvstr*
    jwijaya    10/08/96 -  continue beautification
    jwijaya    10/07/96 -  beautify
    cxcheng    10/07/96 -  more changes
    cxcheng    10/04/96 -  replace short names with long names
    skrishna   10/01/96 -  orlcsiz, orltsiz: change prototype to take errhdl
    skrishna   09/23/96 -  fix lint errors
    skotsovo   09/23/96 -  remove orllmkcur().
    jwijaya    09/17/96 -  comments on null ref
    skrishna   09/19/96 -  change orlraw format
    skotsovo   09/19/96 -  add orlliini and remove orllnul
    skrishna   08/14/96 -  orlvstr: change format to ub4 followed by text
    jboonleu   08/06/96 -  update comment
    skotsovo   08/08/96 -  revert to locators instead of descriptors as input t
    jboonleu   07/23/96 -  remove orlrcur
    skrishna   07/06/96 -  add orltsiz
    skrishna   07/05/96 -  add orld2i and orldi2d
    jwijaya    07/03/96 -  add ANSI prototypes
    skrishna   06/27/96 -  document default string format in orlds2d & orld2s
    skrishna   06/25/96 -  change max date value
    skrishna   06/18/96 -  modify orld2s() comments
    skotsovo   06/13/96 -  orll functions take lob descriptors instead of locat
    rxgovind   06/05/96 -  change prototype of orlrcur to take ocienvh
    skrishna   05/30/96 -  support collection trimming
    skrishna   05/30/96 -  remove orlralo/fre and orllalo/fre instead use 
                           orionew/fre
    skrishna   05/28/96 -  add orlt*() and modify orla*()
    skotsovo   05/23/96 -  add orlbl typedefs for pro*c
    jboonleu   05/14/96 -  add orlrcur
    rxgovind   05/08/96 -  changes for 3gl callbacks
    skotsovo   05/01/96 -  in orllasg, no need to alloc orlbl*
    skrishna   04/21/96 -  merge changes from 960418 object branch into big
    skrishna   04/17/96 -  rename orlrcpy to orlrasg
    skrishna   04/12/96 -  add orlr2h and orlrh2r functions
    skotsovo   04/15/96 -  add fnt to make the lob locator current
    skrishna   04/08/96 -  change orl*() to take ocienvh* and ocierrh* instead
                           of oroenv*
    skotsovo   03/22/96 -  add locator functions
    skotsovo   03/22/96 -  add locator functions
    skrishna   02/27/96 -  remove mlslabel interface
    skotsovo   02/20/96 -  remove orlbty and use dty type instead.
    skotsovo   02/14/96 -  add text file lobs.
    skrishna   01/31/96 -  update comments of orln2r, orldchk, orlds2d & orld2s
    skrishna   01/31/96 -  change orld2s() and orln2s() to return string length
    skrishna   01/21/96 -  remove old raw interface 
    skrishna   12/14/95 -  add raw interface
    skotsovo   01/03/96 -  change LOB offsets and lengths from ub4 to ubig_ora
                           to support 64 bit machines.
    skotsovo   10/30/95 -  add orlblsiz() to get lob locator size
    skrishna   10/24/95 -  move ref functions from ori and update the ref
                           functions to support variable-length ref
    cxcheng    10/20/95 -  add more comments on number versions
    cxcheng    10/13/95 -  add more number functions
    cxcheng    08/29/95 -  Support for segmented varrays
    cxcheng    08/18/95 -  modifiy orlmls structure
    skrishna   06/06/95 -  rename orln, orld, orlvs and orlva to orlnum,
                           orldat, orlvstr and orlvary respectively 
    skrishna   11/15/94 -  remove orlnget() function 
    skrishna   09/20/94 -  modify orldbtw() to return number of days only 
    skrishna   08/24/94 -  change format string length type from ub4 to ub1 
    skrishna   07/19/94 -  Rename orln2c & orlnc2n to orln2s & orlns2n 
    skrishna   06/29/94 -  Add blob interface; add examples 
    skrishna   06/23/94 -  Update comments and format 
    skrishna   05/19/94 -  update varray append comments 
    skrishna    05/05/94 -  Subsetting 
    skrishna    11/24/93 -  Creation
*/

#ifndef ORATYPES
#include <oratypes.h>
#endif

#ifndef ORO_ORACLE
#include <oro.h>
#endif

#ifndef ORT_ORACLE
#include <ort.h>
#endif

#ifndef OCI_ORACLE
#include <oci.h>
#endif

#ifndef ORL_ORACLE
#define ORL_ORACLE

#ifdef K3_ORACLE
#ifndef KOL3_ORACLE
#  include <kol3.h>
#endif
#endif

/*---------------------------------------------------------------------------*/
/*                         SHORT NAMES SUPPORT SECTION                       */
/*---------------------------------------------------------------------------*/

#ifdef SLSHORTNAME

/* the following are short names that are only supported on IBM mainframes
   with the SLSHORTNAME defined.
   With this all subsequent long names will actually be substituted with
   the short names here */

#define OCIArray                        orlvary
#define OCIColl                         orlcol
#define OCICollAppend                   orlcapp
#define OCICollAssign                   orlcasg
#define OCICollAssignElem               orlcase
#define OCICollGetElem                  orlcget
#define OCICollGetElemArray             orlcgeta
#define OCICollMax                      orlcmax
#define OCICollSize                     orlcsiz
#define OCICollTrim                     orlctrm
#define OCICollIsLocator                orlcilc
#define OCIDate                         orldat
#define OCIDateAddDays                  orldadd
#define OCIDateAddMonths                orldadm
#define OCIDateCheck                    orldchk
#define OCIDateCompare                  orldcmp
#define OCIDateDD                       day_orldat
#define OCIDateDaysBetween              orldbtw
#define OCIDateFromText                 orlds2d
#define OCIDateLastDay                  orldlst
#define OCIDateMM                       mon_orldat
#define OCIDateNextDay                  orldndy
#define OCIDateSysDate                  orldsys
#define OCIDateTime                     time_orldat
#define OCIDateYYYY                     gye_orldat
#define OCIDateZoneToZone               orldz2z
#define OCIIter                         orlcitr
#define OCIIterCreate                   orlccit
#define OCIIterDelete                   orlcdit
#define OCIIterGetCurrent               orlcicur
#define OCIIterInit                     orlciit
#define OCIIterNext                     orlcinxt
#define OCIIterPrev                     orlciprv
#define OCINumber                       orlnum
#define OCINumberAbs                    orlnabs
#define OCINumberAdd                    orlnadd
#define OCINumberArcCos                 orlnacos
#define OCINumberArcSin                 orlnasin
#define OCINumberArcTan                 orlnatan
#define OCINumberAssign                 orlnasg
#define OCINumberCeil                   orlncel
#define OCINumberCos                    orlncos
#define OCINumberDiv                    orlndiv
#define OCINumberPower                  orlnbex
#define OCINumberFloor                  orlnflr
#define OCINumberFromInt                orlni2n
#define OCINumberFromReal               orlnr2n
#define OCINumberFromText               orlns2n
#define OCINumberHypCos                 orlncsh
#define OCINumberHypSin                 orlnsnh
#define OCINumberSetZero                orlnini
#define OCINumberSetPi                  orlnspi
#define OCINumberInc                    orlninc
#define OCINumberDec                    orlndec
#define OCINumberIntPower               orlnpwr
#define OCINumberLn                     orlnln
#define OCINumberLog                    orlnlog
#define OCINumberMod                    orlnmod
#define OCINumberMul                    orlnmul
#define OCINumberNeg                    orlnneg
#define OCINumberPart                   orlnpart
#define OCINumberExp                    orlnexp
#define OCINumberRound                  orlnrou
#define OCINumberPrec                   orlnpre
#define OCINumberShift                  orlnsft
#define OCINumberSign                   orlnsgn
#define OCINumberSin                    orlnsin
#define OCINumberSqrt                   orlnsqr
#define OCINumberSub                    orlnsub
#define OCINumberTan                    orlntan
#define OCINumberHypTan                 orlntnh
#define OCINumberArcTan2                orlnatn2
#define OCINumberToInt                  orln2i
#define OCINumberToReal                 orln2r
#define OCINumberToRealArray            orln2ra
#define OCINumberToText                 orln2s
#define OCINumberTrunc                  orlntru
#define OCINumberCmp                    orlncmp
#define OCINumberIsZero                 orlnzer
#define OCINumberIsInt                  orlnint
#define OCIRaw                          orlraw
#define OCIRawAllocSize                 orlwasz
#define OCIRawAssignBytes               orlwabr
#define OCIRawAssignRaw                 orlwarr
#define OCIRawPtr                       orlwgrp
#define OCIRawResize                    orlwrsz
#define OCIRawSize                      orlwgsz
#define OCIRefAssign                    orlrasg
#define OCIRefClear                     orlrclr
#define OCIRefFromHex                   orlrh2r
#define OCIRefHexSize                   orlrhsz
#define OCIRefIsEqual                   orlrequ
#define OCIRefIsNull                    orlrnul
#define OCIRefToHex                     orlr2h
#define OCIString                       orlvstr
#define OCIStringAllocSize              orlvasz
#define OCIStringAssign                 orlvass
#define OCIStringAssignText             orlvats
#define OCIStringPtr                    orlvgsp
#define OCIStringResize                 orlvrsz
#define OCIStringSize                   orlvgsz
#define OCITable                        orltbl
#define OCITableDelete                  orltdel
#define OCITableExists                  orltexi
#define OCITableFirst                   orltfst
#define OCITableLast                    orltlst
#define OCITableNext                    orltnxt
#define OCITablePrev                    orltprv
#define OCITableSize                    orltsiz
#define OCITime                         orldtm
#define OCITimeHH                       orldtmhh
#define OCITimeMI                       orldtmmm
#define OCITimeSS                       orldtmss
#define OCI_LOBMODE_READONLY            ORLBMORO
#define OCI_LOBMODE_READWRITE           ORLBMORW

#endif                                                        /* SLSHORTNAME */

/*****************************************************************************/
/*                         NUMBER/FLOAT/DECIMAL TYPE                         */
/*****************************************************************************/


#define OCI_NUMBER_SIZE 22
#ifndef KUTYXTT3_ORACLE
struct OCINumber
{
  ub1 OCINumberPart[OCI_NUMBER_SIZE];
};
typedef struct OCINumber OCINumber;
#define ORL_OCINUMBER_DEFINED
#endif

/* 
 * OCINumber - OCI Number mapping in c
 *
 * The OTS types: NUMBER, NUMERIC, INT, SHORTINT, REAL, DOUBLE PRECISION,
 * FLOAT and DECIMAL are represented by OCINumber.
 * The contents of OCINumber is opaque to clients. 
 *
 * For binding variables of type OCINumber in OCI calls (OCIBindByName(),
 * OCIBindByPos(), and OCIDefineByPos()) use the type code SQLT_VNU.
 */

/*
   EXAMPLE 

   The following example shows how to manipulate an attribute of type 
   oracle number.
  
     struct person
     {
       OCINumber sal;
     };
     typedef struct person person;

     OCIError *err;
     person* joe;
     person* tom;
     person* debbie;
     OCINumber  *joesal;
     OCINumber  *tomsal;
     OCINumber *debsal;
     sword   status;
     int     inum;
     double  dnum;
     OCINumber ornum;
     char    buffer[21];
     ub4     buflen;
     sword   result;
     
     /o See oci.h for an example of how to initialize OCIError.
      o For this example, assume the OCIEnv and OCIError has been
      o initialized.
      o/

     /o Pin joe, tom and debbie person objects in the object cache. See ori.h
      o for an example on pinning objects. For this example, assume that
      o joe, tom and debbie are pointing to pinned objects.
      o/
     joesal = &joe->sal;
     tomsal = &tom->sal;
     debsal = &debbie->sal;

     /o initialize joe's salary to be $12,000 o/
     inum = 12000;
     status = OCINumberFromInt(err, &inum, sizeof(inum), OCI_NUMBER_SIGNED,
                               joesal);
     if (status != OCI_SUCCESS)
                              /o goto to handle error from OCINumberFromInt o/;

     /o initialize tom's salary to be same as joe o/
     OCINumberAssign(err, joesal, tomsal);

     /o initialize debbie's salary to be 20% more than joe's o/
     dnum = 1.2;
     status = OCINumberFromReal(err, &dnum, sizeof(double), &ornum);
     if (status != OCI_SUCCESS)
                            /o goto to handle error from OCINumberFromReal o/;
     status = OCINumberMul(err, joesal, &ornum, debsal);
     if (status != OCI_SUCCESS)  /o goto to handle error from OCINumberMul o/;

     /o give tom a 50% raise o/
     dnum = 1.5;
     status = OCINumberFromReal(err, &dnum, sizeof(double), &ornum);
     if (status != OCI_SUCCESS)
                            /o goto to handle error from OCINumberFromReal o/;
     status = OCINumberMul(err, tomsal, &ornum, tomsal);
     if (status != OCI_SUCCESS)  /o goto to handle error from OCINumberMul o/;

     /o double joe's salary o/
     status = OCINumberAdd(err, joesal, joesal, joesal);
     if (status != OCI_SUCCESS)  /o goto to handle error from OCINumberAdd o/;

     /o get joe's salary in integer o/
     status = OCINumberToInt(err, joesal, sizeof(inum), OCI_NUMBER_SIGNED,
                             &inum);
     if (status != OCI_SUCCESS)/o goto to handle error from OCINumberToInt o/;
     /o inum is set to 24000 o/

     /o get debbie's salary in double o/
     status = OCINumberToReal(err, debsal, sizeof(dnum), &dnum);
     if (status != OCI_SUCCESS)/o goto to handle error from OCINumberToReal o/;
     /o dnum is set to 14400 o/

     /o print tom's salary as DEM0001`8000.00 o/
     buflen = sizeof(buffer);
     status = OCINumberToText(err, tomsal, "C0999G9999D99", 13, 
                 "NLS_NUMERIC_CHARACTERS='.`' NLS_ISO_CURRENCY='Germany'", 54,
                 &buflen, buffer);
     if (status != OCI_SUCCESS)/o goto to handle error from OCINumberToText o/;
     printf("tom's salary = %s\n", buffer);

     /o compare joe and tom's salary o/
     status = OCINumberCmp(err, joesal, tomsal, &result);
     if (status != OCI_SUCCESS)  /o goto to handle error from OCINumberCmp o/;
     /o result is positive o/

     /o read debbie's new salary from string o/
     status = OCINumberFromText(err, "48`000.00", 9, "99G999D99", 9,
                      "NLS_NUMERIC_CHARACTERS='.`'", 27, debsal);
     if (status != OCI_SUCCESS)
                            /o goto to handle error from OCINumberFromText o/;
     /o debbie's salary is now 48000.00 o/

*/

/*----------------------------- OCINumberInc --------------------------------*/

sword OCINumberInc(    OCIError *err, OCINumber *number    );
/*
   NAME: OCINumberInc - OCINumber INCrement numbers
   PARAMETERS:
        err (IN/OUT) - error handle. If there is an error, it is
                recorded in 'err' and this function returns OCI_ERROR.
                The error recorded in 'err' can be retrieved by calling
                OCIErrorGet().
        number (IN/OUT) a positive Oracle number to be incremented
   DESCRIPTION:
        Increment Oracle number in place. It is assumed that the input is
        an integer between 0 and 100^21-2. If the is input too large, it will
        be treated as 0 - the result will be an Oracle number 1. If the input
        is not a positive integer, the result will be unpredictable.
   RETURNS:
        OCI_SUCCESS if the function completes successfully.
        OCI_INVALID_HANDLE if 'err' is NULL.
        OCI_ERROR if
          any of the number arguments is null
 */

/*----------------------------- OCINumberDec --------------------------------*/

sword OCINumberDec(    OCIError *err, OCINumber *number    );
/*
   NAME: OCINumberDec - OCINumber DECrement numbers
   PARAMETERS:
        err (IN/OUT) - error handle. If there is an error, it is
                recorded in 'err' and this function returns OCI_ERROR.
                The error recorded in 'err' can be retrieved by calling
                OCIErrorGet().
        number (IN/OUT) - a positive Oracle number to be decremented
   DESCRIPTION:
        Decrement Oracle number in place. It is assumed that the input is an
        integer between 1 and 100^21-2. If the input is too large, it will be
        treated as 1 - the result will be an Oracle number 0. If the input is
        not a positive integer, the result will be unpredictable.
   RETURNS:
        OCI_SUCCESS if the function completes successfully.
        OCI_INVALID_HANDLE if 'err' is NULL.
        OCI_ERROR if
          any of the number arguments is null
 */

/*-------------------------- OCINumberSetZero -------------------------------*/

void OCINumberSetZero(    OCIError *err, OCINumber *num    );
/*
   NAME: OCINumberSetZero - OCINumber Set number to Zero value
   PARAMETERS:
        err (IN/OUT) - pointer to OCI error handle
        num (OUT) - set to zero value
   DESCRIPTION:
        Initialize the given number to value 0.
 */

/*--------------------------- OCINumberSetPi --------------------------------*/

void OCINumberSetPi(    OCIError *err, OCINumber *num    );
/*
   NAME: OCINumberSetPi - OCINumber Set number to Pi
        err (IN/OUT) - pointer to OCI error handle
        num (OUT) - set to zero value
   DESCRIPTION:
        Initialize the given number to value Pi.
 */

/*----------------------------- OCINumberAdd --------------------------------*/

sword OCINumberAdd(    OCIError *err, const OCINumber *number1, 
                       const OCINumber *number2, OCINumber *result    );
/*
   NAME: OCINumberAdd - OCINumber ADD numbers
   PARAMETERS:
        err (IN/OUT) - error handle. If there is an error, it is
                recorded in 'err' and this function returns OCI_ERROR.
                The error recorded in 'err' can be retrieved by calling
                OCIErrorGet().
        number1, number2 (IN) - numbers to be added
        result (OUT) - result of adding 'number1' with 'number2'
   DESCRIPTION:
        Add 'number1' with 'number2' and return result in 'result'.
   RETURNS:
        OCI_SUCCESS if the function completes successfully.
        OCI_INVALID_HANDLE if 'err' is NULL.
        OCI_ERROR if
          any of the number arguments is null
 */

/*----------------------------- OCINumberSub --------------------------------*/

sword OCINumberSub(    OCIError *err, const OCINumber *number1, 
                       const OCINumber *number2, OCINumber *result    );
/*
   NAME: OCINumberSub - OCINumber SUBtract numbers
   PARAMETERS:
        err (IN/OUT) - error handle. If there is an error, it is
                recorded in 'err' and this function returns OCI_ERROR.
                The error recorded in 'err' can be retrieved by calling
                OCIErrorGet().
        number1, number2 (IN) - 'number2' subtracted from 'number1'
        result (OUT) - subtraction result
   DESCRIPTION:
        Subtract 'number2' from 'number1' and return result in 'result'.
   RETURNS:
        OCI_SUCCESS if the function completes successfully.
        OCI_INVALID_HANDLE if 'err' is NULL.
        OCI_ERROR if
          any of the number arguments is null
 */

/*----------------------------- OCINumberMul --------------------------------*/

sword OCINumberMul(    OCIError *err, const OCINumber *number1, 
                       const OCINumber *number2, OCINumber *result    );
/*
   NAME: OCINumberMul - OCINumber MULtiply numbers
   PARAMETERS:
        err (IN/OUT) - error handle. If there is an error, it is
                recorded in 'err' and this function returns OCI_ERROR.
                The error recorded in 'err' can be retrieved by calling
                OCIErrorGet().
        number1, number2 (IN) - numbers to be multiplied
        result (OUT) - multiplication result
   DESCRIPTION:
        Multiply 'number1' with 'number2' and return result in 'result'.
   RETURNS:
        OCI_SUCCESS if the function completes successfully.
        OCI_INVALID_HANDLE if 'err' is NULL.
        OCI_ERROR if
          any of the number arguments is null
 */

/*----------------------------- OCINumberDiv --------------------------------*/

sword OCINumberDiv(    OCIError *err, const OCINumber *number1, 
                       const OCINumber *number2, OCINumber *result    );
/*
   NAME: OCINumberDiv - OCINumber DIVide numbers
   PARAMETERS:
        err (IN/OUT) - error handle. If there is an error, it is
                recorded in 'err' and this function returns OCI_ERROR.
                The error recorded in 'err' can be retrieved by calling
                OCIErrorGet().
        number1 (IN) - pointer to the numerator
        number2 (IN) - pointer to the denominator
        result (OUT) - division result
   DESCRIPTION:
        Divide 'number1' by 'number2' and return result in 'result'.
   RETURNS:
        OCI_SUCCESS if the function completes successfully.
        OCI_INVALID_HANDLE if 'err' is NULL.
        OCI_ERROR if
          any of the number arguments is null
          underflow errorr
          overflow errorr
          divide by zero errorr
 */

/*----------------------------- OCINumberMod --------------------------------*/

sword OCINumberMod(    OCIError *err, const OCINumber *number1, 
                       const OCINumber *number2, OCINumber *result    );
/*
   NAME: OCINumberMod - OCINumber MODulous
   PARAMETERS:
        err (IN/OUT) - error handle. If there is an error, it is
                recorded in 'err' and this function returns OCI_ERROR.
                The error recorded in 'err' can be retrieved by calling
                OCIErrorGet().
        number1 (IN) - pointer to the numerator
        number2 (IN) - pointer to the denominator
        result (OUT) - remainder of the result
   DESCRIPTION:
        Finds the remainder of the division of two Oracle numbers.
   RETURNS:
        OCI_SUCCESS if the function completes successfully.
        OCI_INVALID_HANDLE if 'err' is NULL.
        OCI_ERROR if
          any of the number arguments is null
          divide by zero errorr
 */

/*------------------------ OCINumberIntPower --------------------------------*/

sword OCINumberIntPower(    OCIError *err, const OCINumber *base,
                            const sword exp, OCINumber *result    );
/*
   NAME: OCINumberIntPower - OCINumber takes an arbitary base to an arbitary
                             integer PoWeR
   PARAMETERS:
        err (IN/OUT) - error handle. If there is an error, it is
                recorded in 'err' and this function returns OCI_ERROR.
                The error recorded in 'err' can be retrieved by calling
                OCIErrorGet().
        base (IN) - base of the exponentiation
        exp (IN) - exponent to which the base is to be raised
        result (OUT) - output of exponentiation
   DESCRIPTION:
        Takes an arbitary base to an arbitary integer power.
   RETURNS:
        OCI_SUCCESS if the function completes successfully.
        OCI_INVALID_HANDLE if 'err' is NULL.
        OCI_ERROR if
          any of the number arguments is null
 */

/*-------------------------- OCINumberShift ---------------------------------*/

sword OCINumberShift(    OCIError *err, const OCINumber *number,
                            const sword nDig, OCINumber *result    );
/*
   NAME: OCINumberShift - OCINumber multiplies by a power of 10.

   PARAMETERS:
        err (IN/OUT) - error handle. If there is an error, it is
                recorded in 'err' and this function returns OCI_ERROR.
                The error recorded in 'err' can be retrieved by calling
                OCIErrorGet().
        number (IN) - Oracle Number to be shifted.
        nDig   (IN) - number of decimal places to shift.
        result (OUT) - shift result.
   DESCRIPTION:
        Multiplies number by 10^NDig and sets product to the result.
   RETURNS:
        OCI_SUCCESS if the function completes successfully.
        OCI_INVALID_HANDLE if 'err' is NULL.
        OCI_ERROR if
          any of the number arguments is null
 */

/*----------------------------- OCINumberNeg --------------------------------*/

sword OCINumberNeg(    OCIError *err, const OCINumber *number,
                       OCINumber *result    );
/*
   NAME: OCINumberNeg - OCINumber NEGate number
   PARAMETERS:
        err (IN/OUT) - error handle. If there is an error, it is
                recorded in 'err' and this function returns OCI_ERROR.
                The error recorded in 'err' can be retrieved by calling
                OCIErrorGet().
        number (IN) - number to be negated
        result (OUT) - will contain negated value of 'number'
   DESCRIPTION:
        Negates an Oracle number.
   RETURNS:
        OCI_SUCCESS if the function completes successfully.
        OCI_INVALID_HANDLE if 'err' is NULL.
        OCI_ERROR if
          any of the number arguments is null
 */

/*------------------------- OCINumberToText ---------------------------------*/

sword OCINumberToText(    OCIError *err, const OCINumber *number, 
                          const oratext *fmt, ub4 fmt_length,
                          const oratext *nls_params, ub4 nls_p_length,
                          ub4 *buf_size, oratext *buf    );
/*
   NAME: OCINumberToText - OCINumber convert number TO String 
   PARAMETERS:
        err (IN/OUT) - error handle. If there is an error, it is
                recorded in 'err' and this function returns OCI_ERROR.
                The error recorded in 'err' can be retrieved by calling
                OCIErrorGet().
        number (IN) - Oracle number to be converted
        fmt (IN) - conversion format
        fmt_length (IN) - length of the 'fmt' parameter
        nls_params (IN) - nls format specification, if null string 
                i.e. (oratext *)0, then the default parameters for the 
                session is used
        nls_p_length (IN) - length of the 'nls_params' parameter
        buf_size (IN/OUT) - size of the buffer must be passed as input by
                the caller, this function will return the length of the 
                resulting string in bytes via this parameter. The length 
                does not include the terminating null ('\0').
        buf (OUT) - buffer into which the converted string is placed. The
                resulting string is null terminated.
   DESCRIPTION:
        Converts the given number to a character string 
        according to the specified format. Refer to "TO_NUMBER" conversion
        function described in "Oracle SQL Language Reference Manual" for a
        description of format and NLS parameters.
        The converted number string is stored in the buffer 'buf', up to 
        a max of '*buf_size' bytes. Length of the resulting string is
        returned via 'buf_size'.
   RETURNS:
        OCI_SUCCESS if the function completes successfully.
        OCI_INVALID_HANDLE if 'err' is NULL.
        OCI_ERROR if
          'number' or 'buf' is null
          buffer too small
          invalid format
          invalid nls format
          number to text translation for the given format causes overflow
 */

/*-------------------------- OCINumberFromText ------------------------------*/

sword OCINumberFromText(    OCIError *err, const oratext *str, 
                          ub4 str_length, const oratext *fmt, ub4 fmt_length, 
                          const oratext *nls_params, ub4 nls_p_length, 
                          OCINumber *number    );
/*
   NAME: OCINumberFromText - OCINumber convert String TO Number
   PARAMETERS:
        err (IN/OUT) - error handle. If there is an error, it is
                recorded in 'err' and this function returns OCI_ERROR.
                The error recorded in 'err' can be retrieved by calling
                OCIErrorGet().
        str (IN) - input string to be converted to Oracle number
        str_length (IN) - size of the input string
        fmt (IN) - conversion format
        fmt_length (IN) - length of the 'fmt' parameter
        nls_params (IN) - nls format specification, if null string 
                i.e. (oratext *)0, then the default parameters for the 
                session is used
        nls_p_length (IN) - length of the 'nls_params' parameter
        number (OUT) - given string converted to number
   DESCRIPTION:
        Converts the given string to a number
        according to the specified format. Refer to "TO_NUMBER" conversion
        function described in "Oracle SQL Language Reference Manual" for a
        description of format and NLS parameters.
   RETURNS:
        OCI_SUCCESS if the function completes successfully.
        OCI_INVALID_HANDLE if 'err' is NULL.
        OCI_ERROR if
          'number' or 'str' is null
          'str_length' is 0
          invalid format
          invalid nls format
          invalid input string
 */

/*-------------------------- OCINumberToInt ---------------------------------*/

#define OCI_NUMBER_UNSIGNED 0                        /* Unsigned type -- ubX */
#define OCI_NUMBER_SIGNED   2                          /* Signed type -- sbX */

sword OCINumberToInt(    OCIError *err, const OCINumber *number,
                         uword rsl_length, uword rsl_flag, void  *rsl    );
/*
   NAME: OCINumberToInt - OCINumber convert number TO Integer
   PARAMETERS:
        err (IN/OUT) - error handle. If there is an error, it is
                recorded in 'err' and this function returns OCI_ERROR.
                The error recorded in 'err' can be retrieved by calling
                OCIErrorGet().
        number (IN) - number to be converted
        rsl_length (IN) - size of the desired result
        rsl_s_flag (IN) - flag denoting the desired sign of the output; valid 
                values are OCI_NUMBER_UNSIGNED, OCI_NUMBER_SIGNED
        rsl (OUT) - pointer to space for the result
   DESCRIPTION:
        Native type conversion function.
        Converts the given Oracle number into an xbx (e.g. ub2, ub4, sb2 etc.)
   RETURNS:
        OCI_SUCCESS if the function completes successfully.
        OCI_INVALID_HANDLE if 'err' is NULL.
        OCI_ERROR if
          'number' or 'rsl' is null
          integer value of 'number' is too big -- overflow
          integer value of 'number' is too small -- underflow
          invalid sign flag value ('rsl_s_flag')
 */

/*--------------------------- OCINumberFromInt ------------------------------*/

sword OCINumberFromInt(    OCIError *err, const void  *inum, uword inum_length,
                         uword inum_s_flag, OCINumber *number    );
/*
   NAME: OCINumberFromInt - OCINumber convert Integer TO Number 
   PARAMETERS:
        err (IN/OUT) - error handle. If there is an error, it is
                recorded in 'err' and this function returns OCI_ERROR.
                The error recorded in 'err' can be retrieved by calling
                OCIErrorGet().
        inum (IN) - pointer to the integer to be converted
        inum_length (IN) - size of the integer
        inum_s_flag (IN) - flag that designates the sign of the integer; valid 
                values are OCI_NUMBER_UNSIGNED, OCI_NUMBER_SIGNED
        number (OUT) - given integer converted to Oracle number
   DESCRIPTION:
        Native type conversion function. Converts any Oracle standard
        machine-native integer type (xbx) to an Oracle number.
   RETURNS:
        OCI_SUCCESS if the function completes successfully.
        OCI_INVALID_HANDLE if 'err' is NULL.
        OCI_ERROR if
          'number' or 'inum' is null
          integer too BIG -- the number is too large to fit into an Oracle
                number
          invalid sign flag value ('inum_s_flag')
 */

/*------------------------- OCINumberToReal ---------------------------------*/

sword OCINumberToReal(    OCIError *err, const OCINumber *number,
                          uword rsl_length, void  *rsl    );
/*
   NAME: OCINumberToReal - OCINumber convert number TO Real
   PARAMETERS:
        err (IN/OUT) - error handle. If there is an error, it is
                recorded in 'err' and this function returns OCI_ERROR.
                The error recorded in 'err' can be retrieved by calling
                OCIErrorGet().
        number (IN) - number to be converted
        rsl_length (IN) - is the size of the desired result, 
                sizeof( float | double | long double)
        rsl (OUT) - pointer to space for storing the result
   DESCRIPTION:
        Native type conversion function. Converts an Oracle number into a
        machine-native real type. This function only converts numbers up to
        LDBL_DIG, DBL_DIG, or FLT_DIG digits of precision and removes
        trailing zeroes. The above constants are defined in float.h
   RETURNS:
        OCI_SUCCESS if the function completes successfully.
        OCI_INVALID_HANDLE if 'err' is NULL.
        OCI_ERROR if
          'number' or 'rsl' is null
          'rsl_length' is 0
 */

/*------------------------- OCINumberToRealArray ----------------------------*/

sword OCINumberToRealArray(  OCIError *err, const OCINumber **number,
                             uword elems, uword rsl_length, void  *rsl    );
/*
   NAME: OCINumberToRealArray - OCINumber convert array of numbers TO Real
   PARAMETERS:
        err (IN/OUT) - error handle. If there is an error, it is
                recorded in 'err' and this function returns OCI_ERROR.
                The error recorded in 'err' can be retrieved by calling
                OCIErrorGet().
        number (IN) - Pointer to array of number to be converted
        elems (IN) - Upper bound of number array
        rsl_length (IN) - is the size of the desired result,
                                          sizeof( float | double | long double)
        rsl (OUT) - pointer to array of space for storing the result
   DESCRIPTION:
        Native type conversion function. Converts an Oracle number into a
        machine-native real type. This function only converts numbers up to
        LDBL_DIG, DBL_DIG, or FLT_DIG digits of precision and removes
        trailing zeroes. The above constants are defined in float.h
   RETURNS:
        OCI_SUCCESS if the function completes successfully.
        OCI_INVALID_HANDLE if 'err' is NULL.
        OCI_ERROR if
          'number' or 'rsl' is null
          'rsl_length' is 0
 */

/*-------------------------- OCINumberFromReal ------------------------------*/

sword OCINumberFromReal(    OCIError *err, const void  *rnum,
                            uword rnum_length, OCINumber *number    );
/*
   NAME: OCINumberFromReal - OCINumber convert Real TO Number 
   PARAMETERS:
        err (IN/OUT) - error handle. If there is an error, it is
                recorded in 'err' and this function returns OCI_ERROR.
                The error recorded in 'err' can be retrieved by calling
                OCIErrorGet().
        rnum (IN) - pointer to the floating point number to be converted
        rnum_length (IN) - size of the desired result, i.e.
                sizeof({float | double | long double})
        number (OUT) - given float converted to Oracle number
   DESCRIPTION:
        Native type conversion function. Converts a machine-native floating
        point type to an Oracle number.
   RETURNS:
        OCI_SUCCESS if the function completes successfully.
        OCI_INVALID_HANDLE if 'err' is NULL.
        OCI_ERROR if
          'number' or 'rnum' is null
          'rnum_length' is 0
 */

/*----------------------------- OCINumberCmp --------------------------------*/

sword OCINumberCmp(    OCIError *err, const OCINumber *number1, 
                       const OCINumber *number2, sword *result    );
/*
   NAME: OCINumberCmp - OCINumber CoMPare numbers 
   PARAMETERS:
        err (IN/OUT) - error handle. If there is an error, it is
                recorded in 'err' and this function returns OCI_ERROR.
                The error recorded in 'err' can be retrieved by calling
                OCIErrorGet().
        number1, number2 (IN) - numbers to be compared
        result (OUT) - 0 if equal, negative if number1 < number2, 
                positive if number1 > number2 
   DESCRIPTION:
        The function OCINumberCmp compares two numbers.
   RETURNS:
        OCI_SUCCESS if the function completes successfully.
        OCI_INVALID_HANDLE if 'err' is NULL.
        OCI_ERROR if
            'number1' or 'number2' or 'result' is null 
 */

/*---------------------------- OCINumberSign --------------------------------*/

sword OCINumberSign(    OCIError *err, const OCINumber *number,
                        sword *result    );
/*
   NAME: OCINumberSign - OCINumber obtains SiGN of an Oracle number
   PARAMETERS:
        err (IN/OUT) - error handle. If there is an error, it is
                recorded in 'err' and this function returns OCI_ERROR.
                The error recorded in 'err' can be retrieved by calling
                OCIErrorGet().
        number (IN) - number whose sign is returned
        result (OUT) - 0 if number == 0, -1 if number < 0, 
                1 if number > 0 
   DESCRIPTION:
        Obtains sign of an Oracle number
   RETURNS:
        OCI_SUCCESS if the function completes successfully.
        OCI_INVALID_HANDLE if 'err' is NULL.
        OCI_ERROR if
            'number' or 'result' is null
 */

/*---------------------------- OCINumberIsZero ------------------------------*/

sword OCINumberIsZero(    OCIError *err, const OCINumber *number,
                          boolean *result    );
/*
   NAME: OCINumberIsZero - OCINumber comparison with ZERo
   PARAMETERS:
        err (IN/OUT) - error handle. If there is an error, it is
                recorded in 'err' and this function returns OCI_ERROR.
                The error recorded in 'err' can be retrieved by calling
                OCIErrorGet().
        number (IN) - numbers to be compared
        result (OUT) - set to TRUE if equal to zero else FALSE
   DESCRIPTION:
        Test if the given number is equal to zero.
   RETURNS:
        OCI_SUCCESS if the function completes successfully.
        OCI_INVALID_HANDLE if 'err' is NULL.
        OCI_ERROR if
            'number' or 'result' is null
 */

/*---------------------------- OCINumberIsInt -------------------------------*/

sword OCINumberIsInt(    OCIError *err, const OCINumber *number,
                          boolean *result    );
/*
   NAME: OCINumberIsInt - OCINumber Is Integer value.
   PARAMETERS:
        err (IN/OUT) - error handle. If there is an error, it is
                recorded in 'err' and this function returns OCI_ERROR.
                The error recorded in 'err' can be retrieved by calling
                OCIErrorGet().
        number (IN) - number to be tested
        result (OUT) - set to TRUE if integer value else FALSE
   DESCRIPTION:
        Test if the given number is an integer value.
   RETURNS:
        OCI_SUCCESS if the function completes successfully.
        OCI_INVALID_HANDLE if 'err' is NULL.
        OCI_ERROR if
            'number' or 'result' is null
 */

/*-------------------------- OCINumberAssign --------------------------------*/
 
sword OCINumberAssign(    OCIError *err, const OCINumber *from,
                          OCINumber *to    );
/*
   NAME: OCINumberAssign - OCINumber ASsiGn number
   PARAMETERS:
        err (IN/OUT) - error handle. If there is an error, it is
                recorded in 'err' and this function returns OCI_ERROR.
                The error recorded in 'err' can be retrieved by calling
                OCIErrorGet().
        from (IN) - number to be assigned
        to (OUT) - number copied into
   DESCRIPTION:
        Assign number 'from' to 'to'.
   RETURNS:
        OCI_SUCCESS if the function completes successfully.
        OCI_INVALID_HANDLE if 'err' is NULL.
        OCI_ERROR if
          'from' or 'to' is null
 */

/*----------------------------- OCINumberAbs --------------------------------*/

sword OCINumberAbs(    OCIError *err, const OCINumber *number,
                       OCINumber *result    );
/*
   NAME: OCINumberAbs - OCINumber compute ABSolute value
   PARAMETERS:
        err (IN/OUT) - error handle. If there is an error, it is
                recorded in 'err' and this function returns OCI_ERROR.
                The error recorded in 'err' can be retrieved by calling
                OCIErrorGet().
        number (IN) - input number
        result (OUT) - output which will contain the absolue value of the
                input number
   DESCRIPTION:
        Computes the absolute value of an Oracle number.
   RETURNS:
        OCI_SUCCESS if the function completes successfully.
        OCI_INVALID_HANDLE if 'err' is NULL.
        OCI_ERROR if
          any of the number arguments is null
 */

/*---------------------------- OCINumberCeil --------------------------------*/

sword OCINumberCeil(    OCIError *err, const OCINumber *number,
                        OCINumber *result    );
/*
   NAME: OCINumberCeil - OCINumber compute the CEiL value of an Oracle number
   PARAMETERS:
        err (IN/OUT) - error handle. If there is an error, it is
                recorded in 'err' and this function returns OCI_ERROR.
                The error recorded in 'err' can be retrieved by calling
                OCIErrorGet().
        number (IN) - input number
        result (OUT) - output which will contain the ceil value of the
                input number
   DESCRIPTION:
        Computes the ceil value of an Oracle number.
   RETURNS:
        OCI_SUCCESS if the function completes successfully.
        OCI_INVALID_HANDLE if 'err' is NULL.
        OCI_ERROR if
          any of the number arguments is null
 */

/*--------------------------- OCINumberFloor --------------------------------*/

sword OCINumberFloor(    OCIError *err, const OCINumber *number, 
                         OCINumber *result    );
/*
   NAME: OCINumberFloor - OCINumber compute the FLooR value of an Oracle number
   PARAMETERS:
        err (IN/OUT) - error handle. If there is an error, it is
                recorded in 'err' and this function returns OCI_ERROR.
                The error recorded in 'err' can be retrieved by calling
                OCIErrorGet().
        number (IN) - input number
        result (OUT) - output which will contain the floor value of the
                input number
   DESCRIPTION:
        Computes the floor value of an Oracle number.
   RETURNS:
        OCI_SUCCESS if the function completes successfully.
        OCI_INVALID_HANDLE if 'err' is NULL.
        OCI_ERROR if
          any of the number arguments is null
 */

/*----------------------------- OCINumberSqrt -------------------------------*/

sword OCINumberSqrt(    OCIError *err, const OCINumber *number, 
                       OCINumber *result    );
/*
   NAME: OCINumberSqrt - OCINumber compute the SQuare Root of an Oracle number
   PARAMETERS:
        err (IN/OUT) - error handle. If there is an error, it is
                recorded in 'err' and this function returns OCI_ERROR.
                The error recorded in 'err' can be retrieved by calling
                OCIErrorGet().
        number (IN) - input number
        result (OUT) - output which will contain the square root of the
                input number
   DESCRIPTION:
        Computes the square root of an Oracle number.
   RETURNS:
        OCI_SUCCESS if the function completes successfully.
        OCI_INVALID_HANDLE if 'err' is NULL.
        OCI_ERROR if
          any of the number arguments is null
          'number' is negative
 */

/*--------------------------- OCINumberTrunc --------------------------------*/

sword OCINumberTrunc(    OCIError *err, const OCINumber *number, 
                         sword decplace, OCINumber *result    );
/*
   NAME: OCINumberTrunc - OCINumber TRUncate an Oracle number at a 
                          specified decimal place
   PARAMETERS:
        err (IN/OUT) - error handle. If there is an error, it is
                recorded in 'err' and this function returns OCI_ERROR.
                The error recorded in 'err' can be retrieved by calling
                OCIErrorGet().
        number (IN) - input number
        decplace (IN) - number of decimal digits to the right of the 
                decimal point to truncate at. Negative values are allowed.
        result (OUT) - output of truncation
   DESCRIPTION:
        Truncate an Oracle number at a specified decimal place
   RETURNS:
        OCI_SUCCESS if the function completes successfully.
        OCI_INVALID_HANDLE if 'err' is NULL.
        OCI_ERROR if
          any of the number arguments is null
 */

/*----------------------------- OCINumberPower ------------------------------*/

sword OCINumberPower(    OCIError *err, const OCINumber *base, 
                         const OCINumber *number, OCINumber *result    );
/*
   NAME: OCINumberPower - OCINumber takes an arbitary Base to an 
                          arbitary Power
   PARAMETERS:
        err (IN/OUT) - error handle. If there is an error, it is
                recorded in 'err' and this function returns OCI_ERROR.
                The error recorded in 'err' can be retrieved by calling
                OCIErrorGet().
        base (IN) - base of the exponentiation
        number (IN) - exponent to which the base is to be raised
        result (OUT) - output of exponentiation
   DESCRIPTION:
        Takes an arbitary base to an arbitary power.
   RETURNS:
        OCI_SUCCESS if the function completes successfully.
        OCI_INVALID_HANDLE if 'err' is NULL.
        OCI_ERROR if
          any of the number arguments is null
 */

/*--------------------------- OCINumberRound --------------------------------*/

sword OCINumberRound(    OCIError *err, const OCINumber *number,
                         sword decplace, OCINumber *result    );
/*
   NAME: OCINumberRound - OCINumber ROUnds an Oracle number to a specified 
                  decimal place
   PARAMETERS:
        err (IN/OUT) - error handle. If there is an error, it is
                recorded in 'err' and this function returns OCI_ERROR.
                The error recorded in 'err' can be retrieved by calling
                OCIErrorGet().
        number (IN) - round this number and return result in 'result'
        decplace (IN) - number of decimal digits to the right of the 
                decimal point to round to. Negative values are allowed.
        result (OUT) - output of rounding
   DESCRIPTION:
        Rounds an Oracle number to a specified decimal place
   RETURNS:
        OCI_SUCCESS if the function completes successfully.
        OCI_INVALID_HANDLE if 'err' is NULL.
        OCI_ERROR if
          any of the number arguments is null
 */

/*--------------------------- OCINumberPrec ---------------------------------*/

sword OCINumberPrec(    OCIError *err, const OCINumber *number,
                         sword nDigs, OCINumber *result    );
/*
   NAME: OCINumberPrec - Rounds an Oracle number to a specified number of
                         decimal digits.
   PARAMETERS:
        err (IN/OUT) - error handle. If there is an error, it is
                recorded in 'err' and this function returns OCI_ERROR.
                The error recorded in 'err' can be retrieved by calling
                OCIErrorGet().
        number (IN) - number for which to set precision.
        nDig   (IN) - number of decimal digits desired in the result.
        result (OUT) - result.
   DESCRIPTION:
        Performs a floating point round with respect to the number
        of digits.
   RETURNS:
        OCI_SUCCESS if the function completes successfully.
        OCI_INVALID_HANDLE if 'err' is NULL.
        OCI_ERROR if
          any of the number arguments is null
 */

/*----------------------------- OCINumberSin --------------------------------*/

sword OCINumberSin(    OCIError *err, const OCINumber *number,
                       OCINumber *result    );
/*
   NAME: OCINumberSin - OCINumber takes the SINe of an Oracle number
   PARAMETERS:
        err (IN/OUT) - error handle. If there is an error, it is
                recorded in 'err' and this function returns OCI_ERROR.
                The error recorded in 'err' can be retrieved by calling
                OCIErrorGet().
        number (IN) - argument of the sine in radians
        result (OUT) - result of the sine
   DESCRIPTION:
        Takes the sine in radians of an Oracle number.
   RETURNS:
        OCI_SUCCESS if the function completes successfully.
        OCI_INVALID_HANDLE if 'err' is NULL.
        OCI_ERROR if
          any of the number arguments is null
 */

/*-------------------------- OCINumberArcSin --------------------------------*/

sword OCINumberArcSin(    OCIError *err, const OCINumber *number,
                          OCINumber *result    );
/*
   NAME: OCINumberArcSin - OCINumber takes the Arc SINe of an Oracle number
   PARAMETERS:
        err (IN/OUT) - error handle. If there is an error, it is
                recorded in 'err' and this function returns OCI_ERROR.
                The error recorded in 'err' can be retrieved by calling
                OCIErrorGet().
        number (IN) - argument of the arc sine
        result (OUT) - result of the arc sine in radians
   DESCRIPTION:
        Takes the arc sine in radians of an Oracle number.
   RETURNS:
        OCI_SUCCESS if the function completes successfully.
        OCI_INVALID_HANDLE if 'err' is NULL.
        OCI_ERROR if
          any of the number arguments is null
          'number' is < -1 or 'number' is > 1.
 */

/*-------------------------- OCINumberHypSin --------------------------------*/

sword OCINumberHypSin(    OCIError *err, const OCINumber *number, 
                          OCINumber *result    );
/*
   NAME: OCINumberHypSin - OCINumber takes the SiNe Hyperbolic of an 
                           Oracle number
   PARAMETERS:
        err (IN/OUT) - error handle. If there is an error, it is
                recorded in 'err' and this function returns OCI_ERROR.
                The error recorded in 'err' can be retrieved by calling
                OCIErrorGet().
        number (IN) - argument of the sine hyperbolic
        result (OUT) - result of the sine hyperbolic
   DESCRIPTION:
        Takes the hyperbolic sine of an Oracle number.
   RETURNS:
        OCI_SUCCESS if the function completes successfully.
        OCI_INVALID_HANDLE if 'err' is NULL.
        OCI_ERROR if
          any of the number arguments is null
   NOTES:
        An Oracle number overflow causes an unpredictable result value.
 */

/*----------------------------- OCINumberCos --------------------------------*/

sword OCINumberCos(    OCIError *err, const OCINumber *number, 
                       OCINumber *result    );
/*
   NAME: OCINumberCos - OCINumber takes the COSine of an Oracle number
   PARAMETERS:
        err (IN/OUT) - error handle. If there is an error, it is
                recorded in 'err' and this function returns OCI_ERROR.
                The error recorded in 'err' can be retrieved by calling
                OCIErrorGet().
        number (IN) - argument of the cosine in radians
        result (OUT) - result of the cosine
   DESCRIPTION:
        Takes the cosine in radians of an Oracle number.
   RETURNS:
        OCI_SUCCESS if the function completes successfully.
        OCI_INVALID_HANDLE if 'err' is NULL.
        OCI_ERROR if
          any of the number arguments is null
 */

/*-------------------------- OCINumberArcCos --------------------------------*/

sword OCINumberArcCos(    OCIError *err, const OCINumber *number, 
                          OCINumber *result    );
/*
   NAME: OCINumberArcCos - OCINumber takes the Arc COSine of an Oracle number
   PARAMETERS:
        err (IN/OUT) - error handle. If there is an error, it is
                recorded in 'err' and this function returns OCI_ERROR.
                The error recorded in 'err' can be retrieved by calling
                OCIErrorGet().
        number (IN) - argument of the arc cosine
        result (OUT) - result of the arc cosine in radians
   DESCRIPTION:
        Takes the arc cosine in radians of an Oracle number.
   RETURNS:
        OCI_SUCCESS if the function completes successfully.
        OCI_INVALID_HANDLE if 'err' is NULL.
        OCI_ERROR if
          any of the number arguments is null
          'number' is < -1 or 'number' is > 1.
 */

/*-------------------------- OCINumberHypCos --------------------------------*/

sword OCINumberHypCos(    OCIError *err, const OCINumber *number, 
                          OCINumber *result    );
/*
   NAME: OCINumberHypCos - OCINumber takes the CoSine Hyperbolic of an 
                           Oracle number
   PARAMETERS:
        err (IN/OUT) - error handle. If there is an error, it is
                recorded in 'err' and this function returns OCI_ERROR.
                The error recorded in 'err' can be retrieved by calling
                OCIErrorGet().
        number (IN) - argument of the cosine hyperbolic
        result (OUT) - result of the cosine hyperbolic
   DESCRIPTION:
        Takes the hyperbolic cosine of an Oracle number.
   RETURNS:
        OCI_SUCCESS if the function completes successfully.
        OCI_INVALID_HANDLE if 'err' is NULL.
        OCI_ERROR if
          any of the number arguments is null
   NOTES:
        An Oracle number overflow causes an unpredictable result value.
 */

/*----------------------------- OCINumberTan --------------------------------*/

sword OCINumberTan(    OCIError *err, const OCINumber *number, 
                       OCINumber *result    );
/*
   NAME: OCINumberTan - OCINumber takes the TANgent of an Oracle number
   PARAMETERS:
        err (IN/OUT) - error handle. If there is an error, it is
                recorded in 'err' and this function returns OCI_ERROR.
                The error recorded in 'err' can be retrieved by calling
                OCIErrorGet().
        number (IN) - argument of the tangent in radians
        result (OUT) - result of the tangent
   DESCRIPTION:
        Takes the tangent in radians of an Oracle number.
   RETURNS:
        OCI_SUCCESS if the function completes successfully.
        OCI_INVALID_HANDLE if 'err' is NULL.
        OCI_ERROR if
          any of the number arguments is null
 */

/*-------------------------- OCINumberArcTan --------------------------------*/

sword OCINumberArcTan(    OCIError *err, const OCINumber *number, 
                          OCINumber *result    );
/*
   NAME: OCINumberArcTan - OCINumber takes the Arc TANgent of an Oracle number
   PARAMETERS:
        err (IN/OUT) - error handle. If there is an error, it is
                recorded in 'err' and this function returns OCI_ERROR.
                The error recorded in 'err' can be retrieved by calling
                OCIErrorGet().
        number (IN) - argument of the arc tangent
        result (OUT) - result of the arc tangent in radians
   DESCRIPTION:
        Takes the arc tangent in radians of an Oracle number.
   RETURNS:
        OCI_SUCCESS if the function completes successfully.
        OCI_INVALID_HANDLE if 'err' is NULL.
        OCI_ERROR if
          any of the number arguments is null
 */

/*------------------------ OCINumberArcTan2 ---------------------------------*/

sword OCINumberArcTan2(    OCIError *err, const OCINumber *number1, 
                           const OCINumber *number2, OCINumber *result    );
/*
   NAME: OCINumberArcTan2 - OCINumber takes the ATan2 of 2 Oracle numbers
   PARAMETERS:
        err (IN/OUT) - error handle. If there is an error, it is
                recorded in 'err' and this function returns OCI_ERROR.
                The error recorded in 'err' can be retrieved by calling
                OCIErrorGet().
        number1 (IN) - first argument of atan2(y,x) function which 
                corresponds to 'y' parameter in the function
        number2 (IN) - second argument of atan2(y,x) function which 
                corresponds to 'x' parameter in the function
        result (OUT) - result of the atan2() in radians
   DESCRIPTION:
        Takes the atan2(number1, number2).
   RETURNS:
        OCI_SUCCESS if the function completes successfully.
        OCI_INVALID_HANDLE if 'err' is NULL.
        OCI_ERROR if
          any of the number arguments is null
          'number2' is 0
 */

/*----------------------------- OCINumberHypTan -----------------------------*/

sword OCINumberHypTan(    OCIError *err, const OCINumber *number, 
                          OCINumber *result    );
/*
   NAME: OCINumberHypTan - OCINumber takes the TaNgent Hyperbolic of an Oracle
                           number
   PARAMETERS:
        err (IN/OUT) - error handle. If there is an error, it is
                recorded in 'err' and this function returns OCI_ERROR.
                The error recorded in 'err' can be retrieved by calling
                OCIErrorGet().
        number (IN) - argument of the tangent hyperbolic
        result (OUT) - result of the tangent hyperbolic
   DESCRIPTION:
        Takes the hyperbolic tangent of an Oracle number.
   RETURNS:
        OCI_SUCCESS if the function completes successfully.
        OCI_INVALID_HANDLE if 'err' is NULL.
        OCI_ERROR if
          any of the number arguments is null
   NOTES:
        An Oracle number overflow causes an unpredictable result value.
 */

/*--------------------------- OCINumberExp ----------------------------------*/

sword OCINumberExp(    OCIError *err, const OCINumber *number, 
                       OCINumber *result    );
/*
   NAME: OCINumberExp - OCINumber EXPonential
   PARAMETERS:
        err (IN/OUT) - error handle. If there is an error, it is
                recorded in 'err' and this function returns OCI_ERROR.
                The error recorded in 'err' can be retrieved by calling
                OCIErrorGet().
        number (IN) - e raised to this Oracle number power
        result (OUT) - output of exponentiation
   DESCRIPTION:
        Raises e to the specified Oracle number power
   RETURNS:
        OCI_SUCCESS if the function completes successfully.
        OCI_INVALID_HANDLE if 'err' is NULL.
        OCI_ERROR if
          any of the number arguments is null
 */

/*----------------------------- OCINumberLn ---------------------------------*/

sword OCINumberLn(    OCIError *err, const OCINumber *number, 
                      OCINumber *result    );
/*
   NAME: OCINumberLn - OCINumber Logarithm Natural
   PARAMETERS:
        err (IN/OUT) - error handle. If there is an error, it is
                recorded in 'err' and this function returns OCI_ERROR.
                The error recorded in 'err' can be retrieved by calling
                OCIErrorGet().
        number (IN) - logarithm of this number is computed
        result (OUT) - logarithm result
   DESCRIPTION:
        Takes the logarithm of the given Oracle number with respect
        to the given base.
   RETURNS:
        OCI_SUCCESS if the function completes successfully.
        OCI_INVALID_HANDLE if 'err' is NULL.
        OCI_ERROR if
          any of the number arguments is null
          'number' is <= 0
 */

/*----------------------------- OCINumberLog --------------------------------*/

sword OCINumberLog(    OCIError *err, const OCINumber *base, 
                       const OCINumber *number, OCINumber *result    );
/*
   NAME: OCINumberLog - OCINumber LOGarithm any base
   PARAMETERS:
        err (IN/OUT) - error handle. If there is an error, it is
                recorded in 'err' and this function returns OCI_ERROR.
                The error recorded in 'err' can be retrieved by calling
                OCIErrorGet().
        base (IN) - base of the logarithm
        number (IN) - opearnd
        result (OUT) - logarithm result
   DESCRIPTION:
        Takes the logarithm with the specified base of an Oracle number.
   RETURNS:
        OCI_SUCCESS if the function completes successfully.
        OCI_INVALID_HANDLE if 'err' is NULL.
        OCI_ERROR if
          any of the number arguments is null
          'number' is <= 0
          'base' is <= 0
 */

/*****************************************************************************/
/*                             ORACLE DATE TYPE                              */
/*****************************************************************************/
 
struct OCITime
{
  ub1 OCITimeHH;                          /* hours; range is 0 <= hours <=23 */
  ub1 OCITimeMI;                     /* minutes; range is 0 <= minutes <= 59 */
  ub1 OCITimeSS;                     /* seconds; range is 0 <= seconds <= 59 */
};
typedef struct OCITime OCITime;

/* 
 * OCITime - OCI TiMe portion of date
 *
 * This structure should be treated as an opaque structure as the format
 * of this structure may change. Use OCIDateGetTime/OCIDateSetTime 
 * to manipulate time portion of OCIDate.
 */

struct OCIDate
{
  sb2 OCIDateYYYY;         /* gregorian year; range is -4712 <= year <= 9999 */
  ub1 OCIDateMM;                          /* month; range is 1 <= month < 12 */
  ub1 OCIDateDD;                             /* day; range is 1 <= day <= 31 */
  OCITime OCIDateTime;                                               /* time */
};
typedef struct OCIDate OCIDate;

/* 
 * OCIDate - OCI oracle Date representation in C
 *
 * OCIDate represents the C mapping of Oracle date.
 *
 * This structure should be treated as an opaque structure as the format
 * of this structure may change. Use OCIDateGetDate/OCIDateSetDate
 * to access/initialize OCIDate.
 *
 * For binding variables of type OCIDate in OCI calls (OCIBindByName(),
 * OCIBindByPos(), and OCIDefineByPos()) use the type code SQLT_ODT.
 */

/*
   EXAMPLE 

   The following example shows how to manipulate an attribute of type 
   oracle date.
  
     #define FMT "Month dd, YYYY, HH:MI A.M."
     #define LANG "American"

     struct person
     {
       OCIDate start_date;
     };
     typedef struct person person;

     OCIError *err;
     person *joe;
     sword status;                                           /o error status o/
     
     /o See oci.h for an example of how to initialize OCIError.
      o For this example, assume the OCIEnv and OCIError has been
      o initialized.
      o/

     /o Pin joe person object in the object cache. See ori.h
      o for an example on pinning objects. For this example, assume that
      o joe is pointing to the pinned object.
      o/

     /o set the start date of joe o/
     OCIDateSetTime(&joe->start_date, 8, 0, 0);
     OCIDateSetDate(&joe->start_date, 1990, 10, 5); 

     /o check if the date is valid o/
     uword invalid;
     if (OCIDateCheck(err, &joe->start_date, &invalid) != OCI_SUCCESS)
       /o error handling code o/
     if (invalid)
       /o error handling code o/

     /o convert date for display purposes o/
     char str[100];
     ub4 strlen = sizeof(str);
     if (OCIDateToText(err, &joe->start_date, FMT, sizeof(FMT)-1, LANG,
                sizeof(LANG)-1, &strlen, str) != OCI_SUCCESS)
       /o error handling code o/

 */

/*--------------------------- OCIDateGetTime --------------------------------*/
/* void OCIDateGetTime(/o_ const OCIDate *date, ub1 *hour, ub1 *min, 
                           ub1 *sec _o/); */
#define OCIDateGetTime(date, hour, min, sec) \
  { \
     *hour = (date)->OCIDateTime.OCITimeHH; \
     *min = (date)->OCIDateTime.OCITimeMI; \
     *sec = (date)->OCIDateTime.OCITimeSS; \
  }
/*
   NAME: OCIDateGetTime - OCIDate Get Time portion of date
   PARAMETERS:
        date (IN) - Oracle date whose time data is retrieved
        hour (OUT) - hour value returned
        min (OUT) - minute value returned
        sec (OUT) - second value returned
   DESCRIPTION:
        Return time inforamtion stored in the given date. The time
        information returned is: hour, minute and seconds.
   RETURNS:
        NONE
 */

/*--------------------------- OCIDateGetDate --------------------------------*/
/* void OCIDateGetDate(/o_ const OCIDate *date, sb2 *year, ub1 *month, 
                           ub1 *day _o/); */
#define OCIDateGetDate(date, year, month, day) \
  { \
     *year = (date)->OCIDateYYYY; \
     *month = (date)->OCIDateMM; \
     *day = (date)->OCIDateDD; \
  }
/*
   NAME: OCIDateGetDate - OCIDate Get Date (year, month, day) portion of date
   PARAMETERS:
        date (IN) - Oracle date whose year, month, day data is retrieved
        year (OUT) - year value returned
        month (OUT) - month value returned
        day (OUT) - day value returned
   DESCRIPTION:
        Return year, month, day inforamtion stored in the given date.
   RETURNS:
        NONE
 */

/*--------------------------- OCIDateSetTime --------------------------------*/
/* void OCIDateSetTime(/o_ OCIDate *date, ub1 hour, ub1 min, 
                           ub1 sec _o/); */
#define OCIDateSetTime(date, hour, min, sec) \
  { \
     (date)->OCIDateTime.OCITimeHH = hour; \
     (date)->OCIDateTime.OCITimeMI = min; \
     (date)->OCIDateTime.OCITimeSS = sec; \
  }
/*
   NAME: OCIDateSetTime - OCIDate Set Time portion of date
   PARAMETERS:
        date (OUT) - Oracle date whose time data is set
        hour (IN) - hour value to be set 
        min (IN) - minute value to be set
        sec (IN) - second value to be set
   DESCRIPTION:
        Set the date with the given time inforamtion.
   RETURNS:
        NONE
 */

/*--------------------------- OCIDateSetDate --------------------------------*/
/* void OCIDateSetDate(/o_ OCIDate *date, sb2 year, ub1 month, ub1 day _o/); */
#define OCIDateSetDate(date, year, month, day) \
  { \
     (date)->OCIDateYYYY = year; \
     (date)->OCIDateMM = month; \
     (date)->OCIDateDD = day; \
  }
/*
   NAME: OCIDateSetDate - OCIDate Set Date (year, month, day) portion of date
   PARAMETERS:
        date (IN) - Oracle date whose year, month, day data is set
        year (OUT) - year value to be set
        month (OUT) - month value to be set
        day (OUT) - day value to be set
   DESCRIPTION:
        Set the date with the given year, month, day inforamtion.
   RETURNS:
        NONE
 */

/*--------------------------- OCIDateAssign ---------------------------------*/

sword OCIDateAssign(    OCIError *err, const OCIDate *from, 
                        OCIDate *to    );
/*
   NAME: OCIDateAssign - OCIDate Assignment
   PARAMETERS:
        err (IN/OUT) - error handle. If there is an error, it is
                recorded in 'err' and this function returns OCI_ERROR.
                The error recorded in 'err' can be retrieved by calling
                OCIErrorGet().
        from (IN) - date to be assigned
        to (OUT) - lhs of assignment
   DESCRIPTION:
        Performs date assignment.
   RETURNS:
        OCI_SUCCESS
 */

/*--------------------------- OCIDateToText ---------------------------------*/

sword OCIDateToText(    OCIError *err, const OCIDate *date, 
                        const oratext *fmt, ub1 fmt_length, 
                        const oratext *lang_name, ub4 lang_length, 
                        ub4 *buf_size, oratext *buf    );
/*
   NAME: OCIDateToText - OCIDate convert date TO String 
   PARAMETERS:
        err (IN/OUT) - error handle. If there is an error, it is
                recorded in 'err' and this function returns OCI_ERROR.
                The error recorded in 'err' can be retrieved by calling
                OCIErrorGet().
        date (IN) - Oracle date to be converted
        fmt (IN) - conversion format, if null string pointer (oratext*)0, then
                the date is converted to a character string in the
                date format "DD-MON-YY".
        fmt_length (IN) - length of the 'fmt' parameter
        lang_name (IN) - specifies the language in which the names and 
                abbreviations of months and days are returned;
                default language of session is used if 'lang_name' 
                is null i.e. (oratext *)0
        lang_length (IN) - length of the 'nls_params' parameter
        buf_size (IN/OUT) - size of the buffer; size of the resulting string
                is returned via this parameter
        buf (OUT) - buffer into which the converted string is placed
   DESCRIPTION:
        Converts the given date to a string according to the specified format.
        Refer to "TO_DATE" conversion function described in
        "Oracle SQL Language Reference Manual" for a description of format
        and NLS arguments. The converted null-terminated date string is
        stored in the buffer 'buf'.
 
        An error is reported upon overflow, e.g. trying to convert a number
        of value 10 using format '9' causes an overflow.
   RETURNS:
        OCI_SUCCESS if the function completes successfully.
        OCI_INVALID_HANDLE if 'err' is NULL.
        OCI_ERROR if
          buffer too small
          invalid format
          unknown language
          overflow error
 */

/*---------------------------- OCIDateFromText ------------------------------*/

sword OCIDateFromText(    OCIError *err, const oratext *date_str, 
                        ub4 d_str_length, const oratext *fmt, ub1 fmt_length, 
                        const oratext *lang_name, ub4 lang_length, 
                        OCIDate *date    );
/*
   NAME: OCIDateFromText - OCIDate convert String TO Date
   PARAMETERS:
        err (IN/OUT) - error handle. If there is an error, it is
                recorded in 'err' and this function returns OCI_ERROR.
                The error recorded in 'err' can be retrieved by calling
                OCIErrorGet().
        date_str (IN) - input string to be converted to Oracle date
        d_str_length (IN) - size of the input string, if the length is -1
                then 'date_str' is treated as a null terminated  string
        fmt (IN) - conversion format; if 'fmt' is a null pointer, then
                the string is expected to be in 'DD-MON-YY' format.
        fmt_length (IN) - length of the 'fmt' parameter
        lang_name (IN) - language in which the names and abbreviations of
                days and months are specified, if null i.e. (oratext *)0,
                the default language of session is used, 
        lang_length (IN) - length of the 'lang_name' parameter
        date (OUT) - given string converted to date
   DESCRIPTION:
        Converts the given string to Oracle date
        according to the specified format. Refer to "TO_DATE" conversion
        function described in "Oracle SQL Language Reference Manual" for a
        description of format.
   RETURNS:
        OCI_SUCCESS if the function completes successfully.
        OCI_INVALID_HANDLE if 'err' is NULL.
        OCI_ERROR if
          invalid format
          unknown language
          invalid input string
          <to be discovered>
 */

/*----------------------------- OCIDateCompare ------------------------------*/

sword OCIDateCompare(    OCIError *err, const OCIDate *date1, 
                     const OCIDate *date2, sword *result    );
/*
   NAME: OCIDateCompare - OCIDate CoMPare dates
   PARAMETERS:
        err (IN/OUT) - error handle. If there is an error, it is
                recorded in 'err' and this function returns OCI_ERROR.
                The error recorded in 'err' can be retrieved by calling
                OCIErrorGet().
        date1, date2 (IN) - dates to be compared
        result (OUT) - comparison result, 0 if equal, -1 if date1 < date2, 
                1 if date1 > date2
   DESCRIPTION:
        The function OCIDateCompare compares two dates. It returns -1 if date1 
        is smaller than date2, 0 if they are equal, and 1 if date1 is greater 
        than date2.
   RETURNS:
        OCI_SUCCESS if the function completes successfully.
        OCI_INVALID_HANDLE if 'err' is NULL.
        OCI_ERROR if
          invalid date
          <to be discovered>
 */

/*------------------------- OCIDateAddMonths --------------------------------*/

sword OCIDateAddMonths(    OCIError *err, const OCIDate *date, sb4 num_months,
                           OCIDate *result    );
/*
   NAME: OCIDateAddMonths - OCIDate ADd or subtract Months
   PARAMETERS:
        err (IN/OUT) - error handle. If there is an error, it is
                recorded in 'err' and this function returns OCI_ERROR.
                The error recorded in 'err' can be retrieved by calling
                OCIErrorGet().
        date (IN) - 'num_months' added or subtracted from 'date'
        num_months (IN) - number of months to be added or subtracted 
                (a negative value will be subtracted)
        result (IN/OUT) - result of adding or subtracting to 'date'
   DESCRIPTION:
        The function OCIDateAddDays adds or subtracts num_months from the 
        date 'date'.
          If the input 'date' is the last day of a month, then 
        appropriate adjustments are made to ensure that the output date is 
        also the last day of the month. For example, Feb. 28 + 1 month =
        March 31, and November 30 - 3 months = August 31. Otherwise the
        'result' date has the same day component as 'date'. 
   RETURNS:
        OCI_SUCCESS if the function completes successfully.
        OCI_INVALID_HANDLE if 'err' is NULL.
        OCI_ERROR if
          invalid date
          <to be discovered>
 */

/*--------------------------- OCIDateAddDays --------------------------------*/

sword OCIDateAddDays(    OCIError *err, const OCIDate *date, sb4 num_days,
                         OCIDate *result    );
/*
   NAME: OCIDateAddDays - OCIDate ADd or subtract Days
   PARAMETERS:
        err (IN/OUT) - error handle. If there is an error, it is
                recorded in 'err' and this function returns OCI_ERROR.
                The error recorded in 'err' can be retrieved by calling
                OCIErrorGet().
        date (IN) - 'num_days' added or subtracted from 'date'
        num_days (IN) - number of days to be added or subtracted 
                (a negative value will be subtracted)
        result (IN/OUT) - result of adding or subtracting to 'date'
   DESCRIPTION:
        The function OCIDateAddDays adds or subtracts num_days from the 
        date 'date'.
   RETURNS:
        OCI_SUCCESS if the function completes successfully.
        OCI_INVALID_HANDLE if 'err' is NULL.
        OCI_ERROR if
          invalid date
          <to be discovered>
 */

/*--------------------------- OCIDateLastDay --------------------------------*/

sword OCIDateLastDay(    OCIError *err, const OCIDate *date, 
                         OCIDate *last_day    );
/*
   NAME: OCIDateLastDay - OCIDate get date of the LaST day of the month
   PARAMETERS:
        err (IN/OUT) - error handle. If there is an error, it is
                recorded in 'err' and this function returns OCI_ERROR.
                The error recorded in 'err' can be retrieved by calling
                OCIErrorGet().
        date (IN) - input date
        last_day (OUT) - last day of the month in date 'date'
   DESCRIPTION:
        The function OCIDateLastDay returns the date of the last day of the
        month in date 'date'.
   RETURNS:
        OCI_SUCCESS if the function completes successfully.
        OCI_INVALID_HANDLE if 'err' is NULL.
        OCI_ERROR if
          invalid date
          <to be discovered>
 */

/*----------------------- OCIDateDaysBetween --------------------------------*/

sword OCIDateDaysBetween(    OCIError *err, const OCIDate *date1, 
                             const OCIDate *date2, sb4 *num_days    );
/*
   NAME: OCIDateDaysBetween - OCIDate get number of days BeTWeen two dates
   PARAMETERS:
        err (IN/OUT) - error handle. If there is an error, it is
                recorded in 'err' and this function returns OCI_ERROR.
                The error recorded in 'err' can be retrieved by calling
                OCIErrorGet().
        date1, date2 (IN) - input dates
        num_days (OUT) - number of days between date1 and date2
   DESCRIPTION:
        The function OCIDateDaysBetween returns the number of days between 
        date1 and date2. The time is ignored in this computation.
   RETURNS:
        OCI_SUCCESS if the function completes successfully.
        OCI_INVALID_HANDLE if 'err' is NULL.
        OCI_ERROR if
          invalid date
          <to be discovered>
 */

/*------------------------ OCIDateZoneToZone --------------------------------*/

sword OCIDateZoneToZone(    OCIError *err, const OCIDate *date1,
                            const oratext *zon1,
                            ub4 zon1_length, const oratext *zon2, 
                            ub4 zon2_length, OCIDate *date2    );
/*
   NAME: OCIDateZoneToZone - OCIDate convert date from one Zone TO another Zone
   PARAMETERS:
        err (IN/OUT) - error handle. If there is an error, it is
                recorded in 'err' and this function returns OCI_ERROR.
                The error recorded in 'err' can be retrieved by calling
                OCIErrorGet().
        date1 (IN) - date to be converted
        zon1 (IN) - zone of input date
        zon1_length (IN) - length in bytes of string 'zon1'
        zon2 (IN) - zone to be converted to
        zon2_length (IN) - length in bytes of string 'zon2'
        date2 (OUT) - converted date (in 'zon2')
   DESCRIPTION:
        Converts date from one time zone to another. Given date 'date1' 
        in time zone 'zon1' returns date 'date2' in time zone 'zon2'.
   RETURNS:
        OCI_SUCCESS if the function completes successfully.
        OCI_INVALID_HANDLE if 'err' is NULL.
        OCI_ERROR if
          invlid date
          invald input time zone
          invald output time zone
          <to be discovered>
 */

/*--------------------------- OCIDateNextDay --------------------------------*/

sword OCIDateNextDay(    OCIError *err, const OCIDate *date, 
                         const oratext *day_p, ub4 day_length, 
                         OCIDate *next_day    );
/*
   NAME: OCIDateNextDay - OCIDate get date of Next DaY
   PARAMETERS:
        err (IN/OUT) - error handle. If there is an error, it is
                recorded in 'err' and this function returns OCI_ERROR.
                The error recorded in 'err' can be retrieved by calling
                OCIErrorGet().
        date (IN) - returned date should be later than this date
        day (IN) - first day of week named by this is returned
        day_length (IN) - length in bytes of string 'day'
        next_day (OUT) - first day of the week named by 'day' later than 'date'
   DESCRIPTION:
        Returns the date of the first day of the
        week named by 'day' that is later than date 'date'.
   RETURNS:
        OCI_SUCCESS if the function completes successfully.
        OCI_INVALID_HANDLE if 'err' is NULL.
        OCI_ERROR if
          invalid date
          invalid day
          <to be discovered>
 */

/*----------------------------- OCIDateCheck --------------------------------*/

/* Listing of error bits used by OCIDateCheck() */
#define OCI_DATE_INVALID_DAY            0x1                       /* Bad DAy */
#define OCI_DATE_DAY_BELOW_VALID        0x2   /* Bad DAy Low/high bit (1=low)*/
#define OCI_DATE_INVALID_MONTH          0x4                     /* Bad MOnth */
#define OCI_DATE_MONTH_BELOW_VALID      0x8 /* Bad MOnth Low/high bit (1=low)*/
#define OCI_DATE_INVALID_YEAR           0x10                     /* Bad YeaR */
#define OCI_DATE_YEAR_BELOW_VALID       0x20 /* Bad YeaR Low/high bit (1=low)*/
#define OCI_DATE_INVALID_HOUR           0x40                     /* Bad HouR */
#define OCI_DATE_HOUR_BELOW_VALID       0x80 /* Bad HouR Low/high bit (1=low)*/
#define OCI_DATE_INVALID_MINUTE         0x100                  /* Bad MiNute */
#define OCI_DATE_MINUTE_BELOW_VALID     0x200
                                           /* Bad MiNute Low/high bit (1=low)*/
#define OCI_DATE_INVALID_SECOND         0x400                  /* Bad SeCond */
#define OCI_DATE_SECOND_BELOW_VALID     0x800
                                           /* bad second Low/high bit (1=low)*/
#define OCI_DATE_DAY_MISSING_FROM_1582  0x1000
                                  /* Day is one of those "missing" from 1582 */
#define OCI_DATE_YEAR_ZERO              0x2000    /* Year may not equal zero */
#define OCI_DATE_INVALID_FORMAT         0x8000      /* Bad date format input */

sword OCIDateCheck(    OCIError *err, const OCIDate *date, uword *valid    );
/*
   NAME: OCIDateCheck - OCIDate CHecK if the given date is valid
   PARAMETERS:
        err (IN/OUT) - error handle. If there is an error, it is
                recorded in 'err' and this function returns OCI_ERROR.
                The error recorded in 'err' can be retrieved by calling
                OCIErrorGet().
        date (IN) - date to be checked
        valid (OUT) -  returns zero for a valid date, otherwise 
                the ORed combination of all error bits specified below:
  
   Macro name                   Bit number      Error
   ----------                   ----------      -----
   OCI_DATE_INVALID_DAY         0x1             Bad day
   OCI_DATE_DAY_BELOW_VALID     0x2             Bad DAy Low/high bit (1=low)
   OCI_DATE_INVALID_MONTH       0x4             Bad MOnth
   OCI_DATE_MONTH_BELOW_VALID   0x8             Bad MOnth Low/high bit (1=low)
   OCI_DATE_INVALID_YEAR        0x10            Bad YeaR
   OCI_DATE_YEAR_BELOW_VALID    0x20            Bad YeaR Low/high bit (1=low)
   OCI_DATE_INVALID_HOUR        0x40            Bad HouR
   OCI_DATE_HOUR_BELOW_VALID    0x80            Bad HouR Low/high bit (1=low)
   OCI_DATE_INVALID_MINUTE      0x100           Bad MiNute
   OCI_DATE_MINUTE_BELOW_VALID  0x200           Bad MiNute Low/high bit (1=low)
   OCI_DATE_INVALID_SECOND      0x400           Bad SeCond
   OCI_DATE_SECOND_BELOW_VALID  0x800           bad second Low/high bit (1=low)
   OCI_DATE_DAY_MISSING_FROM_1582 0x1000        Day is one of those "missing"
                                                from 1582
   OCI_DATE_YEAR_ZERO           0x2000          Year may not equal zero
   OCI_DATE_INVALID_FORMAT      0x8000          Bad date format input
  
   So, for example, if the date passed in was 2/0/1990 25:61:10 in
   (month/day/year hours:minutes:seconds format), the erroor returned
   would be OCI_DATE_INVALID_DAY | OCI_DATE_DAY_BELOW_VALID |
   OCI_DATE_INVALID_HOUR | OCI_DATE_INVALID_MINUTE

   DESCRIPTION:
        Check if the given date is valid.
   RETURNS:
        OCI_SUCCESS if the function completes successfully.
        OCI_INVALID_HANDLE if 'err' is NULL.
        OCI_ERROR if
          'date' and 'valid' pointers are NULL pointers
 */

/*--------------------------- OCIDateSysDate --------------------------------*/

sword OCIDateSysDate(    OCIError *err, OCIDate *sys_date    );
/*
   NAME: OCIDateSysDate - OCIDate get current SYStem date and time
   PARAMETERS:
        err (IN/OUT) - error handle. If there is an error, it is
                recorded in 'err' and this function returns OCI_ERROR.
                The error recorded in 'err' can be retrieved by calling
                OCIErrorGet().
        sys_date (OUT) - current system date and time
   DESCRIPTION:
        Returns the current system date and time.
   RETURNS:
        OCI_SUCCESS if the function completes successfully.
        OCI_INVALID_HANDLE if 'err' is NULL.
        OCI_ERROR if
          <to be discovered>
 */

/*****************************************************************************/
/*                  FIXED-LENGTH STRING - CHAR (N)                           */
/*****************************************************************************/

/*
 * An ADT attribute declared as "x CHAR(n)" is mapped to "OCIString *x;".
 * The representation of OCIString * is shown below.
 */

/*****************************************************************************/
/*                  VARIABLE-LENGTH STRING                                   */
/*****************************************************************************/

/*
 * The variable-length string is represented in C as a pointer to OCIString
 * structure. The OCIString structure is opaque to the user. Functions are
 * provided to allow the user to manipulate a variable-length string.
 *     
 * A variable-length string can be declared as:
 *
 * OCIString *vstr;
 *
 * For binding variables of type OCIString* in OCI calls (OCIBindByName(),
 * OCIBindByPos() and OCIDefineByPos()) use the external type code SQLT_VST.
 */
#ifndef KUTYXTT3_ORACLE
typedef struct OCIString OCIString;
#define ORL_OCISTRING_DEFINED
#endif

/*-------------------------- OCIStringAssign --------------------------------*/
 
sword OCIStringAssign(    OCIEnv *env, OCIError *err, const OCIString *rhs, 
                          OCIString **lhs    );
/*
   NAME: OCIStringAssign - OCIString Assign String to String
   PARAMETERS:
        env (IN/OUT) - OCI environment handle initialized in object mode.
        err (IN/OUT) - error handle. If there is an error, it is
                recorded in 'err' and this function returns OCI_ERROR.
                The error recorded in 'err' can be retrieved by calling
                OCIErrorGet().
        rhs (IN) - RHS of the assignment, the type of rhs is also OCIString
        lhs (IN/OUT) - LHS of the assignment
   DESCRIPTION:
        Assign 'rhs' string to 'lhs' string. The 'lhs' string may be
        resized depending upon the size of the 'rhs'. The assigned string is
        null-terminated. The 'length' field will not include the extra byte
        needed for null termination.
   RETURNS:
        OCI_SUCCESS if the function completes successfully.
        OCI_INVALID_HANDLE if 'env' or 'err' is NULL.
        OCI_ERROR if
          out of space error
 */
 
/*---------------------- OCIStringAssignText --------------------------------*/
 
sword OCIStringAssignText(    OCIEnv *env, OCIError *err, const oratext *rhs, 
                              ub4 rhs_len, OCIString **lhs    );
/*
   NAME: OCIStringAssignText - OCIString Assign Text string to String
   PARAMETERS:
        env (IN/OUT) - OCI environment handle initialized in object mode.
        err (IN/OUT) - error handle. If there is an error, it is
                recorded in 'err' and this function returns OCI_ERROR.
                The error recorded in 'err' can be retrieved by calling
                OCIErrorGet().
        rhs (IN) - RHS of the assignment, the type of rhs is a text string
        rhs_len (IN) - length of the 'rhs' string
        lhs (IN/OUT) - LHS of the assignment
   DESCRIPTION:
        Assign 'rhs' string to 'lhs' string. The 'lhs' string may be
        resized depending upon the size of the 'rhs'. The assigned string is
        null-terminated. The 'length' field will not include the extra byte
        needed for null termination.
   RETURNS:
        OCI_SUCCESS if the function completes successfully.
        OCI_INVALID_HANDLE if 'env' or 'err' is NULL.
        OCI_ERROR if
          out of space error
 */

/*-------------------------- OCIStringResize --------------------------------*/

sword OCIStringResize(    OCIEnv *env, OCIError *err, ub4 new_size, 
                          OCIString **str    );
/*
   NAME: OCIStringResize - OCIString ReSiZe string memory
   PARAMETERS:
        env (IN/OUT) - OCI environment handle initialized in object mode.
        err (IN/OUT) - error handle. If there is an error, it is
                recorded in 'err' and this function returns OCI_ERROR.
                The error recorded in 'err' can be retrieved by calling
                OCIErrorGet().
        new_size (IN) - new memory size of the string in bytes
        str (IN/OUT) - allocated memory for the string is freed from the 
                OOCI heap
   DESCRIPTION:
        This function resizes the memory of the given variable-length string in
        the object cache. The contents of the string are NOT preserved.
        This function may allocate the string in a new memory region in
        which case the original memory occupied by the given string will
        be freed. If the input string is null (str == NULL), then this
        function will allocate memory for the string.
 
        If the new_size is 0, then this function frees the memory occupied
        by 'str' and a null pointer value is returned.
 
        NOTE: The caller must compute 'new_size' taking into account space
        for the null character ('\0').
   RETURNS:
        OCI_SUCCESS if the function completes successfully.
        OCI_INVALID_HANDLE if 'env' or 'err' is NULL.
        OCI_ERROR if
          out of space error
 */

/*---------------------------- OCIStringSize --------------------------------*/

ub4 OCIStringSize(    OCIEnv *env, const OCIString *vs    );
/*
   NAME: OCIStringSize - OCIString Get String siZe
   PARAMETERS:
        env(IN) - pointer to OCI environment handle
        vs (IN) - string whose size is returned
   DESCRIPTION:
        Return the size of the given string.
   RETURNS:
        size of the string in bytes is returned
 */

/*----------------------------- OCIStringPtr --------------------------------*/

oratext *OCIStringPtr(    OCIEnv *env, const OCIString *vs    );
/*
   NAME: OCIStringPtr - OCIString Get String Pointer
   PARAMETERS:
        env(IN) - pointer to OCI environment handle
        vs (IN) - pointer to the text of this string is returned
   DESCRIPTION:
        Return the pointer to the text of the given string.
   RETURNS:
        pointer to the text of the string is returned
 */

/*----------------------- OCIStringAllocSize --------------------------------*/
 
sword OCIStringAllocSize(    OCIEnv *env, OCIError *err, const OCIString *vs, 
                             ub4 *allocsize    );
/*
   NAME: OCIStringAllocSize - OCIString get Allocated SiZe of string memory 
                              in bytes
   PARAMETERS:
        env (IN/OUT) - OCI environment handle initialized in object mode.
        err (IN/OUT) - error handle. If there is an error, it is
                recorded in 'err' and this function returns OCI_ERROR.
                The error recorded in 'err' can be retrieved by calling
                OCIErrorGet().
        vs (IN) - string whose allocated size in bytes is returned
        allocsize (OUT) - allocated size of string memory in bytes is returned
   DESCRIPTION:
        Return the allocated size of the string memory in bytes. The
        allocated size is >= actual string size.
   REQUIRES:
        vs is a non-null pointer
   RETURNS:
        OCI_SUCCESS if the function completes successfully.
        OCI_INVALID_HANDLE if 'env' or 'err' is NULL.
        OCI_ERROR on error
 */

/*****************************************************************************/
/*                       VARIABLE-LENGTH RAW                                 */
/*****************************************************************************/

/*
 * The variable-length raw is represented in C as a pointer to OCIRaw
 * structure. The OCIRaw structure is opaque to the user. Functions are
 * provided to allow the user to manipulate a variable-length raw.
 *     
 * A variable-length raw can be declared as:
 *
 * OCIRaw *raw;
 *
 * For binding variables of type OCIRaw* in OCI calls (OCIBindByName(),
 * OCIBindByPos() and OCIDefineByPos()) use the external type code SQLT_LVB.
 */
#ifndef KUTYXTT3_ORACLE
typedef struct OCIRaw OCIRaw;
#define ORL_OCIRAW_DEFINED
#endif

/*-------------------------- OCIRawAssignRaw --------------------------------*/
 
sword OCIRawAssignRaw(    OCIEnv *env, OCIError *err, const OCIRaw *rhs, 
                          OCIRaw **lhs    );
/*
   NAME: OCIRawAssignRaw - OCIRaw Assign Raw (of type OCIRaw*) to
                   Raw (of type OCIRaw*)
   PARAMETERS:
        env (IN/OUT) - OCI environment handle initialized in object mode.
        err (IN/OUT) - error handle. If there is an error, it is
                recorded in 'err' and this function returns OCI_ERROR.
                The error recorded in 'err' can be retrieved by calling
                OCIErrorGet().
        rhs (IN) - RHS of the assignment, the type of rhs is also OCIRaw
        lhs (IN/OUT) - LHS of the assignment
   DESCRIPTION:
        Assign 'rhs' raw to 'lhs' raw. The 'lhs' raw may be
        resized depending upon the size of the 'rhs'.
   RETURNS:
        OCI_SUCCESS if the function completes successfully.
        OCI_INVALID_HANDLE if 'env' or 'err' is NULL.
        OCI_ERROR if
          out of space error
 */
 
/*------------------------ OCIRawAssignBytes --------------------------------*/
 
sword OCIRawAssignBytes(    OCIEnv *env, OCIError *err, const ub1 *rhs, 
                            ub4 rhs_len, OCIRaw **lhs    );
/*
   NAME: OCIRawAssignBytes - OCIRaw Assign raw Bytes (of type ub1*) to Raw 
                   (of type OCIRaw*)
   PARAMETERS:
        env (IN/OUT) - OCI environment handle initialized in object mode.
        err (IN/OUT) - error handle. If there is an error, it is
                recorded in 'err' and this function returns OCI_ERROR.
                The error recorded in 'err' can be retrieved by calling
                OCIErrorGet().
        rhs (IN) - RHS of the assignment, the type of rhs is ub1 *
        rhs_len (IN) - length of the 'rhs' raw
        lhs (IN/OUT) - LHS of the assignment
   DESCRIPTION:
        Assign 'rhs' raw to 'lhs' raw. The 'lhs' raw may be
        resized depending upon the size of the 'rhs'.
   RETURNS:
        OCI_SUCCESS if the function completes successfully.
        OCI_INVALID_HANDLE if 'env' or 'err' is NULL.
        OCI_ERROR if
          out of space error
 */

/*---------------------------- OCIRawResize ---------------------------------*/
 
sword OCIRawResize(    OCIEnv *env, OCIError *err, ub4 new_size, 
                       OCIRaw **raw    );
/*
   NAME: OCIRawResize - OCIRaw ReSiZe memory of variable-length raw
   PARAMETERS:
        env (IN/OUT) - OCI environment handle initialized in object mode.
        err (IN/OUT) - error handle. If there is an error, it is
                recorded in 'err' and this function returns OCI_ERROR.
                The error recorded in 'err' can be retrieved by calling
                OCIErrorGet().
        new_size (IN) - new size of the raw data in bytes
        raw (IN) - variable-length raw pointer; the raw is
                resized to 'new_size'
   DESCRIPTION:
        This function resizes the memory of the given variable-length raw in
        the object cache. 
        The previous contents of the raw are NOT preserved.
        This function may allocate the raw in a new memory region in
        which case the original memory occupied by the given raw will
        be freed. If the input raw is null (raw == NULL), then this
        function will allocate memory for the raw data.
 
        If the new_size is 0, then this function frees the memory occupied
        by 'raw' and a null pointer value is returned.
   RETURNS:
        OCI_SUCCESS if the function completes successfully.
        OCI_INVALID_HANDLE if 'env' or 'err' is NULL.
        OCI_ERROR if
          out of space error
 */

/*------------------------------- OCIRawSize --------------------------------*/

ub4 OCIRawSize(    OCIEnv * env, const OCIRaw *raw    );
/*
   NAME: OCIRawSize - OCIRaw Get Raw siZe
   PARAMETERS:
        env (IN)  - pointer to OCI environment handle
        raw (INT) - raw whose size is returned
   DESCRIPTION:
        Return the size of the given raw.
   RETURNS:
        size of the raw in bytes is returned
 */

/*--------------------------------- OCIRawPtr -------------------------------*/
ub1 *OCIRawPtr(    OCIEnv * env, const OCIRaw *raw    );
/*
   NAME: OCIRawPtr - OCIRaw Get Raw data Pointer
   PARAMETERS:
        env (IN) - pointer to OCI environment handle
        raw (IN) - pointer to the data of this raw is returned
   DESCRIPTION:
        Return the pointer to the data of the given raw.
   RETURNS:
        pointer to the data of the raw is returned
 */

/*------------------------------ OCIRawAllocSize ----------------------------*/
 
sword OCIRawAllocSize(    OCIEnv *env, OCIError *err, const OCIRaw *raw,
                          ub4 *allocsize    );
/*
   NAME: OCIRawAllocSize - OCIRaw get Allocated SiZe of raw memory in bytes
   PARAMETERS:
        env (IN/OUT) - OCI environment handle initialized in object mode.
        err (IN/OUT) - error handle. If there is an error, it is
                recorded in 'err' and this function returns OCI_ERROR.
                The error recorded in 'err' can be retrieved by calling
                OCIErrorGet().
        raw (IN) - raw whose allocated size in bytes is returned
        allocsize (OUT) - allocated size of raw memory in bytes is returned
   DESCRIPTION:
        Return the allocated size of the raw memory in bytes. The
        allocated size is >= actual raw size.
   REQUIRES:
        raw is a non-null pointer
   RETURNS:
        OCI_SUCCESS if the function completes successfully.
        OCI_INVALID_HANDLE if 'env' or 'err' is NULL.
        OCI_ERROR upon error
 */

/*****************************************************************************/
/*                     OBJECT REFERENCE OPERATIONS                           */
/*****************************************************************************/

/* 
 * See the definition of OCIRef in oro.h.
 *
 * For binding variables of type OCIRef* in OCI calls (OCIBindByName(),
 * OCIBindByPos() and OCIDefineByPos()) use the code SQLT_REF.
 * 
 */

/*---------------------------- OCIRefClear ----------------------------------*/
void OCIRefClear(    OCIEnv *env, OCIRef *ref    );
/*
   NAME: OCIRefClear - OCIRef CLeaR or nullify a ref
   PARAMETERS:
        env (IN)     - pointer to OCI environment handle
        ref (IN/OUT) - ref to clear
   DESCRIPTION:
        Clear or nullify the given ref. A ref is considered to be a null ref
        if it does not contain a valid OID (and thus doesn't point to an
        object). Logically, a null ref is a dangling ref.

        Note that a null ref is still a valid SQL value and is not SQL-ly null.
        It can be used as a valid non-null constant ref value for NOT NULL
        column or attribute of a row in a table.
        
        If a null pointer value is passed as a ref,
        then this function is a no-op.
 */

/*--------------------------- OCIRefAssign ----------------------------------*/
sword OCIRefAssign(    OCIEnv *env, OCIError *err, const OCIRef *source, 
                       OCIRef **target    );
/*
   NAME: OCIRefAssign - OCIRef CoPY a ref to another
   PARAMETERS:
        env (IN/OUT) - OCI environment handle initialized in object mode.
        err (IN/OUT) - error handle. If there is an error, it is
                recorded in 'err' and this function returns OCI_ERROR.
                The error recorded in 'err' can be retrieved by calling
                OCIErrorGet().
        source (IN) - ref to copy from
        target (IN/OUT) - ref to copy to
   DESCRIPTION:
        Copy 'source' ref to 'target' ref; both then reference the same
        object. If the target ref pointer is null (i.e. *target == NULL)
        then the copy function will allocate memory for the target ref
        in OOCI heap prior to the copy.
   RETURNS:
        OCI_SUCCESS if the function completes successfully.
        OCI_INVALID_HANDLE if 'env' or 'err' is NULL.
        OCI_ERROR if
          1) out of memory
 */

/*-------------------------- OCIRefIsEqual ----------------------------------*/
boolean OCIRefIsEqual(    OCIEnv *env, const OCIRef *x, const OCIRef *y    );
/*
   NAME: OCIRefIsEqual - OCIRef compare two refs for EQUality
   PARAMETERS:
        env (IN) - pointer to OCI environment handle
        x (IN)   - ref to compare
        y (IN)   - ref to compare
   DESCRIPTION:
        Compare the given refs for equality.
        Two refs are equal if and only if:
          - they are both referencing the same persistent object, or
          - they are both referencing the same transient object.

        NOTE THAT TWO NULL REFS ARE CONSIDERED NOT EQUAL BY THIS FUNCTION.
   RETURNS:
        TRUE if the two refs are equal
        FALSE if the two refs are not equal, or X is NULL, or Y is NULL
 */

/*--------------------------- OCIRefIsNull ----------------------------------*/
boolean OCIRefIsNull(    OCIEnv *env, const OCIRef *ref    );
/*
   NAME: OCIRefIsNull - OCIRef test if a ref is NULl
   PARAMETERS:
        env (IN) - pointer to OCI environment handle
        ref (IN) - ref to test for null
   DESCRIPTION:
        Return TRUE if the given ref is null; otherwise, return FALSE.
        A ref is null if and only if:
        - it is supposed to be referencing a persistent object, but
          its OID is null, or
        - it is supposed to be referencing a transient object, but it is
          currently not pointing to an object.
        A ref is a dangling ref if the object that it points to does not
        exist.
   RETURNS:
        TRUE if the given ref is NULL
        FALSE if the given ref is not NULL
 */

/*-------------------------- OCIRefHexSize ----------------------------------*/
ub4 OCIRefHexSize(    OCIEnv *env, const OCIRef *ref    );
/*
   NAME: OCIRefHexSize - OCIRef Hexadecimal buffer SiZe in bytes
   PARAMETERS:
        env (IN) - pointer to OCI environment handle
        ref (IN) - ref whose size in hexadecimal representation in bytes is
                returned
   DESCRIPTION:
        Return the size of the buffer in bytes required for the hexadecimal
        representation of the ref. A buffer of at-least this size must be
        passed to ref-to-hex (OCIRefToHex) conversion function.
   RETURNS:
        size of hexadecimal representation of ref
 */

/*-------------------------- OCIRefFromHex ---------------------------------*/
sword OCIRefFromHex(    OCIEnv *env, OCIError *err, const OCISvcCtx *svc, 
                        const oratext *hex, ub4 length, OCIRef **ref    );
/*
   NAME:
        OCIRefFromHex - OCIRef convert a Hexadecimal string TO a Ref
   PARAMETERS:
        env (IN/OUT) - OCI environment handle initialized in object mode
        err (IN/OUT) - error handle. If there is an error, it is
                recorded in 'err' and this function returns OCI_ERROR.
                The error recorded in 'err' can be retrieved by
                calling OCIErrorGet().
        svc (IN) - OCI service context handle; if the resulting ref is
                initialized with this service context
        hex (IN) - hexadecimal string (that was produced by 'OCIRefToHex()"
                previously) to be convert into a ref
        length (IN) - length of the hexadecimal string
        ref (IN/OUT) - ref is initialized with the given value ('hex').
                If *ref is null, then space for the ref is allocated in the
                object cache, otherwise the memory occupied by the given ref
                is re-used.
   DESCRIPTION:
        Convert the given hexadecimal string into a ref. This function
        ensures that the resulting ref is well formed. It does NOT ensure
        that the object pointed to by the resulting ref exists or not.
   RETURNS:
        OCI_SUCCESS if the function completes successfully.
        OCI_INVALID_HANDLE if 'env' or 'err' is NULL.
        OCI_ERROR if
 */

/*--------------------------- OCIRefToHex -----------------------------------*/
sword OCIRefToHex(    OCIEnv *env, OCIError *err, const OCIRef *ref, 
                      oratext *hex, ub4 *hex_length    );
/*
   NAME:
        OCIRefToHex - OCIRef convert ref to a Hexadecimal string
   PARAMETERS:
        env (IN/OUT) - OCI environment handle initialized in object mode
        err (IN/OUT) - error handle. If there is an error, it is
                recorded in 'err' and this function returns OCI_ERROR.
                The error recorded in 'err' can be retrieved by
                calling OCIErrorGet().
        ref (IN) - ref to be converted into a hexadecimal string; if the
                ref is a null ref (i.e. OCIRefIsNull(ref) == TRUE) then
                a zero hex_length value is returned
        hex (OUT) - buffer that is large enough to contain the resulting
                hexadecimal string; the contents of the string is opaque
                to the caller
        hex_length (IN/OUT) - on input specifies the size of the 'hex' buffer,
                on output specifies the actual size of the hexadecimal
                string being returned in 'hex'
   DESCRIPTION:
        Convert the given ref into a hexadecimal string, and return the length
        of the string. The resulting string is opaque to the caller.
   RETURNS:
        OCI_SUCCESS if the function completes successfully.
        OCI_INVALID_HANDLE if 'env' or 'err' is NULL.
        OCI_ERROR if
          the given buffer is not big enough to hold the resulting string
 */


/*****************************************************************************/
/*                          COLLECTION FUNCTIONS                             */
/*****************************************************************************/

/* 
   The generic collection is represented by the type 'OCIColl'. The following
   operations OCIColl*() are provided on a generic collection:
     - get current size of collection
     - get upper bound of collection
     - get pointer to an element given its index
     - set element at given index (assign element)
     - append an element 
     - trim the given number of elements from the end of the collection
     - collection assignment

   The following iterator based scanning functions are also provided on a
   generic collection. These functions make use of an iterator which is
   defined to be of type OCIIter.

     - create an iterator for scanning collection
     - destroy iterator
     - reset iterator to the beginning of collection
     - get pointer to current element pointed by iterator
     - get pointer to next element
     - get pointer to previous element

   The collections variable-length array (varray) and nested table
   are sub-types of generic collection. This means that the OCIColl*() 
   functions can also be used to manipulate varray and nested table.

   The varray is represented by OCIArray type and nested table by OCITable.
   Besides OCIColl*() functions no additional functions are provided for
   manipulating varrays. The OCIColl*() functions are a complete set of 
   functions to manipulate varrays.

   Besides OCIColl*() functions, the following functions OCITable*() can be 
   used to manipulate nested table. The OCITable*() functions operate on 
   nested tables only and should not be used on a varray.

     - delete an element at index i. Note that the position
       ordinals of the remaining elements of the table is not changed by the
       delete operation. So delete creates "holes" in the table.
     - check if an element exists at the given index i
     - return the smallest value of i for which exists(i) is true
     - return the largest value of i for which exists(i) is true
     - return pointer to the smallest position j, greater than i, such that 
       OCITableExists(j) is true
     - return pointer to the largest position j, less than i, such that 
       OCITableExists(j) is true

   For binding variables of type OCIColl* or OCITable* in OCI calls
   (OCIBindByName(), OCIBindByPos() and OCIDefineByPos()) use the external
   type code SQLT_NTY.
 */

/* OCIColl - generic collection type */
typedef struct OCIColl OCIColl;

/* OCIArray - varray collection type */
typedef OCIColl OCIArray;

/* OCITable - nested table collection type */
typedef OCIColl OCITable;

/* OCIIter - collection iterator */
typedef struct OCIIter OCIIter;

/*----------------------------- OCICollSize ---------------------------------*/

sword OCICollSize( OCIEnv *env, OCIError *err, const OCIColl *coll, 
                   sb4 *size );
/*
   NAME: OCICollSize - OCIColl return current SIZe of the given collection
   PARAMETERS:
        env(IN) - pointer to OCI environment handle
        err (IN/OUT) - error handle. If there is an error, it is
                recorded in 'err' and this function returns OCI_ERROR.
                The error recorded in 'err' can be retrieved by calling
                OCIErrorGet().
        coll (IN) - collection whose number of elements is returned
        size (OUT) - current number of elements in the collection
   DESCRIPTION:
          Returns the current number of elements in the given collection.

          For collections of type nested table wherein 'delete element'
          operation is allowed, the count returned by OCICollSize() will
          NOT be decremented upon deleting elements. For example:

            OCICollSize(...); 
            // assume 'size' returned is equal to 5
            OCITableDelete(...); // delete one element
            OCICollSize(...);
            // 'size' returned will still be 5

          To get the count minus the deleted elements use OCITableSize(). 
          Continuing the above example,

            OCITableSize(...)
            // 'size' returned will be equal to 4

          Note, a trim operation (OCICollTrim) will decrement the count 
          by the number of trimmed elements. Continuing the above example,

            OCICollTrim(..,1..); // trim one element
            OCICollSize(...);
            // 'size' returned will be equal to 4
   RETURNS:
        OCI_SUCCESS if the function completes successfully.
        OCI_INVALID_HANDLE if 'env' or 'err' is NULL.
        OCI_ERROR if
          error during loading of collection into object cache
          any of the input parameters is null
 */

/*------------------------------ OCICollMax ---------------------------------*/

sb4 OCICollMax(    OCIEnv *env, const OCIColl *coll    );
/*
   NAME: OCICollMax - OCIColl return MAXimum size (upper-bound) of the
                   given collection (in number of elements)
   PARAMETERS:
        env(IN) - pointer to OCI environment handle
        coll (IN) - collection whose upper-bound in number of elements 
                is returned
   DESCRIPTION:
        Returns the max number of elements that the given collection can hold.
        A value 0 indicates that the collection has no upper-bound.
   REQUIRES:
        coll must point to a valid collection descriptor
   RETURNS:
        upper-bound of the given collection
 */

/*-------------------------- OCICollGetElem ---------------------------------*/

sword OCICollGetElem(    OCIEnv *env, OCIError *err, const OCIColl *coll, 
                         sb4 index, boolean *exists, void  **elem, 
                         void  **elemind    );
/*
   NAME: OCICollGetElem - OCIColl GET pointer to the element at the given index
   PARAMETERS:
        env (IN/OUT) - OCI environment handle initialized in object mode.
        err (IN/OUT) - error handle. If there is an error, it is
                recorded in 'err' and this function returns OCI_ERROR.
                The error recorded in 'err' can be retrieved by calling
                OCIErrorGet().
        coll (IN) - pointer to the element in this collection is returned
        index (IN) - index of the element whose pointer is returned
        exists (OUT) - set to FALSE if element at the specified index does 
                not exist else TRUE
        elem (OUT) - address of the desired element is returned
        elemind (OUT) [optional] - address of the null indicator information 
                is returned; if (elemind == NULL) then the null indicator
                information will NOT be returned
   DESCRIPTION:
        Get the address of the element at the given position. Optionally
        this function also returns the address of the element's null indicator
        information.
 
        The following table describes for each collection element type
        what the corresponding element pointer type is. The element pointer
        is returned via the 'elem' parameter of OCICollGetElem().
 
           Element Type                       *elem is set to
        -----------------------               ---------------
         Oracle Number (OCINumber)              OCINumber*
         Date (OCIDate)                         OCIDate*
         Variable-length string (OCIString*)    OCIString**
         Variable-length raw (OCIRaw*)          OCIRaw**
         object reference (OCIRef*)             OCIRef**
         lob locator (OCILobLocator*)           OCILobLocator**
         object type (e.g. person)              person*
 
        The element pointer returned by OCICollGetElem() is in a form
        such that it can not only be used to access the
        element data but also is in a form that can be used as the target
        (i.e left-hand-side) of an assignment statement.
 
        For example, assume the user is iterating over the elements of
        a collection whose element type is object reference (OCIRef*). A call
        to OCICollGetElem() returns pointer to a reference handle
        (i.e. OCIRef**). After getting, the pointer to the collection
        element, the user may wish to modify it by assigning a new reference.
        This can be accomplished via the ref assignment function shown below:
 
        sword OCIRefAssign( OCIEnv *env, OCIError *err, const OCIRef *source,
                            OCIRef **target );
 
        Note that the 'target' parameter of OCIRefAssign() is of type
        'OCIRef**'. Hence OCICollGetElem() returns 'OCIRef**'.
        If '*target == NULL' a new ref will be allocated by OCIRefAssign()
        and returned via the 'target' parameter.
 
        Similarly, if the collection element was of type string (OCIString*),
        OCICollGetElem() returns pointer to string handle
        (i.e. OCIString**). If a new string is assigned, via
        OCIStringAssign() or OCIStringAssignText() the type of the target
        must be 'OCIString **'.
 
        If the collection element is of type Oracle number, OCICollGetElem()
        returns OCINumber*. The prototype of OCINumberAssign() is shown below:
 
        sword OCINumberAssign(OCIError *err, const OCINumber *from,
                              OCINumber *to);
   RETURNS:
        OCI_SUCCESS if the function completes successfully.
        OCI_INVALID_HANDLE if 'env' or 'err' is NULL.
        OCI_ERROR if
          any of the input parameters is null
 */

/*------------------------- OCICollGetElemArray -----------------------------*/

sword OCICollGetElemArray(  OCIEnv *env, OCIError *err, const OCIColl *coll, 
                            sb4 index, boolean *exists, void  **elem, 
                            void  **elemind, uword *nelems);
/*
   NAME: OCICollGetElemArray - OCIColl GET pointers to elements from given index
   PARAMETERS:
        env (IN/OUT) - OCI environment handle initialized in object mode.
        err (IN/OUT) - error handle. If there is an error, it is
                recorded in 'err' and this function returns OCI_ERROR.
                The error recorded in 'err' can be retrieved by calling
                OCIErrorGet().
        coll (IN) - pointers to the elements in this collection is returned
        index (IN) - starting index of the element
        exists (OUT) - set to FALSE if element at the specified index does 
                not exist else TRUE
        elem (OUT) - address of the desired elements is returned
        elemind (OUT) [optional] - address of the null indicators information 
                is returned; if (elemind == NULL) then the null indicator
                information will NOT be returned
        nelems(IN/OUT) - Upper bound of elem and/or elemind array
   DESCRIPTION:
        Get the address of the elements from the given position. Optionally
        this function also returns the address of the element's null indicator
        information.
 
        The following table describes for each collection element type
        what the corresponding element pointer type is. The element pointer
        is returned via the 'elem' parameter of OCICollGetElem().
 
           Element Type                       *elem is set to
        -----------------------               ---------------
         Oracle Number (OCINumber)              OCINumber*
         Date (OCIDate)                         OCIDate*
         Variable-length string (OCIString*)    OCIString**
         Variable-length raw (OCIRaw*)          OCIRaw**
         object reference (OCIRef*)             OCIRef**
         lob locator (OCILobLocator*)           OCILobLocator**
         object type (e.g. person)              person*
 
        The element pointer returned by OCICollGetElem() is in a form
        such that it can not only be used to access the
        element data but also is in a form that can be used as the target
        (i.e left-hand-side) of an assignment statement.
 
        For example, assume the user is iterating over the elements of
        a collection whose element type is object reference (OCIRef*). A call
        to OCICollGetElem() returns pointer to a reference handle
        (i.e. OCIRef**). After getting, the pointer to the collection
        element, the user may wish to modify it by assigning a new reference.
        This can be accomplished via the ref assignment function shown below:
 
        sword OCIRefAssign( OCIEnv *env, OCIError *err, const OCIRef *source,
                            OCIRef **target );
 
        Note that the 'target' parameter of OCIRefAssign() is of type
        'OCIRef**'. Hence OCICollGetElem() returns 'OCIRef**'.
        If '*target == NULL' a new ref will be allocated by OCIRefAssign()
        and returned via the 'target' parameter.
 
        Similarly, if the collection element was of type string (OCIString*),
        OCICollGetElem() returns pointer to string handle
        (i.e. OCIString**). If a new string is assigned, via
        OCIStringAssign() or OCIStringAssignText() the type of the target
        must be 'OCIString **'.
 
        If the collection element is of type Oracle number, OCICollGetElem()
        returns OCINumber*. The prototype of OCINumberAssign() is shown below:
 
        sword OCINumberAssign(OCIError *err, const OCINumber *from,
                              OCINumber *to);
   RETURNS:
        OCI_SUCCESS if the function completes successfully.
        OCI_INVALID_HANDLE if 'env' or 'err' is NULL.
        OCI_ERROR if
          any of the input parameters is null
 */

/*----------------------- OCICollAssignElem ---------------------------------*/

sword OCICollAssignElem(    OCIEnv *env, OCIError *err, sb4 index, 
                            const void  *elem, 
                            const void  *elemind, OCIColl *coll    );
/*
   NAME: OCICollAssignElem - OCIColl ASsign Element
   PARAMETERS:
        env (IN/OUT) - OCI environment handle initialized in object mode.
        err (IN/OUT) - error handle. If there is an error, it is
                recorded in 'err' and this function returns OCI_ERROR.
                The error recorded in 'err' can be retrieved by calling
                OCIErrorGet().
        index (IN) - index of the element whose is assigned to
        elem (IN) - element which is assigned from (source element)
        elemind (IN) [optional] - pointer to the element's null indicator 
                information; if (elemind == NULL) then the null indicator
                information of the assigned element will be set to non-null.
        coll (IN/OUT) - collection to be updated
   DESCRIPTION:
        Assign the given element value 'elem' to the element at coll[index].
        If the collection is of type nested table, the element at the given 
        index may not exist (i.e. may have been deleted). In this case, the 
        given element is inserted at index 'index'.
        Otherwise, the element at index 'index' is updated with the value
        of 'elem'.
  
        Note that the given element is deep-copied and 
        'elem' is strictly an input parameter.
   RETURNS:
        OCI_SUCCESS if the function completes successfully.
        OCI_INVALID_HANDLE if 'env' or 'err' is NULL.
        OCI_ERROR if
          any of the input parameters is null
          out of memory error
          given index is out of bounds of the given collection
 */

/*--------------------------- OCICollAssign ---------------------------------*/

sword OCICollAssign(    OCIEnv *env, OCIError *err, const OCIColl *rhs, 
                        OCIColl *lhs    );
/*
   NAME: OCICollAssign - OCIColl ASsiGn collection
   PARAMETERS:
        env (IN/OUT) - OCI environment handle initialized in object mode.
        err (IN/OUT) - error handle. If there is an error, it is
                recorded in 'err' and this function returns OCI_ERROR.
                The error recorded in 'err' can be retrieved by calling
                OCIErrorGet().
        rhs (IN) - collection to be assigned from
        lhs (OUT) - collection to be assigned to
   DESCRIPTION:
        Assign 'rhs' to 'lhs'. The 'lhs' collection may be decreased or 
        increased depending upon the size of 'rhs'. If the 'lhs' contains 
        any elements then the elements will be deleted prior to the 
        assignment. This function performs a deep-copy. The memory for the 
        elements comes from the object cache.
 
        An error is returned if the element types of the lhs and rhs 
        collections do not match. Also, an error is returned if the 
        upper-bound of the lhs collection is less than the current number of 
        elements in the rhs collection.
   RETURNS:
        OCI_SUCCESS if the function completes successfully.
        OCI_INVALID_HANDLE if 'env' or 'err' is NULL.
        OCI_ERROR if
          any of the input parameters is null
          out of memory error
          type mis-match of lhs and rhs collections
          upper-bound of lhs collection is less than the current number of 
          elements in the rhs collection
 */

/*--------------------------- OCICollAppend ---------------------------------*/

sword OCICollAppend(    OCIEnv *env, OCIError *err, const void  *elem, 
                        const void  *elemind, OCIColl *coll    );
/*
   NAME: OCICollAppend - OCIColl APPend collection
   PARAMETERS:
        env (IN/OUT) - OCI environment handle initialized in object mode.
        err (IN/OUT) - error handle. If there is an error, it is
                recorded in 'err' and this function returns OCI_ERROR.
                The error recorded in 'err' can be retrieved by calling
                OCIErrorGet().
        elem (IN) - pointer to the element which is appended to the end
                of the given collection
        elemind (IN) [optional] - pointer to the element's null indicator 
                information; if (elemind == NULL) then the null indicator
                information of the appended element will be set to non-null.
        coll (IN/OUT) - updated collection
   DESCRIPTION:
        Append the given element to the end of the given collection. 
        Appending an element is equivalent to:
          - increasing the size of the collection by 1 element
          - updating (deep-copying) the last element's data with the given 
            element's data
  
        Note that the pointer to the given element 'elem' will not be saved 
        by this function. So 'elem' is strictly an input parameter.
        An error is returned if the current size of the collection
        is equal to the max size (upper-bound) of the collection prior to 
        appending the element.
   RETURNS:
        OCI_SUCCESS if the function completes successfully.
        OCI_INVALID_HANDLE if 'env' or 'err' is NULL.
        OCI_ERROR if
          any of the input parameters is null
          out of memory error
          current size of collection == max size of the collection
 */

/*----------------------------- OCICollTrim ---------------------------------*/

sword OCICollTrim(    OCIEnv *env, OCIError *err, sb4 trim_num, 
                      OCIColl *coll    );
/*
   NAME: OCICollTrim - OCIColl Trim elements from the end of the collection
   PARAMETERS:
        env (IN/OUT) - OCI environment handle initialized in object mode.
        err (IN/OUT) - error handle. If there is an error, it is
                recorded in 'err' and this function returns OCI_ERROR.
                The error recorded in 'err' can be retrieved by calling
                OCIErrorGet().
        trim_num (IN) - number of elements to trim
        coll (IN/OUT) - 'trim_num' of elements are removed (freed) from the
                end of the collection
   DESCRIPTION:
        Trim the collection by the given number of elements. The elements are 
        removed from the end of the collection.
  
        An error is returned if the 'trim_num' is greater than the current 
        size of the collection.
   RETURNS:
        OCI_SUCCESS if the function completes successfully.
        OCI_INVALID_HANDLE if 'env' or 'err' is NULL.
        OCI_ERROR if
          any of the input parameters is null
          'trim_num' is greater than the current size of the collection.
 */

/*--------------------------- OCICollIsLocator ------------------------------*/

sword OCICollIsLocator(OCIEnv *env, OCIError *err, const OCIColl *coll,
                       boolean *result );
/*
Name: OCICollIsLocator - OCIColl indicates whether a collection is locator
                         based or not.
Parameters:
        env(IN) - pointer to OCI environment handle
        err (IN/OUT) - error handle. If there is an error, it is
                recorded in 'err' and this function returns OCI_ERROR.
                The error recorded in 'err' can be retrieved by calling
                OCIErrorGet().
        coll (IN) - collection item.
        result (OUT) - TRUE if the collection item is a locator, FALSE
                       otherwise
Description:
        Returns TRUE in the result OUT parameter if the collection item is a
        locator, otherwise returns FALSE.
Returns:
        OCI_SUCCESS if the function completes successfully.
        OCI_INVALID_HANDLE if 'env' or 'err' is NULL.
*/

/*---------------------------- OCIIterCreate --------------------------------*/

sword OCIIterCreate(    OCIEnv *env, OCIError *err, const OCIColl *coll, 
                        OCIIter **itr    );
/*
   NAME: OCIIterCreate - OCIColl Create an ITerator to scan the collection
                      elements
   PARAMETERS:
        env (IN/OUT) - OCI environment handle initialized in object mode.
        err (IN/OUT) - error handle. If there is an error, it is
                recorded in 'err' and this function returns OCI_ERROR.
                The error recorded in 'err' can be retrieved by calling
                OCIErrorGet().
        coll (IN) - collection which will be scanned; the different 
                collection types are varray and nested table
        itr (OUT) - address to the allocated collection iterator is
                returned by this function
   DESCRIPTION:
        Create an iterator to scan the elements of the collection. The
        iterator is created in the object cache. The iterator is initialized
        to point to the beginning of the collection.
  
        If the next function (OCIIterNext) is called immediately 
        after creating the iterator then the first element of the collection 
        is returned. 
        If the previous function (OCIIterPrev) is called immediately after 
        creating the iterator then "at beginning of collection" error is 
        returned.
   RETURNS:
        OCI_SUCCESS if the function completes successfully.
        OCI_INVALID_HANDLE if 'env' or 'err' is NULL.
        OCI_ERROR if
          any of the input parameters is null
          out of memory error
 */

/*----------------------------- OCIIterDelete ------------------------------*/

sword OCIIterDelete(    OCIEnv *env, OCIError *err, OCIIter **itr    );
/*
   NAME: OCIIterDelete - OCIColl Delete ITerator
   PARAMETERS:
        env (IN/OUT) - OCI environment handle initialized in object mode.
        err (IN/OUT) - error handle. If there is an error, it is
                recorded in 'err' and this function returns OCI_ERROR.
                The error recorded in 'err' can be retrieved by calling
                OCIErrorGet().
        itr (IN/OUT) - the allocated collection iterator is destroyed and
                the 'itr' is set to NULL prior to returning
   DESCRIPTION:
        Delete the iterator which was previously created by a call to 
        OCIIterCreate.
   RETURNS:
        OCI_SUCCESS if the function completes successfully.
        OCI_INVALID_HANDLE if 'env' or 'err' is NULL.
        OCI_ERROR if
          any of the input parameters is null
          to be discovered
 */

/*----------------------------- OCIIterInit ---------------------------------*/

sword OCIIterInit(    OCIEnv *env, OCIError *err, const OCIColl *coll, 
                      OCIIter *itr    );
/*
   NAME: OCIIterInit - OCIColl Initialize ITerator to scan the given 
                   collection
   PARAMETERS:
        env (IN/OUT) - OCI environment handle initialized in object mode.
        err (IN/OUT) - error handle. If there is an error, it is
                recorded in 'err' and this function returns OCI_ERROR.
                The error recorded in 'err' can be retrieved by calling
                OCIErrorGet().
        coll (IN) - collection which will be scanned; the different 
                collection types are varray and nested table
        itr (IN/OUT) - pointer to an allocated  collection iterator
   DESCRIPTION:
        Initializes the given iterator to point to the beginning of the 
        given collection. This function can be used to:
  
        a. reset an iterator to point back to the beginning of the collection 
        b. reuse an allocated iterator to scan a different collection
   RETURNS:
        OCI_SUCCESS if the function completes successfully.
        OCI_INVALID_HANDLE if 'env' or 'err' is NULL.
        OCI_ERROR if
          any of the input parameters is null
 */

/*------------------------ OCIIterGetCurrent --------------------------------*/

sword OCIIterGetCurrent(    OCIEnv *env, OCIError *err, const OCIIter *itr, 
                            void  **elem, void  **elemind    );
/*
   NAME: OCIIterGetCurrent - OCIColl Iterator based, get CURrent collection
                    element
   PARAMETERS:
        env (IN/OUT) - OCI environment handle initialized in object mode.
        err (IN/OUT) - error handle. If there is an error, it is
                recorded in 'err' and this function returns OCI_ERROR.
                The error recorded in 'err' can be retrieved by calling
                OCIErrorGet().
        itr (IN) - iterator which points to the current element
        elem (OUT) - address of the element pointed by the iterator is returned
        elemind (OUT) [optional] - address of the element's null indicator 
                information is returned; if (elemind == NULL) then the null 
                indicator information will NOT be returned
   DESCRIPTION:
        Returns pointer to the current element and its corresponding null
        information.
   RETURNS:
        OCI_SUCCESS if the function completes successfully.
        OCI_INVALID_HANDLE if 'env' or 'err' is NULL.
        OCI_ERROR if
          any of the input parameters is null
 */

/*------------------------------ OCIIterNext --------------------------------*/

sword OCIIterNext(    OCIEnv *env, OCIError *err, OCIIter *itr, 
                      void  **elem, void  **elemind, boolean *eoc    );
/*
   NAME: OCIIterNext - OCIColl Iterator based, get NeXT collection element
   PARAMETERS:
        env (IN/OUT) - OCI environment handle initialized in object mode.
        err (IN/OUT) - error handle. If there is an error, it is
                recorded in 'err' and this function returns OCI_ERROR.
                The error recorded in 'err' can be retrieved by calling
                OCIErrorGet().
        itr (IN/OUT) - iterator is updated to point to the next element
        elem (OUT) - after updating the iterator to point to the next element,
                address of the element is returned
        elemind (OUT) [optional] - address of the element's null indicator 
                information is returned; if (elemind == NULL) then the null 
                indicator information will NOT be returned
        eoc (OUT) - TRUE if iterator is at End Of Collection (i.e. next
                element does not exist) else FALSE
   DESCRIPTION:
        Returns pointer to the next element and its corresponding null
        information. The iterator is updated to point to the next element.
  
        If the iterator is pointing to the last element of the collection
        prior to executing this function, then calling this function will 
        set eoc flag to TRUE. The iterator will be left unchanged in this
        situation.
   RETURNS:
        OCI_SUCCESS if the function completes successfully.
        OCI_INVALID_HANDLE if 'env' or 'err' is NULL.
        OCI_ERROR if
          any of the input parameters is null
 */

/*------------------------------ OCIIterPrev --------------------------------*/

sword OCIIterPrev(    OCIEnv *env, OCIError *err, OCIIter *itr, 
                      void  **elem, void  **elemind, boolean *boc    );
/*
   NAME: OCIIterPrev - OCIColl Iterator based, get PReVious collection element
   PARAMETERS:
        env (IN/OUT) - OCI environment handle initialized in object mode.
        err (IN/OUT) - error handle. If there is an error, it is
                recorded in 'err' and this function returns OCI_ERROR.
                The error recorded in 'err' can be retrieved by calling
                OCIErrorGet().
        itr (IN/OUT) - iterator is updated to point to the previous 
                element
        elem (OUT) - after updating the iterator to point to the previous 
                element, address of the element is returned
        elemind (OUT) [optional] - address of the element's null indicator 
                information is returned; if (elemind == NULL) then the null 
                indicator information will NOT be returned
        boc (OUT) - TRUE if iterator is at Beginning Of Collection (i.e.
                previous element does not exist) else FALSE.
   DESCRIPTION:
        Returns pointer to the previous element and its corresponding null
        information. The iterator is updated to point to the previous element.
  
        If the iterator is pointing to the first element of the collection
        prior to executing this function, then calling this function will 
        set 'boc' to TRUE. The iterator will be left unchanged in this 
        situation.
   RETURNS:
        OCI_SUCCESS if the function completes successfully.
        OCI_INVALID_HANDLE if 'env' or 'err' is NULL.
        OCI_ERROR if
          any of the input parameters is null
 */

/*****************************************************************************/
/*           FUNCTIONS WHICH OPERATE ONLY ON NESTED TABLE OCITable*()        */
/*****************************************************************************/

/*---------------------------- OCITableSize ---------------------------------*/

sword OCITableSize( OCIEnv *env, OCIError *err, const OCITable *tbl, 
                    sb4 *size);
/*
   NAME: OCITableSize - OCITable return current SIZe of the given 
                   nested table (not including deleted elements)
   PARAMETERS:
        env(IN) - pointer to OCI environment handle
        err (IN/OUT) - error handle. If there is an error, it is
                recorded in 'err' and this function returns OCI_ERROR.
                The error recorded in 'err' can be retrieved by calling
                OCIErrorGet().
        tbl (IN) - nested table whose number of elements is returned
        size (OUT) - current number of elements in the nested table. The count
                does not include deleted elements.
   DESCRIPTION:
        Returns the count of elements in the given nested table. 

        The count returned by OCITableSize() will be decremented upon 
        deleting elements from the nested table. So, this count DOES NOT 
        includes any "holes" created by deleting elements. 
        For example:

            OCITableSize(...); 
            // assume 'size' returned is equal to 5
            OCITableDelete(...); // delete one element
            OCITableSize(...);
            // 'size' returned will be equal to 4

        To get the count plus the count of deleted elements use 
        OCICollSize(). Continuing the above example,

            OCICollSize(...)
            // 'size' returned will still be equal to 5
   RETURNS:
        OCI_SUCCESS if the function completes successfully.
        OCI_INVALID_HANDLE if 'env' or 'err' is NULL.
        OCI_ERROR if
          error during loading of nested table into object cache
          any of the input parameters is null
 */

/*---------------------- OCITableExists ---------------------------------*/

sword OCITableExists(    OCIEnv *env, OCIError *err, const OCITable *tbl,
                             sb4 index, boolean *exists    );
/*
   NAME: OCITableExists - OCITable test whether element at the given index
                    EXIsts
   PARAMETERS:
        env (IN/OUT) - OCI environment handle initialized in object mode.
        err (IN/OUT) - error handle. If there is an error, it is
                recorded in 'err' and this function returns OCI_ERROR.
                The error recorded in 'err' can be retrieved by calling
                OCIErrorGet().
        tbl (IN) - table in which the given index is checked
        index (IN) - index of the element which is checked for existence
        exists (OUT) - set to TRUE if element at given 'index' exists
                else set to FALSE
   DESCRIPTION:
        Test whether an element exists at the given 'index'.
   RETURNS:
        OCI_SUCCESS if the function completes successfully.
        OCI_INVALID_HANDLE if 'env' or 'err' is NULL.
        OCI_ERROR if
          any of the input parameters is null
 */

/*--------------------------- OCITableDelete -------------------------------*/

sword OCITableDelete(    OCIEnv *env, OCIError *err, sb4 index, 
                      OCITable *tbl    );
/*
   NAME: OCITableDelete - OCITable DELete element at the specified index
   PARAMETERS:
        env (IN/OUT) - OCI environment handle initialized in object mode.
        err (IN/OUT) - error handle. If there is an error, it is
                recorded in 'err' and this function returns OCI_ERROR.
                The error recorded in 'err' can be retrieved by calling
                OCIErrorGet().
        index (IN) - index of the element which must be deleted
        tbl (IN) - table whose element is deleted
   DESCRIPTION:
        Delete the element at the given 'index'. Note that the position
        ordinals of the remaining elements of the table is not changed by the
        delete operation. So delete creates "holes" in the table.

        An error is returned if the element at the specified 'index' has
        been previously deleted.
   RETURNS:
        OCI_SUCCESS if the function completes successfully.
        OCI_INVALID_HANDLE if 'env' or 'err' is NULL.
        OCI_ERROR if
          any of the input parameters is null
          given index is not valid
 */

/*--------------------------- OCITableFirst ---------------------------------*/

sword OCITableFirst(    OCIEnv *env, OCIError *err, const OCITable *tbl, 
                        sb4 *index     );
/*
   NAME: OCITableFirst - OCITable return FirST index of table
   PARAMETERS:
        env (IN/OUT) - OCI environment handle initialized in object mode.
        err (IN/OUT) - error handle. If there is an error, it is
                recorded in 'err' and this function returns OCI_ERROR.
                The error recorded in 'err' can be retrieved by calling
                OCIErrorGet().
        tbl (IN) - table which is scanned
        index (OUT) - first index of the element which exists in the given 
                table is returned
   DESCRIPTION:
        Return the first index of the element which exists in the given 
        table. 
   RETURNS:
        OCI_SUCCESS if the function completes successfully.
        OCI_INVALID_HANDLE if 'env' or 'err' is NULL.
        OCI_ERROR if
          table is empty
 */

/*---------------------------- OCITableLast ---------------------------------*/

sword OCITableLast(    OCIEnv *env, OCIError *err, const OCITable *tbl, 
                       sb4 *index     );
/*
   NAME: OCITableFirst - OCITable return LaST index of table
   PARAMETERS:
        env (IN/OUT) - OCI environment handle initialized in object mode.
        err (IN/OUT) - error handle. If there is an error, it is
                recorded in 'err' and this function returns OCI_ERROR.
                The error recorded in 'err' can be retrieved by calling
                OCIErrorGet().
        tbl (IN) - table which is scanned
        index (OUT) - last index of the element which exists in the given 
                table is returned
   DESCRIPTION:
        Return the last index of the element which exists in the given 
        table. 
   RETURNS:
        OCI_SUCCESS if the function completes successfully.
        OCI_INVALID_HANDLE if 'env' or 'err' is NULL.
        OCI_ERROR if
          table is empty
 */

/*---------------------------- OCITableNext ---------------------------------*/

sword OCITableNext(    OCIEnv *env, OCIError *err, sb4 index, 
                       const OCITable *tbl, sb4 *next_index,
                       boolean *exists    );
/*
   NAME: OCITableNext - OCITable return NeXT available index of table
   PARAMETERS:
        env (IN/OUT) - OCI environment handle initialized in object mode.
        err (IN/OUT) - error handle. If there is an error, it is
                recorded in 'err' and this function returns OCI_ERROR.
                The error recorded in 'err' can be retrieved by calling
                OCIErrorGet().
        index (IN) - starting at 'index' the index of the next element
                which exists is returned
        tbl (IN) - table which is scanned
        next_index (OUT) - index of the next element which exists 
                is returned
        exists (OUT) - FALSE if no next index available else TRUE
   DESCRIPTION:
        Return the smallest position j, greater than 'index', such that 
        exists(j) is TRUE.
   RETURNS:
        OCI_SUCCESS if the function completes successfully.
        OCI_INVALID_HANDLE if 'env' or 'err' is NULL.
        OCI_ERROR if
          no next index available
 */

/*---------------------------- OCITablePrev ---------------------------------*/

sword OCITablePrev(    OCIEnv *env, OCIError *err, sb4 index, 
                       const OCITable *tbl, sb4 *prev_index,
                       boolean *exists    );
/*
   NAME: OCITablePrev - OCITable return PReVious available index of table
   PARAMETERS:
        env (IN/OUT) - OCI environment handle initialized in object mode.
        err (IN/OUT) - error handle. If there is an error, it is
                recorded in 'err' and this function returns OCI_ERROR.
                The error recorded in 'err' can be retrieved by calling
                OCIErrorGet().
        index (IN) - starting at 'index' the index of the previous element
                which exists is returned
        tbl (IN) - table which is scanned
        prev_index (OUT) - index of the previous element which exists 
                is returned
        exists (OUT) - FALSE if no next index available else TRUE
   DESCRIPTION:
        Return the largest position j, less than 'index', such that 
        exists(j) is TRUE.
   RETURNS:
        OCI_SUCCESS if the function completes successfully.
        OCI_INVALID_HANDLE if 'env' or 'err' is NULL.
        OCI_ERROR if
          no previous index available
 */

/*------------------------ OCINumberToLnx -----------------------------------*/
/* void OCINumberToLnx(/o_ OCINumber *num _o/); */

#define OCINumberToLnx(num) ((lnxnum_t *)num)

/* 
   NAME:   OCINumberToLnx
   PARAMETERS:
           num (IN) - OCINumber to convert ;
   DESCRIPTION: 
           Converts OCINumber to its internal lnx format 
           This is not to be used in Public interfaces , but
           has been provided due to special requirements from 
           SQLPLUS development group as they require to call 
           Core funtions directly . 
*/

/* OCI representation of XMLType */
typedef struct OCIXMLType OCIXMLType;

/* OCI representation of OCIDomDocument */
typedef struct OCIDOMDocument OCIDOMDocument;

/* OCI representation for the Binary XML repository context */
typedef struct OCIBinXmlReposCtx OCIBinXmlReposCtx;

#endif /* ORL_ORACLE */
