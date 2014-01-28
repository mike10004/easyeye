/* 
 * File:   ezisegment_test.cc
 * Author: mchaberski
 *
 * Created on Jul 15, 2013, 3:24:56 PM
 */

#include <stdlib.h>
#include <iostream>

#include "../src/easyeye/common/easyeye_types.h"
#include "../src/easyeye/segment/easyeye_segment.h"
#include <opencv2/highgui/highgui.hpp>

using namespace easyeye;
using namespace std;
using cv::Mat;

void printResult(const char* eyeImagePathname, Segmentation& result) {
    const char* status = Result::DescribeStatus(result.status);
    cout << "  " << status << ' ' << eyeImagePathname << endl;
    cout << "  iris  " << result.boundaryPair.irisX << ' ' << result.boundaryPair.irisY << ' ' << result.boundaryPair.irisR << endl;
    cout << "  pupil " << result.boundaryPair.pupilX << ' ' << result.boundaryPair.pupilY << ' ' << result.boundaryPair.pupilR << endl;
}

void testSegmentSuccess(const char* eyeImagePathname) 
{
    cout << "testSegmentSuccess: " << eyeImagePathname << endl;
    Segmentation result;
    Segmenter segmenter;
    Mat eyeImage = cv::imread(eyeImagePathname, CV_LOAD_IMAGE_GRAYSCALE);
    if (eyeImage.data != NULL) {
        segmenter.SegmentEyeImage(eyeImage, &result);
        printResult(eyeImagePathname, result);//result.Describe(cout);
    }
    if (result.status != Result::SUCCESS) {
        std::cout << "%TEST_FAILED% time=0 testname=testSegmentEyeImage (ezisegment_test) message=failed to segment " << eyeImagePathname << std::endl;
    }
    
}

void testSegmentEyeImage() {
    std::cout << "ezisegment_test test 1" << std::endl;
    const char* eyeImagePathnames[] = {
        "../testdata/images/probes_013.bmp",
        "../testdata/images/gallery_140.bmp",
        "../testdata/images/probes_016.bmp",
        "../testdata/images/probes_022.bmp",
        "../testdata/images/gallery_022.bmp",
        "../testdata/images/gallery_014.bmp",
        "../testdata/images/gallery_103.bmp",
        "../testdata/images/probes_010.bmp",
        "../testdata/images/probes_006.bmp",
        "../testdata/images/gallery_132.bmp"
    };
    const int nimages = 10;
    for (int i = 0; i < nimages; i++) {
        testSegmentSuccess(eyeImagePathnames[i]);
    }
}

void testLoadNonExistent() {
    std::cout << "ezisegment_test testLoadNonExistent" << std::endl;
    IplImage* eyeImage = cvLoadImage("/tmp/ThisImageDoesNotExist.bmp", 0);
    if (eyeImage != NULL) {
        std::cout << "%TEST_FAILED% time=0 testname=testLoadNonExistent (ezisegment_test) message=tried to load image from non-existent file but returned a valid IplImage pointer" << std::endl;
        cvReleaseImage(&eyeImage);
    }
    std::cout << "IplImage* == NULL from attempt to load image from non-existent file" << std::endl;
}

int main(int argc, char** argv) {
    std::cout << "%SUITE_STARTING% ezisegment_test" << std::endl;
    std::cout << "%SUITE_STARTED%" << std::endl;

    std::cout << "%TEST_STARTED% testSegmentEyeImage (ezisegment_test)" << std::endl;
    testSegmentEyeImage();
    std::cout << "%TEST_FINISHED% time=0 testSegmentEyeImage (ezisegment_test)" << std::endl;

    std::cout << "%TEST_STARTED% testLoadNonExistent (ezisegment_test)\n" << std::endl;
    testLoadNonExistent();
    std::cout << "%TEST_FINISHED% time=0 testLoadNonExistent (ezisegment_test)" << std::endl;

    std::cout << "%SUITE_FINISHED% time=0" << std::endl;

    return (EXIT_SUCCESS);
}

