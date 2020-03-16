/* Copyright (c) 2000, 2009, Oracle and/or its affiliates. 
All rights reserved. */
   
/* 
   NAME 
     occiData.h - header file for OCCI data classes

   DESCRIPTION 
     Class definitions for Stream, Blob, Clob ,Bfile,
     Number, Date, IntervalYM, IntervalDS, Time,
     Timestamp

   RELATED DOCUMENTS 
     <note any documents related to this facility>
 
   EXPORT FUNCTION(S) 
   <external functions declared for use outside package - 
    one-line descriptions>

   INTERNAL FUNCTION(S)
     <other external functions declared - one-line descriptions>

   EXAMPLES

   NOTES
     <other useful comments, qualifications, etc.>

   MODIFIED   (MM/DD/YY)
   kkverma     11/23/09 - Replace OCIEnv* type ocienv member to Environment type
   slynn       03/18/08 - Add get/setContentType.
   slynn       09/14/06 - Remove string.clear()
   slynn       07/28/06 - Migrate to new 11g LOB terminology 
   slynn       06/21/06 - Add LobRegion 
   slynn       05/25/06 - New NG Lob Functionality. 
   cparampa    09/06/04 - Date changes
   shiyer      10/31/03 - Timestamp constructors issue 
   rvallam     10/07/03 - bug 3089939 - add private method in Date to compute 
                          hour and min component in daysBetween to be passed 
                          to set method of IntervalDS.
   cparampa    08/21/03 - added toCopy to IntervalDS and IntervalYM
   cparampa    07/14/03 - make SubscriptionImpl friend of Bytes class. 
   rvallam     02/12/03 - modified BFloat/BDouble interface - BFloat/BDouble
                          type is now a struct
   cparampa    01/20/03 - made ProducerImpl friend of Bytes class
   rvallam     11/19/02 - objects support for interval class
   shiyer      11/15/02 - Add UTF16 support to IntervalYM & IntervalDS
   cparampa    12/11/02 - removed references to class Payload
   cparampa    10/12/02 - AQ additions
   shiyer      10/12/02 - Added UTF16 version of get/set CharsetId in Clob
   shiyer      09/06/02 - OCCI globalization support
   aahluwal    06/04/02 - bug 2360115
   vvinay      02/21/02 - operator= added for Bytes
   gayyappa    10/23/01 - fix bug 2073327 , use string instead of 
                          enum CharSet 
   vvinay      12/21/01 - signed char constructor and cast operator
                          (bug 2073334)
                          binary operator methods not friends any more
   gayyappa    15/10/01 - add parameter toCopy to Lob/Timestamp private 
                          constructors 
   rvallam     04/09/01 - change private constructor in Number to pass
                          parameter by reference and made it const
   chliang     03/05/01 - disable olint
   rvallam     01/27/02 - remove #include <ostream>
   gayyappa    01/17/01 - add methods/operators to Interval and
                          timestamp classes..
   gayyappa    12/15/00 - interface changes in set methods
   rvallam     11/29/00 - change method signature in Bytes
                          added 3 new methods in Number  
   rvallam     10/20/00  - change method signatures in Date
   rvallam     09/15/00 - make StmtImpl/ResultSetImpl friend to 
                          interval classes 
   gayyappa    08/21/00 - modified timestamp, interval headers.
                          add OCIEnv to constructor of Bytes.,
                          removed getOCIRaw from Bytes.
                          add const to setVector mthds of anydata.
                          add void* data member to Timestamp/Interval.
   rvallam     08/10/00 - modified CORE class headers to add friends , 
                          added private constructor in Bytes
   slari       08/02/00 - comment out Stream
   rratnam     08/04/00 - updated the LOB stream interface
   rkasamse    08/07/00 - make getVector friend of Time
   slari       07/31/00 - add const to Bytes methods
   slari       07/25/00 - disable Bytes(Bytes *)
   slari       07/23/00 - add Bytes
   gayyappa    07/26/00 - update Timestamp, IntervalYM, IntervalDS.
   gayyappa    07/04/00 - for fixing a problem in occiNumber
   rratnam     06/13/00 - Updated LOB class headers
   kmohan      05/31/00 - Change Environment to Environment * in 
                          Date constructor
   kmohan      05/29/00 - No string
   rkasamse    04/25/00 - Added string class header
   etucker     04/19/00 - Added CORE class headers
   kmohan      04/11/00 - Creation

*/

#ifndef _olint   /* disable olint check */

#ifndef OCCIDATA_ORACLE
# define OCCIDATA_ORACLE

#ifndef OCCICOMMON_ORACLE
#include <occiCommon.h>
#endif

#ifndef OCCICONTROL_ORACLE
#include <occiControl.h>
#endif

namespace oracle {
namespace occi {
class Bytes
{
 
 public:
 
  Bytes(const Environment *env = NULL);      // default constructor
 
  Bytes(unsigned char *value, unsigned int count, 
            unsigned int offset = 0, const Environment *env = NULL); 

  Bytes(const Bytes &e); // copy constructor

 
                    // public methods
 
  void getBytes(unsigned char *dst, unsigned int count, 
                        unsigned int srcBegin = 0,
                        unsigned int dstBegin = 0) const; 
 
  unsigned int length() const;
 
  unsigned char byteAt(unsigned int index) const;

  bool isNull() const;
 
  void setNull();

  void operator=(const Bytes &other);
 
