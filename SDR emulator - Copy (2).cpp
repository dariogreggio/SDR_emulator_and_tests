// SDR emulator.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "SDR emulator.h"

#include "MainFrm.h"
#include "ChildFrm.h"
#include "SDR emulatorDoc.h"
#include "SDR emulatorView.h"

#include <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSDRemulatorApp

BEGIN_MESSAGE_MAP(CSDRemulatorApp, CWinApp)
	//{{AFX_MSG_MAP(CSDRemulatorApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
	// Standard print setup command
	ON_COMMAND(ID_FILE_PRINT_SETUP, CWinApp::OnFilePrintSetup)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSDRemulatorApp construction

const WORD CSDRemulatorApp::SINE_WAVE_TABLE[NUM_SAMPLES] = {
	0x8000,0xb0fb,0xda82,0xf641,0xffff,0xf641,0xda82,0xb0fb,
	0x8000,0x4f04,0x257d,0x9be,0x0,0x9be,0x257d,0x4f04
	};

const int CSDRemulatorApp::RecogTable[SPECTRUM_ACCU+1]={
/*  (75+25)/2,(125+75)/2,(175+125)/2,(225+175)/2,(275+225)/2,(325+275)/2,(375+325)/2,
  (425+375)/2,(475+425)/2,(525+475)/2,(580+525)/2,(630+580)/2,(689+630)/2,(735+689)/2,
  (787+735)/2,(848+787)/2,(918+848)/2,(1002+918)/2,(1102+1002)/2,(1225+1102)/2,
  (1378+1225)/2,(1470+1378)/2,(1575+1470)/2,(1690+1575)/2,(1837+1690)/2,
  (2004+1837)/2,(2205+2004)/2,(2450+2205)/2,(2756+2450)/2,(3150+2756)/2,(3675+3150)/2,
  (4410+3675)/2,(5512+4410)/2,(7350+5512)/2,(11025+7350)/2,(22050l+11025l)/2l};*/
  100,150,200,250,300,350,400,450,500,550,
  600,650,700,750,800,850,900,950,1000,1050,
  1100,1200,1300,1400,1500,1600,1700,1800,1900,
  2000,2200,2400,2600,2800,3000,3200,3400,3600,
  3800,4000,4500,5000,5500,6000,6500,7000,7500,
  8000,8500,9000,9500,10000,11000,11500
//  25,75,125,175,225,275,325,
//  425,475,525,625,725,850,1000,
//  1200,1400,1600,1800,2000,2500,3000,
//  4000,5000,6000,7000,8000,9000,11025
  };

CSDRemulatorApp::CSDRemulatorApp() {

	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CSDRemulatorApp object

CSDRemulatorApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CSDRemulatorApp initialization

BOOL CSDRemulatorApp::InitInstance() {
	int i;

	AfxEnableControlContainer();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	// Change the registry key under which our settings are stored.
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization.
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));

	LoadStdProfileSettings();  // Load standard INI file options (including MRU)

	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views.

	CMultiDocTemplate* pDocTemplate;
	pDocTemplate = new CMultiDocTemplate(
		IDR_SDREMUTYPE,
		RUNTIME_CLASS(CSDRemulatorDoc),
		RUNTIME_CLASS(CChildFrame), // custom MDI child frame
		RUNTIME_CLASS(CSDRemulatorView));
	AddDocTemplate(pDocTemplate);

	// create main MDI Frame window
	CMainFrame* pMainFrame = new CMainFrame;
	if (!pMainFrame->LoadFrame(IDR_MAINFRAME))
		return FALSE;
	m_pMainWnd = pMainFrame;

	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	// Dispatch commands specified on the command line
	if(!ProcessShellCommand(cmdInfo))
		return FALSE;

	initFFT();
	TipoVis=0;

	for(i=0; i<sizeof(theSamples)/sizeof(WORD); i++)
		theSamples[i] = 0x8000;

	Frequency=Modulation=0;
	ModulationRatio=MODULATION_DEFAULT_RATIO;
	ModulationType=0;

	// The main window has been initialized, so show and update it.
	pMainFrame->ShowWindow(m_nCmdShow);
	pMainFrame->UpdateWindow();

	return TRUE;
	}


/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

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

