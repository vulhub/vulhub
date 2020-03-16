
/* Copyright (c) 1997, 2005, Oracle. All rights reserved.  */
 
/* NOTE:  See 'header_template.doc' in the 'doc' dve under the 'forms' 
      directory for the header file template that includes instructions. 
*/
 
/* 
   NAME 
     oci1.h - Cartridge Service definitions

   DESCRIPTION 
     <short description of component this file declares/defines> 

   RELATED DOCUMENTS 
 
   INSPECTION STATUS 
     Inspection date: 
     Inspection status: 
     Estimated increasing cost defects per page: 
     Rule sets: 
 
   ACCEPTANCE REVIEW STATUS 
     Review date: 
     Review status: 
     Reviewers: 
 
   PUBLIC FUNCTION(S) 
     <list of external functions declared/defined - with one-line descriptions>

   PRIVATE FUNCTION(S)
     <list of static functions defined in .c file - with one-line descriptions>

   EXAMPLES

   NOTES
     <other useful comments, qualifications, etc.>

   MODIFIED   (MM/DD/YY)
   mbastawa    09/16/05 - dbhygiene
   dmukhin     06/29/05 - ANSI prototypes; miscellaneous cleanup 
   nramakri    01/16/98 - remove #ifdef NEVER clause
   ewaugh      12/18/97 - Turn type wrappers into functions.
   skabraha    12/02/97 - Adding data structures & constants for OCIFile
   rhwu        12/02/97 - OCI Thread
   nramakri    12/15/97 - move to core4
   ewaugh      12/11/97 - add OCIFormat package constants
   ssamu       12/10/97 - do not include s.h
   nramakri    11/19/97 - add OCIExtract definitions
   ssamu       11/14/97 - creation

*/


#ifndef OCI1_ORACLE
# define OCI1_ORACLE

# ifndef ORATYPES
#  include <oratypes.h>
# endif


/*---------------------------------------------------------------------------
                     PUBLIC TYPES AND CONSTANTS
  ---------------------------------------------------------------------------*/

/* Constants required by the OCIFormat package. */

#define OCIFormatUb1(variable)      OCIFormatTUb1(),      &(variable)
#define OCIFormatUb2(variable)      OCIFormatTUb2(),      &(variable)
#define OCIFormatUb4(variable)      OCIFormatTUb4(),      &(variable)
#define OCIFormatUword(variable)    OCIFormatTUword(),    &(variable)
#define OCIFormatUbig_ora(variable) OCIFormatTUbig_ora(), &(variable)
#define OCIFormatSb1(variable)      OCIFormatTSb1(),      &(variable)
#define OCIFormatSb2(variable)      OCIFormatTSb2(),      &(variable)
#define OCIFormatSb4(variable)      OCIFormatTSb4(),      &(variable)
#define OCIFormatSword(variable)    OCIFormatTSword(),    &(variable)
#define OCIFormatSbig_ora(variable) OCIFormatTSbig_ora(), &(variable)
#define OCIFormatEb1(variable)      OCIFormatTEb1(),      &(variable)
#define OCIFormatEb2(variable)      OCIFormatTEb2(),      &(variable)
#define OCIFormatEb4(variable)      OCIFormatTEb4(),      &(variable)
#define OCIFormatEword(variable)    OCIFormatTEword(),    &(variable)
#define OCIFormatChar(variable)     OCIFormatTChar(),     &(variable)
#define OCIFormatText(variable)     OCIFormatTText(),      (variable)
#define OCIFormatDouble(variable)   OCIFormatTDouble(),   &(variable)
#define OCIFormatDvoid(variable)    OCIFormatTDvoid(),     (variable)
#define OCIFormatEnd                OCIFormatTEnd()

#define OCIFormatDP 6


/*----------------- Public Constants for OCIFile -------------------------*/

/* flags for open.*/
/* flags for mode */
#define OCI_FILE_READ_ONLY         1   /* open for read only */
#define OCI_FILE_WRITE_ONLY        2   /* open for write only */
#define OCI_FILE_READ_WRITE        3   /* open for read & write */
/* flags for create */
#define OCI_FILE_EXIST             0   /* the file should exist */
#define OCI_FILE_CREATE            1   /* create if the file doesn't exist */
#define OCI_FILE_EXCL              2   /* the file should not exist */
#define OCI_FILE_TRUNCATE          4   /* create if the file doesn't exist, 
                                          else truncate file the file to 0 */
#define OCI_FILE_APPEND            8   /* open the file in append mode */

/* flags for seek */
#define OCI_FILE_SEEK_BEGINNING    1  /* seek from the beginning of the file */
#define OCI_FILE_SEEK_CURRENT      2  /* seek from the current position */
#define OCI_FILE_SEEK_END          3  /* seek from the end of the file */

#define OCI_FILE_FORWARD     1     /* seek forward              */
#define OCI_FILE_BACKWARD    2     /* seek backward             */

/* file type */
#define   OCI_FILE_BIN      0    /* binary file */
#define   OCI_FILE_TEXT     1    /* text file */
#define   OCI_FILE_STDIN    2    /* standard i/p */
#define   OCI_FILE_STDOUT   3    /* standard o/p */
#define   OCI_FILE_STDERR   4    /* standard error */

/* Represents an open file */
typedef struct OCIFileObject OCIFileObject;


/*--------------------- OCI Thread Object Definitions------------------------*/

/* OCIThread Context */
typedef struct OCIThreadContext OCIThreadContext;

/* OCIThread Mutual Exclusion Lock */
typedef struct OCIThreadMutex   OCIThreadMutex; 

/* OCIThread Key for Thread-Specific Data */
typedef struct OCIThreadKey     OCIThreadKey;  

/* OCIThread Thread ID */
typedef struct OCIThreadId      OCIThreadId;  

/* OCIThread Thread Handle */
typedef struct OCIThreadHandle  OCIThreadHandle;  


/*-------------------- OCI Thread Callback Function Pointers ----------------*/

/* OCIThread Key Destructor Function Type */
typedef void (*OCIThreadKeyDestFunc)( void  * );


/* Flags passed into OCIExtractFromXXX routines to direct processing         */
#define OCI_EXTRACT_CASE_SENSITIVE     0x1 /* matching is case sensitive     */
#define OCI_EXTRACT_UNIQUE_ABBREVS     0x2 /* unique abbreviations for keys
                                              are allowed                    */
#define OCI_EXTRACT_APPEND_VALUES      0x4 /* if multiple values for a key
                                              exist, this determines if the 
                                              new value should be appended 
                                              to (or replace) the current 
                                              list of values                 */

/* Constants passed into OCIExtractSetKey routine */
#define OCI_EXTRACT_MULTIPLE           0x8 /* key can accept multiple values */
#define OCI_EXTRACT_TYPE_BOOLEAN         1 /* key type is boolean            */
#define OCI_EXTRACT_TYPE_STRING          2 /* key type is string             */
#define OCI_EXTRACT_TYPE_INTEGER         3 /* key type is integer            */
#define OCI_EXTRACT_TYPE_OCINUM          4 /* key type is ocinum             */

/*---------------------------------------------------------------------------
                     PRIVATE TYPES AND CONSTANTS
  ---------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------
                           PUBLIC FUNCTIONS
  ---------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------
                          PRIVATE FUNCTIONS
  ---------------------------------------------------------------------------*/


#endif                                              /* OCI1_ORACLE */
