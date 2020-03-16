#ifdef RCSID
static char *RCSid =
   "$Header: rdbms/demo/cdemo81.c /main/11 2017/09/12 21:21:47 aliu Exp $ ";
#endif /* RCSID */

/* Copyright (c) 1996, 2017, Oracle and/or its affiliates. 
All rights reserved.*/

/*

   NAME
     cdemo81.c - Basic OCI V8 functionality

   DESCRIPTION

 *  An example program which adds new employee
 *  records to the personnel data base.  Checking
 *  is done to insure the integrity of the data base.
 *  The employee numbers are automatically selected using
 *  the current maximum employee number as the start.
 *
 *  The program queries the user for data as follows:
 *
 *  Enter employee name:
 *  Enter employee job:
 *  Enter employee salary:
 *  Enter employee dept:
 *
 *  The program terminates if return key (CR) is entered
 *  when the employee name is requested.
 *
 *  If the record is successfully inserted, the following
 *  is printed:
 *
 *  "ename" added to department "dname" as employee # "empno"

   Demonstrates creating a connection, a session and executing some SQL.
   Also shows the usage of allocating memory for application use which has the
   life time of the handle.

   MODIFIED   (MM/DD/YY)
   aliu        09/11/17 - fix bug 21574212
   gak         02/16/15 - Bug 20364067
   aliu        02/04/14 - fix bug 18181823
   aliu        09/03/13 - XbranchMerge aliu_bug-16007835 from st_rdbms_12.1.0.1
   aliu        01/04/13 - fix bug 16007835 - add an variable for connect string 
                          in OCIServerAttach
   lburgess    04/14/06 - lowercase passwords 
   aliu        04/21/06 - use OCIEnvCreate and exit if it fails 
   mjaeger     07/14/99 - bug 808870: OCCS: convert tabs, no long lines
   dchatter    10/14/98 - add the usage of xtrmemsz and usrmempp
   azhao       06/23/97 - Use OCIBindByPos, OCIBindByName; clean up
   echen       12/17/96 - OCI beautification
   dchatter    07/18/96 - delete spurious header files
   dchatter    07/15/96 - hda is a ub4 array to prevent bus error
   mgianata    06/17/96 - change ociisc() to OCISessionBegin()
   aroy        04/26/96 - change OCITransCommitt -> OCITransCommit
   slari       04/24/96 - use OCITransCommitt
   aroy        02/21/96 - fix bug in get descriptor handle call
   lchidamb    02/20/96 - cdemo81.c converted for v8 OCI
   lchidamb    02/20/96 - Creation

*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <oci.h>

static text *username = (text *) "SCOTT";
static text *password = (text *) "tiger";
  /* this dbname can be changed to point to a remote DB for demo to run with */
static text *dbname   = (text *) "";

/* Define SQL statements to be used in program. */
static text *insert = (text *)"INSERT INTO emp(empno, ename, job, sal, deptno)\
  VALUES (:empno, :ename, :job, :sal, :deptno)";
static text *seldept = (text *)"SELECT dname FROM dept WHERE deptno = :1";
static text *maxemp = (text *)"SELECT NVL(MAX(empno), 0) FROM emp";
static text *selemp = (text *)"SELECT ename, job FROM emp";

static OCIEnv *envhp = NULL;
static OCIError *errhp = NULL;

static void checkerr(/*_ OCIError *errhp, sword status _*/);
static void myfflush(/*_ void _*/);
static void free_handles(/*svchp, srvhp, authp, stmthp, stmthp1, stmthp2*/);
static void logout_detach_server(/*svchp, srvhp, authp*/);
static sword finish_demo(/*svchp, srvhp, authp, stmthp, stmthp1, stmthp2 */);
int main(/*_ int argc, char *argv[] _*/);

static sword status;

static boolean logged_on = FALSE;

