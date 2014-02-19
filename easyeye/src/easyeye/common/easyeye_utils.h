/* 
 * File:   easyeye_utils.h
 * Author: mchaberski
 *
 * Created on July 16, 2013, 1:14 PM
 */

#ifndef EASYEYE_UTILS_H
#define	EASYEYE_UTILS_H

#include <list>
#include <vector>
#include <opencv2/core/core.hpp>
#include <string>
#include <sys/types.h>
#include <list>
#include <iostream>

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
    static bool IsDirectory(const std::string& path);
    static bool IsFile(const std::string& path);
    static bool IsDirectory(const std::string& path, int* errorCode);
    static bool IsFile(const std::string& path, int*errorCode);
    static bool MakeDirs(const std::string& path, mode_t mode, int *errorCode);
    static bool MakeDirs(const std::string& path, mode_t mode);
    static bool MakeDirs(const std::string& path, int* error_code);
    static bool MakeDirs(const std::string& path);
    static const mode_t DEFAULT_MKDIR_MODE = 0755;
private:
    const static int ERRNO_MAX = 34;
public:
    const static int ERR_EMPTY_PATH = ERRNO_MAX + 1;
private:
    static std::string temp_directory_path_windows(int* errorCode);
    static std::string temp_directory_path_unix(int* errorCode);
    static bool pathname_stat_has_mode(const std::string& path, const int queryMode, int*errorCode);
    
};

class Streams
{
public:
    static void Copy(std::istream& in, std::ostream& out);
    static std::string ToString(std::istream& in);
private: 
    Streams();
};

class PathInfo
{
public:
    PathInfo();
    PathInfo(const std::string& path, const std::string& dirname, 
        const std::string& filename, const std::string& filename_stem, 
        const std::string& filename_extension);
    
    /**
     * The path provided as input to the Files::GetPathInfo function. Unchanged
     * from the input.
     */
    std::string path;
    
    /**
     * Path of the directory that is the parent of the pathname this instance
     * represents.
     */
    std::string dirname;
    std::string filename;
    
    /**
     * File name without its file extension or path. This is similar to the 
     * basename unix command. The result does not include the '.'.
     */
    std::string filename_stem;
    
    /**
     * File extension for the given file name, or the empty string if the 
     * file has no extension. The result does not include the '.'.
     */
    std::string filename_extension;
    std::string ToString() const;
    bool Equals(const PathInfo& other) const;
};

class Files
{
public:
    static bool Write(const std::string& data, const std::string& pathname);
    static bool Copy(std::istream& in, const std::string& pathname);
    static std::string Read(const std::string& pathname);
    static PathInfo GetPathInfo(const std::string& pathname);
    static void ParsePathInfo(const std::string& pathname, PathInfo& path_info);
    static std::string GetDirname(const std::string& pathname);
    static std::string GetFilename(const std::string& pathname);
    static std::string GetFilenameStem(const std::string& pathname);
    static std::string GetFilenameExtension(const std::string& pathname);
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
    static void Print(const std::vector<unsigned char>& v, std::ostream& out, size_t limit);
    static void Print(const std::vector<unsigned char>& v, std::ostream& out);
    static std::string ToString(const std::vector<unsigned char>& bytes, size_t limit);
    static std::string ToString(const std::vector<unsigned char>& bytes);
    static std::string ToString(const std::vector<double>& values, size_t limit);
    static std::string ToString(const std::vector<double>& values);
    static void Print(const std::vector<double>& v, std::ostream& out, size_t limit);
    static void Print(const std::vector<double>& v, std::ostream& out);
    static std::string ToString(const std::vector<int>& values, size_t limit);
    static std::string ToString(const std::vector<int>& values);
    static void Print(const std::vector<int>& v, std::ostream& out, size_t limit);
    static void Print(const std::vector<int>& v, std::ostream& out);
    static void CopyTo(const std::vector<unsigned char>& bytev, unsigned char* array, size_t limit);
    static void AddAll(char** src_array, const size_t array_len, std::vector<std::string>& dst);
    static void CopyTo(const std::vector<float>& src, float* array, size_t limit);
    static void CopyFrom(int* src, std::vector<int>& dst, size_t limit);
    static size_t Cardinality(std::vector< std::vector<double> >& vectors);
private:
    Vectors();
};

class Strings
{
public:
    static bool Equals(const std::string& a, const std::string& b);
    static bool EqualsIgnoreCase(const std::string& a, const std::string& b);
    static std::vector<std::string> Split(const std::string &s, char delim);
    static std::vector<std::string> Split(const std::string &s, char delim, bool allow_empty);
    std::string Join(const std::list<std::string>& pieces, const std::string& delimiter);
    class Splitter
    {
    public:
        Splitter(char delim, bool allow_empty);
        size_t Split(const std::string &s);
        size_t Split(std::istream& in);
        std::vector<std::string> tokens;
    private:
        char delim_;
        bool allow_empty_;
    };
    class Joiner
    {
    public:
        Joiner(const std::string& glue);
        void Join(const std::list<std::string>& pieces, std::ostream& out);
        std::string Join(const std::list<std::string>& pieces);
    private:
        const std::string delim_;
    };
private:
    Strings();
};

}
#endif	/* EASYEYE_UTILS_H */

