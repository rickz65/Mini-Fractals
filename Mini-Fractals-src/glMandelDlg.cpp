/////////////////////////////////////////////////////////////////////////////////////
//
//
//  Mini-Fractals/glMandel Copyright © July 2013 Frederick G Zacharias Covered under the CPOL 
//
//  See License-CPOL.txt for the Full License.
//
//


// BlackMandelDlg.cpp : implementation file
//

#include "stdafx.h"

#include "glMandel.h"
#include "glMandelDlg.h"
#include "afxdialogex.h"
#include "htmlhelp.h"

#include <stdio.h>
#include <stdlib.h>
#include <ostream> 
#include <strstream>
#include <sstream>
#include "afxwin.h"


using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define WM_TOGGLEFULLSCREEN (WM_USER+1234)									// Application Define Message For Toggling

static BOOL g_isProgramLooping;											// Window Creation Loop, For FullScreen/Windowed Toggle																		// Between Fullscreen / Windowed Mode
static BOOL g_createFullScreen;											// If TRUE, Then Create Fullscreen


// this member function taken from project "RichEditLog_Demo" by 'Typke' found @ http://www.codeproject.com ... 
int GetNumVisibleLines(CRichEditCtrl* pCtrl)
{
	CRect rect;
	long nFirstChar, nLastChar;
	long nFirstLine, nLastLine;

	// Get client rect of rich edit control
	pCtrl->GetClientRect(rect);

	// Get character index close to upper left corner
	nFirstChar = pCtrl->CharFromPos(CPoint(0, 0));

	// Get character index close to lower right corner
	nLastChar = pCtrl->CharFromPos(CPoint(rect.right, rect.bottom));
	if (nLastChar < 0)
	{
		nLastChar = pCtrl->GetTextLength();
	}

	// Convert to lines
	nFirstLine = pCtrl->LineFromChar(nFirstChar);
	nLastLine  = pCtrl->LineFromChar(nLastChar);

	return (nLastLine - nFirstLine);
}

// this member function taken from project "RichEditLog_Demo" by 'Typke' found @ http://www.codeproject.com ... 
bool GUI_print(CString str, COLORREF color)
{
	long nVisible = 0;
	long nInsertionPoint = 0;
	CHARFORMAT cf;
	CRichEditCtrl* m_pOutput = &((CBlackMandelDlg*)theApp.m_pMainWnd)->m_CEdit_Output ;
	CSize _OutSize = ((CBlackMandelDlg*)theApp.m_pMainWnd)->m_OutSize ;

	// Initialize character format structure
	cf.cbSize = sizeof(CHARFORMAT);
	cf.dwMask = CFM_COLOR;
	cf.dwEffects = 0; // To disable CFE_AUTOCOLOR

	cf.crTextColor = color;

	// Set insertion point to end of text
	nInsertionPoint = m_pOutput->GetWindowTextLength();
	m_pOutput->SetSel(nInsertionPoint, -1);
	
	// Set the character format
	m_pOutput->SetSelectionCharFormat(cf);

	// FGZ: 6-07-2011 Added: Parse 'str' inserting line breaks at word break near or at end of output box widths ...
	int nBreaks = str.GetLength() / max(1, _OutSize.cx) ;
	int iPos = 0 , nlbPos = 0 , ePos = 0 ;
	CString rfind ;
	for(int ilb=1;  ilb <= nBreaks;  ilb++)
	{
		nlbPos = str.Find( _T("\r\n"), iPos );
		if( nlbPos == -1 ) nlbPos = iPos + _OutSize.cx ;
		if( nlbPos < iPos + _OutSize.cx )
		{
			iPos = nlbPos + 2 ;
		}
		else if( nlbPos > iPos + _OutSize.cx )
		{
			rfind.Empty();
			ePos = min( str.GetLength()-1, iPos + _OutSize.cx ) ;
			rfind = str.Mid( iPos, ePos - iPos );
			ePos = rfind.ReverseFind( ' ' ) + iPos ;
			str.Insert( ePos, _T("\r\n") ) ;
			iPos = 2 + ePos ;
		}
		// if( nlbPos == iPos + m_OutSize.cx ) { ; }
	}
	
	// Replace selection. Because we have nothing 
	// selected, this will simply insert
	// the string at the current caret position.
	m_pOutput->ReplaceSel(str);

	// Get number of currently visible lines or maximum number of visible lines
	// (We must call GetNumVisibleLines() before the first call to LineScroll()!)
	nVisible   = GetNumVisibleLines( m_pOutput );

	// Now this is the fix of CRichEditCtrl's abnormal behaviour when used
	// in an application not based on dialogs. Checking the focus prevents
	// us from scrolling when the CRichEditCtrl does so automatically,
	// even though ES_AUTOxSCROLL style is NOT set.
	if( m_pOutput != m_pOutput->GetFocus() )
	{
		m_pOutput->LineScroll(INT_MAX);
		m_pOutput->LineScroll(1 - nVisible);
	}

	return true;
}

bool str_print(std::string str, UINT color)
{
	CString tmp( str.c_str() );
	return GUI_print(tmp, color) ;
}


// as of Jan. 31, 2010 : this function makes only 32 bit bitmaps, not compatible ones. 
HBITMAP MakeCompatibleBitmap(long biWidth, long biHeight, void** ppBitmapBits, void* pSetBits=NULL, bool top_down=true)
{
	BITMAPINFO bmi;
	BITMAPINFOHEADER bmiHeader;
	 
	bmiHeader.biSize = (DWORD)sizeof(BITMAPINFOHEADER) ; 
	bmiHeader.biWidth = biWidth; 
	bmiHeader.biHeight = biHeight * ((top_down)? -1 : 1); // top-down is negative
	bmiHeader.biPlanes = (WORD)0x0001;  // when compatible, was: gPlanes
	bmiHeader.biBitCount = (WORD)32;    // when compatible, was: gBitsPixel
	bmiHeader.biCompression = BI_RGB; 
	bmiHeader.biSizeImage = 0; 
	bmiHeader.biXPelsPerMeter = 0; 
	bmiHeader.biYPelsPerMeter = 0; 
	bmiHeader.biClrUsed = 0; 
	bmiHeader.biClrImportant = 0; 
	 
	bmi.bmiHeader = bmiHeader;
	// bmi.bmiColors ??? 
	int sizeBytes = biWidth * abs(biHeight) * 4 ;  // when compatible, was: (gBitsPixel/8)
	
	CDC dcMem;
	// 10-10-2010 MS-DOCS SAY:
	// BOOL CreateCompatibleDC( CDC* pDC );
	// ... If pDC is NULL, the function creates a memory device context that is compatible with the system display.
	// FGZ: ... ooooh! ... well, umm, if true, this will make for simpler code ... why didn't I pick up on this LONG ago ?
	dcMem.CreateCompatibleDC( NULL ); 
	 
	// alternate way to do above:
	// HDC hDC = GetDC( NULL );  // handle to entire system display, (desktop ?)
	// ... usage ...
	// ReleaseDC( NULL, hDC );  // clean up when finished
	
	HBITMAP hTmp = CreateDIBSection( dcMem.GetSafeHdc(), &bmi, DIB_RGB_COLORS, ppBitmapBits, NULL, 0 ); 
	
	if( AfxIsValidAddress( pSetBits, sizeBytes, TRUE) )
	{
		CBitmap cBM;
		cBM.Attach( hTmp );
		cBM.SetBitmapBits( sizeBytes, pSetBits );
		GdiFlush();
		cBM.Detach(); 
	} 
	// dcMem.DeleteDC(); // not needed, destructor will handle this
	return hTmp;
	 
}

