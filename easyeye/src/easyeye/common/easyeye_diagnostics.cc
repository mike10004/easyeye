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

Diagnostician::Diagnostician(const string& eyeImagePathname)
    : output_dir_("."),
    disabled_(false),
    verbose_(true),
    num_images_written_(0),
        eye_image_pathname_(eyeImagePathname),
    eye_image_basename_(),
    text_output_stream_(cerr),
    image_format_suffix_(".png"),
        collect_pathnames_(false), 
        write_original_(false),
        original_written_(false)
{
#ifndef _WIN32
    char cwd[4096];
    disabled_ = (getcwd(cwd, 4096) == NULL);
    output_dir_.assign(cwd);
    if (disabled_) {
        cerr << "warning: diagnostics disabled because current directory pathname is unavailable" << endl;
    }
#else
    disabled_ = true;
#endif
    char* pathname_copy = strdup(eyeImagePathname.c_str());
    char* base = basename(pathname_copy);
    eye_image_basename_.assign(base);
    free(pathname_copy);
    if (eye_image_basename_.length() == 0) {
        eye_image_basename_.assign("eye_image");
    }
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


#ifdef WIN32

void Diagnostician::DumpNormOutput(Masek::filter* polarArray, Masek::IMAGE* noiseArray)
{
}

void Diagnostician::DumpEncodeOutput(const int width, const int height, const int* irisTemplate, const int* irisMask)
{
}

void Diagnostician::DumpSegOutput(Masek::IMAGE* noiseImage, int iris[], int pupil[])
{
}

IplImage* Diagnostician::CopyToIplImage(Masek::IMAGE* image)
{
    return NULL;
}

void Diagnostician::WriteImage(Masek::IMAGE* image, const char* prefix) 
{
}


#else

//void dump_histo(Masek::IMAGE* noiseArray) 
//{
//    int noiseArrayLen = noiseArray->hsize[0] * noiseArray->hsize[1];
//    int noiseHisto[256];
//    memset(noiseHisto, 0, sizeof(int) * 256);
//    for (int i = 0; i < noiseArrayLen; i++) {
//        unsigned char val = noiseArray->data[i];
//        noiseHisto[val] = noiseHisto[val] + 1;
//    }
//    for (int i = 0; i < 256; i++) {
//        if (noiseHisto[i] > 0) std::cerr << "noiseHisto[" << i << "] = " << noiseHisto[i] << std::endl;
//    }
//}

void Diagnostician::DumpNormOutput(Masek::filter* polarArray, Masek::IMAGE* noiseArray)
{
    if (disabled()) return;
    Masek::IMAGE scaledNoise;
    int noiseArrayLen = noiseArray->hsize[0] * noiseArray->hsize[1];
    scaledNoise.hsize[0] = noiseArray->hsize[0];
    scaledNoise.hsize[1] = noiseArray->hsize[1];
    scaledNoise.data = (unsigned char*) malloc(sizeof(unsigned char) * noiseArrayLen);
    for (int i = 0; i < noiseArrayLen; i++) {
        scaledNoise.data[i] = 255 * noiseArray->data[i];
    }
    WriteImage(&scaledNoise, "normNoiseArray");
    free(scaledNoise.data);
    WriteFilter(polarArray, "normPolarArray");
}

string Diagnostician::ToFilename(const string& label, const string& suffix)
{
    std::ostringstream ss;
    num_images_written_++;
    ss << output_dir_ << '/' << eye_image_basename_ << '-' << num_images_written_ << '-' << label << suffix;
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

static void DrawEyelidEllipse(cv::Mat& eye_image, const EyelidsLocation& eyelids_location, const Scalar color)
{
      const cv::Point2i center(eyelids_location.center_x(), eyelids_location.center_y());
      int width = eyelids_location.ellipse_vals[2], 
              topHeight = eyelids_location.ellipse_vals[3], 
              bottomHeight = eyelids_location.ellipse_vals[4];
      double angle = eyelids_location.angle;
      int rows = eye_image.rows, cols = eye_image.cols;
      int thickness = 3;
      Size top_size(width, topHeight);
      Size bottom_size(width, bottomHeight);
      cv::ellipse(eye_image, center, top_size, angle, 0, 180, color, thickness, CV_AA, 0);
      cv::ellipse(eye_image, center, bottom_size, angle, 180, 360, color, thickness, CV_AA, 0);
}


void Diagnostician::DumpSegOutput(const BoundaryPair& bp, const EyelidsLocation& eyelids, cv::SparseMat& extrema_noise)
{
    if (disabled()) return;
    Scalar iris_color(0x0, 0xff, 0xff), pupil_color(0x0, 0xff, 0xff), noise_color(0xa0, 0x0, 0x0), 
            eyelid_color(0x0, 0xff, 0x0);
    int thickness = 2, line_type = 8;
    Mat eye_image = cv::imread(eye_image_pathname_, CV_LOAD_IMAGE_COLOR);
    DrawEyelidEllipse(eye_image, eyelids, eyelid_color);
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
    Point iris_center(bp.irisX, bp.irisY);
    Point pupil_center(bp.pupilX, bp.pupilY);
    int iris_radius = bp.irisR, pupil_radius = bp.pupilR;
    cv::circle(eye_image, iris_center, iris_radius, iris_color, thickness, line_type);
    cv::circle(eye_image, pupil_center, pupil_radius, pupil_color, thickness, line_type);
    WriteImage(eye_image, "segmentation");
}

void Diagnostician::CopyToMat(Masek::filter* src, cv::Mat& dst)
{
    int rows = src->hsize[1];
    int cols = src->hsize[0];
    dst.create(rows, cols, CV_64FC1);
    for (int y = 0; y < rows; y++) {
        for (int x = 0; x < cols; x++) {
            double v = src->data[y * cols + x];
            dst.at<double>(x, y) = v;
        }
    }
}

void Diagnostician::CopyToMat(Masek::IMAGE* src, cv::Mat& dst)
{
    int rows = src->hsize[1], cols = src->hsize[0];
    dst.create(rows, cols, CV_8U);
    for (int y = 0; y < rows; y++) {
        for (int x = 0; x < cols; x++) {
            dst.at<uchar>(x, y) = src->data[y * cols + x];
        }
    }
}

static void MaybeMakeDirs(const string& dir) 
{
    if (!IOUtils::IsDirectory(dir)) {
        int error_code;
        bool now_exists = IOUtils::MakeDirs(dir, &error_code);
        if (!now_exists) {
            mylog::Logs::GetLogger().Log(mylog::INFO, "failed to create directory %s with error %d %s\n", dir.c_str(), error_code, strerror(error_code));
        }
    }
}

bool Diagnostician::write_original() const
{
    return write_original_;
}

void Diagnostician::set_write_original(bool write_original)
{
    write_original_ = write_original;
}

void Diagnostician::WriteImage(cv::Mat& image, const string& label)
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
        files_written_.push_back(output_pathname);
    } else if (!ok) {
        cerr << "writing failed on " << output_pathname << endl;
    }
}

void Diagnostician::WriteFilter(Masek::filter* image, const char* label) 
{
    if (disabled()) return;
    Mat imagecopy;
    CopyToMat(image, imagecopy);
    WriteImage(imagecopy, label);
}

void Diagnostician::WriteImage(Masek::IMAGE* image, const char* label) 
{
    if (disabled()) return;
    Mat imagecopy;
    CopyToMat(image, imagecopy);
    WriteImage(imagecopy, label);
}


#endif

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
        : diagnostician_(NULL), inactive_diagnostician_("")
{
    inactive_diagnostician_.set_disabled(true);
}

DiagnosticsCreator::~DiagnosticsCreator()
{
    
}