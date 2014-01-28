/*
 * File:   SegmentationSerializerTest.cc
 * Author: ibgmike
 *
 * Created on Jan 26, 2014, 4:32:40 PM
 */

#include <algorithm>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <json/json_object.h>
#include <json/json_tokener.h>
#include "SegmentationSerializerTest.h"
#include "../src/easyeye/segment/easyeye_segment.h"
#include "../src/easyeye/common/mylog.h"

using namespace easyeye;
using namespace std;

CPPUNIT_TEST_SUITE_REGISTRATION(SegmentationSerializerTest);

SegmentationSerializerTest::SegmentationSerializerTest() {
}

SegmentationSerializerTest::~SegmentationSerializerTest() {
}

void SegmentationSerializerTest::setUp() {
}

void SegmentationSerializerTest::tearDown() {
}

void SegmentationSerializerTest::testSerializeAndDeserialize() {
    mylog::SetLogLevel(mylog::TRACE);
    Segmenter segmenter;
    Segmentation expected;
    cv::Mat eyeImage = cv::imread("../testdata/images/gallery_022.bmp", CV_LOAD_IMAGE_GRAYSCALE);
    segmenter.SegmentEyeImage(eyeImage, expected);
    cerr << "expected:" << endl;
    expected.Describe(cerr);
    cerr << endl;
    
    string json = serial::Serialize(expected);
    cerr << "serialized into " << json.length() << " characters" << endl;
    cerr << json.substr(0, std::min(json.length(), (size_t)64)) << endl;
    Segmentation actual;
    bool ok = serial::Deserialize(json, actual);
    cerr << "actual:" << endl;
    actual.Describe(cerr);
    cerr << endl;
    CPPUNIT_ASSERT(ok);
    CPPUNIT_ASSERT(expected.boundary_pair.Equals(actual.boundary_pair));
    CPPUNIT_ASSERT(expected.eyelids_location.Equals(actual.eyelids_location));
    CPPUNIT_ASSERT_EQUAL(expected.status, actual.status);
}

