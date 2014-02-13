#include "easyeye_imaging.h"
#include "../common/base64.h"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <cstring>
#include <opencv2/imgproc/imgproc.hpp>
#include <jsoncpp/json/json.h>
#include <cmath>
#include "../common/mylog.h"

using namespace easyeye;
using namespace cv;
using mylog::Logs;
using cvmore::Pixels;

int Pixels::Clamp(int value, int min_inclusive, int max_inclusive)
{
	if(value < min_inclusive)
		value = min_inclusive;
	if(value > max_inclusive)
		value = max_inclusive;	
	return value;
}

uchar Pixels::Clamp(int value, uchar min_inclusive, uchar max_inclusive) 
{
    if (value < (int) min_inclusive) {
        return min_inclusive;
    }
    if (value > (int) max_inclusive) {
        return max_inclusive;
    }
    return (uchar) value;
}

uchar Pixels::Clamp(int value)
{
    return Clamp(value, (uchar)0, (uchar)0xff);
}

float Pixels::Interpolate(const Mat& img, double px, double py)
{ // http://stackoverflow.com/questions/13299409/how-to-get-the-image-pixel-at-real-locations-in-opencv
    assert(!img.empty());
    int x = (int)px;
    int y = (int)py;

    int x0 = cv::borderInterpolate(x,   img.cols, cv::BORDER_REFLECT_101);
    int x1 = cv::borderInterpolate(x+1, img.cols, cv::BORDER_REFLECT_101);
    int y0 = cv::borderInterpolate(y,   img.rows, cv::BORDER_REFLECT_101);
    int y1 = cv::borderInterpolate(y+1, img.rows, cv::BORDER_REFLECT_101);

    float a = px - (float)x;
    float c = py - (float)y;

    float b = ((img.at<uchar>(y0, x0) * (1.f - a) 
            + img.at<uchar>(y0, x1) * a) * (1.f - c)
            + (img.at<uchar>(y1, x0) * (1.f - a) 
            + img.at<uchar>(y1, x1) * a) * c);
    return b;
} 

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

void Imaging::myRect(const cv::Mat& image, int x, int y, int radius, int* destVal)
{  
	int startX, endX, startY, endY;
	startX = x-radius;// X starting point
	endX = x+radius;// X end point	
    startY = y-radius;// Y starting point
	endY = y+radius;// Y end point
	
	destVal[0] = Pixels::Clamp(startX, 1, image.cols);
	destVal[1] = Pixels::Clamp(endX, 1, image.cols);	
	destVal[2] = Pixels::Clamp(startY, 1, image.rows);
	destVal[3] = Pixels::Clamp(endY, 1, image.rows);
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
    return (long) roundf(float_value);
}

uchar Pixels::Clamp(long value, uchar min_inclusive, uchar max_inclusive)
{
    if (value < min_inclusive) {
        return min_inclusive;
    } 
    if (value > max_inclusive) {
        return max_inclusive;
    }
    return (uchar) value;
}

void Imaging:: myXYRect(const cv::Mat& image, int x, int y, int width, int height, int* destVal)
{  
	int startX, endX, startY, endY;
	startX = x-width;// X starting point
	endX = x+width;// X end point	
    startY = y-height;// Y starting point
	endY = y+height;// Y end point

	// Debugging
	destVal[0] = Pixels::Clamp(startX, 1, image.cols);
	destVal[1] = Pixels::Clamp(endX, 1, image.cols);	
	destVal[2] = Pixels::Clamp(startY, 1, image.rows);
	destVal[3] = Pixels::Clamp(endY, 1, image.rows);
}

float Imaging::myMean(const Mat& img)
{
	float mean = 0.0f;
	double sum =0.0f;

	for(int y=0; y< img.rows; y++)
		for(int x=0; x< img.cols; x++)
		{
			sum += (unsigned char) img.data[y*img.cols+x];
		}
	mean = (float) sum / (float)(img.rows * img.cols);
	return mean;
}

int Imaging::getValue(int value, int maxSize)
{
    return Pixels::Clamp(value, 1, maxSize);
}

float Imaging::mySD(const cv::Mat& image, double n, float mean)
{
	float variance = 0.0f, sd=0.0f;
    const int GRAY_LEVEL = 256;
	// Use a look-up table for faster calculation
	double lookup[GRAY_LEVEL];
	for (int val = 0; val < GRAY_LEVEL; val++) 
	{
		lookup[val] = pow((val - mean), 2);
	}

	for(int y=0; y<image.rows; y++)
	{
		for(int x=0; x<image.cols; x++)
		{
			unsigned char val = (unsigned char) image.data[y * image.cols + x];
			variance += (float)lookup[val];
		}
	}
	sd = sqrt(variance/(float)(n));
	return sd;
}

Imaging::EyeImageROI Imaging::GetEyeImageROI(const cv::Mat& eye_img, cv::Point2i center,
		int cr, int xLimit, int yLimit)
{
	Logs::GetLogger().Log(mylog::TRACE, "ImageUtility::setImage %dx%d center=(%d,%d) cr=%d, xLimit=%d, yLimit=%d\n",
			eye_img.cols, eye_img.rows, center.x, center.y, cr, xLimit, yLimit);
	EyeImageROI setVal;

	// Initialize
	setVal.rect.x = 0;
	setVal.rect.y = 0;
	setVal.rect.width = 0;
	setVal.rect.height = 0;
	setVal.p.x = 0;
	setVal.p.y = 0;	
			
	if(center.x != 0 && center.y != 0 && cr != 0)
	{
		cv::Point2i newCircle;
//		int newRadius= cr;
		
		// Set ROI in the assumed iris area
		int startX, endX, startY, endY;
		
		if(center.x-cr < 0)
		{
			startX = MAX(0, 0);
			newCircle.x = center.x;
		}
		else
		{
			startX = MAX(center.x-xLimit, 0);
			newCircle.x = xLimit;
		}
		
		endX = MIN(center.x+xLimit, eye_img.cols);

		if(center.y-cr < 0)
		{
			startY = MAX(0,0);
			newCircle.y = center.y;
		}
		else
		{
			startY = MAX(center.y-yLimit, 0);
			newCircle.y = yLimit;
		}

		endY = MIN(center.y+yLimit, eye_img.rows);
		
		// Return ROI and center points
		setVal.rect.x = startX;
		setVal.rect.y = startY;
		setVal.rect.width = endX-startX;
		setVal.rect.height = endY-startY;
		setVal.p.x = newCircle.x;
		setVal.p.y = newCircle.y;

		if(setVal.p.x < 1 || setVal.p.y < 1)
		{
			Logs::GetLogger().Log(mylog::ERROR, "ImageUtility::setImage Failed to load ROI for new circle\n");
		}
	} else {
		Logs::GetLogger().Log(mylog::ERROR, "ImageUtility::setImage did not attempt to load ROI for new circle\n");
	}
	Logs::GetLogger().Log(mylog::TRACE, "ImageUtility::setImage setVal.rect = %dx%d+%d+%d, setVal.p = %d,%d\n",
				setVal.rect.width, setVal.rect.height, setVal.rect.x, setVal.rect.y,
				setVal.p.x, setVal.p.y);
	return setVal;
}

uchar Pixels::Clamp(double value, uchar min_inclusive, uchar max_inclusive)
{
    if (value <= min_inclusive) {
        return min_inclusive;
    }
    if (value >= max_inclusive) {
        return max_inclusive;
    }
    return (uchar) round(value);
}

uchar Pixels::Clamp(double value)
{
    return Clamp(value, (uchar) 0x0, (uchar) 0xff);
}