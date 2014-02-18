/* 
 * File:   easyeye_diagnostics.h
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
    Diagnostician();
    void DumpSegOutput(const BoundaryPair& bp, const EyelidsLocation& eyelids, cv::SparseMat& extrema_noise);
    void DumpNormOutput(const cv::Mat& polar_array, const cv::Mat& noise_array);
    void DumpEncodeOutput(const int width, const int height, const int* irisTemplate, const int* irisMask);
    bool disabled() const;
    void set_disabled(bool disabled);
    void set_output_dir(const std::string& output_dir);
    bool collect_pathnames() const;
    void set_collect_pathnames(bool collect_pathnames);
    std::vector<std::string>& files_written();
    void WriteImage(const cv::Mat& image, const std::string& label);
    void WriteText(const std::string& text, const std::string& label);
    void set_write_original(bool write_original);
    bool write_original() const;
    void set_eye_image_pathname(const std::string& eye_image_pathname);
    std::string eye_image_pathname() const;
private:
    std::string output_dir_;
    bool disabled_;
    bool verbose_;
    bool collect_pathnames_;
    int num_images_written_;
    std::string eye_image_pathname_;
    std::ostream& text_output_stream_;
    std::string image_format_suffix_;
    std::vector<std::string> files_written_;
    bool write_original_;
    bool original_written_;
    std::string ToFilename(const std::string& label, const std::string& suffix);
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

class DiagnosticArt
{
public:
    class MaskColoring 
    {
    public:
        /*
         * 4-component vector (BGRA or RGBA)
         */
        cv::Vec4b color;
        /*
         * value in interval [0,1]
         */
        double pinch;
        MaskColoring() : color(0, 0, 0, 0xff), pinch(1.0) {}
        MaskColoring(cv::Vec4b color_) : color(color_), pinch(1.0) {}
        MaskColoring(cv::Vec4b color_, double pinch_) : color(color_), pinch(pinch_) {}
        uchar GetPinched(uchar in_color) 
        {
            int int_in_color = in_color - 0x80;
            int int_out_color = ((int) round(int_in_color * pinch)) + 0x80;
            return (uchar) int_out_color;
        }
    };
    /**
     * 
     * @param base_image CV_8UC1 image
     * @param mask_overlay CV_8UC1 image where pixel value of zero represents 
     * masked area and nonzero represents unchanged area
     * @param masked_color with BGRA scalar color 
     * @return 
     */
    static cv::Mat Compose(const cv::Mat& base_image, const cv::Mat& mask_overlay, MaskColoring masked_color);
    
    /**
     * 
     * @param base_image_to_draw_on CV_8UC3
     * @param mask_overlay CV_8UC1
     * @param masked_color
     */
    static void ComposeOn(cv::Mat& base_image_to_draw_on, const cv::Mat& mask_overlay, MaskColoring masked_color);
    
    /**
     * Computes the values of a vector representing a pixel whose original
     * (base) color may blended with a mask coloring, depending on the overlay
     * color that indicates whether it is masked.
     * @param base_color the original 8-bit gray pixel color
     * @param overlay_value 
     * @param coloring
     * @return 
     */
    static cv::Vec3b ToResultColor(uchar base_color, uchar overlay_value, const MaskColoring& coloring);
    static cv::Vec3b ToResultColor(cv::Vec3b base_color, uchar overlay_value, const MaskColoring& coloring);
    
private:
    DiagnosticArt();
};

} // end namespace easyeye

#endif	/* DIAGNOSTICS_H */

