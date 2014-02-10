/*
 * File:   RotatedShapeTest.cc
 * Author: mike
 *
 * Created on Feb 9, 2014, 5:45:16 PM
 */

#include "RotatedShapeTest.h"
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
#include <vector>
#include <iostream>
#include "../src/easyeye/segment/easyeye_eyelid_detection.h"
#include "src/easyeye/common/easyeye_utils.h"
#include <string>

using namespace cv;
using namespace std;
using namespace cvmore::objdetect;

namespace 
{

CPPUNIT_TEST_SUITE_REGISTRATION(RotatedShapeTest);

class Rectangle : public ShapeInterface
{
public:
    enum FixedPoint { CENTER, RECT_ORIGIN, TRUE_ORIGIN };
    Rectangle() : fixed_point(CENTER) {}
    ~Rectangle() {}
    FixedPoint fixed_point;
    static const int kIndexULX = 0, kIndexULY = 1, kIndexWidth = 2, kIndexHeight = 3;
    bool Calculate(double t, const std::vector<double>& params, cv::Point2d& p) const
    {
        p.x = -1;
        p.y = -1;
        Point2d origin(params[kIndexULX], params[kIndexULY]);
        Size_<double> size(params[kIndexWidth], params[kIndexHeight]);
        if (t < 0) {
            return false;
        } else if (t < 1) {
            p.x = origin.x;
            p.y = origin.y;
        } else if (t < 2) {
            p.x = origin.x + size.width;
            p.y = origin.y;
        } else if (t < 3) {
            p.x = origin.x + size.width;
            p.y = origin.y + size.height;
        } else if (t < 4) {
            p.x = origin.x;
            p.y = origin.y + size.height;
        } else {
            return false;
        }
        return true;
    }
    
    Point2d ComputeFixedPoint(const std::vector<double>& params) const {
        Point2d rect_origin(params[kIndexULX], params[kIndexULY]);
        if (fixed_point == CENTER) {
            Size_<double> size(params[kIndexWidth], params[kIndexHeight]);
            Point2d center(rect_origin.x + size.width / 2.0, rect_origin.y + size.height / 2.0);
            return center;
        } else if (fixed_point == RECT_ORIGIN) {
            return rect_origin;
        } else {
            Point2d origin(0.0, 0.0);
            return origin;
        }
    }
};

class Line : public ShapeInterface
{
public:
    Line() {}
    ~Line() {}
    const static int kX1 = 0, kY1 = 1, kX2 = 2, kY2 = 3;
    bool Calculate(double t, const std::vector<double>& params, cv::Point2d& output) const
    {
        if (t < 0) {
            output.x = params[kX1];
            output.y = params[kY1];
        } else {
            output.x = params[kX2];
            output.y = params[kY2];
        }
        return true;
    }
    
    Point2d ComputeFixedPoint(const std::vector<double>& params) const {
        Point2d v(params[kX1], params[kY1]);
        return v;
    }
};


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

void Print(const vector<Point2d>& v, ostream& out)
{
    if (v.empty()) {
        return;
    }
    out << '(' << v[0].x << ", " << v[0].y << ")";
    for (size_t i = 1; i < v.size(); i++) {
        out << " (" << v[i].x << ", " << v[i].y << ")";
    }
}

vector<Point2d> ToPoints(const ShapeInterface& shape, const vector<double>& params, const vector<double>& t_range)
{
    vector<Point2d> pts;
    for (size_t i = 0; i < t_range.size(); i++) {
        double t = t_range[i];
        Point2d p;
        if (shape.Calculate(t, params, p)) {
            pts.push_back(p);
        }
    }
    cerr << pts.size() << " points: ";
    Print(pts, cerr);
    cerr << endl;
    return pts;
}

void Draw(const ShapeInterface& shape, const vector<double>& params, const vector<double>& t_range, Scalar color, Mat& image) {
    vector<Point2d> pts = ToPoints(shape, params, t_range);
    if (pts.empty()) {
        cerr << "zero points in shape" << endl;
        return;
    }
    int num_drawn = 0;
    for (size_t i = 1; i < t_range.size(); i++) {
        bool drawn = DrawLine(image, pts[i - 1], pts[i], color);
        if (drawn) num_drawn ++;
    }
    bool closure = DrawLine(image, pts[pts.size() - 1], pts[0], color);
    if (closure > 0) {
        num_drawn ++; // first point
    }
//    cerr << num_drawn << " points drawn onto image of dims " << image.rows << "x" << image.cols << endl;
}

static const string output_dir("/tmp/easyeye_tests_rotated_shape");
static string GetOutputPathname(const string& filename) {
    string outpath(output_dir);
    outpath.append("/");
    outpath.append(filename);
    return outpath;
}

Mat CreateAndDraw(const ShapeInterface& shape, const vector<double>& params, const vector<double>& t_range, const string& filename)
{
    CPPUNIT_ASSERT(easyeye::IOUtils::MakeDirs(output_dir));
    Scalar dark_gray(0x40, 0x40, 0x40);
    Scalar green(0x0, 0xff, 0x0);
    int w = 100, h = 100; 
    Mat image(h, w, CV_8UC3, dark_gray);
    Draw(shape, params, t_range,  green, image);
    string output_pathname = GetOutputPathname(filename);
    cerr << cv::imwrite(output_pathname, image) << " " << output_pathname << endl;
    return image;
}

double Distance(const Point2d& a, const Point2d& b) {
    return std::sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
}

void AssertTolerablyEqual(const vector<Point2d> expected, const vector<Point2d>& actual, double tolerance)
{
    CPPUNIT_ASSERT_EQUAL(expected.size(), actual.size());
    vector<bool> used(actual.size(), false);
    for (size_t i = 0; i < expected.size(); i++) {
        bool any_equal = false;
        for (size_t j = 0; j < actual.size(); j++) {
            double dist = Distance(expected[i], actual[j]);
//            cerr << expected[i] << " <-> " << actual[j] << " = " << dist << endl;
            if (!used[j] && dist < tolerance) {
                used[j] = true;
                any_equal = true;
                break;
            }
        }
        if (!any_equal) {
            cerr << expected[i] << " has no matching point in actuals" << endl;
        }
        CPPUNIT_ASSERT(any_equal);
    }
    
}



}

