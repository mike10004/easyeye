/**************************************************
*  This is a C translation from Masek's matlab code
*  Author:
*  Xiaomei Liu
*  xliu5@cse.nd.edu
*  Computer Vision Research Laboratory
*  Department of Computer Science & Engineering
*  U. of Notre Dame
***************************************************/


/*% findline - returns the coordinates of a line in an image using the
% linear Hough transform and Canny edge detection to create
% the edge map.
%
% Usage: 
% lines = findline(image)
%
% Arguments:
%	image   - the input image
%
% Output:
%	lines   - parameters of the detected line in polar form
%
% Author: 
% Libor Masek
% masekl01@csse.uwa.edu.au
% School of Computer Science & Software Engineering
% The University of Western Australia
% November 2003

function lines = findline(image)*/

#include <math.h>
#include <Masek.h>

int Masek::findline(unsigned char* data, int rows, int cols, double **lines)
{
    int hsize0 = rows, hsize1 = cols;
    IMAGE image_obj;
    image_obj.data = data;
    image_obj.hsize[0] = hsize0;
    image_obj.hsize[1] = hsize1;
    return findline(&image_obj, lines);
}

int Masek::findline(IMAGE* image, double **lines)
{
	filter *I2, *orND, *I3, *I4, *edgeimage;
	double theta[180];
	double *R, *xp;
	double maxval;
	int size, linecount;
	double cx, cy;
	int i, j, index, x, y;
	double r, t;
	int tmpcount;
//	FILE *fid;
//
//	fid = fopen("image.txt", "w");
//	for (i = 0; i<image->hsize[0]; i++)
//	{
//		for (j = 0; j<image->hsize[1]; j++)
//			fprintf(fid, "%d %d %d\n", i+1, j+1, image->data[i*image->hsize[1]+j]);
//		
//	}
//	fclose(fid);
	
	I2 = (filter*)malloc(sizeof(filter));
	orND = (filter*)malloc(sizeof(filter));

	
	image = canny(image, 2, 1, 0.00, 1.00, I2, orND);
	//imwrite("C:\\testImage090716.bmp", image);	

	/*fid = fopen("I2.txt", "w");
	for (i = 0; i<I2->hsize[0]; i++)
	{
		for (j = 0; j<I2->hsize[1]; j++)
			fprintf(fid, "%d %d %f\n", i+1, j+1, I2->data[i*I2->hsize[1]+j]);		
	}
	fclose(fid);

	fid = fopen("or.txt", "w");
	for (i = 0; i<or->hsize[0]; i++)
	{
		for (j = 0; j<or->hsize[1]; j++)
			fprintf(fid, "%d %d %f\n", i+1, j+1, or->data[i*or->hsize[1]+j]);
		
	}
	fclose(fid);*/


	I3 = adjgamma(I2, 1.9);
	/*fid = fopen("I3.txt", "w");
	for (i = 0; i<I3->hsize[0]; i++)
	{
		for (j = 0; j<I3->hsize[1]; j++)
			fprintf(fid, "%d %d %f\n", i+1, j+1, I3->data[i*I3->hsize[1]+j]);
		
	}
	fclose(fid);*/

	I4 = nonmaxsup(I3, orND, 1.5);

	/*fid = fopen("I4.txt", "w");
	for (i = 0; i<I4->hsize[0]; i++)
	{
		for (j = 0; j<I4->hsize[1]; j++)
			fprintf(fid, "%d %d %f\n", i+1, j+1, I4->data[i*I4->hsize[1]+j]);
		
	}
	fclose(fid);*/
	
	edgeimage = hysthresh(I4, 0.20, 0.15);
  //edgeimage = hysthresh(I4, 0.25, 0.10);//LEE:
	

	for (i = 0; i<180; i++)
		theta[i] = i;

	/*fid = fopen("edge.txt", "w");
	for (i = 0; i<edgeimage->hsize[0]; i++)
	{
		for (j = 0; j<edgeimage->hsize[1]; j++)
			fprintf(fid, "%d %d %f\n", i+1, j+1, edgeimage->data[i*edgeimage->hsize[1]+j]);
		
	}
	fclose(fid);*/

	size = radonc(edgeimage->data, theta, edgeimage->hsize[0], edgeimage->hsize[1], 180, &R, &xp);

	
	/*fid = fopen("r.txt", "w");
	for (i = 0; i<size; i++)
	{
		for (j = 0; j<180; j++)
			fprintf(fid, "%d %d %f\n", i+1, j+1, R[i*180+j]);
		
	}
	fclose(fid);
	fid = fopen("xp.txt", "w");
	for (i = 0; i<size; i++)
	{
		fprintf(fid, "%f\n", xp[i]);
		
	}
	fclose(fid);
	*/
	
	maxval = -1;
	index = -1;


	linecount=0;
	for (i = 0; i<size*180; i++)
	{
		if (R[i]>maxval)
		{
			maxval = R[i];
			index = i;
			linecount=1;
		}
		else if (R[i]==maxval)
		{
			linecount++;
		}
	}

	if (maxval<=25)
		return 0;

	
	*lines = (double*) malloc(sizeof(double)*linecount*3);
	

	cx = image->hsize[1]/2.0-1;
	cy = image->hsize[0]/2.0-1;

	tmpcount=0;
	
	for (i = index; i<size*180; i++)
	{
		if (R[i]==maxval)
		{
			y = i/180;
			x = i%180;
			t = -theta[x]*PI/180;
			r = xp[y];
			
			(*lines)[tmpcount*3] = cos(t);
			(*lines)[tmpcount*3+1] = sin(t);
			(*lines)[tmpcount*3+2] = -r;
			(*lines)[tmpcount*3+2] = (*lines)[tmpcount*3+2] - (*lines)[tmpcount*3]*cx - (*lines)[tmpcount*3+1]*cy;
			tmpcount++;
		}
	}

	free(R);
	free(xp);
	free(I2->data);
	free(I2);
	free(orND->data);
	free(orND);
	return linecount;
}
