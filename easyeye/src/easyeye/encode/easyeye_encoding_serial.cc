#include <sstream>
#include <string>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <vector>
#include <jsoncpp/json/json.h>
#include "../common/mylog.h"
#include "../common/easyeye_types.h"
#include "../common/easyeye_utils.h"
#include "../common/base64.h"
#include "easyeye_encode.h"

using mylog::Logs;
using namespace std;
using namespace easyeye;

static string ToPackedBase64(int* src_values, size_t num_values)
{
    size_t num_bytes = num_values / 8;
    unsigned char* bytes = new unsigned char[num_bytes];
    if (bytes == NULL) {
        Logs::GetLogger().Log(mylog::ERROR, "Encoding::Serialize out of memory\n");
        return string("");
    }
    BitPacking::Pack(src_values, num_values, bytes);
    Base64 codec;
    string encoded = codec.Encode(bytes, num_bytes);
    delete bytes;
    return encoded;
}

bool IsValidDims(const int width, const int height) {
    return width > 0 && width < 10000 && height > 0 && height < 1000;
}


static bool FromPackedBase64(const string& encoded_values_str, int* dst, size_t num_dst_values)
{
    Base64 codec;
    int expected_num_bytes = num_dst_values / 8;
    vector<unsigned char> byte_vector;
    bool valid_encoding = codec.Decode(encoded_values_str, byte_vector);
    if (!valid_encoding) {
        Logs::GetLogger().Log(mylog::ERROR, "Encoding::UnpackBase64 invalid base-64 encoding \n");
        return false;
    }
    if (byte_vector.size() != expected_num_bytes) {
        Logs::GetLogger().Log(mylog::ERROR, "Encoding::Deserialize wrong number of bytes decoded\n");
        return false;
    }
    BitPacking::Unpack(byte_vector, dst);
    Logs::GetLogger().Log(mylog::TRACE, "Encoding::FromPackedBase64 %s\n", Arrays::ToString(dst, num_dst_values, 64).c_str());
    return true;
}
//
//void Encoding::Deserialize(const std::string& json_str)
//{
//    const char* buffer = json_str.c_str();
//    status = Result::FAILURE;
//    if (irisTemplate != NULL || irisMask != NULL || width_ != 0 || height_ != 0) 
//    {
//        Logs::GetLogger().Log(mylog::ERROR, "Encoding::Deserialize this encoding object has already been deserialized/populated\n");
//        return;
//    }
//        
//    json_object *obj = json_tokener_parse(buffer);
//    enum json_type type = json_object_get_type(obj);
//    if (type != json_type_object) {
//        Logs::GetLogger().Log(mylog::ERROR, "Encoding::Deserialize invalid json object type %s\n", json_type_to_name(type));
//        json_object_put(obj);
//        return;
//    }
//    json_object *arrayEncodingObj = json_object_object_get(obj, key_arrayEncoding);
//    if (strcmp(json_object_get_string(arrayEncodingObj), value_arrayEncoding_Base64) != 0) {
//        Logs::GetLogger().Log(mylog::ERROR, "Encoding::Deserialize unsupported data encoding: %s\n", json_object_get_string(arrayEncodingObj));
//        json_object_put(obj);
//        return;
//    }
//    json_object  *widthObj = json_object_object_get(obj, key_width);
//    json_object *heightObj = json_object_object_get(obj, key_height);
//    width_ = json_object_get_int(widthObj);
//    height_ = json_object_get_int(heightObj);
//    if (!IsValidDims(width_, height_)) {
//        Logs::GetLogger().Log(mylog::ERROR, "Encoding::Deserialize invalid dimensions specified by serialized encoding %d x %d\n", width_, height_);
//        json_object_put(obj);
//        return;
//    }
//    if (irisTemplate != NULL) {
//        free(irisTemplate);
//    }
//    irisTemplate = (int*) malloc(sizeof(int) * width_ * height_);
//    if (irisMask != NULL) {
//        free(irisMask);
//    }
//    irisMask = (int*) malloc(sizeof(int) * width_ * height_);
//    if (irisTemplate == NULL || irisMask == NULL) {
//        Logs::GetLogger().Log(mylog::ERROR, "Encoding::Deserialize out of memory");
//        if (irisTemplate != NULL) free(irisTemplate);
//        if (irisMask != NULL) free(irisMask);
//        json_object_put(obj);
//        return;
//    }
//    bool template_ok = UnpackBase64FromChild(obj, key_irisTemplate, irisTemplate, width_ * height_);
//    bool mask_ok = UnpackBase64FromChild(obj, key_irisMask, irisMask, width_ * height_);
//    if (!template_ok || !mask_ok) {
//        free(irisTemplate);
//        free(irisMask);
//    }
//    json_object_put(obj);
//    status = (template_ok && mask_ok) ? Result::SUCCESS : Result::FAILURE;
//    return;
//}


