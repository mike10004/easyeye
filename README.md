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
- René Nyffenegger (base-64 codec)

New and derived code is copyright (c) 2014 Michael Chaberski, 
distributed under the terms of the MIT License. (See LICENSE file for 
full text.)

## Roadmap

The following changes are planned:

 - Remove unnecessary legacy code (in optimasek) 
 - Support greater configurability/extensibility in image processing and 
   segmentation code
 - Provide API and executables for iris image quality analysis
 - Support building on Windows

