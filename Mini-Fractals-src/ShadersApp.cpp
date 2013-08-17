/////////////////////////////////////////////////////////////////////////////////////
//
//
//  Mini-Fractals/glMandel Copyright © July 2013 Frederick G Zacharias Covered under the CPOL 
//
//  See License-CPOL.txt for the Full License.
//
//

#include "stdafx.h"
#include "config.hpp"

#include "../glm/glm.hpp"

#include "ShadersApp.hpp"

using namespace glm;


#define BUFFER_OFFSET(i) ((char *)NULL + (i))

static GLuint vbo[4], ibo[4];

typedef struct {
	vec2 pos;
} vertex;


// fgz added Round64 Template 
 
template<class num> __int64 Round64(const num& v)
{	
	if( v >= 0.0 )
		return __int64(v + 0.5);
	if( v - __int64(v) == -0.5 ) 
		return __int64(v);
	return __int64(v - 0.5);
}
// */
// end fgz add 


// Based Off Of Code Supplied At OpenGL.org
bool IsExtensionSupported( char* szTargetExtension )
{
	const char *pszExtensions = NULL;
	const char *pszStart;
	char *pszWhere, *pszTerminator;
	std::string mys;

	// Extension names should not have spaces
	pszWhere = (char *) strchr( szTargetExtension, ' ' );
	if( pszWhere || *szTargetExtension == '\0' )
		return false;
	// Get Extensions String
	// pszExtensions = glGetString( GL_EXTENSIONS );
	int max = 0;
	 
	glGetIntegerv(GL_EXTENSIONS, &max);
	for(int i = 0; i < max; i++)
		mys += (char*)glGetStringi(GL_EXTENSIONS, i) ;
	pszExtensions = mys.c_str();
	 
	if( pszExtensions == NULL ) {  printf("  pszExtensions is NULL \n");  return false; }
 
	// Search The Extensions String For An Exact Copy
	pszStart = pszExtensions;
	for(;;)
	{
		pszWhere = (char *) strstr( (const char *) pszStart, szTargetExtension );
		if( !pszWhere )
			break;
		pszTerminator = pszWhere + strlen( szTargetExtension );
		if( pszWhere == pszStart || *( pszWhere - 1 ) == ' ' )
			if( *pszTerminator == ' ' || *pszTerminator == '\0' )
				return true;
		pszStart = pszTerminator;
	}
	return false;
}


static void make_buffers(void) {
	vertex *verts = new vertex[4];
	verts[0].pos = vec2(-1.0f, -1.0f);
	verts[1].pos = vec2(1.0f, -1.0f);

	verts[2].pos = vec2(1.0f, 1.0f);

	verts[3].pos = vec2(-1.0f, 1.0f);

	if( glGenBuffers == NULL ) {
	glGenBuffers = (PFNGLGENBUFFERSPROC)wglGetProcAddress("glGenBuffers");
	if( glGenBuffers == NULL ) MessageBox(NULL, _T("\n GL Function Unavailable !! glGenBuffers \n"), _T("debug"), MB_OK); 
	} 

	if( glBindBuffer == NULL ) {
	glBindBuffer = (PFNGLBINDBUFFERPROC)wglGetProcAddress("glBindBuffer");
	if( glBindBuffer == NULL ) MessageBox(NULL, _T("\n GL Function Unavailable !! glBindBuffer !! \n"), _T("debug"), MB_OK); 
	} 
	if( glBufferData == NULL ) {
	glBufferData = (PFNGLBUFFERDATAPROC)wglGetProcAddress("glBufferData");
	if( glBufferData == NULL ) MessageBox(NULL, _T("\n GL Function Unavailable !! glBufferData !! \n"), _T("debug"), MB_OK); 
	} 
	if( glEnableVertexAttribArray == NULL ) {
	glEnableVertexAttribArray = (PFNGLENABLEVERTEXATTRIBARRAYPROC)wglGetProcAddress("glEnableVertexAttribArray");
	if( glEnableVertexAttribArray == NULL ) MessageBox(NULL, _T("\n GL Function Unavailable !! glEnableVertexAttribArray !! \n"), _T("debug"), MB_OK); 
	} 
	if( glVertexAttribPointer == NULL ) {
	glVertexAttribPointer = (PFNGLVERTEXATTRIBPOINTERPROC)wglGetProcAddress("glVertexAttribPointer");
	if( glVertexAttribPointer == NULL ) MessageBox(NULL, _T("\n GL Function Unavailable !! glVertexAttribPointer !! \n"), _T("debug"), MB_OK); 
	} 

	glGenBuffers(1, vbo);  
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, 4*sizeof(vertex), verts, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(vertex), 0);
	delete [] verts;
	unsigned int *idxs = new unsigned int[4];
	idxs[0] = 3;
	idxs[1] = 0;
	idxs[2] = 2;
	idxs[3] = 1;
	glGenBuffers(1, ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo[0]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 4*sizeof(unsigned int), idxs, GL_STATIC_DRAW);
	delete [] idxs;
}



