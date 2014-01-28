/**************************************************
*  This is a C translation from Masek's matlab code
*  Author:
*  Xiaomei Liu
*  xliu5@cse.nd.edu
*  Computer Vision Research Laboratory
*  Department of Computer Science & Engineering
*  U. of Notre Dame
***************************************************/

/*% shiftbits - function to shift the bit-wise iris patterns in order to provide the best match
% each shift is by two bit values and left to right, since one pixel value in the
% normalised iris pattern gives two bit values in the template
% also takes into account the number of scales used
%
% Usage: 
% [template, mask] = createiristemplate(eyeimage_filename)
%
% Arguments:
%	template        - the template to shift
%   noshifts        - number of shifts to perform to the right, a negative
%                     value results in shifting to the left
%   nscales         - number of filters used for encoding, needed to
%                     determine how many bits to move in a shift
%
% Output:
%   templatenew     - the shifted template
%
% Author: 
% Libor Masek
% masekl01@csse.uwa.edu.au
% School of Computer Science & Software Engineering
% The University of Western Australia
% November 2003*/
#include <math.h>
#include <Masek.h>

void Masek::shiftbits(int *templates, int width, int height, int noshifts,int nscales, int *templatenew)
{
	int i, s, p, x, y;
	for (i = 0; i<width*height; i++)
		templatenew[i] = 0;

	
	s = 2*nscales*(int)fabs((float)noshifts);
	p = width-s;

	if (noshifts == 0)
	{
		for (i = 0; i<width*height; i++)
			templatenew[i] = templates[i];
	}
    
    /*% if noshifts is negatite then shift towards the left
elseif noshifts < 0*/
    
	else if (noshifts<0)
	{
	  for (y = 0; y<height; y++)
	    for (x = 0; x<p; x++)

	      templatenew[y*width+x] = templates[y*width+s+x];

//templatenew[y, x] = templates[y, s+x];
    
	  for (y = 0; y<height; y++)
	    for (x=p; x<width; x++)

	      templatenew[y*width+ x] = templates[y*width+ x-p];

//templatenew[y, x] = templates[y, x-p];
	}
    
	else
	{
	  for (y = 0; y<height; y++)
	    for (x = s; x<width; x++)

	      templatenew[y*width+ x] = templates[y*width+x-s];

//templatenew[y, x] = templates[y, x-s];
    
	  for (y = 0; y<height; y++)
	    for (x=0; x<s; x++)
	      templatenew[y*width+x] = templates[y*width+ p+x];

	}
}
