#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Environment
MKDIR=mkdir
CP=cp
GREP=grep
NM=nm
CCADMIN=CCadmin
RANLIB=ranlib
CC=gcc
CCC=g++
CXX=g++
FC=gfortran
AS=as

# Macros
CND_PLATFORM=GNU-Linux-x86
CND_DLIB_EXT=so
CND_CONF=Release
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/src/easyeye/common/base64.o \
	${OBJECTDIR}/src/easyeye/common/easyeye_config.o \
	${OBJECTDIR}/src/easyeye/common/easyeye_diagnostics.o \
	${OBJECTDIR}/src/easyeye/common/easyeye_imaging.o \
	${OBJECTDIR}/src/easyeye/common/easyeye_program.o \
	${OBJECTDIR}/src/easyeye/common/easyeye_serial.o \
	${OBJECTDIR}/src/easyeye/common/easyeye_types.o \
	${OBJECTDIR}/src/easyeye/common/easyeye_utils.o \
	${OBJECTDIR}/src/easyeye/common/mylog.o \
	${OBJECTDIR}/src/easyeye/encode/easyeye_encode.o \
	${OBJECTDIR}/src/easyeye/encode/easyeye_encoding.o \
	${OBJECTDIR}/src/easyeye/encode/easyeye_encoding_serial.o \
	${OBJECTDIR}/src/easyeye/encode/easyeye_normalize.o \
	${OBJECTDIR}/src/easyeye/match/HDCalculator.o \
	${OBJECTDIR}/src/easyeye/match/easyeye_match.o \
	${OBJECTDIR}/src/easyeye/segment/FindEyelidMix.o \
	${OBJECTDIR}/src/easyeye/segment/FindIrisCircle.o \
	${OBJECTDIR}/src/easyeye/segment/FindPupilCircleNew.o \
	${OBJECTDIR}/src/easyeye/segment/ImageUtility.o \
	${OBJECTDIR}/src/easyeye/segment/easyeye_extrema_noise.o \
	${OBJECTDIR}/src/easyeye/segment/easyeye_segment.o

# Test Directory
TESTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}/tests

# Test Files
TESTFILES= \
	${TESTDIR}/TestFiles/f2 \
	${TESTDIR}/TestFiles/f7 \
	${TESTDIR}/TestFiles/f10 \
	${TESTDIR}/TestFiles/f5 \
	${TESTDIR}/TestFiles/f9 \
	${TESTDIR}/TestFiles/f12 \
	${TESTDIR}/TestFiles/f11 \
	${TESTDIR}/TestFiles/f4 \
	${TESTDIR}/TestFiles/f1 \
	${TESTDIR}/TestFiles/f3 \
	${TESTDIR}/TestFiles/f6 \
	${TESTDIR}/TestFiles/f8

# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=
CXXFLAGS=

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libeasyeye.a

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libeasyeye.a: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libeasyeye.a
	${AR} -rv ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libeasyeye.a ${OBJECTFILES} 
	$(RANLIB) ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libeasyeye.a

${OBJECTDIR}/src/easyeye/common/base64.o: src/easyeye/common/base64.cc 
	${MKDIR} -p ${OBJECTDIR}/src/easyeye/common
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../optimasek/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/easyeye/common/base64.o src/easyeye/common/base64.cc

${OBJECTDIR}/src/easyeye/common/easyeye_config.o: src/easyeye/common/easyeye_config.cc 
	${MKDIR} -p ${OBJECTDIR}/src/easyeye/common
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../optimasek/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/easyeye/common/easyeye_config.o src/easyeye/common/easyeye_config.cc

${OBJECTDIR}/src/easyeye/common/easyeye_diagnostics.o: src/easyeye/common/easyeye_diagnostics.cc 
	${MKDIR} -p ${OBJECTDIR}/src/easyeye/common
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../optimasek/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/easyeye/common/easyeye_diagnostics.o src/easyeye/common/easyeye_diagnostics.cc

${OBJECTDIR}/src/easyeye/common/easyeye_imaging.o: src/easyeye/common/easyeye_imaging.cc 
	${MKDIR} -p ${OBJECTDIR}/src/easyeye/common
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../optimasek/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/easyeye/common/easyeye_imaging.o src/easyeye/common/easyeye_imaging.cc

${OBJECTDIR}/src/easyeye/common/easyeye_program.o: src/easyeye/common/easyeye_program.cc 
	${MKDIR} -p ${OBJECTDIR}/src/easyeye/common
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../optimasek/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/easyeye/common/easyeye_program.o src/easyeye/common/easyeye_program.cc

${OBJECTDIR}/src/easyeye/common/easyeye_serial.o: src/easyeye/common/easyeye_serial.cc 
	${MKDIR} -p ${OBJECTDIR}/src/easyeye/common
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../optimasek/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/easyeye/common/easyeye_serial.o src/easyeye/common/easyeye_serial.cc

