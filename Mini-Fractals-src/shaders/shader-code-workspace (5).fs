

/*

The complete list of preprocessor directives is as follows.
#
#define
#undef
#if
#ifdef
#ifndef
#else
#elif
#endif
#error
#pragma
#extension
#version
#line
The following operators are also available
defined
##

*/
/*

3.6 Keywords
The following are the keywords in the language, and cannot be used for any other purpose than that
defined by this document:

attribute const uniform varying
coherent volatile restrict readonly writeonly
atomic_uint
layout
centroid flat smooth noperspective
patch sample
break continue do for while switch case default
if else
subroutine
in out inout
float double int void bool true false
invariant
discard return
mat2 mat3 mat4 dmat2 dmat3 dmat4
mat2x2 mat2x3 mat2x4 dmat2x2 dmat2x3 dmat2x4
mat3x2 mat3x3 mat3x4 dmat3x2 dmat3x3 dmat3x4
mat4x2 mat4x3 mat4x4 dmat4x2 dmat4x3 dmat4x4
vec2 vec3 vec4 ivec2 ivec3 ivec4 bvec2 bvec3 bvec4 dvec2 dvec3 dvec4
uint uvec2 uvec3 uvec4

lowp mediump highp precision
sampler1D sampler2D sampler3D samplerCube
sampler1DShadow sampler2DShadow samplerCubeShadow
sampler1DArray sampler2DArray
sampler1DArrayShadow sampler2DArrayShadow
isampler1D isampler2D isampler3D isamplerCube
isampler1DArray isampler2DArray
usampler1D usampler2D usampler3D usamplerCube
usampler1DArray usampler2DArray
sampler2DRect sampler2DRectShadow isampler2DRect usampler2DRect
samplerBuffer isamplerBuffer usamplerBuffer
sampler2DMS isampler2DMS usampler2DMS
sampler2DMSArray isampler2DMSArray usampler2DMSArray
samplerCubeArray samplerCubeArrayShadow isamplerCubeArray usamplerCubeArray
image1D iimage1D uimage1D
image2D iimage2D uimage2D
image3D iimage3D uimage3D
image2DRect iimage2DRect uimage2DRect
imageCube iimageCube uimageCube
imageBuffer iimageBuffer uimageBuffer
image1DArray iimage1DArray uimage1DArray
image2DArray iimage2DArray uimage2DArray
imageCubeArray iimageCubeArray uimageCubeArray
image2DMS iimage2DMS uimage2DMS
image2DMSArray iimage2DMSArray uimage2DMSArray
struct

FGZ UNKown:  operator  const  friend 


The following are the keywords reserved for future use. Using them will result in an error:
common partition active
asm
class union enum typedef template this packed
resource
goto
inline noinline public (FGZ: private + protected ?) static extern external interface
long short half fixed unsigned superp
input output
hvec2 hvec3 hvec4 fvec2 fvec3 fvec4
sampler3DRect
filter
sizeof cast
namespace using
row_major


NOTE :    !!!
In addition, all identifiers containing two consecutive underscores (__) are reserved as possible future
keywords.



Operators and Preference
Operator and preference are very similar to
C and C++, but with some restrictions:
•
There is no pointer operator
or de-reference operator.
•
There is no
sizeof
operator.
•
Bits-wise operations are illegal: no shift left/r
ight (<< or >>), no exclusive/inclusive
or (^, | ).
•
Modulus operation is illegal (%).
•
Unary NOT (~) is illegal. 



*/




/*
  C++ Complex Library v1.0
  Laurent Deniau, last modified 03/97
  e-mail: Laurent.Deniau@cern.ch
*/

// -------------------
// define default code
// -------------------
//  #define _DefaultCode_

// OpenGL GLSL will handle this for us ...
// #include "math.h"

// #define  operator  
#define const  
#define friend  


/*  FGZ: the code origin: 
  C++ Complex Library v1.0
  Laurent Deniau, last modified 03/97
  e-mail: Laurent.Deniau@cern.ch
*/

// -------------------
// define default defs
// -------------------
#ifndef _Type0_
#define _Type0_ Complex
#define _Type1_ float
// #define _DefaultDefs_
#define COMPLEX_H
#endif

// why can't I change def to double ??

#define _inline   

// #include "gdefs.h"
// as follows :


// #define  HALFPI  1.570796326794896619
#define HALFPI	1.5707963 

#ifndef DBL_EPSILON
#define   DBL_EPSILON 2.2204460e-8
// #define DBL_EPSILON 2.2204460492503131e-16
#endif

#define SWAP(a,b,t) ((t)=(a),(a)=(b),(b)=(t))
#define SIGN(a)  ((a)<0 ? -1 : 1)
#define ABS(a)   ((a)<0 ? -(a) : (a))
#define SQR(a)   ((a)*(a))
#define MIN(a,b) ((a)<(b) ? (a) : (b))
#define MAX(a,b) ((a)<(b) ? (b) : (a))
#define MIN3(a,b,c) ((a)<(b) ? ((a)<(c) ? (a) : (c)) : ((b)<(c) ? (b) : (c)))
#define MAX3(a,b,c) ((a)>(b) ? ((a)>(c) ? (a) : (c)) : ((b)>(c) ? (b) : (c)))