bool ShadersApp::initGL(bool ShowMsgs) {
	// from mandelbrot::setupGL() 
	
	// glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	
	glShadeModel(GL_SMOOTH);
	glEnable(GL_DEPTH_TEST);
	glViewport (0, 0, (GLsizei) width, (GLsizei) height);
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity();
	
	// TO DO:  consider moving these gl-floats to class members, so we can change the Ortho in the future 
	GLfloat minXX = -2.2f, maxXX = 0.8f, minYY = -1.5f, maxYY = 1.5f;
	// excluding the following draws in a black grid overlay ... 
	glOrtho(minXX, maxXX, minYY, maxYY, ((GLfloat)-1), (GLfloat)1);  // critical to setting the coordin/view relationship.

	//  end mandel ...


	glClearColor(0.0, 0.0, 0.0, 0.0);
			
	make_buffers();
	
	fractalVS = loadShaderFile("./shaders/fractal.vs", GL_VERTEX_SHADER, ShowMsgs);
	fractalFS = loadShaderFile("./shaders/shader-code-workspace.fs", GL_FRAGMENT_SHADER, ShowMsgs);   // fractal.fs
	if( fractalFS == -3 ) return false ;

	
	if( glCreateProgram == NULL ) {
		glCreateProgram = (PFNGLCREATEPROGRAMPROC)wglGetProcAddress("glCreateProgram");
		if( glCreateProgram == NULL ) MessageBox(NULL, _T("\n GL Function Unavailable !! glBindBuffer !! \n"), _T("debug"), MB_OK); 
	} 
	fractalPO = glCreateProgram();
	if( glAttachShader == NULL ) {
		glAttachShader = (PFNGLATTACHSHADERPROC)wglGetProcAddress("glAttachShader");
		if( glAttachShader == NULL ) MessageBox(NULL, _T("\n GL Function Unavailable !! glBindBuffer !! \n"), _T("debug"), MB_OK); 
	}
	glAttachShader(fractalPO, fractalVS);
	glAttachShader(fractalPO, fractalFS);
	if( glLinkProgram == NULL ) {
		glLinkProgram = (PFNGLLINKPROGRAMPROC)wglGetProcAddress("glLinkProgram");
		if( glLinkProgram == NULL ) MessageBox(NULL, _T("\n GL Function Unavailable !! glBindBuffer !! \n"), _T("debug"), MB_OK); 
	} 
	glLinkProgram(fractalPO);
	checkLinkError(fractalPO);
	return true ;
}


void ShadersApp::reshapeView() {
	glViewport(0, 0, curW, curH);

	if(curH == 0) {
		ratio = 1.0f;
	} else {
		ratio = (float)curW/curH;
	}
}


