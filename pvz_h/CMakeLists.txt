# CMakeLists

cmake_minimum_required (VERSION 3.0)

add_definitions(-std=c++11)
 
set(CMAKE_CXX_COMPILER "g++")

include_directories(./include)

aux_source_directory(./source srcs)

add_library(pvz  ${srcs})

#add_executable(test ${srcs})