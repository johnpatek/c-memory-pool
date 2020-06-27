cmake_minimum_required(VERSION 2.8)

project(c-memory-pool)

if(WIN32)
    # Windows specific compiler flags
else()
    set(CMAKE_CXX_FLAGS  "-std=c++14")
endif(WIN32)

file(GLOB MEMORY_POOL_SOURCES
    "include/*.h"
    "src/*.c")

add_library(
    memorypool 
    STATIC 
    ${MEMORY_POOL_SOURCES})

target_include_directories(
    memorypool 
    PUBLIC 
    include)