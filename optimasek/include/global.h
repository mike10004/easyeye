#ifndef _GLOBAL_H_
#define _GLOBAL_H_

#include <stdio.h>
#include <stdlib.h>

struct mat_data
{
    char name[50];		/* the name of the data			*/
    int dim[2];		/* the dimension of the data			*/
    void *ptr;		/* the point to the data*/
};

struct _image
{
	int hsize[2];
	unsigned char *data;
};

struct _filter
{
	int hsize[2];
	double *data;
};

struct a_complex
{
    double real;
    double img;
};


#endif
