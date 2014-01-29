/*
 * eziutils.cc
 * 
 * (c) 2013 IBG, A Novetta Solutions Company
 */

#include <algorithm>
#include <fstream>
#include <string>
#include <errno.h>
#include "mylog.h"
#include "easyeye_utils.h"
#include <sys/stat.h>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <vector>
#include <sstream>

#ifdef EZI_ISWINDOWS
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
#ifdef EZI_ISWINDOWS
    return temp_directory_path_windows(errorCode);
#else
    return temp_directory_path_unix(errorCode);
#endif
    
}
bool IOUtils::is_directory(const string& path){
    int ec = 0;
    bool yes = is_directory(path, &ec);
    return yes;
}

bool IOUtils::is_file(const string& path)
{
    int ec = 0;
    bool yes = is_file(path, &ec);
    return yes;
}


bool IOUtils::is_directory(const string& path, int *errorCode) 
{
    return pathname_stat_has_mode(path, S_IFDIR, errorCode);
}

bool IOUtils::is_file(const string& path, int* errorCode)
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
      if (p.empty() || (!is_directory(p, errorCode)))
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
#ifdef EZI_ISWINDOWS
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
  
string Vectors::ToString(const vector<unsigned char>& bytes, size_t preview_len)
{
    std::ostringstream ss;
    ss.width(2);
    for (size_t i = 0; i < preview_len && i < bytes.size() ;i++) {
        ss << (int) (bytes[i]) << ' ';
    }
    return ss.str();
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

string IOUtils::ToString(std::istream& in)
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