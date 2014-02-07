/*
 * File:   EyelidFixTest.h
 * Author: mchaberski
 *
 * Created on Feb 7, 2014, 10:17:18 AM
 */

#ifndef EYELIDFIXTEST_H
#define	EYELIDFIXTEST_H

#include <cppunit/extensions/HelperMacros.h>

class EyelidFixTest : public CPPUNIT_NS::TestFixture {
    CPPUNIT_TEST_SUITE(EyelidFixTest);

    CPPUNIT_TEST(testCorrectness000);

    CPPUNIT_TEST_SUITE_END();

public:
    EyelidFixTest();
    virtual ~EyelidFixTest();
    void setUp();
    void tearDown();

private:
    void testCorrectness000();
};

#endif	/* EYELIDFIXTEST_H */

