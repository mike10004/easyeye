/* 
 * File:   testdata.h
 * Author: mchaberski
 *
 * Created on January 28, 2014, 2:40 PM
 */

#ifndef TESTDATA_H
#define	TESTDATA_H

const int NUM_SAMPLES = 9;
static const char* serialized_segmentations[] = {
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
const char* eye_image_files[] = {
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

const char* serialized_encodings[] = {
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



#endif	/* TESTDATA_H */

