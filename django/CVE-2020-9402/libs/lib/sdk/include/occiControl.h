/* Copyright  Oracle Corporation 2000, 2012. All Rights Reserved. */ 
 
/* 
   NAME 
     occiControl.h - header file for OCCI control classes

   DESCRIPTION 
     Class definitions for MetaData,SQLException,Environment,
     Connection,Statement, ConnectionPool, StatelessConnectionPool

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

#ifndef OCCICONTROL_ORACLE
# define OCCICONTROL_ORACLE

#ifndef OCCICOMMON_ORACLE
#include <occiCommon.h>
#endif

#ifndef ORAEXCEPTION
#define ORAEXCEPTION
#include <exception>
#endif

namespace oracle {
namespace occi {
/*---------------------------------------------------------------------------
                     PUBLIC TYPES AND CONSTANTS
  ---------------------------------------------------------------------------*/

class MetaData
{
  public :
    
      enum AttrId 
      {ATTR_PTYPE = OCI_ATTR_PTYPE,
      ATTR_TIMESTAMP = OCI_ATTR_TIMESTAMP, 
      ATTR_OBJ_ID = OCI_ATTR_OBJ_ID,
      ATTR_OBJ_NAME = OCI_ATTR_OBJ_NAME, 
      ATTR_OBJ_SCHEMA = OCI_ATTR_OBJ_SCHEMA,
      ATTR_OBJID = OCI_ATTR_OBJID,
      ATTR_NUM_COLS = OCI_ATTR_NUM_COLS,
      ATTR_LIST_COLUMNS = OCI_ATTR_LIST_COLUMNS,
      ATTR_REF_TDO = OCI_ATTR_REF_TDO,
      ATTR_IS_TEMPORARY = OCI_ATTR_IS_TEMPORARY,
      ATTR_IS_TYPED = OCI_ATTR_IS_TYPED,
      ATTR_DURATION = OCI_ATTR_DURATION,
      ATTR_COLLECTION_ELEMENT = OCI_ATTR_COLLECTION_ELEMENT,
      ATTR_RDBA = OCI_ATTR_RDBA,
      ATTR_TABLESPACE = OCI_ATTR_TABLESPACE,
      ATTR_CLUSTERED = OCI_ATTR_CLUSTERED,
      ATTR_PARTITIONED = OCI_ATTR_PARTITIONED,
      ATTR_INDEX_ONLY = OCI_ATTR_INDEX_ONLY,
      ATTR_LIST_ARGUMENTS = OCI_ATTR_LIST_ARGUMENTS,
      ATTR_IS_INVOKER_RIGHTS = OCI_ATTR_IS_INVOKER_RIGHTS,
      ATTR_LIST_SUBPROGRAMS = OCI_ATTR_LIST_SUBPROGRAMS,
      ATTR_NAME = OCI_ATTR_NAME,
      ATTR_OVERLOAD_ID = OCI_ATTR_OVERLOAD_ID,
      ATTR_TYPECODE = OCI_ATTR_TYPECODE,
      ATTR_COLLECTION_TYPECODE = OCI_ATTR_COLLECTION_TYPECODE,
      ATTR_VERSION = OCI_ATTR_VERSION,
      ATTR_IS_INCOMPLETE_TYPE = OCI_ATTR_IS_INCOMPLETE_TYPE,
      ATTR_IS_SYSTEM_TYPE = OCI_ATTR_IS_SYSTEM_TYPE,
      ATTR_IS_PREDEFINED_TYPE = OCI_ATTR_IS_PREDEFINED_TYPE,
      ATTR_IS_TRANSIENT_TYPE = OCI_ATTR_IS_TRANSIENT_TYPE,
      ATTR_IS_SYSTEM_GENERATED_TYPE = 
              OCI_ATTR_IS_SYSTEM_GENERATED_TYPE,
      ATTR_HAS_NESTED_TABLE = OCI_ATTR_HAS_NESTED_TABLE,
      ATTR_HAS_LOB = OCI_ATTR_HAS_LOB,
      ATTR_HAS_FILE = OCI_ATTR_HAS_FILE,
      ATTR_NUM_TYPE_ATTRS = OCI_ATTR_NUM_TYPE_ATTRS,
      ATTR_LIST_TYPE_ATTRS = OCI_ATTR_LIST_TYPE_ATTRS,
      ATTR_NUM_TYPE_METHODS = OCI_ATTR_NUM_TYPE_METHODS, 
      ATTR_LIST_TYPE_METHODS = OCI_ATTR_LIST_TYPE_METHODS,
      ATTR_MAP_METHOD = OCI_ATTR_MAP_METHOD,
      ATTR_ORDER_METHOD = OCI_ATTR_ORDER_METHOD,
      ATTR_DATA_SIZE = OCI_ATTR_DATA_SIZE,
      ATTR_DATA_TYPE = OCI_ATTR_DATA_TYPE,
      ATTR_PRECISION = OCI_ATTR_PRECISION,
      ATTR_SCALE = OCI_ATTR_SCALE,
      ATTR_TYPE_NAME = OCI_ATTR_TYPE_NAME,
      ATTR_SCHEMA_NAME = OCI_ATTR_SCHEMA_NAME,
      ATTR_CHARSET_ID = OCI_ATTR_CHARSET_ID,
      ATTR_CHARSET_FORM = OCI_ATTR_CHARSET_FORM,
      ATTR_ENCAPSULATION = OCI_ATTR_ENCAPSULATION,
      ATTR_IS_CONSTRUCTOR = OCI_ATTR_IS_CONSTRUCTOR,
      ATTR_IS_DESTRUCTOR = OCI_ATTR_IS_DESTRUCTOR,
      ATTR_IS_OPERATOR = OCI_ATTR_IS_OPERATOR,
      ATTR_IS_SELFISH = OCI_ATTR_IS_SELFISH,
      ATTR_IS_MAP = OCI_ATTR_IS_MAP,
      ATTR_IS_ORDER = OCI_ATTR_IS_ORDER,
      ATTR_IS_RNDS = OCI_ATTR_IS_RNDS,
      ATTR_IS_RNPS = OCI_ATTR_IS_RNPS,
      ATTR_IS_WNDS = OCI_ATTR_IS_WNDS,
      ATTR_IS_WNPS = OCI_ATTR_IS_WNPS,
      ATTR_NUM_ELEMS = OCI_ATTR_NUM_ELEMS,
      ATTR_LINK = OCI_ATTR_LINK,
      ATTR_MIN = OCI_ATTR_MIN,
      ATTR_MAX = OCI_ATTR_MAX,
      ATTR_INCR = OCI_ATTR_INCR,
      ATTR_CACHE = OCI_ATTR_CACHE,
      ATTR_ORDER = OCI_ATTR_ORDER,
      ATTR_HW_MARK  = OCI_ATTR_HW_MARK,
      ATTR_IS_NULL = OCI_ATTR_IS_NULL,
      ATTR_POSITION = OCI_ATTR_POSITION,
      ATTR_HAS_DEFAULT =  OCI_ATTR_HAS_DEFAULT,
      ATTR_LEVEL = OCI_ATTR_LEVEL,
      ATTR_IOMODE = OCI_ATTR_IOMODE,
      ATTR_RADIX = OCI_ATTR_RADIX,
      ATTR_SUB_NAME = OCI_ATTR_SUB_NAME,
      ATTR_LIST_OBJECTS = OCI_ATTR_LIST_OBJECTS,
      ATTR_NCHARSET_ID = OCI_ATTR_NCHARSET_ID,
      ATTR_LIST_SCHEMAS = OCI_ATTR_LIST_SCHEMAS,
      ATTR_MAX_PROC_LEN = OCI_ATTR_MAX_PROC_LEN,
      ATTR_MAX_COLUMN_LEN = OCI_ATTR_MAX_COLUMN_LEN,
      ATTR_CURSOR_COMMIT_BEHAVIOR = 
          OCI_ATTR_CURSOR_COMMIT_BEHAVIOR,
      ATTR_MAX_CATALOG_NAMELEN = OCI_ATTR_MAX_CATALOG_NAMELEN,
      ATTR_CATALOG_LOCATION = OCI_ATTR_CATALOG_LOCATION,
      ATTR_SAVEPOINT_SUPPORT =  OCI_ATTR_SAVEPOINT_SUPPORT,
      ATTR_NOWAIT_SUPPORT =  OCI_ATTR_NOWAIT_SUPPORT,
      ATTR_AUTOCOMMIT_DDL =  OCI_ATTR_AUTOCOMMIT_DDL,
      ATTR_LOCKING_MODE =  OCI_ATTR_LOCKING_MODE,
      ATTR_IS_FINAL_TYPE = OCI_ATTR_IS_FINAL_TYPE,
      ATTR_IS_INSTANTIABLE_TYPE = OCI_ATTR_IS_INSTANTIABLE_TYPE,
      ATTR_IS_SUBTYPE = OCI_ATTR_IS_SUBTYPE,
      ATTR_SUPERTYPE_SCHEMA_NAME = OCI_ATTR_SUPERTYPE_SCHEMA_NAME,
      ATTR_SUPERTYPE_NAME = OCI_ATTR_SUPERTYPE_NAME,
      ATTR_FSPRECISION = OCI_ATTR_FSPRECISION,
      ATTR_LFPRECISION = OCI_ATTR_LFPRECISION,
      ATTR_IS_FINAL_METHOD = OCI_ATTR_IS_FINAL_METHOD,
      ATTR_IS_INSTANTIABLE_METHOD = OCI_ATTR_IS_INSTANTIABLE_METHOD,
      ATTR_IS_OVERRIDING_METHOD = OCI_ATTR_IS_OVERRIDING_METHOD,
      ATTR_CHAR_USED = OCI_ATTR_CHAR_USED,
      ATTR_CHAR_SIZE = OCI_ATTR_CHAR_SIZE,
      ATTR_COL_ENC = OCI_ATTR_COL_ENC,
      ATTR_COL_ENC_SALT = OCI_ATTR_COL_ENC_SALT,
      ATTR_TABLE_ENC = OCI_ATTR_TABLE_ENC,
      ATTR_TABLE_ENC_ALG = OCI_ATTR_TABLE_ENC_ALG,
      ATTR_TABLE_ENC_ALG_ID = OCI_ATTR_TABLE_ENC_ALG_ID,
      ATTR_IS_NOT_PERSISTABLE_TYPE = OCI_ATTR_IS_NOT_PERSISTABLE_TYPE
     };

