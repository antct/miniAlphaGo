cmake_minimum_required(VERSION 3.10)
project(MCTSReversi)

set(CMAKE_CXX_STANDARD 11)

link_libraries(Ws2_32)

add_executable(MCTSReversi main.cpp Player.cpp Player.h PlayerServer.cpp PlayerServer.h PoolManager.cpp PoolManager.h Search.cpp Search.h util.cpp util.h)