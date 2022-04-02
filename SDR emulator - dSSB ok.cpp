// SDR emulator.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "SDR emulator.h"

#include "MainFrm.h"
#include "ChildFrm.h"
#include "SDR emulatorDoc.h"
#include "SDR emulatorView.h"
#include "SDRdialogs.h"

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
	ON_COMMAND(ID_PROVE_PORTANTE, OnProvePortante)
	ON_COMMAND(ID_PROVE_FFTSUMIX, OnProveFftsumix)
	ON_COMMAND(ID_PROVE_FFTSUPORTANTE, OnProveFftsuportante)
	ON_COMMAND(ID_PROVE_MIXFILTRATA, OnProveMixfiltrata)
	ON_COMMAND(ID_PROVE_PORTANTEMODULATA, OnProvePortantemodulata)
	ON_COMMAND(ID_PROVE_PORTANTEMODULATAOSCILLATORELOCALE, OnProvePortantemodulataoscillatorelocale)
	ON_COMMAND(ID_PROVE_MODULAZIONEAM, OnProveModulazioneam)
	ON_COMMAND(ID_PROVE_MODULAZIONEFM, OnProveModulazionefm)
	ON_COMMAND(ID_PROVE_MODULAZIONESSB, OnProveModulazionessb)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	ON_COMMAND(ID_PROVE_PARAMETERS, OnProveParameters)
	ON_COMMAND(ID_FILE_NUOVAFORMADONDA, OnFileNuovaformadonda)
	ON_UPDATE_COMMAND_UI(ID_FILE_NUOVAFORMADONDA, OnUpdateFileNuovaformadonda)
	ON_COMMAND(ID_FILE_NUOVAFFT, OnFileNuovafft)
	ON_UPDATE_COMMAND_UI(ID_FILE_NUOVAFFT, OnUpdateFileNuovafft)
	ON_COMMAND(ID_FILE_NEW, OnFileNew)
	//}}AFX_MSG_MAP
	// Standard file based document commands
	//ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
//	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
	// Standard print setup command
	ON_COMMAND(ID_FILE_PRINT_SETUP, CWinApp::OnFilePrintSetup)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSDRemulatorApp construction

CSDRemulatorApp::CSDRemulatorApp() {

	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
	}

CSDRemulatorApp::~CSDRemulatorApp() {

	delete m_FFT;
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
	SetRegistryKey(_T("ADPM Synthesis"));

	LoadStdProfileSettings();  // Load standard INI file options (including MRU)

	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views.

	pDocTemplate = new CMultiDocTemplate(
		IDR_SDREMUTYPE,
		RUNTIME_CLASS(CSDRemulatorDoc),
		RUNTIME_CLASS(CChildFrame), // custom MDI child frame
		RUNTIME_CLASS(CSDRemulatorView));
	AddDocTemplate(pDocTemplate);
	pDocTemplate2 = new CMultiDocTemplate(
		IDR_SDREMUTYPE2,
		RUNTIME_CLASS(CSDRemulatorDoc2),
		RUNTIME_CLASS(CChildFrame), // custom MDI child frame
		RUNTIME_CLASS(CSDRemulatorView2));
	AddDocTemplate(pDocTemplate2);

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
	m_FFT = new CFFT(FFT_LEN);

	TipoVis1=0; TipoVis2=4; TheZoom1=TheZoom2=1;

	for(i=0; i<THE_SAMPLES_SIZE; i++)
		theSamples[i] = 0x8000;

	Frequency=FREQUENCY;
	Tune=FREQUENCY  +10000;		// 10KHz channel
	Modulation=2000;
	ModulationRatio=MODULATION_DEFAULT_RATIO;
	ModulationType=0;
	Filter=1;

	for(i=0; i<RF_SPECTRUM_ACCU; i++)
		RFSpectrumTable[i]=9000*i;		// 0..2.304MHz
	for(i=0; i<BF_SPECTRUM_ACCU; i++)
		BFSpectrumTable[i]=200*i;		// 0..25600Hz

	// The main window has been initialized, so show and update it.
	pMainFrame->ShowWindow(m_nCmdShow);
	pMainFrame->UpdateWindow();

	return TRUE;
	}


