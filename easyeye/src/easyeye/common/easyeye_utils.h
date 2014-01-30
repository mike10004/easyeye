/* 
 * File:   easyeye_utils.h
 * Author: mchaberski
 *
 * Created on July 16, 2013, 1:14 PM
 */

#ifndef EASYEYE_UTILS_H
#define	EASYEYE_UTILS_H

#include <vector>
#include <opencv2/core/core.hpp>
#include <string>

#ifdef _WIN32
#define EASYEYE_ISWINDOWS
#endif

namespace easyeye
{
    
class IOUtils
{
public:
    static std::string GetTempDirPath();
    static std::string GetTempDirPath(int* errorCode);
    static bool is_directory(const std::string& path);
    static bool is_file(const std::string& path);
    static bool is_directory(const std::string& path, int* errorCode);
    static bool is_file(const std::string& path, int*errorCode);
    static std::string ToString(std::istream& in);
private:
    static std::string temp_directory_path_windows(int* errorCode);
    static std::string temp_directory_path_unix(int* errorCode);
    static bool pathname_stat_has_mode(const std::string& path, const int queryMode, int*errorCode);
};

class Streams
{
public:
    static void Copy(std::istream& in, std::ostream& out);
private: 
    Streams();
};

class Files
{
public:
    static bool Write(const std::string& data, const std::string& pathname);
    static bool Copy(std::istream& in, const std::string& pathname);
    static std::string Read(const std::string& pathname);
private:
    Files();
};

class Arrays
{
public:
    static std::string ToString(int const* array, size_t array_len, size_t preview_len);
    static std::string ToString(unsigned char const* array, size_t array_len, size_t preview_len);
    static size_t CountNotEqual(int const* array1, int const* array2, size_t len);
    static size_t CountNotEqual(unsigned char const* array1, unsigned char const* array2, size_t len);
private:
    Arrays();
};

class Vectors
{
public:
    static std::string ToString(const std::vector<unsigned char>& bytes, size_t preview_len);
    static void CopyTo(const std::vector<unsigned char>& bytev, unsigned char* array, size_t limit);
    static void AddAll(char** src_array, const size_t array_len, std::vector<std::string>& dst);
};

}
#endif	/* EASYEYE_UTILS_H */

