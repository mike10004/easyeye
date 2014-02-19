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
#include "../src/easyeye/segment/easyeye_eyelid_detection.h"

using easyeye::Vectors;
using easyeye::IOUtils;
using namespace cvmore::objdetect;
using namespace cv;
using namespace std;

CPPUNIT_TEST_SUITE_REGISTRATION(HoughParabolaTest);

HoughParabolaTest::HoughParabolaTest() {
}

HoughParabolaTest::~HoughParabolaTest() {
}

void HoughParabolaTest::setUp() {
}

void HoughParabolaTest::tearDown() {
}

namespace 
{
static const string output_dir = "/tmp/easyeye_hough_parabola_test";

static string GetOutputPathname(const string& filename) {
    string outpath(output_dir);
    outpath.append("/");
    outpath.append(filename);
    return outpath;
}

Rect BoundsOf(const Mat& image) {
    Rect r(0, 0, image.cols, image.rows);
    return r;
}

bool DrawLine(Mat& image, Point2i from, Point2i to, Scalar color)
{
    int thickness = 2;
    int line_type = 8;
    int shift = 0;
    bool drawn;
    if (drawn = (from.inside(BoundsOf(image)) && to.inside(BoundsOf(image)))) {
        cv::line(image, from, to, color, thickness, line_type, shift);
    }
    return drawn;
}

void Draw(const ShapeInterface& shape, const vector<double>& params, Scalar color, Mat& image) {
    Point2i last;
    Point2i current;
    bool past_first = false;
    int num_drawn = 0;
    for (int x = 0; x < image.cols; x++) {
        Point2d p;
        shape.Calculate((double) x, params, p);
        current.y = (int) round(p.y);
        current.x = x;
        if (past_first) {
            bool drawn = DrawLine(image, last, current, color);
            if (drawn) num_drawn ++;
        } 
        past_first = true;
        last.x = current.x;
        last.y = current.y;
    }
    cerr << num_drawn << " points drawn onto image of dims " << image.rows << "x" << image.cols << endl;
}


Mat CreateInputImage(const ShapeInterface& shape, const vector<double>& params, const string& filename)
{
    
    bool ok = IOUtils::MakeDirs(output_dir);
    CPPUNIT_ASSERT(ok);
    int w = 100, h = 100;
    Scalar init_color(0);
    Mat image(h, w, CV_8U);
    image = init_color;
    Scalar white(0xff);
    Draw(shape, params, white, image);
    int an = 2;
    Mat element = getStructuringElement(MORPH_RECT, Size(an*2+1, an*2+1), Point(an, an) );
    cv::dilate(image, image, element);
    string input_pathname = GetOutputPathname(filename);
    cerr << cv::imwrite(input_pathname, image) << " returned from write " << input_pathname << endl;
    return image;
}

Mat CreateOutputImage(const ShapeInterface& shape, const vector<double>& params, const Mat& image, const string& filename)
{
    Mat viewable;
    cv::cvtColor(image, viewable, CV_GRAY2RGB);
    Scalar red(0x0, 0x0, 0xff);
    Draw(shape, params, red, viewable);
    string output_filename = GetOutputPathname(filename);
    cerr << cv::imwrite(output_filename, viewable) << " returned from write " << output_filename << endl;
    return viewable;
}

vector<double> AssertTolerablyEqual(const vector<double> expected, const vector<double>& actual, vector<double>& tolerances)
{
    
    vector<double> diffs;
    CPPUNIT_ASSERT_EQUAL(expected.size(), actual.size());
    CPPUNIT_ASSERT_EQUAL(expected.size(), tolerances.size());
    for (size_t i = 0; i < expected.size(); i++) {
        diffs.push_back(abs(expected[i] - actual[i]));
    }
    cerr << "diffs: ";
    Vectors::Print(diffs, cerr);
    cerr << endl;
    for (size_t i = 0; i < expected.size(); i++) {
        CPPUNIT_ASSERT(diffs[i] <= tolerances[i]);
    }
    return diffs;
}

vector<double> ToVector(double a, double b, double c) {
    vector<double> v;
    v.push_back(a);
    v.push_back(b);
    v.push_back(c);
    return v;
}

vector<double> ToVector(double a, double b, double c, double d) {
    vector<double> v = ToVector(a, b, c);
    v.push_back(d);
    return v;
}
}
void HoughParabolaTest::testComputeTransformStandardForm() {
    double a = -0.05, b = 5.5, c = -75;    
    vector<double> params(3, 0.0);
    params[0] = a; params[1] = b; params[2] = c;
    StandardFormParabolaShape parabola;
    Mat image = CreateInputImage(parabola, params, "input-standardform.png");
    vector<double> a_range = ParamRange::ScaledIncremental(-10, 1, 20, 0.01);
    vector<double> b_range = ParamRange::ScaledIncremental(500, 10, 10, 0.01);
    vector<double> c_range = ParamRange::ScaledIncremental(-80, 1, 10, 1.0);
    HoughTransform hough_transform;
    hough_transform.AddParamRange(a_range);
    hough_transform.AddParamRange(b_range);
    hough_transform.AddParamRange(c_range);
    hough_transform.set_debug(true);
    cerr << "transforming..." << endl;
    vector<double> best = hough_transform.Compute(image, parabola);
    double best_a = best[0], best_b = best[1], best_c = best[2];
    cerr  << "best: " << best_a << ' ' << best_b << ' ' <<  best_c << endl;
    CreateOutputImage(parabola, best, image, "output-standardform.png");
    vector<double> tolerances = ToVector(0.011, 0.3, 5);
    AssertTolerablyEqual(params, best, tolerances);
}

