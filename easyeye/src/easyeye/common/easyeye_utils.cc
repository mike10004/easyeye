/*
 * easyeye_utils.cc
 * 
 * (c) 2013 IBG, A Novetta Solutions Company
 */

#include <list>
#include <algorithm>
#include <fstream>
#include <string>
#include <cerrno>
#include <iostream>
#include "mylog.h"
#include "easyeye_utils.h"
#include <sys/stat.h>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <vector>
#include <sstream>
#include <sys/stat.h>
#include <sys/types.h>
#include <libgen.h>

#ifdef EASYEYE_ISWINDOWS
#include <Windows.h>
#endif

using mylog::Logs;
using namespace std;
using namespace easyeye;
using cv::Point2i;
using cv::Point2f;
using cv::RotatedRect;

string IOUtils::GetTempDirPath()    
{
    int errorCode = 0;
    string p = GetTempDirPath(&errorCode);
    return p;
}
    
string IOUtils::GetTempDirPath(int* errorCode)
{
#ifdef EASYEYE_ISWINDOWS
    return temp_directory_path_windows(errorCode);
#else
    return temp_directory_path_unix(errorCode);
#endif
    
}
bool IOUtils::IsDirectory(const string& path){
    int ec = 0;
    bool yes = IsDirectory(path, &ec);
    return yes;
}

bool IOUtils::IsFile(const string& path)
{
    int ec = 0;
    bool yes = IsFile(path, &ec);
    return yes;
}


bool IOUtils::IsDirectory(const string& path, int *errorCode) 
{
    return pathname_stat_has_mode(path, S_IFDIR, errorCode);
}

bool IOUtils::IsFile(const string& path, int* errorCode)
{
    return pathname_stat_has_mode(path, S_IFREG, errorCode);
}

bool IOUtils::pathname_stat_has_mode(const string& path, const int queryMode, int*errorCode)
{
    struct stat s;
    (*errorCode) = stat(path.c_str(),&s);
    if( (*errorCode) == 0 ) {
        return ( s.st_mode & queryMode ) != 0;
    } else {
        Logs::GetLogger().Log(mylog::DEBUG, "Utils::pathname_stat_has_mode stat call failed with %d\n", *errorCode);
        return false;
    }    
}

//  Distributed under the Boost Software License, Version 1.0.
//  See http://www.boost.org/LICENSE_1_0.txt
//  See library home page at http://www.boost.org/libs/filesystem
// contributed by Jeff Flinn for Boost filesystem library
// http://www.boost.org/doc/libs/1_47_0/libs/filesystem/v3/src/operations.cpp
  string IOUtils::temp_directory_path_unix(int* errorCode)
  {
#ifndef _WIN32
      const char* val = 0;
      
      (val = std::getenv("TMPDIR" )) ||
      (val = std::getenv("TMP"    )) ||
      (val = std::getenv("TEMP"   )) ||
      (val = std::getenv("TEMPDIR"));
      
      string p((val!=0) ? val : "/tmp");
      if (p.empty() || (!IsDirectory(p, errorCode)))
      {
        (*errorCode) = ENOTDIR;
        Logs::GetLogger().Log(mylog::ERROR, "Utils::temp_directory_path_unix returned bad temp dir path\n");
      } else {
          (*errorCode) = 0;
      }
      return p;
#else
	  *errorCode = 1;
	  return string();
#endif
  }
  
  string IOUtils::temp_directory_path_windows(int* errorCode)
  {
#ifdef EASYEYE_ISWINDOWS
	  //char buf[16384];
	  //int len;
	  //len = GetTempPathW(16384, buf);
	  //if (len == 0) {
		 // *errorCode = 1;
		 // return string();
	  //} else if (len > 16384) {
		 // *errorCode = 2;
		 // return string();
	  //}
	  //string p(buf);
   //   if (!is_directory(p, *ec))
   //   {
   //     ::SetLastError(ENOTDIR);
   //     Logs::GetLogger().Log(mylog::ERROR, "Utils::temp_directory_path_windows returned bad temp dir path");
   //     return path();
   //   }
   //   return p;
	  /* Not yet implemented */
	  (*errorCode) = 1;
	  return string();
#   else
      (*errorCode) = 1;
      return string();
#   endif
  }

