/*********************************************************//**
** @file FindHighLights.h
** Static methods for eyelash and reflection detection.
**
** @date 12/2010
** @author Yooyoung Lee 
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

#ifndef FIND_HIGHLIGHTS_H
#define FIND_HIGHLIGHTS_H

#include <opencv2/core/core.hpp>
//#include "highgui.h"
//#include "cxcore.h"
//#include "cvaux.h"
#include "Masek.h"
#include "../common/easyeye_config.h"


namespace easyeye
{
/**
 * Eyelash and reflection detection class.
 */
class ExtremaNoiseFinder
{
public:
    ExtremaNoiseFinder(ExtremaNoiseFinderConfig config);
	/**
	* Removes eyelashes and reflections on the iris region.
	*
	* @param eyeImg Input Image
	* @return points probably containing eyelashes or reflections
	*/
    
  cv::SparseMat FindExtremaNoise(cv::Mat& eyeImg);
  bool IsExtreme(unsigned char value);
private:
    ExtremaNoiseFinderConfig config_;
};
}
#endif // !FIND_HIGHLIGHTS_H 