UINT DoEvents()  // do not call this if a CColorDialog is running
{
   MSG msg;

	// Process existing messages in the application's message queue.
	// When the queue is empty, do clean up and return.
	while (::PeekMessage(&msg,NULL,0,0,PM_NOREMOVE) )
	{
		if (msg.message == WM_QUIT) return 1;
		 
		if (!AfxGetThread()->PumpMessage())
			return 0;
	}
	return 2; // this will never hit
}

UINT MsgSleep(UINT ms)
{
	/*TotalGenocideOnCColorDialogs = true ;
	Sleep(min(ms,15)); 
	if( CP_DoEvents() == 1 ) return 1;
	TotalGenocideOnCColorDialogs = false ;
	if( ms > 15 ) {
		ms -= 15;
		clock_t Start = clock();
		while( Start + (clock_t)ms > clock() ) {
			Sleep(15);
			if( CP_DoEvents() == 1 ) return 1;
		}
	}
	return 0;*/
	 
	UINT rtv, dms = ms;
	DWORD Start = GetTickCount();
	do 
	{
		Sleep(min(dms,15));
		rtv = DoEvents();
		if( rtv == 1 || dms < 15 ) break;
		dms -= 15;
	} while( Start + (DWORD)ms > GetTickCount() );
	return rtv;
}


//////////////////////////////
//////////////////////////////
//   from NeHeGL.cpp  :


void CBlackMandelDlg::TerminateApplication (GL_Window* window)							// Terminate The Application
{
	PostMessage(WM_QUIT, 0, 0);			// window->hWnd, 				// Send A WM_QUIT Message
	g_isProgramLooping = FALSE;											// Stop Looping Of The Program
}

void CBlackMandelDlg::ToggleFullscreen (GL_Window* window)								// Toggle Fullscreen/Windowed
{
	PostMessage (WM_TOGGLEFULLSCREEN, 0, 0);			// window->hWnd,		// Send A WM_TOGGLEFULLSCREEN Message
}

void CBlackMandelDlg::ReshapeGL (int width, int height)									// Reshape The Window When It's Moved Or Resized
{
	glViewport (0, 0, (GLsizei)(width), (GLsizei)(height));				// Reset The Current Viewport
	glMatrixMode (GL_PROJECTION);										// Select The Projection Matrix
	glLoadIdentity ();													// Reset The Projection Matrix
	gluPerspective (45.0f, (GLfloat)(width)/(GLfloat)(height),			// Calculate The Aspect Ratio Of The Window
					1.0f, 1000.0f);		
	glMatrixMode (GL_MODELVIEW);										// Select The Modelview Matrix
	glLoadIdentity ();													// Reset The Modelview Matrix
}

BOOL CBlackMandelDlg::ChangeScreenResolution (int width, int height, int bitsPerPixel)	// Change The Screen Resolution
{
	DEVMODE dmScreenSettings;											// Device Mode
	ZeroMemory (&dmScreenSettings, sizeof (DEVMODE));					// Make Sure Memory Is Cleared
	dmScreenSettings.dmSize				= sizeof (DEVMODE);				// Size Of The Devmode Structure
	dmScreenSettings.dmPelsWidth		= width;						// Select Screen Width
	dmScreenSettings.dmPelsHeight		= height;						// Select Screen Height
	dmScreenSettings.dmBitsPerPel		= bitsPerPixel;					// Select Bits Per Pixel
	dmScreenSettings.dmFields			= DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;
	if (ChangeDisplaySettings (&dmScreenSettings, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL)
	{
		return FALSE;													// Display Change Failed, Return False
	}
	return TRUE;														// Display Change Was Successful, Return True
}


// code from opengl.org ... 
// Aleksandar Dimitrijevic,  Mar 28, 2010, 5:39 AM  
//  https://sites.google.com/site/opengltutorialsbyaks/introduction-to-opengl-3-2---tutorial-01  
//  need to use the newer contexts for the older machines that don't do the ancient context ... ancient context worked on ati 7660D, but not ati 5450 ...
class CGLRenderer
{
public:
        //CGLRenderer(void);
        //virtual ~CGLRenderer(void);
        bool CreateGLContext(CDC* pDC, GL_Window* window);         // Creates OpenGL Rendering Context
        //void PrepareScene(CDC* pDC);            // Scene preparation stuff
        //void Reshape(CDC* pDC, int w, int h);   // Changing viewport
        //void DrawScene(CDC* pDC);               // Draws the scene
        //void DestroyScene(CDC* pDC);            // Cleanup
 
protected:
        //void SetData();                         // Creates VAO and VBOs and fill them with data
 
protected:
        HGLRC    m_hrc;                        // OpenGL Rendering Context 
        //CGLProgram* m_pProgram;                // Program
        //CGLShader* m_pVertSh;                  // Vertex shader
        //CGLShader* m_pFragSh;                  // Fragment shader
 
        GLuint m_vaoID[2];                      // two vertex array objects, one for each drawn object
        GLuint m_vboID[3];                      // three VBOs
};

bool CGLRenderer::CreateGLContext(CDC* pDC, GL_Window* window)
{
        PIXELFORMATDESCRIPTOR pfd;
        memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));
        pfd.nSize  = sizeof(PIXELFORMATDESCRIPTOR);
        pfd.nVersion   = 1;
        pfd.dwFlags    = PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW;
        pfd.iPixelType = PFD_TYPE_RGBA;
        pfd.cColorBits = 32;
        pfd.cDepthBits = 32;
        pfd.iLayerType = PFD_MAIN_PLANE;
 
        int nPixelFormat = ChoosePixelFormat(pDC->m_hDC, &pfd);
 
        if (nPixelFormat == 0) return false;
 
        BOOL bResult = SetPixelFormat (pDC->m_hDC, nPixelFormat, &pfd);
 
        if (!bResult) return false; 
 
        HGLRC tempContext = wglCreateContext(pDC->m_hDC);
        wglMakeCurrent(pDC->m_hDC, tempContext);
 
        GLenum err = glewInit();
        if (GLEW_OK != err)
        {
                AfxMessageBox(_T("GLEW is not initialized!"));
        }
 
        int attribs[] =
        {
                WGL_CONTEXT_MAJOR_VERSION_ARB, 3,
                WGL_CONTEXT_MINOR_VERSION_ARB, 1,
                WGL_CONTEXT_FLAGS_ARB, 0,
                0
        };
 
        if(wglewIsSupported("WGL_ARB_create_context") == 1)
        {
                m_hrc = wglCreateContextAttribsARB(pDC->m_hDC,0, attribs);
				window->hRC = m_hrc ;
                if( !wglMakeCurrent(NULL,NULL) ) return false;
                if( !wglDeleteContext(tempContext) ) return false;
                if( !wglMakeCurrent(pDC->m_hDC, m_hrc) ) return false;
				cout << " YES! we have new NEW OpenGL Contexts !" << endl; 
        }
        else
        {       //It's not possible to make a GL 3.x context. Use the old style context (GL 2.1 and before)
                m_hrc = tempContext;
				cout << "It's not possible to make a GL 3.x context." << endl << "reverting to old GL Context (GL 2.1 and before) ..." << endl ;
        }
 
        //Checking GL version
        const GLubyte *GLVersionString = glGetString(GL_VERSION);
 
        //Or better yet, use the GL3 way to get the version number
        int OpenGLVersion[2];
        glGetIntegerv(GL_MAJOR_VERSION, &OpenGLVersion[0]);
        glGetIntegerv(GL_MINOR_VERSION, &OpenGLVersion[1]);
 
        if (!m_hrc) return false;
 
        return true;
}


