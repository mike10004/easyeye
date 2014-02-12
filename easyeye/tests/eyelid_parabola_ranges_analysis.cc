/* 
 * File:   eyelid_parabola_ranges_analysis.cc
 * Author: mike
 *
 * Created on Feb 11, 2014, 11:47:45 PM
 */

#include <stdlib.h>
#include <iostream>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
#include <sstream>
#include <string>
using namespace cv;
using std::cout;
using std::cerr;
using std::endl;
using std::ostringstream;
using std::string;

bool IsRed(Vec3b pixel)
{
    uchar r = pixel[2], g = pixel[1], b = pixel[0];
    return r == 0xff && !(g == 0xff && b == 0xff);
}

Mat ToMask(const Mat& eyelid_drawing)
{
    Scalar no_color(0x0);
    Mat mask(eyelid_drawing.rows, eyelid_drawing.cols, CV_8U, no_color);
    uchar white = 0xff, black = 0x0;
    int num_mask_pixels = 0;
    for (int y = 0; y < eyelid_drawing.rows; y++) {
        Mat src_row = eyelid_drawing.row(y);
        Mat dst_row = mask.row(y);
        for (int x = 0; x < eyelid_drawing.cols; x++) {
            if (IsRed(src_row.at<Vec3b>(x))) {
                dst_row.at<uchar>(x) = white;
                num_mask_pixels++;
            }
        }
    }
    cout << num_mask_pixels << " mask pixels in " << mask.cols << "x" << mask.rows << " mask image (" << (mask.cols * mask.rows) << " pixels)" << endl;
    return mask;
}

Mat CropSomewhatZealously(const Mat& uncropped)
{
    int left = uncropped.cols, top = uncropped.rows, right = -1, bottom = -1;
    for (int y = 0; y < uncropped.rows; y++) {
        Mat row = uncropped.row(y);
        for (int x = 0; x < uncropped.cols; x++) {
            uchar pixel = row.at<uchar>(x);
            if (pixel != 0) {
                if (x < left) left = x;
                if (x > right) right = x;
                if (y < top) top = y;
                if (y > bottom) bottom = y;
            }
        }
    }
    cout << "crop: " << left << ' ' << top << ' ' << bottom << ' ' << right << endl;
    int buffer = 30;
    left = std::max(0, left - buffer);
    right = std::min(uncropped.cols, right + 50);
    top = std::max(0, top - buffer);
    bottom = std::min(uncropped.rows, bottom + 50);
//    Size size(right - left, bottom - top);
//    Point2i origin(left, top);
//    Rect roi(origin, size);
    Range row_range(top, bottom);
    Range col_range(left, right);
    cout << "ranges: " << row_range.start << ".." << row_range.end << ' ' << col_range.start << ".." << col_range.end << " (" << uncropped.rows << " rows, " << uncropped.cols << " cols)" << endl;
    Mat cropped(uncropped, row_range, col_range);
    return cropped;
}

void test1() {
    cout << "eyelid_parabola_ranges_analysis test 1" << endl;
    string src_dir("/home/mike/Pictures/iris/irexiv-validation-package/occlusion-upper");
    string dst_dir("/tmp/uppereyelids");
    char const* filenames[] = {
        "gallery_018.bmp",
        "gallery_019.bmp",
        "gallery_020.bmp",
        "gallery_029.bmp",
        "gallery_030.bmp",
        "gallery_031.bmp",
        "gallery_041.bmp",
        "gallery_042.bmp",
        "gallery_051.bmp",
        "gallery_052.bmp",
        "gallery_053.bmp",
        "gallery_062.bmp",
        "gallery_063.bmp",
        "gallery_073.bmp",
        "gallery_074.bmp",
        "gallery_075.bmp",
        "gallery_084.bmp",
        "gallery_085.bmp",
        "gallery_086.bmp",
        "gallery_095.bmp",
        "gallery_096.bmp",
        "gallery_097.bmp",
        "gallery_106.bmp",
        "gallery_107.bmp",
        "gallery_108.bmp",
        "gallery_117.bmp",
        "gallery_128.bmp",
        "gallery_129.bmp",
        "gallery_130.bmp",
        "gallery_139.bmp",
        "gallery_141.bmp",
        "gallery_151.bmp",
        "gallery_152.bmp",
        "probes_007.bmp",
        "probes_008.bmp",
        "probes_009.bmp",
        "probes_018.bmp",
        "probes_019.bmp",
        "probes_020.bmp"
    };
    
    int num_filenames = sizeof(filenames) / sizeof(const char*);
    cout << num_filenames << " filenames to create masks for" << endl;
    
    for (int i = 0; i < num_filenames; i++) {
        string image_pathname(src_dir);
        image_pathname.append("/");
        image_pathname.append(filenames[i]);
        cout << "creating mask for " << image_pathname << endl;
        Mat eyelid_drawing = cv::imread(image_pathname, CV_LOAD_IMAGE_COLOR);
        if (eyelid_drawing.data == NULL) {
            cerr << "failed to load eyelid image; check source directory " << src_dir << " for file " << filenames[i] << endl;
            break;
        }
        Mat mask = ToMask(eyelid_drawing);
        mask = CropSomewhatZealously(mask);
        string mask_pathname(dst_dir);
        mask_pathname.append("/");
        mask_pathname.append(filenames[i]);
        mask_pathname.append(".png");
        cv::imwrite(mask_pathname, mask);
        cout << mask_pathname << " written" << endl;
    }
    
}

int main(int argc, char** argv) {
    cout << "%SUITE_STARTING% eyelid_parabola_ranges_analysis" << endl;
    cout << "%SUITE_STARTED%" << endl;

    cout << "%TEST_STARTED% test1 (eyelid_parabola_ranges_analysis)" << endl;
    test1();
    cout << "%TEST_FINISHED% time=0 test1 (eyelid_parabola_ranges_analysis)" << endl;


    cout << "%SUITE_FINISHED% time=0" << endl;

    return (EXIT_SUCCESS);
}

