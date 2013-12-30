/////////////////////////////////////////////////////////////////////////////////////
//
//
//  Mini-Fractals/glMandel Copyright © July 2013 Frederick G Zacharias Covered under the CPOL 
//
//  See License-CPOL.txt for the Full License.
//
//

#ifndef GDEFS_H
#define GDEFS_H

//#include <stddef.h>
#include <limits.h>
#include <float.h>
#include <assert.h>
// #include "WhichBuild.h"


// typedef unsigned char uchar;

// typedef char* String;
// typedef uchar Byte;

typedef int Int;
// typedef unsigned Index;

// typedef float SReal;
// typedef double Real;

//#ifndef __GNUC__
//typedef enum { false=0, true=1 } bool;
//#endif

/* ISO C */
#ifndef DBL_EPSILON
#define DBL_EPSILON 2.2204460492503131e-16
#endif

#define SWAP(a,b,t) ((t)=(a),(a)=(b),(b)=(t))
#define SIGN(a)  ((a)<0 ? -1 : 1)
#define ABS(a)   ((a)<0 ? -(a) : (a))
#define SQR(a)   ((a)*(a))
#define MIN(a,b) ((a)<(b) ? (a) : (b))
#define MAX(a,b) ((a)<(b) ? (b) : (a))
#define MIN3(a,b,c) ((a)<(b) ? ((a)<(c) ? (a) : (c)) : ((b)<(c) ? (b) : (c)))
#define MAX3(a,b,c) ((a)>(b) ? ((a)>(c) ? (a) : (c)) : ((b)>(c) ? (b) : (c)))

/*
 * -------------
 * Handle Error
 * -------------
 */

//#ifdef __cplusplus
//#define ERROR(mod,fct,msg) \
//  ( cerr << (mod) << " error:" __FILE__ ":" << __LINE__ \
//    << ':' << (fct) << ':' << (msg) << '\n', exit(1) )

//#define WARNING(mod,fct,msg) \
//  ( cerr << (mod) << " warning:" __FILE__ ":" << __LINE__ \
//    << ':' << (fct) << ':' << (msg) << '\n' )
//#else
//#define ERROR(mod,fct,msg) \
//  ( fprintf(sdterr,"%s error:%s:%d:%s:%s\n", \
//			mod,__FILE__,__LINE__,fct,msg), exit(1) )

//#define WARNING(mod,fct,msg) \
//  ( fprintf(sdterr,"%s warning:%s:%d:%s:%s\n", \
//			mod,__FILE__,__LINE__,fct,msg) )
//#endif

/*
 * -------------
 * Handle String
 * -------------
 */

#define _MkString_(a) #a
#define _String_(a) _MkString_(a)

/*
 * -----------------
 * Handle Built type
 * -----------------
 */

#define _MkConcat_(a,b) a ## b
#define _Concat_(a,b) _MkConcat_(a,b)

#endif /* GDEFS_H */

