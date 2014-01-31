#include "ImageUtility.h"
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "../common/mylog.h"
using mylog::Logs;

using cv::Size;
using cv::Mat;
using cv::cvarrToMat;

float ImageUtility::myMean(const Mat& img)
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

float ImageUtility::mySD(const cv::Mat& image, double n, float mean)
{
	float variance = 0.0f, sd=0.0f;

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

//IplImage* ImageUtility::convertToGray(IplImage* img)
//{
//   IplImage* grayImg = NULL;
//   grayImg = cvCreateImage(cvGetSize(img), img->depth, 1);
//   cvCvtColor(img, grayImg, CV_RGB2GRAY);
//   return grayImg; 
//}
//

IplImage* ImageUtility::extractImagePart(IplImage* img, CvRect& rect, int x, int y, int wd, int ht)
{
  rect = cvRect(x, y, wd, ht);
  cvSetImageROI(img, rect);
  IplImage* part = NULL;
  part = cvCreateImage(cvSize(wd, ht), img->depth, img->nChannels);
  cvCopy(img, part); // Copy the image instead of saving it
  cvResetImageROI(img);
  return part;
}

IplImage* ImageUtility:: getROIImage(IplImage* eyeImg, int startX, int width, int startY, int height)
{
	IplImage* setImg = NULL;
	setImg = cvCreateImage(cvSize(width, height), eyeImg->depth, eyeImg->nChannels);
	if (setImg == NULL) {
		return NULL;
	}
	int i, j, x, y;	
	for(j = startY, y = 0; y < height; j++, y++)	
		for(i = startX, x = 0; x < width; i++, x++)		
			setImg->imageData[x+y*setImg->widthStep] = 
				eyeImg->imageData[i+j*eyeImg->widthStep]; // Use the widthStep for the alignment

	//showImage("ROI", setImg);

	return setImg;
}



// Get the ROI set image
Masek::IMAGE* ImageUtility:: getROIImage_C(Masek::IMAGE* eyeImg, int startX, int width, int startY, int height)
{
	Masek::IMAGE* setImg;
	setImg = (Masek::IMAGE*) malloc (sizeof(Masek::IMAGE));
	setImg->hsize[0] = height ;
	setImg->hsize[1] = width ;
	setImg->data = (unsigned char*)malloc(sizeof(unsigned char)*setImg->hsize[0]*setImg->hsize[1]);
	
	int i, j, x, y;	
	for(j = startY-1, y = 0; y < height; j++, y++)	
		for(i = startX-1, x = 0; x < width; i++, x++)		
			setImg->data[x+y*setImg->hsize[1]] = 
			eyeImg->data[i+j*eyeImg->hsize[1]]; // For alignment

	return setImg;
}


ImageUtility::SETVALUE ImageUtility::setImage(const cv::Mat& eye_img, CvPoint center,
		int cr, int xLimit, int yLimit)
{
	Logs::GetLogger().Log(mylog::TRACE, "ImageUtility::setImage %dx%d center=(%d,%d) cr=%d, xLimit=%d, yLimit=%d\n",
			eye_img.cols, eye_img.rows, center.x, center.y, cr, xLimit, yLimit);
	SETVALUE setVal;

	// Initialize
	setVal.rect.x = 0;
	setVal.rect.y = 0;
	setVal.rect.width = 0;
	setVal.rect.height = 0;
	setVal.p.x = 0;
	setVal.p.y = 0;	
			
	if(center.x != 0 && center.y != 0 && cr != 0)
	{
		CvPoint newCircle;
		int newRadius= cr;
		
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

void ImageUtility:: myRect(IplImage* img, int x, int y, int radius, int* destVal)
{  
	int startX, endX, startY, endY;
	startX = x-radius;// X starting point
	endX = x+radius;// X end point	
    startY = y-radius;// Y starting point
	endY = y+radius;// Y end point

	//debug
	destVal[0] = getValue(startX, img->width);
	destVal[1] = getValue(endX, img->width);	
	destVal[2] = getValue(startY, img->height);
	destVal[3] = getValue(endY, img->height);
	
}

void ImageUtility:: myXYRect(const cv::Mat& image, int x, int y, int width, int height, int* destVal)
{  
	int startX, endX, startY, endY;
	startX = x-width;// X starting point
	endX = x+width;// X end point	
    startY = y-height;// Y starting point
	endY = y+height;// Y end point

	// Debugging
	destVal[0] = getValue(startX, image.cols);
	destVal[1] = getValue(endX, image.cols);	
	destVal[2] = getValue(startY, image.rows);
	destVal[3] = getValue(endY, image.rows);
}

void ImageUtility:: myRect_C(Masek::IMAGE *image, int x, int y, int radius, int* destVal)
{  
	int startX, endX, startY, endY;
	startX = x-radius;// X starting point
	endX = x+radius;// X end point	
    startY = y-radius;// Y starting point
	endY = y+radius;// Y end point
	
	destVal[0] = getValue(startX, image->hsize[1]);
	destVal[1] = getValue(endX, image->hsize[1]);	
	destVal[2] = getValue(startY, image->hsize[0]);
	destVal[3] = getValue(endY, image->hsize[0]);

}

// Debug values
int ImageUtility::getValue(int value, int maxSize)
{
	if(value < 1)
		value = 1;
	if(value > maxSize)
		value = maxSize;	
	return value;
}


