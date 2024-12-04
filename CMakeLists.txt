cmake_minimum_required(VERSION 3.29)
project(cpp_deferred)

option(ENABLE_TESTING "Build the tests" OFF)

set(CMAKE_CXX_STANDARD 20)

add_executable(example example.cpp)

if(ENABLE_TESTING)
    enable_testing()
    find_package(GTest REQUIRED)

    add_subdirectory(tests)
endif()
