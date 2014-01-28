/* 
 * File:   eziencode_test.cc
 * Author: mchaberski
 *
 * Created on Jul 15, 2013, 5:52:21 PM
 */

#include <fstream>
#include <string>
#include <stdlib.h>
#include <iostream>
#include "../src/easyeye/common/easyeye_types.h"
#include "../src/easyeye/segment/easyeye_segment.h"
#include <opencv2/highgui/highgui.hpp>
#include "../src/easyeye/encode/easyeye_encode.h"

using namespace std;
using namespace easyeye;
using cv::Mat;

int numSegmented = 0;

void testSegmentSuccess(const char* eyeImagePathname, Segmentation* result) 
{
    Segmenter segmenter;
    Mat eyeImage = cv::imread(eyeImagePathname, CV_LOAD_IMAGE_GRAYSCALE);
    segmenter.SegmentEyeImage(eyeImage, result);
    const char* status = Result::DescribeStatus(result->status);
    std::cout << "segmentation " << status << ' ' << eyeImagePathname << std::endl;
    
    numSegmented++;
    ostringstream foutname;
    foutname << "/tmp/segmentation-";
    foutname << numSegmented << ".json";
    const string fname = foutname.str();
    ofstream fout(fname.c_str());
    if (fout.is_open()) {
        SegmentationSerializer ss;
        ss.Serialize(*result, static_cast<ostream&>(fout));
        fout.close();
        cout << "wrote segmentation to " << fname << endl;
    } else {
        cerr << "failed to serialize segmentation to " << fname << endl;
    }
}

const int maxLen = 32768;

const char* BuildFailMsg(const char* testName, const char* errorMsg, const char* eyeImagePathname)
{
    static char msg[maxLen];
    const char* prefix = "%TEST_FAILED% time=0 testname=";
    const char* infix = " (ezisegment_test) message=";
    if ((strlen(testName) + strlen(errorMsg) + strlen(eyeImagePathname) + strlen(prefix) + strlen(infix) + 5) >= maxLen) {
        std::cerr << "bad input to BuildFailMsg: " << testName << ' ' << errorMsg << ' ' << eyeImagePathname << std::endl;
        return "%TEST_FAILED%";
    }
    sprintf(msg, "%s %s %s %s %s", prefix, testName, infix, errorMsg, eyeImagePathname);
    return msg;
}

Encoding* SegmentAndEncode(const char* testName, const char* eyeImagePathname)
{
    Segmentation result;
    testSegmentSuccess(eyeImagePathname, &result);
    if (result.status != Result::SUCCESS) {
        std::cout << BuildFailMsg(testName, "failed to segment", eyeImagePathname) << std::endl;
        return NULL;
    }
    Encoder encoder;
    Normalization normalization(encoder.angularRes, encoder.radialRes);
    encoder.NormalizeIris(result.noiseImage, &(result.boundaryPair), &normalization);
    std::cout << "normalization " << Result::DescribeStatus(normalization.status) << ' ' << eyeImagePathname << std::endl;
    if (normalization.status != Normalization::SUCCESS) {
        std::cout << BuildFailMsg(testName, "failed to normalize segmented iris", eyeImagePathname) << std::endl;
        return NULL;
    } 
    Encoding* encoding = new Encoding(encoder.encodeScales, encoder.angularRes, encoder.radialRes);
    encoder.EncodeIris(&normalization.polarArray, &normalization.noiseArray, encoding);
    std::cout << "encoding " << Result::DescribeStatus(encoding->status) << ' ' << eyeImagePathname << std::endl;
    if (encoding->status != Encoding::SUCCESS) {
        std::cout << BuildFailMsg(testName, "failed to encode normalized iris", eyeImagePathname) << std::endl;
        delete encoding;
        return NULL;
    }
    return encoding;
}

void testEncodeIris() 
{
    std::cout << "eziencode_test testEncodeIris" << std::endl;
    const char* eyeImagePathname = "../testdata/images/gallery_022.bmp";
    
    Encoding* encoding = SegmentAndEncode("EncodeIris", eyeImagePathname);
    if (encoding != NULL) {
        std::cout << std::endl;
        encoding->Serialize(std::cout);
        std::cout << "encoding iris from " << eyeImagePathname << " succeeded" << std::endl;
        delete encoding;
    }
}
static void testSerializeAndDeserialize()
{
    std::cout << "eziencode_test testEncodeIris" << std::endl;
    const char* eyeImagePathname = "../testdata/images/gallery_022.bmp";
    Encoding* encoding = SegmentAndEncode("EncodeIris", eyeImagePathname);
    if (encoding == NULL) {
        std::cout << BuildFailMsg("testSerializeAndDeserialize", "failed to segment or encode", eyeImagePathname) << std::endl;
        return;
    }
//    char * tmpFilename = "/tmp/encodingXXXXXX";
//    int fd = mkstemp(tmpFilename);
//    if (fd < 0) {
//        std::cout << BuildFailMsg("testSerializeAndDeserialize", "failed to create temp file", eyeImagePathname);
//        delete encoding;
//        return;
//    }
//    FILE* tmpfile = fdopen(tmpFilename, "w");
//    if (tmpfile == NULL) {
//        std::cout << BuildFailMsg("testSerializeAndDeserialize", "failed to open temp file for writing", eyeImagePathname);
//        delete encoding;
//        return;
//    }
    char json[Encoding::ENCODING_DESERIALIZATION_BUFFER_LEN];
    encoding->Serialize(json);
//    fputs(json, tmpfile);
//    fclose(tmpfile);
    Encoding deserialized;
    deserialized.Deserialize(json);
    if (deserialized.status != Encoding::SUCCESS || !IsEqual(*encoding, deserialized)) {
        std::cout << BuildFailMsg("testSerializeAndDeserialize", "failed to serialize or deserialize", eyeImagePathname) << std::endl;
    } else {
//        std::cout << std::endl << json << std::endl;
        std::cout << "Encoding serialization/deserialization to/from string succeeded" << std::endl;
    }
    delete encoding;
//    remove(tmpfile);
}

int main(int argc, char** argv) {
    std::cout << "%SUITE_STARTING% eziencode_test" << std::endl;
    std::cout << "%SUITE_STARTED%" << std::endl;

//    std::cout << "%TEST_STARTED% test1 (eziencode_test)" << std::endl;
//    testEncodeIris();
//    std::cout << "%TEST_FINISHED% time=0 test1 (eziencode_test)" << std::endl;

    std::cout << "%TEST_STARTED% testSerializeAndDeserialize (eziencode_test)\n" << std::endl;
    testSerializeAndDeserialize();
    std::cout << "%TEST_FINISHED% time=0 testSerializeAndDeserialize (eziencode_test)" << std::endl;

    std::cout << "%SUITE_FINISHED% time=0" << std::endl;

    return (EXIT_SUCCESS);
}