BOOL CBlackMandelDlg::SetupWindowGL (GL_Window* window)									// This Code Creates Our OpenGL Window
{
	
	/* 
	DWORD windowStyle = m_diag_output.GetStyle() ; // = WS_OVERLAPPEDWINDOW;							// Define Our Window Style
	DWORD windowExtendedStyle = m_diag_output.GetExStyle() ; // = WS_EX_APPWINDOW;						// Define The Window's Extended Style

	PIXELFORMATDESCRIPTOR pfd =											// pfd Tells Windows How We Want Things To Be
	{
		sizeof (PIXELFORMATDESCRIPTOR),									// Size Of This Pixel Format Descriptor
		1,																// Version Number
		PFD_DRAW_TO_WINDOW |											// Format Must Support Window
		PFD_SUPPORT_OPENGL |											// Format Must Support OpenGL
		PFD_DOUBLEBUFFER,												// Must Support Double Buffering
		PFD_TYPE_RGBA,													// Request An RGBA Format
		window->init.bitsPerPixel,										// Select Our Color Depth
		0, 0, 0, 0, 0, 0,												// Color Bits Ignored
		0,																// No Alpha Buffer
		0,																// Shift Bit Ignored
		0,																// No Accumulation Buffer
		0, 0, 0, 0,														// Accumulation Bits Ignored
		16,																// 16Bit Z-Buffer (Depth Buffer)  
		0,																// No Stencil Buffer
		0,																// No Auxiliary Buffer
		PFD_MAIN_PLANE,													// Main Drawing Layer
		0,																// Reserved
		0, 0, 0															// Layer Masks Ignored
	};

	RECT windowRect;  // {0, 0, window->init.width, window->init.height};	// Define Our Window Coordinates
	m_diag_output.GetWindowRect( &windowRect );

	GLuint PixelFormat;													// Will Hold The Selected Pixel Format
	// */

	/* 
	if (window->init.isFullScreen == TRUE)								// Fullscreen Requested, Try Changing Video Modes
	{
		if (ChangeScreenResolution (window->init.width, window->init.height, window->init.bitsPerPixel) == FALSE)
		{
			// Fullscreen Mode Failed.  Run In Windowed Mode Instead
			MessageBox(_T("Mode Switch Failed.\nRunning In Windowed Mode."), _T("Error"), MB_OK | MB_ICONEXCLAMATION);   // HWND_DESKTOP, 
			window->init.isFullScreen = FALSE;							// Set isFullscreen To False (Windowed Mode)
		}
		else															// Otherwise (If Fullscreen Mode Was Successful)
		{
			ShowCursor (FALSE);											// Turn Off The Cursor
			windowStyle = WS_POPUP;										// Set The WindowStyle To WS_POPUP (Popup Window)
			windowExtendedStyle |= WS_EX_TOPMOST;						// Set The Extended Window Style To WS_EX_TOPMOST
		}																// (Top Window Covering Everything Else)
	}
	else																// If Fullscreen Was Not Selected
	{
		// Adjust Window Rect, Account For Window Borders
		AdjustWindowRectEx( &windowRect, windowStyle, 0, windowExtendedStyle);
	}
	// */
	 
	// Create The OpenGL Window
	/* 	
	window->hWnd = CreateWindowEx (windowExtendedStyle,					// Extended Style
								   window->init.application->className,	// Class Name
								   window->init.title,					// Window Title
								   windowStyle,							// Window Style
								   0, 0,								// Window X,Y Position
								   windowRect.right - windowRect.left,	// Window Width
								   windowRect.bottom - windowRect.top,	// Window Height
								   HWND_DESKTOP,						// Desktop Is Window's Parent
								   0,									// No Menu
								   window->init.application->hInstance, // Pass The Window Instance
								   window);
	// */
	
	// FGZ - Setup the members of 'window' ...
	 
	// m_diag_output.m_Picture.SetWindowPos( NULL, windowRect.left, windowRect.top, abs(windowRect.right-windowRect.left), abs(windowRect.bottom-windowRect.top), 
	//											SWP_NOREPOSITION | SWP_NOZORDER | SWP_NOMOVE );
	 
	// window->init.application->hInstance = ;
	
	// Get The Window Context
	// GL_Window* window = (GL_Window*)(GetWindowLong (hWnd, GWL_USERDATA));  // this line blows up app.
	
	window->hWnd = m_diag_output.m_Picture.GetSafeHwnd() ;   // FGZ -- this ties the OpenCL MFC 'base' app into the NeHeGL stuff ...
	
	if (window->hWnd == 0)												// Was Window Creation A Success?
	{
		return FALSE;													// If Not Return False
	}

	window->hDC = ::GetDC (window->hWnd);									// Grab A Device Context For This Window  FGZ--> may not be an issue to hold onto this, if it's a common DC ...
	if (window->hDC == 0)												// Did We Get A Device Context?
	{
		// Failed
		//::DestroyWindow (window->hWnd);									// Destroy The Window
		window->hWnd = 0;												// Zero The Window Handle
		return FALSE;													// Return False
	}

	/*  
	PixelFormat = ChoosePixelFormat (window->hDC, &pfd);				// Find A Compatible Pixel Format
	if (PixelFormat == 0)												// Did We Find A Compatible Format?
	{
		// Failed
		::ReleaseDC (window->hWnd, window->hDC);							// Release Our Device Context
		window->hDC = 0;												// Zero The Device Context
		//::DestroyWindow (window->hWnd);									// Destroy The Window
		window->hWnd = 0;												// Zero The Window Handle
		return FALSE;													// Return False
	}

	if (SetPixelFormat (window->hDC, PixelFormat, &pfd) == FALSE)		// Try To Set The Pixel Format
	{
		// Failed
		::ReleaseDC (window->hWnd, window->hDC);							// Release Our Device Context
		window->hDC = 0;												// Zero The Device Context
		//::DestroyWindow (window->hWnd);									// Destroy The Window
		window->hWnd = 0;												// Zero The Window Handle
		return FALSE;													// Return False
	}
	// */ 

	CGLRenderer* widget = new CGLRenderer ;
	CDC myDC;
	if( !myDC.Attach(window->hDC) )  { 
		MessageBox(_T("\n CDC Attach failed in SetupWindowGL() \n"), _T("debug"), MB_OK) ; 
		// Failed
		::ReleaseDC (window->hWnd, window->hDC);							// Release Our Device Context
		window->hDC = 0;												// Zero The Device Context
		//::DestroyWindow (window->hWnd);									// Destroy The Window
		window->hWnd = 0;
		myDC.Detach();
		delete widget ;// Zero The Window Handle
		return FALSE;													// Return False
	}
	 
	if( !widget->CreateGLContext( &myDC, window ) )
	// window->hRC = wglCreateContext (window->hDC);						// Try To Get A Rendering Context
	// if (window->hRC == 0)												// Did We Get A Rendering Context?
	{
		// Failed
		::ReleaseDC (window->hWnd, window->hDC);							// Release Our Device Context
		window->hDC = 0;												// Zero The Device Context
		//::DestroyWindow (window->hWnd);									// Destroy The Window
		window->hWnd = 0;
		myDC.Detach();
		delete widget ;// Zero The Window Handle
		cout << "CreateGLContext FAILED !!" << endl ;
		return FALSE;													// Return False
	}

	myDC.Detach();
	delete widget ;
	
	/* 
	// Make The Rendering Context Our Current Rendering Context
	if (wglMakeCurrent (window->hDC, window->hRC) == FALSE)
	{
		// Failed
		wglDeleteContext (window->hRC);									// Delete The Rendering Context
		window->hRC = 0;												// Zero The Rendering Context
		::ReleaseDC (window->hWnd, window->hDC);							// Release Our Device Context
		window->hDC = 0;												// Zero The Device Context
		//::DestroyWindow (window->hWnd);									// Destroy The Window
		window->hWnd = 0;												// Zero The Window Handle
		return FALSE;													// Return False
	}
	*/

	//if( m_shader != NULL ) {
		// m_shader->m_hWnd = window->hWnd ;    
		// m_shader->m_pwindow = window ;       
	//}
	
	ReshapeGL (window->init.width, window->init.height);				// Reshape Our GL Window
	
	window->lastTickCount = GetTickCount ();							// Get Tick Count

	return TRUE;														// Window Creating Was A Success
																		
}


