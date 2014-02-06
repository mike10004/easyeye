/*
 * File:   VasirComparisonTest.h
 * Author: mike
 *
 * Created on Feb 4, 2014, 9:26:37 AM
 */

#ifndef VASIRCOMPARISONTEST_H
#define	VASIRCOMPARISONTEST_H

#include <vector>
#include "../src/easyeye/segment/easyeye_segment.h"
#include <cppunit/extensions/HelperMacros.h>

class VasirComparisonTest : public CPPUNIT_NS::TestFixture {
    CPPUNIT_TEST_SUITE(VasirComparisonTest);

    CPPUNIT_TEST(testIrisBoundaryLocations);
    CPPUNIT_TEST(testEyelidsLocations);

    CPPUNIT_TEST_SUITE_END();

public:
    VasirComparisonTest();
    virtual ~VasirComparisonTest();
    void setUp();
    void tearDown();

private:
    //void testMethod();
    static std::vector<std::string> image_filenames;
    static std::vector<easyeye::Segmentation> expecteds;
    static std::vector<easyeye::Segmentation> actuals;
    void ReadExpecteds();
    void PerformSegmentation();
    void testIrisBoundaryLocations();
    void testEyelidsLocations();
};

#endif	/* VASIRCOMPARISONTEST_H */