string Vectors::ToString(const vector<unsigned char>& bytes)
{
    return ToString(bytes, bytes.size());
}
  
string Vectors::ToString(const vector<double>& values)
{
    return ToString(values, values.size());
}
  
string Vectors::ToString(const vector<unsigned char>& bytes, size_t limit)
{
    ostringstream ss;
    Print(bytes, ss, limit);
    return ss.str();
}

string Vectors::ToString(const vector<double>& values, size_t limit)
{
    ostringstream ss;
    Print(values, ss, limit);
    return ss.str();
}

void Vectors::Print(const std::vector<unsigned char>& bytes, std::ostream& out)
{
    Print(bytes, out, bytes.size());
}

void Vectors::Print(const std::vector<unsigned char>& bytes, std::ostream& out, size_t limit)
{
    out << '[';
    if (!bytes.empty()) {
        out.width(2);
        out << (int) (bytes[0]);
        for (size_t i = 1; i < limit && i < bytes.size() ;i++) {
            out << ", " << (int) (bytes[i]);
        }
        if (limit < bytes.size()) {
            out << ", ...";
        }
    }
    out << ']';
}

void Vectors::Print(const std::vector<double>& values, std::ostream& out)
{
    Print(values, out, values.size());
}

void Vectors::Print(const std::vector<double>& values, std::ostream& out, size_t limit)
{
    out << '[';
    if (!values.empty()) {
        out << values[0];
        for (size_t i = 1; i < limit && i < values.size() ;i++) {
            out << ", " << values[i];
        }
        if (limit < values.size()) {
            out << ", ...";
        }
    }
    out << ']';
}

string Arrays::ToString(int const* array, size_t array_len, size_t preview_len)
{
    ostringstream ss;
    ss << "[";
    preview_len = min(preview_len, array_len);
    if (preview_len > 0) {
        ss << array[0];
    }
    for (size_t i = 1; i < preview_len; i++) {
        ss << ", " << array[i];
    }
    if (array_len > preview_len) {
        ss << "...";
    }
    ss << "]\n";    
    return ss.str();
}

string Arrays::ToString(unsigned char const* array, size_t array_len, size_t preview_len)
{
    ostringstream ss;
    ss << "[";
    preview_len = min(preview_len, array_len);
    if (preview_len > 0) {
        ss << (int) array[0];
    }
    for (size_t i = 1; i < preview_len; i++) {
        ss << ", " << (int) array[i];
    }
    if (array_len > preview_len) {
        ss << "...";
    }
    ss << "]\n";    
    return ss.str();
}

void Vectors::CopyFrom(int* array, std::vector<int>& dst, size_t limit) 
{
    for (size_t i = 0; i < limit; i++) {
        dst.push_back(array[i]);
    }
}

void Vectors::CopyTo(const vector<float>& src, float* array, size_t limit) {
    for (size_t i = 0; i < limit && i < src.size(); i++) {
        array[i] = src[i];
    }
}

size_t Vectors::Cardinality(std::vector<std::vector<double> >& vectors)
{
    if (vectors.empty()) {
        return 0;
    }
    size_t card = 1;
    for (size_t i = 0; i < vectors.size(); i++) {
        card *= vectors[i].size();
        if (card == 0) {
            break;
        }
    }
    return card;
}

void Vectors::CopyTo(const vector<unsigned char>& bytev, unsigned char* array, size_t limit)
{
    for (size_t i = 0; i < limit && i < bytev.size(); i++) {
        array[i] = bytev[i];
    }
}

size_t Arrays::CountNotEqual(int const* a, int const* b, size_t len)
{
    int cmp  = memcmp(a, b, sizeof(int) * len);
    size_t numWrong = 0;
    if (cmp  != 0) {
        for (size_t i = 0; i < len; i++) {
            if (a[i] != b[i]) {
                numWrong++;
            }
        }
    }
    return numWrong;
}

