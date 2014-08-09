cmake_minimum_required(VERSION 2.8.11)
project(cppnntp CXX)

if(("${CMAKE_CXX_COMPILER_ID}" MATCHES "GNU") OR ("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang"))
    set(CMAKE_CXX_FLAGS "-std=c++11")
endif()

add_executable(example examples.cpp examples.hpp)
add_dependencies(example cppnntp)
target_link_libraries(example cppnntp)

add_subdirectory(cppnntp)
