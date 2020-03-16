/* Copyright (c) 1994, 2006, Oracle. All rights reserved.  */

/*
  Author:             Tin Nguyen
  Date:               02/07/94
  Source documents:   "Functional Specification for C Object Interface, Object
                       Management Subsystem", "Oracle C Coding Standards
                       version 2.2", and the header file template
  Rule sets:          the generic and .h file rule sets
  Quality status:     not exited
  Identification tag: [ one or more letters to identify the .h file ] 
  Revision code:      [ date of the last revision of the .h file ]

  Note to the user of this header file:

    Anything in this header file that is marked private is not supported and
    must not be used.  Private sections are included in the header file to 
    improve internal maintenance.

  NAME

    ORID - Oracle Object Interface for Dynamic Data Access

  DESCRIPTION

    This file contains declarations for C object interface functions including
    the dynamic object data access operations that allow callers to dynamically
    access and manipulate objects; these operations include getting and setting
    attributes of an object.  These dynamic object operations are for accessing
    and manipulation objects whose types are not known at compile-time.

  RELATED DOCUMENTS
  
     Functional Specification for C Object Interface / Object Management System

  PUBLIC FUNCTIONS

    OCIObjectSetAttr - ORID SET attribute value
    OCIObjectGetAttr - ORID GET attribute value

  PRIVATE FUNCTIONS

    None

  EXAMPLES 

    EXAMPLE 1

    /o
     o This example illustrates how an interative program can use the dynamic
     o attribute access to display and modify attributes of an ADT instance.
     o The interactive program does not know the type of the object at 
     o compile time.
     o/

    void display(adt_ref, object, null_struct, names, names_count, 
                          names_length, indexes, indexes_count)
    {
      /o Pin the ADT o/
      if (OCIObjectPin(env, &adt_ref, OROOPOCUR, OROOPDTRA, OROOLMNON, &adt) 
            != OROSTASUC)
         /o error handling code o/ 

      /o 
       o Call the type manager to obtain all the attributes in the object.
       o Display the content of each attribute in the ADT instance. If the 
       o attribute is an array, display each element of the array. If the
       o attribute is an ADT instance, recursively call this routine to
       o display the embedded ADT instance.
       o/ 
      numAttrs = OCITypeAttrs(env, adt);
      for (i= 1; i <= numAttrs; i++)
      {
         /o get attribute descriptor o/
         if (ortgabp(env, adt, i, &ado_ref, &ado) != OROSTASUC)
           /o error handling code o/ 

         /o get attribute name o/
         names[names_count] = OCITypeElemName(env, ado, 
                   &names_length[names_count]);
      
         /o dynamically get the attr o/
         if (OCIObjectGetAttr(env, object, null_struct, 0, adt_ref, names, 
                     names_length, names_count+1, indexes, indexes_count, 0, 
                     &null, &null_info, &attr) != OROSTASUC)
            /o error handling code o/  

         /o check if attribute is null o/
         if (null) continue;

         /o get typecode of attribute o/
         typecode = OCITypeElemTypeCode(env, ado);

         /o if attribute is a varray, display each element in varray o/
         if (typecode == OCI_TYPECODE_VARRAY)
         {
            /o get the reference to the type of the element of the array o/ 
            if (OCITypeElemParameterizedTyper(env, ado, &attr_type_ref) 
               != OROSTASUC)
               /o error handling code o/ 

            /o get the size of array o/
            if (orlasiz(env, &attr_type_ref, (orlva *)attr,  
                         &numElm) != OROSTASUC)
               /o error handling code o/  

            /o get the typecode of the element of the array o/
            if (ortty2r(env, attr_type_ref, &typecode) != OROSTASUC)
               /o error handling code o/  

            /o iterate the array o/
            for (j=0; j < numElm; j++)
            {
              /o get an element in the array o/
              if (OCIObjectGetAttr(env, attr, null_info, j+1, attr_type_ref,
                  names, names_length, 0, indexes, 0, 0, &null, &null_info, 
                  &element) != OROSTASUC)
                  /o error handling code o/  

              /o check if element is null o/
              if (null) continue;

              /o if attr is an ADT instance, recursively call this routine o/
              if (typecode == OCI_TYPECODE_ADT || typecode == 
                  OCI_TYPECODE_UNNAMEDADT) 
              {
                /o display the element as an adt o/
                display(attr_type_ref, element, null_info, names, lengths, 
                   0, indexes, 0);
              }
      
              /o if attribute is scalar, print the value to the screen o/
              else output_to_screen(element, typecode);
            }
         }

         /o if attribute is an ADT instance, recursively call this routine o/
         else if (typecode == OCI_TYPECODE_ADT || typecode == 
              OCI_TYPECODE_UNNAMEDADT)
         {
            /o get the type ref of the attribute o/
            if (ortgarf(env, ado, &attr_type_ref) != OROSTASUC)
               /o error handling code o/ 

             display(attr_type_ref, attr, null_info, 0, names, 0, names_length,
                      indexes, 0);
         }

         /o if attribute is scalar, print the value to the screen o/
         else output_to_screen(attr, typecode);
      }
    }

    /o ******** main routine *********** o/
    .... 

    /o 
     o Allocate the arrays for storing the path expression 
     o/

    /o get the tdo of type 'long' o/
    if (orttypget(&env, con, "SYS", sizeof("SYS"), "SINT32", sizeof("SINT32"), 
          OROOPDSES, &long_ref, &long_tdo) != OROSTASUC)
          /o error handling code o/

    /o get the tdo of type 'varchar' o/
    if (orttypget(&env, con, "SYS", sizeof("SYS"), "SQL_VARCHAR2", 
          sizeof("SQL_VARCHAR2"), OROOPDSES, &vchar_ref, &vchar_tdo) 
          != OROSTASUC)
          /o error handling code o/

    /o allocate the varrays for the path expression o/ 
    if (orlalloc(env, &vchar_ref, MAX_ARR_SIZE, &attr_names) != OROSTASUC)
       /o error handling code o/ 

    if (orlalloc(env, &long_ref, MAX_ARR_SIZE, &attr_name_lengths) 
                != OROSTASUC)
       /o error handling code o/ 

    if (orlalloc(env, &long_ref, MAX_ARR_SIZE, &attr_name_indexes) 
                != OROSTASUC)
       /o error handling code o/ 

    /o 
     o Get an ADT instance. The ref to the ADT instance can be obtained
     o by through ORI or OSCI. 
     o/
    if (OCIObjectPin(env, &obj_ref, OROOPOCUR, OROOPDTRA, OROOLMUPD, &object) 
          != OROSTASUC)
       /o error handling code o/ 

    /o get the null structure of the ADT instance o/
    if (OCIObjectGetInd(gp, object, &null_struct) != OROSTASUC)
       /o error handling code o/ 

    /o 
     o Get the type of the ADT instance
     o/

    /o find out the type of the ADT instance o/
    if (oriogto(env, object, &adt_ref) != OROSTASUC)
       /o error handling code o/ 

    /o display the object o/
    display(adt_ref, object, null_struct, attr_names, 0, attr_names_lengths,
                     attr_names_indexes, 0);

    /o After the object is displayed, the program waits for the user to 
     o respond.  The user modifies the values of an attribute and the
     o program generates a path expression for the attribute and calls 
     o OCIObjectSetAttr() to set the value. 
     o/

    if (OCIObjectSetAttr(env, object, null_struct, adt_ref, 
                (text **)attr_names, (ub4 *)attr_name_lengths, 
                attr_names_count, (ub4 *)attr_array_indexes, 
                attr_array_indexes_count, 
                (void *)0, FALSE, (void *)value) != OROSTASUC)
       /o error handling code o/ 

    END OF EXAMPLE 1

  NOTES

    This file has been subsetted to contain only the routines that will
    be in the first release.

  MODIFIED
    dmukhin    06/29/05  - ANSI prototypes; miscellaneous cleanup 
    srseshad   03/12/03  - convert oci public api to ansi
    aahluwal   06/03/02  - bug 2360115
    bpalaval   02/09/01  - Change text to oratext.
    whe        09/01/99 -  976457:check __cplusplus for C++ code
    sthakur    09/18/97 -  collection indexing not supported
    cxcheng    08/05/97 -  fix compile with short names
    skrishna   03/18/97 -  fix ifdef for supporting ansi and k&r proto-types
    cxcheng    02/06/97 -  take out short name support except with SLSHORTNAME
    cxcheng    10/17/96 -  final renaming of functions
    jboonleu   10/07/96 -  beautify with OCI long names
    cxcheng    10/07/96 -  change short names to long names for readability
    jboonleu   09/27/96 -  fix lint
    jwijaya    07/03/96 -  add ANSI prototypes
    jboonleu   04/13/95 -  new interface
    jwijaya    10/11/94 -  fix the sccs header and add namespace
    tanguyen   08/22/94 -  fix example 
    tanguyen   08/09/94 -  remove Sccsid declaration 
    tanguyen   07/20/94 -  fix OCIObjectSetAttr and OCIObjectGetAttr to 
                           use position descriptor 
    tanguyen   07/18/94 -  change 'object' type to become ptr to object 
    tanguyen   06/30/94 -  Fix the ORID_ORACLE ifdef 
    tanguyen   06/27/94 -  update to template format 
    skotsovo   05/12/94 -  replace ado with attribute position 
    jweisz     05/11/94 -  test new checkin facility 
    jwijaya    05/05/94 - orienv/ref/typ -> oroenv/ref/typ
    jwijaya    02/07/94 - Creation

*/

