/* @(#)ort.h    1.44 95/07/07 */

/* Copyright (c) 1994, 2011, Oracle and/or its affiliates. 
All rights reserved. */

/* 
  NAME

    ORT - ORacle's external open Type interface to the open type manager (OTM)

  DESCRIPTION

    The open type manager interface includes dynamic type operations to
    create, delete, update, and access types.  See the "Functional
    Specification for Oracle Object Call Interface (Objects Project),
    Version 1.0" for a user level description of the OTM.  For a more
    detailed description, see the "Component Document for the Open Type
    Manager, Version 1.0".

    NOTE: MOST Of the functions in this header file are being desupported.
          Please use the OCIDescribeAny interface as described in oci.h
          instead.
          The OCIType, OCITypeElem, OCITypeMethod abstract types continue
          to be supported. The only two functions that remain to be documented
          are OCITypeArrayByName and OCITypeArrayByRef.
          All obsolete types/functions are marked accordingly below.

  RELATED DOCUMENTS

    For the functional specification for the OTM, see: 
        [1] Kotsovolos, Susan, "Functional Specification for Oracle Object
            Call Interface (Objects Project), Version 1.0", Oracle
            Corporation, February 1995.
    For the internal design of the OTM, see the following:
        [2] Kotsovolos, Susan, "Component Document for the Open Type Manager",
            Oracle Corporation, November 1994. 
        [3] Kotsovolos, Susan, "Design for The Open Type Manager, Oracle 
            Object Management Subsystem Version 1.0", Oracle Corporation, 
            March 1994.
        [4] Kotsovolos, Susan and Tin A. Nguyen, "The Open Type Manager",
            Oracle Corporation, March 1994.
        [5] Kotsovolos, Susan and Tin A. Nguyen, "Schema Evolution",
            Oracle Corporation, March 1994.
    For a description of the types the OTM must support, see:
        [6] Nguyen, Tin A., "The Open Type System", Oracle Corporation, 
            February 1994. 

  INSPECTION STATUS 

    Inspection date:    
    Inspection status: 
    Estimated increasing cost defects per page:
    Rule sets:        

  ACCEPTANCE REVIEW STATUS 

    Review date:    
    Review status:
    Reviewers:  


  **** ALL OBSOLETE FUNCTIONS/TYPES ARE MARKED ACCORDINGLY ***

  EXPORT FUNCTIONS

    None

  PUBLIC DATA STRUCTURES

      OCIType - type descriptor in the object cache
      OCITypeElem - type element descriptor in the object cache
            (used for attributes and paramters)
      OCITypeCode  - Open Type System type code.
      OCITypeMethod - method descriptor in the object cache
      OCITypeParamMode - parameter modes (ie. IN, IN-OUT etc)

  PUBLIC FUNCTIONS

    ITERATOR (for OCITypeAttrNext and OCITypeMethodNext)

      OCITypeIterNew  - ** OBSOLETE ** Create new instance of an iteraton.
      OCITypeIterSet  - ** OBSOLETE ** Initialize iterator.
      OCITypeIterFree - ** OBSOLETE ** Free instance of iterator.

    TYPE GET

      OCITypeByName      - ** OBSOLETE ** Get a type by name.
      OCITypeArrayByName - Get an array of types by their names.
      OCITypeByRef       - ** OBSOLETE ** Get a type by its CREF.
      OCITypeArrayByRef  - Get an array of types by their CREFs.
      OCITypeByFullName  - Get a type using a full name string.
      OCITypeArrayByFullName - Get an array of types using a full name string.

    TYPE ACCESSORS

      OCITypeName     - ** OBSOLETE ** OCI Get a type's name.
      OCITypeSchema   - ** OBSOLETE ** OCI Get a type's schema name.
      OCITypePackage  - OCI Get a type's package name.
      OCITypeTypeCode - ** OBSOLETE ** OCI Get a type's type code.
      OCITypeVersion  - ** OBSOLETE ** OCI Get a Type's user-readable Version.
      OCITypeAttrs    - ** OBSOLETE ** OCI Get a Type's Number of Attributes.
      OCITypeMethods  - ** OBSOLETE ** OCI Get a Type's Number of Methods.

    TYPE ELEMENT ACCESSORS (they represent attributes/parameters/results)

      OCITypeElemName      - ** OBSOLETE ** Get a type element's (only for
                                attributes) name.
      OCITypeElemType      - ** OBSOLETE ** Get a type element's type
                                descriptor.
      OCITypeElemTypeCode  - ** OBSOLETE ** Get a type element's typecode.
      OCITypeElemParameterizedType - ** OBSOLETE ** Get a type element's
                                        parameterized type's type descriptor.
      OCITypeElemNumPrec   - ** OBSOLETE ** Get a number's precision.
      OCITypeElemNumScale  - ** OBSOLETE ** Get a decimal or oracle Number's
                                            Scale
      OCITypeElemCharSetID - ** OBSOLETE ** Get a fixed or variable length
                                            string's character set ID.
      OCITypeElemCharSetForm - ** OBSOLETE ** Get a fixed or variable length
                                              string's character set form (how
                                              character set information has
                                              been specified).
      OCITypeElemLength    - ** OBSOLETE ** Get a raw, fixed or variable
                                            length string's length.
      OCITypeElemParamMode - ** OBSOLETE ** Get element's parameter's mode
                                            (only valid for parameter).
      OCITypeElemDefaultValue - ** OBSOLETE ** Get element's Default Value.

    ATTRIBUTE ACCESSORS

      OCITypeAttrByName - ** OBSOLETE ** Get an Attribute by Name.
      OCITypeAttrNext   - ** OBSOLETE ** Get an Attribute by Iteration.

    COLLECTION ACCESSORS

      OCITypeCollTypeCode - ** OBSOLETE ** Get a named collection's typecode.
      OCITypeCollElem     - ** OBSOLETE ** Get a named collection's element's
                               type element information.
      OCITypeCollSize     - ** OBSOLETE ** Get a named collection's size in
                               number of elements.

    METHOD ACCESSORS

      OCITypeMethodOverload - ** OBSOLETE ** Get number of overloaded methods
                                             with the given method name.
                                             (no direct equivalent for
                                              OCIDescribe interface)
      OCITypeMethodByName   - ** OBSOLETE ** Get one or more methods by name.
      OCITypeMethodNext     - ** OBSOLETE ** Iterate to the next method to
                                             retrieve.
      OCITypeMethodName     - ** OBSOLETE ** Get method's name.
      OCITypeMethodEncap    - ** OBSOLETE ** Get method's encapsulation level. 
      OCITypeMethodFlags    - ** OBSOLETE ** et method's flags.
      OCITypeMethodMap      - ** OBSOLETE ** Get type's map function.
      OCITypeMethodOrder    - ** OBSOLETE ** Get type's order function.
      OCITypeMethodParams   - ** OBSOLETE ** Get a method's number of
                                             parameters.

    RESULT ACCESSORS

      OCITypeResult - ** OBSOLETE ** OCI Get a method's Result.

      See also ATTRIBUTE/PARAMETER/RESULT TYPE ACCESSORS.

    PARAMETER ACCESSORS

      OCITypeParamByPos  - ** OBSOLETE ** Get a Parameter in a method By
                                          Position.
      OCITypeParamByName - ** OBSOLETE ** Get a Parameter in a method By Name.
      OCITypeParamPos    - ** OBSOLETE ** Get a Parameter's PoSition in a
                                          method.

  CALL GRAPHS:

  Only type accessors are supported for 8.0.
  ** OBSOLETE ** please use OCIDescribe interface

  TYPE ACCESSOR EXAMPLE

      CREATE TYPE CAR
      (
        name   vstring,
        age    number,
          number   car_age;                           /o Oracle number o/
          weight   car_weight;                        /o abstract type o/

        PUBLIC: 

          /o methods o/
          car(orlvs a_name, number an_age, WEIGHT a_weight);
          ~car();
          inline number get_age() const;
          
          /o relative ordering (map) functions o/
          number car_map
      );

      /o the following code accesses the type created above o/

      ub1      meth_flags;
      ub4      i, j; 
      ub4      text_len, position;
      ub4      count;
      ub4      length;
      OCITypeCode    typecode;
      OCIRef  *attr_ref;
      OCIRef  *param_ref;
      OCIType  *tdo, new_tdo, final_tdo;
      OCITypeElem  *elem;
      OCITypeIter  *iterator_ort;
      oratext   (*names)[];
      ub4      lengths[];
      ub4     *positions;
      oratext    *name;
      oratext     name_buffer[M_IDEN];

      /o initialize the references o/
      DISCARD orlrini(env, err, (dvoid *)&attr_ref);
      DISCARD orlrini(env, err, (dvoid *)&param_ref);

      /o ----------------- GET INFORMATION ABOUT A TYPE ----------------- o/

      /o start a transaction o/
      
      /o Pin the type until the end of the transaction.  Pinning the type is 
       o required before using any type accessors. 
       o/
      if (OCITypeByName(env, err, svc, (oratext *)0, 0, "CAR", strlen("CAR"), 
                  OCI_DURATION_TRANS, &car_ref, &car_tdo) != OCI_SUCCESS)
        /o error o/ ;

      /o get the type's name o/
      if (!memcmp(OCITypeName(env, err, car_tdo, &text_len), "person",
                              text_len))
        /o do something o/ ;

      /o get the type's schema name o/
      if (!memcmp(OCITypeSchema(env, err, car_tdo, &text_len), "john",
                  text_len))
        /o do something o/ ;

      /o get the type code of the type o/
      if (OCITypeTypeCode(env, err, car_tdo) == OCI_TYPECODE_ADT)
        /o do something o/ ; 

      /o get the type version o/
      if (!memcmp(OCITypeVersion(env, err, car_tdo, &text_len), "1", text_len))
         /o do something o/  ;

      /o ------- GET FLATTENED POSITION OF AN ATTRIBUTES IN A TYPE ------- o/

      names = malloc(sizeof(oratext *) * 2);
      names[0] = malloc(strlen("car_weight"));
      names[1] = malloc(strlen("ounces"));
      memcpy(names[0], "car_weight", strlen("car_weight"));
      memcpy(names[1], "ounces", strlen("ounces"));

      lengths = malloc(sizeof(ub4) * 2);
      lengths[0] = strlen("car_weight");
      lengths[1] = strlen("ounces");

      /o ---------- GET IMMEDIATE ATTRIBUTES IN A TYPE ---------- o/

      /o loop through all attributes in the type with iterator o/
      if (OCITypeIterNew(env, err, car_tdo, &iterator_ort) != OCI_SUCCESS)
        /o do something o/

      while (OCITypeAttrNext(env, err, iterator_ort, &ado) != OCI_NO_DATA)
      {
        /o get the attribute's name o/
        if (!memcmp(OCITypeElemName(env, err, ado, &text_len),
                                    "tiger", text_len))
          /o do something o/  ;

        /o get the attribute's type descriptor o/
        if (OCITypeElemType(env, err, ado, &tdo) != OCI_SUCCESS)
          /o error o/ ;

        /o get the attribute's type code o/
        typecode = OCITypeElemTypeCode(env, err, ado);

        switch (typecode)
        {
        /o scalar types o/
        case OCI_TYPECODE_DATE:                                     /o date o/
        case OCI_TYPECODE_SIGNED8:                                  /o byte o/
        case OCI_TYPECODE_SIGNED16:                                /o short o/
        case OCI_TYPECODE_UNSIGNED8:                       /o unsigned byte o/
        case OCI_TYPECODE_UNSIGNED16:                     /o unsigned short o/
        case OCI_TYPECODE_OCTET:                                   /o octet o/
        case OCI_TYPECODE_TABLE:                            /o nested table o/
        case OCI_TYPECODE_CLOB:                            /o character lob o/
        case OCI_TYPECODE_BLOB:                               /o binary lob o/
        case OCI_TYPECODE_CFILE:                   /o character file object o/
        case OCI_TYPECODE_BFILE:                      /o binary file object o/

          /o do something o/
          break;

        /o number types o/
        case OCI_TYPECODE_NUMBER:                          /o oracle number o/
        case OCI_TYPECODE_DECIMAL:                               /o decimal o/
          {
            /o get the scale of the number o/
            if (OCITypeElemNumScale(env, err, ado) == 3)
              /o do something o/ ;
          }
          /o fall through to get the precision o/

        case OCI_TYPECODE_FLOAT:                                   /o float o/
        case OCI_TYPECODE_SIGNED32:                                 /o long o/
        case OCI_TYPECODE_UNSIGNED32:                      /o unsigned long o/
        case OCI_TYPECODE_REAL:                                     /o real o/
        case OCI_TYPECODE_DOUBLE:                                 /o double o/
          {
            /o get the precision of the number o/
            if (OCITypeElemNumPrec(env, err, ado) == 2)
              /o do something o/ ;
          }
          break;

        /o string types o/
        case OCI_TYPECODE_CHAR:                      /o fixed length string o/
        case OCI_TYPECODE_VARCHAR2:               /o variable length string o/
        case OCI_TYPECODE_RAW:                                       /o raw o/
          {
            /o get the length of the fixed or variable length string o/
            if (OCITypeElemLength(env, err, ado) < 100)
              /o do something o/
          }
          break;

        /o parameterized types o/
        case OCI_TYPECODE_REF:                          /o reference o/
        case OCI_TYPECODE_PTR:                            /o pointer o/
          {
            /o get the type stored in the parameterized type o/
            if (OCITypeElemParameterizedType(env, err, ado, &tdo)
                 != OCI_SUCCESS)
              /o error o/ ;

            /o do something o/
            if (OCI_TYPEELEM_IS_REF(OCITypeElemFlags(env, err, ado)))...
          }
          break;

        /o domain type o/
        case OCI_TYPECODE_NAMEDCOLLECTION:
          switch (OCITypeCollTypeCode(env, err, tdo))
          {
          case OCI_TYPECODE_VARRAY:                   /o variable array o/
            ub4   num_elems;
            OCIType *element_type;

            /o get the number of elements in the farray or the maximum number
             o of elements in the varray.
             o/
            OCITypeCollSize(env, err, tdo, &num_elems);

            /o get the type of the array o/
            OCITypeElemType(env, err, tdo, &element_type);
          }
          break;

          case OCI_TYPECODE_TABLE:                         /o multiset o/
          {
            OCIType *table_type;

            /o get the type of the multiset o/
            OCITypeElemType(env, err, tdo, &table_type);

            /o do something o/
          }
        }

        /o abstract type o/
        case OCI_TYPECODE_ADT:                 /o abstract data type o/
          {
            /o get the adt information o/
            if (OCITypeElemType(env, err, ado, &tdo) != OCI_SUCCESS)
              /o error o/ ;

            /o do something o/
          }
          break;

        default:
          DISCARD printf("Error:  invalid type code\n");

        } /o end of typecode switch o/

      } /o end of loop through all attributes in a type o/


      /o ------------ GET THE IMMEDIATE METHODS OF A TYPE ------------ o/

      /o loop through all methods in the type by reusing iterator o/
      if (OCITypeIterSet(env, err, car_tdo, iterator_ort) != OCI_SUCCESS)
        /o do something o/

      while (OCITypeMethodNext(env, err, iterator_ort) != OCI_NO_DATA)
      {
        /o get the method's name o/
        if (!memcmp(OCITypeMethodName(env, err, mdo, &text_len), "car",
                    text_len))
          /o do something o/  ;

        /o get the method's encapsulation o/
        if (OCITypeMethodEncap(env, err, mdo) == OCI_TYPEENCAP_PUBLIC)
          /o do something o/  ;

        /o get the method's flags o/
        meth_flags = OCITypeMethodFlags(env, err, mdo);
        if (meth_flags & OCI_TYPEMETHOD_VIRTUAL)  
          /o do something o/  ;


        /o ------------ GET THE PARAMETERS IN A METHOD ------------ o/

        /o loop through all parameters in the method o/
        count = OCITypeMethodParams(env, err, mdo);
        for (j = 1; j <= count; j++)
        {
          /o get the parameter information by position o/
          if (OCITypeParamByPos(env, err, mdo, i, &elem) != OCI_SUCCESS)
            /o error o/ ;

          /o get the parameter's name o/
          if (!memcmp(OCITypeElemName(env, err, elem, &text_len), "an_age",
                      text_len))
            /o do something o/ ;

          /o get the parameter's mode o/
          if (OCITypeElemMode(env, err, elem) == OCI_PARAM_OUT)
            /o do something o/ ;
   
          /o get the parameter's required flag o/
          if (ortgprq(env, err, elem))
            /o do something o/ ;
        }
      }

      /o get a method by name o/
      if (OCITypeMethodByName(env, err, car_tdo, "car_constructor", 
                              strlen("car_constructor"), NULLP(OCIRef), &mdo) 
                              != OCI_SUCCESS)
        /o error o/ ;

      /o get a parameter in a method by name o/
      if (OCITypeParamByName(env, err, mdo, "an_age", strlen("an_age"), &elem) 
                  != OCI_SUCCESS)
        /o error o/ ;

      /o get a parameter's typecode o/
      typecode = OCITypeElemTypeCode(env, err, elem);
   
      /o get a parameter's type object o/
      if (OCITypeElemType(env, err, elem, &tdo)) != OCI_SUCCESS)
        /o error o/ ;

      /o get a parameter's position in a method o/
      if (ortgpps(env, err, mdo, "an_age", strlen("an_age"), 
                  &position, NULLP(OCIRef), NULLP(OCITypeElem)) != OCI_SUCCESS)
        /o error o/ ;

      /o ------------ GET THE METHOD's RESULT ------------ o/
     
      /o get a method by name o/
      if (OCITypeMethodByName(env, err, car_tdo, "get_age", strlen("get_age"),
                  &mdo) != OCI_SUCCESS)
        /o error o/ ;

      /o get the typecode of the method's result o/
      typecode = OCITypeElemTypeCode(env, err, mdo);


      /o ----------------- END ---------------- o/

      /o free the references implicitly allocated o/
      DISCARD orlrfre(env, err, (dvoid *)&attr_ref);
      DISCARD orlrfre(env, err, (dvoid *)&param_ref);

  NOTES

  MODIFIED
    dmukhin    06/29/05  - ANSI prototypes; miscellaneous cleanup 
    srseshad   03/12/03  - convert oci public api to ansi
    aahluwal   06/03/02  - bug 2360115
    skabraha   04/16/02  - fix compiler warnings
    rkasamse   03/02/01  - do not use iterator : keyword in MSVB
    bpalaval   02/09/01  - Change text to oratext.
    rxgovind   01/31/00  - add OCIType interfaces for transient types
    whe        09/01/99 -  976457:check __cplusplus for C++ code
    cxcheng    05/06/97 -  make OCI_TYPE?? test macros return either 1 or 0
    cxcheng    04/22/97 -  add comment on desupporting OCIType functions
    skrishna   03/18/97 -  fix ifdef for supporting ansi and k&r proto-types
    cxcheng    02/26/97 -  fix lint problem with oro names
    cxcheng    02/06/97 -  take out short name support except with SLSHORTNAME
    cxcheng    01/15/97 -  change prototype of OCITypeElemParameterizedType()
    cxcheng    01/03/97 -  replace bit in OCI_TYPEPARAM_IS_REQUIRED with bitwis
    cxcheng    12/31/96 -  replace OCI_PARAM_IS_REQUIRED with OCI_TYPEPARAM_IS_
    cxcheng    12/09/96 -  add prototype for OCITypeElemExtTypeCode and OCIType
    cxcheng    11/25/96 -  add schema name parameter to OCITypeVTInsert()
    cxcheng    11/20/96 -  fix prototype for OCITypeByName()
    cxcheng    11/11/96 -  fix prototype for OCITypeByName()
    cxcheng    11/05/96 -  remove OCITypeElemExtTypeCode and OCITypeCollExtType
    dchatter   10/28/96 -  change ortgatyp to be OCITypeArrayByName
    cxcheng    10/25/96 -  fix problem with ortgatyp at end
    cxcheng    10/22/96 -  add OCITypeByRef and OCITypeArrayByRef
    cxcheng    10/20/96 -  remove ortgtyp() from #define section at end
    cxcheng    10/18/96 -  rename OCITypeGetArray to OCITypeArrayByName
    cxcheng    10/17/96 -  final change to prototype for OCI_TYPEPARAM_IS_REQUI
    cxcheng    10/15/96 -  rename OCIEncapLevel and OCIMethodFlag
    cxcheng    10/14/96 -  change prototype of OCITypeResult
    mluong     10/11/96 -  fix compile error
    jwijaya    10/10/96 -  fix bug on OCI_PARAM_IS_REQUIRED
    cxcheng    10/09/96 -  more lint and link fixes
    cxcheng    10/08/96 -  more lint fixes
    cxcheng    10/07/96 -  more changes
    cxcheng    10/04/96 -  replace short names with long names
    cxcheng    10/01/96 -  change to long names for readability
    cxcheng    09/27/96 -  rename ortgatyp() to ortgtya() for lint
    cxcheng    09/20/96 -  add ortgatyp() for array get type
    cxcheng    09/18/96 -  add array pin and iterator functions
    cxcheng    08/09/96 -  add version table calls
    cxcheng    07/22/96 -  add OCITypeElemType() to top
    jwijaya    07/03/96 -  add ANSI prototypes
    cxcheng    06/28/96 -  add OCITypeElemCharSetForm()
    cxcheng    06/26/96 -  fix comment on OCITypeParamByPos()/ortgpps()
    cxcheng    06/18/96 -  fix comments on OCITypeResult()
    cxcheng    06/17/96 -  improve comments
    skrishna   06/03/96 -  change OCITypeCollElem() prototype
    vkrishna   05/29/96 -  replace OROTCFAR with OROTCCAR
    cxcheng    05/28/96 -  fix comments, remove non-beta1 functions
    cxcheng    05/02/96 -  fix prototype bugs
    cxcheng    04/29/96 -  rename OCITypeElemm() to ortanct()
    cxcheng    04/26/96 -  add ortgrbp and ortftyi,
                           fix comments and examples
    cxcheng    04/22/96 -  big merge to main branch
    cxcheng    04/17/96 -  fix syntax
    cxcheng    04/08/96 -  change prototype to ortaty()
    skrishna   04/08/96 -  change ort*() to take OCIEnv* and OCIError* instead
                           of oroenv*
    cxcheng    03/28/96 -  add ortslob(), change ortsstr() prototype
    cxcheng    03/13/96 -  change alter type interface
    cxcheng    03/11/96 -  ORT interface changes
    cxcheng    02/27/96 -  correct comments
    jboonleu   02/09/96 -  rename oroopd to OCIDuration
    cxcheng    01/19/96 -  change ORTCTYVAL to ORTCTYEMB for embedded ADT
    cxcheng    02/14/96 -  add more comments
    jboonleu   02/09/96 -  rename oroopd to OCIDuration
    cxcheng    02/07/96 -  fix comments and examples
    cxcheng    01/19/96 -  new ORT interface without korfc's
    cxcheng    01/08/96 -  consolidate collection functions
    cxcheng    12/14/95 -  remove obsolete ortgcol() and ortrelease()
    jweisz     12/12/95 -  merge screwup: ortdth twice
    cxcheng    12/05/95 -  change multiset interface for new standard
    skotsovo   12/01/95 -  merge from /vobs/rdbms/public/ort.h@@/main/
                           st_rdbms_big_dev/st_rdbms_obj/
                           st_rdbms_jwijaya_variable_ref
    cxcheng    11/13/95 -  add ortaty()/orteaty()
    cxcheng    11/13/95 -  add new collection type accessors
    skotsovo   10/30/95 -  add 'oid' type b/c extent type uses it.
    skotsovo   10/24/95 -  update according to new variable length ref
    cxcheng    10/05/95 -  add null support, change prototypes to calls
    cxcheng    10/03/95 -  add OCITypeMethodOrder() to get ORDER method
    cxcheng    09/28/95 -  add OCITypeElemm() for collection types support
    skotsovo   06/05/95 -  add adt_type parameter to ortsab() 
    skotsovo   05/10/95 -  ifdef'd out ortgafp() 
    skotsovo   03/07/95 -  update interface to only include release 1
    skotsovo   02/22/95 -  add multiset accessors 
    skotsovo   02/09/95 -  update according to new ots doc 
    skotsovo   01/31/95 -  add rest of release 1 types 
    skotsovo   01/24/95 -  categorize sint32, double, and real as number types 
                           (with precision and scale) instead of scalar types.
    skotsovo   01/12/95 -  remove dependency from ortdty interface 
    skotsovo   01/03/95 -  remove orotyp accessors 
    skotsovo   12/12/94 -  update comments 
    skotsovo   12/05/94 -  change OCITypeElemParameterizedTyper interface 
    skotsovo   10/26/94 -  add type version table 
    skotsovo   10/17/94 -  fix ortgafp() comments 
    skotsovo   10/14/94 -  modify ortgafp() parameters 
    skotsovo   10/14/94 -  add examples 
    skotsovo   10/13/94 -  add a few new routines 
    jwijaya    10/07/94 -  add namespace to pin by name 
    jwijaya    10/02/94 -  connection handle -> connection number 
    skotsovo   09/13/94 -  modify example to use updated oririni interface 
    skotsovo   08/25/94 -  change scale to sb1 from sb2 
    skotsovo   07/28/94 -  add ortbeg() and ortend() 
    skotsovo   07/14/94 -  add decimal type & call graph
    skotsovo   06/28/94 -  subset by removing miscellaneous functions 
    skotsovo   06/28/94 -  consistently put comments before typedefs 
    skotsovo   06/27/94 -  modify according to new header file template, add
                           more examples, and change ortcty() to return a
                           reference to the type
    skotsovo   06/24/94 -  add functions to get type information from orotyp 
    skotsovo   06/20/94 -  finish modifying according to header template 
    skotsovo   06/09/94 -  modify according to header file template 
    skotsovo   06/08/94 -  replace s.h with oratypes.h 
    skotsovo   05/24/94 -  modify comments & update example
    skotsovo   05/23/94 -  modify fnt names for create, alter and drop type 
    skotsovo   05/18/94 -  remove ortdme() -- delete a method 
    skotsovo   05/17/94 -  add tdo parameter to all type modifiers 
    skotsovo   05/11/94 -  return text* instead of including it in arglist 
    skotsovo   11/16/93 -  creation

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
#define ORT_ORACLE

/*---------------------------------------------------------------------------*/
/*                         SHORT NAMES SUPPORT SECTION                       */
/*---------------------------------------------------------------------------*/

