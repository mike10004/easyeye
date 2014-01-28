/**************************************************
*  Funtion: some commonly used functions
*  Author:
*  Xiaomei Liu
*  xliu5@cse.nd.edu
*  Computer Vision Research Laboratory
*  Department of Computer Science & Engineering
*  U. of Notre Dame
***************************************************/

#include <global.h>
#include <math.h>
#include <float.h>
#include <Masek.h>
#include <portability.h>

int Masek::roundND(double x)
{
	int ret;
	if (x>=0.0)
		ret = (int)(x+0.5);
	else
		ret = (int)(x-0.5);
	return ret;
}

int Masek::fix(double x)
{
	int ret;
	
	ret = (int)x;
	
	return ret;
}

//void Masek::printfilter(Masek::filter *mfilter, char *filename)
//{
//	FILE *fid;
//	int i, j;
//    
//	fid = fopen(filename, "w");
//	for (i = 0; i<mfilter->hsize[0]; i++)
//		for (j = 0; j<mfilter->hsize[1]; j++)
//		{
//			if (portability::Math::IsNaN(mfilter->data[i*mfilter->hsize[1]+j])) // Lee: Renamed from 'isnan'
//				fprintf(fid, "%d %d NaN\n", i+1, j+1);
//			else
//				fprintf(fid, "%d %d %f\n", i+1, j+1, mfilter->data[i*mfilter->hsize[1]+j]);
//		}
//	fclose(fid);
//
//}
//
//void Masek::printimage(Masek::IMAGE *m, char *filename)
//{
//	FILE *fid;
//	int i, j;
//
//	fid = fopen(filename, "w");
//	for (i = 0; i<m->hsize[0]; i++)
//		for (j = 0; j<m->hsize[1]; j++)
//		{
//			fprintf(fid, "%d %d %d\n", i+1, j+1, m->data[i*m->hsize[1]+j]);
//		}
//	fclose(fid);
//
//}

/*double* mulmatrix(double *A, double *B, int size1, int size2, int size3, int size4)
{
	int i, j, k;
	double val;
	double *tmp;
	if (size2!=size3)
		return 0;

	tmp = (double*)malloc(sizeof(double)*size1*size3);
	
	for (i = 0; i<size1; i++)
		for (j = 0; j<size4; j++)
		{
			val = 0;
			for (k = 0; k<size2; k++)
			{
				val+=A[i*size2+k]*B[k*size4+j];
			}
			tmp[i*size4+j]=val;
		}
	return tmp;
}

*/
