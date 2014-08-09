cmake_minimum_required(VERSION 2.8.11)
project(cppnntp CXX)

set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)
find_package(Boost 1.55 REQUIRED COMPONENTS unit_test_framework date_time regex)

find_package(OpenSSL)

if(("${CMAKE_CXX_COMPILER_ID}" MATCHES "GNU") OR ("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang"))
    set(CMAKE_CXX_FLAGS "-std=c++11")
endif()

add_executable(example examples.cpp examples.hpp)
add_dependencies(example cppnntp)

add_subdirectory(cppnntp)