size_t Arrays::CountNotEqual(unsigned char const* a, unsigned char const* b, size_t len)
{
    int cmp  = memcmp(a, b, sizeof(unsigned char) * len);
    size_t numWrong = 0;
    if (cmp  != 0) {
        for (size_t i = 0; i < len; i++) {
            if (a[i] != b[i]) {
                numWrong++;
            }
        }
    }
    return numWrong;
}

string Streams::ToString(std::istream& in)
{
    ostringstream ss;
    ss << in.rdbuf();
    return ss.str();
}

bool Files::Write(const std::string& data, const std::string& pathname)
{
    ofstream fout(pathname.c_str());
    bool ok = false;
    if (fout.is_open()) {
        fout << data;
        ok = fout.good();
        fout.close();
    }
    return ok;
}

string Files::Read(const std::string& pathname)
{
    ifstream in(pathname.c_str());
    ostringstream ss;
    if (in.is_open()) 
    {
        ss << in.rdbuf();
        in.close();
    }
    return ss.str();
}

void Vectors::AddAll(char** src_array, const size_t array_len, vector<string>& dst)
{
    for (size_t i = 0; i < array_len; i++) {
        dst.push_back(src_array[i]);
    }
}

static void MakeDirFromParts(const std::vector<string>& parts, size_t end_exclusive, bool absolute, mode_t mode, int *error_code)
{
    ostringstream path_ss;
    if (absolute) {
        path_ss << '/';
    }
    for (size_t i = 0; i < end_exclusive; i++) {
        path_ss << parts[i] << '/';
    }
    string path = path_ss.str();
    int status = mkdir(path.c_str(), mode);
    if (status != 0) {
        *error_code = errno;
    } else {
        *error_code = 0;
    }
}

bool IOUtils::MakeDirs(const std::string& path, mode_t mode, int* error_code)
{
    int code = 0;
    if (!path.empty()) {
        bool absolute = path[0] == '/';
        vector<string> path_parts = Strings::Split(path, '/', false); // false = no empties allowed
        if (!path_parts.empty()) {
            size_t num_parts = path_parts.size();
            for (size_t i = 1; i <= num_parts; i++) {
                MakeDirFromParts(path_parts, i, absolute, mode, &code);
                if (code != 0 && code != EEXIST) {
                    break;
                }
            }
        }
    }
    *error_code = code;
    return IsDirectory(path);
}

bool IOUtils::MakeDirs(const std::string& path)
{
    int error_code;
    return MakeDirs(path, &error_code);
}

bool IOUtils::MakeDirs(const std::string& path, mode_t mode)
{
    int error_code;
    return MakeDirs(path, mode, &error_code);
}

bool IOUtils::MakeDirs(const std::string& path, int* error_code)
{
    return MakeDirs(path, DEFAULT_MKDIR_MODE, error_code);
}

Strings::Splitter::Splitter(char delim, bool allow_empty)
    : delim_(delim), allow_empty_(allow_empty)
{
}

size_t Strings::Splitter::Split(const std::string& s)
{
    istringstream ss(s);
    return Split(ss);
}

size_t Strings::Splitter::Split(istream& in)
{
    std::string item;
    size_t n = 0;
    while (std::getline(in, item, delim_)) {
        if (allow_empty_ || !item.empty()) {
            tokens.push_back(item);
            n++;
        }
    }
    return n;
}

Strings::Joiner::Joiner(const std::string& glue) : delim_(glue)
{
}
    

void Strings::Joiner::Join(const std::list<std::string>& pieces, std::ostream& out)
{
    if (pieces.empty()) {
        return;
    }
    list<string>::const_iterator it = pieces.begin();
    out << *it;
    ++it;
    for (; it != pieces.end(); ++it) {
        out << delim_;
        out << *it;
    }
}

string Strings::Joiner::Join(const list<string>& pieces)
{
    ostringstream ss;
    Join(pieces, ss);
    return ss.str();
}

vector<string> Strings::Split(const std::string& s, char delim, bool allow_empty)
{
    Splitter splitter(delim, allow_empty);
    splitter.Split(s);
    return splitter.tokens;
}

