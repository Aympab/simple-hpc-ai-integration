########################################################################
# Compiler and external dependences
########################################################################
CC        =mpicc
CXX       =mpicxx
# CXX       = clang++
ONNX_DIR   =/net/jabba/home0/am611608/local/onnxruntime-linux-x64-1.11.0
# ONNX_DIR   =/net/jabba/home0/am611608/source/simple-hpc/external/onnxruntime
EIGEN_DIR  =/net/jabba/home0/am611608/source/eigen

TVM_DIR=/net/jabba/home0/am611608/source/tvm#/build
DMLC_CORE=${TVM_DIR}/3rdparty/dmlc-core

########################################################################
# Compiling and linking options
########################################################################
COPTS     = -g -Wall
CINCLUDES = -I$(EIGEN_DIR) \
			-I${TVM_DIR}/include \
			-I${DMLC_CORE}/include \
			-I${TVM_DIR}/3rdparty/dlpack/include \
			-fopenmp \
			-I$(ONNX_DIR)/include

CDEFS     = -DDMLC_USE_LOGGING_LIBRARY=\<tvm/runtime/logging.h\>
CFLAGS    = $(COPTS) $(CINCLUDES) $(CDEFS) 

CXXOPTS   = $(COPTS) -Wno-deprecated
CXXINCLUDES = $(CINCLUDES)
CXXDEFS   = $(CDEFS)

LIBS      =-L${TVM_DIR}/build \
			-ldl \
			-pthread \
			-L$(ONNX_DIR)/lib \
			-lonnxruntime \
			-lm
#-L/home/cea/am611608/source/tvm/3rdparty/dlpack	

# LFLAGS    = $(LIBS) -std=c++11 #For onnxruntime we need -std=c++11
LFLAGS    = $(LIBS) -std=c++14 -O2 -fPIC #But Eigen won't compile with std c++11 so I get rid of it


CXXFLAGS  = $(CXXOPTS) $(CXXINCLUDES) $(CXXDEFS) $(LFLAGS)

########################################################################
# List of programs to be compiled
########################################################################
ALLPROGS = main *.h

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
