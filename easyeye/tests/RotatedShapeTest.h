/*
 * File:   RotatedShapeTest.h
 * Author: mike
 *
 * Created on Feb 9, 2014, 5:45:16 PM
 */

#ifndef ROTATEDSHAPETEST_H
#define	ROTATEDSHAPETEST_H

#include <cppunit/extensions/HelperMacros.h>

class RotatedShapeTest : public CPPUNIT_NS::TestFixture {
    CPPUNIT_TEST_SUITE(RotatedShapeTest);

//    CPPUNIT_TEST(testDrawUnrotatedLine);
//    CPPUNIT_TEST(testDrawUnrotatedRect);
    CPPUNIT_TEST(testDrawRotatedRect);
//    CPPUNIT_TEST(testDrawRotatedLine);
    CPPUNIT_TEST_SUITE_END();

public:
    RotatedShapeTest();
    virtual ~RotatedShapeTest();
    void setUp();
    void tearDown();

private:
    void testDrawUnrotatedLine();
    void testDrawUnrotatedRect();
    void testDrawRotatedRect();
    void testDrawRotatedLine();
};

#endif	/* ROTATEDSHAPETEST_H */

