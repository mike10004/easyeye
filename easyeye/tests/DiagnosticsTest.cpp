/*
 * File:   DiagnosticsTest.cpp
 * Author: mike
 *
 * Created on Jan 5, 2014, 4:10:33 PM
 */

#include "DiagnosticsTest.h"
#include "../src/easyeye/segment/easyeye_segment.h"
#include "../src/easyeye/common/easyeye_diagnostics.h"
#include "testdata.h"
#include <iostream>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
#include <vector>
#include <string>

using std::string;
using std::vector;
using cv::Mat;
using namespace easyeye;
using namespace std;

CPPUNIT_TEST_SUITE_REGISTRATION(DiagnosticsTest);

DiagnosticsTest::DiagnosticsTest() {
}

DiagnosticsTest::~DiagnosticsTest() {
}

void DiagnosticsTest::setUp() {
}

void DiagnosticsTest::tearDown() {
}

void testSegment(const char* eyeImagePathname) 
{
    Segmentation result;
    Segmenter segmenter;
    Mat eyeImage = cv::imread(eyeImagePathname, CV_LOAD_IMAGE_GRAYSCALE);
    segmenter.SegmentEyeImage(eyeImage, result);
    result.Describe(cout);
    cerr << "\t" << eyeImagePathname;
    cerr << endl;
    if (result.status != Result::SUCCESS) {
        cerr << "%TEST_FAILED% time=0 testname=testSegment (diagnostics) message=failed to segment " << eyeImagePathname << endl;
    } else {
        Diagnostician diag;
        diag.set_eye_image_pathname(eyeImagePathname);
        diag.set_output_dir("/tmp/easyeye_tests_diagnostics");
        diag.set_collect_pathnames(true);
        diag.DumpSegOutput(result.boundary_pair, result.eyelids_location, result.extrema_noise);
        vector<string> files = diag.files_written();
        cerr << "wrote " << files.size() << " files" << endl;
        for (vector<string>::iterator it = files.begin(); it != files.end(); ++it) {
            cerr << "  " << *it << endl;
        }
    }
    
    
}



void DiagnosticsTest::testWriteSegmentationOutput() {
    const int nimages = 3;
    for (int i = 0; i < nimages; i++) {
        testSegment(eye_image_files[i]);
    }
}

void DiagnosticsTest::testWriteNormalizationOutput() {
    CPPUNIT_ASSERT(true);
}

