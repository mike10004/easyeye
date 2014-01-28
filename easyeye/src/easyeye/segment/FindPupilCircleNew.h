/*********************************************************//**
** @file FindPupilCircle.h
** Static methods for pupil boundary detection.
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

#ifndef FINDPUPILCIRCLENEW_H
#define FINDPUPILCIRCLENEW_H

#include "../common/easyeye_config.h"
#include "../common/easyeye_diagnostics.h"
#include <opencv2/core/core.hpp>

namespace easyeye
{

/**
 * Pupil boundary detection class.
 */
class FindPupilCircleNew : public DiagnosticsCreator
{
public:
    FindPupilCircleNew(PupilFinderConfig config);
	/**
	* Detects pupil boundaries using contours.
	*
	* @param img Input image
	* @param limitRadius Maximum radius of a potential pupil circle
	* @param destVal Return values -store the pupil's center point and radius
	* @param nScale valid \c scales values:
	*               - \c 1 = if \c img is smaller than 600x440px
	*               - \c 2 = if \c img is greater than 600x440px
	* @param datatype Valid \c dataType values:
	*               - \c 1 = classic still image
	*               - \c 2 = video captured at a distance (distant-videos)
	*/
	IntCircle doDetect(cv::Mat& img);	
	const static int DEST_VAL_LEN = 6;
private:
    PupilFinderConfig config_;
	const static int DEST_VAL_OUTPUT_LEN = 3;
    const static int RATIO_R = 13;
	/**
	* Determines the threshold using standard deviation.
	*
	* @param img Input image
	* @param minVal Minimum intensity within an image
	* @return Threshold
	*/
	int getThreshold(cv::Mat& img, int minVal);

	/**
	* Find the pupil's location from thresholding image
	*
	* @param grayImg Input image
	* @param closeItr Number of iteration for closing
	* @param openItr Number of iteration for opening
	* @param threshold 
	* @param limitRadius Maximum radius of a potential pupil circle 
	* @param circles Returns the pupil center points and radius
    *     (Biggest circle-> 0:x, 1:y, 2:radius, Secondary circle->3:x, 4:y, 5:radius,)
	* @param size Select the greater length bewteen width and height under the size condition
	* @param nScale valid \c scales values:
	*               - \c 1 = if \c img is smaller than 600x440px
	*               - \c 2 = if \c img is greater than 600x440px
	* @param dataType Valid \c dataType values:
	*               - \c 1 = classic still image
	*               - \c 2 = video captured at a distance (distant-videos)
	*/
	void getCoordinates(cv::Mat& grayImg, int closeItr, int openItr,
		int threshold, float size, IntCircle& primary, IntCircle& candidate);
	
	/**
	* Returns the radius of a pupil circle.
	*
	* @param width Image width
	* @param height Image height
    * @param limitRadius Maximum radius of a potential pupil circle
	* @param size Select the greater length bewteen width and height under the size condition
	* @return Circle radius
	*/
	int getRadius(int width, int height, float size);
	
	/**
	* Determines the pupil center-points and radius.
	*
	* @param contour Sequences of points defining a curve within an image
	* @param minCount Minimum contours
	* @param maxCount Maximum contours
	* @param limitRadius Maximum radius of pupil circle
	* @param size Select the greater length bewteen width and height under the size condition
	* @param circles Returns the pupil center points and radius
    *     (Biggest circle-> 0:x, 1:y, 2:radius, Secondary circle->3:x, 4:y, 5:radius,)
    */
	void getPupilPosition(std::vector< std::vector<cv::Point> >& contours, 
            int minCount, int maxCount, float size, IntCircle& primary, IntCircle& candidate);

	/**
	* Determines the maximum contour count.
	*
	* @param contour contour Sequences of points defining a curve within an image
	* @param count Returns the maximum and second maximum contour point
	*/
	void getMaxCount(CvSeq* contour, int* count);
	void getMaxCount(std::vector< std::vector<cv::Point> >& contours, int* count);
	/// Future work.
	//static int fit_circle_radius(CvPoint* arrPt, int crx, int cry, int crar, int biggest_crar);

};

}
#endif // !FINDPUPILCIRCLENEW_H
