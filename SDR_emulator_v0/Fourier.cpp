// Fourier.cpp: implementation of the Fourier class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Fourier.h"
#include <math.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

/*
 * fft.cpp
 *
 * loic fonteneau 15-feb-2001
 * Perform discrete FFT
 *
 * Original code : Don Cross <dcross@intersrv.com>
 * http://www.intersrv.com/~dcross/fft.html
 *
 */

#ifndef NULL
#define NULL '\0'
#endif


CFFT::CFFT(DWORD size) {
	fin = new double[size/2];
	fout = new double[size/2];
	foutimg = new double[size/2];
	fdraw = new double[size/2];

	}

CFFT::~CFFT() {

	delete []fin;
	delete []fout;
	delete []foutimg;
	delete []fdraw;
	}

//////////////////////////////////////////////////////////////////////////////////////
// do the fft for double numbers
//////////////////////////////////////////////////////////////////////////////////////
void CFFT::fft_double(unsigned int p_nSamples, bool p_bInverseTransform, 
											double *p_lpRealIn, double *p_lpImagIn, double *p_lpRealOut, double *p_lpImagOut) {

	if(!p_lpRealIn || !p_lpRealOut || !p_lpImagOut) 
		return;

	unsigned int NumBits;
	unsigned int i, j, k, n;
	unsigned int BlockSize, BlockEnd;

	double angle_numerator = 2.0 * PI;
	double tr, ti;

	if(!IsPowerOfTwo(p_nSamples) )	{
		return;
		}

	if(p_bInverseTransform) 
		angle_numerator = -angle_numerator;

	NumBits = NumberOfBitsNeeded ( p_nSamples );

	for(i=0; i < p_nSamples; i++)	{
		j = ReverseBits(i,NumBits);
		p_lpRealOut[j] = p_lpRealIn[i];
		p_lpImagOut[j] = (p_lpImagIn == NULL) ? 0.0 : p_lpImagIn[i];
		}


	BlockEnd = 1;
	for(BlockSize = 2; BlockSize <= p_nSamples; BlockSize <<= 1) {
		double delta_angle = angle_numerator / (double)BlockSize;
		double sm2 = sin (-2 * delta_angle);
		double sm1 = sin (-delta_angle);
		double cm2 = cos (-2 * delta_angle);
		double cm1 = cos (-delta_angle);
		double w = 2 * cm1;
		double ar[3], ai[3];

		for(i=0; i < p_nSamples; i += BlockSize ) {

			ar[2] = cm2;
			ar[1] = cm1;

			ai[2] = sm2;
			ai[1] = sm1;

			for(j=i, n=0; n < BlockEnd; j++, n++) {

				ar[0] = w*ar[1] - ar[2];
				ar[2] = ar[1];
				ar[1] = ar[0];

				ai[0] = w*ai[1] - ai[2];
				ai[2] = ai[1];
				ai[1] = ai[0];

				k = j + BlockEnd;
				tr = ar[0]*p_lpRealOut[k] - ai[0]*p_lpImagOut[k];
				ti = ar[0]*p_lpImagOut[k] + ai[0]*p_lpRealOut[k];

				p_lpRealOut[k] = p_lpRealOut[j] - tr;
				p_lpImagOut[k] = p_lpImagOut[j] - ti;

				p_lpRealOut[j] += tr;
				p_lpImagOut[j] += ti;

				}
			}

		BlockEnd = BlockSize;
		}

	if(p_bInverseTransform)	{
		double denom = (double)p_nSamples;

		for(i=0; i < p_nSamples; i++) {
			p_lpRealOut[i] /= denom;
			p_lpImagOut[i] /= denom;
			}
		}

	}


//////////////////////////////////////////////////////////////////////////////////////
// check is a number is a power of 2
//////////////////////////////////////////////////////////////////////////////////////
bool CFFT::IsPowerOfTwo( unsigned int p_nX ) {

	if(p_nX < 2) 
		return false;

	if(p_nX & (p_nX-1) )
		return false;

  return true;
	}



//////////////////////////////////////////////////////////////////////////////////////
// return needed bits for fft
//////////////////////////////////////////////////////////////////////////////////////
unsigned int CFFT::NumberOfBitsNeeded(unsigned int p_nSamples) {

	int i;

	if( p_nSamples < 2 ) {
		return 0;
		}

	for(i=0; ; i++) {
		if(p_nSamples & (1 << i) ) 
			return i;
    }

	}


//////////////////////////////////////////////////////////////////////////////////////
// ?
//////////////////////////////////////////////////////////////////////////////////////
unsigned int CFFT::ReverseBits(unsigned int p_nIndex, unsigned int p_nBits) {

	unsigned int i, rev;

	for(i=rev=0; i < p_nBits; i++) {
		rev = (rev << 1) | (p_nIndex & 1);
		p_nIndex >>= 1;
		}

	return rev;
	}



