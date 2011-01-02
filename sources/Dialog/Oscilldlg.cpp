// OscillDlg.cpp : implementation file
//

#include "StdAfx.h"
#include "Oscill.h"
#include "OscillDlg.h"
#include "ComSetting.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CEvent			 g_eventDspy;
CEvent			 g_eventCommRecv;
CCriticalSection g_ccsRWData;

BOOL			 g_bActive;
COpenGL			 g_COGL;
SQueue			 *g_psQueue;
SMeasPara		 g_sMeasPara;

COscillDlg		 *g_pMainThis;

clock_t			 start;
clock_t			 finish;
double			 duration;



/////////////////////////////////////////////////////////////////////////////
// COscillDlg dialog

COscillDlg::COscillDlg(CWnd* pParent /*=NULL*/)
	: CDialog(COscillDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(COscillDlg)
	m_editTimebaseScale = _T("");
	m_editXPosition = _T("");
	m_editScaleA = _T("");
	m_editScaleB = _T("");
	m_editTriggerLevel = _T("");
	m_editTriggerLevelUnit = _T("");
	m_editYPositionA = _T("");
	m_editYPositionB = _T("");
	m_szMSCommRecv = _T("");
	m_szMSCommSend = _T("");
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void COscillDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COscillDlg)
	DDX_Control(pDX, IDC_BTNCOMOPEN, m_ctrlBtnComOpen);
	DDX_Control(pDX, IDC_RECEIVE, m_ctrlbtnReceive);
	DDX_Control(pDX, IDC_SBDRAWFRAME, m_ctrlSbDrawFrame);
	DDX_Control(pDX, IDC_editYPosition_B, m_ctrlYPosition_B);
	DDX_Control(pDX, IDC_spneditYPosition_B, m_ctrlSpnYPosition_B);
	DDX_Control(pDX, IDC_spneditXPosition, m_ctrlSpnXPosition);
	DDX_Control(pDX, IDC_spneditTimebaseScale, m_ctrlSpnTimebaseScale);
	DDX_Control(pDX, IDC_btnTrigger_B, m_ctrlTrigger_B);
	DDX_Control(pDX, IDC_btnTrigger_A, m_ctrlTrigger_A);
	DDX_Control(pDX, IDC_editXPosition, m_ctrlXPosition);
	DDX_Control(pDX, IDC_editTimebaseScale, m_ctrlTimebaseScale);
	DDX_Control(pDX, IDC_btnTimebaseY_T, m_cbtnTimebaseY_T);
	DDX_Text(pDX, IDC_editTimebaseScale, m_editTimebaseScale);
	DDX_Text(pDX, IDC_editXPosition, m_editXPosition);
	DDX_Text(pDX, IDC_editScale_A, m_editScaleA);
	DDX_Text(pDX, IDC_editScale_B, m_editScaleB);
	DDX_Text(pDX, IDC_editTriggerLevel, m_editTriggerLevel);
	DDX_Text(pDX, IDC_editTriggerLevelUnit, m_editTriggerLevelUnit);
	DDX_Text(pDX, IDC_editYPosition_A, m_editYPositionA);
	DDX_Text(pDX, IDC_editYPosition_B, m_editYPositionB);
//	DDX_Control(pDX, IDC_MSCOMM1, m_mscom);
	DDX_Text(pDX, IDC_edtMSCommRecv, m_szMSCommRecv);
	DDX_Text(pDX, IDC_edtMSCommSend, m_szMSCommSend);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(COscillDlg, CDialog)
	//{{AFX_MSG_MAP(COscillDlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_btnTimebaseY_T, OnbtnTimebaseY_T)
	ON_BN_CLICKED(IDC_btnTimebaseAdd, OnbtnTimebaseAdd)
	ON_BN_CLICKED(IDC_btnTimebaseB_A, OnbtnTimebaseB_A)
	ON_BN_CLICKED(IDC_btnTimebaseA_B, OnbtnTimebaseA_B)
	ON_BN_CLICKED(IDC_btnAC_A, OnbtnAC_A)
	ON_BN_CLICKED(IDC_btnZero_A, OnbtnZero_A)
	ON_BN_CLICKED(IDC_btnDC_A, OnbtnDC_A)
	ON_BN_CLICKED(IDC_btnAC_B, OnbtnAC_B)
	ON_BN_CLICKED(IDC_btnZero_B, OnbtnZero_B)
	ON_BN_CLICKED(IDC_btnDC_B, OnbtnDC_B)
	ON_BN_CLICKED(IDC_btnTriggerSingle, OnbtnTriggerSingle)
	ON_BN_CLICKED(IDC_btnTriggerNormal, OnbtnTriggerNormal)
	ON_BN_CLICKED(IDC_btnTriggerAuto, OnbtnTriggerAuto)
	ON_BN_CLICKED(IDC_btnTriggerNone, OnbtnTriggerNone)
	ON_BN_CLICKED(IDC_btnTriggerRising, OnbtnTriggerRising)
	ON_BN_CLICKED(IDC_btnTriggerFalling, OnbtnTriggerFalling)
	ON_BN_CLICKED(IDC_btnTrigger_A, OnbtnTrigger_A)
	ON_BN_CLICKED(IDC_btnTrigger_B, OnbtnTrigger_B)
	ON_WM_ACTIVATE()
	ON_NOTIFY(UDN_DELTAPOS, IDC_spneditTimebaseScale, OnDeltapos_spneditTimebaseScale)
	ON_NOTIFY(UDN_DELTAPOS, IDC_spneditScale_A, OnDeltapos_spneditScaleA)
	ON_NOTIFY(UDN_DELTAPOS, IDC_spneditScale_B, OnDeltapos_spneditScaleB)
	ON_NOTIFY(UDN_DELTAPOS, IDC_spneditXPosition, OnDeltapos_spneditXPosition)
	ON_NOTIFY(UDN_DELTAPOS, IDC_spneditYPosition_A, OnDeltapos_spneditYPositionA)
	ON_NOTIFY(UDN_DELTAPOS, IDC_spneditYPosition_B, OnDeltapos_spneditYPositionB)
	ON_NOTIFY(UDN_DELTAPOS, IDC_spneditTriggerLevel, OnDeltapos_spneditTriggerLevel)
	ON_NOTIFY(UDN_DELTAPOS, IDC_spneditTriggerLevelUnit, OnDeltapos_spneditTriggerLevelUnit)
	ON_WM_HSCROLL()
	ON_BN_CLICKED(IDC_RECEIVE, OnReceive)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_btnMSCommSend, OnbtnMSCommSend)
	ON_BN_CLICKED(IDC_COMSETTING, OnComsetting)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_btnTriggerSingle, OnbtnTriggerSingle)
	ON_BN_CLICKED(IDC_BTNCOMOPEN, OnBtnComOpen)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDOK, &COscillDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_btnReverse, &COscillDlg::OnBnClickedbtnreverse)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COscillDlg message handlers

