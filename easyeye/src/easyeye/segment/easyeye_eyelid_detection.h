/* 
 * File:   easyeye_eyelid_detection.h
 * Author: mike
 *
 * Created on February 7, 2014, 6:43 PM
 */

#ifndef EASYEYE_EYELID_DETECTION_H
#define	EASYEYE_EYELID_DETECTION_H

#include <opencv2/core/core.hpp>
#include <vector>
#include "../common/easyeye_types.h"
#include "../common/easyeye_config.h"

namespace cvmore
{
    namespace objdetect
    {
        
class ParamRange
{
public:
    static std::vector<double> Incremental(double min, double max, double step);
    static std::vector<double> Scaled(const std::vector<int>& values, double scalar);
    static std::vector<int> Steps(int min, int step, int num_values);
    static std::vector<double> ScaledIncremental(int min, int step, 
            int num_values, double scalar);
    static std::vector<double> MakeRangeFromImage(const cv::Mat& image);
private:
    ParamRange();
};

class Pixels
{
public:
    static float Interpolate(const cv::Mat& img, double px, double py);
private:
    Pixels();
};

class MaskInterface
{
public:
    MaskInterface();
    virtual ~MaskInterface();
    virtual bool CanVote(int x, int y) const = 0;
};

class DefaultMask : public MaskInterface
{
public:
    DefaultMask(int num_rows, int num_cols);
    ~DefaultMask();
    bool CanVote(int x, int y) const;
    const int num_rows;
    const int num_cols;
};

class ShapeInterface
{
public:
    ShapeInterface();
    virtual ~ShapeInterface();
    virtual bool Calculate(double t, const std::vector<double>& params, cv::Point2d& output) const = 0;
    virtual cv::Point2d ComputeFixedPoint(const std::vector<double>& params) const = 0;
    static const cv::Point2d kOrigin;
};

class HoughTransform
{
public:
    HoughTransform();
    virtual ~HoughTransform();
    virtual float GetPixelValue(const cv::Mat& image, double x, double y) const;
    static const int max_int;
    static const int min_int;
    bool debug() const;
    void set_debug(bool debug);
    void set_max_candidates(size_t max_candidates);
    size_t max_candidates() const;
    bool normalized() const;
    void set_normalized(bool normalized);
    void set_mask(const MaskInterface& mask);
    const MaskInterface& mask() const; 
    void AddParamRange(const std::vector<double>& param_range);
protected:
    bool debug_;
private:
    std::vector< std::vector<double> > param_ranges_;
    MaskInterface const* mask_;
    cv::Mat accumulator;
    cv::Mat normalized_accumulator;
    cv::Mat candidate_magnitudes;
    size_t num_params;
    size_t max_candidates_;
    bool normalized_;
    static bool IsInsideIntLimits(double value);
    bool AdvanceParameterIndices(int* param_indices);
    void SetParameterValues(int* param_indices, std::vector<double>& param_values);
    void Accumulate(const cv::Mat& image, const ShapeInterface& shape, const std::vector<double>& t_range);
    void GatherCandidates(std::vector< std::vector<double> >& candidates);
public:
    void Compute(const cv::Mat& image, const ShapeInterface& shape, 
            const std::vector<double>& t_range, 
            std::vector< std::vector<double> >& candidates);
    std::vector<double> Compute(const cv::Mat& image, 
            const ShapeInterface& shape, 
            const std::vector<double>& t_range);
    void Compute(const cv::Mat& image, const ShapeInterface& shape,  
            std::vector< std::vector<double> >& candidates);
    std::vector<double> Compute(const cv::Mat& image, const ShapeInterface& shape);
};

class VertexFormParabola : public ShapeInterface
{
public:
    VertexFormParabola();
    virtual ~VertexFormParabola();
    virtual bool Calculate(double t, const std::vector<double>& params, cv::Point2d& output) const;
    virtual cv::Point2d ComputeFixedPoint(const std::vector<double>& params) const;
    static const int kIndexA = 0, kIndexH = 1, kIndexK = 2;
    static const int kMaxIndex = kIndexK;
};

class StandardFormParabola : public ShapeInterface
{
public:
    StandardFormParabola();
    virtual ~StandardFormParabola();
    virtual bool Calculate(double t, const std::vector<double>& params, cv::Point2d& output) const;
    virtual cv::Point2d ComputeFixedPoint(const std::vector<double>& params) const;
    static const int kIndexA = 0, kIndexB = 1, kIndexC = 2;
};

class RotatedShape : public ShapeInterface
{
public:
    RotatedShape(ShapeInterface& original, int theta_index);
    virtual ~RotatedShape();
    virtual bool Calculate(double t, const std::vector<double>& params, cv::Point2d& output) const;
    virtual cv::Point2d ComputeFixedPoint(const std::vector<double>& params) const;

private:
    ShapeInterface* unrotated_;
    int theta_index_;
};

    }
}

namespace easyeye
{

class VertexFormParabola
{
public:
    VertexFormParabola();
    VertexFormParabola(double a_, double h_, double k_);
    const double a;
    const double h;
    const double k;
    std::string ToString() const;
    void Describe(std::ostream& out) const;
};

class EyelidBoundary
{
public:
    EyelidBoundary();
    VertexFormParabola shape;
    bool present;
};

class DualParabolaEyelidsLocation : public EyelidsLocation
{
public:
    DualParabolaEyelidsLocation();
    static const char* kType;
    EyelidBoundary upper;
    EyelidBoundary lower;
    const char* type() const;
    cv::Mat CreateNoiseMask(const cv::Mat& eye_image) const;
    bool Equals(const EyelidsLocation& other) const;
    bool EqualsApprox(const EyelidsLocation& other) const;
    void Describe(std::ostream& out) const;
    std::string ToString() const;
    void Draw(cv::Mat& diagnostic_image, cv::Scalar color) const;
};

class DualParabolaEyelidFinder
{
public:
    DualParabolaEyelidFinder(const EyelidFinderConfig& config);
    virtual ~DualParabolaEyelidFinder();
    virtual DualParabolaEyelidsLocation FindEyelids(const cv::Mat& eye_image, const BoundaryPair& boundary_pair);
protected:
    enum EyelidType { EYELID_UPPER, EYELID_LOWER };
    virtual cv::Mat MakeRegionMask(const cv::Mat& eye_image, const BoundaryPair& boundary_pair, EyelidType eyelid_type);
    virtual EyelidBoundary DetectEyelidBoundary(const cv::Mat& eye_image, const cv::Mat& region_mask, EyelidType eyelid_type);
    EyelidFinderConfig config_;
};

}            

            

#endif	/* EASYEYE_EYELID_DETECTION_H */

