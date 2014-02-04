#include "easyeye_types.h"
#include "easyeye_serial.h"
#include <iostream>
#include <sstream>
#include <string>
#include "mylog.h"

using namespace easyeye;
using namespace std;
using mylog::Logs;

bool serial::PointAdapter::FromJson(const Json::Value& src, void* dst) {
        cv::Point2i* pp = (cv::Point2i*)dst;
        pp->x = src.get("x", 0).asInt();
        pp->y = src.get("y", 0).asInt();
        return src.isMember("x") && src.isMember("y");
    }

void serial::PointAdapter::ToJson(void* src, Json::Value& dst) {
        cv::Point2i* pp = (cv::Point2i*) src;
        dst["x"] = pp->x;
        dst["y"] = pp->y;
        return;
}

void serial::IntCircleAdapter::ToJson(void* src, Json::Value& dst) 
{
    IntCircle& c = *((IntCircle*)src);
    Json::Value center;
    Serialize(c.center, center);
    dst["center"] = center;
    dst["radius"] = c.radius;
}

bool serial::IntCircleAdapter::FromJson(const Json::Value& src, void* data)
{
    IntCircle& dst = *((IntCircle*) data);
    Json::Value center = src["center"];
    bool ok = Deserialize(center, dst.center);
    dst.radius = src["radius"].asInt();
    return ok && src.isMember("radius") && src.isMember("center");
}

void serial::BoundaryPairAdapter::ToJson(void* src, Json::Value& dst) 
{
    BoundaryPair& bp = *((BoundaryPair*)src);
    Json::Value iris, pupil;
    Serialize(bp.iris, iris);
    Serialize(bp.pupil, pupil);
    dst["iris"] = iris;
    dst["pupil"] = pupil;
}

bool serial::BoundaryPairAdapter::FromJson(const Json::Value& src, void* dst)
{
    BoundaryPair& data = *((BoundaryPair*) dst);
    IntCircle iris, pupil;
    Deserialize(src["iris"], iris);
    data.set_iris(iris);
    Deserialize(src["pupil"], pupil);
    data.set_pupil(pupil);
    return src.isMember("iris") && src.isMember("pupil");
}

void serial::EyelidsLocationAdapter::ToJson(void* src, Json::Value& dst)
{
    EyelidsLocation& data = *((EyelidsLocation*)src);
    Json::Value ellipse_vals(Json::arrayValue);
    for (int i = 0; i < EyelidsLocation::NUM_ELLIPSE_VALS; i++) {
        ellipse_vals.append(data.ellipse_vals[i]);
    }
    dst["ellipse_vals"] = ellipse_vals;
    dst["angle"] = data.angle;
}

bool serial::EyelidsLocationAdapter::FromJson(const Json::Value& src, void* dst)
{
    EyelidsLocation& data = *((EyelidsLocation*)dst);
    Json::Value nullValue;
    Json::Value ellipse_vals = src.get("ellipse_vals", nullValue);
    if (ellipse_vals.size() != EyelidsLocation::NUM_ELLIPSE_VALS) {
        return false;
    }
    for (int i = 0; i < EyelidsLocation::NUM_ELLIPSE_VALS; i++) {
        data.ellipse_vals[i] = ellipse_vals[i].asInt();
    }
    data.angle = src.get("angle", 0.0d).asDouble();
    return src.isMember("angle") && src.isMember("ellipse_vals");
}

void serial::Serialize(const cv::Point2i& p, Json::Value& dst)
{
    PointAdapter a;
    Serialize(p, &a, dst);
}

bool serial::Deserialize(Json::Value& src, cv::Point2i& v)
{
    PointAdapter a;
    return Deserialize(src, &a, v);
}

void serial::Serialize(const IntCircle& src, Json::Value& dst)
{
    IntCircleAdapter a;
    Serialize(src, &a, dst);
}

bool serial::Deserialize(const Json::Value& src, IntCircle& v)
{
    IntCircleAdapter a;
    return Deserialize(src, &a, v);
}

void serial::Serialize(const BoundaryPair& src, Json::Value& dst)
{
    BoundaryPairAdapter a;
    Serialize(src, &a, dst);
}

bool serial::Deserialize(const Json::Value& src, BoundaryPair& v)
{
    BoundaryPairAdapter a;
    return Deserialize(src, &a, v);
}

void serial::Serialize(const EyelidsLocation& src, Json::Value& dst)
{
    EyelidsLocationAdapter a;
    Serialize(src, &a, dst);
}

bool serial::Deserialize(const Json::Value& src, EyelidsLocation& v)
{
    EyelidsLocationAdapter a;
    return Deserialize(src, &a, v);
}

unsigned char BitPacking::SetBit( unsigned char ch, int i ) 
{
  unsigned char mask = 1 << i ;  // we could cast to unsigned char, just to be safe
  return mask | ch ;  // using bitwise OR
}



/**
 * Converts a source array of integers, each of which represents a bit and is the 
 * value 0 or 1, into an array of bytes, each of which represents 8 bits from 
 * the source integer array.
 * @param src
 * @param len
 * @param dst
 * @return 
 */
void BitPacking::Pack(int* src, const size_t len, unsigned char* dst)
{
//    static int bits[] = {
    int n = 0;
    if ((len % 8) != 0) {
        Logs::GetLogger().Log(mylog::ERROR, "input array length %d is not a multiple of 8\n", len);
    }
    unsigned char b;
    int srcval;
    for (int i = 0; i < len/ 8; i++) {
        b = 0;
        for (int j = 0; j < 8; j++) {
            srcval = src[i * 8 + j];
            if (srcval != 0) {
                b = SetBit(b, j);
            }
        }
        dst[i] = b;
        n += 1;
    }
}

void BitPacking::Unpack(const vector<unsigned char>& src, int* dst)
{
    unsigned char b;
    int val;
    int n = 0;
    const size_t len = src.size();
    for (size_t i = 0; i < len; i++) {
        b = src[i];
        for (int j = 0; j < 8; j++) {
            if ((b & (1 << j)) != 0) val = 1;
            else val = 0;
            dst[i * 8 + j] = val;
            n += 1;
        }
    }
}

void BitPacking::Unpack(unsigned char const* src, const size_t len, int* dst)
{
    unsigned char b;
    int val;
    int n = 0;
    for (size_t i = 0; i < len; i++) {
        b = src[i];
        for (int j = 0; j < 8; j++) {
            if ((b & (1 << j)) != 0) val = 1;
            else val = 0;
            dst[i * 8 + j] = val;
            n += 1;
        }
    }
}