BOOL COscillDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
// 	HICON	hRisingIcon = AfxGetApp()->LoadIcon( IDI_RISING );
// 	HICON	hFallingIcon = AfxGetApp()->LoadIcon( IDI_FALLING );
// 	m_ctrlbtnTriggerRising.SetIcon( hRisingIcon ); 
// 	m_ctrlbtnTriggerFalling.SetIcon( hFallingIcon );

	m_ctrlbmpbtnRising.AutoLoad( IDC_btnTriggerRising, this );
	m_ctrlbmpbtnFalling.AutoLoad( IDC_btnTriggerFalling, this );

/////////////////////////// TEST ///////////////////////////
	g_pMainThis = this;


	g_sMeasPara.nSampFreq = 100000;	// 100kHz
//	m_nBufLength = ( int )( g_sMeasPara.nSampFreq * 0.02 );
//	m_byTbSUpperLimit = 100000 / g_sMeasPara.nSampFreq;
	g_psQueue = InitSQueue();
	
	SCROLLINFO	sScrollInfo;
	sScrollInfo.cbSize = sizeof( SCROLLINFO );
	sScrollInfo.fMask = SIF_RANGE | SIF_PAGE;
	sScrollInfo.nMax = 5;
	sScrollInfo.nMin = 0;
	sScrollInfo.nPage = 1;
	m_ctrlSbDrawFrame.SetScrollInfo( &sScrollInfo );

//--------------------------------- MSComm ---------------------------------//
	


//--------------------------------------------------------------------------//

////////////////////////////////////////////////////////////

	m_nQueueRearOffset = 0;
	m_nXCnt = 0;
	m_nXCntOffset = 0;
    m_pRecvThread = NULL;

    OnbtnTimebaseY_T();
    OnbtnAC_A();
    OnbtnAC_B();
    OnbtnTrigger_A();
    OnbtnTriggerRising();
    OnbtnTriggerNone();

    //Timebase
    m_byTbScale = 4;
    m_editTimebaseScale.Format( _T( "%s" ), pc_szTbScale[m_byTbScale] );
	m_nSampPerFrame = 
		(int)( 12 * g_sMeasPara.nSampFreq * c_fTbScaleCoef[ m_byTbScale ] );
    m_fXPosition = 0;
    m_editXPosition.Format( _T( "0" ) );

	m_ctrlSbDrawFrame.SetScrollRange( 0, g_psQueue->num / m_nSampPerFrame );

    //Channel A
    m_byScaleA = 9;
    m_editScaleA.Format( _T( "%s" ), pc_szVoltScale[m_byScaleA] );
    m_fYPositionA = 0;
    m_editYPositionA.Format( _T( "0" ) );

    //Channel B
    m_byScaleB = 0;
    m_editScaleB.Format( _T( "%s" ), pc_szVoltScale[m_byScaleB] );
    m_fYPositionB = 0;
    m_editYPositionB.Format( _T( "0" ) );

    //Trigger
    m_fTriggerLevel = 2.5;
    m_editTriggerLevel.Format( _T( "2.5" ) );
    m_byTriggerLevelUnit = 1;
    m_editTriggerLevelUnit.Format( _T( "V" ) );

	UpdateData( FALSE );

    if ( g_COGL.InitOpenGL( this ) != TRUE )
    {
        exit( -1 );
    }

	m_bRecvThreadRunning = true;
	m_pRecvThread = AfxBeginThread( RecvThreadFunc, this/*, THREAD_PRIORITY_LOWEST*/ );

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void COscillDlg::OnPaint() 
{
    if (IsIconic())
    {
        CPaintDC dc(this); // device context for painting
        
        SendMessage( WM_ICONERASEBKGND, ( WPARAM ) dc.GetSafeHdc(), 0 );
        
        // Center icon in client rectangle
        int cxIcon = GetSystemMetrics( SM_CXICON );
        int cyIcon = GetSystemMetrics( SM_CYICON );
        CRect rect;
        GetClientRect( &rect );
        int x = ( rect.Width() - cxIcon + 1 ) / 2;
        int y = ( rect.Height() - cyIcon + 1 ) / 2;
        
        // Draw the icon
        dc.DrawIcon(x, y, m_hIcon);
    }
    else
    {   
		CDialog::OnPaint();
    }

	UpdateFrame( CLEAR );

}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR COscillDlg::OnQueryDragIcon()
{
    return (HCURSOR) m_hIcon;
}

