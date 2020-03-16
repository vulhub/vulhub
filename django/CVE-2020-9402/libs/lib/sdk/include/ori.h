/* Copyright (c) 1994, 2006, Oracle. All rights reserved.  */

/*
  NAME
    ORI - OCI navigational interface

  DESCRIPTION

    This section is intended to give a brief introduction to the navigational
    interfaces.  Readers can refer to the documents listed in the section 
    'RELATED DOCUMENTS' for more information. 

    PURPOSE 
       The Oracle Call Interface (OCI) supports navigational access of objects.
       In the navigational paradigm, data is represented as a graph of objects 
       connected by references.  Objects in the graph are reached by following
       the references.

    OBJECT ENVIRONMENT

      The object environment is initialized when the OCI environment handle is
      initialized with the object option.  An object environment contains a
      heap which buffers type instances in memory.  The object environment also
      contains an object cache which keeps track of the objects in the object 
      environment.  Readers can refer to the "Functional Specification for 
      Programmatic Interface" for more information about the object 
      environment. 

    INSTANCE, OBJECT AND VALUE

      An OTS instance is an occurence of a type specified by the Oracle Type 
      System (OTS). This section describes how an OTS instance can be 
      represented in OCI. In OCI, an OTS instance can be classified based on
      the type, the lifetime and referencability (see the figure below):

      1) A persistent object is an instance of an object type. A persistent
         object resides in a row of a table in the server and can exist longer
         than the duration of a session (connection). Persistent objects can be
         identified by object references which contain the object identifiers.
         A persistent object is obtained by pinning its object reference.

      2) A transient object is an instance of an object type. A transient 
         object cannot exist longer than the duration of a session, and it is 
         used to contain temporary computing results. Transient objects can 
         also be identified by references which contain transient object
         identifiers.

      3) A value is an instance of an user-defined type (object type or 
         collection type) or any built-in OTS type.  Unlike objects, values of 
         object types are identified by memory pointers, rather than by 
         references.  

         A value can be standalone or embbeded.  A standalone value is usually
         obtained by issuing a select statement.  OCI also allows the client
         program to select a row of object table into a value by issuing a SQL
         statement.  Thus, a referenceable object (in the database) can be 
         represented as a value (which cannot be identified by a reference).
         A standalone value can also be an out-of-line attribute in an object 
         (e.g varchar, raw) or an out-of-line element in a collection (e.g.
         varchar, raw, object).
      
         An embedded value is phyiscally included in a containing instance.
         An embedded value can be an in-line attribute in an object (e.g.
         number, nested object) or an in-line element in a collection.

         All values are considered to be transient by OCI, e.g. OCI does not
         support automatic flushing a value to the database, and the client has
         to explicitly execute a SQL statement to store a value into the 
         database. For embedded values, they are flushed when their containing
         instance are flushed.


                                OTS instance
                                 |        |  
                                 v        v
                               object    value         (type)
                               |    |
                               v    v
                       persistent  transient           (lifetime)
                  
 
                     persistent obj   transient obj     value  
      ---------------------------------------------------------------
      |              |              |               |  object type, |
      | type         | object type  |  object type  |  built-in,    |
      |              |              |               |  collection   |
      ---------------------------------------------------------------
      | maximum      | until object |  session      | session       |
      | lifetime     | is deleted   |               |               | 
      ---------------------------------------------------------------
      | referencable |    yes       |     yes       |    no         |    
      ---------------------------------------------------------------
      | embeddable   |    no        |     no        |    yes        |    
      ---------------------------------------------------------------

    REFERENCEABLE OBJECT, STANDALONE OBJECT, EMBEDDED OBJECT

       In the reminding of this include file, the following term will be used:
       1) The term 'object' can be generally referred to a persistent object,
          a transient object, a standalone value of object type, or an embedded
          value of object type.
       2) The term 'referenceable object' refers to a persistent object or a 
          transient object.
       3) The term 'standalone object' refers to a persistent object, a
          transient object or a standalone value of object type.
       4) The term 'embedded object' referes to a embbeded value of object 
          type.

    META ATTRIBUTES 

      There is a set of meta-attributes that are defined for standalone 
      objects.  A meta-attribute can be transient or persistent. A 
      transient meta-attribute is applicable to an instance only when it is 
      in memory. A persistent meta-attribute can be applicable to an instance 
      that is in the disk. 

      The set of user visible meta-attributes for persistent objects are:
      1) existent (persistent) : Does the object exist?
      2) nullness (persistent) : Null information of the instance
      3) locked   (persistent) : Is the object locked?
      4) pinned    (transient) : Is the object being accessed by the client?
      5) dirty     (transient) : Has the object been modified?
      6) allocation duration (transient) : see below
      7) pin duration        (transient) : see below

      The set of user visible meta-attributes for transient objects are:
      1) existent  (transient) : Does the object exist?
      2) nullness  (transient) : Null information of the instance 
      3) pinned    (transient) : Is the object being accessed by the client? 
      4) dirty     (transient) : Has the object been modified?
      4) allocation duration (transient) : see below
      5) pin duration        (transient) : see below

      The set of user visible meta-attributes for standalone values of object  
      type or collections are:
      1) allocation duration (transient) : see below
      2) nullness            (transient) : Null information of the instance 
                                           (of an object type)

    NULLNESS OF AN INSTANCE 

      Each standalone object is associated with a null structure which keeps 
      the null information about the object.  A null indicates the absence of 
      data. The null structure itself contains null indicators that represent:
        1) atomic nullness : a null value that pertains to the whole object 
        2) null status of the individual attribute in the object 

      The layout of a null structure in memory resembles that of the object,
      except that the null structure has additional indicators to represent 
      the atomic nullness for each object. 

      An non-existent object is different than an object that is atomically 
      null. A atomically null object is an existing object that has no data. 

    MEMORY LAYOUT OF AN OBJECT

      A standalone object in memory is composed of a top level memory chunk, 
      a null structure and optionally, a number of secondary memory chunks. 
      For a DEPARTMENT object type,

          OBJECT TYPE department
          {
              dep_name      varchar2(20),
              budget        number,
              manager       person,              /o person is an object type o/
              employees     collection of person
          }

      Each instance of DEPARTMENT will has a top level memory chunk which
      contains the top level attributes such as dep_name, budget, manager and
      employees.  The attributes dep_name and employees are themselves pointers
      to the additional memory (the secondary memory chunks). The secondary
      memory is for the out-of-line attribute (e.g. varray).

    CONSISTENCY MODEL

      Each pin operation behaves like a distinct SQL select.  Thus, the object 
      cache does not guarantee consistency for a graph of objects.  In order to
      retrieve a consistent graph of objects, the user has to explicitly start 
      a serializable transaction or a read-only transaction. 

    DURATION
      In OCI, a duration is used to specify 

        1) the length of memory allocation of an instance 
           When each instance is allocated, it is associate with an allocation
           duration.  The memory occupied by the object is freed automatically 
           at the end of its allocation duration. The allocation duration of an
           instance cannot be changed.  

        2) the length of pinning of an object
           When each object is pinned, the client has to give a pin duration 
           which specify the length of time that the object is intended to be
           used.  It is an user error to specify a pin duration longer than an
           allocation duration of the object. An object is completely unpinned 
           at the end of its pin duration (see OCIObjectUnpin()). 

      An OCI program can use the allocation duration and the pin duration to 
      automatically free the memory of the instances:
       1) Transient objects and values are freed at the end of the allocation
          duration.
       2) Persistent objects ARE freed at the end of the allocation duration.
          Persistent objects CAN be freed at the end of the pin duration when
          the objects are completely unpinned. The persistent objects are said
          to be aged out. See OCIObjectUnpin() for more details.

      There are 3 predefined duration: session, transaction, call.  The time 
      spans of these durations are defined based on the programming model 
      presented by OCI. The call duration is mapped to the transaction 
      duration in the client-side environment. See oro.h for the macros defined
      for these 3 durations.

      A pin duration can be promoted. For example, if an object is pinned with
      duration 1, and the object is later pinned with duration 2, the pin 
      routine will try to find a duration that is longer or equal to the 
      length of both duration 1 and duration 2.  The pin duration of the object
      is set to the that duration. The object is automatically unpinned only 
      after both duration 1 and duration 2 are terminated. 

  RELATED DOCUMENTS
    "Functional Specification for Oracle Object RDBMS" 
    "Functional Specification for Programmatic Interfaces" 
    "Functional Specification for the Oracle Type System (OTS)" 

  INSPECTION STATUS 
    Inspection date:
    Inspection status:
    Estimated increasing cost defects per page:
    Rule sets:        

  ACCEPTANCE REVIEW STATUS 
    Review date:   
    Review status:
    Reviewers: 

  PUBLIC FUNCTIONS
    OCIObjectNew - OCI new a standalone instance 
    OCIObjectPin - OCI pin an object by reference
    OCIObjectUnpin - OCI unpin a referenceable object
    OCIObjectPinCountReset - OCI reset the pin count of a referenceable object 
    OCIObjectLock - OCI lock a persistent object
    OCIObjectLockNoWait - OCI lock a persistent object
    OCIObjectMarkUpdate - OCI mark a referenceable object as updated
    OCIObjectUnmark - OCI unmark a dirtied referenceable object 
    OCIObjectUnmarkByRef - OCI unmark a dirtied object by reference 
    OCIObjectFree - OCI free a standalone instance 
    OCIObjectMarkDelete - OCI mark a referenceable object as deleted 
    OCIObjectMarkDeleteByRef - OCI mark a referenceable object as deleted by 
                               giving a reference
    OCIObjectFlush - OCI flush a persistent object
    OCIObjectRefresh - OCI refresh a persistent object 
    OCIObjectCopy - OCI CoPy one object to another
    OCIObjectGetTypeRef - OCI get the Type Reference of a standalone object 
    OCIObjectGetObjectRef - OCI get the Object's Reference 
    OCIObjectGetInd - OCI get Null Structure of an standalone object 
    OCIObjectExists - OCI get the existence of a referenceable object
    OCIObjectGetProperty - get object property
    OCIObjectIsLocked - OCI get the lock status of a referenceable object
    OCIObjectIsDirty - OCI get the dirty status of a referenceable object
    OCIObjectPinTable - OCI get Table object 
    OCIObjectArrayPin - OCI pin array of objects 
    OCIObjectGetPrimayKeyTypeRef - OCI get the Ref for the primary key OID's 
                                   type 
    OCIObjectMakeObjectRef - OCI Create a pk or sys generated REF  

    OCIObjectGetNewOID - OCI Create a new Object ID

    OCICacheFlush - OCI flsuh the modified persistent objects in the cache
    OCICacheRefresh - OCI refresh persistent objects in the cache 
    OCICacheUnpin - OCI unpin referenceable objects in the cache
    OCICacheFree - OCI free all instances in the environment
    OCICacheUnmark - OCI unmark all dirty referenceable objects in the cache 

  PRIVATE FUNCTIONS
    None

  EXAMPLES 

    The following types will be used in the examples in this section: 

    OBJECT TYPE professor
    (
        varchar2  name;
        number    department;
        number    num_of_students; 
    );

    OBJECT TYPE course 
    (
        varchar2   name;
        number     grade; 
    );

    OBJECT TYPE student
    (
        vstring      name;
        number       department;
        ref          advisor;                      /o advisor is a professor o/
        collection   courses;
    );

    EXAMPLE 1

      Here is a set of examples to illustrate the usages of some of the
      orio and oric functions.  

      OCIenv    *env;              /o OCI environment handle o/
      OCIError  *err;              /o OCI error handle o/
      OCISvcCtx *svc;              /o OCI service handle o/

      void   *stu_tbl;            /o pointer to the student table o/
      OCIType *stu_tdo;            /o student type tdo o/

      OCIRef    *stu2_ref;         /o object reference to student object o/
      student   *stu1;             /o pointer to the student object o/
      student   *stu2;             /o pointer to the student object o/
      professor *pro;              /o pointer to the professor object o/

      /o Initialize the OCI environment handle, error handle and service
         handle and login to the database o/ 
      ...

      /o CREATE A PERSISTENT OBJECT o/

      /o get the table object of student o/
      if (OCIObjectPinTable(env, err, svc, "ORACLEU", sizeof("ORACLEU"), 
          "STUDENT_TABLE", sizeof("STUDENT_TABLE"), (OCIRef *)0, 
          OCI_DURATION_NULL, &stu_tbl) != OCI_SUCCESS)
          /o error handling code o/ 

      /o get type object of student o/
      if (OCITypeByName(env, err, svc, "ORACLEU", sizeof("ORACLEU"), 
          "STUDENT", sizeof("STUDENT"), OCI_DURATION_NULL, OCI_TYPEGET_HEADER,
          &stu_tdo) != OCI_SUCCESS)
          /o error handling code o/ 

      /o create a persistent object 'mark' (of type student) o/ 
      if (OCIObjectNew(env, err, svc, OCI_TYPECODE_ADT, stu_tdo, stu_tbl, 
            OCI_DURATION_TRANS, (ub1)FALSE, (void **)&stu1) != OCI_SUCCESS)
          /o error handling code o/

      /o RETRIEVE OBJECTS IN PERSISTENT STORES o/ 

      /o Use OCI to retrieve a reference to student object 'joe'.
       o The retrieved reference is bound to the variable stu2_ref.
       o/ 

      /o pin/retrieve the student "joe" by reference o/ 
      if (OCIObjectPin(env, err, &stu2_ref, (OCIComplexObject *)0, OCI_PIN_ANY,
                  OCI_DURATION_TRANS, OCI_LOCK_X, &stu2) != OCI_SUCCESS)
          /o error handling code o/ 

      /o pin/retrieve the advisor of student "joe" by reference o/ 
      if (OCIObjectPin(env, err, &stu2->advisor, (OCIComplexObject *)0,
          OCI_PIN_ANY, OCI_DURATION_TRANS, OCI_LOCK_X, &pro) != OCI_SUCCESS)
          /o error handling code o/ 

      /o MODIFY OBJECTS o/

      /o initialize the newly created object "mark" o/
      DISCARD OCIStringAssignText(env, err, "mark", sizeof("mark"), 
                                    &stu1->name);
      department = 522;
      DISCARD OCINumberFromInt(err, &department, sizeof(department), 
                                    OCI_NUMBER_UNSIGNED, &stu1->department);

      /o assign advisor to student "mark" o/
      DISCARD OCIRefAssign(env, err, &stu2->advisor, &stu1->advisor);

      /o update student "joe". o/  
      department = 533;
      DISCARD OCINumberFromInt(err, &department, sizeof(department), 
                                    OCI_NUMBER_UNSIGNED, &stu2->department);
      DISCARD OCIObjectMarkUpdate(env, err, stu2);

      /o UNPIN OBJECTS AFTER FINSIHED PROCESSING THEM o/ 

      /o unpin the student object "mark" o/
      if (OCIObjectUnpin(env, err, stu1) != OCI_SUCCESS)
          /o error handling code o/ 

      /o unpin the student object "joe" o/
      if (OCIObjectUnpin(env, err, stu2) != OCI_SUCCESS)
          /o error handling code o/ 

      /o unpin the professor object o/
      if (OCIObjectUnpin(env, err, pro) != OCI_SUCCESS)
          /o error handling code o/ 

      /o unpin the type object o/
      if (OCIObjectUnpin(env, err, stu_tdo) != OCI_SUCCESS)
          /o error handling code o/ 

      /o unpin the table object o/
      if (OCIObjectUnpin(env, err, stu_tbl) != OCI_SUCCESS)
          /o error handling code o/ 

      /o FLUSH MODIFIED OBJECTS BACK TO PERSISTENT STORE o/

      if (OCICacheFlush(env, err, svc, (void *)0, ((OCIRef*)(*)())0, 
                       (OCIRef *)0) != OCI_SUCCESS)
          /o error handling code o/

      /o commit transaction o/

    END OF EXAMPLE 1

  NOTES
    This file has been subsetted to contain only the routines that will
    be in the first release.

  MODIFIED
    dmukhin    06/29/05 - ANSI prototypes; miscellaneous cleanup 
    srseshad   03/12/03 - convert oci public api to ansi
    aahluwal   06/03/02 - bug 2360115
    bpalaval   02/09/01 - Change text to oratext.
    rkasamse   06/21/00 - add ociobjectgetnewoid
    rkasamse   05/24/00 - add OCIObjectSetData
    whe        09/01/99 - 976457:check __cplusplus for C++ code
    smuralid   10/29/98 - add comments for OCIObjectMakeObjectRef              
    mkrishna   08/19/98 - change OCIGetPkTypeRef to OCIObjectGetPrimaryKeyTypeR
    mkrishna   08/10/98 - add OCIObjectMakeObjectRef & OCIObjectGetPkTypeRef
    rkasamse   06/22/98 - add comments for OCIDurationBegin(End)
    pmitra     04/01/98 - OCIObjectLockNoWait added                            
    pmitra     11/05/97 - [573769] OCIObjectArrayPin pos parameter cannot be NU
    cxcheng    07/29/97 - fix compile for short names
    skrishna   07/14/97 - add OCIObjectGetProperty
    skrishna   04/30/97 - OCIObjectFlushRefresh: remove duplicate declaration
    skrishna   04/24/97 - flag unsupported functions
    sthakur    03/20/97 - modify flag argument to OCIObjectFree
    skrishna   03/18/97 - fix ifdef for supporting ansi and k&r proto-types
    cxcheng    02/19/97 - remove short names support
    cxcheng    02/06/97 - take out short name support except with SLSHORTNAME
    sthakur    12/20/96 - fix a typepo in OCIOBjectArrayPin
    jboonleu   11/07/96 - modify comments
    cxcheng    10/28/96 - more beautification changes
    jboonleu   10/24/96 - add flag to OCIObjectFree
    jboonleu   10/22/96 - change interface of OCICacheFlush
    cxcheng    10/18/96 - rename OCIObjectPinArray to OCIObjectArrayPin
    cxcheng    10/14/96 - more renaming of types
    jboonleu   10/09/96 - add new interfaces
    cxcheng    10/09/96 - more lint fixes
    cxcheng    10/08/96 - more lint fixes
    jboonleu   09/27/96 - fix lint errors
    jboonleu   10/07/96 - beautify ori.h after conversion to long names
    cxcheng    10/04/96 - replace short names with long names
    sthakur    08/20/96 - add COR context to OCIObjectPin
    mluong     07/17/96 - add back orioglk, oriogdr, oriogiv, and oriocur.
    jboonleu   07/17/96 - rename refresh option to conherency option 
    jboonleu   07/16/96 - change comment for cache consistency
    jwijaya    07/03/96 - add ANSI prototypes
    jboonleu   06/12/96 - update comment
    jboonleu   05/08/96 -  change description of OCIDurationGetParent
    jboonleu   05/01/96 -  add OROOCOSFN
    skrishna   04/08/96 -  change ori*() to take OCIEnv* and OCIError* instead
                           of oroenv*
    jboonleu   01/04/96 -  interface change
    jboonleu   10/24/95 -  support of variable ref
    jboonleu   02/15/95 -  new interface
    sthakur    01/05/95 -  pass username to origrgc 
    skotsovo   12/07/94 -  update example 
    jwijaya    11/15/94 -  rename ORONSPTAB to ORONSPEXT 
    jwijaya    10/06/94 -  add namespace to oriopnm() 
    jwijaya    10/02/94 -  connection handle -> connection number 
    jboonleu   08/16/94 -  fix lint errors 
    jboonleu   07/20/94 -  change interface of OCICacheFlush 
    tanguyen   07/18/94 -  add oriocpe, change OCIObjectCopy to oriocps
    tcheng     07/15/94 -  add init param maximum_sga_heap_size 
    tcheng     07/13/94 -  change origini to get param string 
    jboonleu   07/05/94 -  change sccs string from sccid to a comment 
    jboonleu   07/01/94 -  Add examples to ORIO* and ORIC* functions 
    tanguyen   06/30/94 -  Fix the ORI_ORACLE ifdef
    skotsovo   06/27/94 -  include all public functions in public functions 
                           list at top of header file
    tcheng     06/27/94 -  modify comments according to new template 
    tanguyen   06/24/94 -  fix comments for OCIObjectCopy 
    tcheng     06/24/94 -  fix comments in origrgc()
    tanguyen   06/21/94 -  fix comments and format 
    tcheng     06/20/94 -  commenting origini/trm/err/rgc/urg() functions
    tanguyen   06/16/94 -  fix descriptions of ref operations 
    tanguyen   06/16/94 -  clarifies refs comparison 
    tanguyen   05/12/94 -  adds more interfaces (OCIObjectMarkUpdate)
    jwijaya    05/10/94 -  fix examples, add origurg, change origcon to origrgc
    tanguyen   05/03/94 -  remove unnecessary 'type' argument from 
                           'OCIObjectCopy'
    tanguyen   03/08/94 -  clarifies comments
    jwijaya    02/16/94 -  more questions
    jwijaya    02/11/94 -  more comments
    jwijaya    02/10/94 -  identify optional arguments
    jwijaya    02/07/94 -  Creation
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
#ifndef ORT_ORACLE
#include <ort.h>
#endif

#ifndef ORI_ORACLE
#define ORI_ORACLE

/*---------------------------------------------------------------------------*/
/*                         SHORT NAMES SUPPORT SECTION                       */
/*---------------------------------------------------------------------------*/

