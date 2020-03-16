/* Copyright (c) 2000, 2007, Oracle. All rights reserved.  */

/* 
   NAME 
     occiObjects.h - header file for OCCI object classes

   DESCRIPTION 
     Class definitions for Ref, RefAny, AnyData

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


*/

#ifndef _olint   /* disable olint check */

#ifndef OCCIOBJECTS_ORACLE
# define OCCIOBJECTS_ORACLE

#ifndef OCCICOMMON_ORACLE
#include <occiCommon.h>
#endif

namespace oracle {
namespace occi {
struct AnyDataCtx {
   ConnectionImpl *occiSession;
   OCIAnyData *anyData;
   void *objHeader;
   ub4 errNum;
};
typedef struct AnyDataCtx AnyDataCtx;

class PObject
{
  public:
        enum LockOption {OCCI_LOCK_WAIT, OCCI_LOCK_NOWAIT};
        enum UnpinOption {OCCI_PINCOUNT_DECR, OCCI_PINCOUNT_RESET};
        static void destroy(void *);
        static void refresh(void *); 
        PObject();
        PObject(const void *ctx);
        PObject(const PObject& obj);
        virtual ~PObject();
        PObject& operator=(const PObject& obj);
        void *operator new(size_t size);
        void *operator new(size_t size, const Connection *x,
                           const OCCI_STD_NAMESPACE::string& tablename, 
                           const char *typeName);
        void *operator new(size_t size, const Connection *sess,
               const OCCI_STD_NAMESPACE::string& tablename,
               const OCCI_STD_NAMESPACE::string& typName ,
               const OCCI_STD_NAMESPACE::string& schTabName="",
               const OCCI_STD_NAMESPACE::string& schTypName = "");
        void *operator new(size_t size, const Connection *sess,
               const UString& tablename, const UString &typName,
               const UString& schTabName, const UString& schTypName);
        void *operator new(size_t size, void *adctx);
        void operator delete(void *obj, size_t size);
        RefAny getRef() const;
        bool isLocked() const;
        void unpin(UnpinOption mode=OCCI_PINCOUNT_DECR);
        void pin();
        void lock(PObject::LockOption lock_option);
        void unmark();
        void flush();
        void markDelete();
        void markModified();
        bool isNull() const;
        void setNull();
        const Connection *getConnection() const;
        virtual OCCI_STD_NAMESPACE::string getSQLTypeName() const = 0;
        virtual void getSQLTypeName(Environment *env, void **schName,
                     unsigned int &schNameLen, void **typeName,
                     unsigned int &typeNameLen) const = 0;
        void getSQLTypeName(Environment *env, void *(*rSQL)(void *),
                            void **schname, unsigned int &schnamelen,
                            void **typname, unsigned int &typnamelen) const;
        virtual void writeSQL(AnyData& stream) = 0;
        virtual void readSQL(AnyData& stream) = 0;
  private:
       static void initialise( void * obj, const Connection * sess,
              void *schTabName, unsigned int schTabLen,
              void *tableName, unsigned int tabLen,
              void *schTypName, unsigned int schTypLen,
              void *typeName, unsigned int typLen);

        ConnectionImpl  *occiSession_;
        void *objHeader_;
        ub2 customNewed_;
        enum {CUSTOM_NEWED = 0x5cde};
        ub2 flags_;
        enum {NULL_INFO = 0x0001, GARBAGE_COLLECTED = 0x0002,
              REFRESH_OBJECT = 0x0004,
              CACHED_OBJECT = 0xBAF8};
        //check PObject implementation for CACHED_OBJECT flag
        // for future use 
        void *pobjectExt;
        friend class RefImpl;
};

class AnyData
{
    public:
    ~AnyData();
    AnyData(void *any) ;
    AnyData(const Connection *sessp);
    AnyData(const Connection *sessp, OCIAnyData *any, bool freeImg = true) ; 
 
    AnyData(const AnyData &src);
    AnyData& operator = (const AnyData &src);

