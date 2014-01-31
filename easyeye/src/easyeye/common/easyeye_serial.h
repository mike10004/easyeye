/* 
 * File:   easyeye_serial.h
 * Author: mchaberski
 *
 * Created on January 25, 2014, 5:58 PM
 */

#ifndef EASYEYE_SERIAL_H
#define	EASYEYE_SERIAL_H

#include <string>
#include "easyeye_types.h"
#include <iostream>
#include <Masek.h>
#include <jsoncpp/json/json.h>
#include <vector>

namespace easyeye 
{

    class BitPacking
    {
    public:
        static void Pack(int* src, const size_t src_len, unsigned char* dst);
        static void Unpack(unsigned char const* src, const size_t src_len, int* dst);
        static void Unpack(const std::vector<unsigned char>& src, int* dst);
    private:
        BitPacking();
        static unsigned char SetBit( unsigned char ch, int i );
    };

    namespace serial
    {
        class Adapter
        {
        public:
            Adapter() {}
            virtual bool FromJson(const Json::Value& src, void* dst) = 0;
            virtual void ToJson(void* src, Json::Value& dst) = 0; 
        };

        template<typename T> void Serialize(T& data, Adapter* adapter, Json::Value& dst) {
            adapter->ToJson((void*) &data, dst);
        }
        
        template<typename T> bool Deserialize(const Json::Value& src, Adapter* adapter, T& data) {
            return adapter->FromJson(src, (void*) &data);
        }
        
        template<typename T> std::string Serialize(T& data, Adapter* adapter)
        {
            Json::Value dst;
            Serialize(data, adapter, dst);
            Json::StyledWriter writer;
            return writer.write(dst);
        }
        
        template<typename T> bool Deserialize(const std::string& json, Adapter* adapter, T& data)
        {
            Json::Reader reader;
            Json::Value src;
            bool parse_ok = reader.parse(json, src);
            if (!parse_ok) {
                return false;
            }
            return Deserialize(src, adapter, data);
        }

        class PointAdapter : public serial::Adapter 
        {
        public:
            bool FromJson(const Json::Value& src, void* dst);
            void ToJson(void* src, Json::Value& dst);
        };

        class BoundaryPairAdapter : public serial::Adapter 
        {
        public:
            bool FromJson(const Json::Value& src, void* dst);
            void ToJson(void* src, Json::Value& dst);
        };

        class EyelidsLocationAdapter : public serial::Adapter
        {
        public:
            bool FromJson(const Json::Value& src, void* dst);
            void ToJson(void* src, Json::Value& dst);
        };
        
        class IntCircleAdapter : public serial::Adapter 
        {
        public:
            bool FromJson(const Json::Value& src, void* dst);
            void ToJson(void* src, Json::Value& dst);
        };

        void Serialize(const cv::Point2i& p, Json::Value& dst);
        bool Deserialize(Json::Value& src, cv::Point2i& p);
        void Serialize(const IntCircle& c, Json::Value& dst);
        bool Deserialize(const Json::Value& src, IntCircle& dst);
        void Serialize(const BoundaryPair& c, Json::Value& dst);
        bool Deserialize(const Json::Value& src, BoundaryPair& dst);
        void Serialize(const EyelidsLocation& c, Json::Value& dst);
        bool Deserialize(const Json::Value& src, EyelidsLocation& dst);
        
    }
}
#endif	/* EASYEYE_SERIAL_H */