#ifdef SLSHORTNAME

/* the following are short names that are only supported on IBM mainframes
   with the SLSHORTNAME defined.
   With this all subsequent long names will actually be substituted with
   the short names here */

#define OCIDurationBegin                 origbgu
#define OCIDurationEnd                   origedu
#define OCIDurationGetParent             origpdr
#define OCICacheFlushRefresh             oricfrh
#define OCICacheUnpin                    oricunp
#define OCICacheFree                     oricfre
#define OCICacheUnmark                   oricumk
#define OCICacheGetObjects               oricgpr
#define OCICacheRegister                 oricscb
#define OCIObjectUnpin                   oriounp
#define OCIObjectPinCountReset           orioupz
#define OCIObjectLock                    oriolck
#define OCIObjectLockNoWait              oriolnw
#define OCIObjectMarkUpdate              orioupd
#define OCIObjectUnmark                  orioumk
#define OCIObjectUnmarkByRef             orioumr
#define OCIObjectAlwaysLatest            oriomkl
#define OCIObjectNotAlwaysLatest         oriouml
#define OCIObjectMarkDeleteByRef         oriordl
#define OCIObjectMarkDelete              oriopdl
#define OCIObjectFlush                   oriofls
#define OCIObjectFlushRefresh            oriofrh
#define OCIObjectCopy                    oriocpy
#define OCIObjectGetTypeRef              oriogtr
#define OCIObjectGetObjectRef            oriogor
#define OCIObjectGetInd                  oriogns
#define OCIObjectExists                  oriogex
#define OCIObjectGetProperty             oriogpr
#define OCIObjectRefresh                 oriorfs
#define OCIObjectPinTable                oriogtb
#define OCIObjectGetPrimaryKeyTypeRef    oriogpf
#define OCIObjectMakeObjectRef           oriomrf

