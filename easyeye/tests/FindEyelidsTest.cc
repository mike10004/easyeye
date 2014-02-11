/*
 * File:   FindEyelidsTest.cc
 * Author: mchaberski
 *
 * Created on Jan 28, 2014, 2:53:30 PM
 */

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
#include "FindEyelidsTest.h"
#include "testdata.h"
#include <iostream>
#include <string>
#include "../src/easyeye/segment/FindEyelidMix.h"
#include "../src/easyeye/segment/easyeye_segment.h"
#include "../src/easyeye/common/easyeye_utils.h"

using namespace cv;
using namespace std;
using namespace easyeye;

CPPUNIT_TEST_SUITE_REGISTRATION(FindEyelidsTest);

FindEyelidsTest::FindEyelidsTest() {
}

FindEyelidsTest::~FindEyelidsTest() {
}

void FindEyelidsTest::setUp() {
}

void FindEyelidsTest::tearDown() {
}

void FindEyelidsTest::testDetectionCorrectness() {
    int num_approx_equal = 0;
    int num_test_cases = NUM_SAMPLES;
    for (int i = 0 ; i < NUM_SAMPLES; i++) {
        Mat eye_image = imread(eye_image_files[i], CV_LOAD_IMAGE_GRAYSCALE);
        CPPUNIT_ASSERT(eye_image.data != NULL);
        Segmentation seg;
        CPPUNIT_ASSERT(serial::Deserialize(Files::Read(serialized_segmentations[i]), seg));
        EyelidFinderConfig config;
        FindEyelidMix eyelid_finder(config);
        const int max_delta[] = { 3, 3, 3, 3, 3 }; 
        const VasirEyelidsLocation& expected = static_cast<const VasirEyelidsLocation&>(seg.eyelids_location());
        cerr << i << " expected: " << expected.ToString() << endl;
        VasirEyelidsLocation actual;
        eyelid_finder.doFindPoints(eye_image, seg.boundary_pair, actual);
        cerr << i << "   actual: " << actual.ToString() << endl;
        bool approx_equal = expected.Equals(actual, max_delta, 0.05);
        if (approx_equal) {
            num_approx_equal++;
        } else {
            int ellipse_deltas[] = {expected.ellipse_vals[0] - actual.ellipse_vals[0], 
                    expected.ellipse_vals[1] - actual.ellipse_vals[1], 
                    expected.ellipse_vals[2] - actual.ellipse_vals[2], 
                    expected.ellipse_vals[3] - actual.ellipse_vals[3], 
                    expected.ellipse_vals[4] - actual.ellipse_vals[4]};
            cerr << "   delta: [" << ellipse_deltas[0] << ", " 
                    << ellipse_deltas[1] << ", " 
                    << ellipse_deltas[2] << ", " 
                    << ellipse_deltas[3] << ", " 
                    << ellipse_deltas[4] << "], "
                    << (((expected.angle - actual.angle) < 0.01) ? 0 : expected.angle - actual.angle) 
                    << ' ' << eye_image_files[i]
                    << endl;
        }
    }
    cerr << num_approx_equal << " of " << num_test_cases << " results are approx equal to expected" << endl;
    CPPUNIT_ASSERT_EQUAL(num_test_cases, num_approx_equal);
}