    OCIAnyData* getOCIAnyData() const;
    const Connection* getConnection() const;


    bool isNull() const ;
    void setNull() ;
    OCCI_STD_NAMESPACE::string getString() const ;
    UString getUString() const ;
    Blob getBlob() const ;
    Clob getClob() const ;
    Bfile getBfile() const ;
    BFloat  getBFloat() const ;
    BDouble getBDouble() const ;
    Number getNumber() const ;
    Bytes getBytes() const ;
    Date getDate() const ;
    Timestamp getTimestamp() const ;
    IntervalYM getIntervalYM() const ;
    IntervalDS getIntervalDS() const ;
    PObject *getObject(void *(*rSQL)(void *)) const ;
    RefAny getRef() const ;

    void setString(const OCCI_STD_NAMESPACE::string &str)  ;
    void setUString(const UString &str)  ;
    void setBlob(const Blob &blob) ;
    void setClob(const Clob &clob) ;
    void setBfile(const Bfile &bfile) ;
    void setBFloat(const BFloat &n) ;
    void setBDouble(const BDouble &n) ;
    void setNumber(const Number &n) ;
    void setBytes(const Bytes &bytes) ;
    void setDate(const Date &date) ;
    void setTimestamp(const Timestamp &timestamp)  ;
    void setIntervalYM(const IntervalYM &intervalym)  ;
    void setIntervalDS(const IntervalDS &intervalds)  ;
    void setObject(const PObject *objptr) ;
    void setRef(const RefAny &ref) ;

    void setFromString(const OCCI_STD_NAMESPACE::string &str)  ;
    void setFromBfile(const Bfile &bfile) ;
    void setFromBFloat(const BFloat &n) ;
    void setFromBDouble(const BDouble &n) ;
    void setFromNumber(const Number &n) ;
    void setFromBytes(const Bytes &bytes) ;
    void setFromDate(const Date &date) ;
    void setFromTimestamp(const Timestamp &timestamp)  ;
    void setFromIntervalYM(const IntervalYM &intervalym)  ;
    void setFromIntervalDS(const IntervalDS &intervalds)  ;
    void setFromObject(const PObject *objptr) ;
    void setFromRef(const RefAny &ref,
                    const OCCI_STD_NAMESPACE::string &typname,
                    const OCCI_STD_NAMESPACE::string &schname) ;

    OCCI_STD_NAMESPACE::string getAsString() const ;
    Bfile getAsBfile() const ;
    BFloat  getAsBFloat() const ;
    BDouble getAsBDouble() const ;
    Number getAsNumber() const ;
    Bytes getAsBytes() const ;
    Date getAsDate() const ;
    Timestamp getAsTimestamp() const ;
    IntervalYM getAsIntervalYM() const ;
    IntervalDS getAsIntervalDS() const ;
    PObject *getAsObject() const ;
    RefAny getAsRef() const ;

    TypeCode getType() const;
    
    private:

                                 
    // private data members
    Ptr<AnyDataImpl> anyDataImplPtr;              
   

};

template <class T>
class Ref
{
  public:

  Ref();
  Ref(const T *obj) ;
  Ref(const RefAny &refAny) ;
  Ref(const Ref<T> &src) ;
  Ref(const Connection *sessp, OCIRef *tref, bool copy=TRUE) 
  ;
 ~Ref();
  Ref<T>& operator=(const Ref<T> &src) 
  ;
  Ref<T>& operator=(const T *obj) ;
  Ref<T>& operator=(const RefAny &src);
  T * operator->() ;
  T * ptr() ;
  T & operator *() ;
  const T * operator->() const;
  const T * ptr()  const;
  const T & operator *() const ;
  void markDelete() ;
  void unmarkDelete() ;
  void setNull();
  bool isNull() const;
  void clear() ;
  bool isClear() const;
  void setPrefetch(const OCCI_STD_NAMESPACE::string &typeName, 
                   unsigned int depth);
  void setPrefetch(const OCCI_STD_NAMESPACE::string &schName,
                   const OCCI_STD_NAMESPACE::string &typeName,
                   unsigned int depth);
  void setPrefetch(const UString &schName,
                   const UString &typeName,
                   unsigned int depth);
  void setPrefetch(unsigned int depth) ;
  void setLock(LockOptions );
  operator RefAny() const;
  OCIRef *getRef() const;
  const Connection *getConnection() const;
  bool operator == (const Ref<T> &ref) const;
  bool operator != (const Ref<T> &ref) const;
  bool operator == (const RefAny &refAnyR) const ;
  bool operator != (const RefAny &refAnyR) const ;
  OCIComplexObject *getCor() const;
  void setPinnedObject(PObject *objPtr); 
 private:

