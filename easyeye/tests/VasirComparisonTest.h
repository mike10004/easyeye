/*
 * File:   VasirComparisonTest.h
 * Author: mike
 *
 * Created on Feb 4, 2014, 9:26:37 AM
 */

#ifndef VASIRCOMPARISONTEST_H
#define	VASIRCOMPARISONTEST_H

#include <cppunit/extensions/HelperMacros.h>

class VasirComparisonTest : public CPPUNIT_NS::TestFixture {
    CPPUNIT_TEST_SUITE(VasirComparisonTest);

    //CPPUNIT_TEST(testMethod);
    CPPUNIT_TEST(testAgainstVasirResults);

    CPPUNIT_TEST_SUITE_END();

public:
    VasirComparisonTest();
    virtual ~VasirComparisonTest();
    void setUp();
    void tearDown();

private:
    //void testMethod();
    void testAgainstVasirResults();
};

#endif	/* VASIRCOMPARISONTEST_H */

