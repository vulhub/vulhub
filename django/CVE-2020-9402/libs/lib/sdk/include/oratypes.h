/* $RCSfile: oratypes.h $ $Date: 20-jul-00.13:44:19 
   ----------------------------------------------------------------------
 Copyright (c) 1982, 2017, Oracle and/or its affiliates. All rights reserved.
   ---------------------------------------------------------------------- */
 
#ifndef ORATYPES
# define ORATYPES
# define SX_ORACLE
# define SX3_ORACLE 

#ifndef ORASTDDEF
# include <stddef.h>
# define ORASTDDEF
#endif

#ifndef ORALIMITS
# include <limits.h>
# define ORALIMITS
#endif

#ifndef ORASTDARG
# include <stdarg.h>
# define ORASTDARG
#endif

#ifndef TRUE
# define TRUE  1
# define FALSE 0
#endif

/* --- Signed/Unsigned one-byte scalar (sb1/ub1) --- */

#ifndef lint
  typedef unsigned char ub1;
  typedef   signed char sb1;
  typedef          char eb1;
#else 
# define ub1 unsigned char
# define sb1 signed char
# define eb1 char
#endif

#define UB1MAXVAL    ((ub1)UCHAR_MAX)
#define UB1MINVAL    ((ub1)       0)
#define SB1MAXVAL    ((sb1)SCHAR_MAX)
#define SB1MINVAL    ((sb1)SCHAR_MIN)

#define MINUB1MAXVAL ((ub1)  255)
#define MAXUB1MINVAL ((ub1)    0)
#define MINSB1MAXVAL ((sb1)  127)
#define MAXSB1MINVAL ((sb1) -127)

#define EB1MAXVAL    ((eb1)SCHAR_MAX)
#define EB1MINVAL    ((eb1)        0)

#define MINEB1MAXVAL ((eb1)  127)
#define MAXEB1MINVAL ((eb1)    0)

#define UB1BITS      CHAR_BIT
#define UB1MASK      ((1 << ((uword)CHAR_BIT)) - 1)

/* backwards compatibility */

#ifndef lint
  typedef sb1 b1;
#else
# define b1 sb1
#endif

#define B1MAXVAL SB1MAXVAL
#define B1MINVAL SB1MINVAL

/* --- Signed/Unsigned two-byte scalar (sb2/ub2) --- */

#ifndef lint 
  typedef unsigned short ub2;
  typedef   signed short sb2;
  typedef          short eb2;
#else
# define ub2 unsigned short
# define sb2 signed short
# define eb2 short
#endif

#define UB2MAXVAL    ((ub2)USHRT_MAX)
#define UB2MINVAL    ((ub2)        0)
#define SB2MAXVAL    ((sb2) SHRT_MAX)
#define SB2MINVAL    ((sb2) SHRT_MIN)

#define MINUB2MAXVAL ((ub2) 65535)
#define MAXUB2MINVAL ((ub2)     0)
#define MINSB2MAXVAL ((sb2) 32767)
#define MAXSB2MINVAL ((sb2)-32767)

#define EB2MAXVAL    ((eb2) SHRT_MAX)
#define EB2MINVAL    ((eb2)        0)

#define MINEB2MAXVAL ((eb2) 32767)
#define MAXEB2MINVAL ((eb2)     0)

/* backwards compatibility */

#ifndef lint
  typedef sb2 b2;
#else
# define b2 sb2
#endif

#define B2MAXVAL SB2MAXVAL
#define B2MINVAL SB2MINVAL

/* --- Signed/Unsigned four-byte scalar (sb4/ub4) --- */

#ifndef lint 
  typedef unsigned int ub4;
  typedef   signed int sb4;
  typedef          int eb4;
#else
# define ub4 unsigned int
# define sb4 signed int
# define eb4 int
#endif

#define UB4MAXVAL    ((ub4)UINT_MAX)
#define UB4MINVAL    ((ub4)       0)
#define SB4MAXVAL    ((sb4) INT_MAX)
#define SB4MINVAL    ((sb4) INT_MIN)

#define MINUB4MAXVAL ((ub4) 4294967295)
#define MAXUB4MINVAL ((ub4)          0)
#define MINSB4MAXVAL ((sb4) 2147483647)
#define MAXSB4MINVAL ((sb4)-2147483647)

#define EB4MAXVAL    ((eb4) INT_MAX)
#define EB4MINVAL    ((eb4)       0)

#define MINEB4MAXVAL ((eb4) 2147483647)
#define MAXEB4MINVAL ((eb4)          0)

/* --- Signed/Unsigned eight-byte scalar (orasb8/oraub8) --- */