${OBJECTDIR}/src/easyeye/common/easyeye_types.o: src/easyeye/common/easyeye_types.cc 
	${MKDIR} -p ${OBJECTDIR}/src/easyeye/common
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../optimasek/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/easyeye/common/easyeye_types.o src/easyeye/common/easyeye_types.cc

${OBJECTDIR}/src/easyeye/common/easyeye_utils.o: src/easyeye/common/easyeye_utils.cc 
	${MKDIR} -p ${OBJECTDIR}/src/easyeye/common
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../optimasek/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/easyeye/common/easyeye_utils.o src/easyeye/common/easyeye_utils.cc

${OBJECTDIR}/src/easyeye/common/mylog.o: src/easyeye/common/mylog.cc 
	${MKDIR} -p ${OBJECTDIR}/src/easyeye/common
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../optimasek/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/easyeye/common/mylog.o src/easyeye/common/mylog.cc

${OBJECTDIR}/src/easyeye/encode/easyeye_encode.o: src/easyeye/encode/easyeye_encode.cc 
	${MKDIR} -p ${OBJECTDIR}/src/easyeye/encode
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../optimasek/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/easyeye/encode/easyeye_encode.o src/easyeye/encode/easyeye_encode.cc

${OBJECTDIR}/src/easyeye/encode/easyeye_encoding.o: src/easyeye/encode/easyeye_encoding.cc 
	${MKDIR} -p ${OBJECTDIR}/src/easyeye/encode
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../optimasek/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/easyeye/encode/easyeye_encoding.o src/easyeye/encode/easyeye_encoding.cc

${OBJECTDIR}/src/easyeye/encode/easyeye_encoding_serial.o: src/easyeye/encode/easyeye_encoding_serial.cc 
	${MKDIR} -p ${OBJECTDIR}/src/easyeye/encode
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../optimasek/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/easyeye/encode/easyeye_encoding_serial.o src/easyeye/encode/easyeye_encoding_serial.cc

${OBJECTDIR}/src/easyeye/encode/easyeye_normalize.o: src/easyeye/encode/easyeye_normalize.cc 
	${MKDIR} -p ${OBJECTDIR}/src/easyeye/encode
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../optimasek/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/easyeye/encode/easyeye_normalize.o src/easyeye/encode/easyeye_normalize.cc

${OBJECTDIR}/src/easyeye/match/HDCalculator.o: src/easyeye/match/HDCalculator.cc 
	${MKDIR} -p ${OBJECTDIR}/src/easyeye/match
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../optimasek/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/easyeye/match/HDCalculator.o src/easyeye/match/HDCalculator.cc

${OBJECTDIR}/src/easyeye/match/easyeye_match.o: src/easyeye/match/easyeye_match.cc 
	${MKDIR} -p ${OBJECTDIR}/src/easyeye/match
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../optimasek/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/easyeye/match/easyeye_match.o src/easyeye/match/easyeye_match.cc

${OBJECTDIR}/src/easyeye/segment/FindEyelidMix.o: src/easyeye/segment/FindEyelidMix.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/easyeye/segment
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../optimasek/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/easyeye/segment/FindEyelidMix.o src/easyeye/segment/FindEyelidMix.cpp

${OBJECTDIR}/src/easyeye/segment/FindIrisCircle.o: src/easyeye/segment/FindIrisCircle.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/easyeye/segment
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../optimasek/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/easyeye/segment/FindIrisCircle.o src/easyeye/segment/FindIrisCircle.cpp

${OBJECTDIR}/src/easyeye/segment/FindPupilCircleNew.o: src/easyeye/segment/FindPupilCircleNew.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/easyeye/segment
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../optimasek/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/easyeye/segment/FindPupilCircleNew.o src/easyeye/segment/FindPupilCircleNew.cpp

${OBJECTDIR}/src/easyeye/segment/ImageUtility.o: src/easyeye/segment/ImageUtility.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/easyeye/segment
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../optimasek/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/easyeye/segment/ImageUtility.o src/easyeye/segment/ImageUtility.cpp

${OBJECTDIR}/src/easyeye/segment/easyeye_extrema_noise.o: src/easyeye/segment/easyeye_extrema_noise.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/easyeye/segment
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../optimasek/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/easyeye/segment/easyeye_extrema_noise.o src/easyeye/segment/easyeye_extrema_noise.cpp

${OBJECTDIR}/src/easyeye/segment/easyeye_segment.o: src/easyeye/segment/easyeye_segment.cc 
	${MKDIR} -p ${OBJECTDIR}/src/easyeye/segment
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../optimasek/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/easyeye/segment/easyeye_segment.o src/easyeye/segment/easyeye_segment.cc

# Subprojects
.build-subprojects:

# Build Test Targets
.build-tests-conf: .build-conf ${TESTFILES}
${TESTDIR}/TestFiles/f2: ${TESTDIR}/tests/base64_test.o ${OBJECTFILES:%.o=%_nomain.o}
	${MKDIR} -p ${TESTDIR}/TestFiles
	${LINK.cc}   -o ${TESTDIR}/TestFiles/f2 $^ ${LDLIBSOPTIONS} ../optimasek/dist/Release/GNU-Linux-x86/liboptimasek.a -lopencv_core -lopencv_highgui -lopencv_imgproc -lopencv_objdetect -ljsoncpp -luuid 

