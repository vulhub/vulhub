/* DISABLE check_long_lines */

/* Copyright (c) 1996, 2017, Oracle and/or its affiliates. 
All rights reserved.*/

/*
 * 
 */

/* 
 * NAME
 *    nzt.h
 * 
 * DESCRIPTION
 *    Toolkit public declarations.
 *    
 * PUBLIC FUNCTIONS
 *    nztwOpenWallet           - Open a wallet based on a WRL and pwd.
 *    nztwCloseWallet          - Close a wallet.
 * +  nztwCreateWallet         - Create a new wallet.
 * +  nztwDestroyWallet        - Destroy an existing wallet.
 *    nztwRetrievePersonaCopy  - Retieve a copy of a particular persona.
 * +  nzteStorePersona         - Store a persona in the wallet.
 *    nzteOpenPersona          - Open a persona.
 *    nzteClosePersona         - Close a persona.
 * +  nzteRemovePersona        - Remove a persona from a wallet.
 * +  nzteCreatePersona        - Create a persona.
 *    nzteDestroyPersona       - Destroy a persona.
 *    nztiStoreTrustedIdentity - Store an identity with associated trust.
 *    nzteRetrieveTrustedIdentCopy - Retrieves a trusted identity from persona
 * +  nzteSetProtection        - Modify the protection set in a persona.
 * +  nzteGetProtection        - Get the protection set in a persona
 *    nztePriKey               - Get the Private Key (X509 Only)
 *    nzteMyCert               - Get the Certificate (X509 only)
 *    nzteX509CreatePersona    - Create a persona given an X509 Certificate.
 * +  nztiRemoveIdentity       - Remove an identity from a persona.
 *    nztiCreateIdentity       - Create an identity.
 *    nztiDuplicateIdentity    - Create a complete copy of an identity.
 *    nztiAbortIdentity        - Discard an unstored identity.
 *    nztidGetIdentityDesc     - Gets Identity Description from Identity.
 *    nztidFreeIdentityDesc    - Frees memory for Identity Desc object.
 *    nztSign                  - Generate an attached signature.
 * +  nztxSignExpansion        - Determine size of signature.
 *    nztVerify                - Verify an attached signature.               
 *    nztValidate              - Validate an identity.                       
 *    nztsd_SignDetached       - Generate a detached signature.
 * +  nztxsd_SignDetachedExpansion - Determine size of detached signature.
 *    nztved_VerifyDetached    - Verify a detached signature.                 
 * +  nztEncrypt               - Symmetric key encryption.   
 * +  nztxEncryptExpansion     - Determine the tdu length for encryption.
 * +  nztDecrypt               - Symmetric key decryption.                   
 * +  nztEnvelope              - Sign then encrypt data for recipient(s).    
 * +  nztDeEnvelope            - Reverse nztEnvelope.                        
 * +  nztKeyedHash             - Generate keyed hash.                        
 * +  nztxKeyedHashExpansion   - Determine size of TDU for keyed hash.
 *    nztHash                  - Generate hash.   
 * +  nztxHashExpansion        - Determine the size of the TDU for a hash.
 *    nztSeedRandom            - See the random number generator.
 *    nztrb_RandomBytes        - Generate a series of random bytes.          
 *    nztrn_RandomNumber       - Generate a random number.                   
 *    nztbbInitBlock           - Initialize a buffer block.                  
 *    nztbbReuseBlock          - Reuse a buffer block.           
 *    nztbbSizeBlock           - Find the size of the buffer block.
 *    nztbbGrowBlock           - Grow initialized buffer block by 'inc' bytes.
 *    nztbbPurgeBlock          - Purge the memory used within a buffer block.
 *    nztbbSetBlock            - Set block to known state.
 *    nztkec_PKEncrypt         - Encrypt data then encrypt key for recipient.
 *    nztkdc_PKDecrypt         - Decrypt PKEncrypt'ed data.                  
 *    nztific_FreeIdentityContent - Free the contents of an identity.
 *    nztifdn                  - Create an identity from a distinguished name
 *    nztcts_CipherSpecToStr   - Converts the Cipher Spec Code To String
 *    nztiae_IsAuthEnabled     - Checks to see if Authentication is Enabled
 *                               in the current Cipher Spec.
 *    nztiae_IsEncrEnabled     - Checks to see if Encryption is Enabled
 *                               in the current Cipher Spec.
 *    nztiae_IsHashEnabled     - Checks to see if Hashing is Enabled
 *                               in the current Cipher Spec.
 *    nztwGetCertInfo          - Get peer certificate info
 *    nzICE_Install_Cert_ext   - Installs a Cert into a wallet with Trustflag
 *                               value set for the certificate.
 *
 * NOTE: the '+' indicates that these functions are UNSUPPORTED at this time.
 * 
 * NOTES
 *    
 * MODIFIED
 *    abjuneja   09/11/13 - Bug 14245960 - Wallet encryption algo upgrade
 *    nidgarg    05/07/13 - TF Support: Add functions for Trustflags
 *    shiahuan   11/28/07 - 
 *    skalyana   08/15/07 - 
 *    pkale      09/28/06 - Bug 5565668: Removed __STDC__
 *    tnallath   09/22/05 - 
 *    rchahal    07/27/04 - add keyusage 
 *    srtata     11/10/03 - fix nztSetAppDefaultLocation header 
 *    rchahal    10/15/03 - bug 2513821 
 *    rchahal    11/11/02 - pkcs11 support
 *    akoyfman   07/05/02 - adding secret store to persona
 *    supriya    10/11/01 - Fix for bug # 2015732
 *    ajacobs    04/04/01 - make NZT_REGISTRY_WRL always available
 *    ajacobs    03/06/01 - olint fix
 *    ajacobs    03/02/01 - Add GetCertInfo
 *    supriya    02/23/01 - Move nzttKPUsage from nzt0.h
 *    rchahal    01/26/01 - olint fixes
 *    supriya    12/07/00 - Change fn name
 *    supriya    12/01/00 - Certificate API's needed for iAS
 *    supriya    06/19/00 - Adding definitions for MCS and ENTR
 *    lkethana   05/31/00 - multiple cert support
 *    skanjila   06/25/99 - Remove nztcts_CipherSpecToStr() to NZOS.
 *    skanjila   06/23/99 - Change API of nztcts_CipherSpecToStr.
 *    lkethana   06/18/99 - rem nztIPrivateAlloc, etc
 *    lkethana   06/10/99 - changing size_t to ub4
 *    lkethana   06/02/99 - add api for getting auth/encry/hash capability of c
 *    arswamin   12/28/98 - add NZT_MAX_MD5.
 *    arswamin   12/21/98 - change signature of compareDN
 *    qdinh      12/21/98 - change size_t to ub4.
 *    inetwork   11/22/98 - Removing NZDEPRECATED definition
 *    amthakur   09/14/98 - deprecating and updating the c-structures.
 *    arswamin   09/24/98 - adding NZTTWRL_NULL for SSO support.
 *    amthakur   07/30/98 - changing the prototype of nztGetCertChain.
 *    qdinh      05/01/98 - add NZTTIDENTTYPE_INVALID_TYPE
 *    qdinh      04/17/98 - add NZTTWRL_ORACLE.
 *    ascott     10/08/97 - implement nztiStoreTrustedIdentity
 *    ascott     10/07/97 - add nztiGetIdentityDesc
 *    ascott     09/28/97 - clarify prototype comments and error codes
 *    ascott     09/05/97 - update identity: create, destroy, duplicate
 *    ascott     08/21/97 - add GetCert and GetPriKey
 *    ascott     08/07/97 - add other WRL settings
 *    asriniva   03/25/97 - Add ANSI prototypes
 *    rwessman   03/19/97 - Added prototypes for nztific_FreeIdentityContent()
 *    asriniva   03/11/97 - Fix olint errors
 *    sdange     02/28/97 - Removed inclusion of nz0decl.h
 *    sdange     02/18/97 - Moved nzt specific declarations from nz0decl.h
 *    asriniva   01/21/97 - Remove prototypes.
 *    asriniva   10/31/96 - Include oratypes.h
 *    asriniva   10/15/96 - Declare buffer block helper functions
 *    asriniva   10/08/96 - First pass at wallet open/close
 *    asriniva   10/04/96 - Add random number seed function
 *    asriniva   10/03/96 - Reorder parameters in nztbbSetBlock
 *    asriniva   10/03/96 - Keep editing.
 *    asriniva   10/03/96 - Continued edits.
 *    asriniva   10/02/96 - Continue editing.
 *    asriniva   09/26/96 -
 */
   
/* ENABLE check_long_lines */

#ifndef NZT_ORACLE
#define NZT_ORACLE

#ifndef ORATYPES
# include <oratypes.h>
#endif /* ORATYPES */

#ifndef NZERROR_ORACLE
# include <nzerror.h>         /* NZ error type */
#endif /* NZERROR_ORACLE */


#define NZT_MAX_SHA1 20
#define NZT_MAX_MD5  16

/***************************************/
/* PUBLIC CONSTANTS, MACROS, AND TYPES */
/***************************************/

/*
 * Wallet Resource Locator Type Strings
 *
 * WRL TYPE        PARAMETERS      BEHAVIOR
 * ========        ==========      =====================================
 * default:          <none>        Uses directory defined by the parameter
 *                                 SNZD_DEFAULT_FILE_DIRECTORY which in 
 *                                 unix is "$HOME/oracle/oss"
 * 
 * file:            file path      Find the Oracle wallet in this directory.
 *                                 example: file:<dir-path>
 * 
 * sqlnet:           <none>        In this case, the directory path will be 
 *                                 retrieved from the sqlnet.ora file under
 *                                 the oss.source.my_wallet parameter.
 *
 * mcs:              <none>        Microsoft WRL.
 *
 * entr:             dir path      Entrust WRL. eg: ENTR:<dir-path>   
 * 
 */
/* Note that there is no NZT_NULL_WRL.  Instead look in snzd.h for DEFAULT_WRP
 * which is used in our new defaulting mechanism.  The NZT_DEFAULT_WRL
 * should be deprecated.
 */
#define NZT_DEFAULT_WRL    ((text *)"default:")
#define NZT_SQLNET_WRL     ((text *)"sqlnet:")
#define NZT_FILE_WRL       ((text *)"file:")
#define NZT_ENTR_WRL       ((text *)"entr:")
#define NZT_MCS_WRL        ((text *)"mcs:")
#define NZT_ORACLE_WRL      ((text *)"oracle:")
#define NZT_REGISTRY_WRL   ((text *)"reg:")
#define NZT_MEMORY_WRL       ((text *)"memory:")

#define  NZ_TF_SERVER_AUTH  256 /* Trusted server CA certificate */
#define  NZ_TF_CLIENT_AUTH 512  /* Trusted Client CA certificate */
#define  NZ_TF_VALID_PEER 1024  /* Peer's User Certificate */
#define  NZ_TF_USER_CERT 2048   /* User certificate, only for NZ use */
#define  NZ_TF_NULL 4096        /* NULL trustflag to certificate */
#define  NZ_TF_TRUSTED 8192     /* Trusted certificate */
#define  NZ_TF_NONE 16384       /* Used by internal functions when Wallet doesn't support TF */

typedef int nzTrustFlag;
           
