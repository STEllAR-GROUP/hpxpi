# Copyright (c) 2013 Hartmut Kaiser
#
# Distributed under the Boost Software License, Version 1.0. (See accompanying 
# file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

################################################################################
# Top level CMakeList.txt for HPXPI
################################################################################
  
cmake_minimum_required(VERSION 2.8.4 FATAL_ERROR)

project(hpxpi CXX C)

# Project version.
set(HPXPI_MAJOR_VERSION 0)
set(HPXPI_MINOR_VERSION 1)
set(HPXPI_PATCH_LEVEL   0)
set(HPXPI_SOVERSION     ${HPXPI_MAJOR_VERSION})
set(HPXPI_VERSION
    "${HPXPI_MAJOR_VERSION}.${HPXPI_MINOR_VERSION}.${HPXPI_PATCH_LEVEL}")

set(CMAKE_MODULE_PATH
    ${HPX_ROOT}/share/cmake-${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION}/Modules)

if(MSVC)
  set_property(GLOBAL PROPERTY USE_FOLDERS ON)
endif()

find_package(HPX)

if(NOT MSVC)
  set(HPX_RPATH
      "${HPX_RPATH}:${CMAKE_INSTALL_PREFIX}/lib/hpx:${CMAKE_BINARY_DIR}/lib/hpx")
endif()

include_directories(${HPX_INCLUDE_DIR})
link_directories(${HPX_LIBRARY_DIR})

include_directories(${hpxpi_SOURCE_DIR})

if(NOT MSVC)
  hpx_include(CompilerFlags)

  if(NOT ${CMAKE_SYSTEM_NAME} MATCHES "FreeBSD")
    set(HPX_LIBRARIES ${HPX_LIBRARIES} dl)
  endif()

  # Necessary on Linux because of our current strategy wrt to hpx_init
  hpx_use_flag_if_available(-fPIC LANGUAGES C CXX)
  hpx_use_flag_if_available(-pie LANGUAGES C CXX)
endif()

###############################################################################
# Other configuration options
hpx_option(HPXPI_HAVE_NATIVE_INT128 BOOL
    "Enable int128/uint128 operations in AGAS (default: OFF)"
    OFF ADVANCED)
if(HPXPI_HAVE_NATIVE_INT128)
  add_config_define(HPXPI_HAVE_NATIVE_INT128 1)
endif()

###############################################################################
add_subdirectory(src)

###############################################################################
hpx_option(HPXPI_BUILD_TESTS BOOL "Build HPXPI tests (default: ON)" ON ADVANCED)
if(HPXPI_BUILD_TESTS)
  add_hpx_pseudo_target(tests)
  include_directories(tests)
  add_subdirectory(tests)
endif()

##############################################################################
#add_hpx_pseudo_target(examples)
#add_subdirectory(examples)

