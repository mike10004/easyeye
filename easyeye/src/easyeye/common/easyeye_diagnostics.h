/* 
 * File:   Diagnostics.h
 * Author: mchaberski
 *
 * Created on July 17, 2013, 11:26 AM
 */

#ifndef DIAGNOSTICS_H
#define	DIAGNOSTICS_H

#include <string>
#include <vector>
#include <iostream>
#include <Masek.h>
#include <opencv2/core/core.hpp>
#include "easyeye_types.h"

namespace easyeye
{

class Diagnostician 
{
public:
    Diagnostician(const std::string& eyeImagePathname);
    void DumpSegOutput(const BoundaryPair& bp, const EyelidsLocation& eyelids, cv::SparseMat& extrema_noise);
    void DumpNormOutput(Masek::filter* polarArray, Masek::IMAGE* noiseArray);
    void DumpEncodeOutput(const int width, const int height, const int* irisTemplate, const int* irisMask);
    bool disabled() const;
    void set_disabled(bool disabled);
    void set_output_dir(const std::string& output_dir);
    bool collect_pathnames() const;
    void set_collect_pathnames(bool collect_pathnames);
    std::vector<std::string>& files_written();
    void WriteImage(cv::Mat& image, const std::string& label);
private:
    std::string output_dir_;
    bool disabled_;
    bool verbose_;
    bool collect_pathnames_;
    int num_images_written_;
    const std::string eye_image_pathname_;
    std::string eye_image_basename_;
    std::ostream& text_output_stream_;
    std::string image_format_suffix_;
    std::vector<std::string> files_written_;
    std::string ToFilename(const std::string& label, const std::string& suffix);
    void WriteImage(Masek::IMAGE* noiseImage, const char* label);
    void WriteFilter(Masek::filter* noiseImage, const char* label);
    void CopyToMat(Masek::IMAGE* src, cv::Mat& dst);
    void CopyToMat(Masek::filter* src, cv::Mat& dst);
};

class DiagnosticsCreator
{
public:
    DiagnosticsCreator();
    virtual ~DiagnosticsCreator();
    Diagnostician* diagnostician();
    void set_diagnostician(Diagnostician* d);
protected:
    Diagnostician* diagnostician_;
    Diagnostician inactive_diagnostician_;
};

} // end namespace easyeye

#endif	/* DIAGNOSTICS_H */

