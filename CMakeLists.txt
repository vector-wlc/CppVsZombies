# CMakeLists

cmake_minimum_required (VERSION 3.0)

add_definitions(-std=c++17)
 
set(CMAKE_CXX_COMPILER "g++")

include_directories(./pvz_h/include)

aux_source_directory(./pvz_h/source srcs)

add_library(pvz ${srcs}) 