#define OCIObjectNew                     orionew
#define OCIObjectPin                     oriopin
#define OCIObjectFree                    oriofre
#define OCIObjectArrayPin                orioapn
#define OCIObjectIsDirty                 oriodrt
#define OCIObjectIsDirtied               oriodrd
#define OCIObjectIsLoaded                orioldd
#define OCICacheFlush                    oricfls
#define OCICacheRefresh                  oricrfs

#endif                                                        /* SLSHORTNAME */

/*---------------------------------------------------------------------------*/
/*                       PUBLIC TYPES AND CONSTANTS                          */
/*---------------------------------------------------------------------------*/

/* Also see oro.h. */

/*---------------------------------------------------------------------------*/
/*                           PUBLIC FUNCTIONS                                */
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*                       OBJECT/INSTANCE OPERATIONS                          */
/*---------------------------------------------------------------------------*/

/*--------------------------- OCIObjectNew ----------------------------------*/
sword OCIObjectNew(    OCIEnv *env, OCIError *err, const OCISvcCtx *svc,
                       OCITypeCode typecode, OCIType *tdo, void  *table, 
                       OCIDuration duration, boolean value, 
                       void  **instance    );
/*
   NAME: OCIObjectNew - OCI new (create) a standalone instance
   PARAMETERS:
        env  (IN/OUT) - OCI environment handle initialized in object mode
        err  (IN/OUT) - error handle. If there is an error, it is
                        recorded in 'err' and this function returns OCI_ERROR.
                        The error recorded in 'err' can be retrieved by calling
                        OCIErrorGet().
        svc      (IN) - OCI service handle.  
        typecode (IN) - the typecode of the type of the instance. 
        tdo      (IN, optional) - pointer to the type descriptor object. The 
                        TDO describes the type of the instance that is to be 
                        created. Refer to OCITypeByName() for obtaining a TDO. 
                        The TDO is required for creating a named type (e.g. an
                        object or a collection).
        table (IN, optional) - pointer to a table object which specifies a 
                        table in the server.  This parameter can be set to NULL
                        if no table is given. See the description below to find
                        out how the table object and the TDO are used together
                        to determine the kind of instances (persistent, 
                        transient, value) to be created. Also see 
                        OCIObjectPinTable() for retrieving a table object.
        duration (IN) - this is an overloaded parameter. The use of this
                        parameter is based on the kind of the instance that is 
                        to be created.
                        a) persistent object. This parameter specifies the
                           pin duration.
                        b) transient object. This parameter specififes the 
                           allocation duration and pin duration. 
                        c) value. This parameter specifies the allocation
                           duration. 
        value    (IN)  - specifies whether the created object is a value.
                         If TRUE, then a value is created. Otherwise, a 
                         referenceable object is created.  If the instance is 
                         not an object, then this parameter is ignored.
        instance (OUT) - address of the newly created instance
                  
   REQUIRES:
        - a valid OCI environment handle must be given.
   DESCRIPTION:
        This function creates a new instance of the type specified by the 
        typecode or the TDO. Based on the parameters 'typecode' (or 'tdo'), 
        'value' and 'table', different kinds of instances can be created:
            
                                     The parameter 'table' is not NULL?

                                               yes              no
             ----------------------------------------------------------------
             | object type (value=TRUE)   |   value         |   value       |
             ----------------------------------------------------------------
             | object type (value=FALSE)  | persistent obj  | transient obj |
       type  ----------------------------------------------------------------
             | built-in type              |   value         |   value       |
             ----------------------------------------------------------------
             | collection type            |   value         |   value       |
             ----------------------------------------------------------------

        This function allocates the top level memory chunk of an OTS instance.
        The attributes in the top level memory are initialized (e.g. an 
        attribute of varchar2 is initialized to a vstring of 0 length). 

        If the instance is an object, the object is marked existed but is 
        atomically null. 

        FOR PERSISTENT OBJECTS:
        The object is marked dirty and existed.  The allocation duration for 
        the object is session. The object is pinned and the pin duration is 
        specified by the given parameter 'duration'.

        FOR TRANSIENT OBJECTS:
        The object is pinned. The allocation duration and the pin duration are 
        specified by the given parameter 'duration'.

        FOR VALUES:
        The allocation duration is specified by the given parameter 'duration'.

   RETURNS:
        if environment handle or error handle is null, return 
        OCI_INVALID_HANDLE.
        if operation suceeds, return OCI_SUCCESS. 
        if operation fails, return OCI_ERROR. 
 */

/*--------------------------- OCIObjectPin ----------------------------------*/
sword OCIObjectPin(    OCIEnv *env, OCIError *err, OCIRef *object_ref, 
                       OCIComplexObject *corhdl, OCIPinOpt pin_option, 
                       OCIDuration pin_duration, 
                       OCILockOpt lock_option, void  **object    );
/*
   NAME: OCIObjectPin - OCI pin a referenceable object
   PARAMETERS:
        env        (IN/OUT) - OCI environment handle initialized in object mode
        err        (IN/OUT) - error handle. If there is an error, it is
                              recorded in 'err' and this function returns 
                              OCI_ERROR. The error recorded in 'err' can be 
                              retrieved by calling OCIErrorGet().
        object_ref     (IN) - the reference to the object. 
        corhdl         (IN) - handle for complex object retrieval. 
        pin_option     (IN) - See description below.
        pin_duration   (IN) - The duration of which the object is being accesed
                              by a client. The object is implicitly unpinned at
                              the end of the pin duration. 
                              If OCI_DURATION_NULL is passed, there is no pin 
                              promotion if the object is already loaded into 
                              the cache. If the object is not yet loaded, then 
                              the pin duration is set to OCI_DURATION_DEFAULT. 
        lock_option    (IN) - lock option (e.g., exclusive). If a lock option
                              is specified, the object is locked in the server.
                              See 'oro.h' for description about lock option. 
        object        (OUT) - the pointer to the pinned object.

   REQUIRES:
        - a valid OCI environment handle must be given.
   DESCRIPTION:

        This function pins a referenceable object instance given the object
        reference. The process of pinning serves three purposes:

        1) locate an object given its reference. This is done by the object
           cache which keeps track of the objects in the object heap.  

        2) notify the object cache that an object is being in use. An object 
           can be pinned many times. A pinned object will remain in memory 
           until it is completely unpinned (see OCIObjectUnpin()). 

        3) notify the object cache that a persistent object is being in use 
           such that the persistent object cannot be aged out.  Since a 
           persistent object can be loaded from the server whenever is needed, 
           the memory utilization can be increased if a completely unpinned 
           persistent object can be freed (aged out), even before the 
           allocation duration is expired.  

        Also see OCIObjectUnpin() for more information about unpinning.

        FOR PERSISTENT OBJECTS:

        When pinning a persistent object, if it is not in the cache, the object
        will be fetched from the persistent store. The allocation duration of
        the object is session. If the object is already in the cache, it is
        returned to the client.  The object will be locked in the server if a 
        lock option is specified. 

        This function will return an error for a non-existent object.  

        A pin option is used to specify the copy of the object that is to be 
        retrieved: 

        1) If option is OCI_PIN_ANY (pin any), if the object is already 
           in the environment heap, return this object. Otherwise, the object 
           is retrieved from the database.  This option is useful when the 
           client knows that he has the exclusive access to the data in a 
           session.

        2) If option is OCI_PIN_LATEST (pin latest), if the object is 
           not cached, it is retrieved from the database.  If the object is 
           cached, it is refreshed with the latest version. See 
           OCIObjectRefresh() for more information about refreshing.

        3) If option is OCI_PIN_RECENT (pin recent), if the object is loaded
           into the cache in the current transaction, the object is returned.
           If the object is not loaded in the current transaction, the object
           is refreshed from the server.

        FOR TRANSIENT OBJECTS:

        This function will return an error if the transient object has already 
        been freed. This function does not return an error if an exclusive 
        lock is specified in the lock option. 

   RETURNS:
        if environment handle or error handle is null, return 
        OCI_INVALID_HANDLE.
        if operation suceeds, return OCI_SUCCESS. 
        if operation fails, return OCI_ERROR. 
 */

/*------------------------------ OCIObjectUnpin -----------------------------*/
sword OCIObjectUnpin(    OCIEnv *env, OCIError *err, void  *object    );
/*
   NAME: OCIObjectUnpin - OCI unpin a referenceable object
   PARAMETERS:
        env   (IN/OUT) - OCI environment handle initialized in object mode
        err   (IN/OUT) - error handle. If there is an error, it is
                         recorded in 'err' and this function returns OCI_ERROR.
                         The error recorded in 'err' can be retrieved by 
                         calling OCIErrorGet().
        object    (IN) - pointer to an object
   REQUIRES:
        - a valid OCI environment handle must be given.
        - The specified object must be pinned.
   DESCRIPTION:
        This function unpins an object.  An object is completely unpinned when 
          1) the object was unpinned N times after it has been pinned N times
             (by calling OCIObjectPin()).
          2) it is the end of the pin duration
          3) the function OCIObjectPinCountReset() is called 

        There is a pin count associated with each object which is incremented
        whenever an object is pinned. When the pin count of the object is zero,
        the object is said to be completely unpinned. An unpinned object can
        be freed without error.

        FOR PERSISTENT OBJECTS:
        When a persistent object is completely unpinned, it becomes a candidate
        for aging. The memory of an object is freed when it is aged out. Aging
        is used to maximize the utilization of memory.  An dirty object cannot 
        be aged out unless it is flushed.

        FOR TRANSIENT OBJECTS:
        The pin count of the object is decremented. A transient can be freed
        only at the end of its allocation duration or when it is explicitly
        deleted by calling OCIObjectFree().

        FOR VALUE:
        This function will return an error for value.

   RETURNS:
        if environment handle or error handle is null, return 
        OCI_INVALID_HANDLE.
        if operation suceeds, return OCI_SUCCESS. 
        if operation fails, return OCI_ERROR. 
 */