void COscillDlg::OnReceive() 
{
	// TODO: Add your control notification handler code here
	static BOOL bFlag = TRUE;

	
	if ( bFlag )
	{
		bFlag = FALSE;
		m_ctrlbtnReceive.SetWindowText( "Stop" );
		SetTimer( 1, 1, NULL );
	}
	else
	{
		bFlag = TRUE;
		m_ctrlbtnReceive.SetWindowText( "Receive" );
		KillTimer( 1 );
	}
	
/*
    if( NULL != m_pRecvThread )
    {
        if( WAIT_OBJECT_0 == WaitForSingleObject( m_pRecvThread->m_hThread, 0 ) )
        {
            return;
        }
    }
	
    m_pRecvThread = AfxBeginThread( RecvThreadFunc, this );
*/
}

void COscillDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	
	g_eventCommRecv.SetEvent();
//	WaitForSingleObject( m_pRecvThread->m_hThread, INFINITE );
/*
	static int m = 0;
	static int noldPos = 0;

	INSQueue( g_psQueue, g_nTest[m] - 100 );
	m = ( m + 1 ) % 200;
	
	// There are still a problem is that when  
	// m_nSampPerFrame is greater than MAX_SAMP_RECORD. 
	// Temporarily ignored.
	m_nXCnt++;
	if ( m_nXCnt >= m_nSampPerFrame )
	{
		m_nXCnt %= m_nSampPerFrame;
	}
	
	m_nNumOfDiv = ( g_psQueue->num - m_nXCnt ) / ( m_nSampPerFrame / 12 );
	m_ctrlSbDrawFrame.SetScrollRange( 0, m_nNumOfDiv );
	
	noldPos = m_ctrlSbDrawFrame.GetScrollPos();
	m_ctrlSbDrawFrame.SetScrollPos( m_nNumOfDiv, FALSE );

	if ( noldPos != m_nNumOfDiv )
	{
		m_nQueueRearOffset = 0;
		UpdateFrame( CLEAR );
	}
	else
	{
		UpdateFrame( BASIC );
	}
*/	
	CDialog::OnTimer(nIDEvent);

}


//-----------------------------------------------------------------------------
// Name: Calculate()
// Desc: 
//-----------------------------------------------------------------------------
void COscillDlg::Calculate( void )
{
//	TRACE( ">>Enter Calculate\n" );
	TRACE( "        Ask->Calc\n" );
	g_ccsRWData.Lock();
	TRACE( "        ->Calc\n" );

	int nSampPerDiv = (int)( g_sMeasPara.nSampFreq * c_fTbScaleCoef[m_byTbScale] );
	m_nSampPerFrame = 12 * nSampPerDiv;
	
//	m_nXCnt %= m_nSampPerFrame;
	m_nXCnt = g_psQueue->num % m_nSampPerFrame;
	m_nNumOfDiv = ( g_psQueue->num - m_nXCnt ) / nSampPerDiv;
	m_ctrlSbDrawFrame.SetScrollRange( 0, m_nNumOfDiv );

	int nPosition = m_ctrlSbDrawFrame.GetScrollPos();
	
	int ntmp = m_nNumOfDiv - nPosition - ( 12 - m_nXCnt / nSampPerDiv );
	if ( 0 < ntmp )
	{
		m_nQueueRearOffset = - m_nXCnt % nSampPerDiv - nSampPerDiv * ntmp;
	}
	else
	{
		m_nQueueRearOffset = 0;
	}
	
	if ( nPosition == m_nNumOfDiv )
	{
		m_nQueueRearOffset = 0;
	}

	m_nXCntOffset = 0;
	if ( 0 == m_nQueueRearOffset )
	{
		m_nXCntOffset = ( m_nNumOfDiv - nPosition ) * nSampPerDiv;
	}

/*
	TRACE( "SampPerFrame:%5d DivNum:%3d Pos:%3d NUM:%5d QueueRearOffset:%6d XCnt:%5d XCntOffset:%3d\n", 
			  m_nSampPerFrame, 
			  m_nNumOfDiv,
			  nPosition,
			  g_psQueue->num,
			  m_nQueueRearOffset,
			  m_nXCnt,
			  m_nXCntOffset
			  );
*/
	TRACE( "        <-Calc\n" );
	g_ccsRWData.Unlock();
//	TRACE( "  >>Leave Calculate\n" );
}

