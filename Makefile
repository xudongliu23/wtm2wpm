SHELL := /bin/bash

USER_DIR:=
ifeq ($(shell uname),Darwin) #if using a Mac
  ifeq ($(shell hostname -s),Xudongs-MacBook-Pro)                 #if using my MBP laptop
  export PATH := /usr/local/opt/coreutils/libexec/gnubin:$(PATH)  # have to have this for shuf
  USER_DIR:=/Users/xudong
  else ifeq ($(shell hostname -s),CIS-3211-64950)                 #if using my office desktop
  export PATH := /usr/local/opt/coreutils/libexec/gnubin:$(PATH)  # have to have this for shuf
  USER_DIR:=/Users/n01237497
  endif
else ifeq ($(shell uname),Linux) #if using Ubuntu
  ifeq ($(shell hostname -s),cisvm-xudong-1) #if using CIS virtual machine
  USER_DIR:=/home/xudong
  else ifeq ($(shell hostname -s),xudong-ThinkPad-T410)    #if using my Thinkpad
  USER_DIR:=/home/xudong
  else ifeq ($(shell hostname -s),kestrel)   #if using UKY CSR machine
  USER_DIR:=/homes/liu
  endif
endif

# Determine the platform for portability.
UNAME := $(shell uname)

INSTANCES_DIR = instances

ifneq (,$(findstring CYGWIN,$(UNAME)))
OUTPUT_DIR = out/gcccygwin64
else ifeq ($(UNAME),Darwin)
OUTPUT_DIR = out/gccdarwin64
else
OUTPUT_DIR = out/gcclinux64
endif