void CSDRemulatorApp::OnFileNew() {
	
	OnFileNuovaformadonda();
	OnFileNuovafft();
	}

void CSDRemulatorApp::OnFileNuovaformadonda() {

	theWave=(CSDRemulatorDoc *)pDocTemplate->OpenDocumentFile(NULL);
	}

void CSDRemulatorApp::OnUpdateFileNuovaformadonda(CCmdUI* pCmdUI) {

	pCmdUI->Enable(theWave == NULL);
	}

void CSDRemulatorApp::OnFileNuovafft() {

	theFFT=(CSDRemulatorDoc2 *)pDocTemplate2->OpenDocumentFile(NULL);
	}

void CSDRemulatorApp::OnUpdateFileNuovafft(CCmdUI* pCmdUI) {

	pCmdUI->Enable(theFFT == NULL);
	}

// App command to run the dialog
void CSDRemulatorApp::OnAppAbout() {

	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
	}


void CSDRemulatorApp::redrawWindows() {

//	((CMainFrame *)m_pMainWnd)->Invalidate();
//	if(((CMainFrame *)m_pMainWnd)->GetActiveView())
//		((CSDRemulatorView *)(((CMainFrame *)m_pMainWnd)->GetActiveView()))->Invalidate();

	CMDIChildWnd * pChild =
    ((CMDIFrameWnd*)m_pMainWnd)->MDIGetActive();

  if(pChild) {

    CView *pView = pChild->GetActiveView();

    if(pView)
			pView->Invalidate();	

		((CMDIFrameWnd*)m_pMainWnd)->MDINext();

		pChild =
			((CMDIFrameWnd*)m_pMainWnd)->MDIGetActive();
	  if(pChild) {

	    CView *pView = pChild->GetActiveView();

		  if(pView)
				pView->Invalidate();	
			}
		}
	}

void CSDRemulatorApp::OnProvePortante() {

	CreateWave(theSamples,THE_SAMPLES_SIZE,Frequency,SAMPLING_FREQUENCY);
	TipoVis1=0;
	TipoVis2=2;

	OnProveFftsuportante();

	redrawWindows();
	}


void CSDRemulatorApp::OnProveFftsuportante() {
	double fin[FFT_LEN/2],fout[FFT_LEN],foutimg[FFT_LEN];

	for(DWORD dw=0; dw<FFT_LEN/2; dw++) {
		//copy audio signal to fft real component for left channel
		fin[dw] = (double)(int)theSamples[dw] -0x8000;
		}
	//Perform FFT on left channel
	m_FFT->fft_double(FFT_LEN/2,0,fin,NULL,fout,foutimg);
	for(int i=1; i < FFT_LEN/4; i++) { //Use FFT_LEN/4 since the data is mirrored within the array.
		Complex c(fout[i],foutimg[i]);
		//get amplitude and scale to 0..256 range
		//fdraw[i]=AmplitudeScaled(re,im,FFT_LEN/2,256);
		fdraw[i] = ((int)c.hypot()) /5000; //% 256;
		}
	

//	ComputeMatFFT2(FREQUENCY,sizeof(theSamples)/sizeof(WORD),theSamples,nn1,0,
//		RFSpectrumTable,RF_SPECTRUM_ACCU);
	TipoVis1=0;
	TipoVis2=2;
	redrawWindows();
	}

void CSDRemulatorApp::OnProveFftsumix() {
	double fin[FFT_LEN/2/FFT_LEN_BF_REDUCED],fout[FFT_LEN/FFT_LEN_BF_REDUCED],foutimg[FFT_LEN/FFT_LEN_BF_REDUCED];
	int decimate;
	double m;

	decimate=0; m=0;
	for(DWORD dw=0; dw<FFT_LEN/2; dw++) {
		//copy audio signal to fft real component for left channel
		m += (double)(int)theSamples[dw] -0x8000;
		decimate++;
		if(decimate==FFT_LEN_BF_REDUCED) {
			fin[dw/FFT_LEN_BF_REDUCED] = (double)((m/FFT_LEN_BF_REDUCED) );
			decimate=0; m=0;
			}
		}
	//Perform FFT on left channel
	m_FFT->fft_double(FFT_LEN/2/FFT_LEN_BF_REDUCED,0,fin,NULL,fout,foutimg);
	for(int i=1; i < FFT_LEN/4/FFT_LEN_BF_REDUCED; i++) {		// voglio 50-100KHz circa...
		Complex c(fout[i],foutimg[i]);
		//get amplitude and scale to 0..256 range
		//fdraw[i]=AmplitudeScaled(re,im,FFT_LEN/2,256);
		fdraw[i] = ((int)c.hypot()) /5000; //% 256;
		}


//	ComputeMatFFT2(FREQUENCY,THE_SAMPLES_SIZE,theSamples,nn1,0,
//		BFSpectrumTable,BF_SPECTRUM_ACCU);
	TipoVis1=0;
	TipoVis2=2;
	redrawWindows();
	}