     enum ParamType
         { 
           PTYPE_TABLE = OCI_PTYPE_TABLE
          ,PTYPE_VIEW = OCI_PTYPE_VIEW
          ,PTYPE_PROC = OCI_PTYPE_PROC
          ,PTYPE_FUNC = OCI_PTYPE_FUNC
          ,PTYPE_PKG = OCI_PTYPE_PKG
          ,PTYPE_TYPE = OCI_PTYPE_TYPE
          ,PTYPE_TYPE_ATTR = OCI_PTYPE_TYPE_ATTR
          ,PTYPE_TYPE_COLL = OCI_PTYPE_TYPE_COLL
          ,PTYPE_TYPE_METHOD = OCI_PTYPE_TYPE_METHOD
          ,PTYPE_SYN = OCI_PTYPE_SYN
          ,PTYPE_SEQ = OCI_PTYPE_SEQ
          ,PTYPE_COL = OCI_PTYPE_COL
          ,PTYPE_ARG = OCI_PTYPE_ARG
          ,PTYPE_TYPE_ARG = OCI_PTYPE_TYPE_ARG
          ,PTYPE_TYPE_RESULT = OCI_PTYPE_TYPE_RESULT
          ,PTYPE_SCHEMA = OCI_PTYPE_SCHEMA
          ,PTYPE_DATABASE = OCI_PTYPE_DATABASE
          ,PTYPE_UNK = OCI_PTYPE_UNK
         };


     enum { DURATION_SESSION = OCI_DURATION_SESSION
          ,DURATION_TRANS = OCI_DURATION_TRANS
          ,DURATION_NULL = OCI_DURATION_NULL
          ,TYPEENCAP_PRIVATE = OCI_TYPEENCAP_PRIVATE
          ,TYPEENCAP_PUBLIC = OCI_TYPEENCAP_PUBLIC
          ,TYPEPARAM_IN = OCI_TYPEPARAM_IN
          ,TYPEPARAM_OUT = OCI_TYPEPARAM_OUT
          ,TYPEPARAM_INOUT = OCI_TYPEPARAM_INOUT
          ,CURSOR_OPEN = OCI_CURSOR_OPEN
          ,CURSOR_CLOSED = OCI_CURSOR_CLOSED
          ,CL_START = OCI_CL_START
          ,CL_END = OCI_CL_END
          ,SP_SUPPORTED = OCI_SP_SUPPORTED
          ,SP_UNSUPPORTED = OCI_SP_UNSUPPORTED
          ,NW_SUPPORTED = OCI_NW_SUPPORTED
          ,NW_UNSUPPORTED = OCI_NW_UNSUPPORTED
          ,AC_DDL = OCI_AC_DDL
          ,NO_AC_DDL = OCI_NO_AC_DDL
          ,LOCK_IMMEDIATE = OCI_LOCK_IMMEDIATE
          ,LOCK_DELAYED = OCI_LOCK_DELAYED  
         }; 

      enum ColumnAttrId
      {
        ATTR_COL_IS_IDENTITY = OCI_ATTR_COL_PROPERTY_IS_IDENTITY,
        ATTR_COL_IS_GEN_ALWAYS = OCI_ATTR_COL_PROPERTY_IS_GEN_ALWAYS,
        ATTR_COL_IS_GEN_BY_DEF_ON_NULL =
             OCI_ATTR_COL_PROPERTY_IS_GEN_BY_DEF_ON_NULL
      };
      MetaData(const MetaData &omd);
      unsigned int getAttributeCount() const 
      ;
      AttrId getAttributeId(unsigned int attributenum) const 
      ;
      Type getAttributeType(unsigned int attributenum) const 
      ;
      int getInt(MetaData::AttrId attrid) const 
      ;
      bool getBoolean(MetaData::AttrId attrid) const 
      ;
      bool getBoolean(MetaData::ColumnAttrId colAttrId) const
      ;
      unsigned int getUInt(MetaData::AttrId attrid) const 
      ;
      OCCI_STD_NAMESPACE::string getString(MetaData::AttrId attrid) const 
      ;
      UString getUString(MetaData::AttrId attrid) const 
      ;
      Number getNumber(MetaData::AttrId attrid) const
      ;
      RefAny getRef(MetaData::AttrId attrid) const 
      ;
      Timestamp getTimestamp(MetaData::AttrId attrid) const 
      ;
      MetaData getMetaData(MetaData::AttrId attrid) const
      ;
      OCCI_STD_NAMESPACE::vector<MetaData> getVector(MetaData::AttrId attrid)
      const ;
      void  operator  =(const MetaData &omd);

      ~MetaData();  
     
   private:

  enum ociAttrType { OCI_UB1, 
                         OCI_UB2, 
                         OCI_UB4,
                         OCI_SB1, 
                         OCI_WORD,
                         OCI_UB1_BOOL,     
                         OCI_UB1PTR_TIMESTAMP,
                         OCI_UB1PTR_NUMBER,
                         OCI_TEXTPTR,
                         OCI_DVOIDPTR_PARAM,
                         OCI_DVOIDPTR_PARAMLIST,
                         OCI_OCIREFPTR,
                         OCI_OCIDURATION,
                         OCI_OCITYPECODE,
                         OCI_OCITYPEENCAP,
                         OCI_OCITYPEPARAMMODE,
                         OCI_OCIPRECISION
                      };

  enum AttrCount {COMMON_ATTR_COUNT = 5,
        TABLE_ATTR_COUNT = 15,
        VIEW_ATTR_COUNT = 7, 
        FUNCPROC_ATTR_COUNT = 4,
        PKG_ATTR_COUNT = 2,
        TYP_ATTR_COUNT = 27, 
        TYPEATTR_ATTR_COUNT = 14,
        TYPEMTHD_ATTR_COUNT = 16,
        COLL_ATTR_COUNT = 12,
        SYN_ATTR_COUNT = 4,
        SEQ_ATTR_COUNT = 7,
        COL_ATTR_COUNT = 15,
        ARG_TYPARG_TYPRES_ATTR_COUNT = 20, 
        SCHEMA_ATTR_COUNT = 1,
        DATABASE_ATTR_COUNT = 13,
        UNK_ATTR_COUNT = 0,
        COLUMN_ATTR_COUNT = 3
        };

  static const AttrId commonAttrId[COMMON_ATTR_COUNT];
  static const ociAttrType commonAttrType[COMMON_ATTR_COUNT];
  static const AttrId tableAttrId[TABLE_ATTR_COUNT];
  static const ociAttrType tableAttrType[TABLE_ATTR_COUNT];
  static const AttrId viewAttrId[VIEW_ATTR_COUNT];
  static const ociAttrType viewAttrType[VIEW_ATTR_COUNT];
  static const AttrId funcprocAttrId[FUNCPROC_ATTR_COUNT];
  static const ociAttrType funcprocAttrType[FUNCPROC_ATTR_COUNT];
  static const AttrId pkgAttrId[PKG_ATTR_COUNT];
  static const ociAttrType pkgAttrType[PKG_ATTR_COUNT];
  static const AttrId typAttrId[TYP_ATTR_COUNT];
  static const ociAttrType typAttrType[TYP_ATTR_COUNT];
  static const AttrId typeattrAttrId[TYPEATTR_ATTR_COUNT];
  static const ociAttrType typeattrAttrType[TYPEATTR_ATTR_COUNT];
  static const AttrId typmethdAttrId[TYPEMTHD_ATTR_COUNT];
  static const ociAttrType typemthdAttrType[TYPEMTHD_ATTR_COUNT];
  static const AttrId collAttrId[COLL_ATTR_COUNT];
  static const ociAttrType collAttrType[COLL_ATTR_COUNT];
  static const AttrId synAttrId[SYN_ATTR_COUNT];
  static const ociAttrType synAttrType[SYN_ATTR_COUNT];
  static const AttrId seqAttrId[SEQ_ATTR_COUNT];
  static const ociAttrType seqAttrType[SEQ_ATTR_COUNT];
  static const AttrId colAttrId[COL_ATTR_COUNT];
  static const ociAttrType colAttrType[COL_ATTR_COUNT];
  static const AttrId argtargtresAttrId[ARG_TYPARG_TYPRES_ATTR_COUNT];
  static const ociAttrType argtargtresAttrType[
                   ARG_TYPARG_TYPRES_ATTR_COUNT];
  static const AttrId schemaAttrId[SCHEMA_ATTR_COUNT];
  static const ociAttrType schemaAttrType[SCHEMA_ATTR_COUNT];
  static const AttrId databaseAttrId[DATABASE_ATTR_COUNT];
  static const ociAttrType databaseAttrType[DATABASE_ATTR_COUNT];

  static const ColumnAttrId columnAttrId[COLUMN_ATTR_COUNT];
  Ptr<MetaDataImpl> metaDataImplPtr;
  const OCIParam* paramhp;
  const ConnectionImpl* sesn;
  const AttrId* attrIdArray;
  const ociAttrType* attrTypeArray;
  AttrCount attrCount;
  
  MetaData(const Connection *sessp, const OCCI_STD_NAMESPACE::string& objName,
  ParamType prmtyp ) ;
  MetaData(const Connection *sessp, const UString& objName,
  ParamType prmtyp ) ;
  MetaData(const Connection *sessp, 
  const RefAny& ref) ;
  MetaData(const Connection *sessp, MetaDataImpl *implPtr,
  OCIParam* parm) ;
  MetaData(const Connection *sessp, MetaDataImpl *implPtr,
  OCIParam *parm, ub1 parmTyp) ;
  ub1 getParamType(OCIParam* prm) const ;
  const AttrId* getAttrIdArrayAddr(ub1 prmTyp) const;
  const ociAttrType* getAttrTypeArrayAddr(ub1 prmTyp) const;
  AttrCount getAttrCount(ub1 prmTyp) const; 
  Type getType(ociAttrType typ) const;
  bool isListTypeAttribute(AttrId attrid,ub1 ptyp) const;
  boolean isInvalidAttrId(AttrId attrid,sb4* pos, 
            boolean* isTypeSpecificAttrPtr) const;
  boolean isInvalidColumnAttrId(ColumnAttrId colAttrId) const;
  ociAttrType getValidAttrType(sb4 index, boolean isTypeSpecificAttr)
  const;

  int getListType (const OCIParam *plist) const;
  unsigned int getLowerBound(int ltype) const;
  unsigned int getUpperBound(unsigned int ltype, 
    unsigned int paramnum) const;
  friend class ConnectionImpl;
  friend class ResultSetImpl;

};

//return codes for user callbacks
enum
{
  OCCI_SUCCESS = OCI_SUCCESS,
  FO_RETRY = OCI_FO_RETRY
};


class Connection
{
   public :

      // specifies the type of proxy to be created,
      // used for future enhancements
      enum ProxyType
      {PROXY_DEFAULT
      };

      enum FailOverType
      {
        FO_NONE = OCI_FO_NONE,
        FO_SESSION = OCI_FO_SESSION,
        FO_SELECT = OCI_FO_SELECT
      }; 

      enum FailOverEventType
      {
        FO_BEGIN = OCI_FO_BEGIN,
        FO_END = OCI_FO_END,
        FO_ABORT = OCI_FO_ABORT,
        FO_REAUTH = OCI_FO_REAUTH,
        FO_ERROR = OCI_FO_ERROR
      };

