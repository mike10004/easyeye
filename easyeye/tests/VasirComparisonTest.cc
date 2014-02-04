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
    IntCircle pupil2;
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

vector<SegResult> ReadExpecteds() {
    ifstream in("../testdata/vasir-segmentation-results.ssv");
    CPPUNIT_ASSERT (in.is_open());
    vector<SegResult> expecteds;
    string line;
    while (!in.eof()) {
        SegResult r;
        getline(in, line, '\n');
        cerr << "input: \"" << line << "\"" << endl;
        istringstream line_in(line);
        line_in >> r.image_filename 
                >> r.bp.irisX >> r.bp.irisY >> r.bp.irisR
                >> r.bp.pupilX >> r.bp.pupilY >> r.bp.pupilR
                >> r.pupil2.center.x >> r.pupil2.center.y >> r.pupil2.radius 
                >> r.eyelid1 >> r.eyelid2 >> r.eyelid3 >> r.eyelid4 >> r.eyelid5
                >> r.eyelid_angle;
        cerr << "output: ";
        r.Print(cerr);
        if (!line.empty()) {
            expecteds.push_back(r);
        }
    }
    return expecteds;
} 

bool Compare(const IntCircle& a, const IntCircle& b, double center_tolerance, int radius_tolerance)
{
    double distsq = (a.center.x - b.center.x) * (a.center.x - b.center.x)
            + (a.center.y - b.center.y) * (a.center.y + b.center.y);
    if (distsq > (center_tolerance * center_tolerance)) {
        return false;
    }
    return abs(a.radius - b.radius) <= radius_tolerance;
}

bool Compare(SegResult& expected, Segmentation& actual)
{
    double center_tolerance = 10.0;
    int radius_tolerance = 6;
    return Compare(expected.bp.iris(), actual.boundary_pair.iris(), center_tolerance, radius_tolerance)
            && Compare(expected.bp.pupil(), actual.boundary_pair.pupil(), center_tolerance, radius_tolerance);
}

void VasirComparisonTest::testAgainstVasirResults() {
    //CPPUNIT_ASSERT(false);
    vector<SegResult> expecteds = ReadExpecteds();
    int num_diff = 0;
    for (size_t i = 0; i < expecteds.size(); i++) {
        string image_pathname("../testdata/images/");
        image_pathname.append(expecteds[i].image_filename);
        cerr << "testing against " << image_pathname << endl;
        Segmentation result;
        Segmenter segmenter;
        Mat eye_image = cv::imread(image_pathname, CV_LOAD_IMAGE_GRAYSCALE);
        if (eye_image.data == NULL) {
            cerr << "  file not found: " << image_pathname << endl;
            continue;
        }
        segmenter.SegmentEyeImage(eye_image, result);
        cerr << "  expected: " ;
        expecteds[i].Print(cerr);
        cerr << "    actual: ";
        result.Describe(cerr);
        cerr << endl;
        bool same = Compare(expecteds[i], result);
        cerr << "     same: " << same << endl;
        if (!same) {
            num_diff++;
        }
    }
    cerr << num_diff << " results differ from expected" << endl;
    CPPUNIT_ASSERT_EQUAL(0, num_diff);
}

