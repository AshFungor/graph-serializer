cmake_minimum_required(VERSION 3.5)

project(common VERSION 0.0 LANGUAGES CXX)

set(SOURCES common.cpp action-queue.cpp reverted.cpp)
set(HEADERS common.hpp action-queue.hpp reverted.hpp)

add_library(common STATIC ${SOURCES} ${HEADERS})

target_include_directories(common PUBLIC ${SRC_ROOT})
