/*
 * $ ldap.h Release 12.2.1.3.0 $ 
 */

/* Copyright (c) 1998, 2017, Oracle and/or its affiliates. 
All rights reserved.*/
/*
   NAME
     ldap.h - Used by clients. 
   DESCRIPTION
     <short description of component this file declares/defines>
   PUBLIC FUNCTION(S)
     <list of external functions declared/defined - with one-line descriptions>
   PRIVATE FUNCTION(S)
     <list of static functions defined in .c file - with one-line descriptions>
   RETURNS
     <function return values, for .c file with single function>
   NOTES
     <other useful comments, qualifications, etc.>
   MODIFIED   (MM/DD/YY)
*/
/*
* Copyright (c) 1990, 2017, Oracle and/or its affiliates. All rights reserved.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms are permitted
 * provided that this notice is preserved and that due credit is given
 * to the University of Michigan at Ann Arbor. The name of the University
 * may not be used to endorse or promote products derived from this
 * software without specific prior written permission. This software
 * is provided ``as is'' without express or implied warranty.
 */

#ifndef GSLC_ORACLE
#define GSLC_ORACLE

#ifdef __cplusplus
extern "C" {
#endif


#ifdef WINSOCK
#include "msdos.h"
#include <winsock.h>
#endif

/* BER classes and mask */
#define LBER_CLASS_UNIVERSAL	0x00
#define LBER_CLASS_APPLICATION	0x40
#define LBER_CLASS_CONTEXT	0x80
#define LBER_CLASS_PRIVATE	0xc0
#define LBER_CLASS_MASK		0xc0

/* BER encoding type and mask */
#define LBER_PRIMITIVE		0x00
#define LBER_CONSTRUCTED	0x20
#define LBER_ENCODING_MASK	0x20

#define LBER_BIG_TAG_MASK	0x1f
#define LBER_MORE_TAG_MASK	0x80

/*
 * Note that LBER_ERROR and LBER_DEFAULT are values that can never appear
 * as valid BER tags, and so it is safe to use them to report errors.  In
 * fact, any tag for which the following is true is invalid:
 *     (( tag & 0x00000080 ) != 0 ) && (( tag & 0xFFFFFF00 ) != 0 )
 */


#define LBER_ERROR		-1
#define LBER_DEFAULT		-1

/* general BER types we know about */
#define LBER_BOOLEAN		0x01L
#define LBER_INTEGER		0x02L
#define LBER_BITSTRING		0x03L
#define LBER_OCTETSTRING	0x04L
#define LBER_NULL		0x05L
#define LBER_ENUMERATED		0x0aL
#define LBER_SEQUENCE		0x30L	/* constructed */
#define LBER_SET		0x31L	/* constructed */

#define OLD_LBER_SEQUENCE	0x10L	/* w/o constructed bit - broken */
#define OLD_LBER_SET		0x11L	/* w/o constructed bit - broken */

typedef int (*BERTranslateProc)( char **bufp, unsigned int *buflenp,
	int free_input );

typedef struct seqorset {
        unsigned int   sos_clen;
        unsigned int   sos_tag;
        char            *sos_first;
        char            *sos_ptr;
        struct seqorset *sos_next;
} Seqorset;
#define NULLSEQORSET    ((Seqorset *) 0)

#define SOS_STACK_SIZE  8 /* depth of the pre-allocated sos structure stack */

typedef struct berelement {
	char		*ber_buf;
	char		*ber_ptr;
	char		*ber_end;
	struct seqorset	*ber_sos;
	unsigned int	ber_tag;
	unsigned int	ber_len;
	int		ber_usertag;
	char		ber_options;
#define LBER_USE_DER		0x01
#define LBER_USE_INDEFINITE_LEN	0x02
#define LBER_TRANSLATE_STRINGS	0x04
	char		*ber_rwptr;
	BERTranslateProc ber_encode_translate_proc;
	BERTranslateProc ber_decode_translate_proc;
        int             ber_flags;
        int             ber_sos_stack_posn;
        Seqorset        ber_sos_stack[SOS_STACK_SIZE];
} BerElement;
#define NULLBER	((BerElement *) 0)

/* sgsluns - private network endpoint type
 */
struct sgsluns
{
 int   sock_sgsluns;      /* socket identifier */
 long  state_sgsluns;     /* sgsluns state flag (SGSLUNS_STATE_xxx) */
 char  saddr_sgsluns[32]; /* inet address of other end of this connection
                           * in the form A.B.C.D where A,B,C, and D are
                           * base 256 notation integers. eg. "192.0.0.1"
                           */
};
typedef struct sgsluns sgsluns;

typedef struct sockbuf {
#ifndef MACOS
	int		sb_sd;
#else /* MACOS */
	void		*sb_sd;
#endif /* MACOS */
	BerElement	sb_ber;

	int		sb_naddr;	/* > 0 implies using CLDAP (UDP) */
	void		*sb_useaddr;	/* pointer to sockaddr to use next */
	void		*sb_fromaddr;	/* pointer to message source sockaddr */
	void		**sb_addrs;	/* actually an array of pointers to
						sockaddrs */

	int		sb_options;	/* to support copying ber elements */
#define LBER_TO_FILE		0x01	/* to a file referenced by sb_fd   */
#define LBER_TO_FILE_ONLY	0x02	/* only write to file, not network */
#define LBER_MAX_INCOMING_SIZE	0x04	/* impose limit on incoming stuff  */
#define LBER_NO_READ_AHEAD	0x08	/* read only as much as requested  */
	int		sb_fd;
	int		sb_max_incoming;
        void            *sb_nzenv;        /* nzos environment pointer */
        void        *sb_sslContext;
        int         sb_sslauth;       /* type of ssl authentication */
        int         sb_sslflag;
        int         ssl_sd;
        sgsluns         *sb_endp;       /* socket end point */
} Sockbuf;
#define READBUFSIZ	8192

/* structure for returning a sequence of octet strings + length */
struct berval {
	unsigned int	bv_len;
	char		*bv_val;
};

#define LDAP_PORT	389
#define LDAP_SSL_PORT	636

#ifndef DEF_SSL_PORT
extern int sslmodeenabled ;
#endif

#define LDAP_VERSION1	1
#define LDAP_VERSION2	2
#define LDAP_VERSION3   3

#define LDAP_VERSION   LDAP_VERSION2 /*default should stay as LDAPv2*/

#define LDAP_VERSION_MAX LDAP_VERSION3

#define COMPAT20
#define COMPAT30
#if defined(COMPAT20) || defined(COMPAT30)
#define COMPAT
#endif

#define LDAP_MAX_ATTR_LEN	256

/* various options that can be set/unset */
#define LDAP_OPT_DESC                   1
#define LDAP_OPT_DEREF                  2
#define LDAP_OPT_SIZELIMIT              3
#define LDAP_OPT_TIMELIMIT              4
#define LDAP_OPT_THREAD_FN_PTRS         5
#define LDAP_OPT_REBIND_FN              6
#define LDAP_OPT_REBIND_ARG             7
#define LDAP_OPT_REFERRALS              8
#define LDAP_OPT_RESTART                9
#define LDAP_OPT_SSL                    10
#define LDAP_OPT_IO_FN_PTRS             11
#define LDAP_OPT_CACHE_FN_PTRS          13
#define LDAP_OPT_CACHE_STRATEGY         14
#define LDAP_OPT_CACHE_ENABLE           15
#define LDAP_OPT_REFERRAL_HOP_LIMIT     16
#define LDAP_OPT_PROTOCOL_VERSION       17
#define LDAP_OPT_SERVER_CONTROLS        18
#define LDAP_OPT_CLIENT_CONTROLS        19
#define LDAP_OPT_PREFERRED_LANGUAGE     20
#define LDAP_OPT_ERROR_NUMBER           49
#define LDAP_OPT_ERROR_STRING           50

/* client side referral cache enable option */
#define ORA_LDAP_OPT_RFRL_CACHE         200

/* ldap connect timeout */
#define ORA_LDAP_OPT_CONNECT_TIMEOUT    210

/* directory server types */
#define LDAP_DIRTYPE_OID        1  /* Oracle Internet Directory */
#define LDAP_DIRTYPE_AD         2  /* Microsoft Active Directory */
#define LDAP_DIRTYPE_NETSCAPE   4  /* Netscape Directory Server  */
#define LDAP_DIRTYPE_NDS        8  /* Novell Directory Service   */
#define LDAP_DIRTYPE_UNKNOWN    4096 /* Unknown */

/* for on/off options */
#define LDAP_OPT_ON     ((void *)1)
#define LDAP_OPT_OFF    ((void *)0)


/* SSL Authentication modes */
#define GSLC_SSL_NO_AUTH         1
#define GSLC_SSL_ONEWAY_AUTH  	32
#define GSLC_SSL_TWOWAY_AUTH  	64


/* Abandon support */
#define ABANDONorTL(op)      ((op)->o_abandon == 1)

/* debugging stuff */
#ifdef LDAP_DEBUG
extern int	ldap_debug;
extern unsigned int debug_flag;
#define LDAP_DEBUG_TRACE	0x001
#define LDAP_DEBUG_PACKETS	0x002
#define LDAP_DEBUG_ARGS		0x004
#define LDAP_DEBUG_CONNS	0x008
#define LDAP_DEBUG_BER		0x010
#define LDAP_DEBUG_FILTER	0x020
#define LDAP_DEBUG_CONFIG	0x040
#define LDAP_DEBUG_ACL		0x080
#define LDAP_DEBUG_STATS	0x100
#define LDAP_DEBUG_STATS2	0x200
#define LDAP_DEBUG_SHELL	0x400
#define LDAP_DEBUG_PARSE	0x800
#define LDAP_DEBUG_PROCESS	0x2000
#define LDAP_DEBUG_MUST	        0x4000
#ifndef LDAP_DEBUG_ANY
# define LDAP_DEBUG_ANY		0xffff
#endif /* LDAP_DEBUG_ANY */

#ifdef LDAP_SYSLOG
#define Debug( level, fmt, arg1, arg2, arg3 )	\
	{ \
		if ( ldap_debug & level ) \
			fprintf( stderr, fmt, arg1, arg2, arg3 ); \
	}
#else /* LDAP_SYSLOG */
#ifndef WINSOCK
#define Debug( level, fmt, arg1, arg2, arg3 ) \
		if ( ldap_debug & level ) \
			fprintf( stderr, fmt, arg1, arg2, arg3 );
#else /* !WINSOCK */
extern void Debug( int level, char* fmt, ... );
#endif /* !WINSOCK */
#endif /* LDAP_SYSLOG */
#else /* LDAP_DEBUG */
#define Debug( level, fmt, arg1, arg2, arg3 )
#endif /* LDAP_DEBUG */

/* 
 * specific LDAP instantiations of BER types we know about
 */

/* general stuff */
#define LDAP_TAG_MESSAGE	0x30L	/* tag is 0x10 + constructed bit */
#define OLD_LDAP_TAG_MESSAGE	0x10L	/* forgot the constructed bit  */
#define LDAP_TAG_MSGID		0x02L   /* INTEGER */
#define LDAP_TAG_LDAPDN         0x04L   /* OCTET STRING */
#define LDAP_TAG_CONTROLS       0xa0L   /* context specific + constructed + 0 */
#define LDAP_TAG_REFERRAL       0xa3L   /* context specific + constructed + 3 */
#define LDAP_TAG_NEWSUPERIOR    0x80L   /* context specific + primitive */
#define LDAP_TAG_MRA_OID        0x81L   /* context specific + primitive */
#define LDAP_TAG_MRA_TYPE       0x82L   /* context specific + primitive */
#define LDAP_TAG_MRA_VALUE      0x83L   /* context specific + primitive */
#define LDAP_TAG_MRA_DNATTRS    0x84L   /* context specific + primitive */
#define LDAP_TAG_EXOP_REQ_OID   0x80L   /* context specific + primitive */
#define LDAP_TAG_EXOP_REQ_VALUE 0x81L   /* context specific + primitive */
#define LDAP_TAG_EXOP_RES_OID   0x8aL   /* context specific + primitive */
#define LDAP_TAG_EXOP_RES_VALUE 0x8bL   /* context specific + primitive */
#define LDAP_TAG_SK_MATCHRULE   0x80L   /* context specific + primitive */
#define LDAP_TAG_SK_REVERSE     0x81L   /* context specific + primitive */
#define LDAP_TAG_SR_ATTRTYPE    0x80L   /* context specific + primitive */
#define LDAP_TAG_SASL_RES_CREDS 0x87L   /* context specific + primitive */
#define LDAP_TAG_VLV_BY_INDEX   0xa0L   /* context specific + constructed + 0 */
#define LDAP_TAG_VLV_BY_VALUE   0x81L   /* context specific + primitive + 1 */

/* possible operations a client can invoke */
#define LDAP_REQ_BIND			0x60L	/* application + constructed */
#define LDAP_REQ_UNBIND			0x42L	/* application + primitive   */
#define LDAP_REQ_SEARCH			0x63L	/* application + constructed */
#define LDAP_REQ_MODIFY			0x66L	/* application + constructed */
#define LDAP_REQ_ADD			0x68L	/* application + constructed */
#define LDAP_REQ_DELETE			0x4aL	/* application + primitive   */
#define LDAP_REQ_MODRDN			0x6cL	/* application + constructed */
#define LDAP_REQ_MODDN                  0x6cL   /* application + constructed */
#define LDAP_REQ_COMPARE		0x6eL	/* application + constructed */
#define LDAP_REQ_ABANDON		0x50L	/* application + primitive   */

/* New defs added for LDAP V3 support */
#define LDAP_REQ_EXTENDED		0x77L   /* application + constructed */
/* version 3.0 compatibility stuff */
#define LDAP_REQ_UNBIND_30		0x62L
#define LDAP_REQ_DELETE_30		0x6aL
#define LDAP_REQ_ABANDON_30		0x70L

/* 
 * old broken stuff for backwards compatibility - forgot application tag
 * and constructed/primitive bit
 */
#define OLD_LDAP_REQ_BIND		0x00L
#define OLD_LDAP_REQ_UNBIND		0x02L
#define OLD_LDAP_REQ_SEARCH		0x03L
#define OLD_LDAP_REQ_MODIFY		0x06L
#define OLD_LDAP_REQ_ADD		0x08L
#define OLD_LDAP_REQ_DELETE		0x0aL
#define OLD_LDAP_REQ_MODRDN		0x0cL
#define OLD_LDAP_REQ_COMPARE		0x0eL
#define OLD_LDAP_REQ_ABANDON		0x10L

/* possible result types a server can return */
#define LDAP_RES_BIND			0x61L /* application+constructed+1 */
#define LDAP_RES_SEARCH_ENTRY		0x64L	/* 100 */
#define LDAP_RES_SEARCH_RESULT		0x65L	/* 101 */
#define LDAP_RES_MODIFY			0x67L	/* 103 */
#define LDAP_RES_ADD			0x69L	/* 105 */
#define LDAP_RES_DELETE			0x6bL	/* 107 */
#define LDAP_RES_MODRDN			0x6dL	/* 109 */
#define LDAP_RES_RENAME 		0x6dL	/* same as LDAP_RES_MODRDN */
#define LDAP_RES_COMPARE		0x6fL	/* 111 */
#define LDAP_RES_SEARCH_REFERENCE	0x73L	/* 115 */
#define LDAP_RES_EXTENDED               0x78L   /* 120 */
#define LDAP_RES_ANY			(-1L)

/* old broken stuff for backwards compatibility */
#define OLD_LDAP_RES_BIND		0x01L
#define OLD_LDAP_RES_SEARCH_ENTRY	0x04L
#define OLD_LDAP_RES_SEARCH_RESULT	0x05L
#define OLD_LDAP_RES_MODIFY		0x07L
#define OLD_LDAP_RES_ADD		0x09L
#define OLD_LDAP_RES_DELETE		0x0bL
#define OLD_LDAP_RES_MODRDN		0x0dL
#define OLD_LDAP_RES_COMPARE		0x0fL

/* authentication methods available */
#define LDAP_AUTH_NONE		0x00L	/* no authentication		  */
#define LDAP_AUTH_SIMPLE	0x80L	/* context specific + primitive   */
#define LDAP_AUTH_SASL          0xa3L   /* context specific + constructed  */

/* supported SASL methods */
#define LDAP_SASL_SIMPLE        0       /* special value used for simple bind */
#define LDAP_SASL_EXTERNAL      "EXTERNAL"

/* authentication methods supported in v2 */
#define LDAP_AUTH_KRBV4		0xffL	/* means do both of the following */
#define LDAP_AUTH_KRBV41	0x81L	/* context specific + primitive   */
#define LDAP_AUTH_KRBV42	0x82L	/* context specific + primitive   */
#define LDAP_AUTH_SASL_V2	0x83L   /* context specific + primitive */
#define LDAP_AUTH_REPL		0x90L	/* replication specific		  */


/* 3.0 compatibility auth methods */
#define LDAP_AUTH_SIMPLE_30	0xa0L	/* context specific + constructed */
#define LDAP_AUTH_KRBV41_30	0xa1L	/* context specific + constructed */
#define LDAP_AUTH_KRBV42_30	0xa2L	/* context specific + constructed */
#define LDAP_AUTH_SASL_30       0xa3L   /* context specific + constructed */
/* old broken stuff */
#define OLD_LDAP_AUTH_SIMPLE	0x00L
#define OLD_LDAP_AUTH_KRBV4	0x01L
#define OLD_LDAP_AUTH_KRBV42	0x02L

/* filter types */
#define LDAP_FILTER_AND		0xa0L	/* context specific + constructed */
#define LDAP_FILTER_OR		0xa1L	/* context specific + constructed */
#define LDAP_FILTER_NOT		0xa2L	/* context specific + constructed */
#define LDAP_FILTER_EQUALITY	0xa3L	/* context specific + constructed */
#define LDAP_FILTER_SUBSTRINGS	0xa4L	/* context specific + constructed */
#define LDAP_FILTER_GE		0xa5L	/* context specific + constructed */
#define LDAP_FILTER_LE		0xa6L	/* context specific + constructed */
#define LDAP_FILTER_PRESENT	0x87L	/* context specific + primitive   */
#define LDAP_FILTER_APPROX	0xa8L	/* context specific + constructed */

/* 3.0 compatibility filter types */
#define LDAP_FILTER_PRESENT_30	0xa7L	/* context specific + constructed */

/* old broken stuff */
#define OLD_LDAP_FILTER_AND		0x00L
#define OLD_LDAP_FILTER_OR		0x01L
#define OLD_LDAP_FILTER_NOT		0x02L
#define OLD_LDAP_FILTER_EQUALITY	0x03L
#define OLD_LDAP_FILTER_SUBSTRINGS	0x04L
#define OLD_LDAP_FILTER_GE		0x05L
#define OLD_LDAP_FILTER_LE		0x06L
#define OLD_LDAP_FILTER_PRESENT		0x07L
#define OLD_LDAP_FILTER_APPROX		0x08L

/* substring filter component types */
#define LDAP_SUBSTRING_INITIAL	0x80L	/* context specific */
#define LDAP_SUBSTRING_ANY	0x81L	/* context specific */
#define LDAP_SUBSTRING_FINAL	0x82L	/* context specific */

/* 3.0 compatibility substring filter component types */
#define LDAP_SUBSTRING_INITIAL_30	0xa0L	/* context specific */
#define LDAP_SUBSTRING_ANY_30		0xa1L	/* context specific */
#define LDAP_SUBSTRING_FINAL_30		0xa2L	/* context specific */

/* old broken stuff */
#define OLD_LDAP_SUBSTRING_INITIAL	0x00L
#define OLD_LDAP_SUBSTRING_ANY		0x01L
#define OLD_LDAP_SUBSTRING_FINAL	0x02L

/* search scopes */
#define LDAP_SCOPE_BASE		0x00
#define LDAP_SCOPE_ONELEVEL	0x01
#define LDAP_SCOPE_SUBTREE	0x02

/* for modifications */
typedef struct ldapmod {
	int		mod_op;
#define LDAP_MOD_ADD		0x00
#define LDAP_MOD_DELETE		0x01
#define LDAP_MOD_REPLACE	0x02
#define LDAP_MOD_BVALUES	0x80
	char		*mod_type;
        int             mod_ver;
        char            mod_time[48];
        char            mod_server[128];
	union {
		char		**modv_strvals;
		struct berval	**modv_bvals;
	} mod_vals;
        struct berval   **modv_nvals;
#define mod_values	mod_vals.modv_strvals
#define mod_bvalues	mod_vals.modv_bvals
#ifdef ONLDAPD
	struct ldapmod	*mod_next;
	int createTombstone;
#endif
} LDAPMod;

/* 
 * possible error codes we can return
 */

#define LDAP_SUCCESS			              0x00
#define GSL_SUCCESS 	LDAP_SUCCESS
#define LDAP_OPERATIONS_ERROR		0x01
#define LDAP_PROTOCOL_ERROR		0x02
#define LDAP_TIMELIMIT_EXCEEDED		0x03
#define LDAP_SIZELIMIT_EXCEEDED		0x04
#define LDAP_COMPARE_FALSE		0x05
#define LDAP_COMPARE_TRUE		0x06
#define LDAP_STRONG_AUTH_NOT_SUPPORTED	0x07
#define LDAP_STRONG_AUTH_REQUIRED	0x08
#define LDAP_PARTIAL_RESULTS		0x09
#define LDAP_REFERRAL			0x0A
#define LDAP_ADMINLIMIT_EXCEEDED	0x0B
#define LDAP_UNAVAILABLE_CRITICALEXTENSION	0x0C
#define LDAP_CONFIDENTIALITY_REQUIRED	        0x0D
#define LDAP_SASL_BIND_IN_PROGRESS              0x0E

#define LDAP_NO_SUCH_ATTRIBUTE		0x10
#define LDAP_UNDEFINED_TYPE		0x11
#define LDAP_INAPPROPRIATE_MATCHING	0x12
#define LDAP_CONSTRAINT_VIOLATION	0x13
#define LDAP_TYPE_OR_VALUE_EXISTS	0x14
#define LDAP_INVALID_SYNTAX		0x15

#define LDAP_NO_SUCH_OBJECT		0x20
#define LDAP_ALIAS_PROBLEM		0x21
#define LDAP_INVALID_DN_SYNTAX		0x22
#define LDAP_IS_LEAF			0x23
#define LDAP_ALIAS_DEREF_PROBLEM	0x24

#define NAME_ERROR(n)	((n & 0xf0) == 0x20)

#define LDAP_INAPPROPRIATE_AUTH		0x30
#define LDAP_INVALID_CREDENTIALS	0x31
#define LDAP_INSUFFICIENT_ACCESS	0x32
#define LDAP_BUSY			0x33
#define LDAP_UNAVAILABLE		0x34
#define LDAP_UNWILLING_TO_PERFORM	0x35
#define LDAP_LOOP_DETECT		0x36

#define LDAP_NAMING_VIOLATION		0x40
#define LDAP_OBJECT_CLASS_VIOLATION	0x41
#define LDAP_NOT_ALLOWED_ON_NONLEAF	0x42
#define LDAP_NOT_ALLOWED_ON_RDN		0x43
#define LDAP_ALREADY_EXISTS		0x44
#define LDAP_NO_OBJECT_CLASS_MODS	0x45
#define LDAP_RESULTS_TOO_LARGE		0x46
#define LDAP_AFFECT_MULTIPLE_DSAS               0x47

#define LDAP_OTHER			0x50
#define LDAP_SERVER_DOWN		0x51
#define LDAP_LOCAL_ERROR		0x52
#define LDAP_ENCODING_ERROR		0x53
#define LDAP_DECODING_ERROR		0x54
#define LDAP_TIMEOUT			0x55
#define LDAP_AUTH_UNKNOWN		0x56
#define LDAP_FILTER_ERROR		0x57
#define LDAP_USER_CANCELLED		0x58
#define LDAP_PARAM_ERROR		0x59
#define LDAP_NO_MEMORY			0x5a
#define LDAP_CONNECT_ERROR              0x5b    /* 91 */
#define LDAP_NOT_SUPPORTED              0x5c    /* 92 - LDAPv3 */
#define LDAP_CONTROL_NOT_FOUND          0x5d    /* 93 - LDAPv3 */
#define LDAP_NO_RESULTS_RETURNED        0x5e    /* 94 - LDAPv3 */
#define LDAP_MORE_RESULTS_TO_RETURN     0x5f    /* 95 - LDAPv3 */
#define LDAP_CLIENT_LOOP                0x60    /* 96 - LDAPv3 */
#define LDAP_REFERRAL_LIMIT_EXCEEDED    0x61    /* 97 - LDAPv3 */


/* discover server related error codes */
#define ORA_LDAP_INFO_NOT_FOUND         500  /* oracle specific error codes */
#define ORA_LDAP_LOOKUP_ERROR           501
#define ORA_LDAP_DNS_ADDR_NOT_FOUND     502
#define ORA_LDAP_FILE_NOT_FOUND         503
#define ORA_LDAP_FILE_PARSE_ERROR       504

/* SSL bind related error codes */
#define ORA_LDAP_SSL_INITIALIZE_ERROR   550
#define ORA_LDAP_SSL_CTXCONFIG_ERROR    551
#define ORA_LDAP_SSL_OPENWALLET_ERROR   552
#define ORA_LDAP_SSL_CTXCREATE_ERROR    553
#define ORA_LDAP_SSL_HANDSHAKE_ERROR    554
#define ORA_LDAP_SSL_PEERCRED_ERROR     555


/* default limit on nesting of referrals */
#define LDAP_DEFAULT_REFHOPLIMIT	5

/* discover server related structs and declarations */

/* source from where information can be discovered */
#define ORA_DNS_DISCOVER                1
#define ORA_CFGFILE_DISCOVER            2
#define ORA_AUTO_DISCOVER               3

/* method to be applied for obtaining hostname info from DNS */
#define ORA_USE_INPUT_DN_METHOD         1   
#define ORA_USE_MC_DOMAIN_METHOD        2   
#define ORA_USE_DFLT_LOOKUP_METHOD      4   
#define ORA_USE_ALL_METHODS             (ORA_USE_INPUT_DN_METHOD + \
                                         ORA_USE_MC_DOMAIN_METHOD + \
                                         ORA_USE_DFLT_LOOKUP_METHOD) 