  ~Bytes();

private:
                                // private data members
  Bytes(OCIEnv *,OCIRaw *) ;
  Bytes(Ptr<BytesImpl> bytesPtr) ;
  Ptr<BytesImpl> ptr_;
  friend class AnyDataImpl;
  friend class aq::MessageImpl;
  friend class aq::ProducerImpl;
  friend class aq::SubscriptionImpl;
  friend  void getVector(const AnyData &any, 
                         OCCI_STD_NAMESPACE::vector<Bytes> &vect) ;
  friend  void setVector(AnyData &any, 
  const OCCI_STD_NAMESPACE::vector<Bytes> &vect) ;

};
 
class Bfile
{
  public :

    Bfile();
    Bfile(const Connection *connectionp) ;
    Bfile(const Bfile &srcBfile) ;
    ~Bfile();
    unsigned int length() const ;
    OCCI_STD_NAMESPACE::string getDirAlias() const ;
    UString getUStringDirAlias() const ;
    OCCI_STD_NAMESPACE::string getFileName() const ;
    UString getUStringFileName() const ;
    void setName(const OCCI_STD_NAMESPACE::string &dirAlias, 
                 const OCCI_STD_NAMESPACE::string &fileName) ;
    void setName(const UString &dirAlias, const UString &fileName) ;
    bool fileExists() const ; 
    Bfile& operator =(const Bfile &srcBfile) ;
    bool operator ==(const Bfile &srcBfile) const ;
    bool operator !=(const Bfile &srcBfile) const ;
    void setNull() ;
    bool isNull() const ;
    bool isInitialized() const;
    void open() ;
    void close() ;
    bool isOpen() const ;
    unsigned int read(unsigned int  amt, unsigned char *buffer,
      unsigned int  bufsize, unsigned int offset = 1) const ;
    Stream* getStream(unsigned int offset = 1,
      unsigned int amount =0)  ;
    void closeStream(Stream *stream);

  private:

    //Data Members:

    // pointer to the FILE locator
    OCIBFileLocator *filep; 

    // pointer to the ConnectionImpl instance
    const ConnectionImpl *connp; 

    // pointer to the LobStreamImpl instance obtained from this FILE
    LobStreamImpl *streamp;

    void *bfileExt;

    //Enumerations:
    enum {MAXDIRNAMELEN = 32, MAXFILENAMELEN = 256};

    //Constructor: 
    Bfile(const Connection *connectionp, 
    OCIBFileLocator *locatorp, bool toCopy = true) ;
  
    //Methods:
    OCIBFileLocator* getLocator() const;
    void do_getDirAlias( void * dirAlias, ub2 * dirAliasLen) const ;
    void do_getFileName( void * fileName, ub2 * fileNameLen) const ;
    void do_setName( void * alias, ub2 aliasLen,
                     void *fileName, ub2 fileNameLen);   
  // Friends
  friend class AnyDataImpl;
  friend class StatementImpl;
  friend class ResultSetImpl;
  friend class Blob;
  friend class Clob;
  friend class aq::MessageImpl;

  friend void getVector(const AnyData&, OCCI_STD_NAMESPACE::vector<Bfile>&) ;
  friend void getVector(Statement*, unsigned int, 
  OCCI_STD_NAMESPACE::vector<Bfile>&) ;
  friend void getVector(ResultSet*, unsigned int , 
  OCCI_STD_NAMESPACE::vector<Bfile>&) ;
  friend void setVector(AnyData&, const OCCI_STD_NAMESPACE::vector<Bfile>&) ;
  friend void do_setVectorOfBfile(Statement*, unsigned int, 
   const OCCI_STD_NAMESPACE::vector<Bfile>&, void *, unsigned int, 
   void *, unsigned int ) ;

#ifdef ORAXB8_DEFINED
  friend  void readVectorOfBfiles(const Connection *conn,
          OCCI_STD_NAMESPACE::vector<Bfile> &vec,
          oraub8 *byte_amts, oraub8 *offsets,
          unsigned char *buffers[], oraub8 *buffer_lens);
#endif
};


#ifdef ORAXB8_DEFINED
// See the end of this file for implementation of LobRegion
template < typename lobType > class LobRegion
{
  private:
    lobType     *_primary;
    oraub8       _primaryOffset;
    oraub8       _offset;
    oraub8       _length;
    OCCI_STD_NAMESPACE::string  _mimeType;

    void         setPrimary(const ConnectionImpl *connp,
                            OCILobLocator *locator);

  public:
    LobRegion();
    ~LobRegion();
    lobType     *getPrimary();
    oraub8       getPrimaryOffset();
    oraub8       getOffset();
    oraub8       getLength();
    OCCI_STD_NAMESPACE::string  getMimeType();

  friend class Blob;
  friend class Clob;
};

typedef LobRegion<Blob> BlobRegion;
typedef LobRegion<Clob> ClobRegion;
#endif


class Blob
{
  public:

