cmake_minimum_required(VERSION 3.19)

project(graph-serializer VERSION 0.0 LANGUAGES CXX)

# CXX
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_EXTENSIONS ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(GTest REQUIRED)

# Clangd indexing
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Project root might be useful
set(ROOT ${CMAKE_CURRENT_SOURCE_DIR})

# Parse contrib
add_subdirectory(contrib)

# Parse source
add_subdirectory(src)
