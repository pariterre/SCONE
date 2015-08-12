cmake_minimum_required(VERSION 2.8.11)
# 2.8.11 is required for how we use Qt5.

project(scone)

# To create a folder hierarchy within Visual Studio.
set_property(GLOBAL PROPERTY USE_FOLDERS ON)


# User options.
# -------------
set(SHARK_ROOT "" CACHE PATH "Installation directory of shark.")


# Find dependencies.
# ------------------
set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH}" "${CMAKE_SOURCE_DIR}/build/cmake")

# Link with boost's static libraries.
set(Boost_USE_STATIC_LIBS ON)
find_package(Boost REQUIRED COMPONENTS 
	filesystem system thread date_time chrono)


# This uses the FindOpenSim.cmake file in build/cmake.
find_package(OpenSim 3.3 REQUIRED)

# This uses the FindShark.cmake file in build/cmake.
find_package(Shark REQUIRED)

# CMake has the ability to find Qt; we don't need to provide additional files.
find_package(Qt5Widgets)
# http://doc.qt.io/qt-5/cmake-manual.html

# Incorporate dependencies.
# -------------------------
# Reports automatic linking info with Visual Studio.
add_definitions(${Boost_LIB_DIAGNOSTIC_DEFINITIONS})
include_directories(${Boost_INCLUDE_DIRS})
include_directories(${OPENSIMSIMBODY_INCLUDE_DIRS})
include_directories(${Shark_INCLUDE_DIRS})


# Compiler flags.
# ---------------
# Must compile with C++11 with gcc/clang.
if(${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU" OR
        ${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang")
    set(CMAKE_CXX_FLAGS "-std=c++11 -stdlib=libc++")
endif()


# Various settings
# ----------------
# Place build products (libraries, executables) in root
# binary (build) directory. Otherwise, they get scattered around
# the build directory and so the dll's aren't next to the executables.
set(EXECUTABLE_OUTPUT_PATH "${CMAKE_BINARY_DIR}")
set(LIBRARY_OUTPUT_PATH "${CMAKE_BINARY_DIR}")


# Process source code.
# --------------------
add_subdirectory(scone/core)
add_subdirectory(scone/sim)
add_subdirectory(scone/sim/simbody)
add_subdirectory(scone/opt)
add_subdirectory(scone/cs)
add_subdirectory(scone/vis)

add_subdirectory(scone/sconeopt)
add_subdirectory(scone/studio)
add_subdirectory(scone/test)