// App command to run the dialog
void CSDRemulatorApp::OnAppAbout() {

	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

/////////////////////////////////////////////////////////////////////////////
// CSDRemulatorApp message handlers

int CSDRemulatorApp::CreateWave(WORD *buffer,DWORD bufSize,DWORD frequency,DWORD sampleRate,
																DWORD modulation,BYTE modulationType) {
	WORD *p;
	int i,t,cnt,counter, cnt2,counter2;
	int modulazione;

	p=buffer;
	i=0;
	counter=cnt=0;
	counter2=cnt2=0;
	modulazione=50;
	while(i<bufSize) {
		switch(modulationType) {
			case 2:
		    counter += frequency + ((frequency* (modulazione-50))/100);
				break;
			default:
		    counter += frequency;
				break;
			}
    while(counter >= sampleRate) {
      counter -= sampleRate;
      cnt++;
      cnt &= NUM_SAMPLES-1;
			}
		if(modulationType) {
			counter2 += modulation;
			if(counter2 >= sampleRate) {
				counter2 -= sampleRate;
				cnt2++;
				cnt2 &= NUM_SAMPLES-1;
				}
			modulazione=50 + ((SINE_WAVE_TABLE[cnt2]*100)/ModulationRatio)/65536L;
			}

		switch(modulationType) {
			case 1:
				t = SINE_WAVE_TABLE[cnt]-0x8000;
				t = ((t*modulazione)/100)+0x8000;
				*p = t;
				break;
			case 2:
				*p = SINE_WAVE_TABLE[cnt];
				break;
			default:
				*p = SINE_WAVE_TABLE[cnt];
				break;
			}

		i++;
		p++;
		}

	Frequency=frequency;
	Modulation=modulation;
	ModulationType=modulationType;

	return bufSize;
	}



static double *dd, *ee;

static double costable[TRIGO_TABLE_ACCU],sintable[TRIGO_TABLE_ACCU];
static double costable2[TRIGO_TABLE_ACCU*4],sintable2[TRIGO_TABLE_ACCU*4];

int CSDRemulatorApp::initFFT() {
	int i;
	double d;

	if(!(dd=(double *)GlobalAlloc(GPTR,10000L*sizeof(double))))
	  return 0;
	if(!(ee=(double *)GlobalAlloc(GPTR,10000L*sizeof(double))))
	  return 0;

	for(i=0; i<TRIGO_TABLE_ACCU; i++) {
	  d=(PI*i)/TRIGO_TABLE_ACCU;
		sintable[i]=sin(d);
		costable[i]=cos(d);
		}
	for(i=0; i<4*TRIGO_TABLE_ACCU; i++) {
	  d=(PI*i)/(TRIGO_TABLE_ACCU*4);
		sintable2[i]=sin(d);
		costable2[i]=cos(d);
		}
	}

int CSDRemulatorApp::ComputeMatFFT2(int freq, DWORD n, const WORD *p,
  double *s, double *s1, const int *Hertz,int nStep) {

	int i,j,ij,ip,ir,iq,ik,ix,in,im,iy,iz,iu;
	int x,hz;
	double *g;
	double z1,z2,a1,a2,b1,b2,d1,d2;
//	FILE *f;
	
  g=dd;
  for(i=0; i<n; i++)
    *g++=*p++ -128;
  g=ee;
  for(i=0; i<n; i++)
    *g++=0.0;
  ij = 2048; ip = 1;
  for(i=0; i<12; i++) {                                                               
    iq = 0; ir = ij;
    for(ik=0; ik<ip; ik++) {
      ix = iq / ij;
      in = subMirror2(ix);
      z1 = costable2[in]; z2 = sintable2[in];
      for(im = iq; im<ir; im++) {
        a1 = dd[im]; a2 = ee[im];
        iu = im + ij;
        d1=dd[iu]; d2=ee[iu];
        b1 = z1*d1 - z2*d2; b2 = z2*d1 + z1*d2;
        dd[im] = a1+b1; ee[im] = a2+b2; dd[iu] = a1-b1; ee[iu] = a2-b2;
        }
      iz=ij << 1; iq += iz; ir += iz;
      }
    ij >>= 1; ip <<= 1;
	  }

	for(j=0; j<nStep; j++) {
    s[j]=0;
    in=Hertz[j]*2;
    in/=FFT_DIV2;
    im=Hertz[j+1]*2;
    im/=FFT_DIV2;
	  for(i=in; i<im; i++) {
//	    iz=max(i,1);
	    ij=subMirror2(i);
	    a1=dd[ij]; a2=ee[ij];
	    s[j]+=hypot(a1,a2);
	    }
    if(j<15)
      s[j]/=2000;
    else  
      s[j]/=1000;
//    s[j]/=((Hertz[j+1]/FFT_DIV2)-(Hertz[j]/FFT_DIV2));
    s[j]/=10;
	  }
  
  return 0;
	}

int _fastcall CSDRemulatorApp::subMirror(int n) {
	register int i=0;
  
	_asm {
		mov eax,n
		xor edx,edx
		mov cx,10
myLoop:
		rcr eax,1
    rcl edx,1
		loop myLoop

    mov i,edx
    }
/*  if(n & 1) i|=512;
  if(n & 2) i|=256;
  if(n & 4) i|=128;
  if(n & 8) i|=64;
  if(n & 16) i|=32;
  if(n & 32) i|=16;
  if(n & 64) i|=8;
  if(n & 128) i|=4;
  if(n & 256) i|=2;
  if(n & 512) i|=1;*/

	return i;
  }
  
int _fastcall CSDRemulatorApp::subMirror2(int n) {
	register short int i=0;
  
	_asm {
		mov ax,word ptr n
		xor dx,dx
		mov cx,12
myLoop:
		rcr ax,1
    rcl dx,1
		loop myLoop

    mov i,dx
    }
	return i;
  }
  
int CSDRemulatorApp::ComputeMatFFT(int freq, DWORD n, const WORD *p,
  double *s, double *s1,int *Hertz,int nStep) {

	int i,j,ij,ip,ir,iq,ik,ix,in,im,iy,iz,iu;
	int x,hz;
	double *g;
	double z1,z2,a1,a2,b1,b2,d1,d2;
	HFILE hf;
	char myBuf[128];

//	wsprintf(myBuf,"%x %x",dd,ee);
//	MessageBox(NULL,myBuf,NULL,MB_OK);
  g=dd;
  for(i=0; i<n; i++)
    *g++=*p++ -0x8000;
  g=ee;
  for(i=0; i<n; i++)
    *g++=0.0;
  ij = 512; ip = 1;
  for(i=0; i<10; i++) {                                                               
    iq = 0; ir = ij;
    for(ik=0; ik<ip; ik++) {
      ix = iq / ij;
      in = subMirror(ix);
      z1 = costable[in]; z2 = sintable[in];
      for(im = iq; im<ir; im++) {
        a1 = dd[im]; a2 = ee[im];
        iu = im + ij;
        d1=dd[iu]; d2=ee[iu];
        b1 = z1*d1 - z2*d2; b2 = z2*d1 + z1*d2;
        dd[im] = a1+b1; ee[im] = a2+b2; dd[iu] = a1-b1; ee[iu] = a2-b2;
        }
      iz=ij << 1; iq += iz; ir += iz;
      }
    ij >>= 1; ip <<= 1;
	  }

/*  if(hf=_lcreat("rnd2.xls",0)) {
    for(i=0; i<512; i++) {
	    wsprintf(myBuf,"%d\t\t%d\t%d\n",i,(int)(sintable[i]*10000.0),(int)(costable[i]*10000.0));
			_lwrite(hf,myBuf,strlen(myBuf));
      }
    for(i=0; i<n; i++) {
	    wsprintf(myBuf,"%d\t\t%d\n",i,(int)(dd[i]*10000.0));
			_lwrite(hf,myBuf,strlen(myBuf));
      }
    for(i=0; i<512; i++) {
	    ij=subMirror(i);
	    a1=dd[ij]; a2=ee[ij];
	    wsprintf(myBuf,"%d\t\t%d\n",i,hypot(a1,a2)/100);
			_lwrite(hf,myBuf,strlen(myBuf));
      }
    _lclose(hf);  
    }*/

	for(j=0; j<nStep; j++) {
    s[j]=0;
    in=Hertz[j]*2;
    in/=FFT_DIV;
    im=Hertz[j+1]*2;
    im/=FFT_DIV;
	  for(i=in; i<im; i++) {
//	    iz=max(i,1);
	    ij=subMirror(i);
	    a1=dd[ij]; a2=ee[ij];
	    s[j]+=hypot(a1,a2);
	    }
    if(j<10)
      s[j]/=1500;
    else if(j<20)
      s[j]/=900;
    else  
      s[j]/=600;
    s[j]/=((Hertz[j+1]/FFT_DIV)-(Hertz[j]/FFT_DIV));
	  }
  
  return 0;
	}

