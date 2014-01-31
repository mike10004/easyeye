#include "FindEyelidMix.h"

#include "easyeye_segment.h"
#include "../common/mylog.h"
#include <iostream>
#include <string.h>
#include <opencv2/imgproc/imgproc.hpp>
#include "../common/easyeye_utils.h"
#include "../common/easyeye_imaging.h"
#include <algorithm>

using mylog::Logs;
using namespace cv;
using namespace std;
using namespace easyeye;

FindEyelidMix::FindEyelidMix(EyelidFinderConfig config)
    : config_(config)
{
    
}

void FindEyelidMix::doFindPoints(const cv::Mat& image, const BoundaryPair& bp, EyelidsLocation& eyelids_location)
{
    int xPupil = bp.pupilX, yPupil = bp.pupilY, rPupil = bp.pupilR;
    int yIris = bp.irisY, xIris = bp.irisX, rIris = bp.irisR;
    const IrisImageType dataType = config_.iris_image_type;
	Logs::GetLogger().Log(mylog::TRACE, "FindEyelidMix::doFindPoints image %d x %d, pupil (%d, %d) r = %d, iris (%d, %d) r = %d, dataType = %d\n",
			image.cols, image.rows, xPupil, yPupil, rPupil, xIris, yIris, rIris, dataType);
	
	// The ROI will be saved in this variable
	int destVal[4];	

	// ROI to search for the eyelid
    Imaging::myRect(image, xIris, yIris, rIris, destVal);
	int icl, icu, irl, iru;
	icl = destVal[0]; // X starting point
	icu = destVal[1]; // X ending point
	irl = destVal[2]; // Y starting point
	iru = destVal[3]; // Y ending point
	
	
	// Detect the top and bottom eyelid points
	int topHeight = Imaging::getValue(yPupil-irl, irl+yPupil);	
	int bottomHeight = Imaging::getValue(iru-(yPupil+rPupil), iru);	
	int centerX = Imaging::getValue(icl + (icu-icl)/2, image.cols);//eyeImage->hsize[1]);
    Mat top_eyelid = Imaging::GetROI(image, icl, icu-icl, irl, topHeight);
    Mat bottom_eyelid = Imaging::GetROI(image, icl, icu-icl,
			(int)(yPupil+rPupil+(bottomHeight*0.3)), (int)(bottomHeight*0.7));

	CvPoint topPoint, bottomPoint;	
	topPoint.x = centerX;	
	bottomPoint.x = centerX;

	// Calulate Y using Hough Transform
	topPoint.y = getEyelidPoint(top_eyelid, -1, irl+1, icl, MAX);
	bottomPoint.y = getEyelidPoint(bottom_eyelid, iru, iru-bottom_eyelid.rows-1, icl, MIN);
	
    // Debugging
	if(topPoint.y < irl || topPoint.y > yPupil-1)
		topPoint.y = irl;
	if(bottomPoint.y > iru)
		bottomPoint.y = iru; 

	// Detect left and right corner points
	int adj = rIris/4;
	int corRadius_X = cvRound(rIris*1.8)/2;
	int corRadius_Y = (iru-irl)/2;
	int corLeftCenter_X = Imaging::getValue(icl - (corRadius_X + adj), icl-1);
	int corRightCenter_X = Imaging::getValue(icu + (corRadius_X + adj), image.cols - 1);
	int corCenter_Y = Imaging::getValue(yIris+(rPupil/2), image.rows - 1);

	int leftDestVal[4], rightDestVal[4];
	Imaging::myXYRect(image, corLeftCenter_X, corCenter_Y, corRadius_X, corRadius_Y, leftDestVal);
	Imaging::myXYRect(image, corRightCenter_X, corCenter_Y, corRadius_X, corRadius_Y, rightDestVal);

	int leftStartX = leftDestVal[0];
	int leftEndX = leftDestVal[1];
	int leftStartY = leftDestVal[2];	
	int leftEndY = leftDestVal[3];

	int rightStartX = rightDestVal[0];
	int rightEndX = rightDestVal[1];
	int rightStartY = rightDestVal[2];	
	int rightEndY = rightDestVal[3];

	// ROI eyelid corners detection
	Mat leftImg = Imaging::GetROI(image, leftStartX, leftEndX-leftStartX, leftStartY, leftEndY-leftStartY);
	Mat rightImg = Imaging::GetROI(image, rightStartX, rightEndX-rightStartX, rightStartY, rightEndY-rightStartY);
    
	// Find the threshold to detect the eyelid corners
	int leftThres = doFindThres(leftImg, 100);
	int rightThres = doFindThres(rightImg, 100);

	// Determine contour points
	cv::Point2i xyLeft = findContourPoint(leftImg, leftThres, 1, dataType);//1:LEFT
	cv::Point2i xyRight = findContourPoint(rightImg, rightThres, 2, dataType);//2:RIGHT
	cv::Point2i leftPoint = getCornerPoint(xyLeft, leftStartX, leftStartY, leftEndY, dataType);
	cv::Point2i rightPoint= getCornerPoint(xyRight, rightStartX, rightStartY, rightEndY, dataType);
	
	// Get the coordinates in the original image
	int xx1, yy1, xx2, yy2;
	xx1 = abs(rightPoint.x - leftPoint.x);
	yy1 = abs(leftPoint.y - rightPoint.y);
		
	double centerLine = triLength(xx1, yy1) ;
	double val2 = xx1/centerLine;

	cv::Point2i bottomLine;
	bottomLine.x = rightPoint.x;
	bottomLine.y = leftPoint.y;

	// Calculate the angle for rotating the image
	double angle = acos(val2)*(180/PI);
	if(rightPoint.y > leftPoint.y)
		angle = -(angle);

	// M of BottomLine
	const cv::Point2i center((int)(leftPoint.x +rightPoint.x)/2, (int)(leftPoint.y +rightPoint.y)/2);
	// Left, top line (lLine)	
	xx2 = abs(topPoint.x - leftPoint.x);
	yy2 = abs(leftPoint.y - topPoint.y);
	double leftTopLine = triLength(xx2, yy2);

	eyelids_location.ellipse_vals[0] = center.x;
	eyelids_location.ellipse_vals[1] = center.y;
	eyelids_location.ellipse_vals[2] = (int)(centerLine/2);
	eyelids_location.ellipse_vals[3] = abs(center.y-topPoint.y);
	eyelids_location.ellipse_vals[4] = abs(bottomPoint.y-center.y);
	eyelids_location.angle = angle;

//==================================================
}