    Blob();
    Blob(const Connection *connectionp) ;
    Blob(const Blob &srcBlob) ;
    ~Blob();
    unsigned int getChunkSize() const ;
    unsigned int length() const ;
    Blob& operator =(const Blob &srcBlob) ;
    bool operator ==(const Blob &srcBlob) const ;
    bool operator !=(const Blob &srcBlob) const ;
    void setNull() ;
    bool isNull() const ;
    void setEmpty() ;
    void setEmpty(const Connection *connectionp) ;
    bool isInitialized() const;
    void open(LobOpenMode mode=OCCI_LOB_READWRITE) ;
    void close() ;
    bool isOpen() const ;
    void copy(const Blob &srcBlob, unsigned int numBytes,
      unsigned int  dstOffset =1, unsigned int srcOffset =1) ;
    void copy(const Bfile &srcBfile, unsigned int numBytes,
      unsigned int  dstOffset =1, unsigned int srcOffset =1) ;
    void append(const Blob &srcBlob) ;
    unsigned int read(unsigned int  amt, unsigned char *buffer,
      unsigned int  bufsize, unsigned int offset = 1) const ;
    unsigned int write(unsigned int amt, unsigned char *buffer,
      unsigned int bufsize, unsigned int offset = 1) ;
    unsigned int writeChunk(unsigned int amt, unsigned char *buffer,
      unsigned int bufsize, unsigned int offset = 1) ;
    void trim(unsigned int newlen) ;
    Stream* getStream(unsigned int offset = 1,
      unsigned int amount =0)  ;
    void closeStream(Stream *stream);
    LobOptionValue getOptions(LobOptionType optType);
    void setOptions(LobOptionType optType, LobOptionValue value);
    OCCI_STD_NAMESPACE::string getContentType(void);
    void setContentType(const OCCI_STD_NAMESPACE::string contentType);

    void getDeduplicateRegions(OCCI_STD_NAMESPACE::vector<BlobRegion> &regions);
  private:

    //Data Members:

    // pointer to the BLOB locator
    OCIBlobLocator *lobp; 

    // pointer to the ConnectionImpl instance
    const ConnectionImpl *connp; 

    // pointer to the LobStreamImpl instance obtained from this LOB
    LobStreamImpl *streamp; 

    //for future use !
    void *blobExt;

    //Constructor: 
    Blob(const Connection *connectionp, 
    OCIBlobLocator *locatorp, bool toCopy=true) ;

    //Methods:
    OCIBlobLocator* getLocator() const;

  // Friends
  friend class AnyDataImpl;
  friend class StatementImpl;
  friend class ResultSetImpl;

#ifdef ORAXB8_DEFINED
  friend void 
    LobRegion<Blob>::setPrimary(const ConnectionImpl *connp,
                         OCILobLocator *locator);
#endif
  friend void getVector(const AnyData&, OCCI_STD_NAMESPACE::vector<Blob>&) ;
  friend void getVector(Statement*, unsigned int, 
  OCCI_STD_NAMESPACE::vector<Blob>&) ;
  friend void getVector(ResultSet*, unsigned int, 
  OCCI_STD_NAMESPACE::vector<Blob>&) ;
  friend void setVector(AnyData&, const OCCI_STD_NAMESPACE::vector<Blob>&) ;
  friend void do_setVectorOfBlob(Statement*, unsigned int, 
  const OCCI_STD_NAMESPACE::vector<Blob>&, void *, 
  unsigned int, void *, unsigned int ) ;
#ifdef ORAXB8_DEFINED
  friend  void readVectorOfBlobs(const Connection *conn,
          OCCI_STD_NAMESPACE::vector<Blob> &vec,
          oraub8 *byte_amts, oraub8 *offsets,
          unsigned char *buffers[], oraub8 *buffer_lens);
  friend  void writeVectorOfBlobs(const Connection *conn,
          OCCI_STD_NAMESPACE::vector<Blob> &vec,
          oraub8 *byte_amts, oraub8 *offsets,
          unsigned char *buffers[], oraub8 *buffer_lens);
#endif
};

class Clob
{
  public:

    Clob();
    Clob(const Connection *connectionp) ;
    Clob(const Clob &srcClob) ;
    ~Clob();
    unsigned int getChunkSize() const ;
    unsigned int length() const ;
    OCCI_STD_NAMESPACE::string getCharSetId() const;
    CharSetForm getCharSetForm() const;
    void setCharSetId( const OCCI_STD_NAMESPACE::string &charset) ;
    void setCharSetForm( CharSetForm csfrm) ;
    Clob& operator =(const Clob &srcClob) ;
    bool operator ==(const Clob &srcClob) const ;
    bool operator !=(const Clob &srcClob) const ;
    void setNull() ;
    bool isNull() const ;
    void setEmpty() ;
    void setEmpty(const Connection *connectionp) ;
    bool isInitialized() const;
    void open(LobOpenMode mode=OCCI_LOB_READWRITE) ;
    void close() ;
    bool isOpen() const ;
    void copy(const Clob &srcClob, unsigned int numBytes,
              unsigned int  dstOffset = 1, unsigned int srcOffset = 1) ;
    void copy(const Bfile &srcBfile, unsigned int numBytes,
              unsigned int  dstOffset = 1, unsigned int srcOffset = 1) ;
    void append(const Clob &srcClob) ;
    unsigned int read(unsigned int  amt, unsigned char *buffer,
                      unsigned int  bufsize,  unsigned int offset = 1) const;
    unsigned int read(unsigned int  amt, utext *buffer,
                      unsigned int  bufsize,  unsigned int offset = 1) const;
    unsigned int write(unsigned int amt, unsigned char *buffer,
                       unsigned int bufsize,  unsigned int offset = 1 );
    unsigned int write(unsigned int amt, utext *buffer,
                       unsigned int bufsize,  unsigned int offset = 1 );
    unsigned int writeChunk(unsigned int amt, unsigned char *buffer,
                            unsigned int bufsize, unsigned int offset = 1 );
    unsigned int writeChunk(unsigned int amt, utext *buffer,
                            unsigned int bufsize, unsigned int offset = 1 );
    void trim(unsigned int newlen) ;
    Stream* getStream(unsigned int offset = 1,
      unsigned int amount =0 );
    void closeStream(Stream *stream);
    LobOptionValue getOptions(LobOptionType optType);
    void setOptions(LobOptionType optType, LobOptionValue value);
    OCCI_STD_NAMESPACE::string getContentType(void);
    void setContentType(const OCCI_STD_NAMESPACE::string contentType);
    