enum nzttwrl 
{
   NZTTWRL_DEFAULT = 1,    /* Default, use SNZD_DEFAULT_FILE_DIRECTORY */
   NZTTWRL_SQLNET,         /* Use oss.source.my_wallet in sqlnet.ora file */
   NZTTWRL_FILE,           /* Find the oracle wallet in this directory */
   NZTTWRL_ENTR,           /* Find the entrust profile in this directory */
   NZTTWRL_MCS,            /* WRL for Microsoft */
   NZTTWRL_ORACLE,      /* Get the wallet from OSS db */
   NZTTWRL_NULL,           /* New SSO defaulting mechanism */
   NZTTWRL_REGISTRY,       /* Find the wallet in Windows Registry */
   NZTTWRL_MEMORY          /* Read the wallet from Memory/Buffer */
};
typedef enum nzttwrl nzttwrl;

#ifndef NZ0DECL_ORACLE
   /*
    * With the elimination of nz0decl.h from public, we need this
    * redundant typedef.
    */
   typedef struct nzctx nzctx;
   typedef struct nzstrc nzstrc;
   typedef struct nzosContext nzosContext;
#endif /* NZ0DECL_ORACLE */

/* Moved from nz0decl.h */

typedef struct nzttIdentity nzttIdentity;
typedef struct nzttIdentityPrivate nzttIdentityPrivate;
typedef struct nzttPersona nzttPersona;
typedef struct nzttPersonaList nzttPersonaList;
typedef struct nzttPersonaPrivate nzttPersonaPrivate;
typedef struct nzttWallet nzttWallet;
typedef struct nzttWalletPrivate nzttWalletPrivate;
typedef struct nzttWalletObj nzttWalletObj; /* For wallet object */
typedef struct nzssEntry nzssEntry; /* For secretstore */
typedef struct nzpkcs11_Info nzpkcs11_Info;
typedef struct nzpkcs12_Info nzpkcs12_Info;
typedef struct nzttTrustInfo nzttTrustInfo;

/*
 * Crypto Engine State
 *
 * Once the crypto engine (CE) has been initialized for a particular
 * cipher, it is either at the initial state, or it is continuing to
 * use the cipher.  NZTCES_END is used to change the state back to
 * initialized and flush any remaining output.  NZTTCES_RESET can be
 * used to change the state back to initialized and throw away any
 * remaining output.
 */
enum nzttces 
{
   NZTTCES_CONTINUE = 1,    /* Continue processing input */
   NZTTCES_END,             /* End processing input */
   NZTTCES_RESET            /* Reset processing and skip generating output */
};
typedef enum nzttces nzttces;

/*
 * Crypto Engine Functions
 *
 * List of crypto engine categories; used to index into protection
 * vector.
 */
enum nzttcef
{
   NZTTCEF_DETACHEDSIGNATURE = 1,   /* Signature, detached from content */
   NZTTCEF_SIGNATURE,               /* Signature combined with content */
   NZTTCEF_ENVELOPING,              /* Signature and encryption with content */
   NZTTCEF_PKENCRYPTION,            /* Encryption for one or more recipients */
   NZTTCEF_ENCRYPTION,              /* Symmetric encryption */
   NZTTCEF_KEYEDHASH,               /* Keyed hash/checkusm */
   NZTTCEF_HASH,                    /* Hash/checsum */
   NZTTCEF_RANDOM,                  /* Random byte generation */

   NZTTCEF_LAST                     /* Used for array size */
};
typedef enum nzttcef nzttcef;

/*
 * State of the persona.
 */
enum nzttState
{
   NZTTSTATE_EMPTY = 0,     /* is not in any state(senseless???) */
   NZTTSTATE_REQUESTED,     /* cert-request */
   NZTTSTATE_READY,         /* certificate */
   NZTTSTATE_INVALID,         /* certificate */
   NZTTSTATE_RENEWAL        /* renewal-requested */
};
typedef enum nzttState nzttState;

/*
 * Cert-version types
 * 
 * This is used to quickly look-up the cert-type
 */
enum nzttVersion
{
   NZTTVERSION_X509v1 = 1,        /* X.509v1 */
   NZTTVERSION_X509v3,            /* X.509v3 */
#ifdef NZDEPRECATED
   NZTTVERSION_SYMMETRIC,         /* Symmetric */
#endif
   NZTTVERSION_INVALID_TYPE       /* For Initialization */
};
typedef enum nzttVersion nzttVersion;

/*
 * Cipher Types
 *
 * List of all cryptographic algorithms, some of which may not be
 * available.
 */
enum nzttCipherType 
{
   NZTTCIPHERTYPE_INVALID = 0,
   NZTTCIPHERTYPE_RSA = 1,          /* RSA public key */
   NZTTCIPHERTYPE_DES,              /* DES */
   NZTTCIPHERTYPE_RC4,              /* RC4 */
   NZTTCIPHERTYPE_MD5DES,           /* DES encrypted MD5 with salt (PBE) */
   NZTTCIPHERTYPE_MD5RC2,           /* RC2 encrypted MD5 with salt (PBE) */
   NZTTCIPHERTYPE_MD5,              /* MD5 */
   NZTTCIPHERTYPE_SHA,               /* SHA */
   NZTTCIPHERTYPE_ECC               /* ECC */
};
typedef enum nzttCipherType nzttCipherType;

/*
 * TDU Formats
 *
 * List of possible toolkit data unit (TDU) formats.  Depending on the
 * function and cipher used some may be not be available.
 */
enum nztttdufmt
{
   NZTTTDUFMT_PKCS7 = 1,            /* PKCS7 format */
   NZTTTDUFMT_RSAPAD,               /* RSA padded format */
   NZTTTDUFMT_ORACLEv1,             /* Oracle v1 format */
   NZTTTDUFMT_LAST                  /* Used for array size */
};
typedef enum nztttdufmt nztttdufmt;

/*
 * Validate State
 *
 * Possible validation states an identity can be in.
 */
enum nzttValState
{
   NZTTVALSTATE_NONE = 1,        /* Needs to be validated */
   NZTTVALSTATE_GOOD,            /* Validated */
   NZTTVALSTATE_REVOKED          /* Failed to validate */
};
typedef enum nzttValState nzttValState;

/*
 * Policy Fields <----NEW (09/14/98)
 *
 * Policies enforced
 */
enum nzttPolicy
{
   NZTTPOLICY_NONE = 0,
   NZTTPOLICY_RETRY_1, /* number of retries for decryption = 1 */
   NZTTPOLICY_RETRY_2, /* number of retries for decryption = 2 */
   NZTTPOLICY_RETRY_3  /* number of retries for decryption = 3 */
};
typedef enum nzttPolicy nzttPolicy;

/*
 * Persona Usage <----NEW (09/14/98)
 *
 * what a persona will be used for?
 */

#ifdef NZDEPRECATED_MULTIPLECERTS
enum nzttUsage
{
   NZTTUSAGE_NONE = 0,
   NZTTUSAGE_SSL  /* persona for SSL usage */
};
typedef enum nzttUsage nzttUsage;
#endif

/*
 * Personas and identities have unique id's that are represented with
 * 128 bits.
 */
typedef ub1 nzttID[16];

/*
 * Identity Types
 *
 * List of all Identity types..
 */
enum nzttIdentType 
{
   NZTTIDENTITYTYPE_INVALID_TYPE = 0,  
   NZTTIDENTITYTYPE_CERTIFICTAE,      
   NZTTIDENTITYTYPE_CERT_REQ,      
   NZTTIDENTITYTYPE_RENEW_CERT_REQ,      
   NZTTIDENTITYTYPE_CLEAR_ETP,      
   NZTTIDENTITYTYPE_CLEAR_UTP,      
   NZTTIDENTITYTYPE_CLEAR_PTP       
};
typedef enum nzttIdentType nzttIdentType;

typedef ub4 nzttKPUsage;
/* IF new types are added nztiMUS should be changed */
#define NZTTKPUSAGE_NONE 0
#define NZTTKPUSAGE_SSL 1             /* SSL Server */
#define NZTTKPUSAGE_SMIME_ENCR 2
#define NZTTKPUSAGE_SMIME_SIGN 4
#define NZTTKPUSAGE_CODE_SIGN 8
#define NZTTKPUSAGE_CERT_SIGN 16
#define NZTTKPUSAGE_SSL_CLIENT 32     /* SSL Client */
#define NZTTKPUSAGE_INVALID_USE 0xffff


/*
 * Timestamp as 32 bit quantity in UTC.
 */
typedef ub1 nzttTStamp[4];

/*
 * Buffer Block
 *
 * A function that needs to fill (and possibly grow) an output buffer
 * uses an output parameter block to describe each buffer.
 *
 * The flags_nzttBufferBlock member tells the function whether the
 * buffer can be grown or not.  If flags_nzttBufferBlock is 0, then
 * the buffer will be realloc'ed automatically.  
 *
 * The buflen_nzttBufferBLock member is set to the length of the
 * buffer before the function is called and will be the length of the
 * buffer when the function is finished.  If buflen_nzttBufferBlock is
 * 0, then the initial pointer stored in pobj_nzttBufferBlock is
 * ignored.
 *
 * The objlen_nzttBufferBlock member is set to the length of the
 * object stored in the buffer when the function is finished.  If the
 * initial buffer had a non-0 length, then it is possible that the
 * object length is shorter than the buffer length.
 *
 * The pobj_nzttBufferBlock member is a pointer to the output object.
 */
struct nzttBufferBlock
{
# define NZT_NO_AUTO_REALLOC     0x1

   uword flags_nzttBufferBlock;     /* Flags */
   ub4 buflen_nzttBufferBlock;   /* Total length of buffer */
   ub4 usedlen_nzttBufferBlock;  /* Length of used buffer part */
   ub1 *buffer_nzttBufferBlock;     /* Pointer to buffer */
};
typedef struct nzttBufferBlock nzttBufferBlock;

/*
 * Wallet.
 */
struct nzttWallet
{
   ub1 *ldapName_nzttWallet;              /* user's LDAP Name */
   ub4  ldapNamelen_nzttWallet;           /* len of user's LDAP Name */
   nzttPolicy securePolicy_nzttWallet;    /* secured-policy of the wallet */
   nzttPolicy openPolicy_nzttWallet;      /* open-policy of the wallet */
   nzttPersona *persona_nzttWallet;       /* List of personas in wallet */
   nzttWalletPrivate *private_nzttWallet; /* Private wallet information */
#ifdef NZDEPRECATED
   ub4 npersona_nzttWallet;            /* Number of personas */
#endif
};

struct nzttTrustInfo
{
   ub4 serverAuthTFcount_nzttTrustInfo;    /* Number of certificates with
                                                 * SERVER_AUTH trust flag
                                                 */

   ub4 clientAuthTFcount_nzttTrustInfo;    /* Number of certificates with
                                                 * CLIENT_AUTH trust flag
                                                 */

   ub4 peerAuthTFcount_nzttTrustInfo;      /* Number of certificates with
                                                 * PEER_AUTH trust flag
                                                 */
};

/*
 * The wallet contains, one or more personas.  A persona always
 * contains its private key and its identity.  It may also contain
 * other 3rd party identites.  All identities qualified with trust
 * where the qualifier can indicate anything from untrusted to trusted
 * for specific operations.
 */

