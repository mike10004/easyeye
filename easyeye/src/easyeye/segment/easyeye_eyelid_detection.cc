#include "easyeye_eyelid_detection.h"
#include "../common/easyeye_imaging.h"
#include <opencv2/core/core.hpp>
#include "../common/easyeye_utils.h"
#include <vector>
#include <cstring>
#include <ctime>
#include <opencv2/imgproc/imgproc.hpp>

using easyeye::Vectors;
using namespace cvmore::objdetect;
using std::sin;
using std::cos;
using std::vector;
using std::cerr;
using std::endl;
using cv::Mat;

vector<double> ParamRange::Incremental(double min, double max, double step)
{
    vector<double> vals;
    for (double x = min; x <= max; x += step) {
        vals.push_back(x);
    }
    return vals;
}

vector<double> ParamRange::Scaled(const std::vector<int>& values, double scalar)
{
    vector<double> scaled_values;
    for (size_t i = 0; i < values.size(); i++) {
        scaled_values.push_back(values[i] * scalar);
    }
    return scaled_values;
}

vector<int> ParamRange::Steps(int min, int step, int num_values)
{
    vector<int> values;
    for (int i = 0; i < num_values; i++) {
        values.push_back(min + step * i);
    }
    return values;
}

std::vector<double> ParamRange::ScaledIncremental(int min, int step, 
        int num_values, double scalar) 
{
    vector<int> values = Steps(min, step, num_values);
    return Scaled(values, scalar);
}

HoughTransform::HoughTransform() 
    :   debug_(false),
        param_ranges_(), 
        mask_(NULL),
        accumulator(),
        normalized_accumulator(),
        candidate_magnitudes(),
        num_params(0),
        max_candidates_(1),
        normalized_(false)
{
}

HoughTransform:: ~HoughTransform() 
{
}

bool DefaultMask::CanVote(int x, int y) const 
{
    return x >= 0 && x < num_cols && y >= 0 && y < num_rows;
}

float HoughTransform::GetPixelValue(const Mat& image, double x, double y) const 
{
    return cvmore::Pixels::Interpolate(image, x, y);
}

bool HoughTransform::IsInsideIntLimits(double value) {
    return value >= min_int && value <= max_int;
}

bool HoughTransform::debug() const {
    return debug_;
}

void HoughTransform::set_debug(bool debug) {
    debug_ = debug;
}

void HoughTransform::set_max_candidates(size_t max_candidates)
{
    max_candidates_ = max_candidates;
}

size_t HoughTransform::max_candidates() const 
{
    return max_candidates_;
}

bool HoughTransform::AdvanceParameterIndices(int* param_indices) 
{
    bool bump = true;
    for (int i = (int) num_params - 1; i >= 0 && bump; i--) {
        param_indices[i] = param_indices[i] + 1;
        bump = param_indices[i] >= (int) param_ranges_[i].size();
        if (bump) {
            param_indices[i] = 0;
        }
    }
    return !bump;
}

void HoughTransform::SetParameterValues(int* param_indices, vector<double>& param_values) 
{
    for (size_t i = 0; i < num_params; i++) {
        param_values[i] = param_ranges_[i][param_indices[i]];
    }
}

void HoughTransform::set_mask(const MaskInterface& mask)
{
    mask_ = &mask;
}

void HoughTransform::Compute(const cv::Mat& image, 
        const ShapeInterface& shape, 
        const std::vector<double>& t_range, Results& results)
{
    Accumulate(image, shape, t_range);
    GatherCandidates(results);
}