RotatedShapeTest::RotatedShapeTest() {
}

RotatedShapeTest::~RotatedShapeTest() {
}

void RotatedShapeTest::setUp() {
}

void RotatedShapeTest::tearDown() {
}

void RotatedShapeTest::testDrawUnrotatedLine() 
{
    Line line;
    vector<double> p(4, 0.0);
    p[0] = 25; p[1] = 25; p[2] = 75; p[3] = 25;
    vector<double> t_range = ParamRange::ScaledIncremental(-1, 2, 2, 1.0);
    CreateAndDraw(line, p, t_range, "line.png");
}

void RotatedShapeTest::testDrawRotatedLine() 
{
    Line line;
    vector<double> p(5, 0.0);
    p[0] = 25; p[1] = 25; p[2] = 75; p[3] = 25;
    p[4] = M_PI_4;
    vector<double> t_range = ParamRange::ScaledIncremental(-1, 2, 2, 1.0);
    RotatedShape rotated_line(line, 4);
    
    CreateAndDraw(rotated_line, p, t_range, "rotated-line.png");
    
    vector<Point2d> unrotated_pts = ToPoints(line, p, t_range);
    cerr << "unrotated: " ;
    Print(unrotated_pts, cerr);
    cerr << endl;
    cerr << "  rotated: ";
    vector<Point2d> rotated_pts = ToPoints(rotated_line, p, t_range);
    Print(rotated_pts, cerr);
    cerr << endl;
    // first point is fixed
    CPPUNIT_ASSERT_EQUAL(unrotated_pts[0].x, rotated_pts[0].x);
    CPPUNIT_ASSERT_EQUAL(unrotated_pts[0].y, rotated_pts[0].y);
}

void RotatedShapeTest::testDrawUnrotatedRect() {
    Rectangle rectangle;
    vector<double> params(4, 0.0);
    params[0] = 10; 
    params[1] = 25;
    params[2] = 80; 
    params[3] = 50;
    vector<double> t_range = ParamRange::Incremental(0.0, 3.0, 1.0);
    CreateAndDraw(rectangle, params, t_range, "rectangle.png");
}

void RotatedShapeTest::testDrawRotatedRect() {
    Rectangle rectangle;
    rectangle.fixed_point = Rectangle::CENTER;
    vector<double> params(5, 0.0);
    double x = 40, y = 25, d = 20;
    double w = d, h = d;
    double angle_degrees = 45.0, angle_radians = M_PI_4;
    params[0] = x; 
    params[1] = y;
    params[2] = w; 
    params[3] = h;
    params[4] = angle_radians;
    vector<double> t_range = ParamRange::ScaledIncremental(0, 1, 4, 1.0);
    vector<Point2d> unrotated_pts = ToPoints(rectangle, params, t_range);
    cerr << "unrotated: ";
    Print(unrotated_pts, cerr);
    cerr << endl;
    Size size(w, h);
    CPPUNIT_ASSERT_EQUAL((size_t)4, t_range.size());
    RotatedShape rotated_rect(rectangle, 4);   
    Point2d center = rotated_rect.ComputeFixedPoint(params);
    vector<Point2d> actual = ToPoints(rotated_rect, params, t_range);
    RotatedRect cv_rotated_rect(center, size, angle_degrees);
    Point2f expected_pts_array[4];
    cv_rotated_rect.points(expected_pts_array);
    vector<Point2d> expected_pts;
    for (int i = 0; i < 4; i++) {
        Point2d p(expected_pts_array[i].x, expected_pts_array[i].y);
        expected_pts.push_back(p);
    }
    cerr << "expected: ";
    Print(expected_pts, cerr);
    cerr << endl;
    cerr << "  actual: ";
    Print(actual, cerr);
    cerr << endl;
    
    AssertTolerablyEqual(expected_pts, actual, 0.01);
    
    CreateAndDraw(rotated_rect, params, t_range, "rotated-rectangle.png");
}