  RefImpl  *rimplPtr;
};


class RefImpl
{
  public:

  RefImpl();
  RefImpl(PObject *obj) ;
  RefImpl(const RefAny &refAny) ;
  RefImpl(const RefImpl &src) ;
  RefImpl(const Connection *sessp, OCIRef *tref, 
  bool copy=TRUE) ;
  ~RefImpl();
  bool isNull() const ;
  void setNull() ;
  void markDelete() ;
  void unmarkDelete() ;
  void clear() ;
  bool isClear() const ;
  void setPrefetch(const OCCI_STD_NAMESPACE::string &typeName, 
                   unsigned int depth) ;
  void setPrefetch(const OCCI_STD_NAMESPACE::string &schName,
                   const OCCI_STD_NAMESPACE::string &typeName,
                   unsigned int depth);
  void setPrefetch(const UString &schName,
                   const UString &typeName,
                   unsigned int depth);
  void setPrefetch(unsigned int depth) ;
  void setLock(LockOptions lckOption) ;
  PObject *pin() ;
  void unpin(PObject *obj) ;
  void setRefFromObjPtr(const PObject *obj) ;
  OCIRef* getRef() const;
  void setRefImpl(RefImpl *rptr);
  const Connection * getConnection() const;
  bool operator == (const RefImpl &refI) const ;
  bool operator == (const RefAny &refAnyR) const ;
  void assignObj(PObject *newObjPtr) ;
  void assignRefAny(const RefAny &src) ;
  // added following methods
  bool isEqual(PObject *obj);
  void operator = ( const RefImpl &src);
  OCIComplexObject *getCor() const;
  void setPinnedObject( PObject *objPtr);
 private:

  OCIRef *ref;
  const ConnectionImpl *sessp;
  OCIComplexObject *corhp;
  OCCI_STD_NAMESPACE::list<void *> descriptorList;
  LockOptions lockOption;
  // added data member for object header
  void *objHeader;
  //common implementation function for setPrefetch
  void do_setPrefetch(void *schName, unsigned int schNameLen,
                      void *typeName, unsigned int typeNameLen,
                      unsigned int depth);
};


class RefAny
{
  public:

  RefAny();
  RefAny (const Connection *sessptr, const OCIRef *ref);
  RefAny (const Connection *sessptr, const OCIRef *ref, bool isowner);
  ~RefAny() ;
  RefAny(const RefAny& src) ;
  RefAny& operator=(const RefAny& src) ;
  void markDelete() ;
  void unmarkDelete() ;
  void clear() ;
  bool isNull() const;
  OCIRef * getRef() const;
  const Connection * getConnection() const;
  bool operator == (const RefAny &refAnyR) const;
  bool operator != (const RefAny &refAnyR) const;
  bool isOwner() const;

  private:

  OCIRef *ref;
  const ConnectionImpl *sessp;
  // for future use
  void *refanyExt;
  bool owner;

