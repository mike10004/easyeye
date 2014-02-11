/*
 * vasirencode.cc
 *
 *  Created on: Jul 11, 2013
 *      Author: mchaberski
 */

#include <cassert>
#include <cstdlib>
#include <cmath>
#include <Masek.h>
#include "../common/easyeye_types.h"
#include "easyeye_encode.h"
#include "../common/easyeye_imaging.h"
#include "../segment/FindEyelidMix.h"
#include <portability.h>

using namespace easyeye;
using namespace cv;

Normalization::Normalization(const EncoderConfig& encoder_config)
: status(Result::NOT_YET_SET), angular_resolution_(encoder_config.angularRes), 
        radial_resolution_(encoder_config.radialRes)
{
}
    
Normalization::Normalization(const int angularResolution, const int radialResolution)
   :status(Result::NOT_YET_SET), angular_resolution_(angularResolution), radial_resolution_(radialResolution)
{
}

Normalization::~Normalization()
{
}

void Encoder::NormalizeIris(const Mat& src_eye_image, const Segmentation& segmentation, Normalization& result) 
{
    Mat noisified_eye_image = segmentation.eyelids_location().CreateNoiseMask(src_eye_image);
    double noise_indicator = portability::Math::GetNaN();
    uchar noise_color_u = (uchar) noise_indicator;
    assert(noise_color_u == 0);
    
    for(SparseMatConstIterator_<uchar> it = segmentation.extrema_noise.begin<uchar>(); 
            it != segmentation.extrema_noise.end<uchar>(); ++it)
    {
        // print element indices and the element value
        const SparseMat::Node* n = it.node();
        int y = n->idx[0], x = n->idx[1];
        //uchar value = it.value<uchar>();
        uchar* p = noisified_eye_image.ptr<uchar>(y, x);
        for (int j = 0; j < 3; j++) {
            p[j] = noise_color_u;
        }
    }
    
	Masek masek;
    Mat img_with_noise(noisified_eye_image.rows, noisified_eye_image.cols, Imaging::FLOAT_DATA_TYPE);
    const int rows = noisified_eye_image.rows, cols = noisified_eye_image.cols;
    for (int y = 0; y < rows; y++) {
        Mat src_row = noisified_eye_image.row(y);
        Mat row = img_with_noise.row(y);
        for (int x = 0; x < cols; x++) {
            Imaging::GrayDataType src_value = src_row.at<Imaging::GrayDataType>(x);
            double value = src_value < 1 ? noise_indicator : src_value;
            row.at<Imaging::FloatDataType>(x) = (Imaging::FloatDataType) value;
        }
    }
    Masek::filter polarArray;
    Masek::IMAGE noiseArray;
    polarArray.hsize[0] = config_.radialRes;
    polarArray.hsize[1] = config_.angularRes;
	polarArray.data = (double*)malloc(sizeof(double) * config_.angularRes * config_.radialRes);
    noiseArray.hsize[0] = config_.radialRes;
    noiseArray.hsize[1] = config_.angularRes;
    noiseArray.data = (unsigned char*)malloc(sizeof(unsigned char) * config_.angularRes * config_.radialRes);
    
    Masek::filter* imgWithNoise = Imaging::CopyToFilter(img_with_noise);
	masek.normaliseiris(imgWithNoise,
			segmentation.boundary_pair.iris.center.x, segmentation.boundary_pair.iris.center.y, segmentation.boundary_pair.iris.radius,
			segmentation.boundary_pair.pupil.center.x, segmentation.boundary_pair.pupil.center.y, segmentation.boundary_pair.pupil.radius,
			config_.radialRes, config_.angularRes, &polarArray, &noiseArray);
	Imaging::FreeFilter(imgWithNoise);
	result.status = Result::SUCCESS;
    Imaging::CopyFromFilter(&polarArray, result.polar_array);
    Imaging::CopyFromMasek(&noiseArray, result.noise_array);
    diagnostician()->DumpNormOutput(result.polar_array, result.noise_array);
}