/*
 * Persona
 *
 * Structure containing information about a persona.
 */
struct nzttPersona
{
   ub1 *genericName_nzttPersona;              /* user-friendly persona name  */
   ub4  genericNamelen_nzttPersona;           /* persona-name length */
   nzttPersonaPrivate *private_nzttPersona;   /* Opaque part of persona */
   nzttIdentity *mycertreqs_nzttPersona;      /* My cert-requests */
   nzttIdentity *mycerts_nzttPersona;         /* My certificates */
   nzttIdentity *mytps_nzttPersona;           /* List of trusted identities */
   nzssEntry *mystore_nzttPersona;            /* List of secrets */
   nzpkcs11_Info *mypkcs11Info_nzttPersona;   /* PKCS11 token info */
   struct nzttPersona *next_nzttPersona;      /* Next persona */
   boolean bTrustFlagEnabled_nzttPersona;     /* Persona supports Cert with trustflags */
   nzttTrustInfo trustinfo;                   /* Number of certs with C,T,P trustflag
                                               * in Persona 
                                               */
   nzpkcs12_Info *p12Info_nzttPersona;        /* PKCS12 Info */
#ifdef NZDEPRECATED_MULTIPLECERTS
  /* As Persona has multiple certs for different
     usages, Persona Usage does not mean anything. Similarly
     each key pair has its own state and Persona state itself
     does not mean anything. - lk 5/31/00
  */
   nzttUsage usage_nzttPersona;               /* persona usage; SSL/SET/.. */
   nzttState state_nzttPersona;               /* persona state-requested/ready */
   ub4 ntps_nzttPersona;                   /* Num of trusted identities */
#endif
};

/* Persona List */
struct nzttPersonaList
{
struct nzttPersonaList *next_nzttPersonaList;
nzttPersona * mynzttPersona;
};

/*
 * Identity
 *
 * Structure containing information about an identity.
 *
 * NOTE
 *  -- the next_trustpoint field only applies to trusted identities and
 *     has no meaning (i.e. is NULL) for self identities.
 */
struct nzttIdentity
{
   text *dn_nzttIdentity;                      /* Alias */
   ub4 dnlen_nzttIdentity;                  /* Length of alias */
   text *comment_nzttIdentity;                 /* Comment  */
   ub4 commentlen_nzttIdentity;             /* Length of comment */
   nzttIdentityPrivate *private_nzttIdentity;  /* Opaque part of identity */
   nzttIdentity *next_nzttIdentity;            /* next identity in list */
};

struct nzttB64Cert
{
   ub1 *b64Cert_nzttB64Cert;
   ub4  b64Certlen_nzttB64Cert;
   struct nzttB64Cert *next_nzttB64Cert;
};
typedef struct nzttB64Cert nzttB64Cert;


struct nzttPKCS7ProtInfo
{
   nzttCipherType mictype_nzttPKCS7ProtInfo;    /* Hash cipher */
   nzttCipherType symmtype_nzttPKCS7ProtInfo;   /* Symmetric cipher */
   ub4 keylen_nzttPKCS7ProtInfo;             /* Length of key to use */
};
typedef struct nzttPKCS7ProtInfo nzttPKCS7ProtInfo;

/*
 * Protection Information.
 *
 * Information specific to a type of protection.
 */
union nzttProtInfo
{
   nzttPKCS7ProtInfo pkcs7_nzttProtInfo;
};
typedef union nzttProtInfo nzttProtInfo;

/*
 * A description of a persona so that the toolkit can create one.  A
 * persona can be symmetric or asymmetric and both contain an
 * identity.  The identity for an asymmetric persona will be the
 * certificate and the identity for the symmetric persona will be
 * descriptive information about the persona.  In either case, an
 * identity will have been created before the persona is created.
 *
 * A persona can be stored separately from the wallet that references
 * it.  By default, a persona is stored with the wallet (it inherits
 * with WRL used to open the wallet).  If a WRL is specified, then it
 * is used to store the actuall persona and the wallet will have a
 * reference to it.
 */
struct nzttPersonaDesc
{
   ub4 privlen_nzttPersonaDesc;        /* Length of private info (key)*/
   ub1 *priv_nzttPersonaDesc;             /* Private information */
   ub4 prllen_nzttPersonaDesc;         /* Length of PRL */
   text *prl_nzttPersonaDesc;             /* PRL for storage */
   ub4 aliaslen_nzttPersonaDesc;       /* Length of alias */
   text *alias_nzttPersonaDesc;           /* Alias */
   ub4 longlen_nzttPersonaDesc;        /* Length of longer description*/
   text *long_nzttPersonaDesc;            /* Longer persona description */
};
typedef struct nzttPersonaDesc nzttPersonaDesc;

/*
 * A description of an identity so that the toolkit can create one.
 * Since an identity can be symmetric or asymmetric, the asymmetric
 * identity information will not be used when a symmetric identity is
 * created.  This means the publen_nzttIdentityDesc and
 * pub_nzttIdentityDesc members will not be used when creating a
 * symmetric identity.
 */
struct nzttIdentityDesc
{
   ub4 publen_nzttIdentityDesc;        /* Length of identity */
   ub1 *pub_nzttIdentityDesc;             /* Type specific identity */
   ub4 dnlen_nzttIdentityDesc;         /* Length of alias */
   text *dn_nzttIdentityDesc;             /* Alias */
   ub4 longlen_nzttIdentityDesc;       /* Length of longer description */
   text *long_nzttIdentityDesc;           /* Longer description */
   ub4 quallen_nzttIdentityDesc;       /* Length of trust qualifier */
   text *trustqual_nzttIdentityDesc;      /* Trust qualifier */
};
typedef struct nzttIdentityDesc nzttIdentityDesc;

/********************************/
/* PUBLIC FUNCTION DECLARATIONS */
/********************************/

/*---------------------- nztwOpenWallet ----------------------*/

/*
 * NAME
 *    nztwOpenWallet - Open a wallet based on a wallet Resource Locator (WRL).
 * 
 * PARAMETERS
 *    osscntxt {IN}     OSS context. 
 *    wrllen   {IN}     Length of WRL.
 *    wrl      {IN}     WRL.
 *    pwdlen   {IN}     Length of password.
 *    pwd      {IN}     Password.
 *    wallet   {IN/OUT} Initialized wallet structure.   
 * 
 * NOTES
 *    The syntax for a WRL is <Wallet Type>:<Wallet Type Parameters>.
 *
 *    Wallet Type       Wallet Type Parameters.
 *    -----------       ----------------------
 *    File              Pathname (e.g. "file:/home/asriniva")
 *    Oracle            Connect string (e.g. "oracle:scott/tiger@oss")
 *
 *    There are also defaults.  If the WRL is NZT_DEFAULT_WRL, then
 *    the platform specific WRL default is used.  If only the wallet
 *    type is specified, then the WRL type specific default is used
 *    (e.g. "oracle:")
 *
 *    There is an implication with Oracle that should be stated: An
 *    Oracle based wallet can be implemented in a user's private space
 *    or in world readable space.
 *
 *    When the wallet is opened, the password is verified by hashing
 *    it and comparing against the password hash stored with the
 *    wallet.  The list of personas (and their associated identities)
 *    is built and stored into the wallet structure.
 *    
 * RETURNS
 *    NZERROR_OK           Success.
 *    NZERROR_RIO_OPEN     RIO could not open wallet (see network trace file).
 *    NZERROR_TK_PASSWORD  Password verification failed.
 *    NZERROR_TK_WRLTYPE   WRL type is not known.
 *    NZERROR_TK_WRLPARM   WRL parm does not match type.
 */
nzerror nztwOpenWallet( nzctx *, ub4, text *, ub4, text *, 
                           nzttWallet * );


/*---------------------- nztwCloseWallet ----------------------*/

/*
 * NAME
 *    nztwCloseWallet - Close a wallet
 * 
 * PARAMETERS
 *    osscntxt {IN}     OSS context.
 *    wallet   {IN/OUT} Wallet.
 * 
 * NOTES
 *    Closing a wallet also closes all personas associated with that
 *    wallet.  It does not cause a persona to automatically be saved
 *    if it has changed.  The implication is that a persona can be
 *    modified by an application but if it is not explicitly saved it
 *    reverts back to what was in the wallet.
 *
 * RETURNS
 *    NZERROR_OK           Success.
 *    NZERROR_RIO_CLOSE    RIO could not close wallet (see network trace file).
 */
nzerror nztwCloseWallet( nzctx *, nzttWallet * );

/*--------------------nztwGetCertInfo----------------------------*/
/****NOTE: This function is a temporary hack.****/
/****DO NOT CALL.  It will soon disappear.****/
nzerror nztwGetCertInfo( nzctx *nz_context,
                            nzosContext *nzosCtx,
                            nzttWallet *walletRef,
                            void *peerCert );


/*------------------------ nztwConstructWallet -----------------------*/
/*
 * 
 * nzerror nztwConstructWallet( nzctx *oss_context,    
 *              nzttPolicy openPolicy,
 *              nzttPolicy securePolicy,
 *              ub1 *ldapName,
 *              ub4 ldapNamelen,
 *              nzstrc *wrl,
 *              nzttPersona *personas,
 *              nzttWallet **wallet );
 */

/*---------------------- nztwRetrievePersonaCopy ----------------------*/

/*
 * NAME
 *    nztwRetrievePersonaCopy - Retrieves a persona based from wallet
 * 
 * PARAMETERS
 *    osscntxt {IN}     OSS context. 
 *    wallet   {IN}     Wallet.
 *    index    {IN}     Which wallet index to remove (first persona is zero).
 *    persona  {OUT}    Persona found.
 * 
 * NOTES
 *    Retrieves a persona from the wallet based on the index number passed
 *    in.  This persona is a COPY of the one stored in the wallet, therefore
 *    it is perfectly fine for the wallet to be closed after this call is 
 *    made.
 *
 *    The caller is responsible for disposing of the persona when completed.
 *
 * RETURNS
 *    NZERROR_OK           Success.
 */
nzerror nztwRetrievePersonaCopy( nzctx *, nzttWallet *, ub4, 
                           nzttPersona ** );


/*---------------------- nztwRetrievePersonaCopyByName ----------------------*/

/*
 * NAME
 *    nztwRetrievePersonaCopyByName - Retrieves a persona based on its name.
 *
 * PARAMETERS
 *    osscntxt {IN}     OSS context.
 *    wallet   {IN}     Wallet.
 *    name    {IN}     Name of the persona 
 *    persona  {OUT}    Persona found.
 *
 * NOTES
 *    Retrieves a persona from the wallet based on the name of the persona. 
 *    This persona is a COPY of the one stored in the wallet, therefore
 *    it is perfectly fine for the wallet to be closed after this call is
 *    made.
 *
 *    The caller is responsible for disposing of the persona when completed.
 *
 * RETURNS
 *    NZERROR_OK           Success.
 */
nzerror nztwRetrievePersonaCopyByName( nzctx *, nzttWallet *, char *,
                           nzttPersona ** );

/*---------------------- nzteOpenPersona ----------------------*/

