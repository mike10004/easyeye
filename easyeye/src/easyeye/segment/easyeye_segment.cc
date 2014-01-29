/*
 * ezisegment.cc
 *
 *  Created on: Jul 11, 2013
 *      Author: mchaberski
 */


#include "easyeye_imaging.h"
#include "easyeye_segment.h"
#include "ImageUtility.h"
#include "FindPupilCircleNew.h"
#include "FindIrisCircle.h"
#include "FindEyelidMix.h"
#include "easyeye_extrema_noise.h"
#include <Masek.h>
#include "../common/mylog.h"
#include "../common/base64.h"
#include <iostream>
#include <string>
#include <json/json_object.h>
#include <json/json_tokener.h>

using mylog::Logs;
using mylog::TRACE;
using namespace std;
using namespace easyeye;
using namespace cv;

Segmentation::Segmentation() 
    : status(Result::NOT_YET_SET), boundary_pair(), eyelids_location(), extrema_noise()
{
}

Segmentation::~Segmentation()
{
}

void Segmentation::Describe(std::ostream& out) const
{
    out << Result::DescribeStatus(status) << ' ';
    boundary_pair.Describe(out);
}

/*
 * TODO define Segmentation copy constructor and copy assignment operator
 * No client code uses these yet, so we can hold off.
 */

Segmenter::Segmenter() : config_()
{
}

void fill_array(int array[], const int value, const int start, const int len)
{
	for (int i = start; i < (start+len); i++) {
		array[i] = value;
	}
}

void Segmenter::SegmentEyeImage(cv::Mat& eyeImg, Segmentation& seg)
{
    if (!Imaging::IsGray(eyeImg)) {
        Logs::GetLogger().Log(mylog::ERROR, "Segmenter::SegmentEyeImage input image must be grayscale, not type %d", eyeImg.type()); 
        seg.status = Result::FAILURE;
        return;
    }
	int bothnScale = 1;
	// Scale if the image size is larger than w x h // [MC:] shouldn't this be OR instead of AND?
    int imageWidth = eyeImg.cols, imageHeight = eyeImg.rows;
	if(imageWidth > EYE_IMAGE_WIDTH_SCALE_THRESHOLD
			&& imageHeight > EYE_IMAGE_HEIGHT_SCALE_THRESHOLD) {
		bothnScale = 2;
    }
    if (config_.pupil_finder_config.nScale != CircleFinderConfig::NSCALE_AUTO
            || config_.iris_finder_config.nScale != CircleFinderConfig::NSCALE_AUTO) {
        Logs::GetLogger().Log(mylog::ERROR, "Segmenter::SegmentEyeImage configuration " 
                "with non-auto nscale for circle finders is not yet supported");
        seg.status = Result::FAILURE;
        return;
    }
    config_.pupil_finder_config.nScale = bothnScale;
    config_.iris_finder_config.nScale = bothnScale;

	FindPupilCircleNew pupil_finder(config_.pupil_finder_config);
    pupil_finder.set_diagnostician(diagnostician_);
    IntCircle pupilCircle = pupil_finder.doDetect(eyeImg);
	BoundaryPair& bpair = seg.boundary_pair;
    bpair.set_pupil(pupilCircle);
	// Set-up ROI for detecting the iris circle
    const int rIrisMax = config_.iris_finder_config.max_radius();
	ImageUtility::SETVALUE setVal = ImageUtility::setImage(eyeImg,
			pupilCircle.center, bpair.pupilR, rIrisMax, rIrisMax);	//82 is the best for video images, previous 80
    Mat setImg = Imaging::GetROI(eyeImg, setVal.rect.x, setVal.rect.width,
			setVal.rect.y, setVal.rect.height);
	
	/// \todo Possible to optimize?
	// Define maximum distance between pupil and iris center position
//    int centerAdjust= config_.iris_finder_config.max_pupil_center_offset();//int)(rIrisMax/4);

	// Find iris circle using Hough Transform
	FindIrisCircle iris_finder(config_.iris_finder_config);
    IntCircle irisCircle = iris_finder.doDetect(setImg, bpair.pupilR);
    bpair.set_iris(irisCircle);
	CvPoint xyIrisIn;
	xyIrisIn.x = bpair.irisX;
	xyIrisIn.y = bpair.irisY;
	cv::Point2i xyIris = iris_finder.getOriginPoints(pupilCircle.center, xyIrisIn, setVal.p);
	bpair.irisX = xyIris.x;
	bpair.irisY = xyIris.y;
    
	// Find the upper and lower eyelid(s)
	FindEyelidMix eyelid_finder(config_.eyelid_finder_config);
	eyelid_finder.doFindPoints(eyeImg, bpair, seg.eyelids_location);
    Mat eyelidImg = eyelid_finder.CreateNoiseImage(eyeImg, seg.eyelids_location);
    diagnostician()->WriteImage(eyelidImg, "eyelidImg");
    ExtremaNoiseFinder extrema_noise_finder(config_.extrema_noise_finder_config);
	extrema_noise_finder.FindExtremaNoise(eyeImg).copyTo(seg.extrema_noise);
    diagnostician()->DumpSegOutput(seg.boundary_pair, seg.eyelids_location, seg.extrema_noise);
	seg.status = Result::SUCCESS;
}

