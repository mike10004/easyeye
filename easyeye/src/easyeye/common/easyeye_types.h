/* 
 * File:   easyeye_types.h
 * Author: mchaberski
 *
 * Created on July 15, 2013, 10:44 AM
 */

#ifndef EASYEYE_TYPES_H
#define	EASYEYE_TYPES_H

#include <iostream>
#include <opencv2/core/core.hpp>

namespace easyeye
{

enum IrisImageType 
{
    NIR_IRIS_STILL = 1, NIR_FACE_VIDEO = 2
};


class IntCircle
{
public:
    IntCircle();    
    IntCircle(int x, int y, int r);
    cv::Point2i center;
    int radius;
    void CopyFrom(const IntCircle& other);
    bool Equals(const IntCircle& other);
    void set(int x, int y, int r);
};

class BoundaryPair
{
public:
	BoundaryPair();

	int irisX;
	int irisY;
	int irisR;
	int pupilX, pupilY, pupilR;
    void Describe(std::ostream& out) const;
    bool Equals(const BoundaryPair& other) const;
    void set_iris(const IntCircle& iris);
    void set_pupil(const IntCircle& pupil);
    IntCircle iris() const;
    IntCircle pupil() const;
private:
    static void DescribeBoundary(std::ostream& out, int x, int y, int r);
};

class Result {
public:
    static const int OK = 0;
    static const int kStatusNotYetSet = 1;
    static const int kStatusFailure = 2;
    static const int kStatusSuccess = OK;
    enum ResultType { SUCCESS = kStatusSuccess, NOT_YET_SET = kStatusNotYetSet, FAILURE = kStatusFailure };
    static const char* DescribeStatus(const int status);
private:
    Result();
};

class EyelidsLocation
{
public:
    static const int NUM_ELLIPSE_VALS = 5;
    EyelidsLocation();
    int ellipse_vals[NUM_ELLIPSE_VALS];
    double angle;
    int center_x() const;
    int center_y() const;
    bool Equals(const EyelidsLocation& other) const;
    static const double DEFAULT_MAX_ANGLE_DELTA;
    bool Equals(const EyelidsLocation& other, double max_angle_delta) const;
};

}
#endif	/* EASYEYE_TYPES_H */

