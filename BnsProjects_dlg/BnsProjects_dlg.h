// BnsProjects_dlg.h : main header file for the BnsProjects_dlg DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CBnsProjects_dlgApp
// See BnsProjects_dlg.cpp for the implementation of this class
//

class CBnsProjects_dlgApp : public CWinApp
{
public:
	CBnsProjects_dlgApp();

// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	DECLARE_MESSAGE_MAP()
};
