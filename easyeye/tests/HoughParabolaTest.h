/*
 * File:   HoughParabolaTest.h
 * Author: mchaberski
 *
 * Created on Feb 7, 2014, 12:10:37 PM
 */

#ifndef HOUGHPARABOLATEST_H
#define	HOUGHPARABOLATEST_H

#include <cppunit/extensions/HelperMacros.h>

class HoughParabolaTest : public CPPUNIT_NS::TestFixture {
    CPPUNIT_TEST_SUITE(HoughParabolaTest);

    CPPUNIT_TEST(testComputeTransform);

    CPPUNIT_TEST_SUITE_END();

public:
    HoughParabolaTest();
    virtual ~HoughParabolaTest();
    void setUp();
    void tearDown();

private:
    void testComputeTransform();
};

#endif	/* HOUGHPARABOLATEST_H */

