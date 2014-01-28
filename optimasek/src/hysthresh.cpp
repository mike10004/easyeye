/**************************************************
*  This is a C translation of the matlab code
*  Author:
*  Xiaomei Liu
*  xliu5@cse.nd.edu
*  Computer Vision Research Laboratory
*  Department of Computer Science & Engineering
*  U. of Notre Dame
***************************************************/

/*% HYSTHRESH - Hysteresis thresholding
%
% Usage: bw = hysthresh(im, T1, T2)
%
% Arguments:
%             im  - image to be thresholded (assumed to be non-negative)
%             T1  - upper threshold value
%             T2  - lower threshold value
%
% Returns:
%             bw  - the thresholded image (containing values 0 or 1)
%
% Function performs hysteresis thresholding of an image.
% All pixels with values above threshold T1 are marked as edges
% All pixels that are adjacent to points that have been marked as edges
% and with values above threshold T2 are also marked as edges. Eight
% connectivity is used.
%
% It is assumed that the input image is non-negative
%
% Peter Kovesi          December 1996  - Original version
%                       March    2001  - Speed improvements made (~4x)

%
% A stack (implemented as an array) is used to keep track of all the
% indices of pixels that need to be checked.
% Note: For speed the number of conditional tests have been minimised
% This results in the top and bottom edges of the image being considered to
% be connected.  This may cause some stray edges to be propagated further than 
% they should be from the top or bottom.
%


function bw = hysthresh(im, T1, T2)*/

#include <string.h>
#include <Masek.h>

Masek::filter* Masek::hysthresh(Masek::filter *im, double T1, double T2)
{

	int rows, cols, rc, rcmr, rp1;
	double *bw;
	int *pix, *stack;
	int i, k, l;
	int npix;
	int stp, v, ind;
	int tmp[8];
	int index[8];

	if (T2 > T1 || T2 < 0 || T1 < 0)  //% Check thesholds are sensible
		fprintf(stderr, "optimasek: hysthresh: T1 must be >= T2 and both must be >= 0\n");
	
	rows = im->hsize[0];
	cols = im->hsize[1]; // % Precompute some values for speed and convenience.
	
	rc = rows*cols;
	rcmr = rc-cols-1;//rcmr = rc - rows;
	rp1 = cols;//rp1 = rows+1;

	bw = im->data; 
		
	pix =  (int*)malloc(sizeof(int)*rows*cols);

	npix = 0;
	for (i = 0; i<rows*cols; i++)
	{
		if (bw[i]>T1)
			pix[npix++] = i;	// % Find indices of all pixels with value > T1
						        // % Find the number of pixels with value > T1
	}
	
	stack = (int*)malloc(sizeof(int)*rows*cols);
	memset(stack, 0, sizeof(int)*rows*cols); // % Create a stack array (that should never
                            // overflow!)

	for (i = 0; i<npix; i++)
		stack[i] = pix[i];      //  % Put all the edge points on the stack
	
	stp = npix-1;				// set stack pointer
	
	for (k = 0; k<npix; k++)
		bw[pix[k]] = -1;       // % mark points as edges
	


	/*% Precompute an array, O, of index offset values that correspond to the eight 
	% surrounding pixels of any point. Note that the image was transformed into
	% a column vector, so if we reshape the image back to a square the indices 
	% surrounding a pixel with index, n, will be:
	%              n-cols-1   n-1   n+cols-1
	%
	%               n-cols     n     n+cols
	%                     
	%              n-cols+1   n+1   n+cols+1*/

	tmp[0] = -1;
	tmp[1] = 1;
	tmp[2] = -cols-1;//tmp[2] = -rows-1;
	tmp[3] = -cols;//tmp[3] = -rows;
	tmp[4] = -cols+1;//tmp[4] = -rows+1;
	tmp[5] = cols-1;//tmp[5] = rows-1;
	tmp[6] = cols;//tmp[6] = rows;
	tmp[7] = cols+1;//tmp[7] = rows+1;

	while (stp >=0 )//% While the stack is not empty
	{
		v = stack[stp--];         //% Pop next index off the stack
		
    
		if (v > rp1 && v < rcmr)  // % Prevent us from generating illegal indices
							//% Now look at surrounding pixels to see if they
                            //% should be pushed onto the stack to be
                            //% processed as well.
		{
			for (i = 0; i<8; i++)
				index[i] = tmp[i]+v;	    //% Calculate indices of points around this pixel.	    
			
			for (l = 0; l<8; l++)
			{
				ind = index[l];
				if (bw[ind] > T2)  // % if value > T2,
				{
					stp = stp+1; // % push index onto the stack.
					stack[stp] = ind;
					bw[ind] = -1;// % mark this as an edge point
				}
			}
		}
	}




	for (i = 0; i<rows*cols; i++)
	{
		if (bw[i] == -1)
			bw[i] = 1;
		else
			bw[i] = 0;	// % Finally zero out anything that was not an edge 
	}			          

    //bw = reshape(bw,rows,cols); % and reshape the image
	free(pix);
	free(stack);
	return im;
}