#ifdef SLSHORTNAME

/* the following are short names that are only supported on IBM mainframes
   with the SLSHORTNAME defined.
   With this all subsequent long names will actually be substituted with
   the short names here */

#define OCITypeArrayByName              ortgatyp
#define OCITypeAttrByName               ortgabn
#define OCITypeAttrNext                 ortgabi
#define OCITypeAttrs                    ortgtna
#define OCITypeByRef                    ortgtbrf
#define OCITypeCollElem                 ortgcel
#define OCITypeCollExtTypeCode          ortgcsqt
#define OCITypeCollSize                 ortgcne
#define OCITypeCollTypeCode             ortgdttc
#define OCITypeElem                     ortado
#define OCITypeElemCharSetForm          ortgscform
#define OCITypeElemCharSetID            ortgscid
#define OCITypeElemDefaultValue         ortgpdv
#define OCITypeElemExtTypeCode          ortgasqt
#define OCITypeElemLength               ortgsl
#define OCITypeElemName                 ortganm
#define OCITypeElemNumPrec              ortgnp
#define OCITypeElemNumScale             ortgns
#define OCITypeElemParamMode            ortgpmo
#define OCITypeElemParameterizedType    ortgpa
#define OCITypeElemType                 ortgaty
#define OCITypeElemTypeCode             ortgatc
#define OCITypeIter                     ortitr
#define OCITypeIterFree                 ortifre
#define OCITypeIterNew                  ortinew
#define OCITypeIterSet                  ortiset
#define OCITypeMethod                   ortmdo
#define OCITypeMethodByName             ortgmbn
#define OCITypeMethodEncap              ortgmen
#define OCITypeMethodFlags              ortgmfl
#define OCITypeMethodMap                ortgmmap
#define OCITypeMethodName               ortgmnm
#define OCITypeMethodNext               ortgmbi
#define OCITypeMethodOrder              ortgmor
#define OCITypeMethodOverload           ortgmno
#define OCITypeMethodParams             ortgmnp
#define OCITypeMethods                  ortgtnm
#define OCITypeName                     ortgtme
#define OCITypeParamByName              ortgpbn
#define OCITypeParamPos                 ortgpps
#define OCITypeSchema                   ortgtsch
#define OCITypeTypeCode                 ortgttc
#define OCITypeVTInit                   ortvini
#define OCITypeVTInsert                 ortvins
#define OCITypeVTSelect                 ortvsel
#define OCITypeVersion                  ortgtvn

