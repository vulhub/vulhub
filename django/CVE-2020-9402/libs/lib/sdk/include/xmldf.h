/* Copyright (c) 2005, 2007, Oracle. All rights reserved.  */
 
/* 
   NAME 
     xmldf.h - XmlDiff and XmlPatch

   DESCRIPTION 
     Public API for XmlDiff and XmlPatch

   RELATED DOCUMENTS 
     <note any documents related to this facility>
 
   EXPORT FUNCTION(S) 
     XmlDiff   - Diff two XML documents (like UNIX diff, but XML aware)
     XmlPatch  - Patch an XML document (like UNIX patch, but XML aware)
     XmlHash   - Compute a hash value for an XML document or a node in DOM.

   INTERNAL FUNCTION(S)
     <other external functions declared - one-line descriptions>

   EXAMPLES

   NOTES

*/

#ifndef XMLDF_ORACLE
# define XMLDF_ORACLE

#ifndef ORATYPES
# include<oratypes.h>
#endif

#ifndef XML_ORACLE
# include <xml.h>
#endif

/*---------------------------------------------------------------------------
                     PUBLIC TYPES AND CONSTANTS
  ---------------------------------------------------------------------------*/

/* source types for XmlDiff and XmlPatch */
typedef enum 
{
   XMLDF_SRCT_NONE = 0, /* default is DOM */
   XMLDF_SRCT_DOM,      /* DOM: doc node must be specified */
   XMLDF_SRCT_FILE,     /* file name must be specified */
   XMLDF_SRCT_URL,      /* URL in compiler encoding */
   XMLDF_SRCT_BUFFER,   /* buffer: buffer pointer and length must be 
                           specified */
   XMLDF_SRCT_FILEP,    /* FILE * */
   XMLDF_SRCT_OSTREAM,  /* orastream: stream pointer must be specified */
   XMLDF_SRCT_DOMNODE   /* DOM node, used with XmlHash() */
} xmldfsrct;

/*
 * Quality of diff : Given one document there are many ways to transform it
 * into a second document = many was to capture the diff. The quality of diff
 * refers to the total cost of operations in the diff.
 */

/*
 * 'oracle-xmldiff' Processing Instruction (XML PI) in Xdiff document:
 * We use 'oracle-xmldiff' PI to describe certain aspects of the diff.
 * The PI specifies values for 'operations-in-docorder' and 'output-model'.
 * The output of XmlDiff has the PI always. If the user handcodes a diff doc
 * then it must also have the PI in it as the first child of top level xdiff
 * element, to be able to call XmlPatch.
 *
 * A sample output of XmlDiff with the PI is shown below:
 * <xd:xdiff
 *  xsi:schemaLocation="http://xmlns.oracle.com/xdb/xdiff.xsd 
 *                      http://xmlns.oracle.com/xdb/xdiff.xsd" 
 *  xmlns:xsd="http://www.w3.org/2001/XMLSchema" 
 *  xmlns:xd="http://xmlns.oracle.com/xdb/xdiff.xsd"
 *  xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance">
 *    <?oracle-xmldiff operations-in-docorder="true" output-model="snapshot"
 *      diff- algorithm="global"?>
 *      <xd:update-node xd:node-type="text" xd:xpath="/root[1]/t1[1]/text()[2]">
 *         <xd:content>
 *              New Text.
 *         </xd:content>
 *      </xd:update-node>
 * </xd:xdiff>
 *
 * The terms used in the PI "oracle-xmldiff" are explained below.
 *
 * operations-in-docorder: 
 * Can be either 'true' or 'false'.
 * If true, the operations in the diff document refer to the
 * elements of the input doc in the same order as document order. Output of
 * global algorithm meets this requirement while local does not.
 * 
 * output-model: output models for representing the diff. Can be either
 * 'Snapshot' or 'Current'.
 *
 * Snapshot model:
 * Each operation uses Xpaths as if no operations
 * have been applied to the input document. (like UNIX diff)
 * This is the model used in the output of XmlDiff. XmlPatch works with this
 * (and the current model too).
 * For XmlPatch to handle this model, "operations-in-docorder" must be true and
 * the Xpaths must be simple. (see XmlDif C API documentation).
 *
 * Current model :
 * Each operation uses Xpaths as if all operations till the previous one
 * have been applied to the input document. Works with XmlPatch even if the 
 * 'operations-in-docorder' criterion is not met and the xpaths are not simple.
 *
 */

/* flags to control operation of XmlDiff and XmlPatch */

/* use defaults */
#define XMLDF_FL_DEFAULTS 0
/* snapshot model */
#define XMLDF_FL_OUTPUT_SNAPSHOT 0x1
/* current model */
#define XMLDF_FL_OUTPUT_CURRENT 0x2
/* operations in doc order is true */
#define XMLDF_FL_OUTPUT_DOCORDER 0x4