/*---------------------------- OCIObjectPinCountReset -----------------------*/
sword OCIObjectPinCountReset(    OCIEnv *env, OCIError *err, void  *object   );
/*
   NAME: OCIObjectPinCountReset - OCI resets the pin count of a referenceable
                                  object
   PARAMETERS:
        env   (IN/OUT) - OCI environment handle initialized in object mode
        err   (IN/OUT) - error handle. If there is an error, it is
                         recorded in 'err' and this function returns OCI_ERROR.
                         The error recorded in 'err' can be retrieved by
                         calling OCIErrorGet().
        object    (IN) - pointer to an object
   REQUIRES:
        - a valid OCI environment handle must be given.
        - The specified object must be pinned.
   DESCRIPTION:
        This function completely unpins an object.  When an object is 
        completely unpinned, it can be freed without error.  

        FOR PERSISTENT OBJECTS:
        When a persistent object is completely unpinned, it becomes a candidate
        for aging. The memory of an object is freed when it is aged out. Aging
        is used to maximize the utilization of memory.  An dirty object cannot 
        be aged out unless it is flushed.

        FOR TRANSIENT OBJECTS:
        The pin count of the object is decremented. A transient can be freed
        only at the end of its allocation duration or when it is explicitly
        freed by calling OCIObjectFree().

        FOR VALUE:
        This function will return an error for value.

   RETURNS:
        if environment handle or error handle is null, return 
        OCI_INVALID_HANDLE.
        if operation suceeds, return OCI_SUCCESS. 
        if operation fails, return OCI_ERROR. 
 */

/*--------------------------- OCIObjectLock ---------------------------------*/
sword OCIObjectLock(    OCIEnv *env, OCIError *err, void  *object    );
/*
   NAME: OCIObjectLock - OCI lock a persistent object
   PARAMETERS:
        env   (IN/OUT) - OCI environment handle initialized in object mode
        err   (IN/OUT) - error handle. If there is an error, it is
                         recorded in 'err' and this function returns OCI_ERROR.
                         The error recorded in 'err' can be retrieved by
                         calling OCIErrorGet().
        object    (IN) - pointer to the persistent object 
   REQUIRES:
        - a valid OCI environment handle must be given.
        - The specified object must be pinned.
   DESCRIPTION:
        This function locks a persistent object at the server. Unlike
        OCIObjectLockNoWait() this function waits if another user currently
        holds a lock on the desired object. This function
        returns an error if:
          1) the object is non-existent.

        This function will return an error for transient objects and values.
        The lock of an object is released at the end of a transaction.

   RETURNS:
        if environment handle or error handle is null, return 
        OCI_INVALID_HANDLE.
        if operation suceeds, return OCI_SUCCESS. 
        if operation fails, return OCI_ERROR. 
*/

/*------------------------ OCIObjectLockNoWait ------------------------------*/
sword OCIObjectLockNoWait(    OCIEnv *env, OCIError *err, void  *object    );
/*
   NAME: OCIObjectLockNoWait - OCI lock a persistent object, do not wait for
                               the lock, return error if lock not available
   PARAMETERS:
        env   (IN/OUT) - OCI environment handle initialized in object mode
        err   (IN/OUT) - error handle. If there is an error, it is
                         recorded in 'err' and this function returns OCI_ERROR.
                         The error recorded in 'err' can be retrieved by
                         calling OCIErrorGet().
        object    (IN) - pointer to the persistent object 
   REQUIRES:
        - a valid OCI environment handle must be given.
        - The specified object must be pinned.
   DESCRIPTION:
        This function locks a persistent object at the server. Unlike
        OCIObjectLock() this function will not wait if another user holds
        the lock on the desired object. This function returns an error if:
          1) the object is non-existent.
          2) the object is currently locked by another user in which
             case this function returns with an error.

        This function will return an error for transient objects and values.
        The lock of an object is released at the end of a transaction.

   RETURNS:
        if environment handle or error handle is null, return 
        OCI_INVALID_HANDLE.
        if operation suceeds, return OCI_SUCCESS. 
        if operation fails, return OCI_ERROR. 
*/

/*--------------------------- OCIObjectMarkUpdate ---------------------------*/
sword OCIObjectMarkUpdate(    OCIEnv *env, OCIError *err, void  *object    );
/*
   NAME: OCIObjectMarkUpdate - OCI marks an object as updated
   PARAMETERS:
        env   (IN/OUT) - OCI environment handle initialized in object mode
        err   (IN/OUT) - error handle. If there is an error, it is
                         recorded in 'err' and this function returns OCI_ERROR.
                         The error recorded in 'err' can be retrieved by
                         calling OCIErrorGet().
        object    (IN) - pointer to the persistent object 
   REQUIRES:
        - a valid OCI environment handle must be given.
        - The specified object must be pinned.
   DESCRIPTION:
        FOR PERSISTENT OBJECTS:
        This function marks the specified persistent object as updated. The
        persistent objects will be written to the server when the object cache
        is flushed.  The object is not locked or flushed by this function. It
        is an error to update a deleted object.  

        After an object is marked updated and flushed, this function must be
        called again to mark the object as updated if it has been dirtied
        after it is being flushed.

        FOR TRANSIENT OBJECTS:
        This function marks the specified transient object as updated. The
        transient objects will NOT be written to the server. It is an error
        to update a deleted object.

        FOR VALUES:
        It is an no-op for values.

   RETURNS:
        if environment handle or error handle is null, return 
        OCI_INVALID_HANDLE.
        if operation suceeds, return OCI_SUCCESS. 
        if operation fails, return OCI_ERROR. 
 */

/*----------------------------- OCIObjectUnmark -----------------------------*/
sword OCIObjectUnmark(    OCIEnv *env, OCIError *err, void  *object    );
/*
   NAME: OCIObjectUnmark - OCI unmarks an object 
   PARAMETERS:
        env   (IN/OUT) - OCI environment handle initialized in object mode
        err   (IN/OUT) - error handle. If there is an error, it is
                         recorded in 'err' and this function returns OCI_ERROR.
                         The error recorded in 'err' can be retrieved by
                         calling OCIErrorGet().
        object    (IN) - pointer to the persistent object
   REQUIRES:
        - a valid OCI environment handle must be given.
        - The specified object must be pinned.
   DESCRIPTION:
        FOR PERSISTENT OBJECTS AND TRANSIENT OBJECTS:
        This function unmarks the specified persistent object as dirty. Changes
        that are made to the object will not be written to the server. If the
        object is marked locked, it remains marked locked.  The changes that
        have already made to the object will not be undone implicitly. 
 
        FOR VALUES:
        It is an no-op for values.
 
   RETURNS:
        if environment handle or error handle is null, return
        OCI_INVALID_HANDLE.
        if operation suceeds, return OCI_SUCCESS.
        if operation fails, return OCI_ERROR.
 */

/*----------------------------- OCIObjectUnmarkByRef ------------------------*/
sword OCIObjectUnmarkByRef(    OCIEnv *env, OCIError *err, OCIRef *ref    );
/*
   NAME: OCIObjectUnmarkByRef - OCI unmarks an object by Ref
   PARAMETERS:
        env   (IN/OUT) - OCI environment handle initialized in object mode
        err   (IN/OUT) - error handle. If there is an error, it is
                         recorded in 'err' and this function returns OCI_ERROR.
                         The error recorded in 'err' can be retrieved by
                         calling OCIErrorGet().
        ref   (IN) - reference of the object
   REQUIRES:
        - a valid OCI environment handle must be given.
        - The specified object must be pinned.
   DESCRIPTION:
        FOR PERSISTENT OBJECTS AND TRANSIENT OBJECTS:
        This function unmarks the specified persistent object as dirty. Changes
        that are made to the object will not be written to the server. If the
        object is marked locked, it remains marked locked.  The changes that
        have already made to the object will not be undone implicitly.
 
        FOR VALUES:
        It is an no-op for values.
 
   RETURNS:
        if environment handle or error handle is null, return
        OCI_INVALID_HANDLE.
        if operation suceeds, return OCI_SUCCESS.
        if operation fails, return OCI_ERROR.
 */

/*--------------------------- OCIObjectFree ---------------------------------*/
sword OCIObjectFree(    OCIEnv *env, OCIError *err, void  *instance, 
                        ub2 flags   );
/*
   NAME: OCIObjectFree - OCI free (and unpin) an standalone instance 
   PARAMETERS:
        env    (IN/OUT) - OCI environment handle initialized in object mode
        err    (IN/OUT) - error handle. If there is an error, it is
                          recorded in 'err' and this function returns 
                          OCI_ERROR.  The error recorded in 'err' can be 
                          retrieved by calling OCIErrorGet().
        instance   (IN) - pointer to a standalone instance.
        flags      (IN) - If OCI_OBJECT_FREE_FORCE is set, free the object
                          even if it is pinned or dirty.
                          If OCI_OBJECT_FREE_NONULL is set, the null
                          structure will not be freed. 
   REQUIRES:
        - a valid OCI environment handle must be given.
        - The instance to be freed must be standalone.
        - If the instance is a referenceable object, the object must be pinned.
   DESCRIPTION:
        This function deallocates all the memory allocated for an OTS instance,
        including the null structure.

        FOR PERSISTENT OBJECTS:
        This function will return an error if the client is attempting to free 
        a dirty persistent object that has not been flushed. The client should 
        either flush the persistent object or set the parameter 'flag' to  
        OCI_OBJECT_FREE_FORCE.

        This function will call OCIObjectUnpin() once to check if the object 
        can be completely unpin. If it succeeds, the rest of the function will 
        proceed to free the object.  If it fails, then an error is returned 
        unless the parameter 'flag' is set to OCI_OBJECT_FREE_FORCE.
 
        Freeing a persistent object in memory will not change the persistent 
        state of that object at the server.  For example, the object will 
        remain locked after the object is freed.

        FOR TRANSIENT OBJECTS:

        This function will call OCIObjectUnpin() once to check if the object 
        can be completely unpin. If it succeeds, the rest of the function will 
        proceed to free the object.  If it fails, then an error is returned 
        unless the parameter 'flag' is set to OCI_OBJECT_FREE_FORCE. 

        FOR VALUES:
        The memory of the object is freed immediately. 

   RETURNS:
        if environment handle or error handle is null, return 
        OCI_INVALID_HANDLE.
        if operation suceeds, return OCI_SUCCESS. 
        if operation fails, return OCI_ERROR. 
*/

