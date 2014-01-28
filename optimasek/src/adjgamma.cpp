/**************************************************
*  This is a C translation from Masek's matlab code
*  Author:
*  Xiaomei Liu
*  xliu5@cse.nd.edu
*  Computer Vision Research Laboratory
*  Department of Computer Science & Engineering
*  U. of Notre Dame
***************************************************/

/*% ADJGAMMA - Adjusts image gamma.
%
% function g = adjgamma(im, g)
%
% Arguments:
%            im     - image to be processed.
%            g      - image gamma value.
%                     Values in the range 0-1 enhance contrast of bright
%                     regions, values > 1 enhance contrast in dark
%                     regions. 

% Author: Peter Kovesi
% Department of Computer Science & Software Engineering
% The University of Western Australia
% pk@cs.uwa.edu.au     www.cs.uwa.edu.au/~pk
% July 2001*/

#include <math.h>
#include <Masek.h>

Masek::filter * Masek::adjgamma(filter *im, double g)
{
	double min, max;
	int i;
	if (g <= 0)
	{
		printf("Gamma value must be > 0\n");
		return im;
	}

    
	//% rescale range 0-1
    min = im->data[0];
	for (i = 0; i<im->hsize[0]*im->hsize[1]; i++)
		if (im->data[i]<min)
			min = im->data[i];

	
	for (i = 0; i<im->hsize[0]*im->hsize[1]; i++)
		im->data[i] = im->data[i]-min;

	max = im->data[0];
	for (i = 0; i<im->hsize[0]*im->hsize[1]; i++)
		if (im->data[i]>max)
			max = im->data[i];

	
	for (i = 0; i<im->hsize[0]*im->hsize[1]; i++)
	{
		im->data[i] = im->data[i]/max;
		im->data[i] = pow(im->data[i], 1/g); // % Apply gamma function
	}
	return im;
}

