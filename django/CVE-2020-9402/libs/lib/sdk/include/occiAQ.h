/* Copyright (c) 2002, 2013, Oracle and/or its affiliates. 
All rights reserved.*/
 
/* 
   NAME 
     occiAQ.h - Header file for occi AQ classes

   DESCRIPTION 
     Class declarations for Producer, Consumer, Message, Agent
     Listener, Subscription

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
   cparampa    10/12/02 - creation

*/

#ifndef _olint   /* disable olint check */

#ifndef OCCIAQ_ORACLE
# define OCCIAQ_ORACLE

#ifndef OCCICOMMON_ORACLE
#include <occiCommon.h>
#endif

#ifdef RAW 
#undef RAW 
#endif /* RAW */

namespace oracle {
namespace occi {
namespace aq{

/*---------------------------------------------------------------------------
                     PUBLIC TYPES AND CONSTANTS
  ---------------------------------------------------------------------------*/

class Message
{
  public:

     enum MessageState
     {
        MSG_READY = OCI_MSG_READY,
        MSG_WAITING = OCI_MSG_WAITING,
        MSG_PROCESSED = OCI_MSG_PROCESSED,
        MSG_EXPIRED = OCI_MSG_EXPIRED
     };
     enum PayloadType
     {
        RAW,
        ANYDATA,
        OBJECT
     };

     Message( const Environment *env );
     Message( const Message& rhs);
     ~Message();
     
     void operator=(const Message& rhs);
     int getAttemptsToDequeue() const ;
     void setCorrelationId( const OCCI_STD_NAMESPACE::string& corr_id ) ;
     OCCI_STD_NAMESPACE::string getCorrelationId() const ;
     void setDelay( int delay ) ;
     int getDelay() const ;
     Date getMessageEnqueuedTime() const ;
     void setExceptionQueueName( const OCCI_STD_NAMESPACE::string& queue ) ;
     OCCI_STD_NAMESPACE::string getExceptionQueueName() const ;
     void setExpiration( int exp ) ;
     int getExpiration() const ;
     MessageState getMessageState() const ;
     void setPriority( int priority ) ;
     int getPriority() const ;
     void setRecipientList( OCCI_STD_NAMESPACE::vector<Agent>& ag_list ) ;
     void setSenderId( const Agent& sender ) ;
     Agent getSenderId() const ;
     void setOriginalMessageId( const Bytes& queue ) ;
     Bytes getOriginalMessageId() const ;
     void setNull();
     bool isNull() const;

     void setBytes( const Bytes& bytes); 
     void setObject( PObject* pobj);
     void setAnyData( const AnyData& any);

     Bytes getBytes() const ;
     PObject *getObject() ;
     AnyData getAnyData() const ;

     PayloadType getPayloadType( ) const;

   private:
     Ptr<MessageImpl> ptr;
     OCIAQMsgProperties* getOCIMsgProperties() const;
     Message( const Environment *env, const Connection *con, 
          OCIAQMsgProperties *msgprop, void *data, unsigned int dataLen, 
          bool isNull, PayloadType pType);
     friend class ConsumerImpl;
     friend class ProducerImpl;
     friend class NotifyResult;
};

class Producer
{
   public:

     enum EnqueueSequence
     {
       ENQ_BEFORE = OCI_ENQ_BEFORE,
       ENQ_TOP    = OCI_ENQ_TOP
     };
     enum Visibility
     {
       ENQ_IMMEDIATE = OCI_ENQ_IMMEDIATE,
       ENQ_ON_COMMIT = OCI_ENQ_ON_COMMIT
     };
     
     Producer( const Connection *conn ); 
     Producer( const Connection *conn, 
               const OCCI_STD_NAMESPACE::string& queue ); 
     Producer( const Producer& rhs);
     ~Producer();

     void operator=(const Producer& prod);
     void setRelativeMessageId( const Bytes& msgid );
     Bytes getRelativeMessageId() const;
     void setSequenceDeviation( EnqueueSequence option );
     EnqueueSequence getSequenceDeviation() const;
     void setVisibility( Visibility option );
     Visibility getVisibility() const;
     void setQueueName( const OCCI_STD_NAMESPACE::string& queue );
     OCCI_STD_NAMESPACE::string getQueueName() const;
     void setTransformation( const OCCI_STD_NAMESPACE::string& fName);
     OCCI_STD_NAMESPACE::string getTransformation() const;
     Bytes send( Message& msg, const OCCI_STD_NAMESPACE::string& queue );
     Bytes send( Message& msg );
     void setNull();
     bool isNull() const;