${TESTDIR}/TestFiles/f7: ${TESTDIR}/tests/BitPackingTest.o ${TESTDIR}/tests/bitpacking_test_runner.o ${OBJECTFILES:%.o=%_nomain.o}
	${MKDIR} -p ${TESTDIR}/TestFiles
	${LINK.cc}   -o ${TESTDIR}/TestFiles/f7 $^ ${LDLIBSOPTIONS} ../optimasek/dist/Release/GNU-Linux-x86/liboptimasek.a -lopencv_core -lopencv_highgui -lopencv_imgproc -lopencv_objdetect -ljsoncpp -luuid `cppunit-config --libs`   

${TESTDIR}/TestFiles/f10: ${TESTDIR}/tests/CommonSerialTest.o ${TESTDIR}/tests/common_serial_test_runner.o ${OBJECTFILES:%.o=%_nomain.o}
	${MKDIR} -p ${TESTDIR}/TestFiles
	${LINK.cc}   -o ${TESTDIR}/TestFiles/f10 $^ ${LDLIBSOPTIONS} ../optimasek/dist/Release/GNU-Linux-x86/liboptimasek.a -lopencv_core -lopencv_highgui -lopencv_imgproc -lopencv_objdetect -ljsoncpp -luuid `cppunit-config --libs`   

${TESTDIR}/TestFiles/f5: ${TESTDIR}/tests/DiagnosticsTest.o ${TESTDIR}/tests/diagnostics_test_runner.o ${OBJECTFILES:%.o=%_nomain.o}
	${MKDIR} -p ${TESTDIR}/TestFiles
	${LINK.cc}   -o ${TESTDIR}/TestFiles/f5 $^ ${LDLIBSOPTIONS} ../optimasek/dist/Release/GNU-Linux-x86/liboptimasek.a -lopencv_core -lopencv_highgui -lopencv_imgproc -lopencv_objdetect -ljsoncpp -luuid `cppunit-config --libs`   

${TESTDIR}/TestFiles/f9: ${TESTDIR}/tests/EncodingTest.o ${TESTDIR}/tests/encoding_test_runner.o ${OBJECTFILES:%.o=%_nomain.o}
	${MKDIR} -p ${TESTDIR}/TestFiles
	${LINK.cc}   -o ${TESTDIR}/TestFiles/f9 $^ ${LDLIBSOPTIONS} ../optimasek/dist/Release/GNU-Linux-x86/liboptimasek.a -lopencv_core -lopencv_highgui -lopencv_imgproc -lopencv_objdetect -ljsoncpp -luuid `cppunit-config --libs`   

${TESTDIR}/TestFiles/f12: ${TESTDIR}/tests/FilesTest.o ${TESTDIR}/tests/files_test_runner.o ${OBJECTFILES:%.o=%_nomain.o}
	${MKDIR} -p ${TESTDIR}/TestFiles
	${LINK.cc}   -o ${TESTDIR}/TestFiles/f12 $^ ${LDLIBSOPTIONS} ../optimasek/dist/Release/GNU-Linux-x86/liboptimasek.a -lopencv_core -lopencv_highgui -lopencv_imgproc -lopencv_objdetect -ljsoncpp -luuid `cppunit-config --libs`   

${TESTDIR}/TestFiles/f11: ${TESTDIR}/tests/FindEyelidsTest.o ${TESTDIR}/tests/find_eyelids_test_runner.o ${OBJECTFILES:%.o=%_nomain.o}
	${MKDIR} -p ${TESTDIR}/TestFiles
	${LINK.cc}   -o ${TESTDIR}/TestFiles/f11 $^ ${LDLIBSOPTIONS} ../optimasek/dist/Release/GNU-Linux-x86/liboptimasek.a -lopencv_core -lopencv_highgui -lopencv_imgproc -lopencv_objdetect -ljsoncpp -luuid `cppunit-config --libs`   

${TESTDIR}/TestFiles/f4: ${TESTDIR}/tests/ImagingTest.o ${TESTDIR}/tests/imaging_test_runner.o ${OBJECTFILES:%.o=%_nomain.o}
	${MKDIR} -p ${TESTDIR}/TestFiles
	${LINK.cc}   -o ${TESTDIR}/TestFiles/f4 $^ ${LDLIBSOPTIONS} ../optimasek/dist/Release/GNU-Linux-x86/liboptimasek.a -lopencv_core -lopencv_highgui -lopencv_imgproc -lopencv_objdetect -ljsoncpp -luuid `cppunit-config --libs`   

