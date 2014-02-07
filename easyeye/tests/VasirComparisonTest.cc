/*
 * File:   VasirComparisonTest.cc
 * Author: mike
 *
 * Created on Feb 4, 2014, 9:26:38 AM
 */

#include "VasirComparisonTest.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <opencv2/highgui/highgui.hpp>
#include "../src/easyeye/common/easyeye_types.h"
#include "../src/easyeye/segment/easyeye_segment.h"

using namespace std;
using namespace easyeye;
using namespace cv;

CPPUNIT_TEST_SUITE_REGISTRATION(VasirComparisonTest);

vector<string> VasirComparisonTest::image_filenames;
vector<Segmentation> VasirComparisonTest::expecteds;
vector<Segmentation> VasirComparisonTest::actuals;

VasirComparisonTest::VasirComparisonTest() {
}

VasirComparisonTest::~VasirComparisonTest() {
}

void VasirComparisonTest::setUp() {
}

void VasirComparisonTest::tearDown() {
}

class SegResult {
public:
    
    string image_filename;
    BoundaryPair bp;
    int eyelid1, eyelid2, eyelid3, eyelid4, eyelid5;
    double eyelid_angle;
    void Print(ostream& out)
    {
        out << image_filename << ' ';
        bp.Describe(out);
        out << ' ' << eyelid1 << ' '
                << eyelid2 << ' ' << eyelid3 << ' ' << eyelid4 << ' ' << eyelid5 
                << ' ' << eyelid_angle;
        out << endl;
                
    }
};

void VasirComparisonTest::ReadExpecteds() {
    string expecteds_file("../testdata/vasir-segmentation-results.ssv");
    if (!image_filenames.empty()) {
        cerr << "expected segmentations already read from " << expecteds_file << endl;
        return;
    }
    ifstream in(expecteds_file.c_str());
    CPPUNIT_ASSERT (in.is_open());
    string line;
    while (!in.eof()) {
        SegResult r;
        getline(in, line, '\n');
//        cerr << "input: \"" << line << "\"" << endl;
        istringstream line_in(line);
        line_in >> r.image_filename 
                >> r.bp.pupil.center.x >> r.bp.pupil.center.y >> r.bp.pupil.radius
                >> r.bp.iris.center.x >> r.bp.iris.center.y >> r.bp.iris.radius
                >> r.eyelid1 >> r.eyelid2 >> r.eyelid3 >> r.eyelid4 >> r.eyelid5
                >> r.eyelid_angle;
//        cerr << "output: ";
//        r.Print(cerr);
        if (!line.empty()) {
            Segmentation s;
            s.boundary_pair.set_iris(r.bp.iris);
            s.boundary_pair.set_pupil(r.bp.pupil);
            s.eyelids_location.ellipse_vals[0] = r.eyelid1;
            s.eyelids_location.ellipse_vals[1] = r.eyelid2;
            s.eyelids_location.ellipse_vals[2] = r.eyelid3;
            s.eyelids_location.ellipse_vals[3] = r.eyelid4;
            s.eyelids_location.ellipse_vals[4] = r.eyelid5;
            s.eyelids_location.angle = r.eyelid_angle;
            expecteds.push_back(s);
            image_filenames.push_back(r.image_filename);
        }
    }
    cerr << expecteds.size() << " expected segmentations read from " << expecteds_file << endl;
} 

static const int max_ellipse_deltas[] = { 5, 5, 5, 5, 5 };
static const double max_angle_delta = 1.5;
static const double max_center_offset = 3.0;
static const int max_radius_difference = 3;
static const IntCircle::Delta max_delta(max_center_offset, max_radius_difference);

bool Compare(const EyelidsLocation& expected, const EyelidsLocation& actual)
{
    return expected.Equals(actual, max_ellipse_deltas, max_angle_delta);
}

bool Compare(BoundaryPair& expected, BoundaryPair& actual)
{
    return expected.Equals(actual, max_delta, max_delta);
}