  private:
     Ptr<ProducerImpl> ptr; 
};

class Consumer
{
   public:

     enum DequeueMode
     {
       DEQ_BROWSE  = OCI_DEQ_BROWSE,
       DEQ_LOCKED  = OCI_DEQ_LOCKED,
       DEQ_REMOVE  = OCI_DEQ_REMOVE,
       DEQ_REMOVE_NODATA = OCI_DEQ_REMOVE_NODATA
     };
     enum Navigation
     {
       DEQ_FIRST_MSG        = OCI_DEQ_FIRST_MSG,
       DEQ_NEXT_TRANSACTION = OCI_DEQ_NEXT_TRANSACTION,
       DEQ_NEXT_MSG         = OCI_DEQ_NEXT_MSG
     };
     enum Visibility
     {
       DEQ_IMMEDIATE = OCI_DEQ_IMMEDIATE,
       DEQ_ON_COMMIT = OCI_DEQ_ON_COMMIT
     };
     enum
     {
       DEQ_WAIT_FOREVER = OCI_DEQ_WAIT_FOREVER,
       DEQ_NO_WAIT      = OCI_DEQ_NO_WAIT
     };
     
     Consumer( const Connection *conn ); 
     Consumer( const Connection * conn, const Agent& agent); 
     Consumer( const Connection *conn,
               const OCCI_STD_NAMESPACE::string& queue ); 
     Consumer(const Consumer& con);
     ~Consumer();
    
     void operator=(const Consumer& con);
     void setAgent(const Agent& agent);
     void setConsumerName( const OCCI_STD_NAMESPACE::string& name );
     OCCI_STD_NAMESPACE::string getConsumerName() const;
     void setCorrelationId( const OCCI_STD_NAMESPACE::string& cor_id );
     OCCI_STD_NAMESPACE::string getCorrelationId() const;
     void setDequeueMode( DequeueMode mode );
     DequeueMode getDequeueMode() const;
     void setMessageIdToDequeue( const Bytes& msgid );
     Bytes getMessageIdToDequeue() const;
     void setPositionOfMessage( Navigation pos );
     Navigation getPositionOfMessage() const;
     void setVisibility( Visibility option );
     Visibility getVisibility() const;
     void setWaitTime( unsigned int wait );
     unsigned int getWaitTime() const;
     void setQueueName( const OCCI_STD_NAMESPACE::string& queue );
     OCCI_STD_NAMESPACE::string getQueueName() const;
     void setTransformation( const OCCI_STD_NAMESPACE::string& fName);
     OCCI_STD_NAMESPACE::string getTransformation() const;
     Message receive( Message::PayloadType pType,
                      const OCCI_STD_NAMESPACE::string& type="",
                      const OCCI_STD_NAMESPACE::string& schema="");
     void setNull();
     bool isNull() const;

   private:
     Ptr<ConsumerImpl> ptr; 
};

class Agent
{
   public:
     Agent( const Environment *env ); 
     Agent( const Environment *env, 
            const OCCI_STD_NAMESPACE::string& name, 
            const OCCI_STD_NAMESPACE::string& address, 
            unsigned int protocol=0 ) ; 
     Agent(const Agent& a);
     ~Agent() ;
 
     void operator=(const Agent& a);
     void setName( const OCCI_STD_NAMESPACE::string& name );
     OCCI_STD_NAMESPACE::string getName() const;
     void setAddress( const OCCI_STD_NAMESPACE::string& addr );
     OCCI_STD_NAMESPACE::string getAddress() const;
     void setProtocol(unsigned int protocol = 0);
     unsigned int getProtocol() const; 
     void setNull(); 
     bool isNull() const;
     OCIAQAgent* getOCIAQAgent() const;

   private:
     Ptr<AgentImpl> ptr;
     Agent( const Environment *env, OCIAQAgent *rhs, bool toFree = false );
     friend class Listener;
     friend class MessageImpl;
};

class Listener
{
   public:
     Listener( const Connection *conn ); 
     Listener( const Connection *conn, 
               OCCI_STD_NAMESPACE::vector<Agent> &agList, 
               int waitTime=0 );
     ~Listener();
     