BOOL CBlackMandelDlg::DestroyWindowGL (GL_Window* window)								// Destroy The OpenGL Window & Release Resources
{
	if (window->hWnd != 0)												// Does The Window Have A Handle?
	{	
		if (window->hDC != 0)											// Does The Window Have A Device Context?
		{
			wglMakeCurrent (window->hDC, 0);							// Set The Current Active Rendering Context To Zero
			if (window->hRC != 0)										// Does The Window Have A Rendering Context?
			{
				wglDeleteContext (window->hRC);							// Release The Rendering Context
				window->hRC = 0;										// Zero The Rendering Context

			}
			::ReleaseDC (window->hWnd, window->hDC);  // FGZ : isn't this a little too long to hold onto the hDC ??						// Release The Device Context
			window->hDC = 0;											// Zero The Device Context 
			// if( !wglMakeCurrent(NULL,NULL) ) return FALSE ;  // didn't help ... is it needed ?
		}
		window->hWnd = 0;												// Zero The Window Handle
	}

	if (window->init.isFullScreen)										// Is Window In Fullscreen Mode
	{
		ChangeDisplaySettings (NULL,0);									// Switch Back To Desktop Resolution
		ShowCursor (TRUE);												// Show The Cursor
	}	
	return TRUE;														// Return True
}



///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
public:
//	afx_msg void OnHelpAbout();
	virtual BOOL OnInitDialog();
	CStatic m_logo;
	afx_msg void OnBnClickedZaclabs();
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

BOOL CAboutDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	
	m_logo.Create(NULL, SS_ICON|WS_CHILD|WS_VISIBLE, CRect(12,12,108,108), this, IDC_ABOUT_LOGO);
	HICON hIcon = (HICON)LoadImageA(AfxGetApp()->m_hInstance, MAKEINTRESOURCE( IDI_ICON1 ), 
									IMAGE_ICON, 64, 64, LR_VGACOLOR);
	if( hIcon == NULL ) MessageBox(_T("\n Load Image(icon) failed \n"), _T("debug"));
	m_logo.SetIcon( hIcon );  // IDR_MAINFRAME  
	m_logo.ShowWindow( SW_SHOW );
	
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ABOUT_LOGO, m_logo);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
	ON_BN_CLICKED(IDC_ZACLABS, &CAboutDlg::OnBnClickedZaclabs)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////////////////
// CBlackMandelDlg dialog

CBlackMandelDlg::CBlackMandelDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CBlackMandelDlg::IDD, pParent)
	, m_iterations(100)
	, m_pow_f(4) 
	, m_diag_output(this)
	, m_bStretch(FALSE)
	, OrbitFunc(0)   // why does the following equation only sometimes give compile errors??? Mandelbrot equ did too.
	, m_str_User_Function(_T("Complex tmpD = 1 + floatG * ZZ ;\r\nZZ = sin[ZZ]/(CC+tmpD*ZZ) ;")) // (_T("ZZ = ZZ * ZZ + CC ;")) <-- no better behav.    // ZZ = ADD( MUL(ZZ, ZZ), DIV( CC, ADD(_Type1_(1.0), MUL(floatG, ZZ) ))) ; 
	, m_UseEsmooth(FALSE)
	, m_float_G(0.175f)
	, m_bailout(16.0f)
	, m_bJulia(FALSE)
	, m_bTrace(FALSE)
	, m_bSpanG(FALSE)
	, m_fSpanG(4.0)
	, m_iSpanSpeed(100)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_shader = NULL ;
	m_oldCoutStreamBuf = NULL ;

}

/*
CBlackMandelDlg::~CBlackMandelDlg()
{
	KillTimer( 4001 );
	MessageBox(_T("\n GOODBYE ! from destructor \n"),_T("BYE !"),MB_OK);  // this never fires ...
	cout.rdbuf( m_oldCoutStreamBuf ) ;
}
*/

void CBlackMandelDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_Iterations, m_iterations);
	DDV_MinMaxUInt(pDX, m_iterations, 1, 65535);
	DDX_Text(pDX, IDC_EDIT_F, m_pow_f);
	DDV_MinMaxUInt(pDX, m_pow_f, 2, 9);
	DDX_Control(pDX, IDC_EDIT_Output, m_CEdit_Output);
	DDX_Check(pDX, IDC_CHECK_STRETCH, m_bStretch);
	DDX_Control(pDX, IDC_EDIT_F, m_CEdit_F);
	DDX_Control(pDX, IDC_SPIN1, m_CSBC_pow_f);
	DDX_Radio(pDX, IDC_RADIO1, OrbitFunc);
	DDX_Control(pDX, IDC_RADIO1, m_CZTOF);
	DDX_Control(pDX, IDC_RADIO2, m_CSINZ);
	DDX_Control(pDX, IDC_RADIO3, m_CCOSZ);
	DDX_Control(pDX, IDC_RADIO4, m_CUSER);
	DDX_Text(pDX, IDC_EDIT3, m_str_User_Function);
	DDX_Check(pDX, IDC_CHECK2, m_UseEsmooth);
	DDX_Text(pDX, IDC_EDIT1, m_float_G);
	DDX_Text(pDX, IDC_EDIT2, m_bailout);
	DDX_Check(pDX, IDC_CHECK4, m_bJulia);
	DDX_Check(pDX, IDC_CHECK3, m_bTrace);
	DDX_Check(pDX, IDC_CHECK5, m_bSpanG);
	DDX_Text(pDX, IDC_EDIT4, m_fSpanG);
	DDX_Slider(pDX, IDC_SLIDER1, m_iSpanSpeed);
	DDV_MinMaxInt(pDX, m_iSpanSpeed, 10, 1024);
	DDX_Control(pDX, IDC_SLIDER1, m_CSpanSpeed);
}

BEGIN_MESSAGE_MAP(CBlackMandelDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_MAKE, &CBlackMandelDlg::OnBnClickedButtonMake)
	ON_BN_CLICKED(IDC_BUTTON_ZOOM, &CBlackMandelDlg::OnBnClickedButtonZoom)
	ON_BN_CLICKED(IDC_BUTTON_TIME, &CBlackMandelDlg::OnBnClickedButtonTime)
	ON_EN_CHANGE(IDC_EDIT_Iterations, &CBlackMandelDlg::OnChangeEditIterations)
	ON_WM_TIMER()
	ON_WM_DESTROY()
	ON_EN_KILLFOCUS(IDC_EDIT_Iterations, &CBlackMandelDlg::OnEnKillfocusEditIterations)
	ON_MESSAGE(WM_FGZ_UPDATE_PICTURE_SIZE, &CBlackMandelDlg::OnFgzUpdatePictureSize)
	ON_COMMAND(ID_FILE_EXIT, &CBlackMandelDlg::OnFileExit)
	ON_COMMAND(ID_OPTIONS_MAKEFRACTAL, &CBlackMandelDlg::OnOptionsMakefractal)
	ON_COMMAND(ID_HELP_ABOUT, &CBlackMandelDlg::OnHelpAbout)
	ON_COMMAND(ID_HELP_HELP, &CBlackMandelDlg::OnHelpHelp)
	ON_COMMAND(ID_HELP_GLSLHELP, &CBlackMandelDlg::OnHelpGlslhelp)
