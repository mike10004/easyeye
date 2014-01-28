/**************************************************
*  This is a C translation from Masek's matlab code
*  Author:
*  Xiaomei Liu
*  xliu5@cse.nd.edu
*  Computer Vision Research Laboratory
*  Department of Computer Science & Engineering
*  U. of Notre Dame
***************************************************/

/*% segmentiris - peforms automatic segmentation of the iris region
% from an eye image. Also isolates noise areas such as occluding
% eyelids and eyelashes.
%
% Usage: 
% [circleiris, circlepupil, imagewithnoise] = segmentiris(image)
%
% Arguments:
%	eyeimage		- the input eye image
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

function [circleiris, circlepupil, imagewithnoise] = segmentiris(eyeimage)
*/

#include <string.h>
#include <math.h>
#include <float.h>
#include <Masek.h>

//LEE:: segmentation for video images
void Masek::segmentiris(Masek::IMAGE *eyeimage, int *circleiris, int *circlepupil, double *imagewithnoise, int eyelidon, int highlighton, int highlightvalue)
{
	int lpupilradius, upupilradius, lirisradius, uirisradius, reflecthres;
	double scaling;	
	int rowi, coli, ri, rowp, colp, rp, oldrowp;
	double rowid, colid, rid, rowpd, colpd, rpd;
	int irl, iru, icl, icu;
	int imgsize[2];
	IMAGE* imagepupil, *topeyelid, *bottomeyelid;
	int i, j, k, m;
	int linecount;
	double *lines;
	int *xl, *yl;
	int *y2;
	int yla;

	//% define range of pupil & iris radii
	//LEE:Video Data
	//lpupilradius = 25;
	//lpupilradius = 22;
	//in small pupil, 12, 20 works very well.
	//in big pupil, 20, 55
	//the best result 14, 42;
	lpupilradius = 14;//
	upupilradius = 42;
	lirisradius = 62;
	uirisradius = 78;

	 /* //%CASIA
	lpupilradius = 28;
	upupilradius = 75;
	lirisradius = 80;
	uirisradius = 150;*/


	/*%    %LIONS
	%    lpupilradius = 32;
	%    upupilradius = 85;
	%    lirisradius = 145;
	%    uirisradius = 169;
	*/

	//% define scaling factor to speed up Hough transform
	scaling = 0.4;

	reflecthres = 240;
	//reflecthres = 100;

	//% find the iris boundary
	findcircle(eyeimage, lirisradius, uirisradius, scaling, 2, 0.20, 0.19, 1.00, 0.00, &rowi, &coli,&ri);
	//LEE:
	//findcircle(eyeimage, lirisradius, uirisradius, scaling, 2, 0.19, 0.13, 1.00, 0.00, &rowi, &coli,&ri);
	//findcircle(eyeimage, lirisradius, uirisradius, scaling, 2, 0.19, 0.11, 1.00, 0.00, &rowi, &coli,&ri);
	/*rowi = 215;
	coli = 345;
	ri = 115;*/
	/*rowi = 235;
	coli = 275;
	ri = 117;*/

	circleiris[0] = rowi;
	circleiris[1] = coli;
	circleiris[2] = ri;

	printf("iris is %d %d %d\n", rowi, coli, ri);

	rowid = (double)rowi;
	colid = (double)coli;
	rid = (double)ri;

	irl = roundND(rowid-rid);
	iru = roundND(rowid+rid);
	icl = roundND(colid-rid);
	icu = roundND(colid+rid);

	imgsize[0] = eyeimage->hsize[0];
	imgsize[1] = eyeimage->hsize[1];

	if (irl < 1 )
		irl = 1;


	if (icl < 1)
		icl = 1;


	if (iru > imgsize[0])
		iru = imgsize[0];


	if (icu > imgsize[1])
		icu = imgsize[1];
	printf("irl is %d, icl is %d iru is %d, icu is %d\n", irl , icl, iru, icu);

	//% to find the inner pupil, use just the region within the previously
	//% detected iris boundary
	imagepupil = (IMAGE*) malloc (sizeof(IMAGE));

	imagepupil->hsize[0] = iru-irl+1;
	imagepupil->hsize[1] = icu-icl+1;

	imagepupil->data = (unsigned char*)malloc(sizeof(unsigned char)*imagepupil->hsize[0]*imagepupil->hsize[1]);

	for (i = irl-1, k=0; i<iru; i++,k++)
	{
		for (j = icl-1, m=0; j<icu; j++, m++)
			imagepupil->data[k*imagepupil->hsize[1]+m] = eyeimage->data[i*eyeimage->hsize[1]+j];
	}

	//printimage(imagepupil, "imagepupil.txt");
	//%find pupil boundary
	findcircle(imagepupil, lpupilradius, upupilradius ,0.6,2,0.25,0.25,1.00,1.00, &rowp, &colp, &rp);
	//LEE:
	//findcircle(imagepupil, lpupilradius, upupilradius ,0.6,2,0.25,0.21,1.00,1.00, &rowp, &colp, &rp);
	//findcircle(imagepupil, lpupilradius, upupilradius ,0.6,2,0.26,0.24,1.00,1.00, &rowp, &colp, &rp);//Best
	//findcircle(imagepupil, lpupilradius, upupilradius ,0.6,2,0.26,0.25,1.00,1.00, &rowp, &colp, &rp);//Best
	printf("1st: pupil is %d %d %d\n", rowp, colp, rp);

	oldrowp = rowp;
	rowpd = (double)rowp;
	colpd = (double)colp;
	rpd = (double)rp;

	rowpd = (double)irl + rowp;
	colpd = (double)icl + colp;

	rowp = roundND(rowpd);
	colp = roundND(colpd);
	 
	circlepupil[0] = rowp;
	circlepupil[1] = colp;
	circlepupil[2] = rp;
	printf("2nd: pupil is %d %d %d\n", rowp, colp, rp);

	//% set up array for recording noise regions
	//% noise pixels will have NaN values
	for (i = 0; i<eyeimage->hsize[0]*eyeimage->hsize[1]; i++)
		imagewithnoise[i] = eyeimage->data[i];

	if (eyelidon==1)
	{

		//%find top eyelid

		topeyelid = (IMAGE*)malloc(sizeof(IMAGE));
		topeyelid->hsize[0] = oldrowp-rp;
		topeyelid->hsize[1] = imagepupil->hsize[1];
		topeyelid->data = (unsigned char*) malloc(sizeof(unsigned char)*topeyelid->hsize[0]*topeyelid->hsize[1]);

		if (topeyelid->hsize[0]>0 && topeyelid->hsize[1]>0)
		{
			for (i = 0; i<(oldrowp-rp)*imagepupil->hsize[1]; i++)
				topeyelid->data[i] = imagepupil->data[i];

			linecount = findline(topeyelid, &lines);
		}
		else 
			linecount=0;

		if (linecount > 0)
		{
			xl = (int*)malloc(sizeof(int)*topeyelid->hsize[1]);
			yl = (int*)malloc(sizeof(int)*topeyelid->hsize[1]);

			linescoords(lines, topeyelid->hsize[0], topeyelid->hsize[1], xl, yl);

			yla = -1;
			for (i = 0; i<topeyelid->hsize[1]; i++)
			{
				yl[i] = yl[i]+irl-1;
				xl[i]=xl[i]+icl-1;
				if (yla<yl[i])
					yla = yl[i];

				imagewithnoise[(yl[i]-1)*eyeimage->hsize[1]+xl[i]-1] = sqrt((double)-1); // Lee: added cast
			}
			y2 = (int*)malloc(sizeof(int)*yla);
			for (i = 0; i<yla; i++)
			{
				y2[i] = i+1;
				for (j = 0; j<topeyelid->hsize[1]; j++)
					imagewithnoise[(y2[i]-1)*eyeimage->hsize[1]+xl[j]-1] = sqrt((double)-1); // Lee: added cast
			}
			free (xl);
			free (yl);
			free(y2);
			free(lines);
		}

		
		//%find bottom eyelid
		bottomeyelid = (IMAGE*)malloc(sizeof(IMAGE));

		bottomeyelid->hsize[0] = imagepupil->hsize[0]-(oldrowp+rp)+1;
		bottomeyelid->hsize[1] = imagepupil->hsize[1];
		bottomeyelid->data = (unsigned char*) malloc(sizeof(unsigned char)*bottomeyelid->hsize[0]*bottomeyelid->hsize[1]);


		if (bottomeyelid->hsize[0]>0 && bottomeyelid->hsize[1]>0)
		{	
			memcpy(bottomeyelid->data, &imagepupil->data[(oldrowp+rp-1)*imagepupil->hsize[1]], bottomeyelid->hsize[0]*bottomeyelid->hsize[1]);	
			linecount = findline(bottomeyelid, &lines);
		}
		else
			linecount=0;

		if (linecount > 0)
		{
			xl = (int*)malloc(sizeof(int)*bottomeyelid->hsize[1]);
			yl = (int*)malloc(sizeof(int)*bottomeyelid->hsize[1]);

			linescoords(lines, bottomeyelid->hsize[0], bottomeyelid->hsize[1], xl, yl);

			/*fid = fopen("x1.txt", "w");
			for (i=0; i<bottomeyelid->hsize[1]; i++)
				fprintf(fid, "%d %d\n", i+1, xl[i]);
			fclose(fid);

		fid = fopen("y1.txt", "w");
		for (i=0; i<bottomeyelid->hsize[1]; i++)
			fprintf(fid, "%d %d\n", i+1, yl[i]);
		fclose(fid);*/

			yla = eyeimage->hsize[0];
		
			for (i = 0; i<bottomeyelid->hsize[1]; i++)
			{
				yl[i] = yl[i]+irl+oldrowp+rp-2;
				xl[i]=xl[i]+icl-1;
				if ( yla>yl[i])
					yla = yl[i];

				imagewithnoise[(yl[i]-1)*eyeimage->hsize[1]+xl[i]-1] = sqrt((double)-1); // Lee: added cast
			}
			y2 = (int*)malloc(sizeof(int)*(eyeimage->hsize[0]-yla+1));
			for (i = yla-1; i<eyeimage->hsize[0]; i++)
			{
				y2[i-yla+1] = i+1;
				for (j = 0; j<bottomeyelid->hsize[1]; j++)
					imagewithnoise[(y2[i-yla+1]-1)*eyeimage->hsize[1]+xl[j]-1] = sqrt((double)-1); // Lee: added cast
			}
			free (xl);
			free (yl);
			free(y2);
			free(lines);
		}
		free(topeyelid->data);
		free(topeyelid);
		free(bottomeyelid->data);
		free(bottomeyelid);

	}
	/*fid = fopen("imagewithnoise.txt", "w");
	for (i = 0; i<eyeimage->hsize[0]; i++)
		for (j = 0; j<eyeimage->hsize[1]; j++)
		{
			if (gccport::_isnan(imagewithnoise[i*eyeimage->hsize[1]+j]))
					fprintf(fid, "%d %d NaN\n", i+1, j+1);
			else
				fprintf(fid, "%d %d %f\n", i+1, j+1, imagewithnoise[i*eyeimage->hsize[1]+j]);
		}
	fclose(fid);
	*/

	/*%For CASIA, eliminate eyelashes by thresholding
	%ref = eyeimage < 100;
	%coords = find(ref==1);
	%imagewithnoise(coords) = NaN;
	*/
	if (highlighton==1)
	{
		for (i = 0; i<eyeimage->hsize[0]*eyeimage->hsize[1]; i++)
			if (eyeimage->data[i]>highlightvalue)
				imagewithnoise[i] = sqrt((double)-1); // Lee: added cast
	}

	free(imagepupil->data);
	free(imagepupil);
}

