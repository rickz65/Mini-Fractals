/////////////////////////////////////////////////////////////////////////////////////
//
//
//  Mini-Fractals/glMandel Copyright © July 2013 Frederick G Zacharias Covered under the CPOL 
//
//  See License-CPOL.txt for the Full License.
//
//


// BlackMandelDlg.h : header file
//

#pragma once
#include "afxwin.h"
#include "Output.h" 
#include "ShadersApp.hpp"
#include "NeHeGL.h"	
#include <iostream> 
#include <sstream>
#include "afxcmn.h"


#define WM_FGZ_ZOOM_START  5500 
#define WM_FGZ_ZOOM_STOP   5501
#define WM_FGZ_UPDATE_PICTURE_SIZE  5502 


class COutput ;
class CBlackMandelApp ;

extern bool GUI_print(CString str, COLORREF color) ;

// CBlackMandelDlg dialog
class CBlackMandelDlg : public CDialogEx
{
	friend class COutput ;
	friend class CBlackMandelApp;
	friend class ShadersApp; 

// Construction
public:
	CBlackMandelDlg(CWnd* pParent = NULL);	// standard constructor
	// ~CBlackMandelDlg();

// Dialog Data
	enum { IDD = IDD_BLACKMANDEL_DIALOG };

	CRichEditCtrl		m_CEdit_Output;
	CSize				m_OutSize ;
	std::ostringstream  m_strCout;            // our new cout buffer ... echo into m_CEdit_Output
	

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	
	BOOL DoNewOutput() ;
	void SetupMandel(int _Width, int _Height, bool DoInit, bool ShowMsgs=true);
	 
	void TerminateApplication (GL_Window* window);		// Terminate The Application
	void ToggleFullscreen (GL_Window* window);			// Toggle Fullscreen / Windowed Mode
	void ReshapeGL (int width, int height)	;
	BOOL DestroyWindowGL (GL_Window* window);
	BOOL SetupWindowGL (GL_Window* window) ;
	BOOL ChangeScreenResolution (int width, int height, int bitsPerPixel) ;
	// These Are The Function You Must Provide
	BOOL Initialize (GL_Window* window, Keys* keys);	// Performs All Your Initialization
	void Deinitialize (void);							// Performs All Your DeInitialization
	void Update (DWORD milliseconds);					// Perform Motion Updates
	void Draw (void);									// Perform All Your Scene Drawing
	bool TranslateUserFunc() ;
	int getNextOperatorPos(CString src, int start);
	int getLastOperatorPos(CString src, int start);
	int getNextTokenPos(CString src, int start, bool bCheckBegining);
	int getLastTokenPos(CString src, int start, bool bCheckEnd);
	CString digestOperators(CString str_User_Function, bool bCheckToken ); 
	CString DigestParen(CString snipit, bool bCheckToken); 
	 
	UINT				m_iterations;
	UINT				m_pow_f;
	COutput				m_diag_output;
	GL_Window			m_window ;  // NeHeGL sturct of above m_daig_Output
	// Mandelbrot			m_mandelbrot ; 
	ShadersApp*			m_shader ;
	std::streambuf*     m_oldCoutStreamBuf ;  // the original std::cout buffer ... restore in destructor
	BOOL				m_bStretch;   // (Bit)stretch image to the picture box which is linked to window.
	CEdit               m_CEdit_F; 
	CSpinButtonCtrl		m_CSBC_pow_f;
	bool                m_isInitOK ;


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonMake();
	afx_msg void OnBnClickedButtonZoom();
	afx_msg void OnBnClickedButtonTime();
	afx_msg void OnChangeEditIterations();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnDestroy();
	afx_msg void OnEnKillfocusEditIterations();
protected:
	afx_msg LRESULT OnFgzUpdatePictureSize(WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnFileExit();
	afx_msg void OnOptionsMakefractal();
	afx_msg void OnHelpAbout();
protected:
	CButton m_CZTOF;    // our Radio group of function/equations to choose from ...
	CButton m_CSINZ;
	CButton m_CCOSZ;
	CButton m_CUSER;
	CString m_str_User_Function;    // the user enter equation/function to be translated. No Operators in GLSL ... :( 
	BOOL m_UseEsmooth;   // our coloring method 
	float m_float_G;    // a user value to inject into the equations 
	float m_bailout;    // value to compare the norm of the resultant of the function/equation 
	int OrbitFunc;      // which radio button is choosen 
public:
	BOOL m_bJulia;
	BOOL m_bTrace;
	afx_msg void OnHelpHelp();
	afx_msg void OnHelpGlslhelp();
	BOOL m_bSpanG;
	float m_fSpanG;
	int m_iSpanSpeed;
	CSliderCtrl m_CSpanSpeed;
};
