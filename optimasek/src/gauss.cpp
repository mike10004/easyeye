/*from http://www.ltrr.arizona.edu/pub/trees/doc/old_html/gauss_8c-source.html
Main Page   Data Structures   File List   Data Fields   Globals   

--------------------------------------------------------------------------------

gauss.c
Go to the documentation of this file.*/
/*#include "sadie.h"
#include "proto.h"*/

#include <math.h>
#include <Masek.h>

/*----------------------------------------------------------------------------*/
/*-General Information--------------------------------------------------------*/
/*                                                                            */
/* This function computes a (size x size) Gaussian filter with standard   */
/* deviation sigma.                                                           */
/*                                                                            */
/*----------------------------------------------------------------------------*/
/*-Interface Information------------------------------------------------------*/
void reverse(int size, int *index);
void m_select(int oldsize, int newsize, int *index);

//FILE *fid;

int Masek::CREATEGAUSS (
	int size[2],       /*  I   The size of the image.                                */
	double sigma,   /*  I   The standard deviation of the gaussian.               */
    filter *out     /*  O   Address of a pointer to the output image              */)
{ 
	register short i, j, k;
    
    double x, y, *elin, *epix;
	double sum;

    
    /* allocate memory for temporary storage */
    if (!(elin = (double *)malloc(((size[0]/2)+1)*sizeof(double)))) 
	{
         fprintf(stderr, "optimasek: CREATEGAUSS: Memory request failed.\n");
         return -1;
     }
     if (!(epix = (double *)malloc(((size[1]/2)+1)*sizeof(double)))) 
	 {
         fprintf(stderr, "optimasek: CREATEGAUSS: Memory request failed.\n");
		 if (elin)  free(elin);
		return -1;
     }

	 /* store one dimensional components */
     for (j=0; j<size[0]/2+1; j++) 
	 {
		y = ((double)((size[0]/2-j)*(size[0]/2-j)))/(2*sigma*sigma);
        elin[j] = exp(-y);
     }
     for (k=0; k<size[1]/2+1; k++) 
	 {
		x = ((double)((size[1]/2-k)*(size[1]/2-k)))/(2*sigma*sigma);
        epix[k] = exp(-x);
     }
     /* symmetric quadrants */
     for (j=0; j<size[0]/2+1; j++) 
	 {
         for (k=0; k<size[1]/2+1; k++) 
		 {
             out->data[j*size[1]+k] = \
             out->data[j*size[1]+size[1]-k-1] = \
             out->data[(size[0]-j-1)*size[1]+k] = \
             out->data[(size[0]-j-1)*size[1]+size[1]-k-1] = \
             (double)(epix[k]*elin[j]);
         }
     }
     
	 
	 sum = 0;
	 for (i = 0; i < size[0]*size[1]; i++)
	 	 sum+=out->data[i];
	 for (i = 0; i < size[0]*size[1]; i++)
         out->data[i]/=sum;
	 
     if (elin)  
		 free(elin);
     if (epix)  
		 free(epix);
	
	 return 0;

}

Masek::filter * Masek::filter2(filter gaussian,IMAGE *im)
{
	int i;
	int j;
	int a, b, c, d;
	filter *newim;
	
	newim = (filter*) malloc(sizeof(filter));

	newim->hsize[0] = im->hsize[0];
	newim->hsize[1] = im->hsize[1];
	newim->data = (double*)malloc(sizeof(double)*im->hsize[0]*im->hsize[1]);
	
	
	
	for (i =0; i<im->hsize[0]*im->hsize[1]; i++)
		newim->data[i] = 0;

	for (i = 0; i<im->hsize[0]; i++)
		for (j = 0; j<im->hsize[1]; j++)
		{
			for (c=0, a = -gaussian.hsize[0]/2; c<gaussian.hsize[0]; a++, c++)
				for (d=0, b=-gaussian.hsize[1]/2; d<gaussian.hsize[1]; b++, d++)
				{
					if ((a+i>=0) && (a+i<im->hsize[0]) && (b+j>=0) && (b+j<im->hsize[1]))
						newim->data[(i+a)*im->hsize[1]+b+j] += im->data[i*im->hsize[1]+j]*gaussian.data[c*gaussian.hsize[1]+d];
				}
		}

//	free (im->data);
//	free(im);
	
	return newim;
}

Masek::filter * Masek::imresize(filter *im, double scaling)
{
	filter *newim;
	int *index0, *index1;
	int i, j, x, y;

	newim = (filter*)malloc(sizeof(filter));
	if (newim)
	{
		newim->hsize[0] = (int)(im->hsize[0]*scaling+AdjPrecision);
		if (newim->hsize[0] <1)
			newim->hsize[0] = 1;
        
		newim->hsize[1] = (int)(im->hsize[1]*scaling+AdjPrecision);
		if (newim->hsize[1]<1)
			newim->hsize[1] = 1;

		newim->data =(double*) malloc(sizeof(double)*newim->hsize[0]*newim->hsize[1]);
		if (newim->data)
		{
			index0 = (int*)malloc(sizeof(int)*im->hsize[0]);
			index1 = (int*)malloc(sizeof(int)*im->hsize[1]);

			if (!index0 || !index1)
			{
				free(im->data);
				free(im);
				free(newim);
				free(newim->data);
				return NULL;
			}
				
			m_select(im->hsize[0], newim->hsize[0], index0);
			m_select(im->hsize[1], newim->hsize[1], index1);

/*			fid = fopen("index0.txt", "w");
			for (i = 0; i<newim->hsize[0]; i++)
				fprintf(fid, "%d %d\n", i+1, index0[i]);
			fclose(fid);

			fid = fopen("index1.txt", "w");
			for (i = 0; i<newim->hsize[0]; i++)
				fprintf(fid, "%d %d\n", i+1, index1[i]);
			fclose(fid);
*/
			for (i=0, y=0; i<im->hsize[0]; i++)
			{
				if (index0[i]>0)
				{
					for (j=0, x=0; j<im->hsize[1]; j++)
					{
						if (index0[i]>0 && index1[j]>0)
						{
							newim->data[y*newim->hsize[1]+x]=im->data[i*im->hsize[1]+j];
							x++;
						}
					}
					y++;
				}
			}
			free(index0);
			free(index1); 
		}
	}

	free(im->data);
	free(im);
	return newim;
}

void m_select(int oldsize, int newsize, int *index)
{
	int i;
	double cur, step, sum;
	
	cur = 0.0;
	step = ((double)newsize)/oldsize;
//	printf("old size is %d, new size is %d, step is %f\n", oldsize, newsize, step);
	if (oldsize<newsize)
	{
		fprintf(stderr, "optimasek: gauss m_select: oldsize < newsize is not supported");
		return;
	}
	if (newsize>oldsize/2+1)
	{
		m_select(oldsize, oldsize-newsize, index);
		reverse(oldsize, index);
		return;
	}
	sum=0;
	for (i=0; i<oldsize; i++)
	{
		sum+=step;
		
		if (sum-cur>0.5)
		{
			index[i]=1;
			cur++;
		}
		else
			index[i]=0;
	}



}

void reverse(int size, int *index)
{
	int *tmp;
	int i;

	tmp = (int*)malloc(sizeof(int)*size);

	for (i = 0; i<size; i++)
		tmp[i] = 1-index[size-i-1];


	for (i = 0; i<size; i++)
		index[i] = tmp[i];

	free(tmp);
	return;

}
/*--------------------------------------------------------------------------------

Generated on Wed Apr 9 08:56:06 2003 for TREES by 1.2.14 written by Dimitri van Heesch, ? 1997-2002
*/