struct _Type0_ {


  // the actual number : 
  _Type1_ r, i;

  // _Type0_ (_Type1_ x=0, _Type1_ y=0) { r = x;  i = y; }
	// above constructor doesn't work ... why ?
	
  
} ;




 _Type0_   I;  
 _Type0_   J;
 _Type0_  _I;
 _Type0_  _J;

#ifndef fabs
#define fabs(x)  ((x<0.0) ? -x : x ) 
#endif 
 

_Type1_ ATAN2( _Type1_ y, _Type1_ x )
{
	// if ( y == 0    x == 0 ) { return 0 ; }
	//SetLastError( 0 ) ;
	//double resval = atan2(y, x) ;
	// atan2 returns between -pi and pi
	//if ( resval > -4 && resval < 4 ) { return resval ; }
	//return 0 ;
	// seems to be no way to trap -1#IND00
	 
	if ( y == 0 && x == 0 ) { return _Type1_(0.0) ; }
	if ( x == 0 && y > 0  ) { return _Type1_(HALFPI) ; }
	if ( x == 0 && y < 0  ) { return _Type1_(HALFPI * -1.0) ; }
	if ( y == 0 && x < 0  ) { return _Type1_(2.0 * HALFPI) ; }
	_Type1_ amt = atan( y / fabs(x) ); 
	if ( x < 0 && y > 0 ) { amt = _Type1_(HALFPI - amt + HALFPI) ; }
	if ( x < 0 && y < 0 ) { amt = _Type1_(-HALFPI - amt - HALFPI) ; }
	return amt ; 
}






////////////////////////////////////////////////////////////////////  

// Classical functions


_inline _Type1_
re ( _Type0_  x)
{
  return x.r;
}

_inline _Type1_
im ( _Type0_  x)
{
  return x.i;
}

_inline _Type0_
ir( _Type0_  x)
{
  return _Type0_(x.i, x.r);
}


_inline _Type1_
abs ( _Type0_  x) 
{
  return sqrt( (x.r * x.r) + (x.i * x.i) );
}

_inline _Type1_
norm ( _Type0_  x)
{
  return x.r*x.r+x.i*x.i;
}

_inline _Type1_
arg  (_Type0_  x)
{
   return _Type1_(ATAN2(x.i, x.r));
}

// 

_inline int 
sgn ( _Type0_  x)
{
  return x.r==0.0 ? (x.i==0.0 ? 0:(x.i<0.0 ? -1:1)) : (x.r<0.0 ? -1:1);
}

_inline _Type0_
rec ( _Type0_  x) 
{
  return _Type0_ (x.r*cos(x.i), x.r*sin(x.i));
}

_inline _Type0_
rec ( _Type1_ r,  _Type1_ t) 
{
  return _Type0_ (r*cos(t), r*sin(t));
}


#ifndef hypot
#define hypot(r,i)  ( sqrt( (r * r) + (i * i) ) ) 
#endif


_inline _Type0_
pol ( _Type0_  x) 
{
  return _Type0_ ( _Type1_(hypot(x.r,x.i)), _Type1_(ATAN2(x.i,x.r)) );
}

_inline _Type0_
pol ( _Type1_ x,  _Type1_ y) 
{
  return _Type0_( _Type1_(hypot(x,y)), _Type1_(ATAN2(y,x)) );
}


// Unary functions

_Type0_ NEG(_Type0_ x)   // - operator 
{
  return _Type0_ (-x.r, -x.i);
}

_inline _Type0_ COMPLIMENT(_Type0_ x)  // ~ operator 
{
  return _Type0_ (x.r, -x.i);
}



// Arithmetic functions - sorry no operators in GLSL ... yet ?

_inline _Type0_ ADD(const _Type0_  x, const _Type0_  y)
{
  return _Type0_ (x.r+y.r, x.i+y.i);
}

_inline _Type0_ ADD(const _Type0_  x, _Type1_ y) 
{
  return _Type0_ (x.r+y, x.i);
}

_inline _Type0_ ADD(_Type1_ x, const _Type0_  y) 
{
  return _Type0_ (x+y.r, y.i);
}

_inline _Type0_ SUB(const _Type0_  x, const _Type0_  y)
{
  return _Type0_ (x.r-y.r, x.i-y.i);
}

_inline _Type0_ SUB(const _Type0_  x, _Type1_ y) 
{
  return _Type0_ (x.r-y, x.i);
}

_inline _Type0_ SUB(_Type1_ x, const _Type0_  y) 
{
  return _Type0_ (x-y.r, -y.i);
}

_inline _Type0_ MUL(const _Type0_  x, const _Type0_  y)
{
	return _Type0_ (x.r*y.r-x.i*y.i, x.r*y.i+x.i*y.r);
}

_inline _Type0_ MUL(const _Type0_  x, _Type1_ y) 
{
  return _Type0_ (x.r*y, x.i*y);
}

_inline _Type0_ MUL(_Type1_ x, const _Type0_  y) 
{
  return _Type0_ (x*y.r, x*y.i);
}