/*
 * NAME
 *    nzteOpenPersona - Open a persona.
 * 
 * PARAMETERS
 *    osscntxt {IN}     OSS context. 
 *    persona  {IN/OUT} Persona.
 * 
 * NOTES
 *    
 * RETURNS
 *    NZERROR_OK           Success.
 *    NZERROR_TK_PASSWORD  Password failed to decrypt persona.
 *    NZERROR_TK_BADPRL    Persona resource locator did not work.
 *    NZERROR_RIO_OPEN     Could not open persona (see network trace file).
 */
nzerror nzteOpenPersona( nzctx *, nzttPersona * );

/*--------------------- nzteClosePersona ---------------------*/

/*
 * NAME
 *    nzteClosePersona - Close a persona.
 * 
 * PARAMETERS
 *    osscntxt {IN}     OSS context.
 *    persona  {IN/OUT} Persona.
 * 
 * NOTES
 *    Closing a persona does not store the persona, it simply releases
 *    the memory associated with the crypto engine.
 *    
 * RETURNS
 *    NZERROR_OK        Success.
 */
nzerror nzteClosePersona( nzctx *, nzttPersona * );

/*--------------------- nzteDestroyPersona ---------------------*/

/*
 * NAME
 *    nzteDestroyPersona - Destroy a persona.
 * 
 * PARAMETERS
 *    osscntxt {IN}        OSS context.
 *    persona  {IN/OUT}    Persona.
 * 
 * NOTES
 *    The persona is destroyd in the open state, but it will
 *    not be associated with a wallet.
 *    
 *    The persona parameter is doubly indirect so that at the
 *    conclusion of the function, the pointer can be set to NULL.
 *
 *
 * RETURNS
 *    NZERROR_OK        Success.
 *    NZERROR_TK_TYPE   Unsupported itype/ctype combination.
 *    NZERROR_TK_PARMS  Error in persona description.
 */
nzerror nzteDestroyPersona( nzctx *, nzttPersona ** );

/*---------------------- nzteRetrieveTrustedIdentCopy ----------------------*/

/*
 * NAME
 *    nzteRetrieveTrustedIdentCopy - Retrieves a trusted identity from persona
 * 
 * PARAMETERS
 *    osscntxt {IN}     OSS context. 
 *    persona  {IN}     Persona.
 *    index    {IN}     Which wallet index to remove (first element is zero).
 *    identity {OUT}    Trusted Identity from this persona.
 * 
 * NOTES
 *    Retrieves a trusted identity from the persona based on the index 
 *    number passed in.  This identity is a COPY of the one stored in 
 *    the persona, therefore it is perfectly fine to close the persona
 *    after this call is made.
 *
 *    The caller is responsible for freeing the memory of this object 
 *    by calling nztiAbortIdentity it is no longer needed
 *
 * RETURNS
 *    NZERROR_OK           Success.
 */
nzerror nzteRetrieveTrustedIdentCopy( nzctx *, nzttPersona *, ub4, 
                           nzttIdentity ** );

/*--------------------- nztePriKey ---------------------*/

/*
 * NAME
 *    nztePriKey - Get the decrypted Private Key for the Persona
 * 
 * PARAMETERS
 *    osscntxt {IN}     OSS context.
 *    persona  {IN}     Persona.
 *    vkey     {OUT}    Private Key [B_KEY_OBJ]
 *    vkey_len {OUT}    Private Key Length
 * 
 * NOTES
 *    This funiction will only work for X.509 based persona which contain
 *    a private key.  
 *    A copy of the private key is returned to the caller so that they do not 
 *    have to worry about the key changeing "underneath them".
 *    Memory will be allocated for the vkey and therefore, the CALLER
 *    will be responsible for freeing this memory.
 *
 * RETURNS
 *    NZERROR_OK           Success.
 *    NZERROR_NO_MEMORY    ossctx is null.
 *    NZERROR_TK_BADPRL    Persona resource locator did not work.
 */
nzerror nztePriKey( nzctx *, nzttPersona *, ub1 **, ub4 * );

/*--------------------- nzteMyCert ---------------------*/

/*
 * NAME
 *    nzteMyCert - Get the X.509 Certificate for a persona
 * 
 * PARAMETERS
 *    osscntxt {IN}     OSS context.
 *    persona  {IN}     Persona.
 *    cert     {OUT}    X.509 Certificate [BER encoded]
 *    cert_len {OUT}    Certificate length
 * 
 * NOTES
 *    This funiction will only work for X.509 based persona which contain
 *    a certificate for the self identity. 
 *    A copy of the certificate is returned to the caller so that they do not 
 *    have to worry about the certificate changeing "underneath them".
 *    Memory will be allocated for the cert and therefore, the CALLER
 *    will be responsible for freeing this memory.
 *
 * RETURNS
 *    NZERROR_OK           Success.
 *    NZERROR_NO_MEMORY    ossctx is null.
 */
nzerror nzteMyCert( nzctx *, nzttPersona *, ub1 **, ub4 * );

/*--------------------- nzteX509CreatePersona ---------------------*/

/*
 * NAME
 *    nzteX509CreatePersona - Given a BER X.509 cert, create a persona
 * 
 * PARAMETERS
 *    osscntxt {IN}     OSS context.
 *    cert     {IN}     X.509 Certificate [BER encoded]
 *    cert_len {IN}     Certificate length
 *    persona  {OUT}    Persona.
 * 
 * NOTES
 *    Memory will be allocated for the persona and therefore, the CALLER
 *    will be responsible for freeing this memory.
 *
 * RETURNS
 *    NZERROR_OK           Success.
 *    NZERROR_NO_MEMORY    ossctx is null.
 */
nzerror nzteX509CreatePersona( nzctx *, ub1 *, ub4, nzttPersona ** );

/*-------------------- nztiCreateIdentity --------------------*/

/*
 * NAME
 *    nztiCreateIdentity - Create an identity.
 * 
 * PARAMETERS
 *    osscntxt {IN}     OSS context.
 *    itype    {IN}     Identity type.
 *    desc     {IN}     Description of identity.
 *    identity {IN/OUT} Identity.
 * 
 * NOTES
 *    Memory is only allocated for the identity structure.  The elements in
 *    the description struct are not copied.  Rather their pointers are copied
 *    into the identity structure.  Therefore, the caller should not free
 *    the elements referenced by the desc.  These elements will be freed
 *    when the nztiDestroyIdentity is called.
 *    
 * RETURNS
 *    NZERROR_OK        Success.
 *    NZERROR_PARMS     Error in description.
 */
nzerror nztiCreateIdentity( nzctx *, nzttVersion, nzttIdentityDesc *,
                               nzttIdentity ** );

#ifdef NZ_OLD_TOOLS
/*-------------------- nztiDuplicateIdentity --------------------*/

/*
 * NAME
 *    nztiDuplicateIdentity - Duplicate an identity.
 * 
 * PARAMETERS
 *    osscntxt     {IN}     OSS context.
 *    identity     {IN}     Target Identity.
 *    new_identity {IN}     New Identity.
 * 
 * NOTES
 *    Memory for the identity is allocated inside the function, and all
 *    internal identity elements as well.
 *    
 * RETURNS
 *    NZERROR_OK        Success.
 *    NZERROR_TK_NOTFOUND  Identity not found.
 *    NZERROR_PARMS     Error in description.
 */
nzerror nztiDuplicateIdentity( nzctx *, nzttIdentity *, 
                              nzttIdentity ** );
#endif /* NZ_OLD_TOOLS */

/*--------------------- nztiAbortIdentity ---------------------*/

/*
 * NAME
 *    nztiAbortIdentity - Abort an unassociated identity.
 * 
 * PARAMETERS
 *    osscntxt {IN}     OSS context.
 *    identity {IN/OUT} Identity.
 * 
 * NOTES
 *    It is an error to try to abort an identity that can be
 *    referenced through a persona.
 *    
 *    The identity pointer is set to NULL at the conclusion.
 * 
 * RETURNS
 *    NZERROR_OK           Success.
 *    NZERROR_CANTABORT    Identity is associated with persona.
 */
nzerror nztiAbortIdentity( nzctx *, nzttIdentity ** );

#ifdef NZ_OLD_TOOLS
/*----------------- nztidGetIdentityDesc -----------------*/

/*
 * NAME
 *    nztidGetIdentityDesc - Gets an Identity Description from the identity
 * 
 * PARAMETERS
 *    osscntxt    {IN}     Success.
 *    identity    {IN}     Identity.
 *    description {IN/OUT} Identity Description.
 * 
 * NOTES
 *    Memory is allocated for the Identity Description. It
 *    is the callers responsibility to free this memory by calling 
 *    nztiFreeIdentityDesc.
 *
 * RETURNS
 *    NZERROR_OK     Success.
 */
nzerror nztidGetIdentityDesc( nzctx *, nzttIdentity *,
                                     nzttIdentityDesc ** );

/*----------------- nztidFreeIdentityDesc -----------------*/

/*
 * NAME
 *    nztidFreeIdentityDesc - Frees memory for Identity Desc object.
 * 
 * PARAMETERS
 *    osscntxt    {IN}     oss context.
 *    description {IN/OUT} Identity Description.
 * 
 * NOTES
 *    Memory is freed for all Identity description elements.  Pointer is 
 *    then set to null.
 *
 * RETURNS
 *    NZERROR_OK     Success.
 */
nzerror nztidFreeIdentityDesc( nzctx *, nzttIdentityDesc ** );
#endif /* NZ_OLD_TOOLS */

/*---------------- nztific_FreeIdentityContent ----------------*/

/*
 * NAME
 *     nztific_FreeIdentityContent - free the contents of an identity.
 * 
 * PARAMETERS
 *    osscntxt         {IN}     OSS context.
 *    identity         {IN/OUT} freed identity
 * 
 * NOTES
 *    Free a created identity.
 *
 * RETURNS
 *    NZERROR_OK     Success.
 */
/*
 * Free the identity content.
 */
nzerror nztific_FreeIdentityContent( nzctx *ossctx,
                                        nzttIdentity *identity );


/*-------------------------- nztSign --------------------------*/

/*
 * NAME
 *    nztSign - Create an attached signature.
 * 
 * PARAMETERS
 *    osscntxt {IN}     OSS context.
 *    persona  {IN}     Open persona acting as signer.
 *    state    {IN}     State of signature.
 *    inlen    {IN}     Length of this input part.
 *    in       {IN}     This input part.
 *    tdubuf   {IN/OUT} TDU buffer.
 * 
 * NOTES
 *    
 * RETURNS
 *    NZERROR_OK           Success.
 *    NZERROR_TK_CANTGROW  Needed to grow output buffer but could not.
 *    NZERROR_TK_NOTOPEN   Persona is not open.
 *    NZERROR_TK_NOTSUPP   Function not supported with persona.
 */
nzerror nztSign( nzctx *, nzttPersona *, nzttces, ub4, ub1 *,
                    nzttBufferBlock * );

/*------------------------- nztVerify -------------------------*/

/*
 * NAME
 *    nztVerify - Verify an attached signature.
 * 
 * PARAMETERS
 *    osscntxt {IN}     OSS context.
 *    persona  {IN}     Persona.
 *    state    {IN}     State of verification.
 *    intdulen {IN}     TDU length.
 *    intdu    {IN}     TDU.
 *    out      {IN/OUT} Extracted message.
 *    verified {OUT}    TRUE if signature verified.
 *    validated{OUT}    TRUE if signing identity validated.
 *    identity {OUT} Identity of signing party.
 * 
 * NOTES
 *    
 * RETURNS
 *    NZERROR_OK           Success.
 *    NZERROR_TK_CANTGROW  Needed to grow outptu buffer but could not.
 *    NZERROR_TK_NOTOPEN   Persona is not open.
 *    NZERROR_TK_NOTSUPP   Function not supported with persona.
 */
