/*
 * File:   DualParabolaEyelidFinderTest.cc
 * Author: mike
 *
 * Created on Feb 12, 2014, 10:38:46 PM
 */

#include <opencv2/highgui/highgui_c.h>

#include "DualParabolaEyelidFinderTest.h"
#include "testdata.h"
#include "../src/easyeye/segment/easyeye_eyelid_detection.h"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "../src/easyeye/common/easyeye_serial.h"
#include "../src/easyeye/common/easyeye_types.h"
#include "../src/easyeye/common/easyeye_utils.h"
#include "src/easyeye/segment/easyeye_segment.h"
#include <string>
#include "../src/easyeye/common/easyeye_diagnostics.h"
#include "src/easyeye/common/mylog.h"

using namespace cv;
using namespace easyeye;
using namespace std;

CPPUNIT_TEST_SUITE_REGISTRATION(DualParabolaEyelidFinderTest);

DualParabolaEyelidFinderTest::DualParabolaEyelidFinderTest() {
}

DualParabolaEyelidFinderTest::~DualParabolaEyelidFinderTest() {
}

void DualParabolaEyelidFinderTest::setUp() {
}

void DualParabolaEyelidFinderTest::tearDown() {
}

namespace 
{
    
const char* diagnostics_dir = "/tmp/easyeye_tests_dual_parabola";    
    
void testDetectEyelids(const string& eye_image_pathname, const cv::Mat& eye_image, const Segmentation& segmentation) 
{
    Diagnostician diags;
    diags.set_write_original(true);
    diags.set_output_dir(diagnostics_dir);
    diags.set_eye_image_pathname(eye_image_pathname);
    CPPUNIT_ASSERT(IOUtils::MakeDirs(diagnostics_dir));
    EyelidFinderConfig config;
    DualParabolaEyelidFinder upper_finder(config, DualParabolaEyelidFinder::EYELID_UPPER);
    upper_finder.set_diagnostician(&diags);
    DualParabolaEyelidFinder lower_finder(config, DualParabolaEyelidFinder::EYELID_LOWER);
    lower_finder.set_diagnostician(&diags);
    EyelidBoundary upper_boundary = upper_finder.FindEyelid(eye_image, segmentation);
    EyelidBoundary lower_boundary = lower_finder.FindEyelid(eye_image, segmentation);
}
}

void DualParabolaEyelidFinderTest::testDetectEyelidsAsDualParabolas() {
    mylog::Logs::GetLogger().set_level(mylog::DEBUG);
    for (int i = 0; i < NUM_SAMPLES; i++) {
        string eye_image_pathname(eye_image_files[i]);
        Mat eye_image = cv::imread(eye_image_pathname, CV_LOAD_IMAGE_GRAYSCALE);
        cerr <<  "read data from " << eye_image_pathname << endl;
        CPPUNIT_ASSERT(eye_image.data != NULL);
        string segmentation_json = Files::Read(serialized_segmentations[i]);
        Segmentation segmentation;
        CPPUNIT_ASSERT(serial::Deserialize(segmentation_json, segmentation));
        testDetectEyelids(eye_image_pathname, eye_image, segmentation);
    }
}


