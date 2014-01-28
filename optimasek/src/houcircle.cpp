/**************************************************
*  This is a C translation from Masek's matlab code
*  Author:
*  Xiaomei Liu
*  xliu5@cse.nd.edu
*  Computer Vision Research Laboratory
*  Department of Computer Science & Engineering
*  U. of Notre Dame
***************************************************/

/*% houghcircle - takes an edge map image, and performs the Hough transform
% for finding circles in the image.
%
% Usage: 
% h = houghcircle(edgeim, rmin, rmax)
%
% Arguments:
%	edgeim      - the edge map image to be transformed
%   rmin, rmax  - the minimum and maximum radius values
%                 of circles to search for
% Output:
%	h           - the Hough transform
%
% Author: 
% Libor Masek
% masekl01@csse.uwa.edu.au
% School of Computer Science & Software Engineering
% The University of Western Australia
% November 2003*/

#include <math.h>
#include <string.h>
#include <Masek.h>

void addcircle(double *h, int hr, int hc, int cx, int cy, int radius);

void addcircle(double *h, int hr, int hc, int cx, int cy, int radius)
{
	int fixradius;
    int weight;
	double tmp;
	int i;
	int x, y, *px, *py, *validpx, *validpy, *ind;
    double costheta;
	int *mark;//bool
	int valid=0;
    
	tmp = radius/1.4142;
	fixradius = (int)(tmp);
	
	px = (int*) malloc((fixradius+1) *8*sizeof(int));
	py = (int*) malloc((fixradius+1) *8*sizeof(int));
	
	validpx = (int*) malloc((fixradius+1) *8*sizeof(int));
	validpy = (int*) malloc((fixradius+1) *8*sizeof(int));
	ind = (int*) malloc((fixradius+1) *8*sizeof(int));
	mark = (int*) malloc(hr*hc*sizeof(int));
	
	
	for (i = 0; i<hr*hc; i++)
		mark[i] = 0; //false;
	
	
	weight = 1;
	
	
	for (i = 0; i<fixradius+1; i++)
	{
        x = i;
		tmp = x*x;
		tmp = tmp/(radius*radius);
		costheta = sqrt(1 - tmp);
		y = (int)(radius*costheta+0.5);
		

		px[i] = cy+x;
		px[i+fixradius+1]= cy+y;
		px[i+2*(fixradius+1)]= cy+y;
		px[i+3*(fixradius+1)]= cy+x;
		px[i+4*(fixradius+1)] = cy-x;
		px[i+5*(fixradius+1)]= cy-y;
		px[i+6*(fixradius+1)]= cy-y;
		px[i+7*(fixradius+1)]= cy-x;

		py[i] = cx+y;
		py[i+fixradius+1]= cx+x;
		py[i+2*(fixradius+1)]= cx-x;
		py[i+3*(fixradius+1)]= cx-y;
		py[i+4*(fixradius+1)] = cx-y;
		py[i+5*(fixradius+1)]= cx-x;
		py[i+6*(fixradius+1)]= cx+x;
		py[i+7*(fixradius+1)]= cx+y;
	}	
	
	for (i = 0; i<8*(fixradius+1); i++)
	{
		if (px[i]>=1 && px[i]<=hr && py[i]>=1 && py[i]<=hc)
		{
			validpx[valid] = px[i];
			validpy[valid] = py[i];
			ind[valid] = (validpx[valid]-1)*hc+validpy[valid];//validpx[valid]+(validpy[valid]-1)*hr;
			
			if (mark[ind[valid]-1]==0/*false*/)
			{
				h[ind[valid]-1] = h[ind[valid]-1]+weight;
				mark[ind[valid]-1] = 1;//true;
				valid = valid+1;

			}
		}
	}
	/*printf("valid is %d\n", valid);*/
	free(px);
	free(py);
	free(validpx);
	free(validpy);
	free(ind);
	free(mark);

}

double* Masek::houghcircle(Masek::filter *m_edgeim, int rmin, int rmax)
{

  //function h = houghcircle(edgeim, rmin, rmax)
	double *edgeim;

	int rows, cols, nradii;
	int i, j, n;
	int dims[3];
	int *x, *y;
	int ysize = 0;
	int index, cx, cy;
	double *h;
	int valid = 0;
	


	edgeim = m_edgeim->data;
	rows = m_edgeim->hsize[0];
	cols = m_edgeim->hsize[1];
	nradii = rmax-rmin+1;
	
	dims[0] = rows;
	dims[1] = cols;
	dims[2] = nradii;

	h = (double*)malloc(sizeof(double)*dims[0]*dims[1]*dims[2]);
	
	memset(h, 0, sizeof(double)*dims[0]*dims[1]*dims[2]);

	x = (int *)malloc(sizeof(int)*(rows*cols));
	y = (int *)malloc(sizeof(int)*(rows*cols));

	for (i = 0; i<rows; i++)
	{
		for (j = 0; j<cols; j++)
		{	
			if (edgeim[i*cols+j]!=0)
			{
				y[ysize] = i;
				x[ysize] = j;
				ysize = ysize+1;	
			}
		}
	}
		

	for (index=0; index<ysize; index++)
	{    
		cx = x[index];
		cy = y[index];
    	for (n=0; n<nradii; n++)
			addcircle(&h[n*rows*cols],rows, cols, cx+1,cy+1,n+1+rmin);
	}
    
	free(x);
	free(y);
return h;
}