void ShadersApp::drawScene() {    
	
	// the pre-call stuff from Mandelbrot::run()
	iterations = this->maxIterations ;
	ipow_f = (int)this->pow_F ;
	
	OrbitFunc = Orbitfunc ;

	m_UseEsmooth = m_UseEsmooth ;
	m_float_G = m_float_G ;
	m_bailout = m_bailout; 

	posX = xpos;  // minX ; // this->xpos ;  // This is the CENTER of the Area in Complex Plane CoOrdinates ... 
	posY = ypos ; // minY ; // this->ypos ;
	
	zoom = 2.0f / xsize ;  // zoom is inverse size ... 0.5 is radius 2.0 ... zoom > 1 magnifies in
	ratio = float(width) / float(height) ;

	__time = 2.2 ; // unused .. 

	// juliaMode = false ;  
	blinkMode = false ;  // changes to have some stripes.  // To DO : impliment GUI control for this ...
	cursorX = float(this->cursor.x) ;  // only needed for julia point selection.
	cursorY = float(this->cursor.y) ;
	setParam();         // these are for complex plane coOrdinates of a julia set initializer point ...
	
	// this is only needed if using BitBLT() in CBlackMandelDlg::OnBnClickedButtonMake() ... off-screen example ...
	// uncomment following ... item 1 of 5  search for:  FGZ_BITBLT_EXAMPLE 
	  //  if( output == NULL ) if( !MakeOutputSpace(width, height) ) MessageBox(NULL, _T("\n ~~PANIC~~ ouput Bitmap Space is NULL !! \n"), _T("debug"), MB_OK); 
	
	
	
	if( glIsProgram == NULL ) {
		glIsProgram = (PFNGLISPROGRAMPROC)wglGetProcAddress("glIsProgram");
		if( glIsProgram == NULL ) MessageBox(NULL, _T("\n GL Function Unavailable !! glIsProgram !! \n"), _T("debug"), MB_OK); 
	}
	if( glUseProgram == NULL ) {
		glUseProgram = (PFNGLUSEPROGRAMPROC)wglGetProcAddress("glUseProgram");
		if( glUseProgram == NULL ) MessageBox(NULL, _T("\n GL Function Unavailable !! glUseProgram !! \n"), _T("debug"), MB_OK); 
	}
	if( glGetUniformLocation == NULL ) {
		glGetUniformLocation = (PFNGLGETUNIFORMLOCATIONPROC)wglGetProcAddress("glGetUniformLocation");
		if( glGetUniformLocation == NULL ) MessageBox(NULL, _T("\n GL Function Unavailable !! glGetUniformLocation !! \n"), _T("debug"), MB_OK); 
	}
	if( glUniform2f == NULL ) {
		glUniform2f = (PFNGLUNIFORM2FPROC)wglGetProcAddress("glUniform2f");
		if( glUniform2f == NULL ) MessageBox(NULL, _T("\n GL Function Unavailable !! glUniform2f !! \n"), _T("debug"), MB_OK); 
	}
	if( glUniform1i == NULL ) {
		glUniform1i = (PFNGLUNIFORM1IPROC)wglGetProcAddress("glUniform1i");
		if( glUniform1i == NULL ) MessageBox(NULL, _T("\n GL Function Unavailable !! glUniform1i !! \n"), _T("debug"), MB_OK); 
	}
	if( glUniform1f == NULL ) {
		glUniform1f = (PFNGLUNIFORM1FPROC)wglGetProcAddress("glUniform1f");
		if( glUniform1f == NULL ) MessageBox(NULL, _T("\n GL Function Unavailable !! glUniform1f !! \n"), _T("debug"), MB_OK); 
	}
	  
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);   // CLEARING THE DEPTH BIT DREW THE IMAGE NOT THE BLACK SQUARE !!!!
	 
	if( ! glIsProgram(fractalPO) ) MessageBox(NULL, _T("\n INVALID fractalPO  !! \n"), _T("debug"), MB_OK);
	
	glUseProgram(fractalPO);
	
	// now transfer the app's parameters for the fractal to the GLSL programs ...
	glUniform2f(glGetUniformLocation(fractalPO, "param"), paramX, paramY);
	glUniform1i(glGetUniformLocation(fractalPO, "iters"), iterations);
	
	glUniform1i(glGetUniformLocation(fractalPO, "ipow_f"), ipow_f);

	glUniform1i(glGetUniformLocation(fractalPO, "OrbitFunc"), OrbitFunc);

	glUniform1i(glGetUniformLocation(fractalPO, "UseEsmooth"), m_UseEsmooth) ;  
	glUniform1f(glGetUniformLocation(fractalPO, "floatG"), m_float_G) ;
	glUniform1f(glGetUniformLocation(fractalPO, "fBailout"), m_bailout) ;

	glUniform2f(glGetUniformLocation(fractalPO, "viewPos"), posX, posY);
	glUniform1f(glGetUniformLocation(fractalPO, "viewZoom"), zoom);
	glUniform1f(glGetUniformLocation(fractalPO, "viewRatio"), ratio);
	glUniform1f(glGetUniformLocation(fractalPO, "anim"), fmodf(float(__time)*0.5f, 1.0f));
	glUniform1i(glGetUniformLocation(fractalPO, "juliaMode"), juliaMode);
	glUniform1i(glGetUniformLocation(fractalPO, "blinkMode"), blinkMode);
	
	
	// __int64 Start = Get_PC_Count(), Finish=0;           // timing done in CBlackMandelDlg::OnBnClickedButtonMake()
	
	// This Computes the Fractal ... and draws it into the opengl context 
	//   which is tied into the CDC of the Static Picture box in the output-dialog ... and that's how the image appears like magic !
	
	glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_INT, BUFFER_OFFSET(0));       // This returns before image is done ...
	
	// glFinish helps insure the image is flushed to the output memory, avoids flicker, and gives a fairly accurate calc time. 
	
	glFinish();  // this waits until done ...
	// glFlush();  // this does not wait ...
	
	// Finish = Get_PC_Count();					// timing done in CBlackMandelDlg::OnBnClickedButtonMake()
	// m_dRtime = GetTimeSpan( Start, Finish ); 
	
	
	// BELOW DOES WORK FINE before Swap-Buffers !! ... for off-screen drawing example, see CBlackMandelDlg::OnBnClickedButtonMake() 
	//
	  //  glReadPixels((GLint)0, (GLint)0, (GLint)curW, (GLint)curH, GL_BGRA_EXT, GL_UNSIGNED_BYTE, output); 
	//
	// ... item 2 of 5  FGZ_BITBLT_EXAMPLE 
	 
	// this swaps the drawing bit space to the display bit space ... 

	  SwapBuffers( m_pwindow->hDC );  // see note elsewhere: if all i do is this, no bitblt, then i get black square ... result: issue solved.
		// comment out above swap, if using bitBLT to display image ... item 3 of X  FGZ_BITBLT_EXAMPLE
	
	
}


