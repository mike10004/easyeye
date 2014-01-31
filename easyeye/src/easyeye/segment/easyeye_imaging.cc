#include "easyeye_imaging.h"
#include "../common/base64.h"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <cstring>
#include <opencv2/imgproc/imgproc.hpp>
#include <jsoncpp/json/json.h>

using namespace easyeye;
using namespace std;
using namespace cv;

Mat Imaging::CreateImage(int nrows, int ncols)
{
    Mat image(nrows, ncols, CV_8U);
    return image;
}

void Imaging::ShallowCopy(cv::Mat& src, Masek::IMAGE* dst) 
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

Mat Imaging::FromMasek(Masek::IMAGE* src)
{
	Mat dst = CreateImage(src->hsize[HSIZE_ROWS], src->hsize[HSIZE_COLS]);
	memcpy(dst.data, src->data, dst.rows * dst.cols);
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

Mat Imaging::GetROI(Mat& src, int startX, int width, int startY, int height)
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