  friend RefAny MetaData::getRef(MetaData::AttrId) const;
  friend RefAny PObject::getRef() const;
  friend class AnyDataImpl;
  friend class ResultSetImpl;
  friend class StatementImpl;
  friend void getVector(const ResultSet  *rs,
                        unsigned int colIndex,
                        OCCI_STD_NAMESPACE::vector<RefAny> &vect) ;
  friend void getVector(const Statement  *stmt,
                        unsigned int colIndex,
                        OCCI_STD_NAMESPACE::vector<RefAny> &vect) ;
};

template <class T>
Ref<T>::Ref()
{
   rimplPtr = new RefImpl();
}

template <class T>
Ref<T>::Ref(const T *obj)
{
   rimplPtr = new RefImpl((PObject *)obj);
}

template <class T>
Ref<T>::Ref(const RefAny &refAny)

{
  rimplPtr = new RefImpl(refAny);
}

template <class T>
Ref<T>::Ref(const Ref<T>& src)

{
    rimplPtr = new RefImpl(*(src.rimplPtr));
}

template <class T>
Ref<T>::Ref(const Connection *sessp, OCIRef *tref, bool copy)

{
   rimplPtr = new RefImpl(sessp, tref, copy);
}

template <class T>
Ref<T>::~Ref()
{
  delete rimplPtr;
}


template <class T>
Ref<T>& Ref<T>::operator=(const Ref<T> &src)
{
   if (&src == this)
     return *this;
   *rimplPtr = *(src.rimplPtr);
   return *this;
}

template <class T>
Ref<T>& Ref<T>::operator=(const T *obj)
{
  if (rimplPtr->isEqual((PObject *)obj))
    return *this;
  rimplPtr->assignObj((PObject *)obj);
  return *this;
}

template <class T>
Ref<T>& Ref<T>::operator=(const RefAny &src)
{
  rimplPtr->assignRefAny(src);
  return *this;
}

template <class T>
T* Ref<T>::operator->()
{
  return ((T *)rimplPtr->pin());
}

template <class T>
T* Ref<T>::ptr()
{
  return ((T *)rimplPtr->pin());
}

template <class T>
T& Ref<T>::operator * ()
{
  return ((T &)(*(rimplPtr->pin())));
}

template <class T>
const T* Ref<T>::operator->() const
{
  return ((const T *)rimplPtr->pin());
}

template <class T>
const T* Ref<T>::ptr() const
{
  return ((const T *)rimplPtr->pin());
}

template <class T>
const T& Ref<T>::operator * () const
{
  return ((const T &)(*(rimplPtr->pin())));
}

template <class T>
void Ref<T>::markDelete ()
{
   rimplPtr->markDelete();
}

template <class T>
void Ref<T>::unmarkDelete ()
{
   rimplPtr->unmarkDelete();
}

template <class T>
void Ref<T>::setNull()
{
   rimplPtr->setNull();
}

template <class T>
bool Ref<T>::isNull() const
{
  return rimplPtr->isNull();
}

template <class T>
void Ref<T>::clear ()
{
   rimplPtr->clear();
}

template <class T>
bool Ref<T>::isClear() const
{
   return rimplPtr->isClear();
}

template <class T>
void Ref<T>::setPrefetch (const OCCI_STD_NAMESPACE::string &typeName,
unsigned int depth)

{
  rimplPtr->setPrefetch(typeName,depth);
}

template <class T>
void Ref<T>::setPrefetch (const OCCI_STD_NAMESPACE::string &schemaName,
const OCCI_STD_NAMESPACE::string &typeName,
unsigned int depth)

{
  rimplPtr->setPrefetch(schemaName,typeName,depth);
}

template <class T>
void Ref<T>::setPrefetch (const UString &schemaName,
const UString &typeName,
unsigned int depth)

{
  rimplPtr->setPrefetch(schemaName,typeName,depth);
}

template <class T>
void Ref<T>::setPrefetch (unsigned int depth)

{
  rimplPtr->setPrefetch(depth);
}

template <class T>
void Ref<T>::setLock (LockOptions lckOption)
{
  rimplPtr->setLock(lckOption);
}

template <class T>
OCIRef* Ref<T>::getRef() const
{
  return (rimplPtr->getRef());
}

template<class T>
const Connection* Ref<T>::getConnection () const
{
  return (rimplPtr->getConnection());
}

template <class T>
Ref<T>::operator RefAny () const
{
  if (isNull())
    return  RefAny();
  return (RefAny(rimplPtr->getConnection(), rimplPtr->getRef()));
}

template <class T>
bool Ref<T>::operator ==(const Ref<T> &ref) const

{
  return ( (*rimplPtr) == (*(ref.rimplPtr)) );
}

template <class T>
bool Ref<T>::operator !=(const Ref<T> &ref) const

{
  return ( !((*rimplPtr) == (*(ref.rimplPtr))) );
}

template <class T>
bool Ref<T>::operator == (const RefAny & refAnyR) const

{
  return ( (*rimplPtr) == refAnyR );
}

template <class T>
bool Ref<T>::operator != (const RefAny & refAnyR) const

{
  return ( !((*rimplPtr) == refAnyR ));
}

template <class T>
OCIComplexObject * Ref<T>::getCor() const
{
  return (rimplPtr->getCor());
}

template < class T>
void Ref<T>::setPinnedObject( PObject *objPtr)
{
   rimplPtr->setPinnedObject(objPtr);
}

/*---------------------------------------------------------------------------
                PROTOTYPES USED BY FUNCTION TEMPLATES
 ---------------------------------------------------------------------------*/
  void getVectorOfOCIRefs( const AnyData &any, 
  OCCI_STD_NAMESPACE::vector<void *> &vect);
  void getVectorOfPObjects( const AnyData &any,
  OCCI_STD_NAMESPACE::vector< PObject* > &vect,
  void *(*rSQL)(void *))  ;
  void setVectorOfOCIRefs( AnyData &any, 
  const OCCI_STD_NAMESPACE::vector<void *> &vect,
  const OCCI_STD_NAMESPACE::vector< OCIInd> &vec_ind) ;
  void setVectorOfPObjects( AnyData &any, 
  const OCCI_STD_NAMESPACE::vector< PObject* > &vect) ;

/*---------------------------------------------------------------------------
                           EXPORT FUNCTIONS
  ---------------------------------------------------------------------------*/

/*------------------- getVector for POBject----------------------------*/
/*
   NAME
      getVector - overloaded function. Retrieves the attribute in the
   current position as a vector of PObject

   PARAMETERS
      any - AnyData
      vect- reference to vector of PObject (OUT parameter).

   DESCRIPTION
   Retrieves the attribute in the current position as a vector
   of PObject
   The attribute at the current position should be a collection
   type (varray or nested table). The SQL type of the elements in
   the collection should be compatible with PObject

   RETURNS
     nothing

   NOTES
     compatible SQL types : user defined types (SQLT_NTY) etc.
*/

#if defined(WIN32COMMON) || defined(__MVS__)
// and other platforms that do not support
// partial function template specialization
 template <class T>
 void getVector(const AnyData &any, OCCI_STD_NAMESPACE::vector<T> &vect,
 void *(*rSQL)(void *))
 {
   OCCI_STD_NAMESPACE::vector< PObject *> vec_pobj;
   getVectorOfPObjects( any, vec_pobj, rSQL);

   vect.clear();
   unsigned int size= vec_pobj.size();
   vect.reserve( size );
   for( unsigned int i=0; i< size; i++)
      vect.push_back( (T)vec_pobj[i] );
 }
#else
 template <class T>
 void getVector(const AnyData &any, OCCI_STD_NAMESPACE::vector<T*> &vect,
 void *(*rSQL)(void *)) 
 {
   OCCI_STD_NAMESPACE::vector< PObject *> vec_pobj;
   getVectorOfPObjects( any, vec_pobj, rSQL);

   vect.clear();
   unsigned int size= vec_pobj.size();
   vect.reserve( size );
   for( unsigned int i=0; i< size; i++)
      vect.push_back( (T*)vec_pobj[i] );
 }
#endif /* end of #ifdef WIN32COMMON */