# keep a list of filenames for Doxygen
INCLUDE_DIR = .
SRC_DIR = ./src
#INCLUDE_FILENAMES = types.h helper.h permutation.h
#SRC_FILENAMES = helper.cpp permutation.cpp
# to get the full path names, prepend the include dir to each file name.
INCLUDE_FILES = $(wildcard $(INCLUDE_DIR)/*.cpp)
SRC_FILES = $(wildcard $(SRC_DIR)/*.cpp)
OBJ_FILES := $(patsubst $(SRC_DIR)/%.cpp, $(OUTPUT_DIR)/%.o, $(SRC_FILES))
LIB_DIR = ./lib
LIB_FILES = -lmystatlib
LIB_SRC_FILES = $(wildcard $(LIB_DIR)/*.cpp)
LIB_OBJ_FILES = $(patsubst $(LIB_DIR)/%.cpp, $(LIB_DIR)/%.o, $(LIB_SRC_FILES))
BUILD_LIB = ar rcs

# Conventions suggest an overall INCLUDE_FLAGS.
#INCLUDE_FLAGS += -I. -I$(INCLUDE_DIR) -I./lib -I/usr/include
INCLUDE_FLAGS := $(addprefix -I,$(INCLUDE_DIR))
LIB_FLAGS := $(addprefix -L,$(LIB_DIR))

#######################################################
# Here's the first place where gcc and the intel compiler diverge.
# if CPATH isn't defined, we're using gcc
ifeq ($(CPATH),)

# in the normal case, $(CC) is set to cc and we change it to gcc.
# If we're using the CLANG static analyzer, $(CC) comes in set
# to something else and we leave it alone.
ifeq ($(CC),cc)
CC    =gcc
CXX   =g++
endif

# use gcc as the linker
LD    =$(CC)
LINK_ARGS =-o
LINK_ARGS_D =-g -o
LINK_ARGS_P =-ggdb3 -pg -o

else

# we're using the Intel compiler

ifeq ($(CC),cc)
CC    =icc
CXX   =icpc
endif

# use icc as the linker
LD    =$(CC)
LINK_ARGS =-o
LINK_ARGS_D =-g -o
LINK_ARGS_P =-g -p -o

endif
#######################################################

LIBJANSSON  =-L/usr/local/lib -ljansson
# If we're on a mac then the include directories for boost are located
# in a different location.
ifeq "$(UNAME)" "Darwin"
INCLUDE_FLAGS += -I/opt/local/include -I/usr/share/include
LD_FLAGS += -L/opt/local/lib
LIBRT =
CFLAGS += -mno-avx
CFLAGS_D += -mno-avx
CFLAGS_P += -mno-avx
CXXFLAGS += -mno-avx
CXXFLAGS_D += -mno-avx
else
#INCLUDE_FLAGS += -I$(EPREFIX)/usr/include
LD_FLAGS += -L$(EPREFIX)/usr/lib
# if you use static libraries, valgrind generates millions (really!)
# of false positive error messages and skips over the real problems.
# The valgrind folks suggest static linking, not static libraries.
#LD_FLAGS += -static -static-libgcc
endif
LD_FLAGS += -pthread

# if we're on Cygwin, insert the flag so we use the proper include files
ifneq (,$(findstring CYGWIN,$(UNAME)))
CFLAGS += -DCYGWIN
CFLAGS_D += -DCYGWIN
CFLAGS_P += -DCYGWIN
endif
# Here's another place where gcc and the Intel compiler differ.
# Many of the gcc flags work for the Intel compiler, but not all.

ifeq ($(CPATH),)

# USE GCC FLAGS

# gcc flags to produce optimized code on Linux
CFLAGS += -DUNIX_ENV -D_LINUX_ -D__UseSSE__ $(INCLUDE_FLAGS) -m64 -msse4.2 -std=gnu99 -pthread -O3 -ggdb3 -fno-strict-aliasing -fwrapv -pedantic -Wall -W -Wstrict-prototypes -Wpointer-arith -Wwrite-strings -Wcast-qual -Wmissing-prototypes -fPIC
CXXFLAGS += -DUNIX_ENV -D_LINUX_ -D__UseSSE__ $(INCLUDE_FLAGS) -m64 -msse4.2 -pthread -O3 -fno-strict-aliasing -fwrapv -pedantic -Wall -W -Wpointer-arith -Wwrite-strings -Wcast-qual -std=c++0x -fPIC

# gcc flags to produce debuggable code
CFLAGS_D += -DDEBUG -DUNIX_ENV -D_LINUX_ -D__UseSSE__ $(INCLUDE_FLAGS) -m64 -std=gnu99  -pthread -fwrapv -ggdb3 -O0 -pedantic -Wall -W -W    strict-prototypes -Wpointer-arith -Wwrite-strings -Wcast-qual -Wmissing-prototypes
CXXFLAGS_D += -DDEBUG -DUNIX_ENV -D_LINUX_ -D__UseSSE__ $(INCLUDE_FLAGS) -m64 -pthread -fwrapv -ggdb3 -pedantic -Wall -W -Wpointer-arith     -Wwrite-strings -Wcast-qual -std=c++0x

# gcc flags to produce profiling code
CFLAGS_P += -ggdb3 -pg $(CFLAGS)
CXXFLAGS_P += -ggdb3 -pg $(CXXFLAGS)

else

# Use INTEL ICC FLAGS

CXXFLAGS += -DUNIX_ENV -D_LINUX_ -D__UseSSE__ $(INCLUDE_FLAGS) -m64 -xSSE4.2 -pthread -Ofast -no-ansi-alias -unroll -w3 -Wpointer-arith -    Wwrite-strings -Wcast-qual -std=c++0x

# icc flags to produce debuggable code
CFLAGS_D += -DDEBUG -DUNIX_ENV -D_LINUX_ -D__UseSSE__ $(INCLUDE_FLAGS) -m64 -std=gnu99 -pthread -O0 -gdwarf-3 -w3 -Wstrict-prototypes -Wp    ointer-arith -Wwrite-strings -Wmissing-prototypes -o
CXXFLAGS_D += -DDEBUG -DUNIX_ENV -D_LINUX_ -D__UseSSE__ $(INCLUDE_FLAGS) -m64 -pthread -gdwarf-3 -w3 -Wpointer-arith -Wwrite-strings -Wca    st-qual -std=c++0x

# gcc flags to produce profiling code
CFLAGS_P += -gdwarf-3 -p $(CFLAGS)
CXXFLAGS_P += -gdwarf-3 -pg $(CXXFLAGS)

endif

# DEBUG
# CFLAGS=$(CFLAGS_D)

CFLAGS += -Wfatal-errors $(LIBJANSSON)

wtmfile=$(shell pwd)/examples/instance.wtm
wpmfile=$(shell pwd)/examples/instance.wpm

all: make_dir produce
make_dir:
	mkdir -p $(OUTPUT_DIR)

produce: $(OBJ_FILES) $(LIB_OBJ_FILES)
		$(CXX) $(OBJ_FILES) $(LIB_OBJ_FILES) -o $(OUTPUT_DIR)/produce
		$(OUTPUT_DIR)/produce $(wtmfile) $(wpmfile)

$(LIB_DIR)/%.o: $(LIB_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $(INCLUDE_FLAGS) $< -o $@

$(OUTPUT_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $(INCLUDE_FLAGS) $< -o $@