    UString getCharSetIdUString() const;
    void setCharSetIdUString( const UString &charset) ;

    void getDeduplicateRegions(OCCI_STD_NAMESPACE::vector<ClobRegion> &regions);

  private:

    //Data Members:

    // pointer to the CLOB locator
    OCIClobLocator *lobp; 

    // pointer to the ConnectionImpl instance
    const ConnectionImpl *connp; 

    // pointer to the LobStreamImpl instance obtained from this LOB
    LobStreamImpl *streamp; 

    //charset id
    ub2 charsetId;

    //charset form
    CharSetForm charsetForm;

    //for future use !
    void *clobExt;

    //Constructor: 
    Clob(const Connection *connectionp, 
    OCIClobLocator *locatorp, bool toCopy =true ) ;

    //Methods:
    OCIClobLocator* getLocator() const;
    unsigned int do_read( unsigned int amt, void *buffer, 
    unsigned int bufsize, unsigned int offset) const;
    unsigned int do_write( unsigned int amt, void *buffer, 
    unsigned int bufsize, unsigned int offset) ;
    unsigned int do_writeChunk( unsigned int amt, void *buffer, 
    unsigned int bufsize, unsigned int offset) ;
   
  // Friends
  friend class AnyDataImpl;
  friend class StatementImpl;
  friend class ResultSetImpl;

#ifdef ORAXB8_DEFINED
  friend void 
    LobRegion<Clob>::setPrimary(const ConnectionImpl *connp,
                         OCILobLocator *locator);
#endif
  friend void getVector(const AnyData&, OCCI_STD_NAMESPACE::vector<Clob>&) ;
  friend void getVector(Statement*, unsigned int, 
  OCCI_STD_NAMESPACE::vector<Clob>&) ;
  friend void getVector(ResultSet*, unsigned int, 
  OCCI_STD_NAMESPACE::vector<Clob>&) ;
  friend void setVector(AnyData&, const OCCI_STD_NAMESPACE::vector<Clob>&) ;
  friend void do_setVectorOfClob(Statement*, unsigned int, 
  const OCCI_STD_NAMESPACE::vector<Clob>&, void *, 
  unsigned int, void *, unsigned int ) ;
#ifdef ORAXB8_DEFINED
  friend  void readVectorOfClobs(const Connection *conn,
          OCCI_STD_NAMESPACE::vector<Clob> &vec,
          oraub8 *byte_amts, oraub8 *char_amts, oraub8 *offsets,
          unsigned char *buffers[], oraub8 *buffer_lens);
  friend  void writeVectorOfClobs(const Connection *conn,
          OCCI_STD_NAMESPACE::vector<Clob> &vec,
          oraub8 *byte_amts, oraub8 *char_amts, oraub8 *offsets,
          unsigned char *buffers[], oraub8 *buffer_lens);
  friend  void readVectorOfClobs(const Connection *conn,
          OCCI_STD_NAMESPACE::vector<Clob> &vec,
          oraub8 *byte_amts, oraub8 *char_amts, oraub8 *offsets,
          utext *buffers[], oraub8 *buffer_lens);
  friend  void writeVectorOfClobs(const Connection *conn,
          OCCI_STD_NAMESPACE::vector<Clob> &vec,
          oraub8 *byte_amts, oraub8 *char_amts, oraub8 *offsets,
          utext *buffers[], oraub8 *buffer_lens);
#endif
};

class Number
{
 
 public:

 // Constructors
  /* default constructor added  */
  Number();
  Number(const Number &srcNum);
  Number(long double val) ;
  Number(double val) ;
  Number(float val) ;
  Number(long val) ;
  Number(int val) ;
  Number(short val) ;
  Number(char val) ;
  Number(signed char val);
  Number(unsigned long val) ;
  Number(unsigned int val) ;
  Number(unsigned short val) ;
  Number(unsigned char val) ;

  ~Number();
  // Methods
  const Number abs() const ;
  // unary negate
  const Number operator-() ;
  // unary increment
  Number& operator++() ;
  const Number operator++(int) ;
  // unary decrement
  Number& operator--() ;
  const Number operator--(int) ;
  // assigment operator
  Number& operator=(const Number &a);
  // add and assign
  Number& operator+=(const Number &a) ;
  // subtract and assign
  Number& operator-=(const Number &a) ;
  // Mulitply an assign
  Number& operator*=(const Number &a) ;
   // divide and assign
  Number& operator/=(const Number &a) ;
  // Modulo and assign
  Number& operator%=(const Number &a) ;
  // casting operators
  operator long() const;
  operator int() const;
  operator short() const;
  operator char() const;
  operator signed char() const;
  operator unsigned long() const;
  operator unsigned int() const;
  operator unsigned short() const;
  operator unsigned char() const;
  operator long double() const;
  operator double() const;
  operator float() const;
  // Decimal shift
  const Number shift(int val) const ;
  // Integer Power
  const Number intPower(int val) const ;
  const Number ceil() const ;
  const Number floor() const ;
  const Number squareroot() const ;
  int sign() const ;
  // conversion routines
  //  Format Number and return as a OCCI_STD_NAMESPACE::string
  OCCI_STD_NAMESPACE::string toText(const Environment *envp,
                const OCCI_STD_NAMESPACE::string &fmt,
                const OCCI_STD_NAMESPACE::string &nlsParam="") const
                ;
  UString toText(const Environment *envp,
                const UString &fmt,const UString &nlsParam) const
                ;
  // Create an Number from formatted text
  void fromText(const Environment *envp,
                const OCCI_STD_NAMESPACE::string &number,
                const OCCI_STD_NAMESPACE::string &fmt, 
                const OCCI_STD_NAMESPACE::string &nlsParam = "")
               ;
  void fromText(const Environment *envp,
  const UString &number, 
  const UString &fmt, const UString &nlsParam);
  void fromBytes(const Bytes &s) ;
  Bytes toBytes() const;
  // truncate digits
  const Number trunc(int decplace) const ;
   // round to the decplace place.
  const Number round(int decplace) const ;
  // returns an Number with digits decimal digits
  const Number prec(int digits) const ;
  const Number sin() const ;
  const Number cos() const ;
  const Number tan() const ;
  const Number hypSin() const ;
  const Number hypCos() const ;
  const Number hypTan() const ;
  const Number arcSin() const ;
  const Number arcCos() const ;
  const Number arcTan() const ;
  const Number arcTan2(const Number &val) const;
  const Number power(const Number &val) const;
  const Number exp() const ;
  const Number ln() const ;
  const Number log(const Number &val) const;
  bool isNull() const;
  void setNull();
 private:
  /* Private constructor for constructing number from methods inside  */
  Number(const OCINumber &result);
  OCINumber getOCINumber() const;