/* result types */
#define ORA_DEF_ADMIN_CTXT         1 
#define ORA_DIR_SERVERS            2 
#define ORA_DIR_SERVER_TYPE        3 
#define ORA_ALTDIR_SERVERS         4 
#define ORA_ORCLCOMMCTXMAP         5 
#define ORA_DIRSRVR_CONN_SEC       6 
#define ORA_CFGFL_PATH             7

/* properties that can be set in the discovery handle */
#define ORA_DNS_DN                  1
#define ORA_DNS_DISCOVER_METHOD     2
#define ORA_SSLMODE                 3

typedef void * OraLdapHandle;
typedef void * OraResultHandle;

typedef void OraLdapClientCtx;

/* Options for SASL credential functions */
#define ORA_LDAP_SASL_MECH_DIGEST_MD5    "DIGEST-MD5" /* SASL Mechanism : 
                                                         Digest MD5 */
#define   ORA_LDAP_CRED_HANDLE_SASL_MD5   1

/* properties that can be set in Credential handle */
#define   ORA_LDAP_CRED_SASL_REALM                1
#define   ORA_LDAP_CRED_SASL_AUTH_PASSWORD        2
#define   ORA_LDAP_CRED_SASL_AUTHORIZATION_ID     3
#define   ORA_LDAP_CRED_SASL_SECURITY_PROPERTIES  4
#define   ORA_LDAP_CRED_SASL_NORM_AUTHDN          5

