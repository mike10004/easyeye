#include "HDCalculator.h"
#include <iostream>
#include <cstring>
#include "../common/mylog.h"
#include <cmath>
#include <limits>
#include <portability.h>

using namespace easyeye;
using mylog::Logs;



HDCalculator::HDCalculator()
	: C(NULL), mask2s(NULL), mask1s(NULL),
	  template1s(NULL), template2s(NULL), mask(NULL)
{
	width = 0;
	height = 0;
	maxShiftX = 10;
	maxShiftY = 3;
}
HDCalculator::~HDCalculator()
{
	delete[] template1s;
	delete[] mask1s;
	delete[] mask;
	delete[] C;
}

double returnNaN() {
    return std::numeric_limits<double>::quiet_NaN();
}

const double HDCalculator::HD_NAN = returnNaN();//quietnan;//sqrt(0);//nan(NULL);

bool HDCalculator::initializeTemplates(const Encoding& classTemplate1,
						       const Encoding& classTemplate2)
{
	int t_width = classTemplate1.angular_resolution();
	int t_height = classTemplate1.radial_resolution();

	if ( (t_width != classTemplate2.angular_resolution() ) || 
		 (t_height != classTemplate2.radial_resolution() ) ) 
	{
		Logs::GetLogger().Log(mylog::ERROR, "HDCalculator::initializeTemplate encodings are incongruent\n");
		return false;
	}
	
	if (width*height == 0) 
	{
		template1s = new int[t_width*t_height];
		mask1s = new int[t_width*t_height];		
		mask = new int[t_width*t_height];
		C = new int[t_width*t_height];
	}
	else
	{
		delete[] template1s;
		template1s = new int[t_width*t_height];

		delete[] mask1s;
		mask1s = new int[t_width*t_height];

		delete[] mask;
		mask = new int[t_width*t_height];

		delete[] C;
		C = new int[t_width*t_height];
	}

	width = t_width;
	height = t_height;
    return true;
}

double HDCalculator::calcHD(int* tTemplate, int* tMask, int* qTemplate, int* qMask,
									   int* newTemplate, int* newMask)
{
	int nummaskbits = 0;
	int bitsdiff = 0; 
	int totalbits = 0;
	double hd1; 

    for (int i = 0; i < width * height; i++) 
	{
      mask[i] = newMask[i] | qMask[i];
      if (mask[i] == 1)
		nummaskbits++;	

	  C[i] = newTemplate[i] ^ qTemplate[i];
      C[i] = C[i] & (1-mask[i]);
      if (C[i] == 1)
		bitsdiff++;	  
    }

    totalbits = width * height - nummaskbits;    
	
	
    if (totalbits == 0)        
      hd1 = -1;    
    else		        
      hd1 = (double)bitsdiff / totalbits;
	
	return hd1;
}
class ShiftBits 
{
public:
static void X_Shiftbits(int *templates, int width, int height, int noshifts,int nscales, int *templatenew)
{
	int i, s, p, x, y;
	for (i = 0; i<width*height; i++)
		templatenew[i] = 0;
	
	s = 2*nscales*(int)fabs((float)noshifts);	
	p = width-s;
	
	if (noshifts == 0)
	{
		for (i = 0; i<width*height; i++)
			templatenew[i] = templates[i];
	}       
	else if (noshifts<0) //shift left
	{
	  for (y = 0; y<height; y++)
	    for (x = 0; x<p; x++)
	      templatenew[y*width+x] = templates[y*width+s+x];
		   
	  for (y = 0; y<height; y++)
	    for (x=p; x<width; x++)
	      templatenew[y*width+ x] = templates[y*width+ x-p];
		
	}    
	else // shift right
	{
	  for (y = 0; y<height; y++)
	    for (x = s; x<width; x++)
	      templatenew[y*width+ x] = templates[y*width+x-s];		
    
	  for (y = 0; y<height; y++)
	    for (x=0; x<s; x++)
	      templatenew[y*width+x] = templates[y*width+ p+x];
	}
}

static void Y_Shiftbits(int *templates, int width, int height, int noshifts,int nscales, int *templatenew)
{
	int i, s, p, x, y;
	for (i = 0; i<width*height; i++)
		templatenew[i] = 0;

	// For up and down we should shift one bit (Radius)
	s = nscales*(int)fabs((float)noshifts);
	p = height-s;

	if (noshifts == 0)
	{
		for (i = 0; i<width*height; i++)
			templatenew[i] = templates[i];
	}
 	else if (noshifts<0) // Shift upwards
	{
	  for (y = 0; y<p; y++)
	    for (x = 0; x<width; x++)
			templatenew[y*width+x] = templates[(y+s)*width+x];
    
	  for (y = p; y < height; y++)
	    for (x = 0; x<width; x++)
			templatenew[y*width+ x] = 1; // This is not a circle
	}    
	else
	{
	  for (y = s; y<height; y++) // Shift downwards
	    for (x = 0; x<width; x++)
			templatenew[y*width+ x] = templates[(y-s)*width+x];

	  for (y = 0; y<s; y++)
	    for (x=0; x<width; x++)
			templatenew[y*width+ x] = 1; // Again, not a circle
	}
}

};

