/* ----------------------------------------------------------------------
   Oracle Data Structures.
 Copyright (c) 2003, 2017, Oracle and/or its affiliates. All rights reserved.
   ---------------------------------------------------------------------- */
 
#ifndef ORASTRUC
# define ORASTRUC

# ifndef ORATYPES
#  include <oratypes.h>
# endif

typedef ub4 oraerr;    /* Error code: 0 is success, non-0 is failure */
#define ORAERR_OK 0    /* Universal success code */

typedef ub4 oracheck;  /* Checkword for validating data structures */

#if 0 /* COMMENT */
  -------------------------------------------------------------------------- 
                           Hooked Memory Allocation

  Covers for the standard memory allocation functions that allow user hooks
  to replace standard calls.  Use the function prototype macros
  ORAMEM_ALLOC_F and ORAMEM_FREE_F to declare callbacks; if neither is
  provided, malloc() and free() are used.  Either both or neither must be
  given, you cannot mix them.

    STATICF ORAMEM_ALLOC_F(my_alloc, mctx, size)
    {
        mycontext *myctx = mctx;
        void      *ptr; /* return value */
        ...
        return _my_funky_alloc(myctx, size);
    }

    STATICF ORAMEM_FREE_F(my_free, mctx, ptr)
    {
        mycontext *myctx = mctx;
        ...
        _my_funky_free(myctx, ptr);
    }

    main()
    {
        oramemctx *memctx;
        mycontext  myctx;
        ...
        /* initialize myctx here */
        memctx = OraMemInit(&myctx, my_alloc, my_free);
        a = OraMemAlloc(memctx, 1000);
        b = OraMemAlloc(memctx, sizeof(double));
        ...
        OraMemFree(memctx, a);
        OraMemFree(memctx, b);
        OraMemTerm(memctx);
        ...
    }

  --------------------------------------------------------------------------
#endif /* if 0 COMMENT */

/* Opaque memory context */

struct oramemctx;
typedef struct oramemctx oramemctx;

/* Function prototype macros */

#define ORAMEM_ALLOC_F(func, mctx, size) void *func(void *mctx, size_t size)
#define ORAMEM_FREE_F(func, mctx, ptr)   void  func(void *mctx, void *ptr)

/* Create & Destroy memory context */

oramemctx *OraMemInit(void *mctx, ORAMEM_ALLOC_F((*alloc), mctx, size),
                      ORAMEM_FREE_F((*free), mctx, ptr));
void       OraMemTerm(oramemctx *memctx);

/* Allocate & Free memory */

void      *OraMemAlloc(oramemctx *memctx, size_t size);
void      *OraMemCalloc(oramemctx *memctx, size_t size);
void       OraMemFree(oramemctx *memctx, void *ptr);

/*-------------------------------------------------------------------------- 
                               Named Properties

  Some complex top-level APIs take variable-argument name-value pairs
  instead of a fixed set of arguments, to increase flexibility.  Properties
  of this sort are defined by a name, a type, and an enum ID.  When multiple
  sets of pairs is needed, separate them with ORAPROP_SEP.
  -------------------------------------------------------------------------- */

#define ORAPROP_SEP        "__PROPERTY_SEPARATOR__"

typedef sb2 oraprop_id;        /* >= 0 valid, < 0 invalid */

/* type: property value type */

typedef enum {
    ORAPROP_TYPE_BOOLEAN,    /* (boolean) */
    ORAPROP_TYPE_SIGNED,     /* (sb4) */
    ORAPROP_TYPE_UNSIGNED,   /* (ub4) */
    ORAPROP_TYPE_POINTER     /* (void *) */
} oraprop_t;

/* value: union of storage for all data types */

typedef union oraprop_v {
    boolean b_oraprop_v;    /* boolean */
    sb4     s_oraprop_v;    /* signed scalar */
    ub4     u_oraprop_v;    /* unsigned scalar */
    void   *p_oraprop_v;    /* pointer */
} oraprop_v;

/* property */

typedef struct oraprop {
    oratext    *name_oraprop;    /* name of property [compiler enc.] */
    oraprop_id  id_oraprop;      /* unique numeric ID for property */
    oraprop_t   type_oraprop;    /* value type */
    oraprop_v   value_oraprop;   /* value storage */
} oraprop;

/* convenient macro for constructing property tables */

