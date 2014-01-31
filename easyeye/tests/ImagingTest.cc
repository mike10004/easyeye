/*
 * File:   ImagingTest.cc
 * Author: mchaberski
 *
 * Created on Jan 27, 2014, 1:21:34 PM
 */

#include <iostream>
#include "ImagingTest.h"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "../src/easyeye/common/easyeye_imaging.h"
#include "../src/easyeye/segment/ImageUtility.h"

using namespace std;
using namespace easyeye;
using namespace cv;

CPPUNIT_TEST_SUITE_REGISTRATION(ImagingTest);

ImagingTest::ImagingTest() {
}

ImagingTest::~ImagingTest() {
}

void ImagingTest::setUp() {
}

void ImagingTest::tearDown() {
}

void Describe(ostream& out, string name, IplImage* image)
{
    out << name << ' ' << image->width << 'x' << image->height << endl;
}

void Describe(ostream& out, string name, Mat& image)
{
    out << name << ' ' << image.cols << 'x' << image.rows << " continuous = " << image.isContinuous() << "; submatrix = " << image.isSubmatrix() << endl;
}

void ImagingTest::testGetROI() {
    IplImage* ipl = cvLoadImage("../testdata/images/green-rectangle.png", CV_LOAD_IMAGE_GRAYSCALE);
    Mat mat = cv::imread("../testdata/images/green-rectangle.png", CV_LOAD_IMAGE_GRAYSCALE);
    
    Describe(cerr, "ipl", ipl);
    Describe(cerr, "mat", mat);
    
    IplImage* ipl_roi = ImageUtility::getROIImage(ipl, 100, 150, 100, 150);
    Mat mat_roi = Imaging::GetROI(mat, 100, 150, 100, 150);
    
    Describe(cerr, "ipl_roi", ipl_roi);
    Describe(cerr, "mat_roi", mat_roi);

    CPPUNIT_ASSERT_EQUAL(ipl_roi->width, mat_roi.cols);
    CPPUNIT_ASSERT_EQUAL(ipl_roi->height, mat_roi.rows);
    
    cvReleaseImage(&ipl);
    cvReleaseImage(&ipl_roi);
}
