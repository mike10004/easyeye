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
    const static double DEFAULT_CENTER_TOLERANCE;
    class Delta
    {
    public:
        Delta();
        Delta(double center_, int radius_);
        double center;
        int radius;
        bool IsLessThanOrEqualTo(const Delta& other, double center_tolerance) const;
        std::string ToString() const;
    };
    Delta ComputeDelta(const IntCircle& other) const;
    bool Equals(const IntCircle& other, const IntCircle::Delta& max_delta, double center_tolerance) const;
    bool Equals(const IntCircle& other, const IntCircle::Delta& max_delta) const;
    static double ComputeDistance(const cv::Point2i& p, const cv::Point2i& q);
};

class BoundaryPair
{
public:
	BoundaryPair();

    void Describe(std::ostream& out) const;
    bool Equals(const BoundaryPair& other) const;
    void ComputeDeltas(IntCircle::Delta& iris_delta, IntCircle::Delta& pupil_delta);
    bool Equals(const BoundaryPair& other, const IntCircle::Delta& max_iris_delta, const IntCircle::Delta& max_pupil_delta) const;
    bool Equals(const BoundaryPair& other, const IntCircle::Delta& max_iris_delta, const IntCircle::Delta& max_pupil_delta, double center_tolerance) const;
    void set_iris(const IntCircle& iris);
    void set_pupil(const IntCircle& pupil);
    IntCircle iris;
    IntCircle pupil;
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
    bool EqualsApprox(const EyelidsLocation& other) const;
    static const double DEFAULT_MAX_ANGLE_DELTA;
    bool Equals(const EyelidsLocation& other, double max_angle_delta) const;
    bool Equals(const EyelidsLocation& other, const int ellipse_deltas[], double max_angle_delta) const;
    bool Equals(const EyelidsLocation& other, int ellipse_delta, double max_angle_delta) const;
    void Describe(std::ostream& out);
    std::string ToString();
private:
    const static int APPROX_ELLIPSE_DELTA = 3;
};

}
#endif	/* EASYEYE_TYPES_H */