int main(argc, argv)
int argc;
char *argv[];
{

  sword    empno, sal, deptno;
  sword    len, len2, rv, dsize, dsize2;
  sb4      enamelen = 10;
  sb4      joblen = 9;
  sb4      deptlen = 14;
  sb2      sal_ind, job_ind;
  sb2      db_type, db2_type;
  sb1      name_buf[20], name2_buf[20];
  text     *cp, *ename, *job, *dept;

  sb2      ind[2];                                              /* indicator */
  ub2      alen[2];                                         /* actual length */
  ub2      rlen[2];                                         /* return length */

  OCIDescribe  *dschndl1 = (OCIDescribe *) 0,
               *dschndl2 = (OCIDescribe *) 0,
               *dschndl3 = (OCIDescribe *) 0;

  OCISession *authp = (OCISession *) 0;
  OCIServer *srvhp = NULL;
  OCISvcCtx *svchp = NULL;
  OCIStmt   *inserthp = NULL,
            *stmthp = NULL,
            *stmthp1= NULL;
  OCIDefine *defnp = (OCIDefine *) 0;

  OCIBind  *bnd1p = (OCIBind *) 0;             /* the first bind handle */
  OCIBind  *bnd2p = (OCIBind *) 0;             /* the second bind handle */
  OCIBind  *bnd3p = (OCIBind *) 0;             /* the third bind handle */
  OCIBind  *bnd4p = (OCIBind *) 0;             /* the fourth bind handle */
  OCIBind  *bnd5p = (OCIBind *) 0;             /* the fifth bind handle */
  OCIBind  *bnd6p = (OCIBind *) 0;             /* the sixth bind handle */

  sword errcode = 0;

  errcode = OCIEnvCreate((OCIEnv **) &envhp, (ub4) OCI_DEFAULT,
                  (dvoid *) 0, (dvoid * (*)(dvoid *,size_t)) 0,
                  (dvoid * (*)(dvoid *, dvoid *, size_t)) 0,
                  (void (*)(dvoid *, dvoid *)) 0, (size_t) 0, (dvoid **) 0);

  if (errcode != 0) {
    (void) printf("OCIEnvCreate failed with errcode = %d.\n", errcode);
    exit(1);
  }

  (void) OCIHandleAlloc( (dvoid *) envhp, (dvoid **) &errhp, OCI_HTYPE_ERROR,
                   (size_t) 0, (dvoid **) 0);

  /* server contexts */
  (void) OCIHandleAlloc( (dvoid *) envhp, (dvoid **) &srvhp, OCI_HTYPE_SERVER,
                   (size_t) 0, (dvoid **) 0);

  (void) OCIHandleAlloc( (dvoid *) envhp, (dvoid **) &svchp, OCI_HTYPE_SVCCTX,
                   (size_t) 0, (dvoid **) 0);

  checkerr(errhp, OCIServerAttach ( srvhp, errhp, (text *)dbname,
                                    (sb4) strlen((char *)dbname), 0));

  /* set attribute server context in the service context */
  (void) OCIAttrSet( (dvoid *) svchp, OCI_HTYPE_SVCCTX, (dvoid *)srvhp,
                     (ub4) 0, OCI_ATTR_SERVER, (OCIError *) errhp);

  (void) OCIHandleAlloc((dvoid *) envhp, (dvoid **)&authp,
                        (ub4) OCI_HTYPE_SESSION, (size_t) 0, (dvoid **) 0);

  (void) OCIAttrSet((dvoid *) authp, (ub4) OCI_HTYPE_SESSION,
                 (dvoid *) username, (ub4) strlen((char *)username),
                 (ub4) OCI_ATTR_USERNAME, errhp);

  (void) OCIAttrSet((dvoid *) authp, (ub4) OCI_HTYPE_SESSION,
                 (dvoid *) password, (ub4) strlen((char *)password),
                 (ub4) OCI_ATTR_PASSWORD, errhp);

  checkerr(errhp, OCISessionBegin ( svchp,  errhp, authp, OCI_CRED_RDBMS,
                          (ub4) OCI_DEFAULT));

  (void) OCIAttrSet((dvoid *) svchp, (ub4) OCI_HTYPE_SVCCTX,
                   (dvoid *) authp, (ub4) 0,
                   (ub4) OCI_ATTR_SESSION, errhp);

  logged_on = TRUE;
  
  checkerr(errhp, OCIHandleAlloc( (dvoid *) envhp, (dvoid **) &stmthp,
           OCI_HTYPE_STMT, (size_t) 0, (dvoid **) 0));

  checkerr(errhp, OCIHandleAlloc( (dvoid *) envhp, (dvoid **) &stmthp1,
           OCI_HTYPE_STMT, (size_t) 0, (dvoid **) 0));

  /* Retrieve the current maximum employee number. */
  checkerr(errhp, OCIStmtPrepare(stmthp, errhp, maxemp,
                                (ub4) strlen((char *) maxemp),
                                (ub4) OCI_NTV_SYNTAX, (ub4) OCI_DEFAULT));

  /* bind the input variable */
  checkerr(errhp, OCIDefineByPos(stmthp, &defnp, errhp, 1, (dvoid *) &empno,
                   (sword) sizeof(sword), SQLT_INT, (dvoid *) 0, (ub2 *)0,
                   (ub2 *)0, OCI_DEFAULT));

  /* execute and fetch */
  if (status = OCIStmtExecute(svchp, stmthp, errhp, (ub4) 1, (ub4) 0,
               (CONST OCISnapshot *) NULL, (OCISnapshot *) NULL, OCI_DEFAULT))
  {
    if (status == OCI_NO_DATA)
      empno = 10;
    else
    {
      checkerr(errhp, status);
      finish_demo(svchp, srvhp, authp,stmthp, stmthp1,inserthp);
      return OCI_ERROR;
    }
  }


  /*
   * When we bind the insert statement we also need to allocate the storage
   * of the employee name and the job description.
   * Since the lifetime of these buffers are the same as the statement, we
   * will allocate it at the time when the statement handle is allocated; this
   * will get freed when the statement disappears and there is less
   * fragmentation.
   *
   * sizes required are enamelen+2 and joblen+2 to allow for \n and \0
   *
   */


  checkerr(errhp, OCIHandleAlloc( (dvoid *) envhp, (dvoid **) &inserthp,
           OCI_HTYPE_STMT, (size_t) enamelen + 2 + joblen + 2,
           (dvoid **) &ename));
  job   = (text *) (ename+enamelen+2);


  checkerr(errhp, OCIStmtPrepare(stmthp, errhp, insert,
                                (ub4) strlen((char *) insert),
                                (ub4) OCI_NTV_SYNTAX, (ub4) OCI_DEFAULT));

  checkerr(errhp, OCIStmtPrepare(stmthp1, errhp, seldept,
                                (ub4) strlen((char *) seldept),
                                (ub4) OCI_NTV_SYNTAX, (ub4) OCI_DEFAULT));


  /*  Bind the placeholders in the INSERT statement. */
  if ((status = OCIBindByName(stmthp, &bnd1p, errhp, (text *) ":ENAME",
             -1, (dvoid *) ename,
             enamelen+1, SQLT_STR, (dvoid *) 0,
             (ub2 *) 0, (ub2 *) 0, (ub4) 0, (ub4 *) 0, OCI_DEFAULT)) ||
      (status = OCIBindByName(stmthp, &bnd2p, errhp, (text *) ":JOB",
             -1, (dvoid *) job,
             joblen+1, SQLT_STR, (dvoid *) &job_ind,
             (ub2 *) 0, (ub2 *) 0, (ub4) 0, (ub4 *) 0, OCI_DEFAULT)) ||
      (status = OCIBindByName(stmthp, &bnd3p, errhp, (text *) ":SAL",
             -1, (dvoid *) &sal,
             (sword) sizeof(sal), SQLT_INT, (dvoid *) &sal_ind,
             (ub2 *) 0, (ub2 *) 0, (ub4) 0, (ub4 *) 0, OCI_DEFAULT)) ||
      (status = OCIBindByName(stmthp, &bnd4p, errhp, (text *) ":DEPTNO",
             -1, (dvoid *) &deptno,
             (sword) sizeof(deptno), SQLT_INT, (dvoid *) 0,
             (ub2 *) 0, (ub2 *) 0, (ub4) 0, (ub4 *) 0, OCI_DEFAULT)) ||
      (status = OCIBindByName(stmthp, &bnd5p, errhp, (text *) ":EMPNO",
             -1, (dvoid *) &empno,
             (sword) sizeof(empno), SQLT_INT, (dvoid *) 0,
             (ub2 *) 0, (ub2 *) 0, (ub4) 0, (ub4 *) 0, OCI_DEFAULT)))
  {
    checkerr(errhp, status);
    finish_demo(svchp, srvhp, authp,stmthp, stmthp1,inserthp);
    return OCI_ERROR;
  }

  /*  Bind the placeholder in the "seldept" statement. */
  if (status = OCIBindByPos(stmthp1, &bnd6p, errhp, 1,
           (dvoid *) &deptno, (sword) sizeof(deptno),SQLT_INT,
           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0, (ub4) 0, (ub4 *) 0, OCI_DEFAULT))
  {
    checkerr(errhp, status);
    finish_demo(svchp, srvhp, authp,stmthp, stmthp1,inserthp);
    return OCI_ERROR;
  }

  /*  Allocate the dept buffer now that you have length. */
  /* the deptlen should eventually get from dschndl3.    */
  deptlen = 14;
  dept = (text *) malloc((size_t) deptlen + 1);

  /*  Define the output variable for the select-list. */
  if (status = OCIDefineByPos(stmthp1, &defnp, errhp, 1,
                             (dvoid *) dept, deptlen+1, SQLT_STR,
                             (dvoid *) 0, (ub2 *) 0, (ub2 *) 0, OCI_DEFAULT))
  {
    checkerr(errhp, status);
    finish_demo(svchp, srvhp, authp,stmthp, stmthp1,inserthp);
    return OCI_ERROR;
  }

  for (;;)
  {
    /* Prompt for employee name.  Break on no name. */
    printf("\nEnter employee name (or CR to EXIT): ");
    fgets((char *) ename, (int) enamelen+1, stdin);
    cp = (text *) strchr((char *) ename, '\n');
    if (cp == ename)
    {
      printf("Exiting... ");
      finish_demo(svchp, srvhp, authp,stmthp, stmthp1,inserthp);
      return OCI_SUCCESS;
    }
    if (cp)
      *cp = '\0';
    else
    {
      printf("Employee name may be truncated.\n");
      myfflush();
    }
    /*  Prompt for the employee's job and salary. */
    printf("Enter employee job: ");
    job_ind = 0;
    fgets((char *) job, (int) joblen + 1, stdin);
    cp = (text *) strchr((char *) job, '\n');
    if (cp == job)
    {
      job_ind = -1;            /* make it NULL in table */
      printf("Job is NULL.\n");/* using indicator variable */
    }
    else if (cp == 0)
    {
      printf("Job description may be truncated.\n");
      myfflush();
    }
    else
      *cp = '\0';

    printf("Enter employee salary: ");
    scanf("%d", &sal);
    myfflush();
    sal_ind = (sal <= 0) ? -2 : 0;  /* set indicator variable */

    /*
     *  Prompt for the employee's department number, and verify
     *  that the entered department number is valid
     *  by executing and fetching.
     */
    do
    {
      printf("Enter employee dept: ");
      scanf("%d", &deptno);
      myfflush();
      if ((status = OCIStmtExecute(svchp, stmthp1, errhp, (ub4) 1, (ub4) 0,
               (CONST OCISnapshot *) NULL, (OCISnapshot *) NULL, OCI_DEFAULT))
          && (status != OCI_NO_DATA))
      {
        checkerr(errhp, status);
        finish_demo(svchp, srvhp, authp,stmthp, stmthp1,inserthp);
        return OCI_ERROR;
      }
      if (status == OCI_NO_DATA)
        printf("The dept you entered doesn't exist.\n");
      } while (status == OCI_NO_DATA);

      /*
       *  Increment empno by 10, and execute the INSERT
       *  statement. If the return code is 1 (duplicate
       *  value in index), then generate the next
       *  employee number.
       */
      empno += 10;
      if ((status = OCIStmtExecute(svchp, stmthp, errhp, (ub4) 1, (ub4) 0,
               (CONST OCISnapshot *) NULL, (OCISnapshot *) NULL, OCI_DEFAULT))
               && status != 1)
      {
        checkerr(errhp, status);
        finish_demo(svchp, srvhp, authp,stmthp, stmthp1,inserthp);
        return OCI_ERROR;
      }
      while (status == 1)
      {
        empno += 10;
        if ((status = OCIStmtExecute(svchp, stmthp, errhp, (ub4) 1, (ub4) 0,
               (CONST OCISnapshot *) NULL, (OCISnapshot *) NULL, OCI_DEFAULT))
                && status != 1)
        {
          checkerr(errhp, status);
          finish_demo(svchp, srvhp, authp,stmthp, stmthp1,inserthp);
          return OCI_ERROR;
        }
      }  /* end for (;;) */

      /* Commit the change. */
      if (status = OCITransCommit(svchp, errhp, 0))
      {
        checkerr(errhp, status);
        finish_demo(svchp, srvhp, authp,stmthp, stmthp1,inserthp);
        return OCI_ERROR;
      }
      printf("\n\n%s added to the %s department as employee number %d\n",
                 ename, dept, empno);
  }

}


