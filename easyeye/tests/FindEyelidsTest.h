/*
 * File:   FindEyelidsTest.h
 * Author: mchaberski
 *
 * Created on Jan 28, 2014, 2:53:30 PM
 */

#ifndef FINDEYELIDSTEST_H
#define	FINDEYELIDSTEST_H

#include <cppunit/extensions/HelperMacros.h>

class FindEyelidsTest : public CPPUNIT_NS::TestFixture {
    CPPUNIT_TEST_SUITE(FindEyelidsTest);

    CPPUNIT_TEST(testDetectionCorrectness);

    CPPUNIT_TEST_SUITE_END();

public:
    FindEyelidsTest();
    virtual ~FindEyelidsTest();
    void setUp();
    void tearDown();

private:
    void testDetectionCorrectness();
};

#endif	/* FINDEYELIDSTEST_H */

