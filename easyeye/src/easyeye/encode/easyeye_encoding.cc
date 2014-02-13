/*
 * easyeye_encoding.h
 * 
 * (c) 2013 IBG, A Novetta Solutions Company
 * 
 */
#include <sstream>
#include <string>
#include "easyeye_encode.h"
#include <cstdlib>
#include <cstring>
#include <iostream>
#include "../common/mylog.h"
#include "../common/easyeye_types.h"
#include "../common/easyeye_utils.h"
#include "../common/base64.h"
#include <vector>

#ifdef _WIN32
#include <jsoncpp.h>
#else
#include <jsoncpp/json/json.h>
#endif

using namespace easyeye;
using namespace std;
using mylog::Logs;

int Encoding::CalculateTemplateLength(const int nscales, const int angularResolution, const int radialResolution)
{
    return CalculateTemplateWidth(nscales, angularResolution) * CalculateTemplateHeight(radialResolution);
}

int Encoding::CalculateTemplateWidth(const int nscales, const int angularResolution)
{
    const int length = angularResolution * 2 * nscales;
    return length;
}

int Encoding::CalculateTemplateHeight(const int radialResolution) 
{
    return radialResolution;
}

int Encoding::height() const
{
    return height_;
}

int Encoding::width() const
{
    return width_;
}

int Encoding::angular_resolution() const
{
    return width() / (nscales() * 2);
}

int Encoding::radial_resolution() const 
{
    return height();
}

int Encoding::nscales() const
{
    return nscales_;
}

Encoding::Encoding() 
    :   status(Result::NOT_YET_SET), 
        irisTemplate(NULL), 
        irisMask(NULL),
        width_(0), 
        height_(0), 
        nscales_(0)
        
{
}

Encoding::Encoding(const int nscales, const int angularResolution, const int radialResolution)
    :   status(Result::NOT_YET_SET), 
        irisTemplate(NULL),
        irisMask(NULL),
        width_(CalculateTemplateWidth(nscales, angularResolution)),
        height_(CalculateTemplateHeight(radialResolution)),
        nscales_(nscales)
        
{
    int num_bytes = sizeof(int) * width_ * height_;
	irisTemplate = (int*) malloc(num_bytes);
	irisMask = (int*) malloc(num_bytes);
}

Encoding::Encoding(const EncoderConfig& encoder_config)
    :   status(Result::NOT_YET_SET),
        irisTemplate(NULL),
        irisMask(NULL),
        width_(CalculateTemplateWidth(encoder_config.encodeScales, encoder_config.angularRes)),
        height_(CalculateTemplateHeight(encoder_config.radialRes)),
        nscales_(encoder_config.encodeScales)
        
{
	irisTemplate = new int[width_ * height_];
	irisMask = new int[width_ * height_];
}

Encoding::~Encoding()
{
	delete irisTemplate;
	delete irisMask;
}

bool Encoding::IsCongruent(const Encoding& other) const
{
    return width() == other.width() 
            && height() == other.height() 
            && nscales() == other.nscales();
}

char getTemplateOrMaskChar(const int value, bool *printedError) {
    switch (value){
        case 0:
            return '0';
        case 1:
            return '1';
        default:
            if (!(*printedError)) {
                Logs::GetLogger().Log(mylog::WARN, "invalid template/mask value %d; probably wasn't initialized\n", value);
                *printedError = true;
            }
            return '0';
    }
}

void Encoding::CopyFrom(int width, int height, int nscales, int* irisTemplate_, int* irisMask_)
{
    if (irisTemplate != NULL) {
        delete irisTemplate;
        irisTemplate = NULL;
    }
    if (irisMask != NULL) {
        delete irisMask;
        irisMask = NULL;
    }
    width_ = width;
    height_ = height;
    nscales_ = nscales;
    size_t num_bytes = width * height * sizeof(int);
    irisTemplate = (int*) malloc(num_bytes);
    irisMask = (int*) malloc(num_bytes);
    memcpy(irisTemplate, irisTemplate_, num_bytes);
    memcpy(irisMask, irisMask_, num_bytes);
}