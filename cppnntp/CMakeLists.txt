set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)
find_package(Boost 1.55 REQUIRED
    COMPONENTS date_time iostreams regex system)
find_package(OpenSSL)
find_package(ZLIB)
find_package(Threads)

add_library(cppnntp
    boostRegexExceptions.cpp
    nntp.cpp
    socket.cpp
    yencdecode.cpp
    boostRegexExceptions.hpp
    nntp.hpp
    responsecodes.hpp
    socket.hpp
    yencdecode.hpp
    )
target_include_directories(cppnntp
    PRIVATE ${Boost_INCLUDE_DIRS} ${OPENSSL_INCLUDE_DIR} ${ZLIB_INCLUDE_DIRS})
target_link_libraries(cppnntp 
    ${Boost_LIBRARIES} ${OPENSSL_LIBRARIES} ${ZLIB_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT})
