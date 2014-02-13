#include <vector>
#include <string>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "easyeye_eyelid_detection.h"
#include "../common/easyeye_diagnostics.h"
#include "../common/easyeye_types.h"
#include <cassert>

using easyeye::DiagnosticArt;
using easyeye::EyelidBoundary;
using easyeye::EyelidsLocation;
using easyeye::DualParabolaEyelidFinder;
using easyeye::DualParabolaEyelidsLocation;
using easyeye::Diagnostician;
using easyeye::VertexFormParabola;
using easyeye::BoundaryPair;
using easyeye::EyelidFinderConfig;
using easyeye::Config;
using cvmore::objdetect::MaskInterface;
using cvmore::objdetect::HoughTransform;
using cvmore::objdetect::ShapeInterface;
using cvmore::objdetect::RotatedShape;
using cvmore::objdetect::ParamRange;
using cv::Mat;
using cv::Size;
using cv::Point2d;
using cv::Point2i;
using cv::Rect;
using std::string;
using std::vector;
using cv::Range;
using cv::Scalar;
using cv::Vec3b;

DualParabolaEyelidFinder::DualParabolaEyelidFinder(const EyelidFinderConfig& config)
        : config_(config)
{
}

DualParabolaEyelidFinder::~DualParabolaEyelidFinder()
{
}

namespace
{

/**
 * 
 * @param finder the eyelid finder whose diagnostician is to be used
 * @param eye_image CV_8UC1 image of an eye
 * @param mask CV_8UC1 image of same dimensions as eye; nonzero pixel value
 * means pixel is searchworthy, zero pixel value means unsearchworthy
 * @param eyelid_type the eyelid type (left or right)
 */
void DrawMaskDiagnostics(DualParabolaEyelidFinder& finder, const Mat& eye_image, 
        const Mat& mask, DualParabolaEyelidFinder::EyelidType eyelid_type)
{
    cv::Vec4b blue(0xff, 0x0, 0x0, 0x80);
    DiagnosticArt::MaskColoring mask_color(blue);
    Mat mask_on_image = DiagnosticArt::Compose(eye_image, mask, mask_color);
    const char* label = eyelid_type == DualParabolaEyelidFinder::EYELID_LOWER 
            ? "eyelidmasklower" : "eyelidmaskupper";
    finder.diagnostician()->WriteImage(mask_on_image, label);
}

void DrawCannied(DualParabolaEyelidFinder& finder, const Mat& detected_edges, const Mat& mask)
{
    Mat dst;
    dst = Scalar::all(0);
    detected_edges.copyTo(dst);
    cv::Vec4b blue(0xff, 0x0, 0xff, 0x00);
    DiagnosticArt::MaskColoring mask_color(blue);
    Mat mask_on_canny = DiagnosticArt::Compose(dst, mask, mask_color);
    finder.diagnostician()->WriteImage(mask_on_canny, "eyelidcanny");
}
}

EyelidBoundary DualParabolaEyelidFinder::MaskAndDetect(const Mat& eye_image, 
        const BoundaryPair& boundary_pair, EyelidType eyelid_type)
{
    Mat mask = MakeRegionMask(eye_image, boundary_pair, eyelid_type);
    DrawMaskDiagnostics(*this, eye_image, mask, eyelid_type);
    EyelidBoundary eyelid_boundary = DetectEyelidBoundary(eye_image, mask, eyelid_type);
    return eyelid_boundary;
}

DualParabolaEyelidsLocation DualParabolaEyelidFinder::FindEyelids(const cv::Mat& eye_image, 
        const BoundaryPair& boundary_pair)
{
    DualParabolaEyelidsLocation eyelids_location;
    eyelids_location.upper = MaskAndDetect(eye_image, boundary_pair, EYELID_UPPER);
    eyelids_location.lower = MaskAndDetect(eye_image, boundary_pair, EYELID_LOWER);
    return eyelids_location;
}