/*----------------------- OCIObjectMarkDeleteByRef --------------------------*/
sword OCIObjectMarkDeleteByRef(    OCIEnv *env, OCIError *err, 
                                   OCIRef *object_ref);
/*
   NAME: OCIObjectMarkDeleteByRef - OCI "delete" (and unpin) an object given
                                    a reference
   PARAMETERS:
        env     (IN/OUT) - OCI environment handle initialized in object mode
        err     (IN/OUT) - error handle. If there is an error, it is
                           recorded in 'err' and this function returns 
                           OCI_ERROR.  The error recorded in 'err' can be 
                           retrieved by calling OCIErrorGet().
        object_ref  (IN) - ref of the object to be deleted

   REQUIRES:
        - a valid OCI environment handle must be given.
   DESCRIPTION:
        This function marks the object designated by 'object_ref' as deleted.

        FOR PERSISTENT OBJECTS:
        If the object is not loaded, then a temporary object is created and is 
        marked deleted. Otherwise, the object is marked deleted.  

        The object is deleted in the server when the object is flushed.

        FOR TRANSIENT OBJECTS:
        The object is marked deleted.  The object is not freed until it is
        unpinned.

   RETURNS:
        if environment handle or error handle is null, return 
        OCI_INVALID_HANDLE.
        if operation suceeds, return OCI_SUCCESS. 
        if operation fails, return OCI_ERROR. 
 */

/*--------------------------- OCIObjectMarkDelete ---------------------------*/
sword OCIObjectMarkDelete(    OCIEnv *env, OCIError *err, void  *instance    );
/*
   NAME: OCIObjectMarkDelete - OCI "delete" an instance given a Pointer 
   PARAMETERS:
        env    (IN/OUT) - OCI environment handle initialized in object mode
        err    (IN/OUT) - error handle. If there is an error, it is
                          recorded in 'err' and this function returns 
                          OCI_ERROR.  The error recorded in 'err' can be 
                          retrieved by calling OCIErrorGet().
        instance   (IN) - pointer to the instance 
   REQUIRES:
        - a valid OCI environment handle must be given.
        - The instance must be standalone.
        - If the instance is a referenceable object, then it must be pinned.
   DESCRIPTION:

        FOR PERSISTENT OBJECTS:
        The object is marked deleted.  The memory of the object is not freed.
        The object is deleted in the server when the object is flushed.

        FOR TRANSIENT OBJECTS:
        The object is marked deleted.  The memory of the object is not freed.

        FOR VALUES: 
        This function frees a value immediately. 

   RETURNS:
        if environment handle or error handle is null, return 
        OCI_INVALID_HANDLE.
        if operation suceeds, return OCI_SUCCESS. 
        if operation fails, return OCI_ERROR. 
 */

/*---------------------------- OCIObjectFlush -------------------------------*/
sword OCIObjectFlush(    OCIEnv *env, OCIError *err, void  *object    );
/*
   NAME: OCIObjectFlush - OCI flush a persistent object
   PARAMETERS:
        env    (IN/OUT) - OCI environment handle initialized in object mode
        err    (IN/OUT) - error handle. If there is an error, it is
                          recorded in 'err' and this function returns 
                          OCI_ERROR.  The error recorded in 'err' can be 
                          retrieved by calling OCIErrorGet().
        object     (IN) - pointer to the persistent object 
   REQUIRES:
        - a valid OCI environment handle must be given.
        - The specified object must be pinned.
   DESCRIPTION:
        This function flushes a modified persistent object to the server.
        An exclusive lock is obtained implicitly for the object when flushed.

        When the object is written to the server, triggers may be fired.
        Objects can be modified by the triggers at the server.  To keep the  
        objects in the object cache being coherent with the database, the
        clients can free or refresh the objects in the cache. 

        This function will return an error for transient objects and values.
        
   RETURNS:
        if environment handle or error handle is null, return 
        OCI_INVALID_HANDLE.
        if operation suceeds, return OCI_SUCCESS. 
        if operation fails, return OCI_ERROR. 
 */

/*------------------------ OCIObjectRefresh ---------------------------------*/
sword OCIObjectRefresh(    OCIEnv *env, OCIError *err, void  *object    );
/*
   NAME: OCIObjectRefresh - OCI refresh a persistent object
   PARAMETERS:
        env    (IN/OUT) - OCI environment handle initialized in object mode
        err    (IN/OUT) - error handle. If there is an error, it is
                          recorded in 'err' and this function returns 
                          OCI_ERROR.  The error recorded in 'err' can be 
                          retrieved by calling OCIErrorGet().
        object     (IN) - pointer to the persistent object 
   REQUIRES:
        - a valid OCI environment handle must be given.
        - The specified object must be pinned.
   DESCRIPTION:
        This function refreshes an unmarked object with data retrieved from the
        latest snapshot in the server. An object should be refreshed when the 
        objects in the cache are inconsistent with the objects at 
        the server:
        1) When an object is flushed to the server, triggers can be fired to
           modify more objects in the server.  The same objects (modified by 
           the triggers) in the object cache become obsolete.
        2) When the user issues a SQL or executes a PL/SQL procedure to modify
           any object in the server, the same object in the cache becomes
           obsolete.

        The object that is refreshed will be 'replaced-in-place'. When an
        object is 'replaced-in-place', the top level memory of the object will 
        be reused so that new data can be loaded into the same memory address. 
        The top level memory of the null structre is also reused. Unlike the
        top level memory chunk, the secondary memory chunks may be resized and
        reallocated.  The client should be careful when holding onto a pointer 
        to the secondary memory chunk (e.g. assigning the address of a 
        secondary memory to a local variable), since this pointer can become 
        invalid after the object is refreshed.

        The object state will be modified as followed after being refreshed: 
          - existent : set to appropriate value
          - pinned   : unchanged 
          - allocation duration : unchanged 
          - pin duration : unchanged 
        
        This function is an no-op for transient objects or values.

   RETURNS:
        if environment handle or error handle is null, return 
        OCI_INVALID_HANDLE.
        if operation suceeds, return OCI_SUCCESS. 
        if operation fails, return OCI_ERROR. 
 */

/*---------------------------- OCIObjectCopy --------------------------------*/
sword OCIObjectCopy(    OCIEnv *env, OCIError *err, const OCISvcCtx *svc, 
                        void  *source, void  *null_source,
                        void  *target, void  *null_target, OCIType *tdo, 
                        OCIDuration duration, ub1 option    );
/*
   NAME: OCIObjectCopy - OCI copy one instance to another
   PARAMETERS:
        env     (IN/OUT) - OCI environment handle initialized in object mode
        err     (IN/OUT) - error handle. If there is an error, it is
                           recorded in 'err' and this function returns 
                           OCI_ERROR.  The error recorded in 'err' can be 
                           retrieved by calling OCIErrorGet().
        svc         (IN) - OCI service context handle
        source      (IN) - pointer to the source instance 
        null_source (IN) - pointer to the null structure of the source
        target      (IN) - pointer to the target instance
        null_target (IN) - pointer to the null structure of the target 
        tdo         (IN) - the TDO for both source and target
        duration    (IN) - allocation duration of the target memory
        option      (IN) - specify the copy option:
                        OROOCOSFN - Set Reference to Null. All references
                        in the source will not be copied to the target. The
                        references in the target are set to null. 
   REQUIRES:
        - a valid OCI environment handle must be given.
        - If source or target is referenceable, it must be pinned.
        - The target or the containing instance of the target must be already
          be instantiated (e.g. created by OCIObjectNew()).
        - The source and target instances must be of the same type. If the
          source and target are located in a different databases, then the
          same type must exist in both databases.
   DESCRIPTION:
        This function copies the contents of the 'source' instance to the
        'target' instance. This function performs a deep-copy such that the 
        data that is copied/duplicated include:
        a) all the top level attributes (see the exceptions below)
        b) all the secondary memory (of the source) that is reachable from the
           top level attributes.
        c) the null structure of the instance

        Memory is allocated with the specified allocation duration. 

        Certain data items are not copied: 
        a) If the option OCI_OBJECTCOPY_NOREF is specified, then all references
           in the source are not copied. Instead, the references in the target
           are set to null.
        b) If the attribute is a LOB, then it is set to null.

   RETURNS:
        if environment handle or error handle is null, return 
        OCI_INVALID_HANDLE.
        if operation suceeds, return OCI_SUCCESS. 
        if operation fails, return OCI_ERROR. 
 */

/*---------------------------- OCIObjectGetTypeRef --------------------------*/
sword OCIObjectGetTypeRef(    OCIEnv *env, OCIError *err, void  *instance, 
                              OCIRef *type_ref    );
/*
   NAME: OCIObjectGetTypeRef - get the type reference of a standalone object
   PARAMETERS:
        env   (IN/OUT) - OCI environment handle initialized in object mode
        err   (IN/OUT) - error handle. If there is an error, it is
                         recorded in 'err' and this function returns 
                         OCI_ERROR.  The error recorded in 'err' can be 
                         retrieved by calling OCIErrorGet().
        instance  (IN) - pointer to an standalone instance 
        type_ref (OUT) - reference to the type of the object.  The reference 
                         must already be allocated.
   REQUIRES:
        - a valid OCI environment handle must be given.
        - The instance must be standalone.
        - If the object is referenceable, the specified object must be pinned.
        - The reference must already be allocated.
   DESCRIPTION:
        This function returns a reference to the TDO of a standalone instance. 
   RETURNS:
        if environment handle or error handle is null, return 
        OCI_INVALID_HANDLE.
        if operation suceeds, return OCI_SUCCESS. 
        if operation fails, return OCI_ERROR. 
 */

/*--------------------------- OCIObjectGetObjectRef -------------------------*/
sword OCIObjectGetObjectRef(    OCIEnv *env, OCIError *err, void  *object, 
                                OCIRef *object_ref    );
/*
   NAME: OCIObjectGetObjectRef - OCI get the object reference of an 
                                 referenceable object
   PARAMETERS:
        env     (IN/OUT) - OCI environment handle initialized in object mode
        err     (IN/OUT) - error handle. If there is an error, it is
                           recorded in 'err' and this function returns 
                           OCI_ERROR.  The error recorded in 'err' can be 
                           retrieved by calling OCIErrorGet().
        object      (IN) - pointer to a persistent object
        object_ref (OUT) - reference of the given object. The reference must 
                           already be allocated.
   REQUIRES:
        - a valid OCI environment handle must be given.
        - The specified object must be pinned.
        - The reference must already be allocated.
   DESCRIPTION:
        This function returns a reference to the given object.  It returns an 
        error for values. 
   RETURNS:
        if environment handle or error handle is null, return 
        OCI_INVALID_HANDLE.
        if operation suceeds, return OCI_SUCCESS. 
        if operation fails, return OCI_ERROR. 
 */

