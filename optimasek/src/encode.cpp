/**************************************************
*  This is a C translation from Masek's matlab code
*  Author:
*  Xiaomei Liu
*  xliu5@cse.nd.edu
*  Computer Vision Research Laboratory
*  Department of Computer Science & Engineering
*  U. of Notre Dame
***************************************************/


/*% encode - generates a biometric template from the normalised iris region,
% also generates corresponding noise mask
%
% Usage: 
% [template, mask] = encode(polar_array,noise_array, nscales,...
% minWaveLength, mult, sigmaOnf)
%
% Arguments:
% polar_array       - normalised iris region
% noise_array       - corresponding normalised noise region map
% nscales           - number of filters to use in encoding
% minWaveLength     - base wavelength
% mult              - multicative factor between each filter
% sigmaOnf          - bandwidth parameter
%
% Output:
% template          - the binary iris biometric template
% mask              - the binary iris noise mask
%
% Author: 
% Libor Masek
% masekl01@csse.uwa.edu.au
% School of Computer Science & Software Engineering
% The University of Western Australia
% November 2003

function [template, mask] = encode(polar_array,noise_array, nscales, minWaveLength, mult, sigmaOnf)*/

#include <string.h>
#include <math.h>
#include <Masek.h>
#include <assert.h>

//FILE *fid;//LEE


/**
 * 
 * @param polar_array
 * @param noise_array
 * @param nscales
 * @param minWaveLength
 * @param mult
 * @param sigmaOnf
 * @param irisTemplate an array of length (nscales * 2 * angularResolution * radialResolution)
 * @param irisMask an array of length (nscales * 2 * angularResolution * radialResolution)
 */
void Masek::encode(Masek::filter *polar_array, Masek::IMAGE* noise_array,
				   int nscales, int minWaveLength, int mult, double sigmaOnf, 
         int* irisTemplate,  int* irisMask)
//,
//				   int **_template, int **mask, 
//                    int *width, int *height)
{
	Complex *** EO, **E1;
	double *filtersum;
	int i, j;
	int length, length2;
//	int *h;
	int k;
	int *H1, *H2, *H3;
	int lenw, lenh;
	double ja;
		

	//% convolve normalised region with Gabor filters
	EO = (Complex***) malloc(sizeof(Complex**)*nscales);

	//printfilter(polar_array, "polar_array");

	gaborconvolve(polar_array, nscales, minWaveLength, mult, sigmaOnf, EO, &filtersum, &lenh, &lenw);

	length = polar_array->hsize[1]*2*nscales;

//	*_template = (int*)malloc(sizeof(int)*length*polar_array->hsize[0]);
	memset(irisTemplate, 0, sizeof(int)*length*polar_array->hsize[0]);

//	*mask = (int*)malloc(sizeof(int)*length*polar_array->hsize[0]);
	memset(irisMask, 0, sizeof(int)*length*polar_array->hsize[0]);


	length2 = polar_array->hsize[1];

	/*h  = (int*)malloc(sizeof(int)*polar_array->hsize[0]);
	for (i = 0; i<polar_array->hsize[0]; i++)
		h[i] = i+1;
	*/

	//%create the iris template

	//mask = zeros(size(_template));

	H1 = (int*)malloc(sizeof(int)*lenw*lenh);
	H2 = (int*)malloc(sizeof(int)*lenw*lenh);
	H3 = (int*)malloc(sizeof(int)*lenw*lenh);

	for (k=0; k<nscales; k++)
	{
	    
		E1 = EO[k];
	    
		//%Phase quantisation
		for (i = 0; i<lenh; i++)
		{
			for (j = 0; j<lenw; j++)
			{

				if (E1[i][j].real>0)
					H1[i*lenw+j] = 1;
				else
					H1[i*lenw+j] = 0;

				if (E1[i][j].img>0)
					H2[i*lenw+j] = 1;
				else
					H2[i*lenw+j] = 0;   
	    
		/*% if amplitude is close to zero then
		% phase data is not useful, so mark off
		% in the noise mask*/
			
				if (sqrt(E1[i][j].img*E1[i][j].img+E1[i][j].real*E1[i][j].real)<0.0001)
					H3[i*lenw+j] = 1;
				else
					H3[i*lenw+j] = 0;
			}

			free(E1[i]);
			
		}

	/*fid = fopen("E1.txt", "w");
	for (i = 0; i<20; i++)
		for (j = 0; j<240; j++)
	{
		fprintf(fid, "%d %d %f %f\n", i+1, j+1, E1[i][j].real,E1[i][j].img );
	
	}
	fclose(fid);
*/
    free(E1);
    
    for (i=0; i<length2;i++)
	{
                
        ja = (double)(2*nscales*(i));
		for (j = 0; j<polar_array->hsize[0]; j++)
		{
        //%construct the biometric template
			(irisTemplate)[j*length+(int)ja+(2*k)] = H1[j*polar_array->hsize[1]+i];
			(irisTemplate)[j*length+(int)ja+(2*k)+1] = H2[j*polar_array->hsize[1]+i];

			//%create noise mask
		
			(irisMask)[j*length+(int)ja+(2*k)] = noise_array->data[j*noise_array->hsize[1]+ i] || H3[j*polar_array->hsize[1]+i];
			(irisMask)[j*length+(int)ja+(2*k)+1] =   noise_array->data[j*noise_array->hsize[1]+ i] || H3[j*polar_array->hsize[1]+i];
		}
        
	}
	
    
}
free(EO);
//*height = polar_array->hsize[0];
//*width = length;
//=======================================
/*fid = fopen("template.txt", "w");
for (i = 0; i<polar_array->hsize[0]; i++)
{
	for (j = 0; j<length; j++)
//		if ((*_template)[i*length+j] == 1)
			fprintf(fid, "%d %d %d\n", i+1, j+1, (*_template)[i*length+j]);
	
}
fclose(fid);

fid = fopen("mask.txt", "w");
for (i = 0; i<polar_array->hsize[0]; i++)
{
	for (j = 0; j<length; j++)
		if ((*mask)[i*length+j] == 1)
			fprintf(fid, "%d %d %d\n", i+1, j+1, (*mask)[i*length+j]);
	
}
fclose(fid);

fid = fopen("H2.txt", "w");
for (i = 0; i<20; i++)
	for (j = 0; j<240; j++)
{
	fprintf(fid, "%d %d %d\n", i+1, j+1, H2[i*240+j]);
	
}
fclose(fid);*/
//===========================================================

free(filtersum);
free(H1);
free(H2);
free(H3);

}