void CSDRemulatorApp::OnProveMixfiltrata() {
	Complex IQbuffer,IQbuffer2;
	int i;
	Complex c;
	CFirFilter audioLowPass;
	double fil;
	
	
	CSinOscillator myOsc(SAMPLING_FREQUENCY,Frequency);
	CComplexOscillator *localOsc = new CComplexOscillator(SAMPLING_FREQUENCY, /* - */ Tune);

	for(i=0; i<THE_SAMPLES_SIZE /* /2 */; i++) {

		c=localOsc->nextSample();
		IQbuffer.re=myOsc.nextSample();
		IQbuffer.im=0;


// https://github.com/ac2cz/SDR/blob/master/src/tutorial5/OscTest2.java
//		IQbuffer2.re = IQbuffer.re*c.geti() + IQbuffer.im*c.getq();
//		IQbuffer2.im = IQbuffer.re*c.getq() - IQbuffer.im*c.geti() ;		eRRORE invertiti??

		IQbuffer2 = IQbuffer*c;

		double audio = IQbuffer2.re + IQbuffer2.im;

		switch(theApp.Filter) {
			case 0:
				fil = audioLowPass.filter6(audio);
				if(i>=4)		// il filtro si deve riempire...
					theSamples[i-4]= (fil*0x7fff) + 0x8000;
				break;
			case 1:
				fil = audioLowPass.filter32(audio);
				if(i>=32)		// il filtro si deve riempire...
					theSamples[i-32]= (fil*0x7fff) + 0x8000;
				break;
			case 2:
				break;
			}
		}

	delete localOsc;

	TipoVis1=0;
	TipoVis2=3;
	redrawWindows();
	}

void CSDRemulatorApp::OnProvePortantemodulata() {

	CreateWave(theSamples,THE_SAMPLES_SIZE,Frequency,SAMPLING_FREQUENCY,
		Modulation,ModulationType);

	TipoVis1=0;

	OnProveFftsuportante();

	redrawWindows();
	}

void CSDRemulatorApp::OnProvePortantemodulataoscillatorelocale() {
	Complex IQbuffer,IQbuffer2;
	int i;
	Complex c;
	CFirFilter audioLowPass;

	
//	CreateWave(theSamples,THE_SAMPLES_SIZE,FREQUENCY,SAMPLING_FREQUENCY,Modulation,ModulationType);
//	CreateWave(theSamples,THE_SAMPLES_SIZE,FREQUENCY,SAMPLING_FREQUENCY);

//	CreateWave(IQbuffer,THE_SAMPLES_SIZE,FREQUENCY,SAMPLING_FREQUENCY);

	
	CSinOscillator myOsc(SAMPLING_FREQUENCY,Frequency);

#pragma warning MANCA LA MODULAZIONE

	CComplexOscillator localOsc(SAMPLING_FREQUENCY, /* - */ Tune);

	for(i=0; i<THE_SAMPLES_SIZE /* /2 */; i++) {

//		theSamples[i]=(((((int)theSamples[i])-0x8000) + (((int)buffer[i])-0x8000)/2)/2)+0x8000;

		c=localOsc.nextSample();
		IQbuffer.re=myOsc.nextSample();
		IQbuffer.im=0;


// https://github.com/ac2cz/SDR/blob/master/src/tutorial5/OscTest2.java
//		IQbuffer2.re = IQbuffer.re*c.geti() + IQbuffer.im*c.getq();
//		IQbuffer2.im = IQbuffer.im*c.geti() - IQbuffer.re*c.getq();		eRRORE invertire??

		IQbuffer2 = IQbuffer*c;

		double audio = IQbuffer2.re + IQbuffer2.im;
//		double audio = IQbuffer[0];

//			double fil = audioLowPass.filter6(audio);
		double fil = audio;
		theSamples[i]= (fil*0x7fff) + 0x8000;
		}

	TipoVis1=0;
	TipoVis2=2;
	redrawWindows();
	}


