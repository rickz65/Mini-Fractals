/////////////////////////////////////////////////////////////////////////////////////
//
//
//  Mini-Fractals/glMandel Copyright © July 2013 Frederick G Zacharias Covered under the CPOL 
//
//  See License-CPOL.txt for the Full License.
//
//

#pragma once
#include <stdio.h>
#include <assert.h>
#include <math.h>
#include <stdlib.h>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iostream>
#include <string>

#include "../GL/glew.h"  // must come before gl.h 
#include "..\GL\wglew.h"
#pragma comment(lib, "glew32.lib")

// #include "..\GL\ogl.h"   // maybe this can be of help to someone else, but it conflicts with glew.h ... and has link errors.

#include "../GL/gl.h"  // the actual opengl file

#include "GL/glu.h"

// #include "mandel.h"   // Mandelbrot class def.

#include "NeHeGL.h"	
 
// #define need_GL_ARB_vertex_buffer_object

using namespace std;

// from OpenCL SDK ...
#define SDK_SUCCESS 0
#define SDK_FAILURE 1
#define SDK_EXPECTED_FAILURE 2
#ifndef sqr 
#define sqr(x) ( (x) * (x) )
#endif
#ifndef MIN 
#define MIN(a,b) ((a)<(b) ? (a):(b))
#endif
#ifndef min
#define min MIN
#endif
#ifndef MAX
#define MAX(a,b) ((a)>(b) ? (a):(b))
#endif
#ifndef max
#define max MAX
#endif



class ShadersApp {
	friend class COutput ;
	friend class CBlackMandelDlg ;
	 
public:
	ShadersApp(bool fs=false);
	ShadersApp(int w, int h, bool fs=false);
	~ShadersApp();
	// void mainLoop(void);

private:
	 
	void init(int w, int h, bool fs);
	void initWindow();
	GLuint loadShaderFile(const char *filename, GLenum type, bool ShowMsgs = true);
	void checkLinkError(GLuint prog);
	 
protected:
	
	void destroyWindow();

	void unzoom(void);
	void setParam(void);

	bool initGL(bool ShowMsgs);
	void reshapeView();
	void drawScene();

	bool MakeOutputSpace(int w, int h) ;

	float paramX;
	float paramY;
	int iterations;
	
	int ipow_f ;
	int OrbitFunc ;
	float				pow_F ;                     // The power F in  Z <= Z^F + C
	int                 Orbitfunc;      // the radio button selected

	string m_str_User_Function;

	BOOL m_UseEsmooth;
	float m_float_G;
	float m_bailout;
	float posX;
	float posY;
	float zoom;
	float ratio;
	bool juliaMode;
	bool blinkMode;

	/// 
	float               xpos;                      /**< x-coordinate for set */
    float               ypos;                      /**< y-coordinate for set */
    float               xsize;                     /**< window width for set */
    float               ysize;                     /**< window height for set */
    float               xstep;                     /**< x-increment for set */
    float               ystep;                     /**< y-increment for set */
    int                 maxIterations;			   //  max times to recalc the equation in the GLSL core ...
	int                 width;                     /**< width of the output image */
    int                 height;                    /**< height of the output image */
	
	///

	float cursorX;
	float cursorY;
	CPoint                cursor;    // the mouse location.
	
	int pixelMoveBeginX;
	int pixelMoveBeginY;
	float moveBeginX;
	float moveBeginY;
	int zooming;
	int movingX;
	int movingY;

	int curW;   // the window curent width 
	int curH;
	int windowedW;
	int windowedH;
	double __time;
	bool fastMode;
	bool fullscreen;
	bool quitting;

	GLuint*   output ;
	// HWND       m_hWnd ;
	GL_Window*			m_pwindow ;
	
	/* Vertex shader: */
	GLuint fractalVS;
	
	/* Fragment shader: */
	GLuint fractalFS;
	
	/* Program object: */
	GLuint fractalPO;
	
	// from the original opencl mandelbrot class ...
	bool setup(bool ShowMsgs = true); 
	
	void setXPos(float xp); 
	void setYPos(float yp); 
	void  setWidth(int W); 
	void setHeight(int H); 
	void setSize(int nWidth, int nHeight);
	 
	// from mandelbrot.h ... INLINE functions
	// FGZ - was part of openCL SDK sample app class ...
	inline GLuint* getPixels()
	{
		return output ;
	}
    inline int getWidth(void)
	{
		return width;
	}
    inline int getHeight(void)
	{
		return height ;
	}
    inline int getMaxIterations(void)
    {
        return maxIterations;
    }
    inline void setMaxIterations(int maxIter)
    {
        maxIterations = maxIter;
		// transfer across
    }

    inline float getXSize(void)
    {
        return xsize;
    }
    inline void setXSize(float xs)
    {
		xsize = xs ;
		ysize = -xs ;
        xstep = xsize / width;
		ystep = ysize / height;
		//minX = xpos - xstep * (width/2.0f);
		//minY = ypos + ystep * (height/2.0f);
		//maxX = xpos + xstep * (width/2.0f);
		//maxY = ypos - ystep * (height/2.0f);
		zoom = 1.0f / xsize ;  // 0.0021484f
	}
    inline float getXStep(void)
    {
        return xstep;
    }
    inline float getYStep(void)
    {
        return ystep;
    }
    inline float getXPos(void)
    {
        return xpos;
    }
    inline float getYPos(void)
    {
        return ypos;
    }
    
	// FGZ - High Speed, microsecond, timing routines.
	__int64 Get_PC_Count();
	double GetTimeSpan(__int64 Start, __int64 Finish);
	std::string ConvertTimeSpan(double Time);
	 
	 
	 
	// I don't think i use these anymore ...
/*	float               minX ;
	float               minY ;
	float               maxX ;
	float               maxY ;
*/
	 
//	ShadersApp*			  m_shader ;   // our shader object, the link into GLSL ... 
	 
protected:

	double                m_PC_Freq;    // how fast is YOUR CPU ?
	
public:
	
	double				m_dRtime ;   // our last calculation time, in seconds.

	bool         m_isInitOK ;       // safety 

};
