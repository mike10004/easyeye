/*
 * File:   HoughParabolaTest.cc
 * Author: mchaberski
 *
 * Created on Feb 7, 2014, 12:10:37 PM
 */

#include "HoughParabolaTest.h"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <vector>
#include <cmath>
#include <opencv2/imgproc/imgproc.hpp>
#include "../src/easyeye/common/easyeye_utils.h"
#include <ctime>

using namespace cv;
using namespace std;
using namespace easyeye;

CPPUNIT_TEST_SUITE_REGISTRATION(HoughParabolaTest);

template<class T>
class ParamRange
{
public:
    ParamRange() : min(), max(), step() {}
    ParamRange(T min_, T max_, T step_) : min(min_), max(max_), step(step_) {}
    T min;
    T max;
    T step;
    vector<T> values() const {
        vector<T> vals;
        for (T x = min; x <= max; x += step) {
            vals.push_back(x);
        }
        return vals;
    }
};

class Pixels
{
public:
    static float Interpolate(const Mat& img, double px, double py)
    { // http://stackoverflow.com/questions/13299409/how-to-get-the-image-pixel-at-real-locations-in-opencv
        assert(!img.empty());
        int x = (int)px;
        int y = (int)py;

        int x0 = cv::borderInterpolate(x,   img.cols, cv::BORDER_REFLECT_101);
        int x1 = cv::borderInterpolate(x+1, img.cols, cv::BORDER_REFLECT_101);
        int y0 = cv::borderInterpolate(y,   img.rows, cv::BORDER_REFLECT_101);
        int y1 = cv::borderInterpolate(y+1, img.rows, cv::BORDER_REFLECT_101);

        float a = px - (float)x;
        float c = py - (float)y;

        float b = ((img.at<uchar>(y0, x0) * (1.f - a) 
                + img.at<uchar>(y0, x1) * a) * (1.f - c)
                + (img.at<uchar>(y1, x0) * (1.f - a) 
                + img.at<uchar>(y1, x1) * a) * c);
        return b;
    }    
    
private:
    Pixels();
};