void CSDRemulatorApp::OnProveModulazioneam() {

	ModulationType=1;
	OnProvePortantemodulata();
	}

void CSDRemulatorApp::OnProveModulazionefm() {

	ModulationType=2;
	OnProvePortantemodulata();
	}

void CSDRemulatorApp::OnProveModulazionessb() {

	ModulationType=3;
	OnProvePortantemodulata();
	}


/////////////////////////////////////////////////////////////////////////////
// CSDRemulatorApp message handlers

/*int CSDRemulatorApp::CreateWave(WORD *buffer,DWORD bufSize,DWORD frequency,DWORD sampleRate,
																DWORD modulation,BYTE modulationType) {
	WORD *p;
	int i,t,cnt,counter, cnt2,counter2;
	int modulazione;

	frequency*=NUM_SAMPLES;

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
	}*/

int CSDRemulatorApp::CreateWave(WORD *buffer,DWORD bufSize,DWORD frequency,DWORD sampleRate,
																DWORD modulation,BYTE modulationType) {

	CSinOscillator myOsc(sampleRate,frequency);

//	myOsc.setFrequency(frequency);

	switch(modulationType) {
		case 0:
			myOsc.CreateWave(buffer,bufSize);
			break;
		case 1:
		case 2:
		case 3:
			{
/*			CSinOscillator myOsc2(sampleRate,modulation);
//			myOsc2.setFrequency(modulation);
			for(i=0; i<bufSize; i++) {
				buffer[i] = ((buffer[i]-0x8000) * ((1+myOsc2.nextSample()) / ModulationRatio)) + 0x8000;
				}*/
			myOsc.setModulation(modulation,modulationType,ModulationRatio);
			myOsc.CreateWave(buffer,bufSize);
			}
			break;
		}

//	Frequency=frequency;
//	Tune=frequency -10000;			// 10KHz
	ModulationType=modulationType;
	if(modulationType) {
		Modulation=modulation;
		}

	return bufSize;
	}


COscillator::COscillator() {

	samplesPerSecond = 0;
	frequency = 0;
	phase = 0;
	phaseIncrement = 0;
	modulation=0;

	sinTable = new double[TABLE_SIZE];

	frequencyModulation=0;
	}

COscillator::COscillator(DWORD samplesPerSec, int freq) {

	phase = 0;
	modulation=0;

	samplesPerSecond = samplesPerSec;
	frequency = freq;
	phaseIncrement = 2 * PI * frequency / (double)samplesPerSecond;
	//sinTable = new double[TABLE_SIZE];
// ma passa SEMPRE dall'altro costruttore o NO????!

	frequencyModulation=0;
	}

COscillator::~COscillator() {

	delete []sinTable;
	}

double COscillator::nextSample() {

	phase += phaseIncrement;
	if(frequency > 0 && phase >= 2 * PI)
		phase = phase - 2*PI;
	if(frequency < 0 && phase <= 0)
		phase = phase + 2*PI;
	int idx = (int)(((int)(phase * (double)TABLE_SIZE/(2 * PI))) % TABLE_SIZE);

	return sinTable[idx];
	}

void COscillator::setFrequency(double freq) {

	frequency = freq;
	phaseIncrement = 2 * PI * frequency / (double)samplesPerSecond;		
	}

void COscillator::setModulation(double freq,BYTE mode,BYTE ratio) {

	modulation=mode;
	frequencyModulation=freq;
	ratioModulation=ratio;

	}

