/*
 * File:   SegmentationSerializerTest.h
 * Author: ibgmike
 *
 * Created on Jan 26, 2014, 4:32:40 PM
 */

#ifndef SEGMENTATIONSERIALIZERTEST_H
#define	SEGMENTATIONSERIALIZERTEST_H

#include <cppunit/extensions/HelperMacros.h>

class SegmentationSerializerTest : public CPPUNIT_NS::TestFixture {
    CPPUNIT_TEST_SUITE(SegmentationSerializerTest);

    CPPUNIT_TEST(testSerializeAndDeserialize);

    CPPUNIT_TEST_SUITE_END();

public:
    SegmentationSerializerTest();
    virtual ~SegmentationSerializerTest();
    void setUp();
    void tearDown();

private:
    void testSerializeAndDeserialize();
};

#endif	/* SEGMENTATIONSERIALIZERTEST_H */

