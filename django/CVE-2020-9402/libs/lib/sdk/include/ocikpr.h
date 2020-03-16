/* Copyright (c) 1991, 2005, Oracle. All rights reserved.  */
/*
   NAME
     ocikpr.h - header of K & R compilers
   MODIFIED   (MM/DD/YY)
    mbastawa   09/16/05 - dbhygiene 
    porangas   12/04/00 - Forward merge bug#974710 to 9i
    sgollapu   05/19/98 - Change text to OraText
    dchatter   04/21/96 -
    dchatter   11/10/95 -  add ognfd() - get native fd
    lchidamb   04/06/95 -  drop maxdsz from obindps/odefinps
    slari      04/07/95 -  add opinit
    dchatter   03/08/95 -  osetpi and ogetpi
    lchidamb   12/09/94 -  add obindps() and odefinps()
    dchatter   03/06/95 -  merge changes from branch 1.1.720.2
    dchatter   11/14/94 -  merge changes from branch 1.1.720.1
    dchatter   02/08/95 -  olog call; drop onblon
    dchatter   10/31/94 -  new functions for non-blocking oci
    rkooi2     11/27/92 -  Changing datatypes (in comments) and return types 
    rkooi2     10/26/92 -  More portability mods 
    rkooi2     10/18/92 -  Changed to agree with oci.c 
    sjain      03/16/92 -  Creation 
*/

/*
 *  Declare the OCI functions.
 *  Prototype information is commented out.
 *  Use this header for non-ANSI C compilers.
 *  Note that you will need to include ocidfn.h in the .c files
 *    to get the definition for cda_def.
 */

#ifndef OCIKPR
#define OCIKPR

#include <oratypes.h>

/*
 * Oci BIND (Piecewise or with Skips) 
 */
sword  obindps(  struct cda_def *cursor, ub1 opcode, OraText *sqlvar, 
                  sb4 sqlvl, ub1 *pvctx, sb4 progvl, 
                  sword ftype, sword scale,
                  sb2 *indp, ub2 *alen, ub2 *arcode, 
                  sb4 pv_skip, sb4 ind_skip, sb4 alen_skip, sb4 rc_skip,
                  ub4 maxsiz, ub4 *cursiz, 
                  OraText *fmt, sb4 fmtl, sword fmtt  );
sword  obreak(  struct cda_def *lda  );
sword  ocan  (  struct cda_def *cursor  );
sword  oclose(  struct cda_def *cursor  );
sword  ocof  (  struct cda_def *lda  );
sword  ocom  (  struct cda_def *lda  );
sword  ocon  (  struct cda_def *lda  );


/*
 * Oci DEFINe (Piecewise or with Skips) 
 */
sword  odefinps(  struct cda_def *cursor, ub1 opcode, sword pos,ub1 *bufctx,
                   sb4 bufl, sword ftype, sword scale, 
                   sb2 *indp, OraText *fmt, sb4 fmtl, sword fmtt, 
                   ub2 *rlen, ub2 *rcode,
                   sb4 pv_skip, sb4 ind_skip, sb4 alen_skip, sb4 rc_skip  );
sword  odescr(  struct cda_def *cursor, sword pos, sb4 *dbsize,
                   sb2 *dbtype, sb1 *cbuf, sb4 *cbufl, sb4 *dsize,
                   sb2 *prec, sb2 *scale, sb2 *nullok  );
sword  odessp(  struct cda_def *cursor, OraText *objnam, size_t onlen,
                   ub1 *rsv1, size_t rsv1ln, ub1 *rsv2, size_t rsv2ln,
                   ub2 *ovrld, ub2 *pos, ub2 *level, OraText **argnam,
                   ub2 *arnlen, ub2 *dtype, ub1 *defsup, ub1* mode,
                   ub4 *dtsiz, sb2 *prec, sb2 *scale, ub1 *radix,
                   ub4 *spare, ub4 *arrsiz  );
sword  oerhms(  struct cda_def *lda, sb2 rcode, OraText *buf,
                   sword bufsiz  );
sword  oermsg(  sb2 rcode, OraText *buf  );
sword  oexec (  struct cda_def *cursor  );
sword  oexfet(  struct cda_def *cursor, ub4 nrows,
                   sword cancel, sword exact  );