GLuint ShadersApp::loadShaderFile(const char *filename, GLenum type,  bool ShowMsgs) {
	
	if( glCreateShader == NULL ) {
		glCreateShader = (PFNGLCREATESHADERPROC)wglGetProcAddress("glCreateShader");
		if( glCreateShader == NULL ) MessageBox(NULL, _T("\n GL Function Unavailable !! glCreateShader !! \n"), _T("debug"), MB_OK); 
	}
	if( glShaderSource == NULL ) {
		glShaderSource = (PFNGLSHADERSOURCEPROC)wglGetProcAddress("glShaderSource");
		if( glShaderSource == NULL ) MessageBox(NULL, _T("\n GL Function Unavailable !! glShaderSource !! \n"), _T("debug"), MB_OK); 
	}
	if( glCompileShader == NULL ) {
		glCompileShader = (PFNGLCOMPILESHADERPROC)wglGetProcAddress("glCompileShader");
		if( glCompileShader == NULL ) MessageBox(NULL, _T("\n GL Function Unavailable !! glCompileShader !! \n"), _T("debug"), MB_OK); 
	}
	if( glGetShaderiv == NULL ) {
		glGetShaderiv = (PFNGLGETSHADERIVPROC)wglGetProcAddress("glGetShaderiv");
		if( glGetShaderiv == NULL ) MessageBox(NULL, _T("\n GL Function Unavailable !! glGetShaderiv !! \n"), _T("debug"), MB_OK); 
	}
	if( glGetShaderInfoLog == NULL ) {
		glGetShaderInfoLog = (PFNGLGETSHADERINFOLOGPROC)wglGetProcAddress("glGetShaderInfoLog");
		if( glGetShaderInfoLog == NULL ) MessageBox(NULL, _T("\n GL Function Unavailable !! glGetShaderInfoLog !! \n"), _T("debug"), MB_OK); 
	}
	
	GLuint shaderID = glCreateShader(type);

	ifstream file(filename);
	string source;

	while(file.good()) {
		int c = file.get();
		if(c >= 0) {
			source += (char)c;
		} else break;
	}
	file.close();

	// insert User-Func at the User-ID 'marker' String ...
	std::size_t ID_loc = source.find("FGZ_USER_FUNCTION_INSERTION_POINT") ;
	if( ID_loc != string::npos )
		source.insert( ID_loc-5, m_str_User_Function.c_str() );
	// cout << source << endl ;
	
	glShaderSource(shaderID, 1, (const GLchar **)&source, NULL);
	glCompileShader(shaderID);

	GLint status;
	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &status);
	if(status != GL_TRUE) {
		cout << "Failed to compile shader from \"" << filename << "\"" << endl;
		GLchar log[16384];
		glGetShaderInfoLog(shaderID, 16384, NULL, log);
		cout << "Log following:" << endl;
		cout << log << endl;
		if( ShowMsgs ) {
			MessageBox(NULL, _T("\n Failed to COMPILE shaders: see log in next M.B. !! \n"), _T("debug"), MB_OK);
			MessageBoxA(NULL, (char*)log, "debug", MB_OK);
		}
		
		return -3 ;


	}

	return shaderID;
}

