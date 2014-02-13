#include "../common/easyeye_imaging.h"
#include "FindPupilCircleNew.h"

#include "../common/easyeye_utils.h"
#include "easyeye_segment.h"
#include <vector>
#include <iostream>
#include <algorithm>
#include "../common/mylog.h"
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "../common/easyeye_diagnostics.h"
#include "../common/easyeye_config.h"

using std::vector;
using std::sort;
using cv::Point;
using cv::Point2i;
using cv::Point2f;
using cv::Mat;
using namespace easyeye;
using mylog::Logs;
//#define RATIO_R	13

FindPupilCircleNew::FindPupilCircleNew(PupilFinderConfig config) : config_(config)
{
}

static bool IsValidPupil(const IntCircle& pupil) {
    return pupil.radius > 0 && pupil.center.x > 0 && pupil.center.y > 0 ;
}

/**
 * Performs pupil detection.
 * @param destVal an array of length 6; the last 3 values are defaults to use
 * if pupil detection fails, but if the values are invalid, a best-guess
 * default detection -- the center of the image, with radius equal to half
 * the limit -- is used
 */
IntCircle FindPupilCircleNew::doDetect(const cv::Mat& eye_image,
        std::vector<cv::Point> contour_coordinates)
{	
    if (!Imaging::IsGray(eye_image)) {
        Logs::GetLogger().Log(mylog::WARN, "FindPupilCircleNew::doDetect image is not " 
                "grayscale; unpredictable behavior will ensue...\n");
    }
    const int limitRadius = config_.max_radius();
    const int nScale = config_.nScale;
    const IrisImageType dataType = config_.iris_image_type;
	Logs::GetLogger().Log(mylog::TRACE, "FindPupilCircleNew::doDetect: "
			"rLimit=%d nScale=%d dataType=%d\n",
			limitRadius, nScale, dataType);
	// Make a copy of the given image
//    Mat grayImgCopy;
//    img.copyTo(grayImgCopy);
//    IplImage actualGrayImg = grayImgCopy;
//	IplImage* grayImg = &actualGrayImg;

	// Setup the parameters to avoid noise caused by reflections and 
	// eyelashes covering the pupil
	float size=0.0;
	
	// Initialize for Closing and Opening process
	int closeItr = 0;//dilate->erode
	int openItr = 0;//erode->dilate
	if(dataType == easyeye::NIR_IRIS_STILL) { // Classic still images;
		size = RATIO_R-1;//	
		closeItr = 2;
		openItr = 3;
	} else if(dataType == easyeye::NIR_FACE_VIDEO) { // Distant video imagery
		// Still need to find an optimal value
		size = RATIO_R/2+2;
		closeItr = 0;
		openItr = 3;
	}	
	
	// Find the minimum intensity within the image - used to determine
	// the threshold
	double minValue, maxValue; 
	cv::Point minLoc, maxLoc; //location
	cv::minMaxLoc(eye_image, &minValue, &maxValue, &minLoc, &maxLoc);
	
	// Future work => consider to remove the reflections here
	
	// Get the threshold for detecting the pupil
	int threshold = getThreshold(eye_image, (int)minValue);

	Logs::GetLogger().Log(mylog::TRACE, "FindPupilCircleNew::doDetect size=%d, minValue=%.1f maxValue=%.1f threshold=%d\n",
				size, minValue, maxValue, threshold);
	//Originally, we used 1 for m value.
	//If you increas m value, it would be faster, however, the results would be slightly worst.
	const int m = 2;
    IntCircle dstVal(0, 0, 0);
    IntCircle candidate(0, 0, 0);
	// First attempt
	while(size > 2 && dstVal.radius < 1) {
		for(int i=threshold; i >= 0; i=i-m) {//orgin i--;
			getCoordinates(eye_image, closeItr, openItr, i, size, dstVal, candidate, contour_coordinates);
            if (IsValidPupil(dstVal)) {
                break;
            }
		}
		--size;
		++closeItr;	// Reduce unwanted noise	
	}

	// Second attempt
	size = RATIO_R-1;
	closeItr = 0;	
	while(dstVal.radius < 1 && size > 1) {
		openItr=openItr+3; // Count region
		for(int i=threshold+5; i <threshold+30 ; i=i+m) { //origin i=i++
			getCoordinates(eye_image, closeItr, openItr, i, size, dstVal, candidate, contour_coordinates);
            if (IsValidPupil(dstVal)) {
                break;
            }
		}
		--size;
	}

	// If both attempts failed=> need to be reimplemented
	if(dstVal.radius < 1) {
		Logs::GetLogger().Log(mylog::DEBUG, "FindPupilCircleNew::doDetect Failed to load the pupil circle; using defaults\n");
        dstVal.CopyFrom(candidate);
		if(candidate.radius < 1 || candidate.radius > limitRadius) {
		  dstVal.radius = limitRadius / 2;      
		}
		if(dstVal.center.x < 1) {
			Logs::GetLogger().Log(mylog::DEBUG, "FindPupilCircleNew::doDetect Failed to load the pupil's X center position; just guessing instead\n");
			dstVal.center.x = eye_image.cols / 2;
		}
		if(dstVal.center.y < 1) {
			Logs::GetLogger().Log(mylog::DEBUG, "FindPupilCircleNew::doDetect Failed to load the pupil's Y center position; just guessing instead\n");
			dstVal.center.y = eye_image.rows / 2;
		}
	}
	
	Logs::GetLogger().Log(mylog::DEBUG, "FindPupilCircleNew::doDetect: x=%d y=%d r=%d (%d %d %d)\n",
			dstVal.center.x, dstVal.center.y, dstVal.radius, 
			candidate.center.x, candidate.center.y, candidate.radius);
    return dstVal;
}

