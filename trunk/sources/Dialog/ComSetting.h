#if !defined(AFX_COMSETTING_H__862DB9EB_F215_4A0A_B956_6E2DBA29D3D4__INCLUDED_)
#define AFX_COMSETTING_H__862DB9EB_F215_4A0A_B956_6E2DBA29D3D4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ComSetting.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CComSetting dialog

class CComSetting : public CDialog
{
// Construction
public:
	CComSetting(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CComSetting)
	enum { IDD = IDD_COMSETTING };
	CComboBox	m_cboDataBits;
	CComboBox	m_cboParity;
	CComboBox	m_cboStopBits;
	CComboBox	m_cboSpeed;
	CComboBox	m_cboPort;
	int		m_rdoFlow;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CComSetting)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CComSetting)
//	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnbtnApply();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COMSETTING_H__862DB9EB_F215_4A0A_B956_6E2DBA29D3D4__INCLUDED_)
