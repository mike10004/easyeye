/*
 * File:   CommonSerialTest.h
 * Author: mchaberski
 *
 * Created on Jan 28, 2014, 11:34:54 AM
 */

#ifndef COMMONSERIALTEST_H
#define	COMMONSERIALTEST_H

#include <cppunit/extensions/HelperMacros.h>

class CommonSerialTest : public CPPUNIT_NS::TestFixture {
    CPPUNIT_TEST_SUITE(CommonSerialTest);

    CPPUNIT_TEST(testMethod);
    CPPUNIT_TEST(testFailedMethod);

    CPPUNIT_TEST_SUITE_END();

public:
    CommonSerialTest();
    virtual ~CommonSerialTest();
    void setUp();
    void tearDown();

private:
    void testMethod();
    void testFailedMethod();
};

#endif	/* COMMONSERIALTEST_H */