//-----------------------------------------------------------------------------
// Name: UpdateFrame()
// Desc: 
//-----------------------------------------------------------------------------
void COscillDlg::UpdateFrame( BYTE byDrawMask )
{
// 	start = clock();
//	TRACE( "Enter UpdateFrame\n" );
	
	g_COGL.m_sDspyThreadPara.byDrawMask = byDrawMask;
	g_eventDspy.SetEvent();
/*
	int		xCnt;
	m_nXCnt %= m_nSampPerFrame;	// make sure m_nXCnt < m_nSampPerFrame   
								// when m_nSampPerFrameis reduced in 
								// spneditTimebaseScale()

	xCnt = m_nXCnt + m_nXCntOffset + 1;			// add 1 so that fill the display area
	
 	if ( m_nQueueRearOffset < 0 )	// when the nPosition is smaller than  
 	{								// m_nNumOfDiv, m_nQueueRearOffset != 0
 		xCnt = m_nSampPerFrame;
 	}

	if ( xCnt > MAX_SAMP_RECORD )
	{
		xCnt = MAX_SAMP_RECORD;
		bClearFlag = true;
	}
	
//	wglMakeCurrent( g_COGL.hDC, g_COGL.hRC1 );
	
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();
	
	if ( bClearFlag || 0 == m_nXCnt )
	{
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	}

	g_COGL.DrawGrid( GLLEFT, GLRIGHT, GLBOTTOM, GLTOP );

	glTranslatef( m_fXPosition * GLWITDH / 12, 
		          m_fYPositionA * GLHEIGHT / 6 + 150.0f, 
				  0.1f );
	glScalef( ( GLWITDH / m_nSampPerFrame ), 
			  1 / c_fVSCoef[ m_byScaleA ], 1.0f );
	glColor3ub( 255, 0, 0 );
	
 	long x = g_psQueue->rear + m_nQueueRearOffset;
// 	xCnt = (int)fSampPerFrame < g_psQueue->num ? (int)fSampPerFrame : g_psQueue->num;
//	TRACE( "\txCnt:%d", xCnt );
	glBegin( GL_LINE_STRIP );
	{
		for( xCnt--; xCnt >= 0 ; xCnt-- )
		{
			glVertex2f( (GLfloat)xCnt, (GLfloat)g_psQueue->nData[x] );
			x--;
			if ( -1 == x )
			{
				x = MAX_SAMP_RECORD - 1;
			}
		}
	}
	glEnd();

	glFinish();

// 	finish = clock();
// 	duration = (double)(finish - start);
// 	TRACE( "UpdateFrame:\t%f\n", duration );

//	wglMakeCurrent(NULL, NULL);

//	TRACE( "Leave UpdateFrame\n" );
*/
}


void 
COscillDlg::OnDeltapos_spneditTimebaseScale(NMHDR* pNMHDR, LRESULT* pResult) 
{
//	TRACE( ">>Enter OnDeltapos_spneditTimebaseScale()\n" );
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	// TODO: Add your control notification handler code here
    
    if( 0 < pNMUpDown->iDelta )
    {
        if( m_byTbScale <= 0 ||
		   (int)(g_sMeasPara.nSampFreq * c_fTbScaleCoef[m_byTbScale - 1]) <= 0 )
        {
            return;
        }
        m_byTbScale--;
    }
    else if( pNMUpDown->iDelta < 0 )
    {
        if( m_byTbScale >= ( TIMEBASESCALE_NUM - 1 ) )
        {
            return;
        }
        m_byTbScale++;
    }
//	TRACE( "m_byTbScale:%d\n", (int)m_byTbScale );
    m_editTimebaseScale.Format( _T( "%s" ), pc_szTbScale[m_byTbScale] );
//	TRACE( "pc_szTbScale[m_byTbScale]:%s\n", pc_szTbScale[m_byTbScale] );

    UpdateData( FALSE );

//	Calculate();

	UpdateFrame( CLEAR | CALCULATE );

	*pResult = 0;
//	TRACE( "<<Leave OnDeltapos_spneditTimebaseScale()\n" );
}

void COscillDlg::OnDeltapos_spneditXPosition(NMHDR* pNMHDR, LRESULT* pResult) 
{
    NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
    // TODO: Add your control notification handler code here
    if( 0 < pNMUpDown->iDelta )
    {
        if( m_fXPosition <= -4.9 )
        {
            return;
        }
        m_fXPosition -= 0.2f;
    }
    else if( pNMUpDown->iDelta < 0 )
    {
        if( m_fXPosition >= 4.9 )
        {
            return;
        }
        m_fXPosition += 0.2f;
    }
    
    m_editXPosition.Format( _T( "%.1f" ), m_fXPosition );
    
    UpdateData(FALSE);

	UpdateFrame( CLEAR );

    *pResult = 0;
}

void COscillDlg::OnDeltapos_spneditScaleA(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	// TODO: Add your control notification handler code here
    if( 0 < pNMUpDown->iDelta )
    {
        if( m_byScaleA <= 0 )
        {
            return;
        }
        m_byScaleA--;
    }
    else if( pNMUpDown->iDelta < 0 )
    {
        if( m_byScaleA >= ( VOLTAGESCALE_NUM - 1 ) )
        {
            return;
        }
        m_byScaleA++;
    }
    
    m_editScaleA.Format( _T( "%s" ), pc_szVoltScale[m_byScaleA] );
    
    UpdateData(FALSE);
	
	UpdateFrame( CLEAR );

    *pResult = 0;
}

void COscillDlg::OnDeltapos_spneditYPositionA(NMHDR* pNMHDR, LRESULT* pResult) 
{
    NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
    // TODO: Add your control notification handler code here
    if( 0 < pNMUpDown->iDelta )
    {
        if( m_fYPositionA <= -4.9 )
        {
            return;
        }
        m_fYPositionA -= 0.2f;
    }
    else if( pNMUpDown->iDelta < 0 )
    {
        if( m_fYPositionA >= 4.9 )
        {
            return;
        }
        m_fYPositionA += 0.2f;
    }
    
    m_editYPositionA.Format( _T( "%.1f" ), m_fYPositionA );
    
    UpdateData(FALSE);
	
	UpdateFrame( CLEAR );

    *pResult = 0;
}

