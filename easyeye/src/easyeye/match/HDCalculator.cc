#include <algorithm>
#include "HDCalculator.h"
#include <iostream>
#include <cstring>
#include "../common/mylog.h"
#include <cmath>
#include <limits>
#include <portability.h>

using namespace easyeye;
using mylog::Logs;

HDCalculator::HDCalculator(const Encoding& e1, const Encoding& e2) 
    : e1_(e1), e2_(e2), width_(e1.width()), height_(e1.height()), 
        shifted_e2_(), maxShiftX(10), maxShiftY(3)
{
    shifted_e2_.CopyFrom(e2.width(), e2.height(), e2.nscales(), e2.irisTemplate, e2.irisMask);
    if (!e1_.IsCongruent(e2_)) {
        flag_ = Matcher::INCONGRUENT;
    } else {
        flag_ = Matcher::CLEAN;
    }
}

Matcher::Flag HDCalculator::flag() const 
{
    return flag_;
}

HDCalculator::~HDCalculator()
{
}

//double returnNaN() {
//    return 
//}

const double HDCalculator::HD_NAN = std::numeric_limits<double>::quiet_NaN();//;//returnNaN();//quietnan;//sqrt(0);//nan(NULL);

double HDCalculator::calcHD(const Encoding& e1, const Encoding& e2, int* num_diff, int* num_unmasked, int* same_ones, int* same_zeros)
{
	int num_unmasked_bits = 0;
	int num_bits_diff = 0; 
    int num_same_ones = 0, num_same_zeros = 0;
    const int width = e1.width(), height = e1.height();
    for (int i = 0; i < width * height; i++) {
        bool masked = e1.irisMask[i] == 1 || e2.irisMask[i] == 1;
        if (masked) {
            continue;
        }
        num_unmasked_bits++;
        if (e1.irisTemplate[i] != e2.irisTemplate[i]) {
            num_bits_diff++;
        } else {
            if (e1.irisTemplate[i] == 0) {
                num_same_ones++;
            } else {
                num_same_zeros++;
            }
        }
    }
    double hd;
    if (num_unmasked_bits == 0) {
        hd = HD_NAN;
    } else {
        hd = (double)num_bits_diff / (double)num_unmasked_bits;
    }
    *num_diff = num_bits_diff;
    *num_unmasked = num_unmasked_bits;
    *same_ones = num_same_ones;
    *same_zeros = num_same_zeros;
    return hd;
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

double HDCalculator::computeHDX(int scales) 
{
    double min_score = std::numeric_limits<double>::max();
	for (int shifts = -maxShiftX; shifts <= maxShiftX; shifts++) {    
		ShiftBits::X_Shiftbits(e2_.irisTemplate, width_, height_, shifts, scales, shifted_e2_.irisTemplate);
		ShiftBits::X_Shiftbits(e2_.irisMask, width_, height_, shifts, scales, shifted_e2_.irisMask);
        int diff, unmasked, ones, zeros;
		double hd1 = calcHD(e1_, shifted_e2_, &diff, &unmasked, &ones, &zeros);
		if (hd1 < min_score) {
            min_score = hd1;
        }
        if (diff == 0 && unmasked != 0) {
            mylog::Logs::GetLogger().Log(mylog::DEBUG, "at shift %d, num diff = 0; %d unmasked bits; of same unmasked bits, %d ones and %d zeros\n", shifts, unmasked, ones, zeros);
            break;
        }
	}
	return min_score;

}


double HDCalculator::computeHDY(int scales) 
{
	double hd = std::numeric_limits<double>::max();
	for (int shifts = -maxShiftY; shifts <= maxShiftY; shifts++)
	{    
		ShiftBits::Y_Shiftbits(e2_.irisTemplate, width_, height_, shifts, scales, shifted_e2_.irisTemplate);
		ShiftBits::Y_Shiftbits(e2_.irisMask, width_, height_, shifts, scales, shifted_e2_.irisMask);
        int diff, unmasked, ones, zeros;
		double hd1 = calcHD(e1_, shifted_e2_, &diff, &unmasked, &ones, &zeros);
        if (hd1 < hd) {
			hd = hd1;
        }
        if (diff == 0 && unmasked != 0) {
            mylog::Logs::GetLogger().Log(mylog::DEBUG, "at shift %d, num diff = 0; %d unmasked bits; of same unmasked bits, %d ones and %d zeros\n", shifts, unmasked, ones, zeros);
            break;
        }
	}
	return hd;
}

double HDCalculator::computeHDXorY(int scales) 
{
	double xHD = computeHDX(scales);
    if (portability::Math::IsNaN(xHD)) {
        return HD_NAN;
    }
	double yHD = computeHDY(scales);
    if (portability::Math::IsNaN(yHD)) {
        return HD_NAN;
    }
	double hd = std::min(xHD, yHD);
	return hd;
}

double HDCalculator::computeHDXandY(int scales) 
{
    double min_hd = std::numeric_limits<double>::max();
	for (int shifts1 = -maxShiftY; shifts1 <= maxShiftY; shifts1++) { 	 
		ShiftBits::Y_Shiftbits(e2_.irisTemplate, width_, height_, shifts1, scales, shifted_e2_.irisTemplate);
		ShiftBits::Y_Shiftbits(e2_.irisMask, width_, height_, shifts1, scales, shifted_e2_.irisMask);
		for (int shifts2 = -maxShiftX; shifts2 <= maxShiftX; shifts2++) {
            ShiftBits::X_Shiftbits(e2_.irisTemplate, width_, height_, shifts2, scales, shifted_e2_.irisTemplate);
            ShiftBits::X_Shiftbits(e2_.irisMask, width_, height_, shifts2, scales, shifted_e2_.irisMask);
            int diff, unmasked, ones, zeros;
            double hd1 = calcHD(e1_, shifted_e2_, &diff, &unmasked, &ones, &zeros);
            if (hd1 < min_hd) {
                min_hd = hd1;
            }
		}
	}
    return min_hd;
}

