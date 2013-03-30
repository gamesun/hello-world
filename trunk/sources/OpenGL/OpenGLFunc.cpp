//
#include "..\..\sources\Dialog\StdAfx.h"
#include "..\..\sources\Dialog\Resource.h"
#include "..\..\sources\OpenGL\OpenGLFunc.h"
#include "..\..\sources\common\Global.h"




//-----------------------------------------------------------------------------
// Name: COpenGL()
// Desc: 
//-----------------------------------------------------------------------------
COpenGL::COpenGL()
{
    m_pDC		  = NULL;
	m_pCDrawWnd	  = NULL;
	m_pDspyThread = NULL;
}

//-----------------------------------------------------------------------------
// Name: ~COpenGL()
// Desc: 
//-----------------------------------------------------------------------------
COpenGL::~COpenGL()
{
	ReleaseAllObjects();
}

//-----------------------------------------------------------------------------
// Name: InitOpenGL()
// Desc: 
//-----------------------------------------------------------------------------
HRESULT
COpenGL::InitOpenGL( COscillDlg *pCWnd )
{
    PIXELFORMATDESCRIPTOR   pfd;
    
    m_pCDrawWnd = pCWnd->GetDlgItem( IDC_DRAWFRAME );

    m_pDC = new CClientDC( m_pCDrawWnd );
    if( NULL == m_pDC )
    {
        return RptError( pCWnd->m_hWnd, FALSE, "GetDC Failed" );
    }
    
    if( !SetupPixelFormat( pCWnd->m_hWnd ) )
        return FALSE;

    int nIndex = GetPixelFormat( m_pDC->GetSafeHdc() );
    DescribePixelFormat( m_pDC->GetSafeHdc(), nIndex, sizeof(pfd), &pfd );

    m_hDC = m_pDC->GetSafeHdc();
    m_hRC1 = wglCreateContext( m_pDC->GetSafeHdc() );
//	m_hRC2 = wglCreateContext( m_pDC->GetSafeHdc() );
	wglMakeCurrent( m_hDC, m_hRC1 );
//	wglShareLists( m_hRC2, m_hRC1 );

    glClearDepth( 1.0f );
    glEnable( GL_DEPTH_TEST );

    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    
	glClearColor( 1.0f, 1.0f, 1.0f, 1.0f );
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	gluOrtho2D( GLLEFT, GLRIGHT, GLBOTTOM, GLTOP );
    
    CRect   rcDrawFrame;
    GetWindowRect( m_pCDrawWnd->m_hWnd, &rcDrawFrame );

// 	TRACE( "%d,%d,%d,%d\r\n", rcDrawFrame.left,
// 							  rcDrawFrame.top, 
// 							  rcDrawFrame.right,
// 							  rcDrawFrame.bottom );

    rcDrawFrame.SetRect( rcDrawFrame.left + 3,
                         rcDrawFrame.top + 3,
                         rcDrawFrame.right - 3,
                         rcDrawFrame.bottom - 3 );
	
	int nWidth = (int)( ( rcDrawFrame.Height() - 16 ) * 2.3 );

	if ( nWidth > rcDrawFrame.Width() - 20 )
	{
		nWidth = rcDrawFrame.Width() - 20;
	}

	int xMargin = ( rcDrawFrame.Width() - nWidth ) / 2;

    glViewport(  xMargin, 5, nWidth, rcDrawFrame.Height() - 17 );


    glEnable( GL_SCISSOR_TEST );
    glScissor( xMargin, 5, nWidth, rcDrawFrame.Height() - 17 );
    
	wglMakeCurrent( NULL, NULL );

	m_bDspyThreadRunning = true;
	m_pDspyThread = AfxBeginThread( DspyThreadFunc, &m_sDspyThreadPara/*, THREAD_PRIORITY_HIGHEST*/ );

    return TRUE;
}

//-----------------------------------------------------------------------------
// Name: ReleaseAllObjects()
// Desc: 
//-----------------------------------------------------------------------------
GLvoid 
COpenGL::ReleaseAllObjects( void )
{
	wglMakeCurrent( NULL,  NULL );
    
	if ( m_hRC1 )
	{
		wglDeleteContext( m_hRC1 );
		m_hRC1 = NULL;
	}
	if ( m_hDC )
	{
		ReleaseDC( m_pCDrawWnd->m_hWnd, m_hDC );
		m_hDC = NULL;
	}
	if ( m_pDC )
	{
		delete m_pDC;
		m_pDC = NULL;
	}
}

//-----------------------------------------------------------------------------
// Name: RptError()
// Desc: 
//-----------------------------------------------------------------------------
HRESULT
COpenGL::RptError( HWND hWnd, HRESULT hRet, LPCTSTR szError, ... )
{
    char                        szBuff[128];
    va_list                     vl;
    
    va_start( vl, szError );
    vsprintf_s( szBuff, szError, vl );
    ReleaseAllObjects();
    MessageBox( hWnd, szBuff, "Oscilloscopes", MB_OK | MB_ICONHAND );
    va_end( vl );
    return hRet;
}

