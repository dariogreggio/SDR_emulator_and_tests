// SDR emulatorView.cpp : implementation of the CSDRemulatorView class
//

#include "stdafx.h"
#include "SDR emulator.h"

#include "SDR emulatorDoc.h"
#include "SDR emulatorView.h"

#include <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSDRemulatorView

IMPLEMENT_DYNCREATE(CSDRemulatorView, CView)

BEGIN_MESSAGE_MAP(CSDRemulatorView, CView)
	//{{AFX_MSG_MAP(CSDRemulatorView)
	ON_COMMAND(ID_VISUALIZZA_ZOOMIN, OnVisualizzaZoomin)
	ON_COMMAND(ID_VISUALIZZA_ZOOMOUT, OnVisualizzaZoomout)
	ON_WM_MOUSEMOVE()
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSDRemulatorView construction/destruction

CSDRemulatorView::CSDRemulatorView() {
	// TODO: add construction code here

	}

CSDRemulatorView::~CSDRemulatorView() {
	}

BOOL CSDRemulatorView::PreCreateWindow(CREATESTRUCT& cs) {

//	cs.cx=1250;			// se ne frega... v. doc

//	cs.lpszName="Wave";	// idem...
//	cs.style &= ~FWS_ADDTOTITLE; // ma porcodio, v. childwnd

	return CView::PreCreateWindow(cs);
	}

/////////////////////////////////////////////////////////////////////////////
// CSDRemulatorView drawing

