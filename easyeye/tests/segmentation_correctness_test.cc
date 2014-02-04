/* 
 * File:   trace_segmentation_test.cc
 * Author: mchaberski
 *
 * Created on Jan 24, 2014, 3:34:27 PM
 */

#include <fstream>
#include <iostream>
#include <sys/stat.h>
#include <sys/types.h>
#include <cerrno>
#include "../src/easyeye/common/easyeye_diagnostics.h"
#include "../src/easyeye/segment/easyeye_segment.h"
#include "../src/easyeye/common/mylog.h"
#include <opencv2/highgui/highgui.hpp>
#include <string>
#include "../src/easyeye/common/easyeye_utils.h"
#include "testdata.h"

using namespace easyeye;
using namespace std;
using cv::Mat;

static const char* output_dir = "/tmp/easyeye_tests_segmenter";

BoundaryPair expected_boundary_pairs[10];

static bool write_segmentation_serializations = false;

void populateExpectedBoundaryPairs() 
{
    int k = 0;
    expected_boundary_pairs[k].iris.center.x = 401;
    expected_boundary_pairs[k].iris.center.y = 271;
    expected_boundary_pairs[k].iris.radius = 140;
    expected_boundary_pairs[k].pupil.center.x = 400;
    expected_boundary_pairs[k].pupil.center.y = 278;
    expected_boundary_pairs[k].pupil.radius = 40;
    k = 1;
    expected_boundary_pairs[k].iris.center.x = 202;
    expected_boundary_pairs[k].iris.center.y = 272;
    expected_boundary_pairs[k].iris.radius = 117;
    expected_boundary_pairs[k].pupil.center.x = 204;
    expected_boundary_pairs[k].pupil.center.y = 271;
    expected_boundary_pairs[k].pupil.radius = 38;
    k = 2;
    expected_boundary_pairs[k].iris.center.x = 254;
    expected_boundary_pairs[k].iris.center.y = 259;
    expected_boundary_pairs[k].iris.radius = 145;
    expected_boundary_pairs[k].pupil.center.x = 253;
    expected_boundary_pairs[k].pupil.center.y = 258;
    expected_boundary_pairs[k].pupil.radius = 46;
    k = 3;
    expected_boundary_pairs[k].iris.center.x = 388;
    expected_boundary_pairs[k].iris.center.y = 258;
    expected_boundary_pairs[k].iris.radius = 120;
    expected_boundary_pairs[k].pupil.center.x = 385;
    expected_boundary_pairs[k].pupil.center.y = 257;
    expected_boundary_pairs[k].pupil.radius = 43;
    k = 4;
    expected_boundary_pairs[k].iris.center.x = 244;
    expected_boundary_pairs[k].iris.center.y = 258;
    expected_boundary_pairs[k].iris.radius = 122;
    expected_boundary_pairs[k].pupil.center.x = 246;
    expected_boundary_pairs[k].pupil.center.y = 255;
    expected_boundary_pairs[k].pupil.radius = 45;
    k = 5;
    expected_boundary_pairs[k].iris.center.x = 343;
    expected_boundary_pairs[k].iris.center.y = 282;
    expected_boundary_pairs[k].iris.radius = 145;
    expected_boundary_pairs[k].pupil.center.x = 352;
    expected_boundary_pairs[k].pupil.center.y = 281;
    expected_boundary_pairs[k].pupil.radius = 39;
    k = 6;
    expected_boundary_pairs[k].iris.center.x = 258;
    expected_boundary_pairs[k].iris.center.y = 260;
    expected_boundary_pairs[k].iris.radius = 142;
    expected_boundary_pairs[k].pupil.center.x = 257;
    expected_boundary_pairs[k].pupil.center.y = 257;
    expected_boundary_pairs[k].pupil.radius = 46;
    k = 7;
    expected_boundary_pairs[k].iris.center.x = 274;
    expected_boundary_pairs[k].iris.center.y = 309;
    expected_boundary_pairs[k].iris.radius = 137;
    expected_boundary_pairs[k].pupil.center.x = 241;
    expected_boundary_pairs[k].pupil.center.y = 296;
    expected_boundary_pairs[k].pupil.radius = 41;
    k = 8;
    expected_boundary_pairs[k].iris.center.x = 257;
    expected_boundary_pairs[k].iris.center.y = 263;
    expected_boundary_pairs[k].iris.radius = 122;
    expected_boundary_pairs[k].pupil.center.x = 259;
    expected_boundary_pairs[k].pupil.center.y = 260;
    expected_boundary_pairs[k].pupil.radius = 36;
}

const int boundaryPairMaxDelta = 10;

