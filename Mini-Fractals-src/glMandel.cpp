/////////////////////////////////////////////////////////////////////////////////////
//
//
//  Mini-Fractals/glMandel Copyright � July 2013 Frederick G Zacharias Covered under the CPOL 
//
//  See License-CPOL.txt for the Full License.
//
//

// BlackMandel.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "glMandel.h"
#include "glMandelDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CBlackMandelApp

BEGIN_MESSAGE_MAP(CBlackMandelApp, CWinApp)
END_MESSAGE_MAP()


// CBlackMandelApp construction

CBlackMandelApp::CBlackMandelApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


// The one and only CBlackMandelApp object

CBlackMandelApp theApp;


// CBlackMandelApp initialization

BOOL CBlackMandelApp::InitInstance()
{
//TODO: call AfxInitRichEdit2() to initialize richedit2 library.
	// InitCommonControlsEx() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// Set this to include all the common control classes you want to use
	// in your application.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);
	AfxInitRichEdit2();

	CWinApp::InitInstance();


	AfxEnableControlContainer();

	// Create the shell manager, in case the dialog contains
	// any shell tree view or shell list view controls.
	CShellManager *pShellManager = new CShellManager;

	// Activate "Windows Native" visual manager for enabling themes in MFC controls
	CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));

	
	INT_PTR nResponse ;
	try {

		CBlackMandelDlg dlg;   // catch the constructor, too, if there is one.
		m_pMainWnd = &dlg;

		nResponse = dlg.DoModal();
	 
	}
	catch(...) {
		MessageBox(NULL, _T("\n\n  Mini-Fractals can't Use OpenGL correctly  \n\n  Try installing : \n      Video Card Software Package, \n      more than just the driver \n        (ATI SDK = 100MB file) \n\n"), _T("Mini-Fractals - Crash"), MB_OK)	;
	}
	 
	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Cancel
	}
	else if (nResponse == -1)
	{
		TRACE(traceAppMsg, 0, "Warning: dialog creation failed, so application is terminating unexpectedly.\n");
		TRACE(traceAppMsg, 0, "Warning: if you are using MFC controls on the dialog, you cannot #define _AFX_NO_MFC_CONTROLS_IN_DIALOGS.\n");
	}

	// Delete the shell manager created above.
	if (pShellManager != NULL)
	{
		delete pShellManager;
	}

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}