void checkerr(errhp, status)
OCIError *errhp;
sword status;
{
  text errbuf[512];
  sb4 errcode = 0;

  switch (status)
  {
  case OCI_SUCCESS:
    break;
  case OCI_SUCCESS_WITH_INFO:
    (void) printf("Error - OCI_SUCCESS_WITH_INFO\n");
    break;
  case OCI_NEED_DATA:
    (void) printf("Error - OCI_NEED_DATA\n");
    break;
  case OCI_NO_DATA:
    (void) printf("Error - OCI_NODATA\n");
    break;
  case OCI_ERROR:
    (void) OCIErrorGet((dvoid *)errhp, (ub4) 1, (text *) NULL, &errcode,
                        errbuf, (ub4) sizeof(errbuf), OCI_HTYPE_ERROR);
    (void) printf("Error - %.*s\n", 512, errbuf);
    break;
  case OCI_INVALID_HANDLE:
    (void) printf("Error - OCI_INVALID_HANDLE\n");
    break;
  case OCI_STILL_EXECUTING:
    (void) printf("Error - OCI_STILL_EXECUTE\n");
    break;
  case OCI_CONTINUE:
    (void) printf("Error - OCI_CONTINUE\n");
    break;
  default:
    break;
  }
}

void myfflush()
{
  eb1 buf[50];

  fgets((char *) buf, 50, stdin);
}

