cmake_minimum_required(VERSION 3.16)
project(FLowNetwork)

set(CMAKE_CXX_STANDARD 20)

add_executable(FLowNetwork main.cpp FlowEdge.h FlowNetwork.h Bag.h UltraBag.cpp UltraBag.h Flow_Network.h FordFulkerson.h)
