/*
 * vasirmatch.h
 *
 *  Created on: Jul 12, 2013
 *      Author: mchaberski
 */

#ifndef VASIRMATCH_H_
#define VASIRMATCH_H_

#include "../common/mylog.h"
#include "../encode/easyeye_encode.h"

namespace easyeye
{

class MatchInfo {
public:
    enum Flag {
        CLEAN = 0, DIRTY = 100, INCONGRUENT = 200, 
    };
    MatchInfo();
    Flag flag;
    int template_width, template_height;
    int num_unmasked_bits;
    int num_different_bits;
    int common_bits_differential;
    int x_shift, y_shift;
    double hd;
    void Reset();
    void CopyFrom(const MatchInfo& other);
private:
    static const Flag START_FLAG = CLEAN;
    static const int START_TEMPLATE_WIDTH = 0;
    static const int START_TEMPLATE_HEIGHT = 0;
    static const int START_NUM_UNMASKED_BITS = 0;
    static const int START_NUM_DIFFERENT_BITS = 0;
    static const int START_COMMON_BITS_DIFFERENTIAL = 0;
    static const int START_Y_SHIFT = 0;
    static const int START_X_SHIFT = 0;
    static const double START_HD;
};
    
    
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
	double ComputeScore(const Encoding& pEncoding, const Encoding& tEncoding, MatchInfo& match_info) const;
	const MatchingMode mode;
	const int hdScales;
private:
    mylog::Logger& logger_;
    MatchInfo ignored_match_info_;
	const static enum MatchingMode DEFAULT_MODE = SHIFT_BOTH_OR;
    
};


}

#endif /* VASIRMATCH_H_ */