/* ----------------------------------------------------------------- */
/*  Free the specified handles                                       */
/* ----------------------------------------------------------------- */
void free_handles(svchp, srvhp, authp, stmthp, stmthp1, stmthp2)
OCISvcCtx *svchp; 
OCIServer *srvhp; 
OCISession *authp; 
OCIStmt *stmthp;
OCIStmt *stmthp1;
OCIStmt *stmthp2;
{
  if (srvhp)
    (void) OCIHandleFree((dvoid *) srvhp, (ub4) OCI_HTYPE_SERVER);
  if (svchp)
    (void) OCIHandleFree((dvoid *) svchp, (ub4) OCI_HTYPE_SVCCTX);
  if (errhp)
    (void) OCIHandleFree((dvoid *) errhp, (ub4) OCI_HTYPE_ERROR);
  if (authp)
    (void) OCIHandleFree((dvoid *) authp, (ub4) OCI_HTYPE_SESSION);
  if (stmthp)
    (void) OCIHandleFree((dvoid *) stmthp, (ub4) OCI_HTYPE_STMT);
  if (stmthp1)
    (void) OCIHandleFree((dvoid *) stmthp1, (ub4) OCI_HTYPE_STMT);
  if (stmthp2)
    (void) OCIHandleFree((dvoid *) stmthp2, (ub4) OCI_HTYPE_STMT);
    
  if (envhp)
    (void) OCIHandleFree((dvoid *) envhp, (ub4) OCI_HTYPE_ENV);

  return;
}