#define ora_ldap_init_clientctx(ctxptr) ((OraLdapClientCtx **)NULL != (ctxptr) && (*((OraLdapClientCtx **)(ctxptr)) = (OraLdapClientCtx *)NULL,1)) ? LDAP_SUCCESS : LDAP_PARAM_ERROR

#define ora_ldap_free_clientctx(ctxptr) ((OraLdapClientCtx **)NULL != (ctxptr) && (*((OraLdapClientCtx **)(ctxptr)) = (OraLdapClientCtx *)NULL,1)) ? LDAP_SUCCESS : LDAP_PARAM_ERROR

/*
 * This structure represents both ldap messages and ldap responses.
 * These are really the same, except in the case of search responses,
 * where a response has multiple messages.
 */

typedef struct ldapmsg {
	int		lm_msgid;	/* the message id */
	int		lm_msgtype;	/* the message type */
	BerElement	*lm_ber;	/* the ber encoded message contents */
	struct ldapmsg	*lm_chain;	/* for search - next msg in the resp */
	struct ldapmsg	*lm_next;	/* next response */
	unsigned int	lm_time;	/* used to maintain cache */
} LDAPMessage;
#define NULLMSG	((LDAPMessage *) NULL)


#ifdef LDAP_REFERRALS
/*
 * structure for tracking LDAP server host, ports, DNs, etc.
 */