nzerror nztVerify( nzctx *, nzttPersona *, nzttces, ub4, ub1 *,
                      nzttBufferBlock *, boolean *, boolean *,
                      nzttIdentity ** );

/*------------------------ nztValidate ------------------------*/

/*
 * NAME
 *    nztValidate - Validate an identity.
 * 
 * PARAMETERS
 *    osscntxt {IN}  OSS context.
 *    persona  {IN}  Persona.
 *    identity {IN}  Identity.
 *    validated{OUT} TRUE if identity was validated.  
 * 
 * NOTES
 *    
 * RETURNS
 *    NZERROR_OK           Success.
 *    NZERROR_TK_NOTOPEN   Persona is not open.
 *    NZERROR_TK_NOTSUPP   Function not supported with persona.
 */
nzerror nztValidate( nzctx *, nzttPersona *, nzttIdentity *, boolean * );

/*-------------------- nztsd_SignDetached --------------------*/

/*
 * NAME
 *    nztsd_SignDetached - Generate a detached signature.
 * 
 * PARAMETERS
 *    osscntxt {IN}     OSS context.
 *    persona  {IN}     Persona.
 *    state    {IN}     State of signature.
 *    inlen    {IN}     Length of this input part.
 *    in       {IN}     This input part.
 *    tdubuf   {IN/OUT} TDU buffer.
 * 
 * NOTES
 *    
 * RETURNS
 *    NZERROR_OK           Success.
 *    NZERROR_TK_CANTGROW  Needed to grow output buffer but could not.
 *    NZERROR_TK_NOTOPEN   Persona is not open.
 *    NZERROR_TK_NOTSUPP   Function not supported with persona.
 */
nzerror nztsd_SignDetached( nzctx *, nzttPersona *, nzttces, ub4, ub1 *, 
                               nzttBufferBlock * );

/*------------------- nztved_VerifyDetached -------------------*/

/*
 * NAME
 *    nztved_VerifyDetached - Verify a detached signature.
 * 
 * PARAMETERS
 *    osscntxt {IN}     OSS context.
 *    persona  {IN}     Persona.
 *    state    {IN}     State of verification.
 *    inlen    {IN}     Length of data.
 *    in       {IN}     Data.
 *    intdulen {IN}     Input TDU length.
 *    tdu      {IN}     Input TDU.
 *    verified {OUT}    TRUE if signature verified.
 *    validated{OUT}    TRUE if signing identity validated.
 *    identity {OUT} Identity of signing party.
 * 
 * NOTES
 *    
 * RETURNS
 *    NZERROR_OK           Success.
 *    NZERROR_TK_NOTOPEN   Persona is not open.
 *    NZERROR_TK_NOTSUPP   Function not supported with persona.
 */
nzerror nztved_VerifyDetached( nzctx *, nzttPersona *, nzttces, ub4, 
                                  ub1 *, ub4, ub1 *, boolean *, boolean *, 
                                  nzttIdentity ** );

/*--------------------- nztkec_PKEncrypt ---------------------*/

/*
 * NAME
 *    nztkec_PKEncrypt - Encrypt data symmetrically, encrypt key asymmetrically
 * 
 * PARAMETERS
 *    osscntxt    {IN}     OSS context.
 *    persona     {IN}     Persona.
 *    nrecipients {IN}     Number of recipients for this encryption.
 *    recipients  {IN}     List of recipients.
 *    state       {IN}     State of encryption.
 *    inlen       {IN}     Length of this input part.
 *    in          {IN}     This input part.
 *    tdubuf      {IN/OUT} TDU buffer.
 * 
 * NOTES
 *    There is a limitation of 1 recipient (nrecipients = 1) at this
 *    time.
 *    
 * RETURNS
 *    NZERROR_OK           Success.
 *    NZERROR_TK_CANTGROW  Needed to grow output buffer but could not.
 *    NZERROR_TK_NOTOPEN   Persona is not open.
 *    NZERROR_TK_NOTSUPP   Function not supported with persona.
 */
nzerror nztkec_PKEncrypt( nzctx *, nzttPersona *, ub4, nzttIdentity *, 
                             nzttces, ub4, ub1 *, nzttBufferBlock * );

/*---------------- nztxkec_PKEncryptExpansion ----------------*/

/*
 * NAME
 *    nztxkec_PKEncryptExpansion - Determine the buffer needed for PKEncrypt
 * 
 * PARAMETERS
 *    osscntxt    {IN}     OSS context. 
 *    persona     {IN}     Persona.
 *    nrecipients {IN}     Number of recipients.
 *    inlen       {IN}     Length of input.
 *    tdulen      {out}    Length of buffer need.
 * 
 * NOTES
 *    
 * RETURNS
 *    NZERROR_OK           Success.
 *    NZERROR_TK_NOTOPEN   Persona is not open.
 *    NZERROR_TK_NOTSUPP   Function not supported with persona.
 */
nzerror nztxkec_PKEncryptExpansion( nzctx *, nzttPersona *, ub4, ub4,
                                       ub4 * );

/*--------------------- nztkdc_PKDecrypt ---------------------*/

/*
 * NAME
 *    nztkdc_PKDecrypt - Decrypt a PKEncrypted message.
 * 
 * PARAMETERS
 *    osscntxt    {IN}     OSS context.
 *    persona     {IN}     Persona.
 *    state       {IN}     State of encryption.
 *    inlen       {IN}     Length of this input part.
 *    in          {IN}     This input part.
 *    tdubuf      {IN/OUT} TDU buffer.
 * 
 * NOTES
 *    
 * RETURNS
 *    NZERROR_OK           Success.
 *    NZERROR_TK_CANTGROW  Needed to grow output buffer but could not.
 *    NZERROR_TK_NOTOPEN   Persona is not open.
 *    NZERROR_TK_NOTSUPP   Function not supported with persona.
 */
nzerror nztkdc_PKDecrypt( nzctx *, nzttPersona *, nzttces, ub4, ub1 *,
                             nzttBufferBlock * );

/*-------------------------- nztHash --------------------------*/

/*
 * NAME
 *    nztHash - Generate a hash.
 * 
 * PARAMETERS
 *    osscntxt {IN}     OSS context.
 *    persona  {IN}     Persona.
 *    state    {IN}     State of hash.
 *    inlen    {IN}     Length of this input.
 *    in       {IN}     This input.
 *    tdu      {IN/OUT} Output tdu.
 * 
 * NOTES
 *    
 * RETURNS
 *    NZERROR_OK           Success.
 *    NZERROR_TK_CANTGROW  Needed to grow TDU buffer but could not.
 *    NZERROR_TK_NOTOPEN   Persona is not open.
 *    NZERROR_TK_NOTSUPP   Function not supported with persona.
 */
nzerror nztHash( nzctx *, nzttPersona *, nzttces, ub4, ub1 *,
                    nzttBufferBlock * );

/*----------------------- nztSeedRandom -----------------------*/

/*
 * NAME
 *    nztSeedRandom - Seed the random function
 * 
 * PARAMETERS
 *    osscntxt {IN}  OSS context.
 *    persona  {IN}  Persona.
 *    seedlen  {IN}  Length of seed.
 *    seed     {IN}  Seed.
 * 
 * NOTES
 *    
 * RETURNS
 *    NZERROR_OK           Success.
 *    NZERROR_TK_NOTOPEN   Persona is not open.
 *    NZERROR_TK_NOTSUPP   Function not supported with persona.
 */
nzerror nztSeedRandom( nzctx *, nzttPersona *, ub4, ub1 * );

/*--------------------- nztrb_RandomBytes ---------------------*/

/*
 * NAME
 *    nztrb_RandomBytes - Generate a buffer random bytes.
 * 
 * PARAMETERS
 *    osscntxt {IN}     OSS context.
 *    persona  {IN}     Persona.
 *    nbytes   {IN}     Number of bytes desired.
 *    out      {IN/OUT} Buffer block for bytes.
 * 
 * NOTES
 *    
 * RETURNS
 *    NZERROR_OK           Success.
 *    NZERROR_TK_CANTGROW  Needed to grow TDU buffer but could not.
 *    NZERROR_TK_NOTOPEN   Persona is not open.
 *    NZERROR_TK_NOTSUPP   Function not supported with persona.
 */
nzerror nztrb_RandomBytes( nzctx *, nzttPersona *, ub4, 
                              nzttBufferBlock * );

/*-------------------- nztrn_RandomNumber --------------------*/

/*
 * NAME
 *    nztrn_RandomNumber - Generate a random number
 * 
 * PARAMETERS
 *    osscntxt {IN}     OSS context.
 *    persona  {IN}     Persona.
 *    num      {OUT}    Number.
 * 
 * NOTES
 *    
 * RETURNS
 *    NZERROR_OK           Success.
 *    NZERROR_TK_NOTOPEN   Persona is not open.
 *    NZERROR_TK_NOTSUPP   Function not supported with persona.
 */
nzerror nztrn_RandomNumber( nzctx *, nzttPersona *, uword * );

/*---------------------- nztbbInitBlock ----------------------*/

/*
 * NAME
 *    nztbbInitBlock - Initialize a buffer block.
 * 
 * PARAMETERS
 *    osscntxt {IN}     OSS context.
 *    block    {IN/OUT} Buffer block.
 * 
 * NOTES
 *    The buffer block is initialized to be empty (all members are set
 *    to 0/NULL).  Such a block will be allocated memory as needed.
 *    
 * RETURNS
 *    NZERROR_OK        Success.
 */
nzerror nztbbInitBlock( nzctx *, nzttBufferBlock * );

/*---------------------- nztbbReuseBlock ----------------------*/

/*
 * NAME
 *    nztbbReuseBlock - Reuse an already initialized and possibly used block.
 * 
 * PARAMETERS
 *    osscntxt {IN}     OSS context.
 *    block    {IN/OUT} Buffer block.
 * 
 * NOTES
 *    This function simply sets the used length member of the buffer
 *    block to 0.  If the block already has memory allocated to it,
 *    this will cause it to be reused.
 *    
 * RETURNS
 *    NZERROR_OK        Success.
 */
nzerror nztbbReuseBlock( nzctx *, nzttBufferBlock * );

/*---------------------- nztbbSizeBlock ----------------------*/

/*
 * NAME
 *    nztbbSizeBlock - Size an initialized block to a particular size.
 * 
 * PARAMETERS
 *    osscntxt {IN}     OSS context.
 *    len      {IN}     Minimum number of unused bytes desired.
 *    block    {IN/OUT} Buffer block.
 *
 * NOTES
 *    
 * RETURNS
 *    NZERROR_OK        Success.
 */
nzerror nztbbSizeBlock( nzctx *, ub4, nzttBufferBlock * );

/*----------------------- nztbbGrowBlock -----------------------*/