/*--------------------------- OCIObjectMakeObjectRef -----------------------*/
sword OCIObjectMakeObjectRef(    OCIEnv *env, OCIError *err, 
                                 const OCISvcCtx *svc, void  * table,
                                 void  **values, ub4 array_len, 
                                 OCIRef *object_ref    );
/*
   NAME: OCIObjectMakeObjectRef - OCI Create an object reference to a 
                                 referenceable object.
   PARAMETERS:
        env     (IN/OUT) - OCI environment handle initialized in object mode
        err     (IN/OUT) - error handle. If there is an error, it is
                           recorded in 'err' and this function returns 
                           OCI_ERROR.  The error recorded in 'err' can be 
                           retrieved by calling OCIErrorGet().
        svc         (IN) - the service context
        table       (IN) - A pointer to the table object (must be pinned)
        attrlist    (IN) - A list of values (OCI type values) from which
                           the ref is to be created.
        attrcnt     (IN)  - The length of the attrlist array. 
        object_ref (OUT) - reference of the given object. The reference must 
                           already be allocated. 
   REQUIRES:
        - a valid OCI environment handle must be given.
        - The specified table object must be pinned.
        - The reference must already be allocated.
   DESCRIPTION:
        This function creates a reference given the values that make up the 
        reference and also a pointer to the table object. 
        Based on the table's OID property, whether it is a pk based OID or
        a system generated OID, the function creates a sys-generated REF or
        a pk based REF.
        In case of system generated REFs pass in a OCIRaw which is 16 bytes
        long contatining the sys generated OID.
        In case of PK refs pass in the OCI equivalent for numbers, chars etc..
   RETURNS:
        if environment handle or error handle is null, return 
        OCI_INVALID_HANDLE.
        if operation suceeds, return OCI_SUCCESS. 
        if operation fails, return OCI_ERROR. 
 */

/*--------------------------- OCIObjectGetPrimaryKeyTypeRef --------------- */ 
sword OCIObjectGetPrimaryKeyTypeRef( OCIEnv *env, OCIError *err,
                                     const OCISvcCtx *svc, void  *table, 
                                     OCIRef *type_ref );
/*
   NAME: OCIObjectGetPrimaryKeyTypeRef - OCI get the REF to the pk OID type 
   PARAMETERS:
        env     (IN/OUT) - OCI environment handle initialized in object mode
        err     (IN/OUT) - error handle. If there is an error, it is
                           recorded in 'err' and this function returns 
                           OCI_ERROR.  The error recorded in 'err' can be 
                           retrieved by calling OCIErrorGet().
        svc     (IN)     - the service context 
        table   (IN)     - pointer to the table object
        type_ref   (OUT) - reference of the pk type. The reference must 
                           already be allocated.
   REQUIRES:
        - a valid OCI environment handle must be given.
        - The specified table object must be pinned.
        - The reference must already be allocated.
   DESCRIPTION:
        This function returns a reference to the pk type.  It returns an 
        error for values.  If the table is not a Pk oid table/view, then
        it returns error.
   RETURNS:
        if environment handle or error handle is null, return 
        OCI_INVALID_HANDLE.
        if operation suceeds, return OCI_SUCCESS. 
        if operation fails, return OCI_ERROR. 
 */

/*-------------------------- OCIObjectGetInd --------------------------------*/
sword OCIObjectGetInd(    OCIEnv *env, OCIError *err, void  *instance, 
                          void  **null_struct    );
/*
   NAME: OCIObjectGetInd - OCI get the null structure of a standalone object  
   PARAMETERS:
        env     (IN/OUT) - OCI environment handle initialized in object mode
        err     (IN/OUT) - error handle. If there is an error, it is
                           recorded in 'err' and this function returns 
                           OCI_ERROR.  The error recorded in 'err' can be 
                           retrieved by calling OCIErrorGet().
        instance      (IN) - pointer to the instance 
        null_struct (OUT) - null structure 
   REQUIRES:
        - a valid OCI environment handle must be given.
        - The object must be standalone.
        - If the object is referenceable, the specified object must be pinned.
   DESCRIPTION:
        This function returns the null structure of an instance. This function
        will allocate the top level memory of the null structure if it is not
        already allocated. If an null structure cannot be allocated for the 
        instance, then an error is returned. This function only works for 
        ADT or row type instance. 
   RETURNS:
        if environment handle or error handle is null, return 
        OCI_INVALID_HANDLE.
        if operation suceeds, return OCI_SUCCESS. 
        if operation fails, return OCI_ERROR. 
 */

/*------------------------- OCIObjectExists --------------------------------*/
sword OCIObjectExists(OCIEnv *env, OCIError *err, void  *ins, boolean *exist); 
/*
   NAME: OCIObjectExist - OCI checks if the object exists 
   PARAMETERS:
        env       (IN/OUT) - OCI environment handle initialized in object mode
        err       (IN/OUT) - error handle. If there is an error, it is
                             recorded in 'err' and this function returns 
                             OCI_ERROR.  The error recorded in 'err' can be 
                             retrieved by calling OCIErrorGet().
        ins           (IN) - pointer to an instance 
        exist        (OUT) - return TRUE if the object exists
   REQUIRES:
        - a valid OCI environment handle must be given.
        - The object must be standalone.
        - if object is a referenceable, it must be pinned.
   DESCRIPTION:
        This function returns the existence of an instance. If the instance
        is a value, this function always returns TRUE.
   RETURNS:
        if environment handle or error handle is null, return 
        OCI_INVALID_HANDLE.
        if operation suceeds, return OCI_SUCCESS. 
        if operation fails, return OCI_ERROR. 
 */

/*------------------------- OCIObjectGetProperty ---------------------------*/
sword OCIObjectGetProperty(OCIEnv *envh, OCIError *errh, const void  *obj, 
                           OCIObjectPropId propertyId,
                           void  *property, ub4 *size );
/*
   NAME: OCIObjectGetProperty - OCIObject Get Property of given object
   PARAMETERS:
        env       (IN/OUT) - OCI environment handle initialized in object mode
        err       (IN/OUT) - error handle. If there is an error, it is
                             recorded in 'err' and this function returns
                             OCI_ERROR.  The error recorded in 'err' can be
                             retrieved by calling OCIErrorGet().
        obj           (IN) - object whose property is returned
        propertyId    (IN) - id which identifies the desired property
        property     (OUT) - buffer into which the desired property is
                             copied
        size      (IN/OUT) - on input specifies the size of the property buffer
                             passed by caller, on output will contain the 
                             size in bytes of the property returned.
                             This parameter is required for string type 
                             properties only (e.g OCI_OBJECTPROP_SCHEMA,
                             OCI_OBJECTPROP_TABLE). For non-string
                             properties this parameter is ignored since
                             the size is fixed.
   DESCRIPTION:
        This function returns the specified property of the object.
        The desired property is identified by 'propertyId'. The property
        value is copied into 'property' and for string typed properties
        the string size is returned via 'size'. 
        
        Objects are classified as persistent, transient and value
        depending upon the lifetime and referenceability of the object.
        Some of the properties are applicable only to persistent
        objects and some others only apply to persistent and 
        transient objects. An error is returned if the user tries to 
        get a property which in not applicable to the given object. 
        To avoid such an error, the user should first check whether
        the object is persistent or transient or value 
        (OCI_OBJECTPROP_LIFETIME property) and then appropriately
        query for other properties.

        The different property ids and the corresponding type of 
        'property' argument is given below.

          OCI_OBJECTPROP_LIFETIME 
            This identifies whether the given object is a persistent
            object (OCI_OBJECT_PERSISTENT) or a 
            transient object (OCI_OBJECT_TRANSIENT) or a
            value instance (OCI_OBJECT_VALUE).
            'property' argument must be a pointer to a variable of 
            type OCIObjectLifetime.
            
          OCI_OBJECTPROP_SCHEMA
            This returns the schema name of the table in which the 
            object exists. An error is returned if the given object 
            points to a transient instance or a value. If the input 
            buffer is not big enough to hold the schema name an error 
            is returned, the error message will communicate the 
            required size. Upon success, the size of the returned 
            schema name in bytes is returned via 'size'.
            'property' argument must be an array of type text and 'size'
            should be set to size of array in bytes by the caller.

          OCI_OBJECTPROP_TABLE
            This returns the table name in which the object exists. An 
            error is returned if the given object points to a 
            transient instance or a value. If the input buffer is not 
            big enough to hold the table name an error is returned, 
            the error message will communicate the required size. Upon 
            success, the size of the returned table name in bytes is 
            returned via 'size'. 'property' argument must be an array 
            of type text and 'size' should be set to size of array in 
            bytes by the caller.
            
          OCI_OBJECTPROP_PIN_DURATION
            This returns the pin duration of the object.
            An error is returned if the given object points to a value 
            instance. Valid pin durations are: OCI_DURATION_SESSION and
            OCI_DURATION_TRANS.
            'property' argument must be a pointer to a variable of type 
            OCIDuration.
            
          OCI_OBJECTPROP_ALLOC_DURATION
            This returns the allocation duration of the object.
            Valid allocation durations are: OCI_DURATION_SESSION and
            OCI_DURATION_TRANS.
            'property' argument must be a pointer to a variable of type 
            OCIDuration.
            
          OCI_OBJECTPROP_LOCK
            This returns the lock status of the 
            object. The possible lock status is enumerated by OCILockOpt.
            An error is returned if the given object points to a transient
            or value instance.
            'property' argument must be a pointer to a variable of 
            type OCILockOpt.
            Note, the lock status of an object can also be retrieved by
            calling OCIObjectIsLocked().

          OCI_OBJECTPROP_MARKSTATUS
            This returns the status flag which indicates whether the
            object is a new object, updated object and/or deleted object.
            The following macros can be used to test the mark status
            flag:

              OCI_OBJECT_IS_UPDATED(flag)
              OCI_OBJECT_IS_DELETED(flag)
              OCI_OBJECT_IS_NEW(flag)
              OCI_OBJECT_IS_DIRTY(flag)

            An object is dirty if it is a new object or marked deleted or 
            marked updated.
            An error is returned if the given object points to a transient
            or value instance. 'property' argument must be of type 
            OCIObjectMarkStatus.
            
          OCI_OBJECTPROP_VIEW
            This identifies whether the specified object is a view object
            or not. If property value returned is TRUE, it indicates the
            object is a view otherwise it is not.
            'property' argument must be of type boolean.

   RETURNS:
        if environment handle or error handle is null, return
        OCI_INVALID_HANDLE.
        if operation suceeds, return OCI_SUCCESS.
        if operation fails, return OCI_ERROR. Possible errors are TBD
 */