SegmenterConfig::SegmenterConfig()
      : Config(), 
        iris_finder_config(), 
        pupil_finder_config(),
        eyelid_finder_config(), 
        extrema_noise_finder_config()
{
    
}

Contours::Contours()
{
}

cv::RotatedRect Contours::fitEllipse(std::vector<cv::Point2i>& points)
{
    return fitEllipse(points.begin(), points.end());
}

cv::RotatedRect Contours::fitEllipse(std::vector<cv::Point2i>::iterator points_begin, std::vector<cv::Point2i>::iterator points_end)
{
    vector<cv::Point2f> fpoints;
    for (vector<cv::Point2i>::iterator it = points_begin; it != points_end; ++it) {
        Point2f d((*it).x, (*it).y);
        fpoints.push_back(d);
    }
    return cv::fitEllipse(fpoints);
}

bool serial::SegmentationAdapter::FromJson(const Json::Value& src, void* dst)
{
    Segmentation& seg = *((Segmentation*)dst);
    seg.status = (Segmentation::Status) src.get("status", Result::FAILURE).asInt();
    if (!Deserialize(src["boundary_pair"], seg.boundary_pair)) {
        return false;
    }
    SparseMatAdapter sma;
    if (!Deserialize(src["extrema_noise"], &sma, seg.extrema_noise)) {
        return false;
    }
    if (!Deserialize(src["eyelids_location"], seg.eyelids_location)) {
        return false;
    }
    return src.isMember("status") 
            && src.isMember("boundary_pair") 
            && src.isMember("extrema_noise") 
            && src.isMember("eyelids_location");
}

void serial::SegmentationAdapter::ToJson(void* src, Json::Value& dst) 
{
    Segmentation& seg = *((Segmentation*)src);
    dst["status"] = seg.status;
    Json::Value boundary_pair;
    Serialize(seg.boundary_pair, boundary_pair);
    dst["boundary_pair"] = boundary_pair;
    SparseMatAdapter sma;
    Json::Value extrema_noise;
    Serialize(seg.extrema_noise, &sma, extrema_noise);
    dst["extrema_noise"] = extrema_noise;
    Json::Value eyelids_location;
    Serialize(seg.eyelids_location, eyelids_location);
    dst["eyelids_location"] = eyelids_location;
}

string serial::Serialize(const Segmentation& data)
{
    SegmentationAdapter adapter;
    return Serialize(data, &adapter);
}

bool serial::Deserialize(const std::string& json, Segmentation& data)
{
    SegmentationAdapter adapter;
    return Deserialize(json, &adapter, data);
}