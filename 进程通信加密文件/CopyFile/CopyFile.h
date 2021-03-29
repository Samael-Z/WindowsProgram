
// CopyFile.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CCopyFileApp:
// See CopyFile.cpp for the implementation of this class
//

class CCopyFileApp : public CWinApp
{
public:
	CCopyFileApp();

// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CCopyFileApp theApp;
