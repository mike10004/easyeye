/*********************************************************//**
** @file FindIrisCircle.h
** Find the iris circle using Hough Transform.
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

#ifndef FINDIRISCIRCLE_H
#define FINDIRISCIRCLE_H

#include <opencv2/core/core.hpp>
#include "../common/easyeye_config.h"
#include "../common/easyeye_diagnostics.h"

namespace easyeye
{
/**
 * Iris boundaries detection class.
 */
class FindIrisCircle : public DiagnosticsCreator
{
public:
    FindIrisCircle(IrisFinderConfig iris_finder_config);
    virtual ~FindIrisCircle();
  /**
	* Detects the iris boundary using Hough Transform.
	*
	* @param img Input image
	* @param rPupil  Radius of the pupil circle
	* @param uIrisRadius Maximum radius
	* @param destVal Returns the iris center points and radius
	*     (0:x, 1:y, 2:radius)
	*/
	IntCircle doDetect(cv::Mat& img, int rPupil);
	const static int DEST_VAL_LEN = 3;
  /**
	* Finds the iris center points. 
	* The distance between pupil and iris center points is constained.
	*
	* @param xyPupil Pupil's center
	* @param xyIris  Iris's center
	* @param setPt Points out of the ROI (Region Of Interest)
	* @param val Maximum distance between pupil and iris center position
	*/
	cv::Point2i getOriginPoints(const cv::Point2i xyPupil, const cv::Point2i xyIris, 
            const cv::Point2i setPt); 
private:
    IrisFinderConfig config_;
};

}
#endif // !FINDIRISCIRCLE_H