  OCINumber data;
  /* a flag to indicate if the  Number  is null */
  bool numberIsNull;
  void *numberExt;

  // a >= b
  friend bool operator>=(const Number &a, const Number &b);
  // a < = b
  friend bool operator<=(const Number &a, const Number &b);
  // a > b
  friend bool operator>(const Number &a, const Number &b);
  // a < b
  friend bool operator<(const Number &a, const Number &b);

  friend class IntervalDS;
  friend class IntervalYM;
  friend const IntervalYM operator*(const IntervalYM &a,
  const Number& factor) ;
  friend const IntervalDS operator*(const IntervalDS &a,
  const Number& factor) ;
  friend const IntervalYM operator/(const IntervalYM &a,
  const Number &factor) ;
  friend const IntervalDS operator/(const IntervalDS &a,
  const Number &factor) ;
  friend class ResultSetImpl;
  friend class StatementImpl;
  friend class AnyDataImpl;
  friend void getVector(const AnyData &any, 
  OCCI_STD_NAMESPACE::vector<Number> &vect);
  friend void setVector(AnyData &any, 
  const OCCI_STD_NAMESPACE::vector<Number> &vect) ;
  friend Number MetaData::getNumber(MetaData::AttrId attrid)          const ;
  friend void getVector(Statement  *stmt, unsigned int paramIndex,
  OCCI_STD_NAMESPACE::vector<Number> &vect) ;
  friend void do_setVectorOfNumber(Statement *stmt, unsigned int paramIndex,
  const OCCI_STD_NAMESPACE::vector<Number> &vect, void *schemaName,
  unsigned int schemaNameLen,
  void *typeName, unsigned int typeNameLen);
  friend void getVector(ResultSet  *rs, unsigned int colIndex,
  OCCI_STD_NAMESPACE::vector<Number> &vect);

};

class Date
{
 public:

  // Constructors
  Date();
  Date(const Date &a);
  Date(const Environment *envp,int year = 1,unsigned int month = 1,
           unsigned int day = 1,unsigned  int hour = 0,
           unsigned int minute = 0, unsigned int seconds = 0);
  ~Date();
  // Methods

  void setDate(int year = 1,unsigned int month = 1,unsigned int day = 1,
               unsigned int hour = 0,unsigned int minute = 0,
               unsigned int seconds = 0);
  void getDate(int &year,unsigned int &month,unsigned int &day,
           unsigned int &hour ,unsigned int &min ,unsigned int &sec) const;
  Bytes toBytes() const ;
  void fromBytes(const Bytes &byteStream, 
  const Environment *envp = NULL);
  OCCI_STD_NAMESPACE::string toText(
                 const OCCI_STD_NAMESPACE::string &fmt = "",
                 const OCCI_STD_NAMESPACE::string &nlsParam = "") const;
  UString toText(
                 const UString &fmt ,
                 const UString &nlsParam ) const;
  void fromText(const OCCI_STD_NAMESPACE::string &datestr,
                const OCCI_STD_NAMESPACE::string &fmt = "",
                const OCCI_STD_NAMESPACE::string &nlsParam = "",
                const Environment *envp = NULL);
  void fromText(const UString &datestr,
                const UString &fmt , const UString &nlsParam ,
                const Environment *envp = NULL);
  Date toZone(const OCCI_STD_NAMESPACE::string &zone1, 
              const OCCI_STD_NAMESPACE::string &zone2) const;
  Date& operator=(const Date &d);
  Date addMonths(int i) const;
  Date addDays(int i) const ;
  Date lastDay() const ;
  IntervalDS daysBetween(const Date &d) const;
  Date nextDay(const OCCI_STD_NAMESPACE::string &dow) const;
  Date nextDay(const UString &dow) const;
  bool isNull() const;
  void setNull();
  static Date getSystemDate(const Environment *envp) ;

 private:
  OCIDate date;
  const EnvironmentImpl *envp;
  bool dateIsNull;
  void *dateExt;

