/////////////////////////////////////////////////////////////////////////////////////
//
//
//  Mini-Fractals/glMandel Copyright © July 2013 Frederick G Zacharias Covered under the CPOL 
//
//  See License-CPOL.txt for the Full License.
//
//  


// BlackMandel.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CBlackMandelApp:
// See BlackMandel.cpp for the implementation of this class
//

class CBlackMandelApp : public CWinApp
{
public:
	CBlackMandelApp();

// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
	
};

extern CBlackMandelApp theApp;