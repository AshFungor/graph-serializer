cmake_minimum_required(VERSION 3.5)

project(graph-serializer-tests VERSION 0.0 LANGUAGES CXX)

set(SOURCES parcer-test.cpp symbols-test.cpp common-test.cpp)

add_executable(tests ${SOURCES})


target_link_libraries(tests PRIVATE GTest::gtest_main)
target_link_libraries(tests PRIVATE common)
target_link_libraries(tests PRIVATE parcer)
target_link_libraries(tests PRIVATE tiny-fsm)