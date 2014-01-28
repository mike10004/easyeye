/**************************************************
*  This is a C translation from Masek's matlab code
*  Author:
*  Xiaomei Liu
*  xliu5@cse.nd.edu
*  Computer Vision Research Laboratory
*  Department of Computer Science & Engineering
*  U. of Notre Dame
***************************************************/

/*% gaborconvolve - function for convolving each row of an image with 1D log-Gabor filters
%
% Arguments:
%   im              - the image to convolve
%   nscale          - number of filters to use
%   minWaveLength   - wavelength of the basis filter
%   mult            - multiplicative factor between each filter
%   sigmaOnf        - Ratio of the standard deviation of the Gaussian describing
%                     the log Gabor filter's transfer function in the frequency
%                     domain to the filter center frequency.
%
% Output:
%   E0              - a 1D cell array of complex valued comvolution results
%
% Author: 
% Original 'gaborconvolve' by Peter Kovesi, 2001
% Heavily modified by Libor Masek, 2003
% masekl01@csse.uwa.edu.au
% School of Computer Science & Software Engineering
% The University of Western Australia
% November 2003


function [EO, filtersum] = gaborconvolve(im, nscale, minWaveLength, mult, ...
    sigmaOnf)*/

//modifiedy from http://www.cs.princeton.edu/introcs/97data/FFT.java.html

// compute the FFT of x[], assuming its length is a power of 2
#include <math.h>
#include <string.h>
#include <Masek.h>

//FILE *fid;
/*
function y = fftshift(x,dim)
%FFTSHIFT Shift zero-frequency component to center of spectrum.
%   For vectors, FFTSHIFT(X) swaps the left and right halves of
%   X.  For matrices, FFTSHIFT(X) swaps the first and third
%   quadrants and the second and fourth quadrants.  For N-D
%   arrays, FFTSHIFT(X) swaps "half-spaces" of X along each
%   dimension.
%
%   FFTSHIFT(X,DIM) applies the FFTSHIFT operation along the 
%   dimension DIM.
%
%   FFTSHIFT is useful for visualizing the Fourier transform with
%   the zero-frequency component in the middle of the spectrum.
%
%   See also IFFTSHIFT, FFT, FFT2, FFTN.

%   Copyright 1984-2000 The MathWorks, Inc.
%   $Revision: 5.8 $  $Date: 2000/06/14 21:10:10 $


*/
void Masek::fftshift(double *x, int numDims, int size[])
{
	int k;
	int count;
	int i, m, p;
	double *y;
	int *idx;
    
	
	count = 0;

	y = (double*)malloc(sizeof(double)*size[0]*size[1]);
	memcpy(y, x, sizeof(double)*size[0]*size[1]);
	idx = (int*)malloc(sizeof(int)*size[0]*size[1]);

    for (k = numDims-1; k<numDims; k++)
	{
        m = size[k];
        p = (int) ceil(((double)m)/2);
		for (i = p+1; i<=m; i++)
			idx[count++] = i-1;
		
		for (i = 1; i<=p; i++)
			idx[count++] = i-1;
				 
	}


//% Use comma-separated list syntax for N-D indexing.

	for (i = 0; i<count; i++)
		x[i] = y[idx[i]];

	free(y);
	free(idx);
	

}

void Masek::dft(Masek::Complex *x, Masek::Complex *y, int N)
{
	Complex wk, mul;
	double kth;
	int k, j;
	
	// base case
	if (N ==1)
	{
		y[0].real = x[0].real;
		y[0].img = x[0].img;
	}

     
    for (k = 0; k < N; k++) 
	{
		for (j = 0; j<N; j++)
		{
           kth = -2 * k *j*PI / N;
			wk.real = cos(kth);
			wk.img = sin(kth);
            
			mul.real = wk.real*x[j].real-wk.img*x[j].img;
			mul.img = wk.real*x[j].img+wk.img*x[j].real;

			y[k].real += mul.real;
			y[k].img += mul.img;
            
		}    
	}
		
}

Masek::Complex* Masek::fft(Masek::Complex* x, int N)
{

  Complex *y;
	Complex *even, *odd, *q, *r, wk;
	double kth;
	Complex mul;
	int k;
		
	y = (Complex*)malloc(sizeof(Complex)*N);
	memset(y, 0, sizeof(Complex)*N);
    
	// base case
	if (N ==1)
	{
		y[0].real = x[0].real;
		y[0].img = x[0].img;
		return y;
	}

     
    // radix 2 Cooley-Tukey FFT
    if (N % 2 != 0)
	{
	//	printf("N is not a power of 2");
		dft(x, y, N);
		return y;
	}


  even = (Complex*)malloc(sizeof(Complex)*N/2);
	odd = (Complex*)malloc(sizeof(Complex)*N/2);
	
	for (k = 0; k < N/2; k++) 
	{
		even[k].real = x[2*k].real;
		even[k].img = x[2*k].img;
	}

    for (k = 0; k < N/2; k++) 
	{
		odd[k].real  = x[2*k + 1].real;
		odd[k].img  = x[2*k + 1].img;
	}

    q = fft(even, N/2);
    r = fft(odd, N/2);

    for (k = 0; k < N/2; k++) 
	{
            kth = -2 * k * PI / N;
			wk.real = cos(kth);
			wk.img = sin(kth);
            
			mul.real = wk.real*r[k].real-wk.img*r[k].img;
			mul.img = wk.real*r[k].img+wk.img*r[k].real;

			y[k].real = q[k].real+mul.real;
			y[k].img = q[k].img+mul.img;

			y[k+N/2].real = q[k].real-mul.real;
			y[k+N/2].img = q[k].img-mul.img;
            
	}    
	free(q);
	free(r);
	free(even);
	free(odd);
        
    return y;
}


    // compute the inverse FFT of x[], assuming its length is a power of 2