  /* private constructor */
  Date(const Environment *env,OCIDate dateval);
  OCIDate getOCIDate() const;
  void constructHourAndMinute(sb4 &seconds, sb4 &hours, sb4 &minutes) const;
  friend bool operator==(const Date &a,const Date &b);
  friend bool operator>(const Date &a,const Date &b);
  friend bool operator<(const Date &a,const Date &b);
  friend bool operator!=(const Date &a,const Date &b);
  friend bool operator>=(const Date &a,const Date &b);
  friend bool operator<=(const Date &a,const Date &b);
  friend class ResultSetImpl;
  friend class StatementImpl;
  friend class AnyDataImpl;
  friend class aq::MessageImpl;
  friend void getVector(const AnyData &any, 
                        OCCI_STD_NAMESPACE::vector<Date> &vect) ;
  friend void setVector(AnyData &any, 
                        const OCCI_STD_NAMESPACE::vector<Date> &vect);
  friend void getVector(Statement  *stmt, unsigned int paramIndex,
   OCCI_STD_NAMESPACE::vector<Date> &vect)  ;
  friend void do_setVectorOfDate(Statement *stmt, unsigned int paramIndex, 
   const OCCI_STD_NAMESPACE::vector<Date> &vect, void *schemaName,
   unsigned int schemaNameLen,void *typeName, unsigned int typeNameLen) ;
  friend void getVector(ResultSet  *rs, unsigned int colIndex, 
  OCCI_STD_NAMESPACE::vector<Date> &vect) ;

}; //class Date

class Timestamp
{
  public:
  Timestamp() ;

  Timestamp( const Environment *env, int year=1,
             unsigned int month=1, unsigned int day=1, unsigned int hour=0,
             unsigned int min=0 ,unsigned int sec=0, unsigned int fs=0, 
             int tzhour=0, int tzmin=0) ;
  Timestamp( const Environment *env, int year,
             unsigned int month, unsigned int day, unsigned int hour,
             unsigned int min ,unsigned int sec, unsigned int fs, 
             const OCCI_STD_NAMESPACE::string &timezone);
  Timestamp( const Environment *env, int year,
             unsigned int month, unsigned int day, unsigned int hour,
             unsigned int min ,unsigned int sec, unsigned int fs, 
             const UString &timezone);
  Timestamp( const Timestamp &src) ;
  ~Timestamp();

  void getTimeZoneOffset( int &hour, int &minute) const ;
  void getTime( unsigned int &hour, unsigned int &minute,
  unsigned int &second, unsigned int &fs) const ;
  void getDate( int &year, unsigned int &month, unsigned int &day )const ;
  OCCI_STD_NAMESPACE::string toText(const OCCI_STD_NAMESPACE::string &fmt, 
                     unsigned int fsprec, 
                     const OCCI_STD_NAMESPACE::string &nlsParam ="") const   ;
  UString toText(const UString &fmt, 
                 unsigned int fsprec, const UString &nlsParam ) const   ;
  void setTimeZoneOffset( int hour,  int minute)  ;
  void setTime( unsigned int hour, unsigned int minute,
  unsigned int second, unsigned int fs)  ;
  void setDate( int year, unsigned int month, unsigned int day ) ;
  void setNull() ;
  void fromText( const OCCI_STD_NAMESPACE::string &timestmpStr, 
                 const OCCI_STD_NAMESPACE::string &fmt ,
                 const OCCI_STD_NAMESPACE::string &nlsParam= "", 
                 const Environment *env =NULL);
  void fromText( const UString &timestmpStr, 
                 const UString &fmt , const UString &nlsParam, 
                 const Environment *env =NULL);
  bool isNull() const;
  Timestamp & operator =( const Timestamp &src) ;
  const IntervalYM subYM(const Timestamp& val) const ;
  const IntervalDS subDS(const Timestamp& val) const ;
  const Timestamp intervalAdd(const IntervalDS& val) const ;
  const Timestamp intervalSub(const IntervalDS& val) const ;
  const Timestamp intervalAdd(const IntervalYM& val) const ;
  const Timestamp intervalSub(const IntervalYM& val) const ;

  friend bool operator==(const Timestamp &a,const Timestamp &b);
  friend bool operator>(const Timestamp &a,const Timestamp &b);
  friend bool operator<(const Timestamp &a,const Timestamp &b);
  friend bool operator !=(const Timestamp &a,const Timestamp &b);
  friend bool operator >=(const Timestamp &a,const Timestamp &b);
  friend bool operator <=(const Timestamp &a,const Timestamp &b);

  friend class ResultSetImpl;
  friend class StatementImpl;
  friend class AnyDataImpl;

  private:
   OCIDateTime *ocidatetime;
   Environment *env_; 
   void *timestampExt;

   OCIDateTime *getOCIDateTime() const;
   void getTZString( OraText *TimeZone, int th, int tm );  
   Timestamp( Environment *env, OCIDateTime *dt, bool toCopy = true) ;
   void allocateDataMembers( Environment *env) ;
   void do_TimestampConstruct( const Environment *env, int year, 
   unsigned int month, unsigned int day, unsigned int hour, unsigned int min, 
   unsigned int sec, unsigned int fs, void *tz, int tsize);

   friend  void getVector(const AnyData &any,
   OCCI_STD_NAMESPACE::vector<Timestamp> &vect);
   friend  void setVector(AnyData &any, 
   const OCCI_STD_NAMESPACE::vector<Timestamp> &vect) ;
   friend Timestamp MetaData::getTimestamp(
   MetaData::AttrId attrid) const ;
   friend void getVector(ResultSet  *rs, unsigned int,
   OCCI_STD_NAMESPACE::vector<Timestamp> &vect) ;
   friend void getVector(Statement  *rs, unsigned int,
   OCCI_STD_NAMESPACE::vector<Timestamp> &vect) ;
   friend void do_setVectorOfTimestamp(Statement *stmt,
   unsigned int paramIndex,
   const OCCI_STD_NAMESPACE::vector<Timestamp> &vect, 
   void *schemaName, unsigned int schemaNameLen,
   void *typeName, unsigned int typeNameLen) ;
}; // class Timestamp

class IntervalDS
{

