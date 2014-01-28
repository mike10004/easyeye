/*
 * File:   DiagnosticsTest.h
 * Author: mike
 *
 * Created on Jan 5, 2014, 4:10:29 PM
 */

#ifndef DIAGNOSTICSTEST_H
#define	DIAGNOSTICSTEST_H

#include <cppunit/extensions/HelperMacros.h>

class DiagnosticsTest : public CPPUNIT_NS::TestFixture {
    CPPUNIT_TEST_SUITE(DiagnosticsTest);

    CPPUNIT_TEST(testWriteSegmentationOutput);
    CPPUNIT_TEST(testWriteNormalizationOutput);

    CPPUNIT_TEST_SUITE_END();

public:
    DiagnosticsTest();
    virtual ~DiagnosticsTest();
    void setUp();
    void tearDown();

private:
    void testWriteSegmentationOutput();
    void testWriteNormalizationOutput();
};

#endif	/* DIAGNOSTICSTEST_H */