#endif                                                        /* SLSHORTNAME */


/*============================*/        
/* PUBLIC TYPES AND CONSTANTS */
/*============================*/

/*----------------------------- TYPE DESCRIPTION ----------------------------*/

/*
 * OCIType - OCI Type Description Object 
 *
 * The contents of an 'OCIType' is private/opaque to clients.  Clients just 
 * need to declare and pass 'OCIType' pointers in to the type manage
 * functions.
 * The pointer points to the type in the object cache.  Thus, clients don't
 * need to allocate space for this type and must NEVER free the pointer to the
 * 'OCIType'.
 */

typedef struct OCIType OCIType;

/*------------------------- TYPE ELEMENT DESCRIPTION ------------------------*/


/*
 * OCITypeElem - OCI Type Element object
 *
 * The contents of an 'OCITypeElem' is private/opaque to clients. Clients just 
 * need to declare and pass 'OCITypeElem' pointers in to the type manager 
 * functions.
 *
 * 'OCITypeElem' objects contains type element information such as the numeric
 * precision for example, for number objects, and the number of elements for
 * arrays.
 * They ARE used to describe type attributes, collection elements,
 * method parameters, and method results. Hence they are pass in or returned
 * by attribute, collection, and method parameter/result accessors.
 */

typedef struct OCITypeElem OCITypeElem;


/*--------------------------- METHOD DESCRIPTION ---------------------------*/


/*
 * OCITypeMethod - OCI Method Description object 
 *
 * The contents of an 'OCITypeMethod' is private/opaque to clients.  Clients
 * just need to declare and pass 'OCITypeMethod' pointers in to the type
 * manager functions.
 * The pointer points to the method in the object cache.  Thus, clients don't
 * need to allocate space for this type and must NEVER free the pointer to 
 * the 'OCITypeMethod'.
 */

typedef struct OCITypeMethod OCITypeMethod;


/*--------------------------- TYPE ACCESS ITERATOR --------------------------*/

/*
 * OCITypeIter- OCI Type Iterator
 *
 * The contents of an 'orti' is private/opaque to clients.  Clients just 
 * need to declare and pass 'orti' pointers in to the type manager functions.
 * The iterator is used to retreive MDO's and ADO's that belong to the TDO
 * one at a time. It needs to be allocated by the 'OCITypeIterNew()' function
 * call and deallocated with the 'OCITypeIterFree()' function call.
 */

typedef struct OCITypeIter OCITypeIter;


/*==================*/
/* PUBLIC FUNCTIONS */
/*==================*/

/*--------------------------------------------------------------------------*/
/*                                  ITERATOR                                */
/*--------------------------------------------------------------------------*/

/*-----------------------_- OCITypeIterNew ---------------------------------*/

/* ** OBSOLETE ** */
sword OCITypeIterNew(    OCIEnv *env, OCIError *err, OCIType *tdo, 
                         OCITypeIter **iterator_ort    );

/*
  NAME: OCITypeIterNew - OCI Iterator NEW
  PARAMETERS:
       env (IN/OUT) - OCI environment handle initialized in object mode
       err (IN/OUT) - error handle. If there is an error, it is
                recorded in 'err' and this function returns OCI_ERROR.
                The error recorded in 'err' can be retrieved by calling
                OCIErrorGet().
       tdo (IN) - pointer to the pinned type in the object cache to
                initialize the iterator with
       iterator_ort (OUT) - pointer to the pointer to the new iterator created
  DESCRIPTION:
       Create a new instance of a method/attribute iterator and initalize
       it's values.
  RETURNS:
        OCI_SUCCESS if the function completes successfully.
        OCI_INVALID_HANDLE if 'env' or 'err' is null. 
        OCI_ERROR if
           1) any of the required parameters is null.
           2) error while allocating space for the iterator.
*/

/*------------------------ OCITypeIterSet ---------------------------------*/

/* ** OBSOLETE ** */
sword OCITypeIterSet(    OCIEnv *env, OCIError *err, OCIType *tdo, 
                  OCITypeIter *iterator_ort    );

/*
  NAME: OCITypeIterSet - OCI Iterator SET
  PARAMETERS:
       env (IN/OUT) - OCI environment handle initialized in object mode
       err (IN/OUT) - error handle. If there is an error, it is
                recorded in 'err' and this function returns OCI_ERROR.
                The error recorded in 'err' can be retrieved by calling
                OCIErrorGet().
       tdo (IN) - pointer to the pinned type in the object cache to
                initialize the iterator with
       iterator_ort (IN/OUT) - pointer to the iterator to set
  DESCRIPTION:
       Initializes the iterator. This is used to reset the state of the
       iterator.
  RETURNS:
        OCI_SUCCESS if the function completes successfully.
        OCI_INVALID_HANDLE if 'env' or 'err' is null. 
        OCI_ERROR if
           1) any of the required parameters is null.
*/

/*------------------------ OCITypeIterFree ---------------------------------*/

/* ** OBSOLETE ** */
sword OCITypeIterFree(    OCIEnv *env, OCIError *err, OCITypeIter
                          *iterator_ort    );

/*
  NAME: OCITypeIterFree - OCI Iterator FREe
  PARAMETERS:
       env (IN/OUT) - OCI environment handle initialized in object mode
       err (IN/OUT) - error handle. If there is an error, it is
                recorded in 'err' and this function returns OCI_ERROR.
                The error recorded in 'err' can be retrieved by calling
                OCIErrorGet().
       iterator_ort (IN/OUT) - pointer to the iterator to free
  DESCRIPTION:
       Free space allocated for the iterator.
  RETURNS:
        OCI_SUCCESS if the function completes successfully.
        OCI_INVALID_HANDLE if 'env' or 'err' is null. 
        OCI_ERROR if
           1) any of the required parameters is null.
           2) error while freeing the iterator, probably bad iterator pointer.
*/


/*--------------------------------------------------------------------------*/
/*                                  TYPE GET                                */
/*--------------------------------------------------------------------------*/

/* ** OBSOLETE ** */
sword OCITypeByName(    OCIEnv *env, OCIError *err, const OCISvcCtx *svc, 
                        const oratext *schema_name, ub4 s_length,
                        const oratext *type_name, ub4 t_length,
                        const oratext *version_name, ub4 v_length,
                        OCIDuration pin_duration, OCITypeGetOpt get_option,
                        OCIType **tdo    );
/*
  NAME: OCITypeByName - OCI Get the most current version of an existing TYPe
                  by name.
  PARAMETERS:
       env (IN/OUT) - OCI environment handle initialized in object mode
       err (IN/OUT) - error handle. If there is an error, it is
                recorded in 'err' and this function returns OCI_ERROR.
                The error recorded in 'err' can be retrieved by calling
                OCIErrorGet().
       svc (IN) - OCI service handle
       schema_name (IN, optional) - name of schema associated with the 
                  type.  By default, the user's schema name is used.
       s_length (IN) - length of the 'schema_name' parameter
       type_name (IN) - name of the type to get
       t_length (IN) - length of the 'type_name' parameter
       version_name (IN, optional) - user readable version of the type.
                  Pass (oratext *)0 for the most current version.
       v_length (IN) - length of version_name in bytes. Should be 0 if
                  the most current version is to be retrieved.
       pin_duration (IN) - pin duration (e.g. until the end of current
                  transaction).  See 'oro.h' for a description of
                  each option.
       get_option (IN) - options for loading the types. It can be one of two
                   values:
                  OCI_TYPEGET_HEADER for only the header to be loaded, or
                  OCI_TYPEGET_ALL for the TDO and all ADO and MDOs to be
                    loaded.
       tdo (OUT) - pointer to the pinned type in the object cache
  DESCRIPTION:
       Get a pointer to a version of the existing type associated
       with schema/type name.
  RETURNS:
        OCI_SUCCESS if the function completes successfully.
        OCI_INVALID_HANDLE if 'env' or 'err' is null. 
        OCI_ERROR if
           1) any of the required parameters is null.
           2) the adt type associated with schema/type name does not exist.
  NOTE:
       Schema and type names are CASE-SENSITIVE. If they have been created
       via SQL, you need to use uppercase names.
*/

sword OCITypeArrayByName(    OCIEnv *env, OCIError *err, const OCISvcCtx *svc,
                             ub4 array_len,
                             const oratext *schema_name[], ub4 s_length[],
                             const oratext *type_name[], ub4 t_length[],
                             const oratext *version_name[], ub4 v_length[],
                             OCIDuration pin_duration,
                             OCITypeGetOpt get_option, OCIType **tdo    );

/*
  NAME: OCITypeArrayByName - OCI Get array of TYPes by name.
  PARAMETERS:
       env (IN/OUT) - OCI environment handle initialized in object mode
       err (IN/OUT) - error handle. If there is an error, it is
                recorded in 'err' and this function returns OCI_ERROR.
                The error recorded in 'err' can be retrieved by calling
                OCIErrorGet().
       svc (IN) - OCI service handle
       array_len (IN) - number of schema_name/type_name/version_name entries to
                  be retrieved.
       schema_name (IN, optional) - array of schema names associated with the
                  types to be retrieved.  The array must have array_len
                  elements if specified.
                  If 0 is supplied, the default schema is assumed, otherwise
                  it MUST have array_len number of elements.
                  0 can be supplied for one or more of the entries to indicate
                  that the default schema is desired for those entries.
       s_length (IN) - array of schema_name lengths with each entry
                  corresponding to the length of the corresponding schema_name
                  entry in the schema_name array in bytes.
                  The array must either have array_len number of elements or
                  it MUST be 0 if schema_name is not specified.
       type_name (IN) - array of the names of the types to retrieve. This
                  MUST have array_len number of elements.
       t_length (IN) - array of the lengths of type names in the type_name
                  array in bytes.
       version_name (IN) - array of the version names of the types to retrieve
                  corresponding. This can be 0 to indicate retrieval of the
                  most current versions, or it MUST have array_len number of
                  elements.
                  If 0 is supplied, the most current version is assumed,
                  otherwise it MUST have array_len number of elements.
                  0 can be supplied for one or more of the entries to indicate
                  that the current version is desired for those entries.
       v_length (IN) - array of the lengths of version names in the
                  version_name array in bytes.
       pin_duration (IN) - pin duration (e.g. until the end of current
                  transaction) for the types retreieve.  See 'oro.h' for a
                  description of each option.
       get_option (IN) - options for loading the types. It can be one of two
                   values:
                  OCI_TYPEGET_HEADER for only the header to be loaded, or
                  OCI_TYPEGET_ALL for the TDO and all ADO and MDOs to be
                    loaded.
       tdo (OUT) - output array for the pointers to each pinned type in the
                  object cache. It must have space for array_len pointers.
                  Use OCIObjectGetObjectRef() to obtain the CREF to each
                  pinned type descriptor.
  DESCRIPTION:
       Get pointers to the existing types associated with the schema/type name
       array. This is similar to OCITypeByName() except that all the TDO's 
       are retreived via a single network roundtrip.
  RETURNS:
        OCI_SUCCESS if the function completes successfully.
        OCI_INVALID_HANDLE if 'env' or 'err' is null. 
        OCI_ERROR if
           1) any of the required parameters is null.
           2) one or more adt types associated with a schema/type name entry
              does not exist.
*/

sword OCITypeByFullName( OCIEnv *env, OCIError *err, const OCISvcCtx *svc, 
                         const oratext *full_type_name, 
                         ub4 full_type_name_length,
                         const oratext *version_name, ub4 version_name_length,
                         OCIDuration pin_duration, OCITypeGetOpt get_option,
                         OCIType **tdo );
/*
  NAME: OCITypeByFullName - OCI Get the most current version of an existing TYPe
                            by full name.
  PARAMETERS:
       env (IN/OUT) - OCI environment handle initialized in object mode
       err (IN/OUT) - error handle. If there is an error, it is
                recorded in 'err' and this function returns OCI_ERROR.
                The error recorded in 'err' can be retrieved by calling
                OCIErrorGet().
       svc (IN) - OCI service handle
       full_type_name (IN, optional) - full name of type.   
       full_type_name_length (IN) - length of the full_type_name parameter.
       version_name (IN, optional) - user readable version of the type.
                  Pass (oratext *)0 for the most current version.
       version_name_length (IN) - length of version_name in bytes. Should be 0
                  if the most current version is to be retrieved.
       pin_duration (IN) - pin duration (e.g. until the end of current
                  transaction).  See 'oro.h' for a description of
                  each option.
       get_option (IN) - options for loading the types. It can be one of two
                   values:
                  OCI_TYPEGET_HEADER for only the header to be loaded, or
                  OCI_TYPEGET_ALL for the TDO and all ADO and MDOs to be
                    loaded.
       tdo (OUT) - pointer to the pinned type in the object cache
  DESCRIPTION:
       Get a pointer to a version of the existing type associated
       with a full type name.
  RETURNS:
        OCI_SUCCESS if the function completes successfully.
        OCI_INVALID_HANDLE if 'env' or 'err' is null. 
        OCI_ERROR if
           1) if any of the parameters are detected to be incorrect.
           2) the adt type associated with schema/type name does not exist.
  NOTE:
       Schema and type names are CASE-SENSITIVE. If they have been created
       via SQL, you need to use uppercase names.
*/

