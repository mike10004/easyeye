/*
 * eziencoding.h
 * 
 * (c) 2013 IBG, A Novetta Solutions Company
 * 
 */

#include "eziencoding.h"
#include "base64.h"
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <mylog.h>
#include <ezitypes.h>
#include <base64.h>

/*
 * The EasyEye source distribution contains json-c sources so that Windows users
 * don't have to retrieve them separately. Non-windows users have an easier time
 * installing the json-c development files (apt-get install libjson-c-dev), so we
 * don't bother for them. But the json-c sources included with the EasyEye sources
 * are in the json-c-0.11 subdirectory, so we add that as an include directory to 
 * the compiler settings and declare a different path in this #include directive.
 */
#ifdef _WIN32
#include <json.h>
#else
#include <json/json.h>
#endif

using namespace Ezi;

const char * Encoding::key_width = "width";
const char * Encoding::key_height = "height";
const char * Encoding::key_irisTemplate = "irisTemplate";
const char * Encoding::key_irisMask = "irisMask";
const char * Encoding::key_arrayEncoding = "arrayEncoding";
const char * Encoding::value_arrayEncoding_ZeroAndOneChars = "ZeroAndOneChars";
const char * Encoding::value_arrayEncoding_Base64 = "Base64";

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

int Encoding::GetHeight() const
{
    return height;
}

int Encoding::GetWidth() const
{
    return width;
}

Encoding::Encoding() : width(0), height(0), status(NOT_YET_SET),
        arrayEncodingMode(MODE_ZEROSANDONES)
{
    irisTemplate = NULL;
    irisMask = NULL;
}

