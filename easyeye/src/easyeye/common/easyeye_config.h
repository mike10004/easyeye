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
    float upper_search_region_iris_height_proportion;
    float lower_search_region_iris_height_proportion;
    float lateral_search_region_iris_width_proportion;
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

