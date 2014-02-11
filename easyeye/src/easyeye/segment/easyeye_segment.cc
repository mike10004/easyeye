/*
 * easyeye_segment.cc
 *
 *  Created on: Jul 11, 2013
 *      Author: mchaberski
 */


#include "../common/easyeye_imaging.h"
#include "easyeye_segment.h"

#include "FindPupilCircleNew.h"
#include "FindIrisCircle.h"
#include "FindEyelidMix.h"
#include "easyeye_extrema_noise.h"
#include <Masek.h>
#include "../common/mylog.h"
#include "../common/base64.h"
#include "easyeye_eyelid_detection.h"
#include <iostream>
#include <string>

using mylog::Logs;
using mylog::TRACE;
using namespace std;
using namespace easyeye;
using namespace cv;

Segmentation::Segmentation() 
    : status(Result::NOT_YET_SET), boundary_pair(), eyelids_location_(NULL), extrema_noise()
{
}

Segmentation::~Segmentation()
{
    if (eyelids_location_ != NULL) {
        delete eyelids_location_;
    }
}
//Array(const Array& other)
//    : size(other.size), data(new int[other.size]) 
//{
//    std::copy(other.data, other.data + other.size, data); 
//}

Segmentation::Segmentation(const Segmentation& other)
    : status(other.status), boundary_pair(other.boundary_pair), eyelids_location_(NULL), extrema_noise()
{
    other.extrema_noise.copyTo(extrema_noise);
    if (other.IsEyelidsLocationPresent()) {
        if (Strings::Equals(other.eyelids_location().mask_creation_method(), VasirEyelidsLocation::kType)) {
            VasirEyelidsLocation* vel = new VasirEyelidsLocation(static_cast<VasirEyelidsLocation&>(*(other.eyelids_location_)));
            set_eyelids_location(vel);
        } else {
            mylog::Logs::GetLogger().Log(mylog::ERROR, "copying eyelids location of type %s is not yet supported\n", other.eyelids_location().mask_creation_method());
        }
    }
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
    IntCircle pupilCircle = pupil_finder.doDetect(eyeImg, seg.pupil_boundary);
	BoundaryPair& bpair = seg.boundary_pair;
    bpair.set_pupil(pupilCircle);
	// Set-up ROI for detecting the iris circle
    const int rIrisMax = config_.iris_finder_config.max_radius();
    Imaging::EyeImageROI eye_image_roi = Imaging::GetEyeImageROI(eyeImg,
			pupilCircle.center, bpair.pupil.radius, rIrisMax, rIrisMax);	//82 is the best for video images, previous 80
    Mat setImg = Imaging::GetROI(eyeImg, eye_image_roi.rect.x, eye_image_roi.rect.width,
			eye_image_roi.rect.y, eye_image_roi.rect.height);
	
	/// \todo Possible to optimize?
	// Define maximum distance between pupil and iris center position
//    int centerAdjust= config_.iris_finder_config.max_pupil_center_offset();//int)(rIrisMax/4);

	// Find iris circle using Hough Transform
	FindIrisCircle iris_finder(config_.iris_finder_config);
    iris_finder.set_diagnostician(diagnostician_);
    IntCircle irisCircle = iris_finder.doDetect(setImg, bpair.pupil.radius);
    bpair.set_iris(irisCircle);
	cv::Point2i xyIrisIn;
	xyIrisIn.x = bpair.iris.center.x;
	xyIrisIn.y = bpair.iris.center.y;
	cv::Point2i xyIris = iris_finder.getOriginPoints(pupilCircle.center, xyIrisIn, eye_image_roi.p);
	bpair.iris.center.x = xyIris.x;
	bpair.iris.center.y = xyIris.y;
    
	// Find the upper and lower eyelid(s)
	FindEyelidMix eyelid_finder(config_.eyelid_finder_config);
    eyelid_finder.set_diagnostician(diagnostician_);
    if (config_.eyelid_finder_config.method == EyelidFinderConfig::METHOD_ELLIPSE_CONTOUR) {
        VasirEyelidsLocation* eyelids_location = new VasirEyelidsLocation();
        seg.set_eyelids_location(eyelids_location);
        eyelid_finder.doFindPoints(eyeImg, bpair, *eyelids_location);
    } else {
        mylog::Logs::GetLogger().Log(mylog::ERROR, "Segmenter::SegmentEyeImage "
                "alternate eyelid detection methods not yet supported\n");
    }
    ExtremaNoiseFinder extrema_noise_finder(config_.extrema_noise_finder_config);
	extrema_noise_finder.FindExtremaNoise(eyeImg).copyTo(seg.extrema_noise);
    diagnostician()->DumpSegOutput(seg.boundary_pair, seg.eyelids_location(), seg.extrema_noise);
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
    string eyelids_location_type(VasirEyelidsLocation::kType);
    if (src.isMember("eyelids_location_type")) {
        eyelids_location_type.assign(src["eyelids_location_type"].asString());
    }
    if (eyelids_location_type.compare(VasirEyelidsLocation::kType) == 0) {
        VasirEyelidsLocation* eyelids_location = new VasirEyelidsLocation();
        seg.set_eyelids_location(eyelids_location);
        if (!Deserialize(src["eyelids_location"], *eyelids_location)) {
            return false;
        }        
    } else if (eyelids_location_type.compare(DualParabolaEyelidsLocation::kType) == 0) {
        mylog::Logs::GetLogger().Log(mylog::ERROR, "serial::SegmentationAdapter::FromJson deserialization of eyelid location type %s is not yet supported\n", eyelids_location_type.c_str());
        return false;
    } else {
        mylog::Logs::GetLogger().Log(mylog::ERROR, "serial::SegmentationAdapter::FromJson deserialization of eyelid location type %s is not yet supported\n", eyelids_location_type.c_str());
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
    string eyelids_location_type(seg.eyelids_location().mask_creation_method());
    dst["eyelids_location_type"] = eyelids_location_type;
    if (Strings::Equals(eyelids_location_type, VasirEyelidsLocation::kType)) {
        const VasirEyelidsLocation& src_eyelids_location = static_cast<const VasirEyelidsLocation&>(seg.eyelids_location());
        Serialize(src_eyelids_location, eyelids_location);
    } else {
        mylog::Logs::GetLogger().Log(mylog::ERROR, "serial::SegmentationAdapter::ToJson serialization of eyelids location type %s is not yet supported\n", eyelids_location_type.c_str());
    }

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

void Contours::AddAll(std::vector<Point>& from, std::vector<Point>& to)
{
    for (vector<Point>::iterator it = from.begin(); it != from.end(); ++it)
    {
        to.push_back(*it);
    }
}

void Segmentation::set_eyelids_location(EyelidsLocation* eyelids_location)
{
    if (eyelids_location_ != NULL) {
        delete eyelids_location_;
    }
    eyelids_location_ = eyelids_location;
}

const EyelidsLocation& Segmentation::eyelids_location() const
{
    // This will segfault if the eyelids haven't been set.
    // TODO: add API for checking whether eyelids have been set
    return *eyelids_location_; 
}

bool Segmentation::IsEyelidsLocationPresent() const
{
    return eyelids_location_ != NULL;
}