typedef struct ldap_server {
	char			*lsrv_host;
	char			*lsrv_dn;	/* if NULL, use default */
	int			lsrv_port;
	unsigned int		lsrv_options;   /* boolean options */
#define	LDAP_SRV_OPT_SECURE	0x01
	struct ldap_server	*lsrv_next;
} LDAPServer;


/*
 * structure for representing an LDAP server connection
 */
typedef struct ldap_conn {
	Sockbuf			*lconn_sb;
	BerElement		*lconn_ber;  /* non-NULL if in midst of msg. */
	int			lconn_version;  /* LDAP protocol version */
	int			lconn_refcnt;
	char        *lconn_lastused;	/* time */
	int			lconn_status;
#define LDAP_CONNST_NEEDSOCKET		1
#define LDAP_CONNST_CONNECTING		2
#define LDAP_CONNST_CONNECTED		3
#define LDAP_CONNST_DEAD                4
	LDAPServer		*lconn_server;
	char			*lconn_binddn;  /* DN of last successful bind */
	int			lconn_bound;    /* has a bind been done? */
	char			*lconn_krbinstance;
	struct ldap_conn	*lconn_next;
} LDAPConn;


/*
 * structure used to track outstanding requests
 */
typedef struct ldapreq {
	int		lr_msgid;	/* the message id */
	int		lr_status;	/* status of request */
#define LDAP_REQST_INPROGRESS	1
#define LDAP_REQST_CHASINGREFS	2
#define LDAP_REQST_NOTCONNECTED	3
#define LDAP_REQST_WRITING	4
	int		lr_outrefcnt;	/* count of outstanding referrals */
	int		lr_origid;	/* original request's message id */
	int		lr_parentcnt;	/* count of parent requests */
	int		lr_res_msgtype;	/* result message type */
	int		lr_res_errno;	/* result LDAP errno */
	char		*lr_res_error;	/* result error string */
	char		*lr_res_matched;/* result matched DN string */
	BerElement	*lr_ber;	/* ber encoded request contents */
	LDAPConn	*lr_conn;	/* connection used to send request */
	char		*lr_binddn;     /* request is a bind for this DN */
	struct ldapreq	*lr_parent;	/* request that spawned this referral */
	struct ldapreq	*lr_refnext;	/* next referral spawned */
	struct ldapreq	*lr_prev;	/* previous request */
	struct ldapreq	*lr_next;	/* next request */
} LDAPRequest;
#endif /* LDAP_REFERRALS */


