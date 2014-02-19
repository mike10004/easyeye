/* 
 * File:   easyeye_config.h
 * Author: ibgmike
 *
 * Created on January 26, 2014, 9:48 AM
 */

#ifndef EASYEYE_CONFIG_H
#define	EASYEYE_CONFIG_H

#include "easyeye_types.h"

namespace easyeye
{

class Config
{
public: 
    Config();
};

class CircleFinderConfig : public Config
{
public:
    CircleFinderConfig();
    IrisImageType iris_image_type;
    int nScale;
    int max_radius_factor;
    int max_radius();
    const static int NSCALE_AUTO = 0;
};

class PupilFinderConfig : public CircleFinderConfig
{
public:
    PupilFinderConfig();
};

class IrisFinderConfig : public CircleFinderConfig
{
public:
    IrisFinderConfig();
    int max_pupil_center_offset_divisor;
    int max_pupil_center_offset_;
    int max_pupil_center_offset();
    const static int AUTO_MAX_PUPIL_CENTER_OFFSET = 0;
};

class GaussConfig : public Config
{
public:
    GaussConfig();
    GaussConfig(int kernel_width_, int kernel_height_, double sigma_x_);
    int kernel_width;
    int kernel_height;
    double sigma_x;
};

class RangeParams
{
public:
    RangeParams();
    RangeParams(double minimum_, double maximum_, double step);
    double minimum;
    double maximum;
    double step;
};

class EyelidFinderConfig : public Config
{
public:
    EyelidFinderConfig();
    IrisImageType iris_image_type;
    GaussConfig gauss_config;
    int min_contour_count;
    enum Method {
        METHOD_ELLIPSE_CONTOUR, METHOD_HOUGH_PARABOLA
    };
    Method method;
    
    /**
     * Height of the upper eyelid search region, as a proportion of iris
     * height. The top of the search region is the top of the iris.
     */
    float upper_search_region_iris_height_proportion;
    
    /**
     * Height of the lower eyelid search region, as a proportion of iris 
     * height. The bottom of the search region is the bottom of the iris.
     */
    float lower_search_region_iris_height_proportion;
    
    /**
     * Factor that determines the width of the upper and lower eyelid search 
     * regions. The search region width is the width of the iris plus a 
     * lateral extension on either side. This value determines how far to 
     * extend the search region on either side of the iris.
     */
    float lateral_search_region_iris_width_proportion;
    
    /**
     * Factor that determines the coordinates of the upper-left corner of the 
     * search range for the upper parabola vertex.
     */
    cv::Point2f upper_parabola_vertex_min_relative;

    /**
     * Factor that determines the coordinates of the lower-right corner of the 
     * search range for the upper parabola vertex.
     */
    cv::Point2f upper_parabola_vertex_max_relative;
     
    /**
     * Factor that determines the coordinates of the upper-left corner of the 
     * search range for the lower parabola vertex.
     */
    cv::Point2f lower_parabola_vertex_min_relative;

    /**
     * Factor that determines the coordinates of the lower-right corner of the 
     * search range for the lower parabola vertex.
     */
    cv::Point2f lower_parabola_vertex_max_relative;
    
    RangeParams upper_parabola_a_range;
    RangeParams lower_parabola_a_range;
    RangeParams upper_parabola_h_range;
    RangeParams lower_parabola_h_range;
    RangeParams upper_parabola_k_range;
    RangeParams lower_parabola_k_range;
    RangeParams upper_parabola_theta_range;
    RangeParams lower_parabola_theta_range;
    int eyelid_hough_dilation_kernel_size;
    float upper_eyelid_hough_vote_total_min;
    float lower_eyelid_hough_vote_total_min;
};

class ExtremaNoiseFinderConfig : public Config
{
public:
    ExtremaNoiseFinderConfig();
    int highlight_threshold;
    int lowlight_threshold;
};

}

#endif	/* EASYEYE_CONFIG_H */

