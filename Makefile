####################################################################
# 
#       $File: Makefile $
# 
#       Copyright (c) 2009-2017 Nokia Networks. All rights reserved 
#
####################################################################
#
#       Makefile for TPL subsystem
# 
####################################################################
#   
#       $Id$
#   
####################################################################

default-target: build

include /opt/nokia/gbuild/common.mk
include /opt/nokia/gbuild/cpp-compilation.mk

NAME = TPL
TPL = TPL
LIBRARY = libtpl.so

CXXFLAGS = -DCOMPONENT_NAME='"TPL"' -std=c++11 -fpermissive -fPIC -shared -Wno-unused-parameter -lm

TPL_SUBSYSTEM_DIR = $(VOBTAG)/TPL
TPLTEST_DIR = $(VOBTAG)/TPL

TPI_DIR = $(VOBTAG)/TPL/

#TPL 
TPLSRC = $(shell find $(VOBTAG)/TPL/transportlib/src/ -name '*.cpp')
#TPLSRC = $(wildcard transportlib/src/icomgw/*.cpp)
LIBDIRS = $(dir $(TPLSRC))
INCFLAGS += $(foreach temp,$(LIBDIRS), -I$(temp))
INCFLAGS += -I$(TPI_DIR)

LDXXFLAGS += -lboost_system

#test_libs
TPLTEST_LIBS =  $(TPLTEST_DIR)
LIBSRCS = $(shell find $(TPLTEST_LIBS) -name '*.cpp')
TPLTEST_SRC += $(LIBSRCS)
LIBDIRS = $(dir $(LIBSRCS))
TESTINCFLAGS += $(foreach temp,$(LIBDIRS), -I$(temp))

        
#summary 
TPLOBJ = $(patsubst %.cpp, %.o, $(TPLSRC))
TPLDEP = $(patsubst %.cpp, %.d, $(TPLSRC))
CXXFLAGS += $(INCFLAGS)
LDXXFLAGS += -L$(TPLTEST_DIR)
#LDXXFLAGS += -Wl,-rpath $(COMPANY_BASE_DIR)/lib64/

# Unit tests
#TESTRUNNER = tst/testRunner
#LDXXFLAGS += -L$(TPLTEST_DIR)
TSTALLSRC = $(TPLSRC)
TSTALLSRC += $(wildcard tst/*.cpp)

TSTEXCL = src/main.cpp
TSTSRC = $(filter-out $(TSTEXCL), $(TSTALLSRC))

TSTOBJ = $(patsubst %.cpp, %.o, $(TSTSRC))
TSTDEP = $(patsubst %.cpp, %.d, $(TSTSRC))
TSTOTHER = tst/testreport.xml

build: $(TPL)

install: build
	mkdir -p $(INSTALL_DIR_LIBS)$(COMPANY_BASE_DIR)/lib64
	cp -f $(LIBRARY) $(INSTALL_DIR_LIBS)$(COMPANY_BASE_DIR)/lib64
    
# The variable $(TEST) allows to define a filter (--gtest_filter will be assigned this value),
# in order to run only certain tests.
test: 
	@echo "no ut case now"
	#./tst/run.sh $(TEST)

all: build test

$(TPL): $(TPLSRC)
	$(CXX) $(TPLSRC) $(CXXFLAGS) $(LDXXFLAGS) -o $(LIBRARY)
	
$(TESTRUNNER): $(TSTOBJ)
	$(CXX) $(LDXXFLAGS) -lgtest -lgmock -lprotobuf $(TSTOBJ) -o $@
	
clean::
	rm -f $(TPLDEP)
	rm -f $(TSTOBJ) $(TSTDEP) $(TSTOTHER) $(TESTRUNNER)	

-include $(TPLDEP) 

.PHONY: default-target build install test all



