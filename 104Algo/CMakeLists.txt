cmake_minimum_required(VERSION 2.8)
project(104Algo)

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
    include/distance_transform.h
    include/field_operations.h
    include/filtering.h
    include/gaussian_weights.h
    include/interpolation.h
    include/lut_math.h
    include/sampling.h
    include/warping.h
    include/zach_TVL1_optical_flow.h
    distance_transform.cpp
    field_operations.cpp
    filtering.cpp
    gaussian_weights.cpp
    interpolation.cpp
    lut_math.cpp
    sampling.cpp
    warping.cpp
    zach_TVL1_optical_flow.cpp)

# Specify all the targets, this target depends on.
set(DEPENDENCIES
    104Core)   # NOTE: these are other project to be built, NOT all the libs this target depends on.

# Specify other libs this target depends on.
SET(LIBS ${DEPENDENCIES}
    )


# Configure the target.
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/build)     # location for the target
add_library(${PROJECT_NAME} STATIC ${SOURCE_FILES})                 # static library
target_link_libraries(${PROJECT_NAME} ${LIBS})
get_filename_component(ABSOLUTE_INCLUDE_PATH ${PROJECT_SOURCE_DIR}/include ABSOLUTE)
set(${PROJECT_NAME}_INCLUDE_DIRS ${ABSOLUTE_INCLUDE_PATH} CACHE INTERNAL "${PROJECT_NAME}: Include Directories" FORCE)

# Add dependencies themself and include directories from the dependencies.
foreach(DEP ${DEPENDENCIES})
    add_dependencies(${PROJECT_NAME} ${DEP})
    include_directories(${${DEP}_INCLUDE_DIRS})
endforeach(DEP)