//
//void Encoding::Deserialize(std::istream& in)
//{
//    status = Result::FAILURE;
//    char buffer[ENCODING_DESERIALIZATION_BUFFER_LEN];
//    memset(buffer, 0, ENCODING_DESERIALIZATION_BUFFER_LEN);
//    int numToRead = ENCODING_DESERIALIZATION_BUFFER_LEN - 1;
//    in.read(buffer, numToRead);
//    int numRead = strlen(buffer);
//    if (numRead < 1) {
//        Logs::GetLogger().Log(mylog::ERROR, "Encoding::Deserialize nothing read from input stream\n");
//        return;
//    } 
//    Logs::GetLogger().Log(mylog::TRACE, "Encoding::Deserialize read %d bytes from input\n", numRead);
//    Deserialize(std::string(buffer));
//}


void serial::EncodingAdapter::ToJson(void* src, Json::Value& dst) 
{
    Encoding& encoding = *((Encoding*) src);
    dst["width"] = encoding.width();
    dst["height"] = encoding.height();
    dst["nscales"] = encoding.nscales();
    string packed_mask = ToPackedBase64(encoding.irisMask, encoding.width() * encoding.height());
    string packed_template = ToPackedBase64(encoding.irisTemplate, encoding.width() * encoding.height());
    dst["irisTemplate"] = packed_template;
    dst["irisMask"] = packed_mask;
    dst["status"] = encoding.status;
}
    
bool serial::EncodingAdapter::FromJson(const Json::Value& src, void* dst)
{
    Encoding& encoding = *((Encoding*) dst);
    int width = src.get("width", 0).asInt();
    int height = src.get("height", 0).asInt();
    int nscales = src.get("nscales", 0).asInt();
    size_t num_values = width * height;
    if (num_values <= 0) {
        return false;
    }
    int* irisTemplate = new int[num_values];
    int* irisMask = new int[num_values];
    string packed_template = src.get("irisTemplate", "").asString();
    FromPackedBase64(packed_template, irisTemplate, num_values);
    string packed_mask = src.get("irisMask", "").asString();
    FromPackedBase64(packed_mask, irisMask, num_values);
    encoding.CopyFrom(width, height, nscales, irisTemplate, irisMask);
    encoding.status = (Encoding::Status) src.get("status", Result::FAILURE).asInt();
    delete irisTemplate;
    delete irisMask;
    bool has_all_members = src.isMember("irisMask")
            && src.isMember("irisTemplate")
            && src.isMember("width")
            && src.isMember("height")
            && src.isMember("nscales")
            && src.isMember("status");
    return has_all_members;
}


bool serial::Deserialize(const std::string& json, Encoding& encoding)
{
    EncodingAdapter adapter;
    return Deserialize(json, &adapter, encoding);
}

std::string serial::Serialize(const Encoding& encoding)
{
    EncodingAdapter adapter;
    return Serialize(encoding, &adapter);
}
