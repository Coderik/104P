cmake_minimum_required(VERSION 2.8)
project(104Core)

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
get_filename_component(ABSOLUTE_INCLUDE_PATH ${PROJECT_SOURCE_DIR}/include ABSOLUTE)
set(${PROJECT_NAME}_INCLUDE_DIRS ${ABSOLUTE_INCLUDE_PATH} CACHE INTERNAL "${PROJECT_NAME}: Include Directories" FORCE)