      enum Purity
      {
        DEFAULT = OCI_ATTR_PURITY_DEFAULT,
        NEW = OCI_ATTR_PURITY_NEW,
        SELF = OCI_ATTR_PURITY_SELF
      };

      virtual ~Connection() { }
      virtual Statement* createStatement(
        const OCCI_STD_NAMESPACE::string  &sql = "") 
         =0; 
      virtual void terminateStatement(Statement *statement) =0;
      virtual void commit()  =0; 
      virtual void rollback()  =0;
      virtual MetaData getMetaData(const OCCI_STD_NAMESPACE::string &object, 
        MetaData::ParamType prmtyp 
        = MetaData::PTYPE_UNK) const 
         =0; 
      virtual MetaData getMetaData(const RefAny &ref) const
         =0; 
      virtual OCCI_STD_NAMESPACE::string getClientCharSet() const 
         =0; 
      virtual OCCI_STD_NAMESPACE::string getClientNCHARCharSet() const 
         =0; 
      virtual void changePassword(const OCCI_STD_NAMESPACE::string &user,
        const OCCI_STD_NAMESPACE::string &oldPassword,
        const OCCI_STD_NAMESPACE::string &newPassword) 
         =0; 
      virtual void flushCache()  =0; 

      virtual OCIServer*  getOCIServer() const =0;
      virtual OCISvcCtx*  getOCIServiceContext() const =0;
      virtual OCISession* getOCISession() const =0;

      //new interfaces

      virtual Statement* createStatement(const UString &sql) = 0;
      virtual MetaData getMetaData(const UString &object, 
        MetaData::ParamType prmtyp 
        = MetaData::PTYPE_UNK) const 
         =0; 
      virtual UString getClientCharSetUString() const 
         =0; 
      virtual UString getClientNCHARCharSetUString() const 
         =0; 
      virtual void changePassword(const UString &user,
        const UString &oldPassword,
        const UString &newPassword) 
         =0; 
      virtual OCCI_STD_NAMESPACE::string getTag() const =0;
      virtual void setStmtCacheSize(unsigned int cacheSize) = 0;
      virtual unsigned int getStmtCacheSize() const =0;
      virtual Statement* createStatement(const OCCI_STD_NAMESPACE::string &sql,
                    const OCCI_STD_NAMESPACE::string &tag) = 0;
      virtual void terminateStatement(Statement* stmt, 
                    const OCCI_STD_NAMESPACE::string &tag) = 0;
      virtual bool isCached(const OCCI_STD_NAMESPACE::string &sql,
                    const OCCI_STD_NAMESPACE::string &tag = "") = 0;
      virtual void registerSubscriptions(
       const OCCI_STD_NAMESPACE::vector<aq::Subscription>& sub) =0;
      virtual void unregisterSubscription(const aq::Subscription& sub) =0;
      virtual void postToSubscriptions(
       const OCCI_STD_NAMESPACE::vector<aq::Subscription>& sub) =0;
      virtual Statement* createStatement(const UString &sql,
                    const UString &tag) = 0;
      virtual void terminateStatement(Statement* stmt, 
                    const UString &tag) = 0;
      virtual bool isCached(const UString &sql,
                    const UString &tag) = 0;
      virtual void setTAFNotify(
            int (*notifyFn)(Environment *env, Connection *conn, void *ctx,
                      FailOverType foType, FailOverEventType foEvent),
         void *ctx) = 0;
      virtual Bytes getLTXID() const = 0;
      virtual OCCI_STD_NAMESPACE::string getServerVersion() const =0;
      virtual UString getServerVersionUString() const =0;
      virtual void cancel() =0;
      virtual OCCI_STD_NAMESPACE::string getServerRelease2() const =0;
};

class StatelessConnectionPool
{
  public :

    enum PoolType
    {
      HETEROGENEOUS = OCI_DEFAULT,
      HOMOGENEOUS = OCI_SPC_HOMOGENEOUS,
      NO_RLB = OCI_SPC_NO_RLB,
      USES_EXT_AUTH = 16
    };

    enum BusyOption
    {
       WAIT = OCI_SPOOL_ATTRVAL_WAIT,
       NOWAIT = OCI_SPOOL_ATTRVAL_NOWAIT,
       FORCEGET = OCI_SPOOL_ATTRVAL_FORCEGET
    };

    enum DestroyMode
    {
       DEFAULT = OCI_DEFAULT,
       SPD_FORCE = OCI_SPD_FORCE
    };

    virtual ~StatelessConnectionPool() {}
    virtual unsigned int getBusyConnections() const =0;
    virtual unsigned int getOpenConnections() const =0;
    virtual unsigned int getMinConnections() const =0;
    virtual unsigned int getMaxConnections() const =0;
    virtual unsigned int getIncrConnections() const =0;
    virtual OCCI_STD_NAMESPACE::string getPoolName() const =0;
    virtual unsigned int getTimeOut() const =0;
    virtual void setBusyOption(BusyOption busyOption) =0;
    virtual BusyOption getBusyOption() const =0;
    virtual void setTimeOut(unsigned int connTimeOut =0) =0;
    virtual void setPoolSize(unsigned int maxConn =1,
      unsigned int minConn =0, unsigned int incrConn =1) =0;
    virtual Connection* getConnection( 
      const OCCI_STD_NAMESPACE::string &tag ="") =0;
    virtual Connection* getConnection(
      const OCCI_STD_NAMESPACE::string &userName,
      const OCCI_STD_NAMESPACE::string &password,
      const OCCI_STD_NAMESPACE::string &tag = "") =0;
    virtual Connection* getAnyTaggedConnection(
      const OCCI_STD_NAMESPACE::string &tag = "") =0;
    virtual Connection* getAnyTaggedConnection(
      const OCCI_STD_NAMESPACE::string &userName,
      const OCCI_STD_NAMESPACE::string &Password,
      const OCCI_STD_NAMESPACE::string &tag = "") =0;
    virtual Connection* getProxyConnection(
      const OCCI_STD_NAMESPACE::string &name,
      OCCI_STD_NAMESPACE::string roles[], unsigned int numRoles,
      const OCCI_STD_NAMESPACE::string &tag = "",
      Connection::ProxyType proxyType = Connection::PROXY_DEFAULT) =0;
    virtual Connection* getProxyConnection(
      const OCCI_STD_NAMESPACE::string &name,
      const OCCI_STD_NAMESPACE::string &tag = "" ,
      Connection::ProxyType proxyType = Connection::PROXY_DEFAULT) =0;
    virtual Connection* getAnyTaggedProxyConnection(
      const OCCI_STD_NAMESPACE::string &name,
      OCCI_STD_NAMESPACE::string roles[], unsigned int numRoles,
      const OCCI_STD_NAMESPACE::string &tag = "",
      Connection::ProxyType proxyType = Connection::PROXY_DEFAULT) =0;
    virtual Connection* getAnyTaggedProxyConnection(
      const OCCI_STD_NAMESPACE::string &name,
      const OCCI_STD_NAMESPACE::string &tag="",
      Connection::ProxyType proxyType = Connection::PROXY_DEFAULT ) =0;
    virtual void releaseConnection (Connection *connection,
      const OCCI_STD_NAMESPACE::string &tag = "") =0;
    virtual void terminateConnection (Connection *connection) =0;
    virtual void setStmtCacheSize(unsigned int cacheSize) =0;
    virtual unsigned int getStmtCacheSize() const =0;

    virtual Connection* getConnection(const UString &tag)=0;
    virtual Connection* getConnection(const UString &userName,
      const UString &password,
      const UString &tag)=0;
    virtual Connection* getAnyTaggedConnection(const UString &tag)=0;
    virtual Connection* getAnyTaggedConnection( const UString &userName,
      const UString &Password, const UString &tag)=0 ;
    virtual Connection* getProxyConnection(const UString &name,
      OCCI_STD_NAMESPACE::string roles[], unsigned int numRoles,
      const UString &tag,
      Connection::ProxyType proxyType = Connection::PROXY_DEFAULT)=0;
    virtual Connection* getProxyConnection(const UString &name,
      const UString &tag, Connection::ProxyType
      proxyType = Connection::PROXY_DEFAULT)=0;
    virtual Connection* getAnyTaggedProxyConnection(const UString &name,
      OCCI_STD_NAMESPACE::string roles[], unsigned int numRoles,
      const UString &tag,
      Connection::ProxyType proxyType = Connection::PROXY_DEFAULT)=0;
    virtual Connection* getAnyTaggedProxyConnection(const UString &name,
      const UString &tag,
      Connection::ProxyType proxyType = Connection::PROXY_DEFAULT )=0;
    virtual void releaseConnection(Connection *connection,
      const UString  &tag)=0;


    virtual Connection* getConnection( 
     const OCCI_STD_NAMESPACE::string &connectionClass,
     const Connection::Purity purity,
     const OCCI_STD_NAMESPACE::string &tag = "") =0;

    virtual Connection* getConnection(
      const OCCI_STD_NAMESPACE::string &userName,
      const OCCI_STD_NAMESPACE::string &password,
      const OCCI_STD_NAMESPACE::string &connectionClass,
      const Connection::Purity purity,
      const OCCI_STD_NAMESPACE::string &tag = "") =0;

    virtual Connection* getAnyTaggedConnection(
      const OCCI_STD_NAMESPACE::string &connectionClass,
      const Connection::Purity purity,
      const OCCI_STD_NAMESPACE::string &tag = "") =0;

    virtual Connection* getAnyTaggedConnection(
      const OCCI_STD_NAMESPACE::string &userName,
      const OCCI_STD_NAMESPACE::string &Password,
      const OCCI_STD_NAMESPACE::string &connectionClass,
      const Connection::Purity purity,
      const OCCI_STD_NAMESPACE::string &tag = "")= 0;

    virtual Connection* getProxyConnection(
      const OCCI_STD_NAMESPACE::string &name,
      OCCI_STD_NAMESPACE::string roles[], unsigned int numRoles,
      const OCCI_STD_NAMESPACE::string &connectionClass,
      const Connection::Purity purity, 
      const OCCI_STD_NAMESPACE::string &tag = "",
      Connection::ProxyType proxyType  = Connection::PROXY_DEFAULT) = 0;

    virtual Connection* getProxyConnection(
      const OCCI_STD_NAMESPACE::string &name,
      const OCCI_STD_NAMESPACE::string &connectionClass,
      const Connection::Purity purity, 
      const OCCI_STD_NAMESPACE::string &tag = "",
      Connection::ProxyType proxyType = Connection::PROXY_DEFAULT) = 0;

    virtual Connection* getAnyTaggedProxyConnection(
      const OCCI_STD_NAMESPACE::string &name,
      OCCI_STD_NAMESPACE::string roles[], unsigned int numRoles,
      const OCCI_STD_NAMESPACE::string &connectionClass,
      const Connection::Purity purity,
      const OCCI_STD_NAMESPACE::string &tag = "",
      Connection::ProxyType proxyType = Connection::PROXY_DEFAULT) = 0;