 /*------------------- getVector for Ref<T>----------------------------*/
/*
   NAME
      getVector - overloaded function. Retrieves the attribute in the
   current position as a vector of PObject

   PARAMETERS
      any - AnyData
      vect- reference to vector of PObject (OUT parameter).

   DESCRIPTION
   Retrieves the attribute in the current position as a vector
   of PObject
   The attribute at the current position should be a collection
   type (varray or nested table). The SQL type of the elements in
   the collection should be compatible with PObject

   RETURNS
     nothing

   NOTES
     compatible SQL types : user defined types (SQLT_NTY) etc.
*/
#if !defined(WIN32COMMON) && !defined(__MVS__)
 template <class T>
 void getVector(const AnyData &any,OCCI_STD_NAMESPACE::vector< Ref<T> > &vect) 
 {
   OCCI_STD_NAMESPACE::vector< void *> vec_ref;
   getVectorOfOCIRefs( any, vec_ref);

   vect.clear();
   unsigned int size = vec_ref.size();
   vect.reserve( size );
   const Connection *sess = any.getConnection();

   for (unsigned int i=0; i< size; i++)
   {
     if (vec_ref[i] == (OCIRef *)0)
       vect.push_back(Ref<T>());     // pushing a default-constructed Ref
     else
       vect.push_back(Ref<T>(sess, (OCIRef *)vec_ref[i], FALSE));
   }
 }
#endif /* end of #ifndef WIN32COMMON  */
 
/*-----------------------setVector for PObject--------------------------*/
/*
   NAME
     setVector - overloaded function. sets the attribute in the current
   position of anydata with the vector elements.

   PARAMETERS
      none.

   DESCRIPTION
      sets the attribute in the current position in anydata with the
   vector elements.
  The attribute in the current position of anydata should be a
  collection type. If the collection type is a varray, the input vector
  size should be equal to the size of the varray. Also the SQL type of
  the collection's elements should be compatible with PObject.

   RETURNS
     nothing.

   NOTES
     compatible SQL types : SQLT_NTY  (user defined types).
*/
#if defined(WIN32COMMON) || defined(__MVS__)
// and other platforms that do not support
// partial function template specialization

