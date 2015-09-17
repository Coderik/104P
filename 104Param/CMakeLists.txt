cmake_minimum_required(VERSION 2.8)
project(104Param)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

# By default build in Release
if (NOT CMAKE_BUILD_TYPE)
   message(STATUS "No build type selected, set by default to Release")
   set(CMAKE_BUILD_TYPE "Release")
endif()

# Increase optimization level for RelWithDebInfo build type
if (CMAKE_C_FLAGS_RELWITHDEBINFO)
    string(REPLACE "-O2" "-O3" CMAKE_C_FLAGS_RELWITHDEBINFO "${CMAKE_C_FLAGS_RELWITHDEBINFO}")
    string(REPLACE "-O2" "-O3" CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO}")
endif()


# Look for include paths for dependencies
set(CMAKE_MODULE_PATH  ${PROJECT_SOURCE_DIR}/cmake-extensions/ )
find_package(PkgConfig)
pkg_check_modules(SIGC sigc++-2.0)

# Specify target's our own include directories.
include_directories(
    include
    ${SIGC_INCLUDE_DIRS})

# Specify all the source files.
set(SOURCE_FILES
    include/i_list_parameter.h
    include/i_parameter.h
    include/i_parameterizable.h
    include/i_range_parameter.h
    include/list_parameter.h
    include/range_parameter.h
    range_parameter.cpp)

# Specify all the targets, this target depends on.
set(DEPENDENCIES
    )   # NOTE: these are other project to be built, NOT all the libs this target depends on.

# Specify other libs this target depends on.
SET(LIBS ${DEPENDENCIES}
    )

# Configure the target.
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/build)
add_library(${PROJECT_NAME} STATIC ${SOURCE_FILES})
get_filename_component(ABSOLUTE_INCLUDE_PATH ${PROJECT_SOURCE_DIR}/include ABSOLUTE)
set(${PROJECT_NAME}_INCLUDE_DIRS ${ABSOLUTE_INCLUDE_PATH} CACHE INTERNAL "${PROJECT_NAME}: Include Directories" FORCE)