/*
 * NAME
 *    nzbbGrowBlock - Increase the size of the buffer block.
 * 
 * PARAMETERS
 *    osscntxt {IN}     OSS context.
 *    inc      {IN}     Number of bytes to increase.
 *    block    {IN/OUT} Buffer block.
 *
 * NOTES
 *    
 * RETURNS
 *    NZERROR_OK        Success.
 */
nzerror nztbbGrowBlock( nzctx *, ub4, nzttBufferBlock * );

/*---------------------- nztbbPurgeBlock ----------------------*/

/*
 * NAME
 *    nztbbPurgeBlock - Purge a buffer block of its memory.
 * 
 * PARAMETERS
 *    osscntxt {IN}     OSS context.
 *    block    {IN/OUT} Buffer block.
 * 
 * NOTES
 *    The memory used by the buffer block as the buffer is released.
 *    The buffer block itself is not affected.
 *    
 * RETURNS
 *    NZERROR_OK        Success.
 */
nzerror nztbbPurgeBlock( nzctx *, nzttBufferBlock * );

/*----------------------- nztbbSetBlock -----------------------*/

/*
 * NAME
 *    nztbbSetBlock - Set a buffer block to a known state.
 * 
 * PARAMETERS
 *    osscntxt {IN}     OSS context.
 *    flags    {IN}     Flags to set.
 *    buflen   {IN}     Length of buffer.
 *    usedlen  {IN}     Used length.
 *    buffer   {IN}     Buffer.
 *    block    {IN/OUT} Buffer block
 * 
 * NOTES
 *    If buflen > 0, objlen == 0, and obj == NULL, then buflen bytes
 *    of memory is allocated and a pointer is stored in the buffer
 *    block.
 *
 *    The buffer parameter remains unchanged.
 *    
 * RETURNS
 *    NZERROR_OK        Success.
 */
nzerror nztbbSetBlock( nzctx *,  uword, ub4, ub4, ub1 *, 
                          nzttBufferBlock * );


/*--------------------- nztiGetSecInfo ---------------------*/

/*
 * NAME
 *    nztiGetSecInfo - Get some security information for SSL
 * 
 * PARAMETERS
 *    Name   {IN/OUT}   Description
 *       osscntxt {IN}        OSS context.
 *       persona  {IN}      persona
 *       dname    {OUT}       distinguished name of the certificate
 *       dnamelen {OUT}       length of the distinguished name 
 *       issuername {OUT}     issuer name of the certificate
 *       certhash {OUT}      SHA1 hash of the certificate
 *   certhashlen{OUT}   length of the hash
 * NOTES
 *    This function allocate memories for issuername, certhash, and dname.
 *   To deallocate memory for those params, you should call nztdbuf_DestroyBuf.
 * RETURNS
 *    
 */
nzerror nztiGetSecInfo( nzctx *, nzttPersona *, text **, ub4 *,
            text **, ub4 *, ub1 **, ub4 * );


/*---------------------- nztiGetDName ----------------------*/

/*
 * NAME
 *    nztiGetDName - Get the distinguished name for the given identity
 * 
 * PARAMETERS
 *    Name   {IN/OUT}   Description
 *       osscntxt {IN}        OSS context.
 *   identity {IN}      identity need to get dname from
 *   dn   {OUT}      distinguished name
 *   dnlen   {OUT}      length of the dname
 *
 * NOTES
 *    
 * RETURNS
 *    
 */

nzerror nztiGetDName( nzctx *, nzttIdentity *,
              text **, ub4 * );

/*------------------- nztiGetIssuerName -------------------*/

/*
 * NAME
 *    nztiGetIssuerName -  Get IssuerName for the given identity 
 * 
 * PARAMETERS
 *    Name   {IN/OUT}   Description
 *       osscntxt {IN}        OSS context.
 *   identity {IN}      identity need to get issuername from
 *   issuername {OUT}          issuer's name
 *   issuernamelen {OUT}   length of the issuer's name
 * 
 * NOTES
 *    
 * RETURNS
 *    
 */
nzerror nztiGetIssuerName( nzctx *, nzttIdentity *,
               text **, ub4 * );


/*-------------------- nztgch_GetCertHash --------------------*/

/*
 * NAME
 *    nztgch_GetCertHash -  Get SHA1 hash for the certificate of the identity 
 * 
 * PARAMETERS
 *    Name   {IN/OUT}   Description
 *       osscntxt {IN}        OSS context.
 *   identity {IN}      identity need to get issuername from
 *   certHash {OUT}          certHash buffer 
 *   hashLen {OUT}      length of the certHash 
 * 
 * NOTES
 *    Need to call nztdbuf_DestroyBuf to deallocate memory for certHash.   
 * RETURNS
 *    
 */
nzerror nztgch_GetCertHash( nzctx *, const nzttIdentity *,
              ub1 **, ub4 * );

/*-------------------- nztdbuf_DestroyBuf --------------------*/

/*
 * NAME
 *    nztdbuf_DestroyBuf - Deallocation funtions for ub1 and text buffer 
 * 
 * PARAMETERS
 *    Name   {IN/OUT}   Description
 *       osscntxt {IN}        OSS context.
 *   buf      {IN}      Allocated buffer to be destroyed.
 * 
 * NOTES
 *    
 * RETURNS
 *    
 */
nzerror nztdbuf_DestroyBuf( nzctx *, void  ** );


/*----------------------- nztGetCertChain -----------------------*/

/*
 * NAME
 *    nztGetCertChain - 
 * 
 * PARAMETERS
 *    Name   {IN/OUT}   Description
 *       osscntxt {IN}        OSS context.
 * 
 * NOTES
 *    
 * RETURNS
 *    
 */
nzerror nztGetCertChain( nzctx *, nzttWallet * );

/*----------------------- nztCompareDN -----------------------*/

/*
 * NAME
 *    nztCompareDN - 
 * 
 * PARAMETERS
 *    Name   {IN/OUT}   Description
 *       osscntxt {IN}        OSS context.
 *   dn1   {IN}      distinguished name 1
 *   dn2     {IN}      distinguished name 2
 * 
 * NOTES
 *    
 * RETURNS
 *    NZERROR_OK       succeeded
 *   others         failed
 *    
 */
nzerror nztCompareDN( nzctx *, ub1 *,ub4 ,  ub1 *, ub4, boolean * );


#ifdef NZ_OLD_TOOLS
/*--------------------- nztIdentityAlloc ---------------------*/

/*
 * NAME
 *    nztIdentityAlloc - Allocate memory for nzttIdentity context
 * 
 * PARAMETERS
 *    Name   {IN/OUT}   Description
 *       osscntxt {IN}        OSS context.
 *   identity {OUT}      nzttIdentity context
 * 
 * NOTES
 *    
 * RETURNS
 *    NZERROR_OK       succeeded
 *   others         failed
 *    
 */
nzerror nztIdentityAlloc( nzctx *, nzttIdentity ** );

/*--------------------- nztIPrivateAlloc ---------------------*/

/*
 * NAME
 *    nztIPrivateAlloc - Allocate memory for nzttIdentityPrivate
 * 
 * PARAMETERS
 *    Name   {IN/OUT}   Description
 * 
 *       osscntxt {IN}        OSS context.
 *   ipriv   {OUT}      identityPrivate structure
 * NOTES
 *    
 * RETURNS
 *    NZERROR_OK       succeeded
 *   others         failed
 *    
 */

nzerror nztIPrivateAlloc( nzctx *, nzttIdentityPrivate **);


/*---------------------- nztIDupContent ----------------------*/

/*
 * NAME
 *    nztIDupContent - 
 * 
 * PARAMETERS
 *    Name   {IN/OUT}   Description
 *       osscntxt {IN}        OSS context.
 *    targetIdentity{OUT}   target identity
 *   sourceIdentity {IN}   source identity
 * NOTES
 *    
 * RETURNS
 *    NZERROR_OK       succeeded
 *   others         failed
 *    
 */

nzerror nztIDupContent( nzctx *, nzttIdentity *, nzttIdentity * );
/*---------------------- nztIPDuplicate ----------------------*/

/*
 * NAME
 *    nztIPDuplicate - 
 * 
 * PARAMETERS
 *    Name   {IN/OUT}   Description
 *       osscntxt {IN}        OSS context.
 *   target_ipriv {OUT}   target identityPrivate
 *   source_ipriv {IN}   source identityPrivate
 * 
 * NOTES
 *    
 * RETURNS
 *    NZERROR_OK       succeeded
 *   others         failed
 *    
 */
nzerror nztIPDuplicate( nzctx *, nzttIdentityPrivate **,
          nzttIdentityPrivate * );

/*--------------------- nztiDupIdentList ---------------------*/

/*
 * NAME
 *    nztiDupIdentList - 
 * 
 * PARAMETERS
 *    Name   {IN/OUT}   Description
 *       osscntxt {IN}        OSS context.
 *   source_identities {IN}   source identity list
 *   numIdent {OUT}      number of identity in the list
 *   ppidentity {OUT}   Target of identity   
 *   
 * NOTES
 *    
 * RETURNS
 *    NZERROR_OK       succeeded
 *   others         failed
 *    
 */
nzerror nztiDupIdentList( nzctx *, nzttIdentity  *, ub4 *, 
                             nzttIdentity ** );

/*--------------------- nztFreeIdentList ---------------------*/

/*
 * NAME
 *    nztFreeIdentList - Free memory for a list of Identities
 * 
 * PARAMETERS
 *    Name   {IN/OUT}   Description
 *       osscntxt {IN}        OSS context.
 *   identity {IN}      identity context
 *
 * NOTES
 *    
 * RETURNS
 *    NZERROR_OK       succeeded
 *   others         failed
 *    
 */
nzerror nztFreeIdentList( nzctx *, nzttIdentity ** );
#endif /* NZ_OLD_TOOLS */

/*--------------------- nztCheckVaLidity ---------------------*/

/*
 * NAME
 *    nztCheckVaLidity -  Check the validity of certificate
 * 
 * PARAMETERS
 *    Name   {IN/OUT}   Description
 *       osscntxt {IN}        OSS context.
 *   start_time      Start time of the certificate
 *   end_time      End time of the certificate
 * 
 * NOTES
 *    
 * RETURNS
 *    NZERROR_OK       succeeded
 *   others         failed
 *    
 */
nzerror nztCheckValidity( nzctx *, ub4 , ub4 );

nzerror nztCheckValidity_ext( nzctx *, ub8 , ub8 );

/*--------------------- nztwCreateWallet ---------------------*/

/*
 * NAME
 *    nztwCreateWallet - Create a new wallet.
 * 
 * PARAMETERS
 *    osscntxt {IN}     OSS context.
 *    wrllen   {IN}     Length of wallet resource locator.
 *    wrl      {IN}     WRL.
 *    pwdlen   {IN}     Length of password (see notes below).
 *    pwd      {IN}     Password.
 *    wallet   {IN/OUT} Wallet. 
 * 
 * NOTES
 *    It is an error to try to create a wallet that already exists.
 *    The previously existing wallet must be destroyed first.
 *
 *    The wallet itself is not encrypted.  Rather, all the personas in
 *    the wallet are encrypted under the same password.  A hash of the
 *    password is stored in the wallet.
 *
 *    Upon success, an empty open wallet is stored in the wallet
 *    parameter.
 *    
 * RETURNS
 *    NZERROR_OK                 Sucess.
 *    NZERROR_TK_WALLET_EXISTS   Wallet already exists.
 *    NZERROR_RIO_OPEN           RIO could not create wallet (see trace file).
 */
