/**************************************************
*  This is a C translation from Masek's matlab code
*  Author:
*  Xiaomei Liu
*  xliu5@cse.nd.edu
*  Computer Vision Research Laboratory
*  Department of Computer Science & Engineering
*  U. of Notre Dame
***************************************************/


/*% normaliseiris - performs normalisation of the iris region by
% unwraping the circular region into a rectangular block of
% constant dimensions.
%
% Usage: 
% [polar_array, polar_noise] = normaliseiris(image, x_iris, y_iris, r_iris,...
% x_pupil, y_pupil, r_pupil,eyeimage_filename, radpixels, angulardiv)
%
% Arguments:
% image                 - the input eye image to extract iris data from
% x_iris                - the x coordinate of the circle defining the iris
%                         boundary
% y_iris                - the y coordinate of the circle defining the iris
%                         boundary
% r_iris                - the radius of the circle defining the iris
%                         boundary
% x_pupil               - the x coordinate of the circle defining the pupil
%                         boundary
% y_pupil               - the y coordinate of the circle defining the pupil
%                         boundary
% r_pupil               - the radius of the circle defining the pupil
%                         boundary
% eyeimage_filename     - original filename of the input eye image
% radpixels             - radial resolution, defines vertical dimension of
%                         normalised representation
% angulardiv            - angular resolution, defines horizontal dimension
%                         of normalised representation
%
% Output:
% polar_array
% polar_noise
%
% Author: 
% Libor Masek
% masekl01@csse.uwa.edu.au
% School of Computer Science & Software Engineering
% The University of Western Australia
% November 2003

function [polar_array, polar_noise] = normaliseiris(image, x_iris, y_iris, r_iris,...
x_pupil, y_pupil, r_pupil,eyeimage_filename, radpixels, angulardiv)*/

#include <string.h>
#include <math.h>
#include <float.h> // Lee: added
#include <Masek.h>
#include <portability.h>
#include <assert.h>

