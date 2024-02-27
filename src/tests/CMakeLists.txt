cmake_minimum_required(VERSION 3.5)

project(graph-serializer-tests VERSION 0.0 LANGUAGES CXX)

set(SOURCES lexemes-test.cpp symbols-test.cpp)

add_executable(tests ${SOURCES})

target_link_libraries(tests PRIVATE GTest::gtest_main)