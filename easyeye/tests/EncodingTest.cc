/*
 * File:   EncodingTest.cc
 * Author: ibgmike
 *
 * Created on Jan 26, 2014, 12:12:47 PM
 */

#include "../src/easyeye/common/mylog.h"
#include <ctime>
#include <cstdlib>
#include "EncodingTest.h"
#include <fstream>

#include "../src/easyeye/encode/easyeye_encode.h"
#include "../src/easyeye/segment/easyeye_segment.h"
#include <string>
#include <vector>
#include <sstream>
#include <iostream>
#include <fstream>
#include <string>
#include <iostream>
#include "../src/easyeye/common/easyeye_types.h"
#include "../src/easyeye/segment/easyeye_segment.h"
#include <opencv2/highgui/highgui.hpp>
#include <jsoncpp/json/json.h>
#include "../src/easyeye/encode/easyeye_encode.h"
#include "testdata.h"

using namespace easyeye;
using namespace std;
using cv::Mat;

CPPUNIT_TEST_SUITE_REGISTRATION(EncodingTest);

static bool write_encodings = false;

static void testSerializeAndDeserialize(Encoding* encoding)
{
    string json = serial::Serialize(*encoding);
    Encoding deserialized;
    serial::Deserialize(json, deserialized);
    CPPUNIT_ASSERT(deserialized.status == Result::SUCCESS);
    CPPUNIT_ASSERT(encoding->Equals(deserialized));
}

EncodingTest::EncodingTest() {
}

EncodingTest::~EncodingTest() {
}

void EncodingTest::setUp() {
    mylog::Logs::GetLogger().set_level(mylog::TRACE);
}

void EncodingTest::tearDown() {
}

static void PopulateAtRandom(Encoding& encoding)
{
    size_t len = encoding.width() * encoding.height();
    srand(0x12345);
    for (size_t i = 0; i < len; i++) {
        encoding.irisMask[i] = rand() % 2;
        encoding.irisTemplate[i] = rand() % 2;
    }
    encoding.status = Result::SUCCESS;
}

void EncodingTest::testSerializeAndDeserializeRandomEncoding() {
    Encoder encoder;
    Encoding encoding(encoder.config().encodeScales, encoder.config().angularRes, encoder.config().radialRes);
    PopulateAtRandom(encoding);
    testSerializeAndDeserialize(&encoding);
}

void testEncodeFromSegmentation(Segmentation& segmentation, int index)
{
    const char* eye_image_pathname = eye_image_files[index];
    Diagnostician diags;
    diags.set_eye_image_pathname(eye_image_pathname);
    diags.set_output_dir("/tmp/easyeye_tests_encoding");
    Encoder encoder;
    encoder.set_diagnostician(&diags);
    Normalization norm(encoder.config());
    Mat eye_image = cv::imread(eye_image_pathname);
    CPPUNIT_ASSERT(eye_image.data != NULL);
    encoder.NormalizeIris(eye_image, segmentation, norm);
    cerr << eye_image_pathname << "segmentation normalized: " << Result::DescribeStatus(norm.status) << endl;
    CPPUNIT_ASSERT_EQUAL(Result::SUCCESS, norm.status);
    Encoding encoding(encoder.config());
    encoder.EncodeIris(norm, encoding);
    cerr << eye_image_pathname << "normalization encoded: " << Result::DescribeStatus(encoding.status) << endl;
    CPPUNIT_ASSERT_EQUAL(Result::SUCCESS, encoding.status);
    if (write_encodings) {
        bool written = Files::Write(serial::Serialize(encoding), serialized_encodings[index]);
        cerr << ">>> wrote " << serialized_encodings[index] << " success = " << written << endl;
        CPPUNIT_ASSERT(written);
    }
}

void EncodingTest::testEncodeFromSegmentations()
{
    serial::SegmentationAdapter adapter;
    for (int i = 0; i < NUM_SAMPLES; i++) {
        Segmentation seg;
        string json = Files::Read(serialized_segmentations[i]);
        bool deserialized_ok = serial::Deserialize(json, &adapter, seg);
        cerr << "deserialized " << serialized_segmentations[i] << " successfully: " << deserialized_ok << endl;
        CPPUNIT_ASSERT(deserialized_ok);
        testEncodeFromSegmentation(seg, i);
    }
    
    cerr << NUM_SAMPLES << " segmentation serializations successfully encoded " << endl;
}