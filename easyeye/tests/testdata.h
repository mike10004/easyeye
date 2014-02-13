/* 
 * File:   testdata.h
 * Author: mchaberski
 *
 * Created on January 28, 2014, 2:40 PM
 */

#ifndef TESTDATA_H
#define	TESTDATA_H

#include <cassert>

class TestData
{
public:
    static const char* serialized_segmentations[];

    static const char* eye_image_files[];

    static const char* serialized_encodings[];

    static const int NUM_SAMPLES;
private:
    static const int NUM_SERIALIZED_ENCODINGS;
    static const int NUM_SERIALIZED_SEGMENTATIONS;
    TestData(){
        assert(NUM_SAMPLES == NUM_SERIALIZED_ENCODINGS);
        assert(NUM_SAMPLES == NUM_SERIALIZED_SEGMENTATIONS);
    }
    static const TestData instance;
};

const TestData TestData::instance;

const char* TestData::serialized_segmentations[] = {
    "../testdata/segmentations/probes_013-segmentation.json",
    "../testdata/segmentations/gallery_140-segmentation.json",
    "../testdata/segmentations/probes_016-segmentation.json",
    "../testdata/segmentations/gallery_132-segmentation.json",
    "../testdata/segmentations/gallery_022-segmentation.json",
    "../testdata/segmentations/gallery_014-segmentation.json",
    "../testdata/segmentations/gallery_103-segmentation.json",
    "../testdata/segmentations/probes_010-segmentation.json",
    "../testdata/segmentations/probes_006-segmentation.json"
};

const char* TestData::eye_image_files[] = {
    "../testdata/images/probes_013.bmp",
    "../testdata/images/gallery_140.bmp",
    "../testdata/images/probes_016.bmp",
    "../testdata/images/gallery_132.bmp",
    "../testdata/images/gallery_022.bmp",
    "../testdata/images/gallery_014.bmp",
    "../testdata/images/gallery_103.bmp",
    "../testdata/images/probes_010.bmp",
    "../testdata/images/probes_006.bmp"
};

const char* TestData::serialized_encodings[] = {
    "../testdata/encodings/gallery_132-encoding.json",
    "../testdata/encodings/gallery_022-encoding.json",
    "../testdata/encodings/gallery_014-encoding.json",
    "../testdata/encodings/gallery_103-encoding.json",
    "../testdata/encodings/gallery_140-encoding.json",
    "../testdata/encodings/probes_013-encoding.json",
    "../testdata/encodings/probes_016-encoding.json",
    "../testdata/encodings/probes_010-encoding.json",
    "../testdata/encodings/probes_006-encoding.json"
};

const int TestData::NUM_SAMPLES = sizeof(eye_image_files) / sizeof(const char*);
const int TestData::NUM_SERIALIZED_ENCODINGS = sizeof(serialized_encodings) / sizeof(const char*);
const int TestData::NUM_SERIALIZED_SEGMENTATIONS = sizeof(serialized_segmentations) / sizeof(const char*);
const int NUM_SAMPLES = TestData::NUM_SAMPLES;
const char** serialized_segmentations = TestData::serialized_segmentations;
const char** eye_image_files = TestData::eye_image_files;
const char** serialized_encodings = TestData::serialized_encodings;

#endif	/* TESTDATA_H */