int FindPupilCircleNew::getThreshold(const Mat& image, int minVal)
{
	
	int n = image.rows * image.cols;
	// Mean
	float mean = Imaging::myMean(image);
	// Standard Deviation
	float sd = Imaging::mySD(image, n, mean);

	int threshold = 0;
	// Find the threshold for the pupil boundaries
	if(sd < 47)// Might be possible to optimize?
		threshold = minVal+25; // Usually poor quality->orgin 25
	else
		threshold = minVal+35; // Usually good quality->orgin 35
	
	return threshold;
}

/// \todo Code needs to be rearranged
void FindPupilCircleNew::getCoordinates(const Mat& eye_image, 
        int closeItr, int openItr, int threshold, 
        float size, IntCircle& primary, IntCircle& candidate, 
        vector<Point>& contour_coordinates)
{
    contour_coordinates.clear();
//    const int limitRadius = config_.max_radius();
    const int nScale = config_.nScale;
//    const IrisImageType dataType = config_.iris_image_type;
    Mat destImg = eye_image.clone();
    primary.set(0, 0, 0);
    candidate.set(0, 0, 0);
    cv::threshold(eye_image, destImg, threshold, 255, CV_THRESH_BINARY);
	Imaging::cvSmooth(destImg, destImg, CV_GAUSSIAN, 5, 5);
	// Start the morphological operators
    cv::Point anchor(1, 1);
    cv::Point defaultAnchor(-1, -1);
    cv::Size ksize(3, 3);
    int shape = CV_SHAPE_ELLIPSE;
    cv::Mat m_pSE = cv::getStructuringElement(shape, ksize, anchor);
	cv::morphologyEx(destImg, destImg, CV_MOP_CLOSE, m_pSE, defaultAnchor, closeItr);
	cv::morphologyEx(destImg, destImg, CV_MOP_OPEN, m_pSE, defaultAnchor, openItr);
	vector< vector<cv::Point> > contours;
	cv::findContours(destImg, contours, CV_RETR_LIST, CV_CHAIN_APPROX_TC89_L1);    
 			
	// Set up the min. contours to ignore noise within the binary image
	const int minCount = 16*nScale;//is it better to use nScale?
	
	// Find the first and second maximum contour
	int maxCount[2]={0};	
	getMaxCount(contours, maxCount);
		
	// Get the pupil center and radius
	int contour_index = getPupilPosition(contours, minCount, maxCount[0], size, primary, candidate);
    vector<Point>& winning_contour = contours[contour_index];
    Contours::AddAll(winning_contour, contour_coordinates);
}