void ShadersApp::checkLinkError(GLuint prog) {
	if( glGetProgramiv == NULL ) {
		glGetProgramiv = (PFNGLGETPROGRAMIVPROC)wglGetProcAddress("glGetProgramiv");
		if( glGetProgramiv == NULL ) MessageBox(NULL, _T("\n GL Function Unavailable !! glGetProgramiv !! \n"), _T("debug"), MB_OK); 
	}
	if( glGetProgramInfoLog == NULL ) {
		glGetProgramInfoLog = (PFNGLGETPROGRAMINFOLOGPROC)wglGetProcAddress("glGetProgramInfoLog");
		if( glGetProgramInfoLog == NULL ) MessageBox(NULL, _T("\n GL Function Unavailable !! glGetProgramInfoLog !! \n"), _T("debug"), MB_OK); 
	}
	
	GLint status;
	glGetProgramiv(prog, GL_LINK_STATUS, &status);
	if(status != GL_TRUE) {
		cerr << "Failed to LINK shaders:" << endl;
		GLchar log[16384];
		glGetProgramInfoLog(prog, 16384, NULL, log);
		cerr << "Log following:" << endl;
		cerr << log << endl;
		MessageBox(NULL, _T("\n Failed to link shaders: see log in next M.B. !! \n"), _T("debug"), MB_OK);
		MessageBoxA(NULL, (char*)log, "debug", MB_OK);
	}
}

//////////////////////////////////////////////////////
//////////////////////////////////////////////////////

ShadersApp::ShadersApp(bool fs) {
	init(DEF_W, DEF_H, fs);
	output = NULL;
}

ShadersApp::ShadersApp(int w, int h, bool fs) {
	init(w, h, fs);
	output = NULL ;
}

ShadersApp::~ShadersApp() {
	destroyWindow();
	
	 
}

bool ShadersApp::MakeOutputSpace(int w, int h) 
{
	// allocate and init memory used by host.
	GLuint sizeBytes = GLuint(w * h) * sizeof(GLuint);
	if( output != NULL ) free( output );
	
	output = (GLuint *) malloc(sizeBytes);
	
	if( output == NULL ) return false ;
	 
	for(int yy=0; yy<w*h/2; yy++)
		output[yy] = 0x00FFFF00 ;   // the default YELLOW 'background' square, if failure ... GDI only.
	 return true;
}

void ShadersApp::init(int w, int h, bool fs) {
	paramX = 0.0f;
	paramY = 0.0f;
	iterations = 32;
	unzoom();
	ratio = 1.0f;
	juliaMode = false;
	blinkMode = false;

	cursorX = 0.0f;
	cursorY = 0.0f;
	moveBeginX = 0.0f;
	moveBeginY = 0.0f;
	zooming = 0;
	movingX = 0;
	movingY = 0;

	windowedW = w;
	windowedH = h;
	__time = 0.0;
	fastMode = false;
	fullscreen = fs;
	quitting = false;

	initWindow();
	 
	//  from mandelbrot class constructor ...
	m_PC_Freq = 1;
		
	// if( output != NULL ) free(output);  // this is called from constructor ... issue in vs2010
	output = NULL ;
	
	m_isInitOK = false ;
	pow_F = 4 ;
	width = 512;
	height= 512;
	xpos = 0;
	ypos = 0;
	xsize = 0.005859375f;
	ysize = -0.005859375f;
	xstep = float(xsize)/width ;
	ystep = float(ysize)/height ;
	maxIterations = 64;
	//minX = -2;
	//minY = -2;
	//maxX = 2;
	//maxY= 2;

	// from mandelbrot:init() 

	xpos = 0.0f;
    ypos = 0.0f;
    width = w;
    height = h;
    xsize = 2.20f/width;
    ysize = -2.20f/height;
    xstep = xsize / width;
	ystep = ysize / height;
	// minX = xpos - xstep * (width/2.0f);
	// minY = ypos - ystep * (height/2.0f);
	// maxX = xpos + xstep * (width/2.0f);
	// maxY = ypos + ystep * (height/2.0f);
	
	maxIterations = 200;  // Beep(3000, 333);
    
	pow_F = 4.0f ;  //NOTE: if change back to 2.0f then set xpos = -0.7f  !!!
			
	// You will need this line if using the bitBLT in CBlackMandelDlg::OnBnClickedButtonMake()
	
	  // if( !MakeOutputSpace(w,h) ) m_isInitOK = false ;
			 
	// above ... item 4 of 5  FGZ_BITBLT_EXAMPLE
	
	// FGZ - 5-21-2010: get the invariant performance counter frequency
	LARGE_INTEGER pc_freq;
	if( QueryPerformanceFrequency( &pc_freq ) == FALSE ) 
		m_PC_Freq = 1000.0 ;
	else m_PC_Freq = double(pc_freq.QuadPart);
	
}