//-----------------------------------------------------------------------------
// Name: SetupPixelFormat()
// Desc: 
//-----------------------------------------------------------------------------
HRESULT
COpenGL::SetupPixelFormat( HWND hWnd )
{
    static PIXELFORMATDESCRIPTOR pfd = 
    {
        sizeof(PIXELFORMATDESCRIPTOR),		   // size of this pfd
            1,									   // version number
            PFD_DRAW_TO_WINDOW |				   // support window
            PFD_SUPPORT_OPENGL, 				   // support OpenGL
            PFD_TYPE_RGBA,                         // RGBA type
            24,                                    // 24-bit color depth
            0,  0,  0,  0,  0,  0,                 // color bits ignored
            0,                                     // no alpha buffer
            0,                                     // shift bit ignored
            0,                                     // no accumulation buffer
            0,  0,  0,  0,                         // accum bits ignored
            32,                                    // 32-bit z-buffer
            0,                                     // no stencil buffer
            0,                                     // no auxiliary buffer
            PFD_MAIN_PLANE,                        // main layer
            0,                                     // reserved
            0,  0,  0                              // layer masks ignored
    };
    
    int nPixelformat;
    if ( (nPixelformat = ChoosePixelFormat(m_pDC->GetSafeHdc(),  &pfd)) == 0 )
    {
        return RptError( hWnd, FALSE, "ChoosePixelFormat failed" );
    }
    
    if ( SetPixelFormat(m_pDC->GetSafeHdc(),  nPixelformat,  &pfd) == FALSE )
    {
        return RptError( hWnd, FALSE, "SetPixelFormat failed" );
    }

    return TRUE;
}



//-----------------------------------------------------------------------------
// Name: DrawGrid()
// Desc: 
//-----------------------------------------------------------------------------
GLvoid 
COpenGL::DrawGrid(
    GLfloat left, 
    GLfloat right, 
    GLfloat bottom, 
    GLfloat top )
{
    GLfloat    fwidth = right - left;
    GLfloat    fheight = top - bottom;
    GLfloat    fdeltax = 1.0f;
    GLfloat    fdeltay = 1.0f;
    GLfloat    fgridw = ( fwidth - 2 * fdeltax ) / 12;
    GLfloat    fgridh = ( fheight - 2 * fdeltay ) / 6;

	glColor3ub( 0, 0, 0 );
	
    glBegin( GL_LINES );
    {
        glVertex2f( fdeltax,  fheight / 2 /*- 0.1*/ );
        glVertex2f( fwidth - fdeltax,  fheight / 2 /*- 0.1*/ );
		
		for ( int i = 1; i < 60 ; i++ )
		{
			glVertex2f( fdeltax + i * fgridw / 5.0f, fheight / 2.0f + 2.0f );
			glVertex2f( fdeltax + i * fgridw / 5.0f, fheight / 2.0f - 2.0f );
		}
    }
    glEnd();
    
    glBegin( GL_LINE_LOOP );
    {
        glVertex2f( fdeltax, fdeltay );
        glVertex2f( fdeltax, fheight - fdeltay );
        glVertex2f( fwidth - fdeltax, fheight - fdeltay );
        glVertex2f( fwidth - fdeltax, fdeltay );
    }
    glEnd();

    glColor3ub( 127, 127, 127 );
	glEnable( GL_LINE_STIPPLE );
	glLineStipple( 1, 0x0707 );
    glBegin( GL_LINES );
    {
        for ( char x = 1; x <= 11; x++ )
        {
            glVertex2f(  fdeltax + fgridw * x, fdeltay );
            glVertex2f(  fdeltax + fgridw * x, fheight - fdeltay );
        }
        
        for ( char y = 1; y <= 5; y++ )
        {
//             if( 3 == y )
//             {
//                 continue;
//             }
            glVertex2f( fdeltax, fdeltay + fgridh * y  );
            glVertex2f( fwidth - fdeltax, fdeltay + fgridh * y  );
        }
    }
    glEnd();
	glDisable( GL_LINE_STIPPLE );

//	glFinish();
}

//-----------------------------------------------------------------------------
// Name: DrawRuler()
// Desc: 
//-----------------------------------------------------------------------------
GLvoid COpenGL::DrawRuler( GLfloat x )
{
	
	glColor3ub( 255, 0, 0 );

	glBegin( GL_LINES );
	{
		glVertex2f( x, GLTOP );
		glVertex2f( x, GLBOTTOM );

	}
	glEnd();

//	glFinish();
}
