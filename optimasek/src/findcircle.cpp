/**************************************************
*  This is a C translation from Masek's matlab code
*  Author:
*  Xiaomei Liu
*  xliu5@cse.nd.edu
*  Computer Vision Research Laboratory
*  Department of Computer Science & Engineering
*  U. of Notre Dame
***************************************************/


/*% findcircle - returns the coordinates of a circle in an image using the Hough transform
% and Canny edge detection to create the edge map.
%
% Usage: 
% [row, col, r] = findcircle(image,lradius,uradius,scaling, sigma, hithres, lowthres, vert, horz)
%
% Arguments:
%	image		    - the image in which to find circles
%	lradius		    - lower radius to search for
%	uradius		    - upper radius to search for
%	scaling		    - scaling factor for speeding up the
%			          Hough transform
%	sigma		    - amount of Gaussian smoothing to
%			          apply for creating edge map.
%	hithres		    - threshold for creating edge map
%	lowthres	    - threshold for connected edges
%	vert		    - vertical edge contribution (0-1)
%	horz		    - horizontal edge contribution (0-1)
%	
% Output:
%	circleiris	    - centre coordinates and radius
%			          of the detected iris boundary
%	circlepupil	    - centre coordinates and radius
%			          of the detected pupil boundary
%	imagewithnoise	- original eye image, but with
%			          location of noise marked with
%			          NaN values
%
% Author: 
% Libor Masek
% masekl01@csse.uwa.edu.au
% School of Computer Science & Software Engineering
% The University of Western Australia
% November 2003

function [row, col, r] = findcircle(image,lradius,uradius,scaling, sigma, hithres, lowthres, vert, horz)
*/

#include <Masek.h>
//findcircle(eyeimage, lirisradius, uirisradius, 0.4, 2, 0.19, 0.11, 1.00, 0.00, &rowi, &coli,&ri);
//findcircle(imagepupil, lpupilradius, upupilradius ,0.6,2,0.26,0.24,1.00,1.00, &rowp, &colp, &rp);
void Masek::findcircle(Masek::IMAGE *image, int lradius, int uradius, double scaling, double sigma, double hithres, double lowthres, double vert, double horz, int *_row, int *_col, int *_r)
{
	int lradsc, uradsc, rd;
	filter *gradient, *orND;
	int i, j, k, cur;
	filter *I3, *I4, *edgeimage;
	int row, col, tmpi, count;
	double maxtotal;//, tt;
	double *h;
	int r;
//	FILE *fin;
//	char tchar[50];

	lradsc = roundND(lradius*scaling);
	uradsc = roundND(uradius*scaling);
	rd = roundND(uradius*scaling - lradius*scaling);

    //printf("%d %d %d\n", lradsc, uradsc, rd);

//% generate the edge image
	
	gradient = (filter *)malloc(sizeof(filter));
	orND = (filter *)malloc(sizeof(filter));
	
	image = canny(image, sigma, scaling, vert, horz, gradient, orND);

	/*fin = fopen("or.txt", "w");
for (i = 0; i<or->hsize[0]; i++)
for (j = 0; j<or->hsize[1]; j++)
		fprintf(fin, "%d %d %f\n", i+1, j+1, or->data[i*or->hsize[1]+j]);
fclose(fin);

fin = fopen("gradient.txt", "w");
for (i = 0; i<gradient->hsize[0]; i++)
for (j = 0; j<gradient->hsize[1]; j++)
		fprintf(fin, "%d %d %f\n", i+1, j+1, gradient->data[i*gradient->hsize[1]+j]);

fclose(fin);
*/
	I3 = adjgamma(gradient, 1.9);

/*	fin = fopen("I3.txt", "w");
for (i = 0; i<I3->hsize[0]; i++)
for (j = 0; j<I3->hsize[1]; j++)
		fprintf(fin, "%d %d %f\n", i+1, j+1, I3->data[i*I3->hsize[1]+j]);

fclose(fin);
*/	

	I4 = nonmaxsup(I3, orND, 1.5);

/*fin = fopen("I4.txt", "w");
for (i = 0; i<I4->hsize[0]; i++)
for (j = 0; j<I4->hsize[1]; j++)
		fprintf(fin, "%d %d %f\n", i+1, j+1, I4->data[i*I4->hsize[1]+j]);

fclose(fin);	
*/	
	edgeimage = hysthresh(I4, hithres, lowthres);
	count = 0;
/*	fin = fopen("edge.txt", "w");
for (i = 0; i<edgeimage->hsize[0]; i++)
	for (j = 0; j<edgeimage->hsize[1]; j++)
	if (edgeimage->data[i*edgeimage->hsize[1]+j]==1)
	{
		fprintf(fin, "%d %d\n", i+1, j+1);
		count++;
	}
	fclose(fin);
*/

//% perform the circular Hough transform

//%h = houghcircle(edgeimage, lradsc, uradsc);

	
	h = houghcircle(edgeimage, lradsc, uradsc);

	maxtotal = 0;
	count = 0;

	//% find the maximum in the Hough space, and hence
	//% the parameters of the circle
	for (k = 0; k<rd; k++)
	{
		for (i=0; i<edgeimage->hsize[1]; i++)
			for (j=0; j<edgeimage->hsize[0]; j++)
			{
				cur = k*edgeimage->hsize[1]*edgeimage->hsize[0]+j*edgeimage->hsize[1]+i;
    			if (h[cur] > maxtotal)
				{
        			maxtotal = h[cur];
					count = cur;
				}
			}
	}
        
//printf("maxtotal is %f\n", maxtotal);
	tmpi = count/(edgeimage->hsize[0]*edgeimage->hsize[1]);
/*printf("tmpi is %d\n", tmpi);
printf("lradsc is %d\n", lradsc);
printf("scaling is %.20f\n", scaling);
*/
	r = (int)((((lradsc+tmpi+1) / scaling))+AdjPrecision);
	tmpi = count-tmpi*(edgeimage->hsize[0]*edgeimage->hsize[1]);
	
	row = tmpi/edgeimage->hsize[1];
	col = tmpi-row*edgeimage->hsize[1];
	row++;
	col++;

	//printf("row is %d, scaling is %f, newone is %f", row, scaling, row/scaling);
    row = (int)(row / scaling + AdjPrecision);// % returns only first max value
    col = (int)(col / scaling + AdjPrecision);    

	*_row = row;
	*_col = col;
	*_r = r;
        
	free(gradient->data);
	free(gradient);
	free(orND->data);
	free(orND);

	free(h);
}
