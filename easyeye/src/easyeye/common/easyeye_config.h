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

class EyelidFinderConfig : public Config
{
public:
    EyelidFinderConfig();
    IrisImageType iris_image_type;
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

