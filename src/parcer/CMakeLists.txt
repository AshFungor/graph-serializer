cmake_minimum_required(VERSION 3.5)

project(lexemes-fsm VERSION 0.0 LANGUAGES CXX)

set(SOURCES parcer.cpp)
set(HEADERS parcer.hpp)

add_library(parcer ${SOURCES} ${HEADERS})

target_link_libraries(parcer PRIVATE tiny-fsm common)

message(STATUS ${SRC_ROOT})
target_include_directories(parcer PUBLIC ${SRC_ROOT})