#ifndef ORATYPES
#include <oratypes.h>
#endif
#ifndef ORO_ORACLE
#include <oro.h>
#endif
#ifndef OCI_ORACLE
#include <oci.h>
#endif

#ifndef ORID_ORACLE
#define ORID_ORACLE

#ifdef SLSHORTNAME

#define OCIObjectSetAttr oridset
#define OCIObjectGetAttr oridget

#endif /* SLSHORTNAME */

/*---------------------------------------------------------------------------*/
/*                           PUBLIC FUNCTIONS                                */
/*---------------------------------------------------------------------------*/

/*-------------------------- OCIObjectSetAttr ----------------------------*/
sword OCIObjectSetAttr(    OCIEnv *env, OCIError *err, void  *instance, 
                  void  *null_struct, struct OCIType *tdo, 
                  const oratext **names, const ub4 *lengths, 
                  const ub4 name_count, const ub4 *indexes, 
                  const ub4 index_count, const OCIInd null_status, 
                  const void  *attr_null_struct, const void  *attr_value    );
/*
   NAME: OCIObjectSetAttr - ORID SET value
   PARAMETERS:
        env  (IN) - OCI environment handle initialized in object mode
        err  (IN) - error handle. If there is an error, it is
                        recorded in 'err' and this function returns OCI_ERROR.
                        The error recorded in 'err' can be retrieved by calling
                        OCIErrorGet().
        instance    (IN) - pointer to an ADT instance
        null_struct (IN) - the null structure of the ADT instance or array
        tdo         (IN) - pointer to the TDO
        names       (IN) - array of attribute names. This is used to specify
                           the names of the attributes in the path expression.
        lengths     (IN) - array of lengths of attribute names.
        name_count  (IN) - number of element in the array 'names'.
        indexes     (IN) [OPTIONAL] - currently NOT SUPPORTED, pass (ub4 *)0.
        index_count (IN) [OPTIONAL] - currently NOT SUPPORTED, pass (ub4)0.
        attr_null_status (IN) - the null status of the attribute if the type of
                                 attribute is primitive.
        attr_null_struct (IN) - the null structure of an ADT or collection
                                 attribute.
        attr_value       (IN) - pointer to the attribute value.
   REQUIRES:
   DESCRIPTION:
        This function set the attribute of the given object with the given 
        value.  The position of the attribute is specified as a path 
        expression which is an array of names and an array of indexes. 
   RETURNS:
        one of OROSTA*
   EXAMPLES:
        For path expression stanford.cs.stu[5].addr, the arrays will look like
          names = {"stanford", "cs", "stu", "addr"} 
          lengths = {8, 2, 3, 4}
          indexes = {5}
        
        Also see the above example.
 */

