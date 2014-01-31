# easyeye

EasyEye is software for biometric iris image segmentation, encoding, 
and matching. It is at a very early, developmental state.

## Overview

EasyEye is iris image analysis software. It consists of a suite of 
command-line utilities that analyze iris images, encode iris features, 
and match iris encodings. 

The origin of the iris image analysis source code is the VASIR project
(Video-based Automatic System for Iris Recognition) from NIST. The code
has been modified, updated, reorganized and refactored to facilitate 
development.

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

## Does it work?
The short answer is  "I don't know, but maybe." All the pieces are there, and 
the upstream (VASIR) works, in that the segmentation and matching outputs are
rougly within range of what's expected. However, the process of deriving more 
manageable, transparent, and debuggable code from upstream -- that is, turning
VASIR into EasyEye -- could have introduced fundamental errors.

Diagnostic images produced by the software show that the core segmentation
implementation does work, in that it reliably produces reasonably accurate
results on high quality input eye images. Eyelid detection is currently 
pretty hairy, and it's uncertain whether that is a regression from the 
upstream version. Normalization, encoding, and matching have not been tested
for accuracy or reliability. In other words, it's currently possible that 
the matching results are no better than a coin flip, but we know that they
used to be better than a coin flip, and work is underway to get them back
to that performance level.

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

## Releases

Binaries built from the source code are available from a PPA at 
[https://launchpad.net/~mchaberski/+archive/ppa]
([https://launchpad.net/~mchaberski/+archive/ppa). Follow the PPA link for 
detailed instructions. This is the short version:

    $ sudo add-apt-repository ppa:mchaberski/ppa
    $ sudo apt-get update
    $ sudo apt-get install easyeye

This should also work on Debian, but after you add the repository, you may
want to edit the new `.list` file in `/etc/apt/sources.list.d` to align the 
Ubuntu series (e.g. saucy) with your Debian version (e.g. sid).
