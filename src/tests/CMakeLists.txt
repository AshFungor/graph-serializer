cmake_minimum_required(VERSION 3.19)

project(graph-serializer-tests VERSION 0.0 LANGUAGES CXX)

set(SOURCES parser-test.cpp symbols-test.cpp common-test.cpp traversal-test.cpp)

add_executable(tests ${SOURCES})


target_link_libraries(tests PRIVATE GTest::gtest_main)
target_link_libraries(tests PRIVATE common)
target_link_libraries(tests PRIVATE parser)
target_link_libraries(tests PRIVATE lexer)
target_link_libraries(tests PRIVATE tiny-fsm)
target_link_libraries(tests PRIVATE algorithms)