/*
 * structure for client cache
 */
#define LDAP_CACHE_BUCKETS	31	/* cache hash table size */
typedef struct ldapcache {
	LDAPMessage	*lc_buckets[LDAP_CACHE_BUCKETS];/* hash table */
	LDAPMessage	*lc_requests;			/* unfulfilled reqs */
	int		lc_timeout;			/* request timeout */
	int		lc_maxmem;			/* memory to use */
	int		lc_memused;			/* memory in use */
	int		lc_enabled;			/* enabled? */
	unsigned int	lc_options;			/* options */
#define LDAP_CACHE_OPT_CACHENOERRS	0x00000001
#define LDAP_CACHE_OPT_CACHEALLERRS	0x00000002
}  LDAPCache;
#define NULLLDCACHE ((LDAPCache *)NULL)

/*
 * structures for ldap getfilter routines
 */

typedef struct ldap_filt_info {
	char			*lfi_filter;
	char			*lfi_desc;
	int			lfi_scope;	/* LDAP_SCOPE_BASE, etc */
	int			lfi_isexact;	/* exact match filter? */
	struct ldap_filt_info	*lfi_next;
} LDAPFiltInfo;

typedef struct ldap_filt_list {
    char			*lfl_tag;
    char			*lfl_pattern;
    char			*lfl_delims;
    LDAPFiltInfo		*lfl_ilist;
    struct ldap_filt_list	*lfl_next;
} LDAPFiltList;


#define LDAP_FILT_MAXSIZ	1024

typedef struct ldap_filt_desc {
	LDAPFiltList		*lfd_filtlist;
	LDAPFiltInfo		*lfd_curfip;
	LDAPFiltInfo		lfd_retfi;
	char			lfd_filter[ LDAP_FILT_MAXSIZ ];
	char			*lfd_curval;
	char			*lfd_curvalcopy;
	char			**lfd_curvalwords;
	char			*lfd_filtprefix;
	char			*lfd_filtsuffix;
} LDAPFiltDesc;


typedef struct ldapcontrol {
    char             *ldctl_oid;
    struct berval    ldctl_value;
    char             ldctl_iscritical;
} LDAPControl, *PLDAPControl;

/* some common controls */
#define LDAP_CONTROL_MANAGEDSAIT "2.16.840.1.113730.3.4.2"

/* OID controls */
#define LDAP_CONTROL_PWDEXPIRED  "2.16.840.1.113894.1.8.20"
#define LDAP_CONTROL_PWDEXPIRING "2.16.840.1.113894.1.8.7";

typedef struct ldappend {
        void            *lp_sema;       /* semaphore to post */
	int             lp_msgid;       /* message id */
        LDAPMessage     *lp_result;     /* result storage */
        struct ldappend *lp_prev;       /* previous pending */
        struct ldappend *lp_next;       /* next pending */
} LDAPPend;

/* structure for holding credential information - used
 * to bind to referred server when chasing referrals
 */
typedef struct ldapcreds {
       char    *binddn;
       char    *passwd;
       int     authmethod;
       char    *proxyDN;
} LDAPCreds;

/*
 * structure representing an ldap connection
 */

