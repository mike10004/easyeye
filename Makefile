#!/usr/bin/make

DEB_HOST_MULTIARCH ?= $(shell dpkg-architecture -qDEB_HOST_MULTIARCH)
OPTIMASEK_PROJ_ROOT := optimasek
EASYEYE_PROJ_ROOT := $(CURDIR)/easyeye
EXTRACT_PROJ_ROOT := $(CURDIR)/easyeye-extract
COMPARE_PROJ_ROOT := $(CURDIR)/easyeye-compare
STAGEDIR ?= stage
DESTDIR ?= $(STAGEDIR)/install
LIBDIR := $(DESTDIR)/usr/lib/$(DEB_HOST_MULTIARCH)
BINDIR := $(DESTDIR)/usr/bin
MANDIR := $(DESTDIR)/usr/share/man/man1
CONF := Release
#include $(PROJ_ROOT)/nbproject/Makefile-variables.mk
INSTALL := $(shell which install)
MKDIR := mkdir
RM := rm
HEADERS_DEST_DIR := $(DESTDIR)/usr/include
HERE := $(CURDIR)

# Debug configuration
CND_PLATFORM_Debug=GNU-Linux-x86
CND_ARTIFACT_DIR_Debug=dist/Debug/GNU-Linux-x86
# Release configuration
CND_PLATFORM_Release=GNU-Linux-x86
CND_ARTIFACT_DIR_Release=dist/Release/GNU-Linux-x86

all: build

build: build-extract build-compare

build-extract:
	"$(MAKE)" -C $(EXTRACT_PROJ_ROOT) CONF=$(CONF) build

build-compare:
	"$(MAKE)" -C $(COMPARE_PROJ_ROOT) CONF=$(CONF) build
	
clean-extract:	
	"$(MAKE)" -C $(EXTRACT_PROJ_ROOT) CONF=$(CONF) clean
	$(RM) -rf $(EXTRACT_PROJ_ROOT)/dist $(EXTRACT_PROJ_ROOT)/build

clean-compare:
	"$(MAKE)" -C $(COMPARE_PROJ_ROOT) CONF=$(CONF) clean
	$(RM) -rf $(COMPARE_PROJ_ROOT)/dist $(COMPARE_PROJ_ROOT)/build

clean: clean-extract clean-compare
	rm -fr $(STAGEDIR)

install-headers:
	$(MKDIR) -p $(HEADERS_DEST_DIR)/easyeye
	$(MKDIR) -p $(HEADERS_DEST_DIR)/easyeye/common
	$(MKDIR) -p $(HEADERS_DEST_DIR)/easyeye/segment
	$(MKDIR) -p $(HEADERS_DEST_DIR)/easyeye/encode
	$(MKDIR) -p $(HEADERS_DEST_DIR)/easyeye/match
	find easyeye/src/easyeye/common -type f -name "*.h" -print0 | \
		xargs -0 cp --target $(HEADERS_DEST_DIR)/easyeye/common 
	find easyeye/src/easyeye/segment -type f -name "*.h" -print0 | \
		xargs -0 cp --target $(HEADERS_DEST_DIR)/easyeye/segment 
	find easyeye/src/easyeye/encode -type f -name "*.h" -print0 | \
		xargs -0 cp --target $(HEADERS_DEST_DIR)/easyeye/encode 
	find easyeye/src/easyeye/match -type f -name "*.h" -print0 | \
		xargs -0 cp --target $(HEADERS_DEST_DIR)/easyeye/match
	
install-libs: 
	$(MKDIR) -p $(LIBDIR)
	$(INSTALL) -t $(LIBDIR) -m 0644 $(OPTIMASEK_PROJ_ROOT)/$(CND_ARTIFACT_DIR_Release)/liboptimasek.a
	$(INSTALL) -t $(LIBDIR) -m 0644 $(EASYEYE_PROJ_ROOT)/$(CND_ARTIFACT_DIR_Release)/libeasyeye.a

install-extract: build-extract
	$(MKDIR) -p $(BINDIR)
	$(INSTALL) -t $(BINDIR) $(EXTRACT_PROJ_ROOT)/dist/Release/GNU-Linux-x86/easyeye-extract

install-compare: build-compare
	$(MKDIR) -p $(BINDIR)
	$(INSTALL) -t $(BINDIR) $(COMPARE_PROJ_ROOT)/dist/Release/GNU-Linux-x86/easyeye-compare

install-man:
	$(MKDIR) -p $(MANDIR)
	help2man -o $(MANDIR)/easyeye-extract.1 -n 'iris image analysis and feature encoding' $(BINDIR)/easyeye-extract
	help2man -o $(MANDIR)/easyeye-compare.1 -n 'compare iris encodings' $(BINDIR)/easyeye-compare

install-bins: install-extract install-compare

install: all install-extract install-compare install-libs install-man install-headers

test-easyeye:
	$(MAKE) -C $(EASYEYE_PROJ_ROOT) CONF=$(CONF) test
	
test-extract:
	$(MAKE) -C $(EXTRACT_PROJ_ROOT) CONF=$(CONF) test

test-compare:
	$(MAKE) -C $(COMPARE_PROJ_ROOT) CONF=$(CONF) test
	
test: test-easyeye test-compare test-extract
