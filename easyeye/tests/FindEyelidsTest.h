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

    CPPUNIT_TEST(testMethod);
    CPPUNIT_TEST(testFailedMethod);

    CPPUNIT_TEST_SUITE_END();

public:
    FindEyelidsTest();
    virtual ~FindEyelidsTest();
    void setUp();
    void tearDown();

private:
    void testMethod();
    void testFailedMethod();
};

#endif	/* FINDEYELIDSTEST_H */

