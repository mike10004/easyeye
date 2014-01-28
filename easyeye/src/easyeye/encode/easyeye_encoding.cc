/*
 * eziencoding.h
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

/*
 * The EasyEye source distribution contains json-c sources so that Windows users
 * don't have to retrieve them separately. Non-windows users have an easier time
 * installing the json-c development files (apt-get install libjson-c-dev), so we
 * don't bother for them. But the json-c sources included with the EasyEye sources
 * are in the json-c-0.11 subdirectory, so we add that as an include directory to 
 * the compiler settings and declare a different path in this #include directive.
 */
#ifdef _WIN32
#include <jsoncpp.h>
#else
#include <jsoncpp/json/json.h>
#endif

using namespace easyeye;
using namespace std;

//const char * Encoding::key_width = "width";
//const char * Encoding::key_height = "height";
//const char * Encoding::key_irisTemplate = "irisTemplate";
//const char * Encoding::key_irisMask = "irisMask";
//const char * Encoding::key_arrayEncoding = "arrayEncoding";
//const char * Encoding::value_arrayEncoding_ZeroAndOneChars = "ZeroAndOneChars";
//const char * Encoding::value_arrayEncoding_Base64 = "Base64";

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

Encoding::Encoding() : width_(0), height_(0), nscales_(0), status(Result::NOT_YET_SET), irisTemplate(NULL), irisMask(NULL)
{
}

Encoding::Encoding(const int nscales, const int angularResolution, const int radialResolution)
    : width_(CalculateTemplateWidth(nscales, angularResolution)),
        height_(CalculateTemplateHeight(radialResolution)),
        nscales_(nscales),
        status(Result::NOT_YET_SET)
{
	irisTemplate = (int*) malloc(sizeof(int) * width_ * height_);
	irisMask = (int*) malloc(sizeof(int) * width_ * height_);
}

Encoding::Encoding(const EncoderConfig& encoder_config)
    : width_(CalculateTemplateWidth(encoder_config.encodeScales, encoder_config.angularRes)),
        height_(CalculateTemplateHeight(encoder_config.radialRes)),
        status(Result::NOT_YET_SET)
{
	irisTemplate = (int*) malloc(sizeof(int) * width_ * height_);
	irisMask = (int*) malloc(sizeof(int) * width_ * height_);
}

Encoding::~Encoding()
{
	free(irisTemplate);
	free(irisMask);
}

char getTemplateOrMaskChar(const int value, bool *printedError) {
    switch (value){
        case 0:
            return '0';
        case 1:
            return '1';
        default:
            if (!(*printedError)) {
                mylog::Log(mylog::WARN, "invalid template/mask value %d; probably wasn't initialized\n", value);
                *printedError = true;
            }
            return '0';
    }
}

void Encoding::CopyFrom(int width, int height, int nscales, int* irisTemplate_, int* irisMask_)
{
    if (irisTemplate != NULL) {
        free(irisTemplate);
    }
    if (irisMask != NULL) {
        free(irisMask);
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