void COscillDlg::OnDeltapos_spneditScaleB(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	// TODO: Add your control notification handler code here
    if( 0 < pNMUpDown->iDelta )
    {
        if( m_byScaleB <= 0 )
        {
            return;
        }
        m_byScaleB--;
    }
    else if( pNMUpDown->iDelta < 0 )
    {
        if( m_byScaleB >= ( VOLTAGESCALE_NUM - 1 ) )
        {
            return;
        }
        m_byScaleB++;
    }
    
    m_editScaleB.Format( _T( "%s" ), pc_szVoltScale[m_byScaleB] );
    
    UpdateData(FALSE);
	
	UpdateFrame( CLEAR );

	*pResult = 0;
}

void COscillDlg::OnDeltapos_spneditYPositionB(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	// TODO: Add your control notification handler code here
	if( 0 < pNMUpDown->iDelta )
    {
        if( m_fYPositionB <= -4.9 )
        {
            return;
        }
        m_fYPositionB -= 0.2f;
    }
    else if( pNMUpDown->iDelta < 0 )
    {
        if( m_fYPositionB >= 4.9 )
        {
            return;
        }
        m_fYPositionB += 0.2f;
    }
    
    m_editYPositionB.Format( _T( "%.1f" ), m_fYPositionB );
    
    UpdateData(FALSE);
	
	UpdateFrame( CLEAR );

	*pResult = 0;
}

void 
COscillDlg::OnDeltapos_spneditTriggerLevel(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	// TODO: Add your control notification handler code here
    if( 0 < pNMUpDown->iDelta )
    {
        if( m_fTriggerLevel <= -4.95 )
        {
            return;
        }
        m_fTriggerLevel -= 0.1f;
    }
    else if( pNMUpDown->iDelta < 0 )
    {
        if( m_fTriggerLevel >= 4.95 )
        {
            return;
        }
        m_fTriggerLevel += 0.1f;
    }
    
    m_editTriggerLevel.Format( _T( "%.1f" ), m_fTriggerLevel );
    
    UpdateData(FALSE);
	
	*pResult = 0;
}

void 
COscillDlg::OnDeltapos_spneditTriggerLevelUnit(NMHDR* pNMHDR, LRESULT* pResult)
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	// TODO: Add your control notification handler code here
    if( 0 < pNMUpDown->iDelta )
    {
        if( 1 == m_byTriggerLevelUnit )
        {
            m_byTriggerLevelUnit = 0;
            m_editTriggerLevelUnit.Format( _T( "mV" ) );
        }
    }
    else if( pNMUpDown->iDelta < 0 )
    {
        if( 0 == m_byTriggerLevelUnit )
        {
            m_byTriggerLevelUnit = 1;
            m_editTriggerLevelUnit.Format( _T( "V" ) );
        }
    }
	
    UpdateData(FALSE);
	*pResult = 0;
}



void COscillDlg::btnTimebase( BYTE n )
{
    m_byCoordinateMode = n;
    
    ::SendMessage( 
		GetDlgItem( IDC_btnTimebaseY_T )->m_hWnd, BM_SETSTATE,FALSE,0);
    ::SendMessage( 
		GetDlgItem( IDC_btnTimebaseAdd )->m_hWnd, BM_SETSTATE,FALSE,0);
    ::SendMessage( 
		GetDlgItem( IDC_btnTimebaseB_A )->m_hWnd, BM_SETSTATE,FALSE,0);
    ::SendMessage( 
		GetDlgItem( IDC_btnTimebaseA_B )->m_hWnd, BM_SETSTATE,FALSE,0);
	
    switch( n )
    {
    case 1:
        {
			::SendMessage( 
				GetDlgItem( IDC_btnTimebaseY_T )->m_hWnd, BM_SETSTATE,TRUE,0);
			m_ctrlTimebaseScale.EnableWindow( TRUE );
			m_ctrlXPosition.EnableWindow( TRUE );
			m_ctrlSpnTimebaseScale.EnableWindow( TRUE );
			m_ctrlSpnXPosition.EnableWindow( TRUE );
			m_ctrlYPosition_B.EnableWindow( TRUE );
			m_ctrlSpnYPosition_B.EnableWindow( TRUE );
            break;
        }
    case 2:
        {
			::SendMessage( 
				GetDlgItem( IDC_btnTimebaseAdd )->m_hWnd, BM_SETSTATE,TRUE,0);
			m_ctrlTimebaseScale.EnableWindow( TRUE );
			m_ctrlXPosition.EnableWindow( TRUE );
			m_ctrlSpnTimebaseScale.EnableWindow( TRUE );
			m_ctrlSpnXPosition.EnableWindow( TRUE );
			m_ctrlYPosition_B.EnableWindow( FALSE );
			m_ctrlSpnYPosition_B.EnableWindow( FALSE );
            break;
        }
    case 3:
        {
            ::SendMessage( 
				GetDlgItem( IDC_btnTimebaseB_A )->m_hWnd, BM_SETSTATE,TRUE,0);
			m_ctrlTimebaseScale.EnableWindow( FALSE );
			m_ctrlXPosition.EnableWindow( FALSE );
			m_ctrlSpnTimebaseScale.EnableWindow( FALSE );
			m_ctrlSpnXPosition.EnableWindow( FALSE );
			m_ctrlYPosition_B.EnableWindow( TRUE );
			m_ctrlSpnYPosition_B.EnableWindow( TRUE );
            break;
        }
    case 4:
        {
            ::SendMessage( 
				GetDlgItem( IDC_btnTimebaseA_B )->m_hWnd, BM_SETSTATE,TRUE,0);
			m_ctrlTimebaseScale.EnableWindow( FALSE );
			m_ctrlXPosition.EnableWindow( FALSE );
			m_ctrlSpnTimebaseScale.EnableWindow( FALSE );
			m_ctrlSpnXPosition.EnableWindow( FALSE );
			m_ctrlYPosition_B.EnableWindow( TRUE );
			m_ctrlSpnYPosition_B.EnableWindow( TRUE );
            break;
        }
    }
}