sword OCITypeArrayByFullName( OCIEnv *env, OCIError *err, const OCISvcCtx *svc,
                              ub4 array_len,
                              const oratext *full_type_name[], 
                              ub4 full_type_name_length[],
                              const oratext *version_name[], 
                              ub4 version_name_length[],
                              OCIDuration pin_duration,
                              OCITypeGetOpt get_option, OCIType **tdo    );

/*
  NAME: OCITypeArrayByFullName - OCI Get array of TYPes by name.
  PARAMETERS:
       env (IN/OUT) - OCI environment handle initialized in object mode
       err (IN/OUT) - error handle. If there is an error, it is
                recorded in 'err' and this function returns OCI_ERROR.
                The error recorded in 'err' can be retrieved by calling
                OCIErrorGet().
       svc (IN) - OCI service handle
       array_len (IN) - number of full_type_name/version_name entries to
                  be retrieved.
       full_type_name (IN) - array of the full names of the types to retrieve. 
                  This MUST have array_len number of elements.
       full_type_name_length (IN) - array of the lengths of full type names in
                  the full_type_name array in bytes.
       version_name (IN) - array of the version names of the types to retrieve
                  corresponding. This can be 0 to indicate retrieval of the
                  most current versions, or it MUST have array_len number of
                  elements.
                  If 0 is supplied, the most current version is assumed,
                  otherwise it MUST have array_len number of elements.
                  0 can be supplied for one or more of the entries to indicate
                  that the current version is desired for those entries.
       version_name_length (IN) - array of the lengths of version names in the
                  version_name array in bytes.
       pin_duration (IN) - pin duration (e.g. until the end of current
                  transaction) for the types retreieve.  See 'oro.h' for a
                  description of each option.
       get_option (IN) - options for loading the types. It can be one of two
                   values:
                  OCI_TYPEGET_HEADER for only the header to be loaded, or
                  OCI_TYPEGET_ALL for the TDO and all ADO and MDOs to be
                    loaded.
       tdo (OUT) - output array for the pointers to each pinned type in the
                  object cache. It must have space for array_len pointers.
                  Use OCIObjectGetObjectRef() to obtain the CREF to each
                  pinned type descriptor.
  DESCRIPTION:
       Get pointers to the existing types associated with the schema/type name
       array. This is similar to OCITypeByFullName() except that all the TDO's 
       are retreived via a single network roundtrip.
  RETURNS:
        OCI_SUCCESS if the function completes successfully.
        OCI_INVALID_HANDLE if 'env' or 'err' is null. 
        OCI_ERROR if
           1) if any of the parameters are detected to be incorrect.
           2) one or more adt types associated with a schema/type name entry
              does not exist.
*/

sword   OCITypeByRef(    OCIEnv *env, OCIError *err,
                         const OCIRef *type_ref, OCIDuration pin_duration,
                         OCITypeGetOpt get_option, OCIType **tdo    );

/*
  NAME: OCITypeArrayByRef - OCI Get array of TYPes by REF.
  PARAMETERS:
       env (IN/OUT) - OCI environment handle initialized in object mode
       err (IN/OUT) - error handle. If there is an error, it is
                recorded in 'err' and this function returns OCI_ERROR.
                The error recorded in 'err' can be retrieved by calling
                OCIErrorGet().
       type_ref (IN) - OCIRef * pointing to the particular version of
                  the type descriptor object to obtain.
                  The array must have array_len elements if specified.
       pin_duration (IN) - pin duration (e.g. until the end of current
                  transaction) for the type to retreieve.  See 'oro.h' for a
                  description of each option.
       get_option (IN) - options for loading the type. It can be one of two
                   values:
                  OCI_TYPEGET_HEADER for only the header to be loaded, or
                  OCI_TYPEGET_ALL for the TDO and all ADO and MDOs to be
                    loaded.
       tdo (OUT) - pointer to the pinned type in the object cache
  DESCRIPTION:
       Get pointers to the 
       with the schema/type name array. This is similar to OCITypeByName()
       except that all the TDO's are retreived via a single network roundtrip.
  RETURNS:
        OCI_SUCCESS if the function completes successfully.
        OCI_INVALID_HANDLE if 'env' or 'err' is null. 
        OCI_ERROR if
           1) any of the required parameters is null.
           2) one or more adt types associated with a schema/type name entry
              does not exist.
*/

sword   OCITypeArrayByRef(    OCIEnv *env, OCIError *err,
                              ub4 array_len, const OCIRef **type_ref,
                              OCIDuration pin_duration,
                              OCITypeGetOpt get_option, OCIType **tdo    );

/*
  NAME: OCITypeArrayByRef - OCI Get array of TYPes by REF.
  PARAMETERS:
       env (IN/OUT) - OCI environment handle initialized in object mode
       err (IN/OUT) - error handle. If there is an error, it is
                recorded in 'err' and this function returns OCI_ERROR.
                The error recorded in 'err' can be retrieved by calling
                OCIErrorGet().
       array_len (IN) - number of schema_name/type_name/version_name entries to
                  be retrieved.
       type_ref (IN) - array of OCIRef * pointing to the particular version of
                  the type descriptor object to obtain.
                  The array must have array_len elements if specified.
       pin_duration (IN) - pin duration (e.g. until the end of current
                  transaction) for the types retreieve.  See 'oro.h' for a
                  description of each option.
       get_option (IN) - options for loading the types. It can be one of two
                   values:
                  OCI_TYPEGET_HEADER for only the header to be loaded, or
                  OCI_TYPEGET_ALL for the TDO and all ADO and MDOs to be
                    loaded.
       tdo (OUT) - output array for the pointers to each pinned type in the
                  object cache. It must have space for array_len pointers.
                  Use OCIObjectGetObjectRef() to obtain the CREF to each
                  pinned type descriptor.
  DESCRIPTION:
       Get pointers to the 
       with the schema/type name array. This is similar to OCITypeByName()
       except that all the TDO's are retreived via a single network roundtrip.
  RETURNS:
        OCI_SUCCESS if the function completes successfully.
        OCI_INVALID_HANDLE if 'env' or 'err' is null. 
        OCI_ERROR if
           1) any of the required parameters is null.
           2) one or more adt types associated with a schema/type name entry
              does not exist.
*/


/*--------------------------------------------------------------------------*/
/*                              TYPE ACCESSORS                              */
/*--------------------------------------------------------------------------*/

/*---------------------------- OCITypeName ---------------------------------*/

/* ** OBSOLETE ** */
oratext* OCITypeName(    OCIEnv *env, OCIError *err, const OCIType *tdo, 
                      ub4 *n_length    );
/*
   NAME: OCITypeName -  ORT Get a Type's naME.
   PARAMETERS:
        env (IN/OUT) - OCI environment handle initialized in object mode
        err (IN/OUT) - error handle. If there is an error, it is
                recorded in 'err' and this function returns OCI_ERROR.
                The error recorded in 'err' can be retrieved by calling
                OCIErrorGet().
        tdo (IN) - pointer to to the type descriptor in the object cache
        n_length (OUT) - length (in bytes) of the returned type name.  The
               caller must allocate space for the ub4 before calling this
               routine.
   REQUIRES:
        1) All type accessors require that the type be pinned before calling
           any accessor.
        2) All input parameters must not be NULL and must be valid.
        3) 'n_length' must point to an allocated ub4.
   DESCRIPTION:
        Get the name of the type.
   RETURNS:
        the name of the type
   NOTES:
        The type descriptor, 'tdo', must be unpinned when the accessed 
        information is no longer needed.
 */

/*------------------------ OCITypeSchema ---------------------------------*/

/* ** OBSOLETE ** */
oratext* OCITypeSchema(    OCIEnv *env, OCIError *err, const OCIType *tdo, 
                   ub4 *n_length    );
/*
   NAME: OCITypeSchema -  ORT Get a Type's SCHema name.
   PARAMETERS:
        env (IN/OUT) - OCI environment handle initialized in object mode
        err (IN/OUT) - error handle. If there is an error, it is
                recorded in 'err' and this function returns OCI_ERROR.
                The error recorded in 'err' can be retrieved by calling
                OCIErrorGet().
        tdo (IN) - pointer to to the type descriptor in the object cache
        n_length (OUT) - length (in bytes) of the returned schema name.  The
               caller must allocate space for the ub4 before calling this
               routine.
   REQUIRES:
        1) All type accessors require that the type be pinned before calling
           any accessor. 
        2) All input parameters must not be NULL and must be valid.
        3) 'n_length' must point to an allocated ub4.
   DESCRIPTION:
        Get the schema name of the type.
   RETURNS:
        the schema name of the type
   NOTES:
        The type descriptor, 'tdo', must be unpinned when the accessed 
        information is no longer needed.
 */

oratext* OCITypePackage(    OCIEnv *env, OCIError *err, const OCIType *tdo, 
                            ub4 *n_length    );
/*
   NAME: OCITypePackage -  ORT Get a Type's Package name.
   PARAMETERS:
        env (IN/OUT) - OCI environment handle initialized in object mode
        err (IN/OUT) - error handle. If there is an error, it is
                recorded in 'err' and this function returns OCI_ERROR.
                The error recorded in 'err' can be retrieved by calling
                OCIErrorGet().
        tdo (IN) - pointer to to the type descriptor in the object cache
        n_length (OUT) - length (in bytes) of the returned type name.  The
               caller must allocate space for the ub4 before calling this
               routine.
   REQUIRES:
        1) All type accessors require that the type be pinned before calling
           any accessor.
        2) All input parameters must not be NULL and must be valid.
        3) 'n_length' must point to an allocated ub4.
        4) The type is a PL/SQL package type
   DESCRIPTION:
        Get the package name of the type.
   RETURNS:
        the package name of the type
   NOTES:
        The type descriptor, 'tdo', must be unpinned when the accessed 
        information is no longer needed.
 */

/*------------------------ OCITypeTypeCode ---------------------------------*/

/* ** OBSOLETE ** */
OCITypeCode OCITypeTypeCode(    OCIEnv *env, OCIError *err,
                                const OCIType *tdo    );
/*
   NAME: OCITypeTypeCode - OCI Get a Type's Type Code.
   PARAMETERS: 
        env (IN/OUT) - OCI environment handle initialized in object mode
        err (IN/OUT) - error handle. If there is an error, it is
                recorded in 'err' and this function returns OCI_ERROR.
                The error recorded in 'err' can be retrieved by calling
                OCIErrorGet().
        tdo (IN) - pointer to to the type descriptor in the object cache
   REQUIRES:
        1) All type accessors require that the type be pinned before calling 
           any accessor. 
        2) All input parameters must not be NULL and must be valid.
   DESCRIPTION:
        Get the type code of the type.
   RETURNS:
        The type code of the type.
   NOTES:
        The type descriptor, 'tdo', must be unpinned when the accessed 
        information is no longer needed.
 */

/*----------------------- OCITypeCollTypeCode -------------------------------*/

/* ** OBSOLETE ** */
OCITypeCode OCITypeCollTypeCode(    OCIEnv *env, OCIError *err,
                                    const OCIType *tdo    );
/*
   NAME: OCITypeCollTypeCode - OCI Get a Domain Type's Type Code.
   PARAMETERS: 
        env (IN/OUT) - OCI environment handle initialized in object mode
        err (IN/OUT) - error handle. If there is an error, it is
                recorded in 'err' and this function returns OCI_ERROR.
                The error recorded in 'err' can be retrieved by calling
                OCIErrorGet().
        tdo (IN) - pointer to to the type descriptor in the object cache
   REQUIRES:
        1) All type accessors require that the type be pinned before calling 
           any accessor. 
        2) All input parameters must not be NULL and must be valid.
        3) 'tdo' MUST point to a named collection type.
   DESCRIPTION:
        Get the type code of the named collection type. For V8.0, named
        collection types can only be variable length arrays and nested tables.
   RETURNS:
        OCI_TYPECODE_VARRAY for variable length array, and
        OCI_TYPECODE_TABLE for nested tables.
   NOTES:
        The type descriptor, 'tdo', should be unpinned when the accessed 
        information is no longer needed.
 */

/*------------------------- OCITypeVersion ---------------------------------*/

/* ** OBSOLETE ** */
oratext* OCITypeVersion(    OCIEnv *env, OCIError *err, const OCIType *tdo, 
                         ub4 *v_length    );
/*
   NAME: OCITypeVersion - OCI Get a Type's user-readable VersioN. 
   PARAMETERS: 
        env (IN/OUT) - OCI environment handle initialized in object mode
        err (IN/OUT) - error handle. If there is an error, it is
                recorded in 'err' and this function returns OCI_ERROR.
                The error recorded in 'err' can be retrieved by calling
                OCIErrorGet().
        tdo (IN) - pointer to to the type descriptor in the object cache
        v_length (OUT) - length (in bytes) of the returned user-readable 
               version.  The caller must allocate space for the ub4 before 
               calling this routine.
   REQUIRES:
        1) All type accessors require that the type be pinned before calling 
           any accessor. 
        2) All input parameters must not be NULL and must be valid.
        3) 'v_length' must point to an allocated ub4.
   DESCRIPTION:
        Get the user-readable version of the type.
   RETURNS:
        The user-readable version of the type
   NOTES:
        The type descriptor, 'tdo', must be unpinned when the accessed 
        information is no longer needed.
 */

/*--------------------------- OCITypeAttrs ---------------------------------*/

/* ** OBSOLETE ** */
ub4 OCITypeAttrs(    OCIEnv *env, OCIError *err, const OCIType *tdo    );
/*
   NAME: OCITypeAttrs - OCI Get a Type's Number of Attributes. 
   PARAMETERS: 
        env (IN/OUT) - OCI environment handle initialized in object mode
        err (IN/OUT) - error handle. If there is an error, it is
                recorded in 'err' and this function returns OCI_ERROR.
                The error recorded in 'err' can be retrieved by calling
                OCIErrorGet().
        tdo (IN) - pointer to to the type descriptor in the object cache
   REQUIRES:
        1) All type accessors require that the type be pinned before calling 
           any accessor. 
        2) All input parameters must not be NULL and must be valid.
   DESCRIPTION:
        Get the number of attributes in the type.
   RETURNS:
        The number of attributes in the type. 0 for ALL non-ADTs.
   NOTES:
        The type descriptor, 'tdo', must be unpinned when the accessed 
        information is no longer needed.
 */

