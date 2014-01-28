/*
 * vasirmatch.h
 *
 *  Created on: Jul 12, 2013
 *      Author: mchaberski
 */

#ifndef VASIRMATCH_H_
#define VASIRMATCH_H_

#include "../encode/easyeye_encode.h"

namespace easyeye
{


class Matcher
{
public:
	enum MatchingMode {
		SHIFT_VERT_ONLY,
		SHIFT_HORIZ_ONLY,
		SHIFT_BOTH_OR,
		SHIFT_BOTH_AND
	};
	Matcher();
    Matcher(MatchingMode matchingMode);

	double ComputeScore(const Encoding& pEncoding, const Encoding& tEncoding) const;
	const MatchingMode mode;
	const int hdScales;
	const static enum MatchingMode DEFAULT_MODE = SHIFT_BOTH_OR;
};


}

#endif /* VASIRMATCH_H_ */
