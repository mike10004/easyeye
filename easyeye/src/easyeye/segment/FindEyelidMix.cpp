#include <string.h>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <jsoncpp/json/json.h>
#include <portability.h>
#include "../common/easyeye_utils.h"
#include "../common/easyeye_imaging.h"
#include "../common/easyeye_serial.h"
#include "../common/easyeye_types.h"
#include "../common/mylog.h"
#include "easyeye_segment.h"
#include "FindEyelidMix.h"

using mylog::Logs;
using namespace cv;
using namespace std;
using namespace easyeye;

FindEyelidMix::FindEyelidMix(EyelidFinderConfig config)
    : config_(config)
{
    
}

void FindEyelidMix::doFindPoints(const cv::Mat& image, const BoundaryPair& bp, VasirEyelidsLocation& eyelids_location)
{
    int xPupil = bp.pupil.center.x, yPupil = bp.pupil.center.y, rPupil = bp.pupil.radius;
    int yIris = bp.iris.center.y, xIris = bp.iris.center.x, rIris = bp.iris.radius;
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

	cv::Point2i topPoint, bottomPoint;	
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
	cv::Point2i xyLeft = findContourPoint(leftImg, leftThres, LEFT, dataType);//1:LEFT
	cv::Point2i xyRight = findContourPoint(rightImg, rightThres, RIGHT, dataType);//2:RIGHT
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

static string ToString(vector< vector<cv::Point> >& contours, int min_contour_count) 
{
    ostringstream out;
    for (vector< vector<Point> >::iterator it = contours.begin(); 
            it != contours.end(); ++it) {
        vector<Point>& contour = *it;
        int count = contour.size();
        if (count >= min_contour_count) {
            vector<cv::Point2i> arrPoint(contour);
            for (size_t i = 0; i < contour.size(); i++) {
                arrPoint[i] = contour[i];
                out << contour[i].x << ' ' << contour[i].y << ' ';
            }
            out << endl;
        }
	}
    return out.str();
}

static string CreateDiagLabel(const string& prefix, FindEyelidMix::EyelidPosition locate) {
    string label(prefix);
    label.append(locate == FindEyelidMix::LEFT ? "left" : "right");
    return label;
}

cv::Point2i FindEyelidMix::findContourPoint(const Mat& grayMatImg, int threshold, EyelidPosition locate, int mode)
{
	cv::Point2i xyValue;	
	if(locate == LEFT) {
		xyValue.x = grayMatImg.cols;// setImg->width; // minimum
		xyValue.y = -1;
	} else if(locate == RIGHT) {
		xyValue.x = 0; // Minimum
		xyValue.y = -1;		
	}
		
    cv::Size ksize(config_.gauss_config.kernel_width, config_.gauss_config.kernel_height);
    double sigmaX = config_.gauss_config.sigma_x;
    cv::GaussianBlur(grayMatImg, grayMatImg, ksize, sigmaX);
    diagnostician()->WriteImage(grayMatImg, CreateDiagLabel("eyelidgaussed", locate));
	cv::threshold(grayMatImg, grayMatImg, threshold, 255, CV_THRESH_BINARY);
    diagnostician()->WriteImage(grayMatImg, CreateDiagLabel("eyelidthreshed", locate));
    int find_contours_method = CV_LINK_RUNS, find_contours_mode = CV_RETR_LIST;
    vector< vector<Point> > contours;
	cv::findContours(grayMatImg, contours, find_contours_mode, find_contours_method); //BEST  
    if (!diagnostician()->disabled()) {
        string label("eyelidcontours");
        label.append(locate == LEFT ? "left" : "right");
        string contours_str = ToString(contours, config_.min_contour_count);
        diagnostician()->WriteText(contours_str, label);
    }
	/// \todo what about scale matter?
	const int minContourCount = config_.min_contour_count;
    /**
     * Here we iterate through the contours to find the contour point. 
     * For each contour whose length is above a preset minimum, iterate
     * through the points. If this is the left eye, then the contour point
     * is the point with the minimum x-coordinate. If this is the right eye,
     * then the contour point is the point with the maximum x-coordinate.
     */
    for (vector< vector<Point> >::iterator it = contours.begin(); 
            it != contours.end(); ++it) {
        vector<Point>& contour = *it;
        int count = contour.size();
        if (count >= minContourCount) {
            const int v = 1;				
            for (int i = 0; i < count -1; i = i + v) {
                if (contour[i].x == 0 || contour[i].x == grayMatImg.cols - 1)  {
                    //cout << "mode=" << mode << " - x=" << arrPoint[i].x << " -- ignore" << endl;
                    continue;
                }
                if(locate == LEFT) {
                    if (contour[i].x < xyValue.x) {// Left
                        xyValue.x = contour[i].x;
                        xyValue.y = contour[i].y;						
                    }
                } else if (locate == RIGHT) {
                    if(contour[i].x > xyValue.x) {//Right
                        xyValue.x = contour[i].x;
                        xyValue.y = contour[i].y;
                    }	
                } else assert(false);
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
		if(locate == LEFT)
		{
			xyValue.x = 1;
//			cout << "Left ZERO Value" << endl;
            Logs::GetLogger().Log(mylog::DEBUG, "FindEyelidMix::findContourPoint Left ZERO Value\n");
		}
		if(locate == RIGHT)
		{			
			xyValue.x = grayMatImg.cols - 1;
//			cout << "Right ZERO Value" << endl;
            Logs::GetLogger().Log(mylog::DEBUG, "FindEyelidMix::findContourPoint Right ZERO Value\n");
		}
	}
	return xyValue;	
}



// Image without noise
Mat VasirEyelidsLocation::getNoiseImage(const Mat& eye_image) const
{
      const cv::Point2i center(center_x(), center_y());
      int width = ellipse_vals[2], 
              topHeight = ellipse_vals[3], 
              bottomHeight = ellipse_vals[4];
      double angle = angle;
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

cv::Mat VasirEyelidsLocation::CreateNoiseMask(const cv::Mat& image) const
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
    Mat noise_image = getNoiseImage(image);
	Logs::GetLogger().Log(mylog::DEBUG, "FindEyelidMix::doFindPoints ellipse = [%d %d %d %d %d], angle = %.4f\n",
			ellipse_vals[0], ellipse_vals[1], ellipse_vals[2], 
            ellipse_vals[3], ellipse_vals[4], angle);
	return noise_image;
    
}

VasirEyelidsLocation::~VasirEyelidsLocation() 
{
}

const char* VasirEyelidsLocation::kType = "vasir";

VasirEyelidsLocation::VasirEyelidsLocation()
{
    angle = portability::Math::GetNaN();
    for (int i = 0; i < NUM_ELLIPSE_VALS; i++) {
        ellipse_vals[i] = 0;
    }
}

int VasirEyelidsLocation::center_x() const{
    return ellipse_vals[0];
}

int VasirEyelidsLocation::center_y() const
{
    return ellipse_vals[1];
}

const double VasirEyelidsLocation::DEFAULT_MAX_ANGLE_DELTA = 0.1;

bool VasirEyelidsLocation::Equals(const EyelidsLocation& other) const
{
    if (!Strings::Equals(type(), other.type())) {
        return false;
    }
    return Equals(static_cast<const VasirEyelidsLocation&>(other), DEFAULT_MAX_ANGLE_DELTA);
}

bool VasirEyelidsLocation::Equals(const VasirEyelidsLocation& other_location, double max_angle_delta) const
{
    if (strcmp(type(), other_location.type()) != 0) {
        return false;
    }
    VasirEyelidsLocation const* other = static_cast<VasirEyelidsLocation const*>(&other_location);
    return (abs(angle - other->angle) <= max_angle_delta) 
            && (Arrays::CountNotEqual(ellipse_vals, other->ellipse_vals, NUM_ELLIPSE_VALS) == 0);
}

void VasirEyelidsLocation::Describe(ostream& out) const
{
    out << "EyelidsLocation{[" << ellipse_vals[0];
    for (int i = 1; i < NUM_ELLIPSE_VALS; i++) {
        out << ", " << ellipse_vals[i];
    }
    out << "], " << angle << "}";
}

string VasirEyelidsLocation::ToString() const
{
    ostringstream ss;
    Describe(ss);
    return ss.str();
}

bool VasirEyelidsLocation::Equals(const VasirEyelidsLocation& other, const int ellipse_deltas[], double max_angle_delta) const
{
    if (abs(angle - other.angle) > max_angle_delta) {
        return false;
    }
    for (int i = 0; i < NUM_ELLIPSE_VALS; i++) {
        if (abs(ellipse_vals[i] - other.ellipse_vals[i]) > ellipse_deltas[i]) {
            return false;
        }
    }
    return true;
}

bool VasirEyelidsLocation::Equals(const VasirEyelidsLocation& other, int ellipse_delta, double max_angle_delta) const
{
    int ellipse_deltas[NUM_ELLIPSE_VALS];
    for (int i = 0; i < NUM_ELLIPSE_VALS; i++) {
        ellipse_deltas[i] = ellipse_delta;
    }
    return Equals(other, ellipse_deltas, max_angle_delta);
}

bool VasirEyelidsLocation::EqualsApprox(const EyelidsLocation& other) const
{
    if (!Strings::Equals(type(), other.type())) {
        return false;
    }
    return Equals(static_cast<const VasirEyelidsLocation&>(other), APPROX_ELLIPSE_DELTA, DEFAULT_MAX_ANGLE_DELTA);
}


void serial::VasirEyelidsLocationAdapter::ToJson(void* src, Json::Value& dst)
{
    VasirEyelidsLocation& data = *((VasirEyelidsLocation*)src);
    Json::Value ellipse_vals(Json::arrayValue);
    for (int i = 0; i < VasirEyelidsLocation::NUM_ELLIPSE_VALS; i++) {
        ellipse_vals.append(data.ellipse_vals[i]);
    }
    dst["ellipse_vals"] = ellipse_vals;
    dst["angle"] = data.angle;
}

bool serial::VasirEyelidsLocationAdapter::FromJson(const Json::Value& src, void* dst)
{
    VasirEyelidsLocation& data = *((VasirEyelidsLocation*)dst);
    Json::Value nullValue;
    Json::Value ellipse_vals = src.get("ellipse_vals", nullValue);
    if (ellipse_vals.size() != VasirEyelidsLocation::NUM_ELLIPSE_VALS) {
        return false;
    }
    for (int i = 0; i < VasirEyelidsLocation::NUM_ELLIPSE_VALS; i++) {
        data.ellipse_vals[i] = ellipse_vals[i].asInt();
    }
    data.angle = src.get("angle", 0.0).asDouble();
    return src.isMember("angle") && src.isMember("ellipse_vals");
}

void serial::Serialize(const VasirEyelidsLocation& src, Json::Value& dst)
{
    VasirEyelidsLocationAdapter a;
    Serialize(src, &a, dst);
}

bool serial::Deserialize(const Json::Value& src, VasirEyelidsLocation& v)
{
    VasirEyelidsLocationAdapter a;
    return Deserialize(src, &a, v);
}

void VasirEyelidsLocation::Draw(cv::Mat& eye_image, const Scalar color) const
{
      const cv::Point2i center(center_x(), center_y());
      int width = ellipse_vals[2], 
              topHeight = ellipse_vals[3], 
              bottomHeight = ellipse_vals[4];
      double angle = angle;
      int rows = eye_image.rows, cols = eye_image.cols;
      int thickness = 3;
      cv::Size top_size(width, topHeight);
      cv::Size bottom_size(width, bottomHeight);
      cv::ellipse(eye_image, center, top_size,    angle, 0,   180, color, thickness, CV_AA, 0);
      cv::ellipse(eye_image, center, bottom_size, angle, 180, 360, color, thickness, CV_AA, 0);
}

const char* VasirEyelidsLocation::type() const
{
    return kType;
}