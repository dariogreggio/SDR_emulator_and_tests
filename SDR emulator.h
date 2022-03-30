// SDR emulator.h : main header file for the SDR EMULATOR application
//

#if !defined(AFX_SDREMULATOR_H__22D57ECA_E549_45A7_B550_AE8282D21FA6__INCLUDED_)
#define AFX_SDREMULATOR_H__22D57ECA_E549_45A7_B550_AE8282D21FA6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CSDRemulatorApp:
// See SDR emulator.cpp for the implementation of this class
//

#include "fourier.h"

#define NUM_SAMPLES 8
#define THE_SAMPLES_SIZE 1000000UL
#define MODULATION_DEFAULT_RATIO 2

#define TRIGO_TABLE_ACCU 512
#define RF_SPECTRUM_ACCU 256
#define BF_SPECTRUM_ACCU 128
#define FFT_DIV 43
#define FFT_DIV2 256
#define PI2 2*PI
//#define MAX_FREQ 44100
#define MAX_ZTABLE 500
#define FREQUENCY 1000000UL //1010000UL
#define SAMPLING_FREQUENCY 32000000UL		// a 8 samples per ciclo posso fare al max 4MHz
#define SAMPLE_ACCU 5


class CSDRemulatorApp : public CWinApp {
public:
	WORD theSamples[THE_SAMPLES_SIZE+1];
	DWORD Frequency,Modulation;
	BYTE ModulationType,ModulationRatio;
	BYTE TipoVis;
	BYTE TheZoom;
	double nn1[RF_SPECTRUM_ACCU+1],oldnn1[RF_SPECTRUM_ACCU+1];		// è il più grande :)
	static const WORD SINE_WAVE_TABLE[NUM_SAMPLES];
	int RFSpectrumTable[RF_SPECTRUM_ACCU+1];
	int BFSpectrumTable[BF_SPECTRUM_ACCU+1];

	CFFT *m_FFT;
	double fdraw[FFT_LEN/2];

public:
	int CreateWave(WORD *buffer,DWORD bufSize,DWORD frequency,DWORD sampleRate,DWORD modulation=0,BYTE modulationType=0);

	int ComputeMatFFT2(DWORD freq,DWORD n,const WORD *p,double *s,double *s1,const int *Hertz,int nStep);
	int ComputeMatFFT(DWORD freq,DWORD n,const WORD *p,double *s,double *s1,const int *Hertz,int nStep);
	int initFFT();
	int _fastcall subMirror(int n);
	int _fastcall subMirror2(int n);
	inline double CSDRemulatorApp::GetFrequencyIntensity(double re, double im);

	CSDRemulatorApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSDRemulatorApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CSDRemulatorApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

extern CSDRemulatorApp theApp;


class COscillator {
public:
	enum {
		TABLE_SIZE = 128
	};
public:
	double *sinTable;
	int samplesPerSecond;
	double frequency;
	double phase;
	double phaseIncrement;
	
	COscillator();
	COscillator(int samples, int freq);
	~COscillator();

	};

class CSinOscillator : public COscillator {
public:

	CSinOscillator(int samples, int freq);
	};

class CCosOscillator : public COscillator {
public:

	CCosOscillator(int samples, int freq);
	};

class CComplexOscillator {
public:
	CCosOscillator cosOsc;
	CSinOscillator sinOsc;
	
public:
	CComplexOscillator(int samples, int freq);
	
	void setFrequency(double freq);
	
	Complex nextSample();
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SDREMULATOR_H__22D57ECA_E549_45A7_B550_AE8282D21FA6__INCLUDED_)
