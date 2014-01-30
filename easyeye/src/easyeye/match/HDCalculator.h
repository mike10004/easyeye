/*********************************************************//**
** @file GetHammingDistance.h
** Methods for calculating the Hamming Distance (HD).
**
** @date 12/2010
** @author: Added and Modified by Yooyoung Lee
**         - Libor Masek (masekl01@csse.uwa.edu.au) - Matlab code
**         - Xiaomei Liu (xliu5@cse.nd.edu) - Translated to C code
**          
** @note Additions and Modifications to existing source code.
**
** Note: Please send BUG reports to yooyoung@<NOSPAM>nist.gov. 
** For more information, refer to: https://cspot-run2.nist.gov/itl/iad/ig/vasir.cfm
**
** @par Disclaimer 
** This software was developed at the National Institute of Standards 
** and Technology (NIST) by employees of the Federal Government in the
** course of their official duties. Pursuant to Title 17 Section 105 
** of the United States Code, this software is not subject to copyright 
** protection and is in the public domain. NIST assumes no responsibility 
** whatsoever for use by other parties of its source code or open source 
** server, and makes no guarantees, expressed or implied, about its quality, 
** reliability, or any other characteristic.
**************************************************************/

#ifndef _HD_CALCULATOR_H_
#define _HD_CALCULATOR_H_

#include "../encode/easyeye_encode.h"
#include "easyeye_match.h"

namespace easyeye
{
/**
 * Class for calculating the Hamming Distance between two iris templates.
 *
 * The class incorporates noise masks, i.e. noise bits are not used when 
 * calculating the HD.
 */
class HDCalculator
{
 public:
	HDCalculator(const Encoding& e1, const Encoding& e2);
	virtual ~HDCalculator();
    MatchInfo::Flag flag() const;
    static const double HD_NAN;
    
	/**
	* Calculates HD shifting left and right and then returning the minimum 
	* HD value.
	*
	* @param classTemplate1 Input target template
	* @param classTemplate2 Input query template
	* @param scales  Number of filters used for encoding - needed to
    *                 determine how many bits should be moved when shifting.
	* @return HD Minimum
	*/
	void computeHDX(int nscales, MatchInfo& match_info);
	/**
	* Calculate HD in shifting towards the up and down
	* and return minimum HD value.
	*
	* @param classTemplate1 Input target template
	* @param classTemplate2 Input query template
	* @param scales  Number of filters used for encoding - needed to
    *                 determine how many bits should be moved when shifting.
	* @return HD Minimum
	*/
	void computeHDY(int nscales, MatchInfo& match_info); 

	/**
	* Calculate HD in shifting towards left/right or up/down
	* and return minimum HD value.
	*
	* @param classTemplate1 Input target template
	* @param classTemplate2 Input query template
	* @param scales  Number of filters used for encoding - needed to
    *                 determine how many bits should be moved when shifting.
	* @return HD minimum
	*/
	void computeHDXorY(int nscales, MatchInfo& match_info); 

	/**
	* Calculate HD in shifting towards left/right and up/down
	* and return minimum HD value.
	*
	* @param classTemplate1 Input target template
	* @param classTemplate2 Input query template
	* @param scales  Number of filters used for encoding - needed to
    *                 determine how many bits should be moved when shifting.
	* @return HD minimum
	*/
	void computeHDXandY(int nscales, MatchInfo& match_info); 
	
 private:
    MatchInfo::Flag flag_;
	int maxShiftX;
	int maxShiftY;
  
    const Encoding& e1_;
    const Encoding& e2_;
    const int width_, height_;
    Encoding shifted_e2_;

    static void calcHD(const Encoding& e1, const Encoding& e2, MatchInfo& match_info);
}; // HDCalculator

}
#endif // !_HD_CALCULATOR_H_