  public:
  IntervalDS() ;
  IntervalDS( const Environment *env,int day=0, 
  int hour=0, int minute=0, int second=0, 
  int fs=0) ;
  IntervalDS( const IntervalDS &src) ;

  ~IntervalDS();

  int getDay () const ;
  int getHour () const ;
  int getMinute () const ;
  int getSecond() const ;
  int getFracSec () const ;
  void set( int day, int hour, int minute, int second, int fracsec) ;
  void setNull() ;
  void fromText( const OCCI_STD_NAMESPACE::string &inpstr,
  const OCCI_STD_NAMESPACE::string &nlsParam ="",
  const Environment *env=NULL)  ;
  OCCI_STD_NAMESPACE::string toText( unsigned int lfprec, unsigned int fsprec,
  const OCCI_STD_NAMESPACE::string &nlsParam="") const ;
  bool isNull() const;
  IntervalDS& operator =( const IntervalDS &src) ;
  IntervalDS& operator +=( const IntervalDS &a);
  IntervalDS& operator -=( const IntervalDS &a);
  IntervalDS& operator *=( const Number &factor);
  IntervalDS& operator /=( const Number &factor);

  friend bool operator>(const IntervalDS &a,
  const IntervalDS &b) ;
  friend bool operator<(const IntervalDS &a,
  const IntervalDS &b) ;
  friend bool operator >=( const IntervalDS &a, 
  const IntervalDS &b);
  friend bool operator <=( const IntervalDS &a, 
  const IntervalDS &b);
  
  //UTF16 support
  void fromUText( const UString &inpstr, const Environment *env=NULL );
  UString toUText( unsigned int lfprec, unsigned int fsprec) const;

  private:
   OCIInterval *ociinter;
   OCIEnv *ocienv;
   void *intervalDSExt;

   IntervalDS( OCIEnv *env, OCIInterval *inter, bool toCopy = true) ;
   OCIInterval * getOCIInterval() const;

   void allocateDataMembers( OCIEnv *env) ;
   friend const IntervalDS Timestamp::subDS(
   const Timestamp& val) const ;
   friend const Timestamp Timestamp::intervalAdd(
   const IntervalDS& val) const ;
   friend const Timestamp Timestamp::intervalSub(
   const IntervalDS& val) const ;
   friend class Date;
   friend void getVector(ResultSet  *rs, unsigned int,
   OCCI_STD_NAMESPACE::vector<IntervalDS> &vect) ;
   friend void getVector(Statement  *rs, unsigned int,
   OCCI_STD_NAMESPACE::vector<IntervalDS> &vect) ;
   friend void do_setVectorOfIntervalDS(Statement *stmt, 
   unsigned int paramIndex,
   const OCCI_STD_NAMESPACE::vector<IntervalDS> &vect, 
   void *schemaName, unsigned int schemaNameLen,
   void *typeName, unsigned int typeNameLen) ;
   friend class StatementImpl;
   friend class ResultSetImpl;
   friend class AnyDataImpl;
   friend  void getVector(const AnyData &any,
   OCCI_STD_NAMESPACE::vector<IntervalDS> &vect);
   friend  void setVector(AnyData &any,
   const OCCI_STD_NAMESPACE::vector<IntervalDS> &vect) ;

 
};  //class IntervalDS

class IntervalYM
{

  public:
  IntervalYM() ;
  IntervalYM( const Environment *env,int year=0, int month=0) ;
  IntervalYM( const IntervalYM &src) ;
  ~IntervalYM();

  int getYear() const ;
  int getMonth() const ;

  void set( int year, int month) ;
  void setNull() ;
  void fromText( const OCCI_STD_NAMESPACE::string &inpstr, 
  const OCCI_STD_NAMESPACE::string &nlsParam="",
  const Environment *env=NULL) ;
  OCCI_STD_NAMESPACE::string toText( unsigned int lfprec, 
  const OCCI_STD_NAMESPACE::string &nlsParam="") const;
  bool isNull() const;
  IntervalYM & operator =( const IntervalYM &src) ;
  IntervalYM& operator +=( const IntervalYM &a);
  IntervalYM& operator -=( const IntervalYM &a);
  IntervalYM& operator *=( const Number &factor);
  IntervalYM& operator /=( const Number &factor);

  friend bool operator>(const IntervalYM &a, const IntervalYM &b) ;
  friend bool operator<( const IntervalYM &a, const IntervalYM &b) ;
  friend bool operator >=(const IntervalYM &a, const IntervalYM &b);
  friend bool operator <=(const IntervalYM &a, const IntervalYM &b);
  
  //UTF16 support
  void fromUText( const UString &inpstr, const Environment *env=NULL );
  UString toUText( unsigned int lfprec ) const;

  private:
   OCIInterval *ociinter;
   OCIEnv *ocienv;
   void *intervalYMExt;

   IntervalYM( OCIEnv *env, OCIInterval *inter, bool toCopy = true) ;
   OCIInterval *getOCIInterval() const;
   void allocateDataMembers( OCIEnv *env) ;
   friend const IntervalYM Timestamp :: subYM(
   const Timestamp& val) const ;
   friend const Timestamp Timestamp::intervalAdd(
   const IntervalYM &val) const ;
   friend const Timestamp Timestamp::intervalSub(
   const IntervalYM &val) const ;

