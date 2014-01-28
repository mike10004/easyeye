/**************************************************
*  This is a C translation of the matlab code
*  Author:
*  Xiaomei Liu
*  xliu5@cse.nd.edu
*  Computer Vision Research Laboratory
*  Department of Computer Science & Engineering
*  U. of Notre Dame
***************************************************/


/*function zi = interp2(varargin)
%INTERP2 2-D interpolation (table lookup).
%   ZI = INTERP2(X,Y,Z,XI,YI) interpolates to find ZI, the values of the
%   underlying 2-D function Z at the points in matrices XI and YI.
%   Matrices X and Y specify the points at which the data Z is given.
%   Out of range values are returned as NaN.
%
%   XI can be a row vector, in which case it specifies a matrix with
%   constant columns. Similarly, YI can be a column vector and it 
%   specifies a matrix with constant rows. 
%
%   ZI = INTERP2(Z,XI,YI) assumes X=1:N and Y=1:M where [M,N]=SIZE(Z).
%   ZI = INTERP2(Z,NTIMES) expands Z by interleaving interpolates between
%   every element, working recursively for NTIMES.  INTERP2(Z) is the
%   same as INTERP2(Z,1).
%
%   ZI = INTERP2(...,'method') specifies alternate methods.  The default
%   is linear interpolation.  Available methods are:
%
%     'nearest' - nearest neighbor interpolation
%     'linear'  - bilinear interpolation
%     'cubic'   - bicubic interpolation
%     'spline'  - spline interpolation
%
%   All the interpolation methods require that X and Y be monotonic and
%   plaid (as if they were created using MESHGRID).  X and Y can be
%   non-uniformly spaced.  For faster interpolation when X and Y are
%   equally spaced and monotonic, use the methods '*linear', '*cubic', or
%   '*nearest'.
%
%   For example, to generate a coarse approximation of PEAKS and
%   interpolate over a finer mesh:
%       [x,y,z] = peaks(10); [xi,yi] = meshgrid(-3:.1:3,-3:.1:3);
%       zi = interp2(x,y,z,xi,yi); mesh(xi,yi,zi)
%      
%   See also INTERP1, INTERP3, INTERPN, MESHGRID, GRIDDATA.

%   Copyright 1984-2000 The MathWorks, Inc.
%   $Revision: 5.28 $*/

#include <math.h>
#include <Masek.h>
#include <assert.h>

//const bool debug = true;

void Masek::interp2(const Masek::filter *z, const Masek::filter *xi, const Masek::filter *yi, const Masek::filter *zi)
{
	int nrows, ncols;
	int i;
	int ndx;
	double s;
	double t;
    assert(zi->hsize[0] == xi->hsize[0]);
    assert(zi->hsize[1] == xi->hsize[1]);
    assert(yi->hsize[0] == xi->hsize[0]);
    assert(yi->hsize[1] == xi->hsize[1]);
    
	nrows = z->hsize[0];
	ncols = z->hsize[1];

//	if (debug) fprintf(stdout, "interp2: allocating double array of size %lu (length %d) for z filter\n", sizeof(double)*xi->hsize[0]*xi->hsize[1], xi->hsize[0]*xi->hsize[1]);
//	zi->data = (double*)malloc(sizeof(double)*xi->hsize[0]*xi->hsize[1]);
//	if (debug) fprintf(stdout, "interp2: double array of size %lu (length %d) allocated for z filter\n", sizeof(double)*xi->hsize[0]*xi->hsize[1], xi->hsize[0]*xi->hsize[1]);
//	zi->hsize[0] = xi->hsize[0];
//	zi->hsize[1] = xi->hsize[1];

	  
	for (i = 0; i<xi->hsize[0]*xi->hsize[1]; i++)
	{
		if (xi->data[i]<1 || xi->data[i]>ncols || yi->data[i]<1 || yi->data[i]>nrows)
			zi->data[i] = sqrt((double)-1);
		else
		{
			ndx = (int)((floor(xi->data[i])-1)+(floor(yi->data[i])-1)*ncols);

			s = xi->data[i]-floor(xi->data[i]);
			t = yi->data[i]-floor(yi->data[i]);

			zi->data[i] = (z->data[ndx]*(1-t)+z->data[ndx+ncols]*t)*(1-s)+(z->data[ndx+1]*(1-t)+z->data[ndx+ncols+1]*t)*s;
		}

	}



}