#define ORAPROP(name, id, type) \
    { (oratext *) name, (oraprop_id) id, ORAPROP_TYPE_##type }

#define ORAPROP_ERR_NULL_PTR      1    /* NULL pointer argument */
#define ORAPROP_ERR_NO_PROPERTY   2    /* no such named property */

/* given a property table and a property name, look it up it table */
const oraprop *OraPropGet(const oraprop *proptab, oratext *name);

/* look up property by name and return it's value (if type is known) */
oraerr   OraPropGetBoolean(oraprop *proptab, oratext *name, boolean *value);
oraerr   OraPropGetSigned(oraprop *proptab, oratext *name, sb4 *value);
oraerr   OraPropGetUnsigned(oraprop *proptab, oratext *name, ub4 *value);
oraerr   OraPropGetPointer(oraprop *proptab, oratext *name, void **value);

/*-------------------------------------------------------------------------- 
                               Streaming data

  Generic streaming data/chunking API.  All functions return an error code,
  zero on success.  All functions take a user-defined context pointer <ctx>
  and a user-defined stream identifier <id>.

   ---------------- Creation properties (OraStreamInit) --------------------

  Creation properties (OraStreamInit) are:
    PROPERTY NAME        PROPERTY VALUE
    "oramem_context"    (oramemctx *)

        Where to allocate memory for stream object (default: malloc)

    PROPERTY NAME        PROPERTY VALUE
    "open"               ORASTREAM_OPEN_F((*), sctx, sid, hdl, length)

        Open function.  Not required.

    PROPERTY NAME        PROPERTY VALUE
    "close"              ORASTREAM_CLOSE_F((*), sctx, sid, hdl)

        Close function.  Not required.

    PROPERTY NAME        PROPERTY VALUE
    "read"               ORASTREAM_READ_F((*), sctx, sid, hdl, dest,
                         size, start, nread, eoi)

        Read byte data from stream to buf.    nread returns number
        of bytes actually read.

    PROPERTY NAME        PROPERTY VALUE
    "write"              ORASTREAM_WRITE_F((*), sctx, sid, hdl,
                         src, size, written)

        Write byte data from buf to stream.  Number of bytes actually
        written is returned through <written>.

    PROPERTY NAME        PROPERTY VALUE
    "read_char"          ORASTREAM_READ_F((*), sctx, sid, hdl, dest,
                         size, start, nread, eoi)

        Read character data from stream to buf.    nread returns number
        of chars actually read.

    PROPERTY NAME        PROPERTY VALUE
    "write_char"         ORASTREAM_WRITE_F((*), sctx, sid, hdl,
                         src, size, written)

        Write character data from buf to stream.  Number of chars
        actually written is returned through <written>.

    Streams are byte or character mode:
    If "read" or "write" is provided, the stream operates in byte
    mode using OraStreamRead() and OraStreamWrite().  If "read_char"
    or "write_char" is provided, the stream operates in character mode
    using OraStreamReadChar() and OraStreamWriteChar().  In char mode,
    only complete chars are read/written, and are never split over
    buffer boundaries.

    Streams are unidirectional:
    Only one read or write function must be provided, but not both.

   ------------------ Storage for file handles, etc ----------------------- */

typedef union orastreamhdl {
    void  *ptr_orastreamhdl;    /* generic pointer stream/file/etc */
    struct {
        sb4 fd_orastreamhdl;    /* file descriptor(s) [FTP needs all 3!] */
        sb4 fd2_orastreamhdl;
        sb4 fd3_orastreamhdl;
    } fds_lpihdl;
} orastreamhdl;

/* ----------------- Accessor macros for stream handles ------------------ */

#define ORASTREAM_PTR(hdl)    hdl->ptr_orastreamhdl
#define ORASTREAM_FD(hdl)     hdl->fds_lpihdl.fd_orastreamhdl
#define ORASTREAM_FD2(hdl)    hdl->fds_lpihdl.fd2_orastreamhdl
#define ORASTREAM_FD3(hdl)    hdl->fds_lpihdl.fd3_orastreamhdl

/* ------------------- Stream Function Prototype Macros ------------------- */

/* Open stream <sid>; if total size of data is known, <length> is set
   (if not NULL).
*/

#define ORASTREAM_OPEN_F(func, sctx, sid, hdl, length) \
    oraerr func(void *sctx, void *sid, orastreamhdl *hdl, ubig_ora *length)

/* Close stream <sid> */

#define ORASTREAM_CLOSE_F(func, sctx, sid, hdl) \
    oraerr func(void *sctx, void *sid, orastreamhdl *hdl)

/* Read from stream <sid> into buffer <dest> which is of size <size> bytes;
   <nread> is set to the # of bytes or characters read, and <eoi> set to TRUE
   if this is the last chunk of data.  <start> is updated to point to the
   first actual data bytes in the buffer (e.g. first HTTP buffer contains a
   header which is skipped over).
*/

#define ORASTREAM_READ_F(func, sctx, sid, hdl, dest, size, start, nread, eoi) \
    oraerr func(void *sctx, void *sid, orastreamhdl *hdl, oratext *dest, \
        ubig_ora size, oratext **start, ubig_ora *nread, ub1 *eoi)

/* Write to stream <sid> the <size> bytes of data at <src>; the number of
   bytes or characters successfully written is returned through <written>
*/

#define ORASTREAM_WRITE_F(func, sctx, sid, hdl, src, size, written) \
    oraerr func(void *sctx, void *sid, orastreamhdl *hdl, \
        oratext *src, ubig_ora size, ubig_ora *written)

/* ------------------------ Opaque Stream Object ------------------------ */

struct orastream;
typedef struct orastream orastream;

/* -------------------------- Stream Functions -------------------------- */

/* Initialize & Destroy stream object */

orastream *OraStreamInit(void *sctx, void *sid, oraerr *err, ...);
orastream *OraStreamInitVA(void *sctx, void *sid, oraerr *err, va_list va);
oraerr     OraStreamTerm(orastream *stream);

/* [Re]Set SID for stream (returns old SID through <osid>)*/

oraerr     OraStreamSid(orastream *stream, void *sid, void **osid);

/* Is a stream readable or writable? */

boolean    OraStreamReadable(orastream *stream);
boolean    OraStreamWritable(orastream *stream);

/* DEPRECATED DUE TO TYPO IN FUNCTION NAME; USE OraStreamWritable() INSTEAD */
boolean    OraStreamWriteable(orastream *stream);

/* Open & Close stream */

oraerr     OraStreamOpen(orastream *stream, ubig_ora *length);
oraerr     OraStreamClose(orastream *stream);

/* Read | Write byte stream */

oraerr     OraStreamRead(orastream *stream, oratext *dest, ubig_ora size,
             oratext **start, ubig_ora *nread, ub1 *eoi);
oraerr     OraStreamWrite(orastream *stream, oratext *src, ubig_ora size,
              ubig_ora *nwrote);

/* Read | Write char stream */

oraerr     OraStreamReadChar(orastream *stream, oratext *dest, ubig_ora size,
                 oratext **start, ubig_ora *nread, ub1 *eoi);
oraerr     OraStreamWriteChar(orastream *stream, oratext *src, ubig_ora size,
                  ubig_ora *nwrote);

/* Return handles for stream */

orastreamhdl *OraStreamHandle(orastream *stream);

/* Return the cloned stream object */
orastream *OraStreamClone(oramemctx *mem, void *sid, orastream *source,
                                          oraerr *errcode);

/* Returns status if the stream object is currently opened */
boolean OraStreamIsOpen(orastream *stream);

/* -------------------------- Stream Error Codes ------------------------- */

#define ORASTREAM_ERR_NULL_POINTER     1    /* NULL pointer given */
#define ORASTREAM_ERR_BAD_STREAM       2    /* invalid stream object */
#define ORASTREAM_ERR_WRONG_DIRECTION  3    /* tried wrong-direction I/O */
#define ORASTREAM_ERR_UNKNOWN_PROPERTY 4    /* unknown creation prop */
#define ORASTREAM_ERR_NO_DIRECTION     5    /* neither read nor write? */
#define ORASTREAM_ERR_BI_DIRECTION     6    /* both read any write? */
#define ORASTREAM_ERR_NOT_OPEN         7    /* stream not open */
#define ORASTREAM_ERR_WRONG_MODE       8    /* wrote byte/char mode */
#define ORASTREAM_ERR_OUTOF_MEMORY     9    /* ran out of memory */
/* --- Open errors --- */
#define ORASTREAM_ERR_CANT_OPEN       10    /* can't open stream */
/* --- Close errors --- */
#define ORASTREAM_ERR_CANT_CLOSE      20    /* can't close stream */

#endif /* ifndef ORASTRUC */