 template <class T>
 void setVector(AnyData &any, const OCCI_STD_NAMESPACE::vector<T> &vect) 
 {
   OCCI_STD_NAMESPACE::vector< PObject *> vec_pobj;
   unsigned int size= vect.size();
   vec_pobj.reserve( size );
   for( unsigned int i=0; i< size; i++)
      vec_pobj.push_back( vect[i] );
   setVectorOfPObjects( any, vec_pobj);

 }
 
#else

 template <class T>
 void setVector(AnyData &any, const OCCI_STD_NAMESPACE::vector<T*> &vect)
 {
   OCCI_STD_NAMESPACE::vector< PObject *> vec_pobj;
   unsigned int size= vect.size();
   vec_pobj.reserve( size );
   for( unsigned int i=0; i< size; i++)
      vec_pobj.push_back( vect[i] );
   setVectorOfPObjects( any, vec_pobj);

 }
#endif /* end of #ifdef WIN32COMMON */

/*-----------------------setVector for Ref<T>--------------------------*/
/*
   NAME
     setVector - overloaded function. sets the attribute in the current
   position of anydata with the vector elements.

   PARAMETERS
      none.

   DESCRIPTION
      sets the attribute in the current position in anydata with the
   vector elements.
  The attribute in the current position of anydata should be a
  collection type. If the collection type is a varray, the input vector
  size should be equal to the size of the varray. Also the SQL type of
  the collection's elements should be compatible with PObject.

   RETURNS
     nothing.

   NOTES
     compatible SQL types : SQLT_NTY  (user defined types).
*/
#if !defined(WIN32COMMON) && !defined(__MVS__)
 template <class T>
 void setVector(AnyData &any, const OCCI_STD_NAMESPACE::vector< Ref<T> > &vect)
 {
   OCCI_STD_NAMESPACE::vector< void *> vec_ref;
   OCCI_STD_NAMESPACE::vector<OCIInd> vec_ind;

   unsigned int size= vect.size();
   vec_ref.reserve( size );
   vec_ind.reserve( size );
   for( unsigned int i=0; i< size; i++)
   {
      vec_ref.push_back( vect[i].getRef() );
      vec_ind.push_back(vect[i].isNull() ? OCI_IND_NULL : OCI_IND_NOTNULL);
   }
   setVectorOfOCIRefs( any, vec_ref, vec_ind);

 }
#endif  /* end of #ifndef WIN32COMMON */
 
// Platform independent get/setVectorOfRefs method added 
// get(set)Vector of Ref<T> and get(set)VectorOfRefs are identical
// in functionality.

