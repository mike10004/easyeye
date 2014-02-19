#include <vector>
#include <sstream>
#include <string>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "easyeye_eyelid_detection.h"
#include "../common/easyeye_diagnostics.h"
#include "../common/easyeye_imaging.h"
#include "../common/easyeye_types.h"
#include "src/easyeye/common/mylog.h"
#include "easyeye_segment.h"
#include "../common/mylog.h"
#include <cassert>

using easyeye::Imaging;
using easyeye::DiagnosticArt;
using easyeye::EyelidBoundary;
using easyeye::EyelidsLocation;
using easyeye::DualParabolaEyelidFinder;
using easyeye::DualParabolaEyelidsLocation;
using easyeye::Diagnostician;
using easyeye::VertexFormParabola;
using cvmore::objdetect::VertexFormParabolaShape;
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
using cv::Point2f;
using cv::Point2d;
using cv::Point2i;
using cv::Rect;
using std::string;
using std::vector;
using std::ostringstream;
using cv::Range;
using cv::Scalar;
using cv::Vec3b;
using cv::Vec4b;
using mylog::Logs;
using mylog::DEBUG;

DualParabolaEyelidFinder::DualParabolaEyelidFinder(const EyelidFinderConfig& config, 
        EyelidType eyelid_type_)
        : config_(config), eyelid_type(eyelid_type_)
{
    assert(eyelid_type == EYELID_UPPER || eyelid_type == EYELID_LOWER);
}

DualParabolaEyelidFinder::~DualParabolaEyelidFinder()
{
}

/**
 * 
 * @param finder the eyelid finder whose diagnostician is to be used
 * @param eye_image CV_8UC1 image of an eye
 * @param mask CV_8UC1 image of same dimensions as eye; nonzero pixel value
 * means pixel is searchworthy, zero pixel value means unsearchworthy
 * @param eyelid_type the eyelid type (left or right)
 */
void DualParabolaEyelidFinder::DrawMaskDiagnostics(const Mat& eye_image, 
        const Mat& mask, const Rect& vertex_range)
{
    cv::Vec4b blue(0xff, 0x0, 0x0, 0x80);
    DiagnosticArt::MaskColoring mask_color(blue);
    Mat mask_on_image = DiagnosticArt::Compose(eye_image, mask, mask_color);
    const char* label = eyelid_type == DualParabolaEyelidFinder::EYELID_LOWER 
            ? "eyelidmasklower" : "eyelidmaskupper";
    diagnostician()->WriteImage(mask_on_image, label);
}

void DualParabolaEyelidFinder::DrawCannyDiagnostic(const Mat& eye_image, const Mat& detected_edges, const Mat& mask, const Rect& vertex_range)
{
    Vec4b red_opaque(0x0, 0x0, 0xff, 0xff);
    Mat detected_edges_as_mask;
    cv::bitwise_not(detected_edges, detected_edges_as_mask);
    Mat eye_image_with_edges = DiagnosticArt::Compose(eye_image, detected_edges_as_mask, red_opaque);

    Vec4b blue_half_trans(0xff, 0x0, 0x0, 0x80);
    DiagnosticArt::MaskColoring mask_color(blue_half_trans);
    Mat mask_on_edges_on_eye = DiagnosticArt::Compose(eye_image_with_edges, mask, mask_color);
    Scalar yellow(0xff, 0xff, 0x0);
    int range_border_thickness = 3;
    cv::rectangle(mask_on_edges_on_eye, vertex_range, yellow, range_border_thickness);
    const char* label = eyelid_type == DualParabolaEyelidFinder::EYELID_LOWER 
            ? "eyelidcannywithmasklower" : "eyelidcannywithmaskupper";
    diagnostician()->WriteImage(mask_on_edges_on_eye, label);
}

void DualParabolaEyelidFinder::DrawBoundaryDiagnostic(const cv::Mat& eye_image, const VertexFormParabola& parabola)
{
    VertexFormParabolaShape parabola_shape;
    RotatedShape rotated_parabola_shape(parabola_shape, VertexFormParabolaShape::kMaxIndex + 1);
    
}

EyelidBoundary DualParabolaEyelidFinder::FindEyelid(const cv::Mat& eye_image, const Segmentation& segmentation)
{
    assert(eyelid_type == EYELID_UPPER || eyelid_type == EYELID_LOWER);
    Mat mask;
    Rect vertex_range;
    MakeRegionMask(eye_image, segmentation, mask, vertex_range);
    DrawMaskDiagnostics(eye_image, mask, vertex_range);
    EyelidBoundary eyelid_boundary = DetectBoundary(eye_image, segmentation, 
            mask, vertex_range);
    return eyelid_boundary;
}

