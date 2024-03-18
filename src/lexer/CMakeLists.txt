cmake_minimum_required(VERSION 3.5)

project(lexemes-fsm VERSION 0.0 LANGUAGES CXX)

set(SOURCES lexer.cpp)
set(HEADERS lexer.hpp)



add_library(lexer ${SOURCES} ${HEADERS})

target_link_libraries(lexer PRIVATE tiny-fsm common)

message(STATUS ${SRC_ROOT})
target_include_directories(lexer PUBLIC ${SRC_ROOT})