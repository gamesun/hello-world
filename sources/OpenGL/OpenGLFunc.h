// OpenGLFunc.h : header file
//
#pragma once

#ifndef __OPENGLFUNC_H__
#define __OPENGLFUNC_H__


#include "..\..\sources\Dialog\Oscilldlg.h"
#include "..\..\sources\Thread\DspyThread.h"
#include <GL\gl.h>
#include <GL\glu.h>
//#include <GL\glut.h>

#pragma comment (lib, "GLU32.lib")
#pragma comment (lib, "OPENGL32.lib")


#define GLLEFT		0.0f
#define GLRIGHT		600.0f
#define GLBOTTOM	0.0f
#define GLTOP		300.0f

#define GLHEIGHT	300.0f	//	| GLTOP - GLBOTTOM |
#define GLWITDH		600.0f	//	| GLRIGHT - GLLEFT |


class COpenGL
{
public:
    COpenGL();
	~COpenGL();

	HRESULT     InitOpenGL( COscillDlg *phWnd );
    HRESULT     SetupPixelFormat( HWND hWnd );
    HRESULT     RptError( HWND hWnd, HRESULT hRet, LPCTSTR szError,... );
    GLvoid      ReleaseAllObjects( void );
    GLvoid		DrawGrid( GLfloat left, GLfloat right, GLfloat bottom, GLfloat top );
	GLvoid		DrawRuler( GLfloat x );
	
    CWnd		*m_pCDrawWnd;
	CClientDC   *m_pDC;
    HDC         m_hDC;
    HGLRC       m_hRC1;
//	HGLRC		m_hRC2;
	SDTPARA		m_sDspyThreadPara;

	CWinThread	*m_pDspyThread;
	bool		m_bDspyThreadRunning;
protected:
	
	
private:

};


//UINT    DspyThreadFunc( LPVOID lpParam );	//Display

/*
extern	CEvent		g_eventDspy;
extern	CCriticalSection g_ccsRWData;

extern	COpenGL		g_COGL;
extern	SQueue		*g_psQueue;

extern	COscillDlg	*g_pMainThis;
*/


#endif /* __OPENGLFUNC_H__ */