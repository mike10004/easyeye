/*
 * easyeye_match.cc
 *
 *  Created on: Jul 12, 2013
 *      Author: mchaberski
 */


#include "../encode/easyeye_encode.h"
#include "easyeye_match.h"
#include "HDCalculator.h"
#include "../common/mylog.h"
#include <cassert>

using namespace easyeye;
using mylog::Logs;
using mylog::DEBUG;

const double MatchInfo::START_HD = HDCalculator::HD_NAN;

MatchInfo::MatchInfo()
    : flag(START_FLAG), 
        template_width(START_TEMPLATE_WIDTH), 
        template_height(START_TEMPLATE_HEIGHT), 
        num_unmasked_bits(START_NUM_UNMASKED_BITS),
        num_different_bits(START_NUM_DIFFERENT_BITS),
        common_bits_differential(START_COMMON_BITS_DIFFERENTIAL),
        x_shift(START_X_SHIFT), 
        y_shift(START_Y_SHIFT), 
        hd(START_HD)
{
}

void MatchInfo::Reset()
{
    flag = START_FLAG; 
    template_width = START_TEMPLATE_WIDTH; 
    template_height = START_TEMPLATE_HEIGHT; 
    num_unmasked_bits = START_NUM_UNMASKED_BITS;
    num_different_bits = START_NUM_DIFFERENT_BITS;
    common_bits_differential = (START_COMMON_BITS_DIFFERENTIAL);
    x_shift = START_X_SHIFT; 
    y_shift = START_Y_SHIFT; 
    hd = START_HD;
}

void MatchInfo::CopyFrom(const MatchInfo& other)
{
    flag = other.flag; 
    template_width = other.template_width; 
    template_height = other.template_height; 
    num_unmasked_bits = other.num_unmasked_bits;
    num_different_bits = other.num_different_bits;
    common_bits_differential = other.common_bits_differential;
    x_shift = other.x_shift; 
    y_shift = other.y_shift; 
    hd = other.hd;
}

Matcher::Matcher() 
    : mode(DEFAULT_MODE), hdScales(1), logger_(Logs::GetLogger("match"))
{
}

Matcher::Matcher(MatchingMode matchingMode) 
    : mode(matchingMode), hdScales(1), logger_(Logs::GetLogger("match"))
{
}

double Matcher::ComputeScore(const Encoding& pEncoding, const Encoding& tEncoding) const
{
    MatchInfo ignored_match_info_;
    double score = ComputeScore(pEncoding, tEncoding, ignored_match_info_);
    if (ignored_match_info_.flag != MatchInfo::CLEAN) {
        logger_.Log(DEBUG, "match not clean; code %d score %f\n", ignored_match_info_.flag, score);
    }
    return score;
}

double Matcher::ComputeScore(const Encoding& pEncoding, const Encoding& tEncoding, MatchInfo& match_info) const
{
    match_info.Reset();
	HDCalculator hd_calculator(pEncoding, tEncoding);
    if (hd_calculator.flag() != MatchInfo::CLEAN) {
        match_info.flag = hd_calculator.flag();
        return HDCalculator::HD_NAN;
    }
	switch (mode) {
	case SHIFT_VERT_ONLY:
		hd_calculator.computeHDY(hdScales, match_info);
        break;
	case SHIFT_HORIZ_ONLY:
		hd_calculator.computeHDX(hdScales, match_info);
        break;
	case SHIFT_BOTH_AND:
		hd_calculator.computeHDXandY(hdScales, match_info);
        break;
	case SHIFT_BOTH_OR: 
		hd_calculator.computeHDXorY(hdScales, match_info);
        break;
    default:
        assert(false);
	}
    return match_info.hd;
}

