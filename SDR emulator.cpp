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
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	ON_COMMAND(ID_PROVE_PORTANTE, OnProvePortante)
	ON_COMMAND(ID_PROVE_FFTSUMIX, OnProveFftsumix)
	ON_COMMAND(ID_PROVE_FFTSUPORTANTE, OnProveFftsuportante)
	ON_COMMAND(ID_PROVE_MIXFILTRATA, OnProveMixfiltrata)
	ON_COMMAND(ID_PROVE_PORTANTEMODULATA, OnProvePortantemodulata)
	ON_COMMAND(ID_PROVE_PORTANTEMODULATAOSCILLATORELOCALE, OnProvePortantemodulataoscillatorelocale)
	ON_COMMAND(ID_PROVE_MODULAZIONEAM, OnProveModulazioneam)
	ON_COMMAND(ID_PROVE_MODULAZIONEFM, OnProveModulazionefm)
	ON_COMMAND(ID_PROVE_MODULAZIONESSB, OnProveModulazionessb)
	ON_COMMAND(ID_PROVE_PARAMETERS, OnProveParameters)
	ON_COMMAND(ID_FILE_NEW, OnFileNew)
	ON_COMMAND(ID_FILE_NUOVAFORMADONDA, OnFileNuovaformadonda)
	ON_UPDATE_COMMAND_UI(ID_FILE_NUOVAFORMADONDA, OnUpdateFileNuovaformadonda)
	ON_COMMAND(ID_FILE_NUOVAFFT, OnFileNuovafft)
	ON_UPDATE_COMMAND_UI(ID_FILE_NUOVAFFT, OnUpdateFileNuovafft)
	ON_COMMAND(ID_PROVE_MODULAZIONEUSB, OnProveModulazioneusb)
	ON_COMMAND(ID_PROVE_MODULAZIONELSB, OnProveModulazionelsb)
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

	m_FFT = new CFFT(FFT_LEN);

	TipoVis1=0; TipoVis2=4; TheZoom1=TheZoom2=1;
	TheFFTfrom=0; TheFFTto=0;

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

void CSDRemulatorApp::OnProveModulazioneusb() {

	ModulationType=5;
	OnProvePortantemodulata();
	}

void CSDRemulatorApp::OnProveModulazionelsb() {

	ModulationType=4;
	OnProvePortantemodulata();
	}

/////////////////////////////////////////////////////////////////////////////
// CSDRemulatorApp message handlers

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
		case 4:
		case 5:
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
	CCosOscillator *modOsc2=NULL;
	CComplexOscillator *coOsc=NULL;
	double d1,dms,dmc;
	Complex c,c1,c2;

	switch(modulation) {
		case 0:
			break;
		case 1:
		case 2:
			modOsc=new CSinOscillator(samplesPerSecond,frequencyModulation);
			break;
		case 3:
		case 4:
		case 5:
			modOsc=new CSinOscillator(samplesPerSecond,frequencyModulation);
			modOsc2=new CCosOscillator(samplesPerSecond,frequencyModulation);
			coOsc=new CComplexOscillator(samplesPerSecond,frequency);
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
				dms=modOsc->nextSample() / ratioModulation;
				dmc=modOsc2->nextSample() / ratioModulation;
				c=coOsc->nextSample();

				c1.re=dms; c1.im=0;
				c2.re=dmc; c2.im=0;
//				c= c2*c.re + c1*c.im;		// LSB
//				c= c2*c.re - c1*c.im;		// USB
				c.re= (c.im*dmc + c.re*dms) - (c.im*dms + c.re*dmc);	c.im=0;	// SSB

				d1=c.re + c.im;

				buffer[i] = (d1*0x7fff) +0x8000;
				break;
			case 4:		// SSB
				dms=modOsc->nextSample() / ratioModulation;
				dmc=modOsc2->nextSample() / ratioModulation;
				c=coOsc->nextSample();

				c1.re=dms; c1.im=0;
				c2.re=dmc; c2.im=0;
				c= c2*c.re + c1*c.im;		// LSB

				d1=c.re + c.im;

				buffer[i] = (d1*0x7fff) +0x8000;
				break;
			case 5:		// SSB
				dms=modOsc->nextSample() / ratioModulation;
				dmc=modOsc2->nextSample() / ratioModulation;
				c=coOsc->nextSample();

				c1.re=dms; c1.im=0;
				c2.re=dmc; c2.im=0;
				c= c2*c.re - c1*c.im;		// USB

				d1=c.re + c.im;

				buffer[i] = (d1*0x7fff) +0x8000;
				break;
			}
		}

	delete modOsc; delete modOsc2; delete coOsc;

	return bufSize;
	}


CCosOscillator::CCosOscillator(DWORD samplesPerSec, int freq) {

	samplesPerSecond=samplesPerSec; frequency=freq;		// non riesco a chiamare il costruttore base ?$%@#
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