END_MESSAGE_MAP()


// this is as close to a destructor as this window gets.
void CBlackMandelDlg::OnDestroy()
{
	CDialogEx::OnDestroy();
	
	DestroyWindowGL (&m_window);
	 
	m_shader->destroyWindow();
    // Sleep(2500);
	
	KillTimer( 4001 );

	//////////////////////////////////////////////////
	// return cout to normal ...
	cout.rdbuf( m_oldCoutStreamBuf ) ;

}


// CBlackMandelDlg message handlers

BOOL CBlackMandelDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	
	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// EnableHtmlHelp();
	
	// TODO: Add extra initialization here
	m_CSBC_pow_f.SetRange(2, 9);
	m_CSpanSpeed.SetRange(10, 1024, TRUE);  
	
	////////////////////////////////////////////////////////////////////////////
	// redirect cout output/buffer to our buffer ...
	m_oldCoutStreamBuf = cout.rdbuf();
	cout.rdbuf( m_strCout.rdbuf() );
	
	SetTimer( 4101, 57, NULL );
	SetTimer( 4001, 67, NULL );  

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CBlackMandelDlg::SetupMandel(int _Width, int _Height, bool DoInit, bool ShowMsgs)
{
	if( DoInit ) { 
		// from mandel object that was called here ...
		// SETUP:    ShadersApp&			  m_shader ; 
		if( m_shader != NULL ) delete m_shader ;
		m_shader = new ShadersApp(_Width, _Height) ;
		
		m_shader->m_pwindow = &m_window ;
		
		m_shader->setXSize( 4.0f ); 
	}
	else {
		m_shader->setWidth( _Width ) ;
		m_shader->setHeight( _Height ) ;
		ReshapeGL (_Width, _Height);
	}

	m_shader->m_str_User_Function = m_str_User_Function;
	 
	m_isInitOK = m_shader->setup(ShowMsgs);   // this then calls shader::initGL() ... where user-equation gets inserted.
	
	if( !m_isInitOK && ShowMsgs )  {
		cout << "\n Mandelbrot Object failed Setup \n" << endl ; 
		return;
	}
	if( ShowMsgs ) 
	cout << "Mandelbrot Object Successfully Setup!" << endl ;
}

BOOL CBlackMandelDlg::DoNewOutput() 
{
 	if( m_diag_output.ShowWindow(SW_SHOW) ) return TRUE ;
	return FALSE ;
}

void CBlackMandelDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CBlackMandelDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CBlackMandelDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


/////////////////////////////////////////////////////
/////    FGZ translator 
/////
/////  Purpose is to translate algebraic notation into GLSL Complex functions, which can then be called by shaders.
/////

// from Ingo Berg's MU parser ...
/*
  DefineNameChars("0123456789_abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ");
  DefineOprtChars("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ+-*^/?<>=#!$%&|~'_");
  DefineInfixOprtChars("/+-*^?<>=#!$%&|~'_");
*/

// to DO : maybe define a delminting character array also ...

static const char* c_DefaultOprt[] = 
	{ 
	  "*", "/", "+", "-", "<=", ">=", "!=", "==", "<", ">",    // translate operators, in order of precedence
	  "(", ")", "=", ";",  " ",  0     // delimiting operators
	};
static int numOps = 14 ;
static int numTrans = 4 ;

int CBlackMandelDlg::getNextOperatorPos(CString src, int start)  
{
	int posOp = src.GetLength() -1  ,   tmp ;
	bool  bFound = false ;
	 
	for(int ii=0; ii < numOps; ii++) 
	{
		tmp = src.Find( c_DefaultOprt[ii], start ) ;
		if( tmp != -1 && ii == 14 ) {           // right now this isn't active, it may not be a good idea, either.
			while( src.GetAt(tmp++) == ' ' );   // jump over white space ...
		}
		if( tmp != -1 ) bFound = true ;
		if( posOp > tmp && tmp != -1) posOp = tmp ;
	}
	return ((!bFound)? -1 : posOp );
}

int CBlackMandelDlg::getLastOperatorPos(CString src, int start)
{
	int posOp = 0  ,   tmp=start-1 , lenOp ;
	bool  bFound = false ;
	 
	for(int ii=0; ii < numOps; ii++) 
	{
		lenOp = strnlen_s(c_DefaultOprt[ii], 3) ;

		for(int kk=start; kk >= 0 ;  kk--) {
			if( src.Mid(kk, lenOp) == c_DefaultOprt[ii] ) {
				tmp = kk ;
				break;
			}
			else tmp = -1 ;
		}
		if( tmp != -1 && ii == 14 ) {
			while( src.GetAt(++tmp) == ' ' );
			tmp--;
		}
		if( tmp != -1 ) bFound = true ;
		if( posOp < tmp && tmp != -1) posOp = tmp ;
	}
	return posOp ;
}

int CBlackMandelDlg::getNextTokenPos(CString src, int start, bool bCheckBegining)
{
	int tmp = start;
	if( bCheckBegining ) tmp--;
	while( src.GetAt(++tmp) == ' ' );
	if( tmp >= src.GetLength() ) tmp = src.GetLength() -1 ;
	return tmp;
}

int CBlackMandelDlg::getLastTokenPos(CString src, int start, bool bCheckEnd)  
{
	int tmp = start;
	if( tmp >= src.GetLength() ) tmp = src.GetLength() ;
	if( bCheckEnd ) tmp++ ;
	while( src.GetAt(--tmp) == ' ' );
	if( tmp <= 0 ) tmp = 0 ;
	return tmp;
}

