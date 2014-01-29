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
using namespace easyeye::extract;
using namespace std;
using mylog::Logs;
using easyeye::program::Program;
using easyeye::program::Options;
using easyeye::program::Code;

Extract::Extract() : program::Program("easyeye-extract", "0.1.0"), extract_options_()
{
}

Extract::~Extract() {}

bool Extract::IsPositionalsOk(const std::vector<std::string>& positionals)
{
    return positionals.size() == 1 || positionals.size() == 2;
}

void Extract::PrintHelpHeader(std::ostream& out)
{
    out << "Segment iris images and encode features." << endl;
}

void Extract::PrintHelpFooter(std::ostream& out)
{
    out << "Feature encoding is printed as json text." << endl;
}

void Extract::PrintUsage(ostream& out)
{
    out << "Usage:" << endl;
    out << "    " << name << " [options] IMAGEFILE [ENCODING]" << endl;
    out << "Extracts iris features from an eye image and optionally writes the encoding"
            << " in json format to file." << endl;
}

void PrintCsvResult(ostream& out, const string& image_pathname, const Segmentation& seg)
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

Code Extract::Execute(const vector<string>& pathnames) {
    ExtractOptions& options = static_cast<ExtractOptions&>(options_);
    bool writeEncoding = pathnames.size() == 2;
    const string& eyeImagePathname = pathnames[0];
    Logs::GetLogger().set_level(options.verbose ? mylog::TRACE : mylog::INFO);
    cv::Mat eyeImage = cv::imread(eyeImagePathname, CV_LOAD_IMAGE_GRAYSCALE);
    if (eyeImage.data == NULL) {
        cerr << name << ": failed to load image from file " << eyeImagePathname << endl;
        return program::kErrorIO;
    }
    Segmentation segmentation;
    Segmenter segmenter;
    segmenter.SegmentEyeImage(eyeImage, segmentation);
    
    Code rv = program::kExitSuccess;
    PrintCsvResult(cout, eyeImagePathname, segmentation);
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
                    rv = fout.good() ? program::kExitSuccess : program::kErrorIO;
                } else {
                    cerr << name << ": encoding failed with error " << Result::DescribeStatus(encoding.status) << endl;
                    rv = program::kErrorOther;
                }
            } else {
                cerr << name << ": normalization failed with error " << Result::DescribeStatus(normalization.status) << endl;
                rv = program::kErrorOther;
            }
        } else {
            cerr << name << ": failed to open encoding output file for writing" << endl;
            rv = program::kErrorIO;
        }
    } else if (segmentation.status != Result::SUCCESS) {
        cerr << name << ": segmentation failed with error " << Result::DescribeStatus(segmentation.status) << endl;
        rv = program::kErrorOther;
    }
    
    return rv;
}

void Extract::OptionParsed(const std::string& long_form, bool has_arg, const std::string optarg)
{
    
}

#ifndef _WIN32
#ifndef ECLIPSE_TESTING

int main(int argc, char* argv[]) {
    Extract program;
    return program.Main(argc, argv);
}

#endif
#endif