    virtual Connection* getAnyTaggedProxyConnection(
      const OCCI_STD_NAMESPACE::string &name,
      const OCCI_STD_NAMESPACE::string &connectionClass,
      const Connection::Purity purity,
      const OCCI_STD_NAMESPACE::string &tag = "",
      Connection::ProxyType proxyType = Connection::PROXY_DEFAULT) = 0;


    virtual Connection* getConnection(
      const UString &connectionClass,
      const Connection::Purity purity,
      const UString &tag) = 0;



    virtual Connection* getConnection(const UString &userName,
      const UString &password,
      const UString &connectionClass,
      const Connection::Purity purity,
      const UString &tag) = 0;

    virtual Connection* getAnyTaggedConnection(
      const UString &connectionClass,
      const Connection::Purity purity,
      const UString &tag) =0;



    virtual Connection* getAnyTaggedConnection( const UString &userName,
      const UString &Password, 
      const UString &connectionClass,
      const Connection::Purity purity,
      const UString &tag) =0;

    virtual Connection* getProxyConnection(const UString &name,
      OCCI_STD_NAMESPACE::string roles[], unsigned int numRoles,
      const UString &connectionClass,
      const Connection::Purity purity,
      const UString &tag, Connection::ProxyType proxyType) =0;

   virtual Connection* getProxyConnection(const UString &name,
      const UString &connectionClass,
      const Connection::Purity purity,
      const UString &tag, Connection::ProxyType proxyType) = 0;


    virtual Connection* getAnyTaggedProxyConnection(const UString &name,
      OCCI_STD_NAMESPACE::string roles[], unsigned int numRoles,
      const UString &connectionClass,
      const Connection::Purity purity,
      const UString &tag,
      Connection::ProxyType proxyType) = 0; 

    virtual Connection* getAnyTaggedProxyConnection(const UString &name,
      const UString &connectionClass,
      const Connection::Purity purity,
      const UString &tag,
      Connection::ProxyType proxyType ) =0;

};


class ConnectionPool
{
  public :

    virtual ~ConnectionPool() {}
    virtual unsigned int getBusyConnections() const 
       =0; 
    virtual unsigned int getOpenConnections() const 
       =0; 
    virtual unsigned int getMinConnections() const 
       =0; 
    virtual unsigned int getMaxConnections() const 
       =0; 
    virtual unsigned int getIncrConnections() const 
       =0; 
    virtual OCCI_STD_NAMESPACE::string getPoolName() const 
       =0; 
    virtual unsigned int getTimeOut() const 
       =0; 
    virtual void setErrorOnBusy() 
       =0; 
    virtual void setTimeOut(unsigned int connTimeOut =0) 
       =0; 
    virtual void setPoolSize(unsigned int minConn =0,
      unsigned int maxConn =1, unsigned int incrConn =1) 
       =0; 
    virtual Connection* createConnection(
      const OCCI_STD_NAMESPACE::string &userName,
      const OCCI_STD_NAMESPACE::string &password)  =0; 

    virtual Connection* createProxyConnection(
      const OCCI_STD_NAMESPACE::string &name,
      OCCI_STD_NAMESPACE::string roles[], unsigned int numRoles,
      Connection::ProxyType proxyType = 
      Connection::PROXY_DEFAULT)  =0; 

    virtual Connection* createProxyConnection(
      const OCCI_STD_NAMESPACE::string &name,
      Connection::ProxyType proxyType =
      Connection::PROXY_DEFAULT)  =0;

    virtual void terminateConnection
      (Connection *connection) =0;

    //new interfaces

    virtual Connection* createConnection(
      const UString &userName,
      const UString &password)  =0; 
    
    virtual Connection* createProxyConnection(const UString &name,
      OCCI_STD_NAMESPACE::string roles[], unsigned int numRoles,
      Connection::ProxyType proxyType = 
      Connection::PROXY_DEFAULT)  =0; 
    
    virtual Connection* createProxyConnection(const UString &name,
      Connection::ProxyType proxyType =
      Connection::PROXY_DEFAULT)  =0;
    
    virtual void setStmtCacheSize(unsigned int cacheSize) =0;
    virtual unsigned int getStmtCacheSize() const =0;

    virtual UString getPoolNameUString() const 
       =0; 
};

class   Environment     
{
 public:
                    // class constants

  enum Mode
  {
    DEFAULT = OCI_DEFAULT,
    OBJECT = OCI_OBJECT,
    NO_USERCALLBACKS = OCI_NO_UCB,
    THREADED_MUTEXED = OCI_THREADED,
    THREADED_UNMUTEXED = OCI_THREADED | OCI_NO_MUTEX,
    EVENTS = OCI_EVENTS,
    USE_LDAP = OCI_USE_LDAP
  };

  virtual ~Environment(){}

                    // public methods

  static Environment * createEnvironment(
                              Mode mode = DEFAULT,
                              void *ctxp = 0,
                              void *(*malocfp)(void *ctxp, size_t size) = 0,
                              void *(*ralocfp)(void *ctxp, void *memptr,
                                               size_t newsize) = 0,
                              void (*mfreefp)(void *ctxp, void *memptr) = 0);
  
  static Environment * createEnvironment(
                              const OCCI_STD_NAMESPACE::string &charset,
                              const OCCI_STD_NAMESPACE::string &ncharset,
                              Mode mode = DEFAULT,
                              void *ctxp = 0,
                              void *(*malocfp)(void *ctxp, size_t size) = 0,
                              void *(*ralocfp)(void *ctxp, void *memptr,
                                               size_t newsize) = 0,
                              void (*mfreefp)(void *ctxp, void *memptr) = 0);

  static void terminateEnvironment(Environment *env);
  
  static Environment* getXAEnvironment(const 
                           OCCI_STD_NAMESPACE::string& dbname);

  static void releaseXAEnvironment(Environment *env);
  
  static void getClientVersion( int &featureRelease, int &releaseUpdate, 
                                int &releaseUpdateRevision, int &increment, 
                                int &ext );


  virtual Connection * createConnection(
    const OCCI_STD_NAMESPACE::string &userName,
    const OCCI_STD_NAMESPACE::string &password, 
    const OCCI_STD_NAMESPACE::string &connectString = "") = 0;

  virtual void terminateConnection(Connection *connection) = 0;

  virtual ConnectionPool* createConnectionPool(
    const OCCI_STD_NAMESPACE::string &poolUserName,
    const OCCI_STD_NAMESPACE::string &poolPassword, 
    const OCCI_STD_NAMESPACE::string &connectString ="", 
    unsigned int minConn =0,
    unsigned int maxConn =1, unsigned int incrConn =1) = 0;

  virtual void terminateConnectionPool(ConnectionPool *poolp) = 0;

  virtual unsigned int getCurrentHeapSize() const = 0;

  virtual OCIEnv * getOCIEnvironment() const = 0;

  virtual Map *getMap() const = 0;
 
  virtual void setCacheMaxSize(unsigned int maxSize) = 0; 
  
  virtual unsigned int getCacheMaxSize() const = 0; 
  
  virtual void setCacheOptSize(unsigned int OptSize) = 0; 
  
  virtual unsigned int getCacheOptSize() const = 0; 


  //new interfaces

  virtual Connection * createConnection(const UString &userName,
    const UString &password, const UString &connectString) = 0;
  
  virtual ConnectionPool* createConnectionPool(
    const UString &poolUserName,
    const UString &poolPassword, const UString &connectString, 
    unsigned int minConn =0,
    unsigned int maxConn =1, unsigned int incrConn =1) = 0;

  virtual Connection* getXAConnection(const 
                           OCCI_STD_NAMESPACE::string& dbname) = 0;

  virtual void releaseXAConnection(Connection* conn) =0;
  
  virtual StatelessConnectionPool* createStatelessConnectionPool(
    const OCCI_STD_NAMESPACE::string &poolUserName,
    const OCCI_STD_NAMESPACE::string &poolPassword,
    const OCCI_STD_NAMESPACE::string &connectString = "",
    unsigned int maxConn = 1, unsigned int minConn = 0,
    unsigned int incrConn = 1,
    StatelessConnectionPool::PoolType pType 
                                 = StatelessConnectionPool::HETEROGENEOUS) = 0;
  
  virtual StatelessConnectionPool* createStatelessConnectionPool(
    const UString &poolUserName,
    const UString &poolPassword,
    const UString &connectString,
    unsigned int maxConn = 1, unsigned int minConn = 0,
    unsigned int incrConn = 1,
    StatelessConnectionPool::PoolType pType 
                                 = StatelessConnectionPool::HETEROGENEOUS) = 0;

  virtual void terminateStatelessConnectionPool(StatelessConnectionPool *poolp,
  StatelessConnectionPool::DestroyMode mode = StatelessConnectionPool::DEFAULT)
      = 0;
  virtual void setLDAPAuthentication(unsigned int mode) =0;

  virtual unsigned int getLDAPAuthentication() const =0;
  
  virtual void setLDAPLoginNameAndPassword(
     const OCCI_STD_NAMESPACE::string &login,
     const OCCI_STD_NAMESPACE::string &passwd) =0;

  virtual void setLDAPAdminContext(const OCCI_STD_NAMESPACE::string &ctx)=0;

  virtual OCCI_STD_NAMESPACE::string getLDAPAdminContext() const =0;

  virtual void setLDAPHostAndPort(const OCCI_STD_NAMESPACE::string &host,
     unsigned int port) =0;

  virtual OCCI_STD_NAMESPACE::string getLDAPHost() const =0;

  virtual unsigned int getLDAPPort() const =0;  
 
  virtual void registerSubscriptions(
     const OCCI_STD_NAMESPACE::vector<aq::Subscription>& sub) =0;

  virtual void unregisterSubscription(const aq::Subscription& sub) =0; 
 
  virtual void enableSubscription(const aq::Subscription& sub) =0;
  
  virtual void disableSubscription(const aq::Subscription& sub) =0;
  
  virtual bool getCacheSortedFlush() const = 0;

  virtual void setCacheSortedFlush(bool flag) = 0;

  virtual Connection * createConnection(
                   const OCCI_STD_NAMESPACE::string &userName,
                   const OCCI_STD_NAMESPACE::string &password,
                   const OCCI_STD_NAMESPACE::string &connectString,
                   const OCCI_STD_NAMESPACE::string &connectionClass,
                   const Connection::Purity purity) = 0;

  virtual Connection * createConnection(const UString &userName, 
                const  UString &password, const UString &connectString,
                const UString &connectionclass, 
                const Connection::Purity purity) =0;

  virtual OCCI_STD_NAMESPACE::string getNLSLanguage() const =0;
  virtual OCCI_STD_NAMESPACE::string getNLSTerritory() const =0;
  virtual void setNLSLanguage(const OCCI_STD_NAMESPACE::string &lang) =0;
  virtual void setNLSTerritory(const OCCI_STD_NAMESPACE::string &Terr) =0;

  private:

};



class Map
{
 public:

