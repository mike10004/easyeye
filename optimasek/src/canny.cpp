/**************************************************
*  This is a C translation from Masek's matlab code
*  Author:
*  Xiaomei Liu
*  xliu5@cse.nd.edu
*  Computer Vision Research Laboratory
*  Department of Computer Science & Engineering
*  U. of Notre Dame
***************************************************/

/*% CANNY - Canny edge detection
%
% Function to perform Canny edge detection. Code uses modifications as
% suggested by Fleck (IEEE PAMI No. 3, Vol. 14. March 1992. pp 337-345)
%
% Usage: [gradient or] = canny(im, sigma)
%
% Arguments:   im       - image to be procesed
%              sigma    - standard deviation of Gaussian smoothing filter
%                      (typically 1)
%		       scaling  - factor to reduce input image by
%		       vert     - weighting for vertical gradients
%		       horz     - weighting for horizontal gradients
%
% Returns:     gradient - edge strength image (gradient amplitude)
%              or       - orientation image (in degrees 0-180, positive
%                         anti-clockwise)
%
% See also:  NONMAXSUP, HYSTHRESH

% Author: 
% Peter Kovesi   
% Department of Computer Science & Software Engineering
% The University of Western Australia
% pk@cs.uwa.edu.au  www.cs.uwa.edu.au/~pk
%
% April 1999    Original version
% January 2003  Error in calculation of d2 corrected
% March 2003	Modified to accept scaling factor and vertical/horizontal
%		        gradient bias (Libor Masek)

function [gradient, or] = canny(im, sigma, scaling, vert, horz)*/

#include <malloc.h>
#include <Masek.h>
#include "math.h"

Masek::IMAGE* Masek::canny(IMAGE *im, double sigma, double scaling, double vert, double horz, filter *gradient, filter *orND)
{

  filter  gaussian, *newim;
	int i, j;
	int hsize[2];
	int rows, cols;
	double xscaling, yscaling;
	double *h, *v, *d1, *d2, X, Y, begin, end;

	xscaling = vert;
	yscaling = horz;

	hsize[0] = (int)(6*sigma+1);
	hsize[1] = (int)(6*sigma+1);  // % The filter size.

	gaussian.hsize[0] = hsize[0];
	gaussian.hsize[1] = hsize[1];
	gaussian.data = (double*) malloc(sizeof(double)*hsize[0]*hsize[1]);

	/*gaussian = fspecial('gaussian',hsize,sigma);
	im = filter2(gaussian,im);        % Smoothed image.*/
	
    CREATEGAUSS (hsize, sigma, &gaussian);
	
	newim = filter2(gaussian,im);
	
	newim = imresize(newim, scaling);
	
	rows = newim->hsize[0];
	cols = newim->hsize[1];

	
	h = (double*)malloc(sizeof(double)*rows*cols);

	for (i = 0; i<rows; i++)
	{
		for (j = 0; j<cols; j++)
		{
			if (j == 0)
				*(h+i*cols+j) = (newim->data[i*cols+1]);
			else if (j == cols-1)
				*(h+i*cols+j) = (-newim->data[i*cols+j-1]);
			else
				*(h+i*cols+j) = (newim->data[i*cols+j+1]-newim->data[i*cols+j-1]);
			
		}
	}

	v = (double*)malloc(sizeof(double)*rows*cols);

	for (i = 0; i<rows; i++)
	{
		for (j = 0; j<cols; j++)
		{
			if (i == 0)
				*(v+i*cols+j) = (newim->data[(i+1)*cols+j]);
			else if (i == rows-1)
				*(v+i*cols+j) = -newim->data[(i-1)*cols+j];
			else
				*(v+i*cols+j) = (newim->data[(i+1)*cols+j]-newim->data[(i-1)*cols+j]);
			
		}
	}

	d1 = (double*)malloc(sizeof(double)*rows*cols);
	d2 = (double*)malloc(sizeof(double)*rows*cols);

	for (i = 0; i<rows; i++)
	{
		for (j = 0; j<cols; j++)
		{
			if (i == rows-1 || j == cols-1)
				begin = 0;
			else
				begin = newim->data[(i+1)*cols+j+1];
			
			if (i == 0 || j == 0)
				end = 0;
			else
				end = newim->data[(i-1)*cols+j-1];
					
			*(d1+i*cols+j) = begin-end;
			
		}
	}

	for (i = 0; i<rows; i++)
	{
		for (j = 0; j<cols; j++)
		{
			if (i == 0 || j == cols-1)
				begin = 0;
			else
				begin = newim->data[(i-1)*cols+j+1];
			
			if (i == rows-1 || j == 0)
				end = 0;
			else
				end = newim->data[(i+1)*cols+j-1];
					
			*(d2+i*cols+j) = begin-end;
			
		}
	}

	orND->data = (double*)malloc(sizeof(double)*rows*cols);
	orND->hsize[0] = rows;
	orND->hsize[1] = cols;

	gradient->data = (double*)malloc(sizeof(double)*rows*cols);
	gradient->hsize[0] = rows;
	gradient->hsize[1] = cols;

	
	for (i = 0; i<rows*cols;i++)
	{
        X = (h[i]+(d1[i]+d2[i])/2.0)*xscaling;
		
		Y = (v[i]+(d1[i]-d2[i])/2.0)*yscaling;

		gradient->data[i] = sqrt(X*X+Y*Y);
	
		orND->data[i] = atan2(-Y, X);
		
		/*if (i == 14)
		{
		printf("h is %f, d1 is %f, d2 is %f, v is %f xscaling is %f, yscaling is %f\n", h[i], d1[i], d2[i], v[i], xscaling, yscaling);
		printf("X is %f, Y is %f\n", X, Y);
		printf("orND is %f\n", orND->data[i]);
		}
*/
					
		if (orND->data[i]<0)
			orND->data[i]+=PI;
		
//		if (i == 14)
//			printf("orND is %f\n", orND->data[i]);		
		
		orND->data[i] = (orND->data[i]/PI)*180;
//		if (i == 14)
//			printf("orND is %f\n", orND->data[i]);		
		
		
	}
	
	free (gaussian.data);
	free(d1);
	free(d2);
	free(h);
	free(v);

	free(newim->data);
	free(newim);

	return im;
}