${TESTDIR}/TestFiles/f1: ${TESTDIR}/tests/MatchingTest.o ${TESTDIR}/tests/matching_test_runner.o ${OBJECTFILES:%.o=%_nomain.o}
	${MKDIR} -p ${TESTDIR}/TestFiles
	${LINK.cc}   -o ${TESTDIR}/TestFiles/f1 $^ ${LDLIBSOPTIONS} ../optimasek/dist/Release/GNU-Linux-x86/liboptimasek.a -lopencv_core -lopencv_highgui -lopencv_imgproc -lopencv_objdetect -ljsoncpp -luuid `cppunit-config --libs`   

${TESTDIR}/TestFiles/f3: ${TESTDIR}/tests/SegmentationSerializerTest.o ${TESTDIR}/tests/segment_serial_test_runner.o ${OBJECTFILES:%.o=%_nomain.o}
	${MKDIR} -p ${TESTDIR}/TestFiles
	${LINK.cc}   -o ${TESTDIR}/TestFiles/f3 $^ ${LDLIBSOPTIONS} ../optimasek/dist/Release/GNU-Linux-x86/liboptimasek.a -lopencv_core -lopencv_highgui -lopencv_imgproc -lopencv_objdetect -ljsoncpp -luuid `cppunit-config --libs`   

${TESTDIR}/TestFiles/f6: ${TESTDIR}/tests/segmentation_correctness_test.o ${OBJECTFILES:%.o=%_nomain.o}
	${MKDIR} -p ${TESTDIR}/TestFiles
	${LINK.cc}   -o ${TESTDIR}/TestFiles/f6 $^ ${LDLIBSOPTIONS} ../optimasek/dist/Release/GNU-Linux-x86/liboptimasek.a -lopencv_core -lopencv_highgui -lopencv_imgproc -lopencv_objdetect -ljsoncpp -luuid 

${TESTDIR}/TestFiles/f8: ${TESTDIR}/tests/utils_test.o ${OBJECTFILES:%.o=%_nomain.o}
	${MKDIR} -p ${TESTDIR}/TestFiles
	${LINK.cc}   -o ${TESTDIR}/TestFiles/f8 $^ ${LDLIBSOPTIONS} ../optimasek/dist/Release/GNU-Linux-x86/liboptimasek.a -lopencv_core -lopencv_highgui -lopencv_imgproc -lopencv_objdetect -ljsoncpp -luuid 


${TESTDIR}/tests/base64_test.o: tests/base64_test.cc 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../optimasek/include -I. -I. -MMD -MP -MF "$@.d" -o ${TESTDIR}/tests/base64_test.o tests/base64_test.cc


${TESTDIR}/tests/BitPackingTest.o: tests/BitPackingTest.cc 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../optimasek/include `cppunit-config --cflags` -MMD -MP -MF "$@.d" -o ${TESTDIR}/tests/BitPackingTest.o tests/BitPackingTest.cc


${TESTDIR}/tests/bitpacking_test_runner.o: tests/bitpacking_test_runner.cc 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../optimasek/include `cppunit-config --cflags` -MMD -MP -MF "$@.d" -o ${TESTDIR}/tests/bitpacking_test_runner.o tests/bitpacking_test_runner.cc


${TESTDIR}/tests/CommonSerialTest.o: tests/CommonSerialTest.cc 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../optimasek/include `cppunit-config --cflags` -MMD -MP -MF "$@.d" -o ${TESTDIR}/tests/CommonSerialTest.o tests/CommonSerialTest.cc


${TESTDIR}/tests/common_serial_test_runner.o: tests/common_serial_test_runner.cc 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../optimasek/include `cppunit-config --cflags` -MMD -MP -MF "$@.d" -o ${TESTDIR}/tests/common_serial_test_runner.o tests/common_serial_test_runner.cc


${TESTDIR}/tests/DiagnosticsTest.o: tests/DiagnosticsTest.cpp 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../optimasek/include `cppunit-config --cflags` -MMD -MP -MF "$@.d" -o ${TESTDIR}/tests/DiagnosticsTest.o tests/DiagnosticsTest.cpp


${TESTDIR}/tests/diagnostics_test_runner.o: tests/diagnostics_test_runner.cpp 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../optimasek/include `cppunit-config --cflags` -MMD -MP -MF "$@.d" -o ${TESTDIR}/tests/diagnostics_test_runner.o tests/diagnostics_test_runner.cpp


${TESTDIR}/tests/EncodingTest.o: tests/EncodingTest.cc 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../optimasek/include `cppunit-config --cflags` -MMD -MP -MF "$@.d" -o ${TESTDIR}/tests/EncodingTest.o tests/EncodingTest.cc


${TESTDIR}/tests/encoding_test_runner.o: tests/encoding_test_runner.cc 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../optimasek/include `cppunit-config --cflags` -MMD -MP -MF "$@.d" -o ${TESTDIR}/tests/encoding_test_runner.o tests/encoding_test_runner.cc


${TESTDIR}/tests/FilesTest.o: tests/FilesTest.cc 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../optimasek/include `cppunit-config --cflags` -MMD -MP -MF "$@.d" -o ${TESTDIR}/tests/FilesTest.o tests/FilesTest.cc


