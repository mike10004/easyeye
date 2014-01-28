/*
 * File:   CommonSerialTest.cc
 * Author: mchaberski
 *
 * Created on Jan 28, 2014, 11:34:54 AM
 */

#include <string>
#include "CommonSerialTest.h"
#include "../src/easyeye/common/easyeye_serial.h"
#include <iostream>
#include <opencv2/core/core.hpp>
#include <jsoncpp/json/json.h>

using namespace easyeye;
using namespace std;
using namespace easyeye::serial;

CPPUNIT_TEST_SUITE_REGISTRATION(CommonSerialTest);

CommonSerialTest::CommonSerialTest() {
}

CommonSerialTest::~CommonSerialTest() {
}

void CommonSerialTest::setUp() {
}

void CommonSerialTest::tearDown() {
}

void CommonSerialTest::testMethod() {
    
    cv::Point2i p(3, 4);
    PointAdapter pa;
    string json = serial::Serialize(p, &pa);
    cerr << "serialized:" << endl;
    cerr << json << endl;
    
    cv::Point2i q;
    serial::Deserialize(json, &pa, q);
    cerr << "deserialized:" << endl;
    cerr << "x = " << q.x << " y = " << q.y << endl;
    
    CPPUNIT_ASSERT_EQUAL(p.x, q.x);
    CPPUNIT_ASSERT_EQUAL(p.y, q.y);
}

void CommonSerialTest::testFailedMethod() {
    
}

