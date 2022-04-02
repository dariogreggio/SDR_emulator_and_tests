// SDRdialogs.cpp : implementation file
//

#include "stdafx.h"
#include "SDR emulator.h"
#include "SDRdialogs.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD) {
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX) {
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()



/////////////////////////////////////////////////////////////////////////////
// CParametersDlg dialog


CParametersDlg::CParametersDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CParametersDlg::IDD, pParent) {
	//{{AFX_DATA_INIT(CParametersDlg)
	m_samplingFrequency = 0;
	m_carrierFrequency = 0;
	m_modulationFrequency = 0;
	m_LocalOscFrequency = 0;
	//}}AFX_DATA_INIT
}


void CParametersDlg::DoDataExchange(CDataExchange* pDX) {

	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CParametersDlg)
	DDX_Text(pDX, IDC_EDIT1, m_samplingFrequency);
	DDX_Text(pDX, IDC_EDIT2, m_carrierFrequency);
	DDX_Text(pDX, IDC_EDIT3, m_modulationFrequency);
	DDX_Text(pDX, IDC_EDIT4, m_LocalOscFrequency);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CParametersDlg, CDialog)
	//{{AFX_MSG_MAP(CParametersDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CParametersDlg message handlers

BOOL CParametersDlg::OnInitDialog() {
	CDialog::OnInitDialog();
	
	m_samplingFrequency=SAMPLING_FREQUENCY;
	m_carrierFrequency=theApp.Frequency;
	m_modulationFrequency=theApp.Modulation;
	m_LocalOscFrequency=theApp.Tune;
	
	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
	}