/*------------------------- OCITypeMethods ---------------------------------*/

/* ** OBSOLETE ** */
ub4 OCITypeMethods(    OCIEnv *env, OCIError *err, const OCIType *tdo    );
/*
   NAME: OCITypeMethods - OCI Get a Type's Number of Methods. 
   PARAMETERS: 
        env (IN/OUT) - OCI environment handle initialized in object mode
        err (IN/OUT) - error handle. If there is an error, it is
                recorded in 'err' and this function returns OCI_ERROR.
                The error recorded in 'err' can be retrieved by calling
                OCIErrorGet().
        tdo (IN) - pointer to to the type descriptor in the object cache
   REQUIRES:
        1) All type accessors require that the type be pinned before calling 
           any accessor. 
        2) All input parameters must not be NULL and must be valid.
   DESCRIPTION:
        Get the number of methods in a type.
   RETURNS:
        The number of methods in the type
   NOTES:
        The type descriptor, 'tdo', must be unpinned when the accessed 
        information is no longer needed.
 */


/*--------------------------------------------------------------------------*/
/*                     TYPE ELEMENT INFORMATION ACCESSORS                   */
/*--------------------------------------------------------------------------*/

/*------------------------ OCITypeElemName ---------------------------------*/

/* ** OBSOLETE ** */
oratext* OCITypeElemName(    OCIEnv *env, OCIError *err, 
                           const OCITypeElem *elem, ub4 *n_length    );
/*
   NAME: OCITypeElemName - OCI Get an Attribute's NaMe. 
   PARAMETERS: 
        env (IN/OUT) - OCI environment handle initialized in object mode
        err (IN/OUT) - error handle. If there is an error, it is
                recorded in 'err' and this function returns OCI_ERROR.
                The error recorded in 'err' can be retrieved by calling
                OCIErrorGet().
        elem (IN) - pointer to the type element descriptor in the object cache
        n_length (OUT) - length (in bytes) of the returned attribute name.  
               The caller must allocate space for the ub4 before calling this 
               routine.
   REQUIRES:
        1) All type accessors require that the type be pinned before calling 
           any accessor. 
        2) All input parameters must not be NULL and must be valid.
        3) 'n_length' must point to an allocated ub4.
   DESCRIPTION:
        Get the name of the attribute.
   RETURNS:
        the name of the attribute and the length in n_length
   NOTES:
        The type must be unpinned when the accessed information is no 
        longer needed.
 */

/*------------------------ OCITypeElemTypeCode ------------------------------*/

/* ** OBSOLETE ** */
OCITypeCode OCITypeElemTypeCode(    OCIEnv *env, OCIError *err,
                                    const OCITypeElem *elem    );
/*
   NAME: OCITypeElemTypeCode - OCI Get an Attribute's TypeCode. 
   PARAMETERS: 
        env (IN/OUT) - OCI environment handle initialized in object mode
        err (IN/OUT) - error handle. If there is an error, it is
                recorded in 'err' and this function returns OCI_ERROR.
                The error recorded in 'err' can be retrieved by calling
                OCIErrorGet().
        elem (IN) - pointer to the type element descriptor in the object cache
   REQUIRES:
        1) All type accessors require that the type be pinned before calling 
           any accessor. 
        2) All input parameters must not be NULL and must be valid.
   DESCRIPTION:
        Get the typecode of an attribute's type.
   RETURNS:
        the typecode of the attribute's type.  If this is a scalar type, the 
        typecode sufficiently describes the scalar type and no further calls 
        need to be made.  Valid scalar types include: OCI_TYPECODE_SIGNED8,
        OCI_TYPECODE_UNSIGNED8, OCI_TYPECODE_SIGNED16, OCI_TYPECODE_UNSIGNED16,
        OCI_TYPECODE_SIGNED32, OCI_TYPECODE_UNSIGNED32, OCI_TYPECODE_REAL,
        OCI_TYPECODE_DOUBLE, OCI_TYPECODE_DATE,
        OCI_TYPECODE_MLSLABEL, OROTCOID, OCI_TYPECODE_OCTET, or OROTCLOB.
        This function converts the CREF (stored in the attribute) into a
        typecode.
   NOTES:
       The type must be unpinned when the accessed information is no 
       longer needed.
 */

/*------------------------ OCITypeElemType ---------------------------------*/

/* ** OBSOLETE ** */
sword OCITypeElemType(    OCIEnv *env, OCIError *err, const OCITypeElem *elem, 
                          OCIType **elem_tdo    );
/*
  PARAMETERS
     env (IN/OUT) - OCI environment handle initialized in object mode
     err (IN/OUT) - error handle. If there is an error, it is
             recorded in 'err' and this function returns OCI_ERROR.
             The error recorded in 'err' can be retrieved by calling
             OCIErrorGet().
     elem (IN) - pointer to the type element descriptor in the object cache
     elem_tdo (OUT) - If the function completes successfully, 'elem_tdo'
            points to the type descriptor (in the object cache) of the type of
            the element.

  REQUIRES
     1) All type accessors require that the type be pinned before calling
        any accessor.  This can be done by calling 'OCITypeByName()'.
     2) if 'elem' is not null, it must point to a valid type element descriptor
        in the object cache.

  DESCRIPTION
     Get the type tdo of the type of this element.
  RETURNS
     OCI_SUCCESS if the function completes successfully.
     OCI_INVALID_HANDLE if 'env' or 'err' is null. 
     OCI_ERROR if
         1) any of the parameters is null.

  NOTES
     The type must be unpinned when the accessed information is no
     longer needed.  This can be done by calling 'OCIObjectUnpin()'.
 */

/*------------------------- OCITypeElemFlags -------------------------------*/

/* ** OBSOLETE ** */
ub4 OCITypeElemFlags(    OCIEnv *env, OCIError *err,
                         const OCITypeElem *elem    );
/*
   NAME: OCITypeElemFlags - OCI Get a Elem's FLags
                              (inline, constant, virtual, constructor,
                              destructor). 
   PARAMETERS: 
        env (IN/OUT) - OCI environment handle initialized in object mode
        err (IN/OUT) - error handle. If there is an error, it is
                recorded in 'err' and this function returns OCI_ERROR.
                The error recorded in 'err' can be retrieved by calling
                OCIErrorGet().
        elem (IN) - pointer to the type element descriptor in the object cache
   REQUIRES:
        1) All type accessors require that the type be pinned before calling 
           any accessor. 
        2) All input parameters must not be NULL and must be valid.
   DESCRIPTION:
        Get the flags of a type element (attribute, parameter).
   RETURNS:
        The flags of the type element.
   NOTES:
        The flag bits are not externally documented. Use only the macros
        in the last section (ie. OCI_TYPEPARAM_IS_REQUIRED, and
        OCI_TYPEELEM_IS_REF) to test for them only. The type must be unpinned
        when the accessed information is no longer needed.
 */

/*------------------------ OCITypeElemNumPrec ------------------------------*/

/* ** OBSOLETE ** */
ub1 OCITypeElemNumPrec(    OCIEnv *env, OCIError *err,
                           const OCITypeElem *elem    );
/*
   NAME: OCITypeElemNumPrec - Get a Number's Precision.  This includes float,
                              decimal, real, double, and oracle number.
   PARAMETERS: 
        env (IN/OUT) - OCI environment handle initialized in object mode
        err (IN/OUT) - error handle. If there is an error, it is
                recorded in 'err' and this function returns OCI_ERROR.
                The error recorded in 'err' can be retrieved by calling
                OCIErrorGet().
        elem (IN) - pointer to the type element descriptor in the object cache
   REQUIRES:
        All input parameters must not be NULL and must be valid.
   DESCRIPTION:
        Get the precision of a float, decimal, long, unsigned long, real, 
        double, or Oracle number type. 
   RETURNS:
        the precision of the float, decimal, long, unsigned long, real, double,
        or Oracle number 
 */

/*------------------------- OCITypeElemNumScale -----------------------------*/

/* ** OBSOLETE ** */
sb1 OCITypeElemNumScale(    OCIEnv *env, OCIError *err,
                            const OCITypeElem *elem    );
/*
   NAME: OCITypeElemNumScale - Get a decimal or oracle Number's Scale 
   PARAMETERS: 
        env (IN/OUT) - OCI environment handle initialized in object mode
        err (IN/OUT) - error handle. If there is an error, it is
                recorded in 'err' and this function returns OCI_ERROR.
                The error recorded in 'err' can be retrieved by calling
                OCIErrorGet().
        elem (IN) - pointer to the type element descriptor in the object cache
   REQUIRES:
        All input parameters must not be NULL and must be valid.
   DESCRIPTION:
        Get the scale of a decimal, or Oracle number type. 
   RETURNS:
        the scale of the decimal, or Oracle number 
 */

/*------------------------ OCITypeElemLength -------------------------------*/

/* ** OBSOLETE ** */
ub4 OCITypeElemLength(    OCIEnv *env, OCIError *err,
                          const OCITypeElem *elem    );
/*
   NAME: OCITypeElemLength - Get a raw, fixed or variable length String's
                             length in bytes.
   PARAMETERS: 
        env (IN/OUT) - OCI environment handle initialized in object mode
        err (IN/OUT) - error handle. If there is an error, it is
                recorded in 'err' and this function returns OCI_ERROR.
                The error recorded in 'err' can be retrieved by calling
                OCIErrorGet().
        elem (IN) - pointer to the type element descriptor in the object cache
   REQUIRES:
        All input parameters must not be NULL and must be valid.
   DESCRIPTION:
        Get the length of a raw, fixed or variable length string type. 
   RETURNS:
        length of the raw, fixed or variable length string
 */

/*----------------------- OCITypeElemCharSetID -----------------------------*/

/* ** OBSOLETE ** */
ub2 OCITypeElemCharSetID(    OCIEnv *env, OCIError *err,
                             const OCITypeElem *elem    );
/*
   NAME: OCITypeElemCharSetID - Get a fixed or variable length String's
                                character set ID
   PARAMETERS: 
        env (IN/OUT) - OCI environment handle initialized in object mode
        err (IN/OUT) - error handle. If there is an error, it is
                recorded in 'err' and this function returns OCI_ERROR.
                The error recorded in 'err' can be retrieved by calling
                OCIErrorGet().
        elem (IN) - pointer to the type element descriptor in the object cache
   REQUIRES:
        All input parameters must not be NULL and must be valid.
   DESCRIPTION:
        Get the character set ID of a fixed or variable length string type. 
   RETURNS:
        character set ID of the fixed or variable length string
 */

/*---------------------- OCITypeElemCharSetForm ----------------------------*/

/* ** OBSOLETE ** */
ub2 OCITypeElemCharSetForm(    OCIEnv *env, OCIError *err,
                               const OCITypeElem *elem    );
/*
   NAME: OCITypeElemCharSetForm - Get a fixed or variable length String's
                                  character set specification form.
   PARAMETERS: 
        env (IN/OUT) - OCI environment handle initialized in object mode
        err (IN/OUT) - error handle. If there is an error, it is
                recorded in 'err' and this function returns OCI_ERROR.
                The error recorded in 'err' can be retrieved by calling
                OCIErrorGet().
        elem (IN) - pointer to the attribute information in the object cache
   REQUIRES:
        All input parameters must not be NULL and must be valid.
   DESCRIPTION:
        Get the character form of a fixed or variable length string type.
        The character form is an enumerated value that can be one of the
        4 values below:
               SQLCS_IMPLICIT for CHAR, VARCHAR2, CLOB w/o a specified set
               SQLCS_NCHAR    for NCHAR, NCHAR VARYING, NCLOB
               SQLCS_EXPLICIT for CHAR, etc, with "CHARACTER SET ..." syntax
               SQLCS_FLEXIBLE for PL/SQL "flexible" parameters
   RETURNS:
        character form of the fixed or variable string
 */

/*--------------------- OCITypeElemParameterizedType ------------------------*/

/* ** OBSOLETE ** */
sword OCITypeElemParameterizedType(    OCIEnv *env, OCIError *err,
                                       const OCITypeElem *elem, 
                                       OCIType **type_stored    );
/*
   NAME: OCITypeElemParameterizedType 
   PARAMETERS: 
        env (IN/OUT) - OCI environment handle initialized in object mode
        err (IN/OUT) - error handle. If there is an error, it is
                recorded in 'err' and this function returns OCI_ERROR.
                The error recorded in 'err' can be retrieved by calling
                OCIErrorGet().
        elem (IN) - pointer to the type element descriptor in the object cache
        type_stored (OUT) - If the function completes successfully, 
               and the parameterized type is complex, 'type_stored' is NULL.
               Otherwise, 'type_stored' points to the type descriptor (in the 
               object cache) of the type that is stored in the parameterized 
               type.  The caller must allocate space for the OCIType* 
               before calling this routine and must not write into the space.
   REQUIRES:
        All input parameters must be valid.
   DESCRIPTION:
        Get a descriptor to the parameter type of a parameterized type.
        Parameterized types are types of the form:
          REF T
          VARRAY (n) OF T
        etc, where T is the parameter in the parameterized type.
        Additionally is_ref is set if the parameter is a PTR or REF.
        For example, it is set for REF T or VARRAY(n) OF REF T.
   RETURNS:
        OCI_SUCCESS if the function completes successfully.
        OCI_INVALID_HANDLE if 'env' or 'err' is null. 
        OCI_ERROR if
            1) any of the parameters is null.
            2) 'type_stored' is not NULL but points to NULL data.
   NOTES:
        Complex parameterized types will be in a future release (once 
        typedefs are supported.  When setting the parameterized type 
        information, the user must typedef the contents if it's a
        complex parameterized type.  Ex. for varray<varray<car>>, use
        'typedef varray<car> varcar' and then use varray<varcar>. 
 */

/*----------------------- OCITypeElemExtTypeCode ----------------------------*/
 
/* ** OBSOLETE ** */
OCITypeCode OCITypeElemExtTypeCode(    OCIEnv *env, OCIError *err,
                                       const OCITypeElem *elem    );
