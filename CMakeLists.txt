#------------------------------------------------------------------------------#
# SPDX-License-Identifier: MIT                                                 #
# Copyright (C) 2022 Clément Palézis                                           #
#------------------------------------------------------------------------------#
cmake_minimum_required(VERSION 3.20)

#------------------------------------------------------------------------------#
# Project.
#------------------------------------------------------------------------------#

project(simpleHPC LANGUAGES CXX C)

set(CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 17)

SET(CMAKE_CXX_FLAGS "-fopenmp")

#------------------------------------------------------------------------------#
# Compiler version requirements.
#------------------------------------------------------------------------------#

if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  if(CMAKE_CXX_COMPILER_VERSION VERSION_LESS "9.0")
    message(FATAL_ERROR "Version 9.0 of gnu compilers required!")
  endif()
endif()

# SET(CMAKE_CXX_COMPILER mpicxx)
find_package(MPI)
#------------------------------------------------------------------------------#
# Packages
#------------------------------------------------------------------------------#

# find_package(FleCSI REQUIRED)
add_subdirectory("external/eigen")
# find_package (Eigen3 3.3 REQUIRED NO_MODULE)
# set(Eigen3_DIR "/home/cea/am611608/source/eigen/cmake")

#----------------------------------------------------------------------------#
# Add options for backend selection
#----------------------------------------------------------------------------#

set(OpenGL_GL_PREFERENCE "LEGACY")
# set(FLECSI_BACKENDS legion mpi hpx)

# if(NOT FLECSI_BACKEND)
#   list(GET FLECSI_BACKENDS 0 FLECSI_BACKEND)
# endif()

# set(FLECSI_BACKEND "${FLECSI_BACKEND}" CACHE STRING
#   "Select the backend")
# set_property(CACHE FLECSI_BACKEND
#   PROPERTY STRINGS ${FLECSI_BACKENDS})

#----------------------------------------------------------------------------#
# Backend defaults.
#----------------------------------------------------------------------------#

# if(FLECSI_BACKEND STREQUAL "mpi")
#   set(ENABLE_MPI ON CACHE BOOL "Enable MPI" FORCE)
#   set(ENABLE_PARMETIS ON CACHE BOOL "Enable ParMETIS" FORCE)
#   set(ENABLE_LEGION OFF CACHE BOOL "Enable Legion" FORCE)
# elseif(FLECSI_BACKEND STREQUAL "legion")
#   set(ENABLE_MPI ON CACHE BOOL "Enable MPI" FORCE)
#   set(ENABLE_PARMETIS ON CACHE BOOL "Enable ParMETIS" FORCE)
#   set(ENABLE_LEGION ON CACHE BOOL "Enable Legion" FORCE)
# elseif(FLECSI_BACKEND STREQUAL "hpx")
#   set(ENABLE_MPI ON CACHE BOOL "Enable MPI" FORCE)
#   set(ENABLE_PARMETIS ON CACHE BOOL "Enable ParMETIS" FORCE)
#   set(ENABLE_HPX ON CACHE BOOL "Enable HPX" FORCE)
# endif()

# mark_as_advanced(ENABLE_MPI ENABLE_LEGION ENABLE_HPX)

#------------------------------------------------------------------------------#
# User options
#------------------------------------------------------------------------------#

#option template
option(HELLO_ENABLE_OPTION "Enable Option" ON)

#------------------------------------------------------------------------------#
# Setup Hello_FleCSI environment
#------------------------------------------------------------------------------#

list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

include(summary)
include(colors)
find_package(MPI REQUIRED)
set(tvm_DIR "/net/jabba/home0/am611608/source/tvm/build")
find_package(tvm REQUIRED)
# include(flog)

# Output configuration summary
# summary_header()


# summary_info("CMAKE_INSTALL_PREFIX" "${CMAKE_INSTALL_PREFIX}" TRUE)
# summary_info("CMAKE_PREFIX_PATH" "${CMAKE_PREFIX_PATH}" TRUE)

# string(APPEND _summary "\n")
# summary_info("CMAKE_CXX_COMPILER" "${CMAKE_CXX_COMPILER}" TRUE)
# summary_info("EIGEN3_VERSION" "${EIGEN3_VERSION}" TRUE)


# -Dtvm_DIR=/home/cea/am611608/source/tvm/include


# string(APPEND _summary "\n")
# summary_option("HELLO_ENABLE_OPTION" ${HELLO_ENABLE_OPTION} "")

# message(STATUS ${_summary})

#------------------------------------------------------------------------------#
# Main
#------------------------------------------------------------------------------#

add_executable(main
  main.cpp
#   hello_first/hello.cpp
#   hello_first/control.hpp
)
target_link_libraries (main
                        Eigen3::Eigen
                        MPI::MPI_CXX)

# add_subdirectory(prototype)