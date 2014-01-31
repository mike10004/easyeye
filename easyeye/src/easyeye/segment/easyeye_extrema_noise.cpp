#include "easyeye_extrema_noise.h"
#include "ImageUtility.h"
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

static const int TWO_DIMENSIONS = 2;

SparseMat ExtremaNoiseFinder::FindExtremaNoise(Mat& eyeImg)
{
    int size[] = {eyeImg.rows, eyeImg.cols};
    SparseMat extrema_points(TWO_DIMENSIONS, size, CV_8UC1);
    for (int y = 0; y < eyeImg.rows; y++) {
        Mat row = eyeImg.row(y);
        for (int x = 0; x < eyeImg.cols; x++) {
            if (IsExtreme(eyeImg.at<uchar>(y, x))) {
                row.at<uchar>(x) = 255;
            }
        }
    }
    return extrema_points;
}
