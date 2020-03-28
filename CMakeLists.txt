# CMakeLists

cmake_minimum_required (VERSION 3.0)

add_definitions(-std=c++17)
 
set(CMAKE_CXX_COMPILER "g++")

include_directories(./cvz/inc)

aux_source_directory(./cvz/src srcs)

add_library(pvz SHARED ${srcs}) 

SET(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/output)  