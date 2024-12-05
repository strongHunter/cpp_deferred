cmake_minimum_required(VERSION 3.29)
project(cpp_deferred)

option(BUILD_EXAMPLES "Build the examples" OFF)
option(ENABLE_TESTING "Build the tests" OFF)

set(CMAKE_CXX_STANDARD 20)

add_library(deferred INTERFACE)
add_library(deferred::deferred ALIAS deferred)

target_sources(deferred INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/deferred.h>)

if(BUILD_EXAMPLES)
    add_executable(example example.cpp)
endif()

if(ENABLE_TESTING)
    enable_testing()
    find_package(GTest REQUIRED)

    add_subdirectory(tests)
endif()
