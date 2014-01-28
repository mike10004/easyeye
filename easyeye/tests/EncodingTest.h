/*
 * File:   EncodingTest.h
 * Author: ibgmike
 *
 * Created on Jan 26, 2014, 12:12:47 PM
 */

#ifndef ENCODINGTEST_H
#define	ENCODINGTEST_H

#include <cppunit/extensions/HelperMacros.h>

class EncodingTest : public CPPUNIT_NS::TestFixture {
    CPPUNIT_TEST_SUITE(EncodingTest);

    CPPUNIT_TEST(testSerializeAndDeserializeRandomEncoding);
    CPPUNIT_TEST(testEncodeFromSegmentations);

    CPPUNIT_TEST_SUITE_END();

public:
    EncodingTest();
    virtual ~EncodingTest();
    void setUp();
    void tearDown();

private:
    void testEncodeFromSegmentations();
    void testSerializeAndDeserializeRandomEncoding();
};

#endif	/* ENCODINGTEST_H */