vector<string> Strings::Split(const std::string& s, char delim)
{
    return Split(s, delim, false);
}

string Strings::Join(const std::list<std::string>& pieces, const string& delimiter)
{
    Joiner joiner(delimiter);
    return joiner.Join(pieces);
}

PathInfo::PathInfo() 
    : path(), dirname(), filename(), filename_stem(), filename_extension()
{
    
}
    
PathInfo::PathInfo(const std::string& path_, const std::string& dirname_, 
        const std::string& filename_, const std::string& filename_stem_, 
        const std::string& filename_extension_)
    : path(path_), 
      dirname(dirname_), 
      filename(filename_), 
      filename_stem(filename_stem_), 
      filename_extension(filename_extension_)
{
}

static char* portable_strdup(const std::string& src)
{
    char* path = new char[src.length() + 1];
    path[src.length()] = '\0';
    strncpy(path, src.c_str(), src.length());
    return path;
}

static string ParseFilenameStem(const string& base)
{
    if (base.empty()) {
        return string("");
    }
    if (base.find_first_not_of('.') == string::npos) {
        return base;
    }
    size_t last_dot_pos = base.find_last_of('.');
    if (last_dot_pos == string::npos || last_dot_pos == 0 ) {
        return base; // name like "hello" or ".bashrc"
    }
    return base.substr(0, last_dot_pos);
}

static string ParseFilenameExtension(const string& base)
{
    if (base.empty()) {
        return base;
    }
    if (base.find_first_not_of('.') == string::npos) {
        return string("");
    }
    size_t last_dot_pos = base.find_last_of('.');
    if (last_dot_pos == string::npos || last_dot_pos == 0) {
        return string(""); // no extension, e.g. "hello" or ".bashrc"
    }
    return base.substr(last_dot_pos + 1, base.length() - (last_dot_pos + 1));
}

void Files::ParsePathInfo(const std::string& pathname, PathInfo& path_info)
{
    path_info.path.assign(pathname);    
    char* dirname_src = portable_strdup(pathname);
    char* basename_src = portable_strdup(pathname);
    char* dirname_dst = dirname(dirname_src);
    char* basename_dst = basename(basename_src);
    path_info.dirname.assign(dirname_dst);
    path_info.filename.assign(basename_dst);
    delete dirname_src;
    delete basename_src;
    path_info.filename_stem.assign(ParseFilenameStem(path_info.filename));
    path_info.filename_extension.assign(ParseFilenameExtension(path_info.filename));
}

std::string PathInfo::ToString() const
{
    ostringstream ss;
    ss << "PathInfo{path=" << path
            << ",dir=" << dirname
            << ",filename=" << filename
            << ",stem=" << filename_stem
            << ",ext=" << filename_extension << "}";
    return ss.str();
}

bool PathInfo::Equals(const PathInfo& other) const
{
    return (path.compare(other.path) == 0)
            && (dirname.compare(other.dirname) == 0)
            && (filename.compare(other.filename) == 0)
            && (filename_stem.compare(other.filename_stem) == 0)
            && (filename_extension.compare(other.filename_extension) == 0);
}

PathInfo Files::GetPathInfo(const std::string& pathname)
{
    PathInfo path_info;
    ParsePathInfo(pathname, path_info);
    return path_info;
}

std::string Files::GetFilename(const std::string& pathname)
{
    PathInfo p = GetPathInfo(pathname);
    return p.filename;
}

std::string Files::GetFilenameStem(const std::string& pathname)
{
    PathInfo p = GetPathInfo(pathname);
    return p.filename_stem;
}

std::string Files::GetFilenameExtension(const std::string& pathname)
{
    PathInfo p = GetPathInfo(pathname);
    return p.filename_extension;
}

std::string Files::GetDirname(const std::string& pathname) 
{
    PathInfo p = GetPathInfo(pathname);
    return p.dirname;
}

bool Strings::Equals(const std::string& a, const std::string& b)
{
    return a.compare(b) == 0;
}