int COscillator::CreateWave(WORD *buffer,DWORD bufSize) {
	int i;
	CSinOscillator *modOsc=NULL;
	CCosOscillator *cosOsc=NULL,*modOsc2=NULL;
	double d1,d2,d3,dms,dmc;

	switch(modulation) {
		case 0:
			break;
		case 1:
		case 2:
			modOsc=new CSinOscillator(samplesPerSecond,frequencyModulation);
			break;
		case 3:
			modOsc=new CSinOscillator(samplesPerSecond,frequencyModulation);
			modOsc2=new CCosOscillator(samplesPerSecond,frequencyModulation);
			cosOsc=new CCosOscillator(samplesPerSecond,frequency);
			break;
		}

	for(i=0; i<bufSize; i++) {
		switch(modulation) {
			case 0:
				buffer[i] = (nextSample()*0x7fff) +0x8000;
				break;
			case 1:		// AM
				buffer[i] = ((nextSample() * (((1+modOsc->nextSample()) / ratioModulation) /*+.1  /* tanto per, ma peggiora FFT */ )) *0x7fff) +0x8000;
				break;
			case 2:		// FM
				phaseIncrement = ((modOsc->nextSample()) / 20 /* ~ +- 40KHz */ / ratioModulation) + (2 * PI * frequency) / (double)samplesPerSecond;		
				buffer[i] = (nextSample()*0x7fff) +0x8000;
				break;
			case 3:		// SSB
//				https://www.dsprelated.com/showarticle/176.php
//				https://www.geeksforgeeks.org/single-side-band-ssb-modulation-using-matlab/
//				http://dp.nonoo.hu/ssb-modulation-in-software/
//				https://eng.libretexts.org/Bookshelves/Electrical_Engineering/Electronics/Microwave_and_RF_Design_I_-_Radio_Systems_(Steer)/03%3A_Transmitters_and_Receivers/3.02%3A_Single-Sideband_and_Double-Sideband_Modulation
				dms=(((1+modOsc->nextSample()) / ratioModulation));
				dmc=(((1+modOsc2->nextSample()) / ratioModulation));
				d1=cosOsc->nextSample();
				d2=nextSample();

				d3=d1*dmc+d2*dms;
				d2=d1*dms+d2*dmc;
				d1=d3 - d2;		// così esce sia LSB che USB...

				{
				Complex IQbuffer(d1,0),c(d2,0),IQbuffer2;

				IQbuffer2.re = IQbuffer.re*c.geti() + IQbuffer.im*c.getq();
				IQbuffer2.im = IQbuffer.im*c.geti() - IQbuffer.re*c.getq();

//				d1 = IQbuffer2.re + IQbuffer2.im;
//				d1 = d1-d2;
				}

				buffer[i] = (d1*0x7fff) +0x8000;
				break;
			}
		}

	delete modOsc; delete modOsc2; delete cosOsc;

	return bufSize;
	}


CCosOscillator::CCosOscillator(DWORD samplesPerSec, int freq) {

	samplesPerSecond=samplesPerSec; frequency=freq;		// non riesco a chiamare il costruttore base £$%@#
	phaseIncrement = 2 * PI * frequency / (double)samplesPerSecond;		
	for(int n=0; n<TABLE_SIZE; n++) {
		sinTable[n] = cos(n*2.0*PI/(double)TABLE_SIZE);
		}
	}


CSinOscillator::CSinOscillator(DWORD samplesPerSec, int freq) {

	samplesPerSecond=samplesPerSec; frequency=freq;
	phaseIncrement = 2 * PI * frequency / (double)samplesPerSecond;		
	for(int n=0; n<TABLE_SIZE; n++) {
		sinTable[n] = sin(n*2.0*PI/(double)TABLE_SIZE);
		}
	}


CSquareOscillator::CSquareOscillator(DWORD samplesPerSec, int freq) {

	samplesPerSecond=samplesPerSec; frequency=freq;
	phaseIncrement = 2 * PI * frequency / (double)samplesPerSecond;		
	for(int n=0; n<TABLE_SIZE; n++) {
		sinTable[n] = sin(n*2.0*PI/(double)TABLE_SIZE) >= 0 ? 1 : 0;
		}
	}


CTriangleOscillator::CTriangleOscillator(DWORD samplesPerSec, int freq) {

	samplesPerSecond=samplesPerSec; frequency=freq;
	phaseIncrement = 2 * PI * frequency / (double)samplesPerSecond;		
	for(int n=0; n<TABLE_SIZE; n++) {
		sinTable[n] = (n*2.0*PI/(double)TABLE_SIZE)/PI;		// FINIRE!
		}
	}