CString CBlackMandelDlg::digestOperators(CString str_User_Function, bool bCheckToken )  
{
	static int docnt=0;
	
	CString sB, junk ; // = m_str_User_Function ;
	char insert[1024] ;
	size_t  posA=0, posB=0, posOp = 0, lenOp, copyA=0, repA, repB ;
	int adjust, mycnt = ++docnt ;
	bool bIncCopyA = false ;
	
	sB = str_User_Function ;

	for(int ii=0; ii < numTrans; ii++) 
	{
		if( m_bTrace ) {
			junk.Format("begin DO %d loop %d \r\n%s<--sB", mycnt, ii, sB.GetString() );
			cout << endl << junk.GetString() << endl; 
		}
		
		posA=0;  posB=0;  posOp = 0 ;
		lenOp = strnlen_s(c_DefaultOprt[ii], 3) ;
		copyA = 0; 
		bIncCopyA = false;

		while( (int)posOp < sB.GetLength() )
		{
			posOp = sB.Find( c_DefaultOprt[ii], posOp+1 ) ;
			// if not found break 
			if( posOp == -1 ) { 
				if( m_bTrace ) {
					junk.Format("No more %s operators to digest in this loop", c_DefaultOprt[ii] );
					cout << junk.GetString() << endl ;
				}
				break; 
			}

			// find chunk before operator ...
			posA = getLastOperatorPos( sB, posOp-1 );
			if( m_bTrace ) {
				cout << "found posA last oper: " << sB.GetAt(posA) << endl ;
			}
			posA = getNextTokenPos( sB, posA, ((posA == 0)? true : bCheckToken) );
			repA = posA ;
			
			posB = getNextOperatorPos( sB, posOp + lenOp ) ;
			if( posB == -1 ) {
				posB = sB.GetLength() -1;
				repB = posB ;
			}
			else 
				posB = getLastTokenPos( sB, posB, bCheckToken ) ;
			repB = posB ;
			
			if( m_bTrace ) {
				junk.Format("<-- first string DO: %d loop %d", docnt, ii);
				cout << endl << sB.Mid( copyA, posA-copyA ).GetString() << junk.GetString() << endl ;
			}

			copyA = posB + 1 ;
			
			switch ( ii ) 
			{
			case 0:  // "*" 
				sprintf_s(insert, 1022, "MUL[%s,%s]", sB.Mid(posA , posOp-posA ), sB.Mid( posOp + lenOp, posB - (posOp) )); 
				break;
			case 1:  //  "/"
				sprintf_s(insert, 1022, "DIV[%s,%s]", sB.Mid(posA , posOp-posA ), sB.Mid( posOp + lenOp, posB - (posOp) )); 
				break;
			case 2:    // "+"  
				sprintf_s(insert, 1022, "ADD[%s,%s]", sB.Mid(posA , posOp-posA ), sB.Mid( posOp + lenOp, posB - (posOp) )); 
				break;
			case 3 :   //  "-"
				sprintf_s(insert, 1022, "SUB[%s,%s]", sB.Mid(posA , posOp-posA ), sB.Mid( posOp + lenOp, posB - (posOp) )); 
				break;
			default:
				;
				break;
			}  // end switch 
		 
			if( m_bTrace ) {
				junk.Format("sB.Mid(repA, repB-repA-1) has repB = %d  repA = %d  sB len = %d", repB, repA, sB.GetLength() );
				cout << junk.GetString() << endl ;
				cout << sB.Mid(repA, ((repB==sB.GetLength() -1)? sB.GetLength()-repA : repB-repA+1) ).GetString() << "<-- string to be replaced" << endl ;
			}
			if( 0 == sB.Replace( sB.Mid(repA, ((repB==sB.GetLength() -1)? sB.GetLength()-repA : repB-repA+1)), insert ) )   // allow the new insert to be picked up in future passes of above switch
			{
				cout<< "CString Replace Failed !!!" << endl ;
			}
			adjust = strlen((const char*)insert) - (repB-repA) ;  // adjust for char pos change ...
			posOp += adjust ;
			copyA += adjust +1 ;  // these loop inside this while ...
			bIncCopyA = true ;
			if( m_bTrace ) {
				junk.Format("<-- insert string DO: %d loop %d", docnt, ii);
				cout << endl << insert << junk.GetString() << endl ;
				junk.Format("<-- after replace DO: %d loop %d", docnt, ii);
				cout << endl << sB.GetString() << junk.GetString() << endl ;
			} 
		}  // end while
		
		if( m_bTrace ) {
			cout << "exit while loop inside single operator digest loop" << endl ;
		}
		 
		if( bIncCopyA ) {
			if( (int)copyA < sB.GetLength()-1 ) {
				copyA += 1;
				if( m_bTrace ) {
					cout << "Hit Increment copyA" << endl ;
				}
			}
		}
		 
		// copy posB to end of string ... 
		if( (int)posB < sB.GetLength() -1 ) {
			if( m_bTrace ) {
				junk.Format("<-- end string DO: %d loop %d", docnt, ii);
				cout << sB.Mid( copyA, sB.GetLength()-copyA ).GetString() << junk.GetString() << endl << endl;
			}
		} else if( m_bTrace ) {
				cout << "NO end string" << junk.GetString() << endl << endl;
		}

	}  // end for ...
	
	return sB; 
}

CString CBlackMandelDlg::DigestParen(CString snipit, bool bCheckToken)  
{
	static int dpcnt=0;
	 
	CString chunk, junk;
	int iFirst, nCount, tmp, tmp2, mycnt = ++dpcnt ;
	bool repeat = false ;
	
	if( m_bTrace ) {
		junk.Format("enter DP %d snipit==%s", mycnt, snipit.GetString() );
		cout << endl << junk.GetString() << endl ; 
	}
	// if found paren ... 
	iFirst = snipit.Find( c_DefaultOprt[10], 0 );
	if( iFirst != -1 )
	{
		// now find it's matching closing paren ...
		tmp = snipit.Find( c_DefaultOprt[11], iFirst ) ;
		if( tmp != -1 ) {
			tmp2 = snipit.Find( c_DefaultOprt[10], iFirst + 1 ) ;
			if( tmp2 != -1 && tmp > tmp2 ) 
			{
				// there's an open paren in between ... consider:  a * ( b - ( c + d ) / ( e - f ) ) 
				nCount = snipit.GetLength() - iFirst ;
				iFirst++ ;
				 
				if( m_bTrace ) {
					junk.Format("found open paren ! calling DP %d sending==%s", mycnt, snipit.Mid( iFirst , nCount ).GetString() );
					cout << endl << junk.GetString() << endl ; 
				}
				chunk = DigestParen( snipit.Mid( iFirst , nCount ), false ) ;  // +1  is to DROP OUT the leading paren (, nCount cuts out the trailing paren )
				repeat = true ;
			}
			else
			{
				nCount = tmp - (iFirst ) ;
				int iiFirst = getNextTokenPos( snipit, iFirst, false );
				tmp = getLastTokenPos( snipit, tmp, false );
				
				if( m_bTrace ) {
					junk.Format("NO open paren: calling DP %d sending==%s", mycnt, snipit.Mid( iiFirst, tmp - iiFirst +1 ).GetString() );
					cout << endl << junk.GetString() << endl ; 
				}
				chunk = DigestParen( snipit.Mid( iiFirst, tmp - iiFirst +1 ), false ) ;  // digest without the outer parens   
			}
		}
		else //  ERROR !! NO MATCHING )
		{	cout << "USER ERROR -- ( found with no matching )" << endl << snipit.GetString() << endl;  return snipit;   }
		 
		// now replace ... with 'chunk' 
		snipit.Replace( snipit.Mid(iFirst, nCount +1 ), chunk );  // this removes the parens ...
		chunk = snipit ;
		if( m_bTrace ) {
			junk.Format("<-- after replace DP: %d", mycnt); 
			cout << endl << chunk.GetString() << junk.GetString() << endl ;
		}
		if( repeat ) chunk = DigestParen( snipit, false ) ;
	}
	else 
		chunk = digestOperators( snipit, bCheckToken ) ;  // maybe bool is conditional ??
	
	if( m_bTrace ) {
		junk.Format("leave DP %d chunk==%s", mycnt, chunk.GetString() );
		cout << endl << junk.GetString() << endl ; 
	}
	return chunk ;
}

