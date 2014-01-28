/* 
 * File:   main.cpp
 * Author: mchaberski
 *
 * Created on July 15, 2013, 2:25 PM
 */

#include <cstdlib>
#include <easyeye/common/easyeye_types.h>
#include <easyeye/encode/easyeye_encode.h>
#include <easyeye/segment/easyeye_segment.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "easyeye_extract_main.h"
#include <easyeye/common/mylog.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <opencv2/highgui/highgui_c.h>
#include <getopt.h>

using namespace easyeye;
using namespace std;

const char* Extract::kProgName = "easyeye-extract";

void Extract::PrintHelp(std::ostream& out)
{
    
}

void Extract::PrintUsage(ostream& out)
{
        out << Extract::kProgName << ": usage error" << endl;
        out << "Usage:" << endl;
        out << "    " << Extract::kProgName << " [options] IMAGEFILE [ENCODING]" << endl;
        out << "Extracts iris features from an eye image and optionally writes the encoding"
                << " in json format to file." << endl;
}

int Extract::Main(const int argc, char* argv[]) {
	vector<string> args;
	for (int i = 0; i < argc; i++) {
		const char* arg = argv[i];
		string s(arg);
		args.push_back(s);
	}
    if (args.size() < 2 || args.size() > 4) {
        PrintUsage(cerr);
        return Extract::kErrorUsage;
    }
	int rv = Main(args);
	return rv;
}

void PrintResult(ostream& out, const char* image_pathname, Segmentation& seg)
{
    string delim = ", ";
    out << image_pathname << delim 
            << Result::DescribeStatus(seg.status) << delim
            << seg.boundary_pair.irisX << delim
            << seg.boundary_pair.irisY << delim
            << seg.boundary_pair.irisR << delim
            << seg.boundary_pair.pupilX << delim
            << seg.boundary_pair.pupilY << delim
            << seg.boundary_pair.pupilR 
            << endl;
}

int Extract::Main(ExtractOptions& options, const vector<string>& pathnames) {
    bool writeEncoding = pathnames.size() == 2;
    const char* eyeImagePathname = pathnames[0].c_str();
    mylog::SetLogLevel(options.verbose ? mylog::TRACE : mylog::INFO);
    cv::Mat eyeImage = cv::imread(eyeImagePathname, CV_LOAD_IMAGE_GRAYSCALE);
    if (eyeImage.data == NULL) {
        mylog::Log(mylog::ERROR, "%s: failed to load image from file %s\n", Extract::kProgName, eyeImagePathname);
        return Extract::kErrorIO;
    }
    Segmentation segmentation;
    Segmenter segmenter;
    segmenter.SegmentEyeImage(eyeImage, segmentation);
    
    int rv = 0;
    PrintResult(cout, eyeImagePathname, segmentation);
    if (segmentation.status == Result::SUCCESS && writeEncoding) {
        ofstream fout(pathnames[1].c_str());
        if (fout.is_open()) {
            Encoder encoder;
            Normalization normalization(encoder.config());
            encoder.NormalizeIris(eyeImage, segmentation, normalization);
            if (normalization.status == Result::SUCCESS) {
                Encoding encoding(encoder.config());
                encoder.EncodeIris(normalization, encoding);
                if (encoding.status == Result::SUCCESS) {
                    string json = serial::Serialize(encoding);
                    fout << json;
                } else {
                    cerr << Extract::kProgName << ": encoding failed with error " << Result::DescribeStatus(encoding.status) << endl;
                    rv = Extract::kErrorOther;
                }
            } else {
                cerr << Extract::kProgName << ": normalization failed with error " << Result::DescribeStatus(normalization.status) << endl;
                rv = Extract::kErrorOther;
            }
        } else {
            cerr << Extract::kProgName << ": failed to open encoding output file for writing" << endl;
            rv = Extract::kErrorIO;
        }
    } else if (segmentation.status != Result::SUCCESS) {
        cerr << Extract::kProgName << ": segmentation failed with error " << Result::DescribeStatus(segmentation.status) << endl;
        rv = Extract::kErrorOther;
    }
    
    return rv;
}

int Extract::ParseArgs(const vector<string>& args, ExtractOptions& options, vector<string>& positionals) 
{
    int c;
    const int argc = args.size();
    char** argv = new char*[argc];
    for (size_t i = 0; i < args.size(); i++) {
        argv[i] = (char*) args[i].c_str();
    }
    int version_flag = 0, help_flag = 0, verbose_flag = 0;
    bool option_error = false;
    while (true) {
        static struct option long_options[] = {
               {"version", no_argument,       &version_flag, 1},
               {"help",   no_argument,       &help_flag, 0},
               {"verbose", no_argument, &verbose_flag, 'v'},
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
    options.verbose = verbose_flag != 0;
    positionals.clear();
    while (optind < argc) {
        positionals.push_back(argv[optind++]);
    }
    
    delete argv;
    
    if (positionals.size() < 1 || positionals.size() > 2) {
        cerr << kProgName << ": usage error; exactly 1 or 2 arguments required" << endl;
        option_error = true;
    }
    
    if (option_error) {
        return kErrorUsage;
    }
    return 0;
}


int Extract::Main(const vector<string>& args) 
{
    ExtractOptions options;
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
    return Extract::Main(argc, argv);
}

#endif