 /*------------------- getVectorOfRefs for Ref<T>----------------------------*/
/*
   NAME
      getVectorOfRefs - overloaded function. Retrieves the attribute in the
   current position as a vector of PObject

   PARAMETERS
      any - AnyData
      vect- reference to vector of PObject (OUT parameter).

   DESCRIPTION
   Retrieves the attribute in the current position as a vector
   of PObject
   The attribute at the current position should be a collection
   type (varray or nested table). The SQL type of the elements in
   the collection should be compatible with PObject

   RETURNS
     nothing

   NOTES
     compatible SQL types : user defined types (SQLT_NTY) etc.
*/

 template <class T>
 void getVectorOfRefs(const AnyData &any,
                      OCCI_STD_NAMESPACE::vector< Ref<T> > &vect)
 {
   OCCI_STD_NAMESPACE::vector< void *> vec_ref;
   getVectorOfOCIRefs( any, vec_ref);

   vect.clear();
   unsigned int size = vec_ref.size();
   vect.reserve( size );
   const Connection *sess = any.getConnection();

   for (unsigned int i=0; i< size; i++)
   {
     if (vec_ref[i] == (OCIRef *)0)
       vect.push_back(Ref<T>());     // pushing a default-constructed Ref
     else
       vect.push_back(Ref<T>(sess, (OCIRef *)vec_ref[i], FALSE));
   }
 }

/*-----------------------setVectorOfRefs for Ref<T>--------------------------*/
/*
   NAME
     setVectorOfRefs - overloaded function. sets the attribute in the current
   position of anydata with the vector elements.

   PARAMETERS
      none.

   DESCRIPTION
      sets the attribute in the current position in anydata with the
   vector elements.
  The attribute in the current position of anydata should be a
  collection type. If the collection type is a varray, the input vector
  size should be equal to the size of the varray. Also the SQL type of
  the collection's elements should be compatible with PObject.

   RETURNS
     nothing.

   NOTES
     compatible SQL types : SQLT_NTY  (user defined types).
*/

 template <class T>
 void setVectorOfRefs(AnyData &any, 
                      const OCCI_STD_NAMESPACE::vector< Ref<T> > &vect)

 {
   OCCI_STD_NAMESPACE::vector< void *> vec_ref;
   OCCI_STD_NAMESPACE::vector<OCIInd> vec_ind;

   unsigned int size= vect.size();
   vec_ref.reserve( size );
   vec_ind.reserve( size );
   for( unsigned int i=0; i< size; i++)
   {
      vec_ref.push_back( vect[i].getRef() );
      vec_ind.push_back(vect[i].isNull() ? OCI_IND_NULL : OCI_IND_NOTNULL);
   }
   setVectorOfOCIRefs( any, vec_ref, vec_ind);

 }


/*---------------------------------------------------------------------------
                          INTERNAL FUNCTIONS
  ---------------------------------------------------------------------------*/


} /* end of namespace occi */
} /* end of namespace oracle */
#endif                                              /* OCCIOBJECTS_ORACLE */

#endif                                              /* _olint */
