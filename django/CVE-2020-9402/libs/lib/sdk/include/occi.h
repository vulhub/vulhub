/* Copyright (c) 2000, 2002, Oracle Corporation.  All rights reserved.  */
 
/* 
   NAME 
     occi.h - Oracle C++ Interface header files.

   DESCRIPTION 
     <short description of facility this file declares/defines> 

   RELATED DOCUMENTS 
     <note any documents related to this facility>
 
   EXPORT FUNCTION(S) 
     <external functions declared for use outside package - one-line 
      descriptions>

   INTERNAL FUNCTION(S)
     <other external functions declared - one-line descriptions>

   EXAMPLES

   NOTES
     <other useful comments, qualifications, etc.>

   MODIFIED   (MM/DD/YY)
   vvinay      08/19/02 - 
   aahluwal    06/03/02 - bug 2360115
   gayyappa    01/03/01 - removed inclusions before occiCommon.h
   kmohan      04/11/00 - include oci.h and occiCommon.h also
   rkasamse    04/03/00 - header file for all the OCCI classes
   rkasamse    04/03/00 - Creation

*/

#ifndef OCCI_ORACLE
# define OCCI_ORACLE

#ifndef OCCICOMMON_ORACLE
#include <occiCommon.h>
#endif

#ifndef OCCIDATA_ORACLE
#include <occiData.h>
#endif

#ifndef OCCICONTROL_ORACLE
#include <occiControl.h>
#endif

#ifndef OCCIOBJECTS_ORACLE
#include <occiObjects.h>
#endif

#ifndef OCCIAQ_ORACLE
#include <occiAQ.h>
#endif

/*---------------------------------------------------------------------------
                     PUBLIC TYPES AND CONSTANTS
  ---------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------
                     PRIVATE TYPES AND CONSTANTS
  ---------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------
                           EXPORT FUNCTIONS
  ---------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------
                          INTERNAL FUNCTIONS
  ---------------------------------------------------------------------------*/


#endif                                              /* OCCI_ORACLE */
