#ifndef EASYEYE_COMPARE_MAIN_H
#define EASYEYE_COMPARE_MAIN_H

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

class CompareOptions
{
public:
    enum InfoType { ALL_INFO, SCORE_ONLY };
    CompareOptions();
    std::string delim;
    InfoType info_type;
};
        
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
    void OptionParsed(const std::string& long_form, bool has_arg, const std::string& optarg);
    
private:
    int numSuccesses;
    int numErrors;
    Matcher matcher;
    CompareOptions compare_options_;
    std::string probe_encoding_pathname_;
    MatchInfo match_info_;
    void LoadEncoding(const std::string &pathname, Encoding& encoding) const;
    void ComputeScores(const Encoding& probeEncoding, const std::vector<std::string> &targetEncodingPathnames);
    void ComputeScore(const Encoding& probeEncoding, const std::string &targetEncodingPathname);
    void PrintMatchInfo(const std::string& targetEncodingPathname, const int io_code) const;
    static void PrintDelimited(std::ostream& out, const MatchInfo& match_info, CompareOptions::InfoType info_type, const std::string& delim);
};


    }// end namespace compare
} // end namespace easyeye

#endif /* EASYEYE_COMPARE_MAIN_H */