_inline _Type0_ DIV(const _Type0_  x, _Type1_ y) 
{
  return _Type0_ (x.r/y, x.i/y);
}


// Divisions avoiding overflow

   
_Type0_ DIV(_Type1_ x, _Type0_  y) 
{
  // FGZ -- using a faster and simpler real/complex algorithm, mathematical credit to A. Zacharias(MIT MSEE '55 + '57)
	_Type1_ norm_y = norm(y);
	return _Type0_( x*y.r/norm_y , -x*y.i/norm_y );
	 
	// the following is Laurent Deniau's original code (other than base_num replacing _Type1_) :
/* base_num nr, ni;
  if (fabs(y.r) <= fabs(y.i)) {
	base_num t = y.r/y.i;
	base_num n = y.i*(1.0+t*t);
	nr = x*t/n;
	ni = -x/n;
  } else {
	base_num t = y.i/y.r;
	base_num n = y.r*(1.0+t*t);
	nr = x/n;
	ni = -x*t/n;
  }
  return Complex<base_num> (nr, ni); */
}
// */


_Type0_ DIV( _Type0_  x,  _Type0_  y)
{
  _Type1_ nr, ni;
  if (fabs(y.r) <= fabs(y.i)) {
    _Type1_ t = y.r/y.i;
    _Type1_ n = y.i*(1+t*t);
    nr = (x.r*t+x.i)/n;
    ni = (x.i*t-x.r)/n;
  } else {
    _Type1_ t = y.i/y.r;
    _Type1_ n = y.r*(1+t*t);
    nr = (x.r+x.i*t)/n;
    ni = (x.i-x.r*t)/n;
  }
  return _Type0_ (nr, ni);
}

// */

 _inline _Type0_
pow ( _Type0_ x, _Type1_ y)
{
      _Type1_ R = exp( log( sqrt((x.r * x.r) + (x.i * x.i)) ) * y);
	  _Type1_ I = _Type1_(ATAN2(x.i, x.r) * y);
	
	return  _Type0_ (R * cos(I), R * sin(I));
} 
// above is good

/*
// WHAT ???
 _inline _Type0_
pow (_Type0_ x, int y)
{
  if (y == 0) return _Type0_ ( _Type1_(1.0), _Type1_(0.0) );
  
  _Type0_ r( _Type1_(1.0), _Type1_(0.0) );
  if (y < 0) { y = -y; x = DIVIDE( 1.0, x ); }
  for (;;) {
    if( y & 1)  r = MUL(r, x);
    y = y / 2 ;
	if(y != 0) x = MUL(x, x);
    else return r;
  }
}
*/

 _inline _Type0_
sqrt ( _Type0_  x)
{
  _Type1_ n = abs(x);
  _Type1_ nr, ni;
  if (n == 0.0) nr = ni = n;
  else if (x.r >= 0) {
    nr = sqrt( _Type1_(0.5)*(n+x.r));
    ni = _Type1_(0.5)*x.i/nr;
  } else {
    ni = _Type1_(sqrt(0.5*(n-x.r)));
    if (x.i < 0) ni = -ni;
    nr = _Type1_(0.5)*x.i/ni;
  }
  return _Type0_ (nr, ni); 
}

// above is good !!



//  Boolean  functions


_inline bool ISEQU(const _Type0_  x, const _Type0_  y)
{
  return x.r==y.r  &&  x.i==y.i;
}

_inline bool
ISEQU(const _Type0_  x, _Type1_ y) 
{
  return x.r==y  &&  x.i==0;
}

_inline bool
ISEQU(_Type1_ x, const _Type0_  y) 
{
  return x==y.r  &&  y.i==0;
}

_inline bool
ISNOTEQU(const _Type0_  x, const _Type0_  y)
{
  return x.r!=y.r || x.i!=y.i;
}

_inline bool
ISNOTEQU(const _Type0_  x, _Type1_ y) 
{
  return x.r!=y || x.i!=0;
}

_inline bool
ISNOTEQU(_Type1_ x, const _Type0_  y) 
{
  return x!=y.r || y.i!=0;
}

_inline bool
ISLESS(const _Type0_  x, const _Type0_  y)
{
  _Type1_ ax = abs(x)*sgn(x);
  _Type1_ ay = abs(y)*sgn(y);

  if (fabs(ax-ay)<DBL_EPSILON) return false;
  else return ax < ay;
}

_inline bool
ISLESS(const _Type0_  x, _Type1_ y) 
{
  _Type1_ ax = abs(x)*sgn(x);

  if (fabs(ax-y)<DBL_EPSILON) return false;
  else return ax < y;
}

_inline bool
ISLESS(_Type1_ x, const _Type0_  y) 
{
  _Type1_ ay = abs(y)*sgn(y);

  if (fabs(x-ay)<DBL_EPSILON) return false;
  else return x < ay;
}

_inline bool
ISMORE(const _Type0_  x, const _Type0_  y)
{
  return ISLESS(y, x);
}

_inline bool
ISMORE(const _Type0_  x, _Type1_ y) 
{
  return ISLESS(y, x);
}