typedef struct ldap {
	Sockbuf		ld_sb;		/* socket descriptor & buffer */
	char		*ld_host;
	int		ld_version;
	char		ld_lberoptions;
	int		ld_deref;
#define LDAP_DEREF_NEVER	0
#define LDAP_DEREF_SEARCHING	1
#define LDAP_DEREF_FINDING	2
#define LDAP_DEREF_ALWAYS	3

	int		ld_timelimit;
	int		ld_sizelimit;
#define LDAP_NO_LIMIT		0

	LDAPFiltDesc	*ld_filtd;	/* from getfilter for ufn searches */
	char		*ld_ufnprefix;	/* for incomplete ufn's */

	int		ld_errno;
	char		*ld_error;
	char		*ld_matched;
	int		ld_msgid;

	/* do not mess with these */
#ifdef LDAP_REFERRALS
	LDAPRequest	*ld_requests;	/* list of outstanding requests */
#else /* LDAP_REFERRALS */
	LDAPMessage	*ld_requests;	/* list of outstanding requests */
#endif /* LDAP_REFERRALS */
	LDAPMessage	*ld_responses;	/* list of outstanding responses */
	int		*ld_abandoned;	/* array of abandoned requests */
	char		ld_attrbuffer[100];
	LDAPCache	*ld_cache;	/* non-null if cache is initialized */
	char		*ld_cldapdn;	/* DN used in connectionless search */

	/* it is OK to change these next four values directly */
	int		ld_cldaptries;	/* connectionless search retry count */
	int		ld_cldaptimeout;/* time between retries */
	int		ld_refhoplimit;	/* limit on referral nesting */
	unsigned int	ld_options;	/* boolean options */
#ifdef LDAP_DNS
#define LDAP_OPT_DNS		0x00000001	/* use DN & DNS */
#endif /* LDAP_DNS */

#define LDAP_BITOPT_REFERRALS   0x80000000
#define LDAP_BITOPT_SSL         0x40000000
#define LDAP_BITOPT_DNS         0x20000000
#define LDAP_BITOPT_RESTART     0x10000000
#define LDAP_BITOPT_RECONNECT   0x08000000
#define LDAP_BITOPT_ASYNC       0x04000000

	/* do not mess with the rest though */
	char		*ld_defhost;	/* full name of default server */
	int		ld_defport;	/* port of default server */
	BERTranslateProc ld_lber_encode_translate_proc;
	BERTranslateProc ld_lber_decode_translate_proc;
#ifdef LDAP_REFERRALS
	LDAPConn	*ld_defconn;	/* default connection */
	LDAPConn	*ld_conns;	/* list of server connections */
	void		*ld_selectinfo;	/* platform specifics for select */
	int		(*ld_rebindproc)( struct ldap *ld, char **dnp,
				char **passwdp, int *authmethodp, int freeit );
				/* routine to get info needed for re-bind */
#endif /* LDAP_REFERRALS */

        /* V3 ldap controls */
        LDAPControl     **ld_servercontrol;
        LDAPControl     **ld_clientcontrol;
 
        /* Pending results */
        LDAPPend        *ld_pend;       /* list of pending results */

        void            *gsluctx;    /* gslu context handle */ 
        /* client side referral cache-usage enable/disable option */
        int             ld_refcache_enabled; 
        /* current status of the refcache */
        int             ld_refcache_status;
        void            *ld_refcache;  /* client side referral cache */
        void            *ld_wrk_selectinfo;
        LDAPCreds       *ld_creds;     /* credential information */
} LDAP;

/*
 * structure for ldap friendly mapping routines
 */

typedef struct friendly {
	char	*f_unfriendly;
	char	*f_friendly;
} FriendlyMap;


/*
 * handy macro to check whether LDAP struct is set up for CLDAP or not
 */
#define LDAP_IS_CLDAP( ld )	( ld->ld_sb.sb_naddr > 0 )


/*
 * types for ldap URL handling
 */
typedef struct ldap_url_desc {
    char	*lud_host;
    int		lud_port;
    char	*lud_dn;
    char	**lud_attrs;
    int		lud_scope;
    char	*lud_filter;
    char	*lud_string;	/* for internal use only */
} LDAPURLDesc;
#define NULLLDAPURLDESC	((LDAPURLDesc *)NULL)

#define LDAP_URL_ERR_NOTLDAP	1	/* URL doesn't begin with "ldap://" */
#define LDAP_URL_ERR_NODN	2	/* URL has no DN (required) */
#define LDAP_URL_ERR_BADSCOPE	3	/* URL scope string is invalid */
#define LDAP_URL_ERR_MEM	4	/* can't allocate memory space */

/* General Purpose Defines */
#define GSL_ERR_GENERAL 	(int)-1
#define	GSL_NULL_TEXT		(char *)NULL

#ifndef LDAPFUNCDECL
#ifdef _WIN32
#define LDAPFUNCDECL    __declspec( dllexport )
#else /* _WIN32 */
#define LDAPFUNCDECL
#endif /* _WIN32 */
#endif /* LDAPFUNCDECL */

#if !defined(MACOS) && !defined(DOS) && !defined(_WIN32) && !defined(WINSOCK)
#include <sys/time.h>
#endif
LDAPFUNCDECL LDAP *ldap_open( char *host, int port );
LDAPFUNCDECL LDAP *ldap_init( char *defhost, int defport );
LDAPFUNCDECL int ldap_init_SSL( Sockbuf *,char *, char *,int);

LDAPFUNCDECL LDAPMessage *ldap_first_entry( LDAP *ld, LDAPMessage *chain );
LDAPFUNCDECL LDAPMessage *ldap_next_entry( LDAP *ld, LDAPMessage *entry );
LDAPFUNCDECL int ldap_count_entries( LDAP *ld, LDAPMessage *chain );

LDAPFUNCDECL char *ldap_get_dn( LDAP *ld, LDAPMessage *entry );
LDAPFUNCDECL char *ldap_dn2ufn( char *dn );
LDAPFUNCDECL char **ldap_explode_dn( char *dn, int notypes );
LDAPFUNCDECL char **ldap_explode_dns( char *dn );

LDAPFUNCDECL char *ldap_first_attribute( LDAP *ld, LDAPMessage *entry,
        BerElement **ber );
LDAPFUNCDECL char *ldap_next_attribute( LDAP *ld, LDAPMessage *entry,
        BerElement *ber );

LDAPFUNCDECL char **ldap_get_values( LDAP *ld, LDAPMessage *entry, char *target
);
LDAPFUNCDECL struct berval **ldap_get_values_len( LDAP *ld, LDAPMessage *entry,
        char *target );
LDAPFUNCDECL int ldap_count_values( char **vals );
LDAPFUNCDECL int ldap_count_values_len( struct berval **vals );
LDAPFUNCDECL void ldap_value_free( char **vals );
LDAPFUNCDECL void ldap_value_free_len( struct berval **vals );

LDAPFUNCDECL int ldap_result2error( LDAP *ld, LDAPMessage *r, int freeit );
LDAPFUNCDECL char *ldap_err2string( int err );
LDAPFUNCDECL void ldap_perror( LDAP *ld, char *s );

LDAPFUNCDECL void ldap_mods_free( LDAPMod **mods, int freemods );

LDAPFUNCDECL void ldap_free_friendlymap( FriendlyMap **map );

typedef int (*LDAP_SORT_ENTRIES_COMP_CALLBACK) (const char *val1, const char *val2);

LDAPFUNCDECL int ldap_sort_entries( LDAP *ld, LDAPMessage **chain, char *attr,
        LDAP_SORT_ENTRIES_COMP_CALLBACK cmp);

LDAPFUNCDECL int ldap_is_ldap_url( char *url );
LDAPFUNCDECL int ldap_url_parse( char *url, LDAPURLDesc **ludpp );
LDAPFUNCDECL void ldap_free_urldesc( LDAPURLDesc *ludp );
LDAPFUNCDECL int ldap_url_search( LDAP *ld, char *url, int attrsonly );
LDAPFUNCDECL int ldap_url_search_s( LDAP *ld, char *url, int attrsonly,
        LDAPMessage **res );
