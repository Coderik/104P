cmake_minimum_required(VERSION 2.8)
project(104UI)

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

# Look for include paths for dependencies
set(CMAKE_MODULE_PATH  ${PROJECT_SOURCE_DIR}/cmake-extensions/ )
find_package(PkgConfig)
pkg_check_modules(GTKMM gtkmm-3.0)

# Specify dependencies' include directories.
include_directories(${GTKMM_INCLUDE_DIRS})

# Specify all the source files.
set(SOURCE_FILES
    icons/icon_point_selection_24.h
    icons/icon_rectangular_selection.h
    include/background_worker.h
    include/descriptor.h
    include/fitting.h
    include/flow_color_coding.h
    include/hull.h
    include/hull_proxy.h
    include/i_background_insider.h
    include/i_background_worker.h
    include/i_data.h
    include/i_hull.h
    include/i_mask_aware.h
    include/i_mask_provider.h
    include/i_modulable.h
    include/i_module.h
    include/i_module_manager.h
    include/i_optical_flow_aware.h
    include/i_optical_flow_provider.h
    include/i_proxy.h
    include/i_rig.h
    include/i_rig_manager.h
    include/image_viewer.h
    include/interaction.h
    include/interaction_manager.h
    include/layer.h
    include/layer_manager.h
    include/mask_group.h
    include/mask_module.h
    include/mouse_event.h
    include/numerical_entry.h
    include/optical_flow_data.h
    include/optical_flow_module.h
    include/painter.h
    include/parameters_ui_builder.h
    include/point_picker.h
    include/rectangular_selection.h
    include/rectangular_selection_layer.h
    include/request.h
    include/request_module.h
    include/rig.h
    include/selection_interaction.h
    include/signal_watchdog.h
    include/ui_container.h
    include/view.h
    include/view_info.h
    background_worker.cpp
    descriptor.cpp
    flow_color_coding.cpp
    hull.cpp
    hull_proxy.cpp
    image_viewer.cpp
    interaction_manager.cpp
    layer.cpp
    layer_manager.cpp
    layer_manager.hpp
    mask_group.cpp
    mask_module.cpp
    numerical_entry.cpp
    optical_flow_module.cpp
    painter.cpp
    parameters_ui_builder.cpp
    point_picker.cpp
    rectangular_selection.cpp
    rectangular_selection_layer.cpp
    selection_interaction.cpp
    signal_watchdog.cpp
    ui_container.cpp
    view.cpp)

# Specify all the targets, this target depends on.
set(DEPENDENCIES
    104Core
    104IO
    104Algo
    104Param)   # NOTE: these are other project to be built, NOT all the libs this target depends on.

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