${TESTDIR}/tests/files_test_runner.o: tests/files_test_runner.cc 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../optimasek/include `cppunit-config --cflags` -MMD -MP -MF "$@.d" -o ${TESTDIR}/tests/files_test_runner.o tests/files_test_runner.cc


${TESTDIR}/tests/FindEyelidsTest.o: tests/FindEyelidsTest.cc 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../optimasek/include `cppunit-config --cflags` -MMD -MP -MF "$@.d" -o ${TESTDIR}/tests/FindEyelidsTest.o tests/FindEyelidsTest.cc


${TESTDIR}/tests/find_eyelids_test_runner.o: tests/find_eyelids_test_runner.cc 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../optimasek/include `cppunit-config --cflags` -MMD -MP -MF "$@.d" -o ${TESTDIR}/tests/find_eyelids_test_runner.o tests/find_eyelids_test_runner.cc


${TESTDIR}/tests/ImagingTest.o: tests/ImagingTest.cc 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../optimasek/include `cppunit-config --cflags` -MMD -MP -MF "$@.d" -o ${TESTDIR}/tests/ImagingTest.o tests/ImagingTest.cc


${TESTDIR}/tests/imaging_test_runner.o: tests/imaging_test_runner.cc 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../optimasek/include `cppunit-config --cflags` -MMD -MP -MF "$@.d" -o ${TESTDIR}/tests/imaging_test_runner.o tests/imaging_test_runner.cc


${TESTDIR}/tests/MatchingTest.o: tests/MatchingTest.cc 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../optimasek/include -I. -I. `cppunit-config --cflags` -MMD -MP -MF "$@.d" -o ${TESTDIR}/tests/MatchingTest.o tests/MatchingTest.cc


${TESTDIR}/tests/matching_test_runner.o: tests/matching_test_runner.cc 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../optimasek/include -I. -I. `cppunit-config --cflags` -MMD -MP -MF "$@.d" -o ${TESTDIR}/tests/matching_test_runner.o tests/matching_test_runner.cc


${TESTDIR}/tests/SegmentationSerializerTest.o: tests/SegmentationSerializerTest.cc 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../optimasek/include -I. `cppunit-config --cflags` -MMD -MP -MF "$@.d" -o ${TESTDIR}/tests/SegmentationSerializerTest.o tests/SegmentationSerializerTest.cc


${TESTDIR}/tests/segment_serial_test_runner.o: tests/segment_serial_test_runner.cc 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../optimasek/include -I. `cppunit-config --cflags` -MMD -MP -MF "$@.d" -o ${TESTDIR}/tests/segment_serial_test_runner.o tests/segment_serial_test_runner.cc


${TESTDIR}/tests/segmentation_correctness_test.o: tests/segmentation_correctness_test.cc 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../optimasek/include -I. -MMD -MP -MF "$@.d" -o ${TESTDIR}/tests/segmentation_correctness_test.o tests/segmentation_correctness_test.cc


${TESTDIR}/tests/utils_test.o: tests/utils_test.cc 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../optimasek/include -I. -MMD -MP -MF "$@.d" -o ${TESTDIR}/tests/utils_test.o tests/utils_test.cc


${OBJECTDIR}/src/easyeye/common/base64_nomain.o: ${OBJECTDIR}/src/easyeye/common/base64.o src/easyeye/common/base64.cc 
	${MKDIR} -p ${OBJECTDIR}/src/easyeye/common
	@NMOUTPUT=`${NM} ${OBJECTDIR}/src/easyeye/common/base64.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -O2 -I../optimasek/include -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/easyeye/common/base64_nomain.o src/easyeye/common/base64.cc;\
	else  \
	    ${CP} ${OBJECTDIR}/src/easyeye/common/base64.o ${OBJECTDIR}/src/easyeye/common/base64_nomain.o;\
	fi

${OBJECTDIR}/src/easyeye/common/easyeye_config_nomain.o: ${OBJECTDIR}/src/easyeye/common/easyeye_config.o src/easyeye/common/easyeye_config.cc 
	${MKDIR} -p ${OBJECTDIR}/src/easyeye/common
	@NMOUTPUT=`${NM} ${OBJECTDIR}/src/easyeye/common/easyeye_config.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -O2 -I../optimasek/include -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/easyeye/common/easyeye_config_nomain.o src/easyeye/common/easyeye_config.cc;\
	else  \
	    ${CP} ${OBJECTDIR}/src/easyeye/common/easyeye_config.o ${OBJECTDIR}/src/easyeye/common/easyeye_config_nomain.o;\
	fi