void COscillDlg::OnbtnTimebaseY_T() 
{
    btnTimebase( 1 );
}

void COscillDlg::OnbtnTimebaseAdd() 
{
	btnTimebase( 2 );
}

void COscillDlg::OnbtnTimebaseB_A() 
{
	btnTimebase( 3 );
}

void COscillDlg::OnbtnTimebaseA_B() 
{
	btnTimebase( 4 );
}

void COscillDlg::btn_A( BYTE n )
{
    m_byChannelAMode = n;
    
    ::SendMessage( 
		GetDlgItem( IDC_btnAC_A )->m_hWnd, BM_SETSTATE,FALSE,0);
    ::SendMessage( 
		GetDlgItem( IDC_btnZero_A )->m_hWnd, BM_SETSTATE,FALSE,0);
    ::SendMessage( 
		GetDlgItem( IDC_btnDC_A )->m_hWnd, BM_SETSTATE,FALSE,0);
    
    switch( n )
    {
    case 1:
        {
            ::SendMessage( 
				GetDlgItem( IDC_btnAC_A )->m_hWnd, BM_SETSTATE,TRUE,0);
            break;
        }
    case 2:
        {
            ::SendMessage( 
				GetDlgItem( IDC_btnZero_A )->m_hWnd, BM_SETSTATE,TRUE,0);
            break;
        }
    case 3:
        {
            ::SendMessage( 
				GetDlgItem( IDC_btnDC_A )->m_hWnd, BM_SETSTATE,TRUE,0);
            break;
        }
    }
}

void COscillDlg::OnbtnAC_A() 
{
	btn_A( 1 );
}

void COscillDlg::OnbtnZero_A() 
{
	btn_A( 2 );
}

void COscillDlg::OnbtnDC_A() 
{
	btn_A( 3 );
}


void COscillDlg::btn_B( BYTE n )
{
    m_byChannelBMode = n;
    
    ::SendMessage( GetDlgItem( IDC_btnAC_B )->m_hWnd, BM_SETSTATE,FALSE,0);
    ::SendMessage( GetDlgItem( IDC_btnZero_B )->m_hWnd, BM_SETSTATE,FALSE,0);
    ::SendMessage( GetDlgItem( IDC_btnDC_B )->m_hWnd, BM_SETSTATE,FALSE,0);
    
    switch( n )
    {
    case 1:
        {
            ::SendMessage( 
				GetDlgItem( IDC_btnAC_B )->m_hWnd, BM_SETSTATE,TRUE,0);
            break;
        }
    case 2:
        {
            ::SendMessage( 
				GetDlgItem( IDC_btnZero_B )->m_hWnd, BM_SETSTATE,TRUE,0);
            break;
        }
    case 3:
        {
            ::SendMessage( 
				GetDlgItem( IDC_btnDC_B )->m_hWnd, BM_SETSTATE,TRUE,0);
            break;
        }
    }
}

void COscillDlg::OnbtnAC_B() 
{
	btn_B( 1 );
}

void COscillDlg::OnbtnZero_B() 
{
	btn_B( 2 );
}

void COscillDlg::OnbtnDC_B() 
{
	btn_B( 3 );
}


void COscillDlg::btnTriggerMode( BYTE n )
{
    m_byTriggerMode = n;
    
    ::SendMessage( 
		GetDlgItem( IDC_btnTriggerSingle )->m_hWnd, BM_SETSTATE,FALSE,0);

    ::SendMessage( 
		GetDlgItem( IDC_btnTriggerNormal )->m_hWnd, BM_SETSTATE,FALSE,0);

    ::SendMessage( 
		GetDlgItem( IDC_btnTriggerAuto )->m_hWnd, BM_SETSTATE,FALSE,0);

    ::SendMessage( 
		GetDlgItem( IDC_btnTriggerNone )->m_hWnd, BM_SETSTATE,FALSE,0);
    
    switch( n )
    {
    case 1:
        {
            ::SendMessage( 
				GetDlgItem( IDC_btnTriggerSingle )->m_hWnd, BM_SETSTATE,TRUE,0);
            break;
        }
    case 2:
        {
            ::SendMessage( 
				GetDlgItem( IDC_btnTriggerNormal )->m_hWnd, BM_SETSTATE,TRUE,0);
            break;
        }
    case 3:
        {
            ::SendMessage( 
				GetDlgItem( IDC_btnTriggerAuto )->m_hWnd, BM_SETSTATE,TRUE,0);
            break;
        }
    case 4:
        {
            ::SendMessage( 
				GetDlgItem( IDC_btnTriggerNone )->m_hWnd, BM_SETSTATE,TRUE,0);
			m_ctrlTrigger_A.SetState( FALSE );
			m_ctrlTrigger_B.SetState( FALSE );
			m_ctrlTrigger_A.EnableWindow( FALSE );
			m_ctrlTrigger_B.EnableWindow( FALSE );
            return;
        }
    }

	m_ctrlTrigger_A.EnableWindow( TRUE );
	m_ctrlTrigger_B.EnableWindow( TRUE );

	if ( 1 == m_byTriggerCH )
	{
		m_ctrlTrigger_A.SetState( TRUE );
	}
	else if ( 2 == m_byTriggerCH )
	{
		m_ctrlTrigger_B.SetState( TRUE );
	}
}

