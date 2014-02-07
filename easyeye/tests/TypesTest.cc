/*
 * File:   TypesTest.cc
 * Author: mchaberski
 *
 * Created on Feb 6, 2014, 4:00:35 PM
 */
#include "../src/easyeye/common/easyeye_types.h"
#include "TypesTest.h"

using namespace easyeye;
CPPUNIT_TEST_SUITE_REGISTRATION(TypesTest);

TypesTest::TypesTest() {
}

TypesTest::~TypesTest() {
}

void TypesTest::setUp() {
}

void TypesTest::tearDown() {
}

void TypesTest::testIntCircleEquals()
{
    IntCircle a(0, 0, 100);
    IntCircle b(0, 0, 100);
    IntCircle c(1, 1, 100);
    IntCircle d(1, 1, 1);
    IntCircle e(0, 0, 1);

    CPPUNIT_ASSERT(a.Equals(a));
    CPPUNIT_ASSERT(a.Equals(b));
    CPPUNIT_ASSERT(!a.Equals(c));
    CPPUNIT_ASSERT(!a.Equals(d));
    CPPUNIT_ASSERT(!a.Equals(e));
}

void TypesTest::testIntCircleEqualsWithDeltas()
{
    IntCircle a(0, 0, 100);
    IntCircle b(0, 0, 100);
    IntCircle c(1, 1, 100);
    IntCircle d(1, 1, 1);
    IntCircle e(0, 0, 1);
    IntCircle f(1, 1, 101);
    
    IntCircle::Delta zero(0, 0);
    IntCircle::Delta near(2, 2);
    IntCircle::Delta near_and_same_size(2, 0);
    IntCircle::Delta nearer(1.5, 1);
    double center_tolerance = 0.001;
    
    CPPUNIT_ASSERT(a.Equals(a, zero, center_tolerance));
    CPPUNIT_ASSERT(a.Equals(b, zero, center_tolerance));
    CPPUNIT_ASSERT(!a.Equals(c, zero, center_tolerance));
    CPPUNIT_ASSERT(a.Equals(c, near_and_same_size, center_tolerance));
    CPPUNIT_ASSERT(!a.Equals(d, near, center_tolerance));
    CPPUNIT_ASSERT(!a.Equals(e, near, center_tolerance));
    CPPUNIT_ASSERT(!a.Equals(f, zero, center_tolerance));
    CPPUNIT_ASSERT(a.Equals(f, near, center_tolerance));
    CPPUNIT_ASSERT(a.Equals(c, nearer, center_tolerance));

}

void TypesTest::testBoundaryPairEquals()
{
    BoundaryPair bp;
    bp.iris.set(200, 200, 150);
    bp.pupil.set(205, 205, 80);
    BoundaryPair identical;
    identical.iris.set(200, 200, 150);
    identical.pupil.set(205, 205, 80);
    BoundaryPair close_position;
    close_position.iris.set(202, 198, 150);
    close_position.pupil.set(203, 207, 80);
    BoundaryPair close_radius;
    close_radius.iris.set(200, 200, 148);
    close_radius.pupil.set(205, 205, 82);
    BoundaryPair close_both;
    close_both.iris.set(202, 198, 155);
    close_both.pupil.set(203, 207, 77);
    BoundaryPair far_position;
    far_position.iris.set(250, 248, 150);
    far_position.pupil.set(245, 260, 80);
    BoundaryPair far_radius;
    far_radius.iris.set(200, 200, 120);
    far_radius.pupil.set(205, 205, 60);
    BoundaryPair far_both;
    far_both.iris.set(250, 248, 120);
    far_both.pupil.set(245, 260, 80);

    CPPUNIT_ASSERT(bp.Equals(bp));
    CPPUNIT_ASSERT(bp.Equals(identical));
    CPPUNIT_ASSERT(!bp.Equals(close_position));
    CPPUNIT_ASSERT(!bp.Equals(close_radius));
    CPPUNIT_ASSERT(!bp.Equals(close_both));
    CPPUNIT_ASSERT(!bp.Equals(far_position));
    CPPUNIT_ASSERT(!bp.Equals(far_radius));
    CPPUNIT_ASSERT(!bp.Equals(far_both));
}

void TypesTest::testBoundaryPairEqualsWithDeltas()
{
    BoundaryPair bp;
    bp.iris.set(200, 200, 150);
    bp.pupil.set(205, 205, 80);
    BoundaryPair identical;
    identical.iris.set(200, 200, 150);
    identical.pupil.set(205, 205, 80);
    BoundaryPair close_position;
    close_position.iris.set(202, 198, 150);
    close_position.pupil.set(203, 207, 80);
    BoundaryPair close_radius;
    close_radius.iris.set(200, 200, 148);
    close_radius.pupil.set(205, 205, 82);
    BoundaryPair close_both;
    close_both.iris.set(202, 198, 155);
    close_both.pupil.set(203, 207, 77);
    BoundaryPair far_position;
    far_position.iris.set(250, 248, 150);
    far_position.pupil.set(245, 260, 80);
    BoundaryPair far_radius;
    far_radius.iris.set(200, 200, 120);
    far_radius.pupil.set(205, 205, 60);
    BoundaryPair far_both;
    far_both.iris.set(250, 248, 120);
    far_both.pupil.set(245, 260, 80);

    IntCircle::Delta max_iris_delta(10, 5);
    IntCircle::Delta max_pupil_delta(10, 5);
    
    CPPUNIT_ASSERT(bp.Equals(bp, max_iris_delta, max_pupil_delta));
    CPPUNIT_ASSERT(bp.Equals(identical, max_iris_delta, max_pupil_delta));
    CPPUNIT_ASSERT(bp.Equals(close_position, max_iris_delta, max_pupil_delta));
    CPPUNIT_ASSERT(bp.Equals(close_radius, max_iris_delta, max_pupil_delta));
    CPPUNIT_ASSERT(bp.Equals(close_both, max_iris_delta, max_pupil_delta));
    CPPUNIT_ASSERT(!bp.Equals(far_position, max_iris_delta, max_pupil_delta));
    CPPUNIT_ASSERT(!bp.Equals(far_radius, max_iris_delta, max_pupil_delta));
    CPPUNIT_ASSERT(!bp.Equals(far_both, max_iris_delta, max_pupil_delta));    
}

void TypesTest::testEyelidsLocationEquals()
{
    
}

void TypesTest::testEyelidsLocationEqualsWithDeltas()
{
    
}