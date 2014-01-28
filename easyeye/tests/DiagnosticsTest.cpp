/*
 * File:   DiagnosticsTest.cpp
 * Author: mike
 *
 * Created on Jan 5, 2014, 4:10:33 PM
 */

#include "DiagnosticsTest.h"
#include "../src/easyeye/segment/easyeye_segment.h"
#include "../src/easyeye/common/easyeye_diagnostics.h"
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
        cerr << "%TEST_FAILED% time=0 testname=testSegmentEyeImage (ezisegment_test) message=failed to segment " << eyeImagePathname << endl;
    } else {
        Diagnostician diag(eyeImagePathname);
        diag.set_output_dir("/tmp");
        diag.set_collect_pathnames(true);
        diag.DumpSegOutput(result.boundary_pair, result.eyelids_location, result.extrema_noise);
        vector<string> files = diag.files_written();
        for (vector<string>::iterator it = files.begin(); it != files.end(); ++it) {
            cerr << "wrote " << *it << endl;
        }
    }
    
}



void DiagnosticsTest::testWriteSegmentationOutput() {
    const char* eyeImagePathnames[] = {
        "../testdata/images/probes_006.bmp",
        "../testdata/images/probes_010.bmp",
        "../testdata/images/probes_013.bmp",
        "../testdata/images/probes_016.bmp",
        "../testdata/images/probes_022.bmp"
    };
//    const char* eyeImagePathnames[] = {
//        "/tmp/seeks/1.bmp",
//        "/tmp/seeks/2.bmp",
//        "/tmp/seeks/3.bmp",
//        "/tmp/seeks/4.bmp",
//        "/tmp/seeks/5.bmp"
//    };
    const int nimages = 5;
    for (int i = 0; i < nimages; i++) {
        testSegment(eyeImagePathnames[i]);
    }
}

void DiagnosticsTest::testWriteNormalizationOutput() {
    CPPUNIT_ASSERT(true);
}

