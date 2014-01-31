/*
 * File:   FilesTest.h
 * Author: mike
 *
 * Created on Jan 31, 2014, 10:25:32 AM
 */

#ifndef FILESTEST_H
#define	FILESTEST_H

#include <cppunit/extensions/HelperMacros.h>

class FilesTest : public CPPUNIT_NS::TestFixture {
    CPPUNIT_TEST_SUITE(FilesTest);

    CPPUNIT_TEST(testBasename);
    CPPUNIT_TEST(testDirname);
    CPPUNIT_TEST(testNameWithoutExtension);
    CPPUNIT_TEST(testExtension);
    CPPUNIT_TEST(testAbsolute);

    CPPUNIT_TEST_SUITE_END();

public:
    FilesTest();
    virtual ~FilesTest();
    void setUp();
    void tearDown();

private:
    void testBasename();
    void testDirname();
    void testExtension();
    void testAbsolute();
    void testNameWithoutExtension();
};

#endif	/* FILESTEST_H */