_inline bool
ISMORE(_Type1_ x, const _Type0_  y) 
{
  return ISLESS(y, x);
}

_inline bool
ISLESSEQU(const _Type0_  x, const _Type0_  y)
{
  _Type1_ ax = abs(x)*sgn(x);
  _Type1_ ay = abs(y)*sgn(y);

  if (fabs(ax-ay)<DBL_EPSILON) return true;
  else return ax < ay;
}

_inline bool
ISLESSEQU(const _Type0_  x, _Type1_ y) 
{
  _Type1_ ax = abs(x)*sgn(x);

  if (fabs(ax-y)<DBL_EPSILON) return true;
  else return ax < y;
}

_inline bool
ISLESSEQU(_Type1_ x, const _Type0_  y) 
{
  _Type1_ ay = abs(y)*sgn(y);

  if (fabs(x-ay)<DBL_EPSILON) return true;
  else return x < ay;
}

_inline bool
ISMOREEQU(const _Type0_  x, const _Type0_  y)
{
  return ISLESSEQU(y, x);
}

_inline bool
ISMOREEQU(const _Type0_  x, _Type1_ y) 
{
  return ISLESSEQU(y, x);
}

_inline bool
ISMOREEQU(_Type1_ x, const _Type0_  y) 
{
  return ISLESSEQU(y, x);
}




// Power/Logarithmic functions


_inline _Type0_
exp (  _Type0_  x)
{
  return rec( _Type1_(exp(x.r)), x.i );
}


_inline _Type0_
ln (  _Type0_  x)
{
  return _Type0_ (log(abs(x)), arg(x));
}

_Type1_ ln(_Type1_ x) { return log(x); }


_inline _Type0_
log (  _Type0_  x)
{
  return _Type0_ (log(abs(x)), arg(x));
}

_inline _Type0_
log10 (  _Type0_  x)
{
  return DIV(ln(x), log( _Type1_(10.0) )); // FGZ VC2005
}


_inline _Type0_
pow (  _Type0_  x,   _Type0_  y)
{	 
  _Type1_ r = log(abs(x));
  _Type1_ t = arg(x);
  return rec( _Type1_ (exp(r*y.r - y.i*t)), y.i*r + y.r*t );
}


_inline _Type0_
pow (_Type1_ x,   _Type0_  y)
{
  return exp(MUL(y,ln(x)));
}


// Trigonometric functions

_inline _Type0_ 
sin (  _Type0_  x)
{
  	return _Type0_ ( sin(x.r) * cosh(x.i), cos(x.r) * sinh(x.i) );
}


_inline _Type0_ 
cos (  _Type0_  x)
{
	return _Type0_ ( cos(x.r)*cosh(x.i), -sin(x.r)*sinh(x.i) );
}

_inline _Type0_ 
tan (  _Type0_  x)
{
   return ( DIV(sin(x), cos(x)) );
}


// Hyperbolic functions

_inline _Type0_ 
sinh (  _Type0_  x)
{
	return _Type0_ ( sinh(x.r)*cos(x.i), cosh(x.r)*sin(x.i) );
}

_inline _Type0_
cosh (  _Type0_  x)
{
	return _Type0_ ( cosh(x.r)*cos(x.i), sinh(x.r)*sin(x.i) );
}

_inline _Type0_ 
tanh (  _Type0_  x)
{
    return ( DIV(sinh(x), cosh(x)) );
}


// Inverse Hyperbolic functions
 
 
_inline _Type0_
asinh (  _Type0_  x)
{
  return NEG( ln(SUB(sqrt(ADD( _Type1_(1.0) , MUL(x,x))), x)) );
}


_inline _Type0_
acosh (  _Type0_  x)
{
  return ln(ADD(sqrt(SUB(MUL(x,x),_Type1_(1.0))), x));
}


_inline _Type0_
atanh (  _Type0_  x)
{
    _Type0_ I = _Type0_(_Type1_(0.0), _Type1_(1.0));
  _Type0_ Ix = ir(COMPLIMENT(x));
  return DIV( DIV(ln(ADD(I,Ix)), SUB(I,Ix)), 2 );
}


// Trigonometric functions


_inline _Type0_
asin (  _Type0_  x)
{
  return COMPLIMENT(ir(ln(sqrt(ADD(SUB(1,MUL(x,x)), ir(COMPLIMENT(x)))))));
}

_inline _Type0_
acos (  _Type0_  x)
{
  if (x.r*x.i<0)
    return ir(COMPLIMENT(acosh(x)));
  else
    return COMPLIMENT(ir(acosh(x)));
}

_inline _Type0_
atan (  _Type0_  x)
{
   _Type0_ I = _Type0_( _Type1_(0.0), _Type1_(1.0) );
  return ir(COMPLIMENT(DIV(ln(DIV(ADD(I,x),SUB(I,x))),2)));
}




// } // extern "C++"

// ---------------------
// undefine default defs
// ---------------------
#ifdef _DefaultDefs_
#  ifndef _DefaultCode_
#    undef _Type0_
#    undef _Type1_
#    undef _DefaultDefs_
#  endif
#endif

