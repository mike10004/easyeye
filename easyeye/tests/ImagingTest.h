/*
 * File:   ImagingTest.h
 * Author: mchaberski
 *
 * Created on Jan 27, 2014, 1:21:34 PM
 */

#ifndef IMAGINGTEST_H
#define	IMAGINGTEST_H

#include <cppunit/extensions/HelperMacros.h>

class ImagingTest : public CPPUNIT_NS::TestFixture {
    CPPUNIT_TEST_SUITE(ImagingTest);

    CPPUNIT_TEST(testGetROI);

    CPPUNIT_TEST_SUITE_END();

public:
    ImagingTest();
    virtual ~ImagingTest();
    void setUp();
    void tearDown();

private:
    void testGetROI();
};

#endif	/* IMAGINGTEST_H */