bool CBlackMandelDlg::TranslateUserFunc() 
{
	// this translator might have been much easier written with CString::Tokenize ... but this way is more portable (i think).
	
	// disgest everything inside parenthesis (...) first !
	// first locate all the parens, in correct matching pairs, so that we can process innermost to outermost in pairs ... 
	// now that we're done with parens (...) , we can do one last digestion ...

	m_str_User_Function = digestOperators( DigestParen( m_str_User_Function, false ), false ) ;
	m_str_User_Function.Replace( '[' , '(' );
	m_str_User_Function.Replace( ']' , ')' );  // this is a cheap solution ... excludes use of [] for arrays and such ...
	 
	return true ;
}

 
void CBlackMandelDlg::OnBnClickedButtonMake()
{
	static UINT mcnt = 1, StartSpan = 0 ;
	static CString  oldstr; 
	static bool IsSpanning = false, IsSpanningForward = true ; // one shot on turn on.
	// static float tmpG = 0.0f ;
		 
	char  mys[256];
	
	// TODO: Add your control notification handler code here
	if( !m_diag_output.m_Does_Dialog_Exist ) DoNewOutput();
	
	UpdateData( TRUE ); // grab all the values ... easy way.

	// update m object with new data ...
	CRect outr; 
	m_diag_output.m_Picture.GetWindowRect( &outr );

	if( !m_isInitOK ) oldstr.Empty() ; 
	
	if( m_str_User_Function.Compare( oldstr.GetString() ) != 0 ) {  // need to update user-func string, this always fires on first make-button click. 
		oldstr = m_str_User_Function ;
		
		// FGZ: now translate user-string 'operators' to Complex functions ...
		if( !TranslateUserFunc() ) cout << "OpenGL-Complex-Function Translate failure !!" << endl;
		
		// now send the translated string into the .fs code ...
		char mystr[16390];
		sprintf_s( mystr, 16384, "%s", (const char*)m_str_User_Function.GetString() );
		m_shader->m_str_User_Function.assign( mystr );
		GUI_print(CString("Inserting following code :\r\n"), 0x0000A000 ) ;
		str_print(m_shader->m_str_User_Function + _T("\r\n"), 0x0000C700);
		 
		SetupMandel( outr.Width(), outr.Height(), true, ((IsSpanning || m_diag_output.m_is_zooming)? false : true ) );   // recreate GLSL program ...
	}


	if( !m_diag_output.m_is_zooming && !IsSpanning ) {
		sprintf_s(mys, 255, "Making Image %d", mcnt );
	 	cout << mys << endl ;
	}
	if( (mcnt % 100) == 0 )  { 
			sprintf_s(mys, 255, "Animate image # %d ", mcnt );
			std::cout << mys << std::endl;  
			OnBnClickedButtonTime();
		}
	
	if( mcnt == 1 ) {
		int nWidth =  max( 16, ((min( 16384,  outr.Width()) / 16) * 16)  );
		int nHeight = max( 16, ((min( 16384, outr.Height()) / 16) * 16)  );
		sprintf_s(mys, 255, "start size is  %d x %d \r\n", nWidth, nHeight);
		cout << mys ;
		sprintf_s(mys, 255, "iterations set at:  %d ", m_shader->getMaxIterations() );
		cout << mys << endl ;
	}
	
		
	if( outr.Width() != (int)m_shader->getWidth() || outr.Height() != (int)m_shader->getHeight() || !m_isInitOK  ) {
		int nWidth =  max( 16, ((min( 16384,  outr.Width()) / 16) * 16)  );
		int nHeight = max( 16, ((min( 16384, outr.Height()) / 16) * 16)  );
		sprintf_s(mys, 255, "set size to %d x %d \r\n", nWidth, nHeight); 
		cout << mys << endl ;
		if( !TranslateUserFunc() ) cout << "OpenGL-Complex-Function Translate failure !!" << endl;
		// now send the translated string into the .fs code ...
		char mystr[16390];
		sprintf_s( mystr, 16384, "%s", (const char*)m_str_User_Function.GetString() );
		m_shader->m_str_User_Function.assign( mystr );
		SetupMandel( nWidth, nHeight, false, ((IsSpanning || m_diag_output.m_is_zooming)? false : true ) );  
	}
	//  end size ...
	
	m_shader->pow_F = (float)m_pow_f ;
	// F factor in Z ^F +C 

	m_shader->Orbitfunc = OrbitFunc ;
	m_shader->m_str_User_Function;
	m_shader->m_UseEsmooth = m_UseEsmooth ;
	m_shader->m_float_G = m_float_G ;
	if( m_bSpanG && IsSpanning) {
		if( IsSpanningForward ) {
			float tmpSS = 1.0f / float(m_iSpanSpeed), tmpG = ( ((mcnt - StartSpan)*tmpSS) * (m_fSpanG - m_float_G) + m_float_G) ;
			if( tmpG >= m_fSpanG ) {   StartSpan = mcnt ;   IsSpanningForward = false ;   }
			m_shader->m_float_G = tmpG ; 
		}
		else {  // animate backward ...
			float tmpSS = 1.0f / float(m_iSpanSpeed), tmpG = (m_fSpanG - ((mcnt - StartSpan)*tmpSS) * (m_fSpanG - m_float_G) ) ;
			if( tmpG <= m_float_G ) {   StartSpan = mcnt ;   IsSpanningForward = true ;   } 
			m_shader->m_float_G = tmpG ; 
		}
	}
	m_shader->m_bailout = m_bailout;
	m_shader->juliaMode = ((m_bJulia)? true : false) ;


	int iter = max(1, min(65535, m_iterations)) ; // (((cl_uint)m_iterations / 16) * 16)  ;  // changed .cl file core to ccount[n].sN >= maxIterations from == max ... cause set to be colored funny.
	if( m_shader->getMaxIterations() != iter ) { 
		sprintf_s(mys, 255, "changed iterations from %d  to  %d ", (m_shader->getMaxIterations()), iter);
		cout << mys << endl ;
		m_shader->setMaxIterations( iter );
	}
	// end iterations 
	
	// if( !m_isInitOK ) return;
	 
	 
	__int64 Start = m_shader->Get_PC_Count(), Finish ;
	
	// NOW ... (finally) JUST DO IT !!
	m_shader->drawScene() ;


	Finish = m_shader->Get_PC_Count();
	m_shader->m_dRtime = m_shader->GetTimeSpan( Start, Finish );

	mcnt++ ;

	if( StartSpan + 1 == mcnt ) { KillTimer( 7776 );  MsgSleep( 750 );  SetTimer( 7776, 40, NULL ); }  // pause at ends during ani-span.

	if( m_bSpanG && !IsSpanning ) {  IsSpanning = true; IsSpanningForward = true ;   StartSpan = mcnt ;   SetTimer( 7776, 40, NULL );  }
	else if( !m_bSpanG && IsSpanning ) {   IsSpanning = false;  KillTimer( 7776 );   }
	
	// change following to: if( true )  ... item 5 of 5  FGZ_BITBLT_EXAMPLE
	if(  false  )   
	// leaving this here as it serves as a fully working example of off-screen rendering, if going into bitmap, etc.
	{   
		// THIS WORKS GREAT when above if is commented out, and SwapBuffer isn't done in DrawScene(), and glReadBits is instead ... but it's inefficient.
		// THIS method will work for off screen rendering, like to a BMP.
        
		int width = m_shader->getWidth();
        int height = m_shader->getHeight();
		CDC* pDC = m_diag_output.m_Picture.GetWindowDC() ;
		
		UINT* pSRC = m_shader->getPixels();
		
		void* pDIB = NULL;
		CImage out;
		out.Attach( MakeCompatibleBitmap(width, height, &pDIB, pSRC, true) , out.DIBOR_TOPDOWN );
		
		if( m_bStretch ) 
			pDC->StretchBlt(0,0, outr.Width(), outr.Height(), CDC::FromHandle(out.GetDC()), 0,0, width, height, SRCCOPY);
		else 
			pDC->BitBlt(0,0, width, height, CDC::FromHandle(out.GetDC()), 0,0, SRCCOPY);
		 
		GdiFlush();
		 
		ReleaseDC( pDC );
		out.ReleaseDC();
		out.Destroy();

    } // end if false ... example code.
}


void CBlackMandelDlg::OnBnClickedButtonZoom()
{
	// TODO: Add your control notification handler code here
	if( !m_diag_output.m_Does_Dialog_Exist ) DoNewOutput();
	m_diag_output.m_is_zooming ^= true ;
	if( m_diag_output.m_is_zooming ) { 
		cout << "Start Zooming ..." << endl ;
		::PostMessage( m_diag_output.GetSafeHwnd(), WM_FGZ_ZOOM_START, 0,0 );
	}
	else 
		cout << "Stop Zooming" << endl ;
}