// FGZ :  END OF HEADER  - BEGIN OF CPP FILES FROM ORIGINAL ...


 
/*

struct deadmeat {
int XXX;
lrkgldkfgv ;
};

// */

// fractal.fs  as follows :


uniform vec2 param;
uniform int iters;
uniform int ipow_f;
uniform int OrbitFunc;  // UseEsmooth  floatG  fBailout
uniform bool UseEsmooth;
uniform float floatG;
uniform float fBailout;
uniform vec2 viewPos;
uniform float viewZoom;
uniform float viewRatio;
uniform float anim;
uniform bool juliaMode;
uniform bool blinkMode;

// layout(location = 0) 
out vec4 FragmentColor;

in vec2 pos;

#define MUL_ADD(a,b,c)  ( ((a) * (b)) + (c) )

// int ipow_f = 5 ;

//  struct myStruct   GOOD !
//  class myClass     invalid reserved word !
// template<class num> __int64 Round64(const num& v)    invalid reserved word ! no references !

#define ESFPLU_size  (8 * 8 * 256) 
float ESFPLU[ESFPLU_size];

float Fast_eSmooth(_Type1_ dNorm)
	{
		/*
		int bigNorm = int(( float(dNorm) * 256.0f)+0.5f); 
		if( bigNorm < ESFPLU_size )
			return ESFPLU[bigNorm]; 
		else
			return 0.0f ; */
		return float( pow(2.0, -dNorm) );  
	}