void CSDRemulatorView::OnDraw(CDC* pDC) {
	CSDRemulatorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	int i,x,y,y1;
	double ymax;
	RECT rc;
	CString S;
	CFont myFont,*oldFont;
	CPen Pen1,Pen2,Pen3,*oldPen;
	WORD *p;

	GetClientRect(&rc);
	y=rc.bottom;

	Pen1.CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
	Pen2.CreatePen(PS_SOLID, 1, RGB(0, 255, 0));
	Pen3.CreatePen(PS_SOLID, 1, RGB(220, 220, 228));

	myFont.CreateFont(12,0,0,0,300,0,0,0,DEFAULT_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,DEFAULT_PITCH | FF_SWISS,"arial");

	oldFont=(CFont *)pDC->SelectObject(myFont);

	oldPen=(CPen *)pDC->SelectObject(Pen3);
	switch(theApp.TipoVis1) {
		case 0:
			for(x=0; x<rc.right; x+=NUM_SAMPLES) {
				pDC->MoveTo(x,0);
				pDC->LineTo(x,rc.bottom);
				}
			for(y=0; y<rc.bottom; y+=rc.bottom/20) {
				pDC->MoveTo(0,y);
				pDC->LineTo(rc.right,y);
				}
			break;
		case 1:
		case 2:
		case 3:
			for(x=0; x<rc.right; x+=20) {
				pDC->MoveTo(x,0);
				pDC->LineTo(x,rc.bottom);
				}
			for(y=0; y<rc.bottom; y+=rc.bottom/20) {
				pDC->MoveTo(0,y);
				pDC->LineTo(rc.right,y);
				}
			break;
		}

	switch(theApp.TipoVis1) {
		case 0:
			pDC->SelectObject(Pen1);
			pDC->MoveTo(0,y/2);
			p=theApp.theSamples;
			for(i=0; i<rc.right; i++) {
				y1=(rc.bottom/2)-(((((int)*p)-0x8000)*rc.bottom)/(65536));
				pDC->LineTo(i,y1);
				p+=theApp.TheZoom1;
				}
			break;
		case 1:
			pDC->SelectObject(Pen2);
			ymax=1;
			for(x=0; x<FFT_LEN/4; x++) {
				ymax=max(theApp.nn1[x],ymax);
				}
			ymax=1.1*ymax;
			for(x=0; x<RF_SPECTRUM_ACCU; x++) {
				y1=(rc.bottom)-(((theApp.nn1[x])*rc.bottom)/(ymax));
				
				pDC->MoveTo(x*5+10,rc.bottom);
				pDC->LineTo(x*5+10,y1);
				}
			break;
		case 2:
			pDC->SelectObject(Pen2);
			ymax=1;
			for(x=0; x<FFT_LEN/4; x++) {
				ymax=max(theApp.fdraw[x],ymax);
				}
			ymax=1.1*ymax;
			for(x=0; x<FFT_LEN/4; x++) {
				y1=(rc.bottom)-(((theApp.fdraw[x])*rc.bottom)/(ymax));
				
				pDC->MoveTo(x,rc.bottom);
				pDC->LineTo(x,y1);
				}
			break;
		case 3:
			pDC->SelectObject(Pen2);
			ymax=1;
			for(x=0; x<FFT_LEN/4/FFT_LEN_BF_REDUCED; x++) {
				ymax=max(theApp.fdraw[x],ymax);
				}
			ymax=1.1*ymax;
			for(x=0; x<FFT_LEN/4/FFT_LEN_BF_REDUCED; x++) {
				y1=(rc.bottom)-(((theApp.fdraw[x])*rc.bottom)/(ymax));
				
				pDC->MoveTo(x*FFT_LEN_BF_REDUCED,rc.bottom);
				pDC->LineTo(x*FFT_LEN_BF_REDUCED,y1);
				}
			break;
		}

	switch(theApp.TipoVis1) {
		case 0:
			S.Format("samples: %u",rc.right);
			pDC->TextOut(5,5,S);
			if(theApp.TheZoom1==1)
				S.Format("F: %uHz; %u samples",theApp.Frequency,NUM_SAMPLES);
			else
				S.Format("F: %uHz; %u samples",theApp.Frequency,NUM_SAMPLES);
			pDC->TextOut(5,15,S);
			break;
		case 1:
		case 2:
			S.Format("FFT: %u-%uHz",0,4000000);
			pDC->TextOut(5,5,S);
			S.Format("%u FFT bins",FFT_LEN);
			pDC->TextOut(5,15,S);
			S.Format("Vmax=%u",(int)ymax);
			pDC->TextOut(5,25,S);
			break;
		case 3:
			S.Format("FFT: %u-%uHz",0,4000000/FFT_LEN_BF_REDUCED);
			pDC->TextOut(5,5,S);
			S.Format("%u FFT bins",FFT_LEN/FFT_LEN_BF_REDUCED);
			pDC->TextOut(5,15,S);
			S.Format("Vmax=%u",(int)ymax);
			pDC->TextOut(5,25,S);
			break;
		}

	switch(theApp.TipoVis1) {
		case 0:
			for(x=0; x<rc.right; x+=NUM_SAMPLES) {
				if(!(x % 128)) {
					S.Format("%u",x);
					pDC->TextOut(x-2,rc.bottom-10,S);
					}
				}
			break;
		case 1:
		case 2:
			for(x=0; x<rc.right; x+=20) {
				if(x && !(x % 100)) {
					S.Format("%uKHz",theApp.m_FFT->binToFrequency(SAMPLING_FREQUENCY,FFT_LEN/2,x-2)/1000);
					pDC->TextOut(x-10,rc.bottom-10,S);
					}
				}
			break;
		case 3:
			for(x=0; x<rc.right; x+=20) {
				if(x && !(x % 100)) {
					S.Format("%uKHz",
						theApp.m_FFT->binToFrequency(SAMPLING_FREQUENCY/FFT_LEN_BF_REDUCED,FFT_LEN/2/FFT_LEN_BF_REDUCED,(x-2)/FFT_LEN_BF_REDUCED)/1000);
					pDC->TextOut(x-10,rc.bottom-10,S);
					}
				}
			break;
		}

//	pDC->SelectObject(oldFont);
//	myFont.DeleteObject();

	pDC->SelectObject(oldPen);

	}


/////////////////////////////////////////////////////////////////////////////
// CSDRemulatorView printing

BOOL CSDRemulatorView::OnPreparePrinting(CPrintInfo* pInfo) {
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CSDRemulatorView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/) {
	// TODO: add extra initialization before printing
}