void Masek::normaliseiris(Masek::filter *image, int xiris, int yiris, int riris, 
						  int xpupil, int ypupil, int rpupil,
						  int radpixels, int angulardiv, 
						  const Masek::filter *polar_array, const Masek::IMAGE *polar_noise)
{
	//global DIAGPATH
	int radiuspixels;
	int angledivisions;
	double r;
	double *theta, *b, xcosmat, xsinmat, rmat;
	filter xo, yo;
	int i, j;
	double x_iris, y_iris, r_iris, x_pupil, y_pupil, r_pupil, ox, oy;
	int sgn;
	double phi;
	double a;
	//FILE *fid;
	//IMAGE tmpimage;
	int *x, *y, *xp, *yp;
	int len;
	double sum, avg;
	int count;

    assert(polar_array->hsize[0] == radpixels);
    assert(polar_array->hsize[1] == angulardiv);
    assert(polar_noise->hsize[0] == radpixels);
    assert(polar_noise->hsize[1] == angulardiv);
    
	//printfilter(image, "image.txt");
	radiuspixels = radpixels + 2;
	angledivisions = angulardiv-1;


	theta = (double*)malloc(sizeof(double)*(angledivisions+1));
	for (i = 0; i<angledivisions+1; i++)
		theta[i] = 2*i*PI/angledivisions;

	x_iris = (double)xiris;
	y_iris = (double)yiris;
	r_iris = (double)riris;

	x_pupil = (double)xpupil;
	y_pupil = (double)ypupil;
	r_pupil = (double)rpupil;

	//% calculate displacement of pupil center from the iris center
	ox = x_pupil - x_iris;
	oy = y_pupil - y_iris;

	if (ox <= 0)
		sgn = -1;
	else if (ox > 0)
		sgn = 1;


	if (ox==0 && oy > 0)
		sgn = 1;


	//a = ones(1,angledivisions+1)* (ox^2 + oy^2);
	a = ox*ox+oy*oy;

	//% need to do something for ox = 0
	if (ox == 0)
		phi = PI/2;
	else
		phi = atan(oy/ox);

	b = (double*)malloc(sizeof(double)*(angledivisions+1));
	xo.hsize[0] = (radiuspixels-2);
	xo.hsize[1] = angledivisions+1;
	xo.data = (double*)malloc(sizeof(double)*(angledivisions+1)*(radiuspixels-2));

	yo.hsize[0] = (radiuspixels-2);
	yo.hsize[1] = angledivisions+1;
	yo.data = (double*)malloc(sizeof(double)*(angledivisions+1)*(radiuspixels-2));

	//% calculate radius around the iris as a function of the angle
	for (i = 0; i<angledivisions+1; i++)
	{
		b[i] = sgn*cos(PI - phi - theta[i]);
		r = sqrt(a)*b[i]+sqrt(a*b[i]*b[i]-(a-r_iris*r_iris));
		r -= r_pupil;

		//% calculate cartesian location of each data point around the circular iris
		//% region
		xcosmat = cos(theta[i]);
		xsinmat = sin(theta[i]);
	/*% exclude values at the boundary of the pupil iris border, and the iris scelra border
	% as these may not correspond to areas in the iris region and will introduce noise.
	%
	% ie don't take the outside rings as iris data.*/

		for (j = 0; j<radiuspixels; j++)
		{
			rmat = r*j/(radiuspixels-1);
			rmat += r_pupil;
			if (j>0 && j<radiuspixels-1)
			{
				xo.data[(j-1)*(angledivisions+1)+i] = rmat*xcosmat+x_pupil;
				yo.data[(j-1)*(angledivisions+1)+i] = -rmat*xsinmat+y_pupil;
			}
		}
	}

	/*

	% extract intensity values into the normalised polar representation through
	% interpolation
	[x,y] = meshgrid(1:size(image,2),1:size(image,1)); */
	interp2(image,&xo,&yo, polar_array);



	//% create noise array with location of NaNs in polar_array
//	polar_noise->hsize[0] = polar_array->hsize[0];
//	polar_noise->hsize[1] = polar_array->hsize[1];
//	polar_noise->data = (unsigned char*)malloc(sizeof(unsigned char)*polar_noise->hsize[0]*polar_noise->hsize[1]);
	memset(polar_noise->data, 0, polar_noise->hsize[0]*polar_noise->hsize[1]);


	count=0;
	
	for (i = 0; i<polar_noise->hsize[0]*polar_noise->hsize[1]; i++)
	{
		if (portability::Math::IsNaN(polar_array->data[i])) // Lee: renamed from "isnan"
		{
			polar_noise->data[i] = 1;
			count++;
		}
		else
		{
			polar_noise->data[i] = 0;
			polar_array->data[i] = polar_array->data[i]/255;
		}
	}

	//printfilter(polar_array, "polar.txt");
	//printimage(polar_noise, "polar_noise.txt");

	//% start diagnostics, writing out eye image with rings overlayed

	//% get rid of outling points in order to write out the circular pattern
	for (i = 0; i<xo.hsize[0]*xo.hsize[1]; i++)
	{
		if (xo.data[i]>image->hsize[1])
			xo.data[i] = image->hsize[1];
		else if (xo.data[i]<1)
			xo.data[i] = 1;
		xo.data[i] = roundND(xo.data[i]);
	}

	for (i = 0; i<yo.hsize[0]*yo.hsize[1]; i++)
	{
		if (yo.data[i]>image->hsize[0])
			yo.data[i] = image->hsize[0];
		else if (yo.data[i]<1)
			yo.data[i] = 1;
		yo.data[i] = roundND(yo.data[i]);
	}


	/*tmpimage.hsize[0] = image->hsize[0];
	tmpimage.hsize[1] = image->hsize[1];
	tmpimage.data = (unsigned char*)malloc(sizeof(unsigned char)*tmpimage.hsize[0]*tmpimage.hsize[1]);
	for (i = 0; i<tmpimage.hsize[0]*tmpimage.hsize[1]; i++)
	{
		if (gccport::_isnan(image->data[i]))  // Lee: renamed from "isnan"
			tmpimage.data[i] = 0;
		else
			tmpimage.data[i] = (int)image->data[i];
	}*/



	/*for (i = 0; i<yo.hsize[0]*yo.hsize[1]; i++)
		tmpimage.data[(int)((yo.data[i]-1)*tmpimage.hsize[1]+(xo.data[i])-1)] = 255;
	*/

	//%get pixel coords for circle around iris
	len = circlecoords(x_iris,y_iris,r_iris,image->hsize, -1, &x, &y);


	//ind2 = sub2ind(size(image),double(y),double(x));
	/*fid = fopen("xy.txt", "w");
	for (i = 0; i<len; i++)
	{
		fprintf(fid, "%d %d %d\n", i+1, x[i], y[i]);
		tmpimage.data[(y[i]-1)*tmpimage.hsize[1]+(x[i]-1)] = 255;
	}
	fclose(fid);*/
	//%get pixel coords for circle around pupil

	len = circlecoords(x_pupil,y_pupil,r_pupil,image->hsize, -1, &xp, &yp);
	/*for (i = 0; i<len; i++)
		tmpimage.data[(yp[i]-1)*tmpimage.hsize[1]+(xp[i]-1)] = 255;*/
	//printimage(&tmpimage, "tmpimage.txt");


	/*% write out rings overlaying original iris image
	%w = cd;
	%cd(DIAGPATH);
	imwrite(image,[eyeimage_filename,'-normal.jpg'],'jpg');

	%cd(w);

	% end diagnostics
	*/
	//%replace NaNs before performing feature encoding
	sum = 0;
	for (i = 0; i<polar_array->hsize[0]*polar_array->hsize[1]; i++)
	{
		if (portability::Math::IsNaN(polar_array->data[i])) // Lee: renamed from "isnan"
			sum+=0.5;
		else
			sum+=polar_array->data[i];
	}
	avg = sum/(polar_array->hsize[0]*polar_array->hsize[1]);

	for (i = 0; i<polar_array->hsize[0]*polar_array->hsize[1]; i++) {
		if (portability::Math::IsNaN(polar_array->data[i]))
			//polar_array->data[i] = sqrt((double)-1);//LEE:added
			polar_array->data[i] = avg;
	}

	free(xp);
	free(yp);
	free(x);
	free(y);

	free(theta);
	free(b);
	free(xo.data);
	free(yo.data);

	//printfilter(polar_array, "polar_array.txt");
}
