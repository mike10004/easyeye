/*
 * File:   FilesTest.cc
 * Author: mike
 *
 * Created on Jan 31, 2014, 10:25:33 AM
 */

#include "FilesTest.h"


CPPUNIT_TEST_SUITE_REGISTRATION(FilesTest);

// format:
//   {"path", "dirname", "basename", "extension", "nameWithoutExtension"},

//vector< vector<string> > get_test_cases() {
//
//       {"/usr/lib", "/usr", "lib", "", ""},
//       {"/usr/", "/", "usr", "", ""},
//       {"usr", ".", "usr", "", ""},
//       {"/", "/", "/", "", ""},
//       {".", ".", ".", "", ""},
//       {"..", ".", "..", "", ""},
//       {0, 0, 0, 0, 0, 0}
//};


FilesTest::FilesTest() {
}

FilesTest::~FilesTest() {
}

void FilesTest::setUp() {
}

void FilesTest::tearDown() {
}

void FilesTest::testExtension() {
    CPPUNIT_ASSERT(true);
}

void FilesTest::testNameWithoutExtension() {
    CPPUNIT_ASSERT(true);
}

void FilesTest::testAbsolute() {
    CPPUNIT_ASSERT(true);
}

void FilesTest::testDirname() {
    CPPUNIT_ASSERT(true);
}

void FilesTest::testBasename() {
    CPPUNIT_ASSERT(true);
}