void CBlackMandelDlg::OnBnClickedButtonTime()
{
	// TODO: Add your control notification handler code here
	char mys[256];
	sprintf_s(mys, 255, "Calc Time = %s Seconds", m_shader->ConvertTimeSpan( m_shader->m_dRtime ).c_str() );
	cout << mys << endl ;
}

void CBlackMandelDlg::OnEnKillfocusEditIterations()
{
	// TODO: Add your control notification handler code here ... this works well ...
	int nnum = GetDlgItemInt( IDC_EDIT_Iterations ) ;  
		if( nnum < 1 ) nnum = 1 ;
		if( nnum > 65535 ) nnum = 65535 ;
		SetDlgItemInt( IDC_EDIT_Iterations, nnum );
		
}


void CBlackMandelDlg::OnChangeEditIterations()
{
	static bool is_busy = false ;

	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialogEx::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
	if( !is_busy ) {
		is_busy = true ;
		int nnum = GetDlgItemInt( IDC_EDIT_Iterations ) ;  
		if( nnum < 1 ) nnum = 1 ;
		if( nnum > 65535 ) nnum = 65535 ;
		// SetDlgItemInt( IDC_EDIT_Iterations, nnum );  // this has weird user editing issues. caret goes to the left per char typed.
		is_busy = false ;
	}
}


void CBlackMandelDlg::OnTimer(UINT_PTR nIDEvent)
{
	static bool IsBusy = false ;
	 
	switch( nIDEvent ) 
	{
	case 7776 :
		if( !IsBusy ) {   // prevents build up of msgs which makes run-on when image is slow and zooming.
			IsBusy = true ;
			OnBnClickedButtonMake();
			IsBusy = false ;
		}
		break;
	case 4101:
		{
			KillTimer( 4101 );
			 
			if( m_diag_output.Create( IDD_DIALOG_OUTPUT , this ) ) 
				m_diag_output.ShowWindow(SW_SHOW) ;
			m_diag_output.m_pBase = this ;
			 
			///////////////////////////////////////////////////////////////
			// redirect cout output/buffer to our buffer ...
			m_oldCoutStreamBuf = cout.rdbuf();
			cout.rdbuf( m_strCout.rdbuf() );
	
			// update m_mandel with new data for ShaderApp(w,h) constructor.
			CRect outr; 
			m_diag_output.m_Picture.GetWindowRect( &outr );
	 
			// setup NeHeGL GLwindow struct
			if( ! SetupWindowGL (&m_window) ) {
				cout << "in Dlg-Timer:  SetupWindowGL FAILED !" << endl;
				m_isInitOK = false ;
			}
			else
				cout << "Setup GL Window is GOOD" << endl;
			
			// the following must be done AFTER the above ... so if m_shader is needed soon, move it's creation elsewhere ...
			
					SetupMandel(outr.Width(), outr.Height(), true, false);  
			// commenting out, and leaving it up to the make-button from here ... can't, then it doesn't work at all. weird delays, doesn't finish, stays at image 1 ...
			// WHY the first time compile error? Why then does it fix itself? What changes? ... maybe it's just on my AMD A10-5700 (ati 7660D) ? 
					OnBnClickedButtonMake(); // better cheap fix ...
					// if( !m_isInitOK ) GUI_print( CString("\r\n   Ignore the COMPILE ERROR, PLEASE !! \r\n   Just please Click: Make Fractal Button \r\n"), 0x00FF0000 ) ; // cheap fix ... :( sorry.
		}
		break;
	/*   ... doesn't work and results in message-box hell ...
	case 4102:
		{
			CRect outr; 
			m_diag_output.m_Picture.GetWindowRect( &outr );
			SetupMandel(outr.Width(), outr.Height(), true);   // after condensing the two classes, it fails on initial GLSL compile, try again, it works ! WHY ????????? !!
			if( m_isInitOK ) KillTimer ( 4102 );
		}
		break;
	// */
	case 4001: 
		if( m_strCout.str().length() > 0 ) 
			str_print( m_strCout.str(), 0x00000000 );
		m_strCout.clear();
		m_strCout.str("");
		break;
	default :
		KillTimer(nIDEvent) ;
		CString mys;
		mys.Format(_T("\n Event ID = %d \n NOT DEFINED in OnTimer \n") , nIDEvent ); 
		MessageBox(mys.GetString(), _T("switch default"), MB_OK);
		break;
	}

	CDialogEx::OnTimer(nIDEvent);
}


void CBlackMandelDlg::OnOK()
{
	// TODO: Add your specialized code here and/or call the base class  -- this button ID isn't used anymore ...

	CDialogEx::OnOK();
}


void CBlackMandelDlg::OnCancel()
{
	// TODO: Add your specialized code here and/or call the base class  --- this is now the Exit button ...

	CDialogEx::OnCancel();
}


afx_msg LRESULT CBlackMandelDlg::OnFgzUpdatePictureSize(WPARAM wParam, LPARAM lParam)
{
	static bool IsStartUp = true;
	if( IsStartUp ) { IsStartUp = false ;   return 1; }
		 
	// update m_mandel with new data for ShaderApp(w,h) constructor.
	CRect outr; 
	m_diag_output.m_Picture.GetWindowRect( &outr );
	 
	// setup NeHeGL GLwindow struct
	DestroyWindowGL (&m_window);
	SetupWindowGL( &m_window );
	
	SetupMandel(outr.Width(), outr.Height(), true);   // true to remake the shader ...
	// if( !m_isInitOK ) GUI_print( CString("\r\n   Ignore the COMPILE ERROR, PLEASE !! \r\n   Just please Click: Make Fractal Button \r\n"), 0x00FF0000 ) ; // cheap fix ... :( sorry.
	m_shader->setHeight( outr.Height() ) ;  // cheap bug fix ...
	m_shader->setWidth( outr.Width() ) ;  // this keeps set-up-mandel from firing again in make-button, which will be called after this exits.
	
	return 0;
}




void CBlackMandelDlg::OnFileExit()
{
	CDialogEx::OnCancel();
}


void CBlackMandelDlg::OnOptionsMakefractal()
{
	OnBnClickedButtonMake();
}



void CBlackMandelDlg::OnHelpAbout()
{
	CAboutDlg dlgAbout;
		dlgAbout.DoModal();
}


void CBlackMandelDlg::OnHelpHelp()
{
	// TODO: launch .chm here 
	::HtmlHelpA( this->GetSafeHwnd(), "./Mini-Fractals-Help.chm", 15, NULL ) ;
	// PostMessage( ID_HELP_HELP ) ; 
}

void CBlackMandelDlg::OnHelpGlslhelp()
{
	// TODO: launch GLSL pdf here ...
	SHELLEXECUTEINFO sei;
	sei.cbSize = sizeof( sei ); 
    sei.fMask = SEE_MASK_NOCLOSEPROCESS ; 
    sei.hwnd = this->m_hWnd; 
    sei.lpVerb = NULL; 
	sei.lpFile = "GLSLangSpec.3.30.6.pdf" ; 
    sei.lpParameters = NULL; 
    sei.lpDirectory = NULL; 
    sei.nShow = 0; 
    ShellExecuteEx( &sei );
}

void CAboutDlg::OnBnClickedZaclabs()
{
	SHELLEXECUTEINFO sei;
	sei.cbSize = sizeof( sei ); 
    sei.fMask = SEE_MASK_NOCLOSEPROCESS ; 
    sei.hwnd = this->m_hWnd; 
    sei.lpVerb = NULL; 
	sei.lpFile = "http://zac-labs.com" ; 
    sei.lpParameters = NULL; 
    sei.lpDirectory = NULL; 
    sei.nShow = 0; 
    ShellExecuteEx( &sei );
}