int julia(in vec2 z, out vec2 fMU) {
	int i;
	vec2 mu;
	float x,y,x0,y0,Rx,Ry,t;
	bool UseNormalizedColoring = false ;

	if(juliaMode) {
		mu = param;
	} else {
		mu = z;
	}
	x0 = mu.x ;
	y0 = mu.y ;   // mu is C in Z <<- Z^2 + C
	
	Complex ZZ, CC ;
	_Type1_  dNorm;
	ZZ.r = float(x0) ;
	ZZ.i = float(y0);
	CC.r = float(x0);
	CC.i = float(y0);
	
	// for(i=0; i<iters; i++) 
	// 	if(length(z) >= 4.0f) break;   // FGZ :  was 2.0f ...

		//   the original mandelbrot set ...
		//   z = vec2(z.x*z.x - z.y*z.y, 2.0f*z.x*z.y);
		//   z += mu;
		 
		x = z.x;
        y = z.y;

		if( x*x+y*y < fBailout ) 
		{
			// ... sorry ...
			 
		switch( OrbitFunc ) {
		case 1:
			i=0; 
			while( i < iters ) 
			{
				i++ ;
				ZZ = DIV(sin(ZZ), CC);
				dNorm = norm(ZZ);
				if( dNorm >= _Type1_(fBailout) ) break;
				if( UseEsmooth ) fMU.x += Fast_eSmooth(dNorm);
				
			} // end for 16 iter
			if( !UseEsmooth ) {
				   fMU.x = ZZ.r ; 
				   fMU.y = ZZ.i ;
				}
				
			break;
		case 2:
			i=0; 
			while( i < iters ) 
			{
				i++ ;
				ZZ = MUL(cos(ZZ), CC);
				dNorm = norm(ZZ);
				if( dNorm >= _Type1_(fBailout) ) break;
				if( UseEsmooth ) fMU.x += Fast_eSmooth(dNorm);
				
			} // end for 16 iter
			if( !UseEsmooth ) {
				   fMU.x = ZZ.r ; 
				   fMU.y = ZZ.i ;
				}

			break;
		
		 
		//////////////////////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////////////////////
		//    USER ENTERED EQUATION HERE !!
		//
		 
		case 3:
				// UseEsmooth  floatG  fBailout
				i=0; 
				while( i < iters ) 
				{
					i++ ;
					
					/////////////// DO NOT EDIT FOLLOWING  //////////////////
					  
					   // FGZ_USER_FUNCTION_INSERTION_POINT
					  
					//////////////// END /////////////////
					// ZZ = ADD( MUL(ZZ, ZZ), DIV( CC, ADD(_Type1_(1.0), MUL(floatG, ZZ) ))) ;
					
					dNorm = norm(ZZ);
					if( dNorm >= _Type1_(fBailout) ) break;
					if( UseEsmooth ) fMU.x += Fast_eSmooth(dNorm);
				}
				if( !UseEsmooth ) {
				   fMU.x = ZZ.r ; 
				   fMU.y = ZZ.i ;
				}
				 
				break;
				
		
		//////////////////////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////////////////////
		  
		case 0:
			
		switch ( ipow_f ) 
		{
		default :
		case 2:
			i=0; 
			while( i < iters ) 
			{
				i++ ;
				t = x;
				x = MUL_ADD(-y, y, MUL_ADD(x, x, x0));
				y = MUL_ADD(2.0f * t, y, y0);
				dNorm = x*x+y*y ;
				if( dNorm >= fBailout ) break;
				if( UseEsmooth ) fMU.x += Fast_eSmooth(dNorm);
			} 
			break;
		
		// odd powers are slightly different than even powers ... 
		case 3:
			for(i=0; i<iters; i++) {
				if( x*x+y*y >= fBailout ) break;
				Rx = x * x - y * y ;
				Ry = x * y + y * x ;
				
				t = MUL_ADD(-Ry, y, MUL_ADD(Rx, x, x0));     
				y = MUL_ADD(Rx, y, MUL_ADD(Ry, x, y0));
				x = t ;
				if( UseEsmooth ) fMU.x += Fast_eSmooth(x*x+y*y);
			} // end for 16 iter
			break;
		case 5:
			for(i=0; i<iters; i++) {
				if( x*x+y*y >= fBailout ) break;
				Rx = x * x - y * y ;
				Ry = x * y + y * x ;
				t = Rx * x - Ry * y ;
				Ry = Rx * y + Ry * x ;
				
				Rx = t * x - Ry * y ;
				Ry = t * y + Ry * x ;
				
				t = MUL_ADD(-Ry, y, MUL_ADD(Rx, x, x0));     
				y = MUL_ADD(Rx, y, MUL_ADD(Ry, x, y0));
				x = t ;
				if( UseEsmooth ) fMU.x += Fast_eSmooth(x*x+y*y);
			} // end for 16 iter
			break;
		case 7:
			for(i=0; i<iters; i++) {
				if( x*x+y*y >= 16.0f ) break;
				Rx = x * x - y * y ;
				Ry = x * y + y * x ;
				t = Rx * x - Ry * y ;
				Ry = Rx * y + Ry * x ;
				
				Rx = t * x - Ry * y ;
				Ry = t * y + Ry * x ;
				t = Rx * x - Ry * y ;
				Ry = Rx * y + Ry * x ;
				
				Rx = t * x - Ry * y ;
				Ry = t * y + Ry * x ;
				
				t = MUL_ADD(-Ry, y, MUL_ADD(Rx, x, x0));     
				y = MUL_ADD(Rx, y, MUL_ADD(Ry, x, y0));
				x = t ;
				if( UseEsmooth ) fMU.x += Fast_eSmooth(x*x+y*y);
			} // end for 16 iter
			break;
		case 9:
			for(i=0; i<iters; i++) {
				if( x*x+y*y >= 16.0f ) break;
				Rx = x * x - y * y ;
				Ry = x * y + y * x ;
				t = Rx * x - Ry * y ;
				Ry = Rx * y + Ry * x ;
				
				Rx = t * x - Ry * y ;
				Ry = t * y + Ry * x ;
				t = Rx * x - Ry * y ;
				Ry = Rx * y + Ry * x ;
				
				Rx = t * x - Ry * y ;
				Ry = t * y + Ry * x ;
				t = Rx * x - Ry * y ;
				Ry = Rx * y + Ry * x ;
				
				Rx = t * x - Ry * y ;
				Ry = t * y + Ry * x ;
				
				t = MUL_ADD(-Ry, y, MUL_ADD(Rx, x, x0));     
				y = MUL_ADD(Rx, y, MUL_ADD(Ry, x, y0));
				x = t ;
				if( UseEsmooth ) fMU.x += Fast_eSmooth(x*x+y*y);
			} // end for 16 iter
			break;
		
		// odd - Even power div ================= 
		case 4:
			for(i=0; i<iters; i++) {
				if( x*x+y*y >= 16.0f ) break;
				Rx = x * x - y * y ;
				Ry = x * y + y * x ;
				t = Rx * x - Ry * y ;
				Ry = Rx * y + Ry * x ;
				
				Rx = MUL_ADD(-Ry, y, MUL_ADD(t, x, x0));     
				y = MUL_ADD(t, y, MUL_ADD(Ry, x, y0));
				x = Rx ;
				if( UseEsmooth ) fMU.x += Fast_eSmooth(x*x+y*y);
			} // end for 16 iter
			break;
		case 6:
			for(i=0; i<iters; i++) {
				if( x*x+y*y >= 16.0f ) break;
				Rx = x * x - y * y ;
				Ry = x * y + y * x ;
				t = Rx * x - Ry * y ;
				Ry = Rx * y + Ry * x ;
				
				Rx = t * x - Ry * y ;
				Ry = t * y + Ry * x ;
				t = Rx * x - Ry * y ;
				Ry = Rx * y + Ry * x ;
				
				Rx = MUL_ADD(-Ry, y, MUL_ADD(t, x, x0));     
				y = MUL_ADD(t, y, MUL_ADD(Ry, x, y0));
				x = Rx ;
				if( UseEsmooth ) fMU.x += Fast_eSmooth(x*x+y*y);
			} // end for 16 iter
			break;
		case 8:
			for(i=0; i<iters; i++) {
				if( x*x+y*y >= 16.0f ) break;
				Rx = x * x - y * y ;
				Ry = x * y + y * x ;
				t = Rx * x - Ry * y ;
				Ry = Rx * y + Ry * x ;
				
				Rx = t * x - Ry * y ;
				Ry = t * y + Ry * x ;
				t = Rx * x - Ry * y ;
				Ry = Rx * y + Ry * x ;
				
				Rx = t * x - Ry * y ;
				Ry = t * y + Ry * x ;
				t = Rx * x - Ry * y ;
				Ry = Rx * y + Ry * x ;
				
				Rx = MUL_ADD(-Ry, y, MUL_ADD(t, x, x0));     
				y = MUL_ADD(t, y, MUL_ADD(Ry, x, y0));
				x = Rx ;
				if( UseEsmooth ) fMU.x += Fast_eSmooth(x*x+y*y);
			} // end for 16 iter
			break;
		} // end switch iPowF 
		
		} // end switch Orbit-Func
		} // end good to go on if( x*x+y*y < fBailout ) ...
		
	z.x = x ;
	z.y = y ;
	 
	if( OrbitFunc == 0 && !UseEsmooth )
		fMU = z ;
	if(i == iters) return -1;

	return i;
}