/*---------------------------- OCIObjectIsLocked --------------------------*/
sword OCIObjectIsLocked(    OCIEnv *env, OCIError *err, void  *ins,
                              boolean *lock);
/*
   NAME: OCIObjectIsLocked - OCI get the lock status of a standalone object
   PARAMETERS:
        env       (IN/OUT) - OCI environment handle initialized in object mode
        err       (IN/OUT) - error handle. If there is an error, it is
                             recorded in 'err' and this function returns
                             OCI_ERROR.  The error recorded in 'err' can be
                             retrieved by calling OCIErrorGet().
        ins           (IN) - pointer to an instance
        lock         (OUT) - return value for the lock status.
   REQUIRES:
        - a valid OCI environment handle must be given.
        - The instance must be standalone.
        - If the object is referenceable, the specified object must be pinned.
   DESCRIPTION:
        This function returns the lock status of an instance. If the instance
        is a value, this function always returns FALSE.
   RETURNS:
        if environment handle or error handle is null, return
        OCI_INVALID_HANDLE.
        if operation suceeds, return OCI_SUCCESS.
        if operation fails, return OCI_ERROR.
 */

/*------------------------- OCIObjectIsDirty ------------------------------*/
sword OCIObjectIsDirty(    OCIEnv *env, OCIError *err, void  *ins,
                           boolean *dirty);
/*
   NAME: OCIObjectIsDirty - OCI get the dirty status of a standalone object
   PARAMETERS:
        env       (IN/OUT) - OCI environment handle initialized in object mode
        err       (IN/OUT) - error handle. If there is an error, it is
                             recorded in 'err' and this function returns
                             OCI_ERROR.  The error recorded in 'err' can be
                             retrieved by calling OCIErrorGet().
        ins           (IN) - pointer to an instance
        dirty        (OUT) - return value for the dirty status.
   REQUIRES:
        - a valid OCI environment handle must be given.
        - The instance must be standalone.
        - if instance is an object, the instance must be pinned.
   DESCRIPTION:
        This function returns the dirty status of an instance. If the instance
        is a value, this function always returns FALSE.
   RETURNS:
        if environment handle or error handle is null, return
        OCI_INVALID_HANDLE.
        if operation suceeds, return OCI_SUCCESS.
        if operation fails, return OCI_ERROR.
 */

/*--------------------------- OCIObjectPinTable -----------------------------*/
sword OCIObjectPinTable(    OCIEnv *env, OCIError *err, const OCISvcCtx *svc, 
                  const oratext *schema_name, 
                  ub4 s_n_length, const oratext *object_name, ub4 o_n_length, 
                  const OCIRef *scope_obj_ref, OCIDuration pin_duration, 
                  void ** object    );
/*
   NAME: OCIObjectPinTable - OCI get table object 
   PARAMETERS:
        env       (IN/OUT) - OCI environment handle initialized in object mode
        err       (IN/OUT) - error handle. If there is an error, it is
                             recorded in 'err' and this function returns 
                             OCI_ERROR.  The error recorded in 'err' can be 
                             retrieved by calling OCIErrorGet().
        svc                     (IN) - OCI service context handle
        schema_name   (IN, optional) - schema name of the table 
        s_n_length    (IN, optional) - length of the schema name
        object_name   (IN) - name of the table 
        o_n_length    (IN) - length of the table name
        scope_obj_ref (IN, optional) - reference of the scoping object
        pin_duration  (IN) - pin duration. See description in OCIObjectPin(). 
        object       (OUT) - the pinned table object
   REQUIRES:
        - a valid OCI environment handle must be given.
   DESCRIPTION:
        This function pin a table object with the specified pin duration. 
        The client can unpin the object by calling OCIObjectUnpin(). See 
        OCIObjectPin() and OCIObjectUnpin() for more information about pinning
        and unpinning. 
   RETURNS:
        if environment handle or error handle is null, return 
        OCI_INVALID_HANDLE.
        if operation suceeds, return OCI_SUCCESS. 
        if operation fails, return OCI_ERROR. 
 */

/*------------------------- OCIObjectArrayPin -------------------------------*/
sword OCIObjectArrayPin(OCIEnv *env, OCIError *err, OCIRef **ref_array, 
                  ub4 array_size, OCIComplexObject **cor_array,
                  ub4 cor_array_size, OCIPinOpt pin_option, 
                  OCIDuration pin_duration, OCILockOpt lock, 
                  void  **obj_array, ub4 *pos    );
/*
   NAME: OCIObjectArrayPin - ORIO array pin 
   PARAMETERS:
        env       (IN/OUT) - OCI environment handle initialized in object mode
        err       (IN/OUT) - error handle. If there is an error, it is
                             recorded in 'err' and this function returns 
                             OCI_ERROR.  The error recorded in 'err' can be 
                             retrieved by calling OCIErrorGet().
        ref_array     (IN) - array of references to be pinned 
        array_size    (IN) - number of elements in the array of references 
        pin_option    (IN) - pin option. See OCIObjectPin().
        pin_duration  (IN) - pin duration. See OCIObjectPin(). 
        lock_option   (IN) - lock option. See OCIObjectPin().
        obj_array    (OUT) - If this argument is not NULL, the pinned objects 
                             will be returned in the array. The user must 
                             allocate this array with element type being 
                             'void *'. The size of this array is identical to
                             'array'. 
        pos          (OUT) - If there is an error, this argument will contain
                             the element that is causing the error.  Note that
                             this argument is set to 1 for the first element in
                             the ref_array. 
   REQUIRE:
        - a valid OCI environment handle must be given.
        - If 'obj_array' is not NULL, then it must already be allocated and
             the size of 'obj_array' is 'array_size'. 
   DESCRIPTION:
        This function pin an array of references.  All the pinned objects are 
        retrieved from the database in one network roundtrip.  If the user 
        specifies an output array ('obj_array'), then the address of the 
        pinned objects will be assigned to the elements in the array. See
        OCIObjectPin() for more information about pinning.
   RETURNS:
        if environment handle or error handle is null, return 
        OCI_INVALID_HANDLE.
        if operation suceeds, return OCI_SUCCESS. 
        if operation fails, return OCI_ERROR. 
 */

/*---------------------------------------------------------------------------*/
/*                           HEAP/CACHE OPERATIONS                           */
/*---------------------------------------------------------------------------*/

/*--------------------------- OCICacheFlush ---------------------------------*/
sword OCICacheFlush(    OCIEnv *env, OCIError *err, const OCISvcCtx *svc, 
                  void  *context, OCIRef *(*get)(void  *context, ub1 *last),
                  OCIRef **ref  );
/*
   NAME: OCICacheFlush - OCI flush persistent objects 
   PARAMETERS:
        env (IN/OUT) - OCI environment handle initialized in object mode
        err (IN/OUT) - error handle. If there is an error, it is
                      recorded in 'err' and this function returns 
                      OCI_ERROR.  The error recorded in 'err' can be 
                      retrieved by calling OCIErrorGet().
        svc      (IN) [optional] - OCI service context.  If null pointer is
                      specified, then the dirty objects in all connections
                      will be flushed.
        context  (IN) [optional] - specifies an user context that is an 
                      argument to the client callback function 'get'. This 
                      parameter is set to NULL if there is no user context.
        get      (IN) [optional] - an client-defined function which acts an 
                      iterator to retrieve a batch of dirty objects that need 
                      to be flushed. If the function is not NULL, this function
                      will be called to get a reference of a dirty object.  
                      This is repeated until a null reference is returned by 
                      the client function or the parameter 'last' is set to 
                      TRUE. The parameter 'context' is passed to get() 
                      for each invocation of the client function.  This 
                      parameter should be NULL if user callback is not given.
                      If the object that is returned by the client function is
                      not a dirtied persistent object, the object is ignored.
                      All the objects that are returned from the client
                      function must be from newed or pinned the same service 
                      context, otherwise, an error is signalled. Note that the 
                      returned objects are flushed in the order in which they
                      are marked dirty.
        ref     (OUT) [optional] - if there is an error in flushing the 
                      objects, (*ref) will point to the object that
                      is causing the error.  If 'ref' is NULL, then the object 
                      will not be returned.  If '*ref' is NULL, then a 
                      reference will be allocated and set to point to the 
                      object.  If '*ref' is not NULL, then the reference of
                      the object is copied into the given space. If the
                      error is not caused by any of the dirtied object,
                      the given ref is initalized to be a NULL reference
                      (OCIRefIsNull(*ref) is TRUE).
   REQUIRES:
        - a valid OCI environment handle must be given.
   DESCRIPTION:
        This function flushes the modified persistent objects from the 
        environment heap to the server. The objects are flushed in the order 
        that they are marked updated or deleted. 

        See OCIObjectFlush() for more information about flushing.

   RETURNS:
        if environment handle or error handle is null, return 
        OCI_INVALID_HANDLE.
        if operation suceeds, return OCI_SUCCESS. 
        if operation fails, return OCI_ERROR. 
 */

/*--------------------------- OCICacheRefresh -------------------------------*/
sword OCICacheRefresh(OCIEnv *env, OCIError *err, const OCISvcCtx *svc,
                  OCIRefreshOpt option, void  *context,
                  OCIRef *(*get)(void  *context), OCIRef **ref);
/*
   NAME: OCICacheRefresh - OCI ReFreSh persistent objects 
   PARAMETERS:
        env (IN/OUT) - OCI environment handle initialized in object mode
        err (IN/OUT) - error handle. If there is an error, it is
                       recorded in 'err' and this function returns 
                       OCI_ERROR.  The error recorded in 'err' can be 
                       retrieved by calling OCIErrorGet().
        svc     (IN) [optional] - OCI service context.  If null pointer is
                      specified, then the persistent objects in all connections
                      will be refreshed. 
        option   (IN) [optional] - if OCI_REFRESH_LOAD is specified, all
                      objects that is loaded within the transaction are
                      refreshed. If the option is OCI_REFERSH_LOAD and the
                      parameter 'get' is not NULL, this function will ignore
                      the parameter. 
        context  (IN) [optional] - specifies an user context that is an 
                      argument to the client callback function 'get'. This 
                      parameter is set to NULL if there is no user context.
        get      (IN) [optional] - an client-defined function which acts an 
                      iterator to retrieve a batch of objects that need to be
                      refreshed. If the function is not NULL, this function
                      will be called to get a reference of an object.  If 
                      the reference is not NULL, then the object will be 
                      refreshed.  These steps are repeated until a null 
                      reference is returned by this function.  The parameter
                      'context' is passed to get() for each invocation of the
                      client function.  This parameter should be NULL if user 
                      callback is not given.
        ref     (OUT) [optional] - if there is an error in refreshing the
                      objects, (*ref) will point to the object that
                      is causing the error.  If 'ref' is NULL, then the object
                      will not be returned.  If '*ref' is NULL, then a
                      reference will be allocated and set to point to the
                      object.  If '*ref' is not NULL, then the reference of
                      the object is copied into the given space. If the
                      error is not caused by any of the object,
                      the given ref is initalized to be a NULL reference
                      (OCIRefIsNull(*ref) is TRUE).
   REQUIRES:
        - a valid OCI environment handle must be given.
   DESCRIPTION:
        This function refreshes all pinned persistent objects. All unpinned 
        persistent objects are freed.  See OCIObjectRefresh() for more 
        information about refreshing.
   RETURNS:
        if environment handle or error handle is null, return 
        OCI_INVALID_HANDLE.
        if operation suceeds, return OCI_SUCCESS. 
        if operation fails, return OCI_ERROR. 
 */

