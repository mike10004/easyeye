/*********************************************************//**
** @file ImageUtility.h
** Utility functions to process an image.
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

#ifndef IMAGEUTILITY_H
#define IMAGEUTILITY_H

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <Masek.h>

// Valid data types
//#define NIR_IRIS_STILL 1 /// Classical still: Near-infrared iris still image.
//#define NIR_FACE_VIDEO 2 /// Distant video: Near-infrared face video image.

// Future work
//#define NIR_IRIS_VIDEO 3 /// Near-infrared iris video image.

/** Number of gray levels. */
#define GRAY_LEVEL 256

/**
 * Image processing utility class.
 */
class ImageUtility 
{
public: 
	/**
	 * Region of Interest and center points information
	 */
	typedef struct 
	{
		cv::Rect rect; /// ROI.
		cv::Point2i p; /// Center point.
	} SETVALUE;

	/** 
	 * Calculates the mean of an image.
	 * 
	 * @param img Input image
	 * @param n Total number of pixels
	 * @return Mean value
	 */
    static float myMean(const cv::Mat& img);
    
	/** 
	 * Calculates the standard deviation of an image.
	 * 
	 * @param img Input image
	 * @param n Total number of pixels
	 * @param mean Previously calculated mean value
	 * @return Standard deviation value
	 * @see myMean
	 */
	static float mySD(const cv::Mat& img, double n, float mean);

	/** 
	 * Extracts a rectangular part out of an image.
	 *
	 * @note The returned image needs to be deallocated
	 *
	 * @param img input image.
	 * @param rect Rectangle info
	 * @param x Starting x
	 * @param y Starting y
	 * @param Wd width
	 * @param Ht height
	 * @return Extracted image.
	 */
    static cv::Mat extractImagePart(const cv::Mat& img, cv::Rect& rect, int x, int y, int wd, int ht);
	
	
	/** 
	 * Return the ROI of an image as IplImage.
	 *
	 * @param eyeImg Input image.
	 * @param startX X starting point
	 * @param endX X end point
	 * @param startY Y starting point
	 * @param endX X end point
	 * @return Returns Ipl image of ROI
	 * @see getROIImage_C
	 */
//    static cv::Mat getROIImage(cv::Mat& eyeImg, int startX, int endX, int startY, int endY);
	
	/** 
	 * Returns an ROI of an image as IMAGE.
	 *
	 * @param eyeImg Input image.
	 * @param startX X starting point
	 * @param endX X end point
	 * @param startY Y starting point
	 * @param endY Y end point
	 * @return Returns IMAGE type image of ROI
	 * @see getROIImage
	 */
	static Masek::IMAGE* getROIImage_C(Masek::IMAGE* eyeImg, int startX, int endX, int startY, int endY);
	

	/** 
	 * Calculate the square rectangular info of an IMAGE type image.
	 *
	 * @param img Input image.
	 * @param x Input x
	 * @param y Input y
	 * @param radius Input radius
	 * @param destVal Used to return the rect info (0:left, 1:right, 2:bottom, 3:top)
	 */
	static void myRect_C(Masek::IMAGE *lidImg, int x, int y, int radius, int* destVal);
	
	/** 
	 * Debug values.
	 *
	 * @param value Actual value.
	 * @param maxSize Max width or height
	 * @return Proper value
	 */
	static int getValue(int value, int maxSize);	
    

	/** 
	 * Calculates the rectangular info and center point.
	 *
	 * @param eyeImg Input image.
	 * @param center Input center point Rectangle info
	 * @param cr Input radius
	 * @param xLimit Max. distance of X from the center
	 * @param yLimit Max. distance of Y from the center
	 * @return Rect info and center point
	 * See also SETVALUE
	 */
	static SETVALUE setImage(const cv::Mat& eye_image, CvPoint center, int cr, int xLimit, int yLimit);
	
	/** 
	 * Calculate the square rectangular info.
	 *
	 * @param img Input image.
	 * @param x Input x
	 * @param y Input y
	 * @param radius Input radius
	 * @param destVal Used to return the rect info (0:left, 1:right, 2:bottom, 3:top)
	 */
	static void myRect(IplImage* img, int x, int y, int radius, int* destVal);	
	
	/** 
	 * Calculates the rectangular info with different X and Y radius.
	 *
	 * @param img Input image.
	 * @param x Input x
	 * @param y Input y
	 * @param width Input X radius
	 * @param height Input Y radius
	 * @param destVal Used to return the rect info (0:left, 1:right, 2:bottom, 3:top)
	 */
	static void myXYRect(const cv::Mat& image, int x, int y, int width, int height, int* destVal);
    
	static IplImage* getROIImage(IplImage* eyeImg, int startX, int endX, int startY, int endY);
	static IplImage* extractImagePart(IplImage* img, CvRect& rect, int x, int y, int wd, int ht);
	static float myMean(IplImage* img, double n);
	static float mySD(IplImage* img, double n, float mean);

};

#endif // !IMAGEUTILITY_H