void HoughTransform::Accumulate(const cv::Mat& image, 
        const ShapeInterface& shape, 
        const std::vector<double>& t_range)
{
    clock_t start = clock();
    const int ndims = (int) num_params;
    int* sizes = new int[ndims];
    for (int i = 0; i < ndims; i++) {
        sizes[i] = (int) param_ranges_[i].size();
    }
    accumulator.create(ndims, sizes, CV_32F);
    accumulator = cv::Scalar(0.0f);
    if (normalized_) {
        accumulator.copyTo(normalized_accumulator);
        accumulator.copyTo(candidate_magnitudes);
    }
    int* param_indices = new int[ndims];
    memset(param_indices, 0, sizeof (int) * ndims);
    vector<double> param_values(num_params, 0.0);
    DefaultMask default_mask(image.rows, image.cols);
    MaskInterface const* mask;
    if (mask_ == NULL) {
        mask = &default_mask;
    } else {
        mask = mask_;
    }
    size_t cardinality = Vectors::Cardinality(param_ranges_);
    if (debug_) {
        cerr << "hough: transforming in space of cardinality " << cardinality << endl;
    }
    bool can_continue = cardinality > 0;
    cv::Point2d p;
    while (can_continue) {
        SetParameterValues(param_indices, param_values);
        for (size_t i = 0; i < t_range.size(); i++) {
            double t = t_range[i];
            bool in_range = shape.Calculate(t, param_values, p);
            if (in_range && IsInsideIntLimits(p.y) && IsInsideIntLimits(p.x)) {
                int rx = (int) round(p.x), ry = (int) round(p.y);
                if (mask->CanVote(rx, ry)) {
                    float pixel_value = GetPixelValue(image, p.x, p.y);
                    float current_accum = accumulator.at<float>(param_indices);
                    float new_accum = current_accum + pixel_value;
                    accumulator.at<float>(param_indices) = new_accum;
                    if (normalized_) {
                        unsigned long current_magnitude = candidate_magnitudes.at<float>(param_indices);
                        unsigned long new_magnitude = current_magnitude + 1;
                        candidate_magnitudes.at<float>(param_indices) = (float) (new_magnitude);
                        normalized_accumulator.at<float>(param_indices) = (new_accum / (float) new_magnitude);
                    }
                }
            }
        }
        can_continue = AdvanceParameterIndices(param_indices);
    }
    clock_t stop = clock();
    float duration = (stop - start) / (double) CLOCKS_PER_SEC;
    if (debug_) {
        cerr << "hough: " << duration << " seconds to compute transform" << endl;
    }
    delete sizes;
    delete param_indices;
}

void HoughTransform::GatherCandidates(Results& results)
{
    Mat accumulator_copy = accumulator.clone();
    const int ndims = num_params;
    while (results.candidates.size() < max_candidates()) {
        int* max_index = new int[ndims];
        double max_accumulated;
        cv::minMaxIdx(accumulator_copy, NULL, &max_accumulated, NULL, max_index);
        vector<int> max_indices;
        Vectors::CopyFrom(max_index, max_indices, ndims);
        results.indices.push_back(max_indices);
        vector<double> best_param_values;
        for (int i = 0; i < ndims; i++) {
            best_param_values.push_back(param_ranges_[i][max_index[i]]);
        }
        results.candidates.push_back(best_param_values);
        results.vote_totals.push_back(max_accumulated);
        if (debug_) {
            cerr << "hough: max accumulation at index " << max_index[0] 
                    << ' ' << max_index[1] << ' ' << max_index[2] << " = " << max_accumulated << endl;
        }
        accumulator_copy.at<float>(max_index) = 0.f;
        delete max_index;
    }
}

std::vector<double> HoughTransform::Compute(const cv::Mat& image, 
        const ShapeInterface& shape)
{
    return Compute(image, shape, ParamRange::MakeRangeFromImage(image));
}

vector<double> HoughTransform::Compute(const cv::Mat& image, 
        const ShapeInterface& shape, 
        const std::vector<double>& t_range)
{
    Results results;
   Compute(image, shape, t_range, results);
   return results.candidates[0];
}

void HoughTransform::Compute(const cv::Mat& image, 
        const ShapeInterface& shape, HoughTransform::Results& results)
{
    Compute(image, shape, ParamRange::MakeRangeFromImage(image), results);
}

vector<double> ParamRange::MakeRangeFromImage(const cv::Mat& image)
{
    vector<double> range(image.cols, 0.0);
    for (int x = 0; x < image.cols; x++) {
        range[x] = (double)x;
    }
    return range;
}

StandardFormParabolaShape::StandardFormParabolaShape()
{
    
}

StandardFormParabolaShape::~StandardFormParabolaShape()
{
    
}
bool StandardFormParabolaShape::Calculate(double t, const std::vector<double>& params, cv::Point2d& output) const 
{
    double a = params[0], b = params[1], c = params[2];
    output.x = t;
    output.y = a * t * t + b * t + c;
    return true;
}

