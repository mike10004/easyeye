/*
 * vasirencode.cc
 *
 *  Created on: Jul 11, 2013
 *      Author: mchaberski
 */

#include "../common/mylog.h"
#include <cstdlib>
#include <cmath>
#include <Masek.h>
#include "../common/easyeye_types.h"
#include <iostream>
#include <fstream>
#include <cstring>
#include "easyeye_encode.h"
#include "../common/easyeye_diagnostics.h"

using mylog::Logs;
using namespace easyeye;
using namespace cv;
EncoderConfig::EncoderConfig() : Config(), 
        encodeScales(DEFAULT_NSCALES), 
        minWaveLength(18), 
        mult(1), 
        sigmaOnf(0.5), 
        radialRes(DEFAULT_RADIAL_RES), 
        angularRes(DEFAULT_ANGULAR_RES)
{
}

Encoder::Encoder() : config_()
{
    
}

Encoder::Encoder(EncoderConfig config) : config_(config)
{
    
}

void Encoder::EncodeIris(const Normalization& normalization, Encoding& result) const
{
    return EncodeIris(normalization.polar_array, normalization.noise_array, result);
}

void Encoder::EncodeIris(const Imaging::FloatImage& polar_array, const Imaging::ByteImage& noise_array, Encoding& result) const
{
    Masek::filter* polarArray = Imaging::CopyToFilter(polar_array);
    Masek::IMAGE* noiseArray = Imaging::CopyToMasek((Mat&)noise_array);
    int* irisTemplate = result.irisTemplate;
    int* irisMask = result.irisMask;
    if (irisTemplate == NULL || irisMask == NULL) {
        Logs::GetLogger().Log(mylog::ERROR, "Encoder::EncodeIris invalid input; result arrays have not been instantiated\n");
        result.status = Result::FAILURE;
        return;
    }
	Masek masek;

	masek.encode(polarArray, noiseArray, config_.encodeScales, config_.minWaveLength,
		config_.mult, config_.sigmaOnf, irisTemplate, irisMask);
//		&(result.width), &(result.height));
	result.status = Result::SUCCESS;
    Imaging::FreeFilter(polarArray);
    Imaging::FreeImage(noiseArray);
}

bool Encoding::Equals(const Encoding& b) const
{
    if (width() != b.width()) {   
        return false;
    }
    if (height() != b.height()) {
        return false;
    }
    size_t len = height() * width();
    bool templatesEqual = Arrays::CountNotEqual(irisTemplate, b.irisTemplate, len) == 0;
    bool masksEqual = Arrays::CountNotEqual(irisMask, b.irisMask, len) == 0;
    return templatesEqual && masksEqual;
}

const EncoderConfig& Encoder::config()
{
    return config_;
}
