cmake_minimum_required(VERSION 3.19)

project(common VERSION 0.0 LANGUAGES CXX)

set(SOURCES traversal.cpp)
set(HEADERS traversal.hpp)

add_library(algorithms STATIC ${SOURCES} ${HEADERS})

target_include_directories(algorithms PUBLIC ${SRC_ROOT})