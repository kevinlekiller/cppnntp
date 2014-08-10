cmake_minimum_required(VERSION 2.8.11)
project(cppnntp)

if(("${CMAKE_CXX_COMPILER_ID}" MATCHES "GNU") OR ("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang"))
    set(CMAKE_CXX_FLAGS "-std=c++11")
endif()

set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)
find_package(Boost 1.55 REQUIRED
    COMPONENTS date_time iostreams regex system)

add_subdirectory(cppnntp)

add_executable(example examples.cpp examples.hpp)
add_dependencies(example cppnntp)
target_include_directories(example ${Boost_INCLUDE_DIRS})
target_link_libraries(example cppnntp)