/*-------------------------------------------------------------------*/ 
/* Logout and detach from the server                                 */
/*-------------------------------------------------------------------*/ 
void logout_detach_server(svchp, srvhp, authp)
OCISvcCtx *svchp; 
OCIServer *srvhp; 
OCISession *authp; 
{
  if ((status = OCISessionEnd(svchp, errhp, authp, (ub4) 0)))
  {
    printf("FAILED: OCISessionEnd()\n");
    checkerr(errhp, status);
  }

  if ((status = OCIServerDetach(srvhp, errhp, (ub4) OCI_DEFAULT)))
  {
    printf("FAILED: OCIServerDetach()\n");
    checkerr(errhp, status);
  }

  return;
}

/*---------------------------------------------------------------------*/
/* Finish demo and clean up                                            */
/*---------------------------------------------------------------------*/
sword finish_demo(svchp, srvhp, authp, stmthp, stmthp1, stmthp2)
OCISvcCtx *svchp; 
OCIServer *srvhp; 
OCISession *authp; 
OCIStmt *stmthp;
OCIStmt *stmthp1;
OCIStmt *stmthp2;
{

  if (logged_on)
    logout_detach_server(svchp, srvhp, authp);

  free_handles(svchp, srvhp, authp, stmthp, stmthp1, stmthp2);

  OCITerminate(OCI_DEFAULT);

  return OCI_SUCCESS;
}

/* end of file cdemo81.c */ 