LDAPFUNCDECL int ldap_url_search_st( LDAP *ld, char *url, int attrsonly,
        struct timeval *timeout, LDAPMessage **res );

LDAPFUNCDECL int ldap_bind( LDAP *ld, char *who, char *passwd, int authmethod );
LDAPFUNCDECL int ldap_bind_s( LDAP *ld, char *who, char *cred, int method );

LDAPFUNCDECL int ldap_simple_bind( LDAP *ld, char *who, char *passwd );
LDAPFUNCDECL int ldap_simple_bind_s( LDAP *ld, char *who, char *passwd );

LDAPFUNCDECL int ldap_compare( LDAP *ld, char *dn, char *attr, char *value );
LDAPFUNCDECL int ldap_compare_s( LDAP *ld, char *dn, char *attr, char *value );

LDAPFUNCDECL int ldap_delete( LDAP *ld, char *dn );
LDAPFUNCDECL int ldap_delete_s( LDAP *ld, char *dn );

LDAPFUNCDECL int ldap_modify( LDAP *ld, char *dn, LDAPMod **mods );
LDAPFUNCDECL int ldap_modify_s( LDAP *ld, char *dn, LDAPMod **mods );

LDAPFUNCDECL int ldap_modrdn( LDAP *ld, char *dn, char *newrdn );
LDAPFUNCDECL int ldap_modrdn_s( LDAP *ld, char *dn, char *newrdn );
LDAPFUNCDECL int ldap_modrdn2( LDAP *ld, char *dn, char *newrdn,
        int deleteoldrdn );
LDAPFUNCDECL int ldap_modrdn2_s( LDAP *ld, char *dn, char *newrdn,
        int deleteoldrdn);

LDAPFUNCDECL int ldap_result( LDAP *ld, int msgid, int all,
        struct timeval *timeout, LDAPMessage **result );
LDAPFUNCDECL int ldap_msgfree( LDAPMessage *lm );
LDAPFUNCDECL int ldap_msgdelete( LDAP *ld, int msgid );
LDAPFUNCDECL void ldap_memfree( void *p );
LDAPFUNCDECL void ber_free( BerElement *ber, int freebuf );

LDAPFUNCDECL BerElement* ber_init( struct berval *bv );
LDAPFUNCDECL int ber_peek_tag(BerElement *ber, int *len);
LDAPFUNCDECL int ber_skip_tag(BerElement *ber, int *len);
LDAPFUNCDECL int ber_scanf ( BerElement *ber, char * fmt, ... );

LDAPFUNCDECL int ldap_parse_result (LDAP *ld, LDAPMessage *res, int *errcodep, char **matchednp, char **errmsgp, char ***referralsp, LDAPControl ***serverctrlsp, int freeit);
LDAPFUNCDECL int ldap_get_lderrno (LDAP *ld, char **m, char **s);

LDAPFUNCDECL int ldap_search( LDAP *ld, char *base, int scope, char *filter,
        char **attrs, int attrsonly );
LDAPFUNCDECL int ldap_search_s( LDAP *ld, char *base, int scope, char *filter,
        char **attrs, int attrsonly, LDAPMessage **res );
LDAPFUNCDECL int ldap_search_st( LDAP *ld, char *base, int scope, char *filter,
    char **attrs, int attrsonly, struct timeval *timeout, LDAPMessage **res );

LDAPFUNCDECL int ldap_unbind( LDAP *ld );
LDAPFUNCDECL int ldap_unbind_s( LDAP *ld );

LDAPFUNCDECL int ldap_abandon( LDAP *ld, int msgid );

LDAPFUNCDECL int ldap_add( LDAP *ld, char *dn, LDAPMod **attrs );
LDAPFUNCDECL int ldap_add_s( LDAP *ld, char *dn, LDAPMod **attrs );

LDAPFUNCDECL int ldap_rename( LDAP *ld, char *dn, char *newrdn, char *newparent,
                            int deleteoldrdn, LDAPControl **serverctrls,
                            LDAPControl **clientctrls, int *msgidp);   
LDAPFUNCDECL int ldap_rename_s(LDAP *ld,char *dn,char *newrdn,char *newparent,
                            int deleteoldrdn,PLDAPControl *serverctrls,
                            PLDAPControl    *clientctrls);   

LDAPFUNCDECL LDAPFiltDesc *ldap_init_getfilter( char *fname );
LDAPFUNCDECL LDAPFiltDesc *ldap_init_getfilter_buf( char *buf, long buflen );
LDAPFUNCDECL LDAPFiltInfo *ldap_getfirstfilter( LDAPFiltDesc *lfdp, char *tagpat,
	char *value );
LDAPFUNCDECL LDAPFiltInfo *ldap_getnextfilter( LDAPFiltDesc *lfdp );
LDAPFUNCDECL int ldap_setfilteraffixes( LDAPFiltDesc *lfdp, char *prefix, char *suffix );
LDAPFUNCDECL int ldap_build_filter( char *buf, unsigned long buflen,
	char *pattern, char *prefix, char *suffix, char *attr, char *value, char **valwords );
LDAPFUNCDECL void ldap_getfilter_free(LDAPFiltDesc *lfdp);
LDAPFUNCDECL int ldap_abandon_ext( LDAP *ld, int msgid,
	LDAPControl **serverctrls,  LDAPControl **clientctrls );

LDAPFUNCDECL char* ldap_utf8strtok_r (char* src, const char* delim, char** saveptr);
LDAPFUNCDECL int ldap_utf8isspace (char* s);

LDAPFUNCDECL int ldap_add_ext( LDAP *ld, char *dn, LDAPMod **attrs,
		LDAPControl **serverctrls,  LDAPControl **clientctrls,
		int  *msgidp );
LDAPFUNCDECL int ldap_add_ext_s( LDAP *ld, char *dn, LDAPMod **attrs,
		LDAPControl **serverctrls,  LDAPControl **clientctrls);

LDAPFUNCDECL int ldap_replic_bind( LDAP *ld, char *dn, char *passwd );
LDAPFUNCDECL int ldap_replic_bind_s( LDAP *ld, char *dn, char *passwd );

LDAPFUNCDECL int ldap_kerberos_bind_s( LDAP *ld, char *who );
LDAPFUNCDECL int ldap_kerberos_bind1( LDAP *ld, char *who );
LDAPFUNCDECL int ldap_kerberos_bind1_s( LDAP *ld, char *who );
LDAPFUNCDECL int ldap_kerberos_bind2( LDAP *ld, char *who );
LDAPFUNCDECL int ldap_kerberos_bind2_s( LDAP *ld, char *who );

LDAPFUNCDECL int ldap_sasl_bind(LDAP * ld, char * dn, char * mechanism,
				struct berval * cred, 
				LDAPControl   ** serverctrls,
				LDAPControl   ** clientctrls,
				int  * msgidp);
LDAPFUNCDECL int ldap_sasl_bind_s(LDAP * ld, char * dn, char * mechanism,
				struct berval * cred, 
				LDAPControl   ** serverctrls,
				LDAPControl   ** clientctrls);

LDAPFUNCDECL int ldap_parse_sasl_bind_result(LDAP * ld, LDAPMessage * res,
				struct berval ** servercredp,
				int freeit);

