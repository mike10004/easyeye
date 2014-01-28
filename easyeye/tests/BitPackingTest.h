/*
 * File:   BitPackingTest.h
 * Author: mchaberski
 *
 * Created on Jan 25, 2014, 8:42:12 PM
 */

#ifndef BITPACKINGTEST_H
#define	BITPACKINGTEST_H

#include <cppunit/extensions/HelperMacros.h>

class BitPackingTest : public CPPUNIT_NS::TestFixture {
    CPPUNIT_TEST_SUITE(BitPackingTest);

    CPPUNIT_TEST(testBitPackingStatic);
    CPPUNIT_TEST(testBitPackingGenerated);

    CPPUNIT_TEST_SUITE_END();

public:
    BitPackingTest();
    virtual ~BitPackingTest();
    void setUp();
    void tearDown();

private:
    void testBitPackingStatic();
    void testBitPackingGenerated();
};

#endif	/* BITPACKINGTEST_H */