//////////////////////////////////////////



// const vec4 cmap_size4 = vec4(31.0f, 31.0f, 31.0f, 31.0f) ;

// #define zmod(num)  ( remainder(num, cmap_size4) )

/*
	// old 120 can't handle the following :
	const int _color_map[32] =  
	{0x00ffff, 0x206010, 0x200000, 0x38c020, 0x386010, 0x380000, 0x50c020, 0x506010, 0x500000, 0x68c020, 
	 0x686010, 0x680000, 0x80c020, 0x806010, 0x800000, 0x98c020, 0x986010, 0x980000, 0xb0c020, 0xb06010, 
	 0xb00000, 0xc8c020, 0xc86010, 0xc80000, 0xe0c020, 0xe06010, 0xe00000, 0xf8c020, 0xf86010, 0xf80000, 0x0000ff } ;
*/
	int _color_map[32] ;
	
	void LoadColorMap() 
	{   
		_color_map[0] = 0x00ffff;  // NOTE: do NOT use _color_map[08] and such ... it's thinks it's octal ...
		_color_map[1] = 0x206010;
		_color_map[2] = 0x200000;
		_color_map[3] = 0x38c020;
		_color_map[4] = 0x386010;
		_color_map[5] = 0x380000;
		_color_map[6] = 0x50c020;
		_color_map[7] = 0x506010;
		_color_map[8] = 0x500000;
		_color_map[9] = 0x68c020;
		_color_map[10] = 0x686010;
		_color_map[11] = 0x680000;
		_color_map[12] = 0x80c020;
		_color_map[13] = 0x80c020;
		_color_map[14] = 0x806010;
		_color_map[15] = 0x800000;
		_color_map[16] = 0x98c020;
		_color_map[17] = 0x986010;
		_color_map[18] = 0x980000;
		_color_map[19] = 0xb0c020;
		_color_map[20] = 0xb06010;
		_color_map[21] = 0xb00000;
		_color_map[22] = 0xc8c020;
		_color_map[23] = 0xc86010;
		_color_map[24] = 0xc80000;
		_color_map[25] = 0xe0c020;
		_color_map[26] = 0xe06010;
		_color_map[27] = 0xe00000;
		_color_map[28] = 0xf8c020;
		_color_map[29] = 0xf86010;
		_color_map[30] = 0xf80000;
		_color_map[31] = 0x0000ff;
		// */
	}
	
	// const ivec4  iFACTOR4 = ivec4(256, 256, 256, 256) ;
	// const vec4   fFACTOR4 = vec4(256.0f, 256.0f, 256.0f, 256.0f) ;
	// const vec4   ZEROf4    = vec4(0.0f, 0.0f, 0.0f, 0.0f) ;
	// const vec4   ONEf4     = vec4(1.0f, 1.0f, 1.0f, 1.0f) ;
	// const vec4   TWOf4     = vec4(2.0f, 2.0f, 2.0f, 2.0f) ;
	// const vec4   FOURf4    = vec4(4.0f, 4.0f, 4.0f, 4.0f) ;

#define maglimit 16.0f 

	// const vec4 fLOGpow   = vec4(0.6931472f, 0.6931472f, 0.6931472f, 0.6931472f) ;     //  ln(2) =  0.6931472f 
	// const vec4 fLOGpow4  = vec4(1.3862943f, 1.3862943f, 1.3862943f, 1.3862943f) ;     //  ln(4) = 1.3862943f  
	// const vec4 fLOGLOGml = vec4(1.0197814f, 1.0197814f, 1.0197814f, 1.0197814f) ;     //  ln(ln(16)) = 1.0197814f    ln(ln(4)) =  0.3266343f   ln(ln(2)) = -0.3665129f   
	
	const float fLOGpow   = 0.6931472f ;
	const float fLOGpow4  = 1.3862943f ;
	const float fLOGLOGml = 1.0197814f ;
	 
// MakeGradientColor template inforces _nextColor as int and bulletproofs _mantissa,  // ipow_f = 5;
// result is always a color from base_color to next_color, inclusive !