int Masek::segmentiris_gt(Masek::IMAGE *eyeimage, char *gndfilename, int *circleiris, int *circlepupil, double *imagewithnoise, int eyelidon, int highlighton, int highlightvalue)
{
	int lpupilradius, upupilradius, lirisradius, uirisradius, reflecthres;
	double scaling;	
	int rowi, coli, ri;//LEE: colp, rowp, , rp, oldrowp;
	int i, j, k;//LEE: delete m
	int iris_xpart[8], iris_ypart[8], iris_valid[8], pupil_xpart[8], pupil_ypart[8], pupil_valid[8];
	FILE *gndfile;
	char tmpchar;
	int count;
	double a, b;
	int left, right;//LEE: delete hasnoise;

//LEE: delete
//	double rowid, colid, rid, rowpd, colpd, rpd;
//	int irl, iru, icl, icu;
//	int imgsize[2];
//IMAGE* imagepupil, *topeyelid, *bottomeyelid;	
//	int linecount;
//	double *lines;
//	int *xl, *yl;
//	int *y2;
//	int yla;
//	char buffer[100];
//	int tmpr;

//% define range of pupil & iris radii

//%CASIA
lpupilradius = 28;
upupilradius = 75;
lirisradius = 80;
uirisradius = 150;

/*%    %LIONS
%    lpupilradius = 32;
%    upupilradius = 85;
%    lirisradius = 145;
%    uirisradius = 169;
*/

//% define scaling factor to speed up Hough transform
scaling = 0.4;

reflecthres = 240;

//read in iris and pupil boundaries.
gndfile = fopen(gndfilename, "r");
if (gndfile==NULL)
{
	printf("could not open gndfile %s \n", gndfilename);
	return -1;
}

fscanf(gndfile, "points formatted x y [+|-]\n");
fscanf(gndfile, "- denotes occluder, + denotes true boundary\n");
fscanf(gndfile, "pupil-iris boundary: 8 points clockwise from north");

for (i = 0; i<8; i++)
{
    fscanf(gndfile, "%d %d %c\n", &pupil_xpart[i], &pupil_ypart[i], &tmpchar);
	if (tmpchar == '+')
		pupil_valid[i] = 1;
	else if (tmpchar == '-')
		pupil_valid[i] = 0;
	else
		printf("file wrong %s\n", gndfilename);

//	printf("%d: %d %d %d\n", i+1, pupil_xpart[i], pupil_ypart[i], pupil_valid[i]);
}

fscanf(gndfile, "iris-sclera boundary: 8 points clockwise from north");

for (i = 0; i<8; i++)
{
    fscanf(gndfile, "%d %d %c\n", &iris_xpart[i], &iris_ypart[i], &tmpchar);
	if (tmpchar == '+')
		iris_valid[i] = 1;
	else if (tmpchar == '-')
		iris_valid[i] = 0;
	else
		printf("file wrong %s\n", gndfilename);

	//printf("%d: %d %d %d\n", i+1, iris_xpart[i], iris_ypart[i], iris_valid[i]);
}

fclose(gndfile);

count=0;
rowi = 0; 
coli = 0;
//decide pupil boundary
for (i = 0; i<4; i++)
{
	if (pupil_valid[i]*pupil_valid[i+4]==1)
	{
		rowi+=(pupil_xpart[i]+pupil_xpart[i+4])/2;
		coli+=(pupil_ypart[i]+pupil_ypart[i+4])/2;
		count++;
	}
	
}
if (count==0)
{
	printf("eye center is not shown %s\n", gndfilename);
	return -1;
}
circlepupil[0] = coli/count;
circlepupil[1] = rowi/count;

ri = 0;
count = 0;
for (i = 0; i<8; i++)
{
	if (pupil_valid[i])
	{
		ri+=(int)sqrt((double)(circlepupil[0]-pupil_ypart[i])*(circlepupil[0]-pupil_ypart[i])+(circlepupil[1]-pupil_xpart[i])*(circlepupil[1]-pupil_xpart[i]));  // Lee: added cast
		count++;
	}
}
circlepupil[2] = ri/count;
printf("pupil is %d %d %d\n", circlepupil[0], circlepupil[1], circlepupil[2]);



//decide iris boundary
count=0;
rowi = 0; 
coli = 0;

for (i = 0; i<4; i++)
{
	if ((iris_valid[i]*iris_valid[i+4])==1)
	{
		rowi+=(iris_xpart[i]+iris_xpart[i+4])/2;
		coli+=(iris_ypart[i]+iris_ypart[i+4])/2;
		count++;
	}
	
}

if (count>0)
{
	circleiris[0] = coli/count;
	circleiris[1] = rowi/count;
}
else
{
	printf("use pupil center\n");
	circleiris[0] = circlepupil[0];
	circleiris[1] = circlepupil[1];
}

ri = 0;
count = 0;
for (i = 0; i<8; i++)
{
	if (iris_valid[i])
	{
		ri+=(int)sqrt((double)(circleiris[0]-iris_ypart[i])*(circleiris[0]-iris_ypart[i])+(circleiris[1]-iris_xpart[i])*(circleiris[1]-iris_xpart[i])); // Lee: added cast
		count++;
	}
}
circleiris[2] = ri/count;

printf("iris is %d %d %d\n", circleiris[0], circleiris[1], circleiris[2]);


//% find the iris boundary

//% set up array for recording noise regions
//% noise pixels will have NaN values
for (i = 0; i<eyeimage->hsize[0]*eyeimage->hsize[1]; i++)
	imagewithnoise[i] = eyeimage->data[i];

if (eyelidon==1)
{
//mark noise part
	for (i = 0; i<8; i++)
	{
	
		if (iris_valid[i]==1)
			continue;

		left = i-1;
		if (left<0)
			left+=8;
	
		right = i+1;
		if (right>7)
			right-=8;

		if (iris_valid[left]==0)
		{
//		printf ("%d %d %d %d\n", iris_ypart[left], iris_ypart[i], iris_xpart[left], iris_xpart[i]);
			a = ((double) (iris_ypart[left]-iris_ypart[i]))/(iris_xpart[left]-iris_xpart[i]); // Lee: added cast
			b = iris_ypart[i]-a*iris_xpart[i];

			for (k = 0; k<eyeimage->hsize[0]; k++)
			{
				for (j = 0; j<eyeimage->hsize[1]; j++)
					if ((a*j+b-k)*(a*circleiris[1]+b-circleiris[0])<=0)
						imagewithnoise[k*eyeimage->hsize[1]+j] = sqrt((double)-1); // Lee: added cast
			}

		
		}
	
	
		switch (i)
		{
			case 0:
			case 4:
				a = 0; 
				b = iris_ypart[i];
				break;
			case 1:
			case 5:
				a = 0.26;
				b = iris_ypart[i]-a*iris_xpart[i];
				break;
			case 2: 
			case 6:
				a = iris_xpart[i];
				b = 0;
				break;
			case 3:
			case 7:
				a = -0.26;
				b = iris_ypart[i]-a*iris_xpart[i];
				break;

		}

	

//	printf("i is %d, a is %f, b is %f\n", i, a, b);
		if ((i==2 || i==6) && b==0)
		{
			for (k = 0; k<eyeimage->hsize[0]; k++)
			{
				for (j = 0; j<eyeimage->hsize[1]; j++)
					if ((a-j)*(a-circleiris[1])<=0)
						imagewithnoise[k*eyeimage->hsize[1]+j] = sqrt((double)-1); // Lee: added cast
			}
		}
		else
		{
			for (k = 0; k<eyeimage->hsize[0]; k++)
			{
				for (j = 0; j<eyeimage->hsize[1]; j++)
					if ((a*j+b-k)*(a*circleiris[1]+b-circleiris[0])<=0)
						imagewithnoise[k*eyeimage->hsize[1]+j] = sqrt((double)-1); // Lee: added cast
			}
		}

	}	
}


/*%For CASIA, eliminate eyelashes by thresholding
%ref = eyeimage < 100;
%coords = find(ref==1);
%imagewithnoise(coords) = NaN;
*/
if (highlighton==1)
{
	for (i = 0; i<eyeimage->hsize[0]*eyeimage->hsize[1]; i++)
		if (eyeimage->data[i]>highlightvalue)
			imagewithnoise[i] = sqrt((double)-1); // Lee: added cast
}

return 0;

}


