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

Compare::Compare() : Program("easyeye-compare", "0.1.0"), numErrors(0), numSuccesses(0)
{
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

Code Compare::ComputeScores(const string& probeEncodingPathname, const vector<string> &targetEncodingPathnames)
{
    mProbeEncodingPathname.assign(probeEncodingPathname);
    string pathname = string(mProbeEncodingPathname);
    Encoding probeEncoding;
    LoadEncoding(pathname, probeEncoding);
    if (probeEncoding.status != Result::SUCCESS) {
        cerr << name << ": failed to load probe encoding from " << probeEncodingPathname << endl;
        numErrors++;
    } else {
        ComputeScores(probeEncoding, targetEncodingPathnames);
    }
    if (numSuccesses > 0) {
        return kExitSuccess;
    } else {
        return kErrorOther;
    }
}

void Compare::LoadEncoding(const string &pathname, Encoding& encoding)
{
    if (IOUtils::is_file(pathname)) {
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

void Compare::PrintScore(const double score, const string& targetEncodingPathname, const int statusCode)
{
//    static char scoreStr[1024];
//    sprintf(scoreStr, "%1.2f", score);
//    cout << statusCode << '\t' << scoreStr << '\t' << mProbeEncodingPathname << '\t' << targetEncodingPathname << endl;
    cout << statusCode << '\t' << score << '\t' << mProbeEncodingPathname << '\t' << targetEncodingPathname << endl;
}



void Compare::ComputeScore(const Encoding& probeEncoding, const string &targetEncodingPathname)
{
    Encoding targetEncoding;
    LoadEncoding(targetEncodingPathname, targetEncoding);
    double score = HDCalculator::HD_NAN;
    int statusCode;
    if (targetEncoding.status == Result::SUCCESS) {
        score = matcher.ComputeScore(probeEncoding, targetEncoding);
        statusCode = 0;
        numSuccesses++;
    } else {
        statusCode = kErrorIO;
        numErrors++;
    }
    PrintScore(score, targetEncodingPathname, statusCode);
}

Code Compare::Execute(const vector<string>& positionals)
{
    Code rv = kExitSuccess;
    string probeEncodingPathname = positionals[0];
    vector<string> targetEncodingPathnames;
    const int indexOfFirstTargetEncoding = 1;
    for (size_t i = indexOfFirstTargetEncoding; i < positionals.size(); i++) {
        targetEncodingPathnames.push_back(positionals[i]);
    }
    Compare compare;
    rv = compare.ComputeScores(probeEncodingPathname, targetEncodingPathnames);
    return rv;
}

#ifndef _WIN32
#ifndef ECLIPSE_TESTING
int main(int argc, char* argv[]) {
    Compare program;
    return program.Main(argc, argv);
}
#endif
#endif
