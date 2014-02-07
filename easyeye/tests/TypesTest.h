/*
 * File:   TypesTest.h
 * Author: mchaberski
 *
 * Created on Feb 6, 2014, 4:00:35 PM
 */

#ifndef TYPESTEST_H
#define	TYPESTEST_H

#include <cppunit/extensions/HelperMacros.h>

class TypesTest : public CPPUNIT_NS::TestFixture {
    CPPUNIT_TEST_SUITE(TypesTest);

    CPPUNIT_TEST(testEyelidsLocationEquals);
    CPPUNIT_TEST(testEyelidsLocationEqualsWithDeltas);
    CPPUNIT_TEST(testBoundaryPairEquals);
    CPPUNIT_TEST(testBoundaryPairEqualsWithDeltas);
    CPPUNIT_TEST(testIntCircleEquals);
    CPPUNIT_TEST(testIntCircleEqualsWithDeltas);

    CPPUNIT_TEST_SUITE_END();

public:
    TypesTest();
    virtual ~TypesTest();
    void setUp();
    void tearDown();

private:
    void testEyelidsLocationEquals();
    void testEyelidsLocationEqualsWithDeltas();
    void testBoundaryPairEquals();
    void testBoundaryPairEqualsWithDeltas();
    void testIntCircleEquals();
    void testIntCircleEqualsWithDeltas();
};

#endif	/* TYPESTEST_H */