/*
   NAME: OCITypeElemExtTypeCode - OCI Get an element's SQLT constant.
   PARAMETERS:
        env (IN/OUT) - OCI environment handle initialized in object mode
        err (IN/OUT) - error handle. If there is an error, it is
                recorded in 'err' and this function returns OCI_ERROR.
                The error recorded in 'err' can be retrieved by calling
                OCIErrorGet().
        elem (IN) - pointer to the type element descriptor in the object cache
   REQUIRES:
        1) All type accessors require that the type be pinned before calling
           any accessor.
        2) All input parameters must not be NULL and must be valid.
   DESCRIPTION:
        Get the internal Oracle typecode associated with an attribute's type.
        This is the actual typecode for the attribute when it gets mapped
        to a column in the Oracle database.
   RETURNS:
        The Oracle typecode associated with the attribute's type.
   NOTES:
        The type must be unpinned when the accessed information is no
        longer needed.
 */

/*--------------------------------------------------------------------------*/
/*                           ATTRIBUTE ACCESSORS                            */
/*--------------------------------------------------------------------------*/

/*------------------------ OCITypeAttrByName -------------------------------*/

/* ** OBSOLETE ** */
sword OCITypeAttrByName(    OCIEnv *env, OCIError *err, const OCIType *tdo, 
                            const oratext *name, ub4 n_length,
                            OCITypeElem **elem    );
/*
   NAME: OCITypeAttrByName - OCI Get an Attribute By Name. 
   PARAMETERS: 
        env (IN/OUT) - OCI environment handle initialized in object mode
        err (IN/OUT) - error handle. If there is an error, it is
                recorded in 'err' and this function returns OCI_ERROR.
                The error recorded in 'err' can be retrieved by calling
                OCIErrorGet().
        tdo (IN) - pointer to to the type descriptor in the object cache
        name (IN) - the attribute's name 
        n_length (IN) - length (in bytes) of the 'name' parameter
        elem (OUT) - If this function completes successfully, 'elem' points to 
               the selected type element descriptor pertaining to the
               attributein the object cache.
   REQUIRES:
        1) All type accessors require that the type be pinned before calling 
           any accessor. 
        2) if 'tdo' is not null, it must point to a valid type descriptor 
           in the object cache.
   DESCRIPTION:
        Get an attribute given its name.  
   RETURNS:
        OCI_SUCCESS if the function completes successfully.
        OCI_INVALID_HANDLE if 'env' or 'err' is null. 
        OCI_ERROR if
            1) any of the required parameters is null.
            2) the type does not contain an attribute with the input 'name'. 
            3) 'name' is NULL.
   NOTES:
        The type descriptor, 'tdo', must be unpinned when the accessed 
        information is no longer needed.
        Schema and type names are CASE-SENSITIVE. If they have been created
        via SQL, you need to use uppercase names.
 */

/*------------------------ OCITypeAttrNext ---------------------------------*/

/* ** OBSOLETE ** */
sword OCITypeAttrNext(    OCIEnv *env, OCIError *err, 
                        OCITypeIter *iterator_ort, OCITypeElem **elem    );

/*
   NAME: OCITypeAttrNext - OCI Get an Attribute By Iteration.
   PARAMETERS: 
        env (IN/OUT) - OCI environment handle initialized in object mode
        err (IN/OUT) - error handle. If there is an error, it is
                recorded in 'err' and this function returns OCI_ERROR.
                The error recorded in 'err' can be retrieved by calling
                OCIErrorGet().
        iterator_ort (IN/OUT) - iterator for retrieving the next attribute;
               see OCITypeIterNew() to initialize iterator.
        elem (OUT) - If this function completes successfully, 'elem' points to 
               the selected type element descriptor pertaining to the
               attributein the object cache.
   REQUIRES:
        1) All type accessors require that the type be pinned before calling 
            any accessor. 
        2) if 'tdo' is not null, it must point to a valid type descriptor 
           in the object cache.
   DESCRIPTION:
        Iterate to the next attribute to retrieve.
   RETURNS:
        OCI_SUCCESS if the function completes successfully.
        OCI_NO_DATA if there are no more attributes to iterate on; use
            OCITypeIterSet() to reset the iterator if necessary.
        OCI_INVALID_HANDLE if 'env' or 'err' is null. 
        OCI_ERROR if
            1) any of the required parameters is null.
   NOTES:
        The type must be unpinned when the accessed information is no 
        longer needed.
 */

/*--------------------------------------------------------------------------*/
/*                           COLLECTION ACCESSORS                           */
/*--------------------------------------------------------------------------*/

/*------------------------ OCITypeCollElem ---------------------------------*/

/* ** OBSOLETE ** */
sword OCITypeCollElem(    OCIEnv *env, OCIError *err, const OCIType *tdo,
                          OCITypeElem **element    );
/*
   NAME: OCITypeCollElem
   PARAMETERS: 
        env (IN/OUT) - OCI environment handle initialized in object mode
        err (IN/OUT) - error handle. If there is an error, it is
                recorded in 'err' and this function returns OCI_ERROR.
                The error recorded in 'err' can be retrieved by calling
                OCIErrorGet().
        tdo (IN) - pointer to the type descriptor in the object cache
        element (IN/OUT) - If the function completes successfully, this
               points to the descriptor for the collection's element.
               It is stored in the same format as an ADT attribute's
               descriptor.
               If *element is NULL, OCITypeCollElem() implicitly allocates a
               new instance of OCITypeElem in the object cache. This instance
               will be
               automatically freed at the end of the session, and does not have
               to be freed explicitly.
               If *element is not NULL, OCITypeCollElem() assumes that it
               points to a valid OCITypeElem descriptor and will copy the
               results into it.
   REQUIRES:
        All input parameters must be valid.
   DESCRIPTION:
        Get a pointer to the descriptor (OCITypeElem) of the element of an
        array or the rowtype of a nested table.
   RETURNS:
        OCI_SUCCESS if the function completes successfully.
        OCI_INVALID_HANDLE if 'env' or 'err' is null. 
        OCI_ERROR if
            1) any of the parameters is null.
            2) the type TDO does not point to a valid collection's type.
   NOTES:
        Complex parameterized types will be in a future release (once 
        typedefs are supported.  When setting the parameterized type 
        information, the user must typedef the contents if it's a
        complex parameterized type.  Ex. for varray<varray<car>>, use
        'typedef varray<car> varcar' and then use varray<varcar>. 
 */

/*------------------------ OCITypeCollSize ---------------------------------*/

/* ** OBSOLETE ** */
sword OCITypeCollSize(    OCIEnv *env, OCIError *err, const OCIType *tdo, 
                          ub4 *num_elems    );
/*
   NAME: OCITypeCollSize - OCI Get a Collection's Number of Elements.
   PARAMETERS: 
        env (IN/OUT) - OCI environment handle initialized in object mode
        err (IN/OUT) - error handle. If there is an error, it is
                recorded in 'err' and this function returns OCI_ERROR.
                The error recorded in 'err' can be retrieved by calling
                OCIErrorGet().
        tdo (IN) - pointer to the type descriptor in the object cache
        num_elems (OUT) - number of elements in collection
   REQUIRES:
        All input parameters must be valid. tdo points to an array type
        defined as a domain.
   DESCRIPTION:
        Get the number of elements stored in a fixed array or the maximum 
        number of elements in a variable array.
   RETURNS:
        OCI_SUCCESS if the function completes successfully.
        OCI_INVALID_HANDLE if 'env' or 'err' is null. 
        OCI_ERROR if
            1) any of the parameters is null.
            2) 'tdo' does not point to a domain with a collection type.
   NOTES:
        Complex parameterized types will be in a future release (once 
        typedefs are supported.  When setting the parameterized type 
        information, the user must typedef the contents if it's a
        complex parameterized type.  Ex. for varray<varray<car>>, use
        'typedef varray<car> varcar' and then use varray<varcar>. 
 */

/*------------------------ OCITypeCollExtTypeCode ---------------------------*/
 
/* ** OBSOLETE ** */
sword OCITypeCollExtTypeCode(    OCIEnv *env, OCIError *err,
                               const OCIType *tdo, OCITypeCode *sqt_code    );
/*
   NAME: ortcsqt - OCI Get a Collection element's DTY constant.
   PARAMETERS:
        env (IN/OUT) - OCI environment handle initialized in object mode
        err (IN/OUT) - error handle. If there is an error, it is
                recorded in 'err' and this function returns OCI_ERROR.
                The error recorded in 'err' can be retrieved by calling
                OCIErrorGet().
        tdo (IN) - pointer to the type descriptor in the object cache
        sqt_code (OUT) - SQLT code of type element.
   REQUIRES:
        1) All type accessors require that the type be pinned before calling
           any accessor.
        2) All input parameters must not be NULL and must be valid.
   DESCRIPTION:
        Get the SQLT constant associated with an domain's element type.
        The SQLT codes are defined in <sqldef.h> and are needed for OCI/OOCI
        use.
   RETURNS:
        OCI_SUCCESS if the function completes successfully.
        OCI_INVALID_HANDLE if 'env' or 'err' is null.
        OCI_ERROR if
            1) any of the parameters is null.
            2) 'tdo' does not point to a domain with a collection type.
   NOTES:
        The type must be unpinned when the accessed information is no
        longer needed.
 */
 

/*--------------------------------------------------------------------------*/
/*                             METHOD ACCESSORS                             */
/*--------------------------------------------------------------------------*/

/*------------------------- OCITypeMethodOverload --------------------------*/

/* ** OBSOLETE ** */
ub4 OCITypeMethodOverload(    OCIEnv *env, OCIError *err, const OCIType *tdo, 
                              const oratext *method_name, ub4 m_length    );
/*
   NAME: OCITypeMethodOverload - OCI Get type's Number of Overloaded names
                                 for the given method name.
   PARAMETERS:
        gp (IN/OUT) - pga environment handle.  Any errors are recorded here.
        tdo (IN) - pointer to to the type descriptor in the object cache
        method_name (IN) - the method's name
        m_length (IN) - length (in bytes) of the 'method_name' parameter
   REQUIRES:
        1) All type accessors require that the type be pinned before calling
           any accessor.
        2) if 'tdo' is not null, it must point to a valid type descriptor
           in the object cache.
   DESCRIPTION:
        Overloading of methods implies that more than one method may have the
        same method name.  This routine returns the number of methods that
        have the given method name.  If there are no methods with the input
        method name, 'num_methods' is 0.  The caller uses this information when
        allocating space for the array of mdo and/or position pointers before
        calling 'OCITypeMethodByName()' or 'ortgmps()'.
   RETURNS:
        The number of methods with the given name. 0 if none contains the
        name.
   NOTES:
        Schema and type names are CASE-SENSITIVE. If they have been created
        via SQL, you need to use uppercase names.
 */

/*------------------------ OCITypeMethodByName ------------------------------*/

/* ** OBSOLETE ** */
sword OCITypeMethodByName(    OCIEnv *env, OCIError *err, const OCIType *tdo, 
                              const oratext *method_name, ub4 m_length,
                              OCITypeMethod **mdos    );
/*
   NAME: OCITypeMethodByName - OCI Get one or more Methods with Name. 
   PARAMETERS: 
        env (IN/OUT) - OCI environment handle initialized in object mode
        err (IN/OUT) - error handle. If there is an error, it is
                recorded in 'err' and this function returns OCI_ERROR.
                The error recorded in 'err' can be retrieved by calling
                OCIErrorGet().
        tdo (IN) - pointer to to the type descriptor in the object cache
        method_name (IN) - the methods' name  
        m_length (IN) - length (in bytes) of the 'name' parameter
        mdos (OUT) - If this function completes successfully, 'mdos' points to
                the selected methods in the object cache.  The caller must
                allocate space for the array of OCITypeMethod pointers before
                calling this routine and must not write into the space.
                The number of OCITypeMethod pointers that will be returned can
                be obtained by calling 'OCITypeMethodOverload()'.
   REQUIRES:
        1) All type accessors require that the type be pinned before calling 
           any accessor. 
        2) if 'tdo' is not null, it must point to a valid type descriptor 
           in the object cache.
   DESCRIPTION:
        Get one or more methods given the name.  
   RETURNS:
        OCI_SUCCESS if the function completes successfully.
        OCI_INVALID_HANDLE if 'env' or 'err' is null. 
        OCI_ERROR if
            1) any of the required parameters is null.
            2) No methods in type has name 'name'.
            3) 'mdos' is not NULL but points to NULL data.
   NOTES:
        The type must be unpinned when the accessed information is no 
        longer needed.
        Schema and type names are CASE-SENSITIVE. If they have been created
        via SQL, you need to use uppercase names.
 */

/*------------------------ OCITypeMethodNext --------------------------------*/

/* ** OBSOLETE ** */
sword OCITypeMethodNext(    OCIEnv *env, OCIError *err, 
                            OCITypeIter *iterator_ort,
                            OCITypeMethod **mdo   );

/*
   NAME: OCITypeMethodNext - OCI Get a Method By Iteration.
   PARAMETERS: 
        env (IN/OUT) - OCI environment handle initialized in object mode
        err (IN/OUT) - error handle. If there is an error, it is
                recorded in 'err' and this function returns OCI_ERROR.
                The error recorded in 'err' can be retrieved by calling
                OCIErrorGet().
        iterator_ort (IN/OUT) - iterator for retrieving the next method;
               see OCITypeIterNew() to set iterator.
        mdo (OUT) - If this function completes successfully, 'mdo' points to 
               the selected method descriptor in the object cache.  Positions 
               start at 1.  The caller must allocate space for the
               OCITypeMethod* before calling this routine and must not write 
               nto the space. 
   REQUIRES:
         1) All type accessors require that the type be pinned before calling 
            any accessor. 
        2) if 'tdo' is not null, it must point to a valid type descriptor 
           in the object cache.
   DESCRIPTION:
        Iterate to the next method to retrieve.
   RETURNS:
        OCI_SUCCESS if the function completes successfully.
        OCI_NO_DATA if there are no more attributes to iterate on; use
            OCITypeIterSet() to reset the iterator if necessary.
        OCI_INVALID_HANDLE if 'env' or 'err' is null. 
        OCI_ERROR if
            1) any of the required parameters is null.
            2) 'mdo' is not NULL but points to NULL data.
   NOTES:
        The type must be unpinned when the accessed information is no 
        longer needed.
 */

