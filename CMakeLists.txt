cmake_minimum_required(VERSION 2.8)
project(projet-backgammon)

set(CMAKE_C_FLAGS "${CMAKE_CXX_FLAGS} -std=c11")

set(SOURCE_FILES ia_lib.c pile.c )
include_directories("${PROJECT_BINARY_DIR}")
add_executable(dbsg ${SOURCE_FILES})