   friend void getVector(ResultSet  *rs, unsigned int,
   OCCI_STD_NAMESPACE::vector<IntervalYM> &vect) ;
   friend void getVector(Statement  *rs, unsigned int,
   OCCI_STD_NAMESPACE::vector<IntervalYM> &vect) ;
   friend void do_setVectorOfIntervalYM(Statement *stmt,
   unsigned int paramIndex,
   const OCCI_STD_NAMESPACE::vector<IntervalYM> &vect,
   void *schemaName, unsigned int schemaNameLen,
   void *typeName, unsigned int typeNameLen) ;

   friend class StatementImpl;
   friend class ResultSetImpl;
   friend class AnyDataImpl;
   friend  void getVector(const AnyData &any,
   OCCI_STD_NAMESPACE::vector<IntervalYM> &vect);
   friend  void setVector(AnyData &any,
   const OCCI_STD_NAMESPACE::vector<IntervalYM> &vect) ;

}; //class IntervalYM


Number operator+(const Number &a, const Number &b) ;
Number operator/(const Number &dividend, const Number &divisor) ;
Number operator*(const Number &a, const Number &b) ;
Number operator%(const Number &a, const Number &b) ;
Number operator-(const Number &subtrahend, const Number &subtractor) ;
bool operator==(const Number &a, const Number &b);
bool operator!=(const Number &a, const Number &b);

const IntervalYM operator+(const IntervalYM &a, const IntervalYM &b) ;
const IntervalYM operator-(const IntervalYM &a, const IntervalYM &b) ;
const IntervalYM operator*(const IntervalYM &a, const Number& factor);
const IntervalYM operator/(const IntervalYM &a, const Number &factor);
bool operator==(const IntervalYM &a, const IntervalYM &b) ;
bool operator!=(const IntervalYM &a, const IntervalYM &b) ;

const IntervalDS operator+(const IntervalDS &a, const IntervalDS &b) ;
const IntervalDS operator-(const IntervalDS &a, const IntervalDS &b) ;
const IntervalDS operator*(const IntervalDS &a, const Number& factor);
const IntervalDS operator/(const IntervalDS &a, const Number &factor);
bool operator==(const IntervalDS &a, const IntervalDS &b) ;
bool operator!=(const IntervalDS &a, const IntervalDS &b) ;


typedef struct BFloat
{
 float value;
 bool  isNull;

 BFloat()
 {
   isNull = false;
   value = 0.;
 }
} BFloat;

typedef struct BDouble
{
 double value;
 bool   isNull;

 BDouble()
 {
   isNull = false;
   value = 0.;
 }
} BDouble;

/*---------------------------------------------------------------------------
                           EXPORT FUNCTIONS
  ---------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------
                          INTERNAL FUNCTIONS
  ---------------------------------------------------------------------------*/

 
#ifdef ORAXB8_DEFINED
/* 
   NAME 
     Lob Region class

   DESCRIPTION 
     Contains the implementation of the Lob Region template Class.
     This class is the underlying implementation for the BlobRegion and
     ClobRegion classes.
 
   RELATED DOCUMENTS 
     Functional/Design Specifications:
       18209 - Next Generation LOBs: API
       18206 - Next Generation LOBs: Comb. Storage, Compressio & Encryption
 
   EXPORT FUNCTION(S) 
     LobRegion()  - constructors
     ~LobRegion() - destructor
     getPrimary() - Get the Primary Lob object
     getPrimaryOffset() - Get the offset of this region in the Primary Lob.
     getOffset() - Get the offset of this region in this lob.
     getLength() - Get the length of this region
     getMimeType() - Get the mime type of this region

   PUBLIC IMPLEMENTATION FUNCTION(S)

   INTERNAL FUNCTION(S)
     none

   EXAMPLES

   NOTES
*/

/*------------------------------ LobRegion ------------------*/
/*
   NAME
       LobRegion - constructor for the class

   PARAMETERS
       none

   DESCRIPTION 
       default constructor

   RETURNS
       Nothing

   NOTES
*/
template <typename lobType>
LobRegion<lobType>::LobRegion()
{
  _primary = (lobType *)0;
  _primaryOffset = 0;
  _offset  = 0;
  _length  = 0;
}

/*------------------------------ ~LobRegion ------------------*/
/*
   NAME
       ~LobRegion - destructor for the class

   PARAMETERS
       none

   DESCRIPTION 
       default constructor

   RETURNS
       Nothing

   NOTES
*/
template <typename lobType> 
LobRegion<lobType>::~LobRegion()
{
  if (_primary != (lobType *)0)
  {
    delete _primary;
  }
}

template <typename lobType> 
lobType *LobRegion<lobType>::getPrimary()
{
  return _primary;
}

template <typename lobType>
oraub8 LobRegion<lobType>::getPrimaryOffset()
{
  return _primaryOffset;
}

template <typename lobType>
oraub8 LobRegion<lobType>::getOffset()
{
  return _offset;
}

template <typename lobType>
oraub8 LobRegion<lobType>::getLength()
{
  return _length;
}

template <typename lobType>
OCCI_STD_NAMESPACE::string LobRegion<lobType>::getMimeType()
{
  return _mimeType;
}

template <typename lobType> 
void LobRegion<lobType>::setPrimary(const ConnectionImpl *connp,
                                    OCILobLocator *locator)
{
  if (locator != (OCILobLocator *)0)
  {
    _primary = new lobType(connp, locator, true);
  }
}

#endif /* ORAXB8_DEFINED */

} /* end of namespace occi */
} /* end of namespace oracle */
#endif                                              /* OCCIDATA_ORACLE */

#endif                                              /* _olint */