class HoughTransform
{
public:
    HoughTransform(vector< ParamRange<double> >& param_ranges) 
        : debug_(false), param_ranges_(), num_params(param_ranges.size()) 
    {
        for (size_t i = 0; i < param_ranges.size(); i++) {
            param_ranges_.push_back(param_ranges[i].values());
        }
    }
    HoughTransform(vector< vector<double> > param_ranges) 
        : debug_(false), param_ranges_(), num_params(param_ranges.size())
    {
        for (size_t i = 0; i < param_ranges.size(); i++) {
            param_ranges_.push_back(param_ranges[i]);
        }
    }
    virtual ~HoughTransform() {}
    virtual bool CanVote(int x, int y, const Mat& image) const {
        return x >= 0 && x < image.cols && y >= 0 && y < image.rows;
    }
    virtual float GetPixelValue(const Mat& image, double x, double y) const {
        return Pixels::Interpolate(image, x, y);
    }
    virtual double CalculateY(double x, const vector<double>& params) = 0;
    static const int max_int;
    static const int min_int;
    static bool IsInsideIntLimits(double value) {
        return value >= min_int && value <= max_int;
    }
    bool debug() const {
        return debug_;
    }
    void set_debug(bool debug) {
        debug_ = debug;
    }
    void set_normalize(bool normalize) {
        normalize_ = normalize;
    }
    bool normalize() const {
        return normalize_;
    }
protected:
    bool debug_;
private:
    vector< vector<double> > param_ranges_;
    Mat accumulator;
    Mat normalized_accumulator;
    Mat candidate_magnitudes;
    bool normalize_;
    const size_t num_params;
    bool AdvanceParameterIndices(int* param_indices) {
        bool bump = true;
        for (int i = (int) num_params - 1; i >= 0 && bump; i--) {
            param_indices[i] = param_indices[i] + 1;
            bump = param_indices[i] >= param_ranges_[i].size();
            if (bump) {
                param_indices[i] = 0;
            }
        }
        return !bump;
    }
    void SetParameterValues(int* param_indices, vector<double>& param_values) {
        for (int i = 0; i < num_params; i++) {
            param_values[i] = param_ranges_[i][param_indices[i]];
        }
    }
public:
    vector<double> Compute(const Mat& image, vector< vector<double> >& candidates) {
        clock_t start = clock();
        const int ndims = (int) num_params;
        int* sizes = new int[ndims];
        for (int i = 0; i < ndims; i++) {
            sizes[i] = (int) param_ranges_[i].size();
        }
        accumulator.create(ndims, sizes, CV_32F);
        accumulator = Scalar(0.0f);
        if (normalize_) {
            accumulator.copyTo(normalized_accumulator);
            accumulator.copyTo(candidate_magnitudes);
        }
        int* param_indices = new int[ndims];
        memset(param_indices, 0, sizeof(int) * ndims);
        vector<double> param_values(num_params, 0.0);
        bool can_continue = Vectors::Cardinality(param_ranges_) > 0;
        while (can_continue) {
            SetParameterValues(param_indices, param_values);
            for (int x = 0; x < image.cols; x++) {
                double y = CalculateY(x, param_values);
                if (IsInsideIntLimits(y)) {
                    int rx = (int) round(x), ry = (int) round(y);
                    if (CanVote(rx, ry, image)) {
                        float pixel_value = GetPixelValue(image, x, y);
                        float current_accum = accumulator.at<float>(param_indices);
                        float new_accum = current_accum + pixel_value;
                        accumulator.at<float>(param_indices) = new_accum;
                        if (normalize_) {
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
        int* max_index = new int[ndims];
        double max_accumulated;
        Mat& effective_accumulator = normalize_ ? normalized_accumulator : accumulator;
        minMaxIdx(effective_accumulator, NULL, &max_accumulated, NULL, max_index);
        if (debug_) {
            cerr << "hough: max accumulation at index " << max_index[0] << ' ' << max_index[1] << ' ' << max_index[2] << " = " << max_accumulated << endl;
        }
        vector<double> best_param_values;
        for (int i = 0; i < ndims; i++) {
            best_param_values.push_back(param_ranges_[i][max_index[i]]);
        }
        candidates.push_back(best_param_values);
        delete sizes;
        delete param_indices;
        delete max_index;
        return best_param_values;
    }
    
    vector<double> Compute(const Mat& image) {
        vector< vector<double> > candidates;
        return Compute(image, candidates);
    }
};

//class HoughParabolaTransform
//{
//public:
//    HoughParabolaTransform(vector<double> a_range__, vector<double> b_range__, vector<double> c_range__)
//        : a_range_(a_range__), b_range_(b_range__), c_range_(c_range__), max_candidates_(1), accumulator(), debug_(true)
//    {
//    }
//    HoughParabolaTransform(const ParamRange<double>& a_range, const ParamRange<double>& b_range, const ParamRange<double>& c_range)
//        : a_range_(a_range.values()), b_range_(b_range.values()), c_range_(c_range.values()), max_candidates_(1), accumulator(), debug_(true)
//    {
//    }
//    
//    virtual bool CanVote(int x, int y, double a, double b, double c, const Mat& image) {
//        return x >= 0 && x < image.cols && y >= 0 && y < image.rows;
//    }
//    
//    float GetPixelValue(const Mat& image, double x, double y) {
//        return GetSubpixel(image, x, y);
//    }
//    
//    Vec3d Compute(const Mat& image, vector<Vec3d> candidates) {
//        int sizes[] = { (int) a_range_.size(), (int) b_range_.size(), (int) c_range_.size() };
//        accumulator.create(3, sizes, CV_32F);
//        accumulator = Scalar(0.0f);
//        if (debug_) {
//            cerr << "a range: ";
//            for (vector<double>::iterator ait = a_range_.begin(); ait != a_range_.end(); ++ait) {
//                cerr << *ait << ' ' ;
//            }
//            cerr << endl;
//            cerr << "b range: ";
//            for (vector<double>::iterator bit = b_range_.begin(); bit != b_range_.end(); ++bit) {
//                cerr << *bit << ' ' ;
//            }
//            cerr << endl;
//            cerr << "c range: ";
//            for (vector<double>::iterator cit = c_range_.begin(); cit != c_range_.end(); ++cit) {
//                cerr << *cit << ' ' ;
//            }
//            cerr << endl;
//        }
//        for (size_t ai = 0; ai < a_range_.size(); ai++) {
//            double a = a_range_[ai];
//            for (size_t bi = 0; bi < b_range_.size(); bi++) {
//                double b = b_range_[bi];
//                for (size_t ci = 0; ci < c_range_.size(); ci++) {
//                    double c = c_range_[ci];
//                    for (int x = 0; x < image.cols; x++) {
//                        double y = a * x * x + b * x + c;
//                        if (IsInsideIntLimits(y)) {
//                            int rx = (int) round(x), ry = (int) round(y);
//                            if (CanVote(rx, ry, a, b, c, image)) {
//                                float pixel = GetPixelValue(image, x, y);
//                                accumulator.at<float>(ai, bi, ci) += pixel;
//                            }
//                        }
//                    }
//                    ci++;
//                }
//                bi++;
//            }
//            ai++;
//        }
//        int max_index[3];
//        double max_accumulated;
//        minMaxIdx(accumulator, NULL, &max_accumulated, NULL, max_index);
//        if (debug_) {
//            cerr << "hough: max accumulation at index " << max_index[0] << ' ' << max_index[1] << ' ' << max_index[2] << " = " << max_accumulated << endl;
//        }
//        Vec3d most_votes(a_range_[max_index[0]], b_range_[max_index[1]], c_range_[max_index[2]]);
//        candidates.push_back(most_votes);
//        return most_votes;
//    }
//private:
//    vector<double> a_range_, b_range_, c_range_;
//    size_t max_candidates_;
//    Mat accumulator;
//    bool debug_;
//};

class HoughParabolaTransform : public HoughTransform
{
public:
    static vector< vector<double> > ToVector(const ParamRange<double>& a_range, const ParamRange<double>& b_range, const ParamRange<double>& c_range)
    {
        vector< vector<double> > ranges;
        ranges.push_back(a_range.values());
        ranges.push_back(b_range.values());
        ranges.push_back(c_range.values());
        return ranges;
    }
    HoughParabolaTransform(const ParamRange<double>& a_range, const ParamRange<double>& b_range, const ParamRange<double>& c_range)
        : HoughTransform(ToVector(a_range, b_range, c_range))
    {
        
    }
    HoughParabolaTransform(vector< vector<double> >& ranges) : HoughTransform(ranges) 
    {
    }
    virtual double CalculateY(double x, const vector<double>& params) {
        double a = params[0], b = params[1], c = params[2];
        return a * x * x + b * x + c;
    }

};

const int HoughTransform::max_int = numeric_limits<int>::max();
const int HoughTransform::min_int = numeric_limits<int>::min();

HoughParabolaTest::HoughParabolaTest() {
}

HoughParabolaTest::~HoughParabolaTest() {
}

void HoughParabolaTest::setUp() {
}

void HoughParabolaTest::tearDown() {
}

static const string output_dir = "/tmp/easyeye_hough_parabola_test";

static string GetOutputPathname(const string& filename) {
    string outpath(output_dir);
    outpath.append("/");
    outpath.append(filename);
    return outpath;
}

void DrawParabolaUchar(double a, double b, double c, uchar color, Mat& image) {
    for (int x = 0; x < image.cols; x++) {
        int y = (int) round(a * x * x + b * x + c);
        if (y >= 0 && y < image.cols) {
            image.row(y).at<uchar>(x) = color;
        }
    }
}

void DrawParabola3Byte(double a, double b, double c, Scalar color, Mat& image) {
    for (int x = 0; x < image.cols; x++) {
        int y = (int) round(a * x * x + b * x + c);
        if (y >= 0 && y < image.cols) {
            image.row(y).at<Vec3b>(x)[0] = color[0];
            image.row(y).at<Vec3b>(x)[1] = color[1];
            image.row(y).at<Vec3b>(x)[2] = color[2];
        }
    }
}

void HoughParabolaTest::testComputeTransform() {
    bool ok = IOUtils::MakeDirs(output_dir);
    CPPUNIT_ASSERT(ok);
    int w = 100, h = 100;
    Scalar init_color(0);
    Mat image(h, w, CV_8U);
    image = init_color;
    
    double a = -0.05, b = 5.5, c = -75;
    DrawParabolaUchar(a, b, c, 0xff, image);
    int an = 2;
    Mat element = getStructuringElement(MORPH_RECT, Size(an*2+1, an*2+1), Point(an, an) );
    cv::dilate(image, image, element);
    string input_filename = GetOutputPathname("input.png");
    cerr << cv::imwrite(input_filename, image) << " returned from write " << input_filename << endl;

    ParamRange<double> a_range(a - 0.05, a + 0.05, (0.05 * 2.0) / 100.0);
    ParamRange<double> b_range(b - 0.5, b + 0.5, (0.5 * 2.0) / 100.0);
    ParamRange<double> c_range(c - 5.0, c + 5.0, (5.0 * 2.0) / 100.0);
    vector< vector<double> > param_ranges;
    param_ranges.push_back(a_range.values());
    param_ranges.push_back(b_range.values());
    param_ranges.push_back(c_range.values());
    HoughParabolaTransform hough_transform(a_range, b_range, c_range);//param_ranges);
    hough_transform.set_debug(true);
    cerr << "transforming..." << endl;
    vector<double> best = hough_transform.Compute(image);
    double best_a = best[0], best_b = best[1], best_c = best[2];
    cerr  << "best: " << best_a << ' ' << best_b << ' ' <<  best_c << endl;
    
    Mat viewable;
    cv::cvtColor(image, viewable, CV_GRAY2RGB);
    Scalar red(0xff, 0x0, 0x0);
    DrawParabola3Byte(best_a, best_b, best_c, red, viewable);
    string output_filename = GetOutputPathname("output.png");
    cerr << cv::imwrite(output_filename, viewable) << " returned from write " << output_filename << endl;
    
    double a_tolerance = 0.01, b_tolerance = 0.3, c_tolerance = 5;
    double a_diff = abs(a - best_a);
    double b_diff = abs(b - best_b);
    double c_diff = abs(c - best_c);
    cerr << "diffs: " << a_diff << ' ' << b_diff << ' ' << c_diff << endl;
    CPPUNIT_ASSERT(a_diff < a_tolerance);
    CPPUNIT_ASSERT(b_diff < b_tolerance);
    CPPUNIT_ASSERT(c_diff < c_tolerance);
}