/* flags used by only XmlDiff */

/* global algorithm */
#define XMLDF_FL_ALGORITHM_GLOBAL 0x8
/* local algorithm */
#define XMLDF_FL_ALGORITHM_LOCAL 0x10
/* disable update-node operations on text nodes */
#define XMLDF_FL_DISABLE_UPDATE 0x20
/* do we maintain name and position index hash tables. This should be set
   only if a custom OB is provided. Ignored otherwise. */
#define XMLDF_FL_DISABLE_IDX_TBLS 0x40

/* 
 * Set this for performance, if set, NS prefix-URI mapping will not be
 * available in the diff DOM. If you need to call XmlPatch programmatically
 * passing the DOM returned by XmlDiff, do not set this. If you serialize the
 * DOM returned by diff and reparse, you can set this.
*/
#define XMLDF_FL_NO_NSMAPGEN (0x200)

/* operations */
typedef enum {
    XMLDF_OP_NONE, /* this should not be set to nonzero for XMLDF_NUM_OP
                      macro below */
    XMLDF_OP_UPDATE,
    XMLDF_OP_RENAME,
    XMLDF_OP_ATTR, /* for internal use only */
    XMLDF_OP_DELETE,
    XMLDF_OP_INSERT_BEFORE,
    XMLDF_OP_APPEND
} xmldfoptype;
#define XMLDF_NUM_OP (XMLDF_OP_APPEND + 1)

/*
 * An Operation - Diff is represented using one or more operations. Used for
 * passing the diff to custom OB
 */
typedef struct xmldfop 
{
   xmldfoptype op_xmldfop; /* the operation - insert, delete etc. */

   /* the node affected in first tree.  for insert-before this will be the 
      reference node.  for append-node this will be the parent.  for 
      rename-node and update-node this will be the node from first tree */
   xmlnode   *node_xmldfop; 

   /* the node from second tree for insert-node-before or append-node. for
       rename-node and update-node this will be new node */
   xmlnode   *inode_xmldfop; 

   /* positions in the trees, sequence number in document order */
   uword      i_xmldfop;   
   uword      j_xmldfop;

} xmldfop;

/* property names for properties parameter */

   /* caller has a custom Output Builder (OB) */
#define XMLDF_PROPN_CUSTOM_OB "XMLDF_CUSTOM_OB"  
   /* memory context for allocating edit script for sending to custom OB */
#define XMLDF_PROPN_CUSTOM_OBMCX "XMLDF_CUSTOM_OBMCX"  
   /* user specific context, will be returned when OB callback is called */
#define XMLDF_PROPN_CUSTOM_OBUCX "XMLDF_CUSTOM_OBUCX"  

/* property ids for above parameters */
enum {
   XMLDF_PROPI_CUSTOM_OB = 1,   
   XMLDF_PROPI_CUSTOM_OBMCX = 2,
   XMLDF_PROPI_CUSTOM_OBUCX = 3
};

#define XMLDF_DIGEST_MAX 20

struct xmlhasht {
   ub4  l_xmlhasht;   /* lenght of digest in bytes */
   ub1  d_xmlhasht[XMLDF_DIGEST_MAX];   /* the digest */
};
typedef struct xmlhasht xmlhasht;
   
/*
 * Using a custom Output Builder with XmlDiff:
 *
 * By default, XmlDiff will caputure the diff in XML conforming to Xdiff
 * schema. If necessary the caller can plug in his own Output Builder. The diff
 * will be represented as an array of 'xmldfop'. 
 * OB callback:
 *   The caller has to supply this trhough properties to XmlDiff()
 *   signature:
 *     xmlerr  (*xdfobcb)(void *uctx,  xmldfop *escript, ub4 escript_siz);
 *   OB callback will be invoked by the differ once the diff has been found. 
 *   This will happen even before the call to XmlDiff() returns. The OB
 *   callback may be called multiple times.
 *   'uctx' is the user specific context, 'escript' is the diff(array of size
 *   escript_siz).
 * Memory context 'mctx':
 *   The caller has to supply this trhough properties to XmlDiff().
 *   This will be used by differ to allocate 'escript'. 
 * User specific context 'uctx':
 *   The caller can (optionally) supply this trhough properties to XmlDiff().
 *   This will be returned by differ to OB callback.
 *
 * Sample useage:
 *...
 * #include <orastruc.h> / * for 'oraprop' * /
 *...
 * static oraprop diff_props[] = {
 *   ORAPROP(XMLDF_PROPN_CUSTOM_OB, XMLDF_PROPI_CUSTOM_OB, POINTER),
 *   ORAPROP(XMLDF_PROPN_CUSTOM_OBMCX, XMLDF_PROPI_CUSTOM_OBMCX, POINTER),
 *   ORAPROP(XMLDF_PROPN_CUSTOM_OBUCX, XMLDF_PROPI_CUSTOM_OBUCX, POINTER),
 *     { NULL }
 * }; 
 * ...
 * oramemctx    *mymemctx;
 * ...
 * xmlerr  myob(void *uctx,  xmldfop *escript, ub4 escript_siz)
 * {
 *    / * process diff which is available in escript * /
 *    
 *    / * free escript * /
 *    OraMemFree(mymemctx, escript);
 * }
 *
 * main()
 * {
 * ...
 *     myctxt  *myctx;
 *
 *     diff_props[0].value_oraprop.p_oraprop_v = myob;
 *     diff_props[1].value_oraprop.p_oraprop_v = mymemctx;
 *     diff_props[2].value_oraprop.p_oraprop_v = myctx;
 *     DISCARD XmlDiff(xctx, &err, 0, doc1, NULL, 0, doc2, NULL, 0,
 *         diff_props);
 *..
 * }
 */
