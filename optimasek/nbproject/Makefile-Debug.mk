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
CND_CONF=Debug
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/src/adjgamma.o \
	${OBJECTDIR}/src/canny.o \
	${OBJECTDIR}/src/circlecoordinates.o \
	${OBJECTDIR}/src/encode.o \
	${OBJECTDIR}/src/findcircle.o \
	${OBJECTDIR}/src/findline.o \
	${OBJECTDIR}/src/gaborconvolve.o \
	${OBJECTDIR}/src/gauss.o \
	${OBJECTDIR}/src/global.o \
	${OBJECTDIR}/src/houcircle.o \
	${OBJECTDIR}/src/hysthresh.o \
	${OBJECTDIR}/src/interp2.o \
	${OBJECTDIR}/src/linecoords.o \
	${OBJECTDIR}/src/nonmaxsup.o \
	${OBJECTDIR}/src/normalizeiris.o \
	${OBJECTDIR}/src/portability.o \
	${OBJECTDIR}/src/radon.o \
	${OBJECTDIR}/src/segmentiris.o \
	${OBJECTDIR}/src/shiftbits.o


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
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/liboptimasek.a

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/liboptimasek.a: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/liboptimasek.a
	${AR} -rv ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/liboptimasek.a ${OBJECTFILES} 
	$(RANLIB) ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/liboptimasek.a

${OBJECTDIR}/src/adjgamma.o: src/adjgamma.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -Iinclude -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/adjgamma.o src/adjgamma.cpp

${OBJECTDIR}/src/canny.o: src/canny.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -Iinclude -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/canny.o src/canny.cpp

${OBJECTDIR}/src/circlecoordinates.o: src/circlecoordinates.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -Iinclude -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/circlecoordinates.o src/circlecoordinates.cpp

${OBJECTDIR}/src/encode.o: src/encode.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -Iinclude -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/encode.o src/encode.cpp

${OBJECTDIR}/src/findcircle.o: src/findcircle.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -Iinclude -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/findcircle.o src/findcircle.cpp

${OBJECTDIR}/src/findline.o: src/findline.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -Iinclude -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/findline.o src/findline.cpp

${OBJECTDIR}/src/gaborconvolve.o: src/gaborconvolve.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -Iinclude -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/gaborconvolve.o src/gaborconvolve.cpp

${OBJECTDIR}/src/gauss.o: src/gauss.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -Iinclude -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/gauss.o src/gauss.cpp

${OBJECTDIR}/src/global.o: src/global.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -Iinclude -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/global.o src/global.cpp

${OBJECTDIR}/src/houcircle.o: src/houcircle.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -Iinclude -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/houcircle.o src/houcircle.cpp

${OBJECTDIR}/src/hysthresh.o: src/hysthresh.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -Iinclude -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/hysthresh.o src/hysthresh.cpp

${OBJECTDIR}/src/interp2.o: src/interp2.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -Iinclude -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/interp2.o src/interp2.cpp

${OBJECTDIR}/src/linecoords.o: src/linecoords.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -Iinclude -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/linecoords.o src/linecoords.cpp

${OBJECTDIR}/src/nonmaxsup.o: src/nonmaxsup.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -Iinclude -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/nonmaxsup.o src/nonmaxsup.cpp

${OBJECTDIR}/src/normalizeiris.o: src/normalizeiris.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -Iinclude -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/normalizeiris.o src/normalizeiris.cpp

${OBJECTDIR}/src/portability.o: src/portability.cc 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -Iinclude -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/portability.o src/portability.cc

${OBJECTDIR}/src/radon.o: src/radon.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -Iinclude -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/radon.o src/radon.cpp

${OBJECTDIR}/src/segmentiris.o: src/segmentiris.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -Iinclude -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/segmentiris.o src/segmentiris.cpp

${OBJECTDIR}/src/shiftbits.o: src/shiftbits.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -Iinclude -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/shiftbits.o src/shiftbits.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/liboptimasek.a

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
