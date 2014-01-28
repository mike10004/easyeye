/* 
 * File:   ezicompare_main.cc
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
using namespace easyeye;

const char* Compare::kProgName = "easyeye-compare";

Compare::Compare() : numErrors(0), numSuccesses(0)
{
}

void Compare::PrintUsage(ostream& out)
{
        out << Compare::kProgName << ": usage error" << endl;
        out << "Usage:" << endl;
        out << "    " << Compare::kProgName << " PROBE GALLERY [GALLERY...]" << endl;
        out << "Compares a probe iris encoding to one or more gallery iris " 
                "encodings and prints the comparison score for each " 
                "probe-gallery pair. "
                "The score indicates the strength of the match, where the lower"
                " the value, the greater the likelihood that the two encodings "
                "are encodings of the same iris. The encodings must be in the "
                "format produced by eziextract. Output is tab-separated columns"
                ": status code, score, probe pathname, gallery pathname." 
                << endl;
    
}

int Compare::ComputeScores(const string& probeEncodingPathname, const vector<string> &targetEncodingPathnames)
{
    mProbeEncodingPathname.assign(probeEncodingPathname);
    string pathname = string(mProbeEncodingPathname);
    Encoding probeEncoding;
    LoadEncoding(pathname, probeEncoding);
    if (probeEncoding.status != Result::SUCCESS) {
        cerr << kProgName << ": failed to load probe encoding from " << probeEncodingPathname << endl;
        numErrors++;
    } else {
        ComputeScores(probeEncoding, targetEncodingPathnames);
    }
    if (numSuccesses > 0) {
        return kReturnAtLeastOneSuccess;
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
            cerr << kProgName << ": failed to deserialize encoding in " << pathname << endl;
        }
    } else {
        cerr << kProgName << ": file not found: " << pathname << endl;
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
        statusCode = kStatusScoreComputeSuccess;
        numSuccesses++;
    } else {
        statusCode = kStatusTargetLoadFailed;
        numErrors++;
    }
    PrintScore(score, targetEncodingPathname, statusCode);
}

int Compare::Main(const int argc, char* argv[]) {
    vector<string> args;
    for (int i = 0; i < argc; i++) {
        args.push_back(string(argv[i]));
    }
    return Main(args);
}

int Compare::Main(Compare::CompareOptions& options, const vector<string>& positionals)
{
    int rv = 0;
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

int Compare::ParseArgs(const vector<string>& args, CompareOptions& options, vector<string>& positionals) 
{
    int c;
    const int argc = args.size();
    char** argv = new char*[argc];
    for (size_t i = 0; i < args.size(); i++) {
        argv[i] = (char*) args[i].c_str();
    }
    bool option_error = false;
//    bool stop = false;
    int version_flag = 0, help_flag = 0;
    while (true) {
        static struct option long_options[] = {
               {"version", no_argument,       &version_flag, 1},
               {"help",   no_argument,       &help_flag, 0},
//               {"add",     no_argument,       0, 'a'},
//               {"append",  no_argument,       0, 'b'},
//               {"delete",  required_argument, 0, 'd'},
//               {"create",  required_argument, 0, 'c'},
//               {"file",    required_argument, 0, 'f'},
               {0, 0, 0, 0}
             };
        /* getopt_long stores the option index here. */
        int option_index = 0;

        c = getopt_long (argc, argv, "abc:d:f:",
                         long_options, &option_index);

        /* Detect the end of the options. */
        if (c == -1)
          break;

        switch (c) {
            case 0:
              if (long_options[option_index].flag != 0) break;
            //            printf ("option %s", long_options[option_index].name);
            //            if (optarg)
            //              printf (" with arg %s", optarg);
            //            printf ("\n");
              break;
            case 'a':
              puts ("option -a\n");
              break;
            case '?':
              /* getopt_long already printed an error message. */
                option_error = true;
              break;
            default:
                option_error = true;
                break;
        }
    }
    
    options.version = version_flag != 0;
    options.help = help_flag != 0;
    
    positionals.clear();
    while (optind < argc) {
        positionals.push_back(argv[optind++]);
    }
    
    delete argv;

    if (positionals.size() < 2) {
        cerr << kProgName << ": at least 2 arguments required" << endl;
    }
    
    if (option_error) {
        return kErrorUsage;
    }
    return 0;
}

int Compare::Main(const vector<string>& args) 
{
    CompareOptions options;
    vector<string> positionals;
    int rv = ParseArgs(args, options, positionals);
    if (rv != 0) {
        PrintUsage(cerr);
        return rv;
    }
    return Main(options, positionals);
}

#ifndef _WIN32

int main(int argc, char* argv[]) {
    return Compare::Main(argc, argv);
}

#endif