void HoughParabolaTest::testComputeTransformVertexForm() {
    double a = -0.05, h = 50, k = 25;    
    vector<double> params(3, 0.0);
    params[0] = a; params[1] = h; params[2] = k;
    VertexFormParabolaShape parabola;
    Mat image = CreateInputImage(parabola, params, "input-vertexform.png");
    vector<double> a_range = ParamRange::ScaledIncremental(-10, 1, 20, 0.01);
    vector<double> h_range = ParamRange::ScaledIncremental(0, 1, 100, 1.0);
    vector<double> k_range = ParamRange::ScaledIncremental(0, 1, 100, 1.0);
    HoughTransform hough_transform;
    hough_transform.AddParamRange(a_range);
    hough_transform.AddParamRange(h_range);
    hough_transform.AddParamRange(k_range);
    hough_transform.set_debug(true);
    cerr << "transforming..." << endl;
    vector<double> best = hough_transform.Compute(image, parabola);
    cerr << "best fit: ";
    easyeye::Vectors::Print(best, cerr);
    cerr << endl;
    CreateOutputImage(parabola, best, image, "output-vertexform.png");
    vector<double> tolerances = ToVector(0.011, 5, 5);
    AssertTolerablyEqual(params, best, tolerances);
}

void HoughParabolaTest::testComputeTransformVertexFormRotated()
{
    double a = -0.05, h = 50, k = 25;    
    double theta = M_PI / 3;
    vector<double> params(4, 0.0);
    params[0] = a; params[1] = h; params[2] = k; params[3] = theta;
    VertexFormParabolaShape parabola;
    RotatedShape rotated_parabola(parabola, VertexFormParabolaShape::kMaxIndex + 1);
    Mat image = CreateInputImage(rotated_parabola, params, "input-rotated.png");
    vector<double> a_range = ParamRange::ScaledIncremental(-10, 1, 20, 0.01);
    vector<double> h_range = ParamRange::ScaledIncremental(0, 2, 50, 1.0);
    vector<double> k_range = ParamRange::ScaledIncremental(0, 2, 50, 1.0);
    vector<double> theta_range = ParamRange::Incremental(0.0, M_PI * 2.0, M_PI * 2.0 / 16);
    
    HoughTransform hough_transform;
    hough_transform.AddParamRange(a_range);
    hough_transform.AddParamRange(h_range);
    hough_transform.AddParamRange(k_range);
    hough_transform.AddParamRange(theta_range);
    hough_transform.set_debug(true);
    cerr << "transforming..." << endl;
    vector<double> best = hough_transform.Compute(image, rotated_parabola);
    cerr << "best fit: ";
    Vectors::Print(best, cerr);
    cerr << endl;
    CreateOutputImage(rotated_parabola, best, image, "output-rotated.png");
    vector<double> tolerances = ToVector(0.011, 5, 5);
    AssertTolerablyEqual(params, best, tolerances);
}