//////////////////////////////////////////////////////////////////////////////////////
// return a frequency from the basefreq and num of samples
//////////////////////////////////////////////////////////////////////////////////////
double CFFT::Index_to_frequency(unsigned int p_nBaseFreq, unsigned int p_nSamples, unsigned int p_nIndex) {

	if(p_nIndex >= p_nSamples) {
		return 0.0;
		}
	else if(p_nIndex <= p_nSamples/2)	{
		return ( (double)p_nIndex / (double)p_nSamples * p_nBaseFreq );
		}
	else {
		return ( -(double)(p_nSamples-p_nIndex) / (double)p_nSamples * p_nBaseFreq );
		}

	}

DWORD CFFT::Frequency_from_bin(unsigned int p_nBaseFreq, unsigned int fft_len, unsigned int p_nIndex) {

	return (p_nBaseFreq/fft_len)*p_nIndex;
	}

DWORD CFFT::binToFrequency(unsigned int p_nBaseFreq,unsigned int fftLength,int bin) {
	DWORD binBandwidth=p_nBaseFreq/(fftLength);		//VERIFICARE!!

	long freq = 0;
	if(bin < fftLength/2) {
		freq = (long)(bin*binBandwidth);
		} 
	else {
		freq = (long)( -1* (fftLength-bin)*binBandwidth);
		}
	return freq;
	}


// https://www.g0kla.com/sdr/tutorials/sdr_tutorial5.php
double *CFFTWindow::initBlackmanWindow(int len) {
	double *blackmanWindow = new double[len+1];

	for(int i=0; i <= len; i ++) {
		blackmanWindow[i] =  (0.42 - 0.5 * cos(2 * PI * i / len) 
				+ 0.08 * cos((4 * PI * i) / len));
		if(blackmanWindow[i] < 0)
			blackmanWindow[i] = 0;
		}
	return blackmanWindow;
	}


double CFirFilter::coeffs4[] = { 
	0.117119343653851,
	0.129091809713508,
	0.133251953115574,
	0.129091809713508,
	0.117119343653851
	};

double CFirFilter::coeffs32[] = { 
	-0.006907390651426909,
	-0.005312580651013577,
	-0.002644739091749330,
	 0.001183336967173867,
	 0.006192514984703184,
	 0.012327624024018900,
	 0.019453074574047193,
	 0.027354594681004485,
	 0.035747363433714395,
	 0.044290256550950494,
	 0.052605352818230700,
	 0.060301340430963822,
	 0.066999061029964030,
	 0.072357178973056519,
	 0.076095892581739766,
	 0.078016723675218447,
	 0.078016723675218447,
	 0.076095892581739766,
	 0.072357178973056519,
	 0.066999061029964030,
	 0.060301340430963822,
	 0.052605352818230700,
	 0.044290256550950494,
	 0.035747363433714395,
	 0.027354594681004485,
	 0.019453074574047193,
	 0.012327624024018900,
	 0.006192514984703184,
	 0.001183336967173867,
	-0.002644739091749330,
	-0.005312580651013577,
	-0.006907390651426909
	};

CFirFilter::CFirFilter() {

	gain = 1;

	M = 0;
	xv = new double[sizeof(coeffs32)/sizeof(double)];		// il max possibile!
	}

double *CFirFilter::initRaiseCosine(double sampleRate, double freq, double alpha, int len) {
	int M = len-1;
	double *coeffs = new double[len];
	double Fc = freq/sampleRate;
	int i;
	

	// VERIFICARE !
	double sumofsquares = 0;
	double *tempCoeffs = new double[len];
	int limit = (int)(0.5 / (alpha * Fc));
	for(i=0; i <= M; i++) {
		double sinc = (sin(2 * PI * Fc * (i - M/2)))/ (i - M/2);
		double cosc = cos(alpha * PI * Fc * (i - M/2)) / ( 1 - (pow((2 * alpha * Fc * (i - M/2)),2)));
		
		if(i == M/2) {
			tempCoeffs[i] = 2 * PI * Fc * cosc;
			} 
		else {
			tempCoeffs[i] = sinc * cosc;
			}
		
		// Care because ( 1 - ( 2 * pow((alpha * Fc * (i - M/2)),2))) is zero for 
		if((i-M/2) == limit || (i-M/2) == -limit) {
			tempCoeffs[i] = 0.25 * PI * sinc;
			} 
		
		sumofsquares += tempCoeffs[i]*tempCoeffs[i];
		}
	double gain = sqrt(sumofsquares);
	for(i=0; i < len; i++) {
		coeffs[i] = tempCoeffs[len-i-1]/gain;
		//System.out.println(coeffs[i]);
		}
	return coeffs;
	}
	
CFirFilter::~CFirFilter() {

	delete[] xv;
	}

double CFirFilter::filter6(double in) {
	double sum; 
	int i;

	M = sizeof(coeffs4)/sizeof(double)-1;
	for(i=0; i < M; i++) 
		xv[i] = xv[i+1];
	xv[M] = in * gain;
	sum = 0.0;
	for(i=0; i <= M; i++) 
		sum += (coeffs4[i] * xv[i]);

	return sum;
	}

double CFirFilter::filter32(double in) {
	double sum; 
	int i;

	M = sizeof(coeffs32)/sizeof(double)-1;
	for(i=0; i < M; i++) 
		xv[i] = xv[i+1];
	xv[M] = in * gain;
	sum = 0.0;
	for(i=0; i <= M; i++) 
		sum += (coeffs32[i] * xv[i]);

	return sum;
	}

