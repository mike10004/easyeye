# easyeye

EasyEye is software for biometric iris image segmentation, encoding, 
and matching.

## Overview

EasyEye is iris image analysis software. It consists of a suite of 
command-line utilities that analyze iris images, encode iris features, 
and match iris encodings. 

The origin of the iris image analysis source code is the VASIR project
(Video-based Automatic System for Iris Recognition) from NIST. The code
is reorganized and refactored to facilitate development.

The portions of the EasyEye source code that were developed by NIST are
in the public domain. There are many authors deserving acknowledgment 
for contributions, and they are identified in comments in the portions 
of the code that they wrote. To list a few:
- Yooyoung Lee (NIST)
- Xiaomei Liu (University of Notre Dame)
- Libor Masek (University of Western Australia)
- Peter Kovesi (University of Western Australia)

New and derived code is copyright (c) 2014 Michael Chaberski, 
distributed under the terms of the MIT License. (See LICENSE file for 
full text.)

## Roadmap

The following big-picture changes are planned:

 - Support greater configurability/extensibility in image processing, 
   segmentation, and encoding (e.g. alternative boundary and eyelid 
   detection techniques, other normalization and encoding strategies)
 - Remove unnecessary legacy code (in optimasek) 
 - Add support for segmenting/encoding eye images at a distance, as 
   from frames of video (the `NIR_FACE_VIDEO` and `NIR_IRIS_VIDEO`
   data types that are supported in VASIR)
 - Provide API and executables for iris image quality analysis
 - Support Windows as a development and deployment platform

## Binaries

On Ubuntu, you can install binaries generated from these modified 
sources by adding the PPA 
https://launchpad.net/~mchaberski/+archive/ppa to your repository 
sources. Follow the PPA link above for more detailed instructions, but 
this is the short version:

    $ sudo add-apt-repository ppa:mchaberski/ppa
    $ sudo apt-get update
    $ sudo apt-get install easyeye

This should also work on Debian, but you may have to align the Ubuntu
series (e.g. saucy) with your Debian version (e.g. sid).