  virtual ~Map(){}
  virtual void put(const OCCI_STD_NAMESPACE::string&, void *(*)(void *), 
                void (*)(void *, void *)) = 0;
  virtual void getReadSQL(
    void *, unsigned int, void *, unsigned int, void **) const = 0;
  virtual void getWriteSQL(
    void *, unsigned int, void *, unsigned int, void **) const = 0;
  virtual void put(const OCCI_STD_NAMESPACE::string&, 
                const OCCI_STD_NAMESPACE::string&, void *(*)(void *), 
                void (*)(void *, void *)) = 0;
  virtual void putUTF16(const OCCI_STD_NAMESPACE::string&, 
                const OCCI_STD_NAMESPACE::string&, void *(*)(void *), 
                void (*)(void *, void *)) = 0;

 private:
};


   
class SQLException : public OCCI_STD_NAMESPACE::exception
{
 public:

  virtual int getErrorCode() const;
  
  virtual OCCI_STD_NAMESPACE::string getMessage() const;

  const char *what() const throw();

  virtual void setErrorCtx(void *ctx);
  
  SQLException();

  SQLException(const SQLException &e);

  void operator=(const SQLException &other);

  virtual ~SQLException() throw();

  virtual int getXAErrorCode(const OCCI_STD_NAMESPACE::string& dbname) const;
  
  virtual UString getUStringMessage() const;

  virtual OCCI_STD_NAMESPACE::string getNLSMessage(Environment *env) const;
  
  virtual UString getNLSUStringMessage(Environment *env) const;

  virtual bool isRecoverable() const;  

 protected:

  Ptr<SQLExceptionImpl> ptr_;
  SQLException(SQLExceptionImpl *ptr);

  friend SQLException SQLExceptionCreate(int errorCode);
  friend SQLException SQLExceptionCreate(void *handle,
  int handleType);
  friend class BatchSQLException;
};

class BatchSQLException : public SQLException
{
  public:
    virtual ~BatchSQLException() throw();

    unsigned int getFailedRowCount() const;
    unsigned int getRowNum( unsigned int index ) const;
    SQLException getException ( unsigned int index ) const;
 
  private:
    BatchSQLException();

    BatchSQLException(SQLExceptionImpl *ptr); 
    friend BatchSQLException BatchSQLExceptionCreate(void *handle); 
};

class Statement 
{
 public:
                                // class constants

  virtual ~Statement() {}
  
  enum Status                                   
  {
    UNPREPARED,
    PREPARED,
    RESULT_SET_AVAILABLE,
    UPDATE_COUNT_AVAILABLE,
    NEEDS_STREAM_DATA,
    STREAM_DATA_AVAILABLE
  };
  
                                // common methods
  
  virtual void setSQL(const OCCI_STD_NAMESPACE::string &sql) = 0; 

  virtual OCCI_STD_NAMESPACE::string getSQL() const = 0;

  virtual Status execute(const OCCI_STD_NAMESPACE::string &sql = "") = 0;
  
  virtual ResultSet * getResultSet() = 0;
  
  virtual unsigned int getUpdateCount() const = 0; 
  
  virtual ResultSet * executeQuery(
    const OCCI_STD_NAMESPACE::string &sql = "") = 0;
  
  virtual unsigned int executeUpdate(
    const OCCI_STD_NAMESPACE::string &sql = "") = 0; 
  
  virtual Status status() const = 0;  
                                        
  virtual void closeResultSet(ResultSet *resultSet) = 0;
  
  virtual void setPrefetchRowCount(unsigned int rowCount) = 0; 
  
  virtual void setPrefetchMemorySize(unsigned int bytes) = 0;
  
  virtual void setAutoCommit(bool autoCommit) = 0;
  
  virtual bool getAutoCommit() const = 0;
  
  virtual OCIStmt * getOCIStatement() const = 0;


                    // methods for prepared statements with IN
                    // parameters  

  virtual void setMaxParamSize(unsigned int paramIndex,unsigned int maxSize)=0;
  
  virtual unsigned int getMaxParamSize(unsigned int paramIndex) const = 0;

  virtual void setNull(unsigned int paramIndex, Type type) = 0;
 
  virtual void setInt(unsigned int paramIndex, int x) = 0; 
  
  virtual void setUInt(unsigned int paramIndex, unsigned int x) = 0; 
  
  virtual void setFloat(unsigned int paramIndex, float x) = 0; 

  virtual void setDouble(unsigned int paramIndex, double x) = 0; 

  virtual void setNumber(unsigned int paramIndex, const Number &x) = 0; 
  
  virtual void setString(unsigned int paramIndex, 
    const OCCI_STD_NAMESPACE::string &x) = 0; 

  virtual void setBytes(unsigned int paramIndex, const Bytes &x) = 0; 

  virtual void setDate(unsigned int paramIndex, const Date &x) = 0; 
  
  virtual void setTimestamp(unsigned int paramIndex, const Timestamp &x) = 0; 

  virtual void setBlob(unsigned int paramIndex, const Blob &x) = 0;

  virtual void setClob(unsigned int paramIndex, const Clob &x) = 0;

  virtual void setBfile(unsigned int paramIndex, const Bfile &x) = 0;

  virtual void setIntervalYM(unsigned int paramIndex, const IntervalYM &x) = 0;
 
  virtual void setIntervalDS(unsigned int paramIndex, const IntervalDS &x) = 0;
 
  virtual void setRowid(unsigned int paramIndex, const Bytes &x) = 0; 
  
  virtual void setRef(unsigned int paramIndex, const RefAny &x) = 0; 

  virtual void setObject(unsigned int paramIndex, PObject * x) = 0; 
  
  virtual void setDataBuffer(unsigned int paramIndex, void *buffer, 
                             Type type,
                             sb4 size, ub2 *length, sb2 *ind = NULL,
                             ub2 *rc = NULL) = 0; 
  
  virtual void setDataBuffer(unsigned int paramIndex, void *buffer,
                             Type type,
                             sb4 size, ub4 *length, sb2 *ind = NULL,
                             ub2 *rc = NULL) = 0;

  virtual void setDataBufferArray(unsigned int paramIndex, void *buffer, 
                                  Type type,
                                  ub4 arraySize, ub4 *arrayLength,
                                  sb4 elementSize,
                                  ub2 *elementLength, sb2 *ind = NULL,
                                  ub2 *rc = NULL) = 0;
  
  virtual void setDataBufferArray(unsigned int paramIndex, void *buffer,
                                  Type type,
                                  ub4 arraySize, ub4 *arrayLength,
                                  sb4 elementSize,
                                  ub4 *elementLength, sb2 *ind = NULL,
                                  ub2 *rc = NULL) = 0;
  virtual void setCharSet(unsigned int paramIndex, 
  const OCCI_STD_NAMESPACE::string & charSet) = 0; 
  
  virtual OCCI_STD_NAMESPACE::string getCharSet(unsigned int paramIndex) 
  const = 0; 
  
  virtual void setDatabaseNCHARParam(
    unsigned int paramIndex, bool isNCHAR) = 0;
                                         
  virtual bool getDatabaseNCHARParam(unsigned int paramIndex) const = 0;       

  virtual void closeStream(Stream *stream) =0;

  virtual Stream * getStream(unsigned int paramIndex) = 0; 
  
  virtual unsigned int getCurrentStreamParam() const = 0; 
  
  virtual unsigned int getCurrentStreamIteration() const = 0;

  virtual void setBinaryStreamMode(unsigned int colIndex, 
    unsigned int size) =0;

  virtual void setCharacterStreamMode(unsigned int colIndex, 
    unsigned int size) =0;
  
  virtual void setMaxIterations(unsigned int maxIterations) = 0;
  
  virtual unsigned int getMaxIterations() const = 0; 

  virtual void addIteration() = 0; 
  
  virtual unsigned int getCurrentIteration() const = 0; 

  virtual Status executeArrayUpdate(unsigned int arrayLength) = 0;
  

                    // methods for Callable Statements

  virtual void registerOutParam(unsigned int paramIndex, Type type, 
  unsigned int maxSize=0, const OCCI_STD_NAMESPACE::string &sqltype="") = 0;
  
  virtual bool isNull(unsigned int paramIndex) const = 0;

  virtual bool isTruncated(unsigned int paramIndex) const
   =0;

  
  virtual void setErrorOnNull(unsigned int paramIndex, 
    bool causeException) = 0;
                               
  virtual void setErrorOnTruncate(unsigned int paramIndex,
  bool causeException)  = 0;

  virtual int preTruncationLength(unsigned int paramIndex) const
   =0;


  virtual int getInt(unsigned int paramIndex)  = 0; 
  
  virtual unsigned int getUInt(unsigned int paramIndex)  = 0; 
  
  virtual float getFloat(unsigned int paramIndex)  = 0; 

  virtual double getDouble(unsigned int paramIndex)  = 0; 
  
  virtual Number getNumber(unsigned int paramIndex)  = 0; 
  
  virtual OCCI_STD_NAMESPACE::string getString(unsigned int paramIndex)  = 0; 
  
  virtual Bytes getBytes(unsigned int paramIndex)  = 0; 

  virtual Date getDate(unsigned int paramIndex)  = 0; 
  
  virtual Timestamp getTimestamp(unsigned int paramIndex)  = 0; 

  virtual Bytes getRowid(unsigned int paramIndex)  = 0; 
  
  virtual PObject * getObject(unsigned int paramIndex)  = 0; 
  
  virtual Blob getBlob(unsigned int paramIndex)  = 0; 
  
  virtual Clob getClob(unsigned int paramIndex)  = 0; 
  
  virtual Bfile getBfile(unsigned int paramIndex)  = 0; 

  virtual IntervalYM getIntervalYM(unsigned int paramIndex)  = 0; 
 
  virtual IntervalDS getIntervalDS(unsigned int paramIndex)  = 0; 

  virtual RefAny getRef(unsigned int paramIndex)  = 0;  

  virtual ResultSet * getCursor(unsigned int paramIndex)   = 0;

  virtual Connection* getConnection() const =0; 

  //new interfaces

  virtual void setRef(unsigned int paramIndex, const RefAny &x, 
                      const OCCI_STD_NAMESPACE::string &typName, 
                      const OCCI_STD_NAMESPACE::string &schName = "") = 0; 
  
  virtual void setSQLUString(const UString &sql) = 0;
  
  virtual UString getSQLUString() const = 0;
  
  virtual Status execute(const UString &sql) = 0;
                                
  virtual ResultSet * executeQuery(
    const UString &sql) = 0;
  
  virtual unsigned int executeUpdate(
    const UString &sql) = 0; 
  
  virtual void setBFloat(unsigned int paramIndex, const BFloat &fval) = 0;
  
  virtual void setBDouble(unsigned int paramIndex, const BDouble &dval) = 0;
  
  virtual void setUString(unsigned int paramIndex,
    const UString &x) = 0;
  
  virtual void setCharSetUString(unsigned int paramIndex, 
  const UString & charSet) = 0; 
  
