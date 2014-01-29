#ifndef EZICOMPARE_MAIN_H
#define EZICOMPARE_MAIN_H

#include <iostream>
#include <vector>
#include <string>
#include <easyeye/encode/easyeye_encode.h>
#include <easyeye/match/easyeye_match.h>
#include <easyeye/common/easyeye_utils.h>
#include <easyeye/common/easyeye_program.h>
namespace easyeye
{
    namespace compare
    {

        class Compare : public easyeye::program::Program
{
public:
    Compare();
    ~Compare();
    easyeye::program::Code ComputeScores(const std::string& probeEncodingPathname, const std::vector<std::string> &targetEncodingPathnames);
    void PrintUsage(std::ostream& out);
    void PrintHelpHeader(std::ostream& out);
    void PrintHelpFooter(std::ostream& out);
    bool IsPositionalsOk(const std::vector<std::string>& positionals);
    easyeye::program::Code Execute(const std::vector<std::string>& positionals);
    
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


    }// end namespace compare
} // end namespace easyeye

#endif

