/* 
 * File:   easyeye_imaging.h
 * Author: ibgmike
 *
 * Created on January 27, 2014, 9:21 AM
 */

#ifndef EASYEYE_IMAGING_H
#define	EASYEYE_IMAGING_H

#include "../common/easyeye_utils.h"
#include "../common/easyeye_serial.h"
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <Masek.h>
#include "../common/base64.h"
#include <string>

namespace easyeye
{

    const int HSIZE_HEIGHT = 0;
    const int HSIZE_WIDTH = 1;
    const int HSIZE_ROWS = HSIZE_HEIGHT;
    const int HSIZE_COLS = HSIZE_WIDTH;
    
    class Imaging
    {
    public:
        typedef float FloatDataType;
        typedef uchar GrayDataType;
        typedef cv::Mat FloatImage;
        typedef cv::Mat ByteImage;
        static cv::Mat CreateGrayImage(int nrows, int ncols);
        static cv::Mat CopyFromMasek(Masek::IMAGE* dst);
        static void CopyFromMasek(Masek::IMAGE* src, cv::Mat& dst);
        static Masek::IMAGE* CopyToMasek(cv::Mat& src);
        static void ShallowCopy(const cv::Mat& src, Masek::IMAGE* dst);
        static void cvCvtColor(const CvArr* src, CvArr* dst, int code);
        static void cvGetQuadrangleSubPix(const CvArr* src, CvArr* dst, const cv::Mat& map_matrix);
        static void cvSmooth(const cv::Mat& src, cv::Mat& dst, int smoothtype=CV_GAUSSIAN, int size1=3, int size2=0, double sigma1=0, double sigma2=0 );
        static void cvSmooth(const CvArr* src, CvArr* dst, int smoothtype=CV_GAUSSIAN, int size1=3, int size2=0, double sigma1=0, double sigma2=0 );
        static double cvThreshold(const CvArr* src, CvArr* dst, double threshold, double max_value, int threshold_type);
        static cv::Mat GetROI(const cv::Mat& image, int startX, int width, int startY, int height);
        static bool IsGray(const cv::Mat& image);
        static void myRect(const cv::Mat& image, int x, int y, int radius, int* destVal);
        static cv::Mat CopyFromFilter(Masek::filter* src);
        static void CopyFromFilter(Masek::filter* src, FloatImage& dst);
        static Masek::filter* CopyToFilter(const cv::Mat& src);
        static const int GRAY_DATA_TYPE = CV_8UC1;
        static const int FLOAT_DATA_TYPE = CV_32FC1;
        static void FreeFilterData(Masek::filter* filter);
        static void FreeFilter(Masek::filter* filter);
        static void FreeImageData(Masek::IMAGE* image);
        static void FreeImage(Masek::IMAGE* image);
        static void CopyToMat(Masek::IMAGE* src, cv::Mat& dst);
        static void CopyToMat(Masek::filter* src, cv::Mat& dst);
        static long Round(FloatDataType float_value);
        static uchar Clamp(long value, uchar min_inclusive, uchar max_inclusive);
    };

    namespace serial 
    {
        class GrayImageAdapter : public Adapter
        {
        public:
            bool FromJson(const Json::Value& src, void* dst);
            void ToJson(void* src, Json::Value& dst);
        };

        class SparseMatAdapter : public Adapter
        {
        public:
            bool FromJson(const Json::Value& src, void* dst);
            void ToJson(void* src, Json::Value& dst);
        };
    }
}
#endif	/* EASYEYE_IMAGING_H */

