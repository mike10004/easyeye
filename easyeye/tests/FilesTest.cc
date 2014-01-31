/*
 * File:   FilesTest.cc
 * Author: mike
 *
 * Created on Jan 31, 2014, 10:25:33 AM
 */

#include "FilesTest.h"
#include <vector>
#include <string>
#include "../src/easyeye/common/easyeye_utils.h"

using namespace easyeye;
using namespace std;

CPPUNIT_TEST_SUITE_REGISTRATION(FilesTest);

static bool Eq(const string& a, const string& b) 
{
    return a.compare(b) == 0;
}


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

static vector<PathInfo> GetTestCases()
{
    // format: path, dirname, basename, stem, extension
    PathInfo a("/usr/lib", "/usr", "lib", "lib", "");
    PathInfo b("/usr/", "/", "usr", "usr", "");
    PathInfo c("usr", ".", "usr", "usr", "");
    PathInfo d("/",   "/", "/",   "/", "");
    PathInfo e("a/b/c.txt", "a/b", "c.txt", "c", "txt");
    PathInfo f("a.txt", ".", "a.txt", "a", "txt");
    PathInfo g("a/b/c", "a/b", "c", "c", "");
    PathInfo h("a/b/c/", "a/b", "c", "c", "");
    PathInfo l("a/b/c.jpg", "a/b", "c.jpg", "c", "jpg");
    PathInfo m("a/b.txt/c", "a/b.txt", "c", "c", "");
    PathInfo n("/a/b/c.txt", "/a/b", "c.txt", "c", "txt");
    PathInfo q("a/b/.c", "a/b", ".c", ".c", "");
    PathInfo r("a/b/c.", "a/b", "c.", "c", "");
    PathInfo s(".", ".", ".", ".", "");
    PathInfo t("..", ".", "..", "..", "");
    PathInfo u("........", ".", "........", "........", "");
//       {".", ".", ".", "", ""},
//       {"..", ".", "..", "", ""},
    
    vector<PathInfo> test_cases;
    test_cases.push_back(a);
    test_cases.push_back(b);
    test_cases.push_back(c);
    test_cases.push_back(d);
    test_cases.push_back(e);
    test_cases.push_back(f);
    test_cases.push_back(g);
    test_cases.push_back(h);
    test_cases.push_back(l);
    test_cases.push_back(m);
    test_cases.push_back(n);
    test_cases.push_back(q);
    test_cases.push_back(r);
    test_cases.push_back(s);
    test_cases.push_back(t);
    test_cases.push_back(u);
    return test_cases;
}

//Filename
//a/b/c.txt --> c.txt
// a.txt     --> a.txt
// a/b/c     --> c
// a/b/c/    --> ""

// Name without extension
//a/b/c.txt --> c
// a.txt     --> a
// a/b/c     --> c
// a/b/c/    --> ""

//foo.txt      --> "txt"
// a/b/c.jpg    --> "jpg"
// a/b.txt/c    --> ""
// a/b/c        --> ""

FilesTest::FilesTest() {
}

FilesTest::~FilesTest() {
}

void FilesTest::setUp() {
}

void FilesTest::tearDown() {
}

void FilesTest::testExtension() {
    vector<PathInfo> test_cases = GetTestCases();
    int num_diff = 0;
    for (vector<PathInfo>::iterator it = test_cases.begin(); it != test_cases.end(); ++it) {
        PathInfo& expected = *it;
        cerr << endl << "filename_extension of \"" << expected.path << "\"" << endl;
        cerr << "expected: " << expected.filename_extension  << endl;
        PathInfo actual;
        Files::ParsePathInfo(expected.path, actual);
        cerr << "  actual: " << actual.filename_extension << endl;
        bool equiv = Eq(expected.filename_extension, actual.filename_extension);
        cerr << "   equal: " << equiv << endl;
        if (!equiv) num_diff++;
        
    }
    CPPUNIT_ASSERT_EQUAL(0, num_diff);
}

void FilesTest::testNameWithoutExtension() {
    vector<PathInfo> test_cases = GetTestCases();
    int num_diff = 0;
    for (vector<PathInfo>::iterator it = test_cases.begin(); it != test_cases.end(); ++it) {
        PathInfo& expected = *it;
        cerr << endl << "filename_stem of \"" << expected.path << "\"" << endl;
        cerr << "expected: " << expected.filename_stem << endl;
        PathInfo actual;
        Files::ParsePathInfo(expected.path, actual);
        cerr << "  actual: " << actual.filename_stem << endl;
        bool equiv = Eq(expected.filename_stem, actual.filename_stem);
        cerr << "   equal: " << equiv << endl;
        if (!equiv) num_diff++;
        
    }
    CPPUNIT_ASSERT_EQUAL(0, num_diff);
}

void FilesTest::testAbsolute() {
    CPPUNIT_ASSERT(true);
}

void FilesTest::testDirname() {
    vector<PathInfo> test_cases = GetTestCases();
    int num_diff = 0;
    for (vector<PathInfo>::iterator it = test_cases.begin(); it != test_cases.end(); ++it) {
        PathInfo& expected = *it;
        cerr << endl << "dirname of \"" << expected.path << "\"" << endl;
        cerr << "expected: " << expected.dirname << endl;
        PathInfo actual;
        Files::ParsePathInfo(expected.path, actual);
        cerr << "  actual: " << actual.dirname << endl;
        bool equiv = Eq(expected.dirname, actual.dirname);
        cerr << "   equal: " << equiv << endl;
        if (!equiv) num_diff++;
        
    }
    CPPUNIT_ASSERT_EQUAL(0, num_diff);
}


void FilesTest::testBasename() {
    vector<PathInfo> test_cases = GetTestCases();
    int num_diff = 0;
    for (vector<PathInfo>::iterator it = test_cases.begin(); it != test_cases.end(); ++it) {
        PathInfo& expected = *it;
        cerr << endl << "filename of \"" << expected.path << "\"" << endl;
        cerr << "expected: " << expected.filename << endl;
        PathInfo actual;
        Files::ParsePathInfo(expected.path, actual);
        cerr << "  actual: " << actual.filename << endl;
        bool equiv = Eq(expected.filename, actual.filename);
        cerr << "   equal: " << equiv << endl;
        if (!equiv) num_diff++;
        
    }
    CPPUNIT_ASSERT_EQUAL(0, num_diff);
}