Rect DualParabolaEyelidFinder::CreateSearchRegion(const cv::Mat& eye_image, const Segmentation& segmentation)
{
    const IntCircle& iris = segmentation.boundary_pair.iris;
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
    } else { 
        assert(eyelid_type == EYELID_LOWER);
        double from_iris_bottom_relative = config_.lower_search_region_iris_height_proportion;
        int from_iris_bottom_pixels = (int) round(from_iris_bottom_relative * iris_height);
        search_region_rows.start = std::max(0, iris_rows.end - from_iris_bottom_pixels);
        search_region_rows.end = std::min(eye_image.rows, iris_rows.end);
    }
    Size search_region_size(search_region_cols.size(), search_region_rows.size());
    Point2i search_region_origin(search_region_cols.start, search_region_rows.start);
    Rect search_region(search_region_origin, search_region_size);
    return search_region;
}

void DualParabolaEyelidFinder::MakeRegionMask(const cv::Mat& eye_image, 
        const Segmentation& segmentation, cv::Mat& mask, cv::Rect& vertex_range)
{
    assert(eyelid_type == EYELID_UPPER || eyelid_type == EYELID_LOWER);
    Rect search_region = CreateSearchRegion(eye_image, segmentation);
    Scalar zero = Scalar::all(0x0);
    Scalar full = Scalar::all(0xff);
    mask.create(eye_image.rows, eye_image.cols, CV_8UC1);
    mask = zero;
    cv::rectangle(mask, search_region, full, CV_FILLED);
    int thickness = 16; 
    const vector<Point2i>& pupil_boundary = segmentation.pupil_boundary;
    if (!pupil_boundary.empty()) {
        for (size_t i = 1; i < pupil_boundary.size(); i++) {
            cv::line(mask, pupil_boundary[i-1], pupil_boundary[i], zero, thickness);
        }
    }
    Point2f relative_min = eyelid_type == DualParabolaEyelidFinder::EYELID_LOWER 
            ? config_.lower_parabola_vertex_min_relative
            : config_.upper_parabola_vertex_min_relative;
    Point2f relative_max = eyelid_type == DualParabolaEyelidFinder::EYELID_LOWER 
            ? config_.lower_parabola_vertex_max_relative
            : config_.upper_parabola_vertex_max_relative;
    vertex_range.x = search_region.x + (int) (search_region.width * relative_min.x);
    vertex_range.y = search_region.y + (int) (search_region.height * relative_min.y);
    float relative_width = relative_max.x - relative_min.x;
    float relative_height = relative_max.y - relative_min.y;
    vertex_range.width = (int) (search_region.width * relative_width);
    vertex_range.height = (int) (search_region.height * relative_height);
    Logs::GetLogger().Log(DEBUG, "DualParabolaEyelidFinder::MakeRegionMask "
            "search region (%d, %d) %d x %d; vertex range (%d, %d) %d x %d\n", 
            search_region.x, search_region.y, search_region.width, search_region.height,
            vertex_range.x, vertex_range.y, vertex_range.width, vertex_range.height);
}

class UcharImageMask : public MaskInterface
{
public:
    UcharImageMask(Mat image) : image_(image) {}
    ~UcharImageMask() {}
    bool CanVote(int x, int y) const {
        return image_.at<uchar>(y, x) != 0;
    }
private:
    Mat image_;
};