${OBJECTDIR}/src/easyeye/common/easyeye_diagnostics_nomain.o: ${OBJECTDIR}/src/easyeye/common/easyeye_diagnostics.o src/easyeye/common/easyeye_diagnostics.cc 
	${MKDIR} -p ${OBJECTDIR}/src/easyeye/common
	@NMOUTPUT=`${NM} ${OBJECTDIR}/src/easyeye/common/easyeye_diagnostics.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -O2 -I../optimasek/include -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/easyeye/common/easyeye_diagnostics_nomain.o src/easyeye/common/easyeye_diagnostics.cc;\
	else  \
	    ${CP} ${OBJECTDIR}/src/easyeye/common/easyeye_diagnostics.o ${OBJECTDIR}/src/easyeye/common/easyeye_diagnostics_nomain.o;\
	fi

${OBJECTDIR}/src/easyeye/common/easyeye_imaging_nomain.o: ${OBJECTDIR}/src/easyeye/common/easyeye_imaging.o src/easyeye/common/easyeye_imaging.cc 
	${MKDIR} -p ${OBJECTDIR}/src/easyeye/common
	@NMOUTPUT=`${NM} ${OBJECTDIR}/src/easyeye/common/easyeye_imaging.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -O2 -I../optimasek/include -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/easyeye/common/easyeye_imaging_nomain.o src/easyeye/common/easyeye_imaging.cc;\
	else  \
	    ${CP} ${OBJECTDIR}/src/easyeye/common/easyeye_imaging.o ${OBJECTDIR}/src/easyeye/common/easyeye_imaging_nomain.o;\
	fi

${OBJECTDIR}/src/easyeye/common/easyeye_program_nomain.o: ${OBJECTDIR}/src/easyeye/common/easyeye_program.o src/easyeye/common/easyeye_program.cc 
	${MKDIR} -p ${OBJECTDIR}/src/easyeye/common
	@NMOUTPUT=`${NM} ${OBJECTDIR}/src/easyeye/common/easyeye_program.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -O2 -I../optimasek/include -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/easyeye/common/easyeye_program_nomain.o src/easyeye/common/easyeye_program.cc;\
	else  \
	    ${CP} ${OBJECTDIR}/src/easyeye/common/easyeye_program.o ${OBJECTDIR}/src/easyeye/common/easyeye_program_nomain.o;\
	fi

${OBJECTDIR}/src/easyeye/common/easyeye_serial_nomain.o: ${OBJECTDIR}/src/easyeye/common/easyeye_serial.o src/easyeye/common/easyeye_serial.cc 
	${MKDIR} -p ${OBJECTDIR}/src/easyeye/common
	@NMOUTPUT=`${NM} ${OBJECTDIR}/src/easyeye/common/easyeye_serial.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -O2 -I../optimasek/include -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/easyeye/common/easyeye_serial_nomain.o src/easyeye/common/easyeye_serial.cc;\
	else  \
	    ${CP} ${OBJECTDIR}/src/easyeye/common/easyeye_serial.o ${OBJECTDIR}/src/easyeye/common/easyeye_serial_nomain.o;\
	fi

${OBJECTDIR}/src/easyeye/common/easyeye_types_nomain.o: ${OBJECTDIR}/src/easyeye/common/easyeye_types.o src/easyeye/common/easyeye_types.cc 
	${MKDIR} -p ${OBJECTDIR}/src/easyeye/common
	@NMOUTPUT=`${NM} ${OBJECTDIR}/src/easyeye/common/easyeye_types.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -O2 -I../optimasek/include -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/easyeye/common/easyeye_types_nomain.o src/easyeye/common/easyeye_types.cc;\
	else  \
	    ${CP} ${OBJECTDIR}/src/easyeye/common/easyeye_types.o ${OBJECTDIR}/src/easyeye/common/easyeye_types_nomain.o;\
	fi

${OBJECTDIR}/src/easyeye/common/easyeye_utils_nomain.o: ${OBJECTDIR}/src/easyeye/common/easyeye_utils.o src/easyeye/common/easyeye_utils.cc 
	${MKDIR} -p ${OBJECTDIR}/src/easyeye/common
	@NMOUTPUT=`${NM} ${OBJECTDIR}/src/easyeye/common/easyeye_utils.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -O2 -I../optimasek/include -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/easyeye/common/easyeye_utils_nomain.o src/easyeye/common/easyeye_utils.cc;\
	else  \
	    ${CP} ${OBJECTDIR}/src/easyeye/common/easyeye_utils.o ${OBJECTDIR}/src/easyeye/common/easyeye_utils_nomain.o;\
	fi

