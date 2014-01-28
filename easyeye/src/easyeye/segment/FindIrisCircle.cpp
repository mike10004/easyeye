#include "FindIrisCircle.h"
#include <Masek.h>
#include "ImageUtility.h"
#include "../common/mylog.h"
#include <opencv2/highgui/highgui.hpp>
#include "easyeye_imaging.h"

using namespace easyeye;
using cv::Mat;

FindIrisCircle::FindIrisCircle(IrisFinderConfig config)
    :config_(config)
{
    
}

FindIrisCircle::~FindIrisCircle()
{
    
}

IntCircle FindIrisCircle::doDetect(Mat& gray_image, int rPupil)
{
    int uIrisRadius = config_.max_radius();//limitRadius;
	mylog::Log(mylog::TRACE, "FindIrisCircle::doDetect %dx%d rPupil=%d uIrisRadius=%d\n", rPupil, uIrisRadius);
    IntCircle destVal(0, 0, 0);
//	for (int i = 0; i < DEST_VAL_LEN; i++) destVal[i] = 0;
	// Scaling factor to speed up the Hough transform
	const double scaling = 0.4;
	// Minimum radius of a potential iris
	int lIrisRadius=0;		
	// Values returned by the Hough Transform
	int rowIris, colIris, rIris;
	Masek masek;	
	
	// Use the pupil's radius as a minimum for the potential Iris' circle radius
	float ratio_size= (float)rPupil/(float)uIrisRadius;
	lIrisRadius = (int)(rPupil+uIrisRadius/(ratio_size*12));	
	if(ratio_size >= 0.22) {
		for(float i=0.22f; i<0.5f; i=i+0.02f) {
			if(ratio_size < i) {
				lIrisRadius = (int)(rPupil+uIrisRadius/(i*10));//25
				break;
			}
		}
	}
	
	/*lIrisRadius = uIrisRadius/2;
	float ratio_size= (float)rPupil/(float)img->height;
	for(float i=0.05; i<0.4; i=i+0.02)
	{
		if(ratio_size < i)
		{
			lIrisRadius = rPupil+uIrisRadius/(i*25);//20
			break;
		}
	}*/
	
	// Development purposes
	if(lIrisRadius > uIrisRadius)
	{
		mylog::Log(mylog::DEBUG, "FindIrisCircle::doDetect error lIrisRadius > uIrisRadius: %d\n", lIrisRadius);
		lIrisRadius = uIrisRadius;//then store the max radius
	}

	// Convert the IplImage to IMAGE
	Masek::IMAGE * eyeImg = Imaging::CopyToMasek(gray_image);	
	
	double lowThres = 0.11;
	double hiThres = 0.19;

	/// \todo Possible to optimize?
	if(config_.iris_image_type == NIR_IRIS_STILL)
		lowThres= 0.18;
  
	// Find the iris boundaries for both video and still images
	masek.findcircle(eyeImg, lIrisRadius, uIrisRadius, scaling, 2,
			hiThres, lowThres, 1.00, 0.00, &rowIris, &colIris, &rIris);
	free(eyeImg->data);
	free(eyeImg);

	// Set the return values of the iris circle
	destVal.center.x = colIris;//[0] = colIris;//x
	destVal.center.y = rowIris;//[1] = rowIris;//y
	destVal.radius = rIris;//[2] = rIris;//r
	
	// \todo Is this debugging information really necessary?
	if(destVal.center.x < 1 || destVal.center.y < 1) {
		mylog::Log(mylog::DEBUG, "FindIrisCircle::doDetect failed to detect iris center coordinates; got (%d, %d)\n", destVal.center.x, destVal.center.y);
		if (destVal.center.x < 1) destVal.center.x = gray_image.cols / 2;
		if (destVal.center.y < 1) destVal.center.y = gray_image.rows / 2;
	}
	if(destVal.radius < 1 || destVal.radius > uIrisRadius) {
		mylog::Log(mylog::DEBUG, "FindIrisCircle::doDetect failed to detect iris radius\n");
		destVal.radius = uIrisRadius;
	}
	mylog::Log(mylog::DEBUG, "FindIrisCircle::doDetect x=%d y=%d r=%d\n", destVal.center.x, destVal.center.y, destVal.radius);
    return destVal;
}

cv::Point2i FindIrisCircle::getOriginPoints(const cv::Point2i xyPupil, const cv::Point2i xyIrisIn, const cv::Point2i setPt)
{
    int val = config_.max_pupil_center_offset();
    IrisImageType dataType = config_.iris_image_type;
	int oldIrisX = xyIrisIn.x, oldIrisY = xyIrisIn.y;
	// Calculate the difference between pupil center and iris center
	int diffX, diffY;	
	diffX = xyIrisIn.x - setPt.x;
	diffY = xyIrisIn.y - setPt.y;	
 
    // Limit iris center distance from pupil center points based on the given argument
	if(diffX > val || diffX < -val)
		diffX = 0;
	if(diffY > val || diffY < -val)
	    diffY = 0;
	  	
	// Find the iris center within the original image
    cv::Point2i xyIrisOut;
	xyIrisOut.x = xyPupil.x + diffX;
	xyIrisOut.y = xyPupil.y + diffY;
	mylog::Log(mylog::TRACE, "FindIrisCircle::getOriginPoints "
			"pupil (%d, %d) inIris (%d, %d) setPt (%d, %d) "
			"val = %d, dataType = %d, outIris (%d, %d)\n",
			xyPupil.x, xyPupil.y, oldIrisX, oldIrisY, setPt.x, setPt.y,
			val, dataType, xyIrisOut.x, xyIrisOut.y);

    return xyIrisOut;
}