/*---------------------------- OCICacheUnpin --------------------------------*/
sword OCICacheUnpin(    OCIEnv *env, OCIError *err, const OCISvcCtx *svc    );
/*
   NAME: OCICacheUnpin - OCI UNPin objects 
   PARAMETERS:
        env (IN/OUT) - OCI environment handle initialized in object mode
        err (IN/OUT) - error handle. If there is an error, it is
                       recorded in 'err' and this function returns 
                       OCI_ERROR.  The error recorded in 'err' can be 
                       retrieved by calling OCIErrorGet().
        svc     (IN) [optional] - OCI service context. If null pointer is
                       specified, then the objects in all connections
                       will be unpinned.
   REQUIRES:
        - a valid OCI environment handle must be given.
   DESCRIPTION:
        If a connection is specified, this function completely unpins the 
        persistent objects in that connection. Otherwise, all persistent 
        objects in the heap are completely unpinned. All transient objects in 
        the heap are also completely unpinned. See OCIObjectUnpin() for more 
        information about unpinning.
   RETURNS:
        if environment handle or error handle is null, return 
        OCI_INVALID_HANDLE.
        if operation suceeds, return OCI_SUCCESS. 
        if operation fails, return OCI_ERROR. 
 */

/*----------------------------- OCICacheFree --------------------------------*/
sword OCICacheFree(    OCIEnv *env, OCIError *err, const OCISvcCtx *svc    ); 
/*
   NAME: OCICacheFree - OCI FREe instances 
   PARAMETERS:
        env (IN/OUT) - OCI environment handle initialized in object mode
        err (IN/OUT) - error handle. If there is an error, it is
                       recorded in 'err' and this function returns 
                       OCI_ERROR.  The error recorded in 'err' can be 
                       retrieved by calling OCIErrorGet().
        svc     (IN) [optional] - OCI service context. If null pointer is
                       specified, then the objects in all connections
                       will be freed.
   REQUIRES:
        - a valid OCI environment handle must be given.
   DESCRIPTION:
        If a connection is specified, this function frees the persistent 
        objects, transient objects and values allocated for that connection.  
        Otherwise, all persistent objects, transient objects and values in the 
        heap are freed. Objects are freed regardless of their pin count.  See 
        OCIObjectFree() for more information about freeing an instance.
   RETURNS:
        if environment handle or error handle is null, return 
        OCI_INVALID_HANDLE.
        if operation suceeds, return OCI_SUCCESS. 
        if operation fails, return OCI_ERROR. 
*/

/*---------------------------- OCICacheUnmark -------------------------------*/
sword OCICacheUnmark(    OCIEnv *env, OCIError *err, const OCISvcCtx *svc    );
/*
   NAME: OCICacheUnmark - OCI Unmark all dirty objects
   PARAMETERS:
        env (IN/OUT) - OCI environment handle initialized in object mode
        err (IN/OUT) - error handle. If there is an error, it is
                       recorded in 'err' and this function returns
                       OCI_ERROR.  The error recorded in 'err' can be
                       retrieved by calling OCIErrorGet().
        svc     (IN) [optional] - OCI service context. If null pointer is
                       specified, then the objects in all connections
                       will be unmarked. 
   REQUIRES:
        - a valid OCI environment handle must be given.
   DESCRIPTION:
        If a connection is specified, this function unmarks all dirty objects 
        in that connection.  Otherwise, all dirty objects in the cache are
        unmarked. See OCIObjectUnmark() for more information about unmarking
        an object.
   RETURNS:
        if environment handle or error handle is null, return
        OCI_INVALID_HANDLE.
        if operation suceeds, return OCI_SUCCESS.
        if operation fails, return OCI_ERROR.
 */


sword OCIDurationBegin(    OCIEnv *env, OCIError *err, const OCISvcCtx *svc, 
                           OCIDuration parent, OCIDuration *dur    );
/*
   NAME: OCIDurationBegin - OCI DURATION BEGIN
   PARAMETERS:
        env  (IN/OUT) - OCI environment handle initialized in object mode
                        This should be passed NULL, when cartridge services
                        are to be used.
        err  (IN/OUT) - error handle. If there is an error, it is
                        recorded in 'err' and this function returns OCI_ERROR.
                        The error recorded in 'err' can be retrieved by calling
                       OCIErrorGet().
        svc  (IN/OUT) - OCI service handle.  
        parent   (IN) - parent for the duration to be started.
        dur     (OUT) - newly created user duration 
   REQUIRES:
        - a valid OCI environment handle must be given for non-cartridge
          services.
        - For cartridge services, NULL should be given for environment handle
        - A valid service handle must be given in all cases.
   DESCRIPTION:
        This function starts a new user duration.  A user can have multiple
        active user durations simultaneously. The user durations do not have
        to be nested.
 
        The object subsystem predefines 3 durations :
          1) session     - memory allocated with session duration comes from
                           the UGA heap (OCI_DURATION_SESSION). A session 
                           duration terminates at the end of the user session.
          2) transaction - memory allocated with transaction duration comes
                           from the UGA heap (OCI_DURATION_TRANS). A trans-
                           action duration terminates at the end of the user
                           transaction.
          3) call        - memory allocated with call duration comes from PGA
                           heap (OCI_DURATION_CALL). A call duration terminates
                           at the end of the user call.
 
        Each user duration has a parent duration.  A parent duration can be a
        predefined duration or another user duration.  The relationship between
        a user duration and its parent duration (child duration) are:
 
         1) An user duration is nested within the parent duration. When its
             parent duration terminates, the user duration will also terminate.
         2) The memory allocated with an user duration comes from the heap of
             its parent duration. For example, if the parent duration of an
             user duration is call, then the memory allocated with the user
             duration will also come from the PGA heap.

        This function can be used as both part of cartridge services as well 
        as without cartridge services.
        The difference in the function in the case of cartridge and 
        non-cartridge services is:
                In case of cartridge services, as descibed above a new user
        duration is created as a child of the "parent" duration.
                But when used for non-cartridge purposes, when a pre-defined
        duration is passed in as parent, it is mapped to the cache duration
        for that connection (which is created if not already present) and 
        the new user duration will be child of the cache duration.

   RETURNS:
        if environment handle and service handle is null or if error 
        handle is null return OCI_INVALID_HANDLE.
        if operation suceeds, return OCI_SUCCESS.
        if operation fails, return OCI_ERROR.
 */


sword OCIDurationEnd(    OCIEnv *env, OCIError *err, const OCISvcCtx *svc, 
                         OCIDuration duration    );
/*
   NAME: OCIDurationEnd - OCI DURATION END
   PARAMETERS:
        env  (IN/OUT) - OCI environment handle initialized in object mode
                        This should be passed NULL, when cartridge services
                        are to be used.
        err  (IN/OUT) - error handle. If there is an error, it is
                        recorded in 'err' and this function returns OCI_ERROR.
                        The error recorded in 'err' can be retrieved by calling
                       OCIErrorGet().
        svc  (IN/OUT) - OCI service handle.  
        dur     (OUT) - a previously created user duration using 
                        OCIDurationBegin()
   REQUIRES:
        - a valid OCI environment handle must be given for non-cartridge
          services.
        - For cartridge services, NULL should be given for environment handle
        - A valid service handle must be given in all cases.
   DESCRIPTION:
        This function terminates a user duration.  All memory allocated for
        this duration is freed.

        This function can be used as both part of cartridge services as well 
        as without cartridge services.  In both cased, the heap duration
        is freed and all the allocated memory for that duration is freed.
        The difference in the function in the case of cartridge and 
        non-cartridge services is:
                In case of non-cartridge services, if the duration is pre-
        defined, the associated cache duration (see OCIDurationBegin())
        is also terminated and the following is done.
          1) The child durations are terminated.
          2) All objects pinned for this duration are unpinned.
          3) All instances allocated for this duration are freed.

                In case of cartridge services, only the heap duration is
        freed.  All the context entries allocated for that duration are 
        freed from the context hash table..

   RETURNS:
        if environment handle and service handle is null or if error 
        handle is null return OCI_INVALID_HANDLE.
        if operation suceeds, return OCI_SUCCESS.
        if operation fails, return OCI_ERROR.
 */

/******************************************************************************
**          DO  NOT  USE  THE  FUNCTIONS  BELOW!                             **
**          UNSUPPORTED INTERFACE                                            **
**          WILL BE REMOVED/CHANGED IN A FUTURE RELEASE                      **
******************************************************************************/

sword OCIDurationGetParent(    OCIEnv *env, OCIError *err, 
                               OCIDuration duration, OCIDuration *parent    );

sword OCIObjectAlwaysLatest(    OCIEnv *env, OCIError *err, void  *object    );

sword OCIObjectNotAlwaysLatest(    OCIEnv *env, OCIError *err, 
                                   void  *object    );

sword OCIObjectFlushRefresh(    OCIEnv *env, OCIError *err, void  *object);

sword OCIObjectIsLoaded(    OCIEnv *env, OCIError *err, void  *ins, 
                              boolean *load);

sword OCIObjectIsDirtied(    OCIEnv *env, OCIError *err, void  *ins,
                              boolean *dirty);

sword OCICacheGetObjects(       OCIEnv *env, OCIError *err,
                                const OCISvcCtx *svc,
                                OCIObjectProperty property,
                                void  *client_context,
                                void (*client_callback)(
                                    void  *client_context,
                                    void  *object  ));

sword OCICacheRegister(      OCIEnv *env, OCIError *err,
                             OCIObjectEvent event,
                             void  *client_context,
                             void (*client_callback)(
                                    void  *client_context,
                                    OCIObjectEvent event,
                                    void  *object));

sword OCICacheFlushRefresh(    OCIEnv *env, OCIError *err, 
                               const OCISvcCtx *svc, void  *context,
                               OCIRef *(*get)(void  *context, ub1 *last),
                               OCIRef **ref    );

sword OCIObjectSetData(OCIEnv *env, OCIError *err, void  *obj_hdr, 
        void  *data);

sword OCIObjectGetNewOID(OCIEnv *env, OCIError *err, OCISvcCtx *svc, 
        ub1 *oid);


#endif /* ORI_ORACLE */
