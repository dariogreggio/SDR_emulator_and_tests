// Fourier.h: interface for the Fourier class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FOURIER_H__6939C9A4_8D93_41A3_9521_3B863957EB8E__INCLUDED_)
#define AFX_FOURIER_H__6939C9A4_8D93_41A3_9521_3B863957EB8E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <math.h>

/*
 * fft.h
 *
 * loic fonteneau 15-feb-2001
 * Perform discrete FFT
 *
 * Original code : Don Cross <dcross@intersrv.com>
 * http://www.intersrv.com/~dcross/fft.html
 *
 */


class Complex {
public:
	double re,im;
public:
	Complex() { re=0.0; im=0.0; }
	Complex(double r, double i) { re=r; im=i; }
	double getq() { return re; }
	double geti() { return im; }
	double hypot() { return sqrt(re*re+im*im); }
	Complex operator+(Complex c2) { Complex ret; ret.re = re + c2.re; ret.im = im + c2.im; return ret; }
	Complex operator-(Complex c2) { Complex ret; ret.re = re - c2.re; ret.im = im - c2.im; return ret; }
	Complex operator+(double d) { Complex ret; ret.re = re + d; return ret; }
	Complex operator-(double d) { Complex ret; ret.re = re - d; return ret; }
	Complex operator*(Complex c) { double real1,real2; real1=re; real2=c.re; re=(re*c.re)-(im*c.im); \
		/* if (a,b)(c,d) then formula of multiplication is (ac-bd,ad+bc) */ \
		im=(real1*c.im)+(im*real2); Complex temp(re,im); return temp; }
	Complex operator*(double d) { re=re*d; im=im*d; Complex temp(re,im); return temp; }
	Complex operator++() { Complex x; x.re=++re; x.im=++im; return x; }
	};

///////////////////////////
//  function prototypes  //
///////////////////////////

#define FFT_LEN 16384
#define FFT_LEN_BF_REDUCED 16
//#define mag_sqrd(re,im) (re*re+im*im)
#define Decibels(re,im) ((re == 0 && im == 0) ? (0) : 10.0 * log10(double(mag_sqrd(re,im))))
#define Amplitude(re,im,len) (GetFrequencyIntensity(re,im)/(len))
#define AmplitudeScaled(re,im,len,scale) ((int)Amplitude(re,im,len)%scale)
#define dither() (((double)rand())/RAND_MAX)


class CFFT {
public:
	CFFT(DWORD);
	~CFFT();
public:
	void fft_double(unsigned int p_nSamples, bool p_bInverseTransform, double *p_lpRealIn, double *p_lpImagIn, double *p_lpRealOut, double *p_lpImagOut);
	bool IsPowerOfTwo(unsigned int p_nX);
	unsigned int NumberOfBitsNeeded(unsigned int p_nSamples);
	unsigned int ReverseBits(unsigned int p_nIndex, unsigned int p_nBits);
	unsigned int _fastcall CFFT::subMirror(unsigned int n);
	double Index_to_frequency(unsigned int p_nBaseFreq, unsigned int p_nSamples, unsigned int p_nIndex);
	DWORD Frequency_from_bin(unsigned int p_nBaseFreq, unsigned int fft_len, unsigned int p_nIndex);
	DWORD binToFrequency(unsigned int p_nBaseFreq,unsigned int fft_len,int bin);

public:
	double *fin,*fout,*foutimg,*fdraw;

	};



/////////////////////////////
//  constants-definition  //
/////////////////////////////

class CFFTWindow {

	static double *initBlackmanWindow(int len) ;
	};

class CFirFilter {
public:
	static double coeffs4[],coeffs32[];
	
	double *xv;  // This array holds the delayed values
	double gain;
	int M; // The number of taps, the length of the filter
	
public:
	CFirFilter();
	~CFirFilter();
	static double *initRaiseCosine(double sampleRate, double freq, double alpha, int len);
	double filter6(double in);
	double filter32(double in);
	};



#endif // !defined(AFX_FOURIER_H__6939C9A4_8D93_41A3_9521_3B863957EB8E__INCLUDED_)