# define ORAXB8_DEFINED
# ifndef lint
#  ifdef __LP64__
    typedef unsigned long oraub8;
    typedef   signed long orasb8;
#  else
    typedef unsigned long long oraub8;
    typedef   signed long long orasb8;
#  endif /* __LP64__ */
    typedef oraub8 ub8;
    typedef orasb8 sb8;
# else
#  define ub8 oraub8
#  define sb8 orasb8
#  ifdef __LP64__
#   define oraub8 unsigned long
#   define orasb8 signed long
#  else
#   define oraub8 unsigned long long
#   define orasb8 signed long long
#  endif /* __LP64__ */
# endif /* !lint */

# define ORAUB8MINVAL    ((oraub8)0)
# define ORAUB8MAXVAL    ((oraub8)18446744073709551615)
# define ORASB8MINVAL    ((orasb8)-9223372036854775808)
# define ORASB8MAXVAL    ((orasb8) 9223372036854775807)

# define MAXORAUB8MINVAL ((oraub8)0)
# define MINORAUB8MAXVAL ((oraub8)18446744073709551615)
# define MAXORASB8MINVAL ((orasb8)-9223372036854775807)
# define MINORASB8MAXVAL ((orasb8) 9223372036854775807)

/* backwards compatibility */

#ifndef lint
  typedef sb4 b4;
#else
# define b4 sb4
#endif

#define B4MAXVAL SB4MAXVAL
#define B4MINVAL SB4MINVAL

/* --- Character pointer --- */

#ifdef lint
# define oratext unsigned char
#else
  typedef unsigned char oratext;
#endif

#if !defined(LUSEMFC)
# ifdef lint
#  define text    unsigned char
#  define OraText oratext
# else
   typedef oratext text;
   typedef oratext OraText;
# endif
#endif

#if !defined(MOTIF) && !defined(LISPL)  && !defined(__cplusplus) && \
    !defined(LUSEMFC)
  typedef OraText *string;        
#endif 

#ifndef lint
  typedef unsigned short utext;
#else
# define utext  unsigned short
#endif

/* --- Boolean --- */

#ifndef boolean
# ifndef lint
   typedef int boolean;
# else
#  define boolean int
# endif
#endif

/* --- Other data types --- */

#ifndef lint
  typedef          int eword;
  typedef unsigned int uword;
  typedef   signed int sword;
#else
# define eword int
# define uword unsigned int
# define sword signed int
#endif 

#define  EWORDMAXVAL  ((eword) INT_MAX)
#define  EWORDMINVAL  ((eword)       0)

#define  UWORDMAXVAL  ((uword)UINT_MAX)
#define  UWORDMINVAL  ((uword)       0)

#define  SWORDMAXVAL  ((sword) INT_MAX)
#define  SWORDMINVAL  ((sword) INT_MIN)

#define  MINEWORDMAXVAL  ((eword)  2147483647)
#define  MAXEWORDMINVAL  ((eword)           0)

#define  MINUWORDMAXVAL  ((uword)  4294967295)
#define  MAXUWORDMINVAL  ((uword)           0)

#define  MINSWORDMAXVAL  ((sword)  2147483647)
#define  MAXSWORDMINVAL  ((sword) -2147483647)

#ifndef lint
  typedef unsigned long  ubig_ora;             
  typedef   signed long  sbig_ora;             
#else
# define ubig_ora unsigned long
# define sbig_ora signed long
#endif 

#define UBIG_ORAMAXVAL ((ubig_ora)ULONG_MAX)
#define UBIG_ORAMINVAL ((ubig_ora)        0)

#define SBIG_ORAMAXVAL ((sbig_ora) LONG_MAX)
#define SBIG_ORAMINVAL ((sbig_ora) LONG_MIN)

#define MINUBIG_ORAMAXVAL ((ubig_ora) 4294967295)
#define MAXUBIG_ORAMINVAL ((ubig_ora)          0)

#define MINSBIG_ORAMAXVAL ((sbig_ora) 2147483647)
#define MAXSBIG_ORAMINVAL ((sbig_ora)-2147483647)

#define UBIGORABITS      (UB1BITS * sizeof(ubig_ora))

/* --- Const --- */

#undef CONST
#define CONST const

/* --- Misc --- */

#define dvoid void

typedef void (*lgenfp_t)( void );

#ifndef ORASYS_TYPES
# include <sys/types.h>
# define ORASYS_TYPES
#endif 

#ifndef SIZE_TMAXVAL
# define SIZE_TMAXVAL UBIG_ORAMAXVAL
#endif

#ifndef MINSIZE_TMAXVAL
# define MINSIZE_TMAXVAL (size_t)4294967295
#endif

#endif /* ORATYPES */