int FindPupilCircleNew::getPupilPosition(vector< vector<cv::Point2i> >& contours, size_t minCount, 
										  size_t maxCount, float size, IntCircle& primary, IntCircle& candidate)
{	
    const int limitRadius = config_.max_radius();
//    const int nScale = config_.nScale;
//    const IrisImageType dataType = config_.iris_image_type;
	cv::Point2i center;
	int radius;
    int winner = -1;
    for (size_t i = 0; i < contours.size(); i++) {
        vector<Point2i>& contour = contours[i];
        winner = i;
		center.x = 0;
		center.y = 0;
		radius = 0;		
        size_t count = contour.size(); // contour->total;
        //search the maximum contour
        if (count > minCount && count == maxCount) {				
            cv::RotatedRect box = Contours::fitEllipse(contour);
            center.x = (int)box.center.x;
            center.y = (int)box.center.y;			
            int height = (int)box.size.height;
            int width = (int)box.size.width;
            Logs::GetLogger().Log(mylog::TRACE, "FindPupilCircleNew::getPupilPosition fit ellipse from %lu points: %.0fx%.0f with center (%.0f, %0.f) and angle %.0f degrees\n", 
                    contour.size(), box.size.width, box.size.height, box.center.x, box.center.y, box.angle);
            /// @todo We assume that the pupil is the perfect circle
            radius = getRadius(width, height, size);
            // Use below if the radius is bigger than the limitRadius
            candidate.center.x = center.x;//circles[3] = center.x;
            candidate.center.y = center.y;//circles[4]= center.y;
            candidate.radius = radius;//circles[5] = radius;			
            // Stop and draw the biggest circle		
            if(0 < radius && radius < limitRadius) {
                primary.CopyFrom(candidate);
                Logs::GetLogger().Log(mylog::TRACE, "FindPupilCircleNew::getPupilPosition VALID (%d, %d) r=%d\n", primary.center.x, primary.center.y, primary.radius);
                break;
            }
		}		
	}
    return winner;
}

int FindPupilCircleNew::getRadius(int width, int height, float size)
{
    const int limitRadius = config_.max_radius();
	int radius = 0;
	int longRadius=0, shortRadius=0;
	if(height > width)
	{
		longRadius = height;
		shortRadius = width;
	}
	else
	{
		longRadius = width;
		shortRadius = height;
	}

	if( !(shortRadius < (longRadius/RATIO_R)*size) )
		radius = cvRound(longRadius/2)+1;
	else
		radius = limitRadius + 1;

	return radius;
}

// Get the maximum contour count.
void FindPupilCircleNew::getMaxCount(vector< vector<cv::Point> >& contours, int* count)
{	
	std::vector<int> ptCount;
	//while(contour != NULL)
    for (vector< vector<cv::Point> >::iterator it = contours.begin(); it != contours.end(); ++it)
	{
        vector<cv::Point>& contour = *it;
		//if(CV_IS_SEQ_CURVE(contour))
        if (true)
		{
//			ptCount.push_back(contour->total);
            ptCount.push_back(contour.size());
		}
//		contour = contour->h_next;
	}
	sort(ptCount.begin(), ptCount.end());
	
	count[0]=0;
	count[1]=0;
	
	if(ptCount.size() >= 1)
	{
		count[0] = ptCount[ptCount.size()-1];//first maximum
	}
	if(ptCount.size() >= 2)
	{
		count[1] = ptCount[ptCount.size()-2];//second maximum
	}
}

