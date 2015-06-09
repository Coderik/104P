cmake_minimum_required(VERSION 3.2)
project(104Core)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

# By default build in Release
if (NOT CMAKE_BUILD_TYPE)
   message(STATUS "No build type selected, set by default to Release")
   set(CMAKE_BUILD_TYPE "Release")
endif()


# Specify target's our own include directories.
include_directories(include)

# Specify all the source files.
set(SOURCE_FILES
    include/i_iterable_mask.h
    include/i_watchdog.h
    include/image.h
    include/mask.h
    include/mask_iterator.h
    include/mask_sequence.h
    include/patch.h
    include/point.h
    include/sequence.h
    include/shape.h
    mask.cpp
    mask_iterator.cpp
    mask_sequence.cpp
    patch.cpp
    point.cpp
    sequence.hpp
    shape.cpp)

# Configure the target.
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/build)
add_library(${PROJECT_NAME} STATIC ${SOURCE_FILES})
target_include_directories(${PROJECT_NAME} INTERFACE ${PROJECT_SOURCE_DIR}/inlude)