  virtual UString getCharSetUString(unsigned int paramIndex) 
  const = 0; 
  
  virtual void registerOutParam(unsigned int paramIndex, Type type, 
  unsigned int maxSize, const OCCI_STD_NAMESPACE::string &typName,
  const OCCI_STD_NAMESPACE::string &schName) = 0;
  
  virtual void registerOutParam(unsigned int paramIndex, Type type, 
  unsigned int maxSize, const UString &typName, 
  const UString &schName) = 0;
  
  virtual BFloat getBFloat(unsigned int paramIndex)  = 0;
  
  virtual BDouble getBDouble(unsigned int paramIndex)  = 0;

  virtual UString getUString(unsigned int paramIndex)  = 0; 
  
  virtual void disableCaching() =0;
  
  virtual void setRef(unsigned int paramIndex, const RefAny &x, 
                      const UString &typName, 
                      const UString &schName) = 0; 

  virtual void setBinaryStreamMode(unsigned int colIndex, 
    unsigned int size, bool INArg) =0;

  virtual void setCharacterStreamMode(unsigned int colIndex, 
    unsigned int size, bool INArg) =0;

  virtual void setNull(unsigned int paramIndex, Type type,
               const OCCI_STD_NAMESPACE::string &typeName,
               const OCCI_STD_NAMESPACE::string &schemaName = "") = 0;

  virtual void setNull(unsigned int paramIndex, Type type,
               UString &typeName, UString &schemaName) = 0;

  virtual void setBatchErrorMode( bool batchErrorMode ) =0;

  virtual bool getBatchErrorMode( ) const =0;

  virtual void setRowCountsOption(bool rowCountsOption) = 0 ;

  virtual bool getRowCountsOption() const = 0;

  virtual OCCI_STD_NAMESPACE::vector<oraub8> getDMLRowCounts() const = 0; 
 
  virtual oraub8 getUb8RowCount() const = 0; 

};



class ResultSet 
{
 public:
                                // class constants
  
  enum Status                                   
  {
    END_OF_FETCH = 0,
    DATA_AVAILABLE,
    STREAM_DATA_AVAILABLE
  };
  virtual ~ResultSet(){}
 
                                // public methods

  virtual Status next(unsigned int numRows = 1) = 0;
  
  virtual Status status() const = 0;            
  
  virtual unsigned int getNumArrayRows()  const = 0;
  
  virtual void cancel() = 0;

  virtual void setMaxColumnSize(unsigned int colIndex, unsigned int max) = 0;
  
  virtual unsigned int getMaxColumnSize(unsigned int colIndex) const = 0;
  
  virtual bool isNull(unsigned int colIndex) const = 0;
  
  virtual bool isTruncated(unsigned int paramIndex) const
   =0;

  virtual void setErrorOnNull(unsigned int colIndex, bool causeException) = 0;
  virtual void setErrorOnTruncate(unsigned int paramIndex,
  bool causeException)   =0;

  virtual int preTruncationLength(unsigned int paramIndex) const
   =0;

  virtual int getInt(unsigned int colIndex)   = 0; 
  
  virtual unsigned int getUInt(unsigned int colIndex)   = 0; 
  
  virtual float getFloat(unsigned int colIndex)  = 0; 

  virtual double getDouble(unsigned int colIndex)  = 0; 

  virtual Number getNumber(unsigned int colIndex)  = 0; 
  
  virtual OCCI_STD_NAMESPACE::string getString(unsigned int colIndex)  = 0; 
  
  virtual Bytes getBytes(unsigned int colIndex)  = 0; 

  virtual Date getDate(unsigned int colIndex)  = 0; 
  
  virtual Timestamp getTimestamp(unsigned int colIndex)  = 0; 
  
  virtual Bytes getRowid(unsigned int colIndex)  = 0; 
  
  virtual PObject * getObject(unsigned int colIndex)  = 0; 
  
  virtual Blob getBlob(unsigned int colIndex)  = 0; 
  
  virtual Clob getClob(unsigned int colIndex)  =0; 
  
  virtual Bfile getBfile(unsigned int colIndex)  = 0; 

  virtual  IntervalYM getIntervalYM(unsigned int colIndex)  =0;
   
  virtual  IntervalDS getIntervalDS(unsigned int colIndex)  =0;
   
  virtual RefAny getRef(unsigned int colIndex)  = 0; 

  virtual Bytes getRowPosition() const = 0; 

  virtual ResultSet * getCursor(unsigned int colIndex)  = 0; 
  
  virtual void setDataBuffer(unsigned int colIndex, void *buffer, Type type,
                             sb4 size = 0, ub2 *length = NULL,
                             sb2 *ind = NULL, ub2 *rc = NULL) = 0;
  
  virtual void setDataBuffer(unsigned int colIndex, void *buffer, Type type,
                             sb4 size = 0, ub4 *length = NULL,
                             sb2 *ind = NULL, ub2 *rc = NULL) = 0;

  virtual void setCharSet(unsigned int colIndex, 
  const OCCI_STD_NAMESPACE::string & charSet) = 0; 
  
  virtual OCCI_STD_NAMESPACE::string getCharSet(unsigned int colIndex) 
  const = 0; 
  
  virtual void setBinaryStreamMode(unsigned int colIndex, unsigned int size)
    = 0;

  virtual void setCharacterStreamMode(unsigned int colIndex, unsigned int size)
    = 0;
  
  virtual void setDatabaseNCHARParam(unsigned int paramIndex, 
    bool isNCHAR) = 0;
  
  virtual bool getDatabaseNCHARParam(unsigned int paramIndex) const = 0;       
                                         
  virtual Stream * getStream(unsigned int colIndex)  = 0; 

  virtual void closeStream(Stream *stream) =0;
  
  virtual unsigned int getCurrentStreamColumn() const= 0; 
  
  virtual unsigned int getCurrentStreamRow() const= 0;       
  
  virtual OCCI_STD_NAMESPACE::vector<MetaData> getColumnListMetaData() const 
    = 0;

  virtual Statement* getStatement() const=0;

  //new interfaces

  virtual BFloat getBFloat(unsigned int colIndex)  = 0;
  
  virtual BDouble getBDouble(unsigned int colIndex)  = 0;
  
  virtual UString getUString(unsigned int colIndex)  = 0; 
  
  virtual void setCharSetUString(unsigned int colIndex, 
  const UString & charSet) = 0; 
  
  virtual UString getCharSetUString(unsigned int colIndex) 
  const = 0; 

  virtual void setPrefetchRowCount(unsigned int rowCount) = 0; 
  
  virtual void setPrefetchMemorySize(unsigned int bytes) = 0;
};


class Stream
{
  public : 

    enum Status {READY_FOR_READ, READY_FOR_WRITE, INACTIVE};

    virtual ~Stream(){}
    virtual int readBuffer(char *buffer, unsigned int size)
       =0;
    virtual int readLastBuffer(char *buffer, unsigned int size)
       =0;
    virtual void writeBuffer(char *buffer, unsigned int size)
       =0;
    virtual void writeLastBuffer(char *buffer, unsigned int size)
       =0;
    virtual Status status() const  =0;

};

/*---------------------------------------------------------------------------
                PROTOTYPES USED BY FUNCTION TEMPLATES
 -------------------------------------------------------------------------*/
  void getVectorOfPObjects( ResultSet *rs, unsigned int index,
  OCCI_STD_NAMESPACE::vector<PObject *> &vect) ;
  void getVectorOfOCIRefs(ResultSet  *rs, unsigned int index,
  OCCI_STD_NAMESPACE::vector<void *> &vect) ;
  void getVectorOfPObjects( Statement *rs, unsigned int index,
  OCCI_STD_NAMESPACE::vector<PObject *> &vect) ;
  void getVectorOfOCIRefs(Statement  *rs, unsigned int index,
  OCCI_STD_NAMESPACE::vector<void *> &vect) ;
  void setVectorOfPObjects( Statement *stmt, unsigned int paramIndex,
  const OCCI_STD_NAMESPACE::vector<PObject *> &vect, 
  const OCCI_STD_NAMESPACE::string &sqltype) ;
  void setVectorOfPObjects( Statement *stmt, unsigned int paramIndex,
  const OCCI_STD_NAMESPACE::vector<PObject *> &vect, 
  const OCCI_STD_NAMESPACE::string &schemaName,
  const OCCI_STD_NAMESPACE::string &typeName) ;
  void setVectorOfPObjects( Statement *stmt, unsigned int paramIndex,
  const OCCI_STD_NAMESPACE::vector<PObject *> &vect, 
  const UString &schemaName,
  const UString &typeName) ;
  void setVectorOfOCIRefs(Statement *stmt, unsigned int paramIndex,
  const  OCCI_STD_NAMESPACE::vector<void *> &vect,
  const OCCI_STD_NAMESPACE::vector<OCIInd> &vecind,
  const OCCI_STD_NAMESPACE::string &sqltype) ;
  void setVectorOfOCIRefs(Statement *stmt, unsigned int paramIndex,
  const  OCCI_STD_NAMESPACE::vector<void *> &vect,
  const OCCI_STD_NAMESPACE::vector<OCIInd> &vecind,
  const OCCI_STD_NAMESPACE::string &schemaName,
  const OCCI_STD_NAMESPACE::string &typeName) ;
  void setVectorOfOCIRefs(Statement *stmt, unsigned int paramIndex,
  const  OCCI_STD_NAMESPACE::vector<void *> &vect,
  const OCCI_STD_NAMESPACE::vector<OCIInd> &vecind,
  const UString &schemaName,
  const UString &typeName) ;
  void pinVectorOfOCIRefs(const Connection *conn, 
  OCCI_STD_NAMESPACE::vector<void *> & vecRef,
  OCCI_STD_NAMESPACE::vector<void *> & vecCor,
  OCCI_STD_NAMESPACE::vector<PObject *> &vecPObj,LockOptions &lockOpt );


/*---------------------------------------------------------------------------
                           EXPORT FUNCTIONS
  ---------------------------------------------------------------------------*/

/*------------------------ getVector for objects ---------------------------*/
/*
   NAME
      getVector - overloaded function. Retrieves the attribute in the current
position as a vector of objects

   PARAMETERS
      rs - ResultSet
      vect- reference to vector of objects(OUT parameter).

   DESCRIPTION
     Retrieves the column in the specified position as a vector of RefAny.
   The attribute at the current position should be a collection type (varray or
   nested table). The SQL type of the elements in the collection should be
   compatible with objects.

   RETURNS
     nothing

   NOTES
        compatible SQL types : NTY

        will call getVector(..., vector<PObject*>)
*/
#if defined(WIN32COMMON) || defined(__MVS__)
// and other platforms that do not support
// partial function template specialization

