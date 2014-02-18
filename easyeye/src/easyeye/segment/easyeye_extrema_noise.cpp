#include "easyeye_extrema_noise.h"

#include <opencv2/core/core.hpp>
#include "../common/easyeye_imaging.h"

using namespace cv;
using namespace easyeye;

ExtremaNoiseFinder::ExtremaNoiseFinder(ExtremaNoiseFinderConfig config)
: config_(config)
{
    
}

bool ExtremaNoiseFinder::IsExtreme(unsigned char value)
{
    return value < config_.lowlight_threshold
            || value > config_.highlight_threshold;
}

namespace 
{
const int TWO_DIMENSIONS = 2;
}
SparseMat ExtremaNoiseFinder::FindExtremaNoise(const Mat& eyeImg)
{
    int size[] = {eyeImg.rows, eyeImg.cols};
    SparseMat extrema_points(TWO_DIMENSIONS, size, CV_8UC1);
    int idx[2];
    for (int y = 0; y < eyeImg.rows; y++) {
        Mat row = eyeImg.row(y);
        idx[0] = y;
        for (int x = 0; x < eyeImg.cols; x++) {
            idx[1] = x;
            if (IsExtreme(row.at<uchar>(x))) {
                extrema_points.ref<uchar>(idx) = 0xff;
            }
        }
    }
    return extrema_points;
}
