########################################################################
# Compiler and external dependences
########################################################################
CC        = mpicc
CXX       = mpicxx

ONNX_DIR   = external/onnxruntime

########################################################################
# Compiling and linking options
########################################################################
COPTS     = -g -Wall
CINCLUDES = -I$(ONNX_DIR)/include
# CDEFS     = -DHAVE_CONFIG_H -DHYPRE_TIMING
CFLAGS    = $(COPTS) $(CINCLUDES) $(CDEFS)

CXXOPTS   = $(COPTS) -Wno-deprecated
CXXINCLUDES = $(CINCLUDES) #-I..
CXXDEFS   = $(CDEFS)

# LINKOPTS  = $(COPTS)
LIBS      = -L$(ONNX_DIR)/lib -lonnxruntime -lm
LFLAGS    = $(LIBS) -std=c++11 #For onnxruntime we need -std=c++11

CXXFLAGS  = $(CXXOPTS) $(CXXINCLUDES) $(CXXDEFS) $(LFLAGS)

########################################################################
# List of programs to be compiled
########################################################################
ALLPROGS = main

all: $(ALLPROGS)

default: all
########################################################################
# Clean up
########################################################################
clean:
	rm -f $(ALLPROGS)
distclean: clean
	rm -f $(ALLPROGS) $(ALLPROGS:=*~)
