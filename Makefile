########################################################################
# Compiler and external dependences
########################################################################
# export OMPI_CXX=clang++

CC        = mpicc
CXX       = mpicxx
# CXX       = clang++
ONNX_DIR   = external/onnxruntime
EIGEN_DIR  = /net/jabba/home0/am611608/source/eigen
########################################################################
# Compiling and linking options
########################################################################
COPTS     = -g -Wall
CINCLUDES = -I$(EIGEN_DIR) \
			#-I$(ONNX_DIR)/include

# CDEFS     = -DHAVE_CONFIG_H -DHYPRE_TIMING
CFLAGS    = $(COPTS) $(CINCLUDES) $(CDEFS)

CXXOPTS   = $(COPTS) -Wno-deprecated
CXXINCLUDES = $(CINCLUDES)
CXXDEFS   = $(CDEFS)

LIBS      =#-L$(ONNX_DIR)/lib -lonnxruntime -lm

# LFLAGS    = $(LIBS) -std=c++11 #For onnxruntime we need -std=c++11
LFLAGS    = $(LIBS) #But Eigen won't compile with std c++11 so I get rid of it


CXXFLAGS  = $(CXXOPTS) $(CXXINCLUDES) $(CXXDEFS) $(LFLAGS)

########################################################################
# List of programs to be compiled
########################################################################
ALLPROGS = main

all: $(ALLPROGS)

tvm : #TODO :
onnx : #TODO : change std library, add includes, etc..
default: all
########################################################################
# Clean up
########################################################################
clean:
	rm -f $(ALLPROGS)
distclean: clean
	rm -f $(ALLPROGS) $(ALLPROGS:=*~)
