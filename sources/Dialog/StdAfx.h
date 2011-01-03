// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__DF727672_3092_4B25_AE29_A085AFEC192A__INCLUDED_)
#define AFX_STDAFX_H__DF727672_3092_4B25_AE29_A085AFEC192A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

/*------------------------ Added by sun.yt 8/27/2010 ------------------------*/

#define	_AFX_ENABLE_INLINES
#define	_WIN32_WINNT	0x500
#include <Afx.h>
#include <afxmt.h>
#include "..\..\sources\common\Const.h"
#include "..\..\sources\Data\Queue.h"
#include "..\..\sources\SerialPort\serialport.h"
#include "..\..\sources\common\Debug.h"


/*---------------------------------------------------------------------------*/

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdisp.h>        // MFC Automation classes
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxdb.h>
#include <odbcinst.h>



//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__DF727672_3092_4B25_AE29_A085AFEC192A__INCLUDED_)