/*	
int MakeGradientColor(int _BaseColor, int _NextColor, int _iMan)
{
	  int br = _BaseColor & 0xFF ;
	  int bg = (_BaseColor & 0xFF00) >> 8;
	  int bb = (_BaseColor & 0xFF0000) >> 16;
	  
	  int nr = _NextColor & 0xFF ;
	  int ng = (_NextColor & 0xFF00) >> 8 ;
	  int nb = (_NextColor & 0xFF0000) >> 16 ;
	   
	  int iMan =  max( 0, min( 256, _iMan ) );  // adds xx.x ms over haze-color, on std image + AMD RM-70 ... was 2.3 ms
	return int(	  (( br -  ( ((br - (nr)) * iMan)/256 )))       | 
					( (( bg -  ( ((bg - (ng)) * iMan)/256 ))) << 8) | 
					( (( bb -  ( ((bb - (nb)) * iMan)/256 ))) <<16) ); 
}
*/

vec4 MakeGradientVec4(int _BaseColor, int _NextColor, int _iMan)
{
	  int br = _BaseColor & 0xFF ;
	  int bg = (_BaseColor & 0xFF00) >> 8;
	  int bb = (_BaseColor & 0xFF0000) >> 16;
	  
	  int nr = _NextColor & 0xFF ;
	  int ng = (_NextColor & 0xFF00) >> 8 ;
	  int nb = (_NextColor & 0xFF0000) >> 16 ;
	   
	  int iMan =  max( 0, min( 255, _iMan ) );
	
	return vec4( 
	float(( br -  ( ((br - (nr)) * iMan)/256 )))/256.0f  ,
	float(( bg -  ( ((bg - (ng)) * iMan)/256 )))/256.0f  ,
	float(( bb -  ( ((bb - (nb)) * iMan)/256 )))/256.0f  ,
	1.0f);
}	  

/*	
	ivec4 GetGradColor(vec4 fMU)
	{
			  ivec4 bigMU = ivec4(  max(ZEROf4, fMU) * fFACTOR4 ) ; 
		//	bigMU.s0 = (max(1.0f, fMU.s0 + 1.0f)*256.0f); 
		//	bigMU.s1 = (max(1.0f, fMU.s1 + 1.0f)*256.0f); 
		//	bigMU.s2 = (max(1.0f, fMU.s2 + 1.0f)*256.0f);
		//	bigMU.s3 = (max(1.0f, fMU.s3 + 1.0f)*256.0f);
			 
			  ivec4 iMU = (bigMU / 256) ;
			  ivec4 iMan = bigMU % 256  ; 
							
			// iMU-- ; 
			 
			  ivec4 clue, clue2, ecgs = ivec4(31, 31, 31, 31);
			clue = iMU % ecgs;
			clue2.x = ((clue.x+1>=31)? 0 : clue.x+1); 
			clue2.y = ((clue.y+1>=31)? 0 : clue.y+1); 
			clue2.w = ((clue.w+1>=31)? 0 : clue.w+1);
			clue2.z = ((clue.z+1>=31)? 0 : clue.z+1);
			  ivec4 retc ;
			retc.x = MakeGradientColor( _color_map[clue.x], _color_map[clue2.x], iMan.x );
			retc.y = MakeGradientColor( _color_map[clue.y], _color_map[clue2.y], iMan.y );
			retc.w = MakeGradientColor( _color_map[clue.w], _color_map[clue2.w], iMan.w );
			retc.z = MakeGradientColor( _color_map[clue.z], _color_map[clue2.z], iMan.z );
			 
			return retc; 
	}
*/ 

void init_setup() {

	I.r =  0.0 ;  I.i = 1.0 ;           // root of z^2+1 = 0 
	J.r = -0.5 ;  J.i =0.5*sqrt(3.0) ;        // root of z^2+z+1 = 0
	_I = DIV(1.0 , I);
	_J = DIV(1.0 , J);
 
	//float fstep = 1.0f / 256.0f ;
	//for(int istep=0; istep < ESFPLU_size; istep++)
	//	ESFPLU[istep] = pow( 2.0f , fstep * float(-istep) );
 
 
 }


//////////////////////////////////////////

void main(void) {
	init_setup();
	LoadColorMap();
	float r, g, b, pow_f;
	int bailout;
	vec4 ret;
	vec2 fMU ;
		fMU.y = 0.0f ;
		fMU.x = 0.0f ;
	 
	bailout = julia(pos, fMU);
	
	if(OrbitFunc == 0) pow_f = ipow_f;
	else pow_f = 2.0f;  // this needs work ... see ManyFractals code ... 
	
	if(bailout == -1) {
		r = g = b = 0.0f;
	} else {
		float zMU; 
		if( !UseEsmooth ) zMU = bailout + (( log(log((fBailout))) - log(log( length(fMU) )) ) / log(pow_f)  );  //  fLOGpow ) ;
		else zMU = fMU.x  ;
		int bigMU = int(  max( 0.0f, zMU ) * 256.0f ) ; 
		int iMU = (bigMU / 256) ;
		int iMan = bigMU % 256  , clue, clue2 ;
		clue = iMU % 31;
		clue2 = ((clue+1>=31)? 0 : clue+1); 
		ret = MakeGradientVec4(_color_map[clue], _color_map[clue2], iMan);
		if(blinkMode && bailout%32 == int(anim*32)) {
			ret.r = 1.0f;
		} 
	}
	 
	gl_FragColor = ret ;
	// FragmentColor = ret ; // vec4(r, g, b, 1.0f);
}