/*------------------------ OCITypeMethodName --------------------------------*/

/* ** OBSOLETE ** */
oratext *OCITypeMethodName(    OCIEnv *env, OCIError *err,
                            const OCITypeMethod *mdo, ub4 *n_length    );
/*
   NAME: OCITypeMethodName - OCI Get a Method's NaMe. 
   PARAMETERS: 
        env (IN/OUT) - OCI environment handle initialized in object mode
        err (IN/OUT) - error handle. If there is an error, it is
                recorded in 'err' and this function returns OCI_ERROR.
                The error recorded in 'err' can be retrieved by calling
                OCIErrorGet().
        mdo (IN) - pointer to the method descriptor in the object cache
        n_length (OUT) - length (in bytes) of the 'name' parameter.  The caller
               must allocate space for the ub4 before calling this routine.
   REQUIRES:
        1) All type accessors require that the type be pinned before calling 
           any accessor. 
        2) All input parameters must not be NULL and must be valid.
   DESCRIPTION:
        Get the (non-unique) real name of the method.
   RETURNS:
        the non-unique name of the method or NULL if there is an error.
   NOTES:
        The type must be unpinned when the accessed information is no 
        longer needed.
 */

/*------------------------ OCITypeMethodEncap -------------------------------*/

/* ** OBSOLETE ** */
OCITypeEncap OCITypeMethodEncap(    OCIEnv *env, OCIError *err,
                                    const OCITypeMethod *mdo    );
/*
   NAME: OCITypeMethodEncap - Get a Method's ENcapsulation (private/public). 
   PARAMETERS: 
        env (IN/OUT) - OCI environment handle initialized in object mode
        err (IN/OUT) - error handle. If there is an error, it is
                recorded in 'err' and this function returns OCI_ERROR.
                The error recorded in 'err' can be retrieved by calling
                OCIErrorGet().
        mdo (IN) - pointer to the method descriptor in the object cache
   REQUIRES:
        1) All type accessors require that the type be pinned before calling 
           any accessor. 
        2) All input parameters must not be NULL and must be valid.
   DESCRIPTION:
        Get the encapsulation (private, or public) of a method.
   RETURNS:
        the encapsulation (private, or public) of the method
   NOTES:
        The type must be unpinned when the accessed information is no 
        longer needed.
 */

/*------------------------ OCITypeMethodFlags -------------------------------*/

/* ** OBSOLETE ** */
OCITypeMethodFlag OCITypeMethodFlags(    OCIEnv *env, OCIError *err,
                                         const OCITypeMethod *mdo    );
/*
   NAME: OCITypeMethodFlags - OCI Get a Method's FLags
                              (inline, constant, virtual, constructor,
                              destructor). 
   PARAMETERS: 
        env (IN/OUT) - OCI environment handle initialized in object mode
        err (IN/OUT) - error handle. If there is an error, it is
                recorded in 'err' and this function returns OCI_ERROR.
                The error recorded in 'err' can be retrieved by calling
                OCIErrorGet().
        mdo (IN) - pointer to the method descriptor in the object cache
   REQUIRES:
        1) All type accessors require that the type be pinned before calling 
           any accessor. 
        2) All input parameters must not be NULL and must be valid.
   DESCRIPTION:
        Get the flags (inline, constant, virutal, constructor, destructor) of
        a method.
   RETURNS:
        the flags (inline, constant, virutal, constructor, destructor) of
        the method
   NOTES:
        The type must be unpinned when the accessed information is no 
        longer needed.
 */

/*------------------------ OCITypeMethodMap ---------------------------------*/

/* ** OBSOLETE ** */
sword OCITypeMethodMap(    OCIEnv *env, OCIError *err, const OCIType *tdo, 
                           OCITypeMethod **mdo    );
/*
   NAME: OCITypeMethodMap - OCI Get the Method's MAP function.
   PARAMETERS: 
        env (IN/OUT) - OCI environment handle initialized in object mode
        err (IN/OUT) - error handle. If there is an error, it is
                recorded in 'err' and this function returns OCI_ERROR.
                The error recorded in 'err' can be retrieved by calling
                OCIErrorGet().
        tdo (IN) - pointer to to the type descriptor in the object cache
        mdo (OUT) - If this function completes successfully, and there is a 
               map function for this type, 'mdo' points to the selected method 
               descriptor in the object cache.  Otherwise, 'mdo' is null.
   REQUIRES:
        1) All type accessors require that the type be pinned before calling 
           any accessor. 
        2) All required input parameters must not be NULL and must be valid.
   DESCRIPTION:
        A type may have only one map function.  'OCITypeMethodMap()' finds
        this function, if it exists, and returns a reference and a pointer to
        the method descriptor in the object cache.  If the type does not have a
        map (relative ordering) function, then 'mdo_ref' and 'mdo' are set
        to null and an error is returned.
   RETURNS:
        OCI_SUCCESS if the function completes successfully.
        OCI_INVALID_HANDLE if 'env' or 'err' is null. 
        OCI_ERROR if
            the type does not contain a map function.
   NOTES:
        The type must be unpinned when the accessed information is no 
        longer needed.
 */

/*------------------------ OCITypeMethodOrder -------------------------------*/

/* ** OBSOLETE ** */
sword OCITypeMethodOrder(    OCIEnv *env, OCIError *err, const OCIType *tdo, 
                             OCITypeMethod **mdo    );
/*
   NAME: OCITypeMethodOrder - OCI Get the Method's ORder function.
   PARAMETERS: 
        env (IN/OUT) - OCI environment handle initialized in object mode
        err (IN/OUT) - error handle. If there is an error, it is
                recorded in 'err' and this function returns OCI_ERROR.
                The error recorded in 'err' can be retrieved by calling
                OCIErrorGet().
        tdo (IN) - pointer to to the type descriptor in the object cache
        mdo (OUT) - If this function completes successfully, and there is a 
               map function for this type, 'mdo' points to the selected method 
               descriptor in the object cache.  Otherwise, 'mdo' is null.
   REQUIRES:
        1) All type accessors require that the type be pinned before calling 
           any accessor. 
        2) All required input parameters must not be NULL and must be valid.
   DESCRIPTION:
        A type may have only one ORder or MAP function. 'OCITypeMethodOrder()'
        finds this function, if it exists, and returns a ref and a pointer
        to the method descriptor in the object cache.  If the type does not
        have a map (relative ordering) function, then 'mdo_ref' and 'mdo' are
        set to null and an error is returned.
   RETURNS:
        OCI_SUCCESS if the function completes successfully.
        OCI_INVALID_HANDLE if 'env' or 'err' is null. 
        OCI_ERROR if
            the type does not contain a map function.
   NOTES:
        The type must be unpinned when the accessed information is no 
        longer needed.
 */

/*------------------------ OCITypeMethodParams ------------------------------*/

/* ** OBSOLETE ** */
ub4 OCITypeMethodParams(    OCIEnv *env, OCIError *err,
                            const OCITypeMethod *mdo    );
/*
   NAME: OCITypeMethodParams - OCI Get a Method's Number of Parameters. 
   PARAMETERS: 
        env (IN/OUT) - OCI environment handle initialized in object mode
        err (IN/OUT) - error handle. If there is an error, it is
                recorded in 'err' and this function returns OCI_ERROR.
                The error recorded in 'err' can be retrieved by calling
                OCIErrorGet().
        mdo (IN) - pointer to the method descriptor in the object cache
   REQUIRES:
        1) All type accessors require that the type be pinned before calling 
           any accessor. 
        2) All input parameters must not be NULL and must be valid.
   DESCRIPTION:
        Get the number of parameters in a method.
   RETURNS:
        the number of parameters in the method
   NOTES:
        The type must be unpinned when the accessed information is no 
        longer needed.
 */


/*--------------------------------------------------------------------------*/
/*                             RESULT ACCESSORS                             */
/*--------------------------------------------------------------------------*/

/*-------------------------- OCITypeResult ---------------------------------*/

/* ** OBSOLETE ** */
sword OCITypeResult(    OCIEnv *env, OCIError *err, const OCITypeMethod *mdo,
                        OCITypeElem **elem    );
/*
   NAME: OCITypeResult - OCI Get a method's result type descriptor.
   PARAMETERS: 
        env (IN/OUT) - OCI environment handle initialized in object mode
        err (IN/OUT) - error handle. If there is an error, it is
                recorded in 'err' and this function returns OCI_ERROR.
                The error recorded in 'err' can be retrieved by calling
                OCIErrorGet().
        mdo (IN) - pointer to the method descriptor in the object cache
        elem (OUT) - If this function completes successfully, 'rdo' points to 
               the selected result (parameter) descriptor in the object cache.
   REQUIRES:
        1) All type accessors require that the type be pinned before calling 
           any accessor.
        2) 'elem' MUST be the address of an OCITypeElem pointer.
   DESCRIPTION:
        Get the result of a method.
   RETURNS:
        OCI_SUCCESS if the function completes successfully.
        OCI_INVALID_HANDLE if 'env' or 'err' is null. 
        OCI_ERROR if
            1) any of the required parameters is null.
            2) method returns no results.
   NOTES:
        The method must be unpinned when the accessed information is no 
        longer needed.
 */


/*--------------------------------------------------------------------------*/
/*                           PARAMETER ACCESSORS                            */
/*--------------------------------------------------------------------------*/

/*------------------------ OCITypeParamByPos -------------------------------*/

/* ** OBSOLETE ** */
sword OCITypeParamByPos(    OCIEnv *env, OCIError *err,
                            const OCITypeMethod *mdo, ub4 position, 
                            OCITypeElem **elem    );
/*
   NAME: OCITypeParamByPos - OCI Get a Parameter in a method By Position. 
   PARAMETERS: 
        env (IN/OUT) - OCI environment handle initialized in object mode
        err (IN/OUT) - error handle. If there is an error, it is
                recorded in 'err' and this function returns OCI_ERROR.
                The error recorded in 'err' can be retrieved by calling
                OCIErrorGet().
        mdo (IN) - pointer to the method descriptor in the object cache
        position (IN) - the parameter's position.  Positions start at 1.
        elem (OUT) - If this function completes successfully, 'elem' points to 
               the selected parameter descriptor in the object cache.
   REQUIRES:
        1) All type accessors require that the type be pinned before calling 
           any accessor. 
   DESCRIPTION:
        Get a parameter given its position in the method.  Positions start 
        at 1.  
   RETURNS:
        OCI_SUCCESS if the function completes successfully.
        OCI_INVALID_HANDLE if 'env' or 'err' is null. 
        OCI_ERROR if
            1) any of the required parameters is null.
            2) 'position' is not >= 1 and <= the number of parameters in the
               method.
   NOTES:
        The type must be unpinned when the accessed information is no 
        longer needed.
 */

/*------------------------ OCITypeParamByName -------------------------------*/

/* ** OBSOLETE ** */
sword OCITypeParamByName(    OCIEnv *env, OCIError *err,
                             const OCITypeMethod *mdo, 
                             const oratext *name, ub4 n_length,
                             OCITypeElem **elem    );
/*
   NAME: OCITypeParamByName - OCI Get a Parameter in a method By Name. 
   PARAMETERS: 
        env (IN/OUT) - OCI environment handle initialized in object mode
        err (IN/OUT) - error handle. If there is an error, it is
                recorded in 'err' and this function returns OCI_ERROR.
                The error recorded in 'err' can be retrieved by calling
                OCIErrorGet().
        mdo (IN) - pointer to the method descriptor in the object cache
        name (IN) - the parameter's name
        n_length (IN) - length (in bytes) of the 'name' parameter
        elem (OUT) - If this function completes successfully, 'elem' points to 
               the selected parameter descriptor in the object cache.
   REQUIRES:
        1) All type accessors require that the type be pinned before calling 
           any accessor. 
        2) if 'mdo' is not null, it must point to a valid method descriptor 
           in the object cache.
   DESCRIPTION:
        Get a parameter given its name.  
   RETURNS:
        OCI_SUCCESS if the function completes successfully.
        OCI_INVALID_HANDLE if 'env' or 'err' is null. 
        OCI_ERROR if
            1) any of the required parameters is null.
            2) the method does not contain a parameter with the input 'name'. 
   NOTES:
        The type must be unpinned when the accessed information is no 
        longer needed.
 */

/*------------------------ OCITypeParamPos ---------------------------------*/

/* ** OBSOLETE ** */
sword OCITypeParamPos(    OCIEnv *env, OCIError *err,
                          const OCITypeMethod *mdo, 
                          const oratext *name, ub4 n_length, ub4 *position, 
                          OCITypeElem **elem    );
/*
   NAME: OCITypeParamPos - OCI Get a parameter's position in a method
   PARAMETERS: 
        env (IN/OUT) - OCI environment handle initialized in object mode
        err (IN/OUT) - error handle. If there is an error, it is
                recorded in 'err' and this function returns OCI_ERROR.
                The error recorded in 'err' can be retrieved by calling
                OCIErrorGet().
        mdo (IN) - pointer to the method descriptor in the object cache
        name (IN) - the parameter's name
        n_length (IN) - length (in bytes) of the 'name' parameter
        position (OUT) - If this function completes successfully, 'position' 
               points to the position of the parameter in the method starting 
               at position 1. position MUST point to space for a ub4.
        elem (OUT) - If this function completes successfully, and 
               the input 'elem' is not NULL, 'elem' points to the selected 
               parameter descriptor in the object cache.
   REQUIRES:
        1) All type accessors require that the type be pinned before calling 
           any accessor. 
        2) if 'mdo' is not null, it must point to a valid method descriptor 
           in the object cache.
   DESCRIPTION:
        Get the position of a parameter in a method.  Positions start at 1.
   RETURNS:
        OCI_SUCCESS if the function completes successfully.
        OCI_INVALID_HANDLE if 'env' or 'err' is null. 
        OCI_ERROR if
            1) any of the parameters is null.
            2) the method does not contain a parameter with the input 'name'. 
   NOTES:
        The type must be unpinned when the accessed information is no 
        longer needed.
 */

