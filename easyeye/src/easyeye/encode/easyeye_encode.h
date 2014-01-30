/*
 * vasirencode.h
 *
 *  Created on: Jul 11, 2013
 *      Author: mchaberski
 */

#ifndef VASIRENCODE_H_
#define VASIRENCODE_H_

#include <string>
#include <iostream>
#include <fstream>
#include <Masek.h>
#include "../segment/easyeye_segment.h"
#include "../common/easyeye_types.h"
#include "../common/easyeye_utils.h"
#include "../common/easyeye_serial.h"
#include "../common/easyeye_diagnostics.h"
#include "../common/easyeye_config.h"


namespace easyeye
{

    
class EncoderConfig : public Config
{
public:
    EncoderConfig();
	const int radialRes;//20 or 40
	const int angularRes;//240 or 480
	const int encodeScales;
    static const int DEFAULT_ANGULAR_RES = 240;
    static const int DEFAULT_RADIAL_RES = 20;
    static const int DEFAULT_NSCALES = 1;
	const int minWaveLength;
	const int mult;
	const double sigmaOnf;
    
};


    
class Normalization
{
public:
    Normalization(const EncoderConfig& encoder_config);
	Normalization(const int angularResolution, const int radialResolution);
	virtual ~Normalization();
    typedef Result::ResultType Status;
    Status status;
	Masek::filter polarArray;
	Masek::IMAGE noiseArray;
    void Dump(Diagnostician& diag);
private:
    void InitArrays(const int angularResolution, const int radialResolution);
};

class Encoding
{
public:
    /**
     * Constructs an encoding instance, allocating memory for the template
     * and mask.
     * @param nscales
     * @param angularResolution
     * @param radialResolution
     */
	Encoding(const int nscales, const int angularResolution, const int radialResolution);
    /**
     * Constructs a zero-length encoding instance. This constructor should
     * only be used for deserialization of encodings. It allocates
     * no memory.
     */
    Encoding();
    Encoding(const EncoderConfig& encoder_config);
	virtual ~Encoding();
    bool IsCongruent(const Encoding& other) const;
    typedef Result::ResultType Status;
	Status status;
	int* irisTemplate;
	int* irisMask;

	/** Returns the angular value of the template. */
	int angular_resolution() const;

	/** Returns the radial value of the template. */
	int radial_resolution() const;

	/** Returns the iris template. */
	inline int *iris_template_ptr() const { return irisTemplate; }

	/** Returns the noise mask. */
	inline int *iris_mask_ptr() const { return irisMask; }

    int height() const;
    int width() const;
    int nscales() const;
    
    static int CalculateTemplateLength(const int nscales, const int angularResolution, const int radialResolution);
    static int CalculateTemplateWidth(const int nscales, const int angularResolution);
    static int CalculateTemplateHeight(const int radialResolution);    
    bool Equals(const Encoding& other) const;
    void CopyFrom(int width, int height, int nscales, int* irisTemplate, int* irisMask);
private:
    int width_, height_, nscales_;
    const static char * key_width; //= "width";
    const static char * key_height; //= "height";
    const static char * key_irisTemplate;// = "irisTemplate";
    const static char * key_irisMask; //= "irisMask";
    const static char * key_arrayEncoding;// = "arrayEncoding";
    const static char * value_arrayEncoding_ZeroAndOneChars;// = "ZeroAndOneChars";
    const static char * value_arrayEncoding_Base64;
    
};

class Encoder : public DiagnosticsCreator
{
public:
	Encoder();
    Encoder(EncoderConfig config);
	void NormalizeIris(cv::Mat& eye_image, const Segmentation& segmentation, Normalization& result) const;
    void EncodeIris(Normalization& normalization, Encoding& result) const;
    void EncodeIris(Masek::filter* polarArray, Masek::IMAGE* noiseArray, Encoding& result) const;
    const EncoderConfig& config();
private:
    EncoderConfig config_;
};

namespace serial 
{

    class EncodingAdapter : public serial::Adapter
{
public:
    bool FromJson(const Json::Value& src, void* dst);
    void ToJson(void* src, Json::Value& dst);
    /**
     * 
     * @param jsonBuffer the buffer into which the json encoding of this
     * iris encoding will be written. This buffer must be allocated with
     * length at least ENCODING_DESERIALIZATION_BUFFER_LEN.
     */
//    static std::string Serialize() const;
//    static bool Serialize(std::ostream& out) const;
//    const static int ENCODING_DESERIALIZATION_BUFFER_LEN = 65536;
//	static void Deserialize(std::istream & in);
//    static void Deserialize(const std::string& json);
};

bool Deserialize(const std::string& json, Encoding& encoding);
std::string Serialize(const Encoding& encoding);

} // end namespace serial
} // end namespace easyeye
#endif /* VASIRENCODE_H_ */