/*-------------------------- OCIObjectGetAttr ----------------------------*/
sword OCIObjectGetAttr(    OCIEnv *env, OCIError *err, void  *instance, 
                  void  *null_struct, struct OCIType *tdo, 
                  const oratext **names, const ub4 *lengths, 
                  const ub4 name_count, const ub4 *indexes, 
                  const ub4 index_count, OCIInd *attr_null_status, 
                  void  **attr_null_struct, void  **attr_value, 
                  struct OCIType **attr_tdo    );
/*
   NAME: OCIObjectGetAttr - ORID GET value
   PARAMETERS:
        env  (IN) - OCI environment handle initialized in object mode
        err  (IN) - error handle. If there is an error, it is
                        recorded in 'err' and this function returns OCI_ERROR.
                        The error recorded in 'err' can be retrieved by calling
                        OCIErrorGet().
        instance    (IN) - pointer to an ADT instance 
        null_struct (IN) - the null structure of the ADT instance or array
        tdo         (IN) - pointer to the TDO 
        names       (IN) - array of attribute names. This is used to specify
                           the names of the attributes in the path expression.
        lengths     (IN) - array of lengths of attribute names. 
        name_count  (IN) - number of element in the array 'names'.
        indexes     (IN) [OPTIONAL] - currently NOT SUPPORTED, pass (ub4 *)0.
        index_count (IN) [OPTIONAL] - currently NOT SUPPORTED, pass (ub4)0.
        attr_null_status (OUT) - the null status of the attribute if the type 
                                 of attribute is primitive. 
        attr_null_struct (OUT) - the null structure of an ADT or collection
                                 attribute.
        attr_value       (OUT) - pointer to the attribute value.
        attr_tdo         (OUT) - pointer to the TDO of the attribute.
   REQUIRES:
      - a valid OCI environment handle must be given.
   DESCRIPTION:
        This function gets a value from an ADT instance or from an array.
        If the parameter 'instance' points to an ADT instance, then the path
        expression specifies the location of the attribute in the ADT. 
        It is assumed that the object is pinned and that the value returned 
        is valid until the object is unpinned.
   RETURNS:
        one of OROSTA*
   EXAMPLES:
        See example in OCIObjectSetAttr(). Also see the above example.
 */

#endif /* ORID_ORACLE */