// Find the top and bottom eyelid points
int FindEyelidMix::getEyelidPoint(const Mat& image, int yla, int val, 
								int icl, Extremum extremum)
{
    int hsize0 = image.rows, hsize1 = image.cols;
    int lineCount;
    double *lines;
    int *xl, *yl;
    int destVal;
    Masek masek;

    destVal = yla;

    // Find the top eyelid
    if (hsize0>0 && hsize1>0)		
	    lineCount = masek.findline(image.data, image.rows, image.cols, &lines);	
    else 
	    lineCount = 0;

    if (lineCount > 0)
    {
	    xl = (int*)malloc(sizeof(int)*hsize1);
	    yl = (int*)malloc(sizeof(int)*hsize1);

	    masek.linescoords(lines, hsize0, hsize1, xl, yl);

	    for (int i = 0; i<hsize1; i++)
	    {			
		    yl[i] = yl[i]+val;			
		    xl[i] = xl[i]+icl-1;
			destVal = extremum == MIN ? min(destVal, yl[i]) : max(destVal, yl[i]);
	    }
	    free (xl);
	    free (yl);
	    free(lines);
    }
    return destVal;
}

// Find the left and right corner points
cv::Point2i FindEyelidMix::getCornerPoint(cv::Point2i contourPt, int startX, 
									int startY, int endY, int dataType)
{
    cv::Point2i pt;
    pt.x = startX + contourPt.x;
    pt.y = startY + contourPt.y;   
   
	// Limit the rotation
    if(dataType == easyeye::NIR_IRIS_STILL)// classical still images
    {
		const int val = 10;
		if(pt.y < startY+val || pt.y > endY-val )
		    pt.y = startY+abs(endY-startY)/2;
    }
    else if(dataType == easyeye::NIR_FACE_VIDEO) // distant video imagery
    {
		if(pt.y <= startY || pt.y >= endY )
		    pt.y = startY+abs(endY-startY)/2; 
    }
    return pt;
}


