cmake_minimum_required(VERSION 3.5)

# Contributed library, link: https://github.com/digint/tinyfsm
# CMakeLists written by AshFungor.

project(tiny-fsm VERSION 0.3.3 LANGUAGES CXX)

# Library header-only
set(HEADERS tinyfsm.hpp)

# Dummy target
add_library(tiny-fsm INTERFACE ${HEADERS})

# Most important part
target_include_directories(tiny-fsm INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
