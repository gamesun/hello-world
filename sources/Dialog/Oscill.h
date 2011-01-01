// Oscill.h : main header file for the Oscill application
//

#if !defined(AFX_Oscill_H__DF589F0A_196B_4AD6_8D81_F27D3F35B997__INCLUDED_)
#define AFX_Oscill_H__DF589F0A_196B_4AD6_8D81_F27D3F35B997__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif


#include "resource.h"		// main symbols
#include <math.h>
#include <stdlib.h>
#include <time.h>

#include "..\..\sources\OpenGL\OpenGLFunc.h"
#include "..\..\sources\SerialPort\EnumSerialPort.h"
#include "..\..\sources\Thread\RecvThread.h"
#include "..\..\sources\Thread\DspyThread.h"
#include "..\..\sources\common\Global.h"



//#include "winuser.h"

//#pragma comment (lib , "user32.lib")
/////////////////////////////////////////////////////////////////////////////
// COscillApp:
// See Oscill.cpp for the implementation of this class
//

class COscillApp : public CWinApp
{
public:
	COscillApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COscillApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(COscillApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_Oscill_H__DF589F0A_196B_4AD6_8D81_F27D3F35B997__INCLUDED_)