bool isBoundaryPairEquivalent(BoundaryPair& expected, BoundaryPair& actual)
{
    int ixDelta = abs(expected.iris.center.x - actual.iris.center.x);
    int iyDelta = abs(expected.iris.center.y - actual.iris.center.y);
    int irDelta = abs(expected.iris.radius - actual.iris.radius);
    int pxDelta = abs(expected.pupil.center.x - actual.pupil.center.x);
    int pyDelta = abs(expected.pupil.center.y - actual.pupil.center.y);
    int prDelta = abs(expected.pupil.radius - actual.pupil.radius);
    cout << "deltas:" 
            << ' ' << ixDelta 
            << ' ' << iyDelta
            << ' ' << irDelta
            << ' ' << pxDelta
            << ' ' << pyDelta
            << ' ' << prDelta 
            << endl;
    return (ixDelta < boundaryPairMaxDelta) 
            && (iyDelta < boundaryPairMaxDelta) 
            && (irDelta < boundaryPairMaxDelta) 
            && (pxDelta < boundaryPairMaxDelta) 
            && (pyDelta < boundaryPairMaxDelta) 
            && (prDelta < boundaryPairMaxDelta) ;
}


void printResult(const char* eyeImagePathname, Segmentation& result) {
    const char* status = Result::DescribeStatus(result.status);
    cout << "  " << status << ' ' << eyeImagePathname << endl;
    cout << "  iris  " << result.boundary_pair.iris.center.x << ' ' << result.boundary_pair.iris.center.y << ' ' << result.boundary_pair.iris.radius << endl;
    cout << "  pupil " << result.boundary_pair.pupil.center.x << ' ' << result.boundary_pair.pupil.center.y << ' ' << result.boundary_pair.pupil.radius << endl;
}

void stateFailed(const char* testname, const char* message) 
{
    std::cout << "%TEST_FAILED% time=0 testname=" << testname << " (trace_segmentation) message=" << message << std::endl;
}

void configureDiagnostics(Diagnostician& diags)
{
    diags.set_disabled(false);
    diags.set_output_dir(output_dir);
    diags.set_collect_pathnames(true);
    int mkdir_result = mkdir(output_dir, 0755);
    if (mkdir_result != 0) {
        if (errno != EEXIST) {
            cerr << "mkdir result = " << mkdir_result << " with errno = " << errno << endl;
            stateFailed("traceSegmentation", "mkdir failed");
            return;
        }
    }
}

void printParamsAsCsv(ostream& out, const char* category, const char* eyeImagePathname, BoundaryPair& bp)
{
    cout << "CSV," 
            << "\"" << category << "\",\"" << eyeImagePathname 
            << "\",\"circle " << bp.iris.center.x << ' ' << bp.iris.center.y << ' ' << bp.iris.radius 
            << "\",\"circle " << bp.pupil.center.x << ' ' << bp.pupil.center.y << ' ' << bp.pupil.radius 
            << "\"" << endl;
}

bool traceSegmentation(const char* eyeImagePathname, BoundaryPair& expected, const char* serialized_segmentation_pathname) 
{
    cout << "traceSegmentation: " << eyeImagePathname << endl;
    mylog::Logs::GetLogger().set_level(mylog::TRACE);
    Segmentation result;
    Segmenter segmenter;
    Diagnostician diags;
    diags.set_eye_image_pathname(eyeImagePathname);
    configureDiagnostics(diags);
    segmenter.set_diagnostician(&diags);
    Mat eyeImage = cv::imread(eyeImagePathname, CV_LOAD_IMAGE_GRAYSCALE);
    printParamsAsCsv(cout, "expected", eyeImagePathname, expected);
    bool ok = false;
    if (eyeImage.data != NULL) {
        segmenter.SegmentEyeImage(eyeImage, result);
        printResult(eyeImagePathname, result);//result.Describe(cout);
        printParamsAsCsv(cout, "actual", eyeImagePathname, result.boundary_pair);
        if (!isBoundaryPairEquivalent(expected, result.boundary_pair)) {
            cout << "actual boundary pair differs from expected: ";
            expected.Describe(cout);
            cout << endl;
        } else {
            ok = true;
        }
        if (write_segmentation_serializations) {
            serial::SegmentationAdapter adapter;
            string json = serial::Serialize(result, &adapter);
            Files::Write(json, serialized_segmentation_pathname);
            cout << ">>> wrote " << serialized_segmentation_pathname << endl;
        }
    } else {
        stateFailed("traceSegmentation", "eye image could not be loaded");
    }
    return ok;
}

void test1() {
    std::cout << "segmentation_correctness_test test 1" << std::endl;
    int num_ok = 0;
    const int nimages = NUM_SAMPLES;
    for (int i = 0; i < nimages; i++) {
        bool ok = traceSegmentation(eye_image_files[i], expected_boundary_pairs[i], serialized_segmentations[i]);
        if (ok) num_ok++;
    }
    bool all_ok = num_ok == nimages;
    cout << num_ok << " of " << nimages << " has traceSegmentation success" << endl;
    if (!all_ok) {
        stateFailed("test1", "traceSegmentation returned false");
    }
}

int main(int argc, char** argv) {
    std::cout << "%SUITE_STARTING% segmentation_correctness_test" << std::endl;
    std::cout << "%SUITE_STARTED%" << std::endl;
    
    populateExpectedBoundaryPairs();
    std::cout << "%TEST_STARTED% test1 (segmentation_correctness_test)" << std::endl;
    test1();
    std::cout << "%TEST_FINISHED% time=0 test1 (segmentation_correctness_test)" << std::endl;

    std::cout << "%SUITE_FINISHED% time=0" << std::endl;

    return (EXIT_SUCCESS);
}

