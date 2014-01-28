/**************************************************
*  This is a C translation from Masek's matlab code
*  Author:
*  Xiaomei Liu
*  xliu5@cse.nd.edu
*  Computer Vision Research Laboratory
*  Department of Computer Science & Engineering
*  U. of Notre Dame
***************************************************/


/*% linecoords - returns the x y coordinates of positions along a line
%
% Usage: 
% [x,y] = linecoords(lines, imsize)
%
% Arguments:
%	lines       - an array containing parameters of the line in
%                 form
%   imsize      - size of the image, needed so that x y coordinates
%                 are within the image boundary
%
% Output:
%	x           - x coordinates
%	y           - corresponding y coordinates
%
% Author: 
% Libor Masek
% masekl01@csse.uwa.edu.au
% School of Computer Science & Software Engineering
% The University of Western Australia
% November 2003

function [x,y] = linecoords(lines, imsize)*/
//#include "global.h"
#include <Masek.h>

void Masek::linescoords(double *lines, int row, int col, int *x, int *y)
{
	int i;

	double xd, yd;
	
	for (i = 0; i<col; i++)
	{
		xd = i+1;
		yd = (-lines[2]-lines[0]*xd)/lines[1];
		if (yd>row)
			yd = row;
		else if (yd<1)
			yd = 1;

		x[i] = (int)(xd+AdjPrecision);
		y[i] = (int)(yd+AdjPrecision);

	}


}