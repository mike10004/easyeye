/* 
 * File:   easyeye_compare_main.cc
 * Author: mchaberski
 *
 * Created on July 15, 2013, 4:15 PM
 */

#include <easyeye/encode/easyeye_encode.h>
#include <easyeye/match/easyeye_match.h>
#include <easyeye/match/HDCalculator.h>
#include <easyeye/common/easyeye_utils.h>
#include <easyeye/common/easyeye_types.h>
#include <fstream>
#include <iostream>
#include <assert.h>
#include <vector>
#include <string>
#include "easyeye_compare_main.h"
#include <getopt.h>

using namespace std;
using namespace easyeye::compare;
using namespace easyeye::program;

CompareOptions::CompareOptions()
    : delim("\t"), info_type(ALL_INFO)
{
}

Compare::Compare() 
    : Program("easyeye-compare", "0.1.0"), 
        matcher(), numErrors(0), numSuccesses(0), probe_encoding_pathname_(), compare_options_()
{
    AddOption("score-only", 's');
    AddOption("delim", REQUIRED);
}

Compare::~Compare() {}

void Compare::PrintHelpFooter(std::ostream& out) {
    out <<
            "The score indicates the strength of the match, where the lower the value, the" << endl << 
            "greater the likelihood that the two encodings are encodings of the same iris." << endl << 
            "The encodings must be in the format produced by easyeye-extract. Output is " << endl << 
            "tab-separated columns: status code, score, probe pathname, gallery " << endl << 
            "pathname. A nonzero status indicates an error in comparing encodings." << endl; 
}

void Compare::PrintHelpHeader(std::ostream& out)
{
}

void Compare::OptionParsed(const std::string& long_form, bool has_arg, const std::string& optarg)
{
    if (Eq(long_form, "score-only")) {
        compare_options_.info_type = CompareOptions::SCORE_ONLY;
    } else if (Eq(long_form, "delim")) {
        if (Eq(optarg, "TAB")) {
            compare_options_.delim.assign("\t");
        } else {
            compare_options_.delim.assign(optarg);
        }
    }
}

bool Compare::IsPositionalsOk(const std::vector<std::string>& positionals)
{
    bool ok = positionals.size() >= 2;
    if (!ok) {
        cerr << name << ": must specify at least 2 arguments" << endl;
    }
    return ok;
}

void Compare::PrintUsage(ostream& out)
{
out << "Usage: " << name << " [options] PROBE TARGET [TARGET...]" << endl;
out << 
        "Compare a probe iris encoding to one or more target iris encodings and print " << endl << 
        "the comparison score for each probe-gallery pair. " << endl << endl <<
"Options are as follows." << endl <<
"  -h, --help             print help message and exit" << endl <<
"  -v, --verbose          print verbose messages on standard error" << endl <<
"  --version              print version and exit" << endl  << 
"  -d, --diagnostics=DIR  write diagnostic images and data to DIR" << endl;
    
}

Code Compare::ComputeScores(const string& probe_encoding_pathname, const vector<string> &target_encoding_pathnames)
{
    probe_encoding_pathname_.assign(probe_encoding_pathname);
    Encoding probe_encoding;
    LoadEncoding(probe_encoding_pathname_, probe_encoding);
    if (probe_encoding.status != Result::SUCCESS) {
        cerr << name << ": failed to load probe encoding from " << probe_encoding_pathname_ << endl;
        numErrors++;
    } else {
        ComputeScores(probe_encoding, target_encoding_pathnames);
    }
    if (numSuccesses > 0) {
        return kExitSuccess;
    } else {
        return kErrorOther;
    }
}

void Compare::LoadEncoding(const string &pathname, Encoding& encoding) const
{
    if (IOUtils::IsFile(pathname)) {
        string json = Files::Read(pathname);
        bool ok = serial::Deserialize(json, encoding);
        if (!ok) {
            encoding.status = Result::FAILURE;
            cerr << name << ": failed to deserialize encoding in " << pathname << endl;
        }
    } else {
        cerr << name << ": file not found: " << pathname << endl;
    }
}

void Compare::ComputeScores(const Encoding& probeEncoding, const vector<string> &targetEncodingPathnames)
{
    for (int i = 0; i < targetEncodingPathnames.size(); i++) {
        ComputeScore(probeEncoding, targetEncodingPathnames[i]);
    }
}

void Compare::PrintDelimited(std::ostream& out, const MatchInfo& m, CompareOptions::InfoType info_type, const string& delim)
{
    out << m.hd;
    if (info_type == CompareOptions::ALL_INFO) {
        out << delim << m.num_different_bits
                << delim << m.num_unmasked_bits
                << delim << m.template_width
                << delim << m.template_height
                << delim << m.common_bits_differential
                << delim << m.x_shift
                << delim << m.y_shift;
    }
}

void Compare::PrintMatchInfo(const string& target_encoding_pathname, const int io_code) const
{
    cout << io_code << compare_options_.delim 
         << match_info_.flag << compare_options_.delim;
    PrintDelimited(cout, match_info_, compare_options_.info_type, compare_options_.delim);
    cout << compare_options_.delim << probe_encoding_pathname_
         << compare_options_.delim << target_encoding_pathname;
    cout << endl;
}



void Compare::ComputeScore(const Encoding& probeEncoding, const string &target_encoding_pathname)
{
    Encoding targetEncoding;
    LoadEncoding(target_encoding_pathname, targetEncoding);
    int io_code;
    if (targetEncoding.status == Result::SUCCESS) {
        matcher.ComputeScore(probeEncoding, targetEncoding, match_info_);
        io_code = kExitSuccess;
        numSuccesses++;
    } else {
        io_code = kErrorIO;
        numErrors++;
    }
    PrintMatchInfo(target_encoding_pathname, io_code);
}

Code Compare::Execute(const vector<string>& positionals)
{
    vector<string> target_encoding_pathnames;
    const int first_target_index = 1;
    for (size_t i = first_target_index; i < positionals.size(); i++) {
        target_encoding_pathnames.push_back(positionals[i]);
    }
    return ComputeScores(positionals[0], target_encoding_pathnames);
}

#ifndef _WIN32
#ifndef ECLIPSE_TESTING
int main(int argc, char* argv[]) {
    Compare program;
    return program.Main(argc, argv);
}
#endif
#endif
