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
        flag_ = MatchInfo::INCONGRUENT;
    } else {
        flag_ = MatchInfo::CLEAN;
    }
}

MatchInfo::Flag HDCalculator::flag() const 
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

void HDCalculator::calcHD(const Encoding& e1, const Encoding& e2, MatchInfo& match_info)
{
	match_info.num_unmasked_bits = 0;
    match_info.num_different_bits = 0;
    int num_common_ones = 0, num_common_zeros = 0;
    const int width = e1.width(), height = e1.height();
    for (int i = 0; i < width * height; i++) {
        bool masked = e1.irisMask[i] == 1 || e2.irisMask[i] == 1;
        if (masked) {
            continue;
        }
        match_info.num_unmasked_bits++;
        if (e1.irisTemplate[i] != e2.irisTemplate[i]) {
            match_info.num_different_bits++;
        } else {
            if (e1.irisTemplate[i] == 0) {
                num_common_ones++;
            } else {
                num_common_zeros++;
            }
        }
    }
    if (match_info.num_unmasked_bits == 0) {
        match_info.hd = HD_NAN;
    } else {
        match_info.hd = (double)match_info.num_different_bits / (double)match_info.num_unmasked_bits;
    }
    match_info.common_bits_differential = num_common_ones - num_common_zeros;
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

void HDCalculator::computeHDX(int scales, MatchInfo& match_info) 
{
    match_info.Reset();
    MatchInfo candidate;
    double min_score = std::numeric_limits<double>::max();
	for (int shifts = -maxShiftX; shifts <= maxShiftX; shifts++) {    
        candidate.y_shift = 0;
        candidate.x_shift = shifts;
		ShiftBits::X_Shiftbits(e2_.irisTemplate, width_, height_, shifts, scales, shifted_e2_.irisTemplate);
		ShiftBits::X_Shiftbits(e2_.irisMask, width_, height_, shifts, scales, shifted_e2_.irisMask);
		calcHD(e1_, shifted_e2_, candidate);
		if (candidate.hd < min_score) {
            min_score = candidate.hd;
            match_info.CopyFrom(candidate);
        }
	}
}


void HDCalculator::computeHDY(int scales, MatchInfo& match_info) 
{
    match_info.Reset();
    MatchInfo candidate;
	double min_score = std::numeric_limits<double>::max();
	for (int shifts = -maxShiftY; shifts <= maxShiftY; shifts++) {    
        candidate.x_shift = 0;
        candidate.y_shift = shifts;
		ShiftBits::Y_Shiftbits(e2_.irisTemplate, width_, height_, shifts, scales, shifted_e2_.irisTemplate);
		ShiftBits::Y_Shiftbits(e2_.irisMask, width_, height_, shifts, scales, shifted_e2_.irisMask);
		calcHD(e1_, shifted_e2_, candidate);
        if (candidate.hd < min_score) {
			min_score = candidate.hd;
            match_info.CopyFrom(candidate);
        }
	}
}

void HDCalculator::computeHDXorY(int scales, MatchInfo& match_info) 
{
    MatchInfo info_x;
	computeHDX(scales, info_x);
    MatchInfo info_y;
	computeHDY(scales, info_y);
    match_info.CopyFrom(info_x); // fill in common fields
    if (info_x.flag != MatchInfo::CLEAN) {
        match_info.flag = info_x.flag;
    }
    if (info_y.flag != MatchInfo::CLEAN) {
        match_info.flag = info_y.flag;
    }
	match_info.hd = std::min(info_x.hd, info_y.hd);
}

void HDCalculator::computeHDXandY(int scales, MatchInfo& match_info) 
{
    double min_hd = std::numeric_limits<double>::max();
    MatchInfo candidate;
	for (int shifts1 = -maxShiftY; shifts1 <= maxShiftY; shifts1++) { 	 
		ShiftBits::Y_Shiftbits(e2_.irisTemplate, width_, height_, shifts1, scales, shifted_e2_.irisTemplate);
		ShiftBits::Y_Shiftbits(e2_.irisMask, width_, height_, shifts1, scales, shifted_e2_.irisMask);
        candidate.y_shift = shifts1;
		for (int shifts2 = -maxShiftX; shifts2 <= maxShiftX; shifts2++) {
            ShiftBits::X_Shiftbits(e2_.irisTemplate, width_, height_, shifts2, scales, shifted_e2_.irisTemplate);
            ShiftBits::X_Shiftbits(e2_.irisMask, width_, height_, shifts2, scales, shifted_e2_.irisMask);
            candidate.x_shift = shifts2;
            calcHD(e1_, shifted_e2_, candidate);
            if (candidate.hd < min_hd) {
                min_hd = candidate.hd;
                match_info.CopyFrom(candidate);
            }
		}
	}
}

