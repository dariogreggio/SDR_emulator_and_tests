// SDR emulatorDoc.cpp : implementation of the CSDRemulatorDoc class
//

#include "stdafx.h"
#include "SDR emulator.h"

#include "SDR emulatorDoc.h"
#include "SDR emulatorView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSDRemulatorDoc

IMPLEMENT_DYNCREATE(CSDRemulatorDoc, CDocument)

BEGIN_MESSAGE_MAP(CSDRemulatorDoc, CDocument)
	//{{AFX_MSG_MAP(CSDRemulatorDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSDRemulatorDoc construction/destruction

CSDRemulatorDoc::CSDRemulatorDoc() {
	// TODO: add one-time construction code here

	}

CSDRemulatorDoc::~CSDRemulatorDoc() {
	}

BOOL CSDRemulatorDoc::OnNewDocument() {

	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)
	CSDRemulatorView *w=(CSDRemulatorView *)getView();
	w->GetParent()->SetWindowPos(NULL,0,0,1220,350,SWP_NOZORDER);

	return TRUE;
	}



/////////////////////////////////////////////////////////////////////////////
// CSDRemulatorDoc serialization

void CSDRemulatorDoc::Serialize(CArchive& ar) {

	if (ar.IsStoring())	{
		// TODO: add storing code here
	}
	else	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CSDRemulatorDoc diagnostics

#ifdef _DEBUG
void CSDRemulatorDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CSDRemulatorDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CSDRemulatorDoc commands


/////////////////////////////////////////////////////////////////////////////
// CSDRemulatorDoc2

IMPLEMENT_DYNCREATE(CSDRemulatorDoc2, CDocument)

BEGIN_MESSAGE_MAP(CSDRemulatorDoc2, CDocument)
	//{{AFX_MSG_MAP(CSDRemulatorDoc2)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSDRemulatorDoc construction/destruction

CSDRemulatorDoc2::CSDRemulatorDoc2() {
	// TODO: add one-time construction code here

}

CSDRemulatorDoc2::~CSDRemulatorDoc2() {
}

BOOL CSDRemulatorDoc2::OnNewDocument() {

	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)
	CSDRemulatorView2 *w=(CSDRemulatorView2 *)getView();
	w->GetParent()->SetWindowPos(NULL,0,350,1220,350,SWP_NOZORDER);

	return TRUE;
	}



/////////////////////////////////////////////////////////////////////////////
// CSDRemulatorDoc serialization

void CSDRemulatorDoc2::Serialize(CArchive& ar) {

	if(ar.IsStoring())	{
		// TODO: add storing code here
		}
	else {
		// TODO: add loading code here
		}
	}

/////////////////////////////////////////////////////////////////////////////
// CSDRemulatorDoc diagnostics

#ifdef _DEBUG
void CSDRemulatorDoc2::AssertValid() const
{
	CDocument::AssertValid();
}

void CSDRemulatorDoc2::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