/*------------------------ OCITypeParamElemMode -----------------------------*/

/* ** OBSOLETE ** */
OCITypeParamMode OCITypeElemParamMode(    OCIEnv *env, OCIError *err,
                                          const OCITypeElem *elem    );
/*
   NAME: OCITypeElemParamMode - OCI Get a parameter's mode
   PARAMETERS: 
        env (IN/OUT) - OCI environment handle initialized in object mode
        err (IN/OUT) - error handle. If there is an error, it is
                recorded in 'err' and this function returns OCI_ERROR.
                The error recorded in 'err' can be retrieved by calling
                OCIErrorGet().
        elem (IN) - pointer to the parameter descriptor in the object cache
                (represented by an OCITypeElem)
   REQUIRES:
        1) All type accessors require that the type be pinned before calling 
           any accessor. 
        2) All input parameters must not be NULL and must be valid.
   DESCRIPTION:
        Get the mode (in, out, or in/out) of the parameter.
   RETURNS:
        the mode (in, out, or in/out) of the parameter
   NOTES:
        The type must be unpinned when the accessed information is no 
        longer needed.
 */

/*------------------------- OCITypeElemDefaultValue -------------------------*/

/* ** OBSOLETE ** */
oratext* OCITypeElemDefaultValue(    OCIEnv *env, OCIError *err,
                                  const OCITypeElem *elem,
                                  ub4 *d_v_length    );
/*
   NAME: OCITypeElemDefaultValue - OCI Get the element's Default Value. 
   PARAMETERS: 
        env (IN/OUT) - OCI environment handle initialized in object mode
        err (IN/OUT) - error handle. If there is an error, it is
                recorded in 'err' and this function returns OCI_ERROR.
                The error recorded in 'err' can be retrieved by calling
                OCIErrorGet().
        elem (IN) - pointer to the parameter descriptor in the object cache
                (represented by an OCITypeElem)
        d_v_length (OUT) - length (in bytes) of the returned default value.
               The caller must allocate space for the ub4 before calling this 
               routine.
   REQUIRES:
        1) All type accessors require that the type be pinned before calling 
           any accessor. 
        2) All input parameters must not be NULL and must be valid.
   DESCRIPTION:
        Get the default value in text form (PL/SQL) of an element. For V8.0,
        this only makes sense for a method parameter.
   RETURNS:
        The default value (text) of the parameter.
   NOTES:
        The type must be unpinned when the accessed information is no 
        longer needed.
 */
 

/*--------------------------------------------------------------------------*/
/*                           TYPE VERSION TABLE                             */
/*--------------------------------------------------------------------------*/

/* For V8.0, the type version table is meant to be an internal data structure
   only for Oracle clients for type version maintanence purposes. A more
   general version of the API may be made public in subsequent releases. */


/*--------------------------- OCITypeVTInit --------------------------------*/
 
/* ** OBSOLETE ** */
sword OCITypeVTInit(    OCIEnv *env, OCIError *err    );
/*
   NAME: OCITypeVTInit - OCI type Version table INItialize
   PARAMETERS:
        env (IN/OUT) - OCI environment handle initialized in object mode
        err (IN/OUT) - error handle. If there is an error, it is
                recorded in 'err' and this function returns OCI_ERROR.
                The error recorded in 'err' can be retrieved by calling
                OCIErrorGet().
   REQUIRES:
        none
   DESCRIPTION:
        Allocate space for and initialize the type version table and the type
        version table's index.
   RETURNS:
        OCI_SUCCESS if the function completes successfully.
        OCI_INVALID_HANDLE if 'env' or 'err' is null.
        OCI_ERROR if internal errors occurrs during initialization.
 */
 
/*--------------------------- OCITypeVTInsert -------------------------------*/
 
/* ** OBSOLETE ** */
sword OCITypeVTInsert(    OCIEnv *env, OCIError *err, 
                          const oratext *schema_name, ub4 s_n_length, 
                          const oratext *type_name, ub4 t_n_length, 
                          const oratext *user_version, ub4 u_v_length    );
/*
   NAME: OCITypeVTInsert - OCI type Version table INSert entry.
   PARAMETERS:
        env (IN/OUT) - OCI environment handle initialized in object mode
        err (IN/OUT) - error handle. If there is an error, it is
                recorded in 'err' and this function returns OCI_ERROR.
                The error recorded in 'err' can be retrieved by calling
                OCIErrorGet().
        schema_name (IN, optional) - name of schema associated with the 
                  type.  By default, the user's schema name is used.
        s_n_length (IN) - length of the 'schema_name' parameter
        type_name (IN) - type name to insert
        t_n_length (IN) - length (in bytes) of the 'type_name' parameter
        user_version (IN) - user readable version of the type
        u_v_length (IN) - length (in bytes) of the 'user_version' parameter
   REQUIRES:
        none
   DESCRIPTION:
        Insert an entry into the type version table and the type version
        table's index.  The entry's type name and user readable version
        fields are updated with the input values.  All other fields are
        initialized to null.
   RETURNS:
        OCI_SUCCESS if the function completes successfully.
        OCI_INVALID_HANDLE if 'env' or 'err' is null.
        OCI_ERROR if
            1) any of the parameters is invalid.
            2) an entry for 'type_name' has already been registered in the
               type version table.
 */

/*------------------------------ OCITypeVTSelect ----------------------------*/

/* OCITypeVTSelect - OCI type VERSion table SELECT entry */
/* ** OBSOLETE ** */
sword OCITypeVTSelect(    OCIEnv *env, OCIError *err, 
                          const oratext *schema_name, ub4 s_n_length, 
                          const oratext *type_name, ub4 t_n_length, 
                          oratext **user_version, ub4 *u_v_length,
                        ub2 *version );
/*
   NAME: OCITypeVTSelect - OCI type Version table SELect entry.
   PARAMETERS:
        env (IN/OUT) - OCI environment handle initialized in object mode
        err (IN/OUT) - error handle. If there is an error, it is
                recorded in 'err' and this function returns OCI_ERROR.
                The error recorded in 'err' can be retrieved by calling
                OCIErrorGet().
        schema_name (IN, optional) - name of schema associated with the 
                  type.  By default, the user's schema name is used.
        s_n_length (IN) - length of the 'schema_name' parameter
        type_name (IN) - type name to select
        t_n_length (IN) - length (in bytes) of the 'type_name' parameter
        user_version (OUT, optional) - pointer to user readable version of the
                 type
        u_v_length (OUT, optional) - length (in bytes) of the 'user_version'
                 parameter
        version (OUT, optional) - internal type version
   REQUIRES:
        All input parameters must not be NULL and must be valid.
   DESCRIPTION:
        Select an entry in the type version table by name.
   RETURNS:
        OCI_SUCCESS if the function completes successfully.
        OCI_INVALID_HANDLE if 'env' or 'err' is null.
        OCI_ERROR if
            1) any of the parameters is invalid.
            2) an entry with 'type_name' does not exist.
 */

/* Compatibility function - following function prototype retained for 
   compatibility only */
sword ortgcty(    OCIEnv *env, OCIError *err, OCIType *coll_tdo,
                  OCIType **collelem_tdo    );

/*---------------------------------------------------------------------------*/
/*               Transient Type Construction functions                       */
/*---------------------------------------------------------------------------*/

sword OCITypeBeginCreate(OCISvcCtx *svchp, OCIError *errhp, OCITypeCode tc,
                         OCIDuration dur, OCIType **type);
/*
   NAME: OCITypeBeginCreate - OCI Type Begin Creation of a transient type.
   REMARKS
       Begins the construction process for a transient type. The type will be
       anonymous (no name). To create a persistent named type, the CREATE TYPE
       statement should be used from SQL. Transient types have no identity.
       They are pure values.
   PARAMETERS:
       svchp (IN)       - The OCI Service Context.
       errhp (IN/OUT)   - The OCI error handle. If there is an error, it is
                          recorded in errhp and this function returns
                          OCI_ERROR. Diagnostic information can be obtained by
                          calling OCIErrorGet().
       tc               - The TypeCode for the type. The Typecode could
                          correspond to a User Defined Type or a Built-in type.
                          Currently, the permissible values for User Defined 
                          Types are OCI_TYPECODE_OBJECT for an Object Type
                          (structured), OCI_TYPECODE_VARRAY for a VARRAY
                          collection type or OCI_TYPECODE_TABLE for a nested
                          table collection type. For Object types,
                          OCITypeAddAttr() needs to be called to add each of
                          the attribute types. For Collection types,
                          OCITypeSetCollection() needs to be called.
                          Subsequently, OCITypeEndCreate() needs to be called 
                          to finish the creation process.
                          The permissible values for Built-in type codes are
                          specified in the user manual. Additional information
                          on built-ins if any (like precision, scale for
                          numbers, character set info for VARCHAR2s etc.) must
                          be set with a subsequent call to OCITypeSetBuiltin().
                          Subsequently OCITypeEndCreate() needs to be called
                          to finish the creation process.
       dur              - The allocation duration for the Type. Could be a
                          predefined or a user defined duration.
       type(OUT)        - The OCIType (Type Descriptor) that is being
                          constructed.
  RETURNS:
        OCI_SUCCESS if the function completes successfully.
        OCI_ERROR on error.
*/


sword OCITypeSetCollection(OCISvcCtx *svchp, OCIError *errhp, OCIType *type,
                           OCIParam *collelem_info, ub4 coll_count);
/*
   NAME: OCITypeSetCollection - OCI Type Set Collection information
   REMARKS :
       Set Collection type information. This call can be called only if the
       OCIType has been constructed with a collection typecode.
   PARAMETERS:
       svchp (IN)      -  The OCI Service Context.
       errhp (IN/OUT)  -  The OCI error handle. If there is an error, it is 
                          recorded in errhp and this function returns
                          OCI_ERROR. Diagnostic information can be obtained by
                          calling OCIErrorGet().
       type(IN OUT)    -  The OCIType (Type Descriptor) that is being
                          constructed.
       collelem_info   -  collelem_info provides information on the collection
                          element. It is obtained by allocating an OCIParam
                          (parameter handle) and setting type information in
                          the OCIParam using OCIAttrSet() calls.
       coll_count      -  The count of elements in the collection. Pass 0 for
                          a nested table (unbounded).
  RETURNS:
        OCI_SUCCESS if the function completes successfully.
        OCI_ERROR on error.
*/

sword OCITypeSetBuiltin(OCISvcCtx *svchp, OCIError *errhp, OCIType *type,
                        OCIParam *builtin_info);
/*
   NAME: OCITypeSetBuiltin - OCI Type Set Builtin information.
   REMARKS:
       Set Built-in type information. This call can be called only if the
       OCIType has been constructed with a built-in typecode
       (OCI_TYPECODE_NUMBER etc.).
   PARAMETERS:
       svchp (IN)       - The OCI Service Context.
       errhp (IN/OUT)   - The OCI error handle. If there is an error, it is 
                          recorded in errhp and this function returns
                          OCI_ERROR. Diagnostic information can be obtained by
                          calling OCIErrorGet().
       type(IN OUT)     - The OCIType (Type Descriptor) that is being
                          constructed.
       builtin_info     - builtin_info provides information on the built-in
                          (like precision, scale, charater set etc.). It is
                          obtained by allocating an OCIParam (parameter handle)
                          and setting type information in the OCIParam using
                           OCIAttrSet() calls.
  RETURNS:
        OCI_SUCCESS if the function completes successfully.
        OCI_ERROR on error.
*/

sword OCITypeAddAttr(OCISvcCtx *svchp, OCIError *errhp, OCIType *type,
                     const oratext *a_name, ub4 a_length, 
                     OCIParam *attr_info);
/*
   NAME: OCITypeAddAttr - OCI Type Add Attribute to an Object Type.
   REMARKS:
       Adds an attribute to an Object type (that was constructed earlier with
       typecode OCI_TYPECODE_OBJECT).
   PARAMETERS:
       svchp (IN)       - The OCI Service Context
       errhp (IN/OUT)   - The OCI error handle. If there is an error, it is
                          recorded in errhp and this function returns
                          OCI_ERROR. Diagnostic information can be obtained by
                          calling OCIErrorGet().
       type (IN/OUT)    - The Type description that is being constructed.
       a_name(IN)       - Optional. gives the name of the attribute.
       a_length         - Optional. gives length of attribute name.
       attr_info        - Information on the attribute. It is obtained by
                          allocating an OCIParam (parameter handle) and setting
                          type information in the OCIParam using OCIAttrSet()
                          calls.
  RETURNS:
        OCI_SUCCESS if the function completes successfully.
        OCI_ERROR on error.
*/

sword OCITypeEndCreate(OCISvcCtx *svchp, OCIError *errhp, OCIType *type);
/*
   NAME: OCITypeEndCreate - OCI Type End Creation
   REMARKS:
       Finishes construction of a type description.Subsequently, only access
       will be allowed.
   PARAMETERS:
       svchp (IN)       - The OCI Service Context
       errhp (IN/OUT)   - The OCI error handle. If there is an error, it is
                          recorded in errhp and this function returns
                          OCI_ERROR. Diagnostic information can be obtained by
                          calling OCIErrorGet().
       type (IN/OUT)    - The Type description that is being constructed.
   RETURNS:
        OCI_SUCCESS if the function completes successfully.
        OCI_ERROR on error.
*/

/*=========================*/
/* PUBLIC MACROS AND FLAGS */
/*=========================*/

/*--------------------------------------------------------------------------*/
/*                          TYPE ELEMENT FLAGS                              */
/*--------------------------------------------------------------------------*/

#define OCI_TYPEELEM_REF        0x8000                   /* element is a REF */
#define OCI_TYPEPARAM_REQUIRED  0x0800              /* parameter is required */

/* macros to test flags */
#define OCI_TYPEELEM_IS_REF(elem_flag) \
  (((elem_flag) & OCI_TYPEELEM_REF)!=0)
#define OCI_TYPEPARAM_IS_REQUIRED(param_flag) \
  (((param_flag) & OCI_TYPEPARAM_REQUIRED)!=0)


#endif /* ORT_ORACLE */

