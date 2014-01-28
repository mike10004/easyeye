/*
 * ezitypes.cc
 *
 *  Created on: Jul 11, 2013
 *      Author: mchaberski
 */

#include "easyeye_types.h"
#include "easyeye_utils.h"
#include <portability.h>

using namespace easyeye;

BoundaryPair::BoundaryPair() :
        irisX(0),
        irisY(0),
        irisR(0),
        pupilX(0),
        pupilY(0),
        pupilR(0)
{}

void BoundaryPair::DescribeBoundary(std::ostream& out, int x, int y, int r)
{
    out << "(" << x << ", " << y << ") r=" << r;
}

void BoundaryPair::Describe(std::ostream& out) const
{
    out << "iris ";
    DescribeBoundary(out, irisX, irisY, irisR);
    out << "; pupil ";
    DescribeBoundary(out, pupilX, pupilY, pupilR);
    
}

bool BoundaryPair::Equals(const BoundaryPair& other) const
{
    return irisX == other.irisX
            && irisY == other.irisY
            && irisR == other.irisR
            && pupilX == other.pupilX
            && pupilY == other.pupilY
            && pupilR == other.pupilR;
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
  

void BoundaryPair::set_iris(const IntCircle& iris) 
{
    irisX = iris.center.x;
    irisY = iris.center.y;
    irisR = iris.radius;
}

void BoundaryPair::set_pupil(const IntCircle& pupil) 
{
    pupilX = pupil.center.x;
    pupilY = pupil.center.y;
    pupilR = pupil.radius;
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

IntCircle BoundaryPair::iris() const
{
    IntCircle circle(irisX, irisY, irisR);
    return circle;
}

IntCircle BoundaryPair::pupil() const
{
    IntCircle circle(pupilX, pupilY, pupilR);
    return circle;
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
    return (abs(angle - other.angle) < max_angle_delta) 
            && (Arrays::CountNotEqual(ellipse_vals, other.ellipse_vals, NUM_ELLIPSE_VALS) == 0);
}