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
	ON_COMMAND(ID_PROVE_PORTANTE, OnProvePortante)
	ON_COMMAND(ID_PROVE_FFTSUMIX, OnProveFftsumix)
	ON_COMMAND(ID_PROVE_FFTSUPORTANTE, OnProveFftsuportante)
	ON_COMMAND(ID_PROVE_MIXFILTRATA, OnProveMixfiltrata)
	ON_COMMAND(ID_PROVE_PORTANTEMODULATA, OnProvePortantemodulata)
	ON_COMMAND(ID_PROVE_PORTANTEMODULATAOSCILLATORELOCALE, OnProvePortantemodulataoscillatorelocale)
	ON_COMMAND(ID_PROVE_MODULAZIONEAM, OnProveModulazioneam)
	ON_COMMAND(ID_PROVE_MODULAZIONEFM, OnProveModulazionefm)
	ON_COMMAND(ID_PROVE_MODULAZIONESSB, OnProveModulazionessb)
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
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

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

	oldPen=(CPen *)pDC->SelectObject(Pen3);
	switch(theApp.TipoVis) {
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

	switch(theApp.TipoVis) {
		case 0:
			pDC->SelectObject(Pen1);
			pDC->MoveTo(0,y/2);
			p=theApp.theSamples;
			for(i=0; i<rc.right; i++) {
				y1=(rc.bottom/2)-(((((int)*p)-0x8000)*rc.bottom)/(65536));
				pDC->LineTo(i,y1);
				p+=theApp.TheZoom;
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
		}

	myFont.CreateFont(12,0,0,0,300,0,0,0,DEFAULT_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,DEFAULT_PITCH | FF_SWISS,"arial");
	oldFont=(CFont *)pDC->SelectObject(&myFont);

	switch(theApp.TipoVis) {
		case 0:
			S.Format("samples: %u",rc.right);
			pDC->TextOut(5,5,S);
			if(theApp.TheZoom==1)
				S.Format("F: %uHz; %u samples",theApp.Frequency,NUM_SAMPLES);
			else
				S.Format("F: %uHz; %u samples",theApp.Frequency,NUM_SAMPLES);
			pDC->TextOut(5,15,S);
			pDC->SelectObject(oldFont);
			break;
		case 1:
		case 2:
			S.Format("FFT: %u",rc.right);
			pDC->TextOut(5,5,S);
			S.Format("F: %uHz; %u samples",theApp.Frequency,NUM_SAMPLES);
			pDC->TextOut(5,15,S);
			S.Format("Vmax=%u",(int)ymax);
			pDC->TextOut(5,25,S);
			break;
		}

	myFont.DeleteObject();

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

void CSDRemulatorView::OnProvePortante() {

	theApp.CreateWave(theApp.theSamples,THE_SAMPLES_SIZE,FREQUENCY,SAMPLING_FREQUENCY);
	theApp.TipoVis=0;
	Invalidate();
	}


void CSDRemulatorView::OnProveFftsumix() {

	double fin[FFT_LEN/2],fout[FFT_LEN],foutimg[FFT_LEN];
	for(DWORD dw=0; dw<FFT_LEN/2; dw++) {
		//copy audio signal to fft real component for left channel
		fin[dw] = (double)(int)theApp.theSamples[dw] -0x8000;
		}
	//Perform FFT on left channel
	theApp.m_FFT->fft_double(FFT_LEN/2,0,fin,NULL,fout,foutimg);
	float re,im;
	for(int i=1; i < FFT_LEN/4; i++) { //Use FFT_LEN/4 since the data is mirrored within the array.
		re = (float)fout[i];
		im = (float)foutimg[i];
		//get amplitude and scale to 0..256 range
		//fdraw[i]=AmplitudeScaled(re,im,FFT_LEN/2,256);
		theApp.fdraw[i] = ((int)sqrt(mag_sqrd(re,im))) /5000; //% 256;
		}


	theApp.ComputeMatFFT2(FREQUENCY,sizeof(theApp.theSamples)/sizeof(WORD),theApp.theSamples,theApp.nn1,0,
		theApp.BFSpectrumTable,BF_SPECTRUM_ACCU);
	theApp.TipoVis=1;
	Invalidate();
	}

void CSDRemulatorView::OnProveFftsuportante() {

	double fin[FFT_LEN/2],fout[FFT_LEN],foutimg[FFT_LEN];
	for(DWORD dw=0; dw<FFT_LEN/2; dw++) {
		//copy audio signal to fft real component for left channel
		fin[dw] = (double)(int)theApp.theSamples[dw] -0x8000;
		}
	//Perform FFT on left channel
	theApp.m_FFT->fft_double(FFT_LEN/2,0,fin,NULL,fout,foutimg);
	float re,im;
	for(int i=1; i < FFT_LEN/4; i++) { //Use FFT_LEN/4 since the data is mirrored within the array.
		re = (float)fout[i];
		im = (float)foutimg[i];
		//get amplitude and scale to 0..256 range
		//fdraw[i]=AmplitudeScaled(re,im,FFT_LEN/2,256);
		theApp.fdraw[i] = ((int)sqrt(mag_sqrd(re,im))) /5000; //% 256;
		}
	

	theApp.ComputeMatFFT2(FREQUENCY,sizeof(theApp.theSamples)/sizeof(WORD),theApp.theSamples,theApp.nn1,0,
		theApp.RFSpectrumTable,RF_SPECTRUM_ACCU);
	theApp.TipoVis=2;
	Invalidate();
	}

void CSDRemulatorView::OnProveMixfiltrata() {
	
	theApp.TipoVis=0;
	Invalidate();
	}

void CSDRemulatorView::OnProvePortantemodulata() {

	theApp.CreateWave(theApp.theSamples,THE_SAMPLES_SIZE,FREQUENCY,SAMPLING_FREQUENCY,5000,theApp.ModulationType);
	theApp.TipoVis=0;
	Invalidate();
	}

void CSDRemulatorView::OnProvePortantemodulataoscillatorelocale() {
	WORD *buffer;
	int i;
	
//	theApp.CreateWave(theApp.theSamples,THE_SAMPLES_SIZE,FREQUENCY,SAMPLING_FREQUENCY,50000,theApp.ModulationType);
	theApp.CreateWave(theApp.theSamples,THE_SAMPLES_SIZE,FREQUENCY,SAMPLING_FREQUENCY);

	buffer=new WORD[THE_SAMPLES_SIZE];
	theApp.CreateWave(buffer,THE_SAMPLES_SIZE,10000UL,SAMPLING_FREQUENCY);
	for(i=0; i<sizeof(theApp.theSamples)/sizeof(WORD); i++) {
		theApp.theSamples[i]=(((((int)theApp.theSamples[i])-0x8000) + (((int)buffer[i])-0x8000)/2)/2)+0x8000;

// https://github.com/ac2cz/SDR/blob/master/src/tutorial5/OscTest2.java
			IQbuffer2[2*d] = IQbuffer[2*d]*c.geti() + IQbuffer[2*d+1]*c.getq();
			IQbuffer2[2*d+1] = IQbuffer[2*d+1]*c.geti() - IQbuffer[2*d]*c.getq();

		}
	delete []buffer;

	theApp.TipoVis=0;
	Invalidate();
	}


void CSDRemulatorView::OnProveModulazioneam() {

	theApp.ModulationType=1;
	OnProvePortantemodulata();
	}

void CSDRemulatorView::OnProveModulazionefm() {

	theApp.ModulationType=2;
	OnProvePortantemodulata();
	}

void CSDRemulatorView::OnProveModulazionessb() {

	theApp.ModulationType=3;
	OnProvePortantemodulata();
	}

void CSDRemulatorView::OnVisualizzaZoomin() {

	theApp.TheZoom=1;
	Invalidate();
	}

void CSDRemulatorView::OnVisualizzaZoomout() {

	theApp.TheZoom=10;
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
	switch(theApp.TipoVis) {
		case 0:
			S.Format("X=%u",pt.x);
			pDC->TextOut(200,5,S);
			break;
		case 1:
			S.Format("%uHz",theApp.m_FFT->Frequency_from_bin(SAMPLING_FREQUENCY,FFT_LEN/2,(pt.x-10)/5));
			pDC->TextOut(200,5,S);
			break;
		case 2:
			S.Format("%uHz",theApp.m_FFT->Frequency_from_bin(SAMPLING_FREQUENCY,FFT_LEN/2,pt.x -2 /* min ricevuto da mouse...*/));
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

		// no, ora faccio così...

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
	ON_COMMAND(ID_PROVE_FFTSUMIX, OnProveFftsumix)
	ON_COMMAND(ID_PROVE_FFTSUPORTANTE, OnProveFftsuportante)
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

	oldPen=(CPen *)pDC->SelectObject(Pen3);
	switch(theApp.TipoVis) {
		case 0:
			break;
		case 1:
		case 2:
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

	switch(theApp.TipoVis) {
		case 0:
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
		}

	myFont.CreateFont(12,0,0,0,300,0,0,0,DEFAULT_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,DEFAULT_PITCH | FF_SWISS,"arial");
	oldFont=(CFont *)pDC->SelectObject(&myFont);

	switch(theApp.TipoVis) {
		case 0:
			break;
		case 1:
		case 2:
			S.Format("FFT: %u",rc.right);
			pDC->TextOut(5,5,S);
			S.Format("F: %uHz; %u samples",theApp.Frequency,NUM_SAMPLES);
			pDC->TextOut(5,15,S);
			S.Format("Vmax=%u",(int)ymax);
			pDC->TextOut(5,25,S);
			break;
		}

	myFont.DeleteObject();

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

void CSDRemulatorView2::OnProveFftsumix() {

	double fin[FFT_LEN/2],fout[FFT_LEN],foutimg[FFT_LEN];
	for(DWORD dw=0; dw<FFT_LEN/2; dw++) {
		//copy audio signal to fft real component for left channel
		fin[dw] = (double)(int)theApp.theSamples[dw] -0x8000;
		}
	//Perform FFT on left channel
	theApp.m_FFT->fft_double(FFT_LEN/2,0,fin,NULL,fout,foutimg);
	float re,im;
	for(int i=1; i < FFT_LEN/4; i++) { //Use FFT_LEN/4 since the data is mirrored within the array.
		re = (float)fout[i];
		im = (float)foutimg[i];
		//get amplitude and scale to 0..256 range
		//fdraw[i]=AmplitudeScaled(re,im,FFT_LEN/2,256);
		theApp.fdraw[i] = ((int)sqrt(mag_sqrd(re,im))) /5000; //% 256;
		}


	theApp.ComputeMatFFT2(FREQUENCY,sizeof(theApp.theSamples)/sizeof(WORD),theApp.theSamples,theApp.nn1,0,
		theApp.BFSpectrumTable,BF_SPECTRUM_ACCU);
	theApp.TipoVis=1;
	Invalidate();
	}

void CSDRemulatorView2::OnProveFftsuportante() {

	double fin[FFT_LEN/2],fout[FFT_LEN],foutimg[FFT_LEN];
	for(DWORD dw=0; dw<FFT_LEN/2; dw++) {
		//copy audio signal to fft real component for left channel
		fin[dw] = (double)(int)theApp.theSamples[dw] -0x8000;
		}
	//Perform FFT on left channel
	theApp.m_FFT->fft_double(FFT_LEN/2,0,fin,NULL,fout,foutimg);
	float re,im;
	for(int i=1; i < FFT_LEN/4; i++) { //Use FFT_LEN/4 since the data is mirrored within the array.
		re = (float)fout[i];
		im = (float)foutimg[i];
		//get amplitude and scale to 0..256 range
		//fdraw[i]=AmplitudeScaled(re,im,FFT_LEN/2,256);
		theApp.fdraw[i] = ((int)sqrt(mag_sqrd(re,im))) /5000; //% 256;
		}
	

	theApp.ComputeMatFFT2(FREQUENCY,sizeof(theApp.theSamples)/sizeof(WORD),theApp.theSamples,theApp.nn1,0,
		theApp.RFSpectrumTable,RF_SPECTRUM_ACCU);
	theApp.TipoVis=2;
	Invalidate();
	}

void CSDRemulatorView2::OnVisualizzaZoomin() {

	theApp.TheZoom=1;
	Invalidate();
	}

void CSDRemulatorView2::OnVisualizzaZoomout() {

	theApp.TheZoom=10;
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
	switch(theApp.TipoVis) {
		case 0:
			break;
		case 1:
			S.Format("%uHz",theApp.m_FFT->Frequency_from_bin(SAMPLING_FREQUENCY,FFT_LEN/2,(pt.x-10)/5));
			pDC->TextOut(200,5,S);
			break;
		case 2:
			S.Format("%uHz",theApp.m_FFT->Frequency_from_bin(SAMPLING_FREQUENCY,FFT_LEN/2,pt.x -2 /* min ricevuto da mouse...*/));
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


	
	

  