void CSDRemulatorView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/) {
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CSDRemulatorView diagnostics

#ifdef _DEBUG
void CSDRemulatorView::AssertValid() const
{
	CView::AssertValid();
}

void CSDRemulatorView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CSDRemulatorDoc* CSDRemulatorView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CSDRemulatorDoc)));
	return (CSDRemulatorDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CSDRemulatorView message handlers


void CSDRemulatorView::OnVisualizzaZoomin() {

	theApp.TheZoom1=1;
	Invalidate();
	}

void CSDRemulatorView::OnVisualizzaZoomout() {

	theApp.TheZoom1=10;
	Invalidate();
	}


void CSDRemulatorView::OnMouseMove(UINT nFlags, CPoint point) {
	CDC *pDC=GetDC();
	CFont myFont,*oldFont;
	CPen Pen1,*oldPen;
	CString S;
	POINT pt;
	RECT rc;
	
	myFont.CreateFont(12,0,0,0,300,0,0,0,DEFAULT_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,DEFAULT_PITCH | FF_SWISS,"arial");
	oldFont=(CFont *)pDC->SelectObject(&myFont);

	Pen1.CreatePen(PS_SOLID, 1, RGB(255, 255, 255));
	oldPen=(CPen *)pDC->SelectObject(&Pen1);
	pDC->Rectangle(200,5,250,15);
	GetCursorPos(&pt);
	GetWindowRect(&rc);
	pt.x-=rc.left;
	pDC->SetTextColor(RGB(0,0,255));
	switch(theApp.TipoVis1) {
		case 0:
			S.Format("t=%uuS", theApp.TheZoom1==1 ? pt.x/8 : pt.x*(10)/8);
			pDC->TextOut(200,5,S);
			break;
		case 1:
			S.Format("%uKHz",theApp.m_FFT->binToFrequency(SAMPLING_FREQUENCY,FFT_LEN/2,(pt.x-10)/5)/1000);
			pDC->TextOut(200,5,S);
			break;
		case 2:
			S.Format("%uKHz",theApp.m_FFT->binToFrequency(SAMPLING_FREQUENCY,FFT_LEN/2,pt.x -2 /* min ricevuto da mouse...*/) /1000);
			pDC->TextOut(200,5,S);
			break;
		case 3:
			S.Format("%uHz",theApp.m_FFT->binToFrequency(SAMPLING_FREQUENCY/FFT_LEN_BF_REDUCED,FFT_LEN/2/FFT_LEN_BF_REDUCED,(pt.x -2 /* min ricevuto da mouse...*/)/FFT_LEN_BF_REDUCED) );
			pDC->TextOut(200,5,S);
			break;
		}

	myFont.DeleteObject();
	pDC->SelectObject(oldPen);
	ReleaseDC(pDC);

	CView::OnMouseMove(nFlags, point);
	}


void CSDRemulatorView::OnEditCopy() {
	DWORD l;
	BITMAPINFOHEADER bi;
	HANDLE h;
	BYTE *p;
	CBitmap 	bitmap;
	CDC 		memDC;
	CRect		rect;
	CClientDC	dc(this);


	if(OpenClipboard()) {
		GetWindowRect(rect);

		bi.biHeight=rect.bottom;
		bi.biWidth=rect.right;

/*		l=bi.biWidth*bi.biHeight*3;
 		h=GlobalAlloc(GMEM_MOVEABLE,l+sizeof(BITMAPINFOHEADER)+32);
		p=(BYTE *)GlobalLock(h);
		memcpy(p,&bi,sizeof(BITMAPINFOHEADER));
		*/

		// no, ora faccio cos?...

		memDC.CreateCompatibleDC(&dc);
		bitmap.CreateCompatibleBitmap(&dc, rect.Width(),rect.Height());

		CBitmap* pOldBitmap = memDC.SelectObject(&bitmap);
		memDC.BitBlt(0, 0, rect.Width(),rect.Height(), &dc, 0, 0, SRCCOPY);

		OpenClipboard();
		EmptyClipboard();
		SetClipboardData(CF_BITMAP, bitmap.GetSafeHandle());
		CloseClipboard();
	
	/*memcpy(p+sizeof(BITMAPINFOHEADER),theFrame,l);
		GlobalUnlock(h);
		EmptyClipboard();
		SetClipboardData(CF_DIB,h);
		*/

		
		memDC.SelectObject(pOldBitmap);
		bitmap.Detach();
		}	
	}


/////////////////////////////////////////////////////////////////////////////
// CSDRemulatorView2

IMPLEMENT_DYNCREATE(CSDRemulatorView2, CView)

BEGIN_MESSAGE_MAP(CSDRemulatorView2, CView)
	//{{AFX_MSG_MAP(CSDRemulatorView2)
	ON_COMMAND(ID_VISUALIZZA_ZOOMIN, OnVisualizzaZoomin)
	ON_COMMAND(ID_VISUALIZZA_ZOOMOUT, OnVisualizzaZoomout)
	ON_WM_MOUSEMOVE()
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSDRemulatorView2 construction/destruction

CSDRemulatorView2::CSDRemulatorView2() {
	// TODO: add construction code here

}

CSDRemulatorView2::~CSDRemulatorView2() {
}

BOOL CSDRemulatorView2::PreCreateWindow(CREATESTRUCT& cs) {
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CSDRemulatorView drawing

void CSDRemulatorView2::OnDraw(CDC* pDC) {
	CSDRemulatorDoc2* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	int i,x,y,y1;
	double ymax;
	RECT rc;
	CString S;
	CFont myFont,*oldFont;
	CPen Pen1,Pen2,Pen3,*oldPen;
	WORD *p;

	GetClientRect(&rc);
	y=rc.bottom;

	Pen1.CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
	Pen2.CreatePen(PS_SOLID, 1, RGB(0, 255, 0));
	Pen3.CreatePen(PS_SOLID, 1, RGB(220, 220, 228));

	myFont.CreateFont(12,0,0,0,300,0,0,0,DEFAULT_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,DEFAULT_PITCH | FF_SWISS,"arial");

	oldFont=(CFont *)pDC->SelectObject(myFont);

	oldPen=(CPen *)pDC->SelectObject(Pen3);
	switch(theApp.TipoVis2) {
		case 0:
			for(x=0; x<rc.right; x+=NUM_SAMPLES) {
				pDC->MoveTo(x,0);
				pDC->LineTo(x,rc.bottom);
				}
			for(y=0; y<rc.bottom; y+=rc.bottom/20) {
				pDC->MoveTo(0,y);
				pDC->LineTo(rc.right,y);
				}
			break;
		case 1:
		case 2:
		case 3:
			for(x=0; x<rc.right; x+=20) {
				pDC->MoveTo(x,0);
				pDC->LineTo(x,rc.bottom);
				}
			for(y=0; y<rc.bottom; y+=rc.bottom/20) {
				pDC->MoveTo(0,y);
				pDC->LineTo(rc.right,y);
				}
			break;
		}

	switch(theApp.TipoVis2) {
		case 0:
			pDC->SelectObject(Pen1);
			pDC->MoveTo(0,y/2);
			p=theApp.theSamples;
			for(i=0; i<rc.right; i++) {
				y1=(rc.bottom/2)-(((((int)*p)-0x8000)*rc.bottom)/(65536));
				pDC->LineTo(i,y1);
				p+=theApp.TheZoom2;
				}
			break;
		case 1:
			pDC->SelectObject(Pen2);
			ymax=1;
			for(x=0; x<FFT_LEN/4; x++) {
				ymax=max(theApp.nn1[x],ymax);
				}
			ymax=1.1*ymax;
			for(x=0; x<RF_SPECTRUM_ACCU; x++) {
				y1=(rc.bottom)-(((theApp.nn1[x])*rc.bottom)/(ymax));
				
				pDC->MoveTo(x*5+10,rc.bottom);
				pDC->LineTo(x*5+10,y1);
				}
			break;
		case 2:
			pDC->SelectObject(Pen2);
			ymax=1;
			for(x=0; x<FFT_LEN/4; x++) {
				ymax=max(theApp.fdraw[x],ymax);
				}
			ymax=1.1*ymax;
			for(x=0; x<FFT_LEN/4; x++) {
				y1=(rc.bottom)-(((theApp.fdraw[x])*rc.bottom)/(ymax));
				
				pDC->MoveTo(x,rc.bottom);
				pDC->LineTo(x,y1);
				}
			break;
		case 3:
			pDC->SelectObject(Pen2);
			ymax=1;
			for(x=0; x<FFT_LEN/4/FFT_LEN_BF_REDUCED; x++) {
				ymax=max(theApp.fdraw[x],ymax);
				}
			ymax=1.1*ymax;
			for(x=0; x<FFT_LEN/4/FFT_LEN_BF_REDUCED; x++) {
				y1=(rc.bottom)-(((theApp.fdraw[x])*rc.bottom)/(ymax));
				
				pDC->MoveTo(x*FFT_LEN_BF_REDUCED,rc.bottom);
				pDC->LineTo(x*FFT_LEN_BF_REDUCED,y1);
				}
			break;
		}

	switch(theApp.TipoVis2) {
		case 0:
			S.Format("samples: %u",rc.right);
			pDC->TextOut(5,5,S);
			if(theApp.TheZoom2==1)
				S.Format("F: %uHz; %u samples",theApp.Frequency,NUM_SAMPLES);
			else
				S.Format("F: %uHz; %u samples",theApp.Frequency,NUM_SAMPLES);
			pDC->TextOut(5,15,S);
			break;
		case 1:
		case 2:
			S.Format("FFT: %u-%uHz",0,4000000);
			pDC->TextOut(5,5,S);
			S.Format("%u FFT bins",FFT_LEN);
			pDC->TextOut(5,15,S);
			S.Format("Vmax=%u",(int)ymax);
			pDC->TextOut(5,25,S);
			break;
		case 3:
			S.Format("FFT: %u-%uHz",0,4000000/FFT_LEN_BF_REDUCED);
			pDC->TextOut(5,5,S);
			S.Format("%u FFT bins",FFT_LEN/FFT_LEN_BF_REDUCED);
			pDC->TextOut(5,15,S);
			S.Format("Vmax=%u",(int)ymax);
			pDC->TextOut(5,25,S);
			break;
		}

	switch(theApp.TipoVis2) {
		case 0:
			for(x=0; x<rc.right; x+=NUM_SAMPLES) {
				if(!(x % 128)) {
					S.Format("%u",x);
					pDC->TextOut(x-2,rc.bottom-10,S);
					}
				}
			break;
		case 1:
		case 2:
			for(x=0; x<rc.right; x+=20) {
				if(x && !(x % 100)) {
					S.Format("%uKHz",theApp.m_FFT->binToFrequency(SAMPLING_FREQUENCY,FFT_LEN/2,x-2)/1000);
					pDC->TextOut(x-10,rc.bottom-10,S);
					}
				}
			break;
		case 3:
			for(x=0; x<rc.right; x+=20) {
				if(x && !(x % 100)) {
					S.Format("%uKHz",
						theApp.m_FFT->binToFrequency(SAMPLING_FREQUENCY/FFT_LEN_BF_REDUCED,FFT_LEN/2/FFT_LEN_BF_REDUCED,(x-2)/FFT_LEN_BF_REDUCED)/1000);
					pDC->TextOut(x-10,rc.bottom-10,S);
					}
				}
			break;
		}

//	pDC->SelectObject(oldFont);
//	myFont.DeleteObject();

	pDC->SelectObject(oldPen);

	}


/////////////////////////////////////////////////////////////////////////////
// CSDRemulatorView2 printing

BOOL CSDRemulatorView2::OnPreparePrinting(CPrintInfo* pInfo) {
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CSDRemulatorView2::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/) {
	// TODO: add extra initialization before printing
}

void CSDRemulatorView2::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/) {
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CSDRemulatorView diagnostics

#ifdef _DEBUG
void CSDRemulatorView2::AssertValid() const
{
	CView::AssertValid();
}

void CSDRemulatorView2::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CSDRemulatorDoc2* CSDRemulatorView2::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CSDRemulatorDoc2)));
	return (CSDRemulatorDoc2*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CSDRemulatorView2 message handlers

void CSDRemulatorView2::OnVisualizzaZoomin() {

	theApp.TheZoom2=1; theApp.TipoVis2=3;
	Invalidate();
	}

void CSDRemulatorView2::OnVisualizzaZoomout() {

	theApp.TheZoom2=10; theApp.TipoVis2=2;
	Invalidate();
	}


void CSDRemulatorView2::OnMouseMove(UINT nFlags, CPoint point) {
	CDC *pDC=GetDC();
	CFont myFont,*oldFont;
	CPen Pen1,*oldPen;
	CString S;
	POINT pt;
	RECT rc;
	
	myFont.CreateFont(12,0,0,0,300,0,0,0,DEFAULT_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,DEFAULT_PITCH | FF_SWISS,"arial");
	oldFont=(CFont *)pDC->SelectObject(&myFont);

	Pen1.CreatePen(PS_SOLID, 1, RGB(255, 255, 255));
	oldPen=(CPen *)pDC->SelectObject(&Pen1);
	pDC->Rectangle(200,5,250,15);
	GetCursorPos(&pt);
	GetWindowRect(&rc);
	pt.x-=rc.left;
	pDC->SetTextColor(RGB(0,0,255));
	switch(theApp.TipoVis2) {
		case 0:
			S.Format("t=%u",pt.x);
			pDC->TextOut(200,5,S);
			break;
		case 1:
			S.Format("%uKHz",theApp.m_FFT->binToFrequency(SAMPLING_FREQUENCY,FFT_LEN/2,(pt.x-10)/5)/1000);
			pDC->TextOut(200,5,S);
			break;
		case 2:
			S.Format("%uKHz",theApp.m_FFT->binToFrequency(SAMPLING_FREQUENCY,FFT_LEN/2,pt.x -2 /* min ricevuto da mouse...*/) /1000);
			pDC->TextOut(200,5,S);
			break;
		case 3:
			S.Format("%uHz",theApp.m_FFT->binToFrequency(SAMPLING_FREQUENCY/FFT_LEN_BF_REDUCED,FFT_LEN/2/FFT_LEN_BF_REDUCED,(pt.x -2 /* min ricevuto da mouse...*/)/FFT_LEN_BF_REDUCED) );
			pDC->TextOut(200,5,S);
			break;
		}

	myFont.DeleteObject();
	pDC->SelectObject(oldPen);
	ReleaseDC(pDC);

	CView::OnMouseMove(nFlags, point);
	}


void CSDRemulatorView2::OnEditCopy() {
	DWORD l;
	BITMAPINFOHEADER bi;
	HANDLE h;
	BYTE *p;
	CBitmap 	bitmap;
	CDC 		memDC;
	CRect		rect;
	CClientDC	dc(this);


	if(OpenClipboard()) {
		GetWindowRect(rect);

		bi.biHeight=rect.bottom;
		bi.biWidth=rect.right;

		memDC.CreateCompatibleDC(&dc);
		bitmap.CreateCompatibleBitmap(&dc, rect.Width(),rect.Height());

		CBitmap* pOldBitmap = memDC.SelectObject(&bitmap);
		memDC.BitBlt(0, 0, rect.Width(),rect.Height(), &dc, 0, 0, SRCCOPY);

		OpenClipboard();
		EmptyClipboard();
		SetClipboardData(CF_BITMAP, bitmap.GetSafeHandle());
		CloseClipboard();
	
		
		memDC.SelectObject(pOldBitmap);
		bitmap.Detach();
		}	
	}


	
	

  

BOOL CSDRemulatorView2::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) {
	int i;

	if(i=CWnd::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext)) {
		GetParent()->SetWindowText("FFT");
		}
	return i;
	}

BOOL CSDRemulatorView::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) {
	int i;

	if(i=CWnd::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext)) {
		GetParent()->SetWindowText("Wave");
		}
	return i;
	}
	
