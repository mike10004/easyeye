#include <cerrno>
#include <vector>
#include <string>
#include <sstream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <fstream>
#include <Masek.h>
#include "easyeye_diagnostics.h"
#include "easyeye_types.h"
#include "easyeye_utils.h"
#include "easyeye_imaging.h"
#include "mylog.h"

#ifndef _WIN32
#include <unistd.h>
#include <opencv2/imgproc/imgproc.hpp>
#endif

using std::vector;
using cv::Mat;
using std::cerr;
using std::endl;
using cv::Point;
using cv::Scalar;
using namespace easyeye;
using std::string;
using namespace cv;
using mylog::Logs;
using mylog::INFO;
using mylog::TRACE;
using mylog::WARN;
using mylog::ERROR;

namespace 
{
void MaybeMakeDirs(const string& dir) 
{
    if (!IOUtils::IsDirectory(dir)) {
        int error_code;
        bool now_exists = IOUtils::MakeDirs(dir, &error_code);
        if (!now_exists) {
            mylog::Logs::GetLogger().Log(mylog::INFO, "failed to create directory %s with error %d %s\n", dir.c_str(), error_code, strerror(error_code));
        }
    }
}

}



Diagnostician::Diagnostician()
    : output_dir_("."),
    disabled_(false),
    verbose_(true),
    num_images_written_(0),
    eye_image_pathname_(),
    text_output_stream_(cerr),
    image_format_suffix_(".png"),
        collect_pathnames_(false), 
        write_original_(false),
        original_written_(false)
{
#ifndef _WIN32
    char cwd[4096];
    bool got_cwd = (getcwd(cwd, 4096) != NULL);
    if (!got_cwd) {
        int err = errno;
        const char* errstr = strerror(err);
        mylog::Logs::GetLogger().Log(mylog::WARN, "failed to get full path of current working directory; errno = %d %s\n", err, errstr);
        output_dir_.assign(".");
    } else {
        output_dir_.assign(cwd);
    }
#else
    disabled_ = true;
#endif
}

void Diagnostician::set_disabled(bool disabled) {
    disabled_ = disabled;
}

bool Diagnostician::disabled() const
{
    return disabled_;
}

void Diagnostician:: set_output_dir(const string& output_dir)
{
    output_dir_.assign(output_dir);
}

void Diagnostician::set_eye_image_pathname(const std::string& eye_image_pathname)
{
    eye_image_pathname_.assign(eye_image_pathname);
}

string Diagnostician::eye_image_pathname() const
{
    return eye_image_pathname_;
}

void Diagnostician::DumpNormOutput(const cv::Mat& polar_array, const cv::Mat& noise_array)
{
    if (disabled()) return;
    WriteImage(noise_array * (uchar)255, "polarnoise");
    WriteImage(polar_array * 255.0, "polardata");
}

string Diagnostician::ToFilename(const string& label, const string& suffix)
{
    std::ostringstream ss;
    num_images_written_++;
    string stem = Files::GetFilenameStem(eye_image_pathname_);
    ss << output_dir_ << '/' << stem << '-' << num_images_written_ << '-' << label << suffix;
    return ss.str();
}

void Diagnostician::DumpEncodeOutput(const int width, const int height, const int* irisTemplate, const int* irisMask)
{
    if (disabled()) return;
    string outpath = ToFilename("encoding", ".txt");
    std::ofstream out(outpath.c_str());
    if (out.is_open()) {
        out << width << std::endl;
        out << height << std::endl;
        int value;
        for (int i = 0; i < width*height; i++) {
            value = irisTemplate[i];
            out << value;
        }
        out << std::endl;
        for (int i = 0; i < width*height; i++) {
            out << irisMask[i];
        }
        out << std::endl;
        out.close();
        if (verbose_) {
            std::cerr << "wrote " << outpath << std::endl;
        }
        files_written_.push_back(outpath);
    } else {
        std::cerr << "failed to write encoding output because of "
                "failure to open file for writing: " << outpath << std::endl;
    }
}