Encoding::Encoding(const int nscales, const int angularResolution, const int radialResolution)
    : width(CalculateTemplateWidth(nscales, angularResolution)),
        height(CalculateTemplateHeight(radialResolution)),
        status(NOT_YET_SET),
        arrayEncodingMode(MODE_ZEROSANDONES)
{
	irisTemplate = (int*) malloc(sizeof(int) * width * height);
	irisMask = (int*) malloc(sizeof(int) * width * height);
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

void Encoding::Serialize(char* jsonBuffer) const
{
    json_object *jobj = json_object_new_object();
    bool printedError = false;
    char* maskChars = NULL;
    char* templateChars =  NULL;
    if (arrayEncodingMode == MODE_BASE64) {
        unsigned char* bytes = (unsigned char*) malloc(sizeof(unsigned char) * width * height);
        Base64::ToBytes(irisMask, width * height, bytes);
        std::string maskCStr = Base64::Encode(bytes, width * height);
        Base64::ToBytes(irisTemplate, width * height, bytes);
        std::string templateCStr = Base64::Encode(bytes, width * height);
        free(bytes);
        maskChars = strdup(maskCStr.c_str());
        templateChars = strdup(templateCStr.c_str());
    } else {
        maskChars = (char*) malloc(sizeof(char) * width * height + 1);
        templateChars = (char*) malloc(sizeof(char) * width * height + 1);
        if (maskChars == NULL || templateChars == NULL) {
            mylog::Log(mylog::ERROR, "Encoding::Serialize out of memory\n");
            return;
        }
        maskChars[width * height] = '\0';
        templateChars[width * height] = '\0';
        for (int i = 0; i < width * height; i++) {
            maskChars[i] = getTemplateOrMaskChar(irisMask[i], &printedError);
            templateChars[i] = getTemplateOrMaskChar(irisTemplate[i], &printedError);
        }
    }
    json_object *maskStr = json_object_new_string(maskChars);
    json_object *templateStr = json_object_new_string(templateChars);
    json_object *jwidth = json_object_new_int(width);
    json_object *jheight = json_object_new_int(height);
    json_object *arrayEncodingStr = json_object_new_string(value_arrayEncoding_Base64);
    if (maskStr == NULL || templateStr == NULL || jwidth == NULL || jheight == NULL || arrayEncodingStr == NULL) {
        mylog::Log(mylog::ERROR, "Encoding::Serialize out of memory\n");
        free(maskChars);
        free(templateChars);
        return;
    }
    json_object_object_add(jobj, key_width, jwidth);
    json_object_object_add(jobj, key_height, jheight);
    json_object_object_add(jobj, key_irisTemplate, templateStr);
    json_object_object_add(jobj, key_irisMask, maskStr);
    json_object_object_add(jobj, key_arrayEncoding, arrayEncodingStr);
    strncpy(jsonBuffer, json_object_to_json_string_ext(jobj, JSON_C_TO_STRING_PRETTY), ENCODING_DESERIALIZATION_BUFFER_LEN);
    json_object_put(jobj);
    free(maskChars);
    free(templateChars);
    return;
}
    
void Encoding::Serialize(std::ostream & out) const {
    char buff[ENCODING_DESERIALIZATION_BUFFER_LEN];
    Serialize(buff);
    out << buff << std::endl;
}

bool CharsToInts(const char* chars, int* ints, const int len)
{
    bool valid = true;
    for (int i = 0; i < len; i++) {
        switch (chars[i])
        {
            case '0':
                ints[i] = 0;
                break;
            case '1':
                ints[i] = 1;
                break;
            default:
                ints[i] = 0;
                valid = false;
                break;
        }
    }
    return valid;
}

bool IsAnyNull(json_object* a, json_object* b, json_object* c, json_object* d, json_object* e)
{
    bool anyNull = a == NULL || b == NULL || c == NULL || d == NULL || e == NULL;
    return anyNull;
}

bool IsCorrectTypes(json_object* widthObj, json_object* heightObj, json_object* c, json_object* d, json_object* e)
{
    bool typesCorrect = json_object_get_type(widthObj) == json_type_int
            && json_object_get_type(heightObj) == json_type_int
            && json_object_get_type(c) == json_type_string
            && json_object_get_type(d) == json_type_string
            && json_object_get_type(e) == json_type_string;
    return typesCorrect;
}

bool IsValidDims(const int width, const int height) {
    return width > 0 && width < 10000 && height > 0 && height < 1000;
}

void Encoding::Deserialize(std::istream& in)
{
    status = FAILURE;
    char buffer[ENCODING_DESERIALIZATION_BUFFER_LEN];
    memset(buffer, 0, ENCODING_DESERIALIZATION_BUFFER_LEN);
    int numToRead = ENCODING_DESERIALIZATION_BUFFER_LEN - 1;
    in.read(buffer, numToRead);
    int numRead = strlen(buffer);
    if (numRead < 1) {
        mylog::Log(mylog::ERROR, "Encoding::Deserialize nothing read from input stream\n");
        return;
    } else {
        mylog::Log(mylog::TRACE, "Encoding::Deserialize read %d bytes from input\n", numRead);
    }
    return Deserialize(buffer);
}

void Encoding::Deserialize(const char* buffer)
{
    status = FAILURE;
    if (irisTemplate != NULL || irisMask != NULL || width != 0 || height != 0) 
    {
        mylog::Log(mylog::ERROR, "Encoding::Deserialize this encoding object has already been deserialized/populated\n");
        return;
    }
        
    json_object *obj = json_tokener_parse(buffer);
    enum json_type type = json_object_get_type(obj);
    if (type != json_type_object) {
        mylog::Log(mylog::ERROR, "Encoding::Deserialize invalid json object type %s\n", json_type_to_name(type));
        json_object_put(obj);
        return;
    }
    json_object  *widthObj = json_object_object_get(obj, key_width);
    json_object *heightObj = json_object_object_get(obj, key_height);
    json_object *irisTemplateObj = json_object_object_get(obj, key_irisTemplate);
    json_object *irisMaskObj = json_object_object_get(obj, key_irisMask);
    json_object *arrayEncodingObj = json_object_object_get(obj, key_arrayEncoding);
    if (IsAnyNull(widthObj, heightObj, irisTemplateObj, irisMaskObj, arrayEncodingObj)
            || !IsCorrectTypes(widthObj, heightObj, irisTemplateObj, irisMaskObj, arrayEncodingObj)) {
        mylog::Log(mylog::ERROR, "Encoding::Deserialize input encoding serialization is malformed\n");
        json_object_put(obj);
        return;
    }
    const char* arrayEncodingStr = json_object_get_string(arrayEncodingObj);
    width = json_object_get_int(widthObj);
    height = json_object_get_int(heightObj);
    int expectedLen = width * height;
    int templateLen;
    int maskLen;
    if (!IsValidDims(width, height)) {
        mylog::Log(mylog::ERROR, "Encoding::Deserialize invalid dimensions\n");
        json_object_put(obj);
        return;
    }
    const char* irisTemplateStr = json_object_get_string(irisTemplateObj);
    const char* irisMaskStr = json_object_get_string(irisMaskObj);
    irisTemplate = (int*) malloc(sizeof(int) * width * height); // don't free here, even on error; gets freed by encoding destructor
    irisMask = (int*) malloc(sizeof(int) * width * height);
    bool templateValid = false, maskValid = false;
    if (strcmp(Encoding::value_arrayEncoding_ZeroAndOneChars, arrayEncodingStr) == 0) {
        templateLen = json_object_get_string_len(irisTemplateObj);
        maskLen = json_object_get_string_len(irisMaskObj);
        if (templateLen != expectedLen || maskLen != expectedLen) {
            mylog::Log(mylog::ERROR, "Encoding::Deserialize incongruent template/mask strings or one or both has unexpected length\n");
            json_object_put(obj);
            return;
        }
        templateValid = CharsToInts(irisTemplateStr, irisTemplate, expectedLen);
        maskValid = CharsToInts(irisMaskStr, irisMask, expectedLen);
    } else if (strcmp(Encoding::value_arrayEncoding_Base64, arrayEncodingStr) == 0) {
        std::string decodedTemplateStr = Base64::Decode(std::string(irisTemplateStr));
        unsigned char* decodedBytes = (unsigned char*) malloc(sizeof(unsigned char) * width * height);
        int nbytes = Base64::ToBytes(decodedTemplateStr, decodedBytes);
        Base64::ToInts(decodedBytes, nbytes, irisTemplate);
        std::string decodedMaskStr = Base64::Decode(std::string(irisTemplateStr));
        nbytes = Base64::ToBytes(decodedMaskStr, decodedBytes);
        Base64::ToInts(decodedBytes, nbytes, irisMask);
        free(decodedBytes);
    } else {
        mylog::Log(mylog::ERROR, "Encoding::Deserialization invalid array encoding \'%s\'\n", arrayEncodingStr);
        json_object_put(obj);
        return;
    }
    if (!templateValid || !maskValid) {
        mylog::Log(mylog::ERROR, "Encoding::Deserialization invalid characters in template or mask\n");
        json_object_put(obj);
        return;
    }
    json_object_put(obj);
    status = SUCCESS;
    return;
}

