#include "easyeye_config.h"

using namespace easyeye;

Config::Config(){
    
}

IrisFinderConfig::IrisFinderConfig() 
    : CircleFinderConfig(),
        max_pupil_center_offset_divisor(4),
        max_pupil_center_offset_(AUTO_MAX_PUPIL_CENTER_OFFSET)
{

    max_radius_factor = 82;
}

int IrisFinderConfig::max_pupil_center_offset()
{
    if (max_pupil_center_offset_ == AUTO_MAX_PUPIL_CENTER_OFFSET) {
        return max_radius() / max_pupil_center_offset_divisor;
    } else {
        return max_pupil_center_offset_;
    }
}

PupilFinderConfig::PupilFinderConfig() 
    : CircleFinderConfig()
{
    max_radius_factor = (42);
}

CircleFinderConfig::CircleFinderConfig()
    : iris_image_type(NIR_IRIS_STILL), nScale(NSCALE_AUTO), max_radius_factor(82)
{
    
}

int CircleFinderConfig::max_radius()
{
    return nScale * max_radius_factor;
}

EyelidFinderConfig::EyelidFinderConfig()
: iris_image_type(NIR_IRIS_STILL)
{
    
}

ExtremaNoiseFinderConfig::ExtremaNoiseFinderConfig()
    : highlight_threshold(250), lowlight_threshold(3)
{
    
}