/*---------------------------------------------------------------------------
                     PRIVATE TYPES AND CONSTANTS
  ---------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------
                           EXPORT FUNCTIONS
  ---------------------------------------------------------------------------*/

/*--------------------------------- XmlDiff  --------------------------------*/

 /*
    NAME
        XmlDiff

    DESCRIPTION
         Figures out the changes between two XML documents.
         These input documents can be specified either as DOM
         Trees, files, URI, orastream etc.  DOM trees for both the 
         inputs will be created, if they are not supplied as DOM trees.
         The DOM for the diff document is created, and the doc node is
         returned. 

         If the caller supplies inputs as DOMs we will not free the memory
         for the DOMs. 

         Data(DOM) encoding of both the documents must be the same as the
         data encoding in xmlctx. The diff DOM will be created in the data 
         encoding specified in xmlctx.

         There are four algorithms that can be run in Xmldiff - Global, Local,
         Global with hashing and Local with hashing. The diff may be
         different in the four cases.

         Global will generate minimal diff using insert, append, delete and
         update. It needs more memory and time than local. Local may not
         generate minimal diff but is faster and uses less space.

         Hashing can be used with both global and local algorithms. In general,
         if hashing is used with global, it will speed up diff computation
         significantly but may reduce the quality of diff. With local, it 
         improves the quality of the diff.

         The caller must specify a depth at which to use hashing. When we use 
         hashing we associate a hash value for every element node which is a
         digest for the entire subtree rooted at that node. We do not drill
         down into the tree beyond hash level depth while computing the diff.

         The output of global algorithm with or without hashing meets 
         'operations-in-docorder' requirement whereas that of local algorithm 
         does not. See above for details about operations-in-docorder.

    REQUIRES
       
    PARAMETERS
         xctx             (IN)  XML context 
         xmlerr           (OUT) numeric error code, XMLERR_OK [0] on success
         flags            (IN)  XMLDF_FL_DEFAULTS(=0) chooses defaults. 
                                XMLDF_FL_ALGORITHM_GLOBAL - Global algorithm
                                XMLDF_FL_ALGORITHM_LOCAL - Local algorithm
                                XMLDF_FL_DISABLE_UPDATE - diable update 
                                   operation. (With global)
                                By default global algorithm is used. 
         firstSourceType  (IN)  type of source for first document. If zero, 
                                firstSource is assumed to be a DOM doc node.
         firstSource      (IN)  pointer to source for first document.
         firstSourceExtra (IN)  additional pointer to source for first 
                                document.  Used for buffer length pointer.
         secondSourceType (IN)  type of source for second doc. If zero,
                                secondSource is assumed to be a DOM doc node.
         secondSource     (IN)  pointer to source for second document.
         secondSourceExtra(IN)  additional pointer to source for second 
                                document. 
         hashLevel        (IN)  depth (counting from 1 for root) at which to 
                                use hashing for sub trees. <=1 means do not use
                                hashing.  
         properties       (IN)  Used for plugging in custom Output Builder.
       
    RETURNS 

         (xmldocnode)           Doc node for the diff document or NULL on 
                                error. Always returns NULL in case of custom
                                OB. 

    EXCEPTIONS 
       
    EXAMPLES
        
    NOTES  
      The namespace prefixes XmlDiff will use in the xdiff document may be same
      as those in either the first or second doc depending on which prefix was
      seen first while processing. But, the NS URI will be bound to the prefix
      in the output appropriately. If this NS does not have a prefix in both
      docs, a new prefix will be generated and bound to the NS in xdiff doc.
     
 */