Masek::Complex * Masek::ifft(Masek::Complex* x, int N) 
{
    Complex *y;
		int i;

		// take conjugate
        for ( i = 0; i < N; i++)
		{
//            x[i].real = x[i].real;
			x[i].img = -x[i].img;
		}

        // compute forward FFT
        y = fft(x, N);

        // take conjugate again
        for ( i = 0; i < N; i++)
		{
//            y[i].real = y[i].real;
			y[i].img = -y[i].img;
		}
		

        // divide by N
        for ( i = 0; i < N; i++)
		{
            y[i].real = y[i].real/ N;
			y[i].img = y[i].img/ N;
		}

        return y;

    }


void Masek::gaborconvolve(Masek::filter* im, int nscale, int minWaveLength, int mult, double sigmaOnf, Masek::Complex*** EO, double** filtersum, int *EOh, int *EOw)
{
	int rows, cols;
	int ndata;
	double *logGabor, *radius, *m_filter;
	int wavelength;
	int i, j, s, r;
	double fo, rfo;
	Complex *ft, *imagefft, *signal;
	int size[2];

	rows = im->hsize[0];
	cols = im->hsize[1];

	//*filtersum = (double*)malloc(sizeof(double)*cols);
	//memset(*filtersum, 0, sizeof(double)*cols);
	
//	EO = cell(1, nscale);         // % Pre-allocate cell array

	ndata = cols;
	if (ndata/2 == 1)             //% If there is an odd No of data points 
		ndata = ndata-1;               //% throw away the last one.

	/*logGabor = (double*)malloc(sizeof(double)*ndata);
	memset(logGabor, 0, sizeof(double)*ndata);
	*/

	
	//memset(result, 0, sizeof(double)*ndata*rows);

	radius = (double*)malloc(sizeof(double)*(fix(ndata/2)+1));
	
	for (i = 0; i<fix(ndata/2)+1; i++)
	{
		radius[i] = ((double)i)/fix(ndata/2)/2;
	}
	
	radius[0] = 1;

	wavelength = minWaveLength;        //% Initialize filter wavelength.

	logGabor = (double*)malloc(sizeof(double)*ndata);
	memset(logGabor, 0, sizeof(double)*ndata);

	*filtersum = (double*)malloc(sizeof(double)*ndata);
	memset(*filtersum, 0, sizeof(double)*ndata);

	imagefft = (Complex*)malloc(sizeof(Complex)*ndata);

	for (s = 0; s<nscale; s++)                  //% For each scale.  
	{
        //% Construct the filter - first calculate the radial filter component.
		fo = 1.0/wavelength;                 // % Centre frequency of filter.
		rfo = fo/0.5;                         //% Normalised radius from centre of frequency plane 
		//% corresponding to fo.
		

		for (j = 0; j<ndata/2+1; j++)
		{
			logGabor[j] = exp(-log(radius[j]/fo)*log(radius[j]/fo)/(2*log(sigmaOnf)*log(sigmaOnf)));
			
		}
		
		logGabor[0] = 0;  
    
		m_filter = logGabor;
    

		for (j = 0; j<ndata/2+1; j++)
		{
			(*filtersum)[j] += m_filter[j];
			
		}
		    
		//% for each row of the input image, do the convolution, back transform
		signal = (Complex*)malloc(sizeof(Complex)*ndata);
		EO[s] = (Complex**)malloc(sizeof(Complex*)*rows);
		for (r = 0; r<rows; r++)//	% For each row
		{
			for (j = 0; j<ndata; j++)
			{
				signal[j].real = im->data[r*im->hsize[1]+j];
				signal[j].img = 0;
			}
			
			ft = fft( signal, ndata);

			for (j = 0; j<ndata; j++)
			{
				imagefft[j].real = ft[j].real*m_filter[j];
				imagefft[j].img = ft[j].img*m_filter[j];
			}
		//% save the ouput for each scale			                
			EO[s][r] = ifft(imagefft, ndata);
			free(ft);
        
		}
		free(signal);
		wavelength = wavelength * mult;       //% Finally calculate Wavelength of next filter*/
	}                                         //% ... and process the next scale

	size[0] = 1;
	size[1] = cols;
	fftshift(*filtersum, 2, size);

	*EOh = rows;
	*EOw = ndata;

	free(logGabor);
	free(radius);
	free(imagefft);
}
