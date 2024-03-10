cmake_minimum_required(VERSION 3.5)

project(graph-serializer VERSION 0.0 LANGUAGES CXX)

# Set sources and headers (headers optional - for some IDEs)
set(SOURCES main.cpp)
set(HEADERS )

set(SRC_ROOT ${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory(common)
add_subdirectory(parcer)
add_subdirectory(lexer)
add_subdirectory(tests)

add_executable(graph-serializer ${SOURCES} ${HEADERS})

target_link_libraries(graph-serializer tiny-fsm)