nzerror nztwCreateWallet( nzctx *, ub4, text *, ub4, text *, 
                             nzttWallet * );


/*--------------------- nztwDestroyWallet ---------------------*/

/*
 * NAME
 *    nztwDestroyWallet - Destroy an existing wallet.
 * 
 * PARAMETERS
 *    osscntxt {IN}     OSS context.
 *    wrllen   {IN}     Length of wallet resource locator.
 *    wrl      {IN}     WRL.
 *    pwdlen   {IN}     Length of password.
 *    pwd      {IN}     Password.
 * NOTES
 *    
 * RETURNS
 *    NZERROR_OK           Success.
 *    NZERROR_TK_PASSWORD  Password verification failed.
 *    NZERROR_RIO_OPEN     RIO could not open wallet (see trace file).
 *    NZERROR_RIO_DELETE   Delete failed (see trace file).
 */
nzerror nztwDestroyWallet( nzctx *, ub4, text *, ub4, text * );

/*--------------------- nzteStorePersona ---------------------*/

/*
 * NAME
 *    nzteStorePersona - Store an open persona in a wallet.
 * 
 * PARAMETERS
 *    osscntxt {IN}     OSS context. 
 *    persona  {IN/OUT} Persona.
 *    wallet   {IN/OUT} Wallet.
 * 
 * NOTES
 *    If the open persona is not associated with any wallet (it was
 *    created via the nzteClosePersona function), then storing the
 *    persona creates that association.  The wallet will also have an
 *    updated persona list that reflects this association.
 *
 *    If the open persona was associated with wallet 'A' (it was
 *    opened via the nztwOpenWallet function), and is stored back into
 *    wallet 'A', then then the old persona is overwritten by the new
 *    persona if the password can be verified.  Recall that all
 *    personas have a unique identity id.  If that id changes then
 *    storing the persona will put a new persona in the wallet.
 *
 *    If the open persona was associated with wallet 'A' and is stored
 *    into wallet 'B', and if wallet 'B' does not contain a persona
 *    with that unique identity id, then the persona will be copied
 *    into wallet 'B', wallet 'B''s persona list will be updated, and
 *    the persona structure will be updated to be associated with
 *    wallet 'B'.  If wallet 'B' already contained the persona, it
 *    would be overwritten by the new persona.
 *
 *    The persona parameter is doubly indirect so that at the
 *    conclusion of the function call, the pointer can be directed to
 *    the persona in the wallet.
 *
 * RETURNS
 *    NZERROR_OK           Success.
 *    NZERROR_TK_PASSWORD  Password verification failed.
 *    NZERROR_RIO_STORE    Store failed (see network trace file).
 */
nzerror nzteStorePersona( nzctx *, nzttPersona **, nzttWallet * );

/*--------------------- nzteRemovePersona ---------------------*/

/*
 * NAME
 *    nzteRemovePersona - Remove a persona from the wallet.
 * 
 * PARAMETERS
 *    osscntxt {IN}     OSS context.
 *    persona  {IN/OUT} Persona.
 * 
 * NOTES
 *    The password is verified before trying to remove the persona.
 *    
 *    If the persona is open, it is closed.  The persona is removed
 *    from the wallet list and the persona pointer is set to NULL.
 *    
 *    A double indirect pointer to the persona is required so that the
 *    persona pointer can be set to NULL upon completion.
 *
 * RETURNS
 *    NZERROR_OK           Success.
 *    NZERROR_TK_PASSWORD  Password verification failed.
 *    NZERROR_RIO_DELETE   Delete failed.
 */
nzerror nzteRemovePersona( nzctx *, nzttPersona ** );

/*--------------------- nzteCreatePersona ---------------------*/

/*
 * NAME
 *    nzteCreatePersona - Create a persona.
 * 
 * PARAMETERS
 *    osscntxt {IN}     OSS context.
 *    itype    {IN}     Identity type.
 *    ctype    {IN}     Cipher type.
 *    desc     {IN}     Persona description.
 *    persona  {OUT}    Persona.
 * 
 * NOTES
 *    The resulting persona is created in the open state, but it will
 *    not be associated with a wallet.
 *    
 *    The memory for the persona is allocated by the function.
 *
 * RETURNS
 *    NZERROR_OK        Success.
 *    NZERROR_TK_TYPE   Unsupported itype/ctype combination.
 *    NZERROR_TK_PARMS  Error in persona description.
 */
nzerror nzteCreatePersona( nzctx *, nzttVersion, nzttCipherType, 
                              nzttPersonaDesc *, nzttPersona ** );


/*----------------- nztiStoreTrustedIdentity -----------------*/

/*
 * NAME
 *    nztiStoreTrustedIdentity - Store an identity into a persona.
 * 
 * PARAMETERS
 *    osscntxt {IN}     Success.
 *    identity {IN/OUT} Trusted Identity.
 *    persona  {IN/OUT} Persona.    
 * 
 * NOTES
 *    The identity is not saved with the persona in the wallet until
 *    the persona is stored.
 *    
 *    The identity parameter is double indirect so that it can point
 *    into the persona at the conclusion of the call.
 *
 * RETURNS
 *    NZERROR_OK     Success.
 */
nzerror nztiStoreTrustedIdentity( nzctx *, nzttIdentity **,
                                     nzttPersona * );

/*--------------------- nzteSetProtection ---------------------*/

/*
 * NAME
 *    nzteSetProtection - Set the protection type for a CE function.
 * 
 * PARAMETERS
 *    osscntxt {IN}     OSS context.
 *    persona  {IN/OUT} Persona.
 *    func     {IN}     CE function.
 *    tdufmt   {IN}     TDU Format.
 *    protinfo {IN}     Protection information specific to this format.
 * 
 * NOTES
 *    
 * RETURNS
 *    NZERROR_OK              Success.
 *    NZERROR_TK_PROTECTION   Unsupported protection.
 *    NZERROR_TK_PARMS        Error in protection info.
 */
nzerror nzteSetProtection( nzctx *, nzttPersona *, nzttcef, nztttdufmt,
                              nzttProtInfo * );

/*--------------------- nzteGetProtection ---------------------*/

/*
 * NAME
 *    nzteGetProtection - Get the protection type for a CE function.
 * 
 * PARAMETERS
 *    osscntxt {IN}     OSS context.
 *    persona  {IN}     Persona.
 *    func     {IN}     CE function.
 *    tdufmt   {OUT}    TDU format.
 *    protinfo {OUT}    Protection information.
 * 
 * NOTES
 *    
 * RETURNS
 *    NZERROR_OK        Success.
 */
nzerror nzteGetProtection( nzctx *, nzttPersona *, nzttcef, nztttdufmt *,
                              nzttProtInfo * );

/*-------------------- nztiRemoveIdentity --------------------*/

/*
 * NAME
 *    nztiRemoveIdentity - Remove an identity from an open persona.
 * 
 * PARAMETERS
 *    osscntxt {IN}     OSS context. 
 *    identity {IN/OUT}     Identity.
 * 
 * NOTES
 *    If the persona is not stored, this identity will still be in the
 *    persona stored in the wallet.
 *    
 *    The identity parameter is doubly indirect so that at the
 *    conclusion of the function, the pointer can be set to NULL.
 *
 * RETURNS
 *    NZERROR_OK           Success.
 *    NZERROR_TK_NOTFOUND  Identity not found.
 *    NZERROR_TK_NOTOPEN   Persona is not open.
 */
nzerror nztiRemoveIdentity( nzctx *, nzttIdentity ** );

/*----------------- nztifdn -----------------*/

/*
 * NAME
 *    nztifdn - create an Identity From a Distinguished Name
 * 
 * PARAMETERS
 *    osscntxt           {IN}  OSS context.
 *    length             {IN}  Length of the distinguished name
 *    distinguished_name {IN}  distinguished name string
 *    ppidentity         {OUT} created identity
 * 
 * NOTES
 *    Given a distinguished name, return the identity that corresponds to it.
 *
 * RETURNS
 *    NZERROR_OK     Success.
 */
nzerror nztifdn( nzctx         *ossctx,
                    ub4         length,
                    text          *distinguished_name,
                    nzttIdentity **ppidentity );

/*--------------------- nztxSignExpansion ---------------------*/

/*
 * NAME
 *    nztxSignExpansion - Determine the size of the attached signature buffer.
 * 
 * PARAMETERS
 *    osscntxt {IN}  OSS context.
 *    persona  {IN}  Persona.
 *    inlen    {IN}  Length of input.
 *    tdulen   {OUT} Buffer needed for signature.
 *
 * NOTES
 *    
 * RETURNS
 *    NZERROR_OK           Success.
 *    NZERROR_TK_NOTOPEN   Persona is not open.
 *    NZERROR_TK_NOTSUPP   Function not supported with persona.
 */
nzerror nztxSignExpansion( nzctx *, nzttPersona *, ub4, ub4 * );

/*--------------- nztxsd_SignDetachedExpansion ---------------*/

/*
 * NAME
 *    nztxsd_SignDetachedExpansion - Determine the size of buffer needed.
 * 
 * PARAMETERS
 *    osscntxt {IN}  OSS context.
 *    persona  {IN}  Persona.
 *    inlen    {IN}  Length of input.
 *    tdulen   {OUT} Buffer needed for signature.
 * 
 * NOTES
 *    
 * RETURNS
 *    NZERROR_OK           Success.
 *    NZERROR_TK_NOTOPEN   Persona is not open.
 *    NZERROR_TK_NOTSUPP   Function not supported with persona.
 */
nzerror nztxsd_SignDetachedExpansion( nzctx *, nzttPersona *, ub4,
                                         ub4 * );

/*------------------------ nztEncrypt ------------------------*/

/*
 * NAME
 *    nztEncrypt - Symmetrically encrypt
 * 
 * PARAMETERS
 *    osscntxt    {IN}     OSS context.
 *    persona     {IN}     Persona.
 *    inlen       {IN}     Length of this input part.
 *    in          {IN}     This input part.
 *    tdubuf      {IN/OUT} TDU buffer.
 * 
 * NOTES
 *    
 * RETURNS
 *    NZERROR_OK           Success.
 *    NZERROR_TK_CANTGROW  Needed to grow TDU buffer but could not.
 *    NZERROR_TK_NOTOPEN   Persona is not open.
 *    NZERROR_TK_NOTSUPP   Function not supported with persona.
 */
nzerror nztEncrypt( nzctx *, nzttPersona *, nzttces, ub4, ub1 *,
                       nzttBufferBlock * );

/*------------------- nztxEncryptExpansion -------------------*/

/*
 * NAME
 *    nztxEncryptExpansion - Determine the size of the TDU to encrypt.
 * 
 * PARAMETERS
 *    osscntxt    {IN}  OSS context.
 *    persona     {IN}  Persona.
 *    inlen       {IN}  Length of this input part.
 *    tdulen      {OUT} Length of TDU.
 * 
 * NOTES
 *    
 * RETURNS
 *    NZERROR_OK           Success.
 *    NZERROR_TK_NOTOPEN   Persona is not open.
 *    NZERROR_TK_NOTSUPP   Function not supported with persona.
 */