const int HoughTransform::max_int = std::numeric_limits<int>::max();
const int HoughTransform::min_int = std::numeric_limits<int>::min();

MaskInterface::MaskInterface()
{
}

MaskInterface::~MaskInterface()
{
}

DefaultMask::DefaultMask(int num_rows_, int num_cols_)
    : num_rows(num_rows_), num_cols(num_cols_)
{
}

DefaultMask::~DefaultMask()
{
}

ShapeInterface::ShapeInterface()
{
}

ShapeInterface::~ShapeInterface() 
{
}

ShapeArtist::ShapeArtist()
    : closed_(false), thickness_(3), color_(cv::Scalar(0xff, 0xff, 0x0, 0xff))
{
}

void ShapeArtist::set_color(cv::Scalar color)
{
    color_ = color;
}

ShapeArtist::~ShapeArtist()
{
}

static bool InBounds(const cv::Point2d& point, const cv::Mat& image)
{
    int x = cvRound(point.x);
    int y = cvRound(point.y);
    return (0 <= x) && (x < image.cols) && (0 <= y) && (y < image.rows);
}

void ShapeArtist::Draw(cv::Mat& image, ShapeInterface& shape, 
        const std::vector<double>& t_range, const std::vector<double>& params) const
{
    cv::Point2d curr;
    cv::Point2d prev;
    bool prev_ok = false, curr_ok = false;
    if (!t_range.empty()) {
        prev_ok = shape.Calculate(t_range[0], params, prev);
        for (size_t i = 1; i < t_range.size(); i++) {
            double t = t_range[i];
            curr_ok = shape.Calculate(t, params, curr);
            if (prev_ok && curr_ok && InBounds(prev, image) && InBounds(curr, image)) {
                cv::line(image, prev, curr, color_, thickness_);
            }
        }
    }
}

const cv::Point2d ShapeInterface::kOrigin(0.0, 0.0);

void HoughTransform::AddParamRange(const std::vector<double>& param_range)
{
    param_ranges_.push_back(param_range);
    num_params++;
}

RotatedShape::RotatedShape(ShapeInterface& original, int theta_index)
    : unrotated_(dynamic_cast<ShapeInterface*>(&original)), theta_index_(theta_index)
{
}

RotatedShape::~RotatedShape()
{
     
}

cv::Point2d RotatedShape::ComputeFixedPoint(const std::vector<double>& params) const
{
    return unrotated_->ComputeFixedPoint(params);
}

bool RotatedShape::Calculate(double t, const std::vector<double>& params, cv::Point2d& output) const
{
    bool in_range = unrotated_->Calculate(t, params, output);
    cv::Point2d translation = unrotated_->ComputeFixedPoint(params);
    double theta = params[theta_index_];
    double x = output.x - translation.x;
    double y = output.y - translation.y;
    double sin_theta = sin(theta), cos_theta = cos(theta);
    output.x = x * cos_theta - y * sin_theta + translation.x; 
    output.y = x * sin_theta + y * cos_theta + translation.y;
    return in_range;
}

VertexFormParabolaShape::VertexFormParabolaShape() 
{
}

VertexFormParabolaShape::~VertexFormParabolaShape()
{
}

bool VertexFormParabolaShape::Calculate(double t, const std::vector<double>& params, cv::Point2d& p) const
{
    double a = params[kIndexA], h = params[kIndexH], k = params[kIndexK];
    p.x = t;
    p.y = a * (p.x - h) * (p.x - h) + k;
    return true;
}

cv::Point2d StandardFormParabolaShape::ComputeFixedPoint(const std::vector<double>& params) const
{
    cv::Point2d v;
    double b = params[kIndexB], a = params[kIndexA];
    v.x = -b / (2 * a);
    Calculate(v.x, params, v);
    return v;
}

cv::Point2d VertexFormParabolaShape::ComputeFixedPoint(const std::vector<double>& params) const
{
    cv::Point2d v(params[kIndexH], params[kIndexK]);
    return v;
}

const char* easyeye::DualParabolaEyelidsLocation::kType = "dual_parabola";

size_t HoughTransform::Results::size() const
{
    assert(candidates.size() == vote_totals.size());
    assert(candidates.size() == indices.size());
    return candidates.size();
}