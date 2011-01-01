// ComSetting.cpp : implementation file
//

#include "..\..\sources\Dialog\StdAfx.h"
#include "..\..\sources\Dialog\Oscill.h"
#include "..\..\sources\Dialog\ComSetting.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern COscillDlg *g_pMainThis;
/////////////////////////////////////////////////////////////////////////////
// CComSetting dialog


CComSetting::CComSetting(CWnd* pParent /*=NULL*/)
	: CDialog(CComSetting::IDD, pParent)
{
	//{{AFX_DATA_INIT(CComSetting)
	m_rdoFlow = -1;
	//}}AFX_DATA_INIT
}


void CComSetting::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CComSetting)
	DDX_Control(pDX, IDC_cboDataBits, m_cboDataBits);
	DDX_Control(pDX, IDC_cboParity, m_cboParity);
	DDX_Control(pDX, IDC_cboStopBits, m_cboStopBits);
	DDX_Control(pDX, IDC_cboSpeed, m_cboSpeed);
	DDX_Control(pDX, IDC_cboPort, m_cboPort);
	DDX_Radio(pDX, IDC_rdoFlow, m_rdoFlow);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CComSetting, CDialog)
	//{{AFX_MSG_MAP(CComSetting)
	ON_BN_CLICKED(IDC_btnApply, OnbtnApply)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDOK, &CComSetting::OnBnClickedOk)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CComSetting message handlers

BOOL CComSetting::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	m_cboPort.AddString( _T( "COM 1" ) );
	m_cboPort.AddString( _T( "COM 2" ) );
	m_cboPort.AddString( _T( "COM 3" ) );
	m_cboPort.AddString( _T( "COM 4" ) );
	m_cboPort.AddString( _T( "COM 5" ) );
	m_cboPort.AddString( _T( "COM 6" ) );
	m_cboPort.AddString( _T( "COM 7" ) );
	m_cboPort.AddString( _T( "COM 8" ) );
	m_cboPort.SetCurSel( 7 );

	m_cboDataBits.AddString( _T( "4" ) );
	m_cboDataBits.AddString( _T( "5" ) );
	m_cboDataBits.AddString( _T( "6" ) );
	m_cboDataBits.AddString( _T( "7" ) );
	m_cboDataBits.AddString( _T( "8" ) );
	m_cboDataBits.SetCurSel( 4 );

	m_cboParity.AddString( _T( "None" ) );
	m_cboParity.AddString( _T( "Even" ) );
	m_cboParity.AddString( _T( "Odd" ) );
	m_cboParity.AddString( _T( "Mark" ) );
	m_cboParity.AddString( _T( "Space" ) );
	m_cboParity.SetCurSel( 0 );

	m_cboStopBits.AddString( _T( "1" ) );
	m_cboStopBits.AddString( _T( "1.5" ) );
	m_cboStopBits.AddString( _T( "2" ) );
	m_cboStopBits.SetCurSel( 0 );

	m_cboSpeed.AddString( _T( "600" ) );	//0
    m_cboSpeed.AddString( _T( "1200" ) );
    m_cboSpeed.AddString( _T( "2400" ) );
    m_cboSpeed.AddString( _T( "4800" ) );
    m_cboSpeed.AddString( _T( "9600" ) );
    m_cboSpeed.AddString( _T( "14400" ) );	//5
    m_cboSpeed.AddString( _T( "19200" ) );
    m_cboSpeed.AddString( _T( "28800" ) );
    m_cboSpeed.AddString( _T( "38400" ) );
    m_cboSpeed.AddString( _T( "56000" ) );
    m_cboSpeed.AddString( _T( "57600" ) );	//10
    m_cboSpeed.AddString( _T( "115200" ) );
    m_cboSpeed.AddString( _T( "128000" ) );
    m_cboSpeed.AddString( _T( "256000" ) );
	m_cboSpeed.SetCurSel( 11 );

	m_rdoFlow = 0;
	UpdateData( FALSE );

// 	RECT *pDlgRect = new RECT;
// 	pDlgRect->top    = 0;
// 	pDlgRect->bottom = 482;
// 	pDlgRect->left   = 0;
// 	pDlgRect->right  = 631;
// 	
// 	MoveWindow( pDlgRect );
// 	
// 	free( pDlgRect );

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CComSetting::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	OnbtnApply();

	CDialog::OnOK();
}