void COscillDlg::OnbtnTriggerSingle() 
{
	btnTriggerMode( 1 );
}

void COscillDlg::OnbtnTriggerNormal() 
{
	btnTriggerMode( 2 );
}

void COscillDlg::OnbtnTriggerAuto() 
{
	btnTriggerMode( 3 );
}

void COscillDlg::OnbtnTriggerNone() 
{
	btnTriggerMode( 4 );
}


void COscillDlg::OnbtnTriggerRising() 
{
	m_byTriggerEdge = 1;
	::SendMessage( 
		GetDlgItem( IDC_btnTriggerRising )->m_hWnd, BM_SETSTATE,TRUE,0);
    ::SendMessage( 
		GetDlgItem( IDC_btnTriggerFalling )->m_hWnd, BM_SETSTATE,FALSE,0);
}

void COscillDlg::OnbtnTriggerFalling() 
{
	m_byTriggerEdge = 2;
	::SendMessage( 
		GetDlgItem( IDC_btnTriggerFalling )->m_hWnd, BM_SETSTATE,TRUE,0);
    ::SendMessage( 
		GetDlgItem( IDC_btnTriggerRising )->m_hWnd, BM_SETSTATE,FALSE,0);
}

void COscillDlg::OnbtnTrigger_A() 
{
	m_byTriggerCH = 1;
    ::SendMessage( 
		GetDlgItem( IDC_btnTrigger_A )->m_hWnd, BM_SETSTATE,TRUE,0);
    ::SendMessage( 
		GetDlgItem( IDC_btnTrigger_B )->m_hWnd, BM_SETSTATE,FALSE,0);
}

void COscillDlg::OnbtnTrigger_B() 
{
	m_byTriggerCH = 2;
	::SendMessage( 
		GetDlgItem( IDC_btnTrigger_B )->m_hWnd, BM_SETSTATE,TRUE,0);
    ::SendMessage( 
		GetDlgItem( IDC_btnTrigger_A )->m_hWnd, BM_SETSTATE,FALSE,0);
}

void COscillDlg::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized) 
{
	CDialog::OnActivate(nState, pWndOther, bMinimized);
	
	// TODO: Add your message handler code here
	g_bActive = !( ( BOOL )HIWORD( GetCurrentMessage()->wParam ) );
}


void COscillDlg::OnComsetting() 
{
	// TODO: Add your control notification handler code here
	CComSetting	setDlg;
	
//	CRect	pRectMainDlg;
	
//	GetWindowRect( pRectMainDlg );
//	TRACE( "Height:%d, Width:%d\n", pRectMainDlg.Height(), pRectMainDlg.Width() );
	
	setDlg.DoModal();
}

void COscillDlg::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	m_bRecvThreadRunning = false;
	g_eventCommRecv.SetEvent();
	if(	WAIT_OBJECT_0 != WaitForSingleObject( m_pRecvThread->m_hThread, 100 ) )
	{
		TerminateThread( m_pRecvThread->m_hThread, -2 );
		TRACE( "RecvThread has been terminated!(-2)\n" );
	}
	
	g_COGL.m_bDspyThreadRunning = false;
	g_eventDspy.SetEvent();
	
	//glClear would cost 30-80ms
	if(	WAIT_OBJECT_0 !=							
		WaitForSingleObject( g_COGL.m_pDspyThread->m_hThread, 100 ) )	
	{
		TerminateThread( g_COGL.m_pDspyThread->m_hThread, -1 );
		TRACE( "DspyThread has been terminated!(-1)\n" );
	}
	
	delete g_psQueue;
	
	CDialog::OnClose();
}

void COscillDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	OnClose();

	CDialog::OnOK();
}

void COscillDlg::OnBnClickedbtnreverse()
{
	// TODO: 在此添加控件通知处理程序代码
	int i;
	SERIALPORTINFO *psSerialPortInfo = EnumSerialPort();
	
	if ( NULL != psSerialPortInfo )
	{
		for ( i = 1; i <= psSerialPortInfo->nNum ; i++ )
		{
			TRACE( "No.%d,%s\n", i, psSerialPortInfo->pszList[i] );
		}
		
	}
	
	delete psSerialPortInfo;

}

void COscillDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// TODO: Add your message handler code here and/or call default
	if ( NULL != pScrollBar 
		&& m_ctrlSbDrawFrame.GetDlgCtrlID() == pScrollBar->GetDlgCtrlID() )
	{
		int nPosition = m_ctrlSbDrawFrame.GetScrollPos();
		
		if( SB_LINELEFT == nSBCode )
		{
			nPosition--;
		}
		else if(SB_PAGELEFT == nSBCode )
		{
			nPosition -= 10;
		}
		else if( SB_LINERIGHT == nSBCode )
		{
			nPosition++;
		}
		else if(SB_PAGERIGHT == nSBCode )
		{
			nPosition += 10;
		}
		else if( SB_LEFT == nSBCode )
		{	
			nPosition = 0;
		}
		else if( SB_RIGHT == nSBCode )
		{	
			nPosition = m_nNumOfDiv;
		}
		else if (SB_THUMBTRACK == nSBCode )
		{
			SCROLLINFO scrollInfo; 
			scrollInfo.cbSize = sizeof( scrollInfo );
			pScrollBar->GetScrollInfo( &scrollInfo, SIF_TRACKPOS );
			nPosition = scrollInfo.nTrackPos;
		}
		else
		{
			return;
		}
		
		if(nPosition < 0 )
		{
			nPosition = 0;
		}
		if( nPosition > m_nNumOfDiv )
		{
			nPosition = m_nNumOfDiv;
		}
		
		pScrollBar->SetScrollPos( nPosition );


//		Calculate();
//		TRACE( "  m_nQRO = %d, m_nSPF = %d", 
//			   m_nQueueRearOffset, m_nSampPerFrame );
		UpdateFrame( CLEAR | CALCULATE );
	}
	
	CDialog::OnHScroll( nSBCode, nPos, pScrollBar );
}

void COscillDlg::OnbtnMSCommSend() 
{
	// TODO: Add your control notification handler code here

	UpdateData( true );
	
	CByteArray sendArr; 
	WORD wLength;

	wLength = m_szMSCommSend.GetLength();
	sendArr.SetSize( wLength );
	for( int i =0; i<wLength; i++ )
	{
		sendArr.SetAt( i, m_szMSCommSend.GetAt( i ) );
	}
//	m_mscom.SetOutput( COleVariant(sendArr) );
}

BEGIN_EVENTSINK_MAP(COscillDlg, CDialog)
    //{{AFX_EVENTSINK_MAP(COscillDlg)
//	ON_EVENT(COscillDlg, IDC_MSCOMM1, 1 /* OnComm */, OnCommMscomm1, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

void COscillDlg::OnCommMscomm1() 
{
	// TODO: Add your control notification handler code here
//	start = clock();

// 	static int		noldPos = 0;
// 	static bool		bRecvDigitHigh = true;
// 	static SHORT	nTmpH = 0;
// 	static SHORT	nTmpL = 0;

//	UpdateData( true );
	
//	VARIANT variant_inp;
//	COleSafeArray safearray_inp;
	
// 	long i = 0;
// 	int len;
// 	char szRxData[1024];
// 	CString tmp;
/*
	switch ( m_mscom.GetCommEvent() )
	{
	case 2:
		{
			g_eventCommRecv.SetEvent();
*/
/*
			variant_inp = m_mscom.GetInput();
			
			safearray_inp = variant_inp;
			len = safearray_inp.GetOneDimSize();
			for ( i = 0; i < len; i++ )
			{
				safearray_inp.GetElement( &i, &szRxData[i] );
//				szRxData[i] &= 0xff;
//				TRACE( "0x%08x ", szRxData[i] );
			}
//			szRxData[i] = '\0';
			//************************************************************************
//			TRACE( "%s \n", szRxData );
//			TRACE( "\n" );

// 						if( bRecvDigitHigh )
// 						{
// 							nTmpH = szRxData[0] & 0xff;
// 							nTmpH = nTmpH << 8;
// 							TRACE( "\nH %02x ", nTmpH );
// 						}
// 						else
// 						{


				nTmpL = szRxData[0] & 0xff;
				TRACE( "\nL %02x", nTmpL );
				INSQueue( g_psQueue, (SHORT)( ( /*nTmpH | *///nTmpL ) ) ); 

/*
				m_nXCnt++;
				if ( m_nXCnt >= m_nSampPerFrame )
				{
					m_nXCnt %= m_nSampPerFrame;
				}
				
				m_nNumOfDiv = ( g_psQueue->num - m_nXCnt ) / ( m_nSampPerFrame / 12 );
				m_ctrlSbDrawFrame.SetScrollRange( 0, m_nNumOfDiv );
				
				noldPos = m_ctrlSbDrawFrame.GetScrollPos();
				m_ctrlSbDrawFrame.SetScrollPos( m_nNumOfDiv, FALSE );
				
				if ( noldPos != m_nNumOfDiv )
				{
					m_nQueueRearOffset = 0;
					UpdateFrame( CLEAR );
				}
				else
				{
					UpdateFrame( BASIC );
				}
*/

/*
			}
			bRecvDigitHigh = !bRecvDigitHigh;
*/

//			INSQueue( g_psQueue, atoi( szRxData ) );
			
			// There are still a problem is that when  
			// m_nSampPerFrame is greater than MAX_SAMP_RECORD. 
			// Temporarily ignored.

			
			/************************************************************************/
//  			m_szMSCommRecv += szRxData;
//  			UpdateData( false );
/*
			break;
		}
	default:
		{
			break;
		}
	}
*/


// 	finish = clock();
// 	duration = (double)(finish - start);
// 	TRACE( "OnCommMscomm1:\t%f\n", duration );
}

void COscillDlg::OnBtnComOpen() 
{
	// TODO: Add your control notification handler code here
/*
	if( !m_mscom.GetPortOpen() )
	{
		m_mscom.SetPortOpen( TRUE );
		m_ctrlBtnComOpen.SetWindowText( "Close" );
		TRACE( "COM Open!\n" );
	}
	else
	{
		m_mscom.SetPortOpen( FALSE );
		m_ctrlBtnComOpen.SetWindowText( "Open" );
		TRACE( "COM Close!\n" );
	}*/

}

