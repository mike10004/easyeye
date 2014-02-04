/*
 * easyeye_types.cc
 *
 *  Created on: Jul 11, 2013
 *      Author: mchaberski
 */

#include "easyeye_types.h"
#include "easyeye_utils.h"
#include <portability.h>
#include <iostream>
#include <sstream>
#include <string>

using namespace std;
using namespace easyeye;

BoundaryPair::BoundaryPair() :
        iris(),
        pupil()
{}

void BoundaryPair::DescribeBoundary(ostream& out, int x, int y, int r)
{
    out << "(" << x << ", " << y << ") r=" << r;
}

void BoundaryPair::Describe(ostream& out) const
{
    out << "iris ";
    DescribeBoundary(out, iris.center.x, iris.center.y, iris.radius);
    out << "; pupil ";
    DescribeBoundary(out, pupil.center.x, pupil.center.y, pupil.radius);
    
}

bool BoundaryPair::Equals(const BoundaryPair& other) const
{
    return iris.center.x == other.iris.center.x
            && iris.center.y == other.iris.center.y
            && iris.radius == other.iris.radius
            && pupil.center.x == other.pupil.center.x
            && pupil.center.y == other.pupil.center.y
            && pupil.radius == other.pupil.radius;
}

Result::Result() {}
const char* Result::DescribeStatus(const int result)
{
	switch (result)
	{
	case kStatusFailure: return "FAILURE";
	case kStatusNotYetSet: return "NOT_YET_SET";
	case kStatusSuccess: return "SUCCESS";
	default: return "UNDEFINED";
	}
}
  

void BoundaryPair::set_iris(const IntCircle& iris_) 
{
    iris.center.x = iris_.center.x;
    iris.center.y = iris_.center.y;
    iris.radius = iris_.radius;
}

void BoundaryPair::set_pupil(const IntCircle& pupil_) 
{
    pupil.center.x = pupil_.center.x;
    pupil.center.y = pupil_.center.y;
    pupil.radius = pupil_.radius;
}

void IntCircle::CopyFrom(const IntCircle& other)
{
    center.x = other.center.x;
    center.y = other.center.y;
    radius = other.radius;
}

void IntCircle::set(int x, int y, int r)
{
    center.x = x;
    center.y = y;
    radius = r;
}

IntCircle::IntCircle(int x, int y, int r)
    : center(x, y), radius(r)
{
}

IntCircle::IntCircle() 
    : center(0, 0), radius(0)
{
}

EyelidsLocation::EyelidsLocation()
{
    angle = portability::Math::GetNaN();
    for (int i = 0; i < NUM_ELLIPSE_VALS; i++) {
        ellipse_vals[i] = 0;
    }
}

int EyelidsLocation::center_x() const{
    return ellipse_vals[0];
}

int EyelidsLocation::center_y() const
{
    return ellipse_vals[1];
}

const double EyelidsLocation::DEFAULT_MAX_ANGLE_DELTA = 0.1;

bool EyelidsLocation::Equals(const EyelidsLocation& other) const
{
    return Equals(other, DEFAULT_MAX_ANGLE_DELTA);
}

bool EyelidsLocation::Equals(const EyelidsLocation& other, double max_angle_delta) const
{
    return (abs(angle - other.angle) <= max_angle_delta) 
            && (Arrays::CountNotEqual(ellipse_vals, other.ellipse_vals, NUM_ELLIPSE_VALS) == 0);
}

void EyelidsLocation::Describe(ostream& out)
{
    out << "EyelidsLocation{[" << ellipse_vals[0];
    for (int i = 1; i < NUM_ELLIPSE_VALS; i++) {
        out << ", " << ellipse_vals[i];
    }
    out << "], " << angle << "}";
}

string EyelidsLocation::ToString()
{
    ostringstream ss;
    Describe(ss);
    return ss.str();
}

bool EyelidsLocation::Equals(const EyelidsLocation& other, int ellipse_deltas[], double max_angle_delta) const
{
    if (abs(angle - other.angle) > max_angle_delta) {
        return false;
    }
    for (int i = 0; i < NUM_ELLIPSE_VALS; i++) {
        if (abs(ellipse_vals[i] - other.ellipse_vals[i]) > ellipse_deltas[i]) {
            return false;
        }
    }
    return true;
}

bool EyelidsLocation::Equals(const EyelidsLocation& other, int ellipse_delta, double max_angle_delta) const
{
    int ellipse_deltas[NUM_ELLIPSE_VALS];
    for (int i = 0; i < NUM_ELLIPSE_VALS; i++) {
        ellipse_deltas[i] = ellipse_delta;
    }
    return Equals(other, ellipse_deltas, max_angle_delta);
}

bool EyelidsLocation::EqualsApprox(const EyelidsLocation& other) const
{
    return Equals(other, APPROX_ELLIPSE_DELTA, DEFAULT_MAX_ANGLE_DELTA);
}