double HDCalculator::computeHDX(const Encoding& classTemplate1,
						       const Encoding& classTemplate2,
						       int scales) 
{
	if (!initializeTemplates(classTemplate1, classTemplate2)) {
        return HD_NAN;
    }
    

	int *template1 = classTemplate1.iris_template_ptr();
	int *template2 = classTemplate2.iris_template_ptr();
	int *mask1 = classTemplate1.iris_mask_ptr();
	int *mask2 = classTemplate2.iris_mask_ptr();

	double hd = -1;

	for (int shifts = -maxShiftX; shifts <= maxShiftX; shifts++)
	{    
		ShiftBits::X_Shiftbits(template1, width, height, shifts, scales, template1s);
		ShiftBits::X_Shiftbits(mask1, width, height, shifts, scales, mask1s);

		double hd1 = calcHD(template1, mask1, template2, mask2, template1s, mask1s);

		if(hd1 == -1)
			hd = -1;
		else if(hd1 < hd || hd == -1) 
			hd = hd1;
	}

	return hd;

}


double HDCalculator::computeHDY(const Encoding& classTemplate1,
						       const Encoding& classTemplate2,
						       int scales) 
{
	if (!initializeTemplates(classTemplate1, classTemplate2)) {
        return HD_NAN;
    }

	int *template1 = classTemplate1.iris_template_ptr();
	int *template2 = classTemplate2.iris_template_ptr();
	int *mask1 = classTemplate1.iris_mask_ptr();
	int *mask2 = classTemplate2.iris_mask_ptr();

	double hd = -1;
	int index = 1;

	for (int shifts = -maxShiftY; shifts <= maxShiftY; shifts++)
	{    
		ShiftBits::Y_Shiftbits(template1, width, height, shifts, scales, template1s);
		ShiftBits::Y_Shiftbits(mask1, width, height, shifts, scales, mask1s);

		double hd1 = calcHD(template1, mask1, template2, mask2, template1s, mask1s);

		if(hd1 == -1)
			hd = -1;
		else if(hd1 < hd || hd == -1) 
			hd = hd1;
	}
	
	return hd;
}

double HDCalculator::computeHDXorY(const Encoding& classTemplate1,
						       const Encoding& classTemplate2,
						       int scales) 
{
	double xHD = computeHDX(classTemplate1, classTemplate2, scales);
    if (portability::Math::IsNaN(xHD)) {
        return HD_NAN;
    }
	double yHD = computeHDY(classTemplate1, classTemplate2, scales);
    if (portability::Math::IsNaN(yHD)) {
        return HD_NAN;
    }
	double hd;
	if(xHD < yHD)
		hd = xHD;
	else
		hd = yHD;


	return hd;
}

double HDCalculator::computeHDXandY(const Encoding& classTemplate1,
						       const Encoding& classTemplate2,
						       int scales) 
{
	
	if (!initializeTemplates(classTemplate1, classTemplate2)) {
        return HD_NAN;
    }
	template2s = new int[width*height];
	mask2s = new int[width*height];

	int *template1 = classTemplate1.iris_template_ptr();
	int *template2 = classTemplate2.iris_template_ptr();
	int *mask1 = classTemplate1.iris_mask_ptr();
	int *mask2 = classTemplate2.iris_mask_ptr();

	double hd = -1;
	int index = 1;

	for (int shifts1 = -maxShiftY; shifts1 <= maxShiftY; shifts1++)
	{ 	 
		ShiftBits::Y_Shiftbits(template1, width, height, shifts1, scales, template1s);
		ShiftBits::Y_Shiftbits(mask1, width, height, shifts1, scales, mask1s);
		
		for (int shifts2 = -maxShiftX; shifts2 <= maxShiftX; shifts2++)
		{
			ShiftBits::X_Shiftbits(template1s, width, height, shifts2, scales, template2s);
			ShiftBits::X_Shiftbits(mask1s, width, height, shifts2, scales, mask2s);
			
			double hd1 = calcHD(template1, mask1, template2, mask2, template2s, mask2s);

			if(hd1 == -1)
				hd = -1;
			else if(hd1 < hd || hd == -1) 
				hd = hd1;

		}
	}

	delete[] template2s;
	delete[] mask2s;

	return hd;
}