CSawtoothOscillator::CSawtoothOscillator(DWORD samplesPerSec, int freq) {

	samplesPerSecond=samplesPerSec; frequency=freq;
	phaseIncrement = 2 * PI * frequency / (double)samplesPerSecond;		
	for(int n=0; n<TABLE_SIZE; n++) {
		sinTable[n] = (n*2.0*PI/(double)TABLE_SIZE)/PI;	// FINIRE!
		}
	}


CComplexOscillator::CComplexOscillator() {

	cosOsc = NULL;
	sinOsc = NULL;
	}

CComplexOscillator::CComplexOscillator(int samples, int freq) {

	cosOsc = new CCosOscillator(samples, freq);
	sinOsc = new CSinOscillator(samples, freq);
	}

CComplexOscillator::~CComplexOscillator() {

	delete sinOsc;
	delete cosOsc;
	}

void CComplexOscillator::setFrequency(double freq) {

	cosOsc->setFrequency(freq);
	sinOsc->setFrequency(freq);
	}

Complex CComplexOscillator::nextSample() {

	double i = cosOsc->nextSample();
	double q = sinOsc->nextSample();
	Complex c(i,q);
	return c;
	}



static double *dd, *ee;

static double costable[TRIGO_TABLE_ACCU],sintable[TRIGO_TABLE_ACCU];
static double costable2[TRIGO_TABLE_ACCU*4],sintable2[TRIGO_TABLE_ACCU*4];

int CSDRemulatorApp::initFFT() {
	int i;
	double d;

	if(!(dd=(double *)GlobalAlloc(GPTR,THE_SAMPLES_SIZE*sizeof(double))))
	  return 0;
	if(!(ee=(double *)GlobalAlloc(GPTR,THE_SAMPLES_SIZE*sizeof(double))))
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

// poi... https://c.yocker.com/8894/fft-in-tempo-reale.html
int CSDRemulatorApp::ComputeMatFFT2(DWORD freq, DWORD n, const WORD *p,
  double *s, double *s1, const int *Hertz,int nStep) {

	int i,j,ij,ip,ir,iq,ik,ix,in,im,iy,iz,iu;
	int x,hz;
	double *g;
	double z1,z2,a1,a2,b1,b2,d1,d2;
//	FILE *f;
	
  g=dd;
  for(i=0; i<n; i++)
    *g++=*p++ -0x8000;
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
    in /= FFT_DIV2;
    im=Hertz[j+1]*2;
    im /= FFT_DIV2;
	  for(i=in; i<im; i++) {
//	    iz=max(i,1);
	    ij=subMirror2(i);
	    a1=dd[ij]; a2=ee[ij];
	    s[j] += hypot(a1,a2);
	    }
    s[j] /= 40000;
	  }
  
  return 0;
	}

int _fastcall CSDRemulatorApp::subMirror(int n) {
	register int i=0;
  
	_asm {
		mov eax,n
		xor edx,edx
		mov ecx,10
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
	register /*short*/ int i=0;
  
/*	_asm {
		mov ax,word ptr n
		xor dx,dx
		mov cx,12
myLoop:
		rcr ax,1
    rcl dx,1
		loop myLoop

    mov i,dx
    }*/
	_asm {
		mov eax,n
		xor edx,edx
		mov ecx,12
myLoop:
		rcr eax,1
    rcl edx,1
		loop myLoop

    mov i,edx
    }
	return i;
  }
  
int CSDRemulatorApp::ComputeMatFFT(DWORD freq, DWORD n, const WORD *p,
  double *s, double *s1,const int *Hertz,int nStep) {

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

inline double CSDRemulatorApp::GetFrequencyIntensity(double re, double im) {
	return sqrt((re*re)+(im*im));
	}


void CSDRemulatorApp::OnProveParameters() {
	
	CParametersDlg myDlg;
	if(myDlg.DoModal() == IDOK) {
	
//		myDlg.m_samplingFrequency;
		Frequency=myDlg.m_carrierFrequency;
		Modulation=myDlg.m_modulationFrequency;
		Tune=myDlg.m_LocalOscFrequency;
		Filter=myDlg.m_Filter;

		theApp.redrawWindows();
		}
	}



