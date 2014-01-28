/**************************************************
*  This is a C translation from Masek's matlab code
*  Author:
*  Xiaomei Liu
*  xliu5@cse.nd.edu
*  Computer Vision Research Laboratory
*  Department of Computer Science & Engineering
*  U. of Notre Dame
***************************************************/



/*% circlecoords - returns the pixel coordinates of a circle defined by the
%                radius and x, y coordinates of its centre.
%
% Usage: 
% [x,y] = circlecoords(c, r, imgsize,nsides)
%
% Arguments:
%	c           - an array containing the centre coordinates of the circle
%	              [x,y]
%   r           - the radius of the circle
%   imgsize     - size of the image array to plot coordinates onto
%   nsides      - the circle is actually approximated by a polygon, this
%                 argument gives the number of sides used in this approximation. Default
%                 is 600.
%
% Output:
%	x		    - an array containing x coordinates of circle boundary
%	              points
%   y		    - an array containing y coordinates of circle boundary
%                 points
%
% Author: 
% Libor Masek
% masekl01@csse.uwa.edu.au
% School of Computer Science & Software Engineering
% The University of Western Australia
% November 2003

function [x,y] = circlecoords(c, r, imgsize,nsides)*/
#include <math.h>
#include <Masek.h>

int Masek::circlecoords(double x0, double y0, double r, int *imgsize, double _nsides, int **x, int **y)
{
	double a;
	int i;
	int *xd, *yd;
	int nsides;
	

	if (_nsides <0)
    	nsides = 600;
	else
        nsides = roundND(_nsides);

	xd = (int*) malloc(sizeof(int)*(2*nsides+1));
	yd = (int*) malloc(sizeof(int)*(2*nsides+1));
	for (i=0; i<=2*nsides; i++)
	{
		a = i*PI/nsides;
		
		xd[i] = roundND(r*cos(a)+x0+AdjPrecision);
		yd[i] = roundND(r*sin(a)+y0+AdjPrecision);

		//%get rid of -ves    
		//%get rid of values larger than image
		
		if (xd[i]>imgsize[1])
			xd[i] = imgsize[1];
		else if (xd[i]<=0)
			xd[i] = 1;

		if (yd[i]>imgsize[0])
			yd[i] = imgsize[0];
		else if (yd[i]<=0)
			yd[i] = 1;
    
	}
	*x = xd;
	*y = yd;

	return 2*nsides+1;
}