double FindEyelidMix::triLength(int a, int b)
{
    return sqrt((double)(a*a + b*b));
}

// Determine threshold value used to detect the eyelid corners
int FindEyelidMix::doFindThres(const Mat& img, int thresRange)
{
	int mean = cvRound(Imaging::myMean(img));
    int threshold = 0;
	  if(mean!= 0)
		  threshold= mean-32; // classical still images 
    if(mean < thresRange)
	    threshold = mean-10; // distant video imagery	 
    
	  return threshold;
}

cv::Point2i FindEyelidMix::findContourPoint(const Mat& grayMatImg, int threshold, int locate, int mode)
{
	cv::Point2i xyValue;	
	if(locate == 1) // Left
	{
		xyValue.x = grayMatImg.cols;// setImg->width; // minimum
		xyValue.y = -1;
	}
	else if(locate == 2) // Right
	{
		xyValue.x = 0; // Minimum
		xyValue.y = -1;		
	}
		
	//cvSmooth(grayImg,grayImg, CV_GAUSSIAN, 31, 15);//best
    int size1 = 31, size2 = 15;
    cv::Size ksize(size1, size2);
    // cvSmooth's default sigma is calculated based on kernel size 
    // http://docs.opencv.org/trunk/modules/imgproc/doc/filtering.html?highlight=smooth#smooth
    double sigmaX = 0.3 * (size1/2 - 1) + 0.8; 
    cv::GaussianBlur(grayMatImg, grayMatImg, ksize, sigmaX);
	cv::threshold(grayMatImg, grayMatImg, threshold, 255, CV_THRESH_BINARY);
    int find_contours_method = CV_LINK_RUNS, find_contours_mode = CV_RETR_LIST;
    vector< vector<Point> > contours;
	cv::findContours(grayMatImg, contours, find_contours_mode, find_contours_method); //BEST  

	/// \todo what about scale matter?
	const int minContourCount = 70;
    
    for (vector< vector<Point> >::iterator it = contours.begin(); 
            it != contours.end(); ++it) {
        vector<Point>& contour = *it;
        int count = contour.size();
        if (count >= minContourCount) {
            vector<cv::Point2i> arrPoint(contour);
            for (size_t i = 0; i < contour.size(); i++) {
                arrPoint[i] = contour[i];
            }
            const int v = 1;				
            for (int i = 0; i < count -1; i = i + v) {
                if (arrPoint[i].x ==0 || arrPoint[i].x == grayMatImg.cols - 1)  {
                    //cout << "mode=" << mode << " - x=" << arrPoint[i].x << " -- ignore" << endl;
                    continue;
                }
                if(arrPoint[i].x < xyValue.x && locate == 1) {// Left
                    xyValue.x = arrPoint[i].x;
                    xyValue.y = arrPoint[i].y;						
                }
                if(arrPoint[i].x > xyValue.x && locate == 2) {//Right
                    xyValue.x = arrPoint[i].x;
                    xyValue.y = arrPoint[i].y;
                }	

            }
        }
	}

	if(xyValue.y < 2 || xyValue.y > grayMatImg.rows - 1)
	{
		xyValue.y = cvRound(grayMatImg.rows * 0.6);
//		cout << "Top or Bottom ZERO Value" << endl;
        Logs::GetLogger().Log(mylog::DEBUG, "FindEyelidMix::findContourPoint Top or Bottom ZERO Value\n");
	}
  	
	if(xyValue.x < 2 || xyValue.x > grayMatImg.cols - 1)
	{
		if(locate == 1)
		{
			xyValue.x = 1;
//			cout << "Left ZERO Value" << endl;
            Logs::GetLogger().Log(mylog::DEBUG, "FindEyelidMix::findContourPoint Left ZERO Value\n");
		}
		if(locate == 2)
		{			
			xyValue.x = grayMatImg.cols - 1;
//			cout << "Right ZERO Value" << endl;
            Logs::GetLogger().Log(mylog::DEBUG, "FindEyelidMix::findContourPoint Right ZERO Value\n");
		}
	}
	return xyValue;	
}



