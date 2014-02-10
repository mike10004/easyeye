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

    CPPUNIT_TEST(testComputeTransformVertexFormRotated);
    CPPUNIT_TEST(testComputeTransformStandardForm);
    CPPUNIT_TEST(testComputeTransformVertexForm);
    CPPUNIT_TEST_SUITE_END();

public:
    HoughParabolaTest();
    virtual ~HoughParabolaTest();
    void setUp();
    void tearDown();

private:
    void testComputeTransformStandardForm();
    void testComputeTransformVertexForm();
    void testComputeTransformVertexFormRotated();
};

#endif	/* HOUGHPARABOLATEST_H */

