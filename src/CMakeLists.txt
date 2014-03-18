# Copyright (c) 2014 Hartmut Kaiser
#
# Distributed under the Boost Software License, Version 1.0. (See accompanying
# file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

# Source files
add_hpx_library_sources(hpxpi
  GLOB_RECURSE GLOBS "${hpxpi_SOURCE_DIR}/src/*.cpp")

add_hpx_source_group(
  NAME hpxpi
  CLASS "Source Files"
  ROOT ${hpxpi_SOURCE_DIR}/src
  TARGETS ${hpxpi_SOURCES})

# Header files
add_hpx_library_headers(hpxpi
  GLOB_RECURSE GLOBS "${hpxpi_SOURCE_DIR}/hpxpi/*.hpp")
add_hpx_library_headers(hpxpi
  GLOB_RECURSE GLOBS "${hpxpi_SOURCE_DIR}/hpxpi/*.h" APPEND)

add_hpx_source_group(
  NAME hpxpi
  CLASS "Header Files"
  ROOT ${hpxpi_SOURCE_DIR}/hpxpi
  TARGETS ${hpxpi_HEADERS})

add_definitions("-DHPXPI_EXPORTS")
add_definitions("-DHPX_COMPONENT_NAME=hpxpi")
add_definitions("-DHPX_COMPONENT_STRIN=\"hpxpi\"")

# We intentionally check HPX_BUILD_TYPE, not the application build type, as
# the HPX libraries will be suffixed depending on their build type, not ours.
if("${HPX_BUILD_TYPE}" STREQUAL "Debug")
  hpx_debug("hpxpi" "hpxpi_SOURCES: ${hpxpi_SOURCES}")
  hpx_debug("hpxpi" "hpxpi_HEADERS: ${hpxpi_HEADERS}")
  add_hpx_library(hpxpi SHARED
    ${hpxpi_SOURCES} ${hpxpi_HEADERS}
    DEPENDENCIES hpx_init${HPX_DEBUG_POSTFIX}
    OUTPUT_SUFFIX lib/hpx
    ESSENTIAL)
  set_target_properties(hpxpi_lib PROPERTIES
    VERSION ${HPXPI_VERSION}
    SOVERSION ${HPXPI_SOVERSION}
    CLEAN_DIRECT_OUTPUT 1
    OUTPUT_NAME hpxpi${HPX_DEBUG_POSTFIX}
    FOLDER "Core")
else()
  add_hpx_library(hpxpi SHARED
    ${hpxpi_SOURCES} ${hpxpi_HEADERS} ${hpxpi_internal_HEADERS}
    DEPENDENCIES hpx_init
    OUTPUT_SUFFIX lib/hpx
    ESSENTIAL)
  set_target_properties(hpxpi_lib PROPERTIES
    VERSION ${HPXPI_VERSION}
    SOVERSION ${HPXPI_SOVERSION}
    CLEAN_DIRECT_OUTPUT 1
    OUTPUT_NAME hpxpi
    FOLDER "Core")
endif()

