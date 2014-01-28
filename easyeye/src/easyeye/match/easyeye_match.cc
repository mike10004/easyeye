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

double Matcher::ComputeScore(const Encoding& pEncoding, const Encoding& tEncoding) const
{
	HDCalculator hd;
	switch (mode) {
	case SHIFT_VERT_ONLY:
		return hd.computeHDY(pEncoding, tEncoding, hdScales);
	case SHIFT_HORIZ_ONLY:
		return hd.computeHDX(pEncoding, tEncoding, hdScales);
	case SHIFT_BOTH_OR:
		return hd.computeHDXorY(pEncoding, tEncoding, hdScales);
	case SHIFT_BOTH_AND:
	default:
		return hd.computeHDXandY(pEncoding, tEncoding, hdScales);
	}
}

