cmake_minimum_required(VERSION 3.19)

project(lexemes-fsm VERSION 0.0 LANGUAGES CXX)

set(SOURCES parser.cpp)
set(HEADERS parser.hpp)

add_library(parser ${SOURCES} ${HEADERS})

target_link_libraries(parser PRIVATE tiny-fsm common)

target_include_directories(parser PUBLIC ${SRC_ROOT})