void Diagnostician::DumpSegOutput(const BoundaryPair& bp, const EyelidsLocation& eyelids, cv::SparseMat& extrema_noise)
{
    if (disabled()) return;
    Scalar iris_color(0x0, 0xff, 0xff), pupil_color(0x0, 0xff, 0xff), noise_color(0xa0, 0x0, 0x0), 
            eyelid_color(0x0, 0xff, 0x0);
    int thickness = 2, line_type = 8;
    Mat eye_image = cv::imread(eye_image_pathname_, CV_LOAD_IMAGE_COLOR);
    eyelids.Draw(eye_image, eyelid_color);
    SparseMatConstIterator_<uchar>
    it = extrema_noise.begin<uchar>(),
    it_end = extrema_noise.end<uchar>();
    for(; it != it_end; ++it)
    {
        // print element indices and the element value
        const SparseMat::Node* n = it.node();
        int y = n->idx[0], x = n->idx[1];
        uchar value = it.value<uchar>();
        Vec3b* p = eye_image.ptr<Vec3b>(y, x);
        for (int j = 0; j < 3; j++) p[j] = noise_color[j];
    }
    Point iris_center(bp.iris.center.x, bp.iris.center.y);
    Point pupil_center(bp.pupil.center.x, bp.pupil.center.y);
    int iris_radius = bp.iris.radius, pupil_radius = bp.pupil.radius;
    cv::circle(eye_image, iris_center, iris_radius, iris_color, thickness, line_type);
    cv::circle(eye_image, pupil_center, pupil_radius, pupil_color, thickness, line_type);
    WriteImage(eye_image, "segmentation");
}

bool Diagnostician::write_original() const
{
    return write_original_;
}

void Diagnostician::set_write_original(bool write_original)
{
    write_original_ = write_original;
}

void Diagnostician::WriteText(const std::string& text, const std::string& label)
{
    if (disabled()) return;
    string output_pathname = ToFilename(label, ".txt");
    MaybeMakeDirs(output_dir_);
    bool written = Files::Write(text, output_pathname);
    if (!written) {
        mylog::Logs::GetLogger().Log(mylog::INFO, "failed to write to text file at %s\n", output_pathname.c_str());
    } else {
        if (collect_pathnames_) {
            files_written_.push_back(output_pathname);
        }        
    }
}

void Diagnostician::WriteImage(const cv::Mat& image, const string& label)
{
    if (disabled()) return;
    if (write_original_ && !original_written_) {
        original_written_ = true;
        Mat original = cv::imread(eye_image_pathname_);
        WriteImage(original, "original");
    }
    string output_pathname = ToFilename(label, image_format_suffix_);
    MaybeMakeDirs(output_dir_);
    bool ok = cv::imwrite(output_pathname, image);
    if (ok) {
        if (verbose_) {
            cerr << "wrote " << output_pathname << endl;    
        }
        if (collect_pathnames_) {
            files_written_.push_back(output_pathname);
        }
    } else if (!ok) {
        cerr << "writing failed on " << output_pathname << endl;
    }
}

//void Diagnostician::WriteFilter(Masek::filter* image, const char* label) 
//{
//    if (disabled()) return;
//    Mat imagecopy;
//    Imaging::CopyToMat(image, imagecopy);
//    WriteImage(imagecopy, label);
//}
//
//void Diagnostician::WriteImage(Masek::IMAGE* image, const char* label) 
//{
//    if (disabled()) return;
//    Mat imagecopy;
//    Imaging::CopyToMat(image, imagecopy);
//    WriteImage(imagecopy, label);
//}

void Diagnostician::set_collect_pathnames(bool value)
{
    collect_pathnames_ = value;
}

bool Diagnostician::collect_pathnames() const
{
    return collect_pathnames_;
}

vector<string>& Diagnostician::files_written()
{
    return files_written_;
}

void DiagnosticsCreator::set_diagnostician(Diagnostician* d)
{
    diagnostician_ = d;
}

Diagnostician* DiagnosticsCreator::diagnostician()
{
    Diagnostician* d = diagnostician_;
    if (d == NULL) {
        d = &inactive_diagnostician_;
    }
    return d;
}

DiagnosticsCreator::DiagnosticsCreator() 
        : diagnostician_(NULL), inactive_diagnostician_()
{
    inactive_diagnostician_.set_disabled(true);
}

DiagnosticsCreator::~DiagnosticsCreator()
{
    
}