void VasirComparisonTest::PerformSegmentation() 
{
    if (!actuals.empty()) {
        cerr << "actual segmentations already performed for " << expecteds.size() << " image files" << endl;
        return;
    }
    for (size_t i = 0; i < image_filenames.size(); i++) {
        string image_pathname("../testdata/images/");
        image_pathname.append(image_filenames[i]);
        cerr << "testing against " << image_pathname << endl;
        Segmentation result;
        Segmenter segmenter;
        Mat eye_image = cv::imread(image_pathname, CV_LOAD_IMAGE_GRAYSCALE);
        if (eye_image.data == NULL) {
            cerr << "  file not found: " << image_pathname << endl;
            continue;
        }
        segmenter.SegmentEyeImage(eye_image, result);
        actuals.push_back(result);
        cerr << "    ";
        result.Describe(cerr);
        cerr << endl;
    }
}

void Print(ostream& out, const string& tag, const BoundaryPair& boundary_pair)
{
        out << tag
                << ' ' << boundary_pair.iris.center.x 
                << ' ' << boundary_pair.iris.center.y
                << ' ' << boundary_pair.iris.radius
                << ' ' << boundary_pair.pupil.center.y
                << ' ' << boundary_pair.pupil.center.y
                << ' ' << boundary_pair.pupil.radius
                << endl;
}

void Print(ostream& out, const string& tag, const EyelidsLocation& eyelids_location)
{
    out << tag
            << ' ' << eyelids_location.ellipse_vals[0]
            << ' ' << eyelids_location.ellipse_vals[1]
            << ' ' << eyelids_location.ellipse_vals[2]
            << ' ' << eyelids_location.ellipse_vals[3]
            << ' ' << eyelids_location.ellipse_vals[4]
            << ' ' << eyelids_location.angle
            << endl;
}

void PrintDeltas(ostream& out, const string& tag, const EyelidsLocation& a, const EyelidsLocation& b)
{
    out << tag ;
    for (int i = 0; i < EyelidsLocation::NUM_ELLIPSE_VALS; i++) {
        out    << ' ' << (a.ellipse_vals[i] - b.ellipse_vals[i]);
    }
    out << ' ' << (a.angle - b.angle) << endl;
}

void VasirComparisonTest::testEyelidsLocations()
{
    cerr << "testEyelidLocations: " << expecteds.size() << " expected and " << actuals.size() << " actual segmentations" << endl;
    ReadExpecteds();
    PerformSegmentation();
    CPPUNIT_ASSERT_EQUAL(expecteds.size(), actuals.size());
    int num_diff = 0;
    for (size_t i = 0; i < expecteds.size(); i++) {
        Print(cerr, "expected:", expecteds[i].eyelids_location);
        Print(cerr, "  actual:", actuals[i].eyelids_location);
        PrintDeltas(cerr, "   delta:", expecteds[i].eyelids_location, actuals[i].eyelids_location);
        bool same = Compare(expecteds[i].eyelids_location, actuals[i].eyelids_location);
        if (!same) {
            num_diff++;
        }
    }
    cerr << num_diff << " of " << expecteds.size() << " eyelid location results differ from expected" << endl;
    CPPUNIT_ASSERT_EQUAL(0, num_diff);
}

void VasirComparisonTest::testIrisBoundaryLocations()
{
    cerr << "testIrisBoundaryLocations: " << expecteds.size() << " expected and " << actuals.size() << " actual segmentations" << endl;
    ReadExpecteds();
    PerformSegmentation();
    CPPUNIT_ASSERT_EQUAL(expecteds.size(), actuals.size());
    int num_diff = 0;
    for (size_t i = 0; i < expecteds.size(); i++) {
        Print(cerr, "expected:", expecteds[i].boundary_pair);
        Print(cerr, "  actual:", actuals[i].boundary_pair);
        cerr << "  deltas: " << expecteds[i].boundary_pair.iris.ComputeDelta(actuals[i].boundary_pair.iris).ToString()
                << ' ' << expecteds[i].boundary_pair.pupil.ComputeDelta(actuals[i].boundary_pair.pupil).ToString() << endl;
        bool same = Compare(expecteds[i].boundary_pair, actuals[i].boundary_pair);
        if (!same) {
            num_diff++;
        }
    }
    cerr << num_diff << " of " << expecteds.size() << " boundary pair results differ from expected" << endl;
    CPPUNIT_ASSERT_EQUAL(0, num_diff);
}

