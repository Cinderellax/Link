cmake_minimum_required(VERSION 2.8)
project(link)

set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_CXX_FLAGS "$ENV{CXXFLAGS} -m64 -rdynamic -O0 -ggdb -std=c++11 -Wall -Wno-deprecated -Werror -Wno-unused-function")

set(LIB_SRC
    link/base/CThread.cc
    link/base/Thread.cc
    link/net/IOEvent.cc
    link/net/IOMultiplex.cc
    link/net/IOEventCtrl.cc
    link/net/EventLoop.cc
    link/base/Thread.cc
    link/net/EventLoopThread.cc
    link/net/EventLoopThreadPool.cc
    link/net/SocketOperation.cc
    link/net/SocketAddr.cc
    link/net/Socket.cc
    link/net/Buffer.cc
    link/net/TcpAccept.cc
    link/net/TcpConnect.cc
    link/net/TcpServer.cc
    link/net/echo.cc
    )

add_library(link SHARED ${LIB_SRC})

add_executable(test test/test.cc)
add_dependencies(test link)
target_link_libraries(test link)
find_package(Threads)
target_link_libraries(test ${CMAKE_THREAD_LIBS_INIT})

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
SET(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)