     Agent listen();
     void setAgentList(OCCI_STD_NAMESPACE::vector<Agent> &agList);
     void setTimeOutForListen(int waitTime);
     OCCI_STD_NAMESPACE::vector<Agent> getAgentList() const;
     int getTimeOutForListen() const; 

   private:
     const ConnectionImpl *conn; 
     OCIAQAgent** agentList; 
     unsigned int numAgents;
     int timeOut;
     void *listenerExt;
};


class Subscription
{
  public:
    enum Presentation
    {
       PRES_DEFAULT = OCI_SUBSCR_PRES_DEFAULT,
       PRES_XML = OCI_SUBSCR_PRES_XML
    };
    enum Protocol
    {
       PROTO_CBK = OCI_SUBSCR_PROTO_OCI,
       PROTO_MAIL = OCI_SUBSCR_PROTO_MAIL,
       PROTO_SERVER = OCI_SUBSCR_PROTO_SERVER,
       PROTO_HTTP = OCI_SUBSCR_PROTO_HTTP
    };
    enum Namespace
    {
       NS_ANONYMOUS = OCI_SUBSCR_NAMESPACE_ANONYMOUS,
       NS_AQ = OCI_SUBSCR_NAMESPACE_AQ
    }; 
    
    Subscription(const Environment* env);
    Subscription(const Environment* env, OCISubscription* sub);
    Subscription(const Subscription& sub);
    ~Subscription();
    
    void operator=(const Subscription& sub);
    unsigned int getDatabaseServersCount() const;
    void setDatabaseServerNames(
        const OCCI_STD_NAMESPACE::vector<OCCI_STD_NAMESPACE::string>& dbsrv);
    OCCI_STD_NAMESPACE::vector<OCCI_STD_NAMESPACE::string>
                      getDatabaseServerNames() const ;
    void setNotifyCallback(unsigned int (*callback)(Subscription& sub, 
                                                    NotifyResult *nr));
    unsigned int  (*getNotifyCallback() const)(Subscription& sub, 
                                               NotifyResult *nr); 
    void setCallbackContext(void* ctx);
    void* getCallbackContext() const;
    void setSubscriptionName(const OCCI_STD_NAMESPACE::string& name);
    OCCI_STD_NAMESPACE::string getSubscriptionName() const ;
    void setSubscriptionNamespace(Namespace nameSpace);
    Namespace getSubscriptionNamespace() const ;
    void setPayload(const Bytes& payload);
    Bytes getPayload() const ;
    void setRecipientName( const OCCI_STD_NAMESPACE::string& name);
    OCCI_STD_NAMESPACE::string getRecipientName() const;
    void setPresentation( Presentation pres) ;
    Presentation getPresentation() const ;
    void setProtocol( Protocol prot) ;
    Protocol getProtocol() const ;
    OCISubscription* getOCISubscription() const;
    void setNull();
    bool isNull() const;
    Environment* getEnvironment() const;

  private:
    Ptr<SubscriptionImpl> ptr;
};

class NotifyResult
{
   public:
     Bytes getPayload() const;
     Message getMessage() const;
     Bytes getMessageId() const;
     OCCI_STD_NAMESPACE::string getConsumerName() const;
     OCCI_STD_NAMESPACE::string getQueueName() const;

  private:
     const EnvironmentImpl *env;
     void *payload;
     unsigned int payloadLen;
     void *desc;
     ub4 mode;
     void *notifyResultExt;

     //private constructor
     NotifyResult(const Environment *env, void *payload, ub4 payloadLen, 
                  void *pdescriptor, ub4 mode);

     friend class SubscriptionImpl;
};
      
/*---------------------------------------------------------------------------
                     PRIVATE TYPES AND CONSTANTS
  ---------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------
                           EXPORT FUNCTIONS
  ---------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------
                          INTERNAL FUNCTIONS
  ---------------------------------------------------------------------------*/
} /* end of namespace aq */
} /* end of namespace occi */
} /* end of namespace oracle */

#endif                                              /* OCCIAQ_ORACLE */

#endif                                              /* _olint */
