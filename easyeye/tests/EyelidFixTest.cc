/*
 * File:   EyelidFixTest.cc
 * Author: mchaberski
 *
 * Created on Feb 7, 2014, 10:17:18 AM
 */

#include "EyelidFixTest.h"
#include "../src/easyeye/common/easyeye_diagnostics.h"
#include "../src/easyeye/segment/easyeye_segment.h"
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
#include "../src/easyeye/segment/FindEyelidMix.h"

using namespace std;
using namespace easyeye;
using namespace cv;

CPPUNIT_TEST_SUITE_REGISTRATION(EyelidFixTest);

EyelidFixTest::EyelidFixTest() {
}

EyelidFixTest::~EyelidFixTest() {
}

void EyelidFixTest::setUp() {
}

void EyelidFixTest::tearDown() {
}

void EyelidFixTest::testCorrectness000() {
    const string eye_image_pathname = "../testdata/images/gallery_000.bmp";
    cv::Mat eye_image = cv::imread(eye_image_pathname, CV_LOAD_IMAGE_GRAYSCALE);
    
    Diagnostician diags;
    diags.set_eye_image_pathname(eye_image_pathname);
    diags.set_output_dir("/tmp/easyeye_eyelid_fix_test");
    Segmentation segmentation;
    Segmenter segmenter;
    segmenter.set_diagnostician(&diags);
    segmenter.SegmentEyeImage(eye_image, segmentation);
    
    VasirEyelidsLocation expected; // 309 264 308 110 94 -4.65551
    int expected_ellipse_vals[] = { 309, 264, 308, 110, 94 };
    memcpy(expected.ellipse_vals, expected_ellipse_vals, 5 * sizeof(int));
    expected.angle = -4.65551;
    cerr << "expected: " << expected.ToString() << endl;
    cerr << "  actual: " << segmentation.eyelids_location().ToString() << endl;
    bool same = expected.Equals(static_cast<const VasirEyelidsLocation&>(segmentation.eyelids_location()), 3, 1.0);
    CPPUNIT_ASSERT(same);
}
