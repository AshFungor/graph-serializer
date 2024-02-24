cmake_minimum_required(VERSION 3.5)

project(common VERSION 0.0 LANGUAGES CXX)

set(SOURCES )
set(HEADERS common.hpp)

add_library(common STATIC ${SOURCES} ${HEADERS})