void CComSetting::OnbtnApply() 
{
	// TODO: Add your control notification handler code here
/*
	if ( -1 < m_cboPort.GetCurSel() )
	{
		g_pMainThis->m_mscom.SetCommPort( m_cboPort.GetCurSel() + 1 );
	}
	
	g_pMainThis->m_mscom.SetInBufferSize( 1024 );
	g_pMainThis->m_mscom.SetOutBufferSize( 512 );
	g_pMainThis->m_mscom.SetInputMode( 1 );
	
	CString szSpeed;
	m_cboSpeed.GetWindowText( szSpeed );
	TRACE( "szSpeed:%s\n", szSpeed );
	
	CString szParity;
	m_cboParity.GetWindowText( szParity );
	szParity.MakeLower();
	szParity = szParity.GetAt( 0 );
	TRACE( "szParity:%s\n", szParity );
	
	
	CString szDataBits;
	m_cboDataBits.GetWindowText( szDataBits );
	TRACE( "szDataBits:%s\n", szDataBits );
	
	CString szStopBits;
	m_cboStopBits.GetWindowText( szStopBits );
	TRACE( "szStopBits:%s\n", szStopBits );
	
	char	szSet[50];
	sprintf_s( szSet, "%s,%s,%s,%s", szSpeed, szParity, szDataBits, szStopBits );
	TRACE( "szSet:%s\n", szSet );
	
	g_pMainThis->m_mscom.SetSettings( szSet );
	
	g_pMainThis->m_mscom.SetRThreshold( 1 );
	g_pMainThis->m_mscom.SetInputLen( 0 );

*/

	BYTE* pBuf = new BYTE[10000];

	try
	{
		COMMCONFIG config;
		CSerialPort::GetDefaultConfig(1, config);

		CSerialPort port;
		port.Open(1, 1200, CSerialPort::NoParity, 8, CSerialPort::OneStopBit, CSerialPort::XonXoffFlowControl);

		HANDLE hPort = port.Detach();
		port.Attach(hPort);

		DWORD dwModemStatus;
		port.GetModemStatus(dwModemStatus);

		DCB dcb;
		port.GetState(dcb);

		dcb.BaudRate = 9600;
		port.SetState(dcb);    

		DWORD dwErrors;                      
		port.ClearError(dwErrors);

/*
		port.SetBreak();
		port.ClearBreak();
*/

		COMSTAT stat;
		port.GetStatus(stat);

		DWORD dwBytesWaiting = port.BytesWaiting();
		dwBytesWaiting;

		COMMTIMEOUTS timeouts;
		port.GetTimeouts(timeouts);

		port.Setup(10000, 10000);

		port.GetConfig(config);

		config.dcb.BaudRate = 9600;
		port.SetConfig(config);

		port.Set0WriteTimeout();
		port.Set0ReadTimeout();

		char sBuf[] = "This should appear on the serial port";
		port.Write(sBuf, static_cast<DWORD>(strlen(sBuf)));

		DWORD dwMask;
		port.GetMask(dwMask);

		port.SetMask(EV_TXEMPTY); 

		//port.WaitEvent(dwMask);

		//使用常量PURGE_TXABORT调用Purge,终止所有写操作,并立即返回,无论写操作有没有完成
		port.TerminateOutstandingWrites();

/*
		//最高的优先权发送一个字符
		port.TransmitChar('p');
*/
		//发送和接受超时配置为0
		port.Set0Timeout();

		char sRxBuf[10];
		DWORD dwRead = port.Read(sRxBuf, 10);
		dwRead; //To remove unreferrenced variable in VC 6.

		port.TerminateOutstandingReads();

		port.ClearDTR();

		port.ClearRTS();

		port.SetDTR();

		port.SetRTS();

		port.SetXOFF();

		port.SetXON();

		COMMPROP properties;
		port.GetProperties(properties);

		port.ClearWriteBuffer();

		port.ClearReadBuffer();

		port.Flush();

		port.Close();

/*
		//Try out the overlapped functions
		CSerialPort port2;
		port2.Open(1, 9600, CSerialPort::NoParity, 8, CSerialPort::OneStopBit, CSerialPort::XonXoffFlowControl, TRUE);

		CEvent event(FALSE, TRUE);
		OVERLAPPED overlapped;
		memset(&overlapped, 0, sizeof(overlapped));
		overlapped.hEvent = event;

		try
		{
			port2.Write(pBuf, 10000, overlapped);
		}
		catch(CSerialException* pEx)
		{
			if (pEx->m_dwError == ERROR_IO_PENDING)
			{
				DWORD dwBytesTransferred = 0;
				port2.GetOverlappedResult(overlapped, dwBytesTransferred, TRUE);
				pEx->Delete();
			}
			else
			{
				DWORD dwError = pEx->m_dwError;
				pEx->Delete();
				CSerialPort::ThrowSerialException(dwError);
			}
		}
		try
		{
			port2.Read(pBuf, 10, overlapped);
		}
		catch(CSerialException* pEx)
		{
			if (pEx->m_dwError == ERROR_IO_PENDING)
			{
				DWORD dwBytesTransferred = 0;
				port2.GetOverlappedResult(overlapped, dwBytesTransferred, TRUE);
				pEx->Delete();
			}
			else
			{
				DWORD dwError = pEx->m_dwError;
				pEx->Delete();
				CSerialPort::ThrowSerialException(dwError);
			}
		}

		port2.SetMask(EV_TXEMPTY); 
*/
		/* for testing on NT only
		port2.WriteEx(sBuf, static_cast<DWORD>(strlen(sBuf)));
		SleepEx(INFINITE, TRUE);
		port2.ReadEx(pBuf, 10);
		SleepEx(INFINITE, TRUE);
		*/

	}
	catch (CSerialException* pEx)
	{
		TRACE(_T("Handle Exception, Message:%s\n"), pEx->GetErrorMessage().operator LPCTSTR());
		pEx->Delete();
	}

	delete [] pBuf;
}

void CComSetting::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}