LDAPFUNCDECL int ldap_native_bind_s(LDAP * ld,char * dn,char * mechanism,
					struct berval * cred,
					unsigned int dir_type);

LDAPFUNCDECL int ldap_compare_ext( LDAP *ld, char *dn, char *attr, 
				struct berval *value, LDAPControl **serverctrls,
				LDAPControl **clientctrls, int *msgidp );
LDAPFUNCDECL int ldap_compare_ext_s( LDAP *ld, char *dn, char *attr, 
				struct berval *value, LDAPControl **serverctrls,
				LDAPControl **clientctrls );

LDAPFUNCDECL int ldap_delete_ext( LDAP *ld, char *dn, 
			LDAPControl **serverctrls, LDAPControl **clientctrls,
			int *msgidp );
LDAPFUNCDECL int ldap_delete_ext_s( LDAP *ld, char *dn,
			LDAPControl **serverctrls, LDAPControl **clientctrls );

LDAPFUNCDECL int ldap_modify_ext( LDAP *ld, char *dn, LDAPMod **mods,
			 LDAPControl **serverctrls, LDAPControl **clientctrls,
			 int *msgidp );
LDAPFUNCDECL int ldap_modify_ext_s( LDAP *ld, char *dn, LDAPMod **mods,
			 LDAPControl **serverctrls, LDAPControl **clientctrls );


LDAPFUNCDECL int ldap_get_entry_controls(  LDAP *ld, LDAPMessage *entry,
					 LDAPControl ***serverctrlsp);

LDAPFUNCDECL char **ldap_explode_rdn( char *dn, int notypes );

LDAPFUNCDECL int ldap_search_ext ( LDAP *ld, char *base, int scope, 
			char *filter, char **attrs, int attrsonly,
                        LDAPControl **serverctrls, LDAPControl **clientctrls,
			struct timeval *timeoutp, int sizelimit, int *msgidp );
LDAPFUNCDECL int ldap_search_ext_s ( LDAP *ld, char *base, int scope, 
			char *filter, char **attrs, int attrsonly,
                        LDAPControl **serverctrls, LDAPControl **clientctrls,
			struct timeval *timeoutp, int sizelimit, 
			LDAPMessage **res);

LDAPFUNCDECL int ldap_set_option( LDAP *ld, int option, void *optdata );
LDAPFUNCDECL int ldap_get_option( LDAP *ld, int option, void *optdata );

LDAPFUNCDECL void ldap_control_free( LDAPControl *ctrl );
LDAPFUNCDECL void ldap_controls_free( LDAPControl **ctrl );

/* extended operations */
LDAPFUNCDECL int ldap_extended_operation (LDAP *ld, const char *requestoid, struct berval *requestdata,
                             LDAPControl **serverctrls, LDAPControl **clientctrls, int *msgidp);
LDAPFUNCDECL int ldap_extended_operation_s (LDAP *ld, const char *requestoid, struct berval *requestdata,
                               LDAPControl **serverctrls, LDAPControl **clientctrls,
                               char **retoidp, struct berval **retdatap);
LDAPFUNCDECL int ldap_parse_extended_result (LDAP *ld, LDAPMessage *res, char **retoidp,
                                struct berval **retdatap, int freeit);

LDAPFUNCDECL int ldap_msgid( LDAPMessage *lm );
LDAPFUNCDECL int ldap_msgtype( LDAPMessage *lm );

LDAPFUNCDECL LDAPMessage* ldap_next_reference( LDAP *ld, LDAPMessage *res );
LDAPFUNCDECL LDAPMessage* ldap_first_reference( LDAP *ld, LDAPMessage *res );
LDAPFUNCDECL int ldap_count_references( LDAP *ld, LDAPMessage *res );

LDAPFUNCDECL LDAPMessage* ldap_first_message( LDAP *ld, LDAPMessage *chain );
LDAPFUNCDECL LDAPMessage* ldap_next_message( LDAP *ld, LDAPMessage *chain );
LDAPFUNCDECL int ldap_count_messages( LDAP *ld, LDAPMessage *chain );

LDAPFUNCDECL int ora_ldap_normalize_dn( char *dn, char *normDn);
LDAPFUNCDECL int ora_ldap_normalize_dn_with_case( char *dn, char *normDn, 
						int lower_case);

#ifdef LDAP_REFERRALS
LDAPFUNCDECL void ldap_set_rebind_proc(LDAP * ld, int (*rebindproc) (LDAP * ld,
                char **dnp, char **passwdp, int *authmethodp, int freeit));
LDAPFUNCDECL void ora_ldap_set_rebind_proc(OraLdapClientCtx * clientCtx,
                LDAP * ld, int (*rebindproc) (LDAP * ld, char **dnp,
                            char **passwdp, int *authmethodp, int freeit));
#endif /*LDAP_REFERRALS*/



/* SASL functions */

LDAPFUNCDECL int ora_ldap_init_SASL (OraLdapClientCtx *clientCtx, LDAP *ld, 
                                 char * dn, char * mech, void * cred, 
                                 LDAPControl **serverctrls, 
                                 LDAPControl **clientctrls);

LDAPFUNCDECL OraLdapHandle ora_ldap_create_cred_hdl (OraLdapClientCtx *clientCtx,int credType);

LDAPFUNCDECL int ora_ldap_set_cred_props (OraLdapClientCtx * clientCtx, OraLdapHandle cred, 
                             int propType, void *inProperty);

LDAPFUNCDECL int ora_ldap_get_cred_props (OraLdapClientCtx * clientCtx, OraLdapHandle cred, 
                             int propType, void *outProperty);

LDAPFUNCDECL int ora_ldap_free_cred_hdl (OraLdapClientCtx * clientCtx, OraLdapHandle cred);

/* LDAP Client Context Functions */

LDAPFUNCDECL int ora_ldap_create_clientctx(OraLdapClientCtx ** clientCtx);

LDAPFUNCDECL int ora_ldap_set_clientctx(OraLdapClientCtx * clientCtx, 
                                         int property_type, 
                                         void * property);

LDAPFUNCDECL int ora_ldap_destroy_clientctx(OraLdapClientCtx ** clientCtx);

/*
 * discover server functions
 */
LDAPFUNCDECL int ora_ldap_discover ( OraLdapHandle     hdl,
                                     OraResultHandle   * reshdl);
LDAPFUNCDECL LDAP *ora_ldap_discover_open( OraLdapHandle  hdl);

LDAPFUNCDECL OraLdapHandle  ora_create_discov_hdl(int  discoveryType);

LDAPFUNCDECL int  ora_free_discov_hdl(OraLdapHandle  hdl);
LDAPFUNCDECL int  ora_free_result_hdl(OraResultHandle  reshdl);

LDAPFUNCDECL int ora_get_result( OraResultHandle  reshdl,
                                         int resType, char *** result);

LDAPFUNCDECL int ora_set_discov_prop( OraLdapHandle  hdl,
                                         int prop, void * inval);

LDAPFUNCDECL int ora_get_discov_prop( OraLdapHandle  hdl,
                                         int prop, void * outval);


#if defined(ultrix) || defined( nextstep )
extern char *strdup();
#endif

#ifdef __cplusplus
}
#endif
#endif
