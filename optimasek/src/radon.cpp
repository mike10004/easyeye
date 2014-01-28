/**************************************************
*  This is a C translation from the matlab code
*  Author:
*  Xiaomei Liu
*  xliu5@cse.nd.edu
*  Computer Vision Research Laboratory
*  Department of Computer Science & Engineering
*  U. of Notre Dame
***************************************************/

/* Copyright 1993-2000 The MathWorks, Inc. */

/* $Revision: 1.10 $  $Date: 2000/01/21 20:19:26 $ */

/*

	RADONC.C .MEX file
		Implements Radon transform.

        Syntax  [P, r] = radon(I, theta)

                evaluates the Radon transform P, of I along the angles
                specified by the vector THETA.  THETA values measure 
                angle counter-clockwise from the horizontal axis.  
		THETA defaults to [0:179] if not specified.

                The output argument r is a vector giving the
		values of r corresponding to the rows of P.

                The origin of I is computed in the same way as origins
                are computed in filter2:  the origin is the image center
                rounded to the upper left.

        Algorithm
                The Radon transform of a point-mass is known, and the Radon
                transform is linear (although not shift-invariant).  So
                we use superposition of the Radon transforms of each
                image pixel.  The dispersion of each pixel (point mass)
                along the r-axis is a nonlinear function of theta and the
                pixel location, so to improve the accuracy, we divide
		each pixel into 4 submasses located to the NE, NW, SE, 
                and SW of the original location.  Spacing of the submasses
                is 0.5 in the x- and y-directions.  We also smooth 
		the result in the r-direction by a triangular smoothing 
		window of length 2.

        Reference
                Ronald N. Bracewell, Two-Dimensional Imaging, Prentice-Hall,
                1995, pp. 518-525.

        S. Eddins 1-95

        Revised 7-95 to improve accuracy of algorithm and optimize
	implementation. -S. Eddins, D. Orofino
 
*/

#include <math.h>
#include <string.h>
#include <Masek.h>

static void radon(double *pPtr, double *iPtr, double *thetaPtr, int M, int N, 
		  int xOrigin, int yOrigin, int numAngles, int rFirst, 
		  int rSize);

static char rcs_id[] = "$Revision: 1.10 $";

#define MAXX(x,y) ((x) > (y) ? (x) : (y))

/* Input Arguments */
//#define I      (prhs[0])
//#define THETA  (prhs[1])

/* Output Arguments */
//#define	P      (plhs[0])
//#define R      (plhs[1])

int 
Masek::radonc(double *I, double *THETA, int imgrow, int imgcol, int numangles, double **P, double **R)
{
  int numAngles;          /* number of theta values */
  double *thetaPtr;       /* pointer to theta values in radians */
  double *pr1, *pr2;      /* double pointers used in loop */
  double deg2rad;         /* conversion factor */
  int k;                  /* loop counter */
  int M, N;               /* input image size */
  int xOrigin, yOrigin;   /* center of image */
  int temp1, temp2;       /* used in output size computation */
  int rFirst, rLast;      /* r-values for first and last row of output */
  int rSize;              /* number of rows in output */
  
    
  /* Get THETA values */
  deg2rad = 3.14159265358979 / 180.0;
  numAngles = numangles;//mxGetM(THETA) * mxGetN(THETA);
  thetaPtr = (double *) malloc(numAngles* sizeof(double));
  pr1 = THETA;//mxGetPr(THETA);
  pr2 = thetaPtr;
  for (k = 0; k < numAngles; k++)
      *(pr2++) = *(pr1++) * deg2rad;
  
  M = imgrow;
  N = imgcol;

  /* Where is the coordinate system's origin? */
  xOrigin = MAXX(0, (N-1)/2);
  yOrigin = MAXX(0, (M-1)/2);

  /* How big will the output be? */
  temp1 = M - 1 - yOrigin;
  temp2 = N - 1 - xOrigin;
  rLast = (int) ceil(sqrt((double) (temp1*temp1+temp2*temp2))) + 1;
  rFirst = -rLast;
  rSize = rLast - rFirst + 1;

  //if (nlhs == 2) {
    *R = (double*)malloc(rSize*sizeof(double)); //mxCreateDoubleMatrix(rSize, 1, mxREAL);
    pr1 = *R;//mxGetPr(R);
    for (k = rFirst; k <= rLast; k++)
      *(pr1++) = (double) k;
  //}

  /* Invoke main computation routines */
  *P = (double*)malloc(rSize*numAngles*sizeof(double));//mxCreateDoubleMatrix(rSize, numAngles, mxREAL);
  memset(*P, 0, rSize*numAngles*sizeof(double));
  radon(*P, I, thetaPtr, M, N, xOrigin, yOrigin, 
	  numAngles, rFirst, rSize);

  free(thetaPtr);
  return rSize;


}

