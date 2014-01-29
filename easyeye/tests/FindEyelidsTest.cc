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
    for (int i = 0 ; i < NUM_SAMPLES; i++) {
        Mat eye_image = imread(eye_image_files[i], CV_LOAD_IMAGE_GRAYSCALE);
        CPPUNIT_ASSERT(eye_image.data != NULL);
        Segmentation seg;
        CPPUNIT_ASSERT(serial::Deserialize(Files::Read(serialized_segmentations[i]), seg));
        EyelidFinderConfig config;
        FindEyelidMix eyelid_finder(config);
        EyelidsLocation& expected = seg.eyelids_location;
        cerr << i << " expected: " << expected.ToString() << endl;
        EyelidsLocation actual;
        eyelid_finder.doFindPoints(eye_image, seg.boundary_pair, actual);
        cerr << i << "   actual: " << actual.ToString() << endl;
        CPPUNIT_ASSERT(expected.EqualsApprox(actual));
    }
}