Mat DualParabolaEyelidFinder::MakeRegionMask(const cv::Mat& eye_image, 
        const BoundaryPair& boundary_pair, EyelidType eyelid_type)
{
    const IntCircle& iris = boundary_pair.iris;
    int iris_height = iris.radius * 2;
    int iris_width = iris.radius * 2;
    Range iris_rows(iris.center.y - iris.radius, iris.center.y + iris.radius);
    int lateral_extension = config_.lateral_search_region_iris_width_proportion * iris_width;
    int lateral_min = std::max(0, iris.center.x - iris.radius - lateral_extension);
    int lateral_max = std::min(eye_image.cols, iris.center.x + iris.radius + lateral_extension);
    Range search_region_cols(lateral_min, lateral_max);
    Range search_region_rows(0, eye_image.rows);
    if (eyelid_type == EYELID_UPPER) { 
        double from_iris_top_relative = config_.upper_search_region_iris_height_proportion;
        int from_iris_top_pixels = (int) round(from_iris_top_relative * iris_height);
        search_region_rows.start = std::max(0, iris_rows.start);
        search_region_rows.end = std::min(eye_image.rows, iris_rows.start + from_iris_top_pixels);
    } else { assert(eyelid_type == EYELID_LOWER);
        double from_iris_bottom_relative = config_.lower_search_region_iris_height_proportion;
        int from_iris_bottom_pixels = (int) round(from_iris_bottom_relative * iris_height);
        search_region_rows.start = std::max(0, iris_rows.end - from_iris_bottom_pixels);
        search_region_rows.end = std::min(eye_image.rows, iris_rows.end);
    }
    Size search_region_size(search_region_cols.end - search_region_cols.start, 
            search_region_rows.end - search_region_rows.start);
    Point2i search_region_origin(search_region_cols.start, search_region_rows.start);
    Rect search_region(search_region_origin, search_region_size);
    Scalar zero(0x0);
    Scalar full(0xff);
    Mat mask(eye_image.rows, eye_image.cols, CV_8UC1, zero);
    cv::rectangle(mask, search_region, full, CV_FILLED);
    return mask;
}

EyelidBoundary DualParabolaEyelidFinder::DetectEyelidBoundary(const cv::Mat& eye_image, 
        const cv::Mat& region_mask, EyelidType eyelid_type)
{
    int lowThreshold = 35;
    int ratio = 3;
    int kernel_size = 3;
    Mat detected_edges;
    cv::blur(eye_image, detected_edges, Size(3,3));
    cv::Canny(detected_edges, detected_edges, lowThreshold, lowThreshold*ratio, kernel_size);    
    DrawCannied(*this, detected_edges, region_mask);
    EyelidBoundary eyelid_boundary;
    return eyelid_boundary;
}

cv::Mat DualParabolaEyelidsLocation::CreateNoiseMask(const cv::Mat& eye_image) const
{
    assert(false);
}

void DualParabolaEyelidsLocation::Describe(std::ostream& out) const
{
    assert(false);
}

void DualParabolaEyelidsLocation::Draw(cv::Mat& diagnostic_image, cv::Scalar color) const
{
    assert(false);
}

bool DualParabolaEyelidsLocation::Equals(const EyelidsLocation& other) const
{
    return false;
}

bool DualParabolaEyelidsLocation::EqualsApprox(const EyelidsLocation& other) const
{
    return false;
}

std::string DualParabolaEyelidsLocation::ToString() const
{
    assert(false);
}

const char* DualParabolaEyelidsLocation::type() const
{
    return kType;
}

VertexFormParabola::VertexFormParabola()
    : a(0), h(0), k(0)
{
    
}

EyelidBoundary::EyelidBoundary()
    : shape(), present(false)
{
    
}

DualParabolaEyelidsLocation::DualParabolaEyelidsLocation()
    : upper(), lower()
{
    
}


