// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "SDR emulator.h"

#include "sdr emulatorview.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CMDIFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CMDIFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_MOUSEWHEEL()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

static UINT indicators[] = {
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
	};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame() {
	// TODO: add member initialization code here
	
	}

CMainFrame::~CMainFrame() {
	}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct) {
	if (CMDIFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	// TODO: Delete these three lines if you don't want the toolbar to
	//  be dockable
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);

	return 0;
	}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs) {

	cs.cx=1240;
	cs.cy=808;
	cs.style &= ~FWS_ADDTOTITLE;


	if( !CMDIFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying the CREATESTRUCT cs

	return TRUE;
	}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CMDIFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CMDIFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers


void CMainFrame::OnPaint() {
	CPaintDC dc(this); // device context for painting
	
//	GetActiveView()->Invalidate();
//	((CSDRemulatorView *)(((CMainFrame *)m_pMainWnd)->GetActiveView()))->Invalidate();
	
	// Do not call CMDIFrameWnd::OnPaint() for painting messages
	}

BOOL CMainFrame::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) {
	int incr;
	
	if(nFlags & MK_CONTROL) {
		if(theApp.ModulationType)
			incr=(zDelta*100)/120;			//120 � fisso, dice
		if(nFlags & MK_SHIFT)
			incr *=10;
		if((int)theApp.Modulation + incr > 0)
			theApp.Modulation += incr;
		theApp.OnProvePortantemodulata();
		}
	else {
		incr=(zDelta*1000)/120;			//120 � fisso, dice
		if(nFlags & MK_SHIFT)
			incr *=10;
		if((int)theApp.Frequency + incr > 0)
			theApp.Frequency += incr;
/*	if(zDelta>0)
		Frequency++;
	else*/
		theApp.OnProvePortante();
		}
	

	theApp.redrawWindows();


	return CMDIFrameWnd::OnMouseWheel(nFlags, zDelta, pt);
	}

