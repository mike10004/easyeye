/*
 * File:   DualParabolaEyelidFinderTest.h
 * Author: mike
 *
 * Created on Feb 12, 2014, 10:38:45 PM
 */

#ifndef DUALPARABOLAEYELIDFINDERTEST_H
#define	DUALPARABOLAEYELIDFINDERTEST_H

#include <cppunit/extensions/HelperMacros.h>

class DualParabolaEyelidFinderTest : public CPPUNIT_NS::TestFixture {
    CPPUNIT_TEST_SUITE(DualParabolaEyelidFinderTest);

    CPPUNIT_TEST(testMethod);
    CPPUNIT_TEST(testFailedMethod);

    CPPUNIT_TEST_SUITE_END();

public:
    DualParabolaEyelidFinderTest();
    virtual ~DualParabolaEyelidFinderTest();
    void setUp();
    void tearDown();

private:
    void testMethod();
    void testFailedMethod();
};

#endif	/* DUALPARABOLAEYELIDFINDERTEST_H */

