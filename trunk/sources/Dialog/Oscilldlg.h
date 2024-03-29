// OscillDlg.h : header file
//
//{{AFX_INCLUDES()
#include "..\..\sources\SerialPort\mscomm.h"
//}}AFX_INCLUDES

#if !defined(AFX_OscillDLG_H__F2839C20_A0BB_4252_8456_EFCB2829B965__INCLUDED_)
#define AFX_OscillDLG_H__F2839C20_A0BB_4252_8456_EFCB2829B965__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// COscillDlg dialog

class COscillDlg : public CDialog
{
// Construction
public:
	COscillDlg(CWnd* pParent = NULL);	// standard constructor

	CWinThread  *m_pRecvThread;
	bool	m_bRecvThreadRunning;

	void	UpdateFrame( BYTE byDrawMask );
	
	
	//Timebase
    void    btnTimebase( BYTE n );

    //Channel A
	void    btn_A( BYTE n );

	//Channel B
    void    btn_B( BYTE n );

	//Trigger
    void    btnTriggerMode( BYTE n );

	//Calculate
    void	Calculate( void );
	int		m_nXCnt;				//Count the the number of points to display
	int		m_nXCntOffset;			//When review the waveform,...hard to say
	int		m_nSampPerFrame;		//the points' number per frame
	int		m_nNumOfDiv;			//the number of all the divisions
	long	m_nQueueRearOffset;		//
	
    //Timebase
    BYTE	m_byTbScale;
	BYTE	m_byTbSUpperLimit;
    float	m_fXPosition;
    BYTE	m_byCoordinateMode;

    //Channel A
    BYTE    m_byScaleA;
    float   m_fYPositionA;
    BYTE    m_byChannelAMode;

    //Channel B
    BYTE    m_byScaleB;
    float   m_fYPositionB;
    BYTE    m_byChannelBMode;

    //Trigger
    BYTE    m_byTriggerEdge;
    BYTE    m_byTriggerCH;
    float   m_fTriggerLevel;
    BYTE    m_byTriggerLevelUnit;
    BYTE    m_byTriggerMode;

	CBitmapButton	m_ctrlbmpbtnRising;
	CBitmapButton	m_ctrlbmpbtnFalling;
// Dialog Data
	//{{AFX_DATA(COscillDlg)
	enum { IDD = IDD_OSCILL_DIALOG };
	CButton	m_ctrlBtnComOpen;
	CButton	m_ctrlbtnReceive;
	CScrollBar	m_ctrlSbDrawFrame;
	CEdit	m_ctrlYPosition_B;
	CSpinButtonCtrl	m_ctrlSpnYPosition_B;
	CSpinButtonCtrl	m_ctrlSpnXPosition;
	CSpinButtonCtrl	m_ctrlSpnTimebaseScale;
	CButton	m_ctrlTrigger_B;
	CButton	m_ctrlTrigger_A;
	CEdit	m_ctrlXPosition;
	CEdit	m_ctrlTimebaseScale;
	CButton	m_cbtnTimebaseY_T;
	CString	m_editTimebaseScale;
	CString	m_editXPosition;
	CString	m_editScaleA;
	CString	m_editScaleB;
	CString	m_editTriggerLevel;
	CString	m_editTriggerLevelUnit;
	CString	m_editYPositionA;
	CString	m_editYPositionB;
//	CMSComm	m_mscom;
	CString	m_szMSCommRecv;
	CString	m_szMSCommSend;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COscillDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(COscillDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnbtnTimebaseY_T();
	afx_msg void OnbtnTimebaseAdd();
	afx_msg void OnbtnTimebaseB_A();
	afx_msg void OnbtnTimebaseA_B();
	afx_msg void OnbtnAC_A();
	afx_msg void OnbtnZero_A();
	afx_msg void OnbtnDC_A();
	afx_msg void OnbtnAC_B();
	afx_msg void OnbtnZero_B();
	afx_msg void OnbtnDC_B();
	afx_msg void OnbtnTriggerSingle();
	afx_msg void OnbtnTriggerNormal();
	afx_msg void OnbtnTriggerAuto();
	afx_msg void OnbtnTriggerNone();
	afx_msg void OnbtnTriggerRising();
	afx_msg void OnbtnTriggerFalling();
	afx_msg void OnbtnTrigger_A();
	afx_msg void OnbtnTrigger_B();
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	afx_msg void OnActivateApp(BOOL bActive, HTASK hTask);
	afx_msg void OnDeltapos_spneditTimebaseScale(NMHDR* pNMHDR, LRESULT* pResult);
//	virtual void OnOK();
	afx_msg void OnDeltapos_spneditScaleA(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltapos_spneditScaleB(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltapos_spneditXPosition(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltapos_spneditYPositionA(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltapos_spneditYPositionB(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltapos_spneditTriggerLevel(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltapos_spneditTriggerLevelUnit(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnReceive();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnbtnMSCommSend();
//	afx_msg void OnCommMscomm1();
	afx_msg void OnComsetting();
	afx_msg void OnClose();
	afx_msg void OnBtnComOpen();
// 	DECLARE_EVENTSINK_MAP()
// 	//}}AFX_MSG
 	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedbtnreverse();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnBnClickedbtnsave();
};

// struct threadInfo
// {
//     COscillDlg      *that;
// };

struct SMeasPara
{
	INT32	nSampFreq;
	
};

//extern SMeasPara		g_sMeasPara;
///////////// test ///////////////
typedef struct
{
	unsigned int mantissal:32;
	unsigned int mantissah:20;
	unsigned int exponent:11;
	unsigned int sign:1;
}double_t;

//typedef struct
//{
//	unsigned int mantissal:16;
//	unsigned int mantissah:7;
//	unsigned int exponent:8;
//	unsigned int sign:1;
//}float_t;

typedef struct
{
	unsigned int sign:1;
	unsigned int exponent:8;
	unsigned int mantissah:7;
	unsigned int mantissal:16;
}float_t;

#define CVTBUFSIZE  16

#if 0
int isnan(double d);
int isinf(double d);
double my_modf02(double x, double *y);
char * cvt( double arg, int ndigits, int * decpt, int * sign, char * buf, int eflag );
char * fcvtbuf( double arg, int ndigits, int * decpt, int * sign, char * buf );
void cfltcvt( double value, char * buffer, char fmt, int precision );
#endif

void putf( float f );
float my_modf02f(float x, float *y);
int isnanf(float d);
int isinff(float d);
char * cvtf( float arg, int ndigits, int * decpt, int * sign, char * buf, int eflag );
//char * fcvtbuff( float arg, int ndigits, int * decpt, int * sign, char * buf );
void cfltcvtf( float value, char * buffer, int precision );

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OscillDLG_H__F2839C20_A0BB_4252_8456_EFCB2829B965__INCLUDED_)