${OBJECTDIR}/src/easyeye/common/mylog_nomain.o: ${OBJECTDIR}/src/easyeye/common/mylog.o src/easyeye/common/mylog.cc 
	${MKDIR} -p ${OBJECTDIR}/src/easyeye/common
	@NMOUTPUT=`${NM} ${OBJECTDIR}/src/easyeye/common/mylog.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -O2 -I../optimasek/include -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/easyeye/common/mylog_nomain.o src/easyeye/common/mylog.cc;\
	else  \
	    ${CP} ${OBJECTDIR}/src/easyeye/common/mylog.o ${OBJECTDIR}/src/easyeye/common/mylog_nomain.o;\
	fi

${OBJECTDIR}/src/easyeye/encode/easyeye_encode_nomain.o: ${OBJECTDIR}/src/easyeye/encode/easyeye_encode.o src/easyeye/encode/easyeye_encode.cc 
	${MKDIR} -p ${OBJECTDIR}/src/easyeye/encode
	@NMOUTPUT=`${NM} ${OBJECTDIR}/src/easyeye/encode/easyeye_encode.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -O2 -I../optimasek/include -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/easyeye/encode/easyeye_encode_nomain.o src/easyeye/encode/easyeye_encode.cc;\
	else  \
	    ${CP} ${OBJECTDIR}/src/easyeye/encode/easyeye_encode.o ${OBJECTDIR}/src/easyeye/encode/easyeye_encode_nomain.o;\
	fi

${OBJECTDIR}/src/easyeye/encode/easyeye_encoding_nomain.o: ${OBJECTDIR}/src/easyeye/encode/easyeye_encoding.o src/easyeye/encode/easyeye_encoding.cc 
	${MKDIR} -p ${OBJECTDIR}/src/easyeye/encode
	@NMOUTPUT=`${NM} ${OBJECTDIR}/src/easyeye/encode/easyeye_encoding.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -O2 -I../optimasek/include -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/easyeye/encode/easyeye_encoding_nomain.o src/easyeye/encode/easyeye_encoding.cc;\
	else  \
	    ${CP} ${OBJECTDIR}/src/easyeye/encode/easyeye_encoding.o ${OBJECTDIR}/src/easyeye/encode/easyeye_encoding_nomain.o;\
	fi

${OBJECTDIR}/src/easyeye/encode/easyeye_encoding_serial_nomain.o: ${OBJECTDIR}/src/easyeye/encode/easyeye_encoding_serial.o src/easyeye/encode/easyeye_encoding_serial.cc 
	${MKDIR} -p ${OBJECTDIR}/src/easyeye/encode
	@NMOUTPUT=`${NM} ${OBJECTDIR}/src/easyeye/encode/easyeye_encoding_serial.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -O2 -I../optimasek/include -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/easyeye/encode/easyeye_encoding_serial_nomain.o src/easyeye/encode/easyeye_encoding_serial.cc;\
	else  \
	    ${CP} ${OBJECTDIR}/src/easyeye/encode/easyeye_encoding_serial.o ${OBJECTDIR}/src/easyeye/encode/easyeye_encoding_serial_nomain.o;\
	fi

${OBJECTDIR}/src/easyeye/encode/easyeye_normalize_nomain.o: ${OBJECTDIR}/src/easyeye/encode/easyeye_normalize.o src/easyeye/encode/easyeye_normalize.cc 
	${MKDIR} -p ${OBJECTDIR}/src/easyeye/encode
	@NMOUTPUT=`${NM} ${OBJECTDIR}/src/easyeye/encode/easyeye_normalize.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -O2 -I../optimasek/include -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/easyeye/encode/easyeye_normalize_nomain.o src/easyeye/encode/easyeye_normalize.cc;\
	else  \
	    ${CP} ${OBJECTDIR}/src/easyeye/encode/easyeye_normalize.o ${OBJECTDIR}/src/easyeye/encode/easyeye_normalize_nomain.o;\
	fi

${OBJECTDIR}/src/easyeye/match/HDCalculator_nomain.o: ${OBJECTDIR}/src/easyeye/match/HDCalculator.o src/easyeye/match/HDCalculator.cc 
	${MKDIR} -p ${OBJECTDIR}/src/easyeye/match
	@NMOUTPUT=`${NM} ${OBJECTDIR}/src/easyeye/match/HDCalculator.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -O2 -I../optimasek/include -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/easyeye/match/HDCalculator_nomain.o src/easyeye/match/HDCalculator.cc;\
	else  \
	    ${CP} ${OBJECTDIR}/src/easyeye/match/HDCalculator.o ${OBJECTDIR}/src/easyeye/match/HDCalculator_nomain.o;\
	fi

${OBJECTDIR}/src/easyeye/match/easyeye_match_nomain.o: ${OBJECTDIR}/src/easyeye/match/easyeye_match.o src/easyeye/match/easyeye_match.cc 
	${MKDIR} -p ${OBJECTDIR}/src/easyeye/match
	@NMOUTPUT=`${NM} ${OBJECTDIR}/src/easyeye/match/easyeye_match.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -O2 -I../optimasek/include -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/easyeye/match/easyeye_match_nomain.o src/easyeye/match/easyeye_match.cc;\
	else  \
	    ${CP} ${OBJECTDIR}/src/easyeye/match/easyeye_match.o ${OBJECTDIR}/src/easyeye/match/easyeye_match_nomain.o;\
	fi

${OBJECTDIR}/src/easyeye/segment/FindEyelidMix_nomain.o: ${OBJECTDIR}/src/easyeye/segment/FindEyelidMix.o src/easyeye/segment/FindEyelidMix.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/easyeye/segment
	@NMOUTPUT=`${NM} ${OBJECTDIR}/src/easyeye/segment/FindEyelidMix.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -O2 -I../optimasek/include -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/easyeye/segment/FindEyelidMix_nomain.o src/easyeye/segment/FindEyelidMix.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/src/easyeye/segment/FindEyelidMix.o ${OBJECTDIR}/src/easyeye/segment/FindEyelidMix_nomain.o;\
	fi

