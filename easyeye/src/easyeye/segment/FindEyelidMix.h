/*********************************************************//**
** @file FindEyelidMix .h
** Find the eyelid curves using Hough Transform and contour points.
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

#ifndef FINDEYELIDMIX_H
#define FINDEYELIDMIX_H

#include "../common/easyeye_types.h"
#include <opencv2/core/core.hpp>
#include "Masek.h"
#include "../common/easyeye_diagnostics.h"
#include "../common/easyeye_config.h"

namespace easyeye
{

/**
 * Eyelids curve detection class.
 */
class FindEyelidMix : public DiagnosticsCreator
{
public:
	FindEyelidMix(EyelidFinderConfig config);
	//virtual ~FindEyelidMix();
   /**
	* Detects eyelid top/botton curves and left/right corners using Hough 
	* Transform and Contour points.
	*
	* @param iplImg Input image
	* @param xPupil Pupil center X
	* @param yPupil Pupil center Y
	* @param rPupil Pupil's radius
	* @param xIris Iris center X
	* @param yIris Iris center Y
	* @param rIris Iris' radius
	* @param ellipseVal Return values in which to store the eyelid information (array of length 5)
	* @param angleVal Return values in which to store the eyelid orientation (array of length 1)
	* @param dataType Valid \c dataType values:
	*               - \c 1 = classic still image
	*               - \c 2 = video captured at a distance (distant-videos)
	* @return Noise image
	*/
    void doFindPoints(const cv::Mat& image, const BoundaryPair& bp, EyelidsLocation& eyelids_location);
    static cv::Mat CreateNoiseImage(const cv::Mat& image, const EyelidsLocation& eyelids_location);
    const static int ANGLE_DEST_VAL_LEN = 1;
	/// FUTURE WORK.
	//Masek::IMAGE* removeReflections(IplImage* eyeImg, IplImage* noiseImg, int eyelashThres, int reflectThres);
    enum EyelidPosition { LEFT = 1, RIGHT = 2 };
    
private:
	const EyelidFinderConfig config_;

    
   /**
	* Detects eyelid curves using Hough Transform.
	*
	* @param setImg Input image
	* @param threshold Threshold value
	* @param locate Valid \c locate values:
	*               - \c 1 = Left
	*               - \c 2 = Right
	* @param dataType Valid \c dataType values:
	*               - \c 1 = classic still image
	*               - \c 2 = video captured at a distance (distant-videos)
	* @return Contour point
	*/
	cv::Point2i findContourPoint(const cv::Mat& setImg, int threshold, EyelidPosition locate, 
									int dataType);
	
    /**
	* Determines the threshold value to detect the eyelid corners.
	*
	* @param setImg Input image
	* @param thresRange Range of possible threshold values
	* @return Threshold value 
	*/	
	int doFindThres(const cv::Mat& img, int thresRange);
	
	/**
	* Calculates the eyelid's corner points.
	*
	* @param contourPt The detected contour points
	* @param startX X starting point
	* @param startY Y starting point
	* @param endX   X ending point
	* @param endY   Y ending point
	* @param dataType Valid \c dataType values:
	*               - \c 1 = classic still image
	*               - \c 2 = video captured at a distance (distant-videos)
	* @return Corner points 
	*/
	cv::Point2i getCornerPoint(cv::Point2i contourPt, int startX, int startY, 
								int endY , int dataType);

	enum Extremum { MIN, MAX };
	
	/**
	* Finds the top and bottom eyelid points.
	*
	* @param image Input Image (IMAGE* type)
	* @param yla Default value for Y
	* @param val Minimum/Maximum for Y
	* @param icl Minimum for X
	* @param opFunc Valid \c opFunc functions:
	*               - \c Max (Original value, New value)
	*               - \c Min (Original value, New value)
	* @return Top and bottom eyelid's Y value
	*/
	int getEyelidPoint(const cv::Mat& image, int yla, int val, int icl, Extremum extremum);
	/**
	* Square root
	*
	* @param a 
	* @param b
	* @return sqrt(a*a, b*b)
	*/
	double triLength(int a, int b);

	/**
	* Marks noise with zeros in an image.
	*
	* @param img Input Image
	* @param center Center points of eyelid ellipse
	* @param width Ellipse's width
	* @param topHeight Upper ellipse's height
	* @param bottomHeight Lower ellipse's height
	* @param angle Ellipse's orientation
	* @retrun Image with noise mark
	*/
	static cv::Mat getNoiseImage(const cv::Mat& eye_image, const EyelidsLocation& eyelids_location);
    
};

}

#endif // !FINDEYELIDMIX_H