int Masek::segmentiris_iridian(Masek::IMAGE *eyeimage, char *gndfilename, int *circleiris, int *circlepupil, double *imagewithnoise, int eyelidon, int highlighton, int highlightvalue)
{
	int lpupilradius, upupilradius, lirisradius, uirisradius, reflecthres;
	double scaling;	
	int rowi, coli, ri, rowp, colp, rp, oldrowp;
	double rowid, colid, rid;//LEE: delete rowpd, colpd, rpd;
	int irl, iru, icl, icu;
	int imgsize[2];
	IMAGE* imagepupil, *topeyelid, *bottomeyelid;
	int i, j, k, m;
	int linecount;
	double *lines;
	int *xl, *yl;
	int *y2;
	int yla;
//	int iris_xpart[8], iris_ypart[8], iris_valid[8], pupil_xpart[8], pupil_ypart[8], pupil_valid[8];//LEE:
//	char buffer[100];//LEE:
	FILE *gndfile;
//	char tmpchar;//LEE:
//	int count;//LEE:
//	int tmpr;//LEE:
//	double a, b;
//	int left, right, hasnoise;//LEE:

//% define range of pupil & iris radii

 /* //LEE:Video Data
lpupilradius = 25;
upupilradius = 58;
lirisradius = 62;
uirisradius = 78;*/

/*  //LEE:Video Data
lpupilradius = 12;
upupilradius = 58;
lirisradius = 60;
uirisradius = 80;*/

//%CASIA
lpupilradius = 28;
upupilradius = 75;
lirisradius = 80;
uirisradius = 150;

/*%    %LIONS
%    lpupilradius = 32;
%    upupilradius = 85;
%    lirisradius = 145;
%    uirisradius = 169;
*/

//% define scaling factor to speed up Hough transform
scaling = 0.4;

reflecthres = 240;

//read in iris and pupil boundaries.
gndfile = fopen(gndfilename, "r");
if (gndfile==NULL)
{
	printf("could not open gndfile %s \n", gndfilename);
	return -1;
}
fscanf(gndfile, "%d %d %d %d %d %d",&rowi, &coli, &ri, &rowp, &colp, &rp);
fclose(gndfile);


//% find the iris boundary


circleiris[0] = rowi;
circleiris[1] = coli;
circleiris[2] = ri;

//printf("iris is %d %d %d\n", rowi, coli, ri);

rowid = (double)rowi;
colid = (double)coli;
rid = (double)ri;

irl = roundND(rowid-rid);
iru = roundND(rowid+rid);
icl = roundND(colid-rid);
icu = roundND(colid+rid);

imgsize[0] = eyeimage->hsize[0];
imgsize[1] = eyeimage->hsize[1];

if (irl < 1 )
    irl = 1;


if (icl < 1)
    icl = 1;


if (iru > imgsize[0])
    iru = imgsize[0];


if (icu > imgsize[1])
    icu = imgsize[1];
//printf("irl is %d, icl is %d iru is %d, icu is %d\n", irl , icl, iru, icu);

//% to find the inner pupil, use just the region within the previously
//% detected iris boundary
imagepupil = (IMAGE*) malloc (sizeof(IMAGE));

imagepupil->hsize[0] = iru-irl+1;
imagepupil->hsize[1] = icu-icl+1;
imagepupil->data = (unsigned char*)malloc(sizeof(unsigned char)*imagepupil->hsize[0]*imagepupil->hsize[1]);

for (i = irl-1, k=0; i<iru; i++,k++)
{
	for (j = icl-1, m=0; j<icu; j++, m++)
		imagepupil->data[k*imagepupil->hsize[1]+m] = eyeimage->data[i*eyeimage->hsize[1]+j];
}

//printimage(imagepupil, "imagepupil.txt");
//%find pupil boundary
//findcircle(imagepupil, lpupilradius, upupilradius ,0.6,2,0.25,0.25,1.00,1.00, &rowp, &colp, &rp);
//printf("pupil is %d %d %d\n", rowp, colp, rp);

oldrowp = rowp-irl;
/*rowpd = (double)rowp;
colpd = (double)colp;
rpd = (double)rp;

rowpd = (double)irl + rowp;
colpd = (double)icl + colp;

rowp = roundND(rowpd);
colp = roundND(colpd);
 */
circlepupil[0] = rowp;
circlepupil[1] = colp;
circlepupil[2] = rp;

//printf("pupil is %d %d %d\n", rowp, colp, rp);

//% set up array for recording noise regions
//% noise pixels will have NaN values
for (i = 0; i<eyeimage->hsize[0]*eyeimage->hsize[1]; i++)
	imagewithnoise[i] = eyeimage->data[i];

if (eyelidon==1)
{
//%find top eyelid

	topeyelid = (IMAGE*)malloc(sizeof(IMAGE));
	topeyelid->hsize[0] = oldrowp-rp;
	topeyelid->hsize[1] = imagepupil->hsize[1];
	topeyelid->data = (unsigned char*) malloc(sizeof(unsigned char)*topeyelid->hsize[0]*topeyelid->hsize[1]);

	if (topeyelid->hsize[0]>0 && topeyelid->hsize[1]>0)
	{
		for (i = 0; i<(oldrowp-rp)*imagepupil->hsize[1]; i++)
			topeyelid->data[i] = imagepupil->data[i];

		linecount = findline(topeyelid, &lines);
	}
	else 
		linecount=0;

	if (linecount > 0)
	{
		xl = (int*)malloc(sizeof(int)*topeyelid->hsize[1]);
		yl = (int*)malloc(sizeof(int)*topeyelid->hsize[1]);

		linescoords(lines, topeyelid->hsize[0], topeyelid->hsize[1], xl, yl);

		yla = -1;
		for (i = 0; i<topeyelid->hsize[1]; i++)
		{
			yl[i] = yl[i]+irl-1;
			xl[i]=xl[i]+icl-1;
			if (yla<yl[i])
				yla = yl[i];

			imagewithnoise[(yl[i]-1)*eyeimage->hsize[1]+xl[i]-1] = sqrt((double)-1); // Lee: added cast
		}
		y2 = (int*)malloc(sizeof(int)*yla);
		for (i = 0; i<yla; i++)
		{
			y2[i] = i+1;
			for (j = 0; j<topeyelid->hsize[1]; j++)
				imagewithnoise[(y2[i]-1)*eyeimage->hsize[1]+xl[j]-1] = sqrt((double)-1); // Lee: added cast
		}
		free (xl);
		free (yl);
		free(y2);
		free(lines);
	}


//%find bottom eyelid
	bottomeyelid = (IMAGE*)malloc(sizeof(IMAGE));

	bottomeyelid->hsize[0] = imagepupil->hsize[0]-(oldrowp+rp)+1;
	bottomeyelid->hsize[1] = imagepupil->hsize[1];
	bottomeyelid->data = (unsigned char*) malloc(sizeof(unsigned char)*bottomeyelid->hsize[0]*bottomeyelid->hsize[1]);

	if (bottomeyelid->hsize[0]>0 && bottomeyelid->hsize[1]>0)
	{
		memcpy(bottomeyelid->data, &imagepupil->data[(oldrowp+rp-1)*imagepupil->hsize[1]], bottomeyelid->hsize[0]*bottomeyelid->hsize[1]);


		linecount = findline(bottomeyelid, &lines);
	}
	else
		linecount=0;


	if (linecount > 0)
	{
		xl = (int*)malloc(sizeof(int)*bottomeyelid->hsize[1]);
		yl = (int*)malloc(sizeof(int)*bottomeyelid->hsize[1]);

		linescoords(lines, bottomeyelid->hsize[0], bottomeyelid->hsize[1], xl, yl);

	/*fid = fopen("x1.txt", "w");
	for (i=0; i<bottomeyelid->hsize[1]; i++)
		fprintf(fid, "%d %d\n", i+1, xl[i]);
	fclose(fid);

	fid = fopen("y1.txt", "w");
	for (i=0; i<bottomeyelid->hsize[1]; i++)
		fprintf(fid, "%d %d\n", i+1, yl[i]);
	fclose(fid);*/

		yla = eyeimage->hsize[0];
	
		for (i = 0; i<bottomeyelid->hsize[1]; i++)
		{
			yl[i] = yl[i]+irl+oldrowp+rp-2;
			xl[i]=xl[i]+icl-1;
			if ( yla>yl[i])
				yla = yl[i];

			imagewithnoise[(yl[i]-1)*eyeimage->hsize[1]+xl[i]-1] = sqrt((double)-1); // Lee: added cast
		}
		y2 = (int*)malloc(sizeof(int)*(eyeimage->hsize[0]-yla+1));
		for (i = yla-1; i<eyeimage->hsize[0]; i++)
		{
			y2[i-yla+1] = i+1;
			for (j = 0; j<bottomeyelid->hsize[1]; j++)
				imagewithnoise[(y2[i-yla+1]-1)*eyeimage->hsize[1]+xl[j]-1] = sqrt((double)-1); // Lee: added cast
		}
		free (xl);
		free (yl);
		free(y2);
		free(lines);
	}	   
	free(topeyelid->data);
	free(topeyelid);
	free(bottomeyelid->data);
	free(bottomeyelid);

}
/*fid = fopen("imagewithnoise.txt", "w");
for (i = 0; i<eyeimage->hsize[0]; i++)
	for (j = 0; j<eyeimage->hsize[1]; j++)
	{
		if (gccport::_isnan(imagewithnoise[i*eyeimage->hsize[1]+j]))
				fprintf(fid, "%d %d NaN\n", i+1, j+1);
		else
			fprintf(fid, "%d %d %f\n", i+1, j+1, imagewithnoise[i*eyeimage->hsize[1]+j]);
	}
fclose(fid);
*/

/*%For CASIA, eliminate eyelashes by thresholding
%ref = eyeimage < 100;
%coords = find(ref==1);
%imagewithnoise(coords) = NaN;
*/
	if (highlighton==1)
	{
		for (i = 0; i<eyeimage->hsize[0]*eyeimage->hsize[1]; i++)
			if (eyeimage->data[i]>highlightvalue)
				imagewithnoise[i] = sqrt((double)-1);
	}

	free(imagepupil->data);
	free(imagepupil);
	printf("leaving segmentiris\n");
	return 0;

}