xmldocnode *XmlDiff(xmlctx *xctx, xmlerr *err, ub4  flags, xmldfsrct 
                    firstSourceType, void *firstSource, void *firstSourceExtra,
                    xmldfsrct secondSourceType, void *secondSource, void 
                    *secondSourceExtra, uword hashLevel, oraprop *properties);

/*--------------------------------- XmlPatch --------------------------------*/

 /*
    NAME
       XmlPatch

    DESCRIPTION
       Applies changes on input XML document. The changes should conform to
       Xdiff schema. The input document and the diff document can be specified 
       either as DOM tree, file, URI, or buffer. 
       
       We build DOMs for both the input and diff document if they are not 
       supplied as DOMs. 

       Data(DOM) encoding of both input and diff documents must be the same as 
       the data encoding in xmlctx. The patched DOM will be in the data 
       encoding specified in xmlctx.

       Snapshot model:
       We support only simple XPaths in snapshot model. (The xpath should 
       identify a node with a posistion predicate in abbreviated syntax:
       e.g. /a[1]/b[2]). The xpaths generated by XmlDiff meet this requirement.
       Also, operations-in-docorder must be "true". Global (with or without
       hashing) meets this requirement. Local does not.

       The oracle-xmldiff PI should specify the output model used in the diff
       doc. 'oracle-xmldiff' PI should be the first child of the top level 
       xdiff element.  It should also specify if operations-in- docorder is 
       true or false and if the output model is snapshot or current.


    REQUIRES
       
    PARAMETERS
      xctx                (IN)  XML context 
      xmlerr              (OUT) numeric error code, XMLERR_OK [0] on success
      flags               (IN)  XMLDF_FL_DEFAULT(=0) chooses defaults. 
      inputSourceType     (IN)  type of source for input document. If zero, 
                                inputSource is assumed to be a DOM doc node.
      inputSource         (IN)  pointer to source for input document.
      inputSourceExtra    (IN)  additional pointer to source for input 
                                document.  Used for buffer length pointer.
      diffSourceType      (IN)  type of source for the diff document. If zero,
                                diffSource is assumed to be a DOM doc node.
      diffSource          (IN)  pointer to source for diff document.
      diffSourceExtra     (IN)  additional pointer to source for diff document.
                                Used for buffer length pointer.
      properties          (IN)  For future use. Ignored at present.
      
    RETURNS 
      (xmldocnode *)            Pointer to the patched DOM or NULL on error. 


    EXCEPTIONS 
       
    EXAMPLES
        
    NOTES  
 */
xmldocnode *XmlPatch(xmlctx *xctx, xmlerr *err, ub4  flags, xmldfsrct 
                     inputSourceType, void *inputSource, void 
                     *inputSourceExtra, xmldfsrct diffSourceType, void 
                     *diffSource, void *diffSourceExtra, oraprop *properties);

/*--------------------------------- XmlHash  --------------------------------*/

 /*
    NAME
       XmlHash

    DESCRIPTION
       Computes a hash value for an XML document or a node in DOM.

       If the hash values for 2 XML (sub) trees are equal, the corresponding
       (sub) trees are equal to a very high probability. Uses MD5 for computing
       the hash and there is a very small probability that two different inputs
       might map to same MD5 digest.

       The source can be specified as a file, URL, etc. It can also be a doc 
       node in DOM or any DOM node. If the source is a doc or any DOM node, it
       must be specified using inputSource parameter. If inputSource is a DOM
       node (other than doc node), inputSourceExtra must point to the document
       node for the DOM.

    REQUIRES
       
    PARAMETERS
      xctx                (IN)  XML context 
      digest              (OUT) The hash value for the XML (sub) tree.
      flags               (IN)  For future use. Ignored at present.
      inputSourceType     (IN)  type of source for input document. If zero, 
                                inputSource is assumed to be a DOM doc node.
      inputSource         (IN)  pointer to source for input document.
      inputSourceExtra    (IN)  additional pointer to source for input 
                                document.  Used for a node pointer in DOM. If
                                so, inputSource must be a document node.
      properties          (IN)  For future use. Ignored at present.
      
    RETURNS 
      (xmlerr)                  numeric error code, XMLERR_OK [0] on success

    EXCEPTIONS 
       
    EXAMPLES
        
    NOTES  
 */
xmlerr XmlHash(xmlctx *xctx, xmlhasht *digest, ub4 flags, xmldfsrct 
         inputSourceType, void *inputSource, void *inputSourceExtra, oraprop 
         *properties);

/*---------------------------------------------------------------------------
                          INTERNAL FUNCTIONS
  ---------------------------------------------------------------------------*/


#endif                                              /* XMLDF_ORACLE */
