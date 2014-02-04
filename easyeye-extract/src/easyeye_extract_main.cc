/* 
 * File:   main.cpp
 * Author: mchaberski
 *
 * Created on July 15, 2013, 2:25 PM
 */

#include <cstdlib>
#include <easyeye/common/easyeye_types.h>
#include <easyeye/encode/easyeye_encode.h>
#include <easyeye/common/easyeye_diagnostics.h>
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

ExtractOptions::ExtractOptions() 
    : program::Options(), 
        diagnostics_dir(),
        csv_file("/dev/null")
{
}    

Extract::Extract() : program::Program("easyeye-extract", "0.1.0"), extract_options_()
{
    AddOption("diagnostics", 'd', program::REQUIRED);
    AddOption("csv", program::REQUIRED);
}

Extract::~Extract() {}

bool Extract::IsPositionalsOk(const std::vector<std::string>& positionals)
{
    size_t num_positionals = positionals.size();
    bool ok = num_positionals == 1 || num_positionals == 2;
    if (!ok) {
        cerr << name << ": usage error: must provide 1 or 2 arguments" << endl;
    }
    return ok;
}

void Extract::PrintHelpHeader(std::ostream& out)
{
    //out << "Segment iris images and encode features." << endl;
}

void Extract::PrintHelpFooter(std::ostream& out)
{
    out << endl;
    out << "Feature encoding is printed as json text." << endl;
    out << endl;
    out << "Report easyeye bugs on https://github.com/mike10004/easyeye issues page." << endl;
}

void Extract::PrintUsage(ostream& out)
{
    out << "Usage: " << name << " [options] IMAGEFILE [ENCODING]" << endl;
    out << 
"Segment eye image and optionally write encoded iris features to file. The eye" << endl <<
"image is IMAGEFILE and the encoded features are written to ENCODING." << endl << endl <<
"Mandatory arguments to long options are mandatory for short options too." << endl <<
"  -h, --help             print help message and exit" << endl <<
"  -v, --verbose          print verbose messages on standard error" << endl <<
"  --version              print version and exit" << endl  << 
"  -d, --diagnostics=DIR  write diagnostic images and data to DIR" << endl << 
"  --csv=FILE             print segmentation summary to FILE" << endl;
}

void Extract::PrintCsvResult(const string& image_pathname, const Segmentation& seg)
{
    ostream* csv_out = &cout;
    ofstream csv_file_out;
    bool csv_stdout = extract_options_.csv_file.compare("-") == 0;
    if (!csv_stdout) {
        ios_base::openmode mode = ios_base::out | ios_base::app;
        csv_file_out.open(extract_options_.csv_file.c_str(), mode);
        csv_out = &csv_file_out;
    }

    string delim = ", ";
    *csv_out << image_pathname << delim 
            << Result::DescribeStatus(seg.status) << delim
            << seg.boundary_pair.irisX << delim
            << seg.boundary_pair.iris.center.y << delim
            << seg.boundary_pair.iris.radius << delim
            << seg.boundary_pair.pupil.center.x << delim
            << seg.boundary_pair.pupil.center.y << delim
            << seg.boundary_pair.pupil.radius 
            << endl;
}

Code Extract::Execute(const vector<string>& pathnames) {
    assert(pathnames.size() == 1 || pathnames.size() == 2);
    bool writeEncoding = pathnames.size() == 2;
    string eye_image_pathname = pathnames[0];
    Logs::GetLogger().set_level(options_.verbose ? mylog::TRACE : mylog::INFO);
    Diagnostician diags;
    cv::Mat eyeImage = cv::imread(eye_image_pathname, CV_LOAD_IMAGE_GRAYSCALE);
    if (eyeImage.data == NULL) {
        cerr << name << ": failed to load image from file " << eye_image_pathname << endl;
        return program::kErrorIO;
    }
    Segmentation segmentation;
    Segmenter segmenter;
    if (!extract_options_.diagnostics_dir.empty()) {
        diags.set_eye_image_pathname(eye_image_pathname);
        diags.set_output_dir(extract_options_.diagnostics_dir);
        diags.set_disabled(false);
        segmenter.set_diagnostician(&diags);
    }
    segmenter.SegmentEyeImage(eyeImage, segmentation);
    
    Code rv = program::kExitSuccess;
    PrintCsvResult(eye_image_pathname, segmentation);
    
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
                    bool output_good = fout.good();
                    rv = output_good ? program::kExitSuccess : program::kErrorIO;
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

void Extract::OptionParsed(const std::string& long_form, bool has_arg, const std::string& option_arg)
{
    if (Eq(long_form, "diagnostics")) {
        extract_options_.diagnostics_dir.assign(option_arg);
    } else if (Eq(long_form, "csv")) {
        extract_options_.csv_file.assign(option_arg);
    }
}

#ifndef _WIN32
#ifndef ECLIPSE_TESTING

int main(int argc, char* argv[]) {
    Extract program;
    return program.Main(argc, argv);
}

#endif
#endif