template <class T>
void getVector( ResultSet *rs, unsigned int index,OCCI_STD_NAMESPACE::vector<T>
& vect) 
{
  OCCI_STD_NAMESPACE::vector<PObject *> vec_pobj;
  getVectorOfPObjects(rs, index, vec_pobj);

  vect.clear();
  unsigned int size = vec_pobj.size();
  vect.reserve( size );
  for ( unsigned int i=0; i< size; i++)
    vect.push_back((T)vec_pobj[i]);
}

#else
template <class T>
void getVector( ResultSet *rs, unsigned int index, OCCI_STD_NAMESPACE::vector<T
 *> &vect)
{
  OCCI_STD_NAMESPACE::vector<PObject *> vec_pobj;
  getVectorOfPObjects(rs, index, vec_pobj);

  vect.clear();
  unsigned int size = vec_pobj.size();
  vect.reserve( size );
  for (unsigned int i=0; i< size; i++)
    vect.push_back((T *)vec_pobj[i]);
}
#endif

/*------------------------ getVector for objects ---------------------------*/
/*
   NAME
      getVector - overloaded function. Retrieves the attribute in the current
position as a vector of objects

   PARAMETERS
      stmt - Statement
      vect- reference to vector of objects(OUT parameter).

   DESCRIPTION
     Retrieves the column in the specified position as a vector of RefAny.
   The attribute at the current position should be a collection type (varray or
   nested table). The SQL type of the elements in the collection should be
   compatible with objects.

   RETURNS
     nothing

   NOTES
        compatible SQL types : NTY

        will call getVector(..., vector<PObject*>)
*/
#if defined(WIN32COMMON) || defined(__MVS__)
// and other platforms that do not support
// partial function template specialization

template <class T>
void getVector( Statement *stmt, unsigned int index, 
OCCI_STD_NAMESPACE::vector<T> &vect) 
{
  OCCI_STD_NAMESPACE::vector<PObject *> vec_pobj;
  getVectorOfPObjects(stmt, index, vec_pobj);
  vect.clear();
  unsigned int size = vec_pobj.size();
  vect.reserve( size );
  for (unsigned int i=0; i< size; i++)
    vect.push_back((T)vec_pobj[i]);
}
#else
template <class T>
void getVector( Statement *stmt, unsigned int index,
OCCI_STD_NAMESPACE::vector<T *> &vect)
{
  OCCI_STD_NAMESPACE::vector<PObject *> vec_pobj;
  getVectorOfPObjects(stmt, index, vec_pobj);
  vect.clear();
  unsigned int size = vec_pobj.size();
  vect.reserve( size );
  for (unsigned int i=0; i< size; i++)
    vect.push_back((T *)vec_pobj[i]);
}
#endif

/*------------------------ getVector for Ref<T> ---------------------------*/
/*
   NAME
      getVector - overloaded function. Retrieves the attribute in the current
position as a vector of Ref<T>

   PARAMETERS
      rs - ResultSet
      vect- reference to vector of Ref<T>(OUT parameter).

   DESCRIPTION
     Retrieves the column in the specified position as a vector of Ref<T>.
   The attribute at the current position should be a collection type (varray or
   nested table). The SQL type of the elements in the collection should be
   compatible with Ref<T>.

   RETURNS
     nothing

   NOTES
        compatible SQL types : REF
*/
#if !defined(WIN32COMMON) && !defined(__MVS__)
template <class T>
void getVector( ResultSet *rs, unsigned int index, 
                OCCI_STD_NAMESPACE::vector<Ref<T> > &vect) 
{
  OCCI_STD_NAMESPACE::vector<void *> vec_ref;
  getVectorOfOCIRefs(rs, index, vec_ref);

  const Connection *sess = rs->getStatement()->getConnection();

  vect.clear();
  unsigned int size = vec_ref.size();
  vect.reserve( size );
  for (unsigned int i=0; i< size; i++)
  {
    if (vec_ref[i] == (OCIRef *)0)
       vect.push_back(Ref<T>());     // pushing a default-constructed Ref
     else
       vect.push_back(Ref<T>(sess, (OCIRef *)vec_ref[i], FALSE));
  }
}
#endif

/*------------------------ setVector for PObject*---------------------------*/
/*
   NAME
      SetVector - overloaded function. Binds the attribute in the current
      position with a vector of objects.

   PARAMETERS
      rs - ResultSet
      vect- reference to vector of objects(OUT parameter).

   DESCRIPTION
     Binds the column in the specified position with a vector of signed int .
   The column at the current position should be a collection type (varray or
   nested table). The SQL type of the elements in the collection should be
   compatible with objects .

   RETURNS
     nothing

   NOTES
     compatible SQL types : SQLT_NTY
 
     This will be calling setVector(..., vector<PObject*>,..)

*/
#if defined(WIN32COMMON) || defined(__MVS__)
// and other platforms that do not support
// partial function template specialization

template <class T>
void setVector( Statement *stmt, unsigned int index, 
                const OCCI_STD_NAMESPACE::vector<T> &vect, 
                const OCCI_STD_NAMESPACE::string &sqltype) 
{
  OCCI_STD_NAMESPACE::vector<PObject *> vec_pobj;
  unsigned int size = vect.size();
  vec_pobj.reserve( size );
  
  for (unsigned int i = 0; i < size; i++)
    vec_pobj.push_back((PObject *)vect[i]);

  setVectorOfPObjects(stmt, index, vec_pobj, sqltype);
}

template <class T>
void setVector( Statement *stmt, unsigned int index, const OCCI_STD_NAMESPACE::
vector<T> &vect, const OCCI_STD_NAMESPACE::string &schemaName,
const OCCI_STD_NAMESPACE::string &typeName)
{
  OCCI_STD_NAMESPACE::vector<PObject *> vec_pobj;
  unsigned int size = vect.size();
  vec_pobj.reserve( size );

  for (unsigned int i = 0; i < size; i++)
    vec_pobj.push_back((PObject *)vect[i]);

  setVectorOfPObjects(stmt, index, vec_pobj, schemaName, typeName);
}

template <class T>
void setVector( Statement *stmt, unsigned int index, const OCCI_STD_NAMESPACE::
vector<T> &vect, const UString &schemaName,
const UString &typeName)
{
  OCCI_STD_NAMESPACE::vector<PObject *> vec_pobj;
  unsigned int size = vect.size();
  vec_pobj.reserve( size );

  for (unsigned int i = 0; i < size; i++)
    vec_pobj.push_back((PObject *)vect[i]);

  setVectorOfPObjects(stmt, index, vec_pobj, schemaName, typeName);
}
#else
template <class T>
void setVector( Statement *stmt, unsigned int index, const OCCI_STD_NAMESPACE::
vector<T *> &vect, const OCCI_STD_NAMESPACE::string &sqltype)
{
  OCCI_STD_NAMESPACE::vector<PObject *> vec_pobj;
  unsigned int size = vect.size();
  vec_pobj.reserve( size );

  for (unsigned int i = 0; i < size; i++)
    vec_pobj.push_back((PObject *)vect[i]);

  setVectorOfPObjects(stmt, index, vec_pobj, sqltype);
}

template <class T>
void setVector( Statement *stmt, unsigned int index, const OCCI_STD_NAMESPACE::
vector<T *> &vect, const OCCI_STD_NAMESPACE::string &schemaName,
const OCCI_STD_NAMESPACE::string &typeName)
{
  OCCI_STD_NAMESPACE::vector<PObject *> vec_pobj;
  unsigned int size = vect.size();
  vec_pobj.reserve( size );

  for (unsigned int i = 0; i < size; i++)
    vec_pobj.push_back((PObject *)vect[i]);

  setVectorOfPObjects(stmt, index, vec_pobj, schemaName, typeName);
}

template <class T>
void setVector( Statement *stmt, unsigned int index, const OCCI_STD_NAMESPACE::
vector<T *> &vect, const UString &schemaName,
const UString &typeName)
{
  OCCI_STD_NAMESPACE::vector<PObject *> vec_pobj;
  unsigned int size = vect.size();
  vec_pobj.reserve( size );

  for (unsigned int i = 0; i < size; i++)
    vec_pobj.push_back((PObject *)vect[i]);

  setVectorOfPObjects(stmt, index, vec_pobj, schemaName, typeName);
}
#endif

/*------------------------ setVector for Ref<T>---------------------------*/
/*
   NAME
      setVector - overloaded function. Binds the attribute in the current
      position with a vector of Ref<T>.

   PARAMETERS
      rs - ResultSet
      vect- reference to vector of REF

   DESCRIPTION
     Binds the column in the specified position with a vector of signed int .
   The column at the current position should be a collection type (varray or
   nested table). The SQL type of the elements in the collection should be
   compatible with OCIRef* .

   RETURNS
     nothing

   NOTES
     compatible SQL types : REF 

     This will just call setVector(..., vector<OCIRef*>,..)


*/
#if !defined(WIN32COMMON) && !defined(__MVS__)
template <class T>
void setVector( Statement *stmt, unsigned int index, 
                const OCCI_STD_NAMESPACE::vector<Ref<T> > &vect, 
                const OCCI_STD_NAMESPACE::string &sqltype)
{
  OCCI_STD_NAMESPACE::vector<void *> vec_ref;
  OCCI_STD_NAMESPACE::vector<OCIInd> vec_ind;
  unsigned int size = vect.size();
  vec_ref.reserve( size );
  vec_ind.reserve( size );

  for (unsigned int i = 0; i < size; i++)
  {
    vec_ref.push_back((void *)vect[i].getRef());
    vec_ind.push_back( vect[i].isNull() ? OCI_IND_NULL : OCI_IND_NOTNULL);
  }

  setVectorOfOCIRefs(stmt, index, vec_ref, vec_ind, sqltype);
}

template <class T>
void setVector( Statement *stmt, unsigned int index, 
                const OCCI_STD_NAMESPACE::vector<Ref<T> > &vect, 
                const OCCI_STD_NAMESPACE::string &schemaName,
                const OCCI_STD_NAMESPACE::string &typeName)
{
  OCCI_STD_NAMESPACE::vector<void *> vec_ref;
  OCCI_STD_NAMESPACE::vector<OCIInd> vec_ind;
  unsigned int size = vect.size();
  vec_ref.reserve( size );
  vec_ind.reserve( size );

  for (unsigned int i = 0; i < size; i++)
  {
    vec_ref.push_back((void *)vect[i].getRef());
    vec_ind.push_back( vect[i].isNull() ? OCI_IND_NULL : OCI_IND_NOTNULL);
  }

  setVectorOfOCIRefs(stmt, index, vec_ref, vec_ind, schemaName, typeName);
}

template <class T>
void setVector( Statement *stmt, unsigned int index, 
                const OCCI_STD_NAMESPACE::vector<Ref<T> > &vect, 
                const UString &schemaName,
                const UString &typeName)
{
  OCCI_STD_NAMESPACE::vector<void *> vec_ref;
  OCCI_STD_NAMESPACE::vector<OCIInd> vec_ind;
  unsigned int size = vect.size();
  vec_ref.reserve( size );
  vec_ind.reserve( size );

  for (unsigned int i = 0; i < size; i++)
  {
    vec_ref.push_back((void *)vect[i].getRef());
    vec_ind.push_back( vect[i].isNull() ? OCI_IND_NULL : OCI_IND_NOTNULL);
  }

  setVectorOfOCIRefs(stmt, index, vec_ref, vec_ind, schemaName, typeName);
}
#endif

