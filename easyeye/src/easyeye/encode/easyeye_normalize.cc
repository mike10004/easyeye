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
#include "src/easyeye/segment/easyeye_imaging.h"
#include "../segment/FindEyelidMix.h"
#include <portability.h>

using namespace easyeye;
using namespace cv;

Normalization::Normalization(const EncoderConfig& encoder_config)
:status(Result::NOT_YET_SET)
{
    InitArrays(encoder_config.angularRes, encoder_config.radialRes);
}
    
Normalization::Normalization(const int angularResolution, const int radialResolution)
   :status(Result::NOT_YET_SET)
{
    InitArrays(angularResolution, radialResolution);
}

Normalization::~Normalization()
{
	free(polarArray.data);
	free(noiseArray.data);
}

void Normalization::InitArrays(const int angularResolution, const int radialResolution) 
{
    polarArray.hsize[0] = radialResolution;
    polarArray.hsize[1] = angularResolution;
	polarArray.data = (double*)malloc(sizeof(double) * angularResolution * radialResolution);
    noiseArray.hsize[0] = radialResolution;
    noiseArray.hsize[1] = angularResolution;
    noiseArray.data = (unsigned char*)malloc(sizeof(unsigned char) * angularResolution * radialResolution);
    
}

void Encoder::NormalizeIris(cv::Mat& eye_image, const Segmentation& segmentation, Normalization& result) const
{
    eye_image = FindEyelidMix::CreateNoiseImage(eye_image, segmentation.eyelids_location);
    SparseMatConstIterator_<uchar> it = segmentation.extrema_noise.begin<uchar>();
    SparseMatConstIterator_<uchar> it_end = segmentation.extrema_noise.end<uchar>();
    double noise_indicator = portability::Math::GetNaN();
    uchar noise_color_u = (uchar) noise_indicator;
    assert(noise_color_u == 0);
    
    for(; it != it_end; ++it)
    {
        // print element indices and the element value
        const SparseMat::Node* n = it.node();
        int y = n->idx[0], x = n->idx[1];
        //uchar value = it.value<uchar>();
        uchar* p = eye_image.ptr<uchar>(y, x);
        for (int j = 0; j < 3; j++) {
            p[j] = noise_color_u;
        }
    }
    
	Masek masek;
	Masek::filter imgWithNoise;

	imgWithNoise.hsize[HSIZE_WIDTH] = eye_image.cols;
    imgWithNoise.hsize[HSIZE_HEIGHT] = eye_image.rows;
	imgWithNoise.data = (double*) malloc(sizeof(double)*imgWithNoise.hsize[0]*imgWithNoise.hsize[1]);

    const int rows = eye_image.rows, cols = eye_image.cols;
    for (int i = 0; i < rows * cols; i++) {
        if (eye_image.data[i] < 1) {
            imgWithNoise.data[i] = noise_indicator;//sqrt((double) -1); // NaN
        } else {
            imgWithNoise.data[i] = (double) eye_image.data[i];
        }
    }
	masek.normaliseiris(&imgWithNoise,
			segmentation.boundary_pair.irisX, segmentation.boundary_pair.irisY, segmentation.boundary_pair.irisR,
			segmentation.boundary_pair.pupilX, segmentation.boundary_pair.pupilY, segmentation.boundary_pair.pupilR,
			config_.radialRes, config_.angularRes, &(result.polarArray), &(result.noiseArray));
	free(imgWithNoise.data);
	result.status = Result::SUCCESS;
}

