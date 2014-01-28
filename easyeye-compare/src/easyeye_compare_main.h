#ifndef EZICOMPARE_MAIN_H
#define EZICOMPARE_MAIN_H

#include <iostream>
#include <vector>
#include <string>
#include <easyeye/encode/easyeye_encode.h>
#include <easyeye/match/easyeye_match.h>
#include <easyeye/common/easyeye_utils.h>

namespace easyeye
{

class Compare
{
public:
    Compare();
    static const int kErrorUsage = ProgramCode::kErrorUsage;
    static const int kErrorIO = ProgramCode::kErrorIO;
    static const int kErrorOther = ProgramCode::kErrorOther;
    static const char* kProgName;
    static const int kStatusTargetLoadFailed = 1;
    static const int kStatusScoreComputeSuccess = 0;
    static const int kReturnAtLeastOneSuccess = 0;
    static const int kReturnSomeErrors = 5;

    int ComputeScores(const std::string& probeEncodingPathname, const std::vector<std::string> &targetEncodingPathnames);
    
    class CompareOptions 
    {
    public:
        bool help;
        bool version;
        CompareOptions() : help(false), version(false) {}
    };

    static void PrintUsage(std::ostream& out);
    static int Main(const int argc, char* argv[]);
    static int Main(const std::vector<std::string>& args);
    static int ParseArgs(const std::vector<std::string>& args, CompareOptions& options, std::vector<std::string>& positionals);
    static int Main(CompareOptions& options, const std::vector<std::string>& positionals);
    static void PrintHelp(std::ostream& out);
    static void PrintVersion(std::ostream& out);
    
private:
    int numSuccesses;
    int numErrors;
    Matcher matcher;
    std::string mProbeEncodingPathname;
    void LoadEncoding(const std::string &pathname, Encoding& encoding);
    void ComputeScores(const Encoding& probeEncoding, const std::vector<std::string> &targetEncodingPathnames);
    void ComputeScore(const Encoding& probeEncoding, const std::string &targetEncodingPathname);
    void PrintScore(const double score, const std::string &targetEncodingPathname, const int statusCode);
};



} // end namespace Ezi

#endif