nzerror nztxEncryptExpansion( nzctx *, nzttPersona *, ub4, ub4 * );

/*------------------------ nztDecrypt ------------------------*/

/*
 * NAME
 *    nztDecrypt - Decrypt an Encrypted message.
 * 
 * PARAMETERS
 *    osscntxt {IN}     OSS context.
 *    persona  {IN}     Persona.
 *    state    {IN}     State of decryption.
 *    inlen    {IN}     Length of this input part.
 *    in       {IN}     This input part.
 *    out      {IN/OUT} Cleartext message.
 * 
 * NOTES
 *    
 * RETURNS
 *    NZERROR_OK           Success.
 *    NZERROR_TK_CANTGROW  Needed to grow TDU buffer but could not.
 *    NZERROR_TK_NOTOPEN   Persona is not open.
 *    NZERROR_TK_NOTSUPP   Function not supported with persona.
 */
nzerror nztDecrypt( nzctx *, nzttPersona *, nzttces, ub4, ub1 *,
                       nzttBufferBlock * );

/*------------------------ nztEnvelope ------------------------*/

/*
 * NAME
 *    nztEnvelope - Sign and PKEncrypt a message.
 * 
 * PARAMETERS
 *    osscntxt    {IN}     OSS context.
 *    persona     {IN}     Persona.
 *    nrecipients {IN}     Number of recipients for this encryption.
 *    recipients  {IN}     List of recipients.
 *    state       {IN}     State of encryption.
 *    inlen       {IN}     Length of this input part.
 *    in          {IN}     This input part.
 *    tdubuf      {IN/OUT} TDU buffer.
 *
 * NOTES
 *    
 * RETURNS
 *    NZERROR_OK           Success.
 *    NZERROR_TK_CANTGROW  Needed to grow output buffer but could not.
 *    NZERROR_TK_NOTOPEN   Persona is not open.
 *    NZERROR_TK_NOTSUPP   Function not supported with persona.
 */
nzerror nztEnvelope( nzctx *, nzttPersona *, ub4, nzttIdentity *,
                        nzttces, ub4, ub1 *, nzttBufferBlock * );

/*----------------------- nztDeEnvelope -----------------------*/

/*
 * NAME
 *    nztDeEnvelope - PKDecrypt and verify a message.
 * 
 * PARAMETERS
 *    osscntxt    {IN}     OSS context.
 *    persona     {IN}     Persona.
 *    state       {IN}     State of encryption.
 *    inlen       {IN}     Length of this input part.
 *    in          {IN}     This input part.
 *    out         {OUT}    Message from TDU.
 *    verified    {OUT}    TRUE if verified.
 *    validated   {OUT}    TRUE if validated.
 *    sender      {OUT}    Identity of sender.
 *
 * NOTES
 *    
 * RETURNS
 *    NZERROR_OK           Success.
 *    NZERROR_TK_CANTGROW  Needed to grow TDU buffer but could not.
 *    NZERROR_TK_NOTOPEN   Persona is not open.
 *    NZERROR_TK_NOTSUPP   Function not supported with persona.
 */
nzerror nztDeEnvelope( nzctx *, nzttPersona *, nzttces, ub4, ub1 *,
                          nzttBufferBlock *, boolean *, boolean *,
                          nzttIdentity ** );

/*----------------------- nztKeyedHash -----------------------*/

/*
 * NAME
 *    nztKeyedHash - Generate a keyed hash.
 * 
 * PARAMETERS
 *    osscntxt {IN}     OSS context.
 *    persona  {IN}     Persona.
 *    state    {IN}     State of hash.
 *    inlen    {IN}     Length of this input.
 *    in       {IN}     This input.
 *    tdu      {IN/OUT} Output tdu.
 * 
 * NOTES
 *    
 * RETURNS
 *    NZERROR_OK           Success.
 *    NZERROR_TK_CANTGROW  Needed to grow TDU buffer but could not.
 *    NZERROR_TK_NOTOPEN   Persona is not open.
 *    NZERROR_TK_NOTSUPP   Function not supported with persona.
 */
nzerror nztKeyedHash( nzctx *, nzttPersona *, nzttces, ub4, ub1 *,
                         nzttBufferBlock * );

/*------------------ nztxKeyedHashExpansion ------------------*/

/*
 * NAME
 *    nztxKeyedHashExpansion - Determine the space needed for a keyed hash.
 * 
 * PARAMETERS
 *    osscntxt {IN}  OSS context.
 *    persona  {IN}  Persona.
 *    inlen    {IN}  Length of this input.
 *    tdulen   {OUT} TDU length.
 * 
 * NOTES
 *    
 * RETURNS
 *    NZERROR_OK           Success.
 *    NZERROR_TK_NOTOPEN   Persona is not open.
 *    NZERROR_TK_NOTSUPP   Function not supported with persona.
 */
nzerror nztxKeyedHashExpansion( nzctx *, nzttPersona *, ub4, 
                                   ub4 * );

/*--------------------- nztxHashExpansion ---------------------*/

/*
 * NAME
 *    nztxHashExpansion - Determine the size of the TDU for a hash.
 * 
 * PARAMETERS
 *    osscntxt {IN}  OSS context.
 *    persona  {IN}  Persona.
 *    inlen    {IN}  Length of this input.
 *    tdulen   {OUT} TDU length.
 * 
 * NOTES
 *    
 * RETURNS
 *    NZERROR_OK           Success.
 *    NZERROR_TK_NOTOPEN   Persona is not open.
 *    NZERROR_TK_NOTSUPP   Function not supported with persona.
 */
nzerror nztxHashExpansion( nzctx *, nzttPersona *, ub4, ub4 * );

/*---------------- nztiae_IsAuthEnabled ----------------*/

/*
 * NAME
 *    nztiae_IsAuthEnabled -  Checks to see if Authentication is Enabled
 *                               in the current Cipher Spec.
 * 
 * PARAMETERS
 *    ctx   {IN}   Oracle SSL Context
 *      ncipher {IN}    CipherSuite
 *      authEnabled {OUT} Boolean for is Auth Enabled?
 * 
 * NOTES
 *    
 * RETURNS
 *      NZERROR_OK on success.
 *      NZERROR_TK_INV_CIPHR_TYPE if Cipher Spec is not Recognized.
 */

nzerror nztiae_IsAuthEnabled( nzctx *ctx, 
                                  ub2 ncipher, 
                                  boolean *authEnabled );

/*---------------- nztiee_IsEncrEnabled ----------------*/
/*
 * NAME
 *    nztiee_IsEncrEnabled -  Checks to see if Encryption is Enabled
 *                               in the current Cipher Spec.
 * 
 * PARAMETERS
 *    ctx   {IN}   Oracle SSL Context
 *      ncipher {IN}    CipherSuite
 *      EncrEnabled {OUT} Boolean for is Auth Enabled?
 * 
 * NOTES
 *    
 * RETURNS
 *      NZERROR_OK on success.
 *      NZERROR_TK_INV_CIPHR_TYPE if Cipher Spec is not Recognized.
 */

nzerror nztiee_IsEncrEnabled( nzctx *ctx, 
                                  ub2 ncipher, 
                                  boolean *EncrEnabled );

/*---------------- nztihe_IsHashEnabled ----------------*/
/*
 * NAME
 *    nztihe_IsHashEnabled -  Checks to see if HAshing is Enabled
 *                               in the current Cipher Spec.
 * 
 * PARAMETERS
 *    ctx   {IN}   Oracle SSL Context
 *      ncipher {IN}    CipherSuite
 *      hashEnabled {OUT} Boolean for is Auth Enabled?
 * 
 * NOTES
 *    
 * RETURNS
 *      NZERROR_OK on success.
 *      NZERROR_TK_INV_CIPHR_TYPE if Cipher Spec is not Recognized.
 */

nzerror nztihe_IsHashEnabled( nzctx *ctx, 
                                  ub2 ncipher, 
                                  boolean *hashEnabled );

/*
 *
 */

nzerror nztGetIssuerName( nzctx *ctx,
                             nzttIdentity *identity,
                             ub1  **issuername,
                             ub4   *issuernamelen );

nzerror nztGetSubjectName( nzctx *ctx,
                              nzttIdentity *identity,
                              ub1  **subjectname,
                              ub4   *subjectnamelen );

nzerror nztGetBase64Cert( nzctx *ctx,
                              nzttIdentity *identity,
                              ub1  **b64cert,
                              ub4   *b64certlen );

nzerror nztGetSerialNumber( nzctx *ctx,
                              nzttIdentity *identity,
                              ub1   **serialnum,
                              ub4    *serialnumlen );

nzerror nztSS_Serialnum_to_String( nzctx* ctx,
                                    nzstrc *pSerialNum,
                                    ub1 *pCertserial,
                                    ub4 certseriallen);

nzerror nztGetValidDate( nzctx *ctx,
                            nzttIdentity *identity,
                            ub4  *startdate, 
                            ub4  *enddate  );

nzerror nztGetValidDate_ext( nzctx *ctx,
                            nzttIdentity *identity,
                            ub8  *startdate,
                            ub8  *enddate  );

nzerror nztGetVersion( nzctx *ctx,
                          nzttIdentity *identity,
                          nzstrc *pVerStr  );

nzerror nztGetPublicKey( nzctx *ctx,
                            nzttIdentity *identity,
                            ub1  **pubKey,
                            ub4   *pubKeylen );

nzerror nztGenericDestroy( nzctx *ctx,
                              ub1  **var );

nzerror nztSetAppDefaultLocation( nzctx *ctx,
                                     text *,
                                     size_t );

nzerror nztSearchNZDefault( nzctx *ctx,
                            boolean *search );

nzerror nztSetLightWeight(nzctx *ctx,
                          boolean flag);

/* ****************** nzICE_Install_Cert_ext ****************** */
/* Installs a Cert into a wallet if it meets certain conditions.
 * It calls the function below, nztnAC_Add_Certificate().
 * Finally, it returns a ptr to where the actual object has been added into the
 * wallet. This is used in support of OWM
 * This is an extention of nztnIC_Install_Cert to support Trust flag.

 nzctx *oss_context,     (IN)
 nzttWallet *pWallet,    (IN)
 nzttPersona *pPersona,  (IN)
 ub1* pCertbuf,          (IN)
 ub4 certbuflen,         (IN)
 boolean certtype,       (IN)
 nzstrc *pIdtypestr,     (IN)
 nzTrustFlag trustflag   (IN)
 */
nzerror nzICE_Install_Cert_ext( nzctx *oss_context,
                                    nzttWallet *pWallet,
                                    nzttPersona *pPersona,
                                    ub1* pCertbuf,
                                    ub4 certbuflen,
                                    boolean certtype,
                                    nzstrc *pIdtypestr,
                                    nzTrustFlag trustflag,
                                    nzttIdentity **ppCertOrTP );

/******************** nzMF_Modify_TrustFlags **************************/
/* This function would be used to modify trust flags assigned
 * to a certificate. The already assigned trust flag would get replaced
 * by new trust flag value
*/

nzerror nzMF_Modify_TrustFlags(nzctx *oss_context,
                                  nzttWallet *pWallet,
                                  nzttPersona *pPersona,
                                  nzttIdentity *pCert,
                                  int trustflag);


#endif /* NZT_ORACLE */

