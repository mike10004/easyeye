/*% NONMAXSUP
%
% Usage:
%          im = nonmaxsup(inimage, orient, radius);
%
% Function for performing non-maxima suppression on an image using an
% orientation image.  It is assumed that the orientation image gives 
% feature normal orientation angles in degrees (0-180).
%
% input:
%   inimage - image to be non-maxima suppressed.
% 
%   orient  - image containing feature normal orientation angles in degrees
%             (0-180), angles positive anti-clockwise.
% 
%   radius  - distance in pixel units to be looked at on each side of each
%             pixel when determining whether it is a local maxima or not.
%             (Suggested value about 1.2 - 1.5)
%
% Note: This function is slow (1 - 2 mins to process a 256x256 image).  It uses
% bilinear interpolation to estimate intensity values at ideal, real-valued pixel 
% locations on each side of pixels to determine if they are local maxima.
%
% Peter Kovesi     pk@cs.uwa.edu.au
% Department of Computer Science
% The University of Western Australia
%
% December 1996

function im = nonmaxsup(inimage, orient, radius)*/

#include <string.h>
#include <math.h>
#include <Masek.h>

Masek::filter * Masek::nonmaxsup(Masek::filter* inimage, Masek::filter* orient, double radius)
{
	int rows, cols, row, col, cx, cy, fx, fy, i, ori;
	int iradius;
	double hfrac[181], vfrac[181], xoff[181], yoff[181];
	double *im;
	double angle, bl, br, tl, tr, upperavg, loweravg, v1, v2, x, y;
	
	if (inimage->hsize[0] != orient->hsize[0] || inimage->hsize[1] != orient->hsize[1])
		fprintf(stderr, "optimasek: nonmaxsup: image and orientation image are of different sizes\n");

	if (radius < 1)
		fprintf(stderr, "optimasek: nonmaxsup: radius must be >= 1\n");


	rows = inimage->hsize[0];
	cols = inimage->hsize[1];
	
	im = (double *)malloc(sizeof(double)*rows*cols);
	memset(im, 0, sizeof(double)*rows*cols); //% Preallocate memory for output image for speed
	iradius = (int) ceil(radius);

	//% Precalculate x and y offsets relative to centre pixel for each orientation angle 
	for (i = 0; i<=180; i++)
	{
		angle = i*PI/180;			//% Array of angles in 1 degree increments (but in radians).
		xoff[i] = radius*cos(angle);	//x and y offset of points at specified radius and angle
		yoff[i] = radius*sin(angle);	//from each reference position.
		hfrac[i] = xoff[i] - floor(xoff[i]); //% Fractional offset of xoff relative to integer location
		vfrac[i] = yoff[i] - floor(yoff[i]); //% Fractional offset of yoff relative to integer location

	}

	yoff[180] = 0;
	xoff[90] = 0;
		
	/*for (i = 0; i<rows*cols; i++)
		orient->data[i] = fix(orient->data[i]);	// % Orientations start at 0 degrees but arrays start														//% with index 1.*/

	/*% Now run through the image interpolating grey values on each side
	% of the centre pixel to be used for the non-maximal suppression.*/

	for (row = iradius; row<(rows - iradius); row++)
		for (col = iradius; col<(cols - iradius); col++) 
		{
			//ori = roundND(orient->data[row*cols+col]);   //% Index into precomputed arrays
			ori = fix(orient->data[row*cols+col]+AdjPrecision);   //% Index into precomputed arrays
			
			x = col + xoff[ori];     //% x, y location on one side of the point in question
			y = row - yoff[ori];

			fx = (int)floor(x);          //% Get integer pixel locations that surround location x,y
			cx = (int)ceil(x);
			fy = (int)floor(y);
			cy = (int)ceil(y);
			tl = inimage->data[fy*cols+fx];   // % Value at top left integer pixel location.
			tr = inimage->data[fy*cols+cx];  // % top right
			bl = inimage->data[cy*cols+fx];   // % bottom left
			br = inimage->data[cy*cols+cx];   // % bottom right

			upperavg = tl + hfrac[ori] * (tr - tl);  //% Now use bilinear interpolation to
			loweravg = bl + hfrac[ori] * (br - bl);  //% estimate value at x,y
			v1 = upperavg + vfrac[ori] * (loweravg - upperavg);

			if (inimage->data[row*cols+col] > v1) //% We need to check the value on the other side...
			{

				x = col - xoff[ori];    // % x, y location on the `other side' of the point in question
				y = row + yoff[ori];
	
				fx = (int)floor(x);
				cx = (int)ceil(x);
				fy = (int)floor(y);
				cy = (int)ceil(y);
				tl = inimage->data[fy*cols+fx];    //% Value at top left integer pixel location.
				tr = inimage->data[fy*cols+cx];    //% top right
				bl = inimage->data[cy*cols+fx];    //% bottom left
				br = inimage->data[cy*cols+cx];    //% bottom right
				upperavg = tl + hfrac[ori] * (tr - tl);
				loweravg = bl + hfrac[ori] * (br - bl);
				v2 = upperavg + vfrac[ori] * (loweravg - upperavg);

				if (inimage->data[row*cols+col] > v2 )          // % This is a local maximum.
					im[row*cols+col] = inimage->data[row*cols+col]; //% Record value in the output image.
    

			}
		}


		for (i = 0; i<rows*cols; i++)
			inimage->data[i] = im[i];

		free(im);

		return inimage;

}
