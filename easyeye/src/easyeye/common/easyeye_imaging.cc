#include "easyeye_imaging.h"
#include "../common/base64.h"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <cstring>
#include <opencv2/imgproc/imgproc.hpp>
#include <jsoncpp/json/json.h>
#include <cmath>

using namespace easyeye;
using namespace std;
using namespace cv;

Mat Imaging::CreateGrayImage(int nrows, int ncols)
{
    Mat image(nrows, ncols, GRAY_DATA_TYPE);
    return image;
}

void Imaging::ShallowCopy(const cv::Mat& src, Masek::IMAGE* dst) 
{
    dst->hsize[HSIZE_ROWS] = src.rows;
    dst->hsize[HSIZE_COLS] = src.cols;
    dst->data = src.data;
}


Masek::IMAGE* Imaging::CopyToMasek(Mat& src)
{
    Masek::IMAGE * eyeImg = (Masek::IMAGE*) malloc (sizeof(Masek::IMAGE));
    eyeImg->hsize[HSIZE_ROWS] = src.rows;//grayImg->height;
	eyeImg->hsize[HSIZE_COLS] = src.cols;//grayImg->width;
	eyeImg->data = (unsigned char*)malloc(sizeof(unsigned char)*eyeImg->hsize[0]*eyeImg->hsize[1]);
    for (int y = 0; y < src.rows; y++) {
        Mat row = src.row(y);
        for (int x = 0; x < src.cols; x++) {
            eyeImg->data[y * src.cols + x] = row.at<uchar>(x);
        }
    }
	return eyeImg;
}

void Imaging::CopyFromMasek(Masek::IMAGE* src, Mat& dst)
{
    dst.create(src->hsize[HSIZE_ROWS], src->hsize[HSIZE_COLS], GRAY_DATA_TYPE);    
    memcpy(dst.data, src->data, dst.rows * dst.cols);
}

Mat Imaging::CopyFromMasek(Masek::IMAGE* src)
{
    Mat dst;
	CopyFromMasek(src, dst);
	return dst;
}

//double Imaging::cvThreshold(const CvArr* src, CvArr* dst, double threshold, double max_value, int threshold_type)
//{
//    cv::Mat matSrc = cv::cvarrToMat(src);
//    cv::Mat matDst = cv::cvarrToMat(dst);
//	return cv::threshold(matSrc, matDst, threshold, 255, threshold_type);
//}

void Imaging::cvSmooth(const cv::Mat& matSrc, cv::Mat& matDst, int smoothtype, int size1, int size2, double sigmaX, double sigmaY )
{
    assert(smoothtype == CV_GAUSSIAN);
    if (size2 == 0) {
        size2 = size1;
    }
    cv::Size ksize(size1, size2);
	if (sigmaX == 0) {
        // cvSmooth's default sigma is calculated based on kernel size 
        // http://docs.opencv.org/trunk/modules/imgproc/doc/filtering.html?highlight=smooth#smooth
        int size_for_sigma = size1 >= size2 ? size1 : size2;
        sigmaX = 0.3 * (size_for_sigma / 2 - 1) + 0.8; 
    }
    cv::GaussianBlur(matSrc, matDst, ksize, sigmaX, sigmaY);
}

void Imaging::cvSmooth(const CvArr* src, CvArr* dst, int smoothtype, int size1, int size2, double sigmaX, double sigmaY )
{
    cv::Mat matSrc = cv::cvarrToMat(src);
    cv::Mat matDst = cv::cvarrToMat(dst);
    cvSmooth(matSrc, matDst, smoothtype, size1, size2, sigmaX, sigmaY);
}

void Imaging::cvCvtColor(const CvArr* src, CvArr* dst, int code)
{
    Mat matSrc = cvarrToMat(src);
    Mat matDst = cvarrToMat(dst);
    cv::cvtColor(matSrc, matDst, code);
}

void Imaging::cvGetQuadrangleSubPix(const CvArr* src, CvArr* dst, const Mat& map_matrix)
{
    Mat matSrc = cvarrToMat(src);
    Mat matDst = cvarrToMat(dst);
    Size dsize(matDst.cols, matDst.rows);
    cv::warpAffine(matSrc, matDst, map_matrix, dsize);
    
}

Mat Imaging::GetROI(const Mat& src, int startX, int width, int startY, int height)
{
    Rect region(startX, startY, width, height);
    Mat roiSrc = src(region);
    Mat roi = roiSrc.clone();
    return roi;
}

static int clamp(int value, int min_inclusive, int max_inclusive)
{
	if(value < min_inclusive)
		value = min_inclusive;
	if(value > max_inclusive)
		value = max_inclusive;	
	return value;
}


void Imaging::myRect(const cv::Mat& image, int x, int y, int radius, int* destVal)
{  
	int startX, endX, startY, endY;
	startX = x-radius;// X starting point
	endX = x+radius;// X end point	
    startY = y-radius;// Y starting point
	endY = y+radius;// Y end point
	
	destVal[0] = clamp(startX, 1, image.cols);
	destVal[1] = clamp(endX, 1, image.cols);	
	destVal[2] = clamp(startY, 1, image.rows);
	destVal[3] = clamp(endY, 1, image.rows);
}

