########################################################################
# Compiler and external dependences
########################################################################
CC        = mpicc
CXX       = mpicxx

# LIB_DIR = bin_hypre/hypre-master/src/hypre

########################################################################
# Compiling and linking options
########################################################################
COPTS     = -g -Wall
# CINCLUDES = -I$(HYPRE_DIR)/include
# CDEFS     = -DHAVE_CONFIG_H -DHYPRE_TIMING
CFLAGS    = $(COPTS) $(CINCLUDES) $(CDEFS)

CXXOPTS   = $(COPTS) -Wno-deprecated
CXXINCLUDES = $(CINCLUDES) -I..
CXXDEFS   = $(CDEFS)

LINKOPTS  = $(COPTS)
# LIBS      = -L$(HYPRE_DIR)/lib -lHYPRE -lm
LIBS      = -lm
LFLAGS    = $(LINKOPTS) $(LIBS) -lstdc++

CXXFLAGS  = $(CXXOPTS) $(CXXINCLUDES) $(CXXDEFS) $(LFLAGS)

########################################################################
# Rules for compiling the source files
########################################################################
.SUFFIXES: .c .cxx

.cxx.o:
	$(CXX) $(CXXFLAGS) -c $<
.c.o:
	$(CC) $(CFLAGS) -c  $<

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
