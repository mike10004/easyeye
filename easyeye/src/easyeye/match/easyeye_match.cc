/*
 * vasirmatch.cc
 *
 *  Created on: Jul 12, 2013
 *      Author: mchaberski
 */


#include "../encode/easyeye_encode.h"
#include "easyeye_match.h"
#include "HDCalculator.h"

using namespace easyeye;

Matcher::Matcher() : mode(DEFAULT_MODE), hdScales(1)
{
}

Matcher::Matcher(MatchingMode matchingMode) : mode(matchingMode), hdScales(1)
{
}

double Matcher::ComputeScore(const Encoding& pEncoding, const Encoding& tEncoding, Flag *flag) const
{
	HDCalculator hd(pEncoding, tEncoding);
    if (hd.flag() != Matcher::CLEAN) {
        if (flag != NULL) *flag = hd.flag();
        return HDCalculator::HD_NAN;
    }
    double score;
	switch (mode) {
	case SHIFT_VERT_ONLY:
		score = hd.computeHDY(hdScales);
        break;
	case SHIFT_HORIZ_ONLY:
		score = hd.computeHDX(hdScales);
        break;
	case SHIFT_BOTH_OR:
		score = hd.computeHDXorY(hdScales);
        break;
	case SHIFT_BOTH_AND:
	default:
		score = hd.computeHDXandY(hdScales);
        break;
	}
    if (flag != NULL) {
        *flag = hd.flag();
    }
    return score;
}