void ShadersApp::initWindow() {
	
	curW = windowedW;
	curH = windowedH;

	reshapeView();
	
}

bool ShadersApp::setup( bool ShowMsgs)  
{
	// Make sure width is a multiple of 4
    width = (  (int(width) + 3) & ~(4 - 1)  );
    return  initGL(ShowMsgs);
}



void ShadersApp::destroyWindow() {
	
	//  clean up needed.
	free(output);
	output = NULL;  
}

void ShadersApp::unzoom() {
	posX = 0.0f;
	posY = 0.0f;
	zoom = 0.7f;
}

void ShadersApp::setParam() {
	// FGZ - paramX/Y  is the Julia point of the complex plane, initializer for Z ...
	paramX = cursorX*ratio/zoom - posX;
	paramY = cursorY/zoom - posY;
}

// FROM mandelbrot class ... needed for size - coordinate stuff ...


	void ShadersApp::setXPos(float xp)
    {
        if (xp < -12.0)
        {
            xp = -12.0;
        }
        else if (xp > 12.0)
        {
            xp = 12.0;
        }
		xpos = xp;
		posX = xpos ;   // Beep(3000,50); 
		 
		// minX = xp - xstep * (width/2.0f);
		// maxX = xp + xstep * (width/2.0f);
		 
    }
    void ShadersApp::setYPos(float yp)
    {
        if (yp < -12.0)
        {
            yp = -12.0;
        }
        else if (yp > 12.0)
        {
            yp = 12.0;
        }
        ypos = yp;
		posY = ypos ;
		// minY = yp + ystep * (height/2.0f);
		// maxY = yp - ystep * (height/2.0f);
		
    }
	void ShadersApp::setWidth(int W)
	{
		width = max( 4, (min( 16384, W ) + 3) & ~(4 - 1) ) ;    // Ensure multiple of 4 and min 4 ...
		curW = width ;
		reshapeView();
	}
	void ShadersApp::setHeight(int H)
	{
		height = max( 4, min( 16384, H ) ) ;
		curH = height ;
		reshapeView();
	}
	// set new size and make new buffer for ouput
	void ShadersApp::setSize(int nWidth, int nHeight) 
	{
		width = max( 4, (min( 16384, nWidth ) + 3) & ~(4 - 1) ) ;    // Ensure multiple of 4 and min 4 ...
		height = max( 4, min( 16384, nHeight ) ) ;
		curW = width ;
		curH = height ;
		reshapeView();
	}


// end transfer from mandelbrot class 

// FGZ - Timing Helper Functions 


__int64 ShadersApp::Get_PC_Count()
{
	LARGE_INTEGER pc_cnt;
	if( m_PC_Freq == 1000.0 || QueryPerformanceCounter( &pc_cnt ) == FALSE ) 
	{ 
		m_PC_Freq = 1000.0 ;
		return (__int64)GetTickCount();
	}
	return (__int64)pc_cnt.QuadPart ;
}

double ShadersApp::GetTimeSpan(__int64 Start, __int64 Finish)
{
	return double(Finish - Start) / m_PC_Freq ;
}

string ShadersApp::ConvertTimeSpan(double Time)
{
	string mys, myfs ;
	char cbuf[256];
	double pw10 ;
	int pw = max(0, int(log(Time)/log(10.0)) ) , fpw;
	 
	if( m_PC_Freq == 1000.0 ) 
	{
		myfs = "%.3f";
		pw10 = 1000.0;
	}
	else
	{
		fpw = max(4, int(log(m_PC_Freq)/log(10.0)) ) ;
		pw10 = pow(10.0, int(fpw - pw)) ;
		if( pw10 > 1.0 ) 
			sprintf_s(cbuf, 254, "%%.%df", fpw - pw - ((Time < 1.0)? 0 : 1 ) ) ;
		myfs = cbuf ;
	}
	 
	if( pw10 > 1.0 )
		sprintf_s(cbuf,254, myfs.c_str(), double(Round64(Time * pw10))/pw10 );
	else
		sprintf_s(cbuf,254, "%I64u", __int64(double(Round64(Time * pw10))/pw10) );  // this has a slight error at/above 100 billion seconds (or about 3168 years)
	mys = cbuf ; 
	return mys;
}