sword  oexn  (  struct cda_def *cursor, sword iters, sword rowoff  );
sword  ofen  (  struct cda_def *cursor, sword nrows  );
sword  ofetch(  struct cda_def *cursor  );
sword  oflng (  struct cda_def *cursor, sword pos, ub1 *buf,
                   sb4 bufl, sword dtype, ub4 *retl, sb4 offset  );
sword  ogetpi(  struct cda_def *cursor, ub1 *piecep, void  **ctxpp, 
                   ub4 *iterp, ub4 *indexp  );
sword  opinit(  ub4 mode  );
sword  olog  (  struct cda_def *lda, ub1 *hst, 
                   OraText *uid, sword uidl,
                   OraText *psw, sword pswl, 
                   OraText *conn, sword connl, 
                   ub4 mode  );
sword  ologof(  struct cda_def *lda  );
sword  oopen (  struct cda_def *cursor, struct cda_def *lda,
                   OraText *dbn, sword dbnl, sword arsize,
                   OraText *uid, sword uidl  );
sword  oopt  (  struct cda_def *cursor, sword rbopt, sword waitopt  );
sword  oparse(  struct cda_def *cursor, OraText *sqlstm, sb4 sqllen,
                   sword defflg, ub4 lngflg  );
sword  orol  (  struct cda_def *lda  );
sword  osetpi(  struct cda_def *cursor, ub1 piece, void  *bufp, 
                   ub4 *lenp  );
void sqlld2  (  struct cda_def *lda, OraText *cname, sb4 *cnlen  );
void sqllda  (  struct cda_def *lda  );

/* non-blocking functions */
sword onbset(  struct cda_def *lda  ); 
sword onbtst(  struct cda_def *lda  ); 
sword onbclr(  struct cda_def *lda  ); 
sword ognfd (  struct cda_def *lda, void  *fdp  );



/* 
 * OBSOLETE FUNCTIONS 
 */

/* 
 * OBSOLETE BIND CALLS-- use obindps() 
 */
sword  obndra(  struct cda_def *cursor, OraText *sqlvar, sword sqlvl,
                 ub1 *progv, sword progvl, sword ftype, sword scale,
                 sb2 *indp, ub2 *alen, ub2 *arcode, ub4 maxsiz,
                 ub4 *cursiz, OraText *fmt, sword fmtl, sword fmtt  );
sword  obndrn(  struct cda_def *cursor, sword sqlvn, ub1 *progv,
                 sword progvl, sword ftype, sword scale, sb2 *indp,
                 OraText *fmt, sword fmtl, sword fmtt  );
sword  obndrv(  struct cda_def *cursor, OraText *sqlvar, sword sqlvl,
                 ub1 *progv, sword progvl, sword ftype, sword scale,
                 sb2 *indp, OraText *fmt, sword fmtl, sword fmtt  );

/* 
 * OBSOLETE DEFINE CALLS-- use odefinps() 
 */
sword  odefin(  struct cda_def *cursor, sword pos, ub1 *buf,
                 sword bufl, sword ftype, sword scale, sb2 *indp,
                 OraText *fmt, sword fmtl, sword fmtt, ub2 *rlen, 
                 ub2 *rcode  );


/* older calls ; preferred equivalent calls above */
sword  odsc  (  struct cda_def *cursor, sword pos, sb2 *dbsize,
                   sb2 *fsize, sb2 *rcode, sb2 *dtype, sb1 *buf,
                   sb2 *bufl, sb2 *dsize  );
sword  oname (  struct cda_def *cursor, sword pos, sb1 *tbuf,
                   sb2 *tbufl, sb1 *buf, sb2 *bufl  );
sword  olon  (  struct cda_def *lda, OraText *uid, sword uidl,
                   OraText *pswd, sword pswdl, sword audit  );
sword  orlon (  struct cda_def *lda, ub1 *hda, OraText *uid,
                   sword uidl, OraText *pswd, sword pswdl, sword audit  );
sword  osql3 (  struct cda_def *cda, OraText *sqlstm, sword sqllen  );







#endif  /* OCIKPR */