void 
radon(double *pPtr, double *iPtr, double *thetaPtr, int M, int N, 
      int xOrigin, int yOrigin, int numAngles, int rFirst, int rSize)
{
  int k, m, n, p;           /* loop counters */
  double angle;             /* radian angle value */
  double cosine, sine;      /* cosine and sine of current angle */
  double *pr;               /* points inside output array */
  double *pixelPtr;         /* points inside input array */
  double pixel;             /* current pixel value */
  double rIdx;              /* r value offset from initial array element */
  int rLow;                 /* (int) rIdx */
  double pixelLow;          /* amount of pixel's mass to be assigned to */
                            /* the bin below Idx */
  double *yTable, *xTable;  /* x- and y-coordinate tables */
  double *ySinTable, *xCosTable;
                            /* tables for x*cos(angle) and y*sin(angle) */


  /* Allocate space for the lookup tables */
  yTable = (double *) malloc(2*M* sizeof(double));
  xTable = (double *) malloc(2*N* sizeof(double));
  xCosTable = (double *) malloc(2*N* sizeof(double));
  ySinTable = (double *) malloc(2*M* sizeof(double));

  /* x- and y-coordinates are offset from pixel locations by 0.25 */
  /* spaced by intervals of 0.5. */

  /* We want bottom-to-top to be the positive y direction */
  yTable[2*M-1] = -yOrigin - 0.25;  
  for (k = 2*M-2; k >=0; k--)       
    yTable[k] = yTable[k+1] + 0.5;  

  xTable[0] = -xOrigin - 0.25;      
  for (k = 1; k < 2*N; k++)         
    xTable[k] = xTable[k-1] + 0.5;  

  for (k = 0; k < numAngles; k++) {
    angle = thetaPtr[k];
    //pr = pPtr + k*rSize;  /* pointer to the top of the output column */
	pr = pPtr+k;
    cosine = cos(angle); 
    sine = sin(angle);   

    /* Radon impulse response locus:  R = X*cos(angle) + Y*sin(angle) */
    /* Fill the X*cos table and the Y*sin table.  Incorporate the */
    /* origin offset into the X*cos table to save some adds later. */
    for (p = 0; p < 2*N; p++)
      xCosTable[p] = xTable[p] * cosine - rFirst;  
    for (p = 0; p < 2*M; p++)
      ySinTable[p] = yTable[p] * sine;             

    /* Remember that n and m will each change twice as fast as the */
    /* pixel pointer should change. * /
    for (n = 0; n < 2*N; n++) {
      pixelPtr = iPtr + (n/2)*M;
      for (m = 0; m < 2*M; m++) {
	pixel = *pixelPtr;
	if (pixel) {
	  pixel *= 0.25;                         /* 1 flop/pixel * /
	  rIdx = (xCosTable[n] + ySinTable[m]);  /* 1 flop/pixel * /
	  rLow = (int) rIdx;                     /* 1 flop/pixel * /
	  pixelLow = pixel*(1 - rIdx + rLow);    /* 3 flops/pixel * /
	  pr[rLow++] += pixelLow;                /* 1 flop/pixel * /
	  pr[rLow] += pixel - pixelLow;          /* 2 flops/pixel * /
	}
	if (m%2)
	  pixelPtr++;
      }
    }*/

    for (m = 0; m < 2*M; m++) {
      pixelPtr = iPtr + (m/2)*N;
      for (n = 0; n < 2*N; n++) {
	pixel = *pixelPtr;
	if (pixel) {
	  pixel *= 0.25;                         /* 1 flop/pixel */
	  rIdx = (xCosTable[n] + ySinTable[m]);  /* 1 flop/pixel */
	  rLow = (int) rIdx;                     /* 1 flop/pixel */
	  pixelLow = pixel*(1 - rIdx + rLow);    /* 3 flops/pixel */
	  pr[rLow*numAngles] += pixelLow;                /* 1 flop/pixel */
	  rLow++;
	  pr[rLow*numAngles] += pixel - pixelLow;          /* 2 flops/pixel */
	}
	if (n%2)
	  pixelPtr++;
      }
    }

  }

  free( yTable);
  free(xTable);
  free(xCosTable);
  free(ySinTable);
  
}