bool Imaging::IsGray(const Mat& image) 
{
    return image.type() == CV_8UC1;
}

void serial::GrayImageAdapter::ToJson(void* src, Json::Value& dst)
{
   cv::Mat& gray_image = *((cv::Mat*)src);
   dst["rows"] = gray_image.rows;
   dst["cols"] = gray_image.cols;
   Base64 codec;
   std::string encoded_data = codec.Encode(gray_image.data, gray_image.rows * gray_image.cols);
   dst["data"] = encoded_data;
}

bool serial::GrayImageAdapter::FromJson(const Json::Value& src, void* dst)
{
   cv::Mat& gray_image = *((cv::Mat*)dst);
   gray_image.create(src.get("rows", 0).asInt(), src.get("cols", 0).asInt(), CV_8UC1);// = (Masek::IMAGE*) malloc(sizeof(Masek::IMAGE));
   size_t data_len = gray_image.rows * gray_image.cols;
   Base64 codec;
   std::vector<unsigned char> data_bytes;
   codec.Decode(src.get("data", "").asString(), data_bytes);
   bool data_len_ok = data_bytes.size() == data_len;
   if (data_len_ok) {
      Vectors::CopyTo(data_bytes, gray_image.data, data_len);
   }
   return src.isMember("rows") && src.isMember("cols") && src.isMember("data") && data_len_ok;
}

bool serial::SparseMatAdapter::FromJson(const Json::Value& src, void* dst)
{
    SparseMat& dest_sm = *((SparseMat*)dst);
    Mat dest_m;
    GrayImageAdapter gia;
    if (!gia.FromJson(src, (void*)&dest_m)) {
        return false;
    }
    SparseMat tmp(dest_m);
    tmp.copyTo(dest_sm);
    return true;
}

void serial::SparseMatAdapter::ToJson(void* src, Json::Value& dst)
{
    SparseMat& sm = *((SparseMat*)src);
    Mat m;
    sm.convertTo(m, CV_8UC1);
    GrayImageAdapter gia;
    gia.ToJson((void*)&m, dst);
}

Mat Imaging::CopyFromFilter(Masek::filter* filter)
{
    Mat dst;
    CopyFromFilter(filter, dst);
    return dst;
}

void Imaging::CopyFromFilter(Masek::filter* src, FloatImage& dst)
{
    dst.create(src->hsize[HSIZE_ROWS], src->hsize[HSIZE_COLS], FLOAT_DATA_TYPE);
    for (int y = 0; y < dst.rows; y ++) {
        Mat dst_row = dst.row(y);
        for (int x = 0; x < dst.cols; x++) {
            dst_row.at<float>(x) = (float) src->data[y * dst.cols + x];
        }
    }
}

/**
 * Allocates a new filter, copies data from a source image, 
 * and returns a pointer to the filter. The source image must have been
 * created with CV_32FC1 data type. The new filter must be freed when
 * you're done with it.
 * @param src the source image
 * @return a pointer to the newly allocated filter
 */
Masek::filter* Imaging::CopyToFilter(const cv::Mat& src)
{
    Masek::filter* dst = (Masek::filter*) malloc(sizeof(Masek::filter));
    dst->hsize[HSIZE_ROWS] = src.rows;
    dst->hsize[HSIZE_COLS] = src.cols;
    dst->data = (double*) malloc(src.rows * src.cols * sizeof(double));
    for (int y = 0; y < src.rows; y++) {
        Mat row = src.row(y);
        for (int x = 0; x < src.cols; x++) {
            dst->data[y * src.cols + x] = row.at<float>(x);
        }
    }
    return dst;
}

void Imaging::FreeFilter(Masek::filter* filter)
{
    FreeFilterData(filter);
    free(filter);
}

void Imaging::FreeFilterData(Masek::filter* filter)
{
    free(filter->data);
}

void Imaging::FreeImage(Masek::IMAGE* image)
{
    FreeImageData(image);
    free(image);
}

void Imaging::FreeImageData(Masek::IMAGE* image)
{
    free(image->data);
}

void Imaging::CopyToMat(Masek::filter* src, cv::Mat& dst)
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

void Imaging::CopyToMat(Masek::IMAGE* src, cv::Mat& dst)
{
    int rows = src->hsize[1], cols = src->hsize[0];
    dst.create(rows, cols, CV_8U);
    for (int y = 0; y < rows; y++) {
        for (int x = 0; x < cols; x++) {
            dst.at<uchar>(x, y) = src->data[y * cols + x];
        }
    }
}

long Imaging::Round(FloatDataType float_value)
{
    return (long)roundf(float_value);
}

uchar Imaging::Clamp(long value, uchar min_inclusive, uchar max_inclusive)
{
    if (value < min_inclusive) {
        return min_inclusive;
    } 
    if (value > max_inclusive) {
        return max_inclusive;
    }
    return (uchar) value;
}