/*------------------------ getVector for Ref<T> ---------------------------*/
/*
   NAME
      getVector - overloaded function. Retrieves the attribute in the current
position as a vector of Ref<T>

   PARAMETERS
      stmt - Statement
      vect- reference to vector of Ref<T>(OUT parameter).

   DESCRIPTION
     Retrieves the column in the specified position as a vector of Ref<T>.
   The attribute at the current position should be a collection type (varray or
   nested table). The SQL type of the elements in the collection should be
   compatible with Ref<T>.

   RETURNS
     nothing

   NOTES
        compatible SQL types : REF
*/
#if !defined(WIN32COMMON) && !defined(__MVS__)
template <class T>
void getVector( Statement *stmt, unsigned int index, 
                OCCI_STD_NAMESPACE::vector<Ref<T> > &vect) 
{
  OCCI_STD_NAMESPACE::vector<void *> vec_ref;
  getVectorOfOCIRefs(stmt, index, vec_ref);

  const Connection *sess = stmt->getConnection();

  vect.clear();
  unsigned int size = vec_ref.size();
  vect.reserve( size );
  for (unsigned int i=0; i< size; i++)
  {
    if (vec_ref[i] == (OCIRef *)0)
       vect.push_back(Ref <T>());     // pushing a default-constructed Ref
     else
       vect.push_back(Ref<T> (sess, (OCIRef *)vec_ref[i], FALSE));
  }
 
}
#endif

// Platform independent get/setVectorOfRefs method added
// get(set)Vector of Ref<T> and get(set)VectorOfRefs are identical 
// in functionality. 

/*------------------------ getVectorOfRefs for Ref<T> ----------------------*/
/*
   NAME
      getVectorOfRefs - overloaded function. Retrieves the attribute in the
      current position as a vector of Ref<T>

   PARAMETERS
      rs - ResultSet
      vect- reference to vector of Ref<T>(OUT parameter).

   DESCRIPTION
     Retrieves the column in the specified position as a vector of Ref<T>.
   The attribute at the current position should be a collection type (varray or
   nested table). The SQL type of the elements in the collection should be
   compatible with Ref<T>.

   RETURNS
     nothing

   NOTES
        compatible SQL types : REF
*/

template <class T>
void getVectorOfRefs( ResultSet *rs, unsigned int index, 
OCCI_STD_NAMESPACE::vector<Ref<T> > &vect)
{
  OCCI_STD_NAMESPACE::vector<void *> vec_ref;
  getVectorOfOCIRefs(rs, index, vec_ref);

  const Connection *sess = rs->getStatement()->getConnection();

  vect.clear();
  unsigned int size = vec_ref.size();
  vect.reserve( size );
  for (unsigned int i=0; i< size; i++)
  {
    if (vec_ref[i] == (OCIRef *)0)
       vect.push_back(Ref<T>());     // pushing a default-constructed Ref
     else
       vect.push_back(Ref<T>(sess, (OCIRef *)vec_ref[i], FALSE));
  }
}

/*------------------------ setVectorOfRefs for Ref<T>-----------------------*/
/*
   NAME
      setVectorOfRefs - overloaded function. Binds the attribute in the current
      position with a vector of Ref<T>.

   PARAMETERS
      rs - ResultSet
      vect- reference to vector of REF

   DESCRIPTION
     Binds the column in the specified position with a vector of signed int .
   The column at the current position should be a collection type (varray or
   nested table). The SQL type of the elements in the collection should be
   compatible with OCIRef* .

   RETURNS
     nothing

   NOTES
     compatible SQL types : REF

     This will just call setVector(..., vector<OCIRef*>,..)


*/

template <class T>
void setVectorOfRefs( Statement *stmt, unsigned int index, 
const OCCI_STD_NAMESPACE::vector<Ref<T> > &vect, 
const OCCI_STD_NAMESPACE::string &sqltype)
{
  OCCI_STD_NAMESPACE::vector<void *> vec_ref;
  OCCI_STD_NAMESPACE::vector<OCIInd> vec_ind;
  unsigned int size = vect.size();
  vec_ref.reserve( size );
  vec_ind.reserve( size );

  for (unsigned int i = 0; i < size; i++)
  {
    vec_ref.push_back((void *)vect[i].getRef());
    vec_ind.push_back( vect[i].isNull() ? OCI_IND_NULL : OCI_IND_NOTNULL);
  }

  setVectorOfOCIRefs(stmt, index, vec_ref, vec_ind, sqltype);
}

template <class T>
void setVectorOfRefs( Statement *stmt, unsigned int index, 
const OCCI_STD_NAMESPACE::vector<Ref<T> > &vect, 
const OCCI_STD_NAMESPACE::string &schemaName,
const OCCI_STD_NAMESPACE::string &typeName)
{
  OCCI_STD_NAMESPACE::vector<void *> vec_ref;
  OCCI_STD_NAMESPACE::vector<OCIInd> vec_ind;
  unsigned int size = vect.size();
  vec_ref.reserve( size );
  vec_ind.reserve( size );

  for (unsigned int i = 0; i < size; i++)
  {
    vec_ref.push_back((void *)vect[i].getRef());
    vec_ind.push_back( vect[i].isNull() ? OCI_IND_NULL : OCI_IND_NOTNULL);
  }

  setVectorOfOCIRefs(stmt, index, vec_ref, vec_ind, schemaName, typeName);
}

template <class T>
void setVectorOfRefs( Statement *stmt, unsigned int index, 
const OCCI_STD_NAMESPACE::vector<Ref<T> > &vect, 
const UString &schemaName,
const UString &typeName)
{
  OCCI_STD_NAMESPACE::vector<void *> vec_ref;
  OCCI_STD_NAMESPACE::vector<OCIInd> vec_ind;
  unsigned int size = vect.size();
  vec_ref.reserve( size );
  vec_ind.reserve( size );

  for (unsigned int i = 0; i < size; i++)
  {
    vec_ref.push_back((void *)vect[i].getRef());
    vec_ind.push_back( vect[i].isNull() ? OCI_IND_NULL : OCI_IND_NOTNULL);
  }

  setVectorOfOCIRefs(stmt, index, vec_ref, vec_ind, schemaName, typeName);
}

/*------------------------ getVectorOfRefs for Ref<T> ----------------------*/
/*
   NAME
      getVectorOfRefs - overloaded function. Retrieves the attribute in the
      current position as a vector of Ref<T>

   PARAMETERS
      stmt - Statement
      vect- reference to vector of Ref<T>(OUT parameter).

   DESCRIPTION
     Retrieves the column in the specified position as a vector of Ref<T>.
   The attribute at the current position should be a collection type (varray or
   nested table). The SQL type of the elements in the collection should be
   compatible with Ref<T>.

   RETURNS
     nothing

   NOTES
        compatible SQL types : REF
*/

template <class T>
void getVectorOfRefs( Statement *stmt, unsigned int index, 
OCCI_STD_NAMESPACE::vector <Ref<T> > &vect)
{
  OCCI_STD_NAMESPACE::vector<void *> vec_ref;
  getVectorOfOCIRefs(stmt, index, vec_ref);

  const Connection *sess = stmt->getConnection();

  vect.clear();
  unsigned int size = vec_ref.size();
  vect.reserve( size );
  for (unsigned int i=0; i< size; i++)
  {
    if (vec_ref[i] == (OCIRef *)0)
       vect.push_back(Ref <T>());     // pushing a default-constructed Ref
     else
       vect.push_back(Ref<T> (sess, (OCIRef *)vec_ref[i], FALSE));
  }
}
/*----------------------------- pinVectorOfRefs---------------------*/
/*
   NAME
    pinVectorOfRefs - array pin implementation

   PARAMETERS
     conn- Connection object
     vecRef - vector of OCIRefs *
     vecCor - vector of OCIComplexObject *
     vecPOBj - vector of PObject * ( OUT )

   DESCRIPTION
    implements the array pin of refs passed and returns the corresponding
    PObject s

   RETURNS

   NOTES
*/
template <class T>
void pinVectorOfRefs( const Connection *conn,
OCCI_STD_NAMESPACE::vector<Ref<T> > &vect, 
OCCI_STD_NAMESPACE::vector<T* > &vectObj, LockOptions lockOpt) 
{
   
 OCCI_STD_NAMESPACE::vector<void *> vecRef;
 OCCI_STD_NAMESPACE::vector<void *> vecCor;
 OCCI_STD_NAMESPACE::vector<PObject *> vecPObj;
 unsigned int sz = vect.size();
 vecRef.reserve( sz );
 vecCor.reserve( sz );

 for ( unsigned int  i=0; i < sz; i++)
 {
   vecRef.push_back((void *)vect[i].getRef());
   vecCor.push_back((void *)vect[i].getCor());
 }
 pinVectorOfOCIRefs(conn, vecRef, vecCor, vecPObj, lockOpt);
 for ( unsigned int  k=0; k < sz; k++)
 {
   vectObj.push_back((T *)vecPObj[k]);
   vect[k].setPinnedObject(vecPObj[k]);
 }
}

/*----------------------------- pinVectorOfRefs---------------------*/
/*
   NAME
    pinVectorOfRefs - array pin implementation

   PARAMETERS
     conn- Connection object
     vecRef - vector of OCIRefs *
     vecCor - vector of OCIComplexObject *

   DESCRIPTION
    implements the array pin of refs passed 

   RETURNS

   NOTES
*/
template <class T>
void pinVectorOfRefs( const Connection *conn,
OCCI_STD_NAMESPACE::vector<Ref<T> > &vect,
LockOptions lockOpt)
{
  
 OCCI_STD_NAMESPACE::vector<void *> vecRef;
 OCCI_STD_NAMESPACE::vector<void *> vecCor;
 OCCI_STD_NAMESPACE::vector<PObject *> vecPObj;
 unsigned int sz = vect.size();
 vecRef.reserve( sz );
 vecCor.reserve( sz );

 for ( unsigned int  i=0; i < sz; i++)
 {
   vecRef.push_back((void *)vect[i].getRef());
   vecCor.push_back((void *)vect[i].getCor());
 }
 pinVectorOfOCIRefs(conn, vecRef, vecCor,vecPObj, lockOpt);
 for ( unsigned int  k=0; k < sz; k++)
   vect[k].setPinnedObject(vecPObj[k]);
}



/*---------------------------------------------------------------------------
                          INTERNAL FUNCTIONS
  ---------------------------------------------------------------------------*/


} /* end of namespace occi */
} /* end of namespace oracle */
#endif                                              /* OCCICONTROL_ORACLE */

#endif                                              /* _olint */
