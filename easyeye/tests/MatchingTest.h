/*
 * File:   MatchingTest.h
 * Author: ibgmike
 *
 * Created on Jan 26, 2014, 4:12:59 PM
 */

#ifndef MATCHINGTEST_H
#define	MATCHINGTEST_H

#include <cppunit/extensions/HelperMacros.h>

class MatchingTest : public CPPUNIT_NS::TestFixture {
    CPPUNIT_TEST_SUITE(MatchingTest);

    CPPUNIT_TEST(testComputeScoresBasic);

    CPPUNIT_TEST_SUITE_END();

public:
    MatchingTest();
    virtual ~MatchingTest();
    void setUp();
    void tearDown();

private:
    void testComputeScoresBasic();
};

#endif	/* MATCHINGTEST_H */

