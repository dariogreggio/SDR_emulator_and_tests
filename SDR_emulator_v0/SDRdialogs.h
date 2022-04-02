#if !defined(AFX_SDRDIALOGS_H__34DBA922_25D7_46CE_9C08_596B7607BA6A__INCLUDED_)
#define AFX_SDRDIALOGS_H__34DBA922_25D7_46CE_9C08_596B7607BA6A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SDRdialogs.h : header file
//


class CAboutDlg : public CDialog {
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
		// No message handlers
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////
// CParametersDlg dialog

class CParametersDlg : public CDialog {
// Construction
public:
	CParametersDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CParametersDlg)
	enum { IDD = IDD_PARAMETERS };
	UINT	m_samplingFrequency;
	UINT	m_carrierFrequency;
	UINT	m_modulationFrequency;
	UINT	m_LocalOscFrequency;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CParametersDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CParametersDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SDRDIALOGS_H__34DBA922_25D7_46CE_9C08_596B7607BA6A__INCLUDED_)