EyelidBoundary DualParabolaEyelidFinder::DetectBoundary(const cv::Mat& eye_image, 
        const Segmentation& segmentation, 
        const cv::Mat& region_mask, 
        const cv::Rect& vertex_range)
{
    assert(eyelid_type == EYELID_UPPER || eyelid_type == EYELID_LOWER);
    int lowThreshold = 35;
    int ratio = 3;
    int kernel_size = 3;
    bool use_l2_gradient = true;
    int gauss_kernel_size = 3;
    Mat detected_edges;
    cv::blur(eye_image, detected_edges, Size(gauss_kernel_size, gauss_kernel_size));
    cv::Canny(detected_edges, detected_edges, lowThreshold, lowThreshold*ratio, kernel_size, use_l2_gradient);    
    DrawCannyDiagnostic(eye_image, detected_edges, region_mask, vertex_range);
    
    double a_min = eyelid_type == EYELID_UPPER
        ? config_.upper_parabola_a_range.minimum : config_.lower_parabola_a_range.minimum;
    double a_max = eyelid_type == EYELID_UPPER
        ? config_.upper_parabola_a_range.maximum : config_.lower_parabola_a_range.maximum;
    double a_step = eyelid_type == EYELID_UPPER
        ? config_.upper_parabola_a_range.step : config_.lower_parabola_a_range.step;
    double theta_min = eyelid_type == EYELID_UPPER
        ? config_.upper_parabola_theta_range.minimum : config_.lower_parabola_theta_range.minimum;
    double theta_max = eyelid_type == EYELID_UPPER
        ? config_.upper_parabola_theta_range.maximum : config_.lower_parabola_theta_range.maximum;
    double theta_step = eyelid_type == EYELID_UPPER
        ? config_.upper_parabola_theta_range.step : config_.lower_parabola_theta_range.step;
    int h_steps = 20, k_steps = 20;
    vector<double> a_range = ParamRange::Incremental(a_min, a_max, a_step);
    vector<double> h_range = ParamRange::ScaledIncremental(vertex_range.x, vertex_range.width / h_steps, h_steps, 1.0);
    vector<double> k_range = ParamRange::ScaledIncremental(vertex_range.y, vertex_range.height / k_steps, k_steps, 1.0);
    vector<double> theta_range = ParamRange::Incremental(theta_min, theta_max, theta_step);
    Rect search_region = CreateSearchRegion(eye_image, segmentation);
    vector<double> t_range = ParamRange::ScaledIncremental(search_region.x, 1, search_region.width, 1.0);
    if (Logs::GetLogger().IsLevelEnabled(mylog::DEBUG)) {
        Logs::GetLogger().Log(mylog::DEBUG, "DualParabolaEyelidFinder::DetectBoundary a_range[%lu]: %s\n", a_range.size(), Vectors::ToString(a_range).c_str());
        Logs::GetLogger().Log(mylog::DEBUG, "DualParabolaEyelidFinder::DetectBoundary h_range[%lu]: %s\n", h_range.size(), Vectors::ToString(h_range).c_str());
        Logs::GetLogger().Log(mylog::DEBUG, "DualParabolaEyelidFinder::DetectBoundary k_range[%lu]: %s\n", k_range.size(), Vectors::ToString(k_range).c_str());
        Logs::GetLogger().Log(mylog::DEBUG, "DualParabolaEyelidFinder::DetectBoundary theta_range[%lu]: %s\n", theta_range.size(), Vectors::ToString(theta_range).c_str());
//        Logs::GetLogger().Log(mylog::DEBUG, "DualParabolaEyelidFinder::DetectBoundary t_range[%lu]: %s\n", t_range.size(), Vectors::ToString(t_range).c_str());
    }
    Logs::GetLogger().Log(mylog::DEBUG, "DualParabolaEyelidFinder::DetectBoundary search space cardinality = %lu\n", 
            a_range.size() * h_range.size() * k_range.size() * theta_range.size());
    HoughTransform hough;
    UcharImageMask vote_mask(region_mask);
    hough.set_mask(vote_mask);
    hough.AddParamRange(a_range);
    hough.AddParamRange(h_range);
    hough.AddParamRange(k_range);
    hough.AddParamRange(theta_range);
    cvmore::objdetect::VertexFormParabolaShape parabola;
    HoughTransform::Results results;
    hough.Compute(detected_edges, dynamic_cast<ShapeInterface&>(parabola), t_range, results);
    assert(results.size() > 0);
    vector<double>& eyelid_params = results.candidates[0];
    EyelidBoundary eyelid_boundary;
    float vote_threshold = eyelid_type == EYELID_UPPER
            ? config_.upper_eyelid_hough_vote_total_min
            : config_.lower_eyelid_hough_vote_total_min;
    eyelid_boundary.present = results.vote_totals[0] >= vote_threshold;
    eyelid_boundary.shape.set(eyelid_params[VertexFormParabolaShape::kIndexA], 
            eyelid_params[VertexFormParabolaShape::kIndexH], 
            eyelid_params[VertexFormParabolaShape::kIndexK], 
            eyelid_params[VertexFormParabolaShape::kMaxIndex + 1]);
    Logs::GetLogger().Log(mylog::DEBUG, "DualParabolaEyelidFinder::DetectBoundary " 
            "result = %s at indices %s with vote total %f\n", 
            eyelid_boundary.ToString().c_str(), Vectors::ToString(results.indices[0]).c_str(), results.vote_totals[0]);
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
    : a(0), h(0), k(0), theta(0)
{
    
}

void VertexFormParabola::set(double a_, double h_, double k_, double theta_)
{
    a = a_;
    h = h_;
    k = k_;
    theta = theta_;
}

EyelidBoundary::EyelidBoundary()
    : shape(), present(false)
{
    
}

DualParabolaEyelidsLocation::DualParabolaEyelidsLocation()
    : upper(), lower()
{
    
}

void EyelidBoundary::Describe(std::ostream& out) const
{
    out << "EyelidBoundary{present=" << present << ", a=" << shape.a << ", h=" << shape.h << ", k=" << shape.k << ", theta=" << shape.theta << "}";
}

string EyelidBoundary::ToString() const
{
    ostringstream ss;
    Describe(ss);
    return ss.str();
}