// Image without noise
Mat FindEyelidMix::getNoiseImage(const Mat& eye_image, const EyelidsLocation& eyelids_location)
{
      const cv::Point2i center(eyelids_location.center_x(), eyelids_location.center_y());
      int width = eyelids_location.ellipse_vals[2], 
              topHeight = eyelids_location.ellipse_vals[3], 
              bottomHeight = eyelids_location.ellipse_vals[4];
      double angle = eyelids_location.angle;
      int rows = eye_image.rows, cols = eye_image.cols;
	  // Create the mask for normalization
//	  IplImage* maskImg = NULL;
//	  maskImg = cvCreateImage(cvSize(img->width, img->height), 8, 1);
//	  for(int j = 0; j < maskImg->height; j++) {
//		  for(int i = 0; i < maskImg->width; i++) {
//			  maskImg->imageData[i + j * maskImg->widthStep] = (char) 1;
//		  }
//	  }
	  /* Draw the elliptical arcs representing the eyelids into the mask image with value 0 */
      Scalar ellipse_color = CV_RGB(0, 0, 0);
      Size top_size(width, topHeight);
      Size bottom_size(width, bottomHeight);
      Mat mask_image(eye_image.rows, eye_image.cols, CV_8UC1, CV_RGB(255, 255, 255));
      cv::ellipse(mask_image, center, top_size, angle, 0, 180, ellipse_color, CV_FILLED, CV_AA, 0);
      cv::ellipse(mask_image, center, bottom_size, angle, 180, 360, ellipse_color, CV_FILLED, CV_AA, 0);
  	
	  // Paint negative of mask onto eye image
      Mat noise_image = eye_image.clone();//(rows, cols, CV_8UC1);
	  for(int j = 0; j < rows; j++) {
          Mat mask_row = mask_image.row(j);
          Mat noise_image_row = noise_image.row(j);
		  for(int i = 0; i < cols; i++) {
			  if(mask_row.at<uchar>(i) != 0) {
                  noise_image_row.at<uchar>(i) = 0;
			  }
		  }
	  }
      return noise_image;
}

cv::Mat FindEyelidMix::CreateNoiseImage(const cv::Mat& image, const EyelidsLocation& eyelids_location)
{
// Two methods
//==================================================
	//1. Rotate image and draw circle
	//IplImage* rotatedImg = NULL;
	//rotatedImg = getRotatedImage(iplImg, center,ellipseVal[2],ellipseVal[3], ellipseVal[4], angle);
	//ImageUtility::showImage("RotatedImg", rotatedImg);
	//cvReleaseImage(&rotatedImg);

	//2. Without rotating image => rotate image after segmenting the iris region
	//Mark noise parts in image
    Mat noise_image = getNoiseImage(image, eyelids_location);
	Logs::GetLogger().Log(mylog::DEBUG, "FindEyelidMix::doFindPoints ellipse = [%d %d %d %d %d], angle = %.4f\n",
			eyelids_location.ellipse_vals[0], eyelids_location.ellipse_vals[1], eyelids_location.ellipse_vals[2], eyelids_location.ellipse_vals[3], eyelids_location.ellipse_vals[4], eyelids_location.angle);
	return noise_image;
    
}