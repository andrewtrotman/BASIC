#
# CMAKELISTS.TXT
# --------------
#
project(BASIC)

#
#CMake verison 3.2 is required to set CMAKE_CXX_STANDARD to C++14.
#

cmake_minimum_required (VERSION 3.2)

# set(CMAKE_VERBOSE_MAKEFILE on)

set(CMAKE_BUILD_TYPE Debug)

message("Build Type:" ${CMAKE_BUILD_TYPE})

add_definitions("-Wall -D_GLIBCXX_USE_CXX11_ABI=1")
add_definitions("-march=native")

if(CMAKE_BUILD_TYPE MATCHES Debug)
	add_definitions(-DDEBUG)
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Threads)
add_subdirectory(source)

add_executable(BASIC source/BASIC.cpp source/BASIC_io.swift)
target_link_libraries(BASIC BASIClib ${CMAKE_THREAD_LIBS_INIT})