${OBJECTDIR}/src/easyeye/segment/FindIrisCircle_nomain.o: ${OBJECTDIR}/src/easyeye/segment/FindIrisCircle.o src/easyeye/segment/FindIrisCircle.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/easyeye/segment
	@NMOUTPUT=`${NM} ${OBJECTDIR}/src/easyeye/segment/FindIrisCircle.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -O2 -I../optimasek/include -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/easyeye/segment/FindIrisCircle_nomain.o src/easyeye/segment/FindIrisCircle.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/src/easyeye/segment/FindIrisCircle.o ${OBJECTDIR}/src/easyeye/segment/FindIrisCircle_nomain.o;\
	fi

${OBJECTDIR}/src/easyeye/segment/FindPupilCircleNew_nomain.o: ${OBJECTDIR}/src/easyeye/segment/FindPupilCircleNew.o src/easyeye/segment/FindPupilCircleNew.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/easyeye/segment
	@NMOUTPUT=`${NM} ${OBJECTDIR}/src/easyeye/segment/FindPupilCircleNew.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -O2 -I../optimasek/include -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/easyeye/segment/FindPupilCircleNew_nomain.o src/easyeye/segment/FindPupilCircleNew.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/src/easyeye/segment/FindPupilCircleNew.o ${OBJECTDIR}/src/easyeye/segment/FindPupilCircleNew_nomain.o;\
	fi

${OBJECTDIR}/src/easyeye/segment/ImageUtility_nomain.o: ${OBJECTDIR}/src/easyeye/segment/ImageUtility.o src/easyeye/segment/ImageUtility.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/easyeye/segment
	@NMOUTPUT=`${NM} ${OBJECTDIR}/src/easyeye/segment/ImageUtility.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -O2 -I../optimasek/include -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/easyeye/segment/ImageUtility_nomain.o src/easyeye/segment/ImageUtility.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/src/easyeye/segment/ImageUtility.o ${OBJECTDIR}/src/easyeye/segment/ImageUtility_nomain.o;\
	fi

${OBJECTDIR}/src/easyeye/segment/easyeye_extrema_noise_nomain.o: ${OBJECTDIR}/src/easyeye/segment/easyeye_extrema_noise.o src/easyeye/segment/easyeye_extrema_noise.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/easyeye/segment
	@NMOUTPUT=`${NM} ${OBJECTDIR}/src/easyeye/segment/easyeye_extrema_noise.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -O2 -I../optimasek/include -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/easyeye/segment/easyeye_extrema_noise_nomain.o src/easyeye/segment/easyeye_extrema_noise.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/src/easyeye/segment/easyeye_extrema_noise.o ${OBJECTDIR}/src/easyeye/segment/easyeye_extrema_noise_nomain.o;\
	fi

${OBJECTDIR}/src/easyeye/segment/easyeye_segment_nomain.o: ${OBJECTDIR}/src/easyeye/segment/easyeye_segment.o src/easyeye/segment/easyeye_segment.cc 
	${MKDIR} -p ${OBJECTDIR}/src/easyeye/segment
	@NMOUTPUT=`${NM} ${OBJECTDIR}/src/easyeye/segment/easyeye_segment.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -O2 -I../optimasek/include -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/easyeye/segment/easyeye_segment_nomain.o src/easyeye/segment/easyeye_segment.cc;\
	else  \
	    ${CP} ${OBJECTDIR}/src/easyeye/segment/easyeye_segment.o ${OBJECTDIR}/src/easyeye/segment/easyeye_segment_nomain.o;\
	fi

# Run Test Targets
.test-conf:
	@if [ "${TEST}" = "" ]; \
	then  \
	    ${TESTDIR}/TestFiles/f2 || true; \
	    ${TESTDIR}/TestFiles/f7 || true; \
	    ${TESTDIR}/TestFiles/f10 || true; \
	    ${TESTDIR}/TestFiles/f5 || true; \
	    ${TESTDIR}/TestFiles/f9 || true; \
	    ${TESTDIR}/TestFiles/f12 || true; \
	    ${TESTDIR}/TestFiles/f11 || true; \
	    ${TESTDIR}/TestFiles/f4 || true; \
	    ${TESTDIR}/TestFiles/f1 || true; \
	    ${TESTDIR}/TestFiles/f3 || true; \
	    ${TESTDIR}/TestFiles/f6 || true; \
	    ${TESTDIR}/TestFiles/f8 || true; \
	else  \
	    ./${TEST} || true; \
	fi

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libeasyeye.a

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
